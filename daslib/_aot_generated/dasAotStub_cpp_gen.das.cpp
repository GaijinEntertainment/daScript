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
 // require rtti
#include "daScript/simulate/aot_builtin_rtti.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_handle.h"
 // require ast
#include "daScript/ast/ast.h"
#include "daScript/simulate/aot_builtin_ast.h"
#include "daScript/ast/ast_generate.h"
 // require math
#include "daScript/simulate/aot_builtin_math.h"
 // require strings_boost
 // require ast_boost
 // require templates
 // require contracts
 // require array_boost
 // require algorithm
 // require templates_boost
 // require macro_boost
 // require defer
 // require json
 // require apply
 // require json_boost
 // require regex
 // require regex_boost
 // require cpp_gen

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
namespace _anon_4947418327762536717 {

namespace cpp_gen { struct ExEnum; };
namespace cpp_gen { struct Scope; };
namespace cpp_gen { struct ClangAstDecl; };
namespace cpp_gen { struct ClangTypedef; };
namespace cpp_gen { struct ClangEnum; };
namespace cpp_gen { struct ClangGlobalVar; };
namespace cpp_gen { struct ClangStructField; };
namespace cpp_gen { struct ClangFuncArg; };
namespace cpp_gen { struct ClangFunc; };
namespace cpp_gen { struct ClangStruct; };
namespace cpp_gen { struct ClangAst; };
namespace cpp_gen { struct _lambda_cpp_gen_266_1; };
namespace cpp_gen { struct _lambda_cpp_gen_543_2; };
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
namespace templates { struct DecltypeMacro; };
namespace templates { struct DecltypeNoRefMacro; };
namespace templates { struct TemplateMacro; };
namespace contracts { struct IsAnyType; };
namespace contracts { struct IsAnyArrayMacro; };
namespace contracts { struct IsAnyEnumMacro; };
namespace contracts { struct IsAnyBitfieldMacro; };
namespace contracts { struct IsAnyVectorType; };
namespace contracts { struct IsAnyStructMacro; };
namespace contracts { struct IsAnyNumericMacro; };
namespace contracts { struct IsAnyWorkhorse; };
namespace contracts { struct IsAnyWorkhorseNonPtrMacro; };
namespace contracts { struct IsAnyTupleNonPtrMacro; };
namespace contracts { struct IsAnyVariantNonPtrMacro; };
namespace contracts { struct IsAnyFunctionNonPtrMacro; };
namespace contracts { struct IsAnyLambdaMacro; };
namespace contracts { struct IsRefMacro; };
namespace contracts { struct IsPointer; };
namespace contracts { struct IsClass; };
namespace contracts { struct IsValueHandle; };
namespace templates_boost { struct Template; };
namespace templates_boost { struct TemplateVisitor; };
namespace templates_boost { struct RemoveDerefVisitor; };
namespace templates_boost { struct QRulesVisitor; };
namespace templates_boost { struct AstQCallMacro; };
namespace templates_boost { struct QMacro; };
namespace templates_boost { struct QBlockMacro; };
namespace templates_boost { struct QBlockToArrayMacro; };
namespace templates_boost { struct QBlockExprMacro; };
namespace templates_boost { struct QTypeMacro; };
namespace templates_boost { struct AstQNamedMacro; };
namespace templates_boost { struct AstQFunctionMacro; };
namespace templates_boost { struct AstQVariableMacro; };
namespace templates_boost { struct AstQNamedClassMacro; };
namespace templates_boost { struct AstQMethodMacro; };
namespace macro_boost { struct MacroVerifyMacro; };
namespace macro_boost { struct CaptureBlock; };
namespace macro_boost { struct CapturedVariable; };
namespace macro_boost { struct ColletFinally; };
namespace macro_boost { struct ColletLabels; };
namespace macro_boost { struct ReturnSkipLockcheck; };
namespace defer { struct DeferMacro; };
namespace defer { struct DeferDeleteMacro; };
namespace json { struct JsonValue; };
namespace json { struct TokenAt; };
namespace json { struct _lambda_json_99_1; };
namespace apply { struct ApplyMacro; };
namespace json_boost { struct BetterJsonMacro; };
namespace json_boost { struct JsonReader; };
namespace regex { struct ReNode; };
namespace regex { struct Regex; };
namespace regex_boost { struct RegexReader; };
// unused enumeration ConversionResult
// unused structure df_header
// unused enumeration CompilationError
// unused enumeration ConstMatters
// unused enumeration RefMatters
// unused enumeration TemporaryMatters
// unused enumeration Type
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
// unused structure DecltypeMacro
// unused structure DecltypeNoRefMacro
// unused structure TemplateMacro
// unused structure IsAnyType
// unused structure IsAnyArrayMacro
// unused structure IsAnyEnumMacro
// unused structure IsAnyBitfieldMacro
// unused structure IsAnyVectorType
// unused structure IsAnyStructMacro
// unused structure IsAnyNumericMacro
// unused structure IsAnyWorkhorse
// unused structure IsAnyWorkhorseNonPtrMacro
// unused structure IsAnyTupleNonPtrMacro
// unused structure IsAnyVariantNonPtrMacro
// unused structure IsAnyFunctionNonPtrMacro
// unused structure IsAnyLambdaMacro
// unused structure IsRefMacro
// unused structure IsPointer
// unused structure IsClass
// unused structure IsValueHandle
// unused structure Template
// unused structure TemplateVisitor
// unused structure RemoveDerefVisitor
// unused structure QRulesVisitor
// unused structure AstQCallMacro
// unused structure QMacro
// unused structure QBlockMacro
// unused structure QBlockToArrayMacro
// unused structure QBlockExprMacro
// unused structure QTypeMacro
// unused structure AstQNamedMacro
// unused structure AstQFunctionMacro
// unused structure AstQVariableMacro
// unused structure AstQNamedClassMacro
// unused structure AstQMethodMacro
// unused structure MacroVerifyMacro
// unused structure CaptureBlock
// unused structure CapturedVariable
// unused structure ColletFinally
// unused structure ColletLabels
// unused structure ReturnSkipLockcheck
// unused structure DeferMacro
// unused structure DeferDeleteMacro
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
// unused structure ApplyMacro
// unused structure BetterJsonMacro
// unused structure JsonReader
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
// unused structure RegexReader
namespace cpp_gen {

enum class ScopeKind : int32_t {
    Namespace = int32_t(0),
    Struct = int32_t(1),
};
}
namespace cpp_gen {

enum class AccessKind : int32_t {
    Private = int32_t(0),
    Public = int32_t(1),
    Protected = int32_t(2),
};
}
namespace cpp_gen {

struct ExEnum {
    char * name;
    bool removePrefix;
    bool isFlags;
};
}
namespace cpp_gen {

struct Scope {
    char * name;
    DAS_COMMENT(enum) cpp_gen::ScopeKind kind;
};
}
namespace cpp_gen {

struct ClangAstDecl {
    char * name;
    char * cppName;
};
}
namespace cpp_gen {

struct ClangTypedef {
    char * name;
    char * cppName;
    char * qtype;
};
}
namespace cpp_gen {

struct ClangEnum {
    char * name;
    char * cppName;
    bool eclass;
    char * etype;
    TArray<char *> edecl;
};
}
namespace cpp_gen {

struct ClangGlobalVar {
    char * name;
    char * cppName;
    char * mangledName;
    char * gtype;
};
}
namespace cpp_gen {

struct ClangStructField {
    char * name;
    char * cppName;
    char * qtype;
    DAS_COMMENT(enum) cpp_gen::AccessKind access;
    bool isBitfield;
};
}
namespace cpp_gen {

struct ClangFuncArg {
    char * name;
    char * cppName;
    char * atype;
    char * value;
    char * vtype;
};
}
namespace cpp_gen {

struct ClangFunc {
    char * name;
    char * cppName;
    char * mangledName;
    char * ftype;
    bool isMethod;
    bool isStatic;
    char * ofClass;
    TArray<cpp_gen::ClangFuncArg> args;
};
}
namespace cpp_gen {

struct ClangStruct {
    char * name;
    char * cppName;
    char * tag;
    TArray<cpp_gen::ClangStructField> fields;
    TArray<cpp_gen::ClangFunc> methods;
};
}
namespace cpp_gen {

struct ClangAst {
    TTable<char *,cpp_gen::ClangTypedef> typedefs;
    TTable<char *,cpp_gen::ClangEnum> enums;
    TTable<char *,cpp_gen::ClangStruct> structs;
    TTable<char *,cpp_gen::ClangFunc> funcs;
    TTable<char *,cpp_gen::ClangGlobalVar> globals;
};
}
namespace cpp_gen {

struct _lambda_cpp_gen_266_1 {
    Func DAS_COMMENT((bool,cpp_gen::_lambda_cpp_gen_266_1,char * * &)) __lambda;
    Func DAS_COMMENT((void,cpp_gen::_lambda_cpp_gen_266_1 *)) __finalize;
    int32_t __yield;
    bool _loop_at_266_34;
    cpp_gen::Scope * __n_rename_at_266_38;
    void * _pvar_0_at_266_34;
    Sequence DAS_COMMENT((cpp_gen::Scope &)) _source_0_at_266_34;
};
}
namespace cpp_gen {

struct _lambda_cpp_gen_543_2 {
    Func DAS_COMMENT((bool,cpp_gen::_lambda_cpp_gen_543_2,char * * &)) __lambda;
    Func DAS_COMMENT((void,cpp_gen::_lambda_cpp_gen_543_2 *)) __finalize;
    int32_t __yield;
    bool _loop_at_543_30;
    cpp_gen::Scope * __n_rename_at_543_34;
    void * _pvar_0_at_543_30;
    Sequence DAS_COMMENT((cpp_gen::Scope &)) _source_0_at_543_30;
};
}
extern StructInfo __struct_info__31646f6f189752b9;
extern StructInfo __struct_info__d92acaab53aeb957;
extern StructInfo __struct_info__2754d5ab960ed18d;
extern StructInfo __struct_info__f20bf1a74b2e5459;
extern StructInfo __struct_info__1d90238ba589ff21;
extern StructInfo __struct_info__11a4b507a1eef18c;
extern StructInfo __struct_info__f91080311cdb689c;
extern StructInfo __struct_info__65b849f269df3db4;
extern StructInfo __struct_info__e0ec7ef36db1f;
extern StructInfo __struct_info__8102cea4dbf746b1;
extern StructInfo __struct_info__69e2ef6fbeda51a0;
extern TypeInfo __type_info__774985c53b7f8953;
extern TypeInfo __type_info__773785c53b60f353;
extern TypeInfo __type_info__88ccb1e7aa67c692;
extern TypeInfo __type_info__994066f681df7bcd;
extern TypeInfo __type_info__c294005fecf8db6c;
extern TypeInfo __type_info__9d2fc6f14e8e098e;
extern TypeInfo __type_info__3fb2f51b6ebec3f2;
extern TypeInfo __type_info__a3a4f343e75637;
extern TypeInfo __type_info__af6b916ac10294a1;
extern TypeInfo __type_info__1e817199c43cb83d;
extern TypeInfo __type_info__46878b9b6d25c013;
extern TypeInfo __type_info__e796e1d4af74511c;
extern TypeInfo __type_info__12283e04d98e7c73;
extern TypeInfo __type_info__5eddda99b960ff80;
extern TypeInfo __type_info__4247e6b31bed7a2e;
extern TypeInfo __type_info__16d0aa3dd6b69257;
extern TypeInfo __type_info__fa593d0882a72913;
extern TypeInfo __type_info__af81fe4c86352052;
extern TypeInfo __type_info__af8afe4c86446b52;
extern TypeInfo __type_info__af90fe4c864e9d52;
extern TypeInfo __type_info__37d36026a6078a42;
extern TypeInfo __type_info__3ee86589dd970c38;
extern TypeInfo __type_info__39cb2cb435aca4fa;
extern TypeInfo __type_info__a4d78337018e0bdc;
extern TypeInfo __type_info__35d7933e81898305;
extern TypeInfo __type_info__d91f1e32fc332235;
extern TypeInfo __type_info__74b7d404689c2060;
extern TypeInfo __type_info__822f4971865b164a;
extern TypeInfo __type_info__790033b1854b444b;
extern TypeInfo __type_info__888650585a1a67a;
extern TypeInfo __type_info__740cc30e5071c426;
extern TypeInfo __type_info__4e24e452b3e2444a;
extern TypeInfo __type_info__4e466b022bf09776;
extern TypeInfo __type_info__eac86cf3f5a31edb;
extern TypeInfo __type_info__5eb1a2023943c414;
extern TypeInfo __type_info__3fa97cc3c3615476;
extern TypeInfo __type_info__a3a57ed1f9f48fdb;
extern TypeInfo __type_info__2f9ee4ce8425e314;
extern TypeInfo __type_info__da76256779e69e76;
extern TypeInfo __type_info__e0ce276400b1f9db;
extern TypeInfo __type_info__2a5b553d6263f514;
extern TypeInfo __type_info__af63df4c8601f1a5;
extern TypeInfo __type_info__af63d94c8601e773;
extern TypeInfo __type_info__af63e44c8601fa24;
extern TypeInfo __type_info__af63ee4c86020b22;
extern TypeInfo __type_info__af63eb4c86020609;
extern VarInfo __var_info__2657d41961c868d;
extern VarInfo __var_info__92abd3c6db489b19;
extern VarInfo __var_info__7d496dfb33c6a71d;
extern VarInfo __var_info__38b88d1cb4409489;
extern VarInfo __var_info__b22651dba88797d5;
extern VarInfo __var_info__523f3edb5698a11c;
extern VarInfo __var_info__329560101cd1ba1a;
extern VarInfo __var_info__6f2c5455373c29bd;
extern VarInfo __var_info__2e256124825f79eb;
extern VarInfo __var_info__82f9454766e25f92;
extern VarInfo __var_info__c12088bb5b50feb2;
extern VarInfo __var_info__4cd530580e8b3524;
extern VarInfo __var_info__2d96a2696ebec5e8;
extern VarInfo __var_info__27f2e20d35fc140;
extern VarInfo __var_info__bdf1843245b24f75;
extern VarInfo __var_info__fd38dcf5bcd09d05;
extern VarInfo __var_info__e1de223d7f5279ad;
extern VarInfo __var_info__9b10157790889d39;
extern VarInfo __var_info__3ca2022b02534541;
extern VarInfo __var_info__caaeddf333fc49b8;
extern VarInfo __var_info__42e98bc9c5ab35af;
extern VarInfo __var_info__b2ca50be0a2047ec;
extern VarInfo __var_info__69e72572b7faa99;
extern VarInfo __var_info__64106b70a7c13244;
extern VarInfo __var_info__ca65cae3214e32c9;
extern VarInfo __var_info__edf54a8969708638;
extern VarInfo __var_info__ce2b10dc022d6656;
extern VarInfo __var_info__721fddad9dfa7339;
extern VarInfo __var_info__555b034d630608ea;
extern VarInfo __var_info__cfc6acb3f5580944;
extern VarInfo __var_info__7085514dc178c1c;
extern VarInfo __var_info__a8db454182b4f91a;
extern VarInfo __var_info__90740e298b73954a;
extern VarInfo __var_info__8936a4cf58874156;
extern VarInfo __var_info__e2ea95e7e940c191;
extern VarInfo __var_info__95f978b6f82c0fcd;
extern VarInfo __var_info__75c4118e467d22a9;
extern VarInfo __var_info__9904be3b70772b34;
extern VarInfo __var_info__5aae1e8368815000;
extern VarInfo __var_info__15e3ae3019f9dabb;
extern VarInfo __var_info__c1950d81c11eb100;
extern VarInfo __var_info__292cabda15f17c5e;
extern VarInfo __var_info__1e73535401c31c77;
extern VarInfo __var_info__d0d57cb6579c143;
extern VarInfo __var_info__35de5b4ca693f294;
extern VarInfo __var_info__5103da4507460672;
extern VarInfo __var_info__9084600636a9a021;
extern VarInfo __var_info__9cd3372b83fe96fe;
extern VarInfo __var_info__ed967b95d54dc3c;
extern VarInfo __var_info__da1e84b2a7001714;
extern VarInfo __var_info__14200a801a676f09;
extern VarInfo __var_info__5dc56eab2dd5d5fc;
extern VarInfo __var_info__877388a2d657dbeb;
extern VarInfo __var_info__a069695a352e86a9;
extern VarInfo __var_info__553fee762d74789;
extern VarInfo __var_info__5db987cad91e921b;
extern VarInfo __var_info__75d769a8d4fcbb5;
extern VarInfo __var_info__86e817904c65827e;
extern VarInfo __var_info__e8ba625536483ff1;
extern VarInfo __var_info__b3268d2a2076b882;
extern VarInfo __var_info__a798e985ff4af814;
extern VarInfo __var_info__5e8bbc0dcd0f8f7b;
extern VarInfo __var_info__5cabdb29c1c60b5b;
extern VarInfo __var_info__c82d7284956a1d08;
extern VarInfo __var_info__9c4e3507596b38e2;
extern VarInfo __var_info__9f73c6183663c3f4;
extern VarInfo __var_info__81b1eeb109811f5b;
extern VarInfo __var_info__cf7d93746006fb15;
extern FuncInfo __func_info__b989707283fd0f9e;
extern FuncInfo __func_info__4b0de253e8ab06c8;
extern FuncInfo __func_info__7853938a1408c8d1;
extern FuncInfo __func_info__1dd7e9953a1c7073;
extern FuncInfo __func_info__3fa023a6055a72f3;
extern FuncInfo __func_info__ddb31f01d5771c19;
extern FuncInfo __func_info__94a6c10524a0a34c;
extern FuncInfo __func_info__252177d3d36ea5ea;
extern FuncInfo __func_info__8951eb20e251273c;
extern FuncInfo __func_info__3d0cb235470c67cc;
extern FuncInfo __func_info__486af025dcb791d2;
extern FuncInfo __func_info__62423d4f8a0d8cfa;
extern FuncInfo __func_info__b2194afbcdafdd23;
extern FuncInfo __func_info__7ec127c5491418ed;
extern FuncInfo __func_info__a833f7b3668de771;
extern FuncInfo __func_info__2fb829d28965e333;
extern FuncInfo __func_info__2cd99deec9605fd9;
extern FuncInfo __func_info__5f59ad38d8b49003;
extern EnumInfo __enum_info__8cff405a017d2eb5;
extern EnumInfo __enum_info__42710f72a6768679;

EnumValueInfo __enum_info__8cff405a017d2eb5_value_0 = { "Private", 0 };
EnumValueInfo __enum_info__8cff405a017d2eb5_value_1 = { "Public", 1 };
EnumValueInfo __enum_info__8cff405a017d2eb5_value_2 = { "Protected", 2 };
EnumValueInfo * __enum_info__8cff405a017d2eb5_values [] = { &__enum_info__8cff405a017d2eb5_value_0, &__enum_info__8cff405a017d2eb5_value_1, &__enum_info__8cff405a017d2eb5_value_2 };
EnumInfo __enum_info__8cff405a017d2eb5 = { "AccessKind", "cpp_gen", __enum_info__8cff405a017d2eb5_values, 3, UINT64_C(0x8cff405a017d2eb5) };
EnumValueInfo __enum_info__42710f72a6768679_value_0 = { "Namespace", 0 };
EnumValueInfo __enum_info__42710f72a6768679_value_1 = { "Struct", 1 };
EnumValueInfo * __enum_info__42710f72a6768679_values [] = { &__enum_info__42710f72a6768679_value_0, &__enum_info__42710f72a6768679_value_1 };
EnumInfo __enum_info__42710f72a6768679 = { "ScopeKind", "cpp_gen", __enum_info__42710f72a6768679_values, 2, UINT64_C(0x42710f72a6768679) };
VarInfo __struct_info__31646f6f189752b9_field_0 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x92abd3c6db489b19), "name", offsetof(cpp_gen::ClangAstDecl,name), 1 };
VarInfo __struct_info__31646f6f189752b9_field_1 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x2657d41961c868d), "cppName", offsetof(cpp_gen::ClangAstDecl,cppName), 2 };
VarInfo * __struct_info__31646f6f189752b9_fields[2] =  { &__struct_info__31646f6f189752b9_field_0, &__struct_info__31646f6f189752b9_field_1 };
StructInfo __struct_info__31646f6f189752b9 = {"ClangAstDecl", "cpp_gen", 8, __struct_info__31646f6f189752b9_fields, 2, 16, UINT64_C(0x0), nullptr, UINT64_C(0x31646f6f189752b9), 0 };
VarInfo __struct_info__d92acaab53aeb957_field_0 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x329560101cd1ba1a), "name", offsetof(cpp_gen::ClangEnum,name), 1 };
VarInfo __struct_info__d92acaab53aeb957_field_1 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x7d496dfb33c6a71d), "cppName", offsetof(cpp_gen::ClangEnum,cppName), 3 };
VarInfo __struct_info__d92acaab53aeb957_field_2 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0x38b88d1cb4409489), "eclass", offsetof(cpp_gen::ClangEnum,eclass), 0 };
VarInfo __struct_info__d92acaab53aeb957_field_3 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x523f3edb5698a11c), "etype", offsetof(cpp_gen::ClangEnum,etype), 4 };
VarInfo __struct_info__d92acaab53aeb957_field_4 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0xb22651dba88797d5), "edecl", offsetof(cpp_gen::ClangEnum,edecl), 5 };
VarInfo * __struct_info__d92acaab53aeb957_fields[5] =  { &__struct_info__d92acaab53aeb957_field_0, &__struct_info__d92acaab53aeb957_field_1, &__struct_info__d92acaab53aeb957_field_2, &__struct_info__d92acaab53aeb957_field_3, &__struct_info__d92acaab53aeb957_field_4 };
StructInfo __struct_info__d92acaab53aeb957 = {"ClangEnum", "cpp_gen", 28, __struct_info__d92acaab53aeb957_fields, 5, 56, UINT64_C(0x0), nullptr, UINT64_C(0xd92acaab53aeb957), 0 };
VarInfo __struct_info__2754d5ab960ed18d_field_0 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x27f2e20d35fc140), "name", offsetof(cpp_gen::ClangFunc,name), 1 };
VarInfo __struct_info__2754d5ab960ed18d_field_1 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x2e256124825f79eb), "cppName", offsetof(cpp_gen::ClangFunc,cppName), 2 };
VarInfo __struct_info__2754d5ab960ed18d_field_2 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x2d96a2696ebec5e8), "mangledName", offsetof(cpp_gen::ClangFunc,mangledName), 3 };
VarInfo __struct_info__2754d5ab960ed18d_field_3 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x82f9454766e25f92), "ftype", offsetof(cpp_gen::ClangFunc,ftype), 6 };
VarInfo __struct_info__2754d5ab960ed18d_field_4 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0xc12088bb5b50feb2), "isMethod", offsetof(cpp_gen::ClangFunc,isMethod), 0 };
VarInfo __struct_info__2754d5ab960ed18d_field_5 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0x4cd530580e8b3524), "isStatic", offsetof(cpp_gen::ClangFunc,isStatic), 0 };
VarInfo __struct_info__2754d5ab960ed18d_field_6 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xbdf1843245b24f75), "ofClass", offsetof(cpp_gen::ClangFunc,ofClass), 7 };
VarInfo __struct_info__2754d5ab960ed18d_field_7 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__a4d78337018e0bdc, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x6f2c5455373c29bd), "args", offsetof(cpp_gen::ClangFunc,args), 8 };
VarInfo * __struct_info__2754d5ab960ed18d_fields[8] =  { &__struct_info__2754d5ab960ed18d_field_0, &__struct_info__2754d5ab960ed18d_field_1, &__struct_info__2754d5ab960ed18d_field_2, &__struct_info__2754d5ab960ed18d_field_3, &__struct_info__2754d5ab960ed18d_field_4, &__struct_info__2754d5ab960ed18d_field_5, &__struct_info__2754d5ab960ed18d_field_6, &__struct_info__2754d5ab960ed18d_field_7 };
StructInfo __struct_info__2754d5ab960ed18d = {"ClangFunc", "cpp_gen", 28, __struct_info__2754d5ab960ed18d_fields, 8, 72, UINT64_C(0x0), nullptr, UINT64_C(0x2754d5ab960ed18d), 0 };
VarInfo __struct_info__f20bf1a74b2e5459_field_0 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x9b10157790889d39), "name", offsetof(cpp_gen::ClangFuncArg,name), 1 };
VarInfo __struct_info__f20bf1a74b2e5459_field_1 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xe1de223d7f5279ad), "cppName", offsetof(cpp_gen::ClangFuncArg,cppName), 2 };
VarInfo __struct_info__f20bf1a74b2e5459_field_2 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xfd38dcf5bcd09d05), "atype", offsetof(cpp_gen::ClangFuncArg,atype), 3 };
VarInfo __struct_info__f20bf1a74b2e5459_field_3 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x3ca2022b02534541), "value", offsetof(cpp_gen::ClangFuncArg,value), 4 };
VarInfo __struct_info__f20bf1a74b2e5459_field_4 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xcaaeddf333fc49b8), "vtype", offsetof(cpp_gen::ClangFuncArg,vtype), 5 };
VarInfo * __struct_info__f20bf1a74b2e5459_fields[5] =  { &__struct_info__f20bf1a74b2e5459_field_0, &__struct_info__f20bf1a74b2e5459_field_1, &__struct_info__f20bf1a74b2e5459_field_2, &__struct_info__f20bf1a74b2e5459_field_3, &__struct_info__f20bf1a74b2e5459_field_4 };
StructInfo __struct_info__f20bf1a74b2e5459 = {"ClangFuncArg", "cpp_gen", 8, __struct_info__f20bf1a74b2e5459_fields, 5, 40, UINT64_C(0x0), nullptr, UINT64_C(0xf20bf1a74b2e5459), 0 };
VarInfo __struct_info__1d90238ba589ff21_field_0 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x64106b70a7c13244), "name", offsetof(cpp_gen::ClangStruct,name), 1 };
VarInfo __struct_info__1d90238ba589ff21_field_1 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x42e98bc9c5ab35af), "cppName", offsetof(cpp_gen::ClangStruct,cppName), 2 };
VarInfo __struct_info__1d90238ba589ff21_field_2 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xca65cae3214e32c9), "tag", offsetof(cpp_gen::ClangStruct,tag), 3 };
VarInfo __struct_info__1d90238ba589ff21_field_3 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__35d7933e81898305, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0xb2ca50be0a2047ec), "fields", offsetof(cpp_gen::ClangStruct,fields), 4 };
VarInfo __struct_info__1d90238ba589ff21_field_4 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__39cb2cb435aca4fa, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x69e72572b7faa99), "methods", offsetof(cpp_gen::ClangStruct,methods), 5 };
VarInfo * __struct_info__1d90238ba589ff21_fields[5] =  { &__struct_info__1d90238ba589ff21_field_0, &__struct_info__1d90238ba589ff21_field_1, &__struct_info__1d90238ba589ff21_field_2, &__struct_info__1d90238ba589ff21_field_3, &__struct_info__1d90238ba589ff21_field_4 };
StructInfo __struct_info__1d90238ba589ff21 = {"ClangStruct", "cpp_gen", 28, __struct_info__1d90238ba589ff21_fields, 5, 72, UINT64_C(0x0), nullptr, UINT64_C(0x1d90238ba589ff21), 0 };
VarInfo __struct_info__11a4b507a1eef18c_field_0 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x555b034d630608ea), "name", offsetof(cpp_gen::ClangStructField,name), 1 };
VarInfo __struct_info__11a4b507a1eef18c_field_1 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xce2b10dc022d6656), "cppName", offsetof(cpp_gen::ClangStructField,cppName), 2 };
VarInfo __struct_info__11a4b507a1eef18c_field_2 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xcfc6acb3f5580944), "qtype", offsetof(cpp_gen::ClangStructField,qtype), 5 };
VarInfo __struct_info__11a4b507a1eef18c_field_3 =  { Type::tEnumeration, nullptr, &__enum_info__8cff405a017d2eb5, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xedf54a8969708638), "access", offsetof(cpp_gen::ClangStructField,access), 0 };
VarInfo __struct_info__11a4b507a1eef18c_field_4 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0x721fddad9dfa7339), "isBitfield", offsetof(cpp_gen::ClangStructField,isBitfield), 0 };
VarInfo * __struct_info__11a4b507a1eef18c_fields[5] =  { &__struct_info__11a4b507a1eef18c_field_0, &__struct_info__11a4b507a1eef18c_field_1, &__struct_info__11a4b507a1eef18c_field_2, &__struct_info__11a4b507a1eef18c_field_3, &__struct_info__11a4b507a1eef18c_field_4 };
StructInfo __struct_info__11a4b507a1eef18c = {"ClangStructField", "cpp_gen", 8, __struct_info__11a4b507a1eef18c_fields, 5, 32, UINT64_C(0x0), nullptr, UINT64_C(0x11a4b507a1eef18c), 0 };
VarInfo __struct_info__f91080311cdb689c_field_0 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xa8db454182b4f91a), "name", offsetof(cpp_gen::ExEnum,name), 3 };
VarInfo __struct_info__f91080311cdb689c_field_1 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0x90740e298b73954a), "removePrefix", offsetof(cpp_gen::ExEnum,removePrefix), 0 };
VarInfo __struct_info__f91080311cdb689c_field_2 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0x7085514dc178c1c), "isFlags", offsetof(cpp_gen::ExEnum,isFlags), 0 };
VarInfo * __struct_info__f91080311cdb689c_fields[3] =  { &__struct_info__f91080311cdb689c_field_0, &__struct_info__f91080311cdb689c_field_1, &__struct_info__f91080311cdb689c_field_2 };
StructInfo __struct_info__f91080311cdb689c = {"ExEnum", "cpp_gen", 8, __struct_info__f91080311cdb689c_fields, 3, 16, UINT64_C(0x0), nullptr, UINT64_C(0xf91080311cdb689c), 0 };
VarInfo __struct_info__65b849f269df3db4_field_0 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xe2ea95e7e940c191), "name", offsetof(cpp_gen::Scope,name), 2 };
VarInfo __struct_info__65b849f269df3db4_field_1 =  { Type::tEnumeration, nullptr, &__enum_info__42710f72a6768679, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x8936a4cf58874156), "kind", offsetof(cpp_gen::Scope,kind), 0 };
VarInfo * __struct_info__65b849f269df3db4_fields[2] =  { &__struct_info__65b849f269df3db4_field_0, &__struct_info__65b849f269df3db4_field_1 };
StructInfo __struct_info__65b849f269df3db4 = {"Scope", "cpp_gen", 8, __struct_info__65b849f269df3db4_fields, 2, 16, UINT64_C(0x0), nullptr, UINT64_C(0x65b849f269df3db4), 0 };
TypeInfo * __type_info__75c4118e467d22a9_arg_types_var_3956901548579615[2] = { &__type_info__74b7d404689c2060, &__type_info__774985c53b7f8953 };
const char * __type_info__75c4118e467d22a9_arg_names_var_3956901548579615[2] = { "__this", "_ryield266" };
VarInfo __struct_info__e0ec7ef36db1f_field_0 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af81fe4c86352052, nullptr, (TypeInfo **)__type_info__75c4118e467d22a9_arg_types_var_3956901548579615, __type_info__75c4118e467d22a9_arg_names_var_3956901548579615, 2, 0, nullptr, 12, 8, UINT64_C(0x75c4118e467d22a9), "__lambda", offsetof(cpp_gen::_lambda_cpp_gen_266_1,__lambda), 0 };
TypeInfo * __type_info__95f978b6f82c0fcd_arg_types_var_3956901548579615[1] = { &__type_info__c294005fecf8db6c };
const char * __type_info__95f978b6f82c0fcd_arg_names_var_3956901548579615[1] = { "__this" };
VarInfo __struct_info__e0ec7ef36db1f_field_1 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__95f978b6f82c0fcd_arg_types_var_3956901548579615, __type_info__95f978b6f82c0fcd_arg_names_var_3956901548579615, 1, 0, nullptr, 12, 8, UINT64_C(0x95f978b6f82c0fcd), "__finalize", offsetof(cpp_gen::_lambda_cpp_gen_266_1,__finalize), 0 };
VarInfo __struct_info__e0ec7ef36db1f_field_2 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x5aae1e8368815000), "__yield", offsetof(cpp_gen::_lambda_cpp_gen_266_1,__yield), 0 };
VarInfo __struct_info__e0ec7ef36db1f_field_3 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0x15e3ae3019f9dabb), "_loop_at_266_34", offsetof(cpp_gen::_lambda_cpp_gen_266_1,_loop_at_266_34), 0 };
VarInfo __struct_info__e0ec7ef36db1f_field_4 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__d91f1e32fc332235, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0x9904be3b70772b34), "__n_rename_at_266_38", offsetof(cpp_gen::_lambda_cpp_gen_266_1,__n_rename_at_266_38), 5 };
VarInfo __struct_info__e0ec7ef36db1f_field_5 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0xc1950d81c11eb100), "_pvar_0_at_266_34", offsetof(cpp_gen::_lambda_cpp_gen_266_1,_pvar_0_at_266_34), 6 };
VarInfo __struct_info__e0ec7ef36db1f_field_6 =  { Type::tIterator, nullptr, nullptr, nullptr, &__type_info__88ccb1e7aa67c692, nullptr, nullptr, nullptr, 0, 0, nullptr, 24602, 8, UINT64_C(0x292cabda15f17c5e), "_source_0_at_266_34", offsetof(cpp_gen::_lambda_cpp_gen_266_1,_source_0_at_266_34), 7 };
VarInfo * __struct_info__e0ec7ef36db1f_fields[7] =  { &__struct_info__e0ec7ef36db1f_field_0, &__struct_info__e0ec7ef36db1f_field_1, &__struct_info__e0ec7ef36db1f_field_2, &__struct_info__e0ec7ef36db1f_field_3, &__struct_info__e0ec7ef36db1f_field_4, &__struct_info__e0ec7ef36db1f_field_5, &__struct_info__e0ec7ef36db1f_field_6 };
StructInfo __struct_info__e0ec7ef36db1f = {"_lambda_cpp_gen_266_1", "cpp_gen", 14, __struct_info__e0ec7ef36db1f_fields, 7, 48, UINT64_C(0x0), nullptr, UINT64_C(0xe0ec7ef36db1f), 4 };
TypeInfo * __type_info__d0d57cb6579c143_arg_types_var_9296219788306499249[2] = { &__type_info__822f4971865b164a, &__type_info__774985c53b7f8953 };
const char * __type_info__d0d57cb6579c143_arg_names_var_9296219788306499249[2] = { "__this", "_ryield543" };
VarInfo __struct_info__8102cea4dbf746b1_field_0 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af81fe4c86352052, nullptr, (TypeInfo **)__type_info__d0d57cb6579c143_arg_types_var_9296219788306499249, __type_info__d0d57cb6579c143_arg_names_var_9296219788306499249, 2, 0, nullptr, 12, 8, UINT64_C(0xd0d57cb6579c143), "__lambda", offsetof(cpp_gen::_lambda_cpp_gen_543_2,__lambda), 0 };
TypeInfo * __type_info__1e73535401c31c77_arg_types_var_9296219788306499249[1] = { &__type_info__9d2fc6f14e8e098e };
const char * __type_info__1e73535401c31c77_arg_names_var_9296219788306499249[1] = { "__this" };
VarInfo __struct_info__8102cea4dbf746b1_field_1 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__1e73535401c31c77_arg_types_var_9296219788306499249, __type_info__1e73535401c31c77_arg_names_var_9296219788306499249, 1, 0, nullptr, 12, 8, UINT64_C(0x1e73535401c31c77), "__finalize", offsetof(cpp_gen::_lambda_cpp_gen_543_2,__finalize), 0 };
VarInfo __struct_info__8102cea4dbf746b1_field_2 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x5103da4507460672), "__yield", offsetof(cpp_gen::_lambda_cpp_gen_543_2,__yield), 0 };
VarInfo __struct_info__8102cea4dbf746b1_field_3 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0x9084600636a9a021), "_loop_at_543_30", offsetof(cpp_gen::_lambda_cpp_gen_543_2,_loop_at_543_30), 0 };
VarInfo __struct_info__8102cea4dbf746b1_field_4 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__d91f1e32fc332235, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0x35de5b4ca693f294), "__n_rename_at_543_34", offsetof(cpp_gen::_lambda_cpp_gen_543_2,__n_rename_at_543_34), 5 };
VarInfo __struct_info__8102cea4dbf746b1_field_5 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0x9cd3372b83fe96fe), "_pvar_0_at_543_30", offsetof(cpp_gen::_lambda_cpp_gen_543_2,_pvar_0_at_543_30), 6 };
VarInfo __struct_info__8102cea4dbf746b1_field_6 =  { Type::tIterator, nullptr, nullptr, nullptr, &__type_info__88ccb1e7aa67c692, nullptr, nullptr, nullptr, 0, 0, nullptr, 24602, 8, UINT64_C(0xed967b95d54dc3c), "_source_0_at_543_30", offsetof(cpp_gen::_lambda_cpp_gen_543_2,_source_0_at_543_30), 7 };
VarInfo * __struct_info__8102cea4dbf746b1_fields[7] =  { &__struct_info__8102cea4dbf746b1_field_0, &__struct_info__8102cea4dbf746b1_field_1, &__struct_info__8102cea4dbf746b1_field_2, &__struct_info__8102cea4dbf746b1_field_3, &__struct_info__8102cea4dbf746b1_field_4, &__struct_info__8102cea4dbf746b1_field_5, &__struct_info__8102cea4dbf746b1_field_6 };
StructInfo __struct_info__8102cea4dbf746b1 = {"_lambda_cpp_gen_543_2", "cpp_gen", 14, __struct_info__8102cea4dbf746b1_fields, 7, 48, UINT64_C(0x0), nullptr, UINT64_C(0x8102cea4dbf746b1), 4 };
TypeInfo * __type_info__a798e985ff4af814_arg_types_var_7629923981941428640[6] = { &__type_info__e796e1d4af74511c, &__type_info__994066f681df7bcd, &__type_info__af63ee4c86020b22, &__type_info__af63d94c8601e773, &__type_info__af63df4c8601f1a5, &__type_info__12283e04d98e7c73 };
const char * __type_info__a798e985ff4af814_arg_names_var_7629923981941428640[6] = { "_object", "_array", "_string", "_number", "_bool", "_null" };
VarInfo __struct_info__69e2ef6fbeda51a0_field_0 =  { Type::tVariant, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__a798e985ff4af814_arg_types_var_7629923981941428640, __type_info__a798e985ff4af814_arg_names_var_7629923981941428640, 6, 0, nullptr, 57346, 56, UINT64_C(0xa798e985ff4af814), "value", offsetof(json::JsonValue,value), 1 };
VarInfo * __struct_info__69e2ef6fbeda51a0_fields[1] =  { &__struct_info__69e2ef6fbeda51a0_field_0 };
StructInfo __struct_info__69e2ef6fbeda51a0 = {"JsonValue", "json", 28, __struct_info__69e2ef6fbeda51a0_fields, 1, 56, UINT64_C(0x0), nullptr, UINT64_C(0x69e2ef6fbeda51a0), 0 };
VarInfo __func_info__b989707283fd0f9e_field_0 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16484, 8, UINT64_C(0xda1e84b2a7001714), "data", 0, 0 };
VarInfo * __func_info__b989707283fd0f9e_fields[1] =  { &__func_info__b989707283fd0f9e_field_0 };
FuncInfo __func_info__b989707283fd0f9e = {"invoke block<(data:string const#):void> const", "", __func_info__b989707283fd0f9e_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xb989707283fd0f9e), 0x0 };
VarInfo __func_info__4b0de253e8ab06c8_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__16d0aa3dd6b69257, nullptr, nullptr, nullptr, 0, 0, nullptr, 8236, 8, UINT64_C(0x14200a801a676f09), "df", 0, 0 };
VarInfo * __func_info__4b0de253e8ab06c8_fields[1] =  { &__func_info__4b0de253e8ab06c8_field_0 };
FuncInfo __func_info__4b0de253e8ab06c8 = {"invoke block<(df:fio::FILE const? const):void> const", "", __func_info__4b0de253e8ab06c8_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x4b0de253e8ab06c8), 0x0 };
VarInfo __func_info__7853938a1408c8d1_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__16d0aa3dd6b69257, nullptr, nullptr, nullptr, 0, 0, nullptr, 8236, 8, UINT64_C(0x5dc56eab2dd5d5fc), "df_ann", 0, 0 };
VarInfo * __func_info__7853938a1408c8d1_fields[1] =  { &__func_info__7853938a1408c8d1_field_0 };
FuncInfo __func_info__7853938a1408c8d1 = {"invoke block<(df_ann:fio::FILE const? const):void> const", "", __func_info__7853938a1408c8d1_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x7853938a1408c8d1), 0x0 };
VarInfo __func_info__1dd7e9953a1c7073_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__16d0aa3dd6b69257, nullptr, nullptr, nullptr, 0, 0, nullptr, 8236, 8, UINT64_C(0x877388a2d657dbeb), "df_enum", 0, 0 };
VarInfo * __func_info__1dd7e9953a1c7073_fields[1] =  { &__func_info__1dd7e9953a1c7073_field_0 };
FuncInfo __func_info__1dd7e9953a1c7073 = {"invoke block<(df_enum:fio::FILE const? const):void> const", "", __func_info__1dd7e9953a1c7073_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x1dd7e9953a1c7073), 0x0 };
VarInfo __func_info__3fa023a6055a72f3_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__16d0aa3dd6b69257, nullptr, nullptr, nullptr, 0, 0, nullptr, 8236, 8, UINT64_C(0xa069695a352e86a9), "df_method", 0, 0 };
VarInfo * __func_info__3fa023a6055a72f3_fields[1] =  { &__func_info__3fa023a6055a72f3_field_0 };
FuncInfo __func_info__3fa023a6055a72f3 = {"invoke block<(df_method:fio::FILE const? const):void> const", "", __func_info__3fa023a6055a72f3_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x3fa023a6055a72f3), 0x0 };
VarInfo __func_info__ddb31f01d5771c19_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__16d0aa3dd6b69257, nullptr, nullptr, nullptr, 0, 0, nullptr, 8236, 8, UINT64_C(0x5db987cad91e921b), "f", 0, 0 };
VarInfo * __func_info__ddb31f01d5771c19_fields[1] =  { &__func_info__ddb31f01d5771c19_field_0 };
FuncInfo __func_info__ddb31f01d5771c19 = {"invoke block<(f:fio::FILE const? const):void> const", "", __func_info__ddb31f01d5771c19_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xddb31f01d5771c19), 0x0 };
VarInfo __func_info__94a6c10524a0a34c_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__16d0aa3dd6b69257, nullptr, nullptr, nullptr, 0, 0, nullptr, 8236, 8, UINT64_C(0x75d769a8d4fcbb5), "hf", 0, 0 };
VarInfo * __func_info__94a6c10524a0a34c_fields[1] =  { &__func_info__94a6c10524a0a34c_field_0 };
FuncInfo __func_info__94a6c10524a0a34c = {"invoke block<(hf:fio::FILE const? const):void> const", "", __func_info__94a6c10524a0a34c_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x94a6c10524a0a34c), 0x0 };
VarInfo __func_info__252177d3d36ea5ea_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__16d0aa3dd6b69257, nullptr, nullptr, nullptr, 0, 0, nullptr, 8236, 8, UINT64_C(0x5cabdb29c1c60b5b), "mf_ann", 0, 0 };
VarInfo * __func_info__252177d3d36ea5ea_fields[1] =  { &__func_info__252177d3d36ea5ea_field_0 };
FuncInfo __func_info__252177d3d36ea5ea = {"invoke block<(mf_ann:fio::FILE const? const):void> const", "", __func_info__252177d3d36ea5ea_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x252177d3d36ea5ea), 0x0 };
VarInfo __func_info__8951eb20e251273c_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__16d0aa3dd6b69257, nullptr, nullptr, nullptr, 0, 0, nullptr, 8236, 8, UINT64_C(0xc82d7284956a1d08), "mf_enum", 0, 0 };
VarInfo * __func_info__8951eb20e251273c_fields[1] =  { &__func_info__8951eb20e251273c_field_0 };
FuncInfo __func_info__8951eb20e251273c = {"invoke block<(mf_enum:fio::FILE const? const):void> const", "", __func_info__8951eb20e251273c_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x8951eb20e251273c), 0x0 };
VarInfo __func_info__3d0cb235470c67cc_field_0 =  { Type::tRange, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 8, UINT64_C(0x9c4e3507596b38e2), "r", 0, 0 };
VarInfo * __func_info__3d0cb235470c67cc_fields[1] =  { &__func_info__3d0cb235470c67cc_field_0 };
FuncInfo __func_info__3d0cb235470c67cc = {"invoke block<(r:range const):bool const> const", "", __func_info__3d0cb235470c67cc_fields, 1, 32, &__type_info__af81fe4c86352052, nullptr,0,UINT64_C(0x3d0cb235470c67cc), 0x0 };
VarInfo __func_info__486af025dcb791d2_field_0 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16645, 8, UINT64_C(0x553fee762d74789), "en", 0, 0 };
VarInfo * __func_info__486af025dcb791d2_fields[1] =  { &__func_info__486af025dcb791d2_field_0 };
FuncInfo __func_info__486af025dcb791d2 = {"invoke block<(var en:string&):void> const", "", __func_info__486af025dcb791d2_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x486af025dcb791d2), 0x0 };
VarInfo __func_info__62423d4f8a0d8cfa_field_0 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__3fb2f51b6ebec3f2, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x86e817904c65827e), "iarr", 0, 0 };
VarInfo * __func_info__62423d4f8a0d8cfa_fields[1] =  { &__func_info__62423d4f8a0d8cfa_field_0 };
FuncInfo __func_info__62423d4f8a0d8cfa = {"invoke block<(var iarr:array<json::JsonValue?>):void> const", "", __func_info__62423d4f8a0d8cfa_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x62423d4f8a0d8cfa), 0x0 };
VarInfo __func_info__b2194afbcdafdd23_field_0 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__3fb2f51b6ebec3f2, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0xe8ba625536483ff1), "iinner", 0, 0 };
VarInfo * __func_info__b2194afbcdafdd23_fields[1] =  { &__func_info__b2194afbcdafdd23_field_0 };
FuncInfo __func_info__b2194afbcdafdd23 = {"invoke block<(var iinner:array<json::JsonValue?>):void> const", "", __func_info__b2194afbcdafdd23_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xb2194afbcdafdd23), 0x0 };
VarInfo __func_info__7ec127c5491418ed_field_0 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__3fb2f51b6ebec3f2, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0xb3268d2a2076b882), "inner", 0, 0 };
VarInfo * __func_info__7ec127c5491418ed_fields[1] =  { &__func_info__7ec127c5491418ed_field_0 };
FuncInfo __func_info__7ec127c5491418ed = {"invoke block<(var inner:array<json::JsonValue?>):void> const", "", __func_info__7ec127c5491418ed_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x7ec127c5491418ed), 0x0 };
VarInfo __func_info__a833f7b3668de771_field_0 =  { Type::tTable, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, &__type_info__3fb2f51b6ebec3f2, nullptr, nullptr, 0, 0, nullptr, 57346, 48, UINT64_C(0x5e8bbc0dcd0f8f7b), "loc", 0, 0 };
VarInfo * __func_info__a833f7b3668de771_fields[1] =  { &__func_info__a833f7b3668de771_field_0 };
FuncInfo __func_info__a833f7b3668de771 = {"invoke block<(var loc:table<string;json::JsonValue?>):void> const", "", __func_info__a833f7b3668de771_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xa833f7b3668de771), 0x0 };
VarInfo __func_info__2fb829d28965e333_field_0 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16645, 8, UINT64_C(0x9f73c6183663c3f4), "rt", 0, 0 };
VarInfo * __func_info__2fb829d28965e333_fields[1] =  { &__func_info__2fb829d28965e333_field_0 };
FuncInfo __func_info__2fb829d28965e333 = {"invoke block<(var rt:string&):void> const", "", __func_info__2fb829d28965e333_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x2fb829d28965e333), 0x0 };
VarInfo __func_info__2cd99deec9605fd9_field_0 =  { Type::tStructure, &__struct_info__f91080311cdb689c, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16390, 16, UINT64_C(0xcf7d93746006fb15), "val", 0, 0 };
VarInfo * __func_info__2cd99deec9605fd9_fields[1] =  { &__func_info__2cd99deec9605fd9_field_0 };
FuncInfo __func_info__2cd99deec9605fd9 = {"invoke block<(var val:cpp_gen::ExEnum):void> const", "", __func_info__2cd99deec9605fd9_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x2cd99deec9605fd9), 0x0 };
VarInfo __func_info__5f59ad38d8b49003_field_0 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16645, 8, UINT64_C(0x81b1eeb109811f5b), "val", 0, 0 };
VarInfo * __func_info__5f59ad38d8b49003_fields[1] =  { &__func_info__5f59ad38d8b49003_field_0 };
FuncInfo __func_info__5f59ad38d8b49003 = {"invoke block<(var val:string&):void> const", "", __func_info__5f59ad38d8b49003_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x5f59ad38d8b49003), 0x0 };
TypeInfo __type_info__774985c53b7f8953 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, nullptr, nullptr, nullptr, 0, 0, nullptr, 24845, 8, UINT64_C(0x774985c53b7f8953) };
TypeInfo __type_info__773785c53b60f353 = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, nullptr, nullptr, nullptr, 0, 0, nullptr, 57602, 24, UINT64_C(0x773785c53b60f353) };
TypeInfo __type_info__88ccb1e7aa67c692 = { Type::tStructure, &__struct_info__65b849f269df3db4, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16646, 16, UINT64_C(0x88ccb1e7aa67c692) };
TypeInfo __type_info__994066f681df7bcd = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__3fb2f51b6ebec3f2, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x994066f681df7bcd) };
TypeInfo __type_info__c294005fecf8db6c = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__74b7d404689c2060, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0xc294005fecf8db6c) };
TypeInfo __type_info__9d2fc6f14e8e098e = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__822f4971865b164a, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0x9d2fc6f14e8e098e) };
TypeInfo __type_info__3fb2f51b6ebec3f2 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__790033b1854b444b, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0x3fb2f51b6ebec3f2) };
TypeInfo __type_info__a3a4f343e75637 = { Type::tTable, nullptr, nullptr, nullptr, &__type_info__740cc30e5071c426, &__type_info__da76256779e69e76, nullptr, nullptr, 0, 0, nullptr, 57346, 48, UINT64_C(0xa3a4f343e75637) };
TypeInfo __type_info__af6b916ac10294a1 = { Type::tTable, nullptr, nullptr, nullptr, &__type_info__740cc30e5071c426, &__type_info__e0ce276400b1f9db, nullptr, nullptr, 0, 0, nullptr, 57346, 48, UINT64_C(0xaf6b916ac10294a1) };
TypeInfo __type_info__1e817199c43cb83d = { Type::tTable, nullptr, nullptr, nullptr, &__type_info__740cc30e5071c426, &__type_info__2a5b553d6263f514, nullptr, nullptr, 0, 0, nullptr, 57346, 48, UINT64_C(0x1e817199c43cb83d) };
TypeInfo __type_info__46878b9b6d25c013 = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__4e24e452b3e2444a, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x46878b9b6d25c013) };
TypeInfo __type_info__e796e1d4af74511c = { Type::tTable, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, &__type_info__3fb2f51b6ebec3f2, nullptr, nullptr, 0, 0, nullptr, 57346, 48, UINT64_C(0xe796e1d4af74511c) };
TypeInfo __type_info__12283e04d98e7c73 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0x12283e04d98e7c73) };
TypeInfo __type_info__5eddda99b960ff80 = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x5eddda99b960ff80) };
TypeInfo __type_info__4247e6b31bed7a2e = { Type::tStructure, &__struct_info__2754d5ab960ed18d, nullptr, nullptr, &__type_info__3ee86589dd970c38, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 72, UINT64_C(0x4247e6b31bed7a2e) };
TypeInfo __type_info__16d0aa3dd6b69257 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~fio::FILE"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 34, 16, UINT64_C(0x16d0aa3dd6b69257) };
TypeInfo __type_info__fa593d0882a72913 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16548, 8, UINT64_C(0xfa593d0882a72913) };
TypeInfo __type_info__af81fe4c86352052 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 1, UINT64_C(0xaf81fe4c86352052) };
TypeInfo __type_info__af8afe4c86446b52 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 4, UINT64_C(0xaf8afe4c86446b52) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, 8, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__37d36026a6078a42 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~strings::StringBuilderWriter"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 2, 288, UINT64_C(0x37d36026a6078a42) };
TypeInfo __type_info__3ee86589dd970c38 = { Type::tStructure, &__struct_info__31646f6f189752b9, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16390, 16, UINT64_C(0x3ee86589dd970c38) };
TypeInfo __type_info__39cb2cb435aca4fa = { Type::tStructure, &__struct_info__2754d5ab960ed18d, nullptr, nullptr, &__type_info__3ee86589dd970c38, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 72, UINT64_C(0x39cb2cb435aca4fa) };
TypeInfo __type_info__a4d78337018e0bdc = { Type::tStructure, &__struct_info__f20bf1a74b2e5459, nullptr, nullptr, &__type_info__3ee86589dd970c38, nullptr, nullptr, nullptr, 0, 0, nullptr, 16390, 40, UINT64_C(0xa4d78337018e0bdc) };
TypeInfo __type_info__35d7933e81898305 = { Type::tStructure, &__struct_info__11a4b507a1eef18c, nullptr, nullptr, &__type_info__3ee86589dd970c38, nullptr, nullptr, nullptr, 0, 0, nullptr, 16390, 32, UINT64_C(0x35d7933e81898305) };
TypeInfo __type_info__d91f1e32fc332235 = { Type::tStructure, &__struct_info__65b849f269df3db4, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16390, 16, UINT64_C(0xd91f1e32fc332235) };
TypeInfo __type_info__74b7d404689c2060 = { Type::tStructure, &__struct_info__e0ec7ef36db1f, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24586, 48, UINT64_C(0x74b7d404689c2060) };
TypeInfo __type_info__822f4971865b164a = { Type::tStructure, &__struct_info__8102cea4dbf746b1, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24586, 48, UINT64_C(0x822f4971865b164a) };
TypeInfo __type_info__790033b1854b444b = { Type::tStructure, &__struct_info__69e2ef6fbeda51a0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 56, UINT64_C(0x790033b1854b444b) };
TypeInfo __type_info__888650585a1a67a = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x888650585a1a67a) };
TypeInfo __type_info__740cc30e5071c426 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x740cc30e5071c426) };
TypeInfo __type_info__4e24e452b3e2444a = { Type::tStructure, &__struct_info__2754d5ab960ed18d, nullptr, nullptr, &__type_info__3ee86589dd970c38, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 72, UINT64_C(0x4e24e452b3e2444a) };
TypeInfo __type_info__4e466b022bf09776 = { Type::tStructure, &__struct_info__d92acaab53aeb957, nullptr, nullptr, &__type_info__3ee86589dd970c38, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 56, UINT64_C(0x4e466b022bf09776) };
TypeInfo __type_info__eac86cf3f5a31edb = { Type::tStructure, &__struct_info__2754d5ab960ed18d, nullptr, nullptr, &__type_info__3ee86589dd970c38, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 72, UINT64_C(0xeac86cf3f5a31edb) };
TypeInfo __type_info__5eb1a2023943c414 = { Type::tStructure, &__struct_info__1d90238ba589ff21, nullptr, nullptr, &__type_info__3ee86589dd970c38, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 72, UINT64_C(0x5eb1a2023943c414) };
TypeInfo __type_info__3fa97cc3c3615476 = { Type::tStructure, &__struct_info__d92acaab53aeb957, nullptr, nullptr, &__type_info__3ee86589dd970c38, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 56, UINT64_C(0x3fa97cc3c3615476) };
TypeInfo __type_info__a3a57ed1f9f48fdb = { Type::tStructure, &__struct_info__2754d5ab960ed18d, nullptr, nullptr, &__type_info__3ee86589dd970c38, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 72, UINT64_C(0xa3a57ed1f9f48fdb) };
TypeInfo __type_info__2f9ee4ce8425e314 = { Type::tStructure, &__struct_info__1d90238ba589ff21, nullptr, nullptr, &__type_info__3ee86589dd970c38, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 72, UINT64_C(0x2f9ee4ce8425e314) };
TypeInfo __type_info__da76256779e69e76 = { Type::tStructure, &__struct_info__d92acaab53aeb957, nullptr, nullptr, &__type_info__3ee86589dd970c38, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 56, UINT64_C(0xda76256779e69e76) };
TypeInfo __type_info__e0ce276400b1f9db = { Type::tStructure, &__struct_info__2754d5ab960ed18d, nullptr, nullptr, &__type_info__3ee86589dd970c38, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 72, UINT64_C(0xe0ce276400b1f9db) };
TypeInfo __type_info__2a5b553d6263f514 = { Type::tStructure, &__struct_info__1d90238ba589ff21, nullptr, nullptr, &__type_info__3ee86589dd970c38, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 72, UINT64_C(0x2a5b553d6263f514) };
TypeInfo __type_info__af63df4c8601f1a5 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0xaf63df4c8601f1a5) };
TypeInfo __type_info__af63d94c8601e773 = { Type::tDouble, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 8, UINT64_C(0xaf63d94c8601e773) };
TypeInfo __type_info__af63e44c8601fa24 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xaf63e44c8601fa24) };
TypeInfo __type_info__af63ee4c86020b22 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xaf63ee4c86020b22) };
TypeInfo __type_info__af63eb4c86020609 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 0, UINT64_C(0xaf63eb4c86020609) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {__type_info__16d0aa3dd6b69257, __type_info__37d36026a6078a42, };
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[1] = { &__type_info__5eb1a2023943c414 };
TypeInfo * __tinfo_1[1] = { &__type_info__2f9ee4ce8425e314 };
TypeInfo * __tinfo_2[1] = { &__type_info__4e466b022bf09776 };
TypeInfo * __tinfo_3[1] = { &__type_info__3fa97cc3c3615476 };
TypeInfo * __tinfo_4[1] = { &__type_info__a3a4f343e75637 };
TypeInfo * __tinfo_5[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_6[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_7[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_8[1] = { &__type_info__eac86cf3f5a31edb };
TypeInfo * __tinfo_9[1] = { &__type_info__a3a57ed1f9f48fdb };
TypeInfo * __tinfo_10[1] = { &__type_info__46878b9b6d25c013 };
TypeInfo * __tinfo_11[2] = { &__type_info__37d36026a6078a42, &__type_info__fa593d0882a72913 };
TypeInfo * __tinfo_12[2] = { &__type_info__37d36026a6078a42, &__type_info__888650585a1a67a };
TypeInfo * __tinfo_13[1] = { &__type_info__4247e6b31bed7a2e };
TypeInfo * __tinfo_14[1] = { &__type_info__af6b916ac10294a1 };
TypeInfo * __tinfo_15[3] = { &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_16[1] = { &__type_info__1e817199c43cb83d };
TypeInfo * __tinfo_17[2] = { &__type_info__37d36026a6078a42, &__type_info__fa593d0882a72913 };
TypeInfo * __tinfo_18[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_19[2] = { &__type_info__af90fe4c864e9d52, &__type_info__3fb2f51b6ebec3f2 };
TypeInfo * __tinfo_20[3] = { &__type_info__af90fe4c864e9d52, &__type_info__e796e1d4af74511c, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_21[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_22[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_23[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_24[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_25[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_26[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_27[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_28[1] = { &__type_info__5eddda99b960ff80 };
TypeInfo * __tinfo_29[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_30[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_31[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_32[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_33[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_34[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_35[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_36[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_37[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_38[11] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_39[2] = { &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_40[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_41[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_42[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_43[9] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_44[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_45[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_46[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_47[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_48[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_49[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_50[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_51[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_52[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_53[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_54[3] = { &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_55[7] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_56[9] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_57[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_58[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_59[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_60[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24 };
TypeInfo * __tinfo_61[4] = { &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_62[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_63[1] = { &__type_info__af8afe4c86446b52 };
TypeInfo * __tinfo_64[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_65[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_66[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_67[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_68[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_69[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_70[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_71[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_72[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_73[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_74[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_75[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_76[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_77[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_78[8] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_79[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_80[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_81[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__773785c53b60f353, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_82[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_83[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_84[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_85[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_86[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_87[7] = { &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_88[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_89[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_90[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_91[7] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_92[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_93[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_94[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_95[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_96[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_97[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_98[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_99[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_100[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_101[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_102[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_103[10] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_104[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_105[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_106[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_107[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };

inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_c777430705b7c258 ( Context * __context__, cpp_gen::ClangStruct & __a_rename_at_32_0, cpp_gen::ClangStruct & __b_rename_at_32_1 );
inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_e79b4e2307f7ae1c ( Context * __context__, cpp_gen::ClangEnum & __a_rename_at_32_2, cpp_gen::ClangEnum & __b_rename_at_32_3 );
inline AutoTuple<char *,char *> _Funccpp_genTickonNumericLiteralTick10275515307319155835_73739de28d614e90 ( Context * __context__, TTable<char *,json::JsonValue *> & __inner_obj_rename_at_337_4 );
inline void _FuncbuiltinTickpushTick10769833213962245646_5a77afdd4cbd09e1 ( Context * __context__, TArray<char *> & __Arr_rename_at_181_8, char * __value_rename_at_181_9 );
inline cpp_gen::ClangEnum & _FuncbuiltinTick_at_with_lockcheckTick7807051423786862253_68d1e221d9f9bb60 ( Context * __context__, TTable<char *,cpp_gen::ClangEnum> & __Tab_rename_at_871_10, char * const  __at_rename_at_871_11 );
inline void _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af ( Context * __context__, TArray<char *> & __Arr_rename_at_165_12, char * const  __value_rename_at_165_13 );
inline void finalize_8a84dcb6bb73936b ( Context * __context__, cpp_gen::_lambda_cpp_gen_543_2 & ____this_rename_at_543_14 );
inline AutoTuple<char *,char *> _Funccpp_genTickonInitArgTick7456162652026714814_980c8ded9aae7f9d ( Context * __context__, TTable<char *,json::JsonValue *> & __root_rename_at_348_15 );
inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_cfad5be86fc7ec48 ( Context * __context__, cpp_gen::ClangFunc & __a_rename_at_32_32, cpp_gen::ClangFunc & __b_rename_at_32_33 );
inline void _FuncbuiltinTickemplaceTick13923705686753630697_b7c430019eafb474 ( Context * __context__, TArray<cpp_gen::ClangFunc> & __Arr_rename_at_287_34, cpp_gen::ClangFunc & __value_rename_at_287_35 );
inline bool _Func_lambda_cpp_gen_543_2Tickfunction_10be92d9c230e7ac ( Context * __context__, cpp_gen::_lambda_cpp_gen_543_2 & ____this_rename_at_543_36, char * * & ___ryield543_rename_at_543_37 );
inline void _Func_lambda_cpp_gen_543_2Tickfinalizer_67e83694801db280 ( Context * __context__, cpp_gen::_lambda_cpp_gen_543_2 * ____this_rename_at_543_38 );
inline Sequence DAS_COMMENT((cpp_gen::Scope &)) _FuncbuiltinTickeachTick6002865651812066953_ab768a47fa5edfe2 ( Context * __context__, TArray<cpp_gen::Scope> const  & __a_rename_at_1273_39 );
inline char * _Funcstrings_boostTickjoinTick17792841289284275598_f97bd9e0a3ac85c9 ( Context * __context__, Sequence DAS_COMMENT((char * &)) & __it_rename_at_46_41, char * const  __separator_rename_at_46_42 );
inline void finalize_ca4ff32f655e0ba8 ( Context * __context__, cpp_gen::_lambda_cpp_gen_266_1 & ____this_rename_at_266_47 );
inline cpp_gen::ClangFunc & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_7e2d4453a57c7761 ( Context * __context__, cpp_gen::ClangFunc & __a_rename_at_50_48 );
inline Sequence DAS_COMMENT((char * &)) _FuncbuiltinTickeach_refTick16137060296763333387_fc747e0a786c7774 ( Context * __context__, Lambda DAS_COMMENT((bool,char * * &)) const  __lam_rename_at_1299_49 );
inline void _FuncbuiltinTickpushTick10769833213962245646_3023c4bc268c52d7 ( Context * __context__, TArray<cpp_gen::ClangStructField> & __Arr_rename_at_181_51, cpp_gen::ClangStructField & __value_rename_at_181_52 );
inline cpp_gen::ClangFunc & _FuncbuiltinTick_at_with_lockcheckTick7807051423786862253_81b03a6fcc783e7f ( Context * __context__, TTable<char *,cpp_gen::ClangFunc> & __Tab_rename_at_871_53, char * const  __at_rename_at_871_54 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_75085f7298948768 ( Context * __context__, TArray<cpp_gen::Scope> & __Arr_rename_at_68_55, int32_t __newSize_rename_at_68_56 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_9f21c57cd4b4c83 ( Context * __context__, TArray<DAS_COMMENT(enum) cpp_gen::AccessKind> & __Arr_rename_at_68_57, int32_t __newSize_rename_at_68_58 );
inline void _FuncbuiltinTickemplaceTick13923705686753630697_650e65a9cf1ca823 ( Context * __context__, TArray<cpp_gen::ClangFuncArg> & __Arr_rename_at_287_59, cpp_gen::ClangFuncArg & __value_rename_at_287_60 );
inline char * _Funccpp_genTickgetCppNameTick2578297908749988033_7a531127a37dc761 ( Context * __context__, char * const  __baseName_rename_at_542_61 );
inline bool _Func_lambda_cpp_gen_266_1Tickfunction_8e17cb5a29f7eaff ( Context * __context__, cpp_gen::_lambda_cpp_gen_266_1 & ____this_rename_at_266_63, char * * & ___ryield266_rename_at_266_64 );
inline void _Func_lambda_cpp_gen_266_1Tickfinalizer_bbb8c1e2c3d5dafe ( Context * __context__, cpp_gen::_lambda_cpp_gen_266_1 * ____this_rename_at_266_65 );
inline cpp_gen::ClangStruct & _FuncbuiltinTick_at_with_lockcheckTick7807051423786862253_9ef1c864c9d2f80 ( Context * __context__, TTable<char *,cpp_gen::ClangStruct> & __Tab_rename_at_871_66, char * const  __at_rename_at_871_67 );
inline bool _Funccpp_genTickis_valid_locTick10808118537805394975_6bace0942c51e77f ( Context * __context__, TTable<char *,json::JsonValue *> & __loc_rename_at_192_68 );
inline void _FuncbuiltinTickpushTick14133213201864676143_f626dd014072c607 ( Context * __context__, TArray<DAS_COMMENT(enum) cpp_gen::AccessKind> & __Arr_rename_at_165_69, DAS_COMMENT(enum) cpp_gen::AccessKind __value_rename_at_165_70 );
inline void _FuncbuiltinTickpushTick10769833213962245646_440dfee5ce51ee9b ( Context * __context__, TArray<cpp_gen::Scope> & __Arr_rename_at_181_71, cpp_gen::Scope & __value_rename_at_181_72 );
inline void _FuncbuiltinTickpopTick1161079256290593740_76e5d0bb5a9d86cc ( Context * __context__, TArray<char *> & __Arr_rename_at_132_73 );
inline void _FuncbuiltinTickpopTick1161079256290593740_15fc7a604b10573f ( Context * __context__, TArray<cpp_gen::Scope> & __Arr_rename_at_132_74 );
inline void _FuncbuiltinTickpopTick1161079256290593740_9fb9082827455aec ( Context * __context__, TArray<DAS_COMMENT(enum) cpp_gen::AccessKind> & __Arr_rename_at_132_75 );
inline char * _Funccpp_genTickgetQualTypeTick1400640308412000890_a58540215c7d0fa9 ( Context * __context__, TTable<char *,json::JsonValue *> & __root_rename_at_332_76, char * const  __field_rename_at_332_77 );
inline bool _Funccpp_genTickisBitfieldTick9047106797565157337_d37f3cd228cd993f ( Context * __context__, TTable<char *,json::JsonValue *> & __root_rename_at_516_79 );
inline cpp_gen::ClangFunc _Funccpp_genTickonFunctionBodyTick983279341569892450_8caee9a44ff10098 ( Context * __context__, TTable<char *,json::JsonValue *> & __root_rename_at_404_80, bool __isMethod_rename_at_404_81, char * const  __ofClass_rename_at_404_82 );
inline char * _Funcstrings_boostTickjoinTick16475640899284277631_97cbd29033d05835 ( Context * __context__, TArray<char *> const  & __it_rename_at_22_97, char * const  __separator_rename_at_22_98 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_963e1e05a7a36522 ( Context * __context__, TTable<char *,json::JsonValue *> const  & __Tab_rename_at_1014_103, char * const  __at_rename_at_1014_104 );
inline void _Funccpp_genTickwith_objectTick7594167909273532910_43fb3bcbe18cb920 ( Context * __context__, TTable<char *,json::JsonValue *> & __jso_rename_at_182_105, char * const  __field_rename_at_182_106, Block DAS_COMMENT((void,TTable<char *,json::JsonValue *>)) const  & __blk_rename_at_182_107 );
inline void _Funccpp_genTickonStructTick15011585540411908992_8d9737e4ce0fff22 ( Context * __context__, TTable<char *,json::JsonValue *> & __root_rename_at_472_108 );
inline void _Funccpp_genTickonNamespaceTick1569694100191165612_31878f48597dada2 ( Context * __context__, TTable<char *,json::JsonValue *> & __root_rename_at_261_112 );
inline void _Funccpp_genTickonFieldTick11712683440231282153_f030e55d3f2f1f92 ( Context * __context__, TTable<char *,json::JsonValue *> & __root_rename_at_525_115 );
inline void _Funccpp_genTickonFunctionTick9237376346942193985_2dab482eabe1f2c3 ( Context * __context__, TTable<char *,json::JsonValue *> & __root_rename_at_467_120 );
inline void _Funccpp_genTickonMethodTick5355858089746197764_68eed9a736f85c63 ( Context * __context__, TTable<char *,json::JsonValue *> & __root_rename_at_458_122 );
inline void _Funccpp_genTickonEnumTick2734318481451533008_f3a14439e6d126ab ( Context * __context__, TTable<char *,json::JsonValue *> & __root_rename_at_275_124 );
inline void _Funccpp_genTickonVarTick7763998161592485260_1b4b388a8d41c569 ( Context * __context__, TTable<char *,json::JsonValue *> & __root_rename_at_316_132 );
inline void _Funccpp_genTickonTypeDeclTick13685927393030072381_48d18a41d3948b70 ( Context * __context__, TTable<char *,json::JsonValue *> & __root_rename_at_552_137 );
inline void _Funccpp_genTickonAccessTick18228828068813142789_a461a7e898d3234f ( Context * __context__, TTable<char *,json::JsonValue *> & __root_rename_at_501_141 );
inline void _Funccpp_genTickonLinkageSpecTick9588560724416692279_347cf8f7a9c1d56 ( Context * __context__, TTable<char *,json::JsonValue *> & __root_rename_at_246_144 );
inline void _Funccpp_genTickwith_arrayTick6272002616190333845_c90f8a054aaf2ef6 ( Context * __context__, TTable<char *,json::JsonValue *> & __jso_rename_at_187_145, char * const  __field_rename_at_187_146, Block DAS_COMMENT((void,TArray<json::JsonValue *>)) const  & __blk_rename_at_187_147 );
inline bool _Funccpp_genTickis_strTick14919838023837841642_48d382d152e59de8 ( Context * __context__, json::JsonValue * const  __jso_rename_at_178_148, char * const  __str_rename_at_178_149 );
inline void _Funccpp_genTickonInnerTick15341579017765610681_62ed76bf0329fb0d ( Context * __context__, TTable<char *,json::JsonValue *> & __root_rename_at_199_150 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_4ca5259fa9e9c74e ( Context * __context__, TDim<char *,1> const  & __a_rename_at_581_157 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_14682004b38ad2a1 ( Context * __context__, TArray<char *> & __Arr_rename_at_68_158, int32_t __newSize_rename_at_68_159 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_401f694502683cef ( Context * __context__, TArray<AutoTuple<range,char *>> & __Arr_rename_at_68_160, int32_t __newSize_rename_at_68_161 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_ddf4f34775c3f6ad ( Context * __context__, TDim<char *,4> const  & __a_rename_at_581_162 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_30458db6e3c9dc4b ( Context * __context__, TDim<char *,2> const  & __a_rename_at_581_163 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_3a6bd4fafb6564b7 ( Context * __context__, TDim<AutoTuple<range,char *>,1> const  & __a_rename_at_581_164 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_d57f120a5eb06ce3 ( Context * __context__, TDim<AutoTuple<range,char *>,3> const  & __a_rename_at_581_165 );
inline void _FuncfioTickfreadTick9799048084303762322_c82e6deab5e5b700 ( Context * __context__, FILE const  * const  __f_rename_at_43_166, Block DAS_COMMENT((void,char * const )) const  & __blk_rename_at_43_167 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_9d11994217d1865a ( Context * __context__, TTable<int32_t,bool> const  & __Tab_rename_at_1014_169, int32_t __at_rename_at_1014_170 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_9b8b56716258076a ( Context * __context__, TTable<char *,cpp_gen::ClangStruct> const  & __Tab_rename_at_1014_171, char * const  __at_rename_at_1014_172 );
inline bool _FuncbuiltinTickgetTick8447005936052527643_c25c472f7c63dd94 ( Context * __context__, TTable<char *,cpp_gen::ExEnum> & __Tab_rename_at_654_173, char * const  __at_rename_at_654_174, Block DAS_COMMENT((void,cpp_gen::ExEnum)) const  & __blk_rename_at_654_175 );
inline bool _Funccpp_genTickneedToGenStructTick14158910623600840251_fd93d910f98eae6b ( Context * __context__, cpp_gen::ClangStruct const  & __st_rename_at_652_177 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_1a54ecb7273c2da1 ( Context * __context__, TTable<char *,char *> const  & __Tab_rename_at_1014_178, char * const  __at_rename_at_1014_179 );
inline bool _Funccpp_genTickneedToGenFieldTick2521814960739047210_6a0d55d67bdff534 ( Context * __context__, cpp_gen::ClangStructField const  & __fld_rename_at_664_180 );
inline bool _Funccpp_genTickisBlockedFunTick2478178585869301773_97ae78cd67b212a3 ( Context * __context__, cpp_gen::ClangFunc const  & __fn_rename_at_771_181 );
inline TArray<char *> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_354a9a0b57d554a0 ( Context * __context__, TArray<char *> & __a_rename_at_50_183 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_79f9e841b4434fd8 ( Context * __context__, TArray<char *> & __a_rename_at_1182_184 );
inline void _FuncbuiltinTickfinalizeTick5454204887383796109_9c4083566563a85a ( Context * __context__, TTable<char *,bool> & __a_rename_at_1193_185 );
inline TArray<char *> _FuncbuiltinTickto_array_moveTick3185538323411982277_9285ff1255c3d503 ( Context * __context__, TDim<char *,1> & __a_rename_at_1342_186 );
inline Sequence DAS_COMMENT((cpp_gen::ClangEnum &)) _FuncbuiltinTickvaluesTick1351216622833168869_93f4fc8357f87453 ( Context * __context__, TTable<char *,cpp_gen::ClangEnum> & __a_rename_at_1150_188 );
inline Sequence DAS_COMMENT((cpp_gen::ClangStruct &)) _FuncbuiltinTickvaluesTick1351216622833168869_a5f65aa1a7088367 ( Context * __context__, TTable<char *,cpp_gen::ClangStruct> & __a_rename_at_1150_190 );
inline bool _FuncbuiltinTickgetTick8447005936052527643_30d18a39e61d48bc ( Context * __context__, TTable<char *,char *> & __Tab_rename_at_654_192, char * const  __at_rename_at_654_193, Block DAS_COMMENT((void,char * &)) const  & __blk_rename_at_654_194 );
inline Sequence DAS_COMMENT((cpp_gen::ClangFunc &)) _FuncbuiltinTickvaluesTick1351216622833168869_7c0785e58fa9645f ( Context * __context__, TTable<char *,cpp_gen::ClangFunc> & __a_rename_at_1150_196 );
inline Sequence DAS_COMMENT((char *)) _FuncbuiltinTickkeysTick2205854368403803976_2df91d3f4a5c8888 ( Context * __context__, TTable<char *,char *> const  & __a_rename_at_1128_198 );
inline Sequence DAS_COMMENT((char * &)) _FuncbuiltinTickvaluesTick1351216622833168869_16866e32c6607afb ( Context * __context__, TTable<char *,char *> & __a_rename_at_1150_200 );
inline char * _Funccpp_genTickqType2FnTypeClassTick18208979152004666060_cdb79d2da470a00b ( Context * __context__, char * const  __st_rename_at_806_202, char * const  __className_rename_at_806_203 );
inline char * _Funccpp_genTickqType2FnTypeTick1482691952773259010_a02b5143c6271bc3 ( Context * __context__, char * const  __st_rename_at_800_205 );
inline bool _Funccpp_genTickisRefFunTick11632776912375340809_d4070b3dd9da7cd2 ( Context * __context__, cpp_gen::ClangFunc const  & __fn_rename_at_822_207 );
inline bool _Funccpp_genTickisCmresFunTick16266856454814809519_86372ea3375f1a8c ( Context * __context__, cpp_gen::ClangFunc const  & __fn_rename_at_838_210 );
inline Sequence DAS_COMMENT((char *)) _FuncbuiltinTickkeysTick2205854368403803976_bcec88c808d50381 ( Context * __context__, TTable<char *,cpp_gen::ExEnum> const  & __a_rename_at_1128_216 );
inline Sequence DAS_COMMENT((cpp_gen::ExEnum &)) _FuncbuiltinTickvaluesTick1351216622833168869_7282d82fa9dd61d5 ( Context * __context__, TTable<char *,cpp_gen::ExEnum> & __a_rename_at_1150_218 );
inline void _FuncfioTickfopenTick3937565566638487747_feeb71cd63fd8eac ( Context * __context__, char * const  __name_rename_at_12_220, char * const  __mode_rename_at_12_221, Block DAS_COMMENT((void,FILE const  * const )) const  & __blk_rename_at_12_222 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_efda835c0359ec76 ( Context * __context__, TTable<char *,bool> const  & __Tab_rename_at_1014_224, char * const  __at_rename_at_1014_225 );
inline bool isBlockedType_cc1ed1c7968e329c ( Context * __context__, char * const  __qtype_rename_at_566_226 );
inline bool isLocalType_36973d575e8241e5 ( Context * __context__, char * const  __qtype_rename_at_575_228 );
inline void genEnums_816c948d88e68308 ( Context * __context__, FILE const  * const  __hf_rename_at_585_230, FILE const  * const  __mf_rename_at_585_231, FILE const  * const  __df_rename_at_585_232 );
inline char * getBindFldName_4490111cebfe248e ( Context * __context__, char * const  __name_rename_at_677_242 );
inline void genStructs_e7c6b33a2f752b8e ( Context * __context__, FILE const  * const  __hf_rename_at_684_243, FILE const  * const  __mf_rename_at_684_244, FILE const  * const  __df_rename_at_684_245 );
inline char * methodName_ff1251eb1b75eec2 ( Context * __context__, char * const  __name_rename_at_875_252 );
inline void openSplitFile_ea32c746c410d324 ( Context * __context__ );
inline void closeSplitFile_233b8cc7d4e11a96 ( Context * __context__ );
inline void getFunctions_26873a00d281aa69 ( Context * __context__, FILE const  * const  __df_rename_at_905_256, FILE const  * const  __df_method_rename_at_905_257 );
inline TArray<char *> parseQualArgs_3f5a273f6933fa8f ( Context * __context__, char * const  __qname_rename_at_947_267, char * & __rtype_rename_at_947_268 );
inline char * getDefinePrefix_68940f3e2e3e9e89 ( Context * __context__, char * const  __xname_rename_at_956_270 );
inline void genFunction_d3eae2156dccabbe ( Context * __context__, cpp_gen::ClangFunc const  & __fn_rename_at_967_275, bool __castbind_rename_at_967_276, FILE const  * const  __df_rename_at_967_277 );
inline void genBindings_6b7b54142f4c772b ( Context * __context__, FILE const  * const  __hf_rename_at_1086_300, FILE const  * const  __df_rename_at_1086_301, FILE const  * const  __mf_enum_rename_at_1086_302, FILE const  * const  __df_enum_rename_at_1086_303, FILE const  * const  __mf_ann_rename_at_1086_304, FILE const  * const  __df_ann_rename_at_1086_305, FILE const  * const  __df_method_rename_at_1086_306 );
inline void bindHeaders_5b0cabfc50efc6fe ( Context * __context__, char * const  __fname_rename_at_1093_307, char * const  __outprefix_rename_at_1093_308 );
inline void genDefineConstants_9a873908595aec49 ( Context * __context__, char * const  __fname_rename_at_1146_328, char * const  __prefix_rename_at_1146_329 );
inline char * getGenConstType_ba5129bb3b413133 ( Context * __context__, char * const  __name_rename_at_1152_331, char * const  __default_name_rename_at_1152_332 );
inline void searchAndGenConst_ec8cac3d091d1a8 ( Context * __context__, regex::Regex & __regex_def_rename_at_1156_333, char * const  __defTName_rename_at_1156_334, char * const  __suffix_rename_at_1156_335, TTable<int32_t,bool> & __ofs_rename_at_1156_336, char * const  __data_rename_at_1156_337, TTable<char *,bool> & __dup_rename_at_1156_338, FILE const  * const  __hf_rename_at_1156_339 );
inline void genDefineConstants_7bfbc66c3d4f2529 ( Context * __context__, TArray<char *> const  & __fnames_rename_at_1177_344, char * const  __prefix_rename_at_1177_345 );
inline cpp_gen::ExEnum ExEnum_93a02f0673e373b0 ( Context * __context__ );
inline TArray<char *> _FuncbuiltinTickto_array_moveTick3185538323411982277_983e7a1ec42b37c0 ( Context * __context__, TDim<char *,4> & __a_rename_at_1342_359 );
inline TArray<char *> _FuncbuiltinTickto_array_moveTick3185538323411982277_d6a74d6804670a19 ( Context * __context__, TDim<char *,2> & __a_rename_at_1342_361 );
inline TArray<AutoTuple<range,char *>> _FuncbuiltinTickto_array_moveTick3185538323411982277_a9d6d54733762ba ( Context * __context__, TDim<AutoTuple<range,char *>,1> & __a_rename_at_1342_363 );
inline TArray<AutoTuple<range,char *>> _FuncbuiltinTickto_array_moveTick3185538323411982277_71fbdf75ac58194 ( Context * __context__, TDim<AutoTuple<range,char *>,3> & __a_rename_at_1342_365 );

void __init_script ( Context * __context__, bool __init_shared )
{
    TDim<char *,4> _temp_make_local_36_42_32;
    TDim<char *,2> _temp_make_local_45_39_32;
    regex::Regex _temp_make_local_769_16_32;
    TDim<AutoTuple<range,char *>,1> _temp_make_local_769_16_160;
    regex::Regex _temp_make_local_944_16_32;
    TDim<AutoTuple<range,char *>,3> _temp_make_local_944_16_336;
    das_global<bool,0x85c0860e757e6aef>(__context__) = false;/*verbose*/
    das_global<char *,0xeb1757bee891e8da>(__context__) = nullptr;/*extra_args*/
    das_global_zero<TArray<char *>,0x7047550f5f3a4677>(__context__);/*local_type_names*/
    das_global_zero<TArray<char *>,0xbf1aa9ac043ebf89>(__context__);/*blocked_type_names*/
    das_global<TArray<char *>,0xfecc248913d2b09d>(__context__) = _FuncbuiltinTickto_array_moveTick3185538323411982277_983e7a1ec42b37c0(__context__,das_arg<TDim<char *,4>>::pass((([&]() -> TDim<char *,4>& {
        _temp_make_local_36_42_32(0,__context__) = ((char *) "operator delete");
        _temp_make_local_36_42_32(1,__context__) = ((char *) "operator new");
        _temp_make_local_36_42_32(2,__context__) = ((char *) "__security_init_cookie");
        _temp_make_local_36_42_32(3,__context__) = ((char *) "__security_check_cookie");
        return _temp_make_local_36_42_32;
    })())));/*blocked_functions*/
    das_global_zero<TTable<char *,bool>,0xd633af84de8bdc21>(__context__);/*blocked_functions_table*/
    das_global<TArray<char *>,0x63b059e161d0e5e3>(__context__) = _FuncbuiltinTickto_array_moveTick3185538323411982277_d6a74d6804670a19(__context__,das_arg<TDim<char *,2>>::pass((([&]() -> TDim<char *,2>& {
        _temp_make_local_45_39_32(0,__context__) = ((char *) "true");
        _temp_make_local_45_39_32(1,__context__) = ((char *) "false");
        return _temp_make_local_45_39_32;
    })())));/*blocked_defines*/
    das_global_zero<TTable<char *,bool>,0xc079186d7d42988b>(__context__);/*blocked_defines_table*/
    das_global_zero<TArray<char *>,0x8cd67f1342cfa70e>(__context__);/*blocked_enumerations*/
    das_global_zero<TTable<char *,bool>,0x1167f377740b3cc>(__context__);/*blocked_enumerations_table*/
    das_global_zero<Lambda DAS_COMMENT((bool,char * const )),0xdda227e969ec720f>(__context__);/*is_function_blocked*/
    das_global_zero<Lambda DAS_COMMENT((bool,char * const )),0x113c776499a2302e>(__context__);/*is_const_blocked*/
    das_global_zero<Lambda DAS_COMMENT((bool,char * const )),0xcf018a17ba6e16de>(__context__);/*is_struct_blocked*/
    das_global_zero<Lambda DAS_COMMENT((char *,char * const )),0x3451b360923f86eb>(__context__);/*alt_struct_name*/
    das_global_zero<TArray<char *>,0x41b00c9a154fb7c5>(__context__);/*cmres_functions*/
    das_global<bool,0xbe90e5ba972c8a8b>(__context__) = false;/*allow_extern_c*/
    das_global_zero<TTable<char *,cpp_gen::ExEnum>,0x3b1deb0a06e30b10>(__context__);/*ex_enums*/
    das_global_zero<TTable<char *,char *>,0xc86829fad0577a95>(__context__);/*substitute_field_types*/
    das_global_zero<TTable<char *,char *>,0x5f4171982abde6b1>(__context__);/*alias_types*/
    das_global_zero<TTable<char *,char *>,0xa8937f5fc0357310>(__context__);/*namespace_to_prefix*/
    das_global<bool,0x14dfebd35adf67c4>(__context__) = false;/*generate_split_functions*/
    das_global<char *,0x2572ec17c7f55c54>(__context__) = nullptr;/*split_prefix*/
    das_global<char *,0x2505284bc104585d>(__context__) = nullptr;/*split_cpp_prefix*/
    das_global<char *,0xe914b75272ad2062>(__context__) = nullptr;/*split_cpp_suffix*/
    das_global<char *,0xc951cb83148d6076>(__context__) = nullptr;/*enum_suffix*/
    das_global_zero<TTable<char *,char *>,0x713f35e9b29fd64e>(__context__);/*const_type_table*/
    das_global<char *,0x26b521e3cb75743>(__context__) = ((char *) "uint8_t");/*const_uint8_type*/
    das_global<char *,0x27b3ae2f4cd224dd>(__context__) = ((char *) "uint16_t");/*const_uint16_type*/
    das_global_zero<TTable<char *,bool>,0xff4ca70677f05db6>(__context__);/*ex_keywords*/
    das_global<TDim<char *,17>,0xc6488469a096d2fc>(__context__) = (([&]() -> TDim<char *,17> {
        TDim<char *,17> __mka_90;
        __mka_90(0,__context__) = ((char *) "float");
        __mka_90(1,__context__) = ((char *) "double");
        __mka_90(2,__context__) = ((char *) "uint");
        __mka_90(3,__context__) = ((char *) "uint8");
        __mka_90(4,__context__) = ((char *) "uint16");
        __mka_90(5,__context__) = ((char *) "uint64");
        __mka_90(6,__context__) = ((char *) "uint2");
        __mka_90(7,__context__) = ((char *) "uint3");
        __mka_90(8,__context__) = ((char *) "uint4");
        __mka_90(9,__context__) = ((char *) "int");
        __mka_90(10,__context__) = ((char *) "int8");
        __mka_90(11,__context__) = ((char *) "int16");
        __mka_90(12,__context__) = ((char *) "int64");
        __mka_90(13,__context__) = ((char *) "int2");
        __mka_90(14,__context__) = ((char *) "int3");
        __mka_90(15,__context__) = ((char *) "int4");
        __mka_90(16,__context__) = ((char *) "bool");
        return __mka_90;
    })());/*all_keywords*/
    das_global_zero<TArray<cpp_gen::Scope>,0x82cea5b26477762c>(__context__);/*name_stack*/
    das_global_zero<TArray<DAS_COMMENT(enum) cpp_gen::AccessKind>,0x6adb8b8c35686843>(__context__);/*access_stack*/
    das_global_zero<TArray<char *>,0x5425979f44e6bd00>(__context__);/*struct_stack*/
    das_global_zero<cpp_gen::ClangAst,0x3d3f7dbddddbae8e>(__context__);/*ast*/
    das_global_zero<TTable<char *,char *>,0x69fa647f2b48b85a>(__context__);/*rev_enums*/
    das_global<regex::Regex,0xbf41002a7c46ff12>(__context__) = das_invoke_function<regex::Regex>::invoke_cmres<regex::Regex &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::regex_compile S<regex::Regex>*/ 0x235aa29958cfc4c4)),das_arg<regex::Regex>::pass((([&]() -> regex::Regex& {
        das_zero(_temp_make_local_769_16_32);
        das_copy((_temp_make_local_769_16_32.root),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
            regex::ReNode __mks_769;
            das_zero(__mks_769);
            das_copy((__mks_769.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
            das_copy((__mks_769.id),(0));
            das_copy((__mks_769.at),(range(0,18)));
            das_copy((__mks_769.text),(nullptr));
            das_copy((__mks_769.textLen),(0));
            das_move((__mks_769.all),((([&]() -> TArray<regex::ReNode *> {
                TArray<regex::ReNode *> __mks_769;
                das_zero(__mks_769);
                return __mks_769;
            })())));
            das_copy((__mks_769.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_769;
                das_zero(__mks_769);
                das_copy((__mks_769.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                das_copy((__mks_769.id),(1));
                das_copy((__mks_769.at),(range(0,8)));
                das_copy((__mks_769.text),(((char *) "operator")));
                das_copy((__mks_769.textLen),(8));
                das_move((__mks_769.all),((([&]() -> TArray<regex::ReNode *> {
                    TArray<regex::ReNode *> __mks_769;
                    das_zero(__mks_769);
                    return __mks_769;
                })())));
                das_copy((__mks_769.left),(nullptr));
                das_copy((__mks_769.right),(nullptr));
                das_copy((__mks_769.subexpr),(nullptr));
                das_copy((__mks_769.cset),((([&]() -> TDim<uint32_t,8> {
                    TDim<uint32_t,8> __mka_769;
                    __mka_769(0,__context__) = 0x0u;
                    __mka_769(1,__context__) = 0x0u;
                    __mka_769(2,__context__) = 0x0u;
                    __mka_769(3,__context__) = 0x0u;
                    __mka_769(4,__context__) = 0x0u;
                    __mka_769(5,__context__) = 0x0u;
                    __mka_769(6,__context__) = 0x0u;
                    __mka_769(7,__context__) = 0x0u;
                    return __mka_769;
                })())));
                das_copy((__mks_769.index),(0));
                return __mks_769;
            })()))));
            das_copy((__mks_769.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_769;
                das_zero(__mks_769);
                das_copy((__mks_769.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                das_copy((__mks_769.id),(2));
                das_copy((__mks_769.at),(range(8,18)));
                das_copy((__mks_769.text),(nullptr));
                das_copy((__mks_769.textLen),(0));
                das_move((__mks_769.all),((([&]() -> TArray<regex::ReNode *> {
                    TArray<regex::ReNode *> __mks_769;
                    das_zero(__mks_769);
                    return __mks_769;
                })())));
                das_copy((__mks_769.left),(nullptr));
                das_copy((__mks_769.right),(nullptr));
                das_copy((__mks_769.subexpr),(nullptr));
                das_copy((__mks_769.cset),((([&]() -> TDim<uint32_t,8> {
                    TDim<uint32_t,8> __mka_769;
                    __mka_769(0,__context__) = 0xffffffffu;
                    __mka_769(1,__context__) = 0xffffffffu;
                    __mka_769(2,__context__) = 0x78000001u;
                    __mka_769(3,__context__) = 0xf8000001u;
                    __mka_769(4,__context__) = 0xffffffffu;
                    __mka_769(5,__context__) = 0xffffffffu;
                    __mka_769(6,__context__) = 0xffffffffu;
                    __mka_769(7,__context__) = 0xffffffffu;
                    return __mka_769;
                })())));
                das_copy((__mks_769.index),(0));
                return __mks_769;
            })()))));
            das_copy((__mks_769.subexpr),(nullptr));
            das_copy((__mks_769.cset),((([&]() -> TDim<uint32_t,8> {
                TDim<uint32_t,8> __mka_769;
                __mka_769(0,__context__) = 0x0u;
                __mka_769(1,__context__) = 0x0u;
                __mka_769(2,__context__) = 0x0u;
                __mka_769(3,__context__) = 0x0u;
                __mka_769(4,__context__) = 0x0u;
                __mka_769(5,__context__) = 0x0u;
                __mka_769(6,__context__) = 0x0u;
                __mka_769(7,__context__) = 0x0u;
                return __mka_769;
            })())));
            das_copy((__mks_769.index),(0));
            return __mks_769;
        })()))));
        das_move((_temp_make_local_769_16_32.groups),(_FuncbuiltinTickto_array_moveTick3185538323411982277_a9d6d54733762ba(__context__,das_arg<TDim<AutoTuple<range,char *>,1>>::pass((([&]() -> TDim<AutoTuple<range,char *>,1>& {
            _temp_make_local_769_16_160(0,__context__) = (([&]() -> AutoTuple<range,char *> {
                AutoTuple<range,char *> __mkt_769;
                das_get_auto_tuple_field<range,0,range,char *>::get(__mkt_769) = range(0,0);
                das_get_auto_tuple_field<char *,1,range,char *>::get(__mkt_769) = nullptr;
                return __mkt_769;
            })());
            return _temp_make_local_769_16_160;
        })())))));
        das_copy((_temp_make_local_769_16_32.earlyOut),((([&]() -> TDim<uint32_t,8> {
            TDim<uint32_t,8> __mka_769;
            __mka_769(0,__context__) = 0x0u;
            __mka_769(1,__context__) = 0x0u;
            __mka_769(2,__context__) = 0x0u;
            __mka_769(3,__context__) = 0x8000u;
            __mka_769(4,__context__) = 0x0u;
            __mka_769(5,__context__) = 0x0u;
            __mka_769(6,__context__) = 0x0u;
            __mka_769(7,__context__) = 0x0u;
            return __mka_769;
        })())));
        das_copy((_temp_make_local_769_16_32.canEarlyOut),(true));
        return _temp_make_local_769_16_32;
    })())));/*op_regex*/
    das_global<int32_t,0x89f4705947df9582>(__context__) = 0;/*g_method_name*/
    das_global<int32_t,0x1baec29b32162c17>(__context__) = 0;/*g_split_count*/
    das_global<int32_t,0x402eefb06d5e9cd0>(__context__) = 20;/*g_split_factor*/
    das_global<FILE const  *,0x77423da7a96350b8>(__context__) = nullptr;/*g_split_file*/
    das_global<regex::Regex,0x446bace93c1e6134>(__context__) = das_invoke_function<regex::Regex>::invoke_cmres<regex::Regex &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::regex_compile S<regex::Regex>*/ 0x235aa29958cfc4c4)),das_arg<regex::Regex>::pass((([&]() -> regex::Regex& {
        das_zero(_temp_make_local_944_16_32);
        das_copy((_temp_make_local_944_16_32.root),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
            regex::ReNode __mks_944;
            das_zero(__mks_944);
            das_copy((__mks_944.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
            das_copy((__mks_944.id),(0));
            das_copy((__mks_944.at),(range(0,15)));
            das_copy((__mks_944.text),(nullptr));
            das_copy((__mks_944.textLen),(0));
            das_move((__mks_944.all),((([&]() -> TArray<regex::ReNode *> {
                TArray<regex::ReNode *> __mks_944;
                das_zero(__mks_944);
                return __mks_944;
            })())));
            das_copy((__mks_944.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_944;
                das_zero(__mks_944);
                das_copy((__mks_944.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                das_copy((__mks_944.id),(1));
                das_copy((__mks_944.at),(range(0,13)));
                das_copy((__mks_944.text),(nullptr));
                das_copy((__mks_944.textLen),(0));
                das_move((__mks_944.all),((([&]() -> TArray<regex::ReNode *> {
                    TArray<regex::ReNode *> __mks_944;
                    das_zero(__mks_944);
                    return __mks_944;
                })())));
                das_copy((__mks_944.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_944;
                    das_zero(__mks_944);
                    das_copy((__mks_944.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                    das_copy((__mks_944.id),(2));
                    das_copy((__mks_944.at),(range(0,9)));
                    das_copy((__mks_944.text),(nullptr));
                    das_copy((__mks_944.textLen),(0));
                    das_move((__mks_944.all),((([&]() -> TArray<regex::ReNode *> {
                        TArray<regex::ReNode *> __mks_944;
                        das_zero(__mks_944);
                        return __mks_944;
                    })())));
                    das_copy((__mks_944.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_944;
                        das_zero(__mks_944);
                        das_copy((__mks_944.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                        das_copy((__mks_944.id),(3));
                        das_copy((__mks_944.at),(range(0,7)));
                        das_copy((__mks_944.text),(nullptr));
                        das_copy((__mks_944.textLen),(0));
                        das_move((__mks_944.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_944;
                            das_zero(__mks_944);
                            return __mks_944;
                        })())));
                        das_copy((__mks_944.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                            regex::ReNode __mks_944;
                            das_zero(__mks_944);
                            das_copy((__mks_944.op),(DAS_COMMENT(enum) regex::ReOp::Group));
                            das_copy((__mks_944.id),(4));
                            das_copy((__mks_944.at),(range(0,4)));
                            das_copy((__mks_944.text),(nullptr));
                            das_copy((__mks_944.textLen),(0));
                            das_move((__mks_944.all),((([&]() -> TArray<regex::ReNode *> {
                                TArray<regex::ReNode *> __mks_944;
                                das_zero(__mks_944);
                                return __mks_944;
                            })())));
                            das_copy((__mks_944.left),(nullptr));
                            das_copy((__mks_944.right),(nullptr));
                            das_copy((__mks_944.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                regex::ReNode __mks_944;
                                das_zero(__mks_944);
                                das_copy((__mks_944.op),(DAS_COMMENT(enum) regex::ReOp::Plus));
                                das_copy((__mks_944.id),(5));
                                das_copy((__mks_944.at),(range(1,3)));
                                das_copy((__mks_944.text),(nullptr));
                                das_copy((__mks_944.textLen),(0));
                                das_move((__mks_944.all),((([&]() -> TArray<regex::ReNode *> {
                                    TArray<regex::ReNode *> __mks_944;
                                    das_zero(__mks_944);
                                    return __mks_944;
                                })())));
                                das_copy((__mks_944.left),(nullptr));
                                das_copy((__mks_944.right),(nullptr));
                                das_copy((__mks_944.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                    regex::ReNode __mks_944;
                                    das_zero(__mks_944);
                                    das_copy((__mks_944.op),(DAS_COMMENT(enum) regex::ReOp::Any));
                                    das_copy((__mks_944.id),(6));
                                    das_copy((__mks_944.at),(range(1,2)));
                                    das_copy((__mks_944.text),(nullptr));
                                    das_copy((__mks_944.textLen),(0));
                                    das_move((__mks_944.all),((([&]() -> TArray<regex::ReNode *> {
                                        TArray<regex::ReNode *> __mks_944;
                                        das_zero(__mks_944);
                                        return __mks_944;
                                    })())));
                                    das_copy((__mks_944.left),(nullptr));
                                    das_copy((__mks_944.right),(nullptr));
                                    das_copy((__mks_944.subexpr),(nullptr));
                                    das_copy((__mks_944.cset),((([&]() -> TDim<uint32_t,8> {
                                        TDim<uint32_t,8> __mka_944;
                                        __mka_944(0,__context__) = 0x0u;
                                        __mka_944(1,__context__) = 0x0u;
                                        __mka_944(2,__context__) = 0x0u;
                                        __mka_944(3,__context__) = 0x0u;
                                        __mka_944(4,__context__) = 0x0u;
                                        __mka_944(5,__context__) = 0x0u;
                                        __mka_944(6,__context__) = 0x0u;
                                        __mka_944(7,__context__) = 0x0u;
                                        return __mka_944;
                                    })())));
                                    das_copy((__mks_944.index),(0));
                                    return __mks_944;
                                })()))));
                                das_copy((__mks_944.cset),((([&]() -> TDim<uint32_t,8> {
                                    TDim<uint32_t,8> __mka_944;
                                    __mka_944(0,__context__) = 0x0u;
                                    __mka_944(1,__context__) = 0x0u;
                                    __mka_944(2,__context__) = 0x0u;
                                    __mka_944(3,__context__) = 0x0u;
                                    __mka_944(4,__context__) = 0x0u;
                                    __mka_944(5,__context__) = 0x0u;
                                    __mka_944(6,__context__) = 0x0u;
                                    __mka_944(7,__context__) = 0x0u;
                                    return __mka_944;
                                })())));
                                das_copy((__mks_944.index),(0));
                                return __mks_944;
                            })()))));
                            das_copy((__mks_944.cset),((([&]() -> TDim<uint32_t,8> {
                                TDim<uint32_t,8> __mka_944;
                                __mka_944(0,__context__) = 0x0u;
                                __mka_944(1,__context__) = 0x0u;
                                __mka_944(2,__context__) = 0x0u;
                                __mka_944(3,__context__) = 0x0u;
                                __mka_944(4,__context__) = 0x0u;
                                __mka_944(5,__context__) = 0x0u;
                                __mka_944(6,__context__) = 0x0u;
                                __mka_944(7,__context__) = 0x0u;
                                return __mka_944;
                            })())));
                            das_copy((__mks_944.index),(1));
                            return __mks_944;
                        })()))));
                        das_copy((__mks_944.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                            regex::ReNode __mks_944;
                            das_zero(__mks_944);
                            das_copy((__mks_944.op),(DAS_COMMENT(enum) regex::ReOp::Star));
                            das_copy((__mks_944.id),(7));
                            das_copy((__mks_944.at),(range(4,7)));
                            das_copy((__mks_944.text),(nullptr));
                            das_copy((__mks_944.textLen),(0));
                            das_move((__mks_944.all),((([&]() -> TArray<regex::ReNode *> {
                                TArray<regex::ReNode *> __mks_944;
                                das_zero(__mks_944);
                                return __mks_944;
                            })())));
                            das_copy((__mks_944.left),(nullptr));
                            das_copy((__mks_944.right),(nullptr));
                            das_copy((__mks_944.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                regex::ReNode __mks_944;
                                das_zero(__mks_944);
                                das_copy((__mks_944.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                                das_copy((__mks_944.id),(8));
                                das_copy((__mks_944.at),(range(4,6)));
                                das_copy((__mks_944.text),(nullptr));
                                das_copy((__mks_944.textLen),(0));
                                das_move((__mks_944.all),((([&]() -> TArray<regex::ReNode *> {
                                    TArray<regex::ReNode *> __mks_944;
                                    das_zero(__mks_944);
                                    return __mks_944;
                                })())));
                                das_copy((__mks_944.left),(nullptr));
                                das_copy((__mks_944.right),(nullptr));
                                das_copy((__mks_944.subexpr),(nullptr));
                                das_copy((__mks_944.cset),((([&]() -> TDim<uint32_t,8> {
                                    TDim<uint32_t,8> __mka_944;
                                    __mka_944(0,__context__) = 0x200u;
                                    __mka_944(1,__context__) = 0x1u;
                                    __mka_944(2,__context__) = 0x0u;
                                    __mka_944(3,__context__) = 0x0u;
                                    __mka_944(4,__context__) = 0x0u;
                                    __mka_944(5,__context__) = 0x0u;
                                    __mka_944(6,__context__) = 0x0u;
                                    __mka_944(7,__context__) = 0x0u;
                                    return __mka_944;
                                })())));
                                das_copy((__mks_944.index),(0));
                                return __mks_944;
                            })()))));
                            das_copy((__mks_944.cset),((([&]() -> TDim<uint32_t,8> {
                                TDim<uint32_t,8> __mka_944;
                                __mka_944(0,__context__) = 0x0u;
                                __mka_944(1,__context__) = 0x0u;
                                __mka_944(2,__context__) = 0x0u;
                                __mka_944(3,__context__) = 0x0u;
                                __mka_944(4,__context__) = 0x0u;
                                __mka_944(5,__context__) = 0x0u;
                                __mka_944(6,__context__) = 0x0u;
                                __mka_944(7,__context__) = 0x0u;
                                return __mka_944;
                            })())));
                            das_copy((__mks_944.index),(0));
                            return __mks_944;
                        })()))));
                        das_copy((__mks_944.subexpr),(nullptr));
                        das_copy((__mks_944.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_944;
                            __mka_944(0,__context__) = 0x0u;
                            __mka_944(1,__context__) = 0x0u;
                            __mka_944(2,__context__) = 0x0u;
                            __mka_944(3,__context__) = 0x0u;
                            __mka_944(4,__context__) = 0x0u;
                            __mka_944(5,__context__) = 0x0u;
                            __mka_944(6,__context__) = 0x0u;
                            __mka_944(7,__context__) = 0x0u;
                            return __mka_944;
                        })())));
                        das_copy((__mks_944.index),(0));
                        return __mks_944;
                    })()))));
                    das_copy((__mks_944.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_944;
                        das_zero(__mks_944);
                        das_copy((__mks_944.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                        das_copy((__mks_944.id),(9));
                        das_copy((__mks_944.at),(range(7,9)));
                        das_copy((__mks_944.text),(((char *) "(")));
                        das_copy((__mks_944.textLen),(1));
                        das_move((__mks_944.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_944;
                            das_zero(__mks_944);
                            return __mks_944;
                        })())));
                        das_copy((__mks_944.left),(nullptr));
                        das_copy((__mks_944.right),(nullptr));
                        das_copy((__mks_944.subexpr),(nullptr));
                        das_copy((__mks_944.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_944;
                            __mka_944(0,__context__) = 0x0u;
                            __mka_944(1,__context__) = 0x0u;
                            __mka_944(2,__context__) = 0x0u;
                            __mka_944(3,__context__) = 0x0u;
                            __mka_944(4,__context__) = 0x0u;
                            __mka_944(5,__context__) = 0x0u;
                            __mka_944(6,__context__) = 0x0u;
                            __mka_944(7,__context__) = 0x0u;
                            return __mka_944;
                        })())));
                        das_copy((__mks_944.index),(0));
                        return __mks_944;
                    })()))));
                    das_copy((__mks_944.subexpr),(nullptr));
                    das_copy((__mks_944.cset),((([&]() -> TDim<uint32_t,8> {
                        TDim<uint32_t,8> __mka_944;
                        __mka_944(0,__context__) = 0x0u;
                        __mka_944(1,__context__) = 0x0u;
                        __mka_944(2,__context__) = 0x0u;
                        __mka_944(3,__context__) = 0x0u;
                        __mka_944(4,__context__) = 0x0u;
                        __mka_944(5,__context__) = 0x0u;
                        __mka_944(6,__context__) = 0x0u;
                        __mka_944(7,__context__) = 0x0u;
                        return __mka_944;
                    })())));
                    das_copy((__mks_944.index),(0));
                    return __mks_944;
                })()))));
                das_copy((__mks_944.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_944;
                    das_zero(__mks_944);
                    das_copy((__mks_944.op),(DAS_COMMENT(enum) regex::ReOp::Group));
                    das_copy((__mks_944.id),(10));
                    das_copy((__mks_944.at),(range(9,13)));
                    das_copy((__mks_944.text),(nullptr));
                    das_copy((__mks_944.textLen),(0));
                    das_move((__mks_944.all),((([&]() -> TArray<regex::ReNode *> {
                        TArray<regex::ReNode *> __mks_944;
                        das_zero(__mks_944);
                        return __mks_944;
                    })())));
                    das_copy((__mks_944.left),(nullptr));
                    das_copy((__mks_944.right),(nullptr));
                    das_copy((__mks_944.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_944;
                        das_zero(__mks_944);
                        das_copy((__mks_944.op),(DAS_COMMENT(enum) regex::ReOp::Plus));
                        das_copy((__mks_944.id),(11));
                        das_copy((__mks_944.at),(range(10,12)));
                        das_copy((__mks_944.text),(nullptr));
                        das_copy((__mks_944.textLen),(0));
                        das_move((__mks_944.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_944;
                            das_zero(__mks_944);
                            return __mks_944;
                        })())));
                        das_copy((__mks_944.left),(nullptr));
                        das_copy((__mks_944.right),(nullptr));
                        das_copy((__mks_944.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                            regex::ReNode __mks_944;
                            das_zero(__mks_944);
                            das_copy((__mks_944.op),(DAS_COMMENT(enum) regex::ReOp::Any));
                            das_copy((__mks_944.id),(12));
                            das_copy((__mks_944.at),(range(10,11)));
                            das_copy((__mks_944.text),(nullptr));
                            das_copy((__mks_944.textLen),(0));
                            das_move((__mks_944.all),((([&]() -> TArray<regex::ReNode *> {
                                TArray<regex::ReNode *> __mks_944;
                                das_zero(__mks_944);
                                return __mks_944;
                            })())));
                            das_copy((__mks_944.left),(nullptr));
                            das_copy((__mks_944.right),(nullptr));
                            das_copy((__mks_944.subexpr),(nullptr));
                            das_copy((__mks_944.cset),((([&]() -> TDim<uint32_t,8> {
                                TDim<uint32_t,8> __mka_944;
                                __mka_944(0,__context__) = 0x0u;
                                __mka_944(1,__context__) = 0x0u;
                                __mka_944(2,__context__) = 0x0u;
                                __mka_944(3,__context__) = 0x0u;
                                __mka_944(4,__context__) = 0x0u;
                                __mka_944(5,__context__) = 0x0u;
                                __mka_944(6,__context__) = 0x0u;
                                __mka_944(7,__context__) = 0x0u;
                                return __mka_944;
                            })())));
                            das_copy((__mks_944.index),(0));
                            return __mks_944;
                        })()))));
                        das_copy((__mks_944.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_944;
                            __mka_944(0,__context__) = 0x0u;
                            __mka_944(1,__context__) = 0x0u;
                            __mka_944(2,__context__) = 0x0u;
                            __mka_944(3,__context__) = 0x0u;
                            __mka_944(4,__context__) = 0x0u;
                            __mka_944(5,__context__) = 0x0u;
                            __mka_944(6,__context__) = 0x0u;
                            __mka_944(7,__context__) = 0x0u;
                            return __mka_944;
                        })())));
                        das_copy((__mks_944.index),(0));
                        return __mks_944;
                    })()))));
                    das_copy((__mks_944.cset),((([&]() -> TDim<uint32_t,8> {
                        TDim<uint32_t,8> __mka_944;
                        __mka_944(0,__context__) = 0x0u;
                        __mka_944(1,__context__) = 0x0u;
                        __mka_944(2,__context__) = 0x0u;
                        __mka_944(3,__context__) = 0x0u;
                        __mka_944(4,__context__) = 0x0u;
                        __mka_944(5,__context__) = 0x0u;
                        __mka_944(6,__context__) = 0x0u;
                        __mka_944(7,__context__) = 0x0u;
                        return __mka_944;
                    })())));
                    das_copy((__mks_944.index),(2));
                    return __mks_944;
                })()))));
                das_copy((__mks_944.subexpr),(nullptr));
                das_copy((__mks_944.cset),((([&]() -> TDim<uint32_t,8> {
                    TDim<uint32_t,8> __mka_944;
                    __mka_944(0,__context__) = 0x0u;
                    __mka_944(1,__context__) = 0x0u;
                    __mka_944(2,__context__) = 0x0u;
                    __mka_944(3,__context__) = 0x0u;
                    __mka_944(4,__context__) = 0x0u;
                    __mka_944(5,__context__) = 0x0u;
                    __mka_944(6,__context__) = 0x0u;
                    __mka_944(7,__context__) = 0x0u;
                    return __mka_944;
                })())));
                das_copy((__mks_944.index),(0));
                return __mks_944;
            })()))));
            das_copy((__mks_944.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_944;
                das_zero(__mks_944);
                das_copy((__mks_944.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                das_copy((__mks_944.id),(13));
                das_copy((__mks_944.at),(range(13,15)));
                das_copy((__mks_944.text),(((char *) ")")));
                das_copy((__mks_944.textLen),(1));
                das_move((__mks_944.all),((([&]() -> TArray<regex::ReNode *> {
                    TArray<regex::ReNode *> __mks_944;
                    das_zero(__mks_944);
                    return __mks_944;
                })())));
                das_copy((__mks_944.left),(nullptr));
                das_copy((__mks_944.right),(nullptr));
                das_copy((__mks_944.subexpr),(nullptr));
                das_copy((__mks_944.cset),((([&]() -> TDim<uint32_t,8> {
                    TDim<uint32_t,8> __mka_944;
                    __mka_944(0,__context__) = 0x0u;
                    __mka_944(1,__context__) = 0x0u;
                    __mka_944(2,__context__) = 0x0u;
                    __mka_944(3,__context__) = 0x0u;
                    __mka_944(4,__context__) = 0x0u;
                    __mka_944(5,__context__) = 0x0u;
                    __mka_944(6,__context__) = 0x0u;
                    __mka_944(7,__context__) = 0x0u;
                    return __mka_944;
                })())));
                das_copy((__mks_944.index),(0));
                return __mks_944;
            })()))));
            das_copy((__mks_944.subexpr),(nullptr));
            das_copy((__mks_944.cset),((([&]() -> TDim<uint32_t,8> {
                TDim<uint32_t,8> __mka_944;
                __mka_944(0,__context__) = 0x0u;
                __mka_944(1,__context__) = 0x0u;
                __mka_944(2,__context__) = 0x0u;
                __mka_944(3,__context__) = 0x0u;
                __mka_944(4,__context__) = 0x0u;
                __mka_944(5,__context__) = 0x0u;
                __mka_944(6,__context__) = 0x0u;
                __mka_944(7,__context__) = 0x0u;
                return __mka_944;
            })())));
            das_copy((__mks_944.index),(0));
            return __mks_944;
        })()))));
        das_move((_temp_make_local_944_16_32.groups),(_FuncbuiltinTickto_array_moveTick3185538323411982277_71fbdf75ac58194(__context__,das_arg<TDim<AutoTuple<range,char *>,3>>::pass((([&]() -> TDim<AutoTuple<range,char *>,3>& {
            _temp_make_local_944_16_336(0,__context__) = (([&]() -> AutoTuple<range,char *> {
                AutoTuple<range,char *> __mkt_944;
                das_get_auto_tuple_field<range,0,range,char *>::get(__mkt_944) = range(0,0);
                das_get_auto_tuple_field<char *,1,range,char *>::get(__mkt_944) = nullptr;
                return __mkt_944;
            })());
            _temp_make_local_944_16_336(1,__context__) = (([&]() -> AutoTuple<range,char *> {
                AutoTuple<range,char *> __mkt_944;
                das_get_auto_tuple_field<range,0,range,char *>::get(__mkt_944) = range(0,0);
                das_get_auto_tuple_field<char *,1,range,char *>::get(__mkt_944) = ((char *) "1");
                return __mkt_944;
            })());
            _temp_make_local_944_16_336(2,__context__) = (([&]() -> AutoTuple<range,char *> {
                AutoTuple<range,char *> __mkt_944;
                das_get_auto_tuple_field<range,0,range,char *>::get(__mkt_944) = range(0,0);
                das_get_auto_tuple_field<char *,1,range,char *>::get(__mkt_944) = ((char *) "2");
                return __mkt_944;
            })());
            return _temp_make_local_944_16_336;
        })())))));
        das_copy((_temp_make_local_944_16_32.earlyOut),((([&]() -> TDim<uint32_t,8> {
            TDim<uint32_t,8> __mka_944;
            __mka_944(0,__context__) = 0xffffffffu;
            __mka_944(1,__context__) = 0xffffffffu;
            __mka_944(2,__context__) = 0xffffffffu;
            __mka_944(3,__context__) = 0xffffffffu;
            __mka_944(4,__context__) = 0xffffffffu;
            __mka_944(5,__context__) = 0xffffffffu;
            __mka_944(6,__context__) = 0xffffffffu;
            __mka_944(7,__context__) = 0xffffffffu;
            return __mka_944;
        })())));
        das_copy((_temp_make_local_944_16_32.canEarlyOut),(true));
        return _temp_make_local_944_16_32;
    })())));/*reg_args*/
    das_global_zero<Lambda DAS_COMMENT((void,cpp_gen::ClangFunc const )),0x4f69680403ea7c7>(__context__);/*gen_fn_callback*/
}

inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_c777430705b7c258 ( Context * __context__, cpp_gen::ClangStruct &  __a_rename_at_32_0, cpp_gen::ClangStruct &  __b_rename_at_32_1 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_0,cast<cpp_gen::ClangStruct &>::from(__a_rename_at_32_0)));
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_1,cast<cpp_gen::ClangStruct &>::from(__b_rename_at_32_1)));
    das_move(__a_rename_at_32_0,__b_rename_at_32_1);
}

inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_e79b4e2307f7ae1c ( Context * __context__, cpp_gen::ClangEnum &  __a_rename_at_32_2, cpp_gen::ClangEnum &  __b_rename_at_32_3 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_2,cast<cpp_gen::ClangEnum &>::from(__a_rename_at_32_2)));
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_3,cast<cpp_gen::ClangEnum &>::from(__b_rename_at_32_3)));
    das_move(__a_rename_at_32_2,__b_rename_at_32_3);
}

inline AutoTuple<char *,char *> _Funccpp_genTickonNumericLiteralTick10275515307319155835_73739de28d614e90 ( Context * __context__, TTable<char *,json::JsonValue *> &  __inner_obj_rename_at_337_4 )
{
    char * __value_rename_at_338_5 = (char *)(nullptr);
    char * __valueType_rename_at_339_6 = (char *)(nullptr);
    char * __valueCategory_rename_at_340_7 = ((char *)(char *)(das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__inner_obj_rename_at_337_4(((char *) "valueCategory"),__context__)->value,__context__)));
    if ( SimPolicy<char *>::Equ(cast<char *>::from(__valueCategory_rename_at_340_7),cast<char *>::from(((char *) "rvalue")),*__context__,nullptr) )
    {
        das_copy(__value_rename_at_338_5,das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__inner_obj_rename_at_337_4(((char *) "value"),__context__)->value,__context__));
    };
    das_copy(__valueType_rename_at_339_6,_Funccpp_genTickgetQualTypeTick1400640308412000890_a58540215c7d0fa9(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__inner_obj_rename_at_337_4),((char *) "type")));
    return das_auto_cast_ref<AutoTuple<char *,char *>>::cast((([&]() -> AutoTuple<char *,char *> {
        AutoTuple<char *,char *> __mkt_345;
        das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_345) = __value_rename_at_338_5;
        das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_345) = __valueType_rename_at_339_6;
        return __mkt_345;
    })()));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_5a77afdd4cbd09e1 ( Context * __context__, TArray<char *> &  __Arr_rename_at_181_8, char * __value_rename_at_181_9 )
{
    das_copy(__Arr_rename_at_181_8(builtin_array_push_back(das_arg<TArray<char *>>::pass(__Arr_rename_at_181_8),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_9);
}

inline cpp_gen::ClangEnum & _FuncbuiltinTick_at_with_lockcheckTick7807051423786862253_68d1e221d9f9bb60 ( Context * __context__, TTable<char *,cpp_gen::ClangEnum> &  __Tab_rename_at_871_10, char * const  __at_rename_at_871_11 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_4,cast<TTable<char *,cpp_gen::ClangEnum> &>::from(__Tab_rename_at_871_10)));
    return das_auto_cast_ref<cpp_gen::ClangEnum &>::cast(__Tab_rename_at_871_10(__at_rename_at_871_11,__context__));
}

inline void _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af ( Context * __context__, TArray<char *> &  __Arr_rename_at_165_12, char * const  __value_rename_at_165_13 )
{
    das_copy(__Arr_rename_at_165_12(builtin_array_push_back(das_arg<TArray<char *>>::pass(__Arr_rename_at_165_12),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_165_13);
}

inline void finalize_8a84dcb6bb73936b ( Context * __context__, cpp_gen::_lambda_cpp_gen_543_2 &  ____this_rename_at_543_14 )
{
    builtin_iterator_delete(das_arg<Sequence DAS_COMMENT((cpp_gen::Scope))>::pass(____this_rename_at_543_14._source_0_at_543_30),__context__);
    memset((void*)&(____this_rename_at_543_14), 0, TypeSize<cpp_gen::_lambda_cpp_gen_543_2>::size);
}

inline AutoTuple<char *,char *> _Funccpp_genTickonInitArgTick7456162652026714814_980c8ded9aae7f9d ( Context * __context__, TTable<char *,json::JsonValue *> &  __root_rename_at_348_15 ) { das_stack_prologue __prologue(__context__,448,"cpp_gen`onInitArg`7456162652026714814 " DAS_FILE_LINE);
{
    char * __value_rename_at_349_16 = (char *)(nullptr);
    char * __valueType_rename_at_350_17 = (char *)(nullptr);
    if ( _FuncbuiltinTickkey_existsTick16808803843923989214_963e1e05a7a36522(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__root_rename_at_348_15),((char *) "inner")) )
    {
        _Funccpp_genTickwith_arrayTick6272002616190333845_c90f8a054aaf2ef6(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__root_rename_at_348_15),((char *) "inner"),das_make_block<void,TArray<json::JsonValue *> &>(__context__,112,0,&__func_info__7ec127c5491418ed,[&](TArray<json::JsonValue *> & __inner_rename_at_352_18) -> void{
            TTable<char *,json::JsonValue *> * __inner_obj_rename_at_353_19 = &(das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__inner_rename_at_352_18(0,__context__)->value,__context__));
            char * __kind_rename_at_354_20 = ((char *)(char *)(das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as((*__inner_obj_rename_at_353_19)(((char *) "kind"),__context__)->value,__context__)));
            if ( SimPolicy<char *>::Equ(cast<char *>::from(__kind_rename_at_354_20),cast<char *>::from(((char *) "ImplicitCastExpr")),*__context__,nullptr) )
            {
                char * __castKind_rename_at_356_21 = ((char *)(char *)(das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as((*__inner_obj_rename_at_353_19)(((char *) "castKind"),__context__)->value,__context__)));
                if ( SimPolicy<char *>::Equ(cast<char *>::from(__castKind_rename_at_356_21),cast<char *>::from(((char *) "NullToPointer")),*__context__,nullptr) )
                {
                    das_copy(__value_rename_at_349_16,((char *) "nullptr"));
                } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__castKind_rename_at_356_21),cast<char *>::from(((char *) "ArrayToPointerDecay")),*__context__,nullptr) )
                {
                    char * __valueCategory_rename_at_360_22 = ((char *)(char *)(das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as((*__inner_obj_rename_at_353_19)(((char *) "valueCategory"),__context__)->value,__context__)));
                    das_copy(__valueType_rename_at_350_17,_Funccpp_genTickgetQualTypeTick1400640308412000890_a58540215c7d0fa9(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass((*__inner_obj_rename_at_353_19)),((char *) "type")));
                    if ( SimPolicy<char *>::Equ(cast<char *>::from(__valueCategory_rename_at_360_22),cast<char *>::from(((char *) "rvalue")),*__context__,nullptr) )
                    {
                        if ( SimPolicy<char *>::Equ(cast<char *>::from(__valueType_rename_at_350_17),cast<char *>::from(((char *) "const char *")),*__context__,nullptr) )
                        {
                            _Funccpp_genTickwith_arrayTick6272002616190333845_c90f8a054aaf2ef6(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass((*__inner_obj_rename_at_353_19)),((char *) "inner"),das_make_block<void,TArray<json::JsonValue *> &>(__context__,240,0,&__func_info__62423d4f8a0d8cfa,[&](TArray<json::JsonValue *> & __iarr_rename_at_364_23) -> void{
                                TTable<char *,json::JsonValue *> * __iobj_rename_at_365_24 = &(das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__iarr_rename_at_364_23(0,__context__)->value,__context__));
                                das_copy(__value_rename_at_349_16,das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as((*__iobj_rename_at_365_24)(((char *) "value"),__context__)->value,__context__));
                            }));
                        } else {
                            builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_5, cast<char * const >::from(((char *) "warning: unsupported decay type ")), cast<char *>::from(__valueType_rename_at_350_17))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                        };
                    };
                } else {
                    builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_6, cast<char * const >::from(((char *) "warning: unsupproted cast kind ")), cast<char * const >::from(__castKind_rename_at_356_21), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                };
            } else if ( (SimPolicy<char *>::Equ(cast<char *>::from(__kind_rename_at_354_20),cast<char *>::from(((char *) "IntegerLiteral")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__kind_rename_at_354_20),cast<char *>::from(((char *) "FloatingLiteral")),*__context__,nullptr)) )
            {
                AutoTuple<char *,char *> __literal_rename_at_376_25_ConstRef = ((AutoTuple<char *,char *>)_Funccpp_genTickonNumericLiteralTick10275515307319155835_73739de28d614e90(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass((*__inner_obj_rename_at_353_19))));
    AutoTuple<char *,char *> const  & __literal_rename_at_376_25 = __literal_rename_at_376_25_ConstRef; ;
                das_copy(__value_rename_at_349_16,das_get_auto_tuple_field<char *,0,char *,char *>::get(__literal_rename_at_376_25));
                das_copy(__valueType_rename_at_350_17,das_get_auto_tuple_field<char *,1,char *,char *>::get(__literal_rename_at_376_25));
            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__kind_rename_at_354_20),cast<char *>::from(((char *) "UnaryOperator")),*__context__,nullptr) )
            {
                char * __opCode_rename_at_380_26 = ((char *)(char *)(das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as((*__inner_obj_rename_at_353_19)(((char *) "opcode"),__context__)->value,__context__)));
                _Funccpp_genTickwith_arrayTick6272002616190333845_c90f8a054aaf2ef6(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass((*__inner_obj_rename_at_353_19)),((char *) "inner"),das_make_block<void,TArray<json::JsonValue *> &>(__context__,352,0,&__func_info__b2194afbcdafdd23,[&](TArray<json::JsonValue *> & __iinner_rename_at_381_27) -> void{
                    TTable<char *,json::JsonValue *> * __iinner_obj_rename_at_382_28 = &(das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__iinner_rename_at_381_27(0,__context__)->value,__context__));
                    char * __ikind_rename_at_383_29 = ((char *)(char *)(das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as((*__iinner_obj_rename_at_382_28)(((char *) "kind"),__context__)->value,__context__)));
                    if ( (SimPolicy<char *>::Equ(cast<char *>::from(__ikind_rename_at_383_29),cast<char *>::from(((char *) "IntegerLiteral")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ikind_rename_at_383_29),cast<char *>::from(((char *) "FloatingLiteral")),*__context__,nullptr)) )
                    {
                        AutoTuple<char *,char *> __literal_rename_at_385_30_ConstRef = ((AutoTuple<char *,char *>)_Funccpp_genTickonNumericLiteralTick10275515307319155835_73739de28d614e90(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass((*__iinner_obj_rename_at_382_28))));
    AutoTuple<char *,char *> const  & __literal_rename_at_385_30 = __literal_rename_at_385_30_ConstRef; ;
                        das_copy(__value_rename_at_349_16,cast<char *>::to(SimPolicy<char *>::Add(cast<char *>::from(__opCode_rename_at_380_26),cast<char *>::from(das_get_auto_tuple_field<char *,0,char *,char *>::get(__literal_rename_at_385_30)),*__context__,nullptr)));
                        das_copy(__valueType_rename_at_350_17,das_get_auto_tuple_field<char *,1,char *,char *>::get(__literal_rename_at_385_30));
                    };
                }));
            } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__kind_rename_at_354_20),cast<char *>::from(((char *) "CXXBoolLiteralExpr")),*__context__,nullptr) )
            {
                char * __valueCategory_rename_at_391_31 = ((char *)(char *)(das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as((*__inner_obj_rename_at_353_19)(((char *) "valueCategory"),__context__)->value,__context__)));
                if ( SimPolicy<char *>::Equ(cast<char *>::from(__valueCategory_rename_at_391_31),cast<char *>::from(((char *) "rvalue")),*__context__,nullptr) )
                {
                    das_copy(__value_rename_at_349_16,das_get_auto_variant_field<bool,4,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as((*__inner_obj_rename_at_353_19)(((char *) "value"),__context__)->value,__context__) ? das_auto_cast<char * const >::cast(((char *) "true")) : das_auto_cast<char * const >::cast(((char *) "false")));
                };
                das_copy(__valueType_rename_at_350_17,_Funccpp_genTickgetQualTypeTick1400640308412000890_a58540215c7d0fa9(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass((*__inner_obj_rename_at_353_19)),((char *) "type")));
            } else {
                builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_7, cast<char * const >::from(((char *) "warning: unsupproted init kind ")), cast<char * const >::from(__kind_rename_at_354_20), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            };
        }));
    };
    return das_auto_cast_ref<AutoTuple<char *,char *>>::cast((([&]() -> AutoTuple<char *,char *> {
        AutoTuple<char *,char *> __mkt_401;
        das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_401) = __value_rename_at_349_16;
        das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_401) = __valueType_rename_at_350_17;
        return __mkt_401;
    })()));
}}

inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_cfad5be86fc7ec48 ( Context * __context__, cpp_gen::ClangFunc &  __a_rename_at_32_32, cpp_gen::ClangFunc &  __b_rename_at_32_33 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_8,cast<cpp_gen::ClangFunc &>::from(__a_rename_at_32_32)));
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_9,cast<cpp_gen::ClangFunc &>::from(__b_rename_at_32_33)));
    das_move(__a_rename_at_32_32,__b_rename_at_32_33);
}

inline void _FuncbuiltinTickemplaceTick13923705686753630697_b7c430019eafb474 ( Context * __context__, TArray<cpp_gen::ClangFunc> &  __Arr_rename_at_287_34, cpp_gen::ClangFunc &  __value_rename_at_287_35 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_10,cast<TArray<cpp_gen::ClangFunc> &>::from(__Arr_rename_at_287_34)));
    das_move(__Arr_rename_at_287_34(builtin_array_push_back(das_arg<TArray<cpp_gen::ClangFunc>>::pass(__Arr_rename_at_287_34),72,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_287_35);
}

inline bool _Func_lambda_cpp_gen_543_2Tickfunction_10be92d9c230e7ac ( Context * __context__, cpp_gen::_lambda_cpp_gen_543_2 &  ____this_rename_at_543_36, char * * & ___ryield543_rename_at_543_37 )
{
    switch (____this_rename_at_543_36.__yield) {
    case 0: goto label_0;
    case 2: goto label_2;
    case 1: goto label_1;
    case 3: goto label_3;
    case 4: goto label_4;
    default: __context__->throw_error("invalid label");
    };
    label_0:;;
    das_copy(____this_rename_at_543_36._loop_at_543_30,true);
    das_move(____this_rename_at_543_36._source_0_at_543_30,_FuncbuiltinTickeachTick6002865651812066953_ab768a47fa5edfe2(__context__,das_arg<TArray<cpp_gen::Scope>>::pass(das_global<TArray<cpp_gen::Scope>,0x82cea5b26477762c>(__context__) /*name_stack*/)));
    memset((void*)&(____this_rename_at_543_36.__n_rename_at_543_34), 0, TypeSize<cpp_gen::Scope *>::size);
    das_copy(____this_rename_at_543_36._pvar_0_at_543_30,das_cast<void *>::cast(das_ref(__context__,____this_rename_at_543_36.__n_rename_at_543_34)));
    DAS_SETBOOLAND((____this_rename_at_543_36._loop_at_543_30),(builtin_iterator_first(das_arg<Sequence DAS_COMMENT((cpp_gen::Scope))>::pass(____this_rename_at_543_36._source_0_at_543_30),____this_rename_at_543_36._pvar_0_at_543_30,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))));
    label_2:;;
    if ( !____this_rename_at_543_36._loop_at_543_30 )
    {
        goto label_4;
    };
    das_copy(___ryield543_rename_at_543_37,das_ref(__context__,das_deref(__context__,____this_rename_at_543_36.__n_rename_at_543_34).name));
    das_copy(____this_rename_at_543_36.__yield,1);
    return das_auto_cast<bool>::cast(true);
    label_1:;;
    label_3:;;
    DAS_SETBOOLAND((____this_rename_at_543_36._loop_at_543_30),(builtin_iterator_next(das_arg<Sequence DAS_COMMENT((cpp_gen::Scope))>::pass(____this_rename_at_543_36._source_0_at_543_30),____this_rename_at_543_36._pvar_0_at_543_30,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))));
    goto label_2;
    label_4:;;
    builtin_iterator_close(das_arg<Sequence DAS_COMMENT((cpp_gen::Scope))>::pass(____this_rename_at_543_36._source_0_at_543_30),____this_rename_at_543_36._pvar_0_at_543_30,__context__);
    return das_auto_cast<bool>::cast(false);
}

inline void _Func_lambda_cpp_gen_543_2Tickfinalizer_67e83694801db280 ( Context * __context__, cpp_gen::_lambda_cpp_gen_543_2 * ____this_rename_at_543_38 )
{
    finalize_8a84dcb6bb73936b(__context__,das_arg<cpp_gen::_lambda_cpp_gen_543_2>::pass(das_deref(__context__,____this_rename_at_543_38)));
    das_delete_lambda_struct<cpp_gen::_lambda_cpp_gen_543_2 *>::clear(__context__,____this_rename_at_543_38);
}

inline Sequence DAS_COMMENT((cpp_gen::Scope &)) _FuncbuiltinTickeachTick6002865651812066953_ab768a47fa5edfe2 ( Context * __context__, TArray<cpp_gen::Scope> const  &  __a_rename_at_1273_39 )
{
    Sequence DAS_COMMENT((cpp_gen::Scope *)) __it_rename_at_1274_40;das_zero(__it_rename_at_1274_40);
    builtin_make_good_array_iterator(das_arg<Sequence DAS_COMMENT((cpp_gen::Scope))>::pass(__it_rename_at_1274_40),__a_rename_at_1273_39,16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((cpp_gen::Scope &))>::cast(__it_rename_at_1274_40);
}

inline char * _Funcstrings_boostTickjoinTick17792841289284275598_f97bd9e0a3ac85c9 ( Context * __context__, Sequence DAS_COMMENT((char * &)) &  __it_rename_at_46_41, char * const  __separator_rename_at_46_42 )
{
    char * __st_rename_at_47_43 = ((char *)(char *)(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_47_44) DAS_AOT_INLINE_LAMBDA -> void{
        bool __skip_first_rename_at_48_45 = true;
        {
            bool __need_loop_49 = true;
            // elem: string aka TT&
            das_iterator<Sequence DAS_COMMENT((char *))> __elem_iterator(__it_rename_at_46_41);
            char * * __elem_rename_at_49_46;
            __need_loop_49 = __elem_iterator.first(__context__,(__elem_rename_at_49_46)) && __need_loop_49;
            for ( ; __need_loop_49 ; __need_loop_49 = __elem_iterator.next(__context__,(__elem_rename_at_49_46)) )
            {
                if ( __skip_first_rename_at_48_45 )
                {
                    das_copy(__skip_first_rename_at_48_45,false);
                } else {
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_11,cast<StringBuilderWriter &>::from(__writer_rename_at_47_44),cast<char * const >::from(__separator_rename_at_46_42)));
                };
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__writer_rename_at_47_44),cast<char *>::from((*__elem_rename_at_49_46))));
            }
            __elem_iterator.close(__context__,(__elem_rename_at_49_46));
        };
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    return das_auto_cast<char *>::cast(__st_rename_at_47_43);
}

inline void finalize_ca4ff32f655e0ba8 ( Context * __context__, cpp_gen::_lambda_cpp_gen_266_1 &  ____this_rename_at_266_47 )
{
    builtin_iterator_delete(das_arg<Sequence DAS_COMMENT((cpp_gen::Scope))>::pass(____this_rename_at_266_47._source_0_at_266_34),__context__);
    memset((void*)&(____this_rename_at_266_47), 0, TypeSize<cpp_gen::_lambda_cpp_gen_266_1>::size);
}

inline cpp_gen::ClangFunc & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_7e2d4453a57c7761 ( Context * __context__, cpp_gen::ClangFunc &  __a_rename_at_50_48 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_13,cast<cpp_gen::ClangFunc &>::from(__a_rename_at_50_48)));
    return das_auto_cast_ref<cpp_gen::ClangFunc &>::cast(__a_rename_at_50_48);
}

inline Sequence DAS_COMMENT((char * &)) _FuncbuiltinTickeach_refTick16137060296763333387_fc747e0a786c7774 ( Context * __context__, Lambda DAS_COMMENT((bool,char * * &)) const  __lam_rename_at_1299_49 )
{
    Sequence DAS_COMMENT((char * *)) __it_rename_at_1300_50;das_zero(__it_rename_at_1300_50);
    builtin_make_lambda_iterator(das_arg<Sequence DAS_COMMENT((char *))>::pass(__it_rename_at_1300_50),__lam_rename_at_1299_49,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((char * &))>::cast(__it_rename_at_1300_50);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_3023c4bc268c52d7 ( Context * __context__, TArray<cpp_gen::ClangStructField> &  __Arr_rename_at_181_51, cpp_gen::ClangStructField &  __value_rename_at_181_52 )
{
    das_copy(__Arr_rename_at_181_51(builtin_array_push_back(das_arg<TArray<cpp_gen::ClangStructField>>::pass(__Arr_rename_at_181_51),32,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_52);
}

inline cpp_gen::ClangFunc & _FuncbuiltinTick_at_with_lockcheckTick7807051423786862253_81b03a6fcc783e7f ( Context * __context__, TTable<char *,cpp_gen::ClangFunc> &  __Tab_rename_at_871_53, char * const  __at_rename_at_871_54 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_14,cast<TTable<char *,cpp_gen::ClangFunc> &>::from(__Tab_rename_at_871_53)));
    return das_auto_cast_ref<cpp_gen::ClangFunc &>::cast(__Tab_rename_at_871_53(__at_rename_at_871_54,__context__));
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_75085f7298948768 ( Context * __context__, TArray<cpp_gen::Scope> &  __Arr_rename_at_68_55, int32_t __newSize_rename_at_68_56 )
{
    builtin_array_resize(das_arg<TArray<cpp_gen::Scope>>::pass(__Arr_rename_at_68_55),__newSize_rename_at_68_56,16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_9f21c57cd4b4c83 ( Context * __context__, TArray<DAS_COMMENT(enum) cpp_gen::AccessKind> &  __Arr_rename_at_68_57, int32_t __newSize_rename_at_68_58 )
{
    builtin_array_resize(das_arg<TArray<DAS_COMMENT(enum) cpp_gen::AccessKind>>::pass(__Arr_rename_at_68_57),__newSize_rename_at_68_58,4,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickemplaceTick13923705686753630697_650e65a9cf1ca823 ( Context * __context__, TArray<cpp_gen::ClangFuncArg> &  __Arr_rename_at_287_59, cpp_gen::ClangFuncArg &  __value_rename_at_287_60 )
{
    das_move(__Arr_rename_at_287_59(builtin_array_push_back(das_arg<TArray<cpp_gen::ClangFuncArg>>::pass(__Arr_rename_at_287_59),40,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_287_60);
}

inline char * _Funccpp_genTickgetCppNameTick2578297908749988033_7a531127a37dc761 ( Context * __context__, char * const  __baseName_rename_at_542_61 )
{
    Sequence DAS_COMMENT((char * *)) _temp_make_local_543_30_48; _temp_make_local_543_30_48;
    char * __name_rename_at_543_62 = (char *)(_Funcstrings_boostTickjoinTick17792841289284275598_f97bd9e0a3ac85c9(__context__,das_arg<Sequence DAS_COMMENT((char *))>::pass((_temp_make_local_543_30_48 = (_FuncbuiltinTickeach_refTick16137060296763333387_fc747e0a786c7774(__context__,das_ascend<bool,cpp_gen::_lambda_cpp_gen_543_2>::make(__context__,&__type_info__822f4971865b164a,(([&]() -> cpp_gen::_lambda_cpp_gen_543_2 {
        cpp_gen::_lambda_cpp_gen_543_2 __mks_543;
        das_zero(__mks_543);
        das_copy((__mks_543.__lambda),(Func(__context__->fnByMangledName(/*@cpp_gen::_lambda_cpp_gen_543_2`function XS<cpp_gen::_lambda_cpp_gen_543_2> &1<s>?*/ 0x9048cc66531f3b60))));
        das_copy((__mks_543.__finalize),(Func(__context__->fnByMangledName(/*@cpp_gen::_lambda_cpp_gen_543_2`finalizer X1<S<cpp_gen::_lambda_cpp_gen_543_2>>?*/ 0xe39b8d88c466ee1d))));
        return __mks_543;
    })())))))),((char *) "::")));
    if ( builtin_string_length(__name_rename_at_543_62,__context__) != 0 )
    {
        das_copy(__name_rename_at_543_62,das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_15, cast<char *>::from(__name_rename_at_543_62), cast<char * const >::from(((char *) "::")), cast<char * const >::from(__baseName_rename_at_542_61))));
    } else {
        das_copy(__name_rename_at_543_62,__baseName_rename_at_542_61);
    };
    return das_auto_cast<char *>::cast(__name_rename_at_543_62);
}

inline bool _Func_lambda_cpp_gen_266_1Tickfunction_8e17cb5a29f7eaff ( Context * __context__, cpp_gen::_lambda_cpp_gen_266_1 &  ____this_rename_at_266_63, char * * & ___ryield266_rename_at_266_64 )
{
    switch (____this_rename_at_266_63.__yield) {
    case 0: goto label_0;
    case 2: goto label_2;
    case 1: goto label_1;
    case 3: goto label_3;
    case 4: goto label_4;
    default: __context__->throw_error("invalid label");
    };
    label_0:;;
    das_copy(____this_rename_at_266_63._loop_at_266_34,true);
    das_move(____this_rename_at_266_63._source_0_at_266_34,_FuncbuiltinTickeachTick6002865651812066953_ab768a47fa5edfe2(__context__,das_arg<TArray<cpp_gen::Scope>>::pass(das_global<TArray<cpp_gen::Scope>,0x82cea5b26477762c>(__context__) /*name_stack*/)));
    memset((void*)&(____this_rename_at_266_63.__n_rename_at_266_38), 0, TypeSize<cpp_gen::Scope *>::size);
    das_copy(____this_rename_at_266_63._pvar_0_at_266_34,das_cast<void *>::cast(das_ref(__context__,____this_rename_at_266_63.__n_rename_at_266_38)));
    DAS_SETBOOLAND((____this_rename_at_266_63._loop_at_266_34),(builtin_iterator_first(das_arg<Sequence DAS_COMMENT((cpp_gen::Scope))>::pass(____this_rename_at_266_63._source_0_at_266_34),____this_rename_at_266_63._pvar_0_at_266_34,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))));
    label_2:;;
    if ( !____this_rename_at_266_63._loop_at_266_34 )
    {
        goto label_4;
    };
    das_copy(___ryield266_rename_at_266_64,das_ref(__context__,das_deref(__context__,____this_rename_at_266_63.__n_rename_at_266_38).name));
    das_copy(____this_rename_at_266_63.__yield,1);
    return das_auto_cast<bool>::cast(true);
    label_1:;;
    label_3:;;
    DAS_SETBOOLAND((____this_rename_at_266_63._loop_at_266_34),(builtin_iterator_next(das_arg<Sequence DAS_COMMENT((cpp_gen::Scope))>::pass(____this_rename_at_266_63._source_0_at_266_34),____this_rename_at_266_63._pvar_0_at_266_34,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))));
    goto label_2;
    label_4:;;
    builtin_iterator_close(das_arg<Sequence DAS_COMMENT((cpp_gen::Scope))>::pass(____this_rename_at_266_63._source_0_at_266_34),____this_rename_at_266_63._pvar_0_at_266_34,__context__);
    return das_auto_cast<bool>::cast(false);
}

inline void _Func_lambda_cpp_gen_266_1Tickfinalizer_bbb8c1e2c3d5dafe ( Context * __context__, cpp_gen::_lambda_cpp_gen_266_1 * ____this_rename_at_266_65 )
{
    finalize_ca4ff32f655e0ba8(__context__,das_arg<cpp_gen::_lambda_cpp_gen_266_1>::pass(das_deref(__context__,____this_rename_at_266_65)));
    das_delete_lambda_struct<cpp_gen::_lambda_cpp_gen_266_1 *>::clear(__context__,____this_rename_at_266_65);
}

inline cpp_gen::ClangStruct & _FuncbuiltinTick_at_with_lockcheckTick7807051423786862253_9ef1c864c9d2f80 ( Context * __context__, TTable<char *,cpp_gen::ClangStruct> &  __Tab_rename_at_871_66, char * const  __at_rename_at_871_67 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_16,cast<TTable<char *,cpp_gen::ClangStruct> &>::from(__Tab_rename_at_871_66)));
    return das_auto_cast_ref<cpp_gen::ClangStruct &>::cast(__Tab_rename_at_871_66(__at_rename_at_871_67,__context__));
}

inline bool _Funccpp_genTickis_valid_locTick10808118537805394975_6bace0942c51e77f ( Context * __context__, TTable<char *,json::JsonValue *> &  __loc_rename_at_192_68 )
{
    return das_auto_cast<bool>::cast((builtin_table_size(das_arg<TTable<char *,json::JsonValue *>>::pass(__loc_rename_at_192_68)) == 0) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(true));
}

inline void _FuncbuiltinTickpushTick14133213201864676143_f626dd014072c607 ( Context * __context__, TArray<DAS_COMMENT(enum) cpp_gen::AccessKind> &  __Arr_rename_at_165_69, DAS_COMMENT(enum) cpp_gen::AccessKind __value_rename_at_165_70 )
{
    das_copy(__Arr_rename_at_165_69(builtin_array_push_back(das_arg<TArray<DAS_COMMENT(enum) cpp_gen::AccessKind>>::pass(__Arr_rename_at_165_69),4,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_165_70);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_440dfee5ce51ee9b ( Context * __context__, TArray<cpp_gen::Scope> &  __Arr_rename_at_181_71, cpp_gen::Scope &  __value_rename_at_181_72 )
{
    das_copy(__Arr_rename_at_181_71(builtin_array_push_back(das_arg<TArray<cpp_gen::Scope>>::pass(__Arr_rename_at_181_71),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_72);
}

inline void _FuncbuiltinTickpopTick1161079256290593740_76e5d0bb5a9d86cc ( Context * __context__, TArray<char *> &  __Arr_rename_at_132_73 )
{
    _FuncbuiltinTickresizeTick4811697762258667383_14682004b38ad2a1(__context__,das_arg<TArray<char *>>::pass(__Arr_rename_at_132_73),builtin_array_size(das_arg<TArray<char *>>::pass(__Arr_rename_at_132_73)) - 1);
}

inline void _FuncbuiltinTickpopTick1161079256290593740_15fc7a604b10573f ( Context * __context__, TArray<cpp_gen::Scope> &  __Arr_rename_at_132_74 )
{
    _FuncbuiltinTickresizeTick4811697762258667383_75085f7298948768(__context__,das_arg<TArray<cpp_gen::Scope>>::pass(__Arr_rename_at_132_74),builtin_array_size(das_arg<TArray<cpp_gen::Scope>>::pass(__Arr_rename_at_132_74)) - 1);
}

inline void _FuncbuiltinTickpopTick1161079256290593740_9fb9082827455aec ( Context * __context__, TArray<DAS_COMMENT(enum) cpp_gen::AccessKind> &  __Arr_rename_at_132_75 )
{
    _FuncbuiltinTickresizeTick4811697762258667383_9f21c57cd4b4c83(__context__,das_arg<TArray<DAS_COMMENT(enum) cpp_gen::AccessKind>>::pass(__Arr_rename_at_132_75),builtin_array_size(das_arg<TArray<DAS_COMMENT(enum) cpp_gen::AccessKind>>::pass(__Arr_rename_at_132_75)) - 1);
}

inline char * _Funccpp_genTickgetQualTypeTick1400640308412000890_a58540215c7d0fa9 ( Context * __context__, TTable<char *,json::JsonValue *> &  __root_rename_at_332_76, char * const  __field_rename_at_332_77 )
{
    TTable<char *,json::JsonValue *> * __qto_rename_at_333_78 = &(das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__root_rename_at_332_76(__field_rename_at_332_77,__context__)->value,__context__));
    return das_auto_cast<char *>::cast(das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as((*__qto_rename_at_333_78)(((char *) "qualType"),__context__)->value,__context__));
}

inline bool _Funccpp_genTickisBitfieldTick9047106797565157337_d37f3cd228cd993f ( Context * __context__, TTable<char *,json::JsonValue *> &  __root_rename_at_516_79 )
{
    if ( _FuncbuiltinTickkey_existsTick16808803843923989214_963e1e05a7a36522(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__root_rename_at_516_79),((char *) "isBitfield")) )
    {
        if ( das_get_auto_variant_field<bool,4,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__root_rename_at_516_79(((char *) "isBitfield"),__context__)->value,__context__) )
        {
            return das_auto_cast<bool>::cast(true);
        };
    };
    return das_auto_cast<bool>::cast(false);
}

inline cpp_gen::ClangFunc _Funccpp_genTickonFunctionBodyTick983279341569892450_8caee9a44ff10098 ( Context * __context__, TTable<char *,json::JsonValue *> &  __root_rename_at_404_80, bool __isMethod_rename_at_404_81, char * const  __ofClass_rename_at_404_82 ) { das_stack_prologue __prologue(__context__,416,"cpp_gen`onFunctionBody`983279341569892450 " DAS_FILE_LINE);
{
    char * __name_rename_at_405_83 = ((char *)(char *)(das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__root_rename_at_404_80(((char *) "name"),__context__)->value,__context__)));
    char * __fullName_rename_at_406_84 = ((char *)(char *)(_Funccpp_genTickgetCppNameTick2578297908749988033_7a531127a37dc761(__context__,__name_rename_at_405_83)));
    char * __mangledName_rename_at_407_85 = ((char *)(char *)(das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__root_rename_at_404_80(((char *) "mangledName"),__context__)->value,__context__)));
    char * __qualType_rename_at_408_86 = (char *)(_Funccpp_genTickgetQualTypeTick1400640308412000890_a58540215c7d0fa9(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__root_rename_at_404_80),((char *) "type")));
    bool __isStatic_rename_at_412_87 = false;
    if ( __isMethod_rename_at_404_81 && _FuncbuiltinTickkey_existsTick16808803843923989214_963e1e05a7a36522(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__root_rename_at_404_80),((char *) "storageClass")) )
    {
        if ( SimPolicy<char *>::Equ(cast<char *>::from(das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__root_rename_at_404_80(((char *) "storageClass"),__context__)->value,__context__)),cast<char *>::from(((char *) "static")),*__context__,nullptr) )
        {
            das_copy(__isStatic_rename_at_412_87,true);
        };
    };
    cpp_gen::ClangFunc __ffunc_rename_at_418_88; das_zero(__ffunc_rename_at_418_88); das_move(__ffunc_rename_at_418_88, (([&]() -> cpp_gen::ClangFunc {
        cpp_gen::ClangFunc __mks_418;
        das_zero(__mks_418);
        das_copy((__mks_418.name),(__name_rename_at_405_83));
        das_copy((__mks_418.cppName),(__fullName_rename_at_406_84));
        das_copy((__mks_418.mangledName),(__mangledName_rename_at_407_85));
        das_copy((__mks_418.ftype),(__qualType_rename_at_408_86));
        das_copy((__mks_418.isMethod),(__isMethod_rename_at_404_81));
        das_copy((__mks_418.isStatic),(__isStatic_rename_at_412_87));
        das_copy((__mks_418.ofClass),(__ofClass_rename_at_404_82));
        return __mks_418;
    })()));
    if ( _FuncbuiltinTickkey_existsTick16808803843923989214_963e1e05a7a36522(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__root_rename_at_404_80),((char *) "inner")) )
    {
        _Funccpp_genTickwith_arrayTick6272002616190333845_c90f8a054aaf2ef6(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__root_rename_at_404_80),((char *) "inner"),das_make_block<void,TArray<json::JsonValue *> &>(__context__,240,0,&__func_info__7ec127c5491418ed,[&](TArray<json::JsonValue *> & __inner_rename_at_428_89) -> void{
            {
                bool __need_loop_429 = true;
                // fad: json::JsonValue?&
                das_iterator<TArray<json::JsonValue *>> __fad_iterator(__inner_rename_at_428_89);
                json::JsonValue * * __fad_rename_at_429_90;
                __need_loop_429 = __fad_iterator.first(__context__,(__fad_rename_at_429_90)) && __need_loop_429;
                for ( ; __need_loop_429 ; __need_loop_429 = __fad_iterator.next(__context__,(__fad_rename_at_429_90)) )
                {
                    DAS_ASSERT((das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is((*__fad_rename_at_429_90)->value)));
                    TTable<char *,json::JsonValue *> * __ad_rename_at_431_91 = &(das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as((*__fad_rename_at_429_90)->value,__context__));
                    if ( SimPolicy<char *>::Equ(cast<char *>::from(das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as((*__ad_rename_at_431_91)(((char *) "kind"),__context__)->value,__context__)),cast<char *>::from(((char *) "ParmVarDecl")),*__context__,nullptr) )
                    {
                        cpp_gen::ClangFuncArg _temp_make_local_442_42_368; _temp_make_local_442_42_368;
                        char * __fieldName_rename_at_433_92 = ((char *)(char *)((_FuncbuiltinTickkey_existsTick16808803843923989214_963e1e05a7a36522(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass((*__ad_rename_at_431_91)),((char *) "name")) ? das_auto_cast<char * const >::cast(das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as((*__ad_rename_at_431_91)(((char *) "name"),__context__)->value,__context__)) : das_auto_cast<char * const >::cast(nullptr))));
                        char * __fqualType_rename_at_434_93 = (char *)(_Funccpp_genTickgetQualTypeTick1400640308412000890_a58540215c7d0fa9(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass((*__ad_rename_at_431_91)),((char *) "type")));
                        char * __fieldInit_rename_at_435_94 = (char *)(nullptr);
                        char * __fieldInitType_rename_at_436_95 = (char *)(nullptr);
                        if ( _FuncbuiltinTickkey_existsTick16808803843923989214_963e1e05a7a36522(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass((*__ad_rename_at_431_91)),((char *) "init")) )
                        {
                            AutoTuple<char *,char *> __arg_rename_at_438_96_ConstRef = ((AutoTuple<char *,char *>)_Funccpp_genTickonInitArgTick7456162652026714814_980c8ded9aae7f9d(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass((*__ad_rename_at_431_91))));
    AutoTuple<char *,char *> const  & __arg_rename_at_438_96 = __arg_rename_at_438_96_ConstRef; ;
                            das_copy(__fieldInit_rename_at_435_94,das_get_auto_tuple_field<char *,0,char *,char *>::get(__arg_rename_at_438_96));
                            das_copy(__fieldInitType_rename_at_436_95,das_get_auto_tuple_field<char *,1,char *,char *>::get(__arg_rename_at_438_96));
                        };
                        _FuncbuiltinTickemplaceTick13923705686753630697_650e65a9cf1ca823(__context__,das_arg<TArray<cpp_gen::ClangFuncArg>>::pass(__ffunc_rename_at_418_88.args),das_arg<cpp_gen::ClangFuncArg>::pass((([&]() -> cpp_gen::ClangFuncArg& {
                            das_zero(_temp_make_local_442_42_368);
                            das_copy((_temp_make_local_442_42_368.name),(__fieldName_rename_at_433_92));
                            das_copy((_temp_make_local_442_42_368.atype),(__fqualType_rename_at_434_93));
                            das_copy((_temp_make_local_442_42_368.value),(__fieldInit_rename_at_435_94));
                            das_copy((_temp_make_local_442_42_368.vtype),(__fieldInitType_rename_at_436_95));
                            return _temp_make_local_442_42_368;
                        })())));
                    };
                }
                __fad_iterator.close(__context__,(__fad_rename_at_429_90));
            };
        }));
    };
    return /* <- */ das_auto_cast_move<cpp_gen::ClangFunc>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_7e2d4453a57c7761(__context__,das_arg<cpp_gen::ClangFunc>::pass(__ffunc_rename_at_418_88)));
}}

inline char * _Funcstrings_boostTickjoinTick16475640899284277631_97cbd29033d05835 ( Context * __context__, TArray<char *> const  &  __it_rename_at_22_97, char * const  __separator_rename_at_22_98 )
{
    char * __st_rename_at_27_99 = ((char *)(char *)(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __writer_rename_at_27_100) DAS_AOT_INLINE_LAMBDA -> void{
        bool __skip_first_rename_at_28_101 = true;
        {
            bool __need_loop_29 = true;
            // elem: string const&
            das_iterator<TArray<char *> const > __elem_iterator(__it_rename_at_22_97);
            char * const  * __elem_rename_at_29_102;
            __need_loop_29 = __elem_iterator.first(__context__,(__elem_rename_at_29_102)) && __need_loop_29;
            for ( ; __need_loop_29 ; __need_loop_29 = __elem_iterator.next(__context__,(__elem_rename_at_29_102)) )
            {
                if ( __skip_first_rename_at_28_101 )
                {
                    das_copy(__skip_first_rename_at_28_101,false);
                } else {
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_17,cast<StringBuilderWriter &>::from(__writer_rename_at_27_100),cast<char * const >::from(__separator_rename_at_22_98)));
                };
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_18,cast<StringBuilderWriter &>::from(__writer_rename_at_27_100),cast<char * const >::from((*__elem_rename_at_29_102))));
            }
            __elem_iterator.close(__context__,(__elem_rename_at_29_102));
        };
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    return das_auto_cast<char *>::cast(__st_rename_at_27_99);
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_963e1e05a7a36522 ( Context * __context__, TTable<char *,json::JsonValue *> const  &  __Tab_rename_at_1014_103, char * const  __at_rename_at_1014_104 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1014_103,__at_rename_at_1014_104));
}

inline void _Funccpp_genTickwith_objectTick7594167909273532910_43fb3bcbe18cb920 ( Context * __context__, TTable<char *,json::JsonValue *> &  __jso_rename_at_182_105, char * const  __field_rename_at_182_106, Block DAS_COMMENT((void,TTable<char *,json::JsonValue *>)) const  &  __blk_rename_at_182_107 )
{
    DAS_ASSERT((das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__jso_rename_at_182_105(__field_rename_at_182_106,__context__)->value)));
    das_invoke<void>::invoke<TTable<char *,json::JsonValue *> &>(__context__,nullptr,__blk_rename_at_182_107,das_arg<TTable<char *,json::JsonValue *>>::pass(das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__jso_rename_at_182_105(__field_rename_at_182_106,__context__)->value,__context__)));
}

inline void _Funccpp_genTickonStructTick15011585540411908992_8d9737e4ce0fff22 ( Context * __context__, TTable<char *,json::JsonValue *> &  __root_rename_at_472_108 )
{
    char * __name_rename_at_473_109 = ((char *)(char *)((_FuncbuiltinTickkey_existsTick16808803843923989214_963e1e05a7a36522(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__root_rename_at_472_108),((char *) "name")) ? das_auto_cast<char * const >::cast(das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__root_rename_at_472_108(((char *) "name"),__context__)->value,__context__)) : das_auto_cast<char * const >::cast(nullptr))));
    char * __tagUsed_rename_at_474_110 = ((char *)(char *)(das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__root_rename_at_472_108(((char *) "tagUsed"),__context__)->value,__context__)));
    char * __fullName_rename_at_475_111 = ((char *)(char *)(_Funccpp_genTickgetCppNameTick2578297908749988033_7a531127a37dc761(__context__,__name_rename_at_473_109)));
    if ( !_FuncbuiltinTickkey_existsTick16808803843923989214_963e1e05a7a36522(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__root_rename_at_472_108),((char *) "inner")) )
    {
        return ;
    } else {
        cpp_gen::ClangStruct _temp_make_local_488_29_80; _temp_make_local_488_29_80;
        cpp_gen::Scope _temp_make_local_493_23_160; _temp_make_local_493_23_160;
        if ( SimPolicy<char *>::Equ(cast<char *>::from(__tagUsed_rename_at_474_110),cast<char *>::from(((char *) "struct")),*__context__,nullptr) )
        {
            _FuncbuiltinTickpushTick14133213201864676143_f626dd014072c607(__context__,das_arg<TArray<DAS_COMMENT(enum) cpp_gen::AccessKind>>::pass(das_global<TArray<DAS_COMMENT(enum) cpp_gen::AccessKind>,0x6adb8b8c35686843>(__context__) /*access_stack*/),DAS_COMMENT(enum) cpp_gen::AccessKind::Public);
        } else {
            _FuncbuiltinTickpushTick14133213201864676143_f626dd014072c607(__context__,das_arg<TArray<DAS_COMMENT(enum) cpp_gen::AccessKind>>::pass(das_global<TArray<DAS_COMMENT(enum) cpp_gen::AccessKind>,0x6adb8b8c35686843>(__context__) /*access_stack*/),DAS_COMMENT(enum) cpp_gen::AccessKind::Private);
        };
        _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_c777430705b7c258(__context__,das_arg<cpp_gen::ClangStruct>::pass(_FuncbuiltinTick_at_with_lockcheckTick7807051423786862253_9ef1c864c9d2f80(__context__,das_arg<TTable<char *,cpp_gen::ClangStruct>>::pass(das_global<cpp_gen::ClangAst,0x3d3f7dbddddbae8e>(__context__) /*ast*/.structs),__fullName_rename_at_475_111)),das_arg<cpp_gen::ClangStruct>::pass((([&]() -> cpp_gen::ClangStruct& {
            das_zero(_temp_make_local_488_29_80);
            das_copy((_temp_make_local_488_29_80.name),(__name_rename_at_473_109));
            das_copy((_temp_make_local_488_29_80.cppName),(__fullName_rename_at_475_111));
            das_copy((_temp_make_local_488_29_80.tag),(__tagUsed_rename_at_474_110));
            return _temp_make_local_488_29_80;
        })())));
        _FuncbuiltinTickpushTick10769833213962245646_440dfee5ce51ee9b(__context__,das_arg<TArray<cpp_gen::Scope>>::pass(das_global<TArray<cpp_gen::Scope>,0x82cea5b26477762c>(__context__) /*name_stack*/),das_arg<cpp_gen::Scope>::pass((([&]() -> cpp_gen::Scope& {
            das_copy((_temp_make_local_493_23_160.name),(__name_rename_at_473_109));
            das_copy((_temp_make_local_493_23_160.kind),(DAS_COMMENT(enum) cpp_gen::ScopeKind::Struct));
            return _temp_make_local_493_23_160;
        })())));
        _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(das_global<TArray<char *>,0x5425979f44e6bd00>(__context__) /*struct_stack*/),__fullName_rename_at_475_111);
        _Funccpp_genTickonInnerTick15341579017765610681_62ed76bf0329fb0d(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__root_rename_at_472_108));
        _FuncbuiltinTickpopTick1161079256290593740_76e5d0bb5a9d86cc(__context__,das_arg<TArray<char *>>::pass(das_global<TArray<char *>,0x5425979f44e6bd00>(__context__) /*struct_stack*/));
        _FuncbuiltinTickpopTick1161079256290593740_15fc7a604b10573f(__context__,das_arg<TArray<cpp_gen::Scope>>::pass(das_global<TArray<cpp_gen::Scope>,0x82cea5b26477762c>(__context__) /*name_stack*/));
        _FuncbuiltinTickpopTick1161079256290593740_9fb9082827455aec(__context__,das_arg<TArray<DAS_COMMENT(enum) cpp_gen::AccessKind>>::pass(das_global<TArray<DAS_COMMENT(enum) cpp_gen::AccessKind>,0x6adb8b8c35686843>(__context__) /*access_stack*/));
    };
}

inline void _Funccpp_genTickonNamespaceTick1569694100191165612_31878f48597dada2 ( Context * __context__, TTable<char *,json::JsonValue *> &  __root_rename_at_261_112 )
{
    if ( !_FuncbuiltinTickkey_existsTick16808803843923989214_963e1e05a7a36522(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__root_rename_at_261_112),((char *) "inner")) )
    {
        return ;
    } else {
        Sequence DAS_COMMENT((char * *)) _temp_make_local_266_34_64; _temp_make_local_266_34_64;
        cpp_gen::Scope _temp_make_local_270_23_96; _temp_make_local_270_23_96;
        char * __name_rename_at_265_113 = ((char *)(char *)(das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__root_rename_at_261_112(((char *) "name"),__context__)->value,__context__)));
        char * __fullName_rename_at_266_114 = ((char *)(char *)((cast<char *>::to(SimPolicy<char *>::Add(cast<char *>::from((cast<char *>::to(SimPolicy<char *>::Add(cast<char *>::from(_Funcstrings_boostTickjoinTick17792841289284275598_f97bd9e0a3ac85c9(__context__,das_arg<Sequence DAS_COMMENT((char *))>::pass((_temp_make_local_266_34_64 = (_FuncbuiltinTickeach_refTick16137060296763333387_fc747e0a786c7774(__context__,das_ascend<bool,cpp_gen::_lambda_cpp_gen_266_1>::make(__context__,&__type_info__74b7d404689c2060,(([&]() -> cpp_gen::_lambda_cpp_gen_266_1 {
            cpp_gen::_lambda_cpp_gen_266_1 __mks_266;
            das_zero(__mks_266);
            das_copy((__mks_266.__lambda),(Func(__context__->fnByMangledName(/*@cpp_gen::_lambda_cpp_gen_266_1`function XS<cpp_gen::_lambda_cpp_gen_266_1> &1<s>?*/ 0x8a5534149b14699d))));
            das_copy((__mks_266.__finalize),(Func(__context__->fnByMangledName(/*@cpp_gen::_lambda_cpp_gen_266_1`finalizer X1<S<cpp_gen::_lambda_cpp_gen_266_1>>?*/ 0x5c5d42bd02bb7d7b))));
            return __mks_266;
        })())))))),((char *) "::"))),cast<char *>::from(((char *) "::")),*__context__,nullptr)))),cast<char *>::from(__name_rename_at_265_113),*__context__,nullptr)))));
        _FuncbuiltinTickpushTick10769833213962245646_440dfee5ce51ee9b(__context__,das_arg<TArray<cpp_gen::Scope>>::pass(das_global<TArray<cpp_gen::Scope>,0x82cea5b26477762c>(__context__) /*name_stack*/),das_arg<cpp_gen::Scope>::pass((([&]() -> cpp_gen::Scope& {
            das_copy((_temp_make_local_270_23_96.name),(__name_rename_at_265_113));
            das_copy((_temp_make_local_270_23_96.kind),(DAS_COMMENT(enum) cpp_gen::ScopeKind::Namespace));
            return _temp_make_local_270_23_96;
        })())));
        _Funccpp_genTickonInnerTick15341579017765610681_62ed76bf0329fb0d(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__root_rename_at_261_112));
        _FuncbuiltinTickpopTick1161079256290593740_15fc7a604b10573f(__context__,das_arg<TArray<cpp_gen::Scope>>::pass(das_global<TArray<cpp_gen::Scope>,0x82cea5b26477762c>(__context__) /*name_stack*/));
    };
}

inline void _Funccpp_genTickonFieldTick11712683440231282153_f030e55d3f2f1f92 ( Context * __context__, TTable<char *,json::JsonValue *> &  __root_rename_at_525_115 )
{
    cpp_gen::ClangStructField _temp_make_local_533_36_96; _temp_make_local_533_36_96;
    char * __name_rename_at_526_116 = ((char *)(char *)((_FuncbuiltinTickkey_existsTick16808803843923989214_963e1e05a7a36522(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__root_rename_at_525_115),((char *) "name")) ? das_auto_cast<char * const >::cast(das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__root_rename_at_525_115(((char *) "name"),__context__)->value,__context__)) : das_auto_cast<char * const >::cast(nullptr))));
    char * __qualType_rename_at_527_117 = (char *)(_Funccpp_genTickgetQualTypeTick1400640308412000890_a58540215c7d0fa9(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__root_rename_at_525_115),((char *) "type")));
    DAS_COMMENT(enum) cpp_gen::AccessKind __access_rename_at_528_118 = das_global<TArray<DAS_COMMENT(enum) cpp_gen::AccessKind>,0x6adb8b8c35686843>(__context__) /*access_stack*/((builtin_array_size(das_arg<TArray<DAS_COMMENT(enum) cpp_gen::AccessKind>>::pass(das_global<TArray<DAS_COMMENT(enum) cpp_gen::AccessKind>,0x6adb8b8c35686843>(__context__) /*access_stack*/)) - 1),__context__);
    char * __sfn_rename_at_532_119 = ((char *)(char *)(das_global<TArray<char *>,0x5425979f44e6bd00>(__context__) /*struct_stack*/((builtin_array_size(das_arg<TArray<char *>>::pass(das_global<TArray<char *>,0x5425979f44e6bd00>(__context__) /*struct_stack*/)) - 1),__context__)));
    _FuncbuiltinTickpushTick10769833213962245646_3023c4bc268c52d7(__context__,das_arg<TArray<cpp_gen::ClangStructField>>::pass(_FuncbuiltinTick_at_with_lockcheckTick7807051423786862253_9ef1c864c9d2f80(__context__,das_arg<TTable<char *,cpp_gen::ClangStruct>>::pass(das_global<cpp_gen::ClangAst,0x3d3f7dbddddbae8e>(__context__) /*ast*/.structs),__sfn_rename_at_532_119).fields),das_arg<cpp_gen::ClangStructField>::pass((([&]() -> cpp_gen::ClangStructField& {
        das_copy((_temp_make_local_533_36_96.name),(__name_rename_at_526_116));
        das_copy((_temp_make_local_533_36_96.cppName),(__name_rename_at_526_116));
        das_copy((_temp_make_local_533_36_96.qtype),(__qualType_rename_at_527_117));
        das_copy((_temp_make_local_533_36_96.access),(__access_rename_at_528_118));
        das_copy((_temp_make_local_533_36_96.isBitfield),(_Funccpp_genTickisBitfieldTick9047106797565157337_d37f3cd228cd993f(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__root_rename_at_525_115))));
        return _temp_make_local_533_36_96;
    })())));
}

inline void _Funccpp_genTickonFunctionTick9237376346942193985_2dab482eabe1f2c3 ( Context * __context__, TTable<char *,json::JsonValue *> &  __root_rename_at_467_120 )
{
    cpp_gen::ClangFunc __fdecl_rename_at_468_121; das_zero(__fdecl_rename_at_468_121); das_move(__fdecl_rename_at_468_121, _Funccpp_genTickonFunctionBodyTick983279341569892450_8caee9a44ff10098(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__root_rename_at_467_120),false,nullptr));
    _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_cfad5be86fc7ec48(__context__,das_arg<cpp_gen::ClangFunc>::pass(_FuncbuiltinTick_at_with_lockcheckTick7807051423786862253_81b03a6fcc783e7f(__context__,das_arg<TTable<char *,cpp_gen::ClangFunc>>::pass(das_global<cpp_gen::ClangAst,0x3d3f7dbddddbae8e>(__context__) /*ast*/.funcs),__fdecl_rename_at_468_121.mangledName)),das_arg<cpp_gen::ClangFunc>::pass(__fdecl_rename_at_468_121));
}

inline void _Funccpp_genTickonMethodTick5355858089746197764_68eed9a736f85c63 ( Context * __context__, TTable<char *,json::JsonValue *> &  __root_rename_at_458_122 )
{
    if ( builtin_array_size(das_arg<TArray<char *>>::pass(das_global<TArray<char *>,0x5425979f44e6bd00>(__context__) /*struct_stack*/)) > 0 )
    {
        cpp_gen::ClangFunc _temp_make_local_461_52_48; _temp_make_local_461_52_48;
        char * __in_class_rename_at_460_123 = (char *)(das_global<TArray<char *>,0x5425979f44e6bd00>(__context__) /*struct_stack*/((builtin_array_size(das_arg<TArray<char *>>::pass(das_global<TArray<char *>,0x5425979f44e6bd00>(__context__) /*struct_stack*/)) - 1),__context__));
        _FuncbuiltinTickemplaceTick13923705686753630697_b7c430019eafb474(__context__,das_arg<TArray<cpp_gen::ClangFunc>>::pass(_FuncbuiltinTick_at_with_lockcheckTick7807051423786862253_9ef1c864c9d2f80(__context__,das_arg<TTable<char *,cpp_gen::ClangStruct>>::pass(das_global<cpp_gen::ClangAst,0x3d3f7dbddddbae8e>(__context__) /*ast*/.structs),__in_class_rename_at_460_123).methods),das_arg<cpp_gen::ClangFunc>::pass((_temp_make_local_461_52_48 = (_Funccpp_genTickonFunctionBodyTick983279341569892450_8caee9a44ff10098(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__root_rename_at_458_122),true,_Funcstrings_boostTickjoinTick16475640899284277631_97cbd29033d05835(__context__,das_arg<TArray<char *>>::pass(das_global<TArray<char *>,0x5425979f44e6bd00>(__context__) /*struct_stack*/),((char *) "::")))))));
    } else {
        builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_19, cast<char * const >::from(((char *) "WARNING: method not in class ")), cast<json::JsonValue *>::from(__root_rename_at_458_122(((char *) "name"),__context__)))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}

inline void _Funccpp_genTickonEnumTick2734318481451533008_f3a14439e6d126ab ( Context * __context__, TTable<char *,json::JsonValue *> &  __root_rename_at_275_124 ) { das_stack_prologue __prologue(__context__,320,"cpp_gen`onEnum`2734318481451533008 " DAS_FILE_LINE);
{
    if ( !_FuncbuiltinTickkey_existsTick16808803843923989214_963e1e05a7a36522(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__root_rename_at_275_124),((char *) "name")) )
    {
        builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_20, cast<char * const >::from(((char *) "enum 'name' missing in ")), cast<TTable<char *,json::JsonValue *> &>::from(__root_rename_at_275_124), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        return ;
    } else {
        char * __name_rename_at_280_125 = ((char *)(char *)(das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__root_rename_at_275_124(((char *) "name"),__context__)->value,__context__)));
        char * __fullName_rename_at_281_126 = ((char *)(char *)(_Funccpp_genTickgetCppNameTick2578297908749988033_7a531127a37dc761(__context__,__name_rename_at_280_125)));
        if ( _FuncbuiltinTickkey_existsTick16808803843923989214_963e1e05a7a36522(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__root_rename_at_275_124),((char *) "scopedEnumTag")) )
        {
            cpp_gen::ClangEnum _temp_make_local_288_31_80; _temp_make_local_288_31_80;
            char * __qtype_rename_at_284_127 = ((char *)(char *)(_Funccpp_genTickgetQualTypeTick1400640308412000890_a58540215c7d0fa9(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__root_rename_at_275_124),((char *) "fixedUnderlyingType"))));
            _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_e79b4e2307f7ae1c(__context__,das_arg<cpp_gen::ClangEnum>::pass(_FuncbuiltinTick_at_with_lockcheckTick7807051423786862253_68d1e221d9f9bb60(__context__,das_arg<TTable<char *,cpp_gen::ClangEnum>>::pass(das_global<cpp_gen::ClangAst,0x3d3f7dbddddbae8e>(__context__) /*ast*/.enums),__fullName_rename_at_281_126)),das_arg<cpp_gen::ClangEnum>::pass((([&]() -> cpp_gen::ClangEnum& {
                das_zero(_temp_make_local_288_31_80);
                das_copy((_temp_make_local_288_31_80.name),(__name_rename_at_280_125));
                das_copy((_temp_make_local_288_31_80.cppName),(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_21, cast<char * const >::from(__fullName_rename_at_281_126), cast<char *>::from(das_global<char *,0xc951cb83148d6076>(__context__) /*enum_suffix*/)))));
                das_copy((_temp_make_local_288_31_80.eclass),(true));
                das_copy((_temp_make_local_288_31_80.etype),(__qtype_rename_at_284_127));
                return _temp_make_local_288_31_80;
            })())));
        } else {
            cpp_gen::ClangEnum _temp_make_local_298_31_144; _temp_make_local_298_31_144;
            _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_e79b4e2307f7ae1c(__context__,das_arg<cpp_gen::ClangEnum>::pass(_FuncbuiltinTick_at_with_lockcheckTick7807051423786862253_68d1e221d9f9bb60(__context__,das_arg<TTable<char *,cpp_gen::ClangEnum>>::pass(das_global<cpp_gen::ClangAst,0x3d3f7dbddddbae8e>(__context__) /*ast*/.enums),__fullName_rename_at_281_126)),das_arg<cpp_gen::ClangEnum>::pass((([&]() -> cpp_gen::ClangEnum& {
                das_zero(_temp_make_local_298_31_144);
                das_copy((_temp_make_local_298_31_144.name),(__name_rename_at_280_125));
                das_copy((_temp_make_local_298_31_144.cppName),(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_22, cast<char * const >::from(__fullName_rename_at_281_126), cast<char *>::from(das_global<char *,0xc951cb83148d6076>(__context__) /*enum_suffix*/)))));
                das_copy((_temp_make_local_298_31_144.eclass),(false));
                return _temp_make_local_298_31_144;
            })())));
        };
        _Funccpp_genTickwith_arrayTick6272002616190333845_c90f8a054aaf2ef6(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__root_rename_at_275_124),((char *) "inner"),das_make_block<void,TArray<json::JsonValue *> &>(__context__,256,0,&__func_info__7ec127c5491418ed,[&](TArray<json::JsonValue *> & __inner_rename_at_304_128) -> void{
            {
                bool __need_loop_305 = true;
                // edo: json::JsonValue?&
                das_iterator<TArray<json::JsonValue *>> __edo_iterator(__inner_rename_at_304_128);
                json::JsonValue * * __edo_rename_at_305_129;
                __need_loop_305 = __edo_iterator.first(__context__,(__edo_rename_at_305_129)) && __need_loop_305;
                for ( ; __need_loop_305 ; __need_loop_305 = __edo_iterator.next(__context__,(__edo_rename_at_305_129)) )
                {
                    DAS_ASSERT((das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is((*__edo_rename_at_305_129)->value)));
                    TTable<char *,json::JsonValue *> * __ed_rename_at_307_130 = &(das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as((*__edo_rename_at_305_129)->value,__context__));
                    if ( SimPolicy<char *>::Equ(cast<char *>::from(das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as((*__ed_rename_at_307_130)(((char *) "kind"),__context__)->value,__context__)),cast<char *>::from(((char *) "EnumConstantDecl")),*__context__,nullptr) )
                    {
                        char * __cname_rename_at_309_131 = (char *)(das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as((*__ed_rename_at_307_130)(((char *) "name"),__context__)->value,__context__));
                        _FuncbuiltinTickpushTick10769833213962245646_5a77afdd4cbd09e1(__context__,das_arg<TArray<char *>>::pass(_FuncbuiltinTick_at_with_lockcheckTick7807051423786862253_68d1e221d9f9bb60(__context__,das_arg<TTable<char *,cpp_gen::ClangEnum>>::pass(das_global<cpp_gen::ClangAst,0x3d3f7dbddddbae8e>(__context__) /*ast*/.enums),__fullName_rename_at_281_126).edecl),__cname_rename_at_309_131);
                    };
                }
                __edo_iterator.close(__context__,(__edo_rename_at_305_129));
            };
        }));
    };
}}

inline void _Funccpp_genTickonVarTick7763998161592485260_1b4b388a8d41c569 ( Context * __context__, TTable<char *,json::JsonValue *> &  __root_rename_at_316_132 )
{
    char * __name_rename_at_317_133 = ((char *)(char *)(das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__root_rename_at_316_132(((char *) "name"),__context__)->value,__context__)));
    char * __fullName_rename_at_318_134 = ((char *)(char *)(_Funccpp_genTickgetCppNameTick2578297908749988033_7a531127a37dc761(__context__,__name_rename_at_317_133)));
    char * __mangledName_rename_at_319_135 = ((char *)(char *)(das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__root_rename_at_316_132(((char *) "mangledName"),__context__)->value,__context__)));
    char * __qualType_rename_at_320_136 = (char *)(_Funccpp_genTickgetQualTypeTick1400640308412000890_a58540215c7d0fa9(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__root_rename_at_316_132),((char *) "type")));
    das_move(das_global<cpp_gen::ClangAst,0x3d3f7dbddddbae8e>(__context__) /*ast*/.globals(__fullName_rename_at_318_134,__context__),(([&]() -> cpp_gen::ClangGlobalVar {
        cpp_gen::ClangGlobalVar __mks_324;
        das_copy((__mks_324.name),(__name_rename_at_317_133));
        das_copy((__mks_324.cppName),(__fullName_rename_at_318_134));
        das_copy((__mks_324.mangledName),(__mangledName_rename_at_319_135));
        das_copy((__mks_324.gtype),(__qualType_rename_at_320_136));
        return __mks_324;
    })()));
}

inline void _Funccpp_genTickonTypeDeclTick13685927393030072381_48d18a41d3948b70 ( Context * __context__, TTable<char *,json::JsonValue *> &  __root_rename_at_552_137 )
{
    char * __name_rename_at_553_138 = ((char *)(char *)(das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__root_rename_at_552_137(((char *) "name"),__context__)->value,__context__)));
    char * __fullName_rename_at_554_139 = ((char *)(char *)(_Funccpp_genTickgetCppNameTick2578297908749988033_7a531127a37dc761(__context__,__name_rename_at_553_138)));
    char * __qualType_rename_at_555_140 = (char *)(_Funccpp_genTickgetQualTypeTick1400640308412000890_a58540215c7d0fa9(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__root_rename_at_552_137),((char *) "type")));
    das_copy(das_global<cpp_gen::ClangAst,0x3d3f7dbddddbae8e>(__context__) /*ast*/.typedefs(__fullName_rename_at_554_139,__context__),(([&]() -> cpp_gen::ClangTypedef {
        cpp_gen::ClangTypedef __mks_559;
        das_copy((__mks_559.name),(__name_rename_at_553_138));
        das_copy((__mks_559.cppName),(__fullName_rename_at_554_139));
        das_copy((__mks_559.qtype),(__qualType_rename_at_555_140));
        return __mks_559;
    })()));
}

inline void _Funccpp_genTickonAccessTick18228828068813142789_a461a7e898d3234f ( Context * __context__, TTable<char *,json::JsonValue *> &  __root_rename_at_501_141 )
{
    char * __access_rename_at_502_142 = ((char *)(char *)(das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__root_rename_at_501_141(((char *) "access"),__context__)->value,__context__)));
    DAS_COMMENT(enum) cpp_gen::AccessKind __kind_rename_at_503_143 = DAS_COMMENT(enum) cpp_gen::AccessKind::Public;
    if ( SimPolicy<char *>::Equ(cast<char *>::from(__access_rename_at_502_142),cast<char *>::from(((char *) "public")),*__context__,nullptr) )
    {
        das_copy(__kind_rename_at_503_143,DAS_COMMENT(enum) cpp_gen::AccessKind::Public);
    } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__access_rename_at_502_142),cast<char *>::from(((char *) "private")),*__context__,nullptr) )
    {
        das_copy(__kind_rename_at_503_143,DAS_COMMENT(enum) cpp_gen::AccessKind::Private);
    } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__access_rename_at_502_142),cast<char *>::from(((char *) "protected")),*__context__,nullptr) )
    {
        das_copy(__kind_rename_at_503_143,DAS_COMMENT(enum) cpp_gen::AccessKind::Protected);
    } else {
        builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_23, cast<char * const >::from(((char *) "unsupported access kind ")), cast<char * const >::from(__access_rename_at_502_142), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    das_copy(das_global<TArray<DAS_COMMENT(enum) cpp_gen::AccessKind>,0x6adb8b8c35686843>(__context__) /*access_stack*/((builtin_array_size(das_arg<TArray<DAS_COMMENT(enum) cpp_gen::AccessKind>>::pass(das_global<TArray<DAS_COMMENT(enum) cpp_gen::AccessKind>,0x6adb8b8c35686843>(__context__) /*access_stack*/)) - 1),__context__),__kind_rename_at_503_143);
}

inline void _Funccpp_genTickonLinkageSpecTick9588560724416692279_347cf8f7a9c1d56 ( Context * __context__, TTable<char *,json::JsonValue *> &  __root_rename_at_246_144 )
{
    if ( !das_global<bool,0xbe90e5ba972c8a8b>(__context__) /*allow_extern_c*/ )
    {
        return ;
    } else {
        if ( !_FuncbuiltinTickkey_existsTick16808803843923989214_963e1e05a7a36522(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__root_rename_at_246_144),((char *) "inner")) )
        {
            return ;
        } else {
            _Funccpp_genTickonInnerTick15341579017765610681_62ed76bf0329fb0d(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__root_rename_at_246_144));
        };
    };
}

inline void _Funccpp_genTickwith_arrayTick6272002616190333845_c90f8a054aaf2ef6 ( Context * __context__, TTable<char *,json::JsonValue *> &  __jso_rename_at_187_145, char * const  __field_rename_at_187_146, Block DAS_COMMENT((void,TArray<json::JsonValue *>)) const  &  __blk_rename_at_187_147 )
{
    DAS_ASSERT((das_get_auto_variant_field<TArray<json::JsonValue *>,1,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__jso_rename_at_187_145(__field_rename_at_187_146,__context__)->value)));
    das_invoke<void>::invoke<TArray<json::JsonValue *> &>(__context__,nullptr,__blk_rename_at_187_147,das_arg<TArray<json::JsonValue *>>::pass(das_get_auto_variant_field<TArray<json::JsonValue *>,1,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__jso_rename_at_187_145(__field_rename_at_187_146,__context__)->value,__context__)));
}

inline bool _Funccpp_genTickis_strTick14919838023837841642_48d382d152e59de8 ( Context * __context__, json::JsonValue * const  __jso_rename_at_178_148, char * const  __str_rename_at_178_149 )
{
    return das_auto_cast<bool>::cast(das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__jso_rename_at_178_148->value) && (SimPolicy<char *>::Equ(cast<char *>::from(das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__jso_rename_at_178_148->value,__context__)),cast<char *>::from(__str_rename_at_178_149),*__context__,nullptr)));
}

inline void _Funccpp_genTickonInnerTick15341579017765610681_62ed76bf0329fb0d ( Context * __context__, TTable<char *,json::JsonValue *> &  __root_rename_at_199_150 ) { das_stack_prologue __prologue(__context__,224,"cpp_gen`onInner`15341579017765610681 " DAS_FILE_LINE);
{
    _Funccpp_genTickwith_arrayTick6272002616190333845_c90f8a054aaf2ef6(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__root_rename_at_199_150),((char *) "inner"),das_make_block<void,TArray<json::JsonValue *> &>(__context__,80,0,&__func_info__7ec127c5491418ed,[&](TArray<json::JsonValue *> & __inner_rename_at_200_151) -> void{
        {
            bool __need_loop_201 = true;
            // idecl: json::JsonValue?&
            das_iterator<TArray<json::JsonValue *>> __idecl_iterator(__inner_rename_at_200_151);
            json::JsonValue * * __idecl_rename_at_201_152;
            __need_loop_201 = __idecl_iterator.first(__context__,(__idecl_rename_at_201_152)) && __need_loop_201;
            for ( ; __need_loop_201 ; __need_loop_201 = __idecl_iterator.next(__context__,(__idecl_rename_at_201_152)) )
            {
                DAS_ASSERT((das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is((*__idecl_rename_at_201_152)->value)));
                TTable<char *,json::JsonValue *> * __decl_rename_at_203_153 = &(das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as((*__idecl_rename_at_201_152)->value,__context__));
                bool __skip_rename_at_204_154 = false;
                if ( _FuncbuiltinTickkey_existsTick16808803843923989214_963e1e05a7a36522(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass((*__decl_rename_at_203_153)),((char *) "loc")) )
                {
                    _Funccpp_genTickwith_objectTick7594167909273532910_43fb3bcbe18cb920(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass((*__decl_rename_at_203_153)),((char *) "loc"),das_make_block<void,TTable<char *,json::JsonValue *> &>(__context__,192,0,&__func_info__a833f7b3668de771,[&](TTable<char *,json::JsonValue *> & __loc_rename_at_206_155) -> void{
                        das_copy(__skip_rename_at_204_154,!_Funccpp_genTickis_valid_locTick10808118537805394975_6bace0942c51e77f(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass(__loc_rename_at_206_155)));
                    }));
                };
                if ( __skip_rename_at_204_154 )
                {
                    continue;
                } else {
                    char * __kind_rename_at_216_156 = ((char *)(char *)(das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as((*__decl_rename_at_203_153)(((char *) "kind"),__context__)->value,__context__)));
                    if ( SimPolicy<char *>::Equ(cast<char *>::from(__kind_rename_at_216_156),cast<char *>::from(((char *) "CXXRecordDecl")),*__context__,nullptr) )
                    {
                        _Funccpp_genTickonStructTick15011585540411908992_8d9737e4ce0fff22(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass((*__decl_rename_at_203_153)));
                    } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__kind_rename_at_216_156),cast<char *>::from(((char *) "NamespaceDecl")),*__context__,nullptr) )
                    {
                        _Funccpp_genTickonNamespaceTick1569694100191165612_31878f48597dada2(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass((*__decl_rename_at_203_153)));
                    } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__kind_rename_at_216_156),cast<char *>::from(((char *) "FieldDecl")),*__context__,nullptr) )
                    {
                        _Funccpp_genTickonFieldTick11712683440231282153_f030e55d3f2f1f92(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass((*__decl_rename_at_203_153)));
                    } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__kind_rename_at_216_156),cast<char *>::from(((char *) "FunctionDecl")),*__context__,nullptr) )
                    {
                        _Funccpp_genTickonFunctionTick9237376346942193985_2dab482eabe1f2c3(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass((*__decl_rename_at_203_153)));
                    } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__kind_rename_at_216_156),cast<char *>::from(((char *) "CXXMethodDecl")),*__context__,nullptr) )
                    {
                        _Funccpp_genTickonMethodTick5355858089746197764_68eed9a736f85c63(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass((*__decl_rename_at_203_153)));
                    } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__kind_rename_at_216_156),cast<char *>::from(((char *) "EnumDecl")),*__context__,nullptr) )
                    {
                        _Funccpp_genTickonEnumTick2734318481451533008_f3a14439e6d126ab(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass((*__decl_rename_at_203_153)));
                    } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__kind_rename_at_216_156),cast<char *>::from(((char *) "VarDecl")),*__context__,nullptr) )
                    {
                        _Funccpp_genTickonVarTick7763998161592485260_1b4b388a8d41c569(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass((*__decl_rename_at_203_153)));
                    } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__kind_rename_at_216_156),cast<char *>::from(((char *) "TypedefDecl")),*__context__,nullptr) )
                    {
                        _Funccpp_genTickonTypeDeclTick13685927393030072381_48d18a41d3948b70(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass((*__decl_rename_at_203_153)));
                    } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__kind_rename_at_216_156),cast<char *>::from(((char *) "AccessSpecDecl")),*__context__,nullptr) )
                    {
                        _Funccpp_genTickonAccessTick18228828068813142789_a461a7e898d3234f(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass((*__decl_rename_at_203_153)));
                    } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__kind_rename_at_216_156),cast<char *>::from(((char *) "LinkageSpecDecl")),*__context__,nullptr) )
                    {
                        _Funccpp_genTickonLinkageSpecTick9588560724416692279_347cf8f7a9c1d56(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass((*__decl_rename_at_203_153)));
                    } else {
                        builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_24, cast<char * const >::from(((char *) "warning: unsupported ")), cast<char * const >::from(__kind_rename_at_216_156), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    };
                };
            }
            __idecl_iterator.close(__context__,(__idecl_rename_at_201_152));
        };
    }));
}}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_4ca5259fa9e9c74e ( Context * __context__, TDim<char *,1> const  &  __a_rename_at_581_157 )
{
    return das_auto_cast<int32_t>::cast(1);
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_14682004b38ad2a1 ( Context * __context__, TArray<char *> &  __Arr_rename_at_68_158, int32_t __newSize_rename_at_68_159 )
{
    builtin_array_resize(das_arg<TArray<char *>>::pass(__Arr_rename_at_68_158),__newSize_rename_at_68_159,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_401f694502683cef ( Context * __context__, TArray<AutoTuple<range,char *>> &  __Arr_rename_at_68_160, int32_t __newSize_rename_at_68_161 )
{
    builtin_array_resize(das_arg<TArray<AutoTuple<range,char *>>>::pass(__Arr_rename_at_68_160),__newSize_rename_at_68_161,16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_ddf4f34775c3f6ad ( Context * __context__, TDim<char *,4> const  &  __a_rename_at_581_162 )
{
    return das_auto_cast<int32_t>::cast(4);
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_30458db6e3c9dc4b ( Context * __context__, TDim<char *,2> const  &  __a_rename_at_581_163 )
{
    return das_auto_cast<int32_t>::cast(2);
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_3a6bd4fafb6564b7 ( Context * __context__, TDim<AutoTuple<range,char *>,1> const  &  __a_rename_at_581_164 )
{
    return das_auto_cast<int32_t>::cast(1);
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_d57f120a5eb06ce3 ( Context * __context__, TDim<AutoTuple<range,char *>,3> const  &  __a_rename_at_581_165 )
{
    return das_auto_cast<int32_t>::cast(3);
}

inline void _FuncfioTickfreadTick9799048084303762322_c82e6deab5e5b700 ( Context * __context__, FILE const  * const  __f_rename_at_43_166, Block DAS_COMMENT((void,char * const )) const  &  __blk_rename_at_43_167 )
{
    char * __stmp_rename_at_44_168 = (char *)(((char * const )(builtin_fread(__f_rename_at_43_166,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
    das_invoke<void>::invoke<char *>(__context__,nullptr,__blk_rename_at_43_167,das_cast<char *>::cast(__stmp_rename_at_44_168));
    delete_string(__stmp_rename_at_44_168,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_9d11994217d1865a ( Context * __context__, TTable<int32_t,bool> const  &  __Tab_rename_at_1014_169, int32_t __at_rename_at_1014_170 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1014_169,__at_rename_at_1014_170));
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_9b8b56716258076a ( Context * __context__, TTable<char *,cpp_gen::ClangStruct> const  &  __Tab_rename_at_1014_171, char * const  __at_rename_at_1014_172 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1014_171,__at_rename_at_1014_172));
}

inline bool _FuncbuiltinTickgetTick8447005936052527643_c25c472f7c63dd94 ( Context * __context__, TTable<char *,cpp_gen::ExEnum> &  __Tab_rename_at_654_173, char * const  __at_rename_at_654_174, Block DAS_COMMENT((void,cpp_gen::ExEnum)) const  &  __blk_rename_at_654_175 )
{
    cpp_gen::ExEnum * __val_rename_at_655_176 = __builtin_table_find(__context__,__Tab_rename_at_654_173,__at_rename_at_654_174);
    if ( __val_rename_at_655_176 != nullptr )
    {
        builtin_table_lock(das_arg<TTable<char *,cpp_gen::ExEnum>>::pass(__Tab_rename_at_654_173),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        das_invoke<void>::invoke<cpp_gen::ExEnum &>(__context__,nullptr,__blk_rename_at_654_175,das_arg<cpp_gen::ExEnum>::pass(das_deref(__context__,das_cast<cpp_gen::ExEnum *>::cast(__val_rename_at_655_176))));
        builtin_table_unlock(das_arg<TTable<char *,cpp_gen::ExEnum>>::pass(__Tab_rename_at_654_173),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        return das_auto_cast<bool>::cast(true);
    } else {
        return das_auto_cast<bool>::cast(false);
    };
}

inline bool _Funccpp_genTickneedToGenStructTick14158910623600840251_fd93d910f98eae6b ( Context * __context__, cpp_gen::ClangStruct const  &  __st_rename_at_652_177 )
{
    if ( builtin_empty(__st_rename_at_652_177.name) )
    {
        return das_auto_cast<bool>::cast(false);
    } else {
        if ( das_global<Lambda DAS_COMMENT((bool,char * const )),0xcf018a17ba6e16de>(__context__) /*is_struct_blocked*/ != nullptr )
        {
            if ( das_invoke_lambda<bool>::invoke<char * const >(__context__,nullptr,das_global<Lambda DAS_COMMENT((bool,char * const )),0xcf018a17ba6e16de>(__context__) /*is_struct_blocked*/,__st_rename_at_652_177.name) )
            {
                return das_auto_cast<bool>::cast(false);
            };
        };
        return das_auto_cast<bool>::cast(true);
    };
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_1a54ecb7273c2da1 ( Context * __context__, TTable<char *,char *> const  &  __Tab_rename_at_1014_178, char * const  __at_rename_at_1014_179 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1014_178,__at_rename_at_1014_179));
}

inline bool _Funccpp_genTickneedToGenFieldTick2521814960739047210_6a0d55d67bdff534 ( Context * __context__, cpp_gen::ClangStructField const  &  __fld_rename_at_664_180 )
{
    return das_auto_cast<bool>::cast(builtin_empty(__fld_rename_at_664_180.name) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((__fld_rename_at_664_180.access != DAS_COMMENT(enum) cpp_gen::AccessKind::Public) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast((isBlockedType_cc1ed1c7968e329c(__context__,__fld_rename_at_664_180.qtype) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(true))))));
}

inline bool _Funccpp_genTickisBlockedFunTick2478178585869301773_97ae78cd67b212a3 ( Context * __context__, cpp_gen::ClangFunc const  &  __fn_rename_at_771_181 )
{
    if ( __fn_rename_at_771_181.isMethod && __fn_rename_at_771_181.isStatic )
    {
        builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_25, cast<char * const >::from(((char *) "warning: skipping static method ")), cast<char * const >::from(__fn_rename_at_771_181.name), cast<char * const >::from(((char *) " in ")), cast<char * const >::from(__fn_rename_at_771_181.cppName), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        return das_auto_cast<bool>::cast(true);
    } else {
        if ( das_invoke_function<int32_t>::invoke<regex::Regex &,char * const ,int32_t>(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::regex_match S<regex::Regex> Cs Ci*/ 0x297b408d218ed2d)),das_arg<regex::Regex>::pass(das_global<regex::Regex,0xbf41002a7c46ff12>(__context__) /*op_regex*/),__fn_rename_at_771_181.name,0) != -1 )
        {
            builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_26, cast<char * const >::from(((char *) "warning: skipping ")), cast<char * const >::from(__fn_rename_at_771_181.name), cast<char * const >::from(((char *) " in ")), cast<char * const >::from(__fn_rename_at_771_181.cppName), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            return das_auto_cast<bool>::cast(true);
        } else {
            if ( builtin_string_find2(__fn_rename_at_771_181.ftype,((char *) "...")) != -1 )
            {
                builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_27, cast<char * const >::from(((char *) "warning: variadic function ")), cast<char * const >::from(__fn_rename_at_771_181.name), cast<char * const >::from(((char *) " aka ")), cast<char * const >::from(__fn_rename_at_771_181.cppName), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                return das_auto_cast<bool>::cast(true);
            } else {
                {
                    bool __need_loop_784 = true;
                    // fa: cpp_gen::ClangFuncArg const&
                    das_iterator<TArray<cpp_gen::ClangFuncArg> const > __fa_iterator(__fn_rename_at_771_181.args);
                    cpp_gen::ClangFuncArg const  * __fa_rename_at_784_182;
                    __need_loop_784 = __fa_iterator.first(__context__,(__fa_rename_at_784_182)) && __need_loop_784;
                    for ( ; __need_loop_784 ; __need_loop_784 = __fa_iterator.next(__context__,(__fa_rename_at_784_182)) )
                    {
                        if ( isBlockedType_cc1ed1c7968e329c(__context__,(*__fa_rename_at_784_182).atype) )
                        {
                            return das_auto_cast<bool>::cast(true);
                        };
                    }
                    __fa_iterator.close(__context__,(__fa_rename_at_784_182));
                };
                if ( _FuncbuiltinTickkey_existsTick16808803843923989214_efda835c0359ec76(__context__,das_arg<TTable<char *,bool>>::pass(das_global<TTable<char *,bool>,0xd633af84de8bdc21>(__context__) /*blocked_functions_table*/),__fn_rename_at_771_181.cppName) )
                {
                    return das_auto_cast<bool>::cast(true);
                } else {
                    if ( das_global<Lambda DAS_COMMENT((bool,char * const )),0xdda227e969ec720f>(__context__) /*is_function_blocked*/ != nullptr )
                    {
                        if ( das_invoke_lambda<bool>::invoke<char * const >(__context__,nullptr,das_global<Lambda DAS_COMMENT((bool,char * const )),0xdda227e969ec720f>(__context__) /*is_function_blocked*/,__fn_rename_at_771_181.cppName) )
                        {
                            return das_auto_cast<bool>::cast(true);
                        };
                    };
                    return das_auto_cast<bool>::cast(false);
                };
            };
        };
    };
}

inline TArray<char *> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_354a9a0b57d554a0 ( Context * __context__, TArray<char *> &  __a_rename_at_50_183 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_28,cast<TArray<char *> &>::from(__a_rename_at_50_183)));
    return das_auto_cast_ref<TArray<char *> &>::cast(__a_rename_at_50_183);
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_79f9e841b4434fd8 ( Context * __context__, TArray<char *> &  __a_rename_at_1182_184 )
{
    builtin_array_free(das_arg<TArray<char *>>::pass(__a_rename_at_1182_184),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick5454204887383796109_9c4083566563a85a ( Context * __context__, TTable<char *,bool> &  __a_rename_at_1193_185 )
{
    builtin_table_free(das_arg<TTable<char *,bool>>::pass(__a_rename_at_1193_185),8,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline TArray<char *> _FuncbuiltinTickto_array_moveTick3185538323411982277_9285ff1255c3d503 ( Context * __context__, TDim<char *,1> &  __a_rename_at_1342_186 )
{
    TArray<char *> __arr_rename_at_1344_187;das_zero(__arr_rename_at_1344_187);
    _FuncbuiltinTickresizeTick4811697762258667383_14682004b38ad2a1(__context__,das_arg<TArray<char *>>::pass(__arr_rename_at_1344_187),1);
    das_copy(das_cast<TDim<char *,1>>::cast(das_ref(__context__,__arr_rename_at_1344_187(0,__context__))),__a_rename_at_1342_186);
    return /* <- */ das_auto_cast_move<TArray<char *>>::cast(__arr_rename_at_1344_187);
}

inline Sequence DAS_COMMENT((cpp_gen::ClangEnum &)) _FuncbuiltinTickvaluesTick1351216622833168869_93f4fc8357f87453 ( Context * __context__, TTable<char *,cpp_gen::ClangEnum> &  __a_rename_at_1150_188 )
{
    Sequence DAS_COMMENT((cpp_gen::ClangEnum *)) __it_rename_at_1151_189;das_zero(__it_rename_at_1151_189);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((cpp_gen::ClangEnum))>::pass(__it_rename_at_1151_189),das_arg<TTable<char *,cpp_gen::ClangEnum>>::pass(__a_rename_at_1150_188),56,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((cpp_gen::ClangEnum &))>::cast(__it_rename_at_1151_189);
}

inline Sequence DAS_COMMENT((cpp_gen::ClangStruct &)) _FuncbuiltinTickvaluesTick1351216622833168869_a5f65aa1a7088367 ( Context * __context__, TTable<char *,cpp_gen::ClangStruct> &  __a_rename_at_1150_190 )
{
    Sequence DAS_COMMENT((cpp_gen::ClangStruct *)) __it_rename_at_1151_191;das_zero(__it_rename_at_1151_191);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((cpp_gen::ClangStruct))>::pass(__it_rename_at_1151_191),das_arg<TTable<char *,cpp_gen::ClangStruct>>::pass(__a_rename_at_1150_190),72,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((cpp_gen::ClangStruct &))>::cast(__it_rename_at_1151_191);
}

inline bool _FuncbuiltinTickgetTick8447005936052527643_30d18a39e61d48bc ( Context * __context__, TTable<char *,char *> &  __Tab_rename_at_654_192, char * const  __at_rename_at_654_193, Block DAS_COMMENT((void,char * &)) const  &  __blk_rename_at_654_194 )
{
    char * * __val_rename_at_655_195 = __builtin_table_find(__context__,__Tab_rename_at_654_192,__at_rename_at_654_193);
    if ( __val_rename_at_655_195 != nullptr )
    {
        builtin_table_lock(das_arg<TTable<char *,char *>>::pass(__Tab_rename_at_654_192),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        das_invoke<void>::invoke<char * &>(__context__,nullptr,__blk_rename_at_654_194,das_deref(__context__,das_cast<char * *>::cast(__val_rename_at_655_195)));
        builtin_table_unlock(das_arg<TTable<char *,char *>>::pass(__Tab_rename_at_654_192),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        return das_auto_cast<bool>::cast(true);
    } else {
        return das_auto_cast<bool>::cast(false);
    };
}

inline Sequence DAS_COMMENT((cpp_gen::ClangFunc &)) _FuncbuiltinTickvaluesTick1351216622833168869_7c0785e58fa9645f ( Context * __context__, TTable<char *,cpp_gen::ClangFunc> &  __a_rename_at_1150_196 )
{
    Sequence DAS_COMMENT((cpp_gen::ClangFunc *)) __it_rename_at_1151_197;das_zero(__it_rename_at_1151_197);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((cpp_gen::ClangFunc))>::pass(__it_rename_at_1151_197),das_arg<TTable<char *,cpp_gen::ClangFunc>>::pass(__a_rename_at_1150_196),72,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((cpp_gen::ClangFunc &))>::cast(__it_rename_at_1151_197);
}

inline Sequence DAS_COMMENT((char *)) _FuncbuiltinTickkeysTick2205854368403803976_2df91d3f4a5c8888 ( Context * __context__, TTable<char *,char *> const  &  __a_rename_at_1128_198 )
{
    Sequence DAS_COMMENT((char *)) __it_rename_at_1129_199;das_zero(__it_rename_at_1129_199);
    builtin_table_keys(das_arg<Sequence DAS_COMMENT((char *))>::pass(__it_rename_at_1129_199),__a_rename_at_1128_198,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((char *))>::cast(__it_rename_at_1129_199);
}

inline Sequence DAS_COMMENT((char * &)) _FuncbuiltinTickvaluesTick1351216622833168869_16866e32c6607afb ( Context * __context__, TTable<char *,char *> &  __a_rename_at_1150_200 )
{
    Sequence DAS_COMMENT((char * *)) __it_rename_at_1151_201;das_zero(__it_rename_at_1151_201);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((char *))>::pass(__it_rename_at_1151_201),das_arg<TTable<char *,char *>>::pass(__a_rename_at_1150_200),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((char * &))>::cast(__it_rename_at_1151_201);
}

inline char * _Funccpp_genTickqType2FnTypeClassTick18208979152004666060_cdb79d2da470a00b ( Context * __context__, char * const  __st_rename_at_806_202, char * const  __className_rename_at_806_203 )
{
    int32_t __ob_rename_at_807_204 = ((int32_t)builtin_string_find2(__st_rename_at_806_202,((char *) "(")));
    DAS_ASSERT((__ob_rename_at_807_204 != -1));
    return das_auto_cast<char *>::cast(cast<char *>::to(SimPolicy<char *>::Add(cast<char *>::from((cast<char *>::to(SimPolicy<char *>::Add(cast<char *>::from(((char * const )(builtin_string_slice1(__st_rename_at_806_202,0,__ob_rename_at_807_204,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))),cast<char *>::from(das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_29, cast<char * const >::from(((char *) "(")), cast<char * const >::from(__className_rename_at_806_203), cast<char * const >::from(((char *) "::*)"))))),*__context__,nullptr)))),cast<char *>::from(((char * const )(builtin_string_slice2(__st_rename_at_806_202,__ob_rename_at_807_204,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))),*__context__,nullptr)));
}

inline char * _Funccpp_genTickqType2FnTypeTick1482691952773259010_a02b5143c6271bc3 ( Context * __context__, char * const  __st_rename_at_800_205 )
{
    int32_t __ob_rename_at_801_206 = ((int32_t)builtin_string_find2(__st_rename_at_800_205,((char *) "(")));
    DAS_ASSERT((__ob_rename_at_801_206 != -1));
    return das_auto_cast<char *>::cast(cast<char *>::to(SimPolicy<char *>::Add(cast<char *>::from((cast<char *>::to(SimPolicy<char *>::Add(cast<char *>::from(((char * const )(builtin_string_slice1(__st_rename_at_800_205,0,__ob_rename_at_801_206,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))),cast<char *>::from(((char *) "(*)")),*__context__,nullptr)))),cast<char *>::from(((char * const )(builtin_string_slice2(__st_rename_at_800_205,__ob_rename_at_801_206,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))),*__context__,nullptr)));
}

inline bool _Funccpp_genTickisRefFunTick11632776912375340809_d4070b3dd9da7cd2 ( Context * __context__, cpp_gen::ClangFunc const  &  __fn_rename_at_822_207 )
{
    int32_t __ob_rename_at_823_208 = ((int32_t)builtin_string_find2(__fn_rename_at_822_207.ftype,((char *) "(")));
    DAS_ASSERT((__ob_rename_at_823_208 != -1));
    char * __cppResType_rename_at_825_209 = (char *)(((char * const )(builtin_string_slice1(__fn_rename_at_822_207.ftype,0,__ob_rename_at_823_208,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
    if ( builtin_string_startswith(__cppResType_rename_at_825_209,((char *) "const "),__context__) )
    {
        das_copy(__cppResType_rename_at_825_209,((char * const )(builtin_string_slice2(__cppResType_rename_at_825_209,6,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
    };
    if ( builtin_string_endswith(__cppResType_rename_at_825_209,((char *) " "),__context__) )
    {
        das_copy(__cppResType_rename_at_825_209,((char * const )(builtin_string_slice1(__cppResType_rename_at_825_209,0,-1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
    };
    return das_auto_cast<bool>::cast(builtin_string_endswith(__cppResType_rename_at_825_209,((char *) "&"),__context__) ? das_auto_cast<bool>::cast(true) : das_auto_cast<bool>::cast(false));
}

inline bool _Funccpp_genTickisCmresFunTick16266856454814809519_86372ea3375f1a8c ( Context * __context__, cpp_gen::ClangFunc const  &  __fn_rename_at_838_210 )
{
    Sequence DAS_COMMENT((char *)) _temp_make_local_849_18_64; _temp_make_local_849_18_64;
    int32_t __ob_rename_at_840_211 = ((int32_t)builtin_string_find2(__fn_rename_at_838_210.ftype,((char *) "(")));
    DAS_ASSERT((__ob_rename_at_840_211 != -1));
    char * __cppResType_rename_at_842_212 = (char *)(((char * const )(builtin_string_slice1(__fn_rename_at_838_210.ftype,0,__ob_rename_at_840_211,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
    if ( builtin_string_startswith(__cppResType_rename_at_842_212,((char *) "const "),__context__) )
    {
        das_copy(__cppResType_rename_at_842_212,((char * const )(builtin_string_slice2(__cppResType_rename_at_842_212,6,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
    };
    if ( builtin_string_endswith(__cppResType_rename_at_842_212,((char *) " "),__context__) )
    {
        das_copy(__cppResType_rename_at_842_212,((char * const )(builtin_string_slice1(__cppResType_rename_at_842_212,0,-1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
    };
    {
        bool __need_loop_849 = true;
        // atype: string
        das_iterator<Sequence DAS_COMMENT((char *))> __atype_iterator((_temp_make_local_849_18_64 = (_FuncbuiltinTickkeysTick2205854368403803976_2df91d3f4a5c8888(__context__,das_arg<TTable<char *,char *>>::pass(das_global<TTable<char *,char *>,0x5f4171982abde6b1>(__context__) /*alias_types*/)))));
        char * __atype_rename_at_849_213;
        __need_loop_849 = __atype_iterator.first(__context__,(__atype_rename_at_849_213)) && __need_loop_849;
        for ( ; __need_loop_849 ; __need_loop_849 = __atype_iterator.next(__context__,(__atype_rename_at_849_213)) )
        {
            if ( SimPolicy<char *>::Equ(cast<char *>::from(__cppResType_rename_at_842_212),cast<char *>::from(__atype_rename_at_849_213),*__context__,nullptr) )
            {
                return das_auto_cast<bool>::cast(false);
            };
        }
        __atype_iterator.close(__context__,(__atype_rename_at_849_213));
    };
    if ( _FuncbuiltinTickkey_existsTick16808803843923989214_9b8b56716258076a(__context__,das_arg<TTable<char *,cpp_gen::ClangStruct>>::pass(das_global<cpp_gen::ClangAst,0x3d3f7dbddddbae8e>(__context__) /*ast*/.structs),__cppResType_rename_at_842_212) )
    {
        return das_auto_cast<bool>::cast(true);
    } else {
        if ( das_global<Lambda DAS_COMMENT((char *,char * const )),0x3451b360923f86eb>(__context__) /*alt_struct_name*/ != nullptr )
        {
            char * __aname_rename_at_858_214 = ((char *)(char *)(das_invoke_lambda<char *>::invoke<char *>(__context__,nullptr,das_global<Lambda DAS_COMMENT((char *,char * const )),0x3451b360923f86eb>(__context__) /*alt_struct_name*/,__cppResType_rename_at_842_212)));
            if ( SimPolicy<char *>::NotEqu(cast<char *>::from(__aname_rename_at_858_214),cast<char *>::from(nullptr),*__context__,nullptr) )
            {
                if ( _FuncbuiltinTickkey_existsTick16808803843923989214_9b8b56716258076a(__context__,das_arg<TTable<char *,cpp_gen::ClangStruct>>::pass(das_global<cpp_gen::ClangAst,0x3d3f7dbddddbae8e>(__context__) /*ast*/.structs),__aname_rename_at_858_214) )
                {
                    return das_auto_cast<bool>::cast(true);
                };
            };
        };
        {
            bool __need_loop_865 = true;
            // bf: string&
            das_iterator<TArray<char *>> __bf_iterator(das_global<TArray<char *>,0x41b00c9a154fb7c5>(__context__) /*cmres_functions*/);
            char * * __bf_rename_at_865_215;
            __need_loop_865 = __bf_iterator.first(__context__,(__bf_rename_at_865_215)) && __need_loop_865;
            for ( ; __need_loop_865 ; __need_loop_865 = __bf_iterator.next(__context__,(__bf_rename_at_865_215)) )
            {
                if ( SimPolicy<char *>::Equ(cast<char *>::from((*__bf_rename_at_865_215)),cast<char *>::from(__fn_rename_at_838_210.cppName),*__context__,nullptr) )
                {
                    return das_auto_cast<bool>::cast(true);
                };
            }
            __bf_iterator.close(__context__,(__bf_rename_at_865_215));
        };
        return das_auto_cast<bool>::cast(false);
    };
}

inline Sequence DAS_COMMENT((char *)) _FuncbuiltinTickkeysTick2205854368403803976_bcec88c808d50381 ( Context * __context__, TTable<char *,cpp_gen::ExEnum> const  &  __a_rename_at_1128_216 )
{
    Sequence DAS_COMMENT((char *)) __it_rename_at_1129_217;das_zero(__it_rename_at_1129_217);
    builtin_table_keys(das_arg<Sequence DAS_COMMENT((char *))>::pass(__it_rename_at_1129_217),__a_rename_at_1128_216,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((char *))>::cast(__it_rename_at_1129_217);
}

inline Sequence DAS_COMMENT((cpp_gen::ExEnum &)) _FuncbuiltinTickvaluesTick1351216622833168869_7282d82fa9dd61d5 ( Context * __context__, TTable<char *,cpp_gen::ExEnum> &  __a_rename_at_1150_218 )
{
    Sequence DAS_COMMENT((cpp_gen::ExEnum *)) __it_rename_at_1151_219;das_zero(__it_rename_at_1151_219);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((cpp_gen::ExEnum))>::pass(__it_rename_at_1151_219),das_arg<TTable<char *,cpp_gen::ExEnum>>::pass(__a_rename_at_1150_218),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((cpp_gen::ExEnum &))>::cast(__it_rename_at_1151_219);
}

inline void _FuncfioTickfopenTick3937565566638487747_feeb71cd63fd8eac ( Context * __context__, char * const  __name_rename_at_12_220, char * const  __mode_rename_at_12_221, Block DAS_COMMENT((void,FILE const  * const )) const  &  __blk_rename_at_12_222 )
{
    FILE const  * __f_rename_at_13_223 = ((FILE const  *)builtin_fopen(__name_rename_at_12_220,__mode_rename_at_12_221));
    das_invoke<void>::invoke<FILE const  * const >(__context__,nullptr,__blk_rename_at_12_222,__f_rename_at_13_223);
    if ( __f_rename_at_13_223 != nullptr )
    {
        builtin_fclose(__f_rename_at_13_223,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_efda835c0359ec76 ( Context * __context__, TTable<char *,bool> const  &  __Tab_rename_at_1014_224, char * const  __at_rename_at_1014_225 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1014_224,__at_rename_at_1014_225));
}

inline bool isBlockedType_cc1ed1c7968e329c ( Context * __context__, char * const  __qtype_rename_at_566_226 )
{
    {
        bool __need_loop_567 = true;
        // btn: string&
        das_iterator<TArray<char *>> __btn_iterator(das_global<TArray<char *>,0xbf1aa9ac043ebf89>(__context__) /*blocked_type_names*/);
        char * * __btn_rename_at_567_227;
        __need_loop_567 = __btn_iterator.first(__context__,(__btn_rename_at_567_227)) && __need_loop_567;
        for ( ; __need_loop_567 ; __need_loop_567 = __btn_iterator.next(__context__,(__btn_rename_at_567_227)) )
        {
            if ( builtin_string_find2(__qtype_rename_at_566_226,(*__btn_rename_at_567_227)) != -1 )
            {
                return das_auto_cast<bool>::cast(true);
            };
        }
        __btn_iterator.close(__context__,(__btn_rename_at_567_227));
    };
    return das_auto_cast<bool>::cast(false);
}

inline bool isLocalType_36973d575e8241e5 ( Context * __context__, char * const  __qtype_rename_at_575_228 )
{
    {
        bool __need_loop_576 = true;
        // btn: string&
        das_iterator<TArray<char *>> __btn_iterator(das_global<TArray<char *>,0x7047550f5f3a4677>(__context__) /*local_type_names*/);
        char * * __btn_rename_at_576_229;
        __need_loop_576 = __btn_iterator.first(__context__,(__btn_rename_at_576_229)) && __need_loop_576;
        for ( ; __need_loop_576 ; __need_loop_576 = __btn_iterator.next(__context__,(__btn_rename_at_576_229)) )
        {
            if ( builtin_string_find2(__qtype_rename_at_575_228,(*__btn_rename_at_576_229)) != -1 )
            {
                return das_auto_cast<bool>::cast(true);
            };
        }
        __btn_iterator.close(__context__,(__btn_rename_at_576_229));
    };
    return das_auto_cast<bool>::cast(false);
}

inline void genEnums_816c948d88e68308 ( Context * __context__, FILE const  * const  __hf_rename_at_585_230, FILE const  * const  __mf_rename_at_585_231, FILE const  * const  __df_rename_at_585_232 ) { das_stack_prologue __prologue(__context__,288,"genEnums " DAS_FILE_LINE);
{
    Sequence DAS_COMMENT((cpp_gen::ClangEnum *)) _temp_make_local_589_15_32; _temp_make_local_589_15_32;
    builtin_fprint(__hf_rename_at_585_230,((char *) "// ------------\n// enumerations\n// ------------\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fprint(__mf_rename_at_585_231,((char *) "// ------------\n// enumerations\n// ------------\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fprint(__df_rename_at_585_232,((char *) "// ------------\n// enumerations\n// ------------\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    {
        bool __need_loop_589 = true;
        // en: cpp_gen::ClangEnum&
        das_iterator<Sequence DAS_COMMENT((cpp_gen::ClangEnum))> __en_iterator((_temp_make_local_589_15_32 = (_FuncbuiltinTickvaluesTick1351216622833168869_93f4fc8357f87453(__context__,das_arg<TTable<char *,cpp_gen::ClangEnum>>::pass(das_global<cpp_gen::ClangAst,0x3d3f7dbddddbae8e>(__context__) /*ast*/.enums)))));
        cpp_gen::ClangEnum * __en_rename_at_589_233;
        __need_loop_589 = __en_iterator.first(__context__,(__en_rename_at_589_233)) && __need_loop_589;
        for ( ; __need_loop_589 ; __need_loop_589 = __en_iterator.next(__context__,(__en_rename_at_589_233)) )
        {
            char * __enName_rename_at_590_234 = (char *)((*__en_rename_at_589_233).name);
            bool __removePrefix_rename_at_591_235 = false;
            bool __isFlags_rename_at_592_236 = false;
            _FuncbuiltinTickgetTick8447005936052527643_c25c472f7c63dd94(__context__,das_arg<TTable<char *,cpp_gen::ExEnum>>::pass(das_global<TTable<char *,cpp_gen::ExEnum>,0x3b1deb0a06e30b10>(__context__) /*ex_enums*/),(*__en_rename_at_589_233).name,das_make_block<void,cpp_gen::ExEnum &>(__context__,208,0,&__func_info__2cd99deec9605fd9,[&](cpp_gen::ExEnum & __val_rename_at_593_237) -> void{
                das_copy(__enName_rename_at_590_234,__val_rename_at_593_237.name);
                das_copy(__removePrefix_rename_at_591_235,__val_rename_at_593_237.removePrefix);
                das_copy(__isFlags_rename_at_592_236,__val_rename_at_593_237.isFlags);
            }));
            if ( _FuncbuiltinTickkey_existsTick16808803843923989214_efda835c0359ec76(__context__,das_arg<TTable<char *,bool>>::pass(das_global<TTable<char *,bool>,0x1167f377740b3cc>(__context__) /*blocked_enumerations_table*/),(*__en_rename_at_589_233).cppName) )
            {
                continue;
            } else {
                builtin_fprint(__df_rename_at_585_232,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_30, cast<char * const >::from(((char *) "#ifdef ")), cast<char * const >::from(getDefinePrefix_68940f3e2e3e9e89(__context__,(*__en_rename_at_589_233).cppName)), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                builtin_fprint(__df_rename_at_585_232,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_31, cast<char * const >::from(((char *) "addEnumeration(make_smart<Enumeration")), cast<char *>::from(__enName_rename_at_590_234), cast<char * const >::from(((char *) ">());\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                if ( __isFlags_rename_at_592_236 )
                {
                    builtin_fprint(__df_rename_at_585_232,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_32, cast<char * const >::from(((char *) "addEnumFlagOps<")), cast<char *>::from((*__en_rename_at_589_233).cppName), cast<char * const >::from(((char *) ">(*this,lib,\"")), cast<char *>::from((*__en_rename_at_589_233).cppName), cast<char * const >::from(((char *) "\");\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                };
                builtin_fprint(__df_rename_at_585_232,((char *) "#endif\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                if ( (*__en_rename_at_589_233).eclass )
                {
                    builtin_fprint(__hf_rename_at_585_230,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_33, cast<char * const >::from(((char *) "BIND_ENUM_CAST(")), cast<char *>::from((*__en_rename_at_589_233).cppName), cast<char * const >::from(((char *) ")\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    builtin_fprint(__hf_rename_at_585_230,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_34, cast<char * const >::from(((char *) "DAS_BASE_BIND_ENUM_GEN(")), cast<char *>::from((*__en_rename_at_589_233).cppName), cast<char * const >::from(((char *) ",")), cast<char *>::from(__enName_rename_at_590_234), cast<char * const >::from(((char *) ")\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                } else {
                    builtin_fprint(__hf_rename_at_585_230,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_35, cast<char * const >::from(((char *) "DAS_BIND_ENUM_CAST(")), cast<char *>::from((*__en_rename_at_589_233).cppName), cast<char * const >::from(((char *) ")\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    builtin_fprint(__hf_rename_at_585_230,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_36, cast<char * const >::from(((char *) "DAS_BASE_BIND_ENUM_GEN(")), cast<char *>::from((*__en_rename_at_589_233).cppName), cast<char * const >::from(((char *) ",")), cast<char *>::from(__enName_rename_at_590_234), cast<char * const >::from(((char *) ")\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                };
                builtin_fprint(__mf_rename_at_585_231,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_37, cast<char * const >::from(((char *) "#ifdef ")), cast<char * const >::from(getDefinePrefix_68940f3e2e3e9e89(__context__,(*__en_rename_at_589_233).cppName)), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                builtin_fprint(__mf_rename_at_585_231,das_string_builder_temp(__context__,SimNode_AotInterop<11>(__tinfo_38, cast<char * const >::from(((char *) "\nclass Enumeration")), cast<char *>::from(__enName_rename_at_590_234), cast<char * const >::from(((char *) " : public das::Enumeration {\npublic:\n    Enumeration")), cast<char *>::from(__enName_rename_at_590_234), cast<char * const >::from(((char *) "() : das::Enumeration(\"")), cast<char *>::from(__enName_rename_at_590_234), cast<char * const >::from(((char *) "\") {\n        external = true\n        cppName = \"")), cast<char *>::from((*__en_rename_at_589_233).cppName), cast<char * const >::from(((char *) "\"\n        baseType = (das::Type) das::ToBasicType< das::underlying_type< ")), cast<char *>::from((*__en_rename_at_589_233).cppName), cast<char * const >::from(((char *) " >::type >::type\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                char * __uename_rename_at_623_238 = ((char *)(char *)(((char * const )(builtin_string_toupper((*__en_rename_at_589_233).name,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                {
                    bool __need_loop_624 = true;
                    // ene: string&
                    das_iterator<TArray<char *>> __ene_iterator((*__en_rename_at_589_233).edecl);
                    char * * __ene_rename_at_624_239;
                    __need_loop_624 = __ene_iterator.first(__context__,(__ene_rename_at_624_239)) && __need_loop_624;
                    for ( ; __need_loop_624 ; __need_loop_624 = __ene_iterator.next(__context__,(__ene_rename_at_624_239)) )
                    {
                        char * __entryName_rename_at_625_240 = (char *)((*__ene_rename_at_624_239));
                        char * __entryCppName_rename_at_626_241 = (char *)((*__ene_rename_at_624_239));
                        if ( __removePrefix_rename_at_591_235 )
                        {
                            if ( builtin_string_startswith((*__ene_rename_at_624_239),das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_39, cast<char *>::from((*__en_rename_at_589_233).name), cast<char * const >::from(((char *) "_")))),__context__) )
                            {
                                das_copy(__entryName_rename_at_625_240,((char * const )(builtin_string_slice2((*__ene_rename_at_624_239),builtin_string_length((*__en_rename_at_589_233).name,__context__) + 1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
                            } else if ( builtin_string_startswith((*__ene_rename_at_624_239),(*__en_rename_at_589_233).name,__context__) )
                            {
                                das_copy(__entryName_rename_at_625_240,((char * const )(builtin_string_slice2((*__ene_rename_at_624_239),builtin_string_length((*__en_rename_at_589_233).name,__context__),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
                            } else if ( builtin_string_startswith((*__ene_rename_at_624_239),das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_40, cast<char * const >::from(__uename_rename_at_623_238), cast<char * const >::from(((char *) "_")))),__context__) )
                            {
                                das_copy(__entryName_rename_at_625_240,((char * const )(builtin_string_slice2((*__ene_rename_at_624_239),builtin_string_length((*__en_rename_at_589_233).name,__context__) + 1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
                            } else if ( builtin_string_startswith((*__ene_rename_at_624_239),__uename_rename_at_623_238,__context__) )
                            {
                                das_copy(__entryName_rename_at_625_240,((char * const )(builtin_string_slice2((*__ene_rename_at_624_239),builtin_string_length((*__en_rename_at_589_233).name,__context__),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
                            } else {
                                builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_41, cast<char * const >::from(((char *) "warning: ")), cast<char *>::from((*__ene_rename_at_624_239)), cast<char * const >::from(((char *) " does not start with ")), cast<char *>::from((*__en_rename_at_589_233).name), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                            };
                        };
                        if ( _FuncbuiltinTickkey_existsTick16808803843923989214_efda835c0359ec76(__context__,das_arg<TTable<char *,bool>>::pass(das_global<TTable<char *,bool>,0xff4ca70677f05db6>(__context__) /*ex_keywords*/),__entryName_rename_at_625_240) )
                        {
                            das_copy(__entryName_rename_at_625_240,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_42, cast<char * const >::from(((char *) "_")), cast<char *>::from(__entryName_rename_at_625_240))));
                        };
                        builtin_fprint(__mf_rename_at_585_231,das_string_builder_temp(__context__,SimNode_AotInterop<9>(__tinfo_43, cast<char * const >::from(((char *) "\t\taddIEx(\"")), cast<char *>::from(__entryName_rename_at_625_240), cast<char * const >::from(((char *) "\", \"")), cast<char *>::from(__entryCppName_rename_at_626_241), cast<char * const >::from(((char *) "\", int64_t(")), cast<char *>::from((*__en_rename_at_589_233).cppName), cast<char * const >::from(((char *) "::")), cast<char *>::from((*__ene_rename_at_624_239)), cast<char * const >::from(((char *) "), das::LineInfo());\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    }
                    __ene_iterator.close(__context__,(__ene_rename_at_624_239));
                };
                builtin_fprint(__mf_rename_at_585_231,((char *) "\t}\n};\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                builtin_fprint(__mf_rename_at_585_231,((char *) "#endif\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            };
        }
        __en_iterator.close(__context__,(__en_rename_at_589_233));
    };
    builtin_fprint(__hf_rename_at_585_230,((char *) "\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fprint(__df_rename_at_585_232,((char *) "\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}}

inline char * getBindFldName_4490111cebfe248e ( Context * __context__, char * const  __name_rename_at_677_242 )
{
    return das_auto_cast<char *>::cast((SimPolicy<char *>::Equ(cast<char *>::from(__name_rename_at_677_242),cast<char *>::from(((char *) "type")),*__context__,nullptr)) ? das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_44, cast<char * const >::from(((char *) "_")), cast<char * const >::from(__name_rename_at_677_242)))) : das_auto_cast<char *>::cast(__name_rename_at_677_242));
}

inline void genStructs_e7c6b33a2f752b8e ( Context * __context__, FILE const  * const  __hf_rename_at_684_243, FILE const  * const  __mf_rename_at_684_244, FILE const  * const  __df_rename_at_684_245 ) { das_stack_prologue __prologue(__context__,432,"genStructs " DAS_FILE_LINE);
{
    Sequence DAS_COMMENT((cpp_gen::ClangStruct *)) _temp_make_local_688_15_32; _temp_make_local_688_15_32;
    Sequence DAS_COMMENT((cpp_gen::ClangStruct *)) _temp_make_local_699_15_128; _temp_make_local_699_15_128;
    Sequence DAS_COMMENT((cpp_gen::ClangStruct *)) _temp_make_local_754_15_336; _temp_make_local_754_15_336;
    builtin_fprint(__hf_rename_at_684_243,((char *) "// ------------\n// structures\n// ------------\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fprint(__mf_rename_at_684_244,((char *) "// ------------\n// structures\n// ------------\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fprint(__df_rename_at_684_245,((char *) "// ------------\n// structures\n// ------------\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    {
        bool __need_loop_688 = true;
        // st: cpp_gen::ClangStruct&
        das_iterator<Sequence DAS_COMMENT((cpp_gen::ClangStruct))> __st_iterator((_temp_make_local_688_15_32 = (_FuncbuiltinTickvaluesTick1351216622833168869_a5f65aa1a7088367(__context__,das_arg<TTable<char *,cpp_gen::ClangStruct>>::pass(das_global<cpp_gen::ClangAst,0x3d3f7dbddddbae8e>(__context__) /*ast*/.structs)))));
        cpp_gen::ClangStruct * __st_rename_at_688_246;
        __need_loop_688 = __st_iterator.first(__context__,(__st_rename_at_688_246)) && __need_loop_688;
        for ( ; __need_loop_688 ; __need_loop_688 = __st_iterator.next(__context__,(__st_rename_at_688_246)) )
        {
            if ( !_Funccpp_genTickneedToGenStructTick14158910623600840251_fd93d910f98eae6b(__context__,das_arg<cpp_gen::ClangStruct>::pass((*__st_rename_at_688_246))) )
            {
                continue;
            } else {
                if ( !_FuncbuiltinTickkey_existsTick16808803843923989214_1a54ecb7273c2da1(__context__,das_arg<TTable<char *,char *>>::pass(das_global<TTable<char *,char *>,0x5f4171982abde6b1>(__context__) /*alias_types*/),(*__st_rename_at_688_246).cppName) )
                {
                    builtin_fprint(__mf_rename_at_684_244,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_45, cast<char * const >::from(((char *) "#ifdef ")), cast<char * const >::from(getDefinePrefix_68940f3e2e3e9e89(__context__,(*__st_rename_at_688_246).cppName)), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    builtin_fprint(__mf_rename_at_684_244,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_46, cast<char * const >::from(((char *) "\tIMPLEMENT_EXTERNAL_TYPE_FACTORY(")), cast<char *>::from((*__st_rename_at_688_246).name), cast<char * const >::from(((char *) ",")), cast<char *>::from((*__st_rename_at_688_246).cppName), cast<char * const >::from(((char *) ")\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    builtin_fprint(__mf_rename_at_684_244,((char *) "#endif\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                };
            };
        }
        __st_iterator.close(__context__,(__st_rename_at_688_246));
    };
    builtin_fprint(__mf_rename_at_684_244,((char *) "\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    {
        bool __need_loop_699 = true;
        // st: cpp_gen::ClangStruct&
        das_iterator<Sequence DAS_COMMENT((cpp_gen::ClangStruct))> __st_iterator((_temp_make_local_699_15_128 = (_FuncbuiltinTickvaluesTick1351216622833168869_a5f65aa1a7088367(__context__,das_arg<TTable<char *,cpp_gen::ClangStruct>>::pass(das_global<cpp_gen::ClangAst,0x3d3f7dbddddbae8e>(__context__) /*ast*/.structs)))));
        cpp_gen::ClangStruct * __st_rename_at_699_247;
        __need_loop_699 = __st_iterator.first(__context__,(__st_rename_at_699_247)) && __need_loop_699;
        for ( ; __need_loop_699 ; __need_loop_699 = __st_iterator.next(__context__,(__st_rename_at_699_247)) )
        {
            if ( !_Funccpp_genTickneedToGenStructTick14158910623600840251_fd93d910f98eae6b(__context__,das_arg<cpp_gen::ClangStruct>::pass((*__st_rename_at_699_247))) )
            {
                continue;
            } else {
                if ( _FuncbuiltinTickkey_existsTick16808803843923989214_1a54ecb7273c2da1(__context__,das_arg<TTable<char *,char *>>::pass(das_global<TTable<char *,char *>,0x5f4171982abde6b1>(__context__) /*alias_types*/),(*__st_rename_at_699_247).cppName) )
                {
                    continue;
                } else {
                    builtin_fprint(__df_rename_at_684_245,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_47, cast<char * const >::from(((char *) "#ifdef ")), cast<char * const >::from(getDefinePrefix_68940f3e2e3e9e89(__context__,(*__st_rename_at_699_247).cppName)), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    builtin_fprint(__df_rename_at_684_245,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_48, cast<char * const >::from(((char *) "auto ann_")), cast<char *>::from((*__st_rename_at_699_247).name), cast<char * const >::from(((char *) " = make_smart<")), cast<char *>::from((*__st_rename_at_699_247).name), cast<char * const >::from(((char *) "_GeneratedAnnotation>(lib);\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    builtin_fprint(__df_rename_at_684_245,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_49, cast<char * const >::from(((char *) "addAnnotation(ann_")), cast<char *>::from((*__st_rename_at_699_247).name), cast<char * const >::from(((char *) ");\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    builtin_fprint(__df_rename_at_684_245,((char *) "#endif\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    builtin_fprint(__hf_rename_at_684_243,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_50, cast<char * const >::from(((char *) "MAKE_EXTERNAL_TYPE_FACTORY(")), cast<char *>::from((*__st_rename_at_699_247).name), cast<char * const >::from(((char *) ",")), cast<char *>::from((*__st_rename_at_699_247).cppName), cast<char * const >::from(((char *) ")\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    builtin_fprint(__mf_rename_at_684_244,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_51, cast<char * const >::from(((char *) "#ifdef ")), cast<char * const >::from(getDefinePrefix_68940f3e2e3e9e89(__context__,(*__st_rename_at_699_247).cppName)), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    builtin_fprint(__mf_rename_at_684_244,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_52, cast<char * const >::from(((char *) "struct ")), cast<char *>::from((*__st_rename_at_699_247).name), cast<char * const >::from(((char *) "_GeneratedAnnotation : ManagedStructureAnnotation<")), cast<char *>::from((*__st_rename_at_699_247).cppName), cast<char * const >::from(((char *) "> {\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    if ( isLocalType_36973d575e8241e5(__context__,(*__st_rename_at_699_247).name) )
                    {
                        builtin_fprint(__mf_rename_at_684_244,((char *) "\tvirtual bool isLocal() const override { return true; }\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                        builtin_fprint(__mf_rename_at_684_244,((char *) "\tvirtual bool canBePlacedInContainer() const override { return true; }\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    };
                    builtin_fprint(__mf_rename_at_684_244,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_53, cast<char * const >::from(((char *) "\t")), cast<char *>::from((*__st_rename_at_699_247).name), cast<char * const >::from(((char *) "_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation (\"")), cast<char *>::from((*__st_rename_at_699_247).name), cast<char * const >::from(((char *) "\", ml) {\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    builtin_fprint(__mf_rename_at_684_244,((char *) "\t}\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    builtin_fprint(__mf_rename_at_684_244,((char *) "\tvoid init () {\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    {
                        bool __need_loop_733 = true;
                        // fld: cpp_gen::ClangStructField&
                        das_iterator<TArray<cpp_gen::ClangStructField>> __fld_iterator((*__st_rename_at_699_247).fields);
                        cpp_gen::ClangStructField * __fld_rename_at_733_248;
                        __need_loop_733 = __fld_iterator.first(__context__,(__fld_rename_at_733_248)) && __need_loop_733;
                        for ( ; __need_loop_733 ; __need_loop_733 = __fld_iterator.next(__context__,(__fld_rename_at_733_248)) )
                        {
                            if ( !_Funccpp_genTickneedToGenFieldTick2521814960739047210_6a0d55d67bdff534(__context__,das_arg<cpp_gen::ClangStructField>::pass((*__fld_rename_at_733_248))) )
                            {
                                continue;
                            } else {
                                if ( (*__fld_rename_at_733_248).isBitfield )
                                {
                                    continue;
                                } else {
                                    char * __substType_rename_at_740_249 = (char *)(nullptr);
                                    _FuncbuiltinTickgetTick8447005936052527643_30d18a39e61d48bc(__context__,das_arg<TTable<char *,char *>>::pass(das_global<TTable<char *,char *>,0xc86829fad0577a95>(__context__) /*substitute_field_types*/),das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_54, cast<char *>::from((*__st_rename_at_699_247).cppName), cast<char * const >::from(((char *) "::")), cast<char *>::from((*__fld_rename_at_733_248).cppName))),das_make_block<void,char * &>(__context__,320,0,&__func_info__5f59ad38d8b49003,[&](char * & __val_rename_at_741_250) -> void{
                                        das_copy(__substType_rename_at_740_249,__val_rename_at_741_250);
                                    }));
                                    if ( builtin_empty(__substType_rename_at_740_249) )
                                    {
                                        builtin_fprint(__mf_rename_at_684_244,das_string_builder_temp(__context__,SimNode_AotInterop<7>(__tinfo_55, cast<char * const >::from(((char *) "\t\taddField<DAS_BIND_MANAGED_FIELD(")), cast<char *>::from((*__fld_rename_at_733_248).cppName), cast<char * const >::from(((char *) ")>(\"")), cast<char * const >::from(getBindFldName_4490111cebfe248e(__context__,(*__fld_rename_at_733_248).name)), cast<char * const >::from(((char *) "\",\"")), cast<char *>::from((*__fld_rename_at_733_248).cppName), cast<char * const >::from(((char *) "\");\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                                    } else {
                                        builtin_fprint(__mf_rename_at_684_244,das_string_builder_temp(__context__,SimNode_AotInterop<9>(__tinfo_56, cast<char * const >::from(((char *) "\t\taddField<")), cast<char *>::from(__substType_rename_at_740_249), cast<char * const >::from(((char *) ",offsetof(ManagedType,")), cast<char *>::from((*__fld_rename_at_733_248).cppName), cast<char * const >::from(((char *) ")>(\"")), cast<char * const >::from(getBindFldName_4490111cebfe248e(__context__,(*__fld_rename_at_733_248).name)), cast<char * const >::from(((char *) "\",\"")), cast<char *>::from((*__fld_rename_at_733_248).cppName), cast<char * const >::from(((char *) "\")\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                                        builtin_fprint(__mf_rename_at_684_244,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_57, cast<char * const >::from(((char *) "\t\t\t.adjustAot(\"das_reinterpret<")), cast<char *>::from(__substType_rename_at_740_249), cast<char * const >::from(((char *) ">::pass(\",\")\");\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                                    };
                                };
                            };
                        }
                        __fld_iterator.close(__context__,(__fld_rename_at_733_248));
                    };
                    builtin_fprint(__mf_rename_at_684_244,((char *) "\t}\n};\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    builtin_fprint(__mf_rename_at_684_244,((char *) "#endif\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                };
            };
        }
        __st_iterator.close(__context__,(__st_rename_at_699_247));
    };
    {
        bool __need_loop_754 = true;
        // st: cpp_gen::ClangStruct&
        das_iterator<Sequence DAS_COMMENT((cpp_gen::ClangStruct))> __st_iterator((_temp_make_local_754_15_336 = (_FuncbuiltinTickvaluesTick1351216622833168869_a5f65aa1a7088367(__context__,das_arg<TTable<char *,cpp_gen::ClangStruct>>::pass(das_global<cpp_gen::ClangAst,0x3d3f7dbddddbae8e>(__context__) /*ast*/.structs)))));
        cpp_gen::ClangStruct * __st_rename_at_754_251;
        __need_loop_754 = __st_iterator.first(__context__,(__st_rename_at_754_251)) && __need_loop_754;
        for ( ; __need_loop_754 ; __need_loop_754 = __st_iterator.next(__context__,(__st_rename_at_754_251)) )
        {
            if ( !_Funccpp_genTickneedToGenStructTick14158910623600840251_fd93d910f98eae6b(__context__,das_arg<cpp_gen::ClangStruct>::pass((*__st_rename_at_754_251))) )
            {
                continue;
            } else {
                if ( _FuncbuiltinTickkey_existsTick16808803843923989214_1a54ecb7273c2da1(__context__,das_arg<TTable<char *,char *>>::pass(das_global<TTable<char *,char *>,0x5f4171982abde6b1>(__context__) /*alias_types*/),(*__st_rename_at_754_251).cppName) )
                {
                    continue;
                } else {
                    builtin_fprint(__df_rename_at_684_245,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_58, cast<char * const >::from(((char *) "#ifdef ")), cast<char * const >::from(getDefinePrefix_68940f3e2e3e9e89(__context__,(*__st_rename_at_754_251).cppName)), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    builtin_fprint(__df_rename_at_684_245,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_59, cast<char * const >::from(((char *) "initRecAnnotation(ann_")), cast<char *>::from((*__st_rename_at_754_251).name), cast<char * const >::from(((char *) ",lib);\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    builtin_fprint(__df_rename_at_684_245,((char *) "#endif\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                };
            };
        }
        __st_iterator.close(__context__,(__st_rename_at_754_251));
    };
    builtin_fprint(__hf_rename_at_684_243,((char *) "\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fprint(__df_rename_at_684_245,((char *) "\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}}

inline char * methodName_ff1251eb1b75eec2 ( Context * __context__, char * const  __name_rename_at_875_252 )
{
    ++das_global<int32_t,0x89f4705947df9582>(__context__) /*g_method_name*/;
    return das_auto_cast<char *>::cast(das_string_builder(__context__,SimNode_AotInterop<4>(__tinfo_60, cast<char * const >::from(((char *) "_")), cast<char * const >::from(__name_rename_at_875_252), cast<char * const >::from(((char *) "_method_")), cast<int32_t>::from(das_global<int32_t,0x89f4705947df9582>(__context__) /*g_method_name*/))));
}

inline void openSplitFile_ea32c746c410d324 ( Context * __context__ )
{
    if ( (SimPolicy<int32_t>::Mod(das_global<int32_t,0x1baec29b32162c17>(__context__) /*g_split_count*/,das_global<int32_t,0x402eefb06d5e9cd0>(__context__) /*g_split_factor*/,*__context__,nullptr)) == 0 )
    {
        closeSplitFile_233b8cc7d4e11a96(__context__);
        int32_t __split_index_rename_at_887_253 = ((int32_t)(SimPolicy<int32_t>::Div(das_global<int32_t,0x1baec29b32162c17>(__context__) /*g_split_count*/,das_global<int32_t,0x402eefb06d5e9cd0>(__context__) /*g_split_factor*/,*__context__,nullptr)));
        char * __fname_rename_at_888_254 = ((char *)(char *)(das_string_builder(__context__,SimNode_AotInterop<4>(__tinfo_61, cast<char *>::from(das_global<char *,0x2572ec17c7f55c54>(__context__) /*split_prefix*/), cast<char * const >::from(((char *) "_")), cast<int32_t>::from(__split_index_rename_at_887_253), cast<char * const >::from(((char *) ".cpp"))))));
        builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_62, cast<char * const >::from(((char *) "SPLIT ")), cast<int32_t>::from(__split_index_rename_at_887_253), cast<char * const >::from(((char *) " at ")), cast<char * const >::from(__fname_rename_at_888_254), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        das_copy(das_global<FILE const  *,0x77423da7a96350b8>(__context__) /*g_split_file*/,builtin_fopen(__fname_rename_at_888_254,((char *) "wb")));
        char * __prefix_rename_at_891_255 = (char *)(((char * const )(builtin_string_replace(das_global<char *,0x2505284bc104585d>(__context__) /*split_cpp_prefix*/,((char *) "$$$"),das_string_builder_temp(__context__,SimNode_AotInterop<1>(__tinfo_63, cast<int32_t>::from(__split_index_rename_at_887_253))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
        builtin_fprint(das_global<FILE const  *,0x77423da7a96350b8>(__context__) /*g_split_file*/,__prefix_rename_at_891_255,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    ++das_global<int32_t,0x1baec29b32162c17>(__context__) /*g_split_count*/;
}

inline void closeSplitFile_233b8cc7d4e11a96 ( Context * __context__ )
{
    if ( das_global<FILE const  *,0x77423da7a96350b8>(__context__) /*g_split_file*/ != nullptr )
    {
        builtin_fprint(das_global<FILE const  *,0x77423da7a96350b8>(__context__) /*g_split_file*/,das_global<char *,0xe914b75272ad2062>(__context__) /*split_cpp_suffix*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        builtin_fclose(das_global<FILE const  *,0x77423da7a96350b8>(__context__) /*g_split_file*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        das_copy(das_global<FILE const  *,0x77423da7a96350b8>(__context__) /*g_split_file*/,nullptr);
    };
}

inline void getFunctions_26873a00d281aa69 ( Context * __context__, FILE const  * const  __df_rename_at_905_256, FILE const  * const  __df_method_rename_at_905_257 )
{
    Sequence DAS_COMMENT((cpp_gen::ClangFunc *)) _temp_make_local_908_15_80; _temp_make_local_908_15_80;
    Sequence DAS_COMMENT((cpp_gen::ClangFunc *)) _temp_make_local_911_15_176; _temp_make_local_911_15_176;
    Sequence DAS_COMMENT((cpp_gen::ClangStruct *)) _temp_make_local_924_15_288; _temp_make_local_924_15_288;
    builtin_fprint(__df_rename_at_905_256,((char *) "// ------------\n// functions\n// ------------\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    TTable<char *,int32_t> __all_fn_rename_at_907_258;das_zero(__all_fn_rename_at_907_258);
    {
        bool __need_loop_908 = true;
        // fn: cpp_gen::ClangFunc&
        das_iterator<Sequence DAS_COMMENT((cpp_gen::ClangFunc))> __fn_iterator((_temp_make_local_908_15_80 = (_FuncbuiltinTickvaluesTick1351216622833168869_7c0785e58fa9645f(__context__,das_arg<TTable<char *,cpp_gen::ClangFunc>>::pass(das_global<cpp_gen::ClangAst,0x3d3f7dbddddbae8e>(__context__) /*ast*/.funcs)))));
        cpp_gen::ClangFunc * __fn_rename_at_908_259;
        __need_loop_908 = __fn_iterator.first(__context__,(__fn_rename_at_908_259)) && __need_loop_908;
        for ( ; __need_loop_908 ; __need_loop_908 = __fn_iterator.next(__context__,(__fn_rename_at_908_259)) )
        {
            ++__all_fn_rename_at_907_258((*__fn_rename_at_908_259).cppName,__context__);
        }
        __fn_iterator.close(__context__,(__fn_rename_at_908_259));
    };
    {
        bool __need_loop_911 = true;
        // fn: cpp_gen::ClangFunc&
        das_iterator<Sequence DAS_COMMENT((cpp_gen::ClangFunc))> __fn_iterator((_temp_make_local_911_15_176 = (_FuncbuiltinTickvaluesTick1351216622833168869_7c0785e58fa9645f(__context__,das_arg<TTable<char *,cpp_gen::ClangFunc>>::pass(das_global<cpp_gen::ClangAst,0x3d3f7dbddddbae8e>(__context__) /*ast*/.funcs)))));
        cpp_gen::ClangFunc * __fn_rename_at_911_260;
        __need_loop_911 = __fn_iterator.first(__context__,(__fn_rename_at_911_260)) && __need_loop_911;
        for ( ; __need_loop_911 ; __need_loop_911 = __fn_iterator.next(__context__,(__fn_rename_at_911_260)) )
        {
            if ( _Funccpp_genTickisBlockedFunTick2478178585869301773_97ae78cd67b212a3(__context__,das_arg<cpp_gen::ClangFunc>::pass((*__fn_rename_at_911_260))) )
            {
                continue;
            } else {
                bool __castbind_rename_at_915_261 = ((bool)(__all_fn_rename_at_907_258((*__fn_rename_at_911_260).cppName,__context__) > 1));
                genFunction_d3eae2156dccabbe(__context__,das_arg<cpp_gen::ClangFunc>::pass((*__fn_rename_at_911_260)),__castbind_rename_at_915_261,__df_rename_at_905_256);
                if ( das_global<bool,0x14dfebd35adf67c4>(__context__) /*generate_split_functions*/ )
                {
                    openSplitFile_ea32c746c410d324(__context__);
                    genFunction_d3eae2156dccabbe(__context__,das_arg<cpp_gen::ClangFunc>::pass((*__fn_rename_at_911_260)),__castbind_rename_at_915_261,das_global<FILE const  *,0x77423da7a96350b8>(__context__) /*g_split_file*/);
                };
            };
        }
        __fn_iterator.close(__context__,(__fn_rename_at_911_260));
    };
    builtin_fprint(__df_method_rename_at_905_257,((char *) "// ------------\n// methods \n// ------------\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    {
        bool __need_loop_924 = true;
        // st: cpp_gen::ClangStruct&
        das_iterator<Sequence DAS_COMMENT((cpp_gen::ClangStruct))> __st_iterator((_temp_make_local_924_15_288 = (_FuncbuiltinTickvaluesTick1351216622833168869_a5f65aa1a7088367(__context__,das_arg<TTable<char *,cpp_gen::ClangStruct>>::pass(das_global<cpp_gen::ClangAst,0x3d3f7dbddddbae8e>(__context__) /*ast*/.structs)))));
        cpp_gen::ClangStruct * __st_rename_at_924_262;
        __need_loop_924 = __st_iterator.first(__context__,(__st_rename_at_924_262)) && __need_loop_924;
        for ( ; __need_loop_924 ; __need_loop_924 = __st_iterator.next(__context__,(__st_rename_at_924_262)) )
        {
            TTable<char *,int32_t> __all_met_rename_at_925_263;das_zero(__all_met_rename_at_925_263);
            {
                bool __need_loop_926 = true;
                // fn: cpp_gen::ClangFunc&
                das_iterator<TArray<cpp_gen::ClangFunc>> __fn_iterator((*__st_rename_at_924_262).methods);
                cpp_gen::ClangFunc * __fn_rename_at_926_264;
                __need_loop_926 = __fn_iterator.first(__context__,(__fn_rename_at_926_264)) && __need_loop_926;
                for ( ; __need_loop_926 ; __need_loop_926 = __fn_iterator.next(__context__,(__fn_rename_at_926_264)) )
                {
                    ++__all_met_rename_at_925_263((*__fn_rename_at_926_264).name,__context__);
                }
                __fn_iterator.close(__context__,(__fn_rename_at_926_264));
            };
            {
                bool __need_loop_929 = true;
                // fn: cpp_gen::ClangFunc&
                das_iterator<TArray<cpp_gen::ClangFunc>> __fn_iterator((*__st_rename_at_924_262).methods);
                cpp_gen::ClangFunc * __fn_rename_at_929_265;
                __need_loop_929 = __fn_iterator.first(__context__,(__fn_rename_at_929_265)) && __need_loop_929;
                for ( ; __need_loop_929 ; __need_loop_929 = __fn_iterator.next(__context__,(__fn_rename_at_929_265)) )
                {
                    if ( _Funccpp_genTickisBlockedFunTick2478178585869301773_97ae78cd67b212a3(__context__,das_arg<cpp_gen::ClangFunc>::pass((*__fn_rename_at_929_265))) )
                    {
                        continue;
                    } else {
                        bool __castbind_rename_at_933_266 = ((bool)(__all_met_rename_at_925_263((*__fn_rename_at_929_265).name,__context__) > 1));
                        genFunction_d3eae2156dccabbe(__context__,das_arg<cpp_gen::ClangFunc>::pass((*__fn_rename_at_929_265)),__castbind_rename_at_933_266,__df_method_rename_at_905_257);
                        if ( das_global<bool,0x14dfebd35adf67c4>(__context__) /*generate_split_functions*/ )
                        {
                            openSplitFile_ea32c746c410d324(__context__);
                            genFunction_d3eae2156dccabbe(__context__,das_arg<cpp_gen::ClangFunc>::pass((*__fn_rename_at_929_265)),__castbind_rename_at_933_266,das_global<FILE const  *,0x77423da7a96350b8>(__context__) /*g_split_file*/);
                        };
                    };
                }
                __fn_iterator.close(__context__,(__fn_rename_at_929_265));
            };
        }
        __st_iterator.close(__context__,(__st_rename_at_924_262));
    };
}

inline TArray<char *> parseQualArgs_3f5a273f6933fa8f ( Context * __context__, char * const  __qname_rename_at_947_267, char * & __rtype_rename_at_947_268 )
{
    if ( das_invoke_function<int32_t>::invoke<regex::Regex &,char * const ,int32_t>(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::regex_match S<regex::Regex> Cs Ci*/ 0x297b408d218ed2d)),das_arg<regex::Regex>::pass(das_global<regex::Regex,0x446bace93c1e6134>(__context__) /*reg_args*/),__qname_rename_at_947_267,0) != -1 )
    {
        TArray<char *> _temp_make_local_951_19_48; _temp_make_local_951_19_48;
        char * __qargs_rename_at_949_269 = ((char *)(char *)(das_invoke_function<char * const >::invoke<regex::Regex const  &,int32_t,char * const >(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::regex_group CS<regex::Regex> Ci Cs*/ 0x56501aba1922680a)),das_arg<regex::Regex>::pass(das_global<regex::Regex,0x446bace93c1e6134>(__context__) /*reg_args*/),2,__qname_rename_at_947_267)));
        das_copy(__rtype_rename_at_947_268,das_invoke_function<char * const >::invoke<regex::Regex const  &,int32_t,char * const >(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::regex_group CS<regex::Regex> Ci Cs*/ 0x56501aba1922680a)),das_arg<regex::Regex>::pass(das_global<regex::Regex,0x446bace93c1e6134>(__context__) /*reg_args*/),1,__qname_rename_at_947_267));
        return /* <- */ das_auto_cast_move<TArray<char *>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_354a9a0b57d554a0(__context__,das_arg<TArray<char *>>::pass((_temp_make_local_951_19_48 = (das_invoke_function<TArray<char *>>::invoke_cmres<char * const ,char * const >(__context__,nullptr,Func(__context__->fnByMangledName(/*@strings_boost::split CIs CIs*/ 0x427078f3f7fe6bb)),__qargs_rename_at_949_269,((char *) ", ")))))));
    } else {
        TArray<char *> _temp_make_local_953_20_80; _temp_make_local_953_20_80;
        return /* <- */ das_auto_cast_move<TArray<char *>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_354a9a0b57d554a0(__context__,das_arg<TArray<char *>>::pass((([&]() -> TArray<char *>& {
            das_zero(_temp_make_local_953_20_80);
            return _temp_make_local_953_20_80;
        })()))));
    };
}

inline char * getDefinePrefix_68940f3e2e3e9e89 ( Context * __context__, char * const  __xname_rename_at_956_270 )
{
    Sequence DAS_COMMENT((char *)) _temp_make_local_957_19_32; _temp_make_local_957_19_32;
    Sequence DAS_COMMENT((char * *)) _temp_make_local_957_46_48; _temp_make_local_957_46_48;
    {
        bool __need_loop_957 = true;
        // nn: string
        das_iterator<Sequence DAS_COMMENT((char *))> __nn_iterator((_temp_make_local_957_19_32 = (_FuncbuiltinTickkeysTick2205854368403803976_2df91d3f4a5c8888(__context__,das_arg<TTable<char *,char *>>::pass(das_global<TTable<char *,char *>,0xa8937f5fc0357310>(__context__) /*namespace_to_prefix*/)))));
        char * __nn_rename_at_957_273;
        __need_loop_957 = __nn_iterator.first(__context__,(__nn_rename_at_957_273)) && __need_loop_957;
        // pn: string&
        das_iterator<Sequence DAS_COMMENT((char *))> __pn_iterator((_temp_make_local_957_46_48 = (_FuncbuiltinTickvaluesTick1351216622833168869_16866e32c6607afb(__context__,das_arg<TTable<char *,char *>>::pass(das_global<TTable<char *,char *>,0xa8937f5fc0357310>(__context__) /*namespace_to_prefix*/)))));
        char * * __pn_rename_at_957_274;
        __need_loop_957 = __pn_iterator.first(__context__,(__pn_rename_at_957_274)) && __need_loop_957;
        for ( ; __need_loop_957 ; __need_loop_957 = __nn_iterator.next(__context__,(__nn_rename_at_957_273)) && __pn_iterator.next(__context__,(__pn_rename_at_957_274)) )
        {
            if ( builtin_string_startswith(__xname_rename_at_956_270,__nn_rename_at_957_273,__context__) )
            {
                return das_auto_cast<char *>::cast((*__pn_rename_at_957_274));
            };
        }
        __nn_iterator.close(__context__,(__nn_rename_at_957_273));
        __pn_iterator.close(__context__,(__pn_rename_at_957_274));
    };
    return das_auto_cast<char *>::cast(((char *) "GLOBAL_NAMESPACE"));
}

inline void genFunction_d3eae2156dccabbe ( Context * __context__, cpp_gen::ClangFunc const  &  __fn_rename_at_967_275, bool __castbind_rename_at_967_276, FILE const  * const  __df_rename_at_967_277 ) { das_stack_prologue __prologue(__context__,528,"genFunction " DAS_FILE_LINE);
{
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_1018_31_144; _temp_make_local_1018_31_144;
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_1048_32_352; _temp_make_local_1048_32_352;
    char * __bind_enchantation_rename_at_968_278 = (char *)(nullptr);
    char * __cpp_enchantation_rename_at_969_279 = (char *)(nullptr);
    char * __using_mname_rename_at_970_280 = (char *)(nullptr);
    char * __using_mname_expr_rename_at_971_281 = (char *)(nullptr);
    if ( das_global<Lambda DAS_COMMENT((void,cpp_gen::ClangFunc const )),0x4f69680403ea7c7>(__context__) /*gen_fn_callback*/ != nullptr )
    {
        das_invoke_lambda<void>::invoke<cpp_gen::ClangFunc const  &>(__context__,nullptr,das_global<Lambda DAS_COMMENT((void,cpp_gen::ClangFunc const )),0x4f69680403ea7c7>(__context__) /*gen_fn_callback*/,__fn_rename_at_967_275);
    };
    if ( __fn_rename_at_967_275.isMethod )
    {
        if ( __castbind_rename_at_967_276 )
        {
            char * __fType_rename_at_977_282 = ((char *)(char *)(_Funccpp_genTickqType2FnTypeClassTick18208979152004666060_cdb79d2da470a00b(__context__,__fn_rename_at_967_275.ftype,__fn_rename_at_967_275.ofClass)));
            das_copy(__using_mname_rename_at_970_280,methodName_ff1251eb1b75eec2(__context__,__fn_rename_at_967_275.name));
            das_copy(__using_mname_expr_rename_at_971_281,das_string_builder(__context__,SimNode_AotInterop<5>(__tinfo_64, cast<char * const >::from(((char *) "das::das_call_member<")), cast<char * const >::from(__fType_rename_at_977_282), cast<char * const >::from(((char *) ",&")), cast<char * const >::from(__fn_rename_at_967_275.cppName), cast<char * const >::from(((char *) ">")))));
            das_copy(__bind_enchantation_rename_at_968_278,das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_65, cast<char * const >::from(((char *) "DAS_CALL_METHOD(")), cast<char *>::from(__using_mname_rename_at_970_280), cast<char * const >::from(((char *) ")")))));
            das_copy(__cpp_enchantation_rename_at_969_279,das_string_builder(__context__,SimNode_AotInterop<5>(__tinfo_66, cast<char * const >::from(((char *) "\"das_call_member<")), cast<char * const >::from(__fType_rename_at_977_282), cast<char * const >::from(((char *) ",&")), cast<char * const >::from(__fn_rename_at_967_275.cppName), cast<char * const >::from(((char *) ">::invoke\"")))));
            builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_67, cast<char * const >::from(((char *) "warning: ")), cast<char * const >::from(__fn_rename_at_967_275.cppName), cast<char * const >::from(((char *) " using clang-ast binding of ")), cast<char * const >::from(__fType_rename_at_977_282), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        } else {
            das_copy(__using_mname_rename_at_970_280,methodName_ff1251eb1b75eec2(__context__,__fn_rename_at_967_275.name));
            das_copy(__bind_enchantation_rename_at_968_278,das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_68, cast<char * const >::from(((char *) "DAS_CALL_METHOD(")), cast<char *>::from(__using_mname_rename_at_970_280), cast<char * const >::from(((char *) ")")))));
            das_copy(__cpp_enchantation_rename_at_969_279,das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_69, cast<char * const >::from(((char *) "DAS_CALL_MEMBER_CPP(")), cast<char * const >::from(__fn_rename_at_967_275.cppName), cast<char * const >::from(((char *) ")")))));
        };
    } else {
        if ( __castbind_rename_at_967_276 )
        {
            char * __fType_rename_at_990_283 = ((char *)(char *)(_Funccpp_genTickqType2FnTypeTick1482691952773259010_a02b5143c6271bc3(__context__,__fn_rename_at_967_275.ftype)));
            das_copy(__bind_enchantation_rename_at_968_278,das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_70, cast<char * const >::from(__fType_rename_at_990_283), cast<char * const >::from(((char *) ",")), cast<char * const >::from(__fn_rename_at_967_275.cppName))));
            das_copy(__cpp_enchantation_rename_at_969_279,das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_71, cast<char * const >::from(((char *) "\"")), cast<char * const >::from(__fn_rename_at_967_275.cppName), cast<char * const >::from(((char *) "\"")))));
            builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_72, cast<char * const >::from(((char *) "warning: ")), cast<char * const >::from(__fn_rename_at_967_275.cppName), cast<char * const >::from(((char *) " using clang-ast binding of ")), cast<char * const >::from(__fType_rename_at_990_283), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        } else {
            das_copy(__bind_enchantation_rename_at_968_278,das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_73, cast<char * const >::from(((char *) "DAS_BIND_FUN(")), cast<char * const >::from(__fn_rename_at_967_275.cppName), cast<char * const >::from(((char *) ")")))));
            das_copy(__cpp_enchantation_rename_at_969_279,das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_74, cast<char * const >::from(((char *) "\"")), cast<char * const >::from(__fn_rename_at_967_275.cppName), cast<char * const >::from(((char *) "\"")))));
        };
    };
    char * __extra_enchantation_rename_at_999_284 = (char *)(nullptr);
    if ( _Funccpp_genTickisRefFunTick11632776912375340809_d4070b3dd9da7cd2(__context__,__fn_rename_at_967_275) )
    {
        das_copy(__extra_enchantation_rename_at_999_284,((char *) ", SimNode_ExtFuncCallRef"));
    } else if ( _Funccpp_genTickisCmresFunTick16266856454814809519_86372ea3375f1a8c(__context__,__fn_rename_at_967_275) )
    {
        das_copy(__extra_enchantation_rename_at_999_284,((char *) ", SimNode_ExtFuncCallAndCopyOrMove"));
    };
    builtin_fprint(__df_rename_at_967_277,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_75, cast<char * const >::from(((char *) "#ifdef ")), cast<char * const >::from(getDefinePrefix_68940f3e2e3e9e89(__context__,__fn_rename_at_967_275.cppName)), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( !builtin_empty(__using_mname_rename_at_970_280) )
    {
        if ( !builtin_empty(__using_mname_expr_rename_at_971_281) )
        {
            builtin_fprint(__df_rename_at_967_277,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_76, cast<char * const >::from(((char *) "using ")), cast<char *>::from(__using_mname_rename_at_970_280), cast<char * const >::from(((char *) " = ")), cast<char *>::from(__using_mname_expr_rename_at_971_281), cast<char * const >::from(((char *) ";\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        } else {
            builtin_fprint(__df_rename_at_967_277,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_77, cast<char * const >::from(((char *) "using ")), cast<char *>::from(__using_mname_rename_at_970_280), cast<char * const >::from(((char *) " = DAS_CALL_MEMBER(")), cast<char * const >::from(__fn_rename_at_967_275.cppName), cast<char * const >::from(((char *) ");\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
    };
    builtin_fprint(__df_rename_at_967_277,das_string_builder_temp(__context__,SimNode_AotInterop<8>(__tinfo_78, cast<char * const >::from(((char *) "addExtern<")), cast<char *>::from(__bind_enchantation_rename_at_968_278), cast<char *>::from(__extra_enchantation_rename_at_999_284), cast<char * const >::from(((char *) ">(*this, lib, \"")), cast<char * const >::from(__fn_rename_at_967_275.name), cast<char * const >::from(((char *) "\",SideEffects::worstDefault, ")), cast<char *>::from(__cpp_enchantation_rename_at_969_279), cast<char * const >::from(((char *) ")\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fprint(__df_rename_at_967_277,((char *) "\t->args({"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( __fn_rename_at_967_275.isMethod )
    {
        builtin_fprint(__df_rename_at_967_277,((char *) "\"self\","),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    {
        bool __need_loop_1018 = true;
        // arg: cpp_gen::ClangFuncArg const&
        das_iterator<TArray<cpp_gen::ClangFuncArg> const > __arg_iterator(__fn_rename_at_967_275.args);
        cpp_gen::ClangFuncArg const  * __arg_rename_at_1018_287;
        __need_loop_1018 = __arg_iterator.first(__context__,(__arg_rename_at_1018_287)) && __need_loop_1018;
        // argi: int
        das_iterator_count DAS_COMMENT((_temp_make_local_1018_31_144 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __argi_iterator(0,1);
        int32_t __argi_rename_at_1018_288;
        __need_loop_1018 = __argi_iterator.first(__context__,(__argi_rename_at_1018_288)) && __need_loop_1018;
        for ( ; __need_loop_1018 ; __need_loop_1018 = __arg_iterator.next(__context__,(__arg_rename_at_1018_287)) && __argi_iterator.next(__context__,(__argi_rename_at_1018_288)) )
        {
            if ( __argi_rename_at_1018_288 != 0 )
            {
                builtin_fprint(__df_rename_at_967_277,((char *) ","),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            };
            if ( builtin_empty((*__arg_rename_at_1018_287).name) )
            {
                builtin_fprint(__df_rename_at_967_277,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_79, cast<char * const >::from(((char *) "\"arg")), cast<int32_t>::from((__fn_rename_at_967_275.isMethod ? das_auto_cast<int32_t>::cast((__argi_rename_at_1018_288 + 1)) : das_auto_cast<int32_t>::cast(__argi_rename_at_1018_288))), cast<char * const >::from(((char *) "\"")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            } else {
                builtin_fprint(__df_rename_at_967_277,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_80, cast<char * const >::from(((char *) "\"")), cast<char * const >::from((*__arg_rename_at_1018_287).name), cast<char * const >::from(((char *) "\"")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            };
        }
        __arg_iterator.close(__context__,(__arg_rename_at_1018_287));
        __argi_iterator.close(__context__,(__argi_rename_at_1018_288));
    };
    if ( !builtin_empty(das_global<char *,0xeb1757bee891e8da>(__context__) /*extra_args*/) )
    {
        if ( builtin_array_size(__fn_rename_at_967_275.args) > 0 )
        {
            builtin_fprint(__df_rename_at_967_277,((char *) ","),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        builtin_fprint(__df_rename_at_967_277,das_global<char *,0xeb1757bee891e8da>(__context__) /*extra_args*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    builtin_fprint(__df_rename_at_967_277,((char *) "})\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    char * __rtype_rename_at_1035_289 = 0;
    TArray<char *> __qargs_rename_at_1036_290; das_zero(__qargs_rename_at_1036_290); das_move(__qargs_rename_at_1036_290, parseQualArgs_3f5a273f6933fa8f(__context__,__fn_rename_at_967_275.ftype,__rtype_rename_at_1035_289));
    bool __revArgs_rename_at_1037_291 = true;
    if ( builtin_array_size(das_arg<TArray<char *>>::pass(__qargs_rename_at_1036_290)) != builtin_array_size(__fn_rename_at_967_275.args) )
    {
        builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_81, cast<char * const >::from(((char *) "warning: args did not parse '")), cast<char * const >::from(__fn_rename_at_967_275.ftype), cast<char * const >::from(((char *) "' to ")), cast<TArray<char *> &>::from(__qargs_rename_at_1036_290), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        _FuncbuiltinTickfinalizeTick13836114024949725080_79f9e841b4434fd8(__context__,das_arg<TArray<char *>>::pass(__qargs_rename_at_1036_290));
        das_copy(__revArgs_rename_at_1037_291,false);
    };
    if ( __revArgs_rename_at_1037_291 )
    {
        _FuncbuiltinTickgetTick8447005936052527643_30d18a39e61d48bc(__context__,das_arg<TTable<char *,char *>>::pass(das_global<TTable<char *,char *>,0x69fa647f2b48b85a>(__context__) /*rev_enums*/),__rtype_rename_at_1035_289,das_make_block<void,char * &>(__context__,336,0,&__func_info__2fb829d28965e333,[&](char * & __rt_rename_at_1044_292) -> void{
            builtin_fprint(__df_rename_at_967_277,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_82, cast<char * const >::from(((char *) "\t\t->res_type(makeType<")), cast<char *>::from(__rt_rename_at_1044_292), cast<char * const >::from(((char *) ">(lib))\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        }));
    };
    {
        bool __need_loop_1048 = true;
        // arg: cpp_gen::ClangFuncArg const&
        das_iterator<TArray<cpp_gen::ClangFuncArg> const > __arg_iterator(__fn_rename_at_967_275.args);
        cpp_gen::ClangFuncArg const  * __arg_rename_at_1048_295;
        __need_loop_1048 = __arg_iterator.first(__context__,(__arg_rename_at_1048_295)) && __need_loop_1048;
        // _argi: int
        das_iterator_count DAS_COMMENT((_temp_make_local_1048_32_352 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) ___argi_iterator(0,1);
        int32_t ___argi_rename_at_1048_296;
        __need_loop_1048 = ___argi_iterator.first(__context__,(___argi_rename_at_1048_296)) && __need_loop_1048;
        for ( ; __need_loop_1048 ; __need_loop_1048 = __arg_iterator.next(__context__,(__arg_rename_at_1048_295)) && ___argi_iterator.next(__context__,(___argi_rename_at_1048_296)) )
        {
            int32_t __argi_rename_at_1049_297 = ((int32_t)(__fn_rename_at_967_275.isMethod ? das_auto_cast<int32_t>::cast((___argi_rename_at_1048_296 + 1)) : das_auto_cast<int32_t>::cast(___argi_rename_at_1048_296)));
            char * __thisArgEnum_rename_at_1050_298 = (char *)(nullptr);
            if ( __revArgs_rename_at_1037_291 )
            {
                _FuncbuiltinTickgetTick8447005936052527643_30d18a39e61d48bc(__context__,das_arg<TTable<char *,char *>>::pass(das_global<TTable<char *,char *>,0x69fa647f2b48b85a>(__context__) /*rev_enums*/),__qargs_rename_at_1036_290(___argi_rename_at_1048_296,__context__),das_make_block<void,char * &>(__context__,512,0,&__func_info__486af025dcb791d2,[&](char * & __en_rename_at_1052_299) -> void{
                    das_copy(__thisArgEnum_rename_at_1050_298,__en_rename_at_1052_299);
                    builtin_fprint(__df_rename_at_967_277,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_83, cast<char * const >::from(((char *) "\t\t->arg_type(")), cast<int32_t>::from(__argi_rename_at_1049_297), cast<char * const >::from(((char *) ",makeType<")), cast<char *>::from(__thisArgEnum_rename_at_1050_298), cast<char * const >::from(((char *) ">(lib))\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                }));
            };
            if ( !builtin_empty((*__arg_rename_at_1048_295).value) )
            {
                if ( SimPolicy<char *>::Equ(cast<char *>::from((*__arg_rename_at_1048_295).value),cast<char *>::from(((char *) "nullptr")),*__context__,nullptr) )
                {
                    if ( SimPolicy<char *>::Equ(cast<char *>::from((*__arg_rename_at_1048_295).atype),cast<char *>::from(((char *) "const char *")),*__context__,nullptr) )
                    {
                        builtin_fprint(__df_rename_at_967_277,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_84, cast<char * const >::from(((char *) "\t\t->arg_init(")), cast<int32_t>::from(__argi_rename_at_1049_297), cast<char * const >::from(((char *) ",make_smart<ExprConstString>(\"\"))\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    } else {
                        builtin_fprint(__df_rename_at_967_277,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_85, cast<char * const >::from(((char *) "\t\t->arg_init(")), cast<int32_t>::from(__argi_rename_at_1049_297), cast<char * const >::from(((char *) ",make_smart<ExprConstPtr>())\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    };
                } else if ( SimPolicy<char *>::Equ(cast<char *>::from((*__arg_rename_at_1048_295).vtype),cast<char *>::from(((char *) "int")),*__context__,nullptr) )
                {
                    if ( builtin_empty(__thisArgEnum_rename_at_1050_298) )
                    {
                        builtin_fprint(__df_rename_at_967_277,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_86, cast<char * const >::from(((char *) "\t\t->arg_init(")), cast<int32_t>::from(__argi_rename_at_1049_297), cast<char * const >::from(((char *) ",make_smart<ExprConstInt>(")), cast<char * const >::from((*__arg_rename_at_1048_295).value), cast<char * const >::from(((char *) "))\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    } else {
                        builtin_fprint(__df_rename_at_967_277,das_string_builder_temp(__context__,SimNode_AotInterop<7>(__tinfo_87, cast<char * const >::from(((char *) "\t\t->arg_init(")), cast<int32_t>::from(__argi_rename_at_1049_297), cast<char * const >::from(((char *) ",make_smart<ExprConstEnumeration>(")), cast<char * const >::from((*__arg_rename_at_1048_295).value), cast<char * const >::from(((char *) ",makeType<")), cast<char *>::from(__thisArgEnum_rename_at_1050_298), cast<char * const >::from(((char *) ">(lib)))\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    };
                } else if ( SimPolicy<char *>::Equ(cast<char *>::from((*__arg_rename_at_1048_295).vtype),cast<char *>::from(((char *) "float")),*__context__,nullptr) )
                {
                    builtin_fprint(__df_rename_at_967_277,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_88, cast<char * const >::from(((char *) "\t\t->arg_init(")), cast<int32_t>::from(__argi_rename_at_1049_297), cast<char * const >::from(((char *) ",make_smart<ExprConstFloat>(")), cast<char * const >::from((*__arg_rename_at_1048_295).value), cast<char * const >::from(((char *) "))\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                } else if ( SimPolicy<char *>::Equ(cast<char *>::from((*__arg_rename_at_1048_295).vtype),cast<char *>::from(((char *) "bool")),*__context__,nullptr) )
                {
                    builtin_fprint(__df_rename_at_967_277,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_89, cast<char * const >::from(((char *) "\t\t->arg_init(")), cast<int32_t>::from(__argi_rename_at_1049_297), cast<char * const >::from(((char *) ",make_smart<ExprConstBool>(")), cast<char * const >::from((*__arg_rename_at_1048_295).value), cast<char * const >::from(((char *) "))\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                } else if ( SimPolicy<char *>::Equ(cast<char *>::from((*__arg_rename_at_1048_295).vtype),cast<char *>::from(((char *) "const char *")),*__context__,nullptr) )
                {
                    builtin_fprint(__df_rename_at_967_277,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_90, cast<char * const >::from(((char *) "\t\t->arg_init(")), cast<int32_t>::from(__argi_rename_at_1049_297), cast<char * const >::from(((char *) ",make_smart<ExprConstString>(")), cast<char * const >::from((*__arg_rename_at_1048_295).value), cast<char * const >::from(((char *) "))\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                } else {
                    builtin_fprint(__df_rename_at_967_277,das_string_builder_temp(__context__,SimNode_AotInterop<7>(__tinfo_91, cast<char * const >::from(((char *) "\t\t// ")), cast<char * const >::from((*__arg_rename_at_1048_295).name), cast<char * const >::from(((char *) " : ")), cast<char * const >::from((*__arg_rename_at_1048_295).atype), cast<char * const >::from(((char *) " = ")), cast<char * const >::from((*__arg_rename_at_1048_295).value), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_92, cast<char * const >::from(((char *) "warning: unsupported init ")), cast<char * const >::from((*__arg_rename_at_1048_295).atype), cast<char * const >::from(((char *) " of ")), cast<char * const >::from((*__arg_rename_at_1048_295).vtype), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                };
            };
        }
        __arg_iterator.close(__context__,(__arg_rename_at_1048_295));
        ___argi_iterator.close(__context__,(___argi_rename_at_1048_296));
    };
    builtin_fprint(__df_rename_at_967_277,((char *) ";\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fprint(__df_rename_at_967_277,((char *) "#endif\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}}

inline void genBindings_6b7b54142f4c772b ( Context * __context__, FILE const  * const  __hf_rename_at_1086_300, FILE const  * const  __df_rename_at_1086_301, FILE const  * const  __mf_enum_rename_at_1086_302, FILE const  * const  __df_enum_rename_at_1086_303, FILE const  * const  __mf_ann_rename_at_1086_304, FILE const  * const  __df_ann_rename_at_1086_305, FILE const  * const  __df_method_rename_at_1086_306 )
{
    genEnums_816c948d88e68308(__context__,__hf_rename_at_1086_300,__mf_enum_rename_at_1086_302,__df_enum_rename_at_1086_303);
    genStructs_e7c6b33a2f752b8e(__context__,__hf_rename_at_1086_300,__mf_ann_rename_at_1086_304,__df_ann_rename_at_1086_305);
    getFunctions_26873a00d281aa69(__context__,__df_rename_at_1086_301,__df_method_rename_at_1086_306);
    closeSplitFile_233b8cc7d4e11a96(__context__);
}

inline void bindHeaders_5b0cabfc50efc6fe ( Context * __context__, char * const  __fname_rename_at_1093_307, char * const  __outprefix_rename_at_1093_308 ) { das_stack_prologue __prologue(__context__,768,"bindHeaders " DAS_FILE_LINE);
{
    Sequence DAS_COMMENT((char *)) _temp_make_local_1098_21_48; _temp_make_local_1098_21_48;
    Sequence DAS_COMMENT((cpp_gen::ExEnum *)) _temp_make_local_1098_37_64; _temp_make_local_1098_37_64;
    _FuncbuiltinTickfinalizeTick5454204887383796109_9c4083566563a85a(__context__,das_arg<TTable<char *,bool>>::pass(das_global<TTable<char *,bool>,0xd633af84de8bdc21>(__context__) /*blocked_functions_table*/));
    {
        bool __need_loop_1095 = true;
        // fn: string&
        das_iterator<TArray<char *>> __fn_iterator(das_global<TArray<char *>,0xfecc248913d2b09d>(__context__) /*blocked_functions*/);
        char * * __fn_rename_at_1095_309;
        __need_loop_1095 = __fn_iterator.first(__context__,(__fn_rename_at_1095_309)) && __need_loop_1095;
        for ( ; __need_loop_1095 ; __need_loop_1095 = __fn_iterator.next(__context__,(__fn_rename_at_1095_309)) )
        {
            das_copy(das_global<TTable<char *,bool>,0xd633af84de8bdc21>(__context__) /*blocked_functions_table*/((*__fn_rename_at_1095_309),__context__),true);
        }
        __fn_iterator.close(__context__,(__fn_rename_at_1095_309));
    };
    {
        bool __need_loop_1098 = true;
        // enk: string
        das_iterator<Sequence DAS_COMMENT((char *))> __enk_iterator((_temp_make_local_1098_21_48 = (_FuncbuiltinTickkeysTick2205854368403803976_bcec88c808d50381(__context__,das_arg<TTable<char *,cpp_gen::ExEnum>>::pass(das_global<TTable<char *,cpp_gen::ExEnum>,0x3b1deb0a06e30b10>(__context__) /*ex_enums*/)))));
        char * __enk_rename_at_1098_312;
        __need_loop_1098 = __enk_iterator.first(__context__,(__enk_rename_at_1098_312)) && __need_loop_1098;
        // env: cpp_gen::ExEnum&
        das_iterator<Sequence DAS_COMMENT((cpp_gen::ExEnum))> __env_iterator((_temp_make_local_1098_37_64 = (_FuncbuiltinTickvaluesTick1351216622833168869_7282d82fa9dd61d5(__context__,das_arg<TTable<char *,cpp_gen::ExEnum>>::pass(das_global<TTable<char *,cpp_gen::ExEnum>,0x3b1deb0a06e30b10>(__context__) /*ex_enums*/)))));
        cpp_gen::ExEnum * __env_rename_at_1098_313;
        __need_loop_1098 = __env_iterator.first(__context__,(__env_rename_at_1098_313)) && __need_loop_1098;
        for ( ; __need_loop_1098 ; __need_loop_1098 = __enk_iterator.next(__context__,(__enk_rename_at_1098_312)) && __env_iterator.next(__context__,(__env_rename_at_1098_313)) )
        {
            das_copy(das_global<TTable<char *,char *>,0x69fa647f2b48b85a>(__context__) /*rev_enums*/((*__env_rename_at_1098_313).name,__context__),__enk_rename_at_1098_312);
        }
        __enk_iterator.close(__context__,(__enk_rename_at_1098_312));
        __env_iterator.close(__context__,(__env_rename_at_1098_313));
    };
    {
        bool __need_loop_1101 = true;
        // kwd: string&
        das_iterator<TDim<char *,17>> __kwd_iterator(das_global<TDim<char *,17>,0xc6488469a096d2fc>(__context__) /*all_keywords*/);
        char * * __kwd_rename_at_1101_314;
        __need_loop_1101 = __kwd_iterator.first(__context__,(__kwd_rename_at_1101_314)) && __need_loop_1101;
        for ( ; __need_loop_1101 ; __need_loop_1101 = __kwd_iterator.next(__context__,(__kwd_rename_at_1101_314)) )
        {
            das_copy(das_global<TTable<char *,bool>,0xff4ca70677f05db6>(__context__) /*ex_keywords*/((*__kwd_rename_at_1101_314),__context__),true);
        }
        __kwd_iterator.close(__context__,(__kwd_rename_at_1101_314));
    };
    {
        bool __need_loop_1104 = true;
        // be: string&
        das_iterator<TArray<char *>> __be_iterator(das_global<TArray<char *>,0x8cd67f1342cfa70e>(__context__) /*blocked_enumerations*/);
        char * * __be_rename_at_1104_315;
        __need_loop_1104 = __be_iterator.first(__context__,(__be_rename_at_1104_315)) && __need_loop_1104;
        for ( ; __need_loop_1104 ; __need_loop_1104 = __be_iterator.next(__context__,(__be_rename_at_1104_315)) )
        {
            das_copy(das_global<TTable<char *,bool>,0x1167f377740b3cc>(__context__) /*blocked_enumerations_table*/((*__be_rename_at_1104_315),__context__),true);
        }
        __be_iterator.close(__context__,(__be_rename_at_1104_315));
    };
    _FuncfioTickfopenTick3937565566638487747_feeb71cd63fd8eac(__context__,__fname_rename_at_1093_307,((char *) "rb"),das_make_block<void,FILE const  * const >(__context__,192,0,&__func_info__ddb31f01d5771c19,[&](FILE const  * const  __f_rename_at_1107_316) -> void{
        if ( __f_rename_at_1107_316 == nullptr )
        {
            builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_93, cast<char * const >::from(((char *) "can't open ")), cast<char * const >::from(__fname_rename_at_1093_307))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        } else {
            _FuncfioTickfreadTick9799048084303762322_c82e6deab5e5b700(__context__,__f_rename_at_1107_316,das_make_block<void,char * const >(__context__,256,0,&__func_info__b989707283fd0f9e,[&](char * const  __data_rename_at_1111_317) -> void{
                builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_94, cast<char * const >::from(((char *) "\n")), cast<char * const >::from(__fname_rename_at_1093_307), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                char * __error_rename_at_1113_318 = (char *)(nullptr);
                json::JsonValue * __json_rename_at_1114_319 = das_invoke_function<json::JsonValue *>::invoke<char * const ,char * &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::read_json CIs &s*/ 0x16c53b6851f56f38)),__data_rename_at_1111_317,__error_rename_at_1113_318);
                if ( __json_rename_at_1114_319 == nullptr )
                {
                    builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_95, cast<char * const >::from(((char *) "failed to parse, ")), cast<char *>::from(__error_rename_at_1113_318), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                } else {
                    DAS_ASSERT((das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__json_rename_at_1114_319->value)));
                    TTable<char *,json::JsonValue *> * __root_rename_at_1119_320 = &(das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__json_rename_at_1114_319->value,__context__));
                    DAS_VERIFY((_Funccpp_genTickis_strTick14919838023837841642_48d382d152e59de8(__context__,(*__root_rename_at_1119_320)(((char *) "kind"),__context__),((char *) "TranslationUnitDecl"))));
                    _Funccpp_genTickonInnerTick15341579017765610681_62ed76bf0329fb0d(__context__,das_arg<TTable<char *,json::JsonValue *>>::pass((*__root_rename_at_1119_320)));
                    _FuncfioTickfopenTick3937565566638487747_feeb71cd63fd8eac(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_96, cast<char * const >::from(__outprefix_rename_at_1093_308), cast<char * const >::from(((char *) ".h")))),((char *) "wb"),das_make_block<void,FILE const  * const >(__context__,368,0,&__func_info__94a6c10524a0a34c,[&](FILE const  * const  __hf_rename_at_1123_321) -> void{
                        builtin_fwrite(__hf_rename_at_1123_321,((char *) "#pragma once\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                        _FuncfioTickfopenTick3937565566638487747_feeb71cd63fd8eac(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_97, cast<char * const >::from(__outprefix_rename_at_1093_308), cast<char * const >::from(((char *) ".inc")))),((char *) "wb"),das_make_block<void,FILE const  * const >(__context__,432,0,&__func_info__4b0de253e8ab06c8,[&](FILE const  * const  __df_rename_at_1125_322) -> void{
                            _FuncfioTickfopenTick3937565566638487747_feeb71cd63fd8eac(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_98, cast<char * const >::from(__outprefix_rename_at_1093_308), cast<char * const >::from(((char *) ".enum.cpp_inc")))),((char *) "wb"),das_make_block<void,FILE const  * const >(__context__,496,0,&__func_info__8951eb20e251273c,[&](FILE const  * const  __mf_enum_rename_at_1126_323) -> void{
                                _FuncfioTickfopenTick3937565566638487747_feeb71cd63fd8eac(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_99, cast<char * const >::from(__outprefix_rename_at_1093_308), cast<char * const >::from(((char *) ".enum.inc")))),((char *) "wb"),das_make_block<void,FILE const  * const >(__context__,560,0,&__func_info__1dd7e9953a1c7073,[&](FILE const  * const  __df_enum_rename_at_1127_324) -> void{
                                    _FuncfioTickfopenTick3937565566638487747_feeb71cd63fd8eac(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_100, cast<char * const >::from(__outprefix_rename_at_1093_308), cast<char * const >::from(((char *) ".ann.cpp_inc")))),((char *) "wb"),das_make_block<void,FILE const  * const >(__context__,624,0,&__func_info__252177d3d36ea5ea,[&](FILE const  * const  __mf_ann_rename_at_1128_325) -> void{
                                        _FuncfioTickfopenTick3937565566638487747_feeb71cd63fd8eac(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_101, cast<char * const >::from(__outprefix_rename_at_1093_308), cast<char * const >::from(((char *) ".ann.inc")))),((char *) "wb"),das_make_block<void,FILE const  * const >(__context__,688,0,&__func_info__7853938a1408c8d1,[&](FILE const  * const  __df_ann_rename_at_1129_326) -> void{
                                            _FuncfioTickfopenTick3937565566638487747_feeb71cd63fd8eac(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_102, cast<char * const >::from(__outprefix_rename_at_1093_308), cast<char * const >::from(((char *) ".method.inc")))),((char *) "wb"),das_make_block<void,FILE const  * const >(__context__,752,0,&__func_info__3fa023a6055a72f3,[&](FILE const  * const  __df_method_rename_at_1130_327) -> void{
                                                genBindings_6b7b54142f4c772b(__context__,__hf_rename_at_1123_321,__df_rename_at_1125_322,__mf_enum_rename_at_1126_323,__df_enum_rename_at_1127_324,__mf_ann_rename_at_1128_325,__df_ann_rename_at_1129_326,__df_method_rename_at_1130_327);
                                            }));
                                        }));
                                    }));
                                }));
                            }));
                        }));
                    }));
                };
            }));
        };
    }));
}}

inline void genDefineConstants_9a873908595aec49 ( Context * __context__, char * const  __fname_rename_at_1146_328, char * const  __prefix_rename_at_1146_329 )
{
    TArray<char *> __fnames_rename_at_1147_330; memset((void*)&__fnames_rename_at_1147_330,0,sizeof(__fnames_rename_at_1147_330));
    TDim<char *,1> _temp_make_local_1147_19_64; _temp_make_local_1147_19_64;
    /* finally */ auto __finally_1146= das_finally([&](){
    _FuncbuiltinTickfinalizeTick13836114024949725080_79f9e841b4434fd8(__context__,das_arg<TArray<char *>>::pass(__fnames_rename_at_1147_330));
    /* end finally */ });
    __fnames_rename_at_1147_330; das_zero(__fnames_rename_at_1147_330); das_move(__fnames_rename_at_1147_330, _FuncbuiltinTickto_array_moveTick3185538323411982277_9285ff1255c3d503(__context__,das_arg<TDim<char *,1>>::pass((([&]() -> TDim<char *,1>& {
        _temp_make_local_1147_19_64(0,__context__) = __fname_rename_at_1146_328;
        return _temp_make_local_1147_19_64;
    })()))));
    genDefineConstants_7bfbc66c3d4f2529(__context__,das_arg<TArray<char *>>::pass(__fnames_rename_at_1147_330),__prefix_rename_at_1146_329);
}

inline char * getGenConstType_ba5129bb3b413133 ( Context * __context__, char * const  __name_rename_at_1152_331, char * const  __default_name_rename_at_1152_332 )
{
    return das_auto_cast<char *>::cast(das_null_coalescing<char * const >::get(TTable<char *,char *>::safe_index(&(das_global<TTable<char *,char *>,0x713f35e9b29fd64e>(__context__) /*const_type_table*/),__name_rename_at_1152_331,__context__),__default_name_rename_at_1152_332));
}

inline void searchAndGenConst_ec8cac3d091d1a8 ( Context * __context__, regex::Regex &  __regex_def_rename_at_1156_333, char * const  __defTName_rename_at_1156_334, char * const  __suffix_rename_at_1156_335, TTable<int32_t,bool> &  __ofs_rename_at_1156_336, char * const  __data_rename_at_1156_337, TTable<char *,bool> &  __dup_rename_at_1156_338, FILE const  * const  __hf_rename_at_1156_339 ) { das_stack_prologue __prologue(__context__,144,"searchAndGenConst " DAS_FILE_LINE);
{
    das_invoke_function<void>::invoke<regex::Regex &,char * const ,Block DAS_COMMENT((bool,range)) const  &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::regex_foreach S<regex::Regex> Cs CN<at>0<Cr>1<b>$*/ 0x2eebc51fb5fe7e28)),das_arg<regex::Regex>::pass(__regex_def_rename_at_1156_333),__data_rename_at_1156_337,das_make_block<bool,range>(__context__,80,0,&__func_info__3d0cb235470c67cc,[&](range __r_rename_at_1157_340) -> bool{
        if ( !_FuncbuiltinTickkey_existsTick16808803843923989214_9d11994217d1865a(__context__,das_arg<TTable<int32_t,bool>>::pass(__ofs_rename_at_1156_336),v_extract_xi(v_cast_vec4i(__r_rename_at_1157_340)) /*x*/) )
        {
            char * __DEF_rename_at_1159_341 = ((char *)(char *)(das_invoke_function<char * const >::invoke<regex::Regex const  &,int32_t,char * const >(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::regex_group CS<regex::Regex> Ci Cs*/ 0x56501aba1922680a)),das_arg<regex::Regex>::pass(__regex_def_rename_at_1156_333),1,__data_rename_at_1156_337)));
            char * __VAL_rename_at_1160_342 = ((char *)(char *)(das_invoke_function<char * const >::invoke<regex::Regex const  &,int32_t,char * const >(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::regex_group CS<regex::Regex> Ci Cs*/ 0x56501aba1922680a)),das_arg<regex::Regex>::pass(__regex_def_rename_at_1156_333),2,__data_rename_at_1156_337)));
            if ( !_FuncbuiltinTickkey_existsTick16808803843923989214_efda835c0359ec76(__context__,das_arg<TTable<char *,bool>>::pass(das_global<TTable<char *,bool>,0xc079186d7d42988b>(__context__) /*blocked_defines_table*/),__DEF_rename_at_1159_341) )
            {
                if ( (das_global<Lambda DAS_COMMENT((bool,char * const )),0x113c776499a2302e>(__context__) /*is_const_blocked*/ == nullptr) || !(das_invoke_lambda<bool>::invoke<char * const >(__context__,nullptr,das_global<Lambda DAS_COMMENT((bool,char * const )),0x113c776499a2302e>(__context__) /*is_const_blocked*/,__DEF_rename_at_1159_341)) )
                {
                    char * __ctt_rename_at_1163_343 = ((char *)(char *)(getGenConstType_ba5129bb3b413133(__context__,__DEF_rename_at_1159_341,__defTName_rename_at_1156_334)));
                    if ( !__dup_rename_at_1156_338(__DEF_rename_at_1159_341,__context__) )
                    {
                        builtin_fprint(__hf_rename_at_1156_339,das_string_builder_temp(__context__,SimNode_AotInterop<10>(__tinfo_103, cast<char * const >::from(((char *) "addConstant<")), cast<char * const >::from(__ctt_rename_at_1163_343), cast<char * const >::from(((char *) ">(*this,\"")), cast<char * const >::from(__DEF_rename_at_1159_341), cast<char * const >::from(((char *) "\",")), cast<char * const >::from(__ctt_rename_at_1163_343), cast<char * const >::from(((char *) "(")), cast<char * const >::from(__VAL_rename_at_1160_342), cast<char * const >::from(__suffix_rename_at_1156_335), cast<char * const >::from(((char *) "));\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                        das_copy(__dup_rename_at_1156_338(__DEF_rename_at_1159_341,__context__),true);
                    } else {
                        builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_104, cast<char * const >::from(((char *) "const: duplicate ")), cast<char * const >::from(__DEF_rename_at_1159_341), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    };
                };
            };
        };
        return das_auto_cast<bool>::cast(true);
    }));
}}

inline void genDefineConstants_7bfbc66c3d4f2529 ( Context * __context__, TArray<char *> const  &  __fnames_rename_at_1177_344, char * const  __prefix_rename_at_1177_345 ) { das_stack_prologue __prologue(__context__,3728,"genDefineConstants " DAS_FILE_LINE);
{
    regex::Regex _temp_make_local_1182_23_128; _temp_make_local_1182_23_128;
    TDim<AutoTuple<range,char *>,3> _temp_make_local_1182_23_480; _temp_make_local_1182_23_480;
    regex::Regex _temp_make_local_1183_23_608; _temp_make_local_1183_23_608;
    TDim<AutoTuple<range,char *>,3> _temp_make_local_1183_23_928; _temp_make_local_1183_23_928;
    regex::Regex _temp_make_local_1184_25_1056; _temp_make_local_1184_25_1056;
    TDim<AutoTuple<range,char *>,3> _temp_make_local_1184_25_1552; _temp_make_local_1184_25_1552;
    regex::Regex _temp_make_local_1185_26_1680; _temp_make_local_1185_26_1680;
    TDim<AutoTuple<range,char *>,3> _temp_make_local_1185_26_2176; _temp_make_local_1185_26_2176;
    regex::Regex _temp_make_local_1186_26_2304; _temp_make_local_1186_26_2304;
    TDim<AutoTuple<range,char *>,3> _temp_make_local_1186_26_2800; _temp_make_local_1186_26_2800;
    regex::Regex _temp_make_local_1187_26_2928; _temp_make_local_1187_26_2928;
    TDim<AutoTuple<range,char *>,3> _temp_make_local_1187_26_3424; _temp_make_local_1187_26_3424;
    _FuncbuiltinTickfinalizeTick5454204887383796109_9c4083566563a85a(__context__,das_arg<TTable<char *,bool>>::pass(das_global<TTable<char *,bool>,0xc079186d7d42988b>(__context__) /*blocked_defines_table*/));
    {
        bool __need_loop_1179 = true;
        // bd: string&
        das_iterator<TArray<char *>> __bd_iterator(das_global<TArray<char *>,0x63b059e161d0e5e3>(__context__) /*blocked_defines*/);
        char * * __bd_rename_at_1179_346;
        __need_loop_1179 = __bd_iterator.first(__context__,(__bd_rename_at_1179_346)) && __need_loop_1179;
        for ( ; __need_loop_1179 ; __need_loop_1179 = __bd_iterator.next(__context__,(__bd_rename_at_1179_346)) )
        {
            das_copy(das_global<TTable<char *,bool>,0xc079186d7d42988b>(__context__) /*blocked_defines_table*/((*__bd_rename_at_1179_346),__context__),true);
        }
        __bd_iterator.close(__context__,(__bd_rename_at_1179_346));
    };
    regex::Regex __reg_def_hex_rename_at_1182_347; das_zero(__reg_def_hex_rename_at_1182_347); das_move(__reg_def_hex_rename_at_1182_347, das_invoke_function<regex::Regex>::invoke_cmres<regex::Regex &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::regex_compile S<regex::Regex>*/ 0x235aa29958cfc4c4)),das_arg<regex::Regex>::pass((([&]() -> regex::Regex& {
        das_zero(_temp_make_local_1182_23_128);
        das_copy((_temp_make_local_1182_23_128.root),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
            regex::ReNode __mks_1182;
            das_zero(__mks_1182);
            das_copy((__mks_1182.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
            das_copy((__mks_1182.id),(0));
            das_copy((__mks_1182.at),(range(0,34)));
            das_copy((__mks_1182.text),(nullptr));
            das_copy((__mks_1182.textLen),(0));
            das_move((__mks_1182.all),((([&]() -> TArray<regex::ReNode *> {
                TArray<regex::ReNode *> __mks_1182;
                das_zero(__mks_1182);
                return __mks_1182;
            })())));
            das_copy((__mks_1182.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_1182;
                das_zero(__mks_1182);
                das_copy((__mks_1182.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                das_copy((__mks_1182.id),(1));
                das_copy((__mks_1182.at),(range(0,18)));
                das_copy((__mks_1182.text),(nullptr));
                das_copy((__mks_1182.textLen),(0));
                das_move((__mks_1182.all),((([&]() -> TArray<regex::ReNode *> {
                    TArray<regex::ReNode *> __mks_1182;
                    das_zero(__mks_1182);
                    return __mks_1182;
                })())));
                das_copy((__mks_1182.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_1182;
                    das_zero(__mks_1182);
                    das_copy((__mks_1182.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                    das_copy((__mks_1182.id),(2));
                    das_copy((__mks_1182.at),(range(0,15)));
                    das_copy((__mks_1182.text),(nullptr));
                    das_copy((__mks_1182.textLen),(0));
                    das_move((__mks_1182.all),((([&]() -> TArray<regex::ReNode *> {
                        TArray<regex::ReNode *> __mks_1182;
                        das_zero(__mks_1182);
                        return __mks_1182;
                    })())));
                    das_copy((__mks_1182.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_1182;
                        das_zero(__mks_1182);
                        das_copy((__mks_1182.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                        das_copy((__mks_1182.id),(3));
                        das_copy((__mks_1182.at),(range(0,10)));
                        das_copy((__mks_1182.text),(nullptr));
                        das_copy((__mks_1182.textLen),(0));
                        das_move((__mks_1182.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_1182;
                            das_zero(__mks_1182);
                            return __mks_1182;
                        })())));
                        das_copy((__mks_1182.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                            regex::ReNode __mks_1182;
                            das_zero(__mks_1182);
                            das_copy((__mks_1182.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                            das_copy((__mks_1182.id),(4));
                            das_copy((__mks_1182.at),(range(0,7)));
                            das_copy((__mks_1182.text),(((char *) "#define")));
                            das_copy((__mks_1182.textLen),(7));
                            das_move((__mks_1182.all),((([&]() -> TArray<regex::ReNode *> {
                                TArray<regex::ReNode *> __mks_1182;
                                das_zero(__mks_1182);
                                return __mks_1182;
                            })())));
                            das_copy((__mks_1182.left),(nullptr));
                            das_copy((__mks_1182.right),(nullptr));
                            das_copy((__mks_1182.subexpr),(nullptr));
                            das_copy((__mks_1182.cset),((([&]() -> TDim<uint32_t,8> {
                                TDim<uint32_t,8> __mka_1182;
                                __mka_1182(0,__context__) = 0x0u;
                                __mka_1182(1,__context__) = 0x0u;
                                __mka_1182(2,__context__) = 0x0u;
                                __mka_1182(3,__context__) = 0x0u;
                                __mka_1182(4,__context__) = 0x0u;
                                __mka_1182(5,__context__) = 0x0u;
                                __mka_1182(6,__context__) = 0x0u;
                                __mka_1182(7,__context__) = 0x0u;
                                return __mka_1182;
                            })())));
                            das_copy((__mks_1182.index),(0));
                            return __mks_1182;
                        })()))));
                        das_copy((__mks_1182.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                            regex::ReNode __mks_1182;
                            das_zero(__mks_1182);
                            das_copy((__mks_1182.op),(DAS_COMMENT(enum) regex::ReOp::Plus));
                            das_copy((__mks_1182.id),(5));
                            das_copy((__mks_1182.at),(range(7,10)));
                            das_copy((__mks_1182.text),(nullptr));
                            das_copy((__mks_1182.textLen),(0));
                            das_move((__mks_1182.all),((([&]() -> TArray<regex::ReNode *> {
                                TArray<regex::ReNode *> __mks_1182;
                                das_zero(__mks_1182);
                                return __mks_1182;
                            })())));
                            das_copy((__mks_1182.left),(nullptr));
                            das_copy((__mks_1182.right),(nullptr));
                            das_copy((__mks_1182.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                regex::ReNode __mks_1182;
                                das_zero(__mks_1182);
                                das_copy((__mks_1182.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                                das_copy((__mks_1182.id),(6));
                                das_copy((__mks_1182.at),(range(7,9)));
                                das_copy((__mks_1182.text),(nullptr));
                                das_copy((__mks_1182.textLen),(0));
                                das_move((__mks_1182.all),((([&]() -> TArray<regex::ReNode *> {
                                    TArray<regex::ReNode *> __mks_1182;
                                    das_zero(__mks_1182);
                                    return __mks_1182;
                                })())));
                                das_copy((__mks_1182.left),(nullptr));
                                das_copy((__mks_1182.right),(nullptr));
                                das_copy((__mks_1182.subexpr),(nullptr));
                                das_copy((__mks_1182.cset),((([&]() -> TDim<uint32_t,8> {
                                    TDim<uint32_t,8> __mka_1182;
                                    __mka_1182(0,__context__) = 0x200u;
                                    __mka_1182(1,__context__) = 0x1u;
                                    __mka_1182(2,__context__) = 0x0u;
                                    __mka_1182(3,__context__) = 0x0u;
                                    __mka_1182(4,__context__) = 0x0u;
                                    __mka_1182(5,__context__) = 0x0u;
                                    __mka_1182(6,__context__) = 0x0u;
                                    __mka_1182(7,__context__) = 0x0u;
                                    return __mka_1182;
                                })())));
                                das_copy((__mks_1182.index),(0));
                                return __mks_1182;
                            })()))));
                            das_copy((__mks_1182.cset),((([&]() -> TDim<uint32_t,8> {
                                TDim<uint32_t,8> __mka_1182;
                                __mka_1182(0,__context__) = 0x200u;
                                __mka_1182(1,__context__) = 0x1u;
                                __mka_1182(2,__context__) = 0x0u;
                                __mka_1182(3,__context__) = 0x0u;
                                __mka_1182(4,__context__) = 0x0u;
                                __mka_1182(5,__context__) = 0x0u;
                                __mka_1182(6,__context__) = 0x0u;
                                __mka_1182(7,__context__) = 0x0u;
                                return __mka_1182;
                            })())));
                            das_copy((__mks_1182.index),(0));
                            return __mks_1182;
                        })()))));
                        das_copy((__mks_1182.subexpr),(nullptr));
                        das_copy((__mks_1182.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_1182;
                            __mka_1182(0,__context__) = 0x0u;
                            __mka_1182(1,__context__) = 0x0u;
                            __mka_1182(2,__context__) = 0x0u;
                            __mka_1182(3,__context__) = 0x0u;
                            __mka_1182(4,__context__) = 0x0u;
                            __mka_1182(5,__context__) = 0x0u;
                            __mka_1182(6,__context__) = 0x0u;
                            __mka_1182(7,__context__) = 0x0u;
                            return __mka_1182;
                        })())));
                        das_copy((__mks_1182.index),(0));
                        return __mks_1182;
                    })()))));
                    das_copy((__mks_1182.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_1182;
                        das_zero(__mks_1182);
                        das_copy((__mks_1182.op),(DAS_COMMENT(enum) regex::ReOp::Group));
                        das_copy((__mks_1182.id),(7));
                        das_copy((__mks_1182.at),(range(10,15)));
                        das_copy((__mks_1182.text),(nullptr));
                        das_copy((__mks_1182.textLen),(0));
                        das_move((__mks_1182.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_1182;
                            das_zero(__mks_1182);
                            return __mks_1182;
                        })())));
                        das_copy((__mks_1182.left),(nullptr));
                        das_copy((__mks_1182.right),(nullptr));
                        das_copy((__mks_1182.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                            regex::ReNode __mks_1182;
                            das_zero(__mks_1182);
                            das_copy((__mks_1182.op),(DAS_COMMENT(enum) regex::ReOp::Plus));
                            das_copy((__mks_1182.id),(8));
                            das_copy((__mks_1182.at),(range(11,14)));
                            das_copy((__mks_1182.text),(nullptr));
                            das_copy((__mks_1182.textLen),(0));
                            das_move((__mks_1182.all),((([&]() -> TArray<regex::ReNode *> {
                                TArray<regex::ReNode *> __mks_1182;
                                das_zero(__mks_1182);
                                return __mks_1182;
                            })())));
                            das_copy((__mks_1182.left),(nullptr));
                            das_copy((__mks_1182.right),(nullptr));
                            das_copy((__mks_1182.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                regex::ReNode __mks_1182;
                                das_zero(__mks_1182);
                                das_copy((__mks_1182.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                                das_copy((__mks_1182.id),(9));
                                das_copy((__mks_1182.at),(range(11,13)));
                                das_copy((__mks_1182.text),(nullptr));
                                das_copy((__mks_1182.textLen),(0));
                                das_move((__mks_1182.all),((([&]() -> TArray<regex::ReNode *> {
                                    TArray<regex::ReNode *> __mks_1182;
                                    das_zero(__mks_1182);
                                    return __mks_1182;
                                })())));
                                das_copy((__mks_1182.left),(nullptr));
                                das_copy((__mks_1182.right),(nullptr));
                                das_copy((__mks_1182.subexpr),(nullptr));
                                das_copy((__mks_1182.cset),((([&]() -> TDim<uint32_t,8> {
                                    TDim<uint32_t,8> __mka_1182;
                                    __mka_1182(0,__context__) = 0x0u;
                                    __mka_1182(1,__context__) = 0x3ff0000u;
                                    __mka_1182(2,__context__) = 0x87fffffeu;
                                    __mka_1182(3,__context__) = 0x7fffffeu;
                                    __mka_1182(4,__context__) = 0x0u;
                                    __mka_1182(5,__context__) = 0x0u;
                                    __mka_1182(6,__context__) = 0x0u;
                                    __mka_1182(7,__context__) = 0x0u;
                                    return __mka_1182;
                                })())));
                                das_copy((__mks_1182.index),(0));
                                return __mks_1182;
                            })()))));
                            das_copy((__mks_1182.cset),((([&]() -> TDim<uint32_t,8> {
                                TDim<uint32_t,8> __mka_1182;
                                __mka_1182(0,__context__) = 0x0u;
                                __mka_1182(1,__context__) = 0x3ff0000u;
                                __mka_1182(2,__context__) = 0x87fffffeu;
                                __mka_1182(3,__context__) = 0x7fffffeu;
                                __mka_1182(4,__context__) = 0x0u;
                                __mka_1182(5,__context__) = 0x0u;
                                __mka_1182(6,__context__) = 0x0u;
                                __mka_1182(7,__context__) = 0x0u;
                                return __mka_1182;
                            })())));
                            das_copy((__mks_1182.index),(0));
                            return __mks_1182;
                        })()))));
                        das_copy((__mks_1182.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_1182;
                            __mka_1182(0,__context__) = 0x0u;
                            __mka_1182(1,__context__) = 0x0u;
                            __mka_1182(2,__context__) = 0x0u;
                            __mka_1182(3,__context__) = 0x0u;
                            __mka_1182(4,__context__) = 0x0u;
                            __mka_1182(5,__context__) = 0x0u;
                            __mka_1182(6,__context__) = 0x0u;
                            __mka_1182(7,__context__) = 0x0u;
                            return __mka_1182;
                        })())));
                        das_copy((__mks_1182.index),(1));
                        return __mks_1182;
                    })()))));
                    das_copy((__mks_1182.subexpr),(nullptr));
                    das_copy((__mks_1182.cset),((([&]() -> TDim<uint32_t,8> {
                        TDim<uint32_t,8> __mka_1182;
                        __mka_1182(0,__context__) = 0x0u;
                        __mka_1182(1,__context__) = 0x0u;
                        __mka_1182(2,__context__) = 0x0u;
                        __mka_1182(3,__context__) = 0x0u;
                        __mka_1182(4,__context__) = 0x0u;
                        __mka_1182(5,__context__) = 0x0u;
                        __mka_1182(6,__context__) = 0x0u;
                        __mka_1182(7,__context__) = 0x0u;
                        return __mka_1182;
                    })())));
                    das_copy((__mks_1182.index),(0));
                    return __mks_1182;
                })()))));
                das_copy((__mks_1182.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_1182;
                    das_zero(__mks_1182);
                    das_copy((__mks_1182.op),(DAS_COMMENT(enum) regex::ReOp::Plus));
                    das_copy((__mks_1182.id),(10));
                    das_copy((__mks_1182.at),(range(15,18)));
                    das_copy((__mks_1182.text),(nullptr));
                    das_copy((__mks_1182.textLen),(0));
                    das_move((__mks_1182.all),((([&]() -> TArray<regex::ReNode *> {
                        TArray<regex::ReNode *> __mks_1182;
                        das_zero(__mks_1182);
                        return __mks_1182;
                    })())));
                    das_copy((__mks_1182.left),(nullptr));
                    das_copy((__mks_1182.right),(nullptr));
                    das_copy((__mks_1182.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_1182;
                        das_zero(__mks_1182);
                        das_copy((__mks_1182.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                        das_copy((__mks_1182.id),(11));
                        das_copy((__mks_1182.at),(range(15,17)));
                        das_copy((__mks_1182.text),(nullptr));
                        das_copy((__mks_1182.textLen),(0));
                        das_move((__mks_1182.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_1182;
                            das_zero(__mks_1182);
                            return __mks_1182;
                        })())));
                        das_copy((__mks_1182.left),(nullptr));
                        das_copy((__mks_1182.right),(nullptr));
                        das_copy((__mks_1182.subexpr),(nullptr));
                        das_copy((__mks_1182.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_1182;
                            __mka_1182(0,__context__) = 0x200u;
                            __mka_1182(1,__context__) = 0x1u;
                            __mka_1182(2,__context__) = 0x0u;
                            __mka_1182(3,__context__) = 0x0u;
                            __mka_1182(4,__context__) = 0x0u;
                            __mka_1182(5,__context__) = 0x0u;
                            __mka_1182(6,__context__) = 0x0u;
                            __mka_1182(7,__context__) = 0x0u;
                            return __mka_1182;
                        })())));
                        das_copy((__mks_1182.index),(0));
                        return __mks_1182;
                    })()))));
                    das_copy((__mks_1182.cset),((([&]() -> TDim<uint32_t,8> {
                        TDim<uint32_t,8> __mka_1182;
                        __mka_1182(0,__context__) = 0x200u;
                        __mka_1182(1,__context__) = 0x1u;
                        __mka_1182(2,__context__) = 0x0u;
                        __mka_1182(3,__context__) = 0x0u;
                        __mka_1182(4,__context__) = 0x0u;
                        __mka_1182(5,__context__) = 0x0u;
                        __mka_1182(6,__context__) = 0x0u;
                        __mka_1182(7,__context__) = 0x0u;
                        return __mka_1182;
                    })())));
                    das_copy((__mks_1182.index),(0));
                    return __mks_1182;
                })()))));
                das_copy((__mks_1182.subexpr),(nullptr));
                das_copy((__mks_1182.cset),((([&]() -> TDim<uint32_t,8> {
                    TDim<uint32_t,8> __mka_1182;
                    __mka_1182(0,__context__) = 0x0u;
                    __mka_1182(1,__context__) = 0x0u;
                    __mka_1182(2,__context__) = 0x0u;
                    __mka_1182(3,__context__) = 0x0u;
                    __mka_1182(4,__context__) = 0x0u;
                    __mka_1182(5,__context__) = 0x0u;
                    __mka_1182(6,__context__) = 0x0u;
                    __mka_1182(7,__context__) = 0x0u;
                    return __mka_1182;
                })())));
                das_copy((__mks_1182.index),(0));
                return __mks_1182;
            })()))));
            das_copy((__mks_1182.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_1182;
                das_zero(__mks_1182);
                das_copy((__mks_1182.op),(DAS_COMMENT(enum) regex::ReOp::Group));
                das_copy((__mks_1182.id),(12));
                das_copy((__mks_1182.at),(range(18,34)));
                das_copy((__mks_1182.text),(nullptr));
                das_copy((__mks_1182.textLen),(0));
                das_move((__mks_1182.all),((([&]() -> TArray<regex::ReNode *> {
                    TArray<regex::ReNode *> __mks_1182;
                    das_zero(__mks_1182);
                    return __mks_1182;
                })())));
                das_copy((__mks_1182.left),(nullptr));
                das_copy((__mks_1182.right),(nullptr));
                das_copy((__mks_1182.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_1182;
                    das_zero(__mks_1182);
                    das_copy((__mks_1182.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                    das_copy((__mks_1182.id),(13));
                    das_copy((__mks_1182.at),(range(19,33)));
                    das_copy((__mks_1182.text),(nullptr));
                    das_copy((__mks_1182.textLen),(0));
                    das_move((__mks_1182.all),((([&]() -> TArray<regex::ReNode *> {
                        TArray<regex::ReNode *> __mks_1182;
                        das_zero(__mks_1182);
                        return __mks_1182;
                    })())));
                    das_copy((__mks_1182.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_1182;
                        das_zero(__mks_1182);
                        das_copy((__mks_1182.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                        das_copy((__mks_1182.id),(14));
                        das_copy((__mks_1182.at),(range(19,21)));
                        das_copy((__mks_1182.text),(((char *) "0x")));
                        das_copy((__mks_1182.textLen),(2));
                        das_move((__mks_1182.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_1182;
                            das_zero(__mks_1182);
                            return __mks_1182;
                        })())));
                        das_copy((__mks_1182.left),(nullptr));
                        das_copy((__mks_1182.right),(nullptr));
                        das_copy((__mks_1182.subexpr),(nullptr));
                        das_copy((__mks_1182.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_1182;
                            __mka_1182(0,__context__) = 0x0u;
                            __mka_1182(1,__context__) = 0x0u;
                            __mka_1182(2,__context__) = 0x0u;
                            __mka_1182(3,__context__) = 0x0u;
                            __mka_1182(4,__context__) = 0x0u;
                            __mka_1182(5,__context__) = 0x0u;
                            __mka_1182(6,__context__) = 0x0u;
                            __mka_1182(7,__context__) = 0x0u;
                            return __mka_1182;
                        })())));
                        das_copy((__mks_1182.index),(0));
                        return __mks_1182;
                    })()))));
                    das_copy((__mks_1182.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_1182;
                        das_zero(__mks_1182);
                        das_copy((__mks_1182.op),(DAS_COMMENT(enum) regex::ReOp::Plus));
                        das_copy((__mks_1182.id),(15));
                        das_copy((__mks_1182.at),(range(21,33)));
                        das_copy((__mks_1182.text),(nullptr));
                        das_copy((__mks_1182.textLen),(0));
                        das_move((__mks_1182.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_1182;
                            das_zero(__mks_1182);
                            return __mks_1182;
                        })())));
                        das_copy((__mks_1182.left),(nullptr));
                        das_copy((__mks_1182.right),(nullptr));
                        das_copy((__mks_1182.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                            regex::ReNode __mks_1182;
                            das_zero(__mks_1182);
                            das_copy((__mks_1182.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                            das_copy((__mks_1182.id),(16));
                            das_copy((__mks_1182.at),(range(21,32)));
                            das_copy((__mks_1182.text),(nullptr));
                            das_copy((__mks_1182.textLen),(0));
                            das_move((__mks_1182.all),((([&]() -> TArray<regex::ReNode *> {
                                TArray<regex::ReNode *> __mks_1182;
                                das_zero(__mks_1182);
                                return __mks_1182;
                            })())));
                            das_copy((__mks_1182.left),(nullptr));
                            das_copy((__mks_1182.right),(nullptr));
                            das_copy((__mks_1182.subexpr),(nullptr));
                            das_copy((__mks_1182.cset),((([&]() -> TDim<uint32_t,8> {
                                TDim<uint32_t,8> __mka_1182;
                                __mka_1182(0,__context__) = 0x0u;
                                __mka_1182(1,__context__) = 0x3ff0000u;
                                __mka_1182(2,__context__) = 0x7fffffeu;
                                __mka_1182(3,__context__) = 0x7fffffeu;
                                __mka_1182(4,__context__) = 0x0u;
                                __mka_1182(5,__context__) = 0x0u;
                                __mka_1182(6,__context__) = 0x0u;
                                __mka_1182(7,__context__) = 0x0u;
                                return __mka_1182;
                            })())));
                            das_copy((__mks_1182.index),(0));
                            return __mks_1182;
                        })()))));
                        das_copy((__mks_1182.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_1182;
                            __mka_1182(0,__context__) = 0x0u;
                            __mka_1182(1,__context__) = 0x3ff0000u;
                            __mka_1182(2,__context__) = 0x7fffffeu;
                            __mka_1182(3,__context__) = 0x7fffffeu;
                            __mka_1182(4,__context__) = 0x0u;
                            __mka_1182(5,__context__) = 0x0u;
                            __mka_1182(6,__context__) = 0x0u;
                            __mka_1182(7,__context__) = 0x0u;
                            return __mka_1182;
                        })())));
                        das_copy((__mks_1182.index),(0));
                        return __mks_1182;
                    })()))));
                    das_copy((__mks_1182.subexpr),(nullptr));
                    das_copy((__mks_1182.cset),((([&]() -> TDim<uint32_t,8> {
                        TDim<uint32_t,8> __mka_1182;
                        __mka_1182(0,__context__) = 0x0u;
                        __mka_1182(1,__context__) = 0x0u;
                        __mka_1182(2,__context__) = 0x0u;
                        __mka_1182(3,__context__) = 0x0u;
                        __mka_1182(4,__context__) = 0x0u;
                        __mka_1182(5,__context__) = 0x0u;
                        __mka_1182(6,__context__) = 0x0u;
                        __mka_1182(7,__context__) = 0x0u;
                        return __mka_1182;
                    })())));
                    das_copy((__mks_1182.index),(0));
                    return __mks_1182;
                })()))));
                das_copy((__mks_1182.cset),((([&]() -> TDim<uint32_t,8> {
                    TDim<uint32_t,8> __mka_1182;
                    __mka_1182(0,__context__) = 0x0u;
                    __mka_1182(1,__context__) = 0x0u;
                    __mka_1182(2,__context__) = 0x0u;
                    __mka_1182(3,__context__) = 0x0u;
                    __mka_1182(4,__context__) = 0x0u;
                    __mka_1182(5,__context__) = 0x0u;
                    __mka_1182(6,__context__) = 0x0u;
                    __mka_1182(7,__context__) = 0x0u;
                    return __mka_1182;
                })())));
                das_copy((__mks_1182.index),(2));
                return __mks_1182;
            })()))));
            das_copy((__mks_1182.subexpr),(nullptr));
            das_copy((__mks_1182.cset),((([&]() -> TDim<uint32_t,8> {
                TDim<uint32_t,8> __mka_1182;
                __mka_1182(0,__context__) = 0x0u;
                __mka_1182(1,__context__) = 0x0u;
                __mka_1182(2,__context__) = 0x0u;
                __mka_1182(3,__context__) = 0x0u;
                __mka_1182(4,__context__) = 0x0u;
                __mka_1182(5,__context__) = 0x0u;
                __mka_1182(6,__context__) = 0x0u;
                __mka_1182(7,__context__) = 0x0u;
                return __mka_1182;
            })())));
            das_copy((__mks_1182.index),(0));
            return __mks_1182;
        })()))));
        das_move((_temp_make_local_1182_23_128.groups),(_FuncbuiltinTickto_array_moveTick3185538323411982277_71fbdf75ac58194(__context__,das_arg<TDim<AutoTuple<range,char *>,3>>::pass((([&]() -> TDim<AutoTuple<range,char *>,3>& {
            _temp_make_local_1182_23_480(0,__context__) = (([&]() -> AutoTuple<range,char *> {
                AutoTuple<range,char *> __mkt_1182;
                das_get_auto_tuple_field<range,0,range,char *>::get(__mkt_1182) = range(0,0);
                das_get_auto_tuple_field<char *,1,range,char *>::get(__mkt_1182) = nullptr;
                return __mkt_1182;
            })());
            _temp_make_local_1182_23_480(1,__context__) = (([&]() -> AutoTuple<range,char *> {
                AutoTuple<range,char *> __mkt_1182;
                das_get_auto_tuple_field<range,0,range,char *>::get(__mkt_1182) = range(0,0);
                das_get_auto_tuple_field<char *,1,range,char *>::get(__mkt_1182) = ((char *) "1");
                return __mkt_1182;
            })());
            _temp_make_local_1182_23_480(2,__context__) = (([&]() -> AutoTuple<range,char *> {
                AutoTuple<range,char *> __mkt_1182;
                das_get_auto_tuple_field<range,0,range,char *>::get(__mkt_1182) = range(0,0);
                das_get_auto_tuple_field<char *,1,range,char *>::get(__mkt_1182) = ((char *) "2");
                return __mkt_1182;
            })());
            return _temp_make_local_1182_23_480;
        })())))));
        das_copy((_temp_make_local_1182_23_128.earlyOut),((([&]() -> TDim<uint32_t,8> {
            TDim<uint32_t,8> __mka_1182;
            __mka_1182(0,__context__) = 0x0u;
            __mka_1182(1,__context__) = 0x8u;
            __mka_1182(2,__context__) = 0x0u;
            __mka_1182(3,__context__) = 0x0u;
            __mka_1182(4,__context__) = 0x0u;
            __mka_1182(5,__context__) = 0x0u;
            __mka_1182(6,__context__) = 0x0u;
            __mka_1182(7,__context__) = 0x0u;
            return __mka_1182;
        })())));
        das_copy((_temp_make_local_1182_23_128.canEarlyOut),(true));
        return _temp_make_local_1182_23_128;
    })()))));
    regex::Regex __reg_def_dec_rename_at_1183_348; das_zero(__reg_def_dec_rename_at_1183_348); das_move(__reg_def_dec_rename_at_1183_348, das_invoke_function<regex::Regex>::invoke_cmres<regex::Regex &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::regex_compile S<regex::Regex>*/ 0x235aa29958cfc4c4)),das_arg<regex::Regex>::pass((([&]() -> regex::Regex& {
        das_zero(_temp_make_local_1183_23_608);
        das_copy((_temp_make_local_1183_23_608.root),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
            regex::ReNode __mks_1183;
            das_zero(__mks_1183);
            das_copy((__mks_1183.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
            das_copy((__mks_1183.id),(0));
            das_copy((__mks_1183.at),(range(0,23)));
            das_copy((__mks_1183.text),(nullptr));
            das_copy((__mks_1183.textLen),(0));
            das_move((__mks_1183.all),((([&]() -> TArray<regex::ReNode *> {
                TArray<regex::ReNode *> __mks_1183;
                das_zero(__mks_1183);
                return __mks_1183;
            })())));
            das_copy((__mks_1183.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_1183;
                das_zero(__mks_1183);
                das_copy((__mks_1183.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                das_copy((__mks_1183.id),(1));
                das_copy((__mks_1183.at),(range(0,18)));
                das_copy((__mks_1183.text),(nullptr));
                das_copy((__mks_1183.textLen),(0));
                das_move((__mks_1183.all),((([&]() -> TArray<regex::ReNode *> {
                    TArray<regex::ReNode *> __mks_1183;
                    das_zero(__mks_1183);
                    return __mks_1183;
                })())));
                das_copy((__mks_1183.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_1183;
                    das_zero(__mks_1183);
                    das_copy((__mks_1183.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                    das_copy((__mks_1183.id),(2));
                    das_copy((__mks_1183.at),(range(0,15)));
                    das_copy((__mks_1183.text),(nullptr));
                    das_copy((__mks_1183.textLen),(0));
                    das_move((__mks_1183.all),((([&]() -> TArray<regex::ReNode *> {
                        TArray<regex::ReNode *> __mks_1183;
                        das_zero(__mks_1183);
                        return __mks_1183;
                    })())));
                    das_copy((__mks_1183.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_1183;
                        das_zero(__mks_1183);
                        das_copy((__mks_1183.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                        das_copy((__mks_1183.id),(3));
                        das_copy((__mks_1183.at),(range(0,10)));
                        das_copy((__mks_1183.text),(nullptr));
                        das_copy((__mks_1183.textLen),(0));
                        das_move((__mks_1183.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_1183;
                            das_zero(__mks_1183);
                            return __mks_1183;
                        })())));
                        das_copy((__mks_1183.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                            regex::ReNode __mks_1183;
                            das_zero(__mks_1183);
                            das_copy((__mks_1183.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                            das_copy((__mks_1183.id),(4));
                            das_copy((__mks_1183.at),(range(0,7)));
                            das_copy((__mks_1183.text),(((char *) "#define")));
                            das_copy((__mks_1183.textLen),(7));
                            das_move((__mks_1183.all),((([&]() -> TArray<regex::ReNode *> {
                                TArray<regex::ReNode *> __mks_1183;
                                das_zero(__mks_1183);
                                return __mks_1183;
                            })())));
                            das_copy((__mks_1183.left),(nullptr));
                            das_copy((__mks_1183.right),(nullptr));
                            das_copy((__mks_1183.subexpr),(nullptr));
                            das_copy((__mks_1183.cset),((([&]() -> TDim<uint32_t,8> {
                                TDim<uint32_t,8> __mka_1183;
                                __mka_1183(0,__context__) = 0x0u;
                                __mka_1183(1,__context__) = 0x0u;
                                __mka_1183(2,__context__) = 0x0u;
                                __mka_1183(3,__context__) = 0x0u;
                                __mka_1183(4,__context__) = 0x0u;
                                __mka_1183(5,__context__) = 0x0u;
                                __mka_1183(6,__context__) = 0x0u;
                                __mka_1183(7,__context__) = 0x0u;
                                return __mka_1183;
                            })())));
                            das_copy((__mks_1183.index),(0));
                            return __mks_1183;
                        })()))));
                        das_copy((__mks_1183.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                            regex::ReNode __mks_1183;
                            das_zero(__mks_1183);
                            das_copy((__mks_1183.op),(DAS_COMMENT(enum) regex::ReOp::Plus));
                            das_copy((__mks_1183.id),(5));
                            das_copy((__mks_1183.at),(range(7,10)));
                            das_copy((__mks_1183.text),(nullptr));
                            das_copy((__mks_1183.textLen),(0));
                            das_move((__mks_1183.all),((([&]() -> TArray<regex::ReNode *> {
                                TArray<regex::ReNode *> __mks_1183;
                                das_zero(__mks_1183);
                                return __mks_1183;
                            })())));
                            das_copy((__mks_1183.left),(nullptr));
                            das_copy((__mks_1183.right),(nullptr));
                            das_copy((__mks_1183.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                regex::ReNode __mks_1183;
                                das_zero(__mks_1183);
                                das_copy((__mks_1183.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                                das_copy((__mks_1183.id),(6));
                                das_copy((__mks_1183.at),(range(7,9)));
                                das_copy((__mks_1183.text),(nullptr));
                                das_copy((__mks_1183.textLen),(0));
                                das_move((__mks_1183.all),((([&]() -> TArray<regex::ReNode *> {
                                    TArray<regex::ReNode *> __mks_1183;
                                    das_zero(__mks_1183);
                                    return __mks_1183;
                                })())));
                                das_copy((__mks_1183.left),(nullptr));
                                das_copy((__mks_1183.right),(nullptr));
                                das_copy((__mks_1183.subexpr),(nullptr));
                                das_copy((__mks_1183.cset),((([&]() -> TDim<uint32_t,8> {
                                    TDim<uint32_t,8> __mka_1183;
                                    __mka_1183(0,__context__) = 0x200u;
                                    __mka_1183(1,__context__) = 0x1u;
                                    __mka_1183(2,__context__) = 0x0u;
                                    __mka_1183(3,__context__) = 0x0u;
                                    __mka_1183(4,__context__) = 0x0u;
                                    __mka_1183(5,__context__) = 0x0u;
                                    __mka_1183(6,__context__) = 0x0u;
                                    __mka_1183(7,__context__) = 0x0u;
                                    return __mka_1183;
                                })())));
                                das_copy((__mks_1183.index),(0));
                                return __mks_1183;
                            })()))));
                            das_copy((__mks_1183.cset),((([&]() -> TDim<uint32_t,8> {
                                TDim<uint32_t,8> __mka_1183;
                                __mka_1183(0,__context__) = 0x200u;
                                __mka_1183(1,__context__) = 0x1u;
                                __mka_1183(2,__context__) = 0x0u;
                                __mka_1183(3,__context__) = 0x0u;
                                __mka_1183(4,__context__) = 0x0u;
                                __mka_1183(5,__context__) = 0x0u;
                                __mka_1183(6,__context__) = 0x0u;
                                __mka_1183(7,__context__) = 0x0u;
                                return __mka_1183;
                            })())));
                            das_copy((__mks_1183.index),(0));
                            return __mks_1183;
                        })()))));
                        das_copy((__mks_1183.subexpr),(nullptr));
                        das_copy((__mks_1183.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_1183;
                            __mka_1183(0,__context__) = 0x0u;
                            __mka_1183(1,__context__) = 0x0u;
                            __mka_1183(2,__context__) = 0x0u;
                            __mka_1183(3,__context__) = 0x0u;
                            __mka_1183(4,__context__) = 0x0u;
                            __mka_1183(5,__context__) = 0x0u;
                            __mka_1183(6,__context__) = 0x0u;
                            __mka_1183(7,__context__) = 0x0u;
                            return __mka_1183;
                        })())));
                        das_copy((__mks_1183.index),(0));
                        return __mks_1183;
                    })()))));
                    das_copy((__mks_1183.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_1183;
                        das_zero(__mks_1183);
                        das_copy((__mks_1183.op),(DAS_COMMENT(enum) regex::ReOp::Group));
                        das_copy((__mks_1183.id),(7));
                        das_copy((__mks_1183.at),(range(10,15)));
                        das_copy((__mks_1183.text),(nullptr));
                        das_copy((__mks_1183.textLen),(0));
                        das_move((__mks_1183.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_1183;
                            das_zero(__mks_1183);
                            return __mks_1183;
                        })())));
                        das_copy((__mks_1183.left),(nullptr));
                        das_copy((__mks_1183.right),(nullptr));
                        das_copy((__mks_1183.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                            regex::ReNode __mks_1183;
                            das_zero(__mks_1183);
                            das_copy((__mks_1183.op),(DAS_COMMENT(enum) regex::ReOp::Plus));
                            das_copy((__mks_1183.id),(8));
                            das_copy((__mks_1183.at),(range(11,14)));
                            das_copy((__mks_1183.text),(nullptr));
                            das_copy((__mks_1183.textLen),(0));
                            das_move((__mks_1183.all),((([&]() -> TArray<regex::ReNode *> {
                                TArray<regex::ReNode *> __mks_1183;
                                das_zero(__mks_1183);
                                return __mks_1183;
                            })())));
                            das_copy((__mks_1183.left),(nullptr));
                            das_copy((__mks_1183.right),(nullptr));
                            das_copy((__mks_1183.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                regex::ReNode __mks_1183;
                                das_zero(__mks_1183);
                                das_copy((__mks_1183.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                                das_copy((__mks_1183.id),(9));
                                das_copy((__mks_1183.at),(range(11,13)));
                                das_copy((__mks_1183.text),(nullptr));
                                das_copy((__mks_1183.textLen),(0));
                                das_move((__mks_1183.all),((([&]() -> TArray<regex::ReNode *> {
                                    TArray<regex::ReNode *> __mks_1183;
                                    das_zero(__mks_1183);
                                    return __mks_1183;
                                })())));
                                das_copy((__mks_1183.left),(nullptr));
                                das_copy((__mks_1183.right),(nullptr));
                                das_copy((__mks_1183.subexpr),(nullptr));
                                das_copy((__mks_1183.cset),((([&]() -> TDim<uint32_t,8> {
                                    TDim<uint32_t,8> __mka_1183;
                                    __mka_1183(0,__context__) = 0x0u;
                                    __mka_1183(1,__context__) = 0x3ff0000u;
                                    __mka_1183(2,__context__) = 0x87fffffeu;
                                    __mka_1183(3,__context__) = 0x7fffffeu;
                                    __mka_1183(4,__context__) = 0x0u;
                                    __mka_1183(5,__context__) = 0x0u;
                                    __mka_1183(6,__context__) = 0x0u;
                                    __mka_1183(7,__context__) = 0x0u;
                                    return __mka_1183;
                                })())));
                                das_copy((__mks_1183.index),(0));
                                return __mks_1183;
                            })()))));
                            das_copy((__mks_1183.cset),((([&]() -> TDim<uint32_t,8> {
                                TDim<uint32_t,8> __mka_1183;
                                __mka_1183(0,__context__) = 0x0u;
                                __mka_1183(1,__context__) = 0x3ff0000u;
                                __mka_1183(2,__context__) = 0x87fffffeu;
                                __mka_1183(3,__context__) = 0x7fffffeu;
                                __mka_1183(4,__context__) = 0x0u;
                                __mka_1183(5,__context__) = 0x0u;
                                __mka_1183(6,__context__) = 0x0u;
                                __mka_1183(7,__context__) = 0x0u;
                                return __mka_1183;
                            })())));
                            das_copy((__mks_1183.index),(0));
                            return __mks_1183;
                        })()))));
                        das_copy((__mks_1183.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_1183;
                            __mka_1183(0,__context__) = 0x0u;
                            __mka_1183(1,__context__) = 0x0u;
                            __mka_1183(2,__context__) = 0x0u;
                            __mka_1183(3,__context__) = 0x0u;
                            __mka_1183(4,__context__) = 0x0u;
                            __mka_1183(5,__context__) = 0x0u;
                            __mka_1183(6,__context__) = 0x0u;
                            __mka_1183(7,__context__) = 0x0u;
                            return __mka_1183;
                        })())));
                        das_copy((__mks_1183.index),(1));
                        return __mks_1183;
                    })()))));
                    das_copy((__mks_1183.subexpr),(nullptr));
                    das_copy((__mks_1183.cset),((([&]() -> TDim<uint32_t,8> {
                        TDim<uint32_t,8> __mka_1183;
                        __mka_1183(0,__context__) = 0x0u;
                        __mka_1183(1,__context__) = 0x0u;
                        __mka_1183(2,__context__) = 0x0u;
                        __mka_1183(3,__context__) = 0x0u;
                        __mka_1183(4,__context__) = 0x0u;
                        __mka_1183(5,__context__) = 0x0u;
                        __mka_1183(6,__context__) = 0x0u;
                        __mka_1183(7,__context__) = 0x0u;
                        return __mka_1183;
                    })())));
                    das_copy((__mks_1183.index),(0));
                    return __mks_1183;
                })()))));
                das_copy((__mks_1183.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_1183;
                    das_zero(__mks_1183);
                    das_copy((__mks_1183.op),(DAS_COMMENT(enum) regex::ReOp::Plus));
                    das_copy((__mks_1183.id),(10));
                    das_copy((__mks_1183.at),(range(15,18)));
                    das_copy((__mks_1183.text),(nullptr));
                    das_copy((__mks_1183.textLen),(0));
                    das_move((__mks_1183.all),((([&]() -> TArray<regex::ReNode *> {
                        TArray<regex::ReNode *> __mks_1183;
                        das_zero(__mks_1183);
                        return __mks_1183;
                    })())));
                    das_copy((__mks_1183.left),(nullptr));
                    das_copy((__mks_1183.right),(nullptr));
                    das_copy((__mks_1183.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_1183;
                        das_zero(__mks_1183);
                        das_copy((__mks_1183.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                        das_copy((__mks_1183.id),(11));
                        das_copy((__mks_1183.at),(range(15,17)));
                        das_copy((__mks_1183.text),(nullptr));
                        das_copy((__mks_1183.textLen),(0));
                        das_move((__mks_1183.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_1183;
                            das_zero(__mks_1183);
                            return __mks_1183;
                        })())));
                        das_copy((__mks_1183.left),(nullptr));
                        das_copy((__mks_1183.right),(nullptr));
                        das_copy((__mks_1183.subexpr),(nullptr));
                        das_copy((__mks_1183.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_1183;
                            __mka_1183(0,__context__) = 0x200u;
                            __mka_1183(1,__context__) = 0x1u;
                            __mka_1183(2,__context__) = 0x0u;
                            __mka_1183(3,__context__) = 0x0u;
                            __mka_1183(4,__context__) = 0x0u;
                            __mka_1183(5,__context__) = 0x0u;
                            __mka_1183(6,__context__) = 0x0u;
                            __mka_1183(7,__context__) = 0x0u;
                            return __mka_1183;
                        })())));
                        das_copy((__mks_1183.index),(0));
                        return __mks_1183;
                    })()))));
                    das_copy((__mks_1183.cset),((([&]() -> TDim<uint32_t,8> {
                        TDim<uint32_t,8> __mka_1183;
                        __mka_1183(0,__context__) = 0x200u;
                        __mka_1183(1,__context__) = 0x1u;
                        __mka_1183(2,__context__) = 0x0u;
                        __mka_1183(3,__context__) = 0x0u;
                        __mka_1183(4,__context__) = 0x0u;
                        __mka_1183(5,__context__) = 0x0u;
                        __mka_1183(6,__context__) = 0x0u;
                        __mka_1183(7,__context__) = 0x0u;
                        return __mka_1183;
                    })())));
                    das_copy((__mks_1183.index),(0));
                    return __mks_1183;
                })()))));
                das_copy((__mks_1183.subexpr),(nullptr));
                das_copy((__mks_1183.cset),((([&]() -> TDim<uint32_t,8> {
                    TDim<uint32_t,8> __mka_1183;
                    __mka_1183(0,__context__) = 0x0u;
                    __mka_1183(1,__context__) = 0x0u;
                    __mka_1183(2,__context__) = 0x0u;
                    __mka_1183(3,__context__) = 0x0u;
                    __mka_1183(4,__context__) = 0x0u;
                    __mka_1183(5,__context__) = 0x0u;
                    __mka_1183(6,__context__) = 0x0u;
                    __mka_1183(7,__context__) = 0x0u;
                    return __mka_1183;
                })())));
                das_copy((__mks_1183.index),(0));
                return __mks_1183;
            })()))));
            das_copy((__mks_1183.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_1183;
                das_zero(__mks_1183);
                das_copy((__mks_1183.op),(DAS_COMMENT(enum) regex::ReOp::Group));
                das_copy((__mks_1183.id),(12));
                das_copy((__mks_1183.at),(range(18,23)));
                das_copy((__mks_1183.text),(nullptr));
                das_copy((__mks_1183.textLen),(0));
                das_move((__mks_1183.all),((([&]() -> TArray<regex::ReNode *> {
                    TArray<regex::ReNode *> __mks_1183;
                    das_zero(__mks_1183);
                    return __mks_1183;
                })())));
                das_copy((__mks_1183.left),(nullptr));
                das_copy((__mks_1183.right),(nullptr));
                das_copy((__mks_1183.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_1183;
                    das_zero(__mks_1183);
                    das_copy((__mks_1183.op),(DAS_COMMENT(enum) regex::ReOp::Plus));
                    das_copy((__mks_1183.id),(13));
                    das_copy((__mks_1183.at),(range(19,22)));
                    das_copy((__mks_1183.text),(nullptr));
                    das_copy((__mks_1183.textLen),(0));
                    das_move((__mks_1183.all),((([&]() -> TArray<regex::ReNode *> {
                        TArray<regex::ReNode *> __mks_1183;
                        das_zero(__mks_1183);
                        return __mks_1183;
                    })())));
                    das_copy((__mks_1183.left),(nullptr));
                    das_copy((__mks_1183.right),(nullptr));
                    das_copy((__mks_1183.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_1183;
                        das_zero(__mks_1183);
                        das_copy((__mks_1183.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                        das_copy((__mks_1183.id),(14));
                        das_copy((__mks_1183.at),(range(19,21)));
                        das_copy((__mks_1183.text),(nullptr));
                        das_copy((__mks_1183.textLen),(0));
                        das_move((__mks_1183.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_1183;
                            das_zero(__mks_1183);
                            return __mks_1183;
                        })())));
                        das_copy((__mks_1183.left),(nullptr));
                        das_copy((__mks_1183.right),(nullptr));
                        das_copy((__mks_1183.subexpr),(nullptr));
                        das_copy((__mks_1183.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_1183;
                            __mka_1183(0,__context__) = 0x0u;
                            __mka_1183(1,__context__) = 0x3ff0000u;
                            __mka_1183(2,__context__) = 0x0u;
                            __mka_1183(3,__context__) = 0x0u;
                            __mka_1183(4,__context__) = 0x0u;
                            __mka_1183(5,__context__) = 0x0u;
                            __mka_1183(6,__context__) = 0x0u;
                            __mka_1183(7,__context__) = 0x0u;
                            return __mka_1183;
                        })())));
                        das_copy((__mks_1183.index),(0));
                        return __mks_1183;
                    })()))));
                    das_copy((__mks_1183.cset),((([&]() -> TDim<uint32_t,8> {
                        TDim<uint32_t,8> __mka_1183;
                        __mka_1183(0,__context__) = 0x0u;
                        __mka_1183(1,__context__) = 0x3ff0000u;
                        __mka_1183(2,__context__) = 0x0u;
                        __mka_1183(3,__context__) = 0x0u;
                        __mka_1183(4,__context__) = 0x0u;
                        __mka_1183(5,__context__) = 0x0u;
                        __mka_1183(6,__context__) = 0x0u;
                        __mka_1183(7,__context__) = 0x0u;
                        return __mka_1183;
                    })())));
                    das_copy((__mks_1183.index),(0));
                    return __mks_1183;
                })()))));
                das_copy((__mks_1183.cset),((([&]() -> TDim<uint32_t,8> {
                    TDim<uint32_t,8> __mka_1183;
                    __mka_1183(0,__context__) = 0x0u;
                    __mka_1183(1,__context__) = 0x0u;
                    __mka_1183(2,__context__) = 0x0u;
                    __mka_1183(3,__context__) = 0x0u;
                    __mka_1183(4,__context__) = 0x0u;
                    __mka_1183(5,__context__) = 0x0u;
                    __mka_1183(6,__context__) = 0x0u;
                    __mka_1183(7,__context__) = 0x0u;
                    return __mka_1183;
                })())));
                das_copy((__mks_1183.index),(2));
                return __mks_1183;
            })()))));
            das_copy((__mks_1183.subexpr),(nullptr));
            das_copy((__mks_1183.cset),((([&]() -> TDim<uint32_t,8> {
                TDim<uint32_t,8> __mka_1183;
                __mka_1183(0,__context__) = 0x0u;
                __mka_1183(1,__context__) = 0x0u;
                __mka_1183(2,__context__) = 0x0u;
                __mka_1183(3,__context__) = 0x0u;
                __mka_1183(4,__context__) = 0x0u;
                __mka_1183(5,__context__) = 0x0u;
                __mka_1183(6,__context__) = 0x0u;
                __mka_1183(7,__context__) = 0x0u;
                return __mka_1183;
            })())));
            das_copy((__mks_1183.index),(0));
            return __mks_1183;
        })()))));
        das_move((_temp_make_local_1183_23_608.groups),(_FuncbuiltinTickto_array_moveTick3185538323411982277_71fbdf75ac58194(__context__,das_arg<TDim<AutoTuple<range,char *>,3>>::pass((([&]() -> TDim<AutoTuple<range,char *>,3>& {
            _temp_make_local_1183_23_928(0,__context__) = (([&]() -> AutoTuple<range,char *> {
                AutoTuple<range,char *> __mkt_1183;
                das_get_auto_tuple_field<range,0,range,char *>::get(__mkt_1183) = range(0,0);
                das_get_auto_tuple_field<char *,1,range,char *>::get(__mkt_1183) = nullptr;
                return __mkt_1183;
            })());
            _temp_make_local_1183_23_928(1,__context__) = (([&]() -> AutoTuple<range,char *> {
                AutoTuple<range,char *> __mkt_1183;
                das_get_auto_tuple_field<range,0,range,char *>::get(__mkt_1183) = range(0,0);
                das_get_auto_tuple_field<char *,1,range,char *>::get(__mkt_1183) = ((char *) "1");
                return __mkt_1183;
            })());
            _temp_make_local_1183_23_928(2,__context__) = (([&]() -> AutoTuple<range,char *> {
                AutoTuple<range,char *> __mkt_1183;
                das_get_auto_tuple_field<range,0,range,char *>::get(__mkt_1183) = range(0,0);
                das_get_auto_tuple_field<char *,1,range,char *>::get(__mkt_1183) = ((char *) "2");
                return __mkt_1183;
            })());
            return _temp_make_local_1183_23_928;
        })())))));
        das_copy((_temp_make_local_1183_23_608.earlyOut),((([&]() -> TDim<uint32_t,8> {
            TDim<uint32_t,8> __mka_1183;
            __mka_1183(0,__context__) = 0x0u;
            __mka_1183(1,__context__) = 0x8u;
            __mka_1183(2,__context__) = 0x0u;
            __mka_1183(3,__context__) = 0x0u;
            __mka_1183(4,__context__) = 0x0u;
            __mka_1183(5,__context__) = 0x0u;
            __mka_1183(6,__context__) = 0x0u;
            __mka_1183(7,__context__) = 0x0u;
            return __mka_1183;
        })())));
        das_copy((_temp_make_local_1183_23_608.canEarlyOut),(true));
        return _temp_make_local_1183_23_608;
    })()))));
    regex::Regex __reg_def_UINT8_rename_at_1184_349; das_zero(__reg_def_UINT8_rename_at_1184_349); das_move(__reg_def_UINT8_rename_at_1184_349, das_invoke_function<regex::Regex>::invoke_cmres<regex::Regex &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::regex_compile S<regex::Regex>*/ 0x235aa29958cfc4c4)),das_arg<regex::Regex>::pass((([&]() -> regex::Regex& {
        das_zero(_temp_make_local_1184_25_1056);
        das_copy((_temp_make_local_1184_25_1056.root),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
            regex::ReNode __mks_1184;
            das_zero(__mks_1184);
            das_copy((__mks_1184.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
            das_copy((__mks_1184.id),(0));
            das_copy((__mks_1184.at),(range(0,48)));
            das_copy((__mks_1184.text),(nullptr));
            das_copy((__mks_1184.textLen),(0));
            das_move((__mks_1184.all),((([&]() -> TArray<regex::ReNode *> {
                TArray<regex::ReNode *> __mks_1184;
                das_zero(__mks_1184);
                return __mks_1184;
            })())));
            das_copy((__mks_1184.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_1184;
                das_zero(__mks_1184);
                das_copy((__mks_1184.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                das_copy((__mks_1184.id),(1));
                das_copy((__mks_1184.at),(range(0,46)));
                das_copy((__mks_1184.text),(nullptr));
                das_copy((__mks_1184.textLen),(0));
                das_move((__mks_1184.all),((([&]() -> TArray<regex::ReNode *> {
                    TArray<regex::ReNode *> __mks_1184;
                    das_zero(__mks_1184);
                    return __mks_1184;
                })())));
                das_copy((__mks_1184.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_1184;
                    das_zero(__mks_1184);
                    das_copy((__mks_1184.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                    das_copy((__mks_1184.id),(2));
                    das_copy((__mks_1184.at),(range(0,30)));
                    das_copy((__mks_1184.text),(nullptr));
                    das_copy((__mks_1184.textLen),(0));
                    das_move((__mks_1184.all),((([&]() -> TArray<regex::ReNode *> {
                        TArray<regex::ReNode *> __mks_1184;
                        das_zero(__mks_1184);
                        return __mks_1184;
                    })())));
                    das_copy((__mks_1184.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_1184;
                        das_zero(__mks_1184);
                        das_copy((__mks_1184.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                        das_copy((__mks_1184.id),(3));
                        das_copy((__mks_1184.at),(range(0,28)));
                        das_copy((__mks_1184.text),(nullptr));
                        das_copy((__mks_1184.textLen),(0));
                        das_move((__mks_1184.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_1184;
                            das_zero(__mks_1184);
                            return __mks_1184;
                        })())));
                        das_copy((__mks_1184.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                            regex::ReNode __mks_1184;
                            das_zero(__mks_1184);
                            das_copy((__mks_1184.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                            das_copy((__mks_1184.id),(4));
                            das_copy((__mks_1184.at),(range(0,25)));
                            das_copy((__mks_1184.text),(nullptr));
                            das_copy((__mks_1184.textLen),(0));
                            das_move((__mks_1184.all),((([&]() -> TArray<regex::ReNode *> {
                                TArray<regex::ReNode *> __mks_1184;
                                das_zero(__mks_1184);
                                return __mks_1184;
                            })())));
                            das_copy((__mks_1184.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                regex::ReNode __mks_1184;
                                das_zero(__mks_1184);
                                das_copy((__mks_1184.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                                das_copy((__mks_1184.id),(5));
                                das_copy((__mks_1184.at),(range(0,18)));
                                das_copy((__mks_1184.text),(nullptr));
                                das_copy((__mks_1184.textLen),(0));
                                das_move((__mks_1184.all),((([&]() -> TArray<regex::ReNode *> {
                                    TArray<regex::ReNode *> __mks_1184;
                                    das_zero(__mks_1184);
                                    return __mks_1184;
                                })())));
                                das_copy((__mks_1184.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                    regex::ReNode __mks_1184;
                                    das_zero(__mks_1184);
                                    das_copy((__mks_1184.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                                    das_copy((__mks_1184.id),(6));
                                    das_copy((__mks_1184.at),(range(0,15)));
                                    das_copy((__mks_1184.text),(nullptr));
                                    das_copy((__mks_1184.textLen),(0));
                                    das_move((__mks_1184.all),((([&]() -> TArray<regex::ReNode *> {
                                        TArray<regex::ReNode *> __mks_1184;
                                        das_zero(__mks_1184);
                                        return __mks_1184;
                                    })())));
                                    das_copy((__mks_1184.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                        regex::ReNode __mks_1184;
                                        das_zero(__mks_1184);
                                        das_copy((__mks_1184.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                                        das_copy((__mks_1184.id),(7));
                                        das_copy((__mks_1184.at),(range(0,10)));
                                        das_copy((__mks_1184.text),(nullptr));
                                        das_copy((__mks_1184.textLen),(0));
                                        das_move((__mks_1184.all),((([&]() -> TArray<regex::ReNode *> {
                                            TArray<regex::ReNode *> __mks_1184;
                                            das_zero(__mks_1184);
                                            return __mks_1184;
                                        })())));
                                        das_copy((__mks_1184.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                            regex::ReNode __mks_1184;
                                            das_zero(__mks_1184);
                                            das_copy((__mks_1184.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                                            das_copy((__mks_1184.id),(8));
                                            das_copy((__mks_1184.at),(range(0,7)));
                                            das_copy((__mks_1184.text),(((char *) "#define")));
                                            das_copy((__mks_1184.textLen),(7));
                                            das_move((__mks_1184.all),((([&]() -> TArray<regex::ReNode *> {
                                                TArray<regex::ReNode *> __mks_1184;
                                                das_zero(__mks_1184);
                                                return __mks_1184;
                                            })())));
                                            das_copy((__mks_1184.left),(nullptr));
                                            das_copy((__mks_1184.right),(nullptr));
                                            das_copy((__mks_1184.subexpr),(nullptr));
                                            das_copy((__mks_1184.cset),((([&]() -> TDim<uint32_t,8> {
                                                TDim<uint32_t,8> __mka_1184;
                                                __mka_1184(0,__context__) = 0x0u;
                                                __mka_1184(1,__context__) = 0x0u;
                                                __mka_1184(2,__context__) = 0x0u;
                                                __mka_1184(3,__context__) = 0x0u;
                                                __mka_1184(4,__context__) = 0x0u;
                                                __mka_1184(5,__context__) = 0x0u;
                                                __mka_1184(6,__context__) = 0x0u;
                                                __mka_1184(7,__context__) = 0x0u;
                                                return __mka_1184;
                                            })())));
                                            das_copy((__mks_1184.index),(0));
                                            return __mks_1184;
                                        })()))));
                                        das_copy((__mks_1184.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                            regex::ReNode __mks_1184;
                                            das_zero(__mks_1184);
                                            das_copy((__mks_1184.op),(DAS_COMMENT(enum) regex::ReOp::Plus));
                                            das_copy((__mks_1184.id),(9));
                                            das_copy((__mks_1184.at),(range(7,10)));
                                            das_copy((__mks_1184.text),(nullptr));
                                            das_copy((__mks_1184.textLen),(0));
                                            das_move((__mks_1184.all),((([&]() -> TArray<regex::ReNode *> {
                                                TArray<regex::ReNode *> __mks_1184;
                                                das_zero(__mks_1184);
                                                return __mks_1184;
                                            })())));
                                            das_copy((__mks_1184.left),(nullptr));
                                            das_copy((__mks_1184.right),(nullptr));
                                            das_copy((__mks_1184.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                                regex::ReNode __mks_1184;
                                                das_zero(__mks_1184);
                                                das_copy((__mks_1184.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                                                das_copy((__mks_1184.id),(10));
                                                das_copy((__mks_1184.at),(range(7,9)));
                                                das_copy((__mks_1184.text),(nullptr));
                                                das_copy((__mks_1184.textLen),(0));
                                                das_move((__mks_1184.all),((([&]() -> TArray<regex::ReNode *> {
                                                    TArray<regex::ReNode *> __mks_1184;
                                                    das_zero(__mks_1184);
                                                    return __mks_1184;
                                                })())));
                                                das_copy((__mks_1184.left),(nullptr));
                                                das_copy((__mks_1184.right),(nullptr));
                                                das_copy((__mks_1184.subexpr),(nullptr));
                                                das_copy((__mks_1184.cset),((([&]() -> TDim<uint32_t,8> {
                                                    TDim<uint32_t,8> __mka_1184;
                                                    __mka_1184(0,__context__) = 0x200u;
                                                    __mka_1184(1,__context__) = 0x1u;
                                                    __mka_1184(2,__context__) = 0x0u;
                                                    __mka_1184(3,__context__) = 0x0u;
                                                    __mka_1184(4,__context__) = 0x0u;
                                                    __mka_1184(5,__context__) = 0x0u;
                                                    __mka_1184(6,__context__) = 0x0u;
                                                    __mka_1184(7,__context__) = 0x0u;
                                                    return __mka_1184;
                                                })())));
                                                das_copy((__mks_1184.index),(0));
                                                return __mks_1184;
                                            })()))));
                                            das_copy((__mks_1184.cset),((([&]() -> TDim<uint32_t,8> {
                                                TDim<uint32_t,8> __mka_1184;
                                                __mka_1184(0,__context__) = 0x200u;
                                                __mka_1184(1,__context__) = 0x1u;
                                                __mka_1184(2,__context__) = 0x0u;
                                                __mka_1184(3,__context__) = 0x0u;
                                                __mka_1184(4,__context__) = 0x0u;
                                                __mka_1184(5,__context__) = 0x0u;
                                                __mka_1184(6,__context__) = 0x0u;
                                                __mka_1184(7,__context__) = 0x0u;
                                                return __mka_1184;
                                            })())));
                                            das_copy((__mks_1184.index),(0));
                                            return __mks_1184;
                                        })()))));
                                        das_copy((__mks_1184.subexpr),(nullptr));
                                        das_copy((__mks_1184.cset),((([&]() -> TDim<uint32_t,8> {
                                            TDim<uint32_t,8> __mka_1184;
                                            __mka_1184(0,__context__) = 0x0u;
                                            __mka_1184(1,__context__) = 0x0u;
                                            __mka_1184(2,__context__) = 0x0u;
                                            __mka_1184(3,__context__) = 0x0u;
                                            __mka_1184(4,__context__) = 0x0u;
                                            __mka_1184(5,__context__) = 0x0u;
                                            __mka_1184(6,__context__) = 0x0u;
                                            __mka_1184(7,__context__) = 0x0u;
                                            return __mka_1184;
                                        })())));
                                        das_copy((__mks_1184.index),(0));
                                        return __mks_1184;
                                    })()))));
                                    das_copy((__mks_1184.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                        regex::ReNode __mks_1184;
                                        das_zero(__mks_1184);
                                        das_copy((__mks_1184.op),(DAS_COMMENT(enum) regex::ReOp::Group));
                                        das_copy((__mks_1184.id),(11));
                                        das_copy((__mks_1184.at),(range(10,15)));
                                        das_copy((__mks_1184.text),(nullptr));
                                        das_copy((__mks_1184.textLen),(0));
                                        das_move((__mks_1184.all),((([&]() -> TArray<regex::ReNode *> {
                                            TArray<regex::ReNode *> __mks_1184;
                                            das_zero(__mks_1184);
                                            return __mks_1184;
                                        })())));
                                        das_copy((__mks_1184.left),(nullptr));
                                        das_copy((__mks_1184.right),(nullptr));
                                        das_copy((__mks_1184.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                            regex::ReNode __mks_1184;
                                            das_zero(__mks_1184);
                                            das_copy((__mks_1184.op),(DAS_COMMENT(enum) regex::ReOp::Plus));
                                            das_copy((__mks_1184.id),(12));
                                            das_copy((__mks_1184.at),(range(11,14)));
                                            das_copy((__mks_1184.text),(nullptr));
                                            das_copy((__mks_1184.textLen),(0));
                                            das_move((__mks_1184.all),((([&]() -> TArray<regex::ReNode *> {
                                                TArray<regex::ReNode *> __mks_1184;
                                                das_zero(__mks_1184);
                                                return __mks_1184;
                                            })())));
                                            das_copy((__mks_1184.left),(nullptr));
                                            das_copy((__mks_1184.right),(nullptr));
                                            das_copy((__mks_1184.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                                regex::ReNode __mks_1184;
                                                das_zero(__mks_1184);
                                                das_copy((__mks_1184.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                                                das_copy((__mks_1184.id),(13));
                                                das_copy((__mks_1184.at),(range(11,13)));
                                                das_copy((__mks_1184.text),(nullptr));
                                                das_copy((__mks_1184.textLen),(0));
                                                das_move((__mks_1184.all),((([&]() -> TArray<regex::ReNode *> {
                                                    TArray<regex::ReNode *> __mks_1184;
                                                    das_zero(__mks_1184);
                                                    return __mks_1184;
                                                })())));
                                                das_copy((__mks_1184.left),(nullptr));
                                                das_copy((__mks_1184.right),(nullptr));
                                                das_copy((__mks_1184.subexpr),(nullptr));
                                                das_copy((__mks_1184.cset),((([&]() -> TDim<uint32_t,8> {
                                                    TDim<uint32_t,8> __mka_1184;
                                                    __mka_1184(0,__context__) = 0x0u;
                                                    __mka_1184(1,__context__) = 0x3ff0000u;
                                                    __mka_1184(2,__context__) = 0x87fffffeu;
                                                    __mka_1184(3,__context__) = 0x7fffffeu;
                                                    __mka_1184(4,__context__) = 0x0u;
                                                    __mka_1184(5,__context__) = 0x0u;
                                                    __mka_1184(6,__context__) = 0x0u;
                                                    __mka_1184(7,__context__) = 0x0u;
                                                    return __mka_1184;
                                                })())));
                                                das_copy((__mks_1184.index),(0));
                                                return __mks_1184;
                                            })()))));
                                            das_copy((__mks_1184.cset),((([&]() -> TDim<uint32_t,8> {
                                                TDim<uint32_t,8> __mka_1184;
                                                __mka_1184(0,__context__) = 0x0u;
                                                __mka_1184(1,__context__) = 0x3ff0000u;
                                                __mka_1184(2,__context__) = 0x87fffffeu;
                                                __mka_1184(3,__context__) = 0x7fffffeu;
                                                __mka_1184(4,__context__) = 0x0u;
                                                __mka_1184(5,__context__) = 0x0u;
                                                __mka_1184(6,__context__) = 0x0u;
                                                __mka_1184(7,__context__) = 0x0u;
                                                return __mka_1184;
                                            })())));
                                            das_copy((__mks_1184.index),(0));
                                            return __mks_1184;
                                        })()))));
                                        das_copy((__mks_1184.cset),((([&]() -> TDim<uint32_t,8> {
                                            TDim<uint32_t,8> __mka_1184;
                                            __mka_1184(0,__context__) = 0x0u;
                                            __mka_1184(1,__context__) = 0x0u;
                                            __mka_1184(2,__context__) = 0x0u;
                                            __mka_1184(3,__context__) = 0x0u;
                                            __mka_1184(4,__context__) = 0x0u;
                                            __mka_1184(5,__context__) = 0x0u;
                                            __mka_1184(6,__context__) = 0x0u;
                                            __mka_1184(7,__context__) = 0x0u;
                                            return __mka_1184;
                                        })())));
                                        das_copy((__mks_1184.index),(1));
                                        return __mks_1184;
                                    })()))));
                                    das_copy((__mks_1184.subexpr),(nullptr));
                                    das_copy((__mks_1184.cset),((([&]() -> TDim<uint32_t,8> {
                                        TDim<uint32_t,8> __mka_1184;
                                        __mka_1184(0,__context__) = 0x0u;
                                        __mka_1184(1,__context__) = 0x0u;
                                        __mka_1184(2,__context__) = 0x0u;
                                        __mka_1184(3,__context__) = 0x0u;
                                        __mka_1184(4,__context__) = 0x0u;
                                        __mka_1184(5,__context__) = 0x0u;
                                        __mka_1184(6,__context__) = 0x0u;
                                        __mka_1184(7,__context__) = 0x0u;
                                        return __mka_1184;
                                    })())));
                                    das_copy((__mks_1184.index),(0));
                                    return __mks_1184;
                                })()))));
                                das_copy((__mks_1184.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                    regex::ReNode __mks_1184;
                                    das_zero(__mks_1184);
                                    das_copy((__mks_1184.op),(DAS_COMMENT(enum) regex::ReOp::Plus));
                                    das_copy((__mks_1184.id),(14));
                                    das_copy((__mks_1184.at),(range(15,18)));
                                    das_copy((__mks_1184.text),(nullptr));
                                    das_copy((__mks_1184.textLen),(0));
                                    das_move((__mks_1184.all),((([&]() -> TArray<regex::ReNode *> {
                                        TArray<regex::ReNode *> __mks_1184;
                                        das_zero(__mks_1184);
                                        return __mks_1184;
                                    })())));
                                    das_copy((__mks_1184.left),(nullptr));
                                    das_copy((__mks_1184.right),(nullptr));
                                    das_copy((__mks_1184.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                        regex::ReNode __mks_1184;
                                        das_zero(__mks_1184);
                                        das_copy((__mks_1184.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                                        das_copy((__mks_1184.id),(15));
                                        das_copy((__mks_1184.at),(range(15,17)));
                                        das_copy((__mks_1184.text),(nullptr));
                                        das_copy((__mks_1184.textLen),(0));
                                        das_move((__mks_1184.all),((([&]() -> TArray<regex::ReNode *> {
                                            TArray<regex::ReNode *> __mks_1184;
                                            das_zero(__mks_1184);
                                            return __mks_1184;
                                        })())));
                                        das_copy((__mks_1184.left),(nullptr));
                                        das_copy((__mks_1184.right),(nullptr));
                                        das_copy((__mks_1184.subexpr),(nullptr));
                                        das_copy((__mks_1184.cset),((([&]() -> TDim<uint32_t,8> {
                                            TDim<uint32_t,8> __mka_1184;
                                            __mka_1184(0,__context__) = 0x200u;
                                            __mka_1184(1,__context__) = 0x1u;
                                            __mka_1184(2,__context__) = 0x0u;
                                            __mka_1184(3,__context__) = 0x0u;
                                            __mka_1184(4,__context__) = 0x0u;
                                            __mka_1184(5,__context__) = 0x0u;
                                            __mka_1184(6,__context__) = 0x0u;
                                            __mka_1184(7,__context__) = 0x0u;
                                            return __mka_1184;
                                        })())));
                                        das_copy((__mks_1184.index),(0));
                                        return __mks_1184;
                                    })()))));
                                    das_copy((__mks_1184.cset),((([&]() -> TDim<uint32_t,8> {
                                        TDim<uint32_t,8> __mka_1184;
                                        __mka_1184(0,__context__) = 0x200u;
                                        __mka_1184(1,__context__) = 0x1u;
                                        __mka_1184(2,__context__) = 0x0u;
                                        __mka_1184(3,__context__) = 0x0u;
                                        __mka_1184(4,__context__) = 0x0u;
                                        __mka_1184(5,__context__) = 0x0u;
                                        __mka_1184(6,__context__) = 0x0u;
                                        __mka_1184(7,__context__) = 0x0u;
                                        return __mka_1184;
                                    })())));
                                    das_copy((__mks_1184.index),(0));
                                    return __mks_1184;
                                })()))));
                                das_copy((__mks_1184.subexpr),(nullptr));
                                das_copy((__mks_1184.cset),((([&]() -> TDim<uint32_t,8> {
                                    TDim<uint32_t,8> __mka_1184;
                                    __mka_1184(0,__context__) = 0x0u;
                                    __mka_1184(1,__context__) = 0x0u;
                                    __mka_1184(2,__context__) = 0x0u;
                                    __mka_1184(3,__context__) = 0x0u;
                                    __mka_1184(4,__context__) = 0x0u;
                                    __mka_1184(5,__context__) = 0x0u;
                                    __mka_1184(6,__context__) = 0x0u;
                                    __mka_1184(7,__context__) = 0x0u;
                                    return __mka_1184;
                                })())));
                                das_copy((__mks_1184.index),(0));
                                return __mks_1184;
                            })()))));
                            das_copy((__mks_1184.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                regex::ReNode __mks_1184;
                                das_zero(__mks_1184);
                                das_copy((__mks_1184.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                                das_copy((__mks_1184.id),(16));
                                das_copy((__mks_1184.at),(range(18,25)));
                                das_copy((__mks_1184.text),(((char *) "UINT8_C")));
                                das_copy((__mks_1184.textLen),(7));
                                das_move((__mks_1184.all),((([&]() -> TArray<regex::ReNode *> {
                                    TArray<regex::ReNode *> __mks_1184;
                                    das_zero(__mks_1184);
                                    return __mks_1184;
                                })())));
                                das_copy((__mks_1184.left),(nullptr));
                                das_copy((__mks_1184.right),(nullptr));
                                das_copy((__mks_1184.subexpr),(nullptr));
                                das_copy((__mks_1184.cset),((([&]() -> TDim<uint32_t,8> {
                                    TDim<uint32_t,8> __mka_1184;
                                    __mka_1184(0,__context__) = 0x0u;
                                    __mka_1184(1,__context__) = 0x0u;
                                    __mka_1184(2,__context__) = 0x0u;
                                    __mka_1184(3,__context__) = 0x0u;
                                    __mka_1184(4,__context__) = 0x0u;
                                    __mka_1184(5,__context__) = 0x0u;
                                    __mka_1184(6,__context__) = 0x0u;
                                    __mka_1184(7,__context__) = 0x0u;
                                    return __mka_1184;
                                })())));
                                das_copy((__mks_1184.index),(0));
                                return __mks_1184;
                            })()))));
                            das_copy((__mks_1184.subexpr),(nullptr));
                            das_copy((__mks_1184.cset),((([&]() -> TDim<uint32_t,8> {
                                TDim<uint32_t,8> __mka_1184;
                                __mka_1184(0,__context__) = 0x0u;
                                __mka_1184(1,__context__) = 0x0u;
                                __mka_1184(2,__context__) = 0x0u;
                                __mka_1184(3,__context__) = 0x0u;
                                __mka_1184(4,__context__) = 0x0u;
                                __mka_1184(5,__context__) = 0x0u;
                                __mka_1184(6,__context__) = 0x0u;
                                __mka_1184(7,__context__) = 0x0u;
                                return __mka_1184;
                            })())));
                            das_copy((__mks_1184.index),(0));
                            return __mks_1184;
                        })()))));
                        das_copy((__mks_1184.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                            regex::ReNode __mks_1184;
                            das_zero(__mks_1184);
                            das_copy((__mks_1184.op),(DAS_COMMENT(enum) regex::ReOp::Star));
                            das_copy((__mks_1184.id),(17));
                            das_copy((__mks_1184.at),(range(25,28)));
                            das_copy((__mks_1184.text),(nullptr));
                            das_copy((__mks_1184.textLen),(0));
                            das_move((__mks_1184.all),((([&]() -> TArray<regex::ReNode *> {
                                TArray<regex::ReNode *> __mks_1184;
                                das_zero(__mks_1184);
                                return __mks_1184;
                            })())));
                            das_copy((__mks_1184.left),(nullptr));
                            das_copy((__mks_1184.right),(nullptr));
                            das_copy((__mks_1184.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                regex::ReNode __mks_1184;
                                das_zero(__mks_1184);
                                das_copy((__mks_1184.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                                das_copy((__mks_1184.id),(18));
                                das_copy((__mks_1184.at),(range(25,27)));
                                das_copy((__mks_1184.text),(nullptr));
                                das_copy((__mks_1184.textLen),(0));
                                das_move((__mks_1184.all),((([&]() -> TArray<regex::ReNode *> {
                                    TArray<regex::ReNode *> __mks_1184;
                                    das_zero(__mks_1184);
                                    return __mks_1184;
                                })())));
                                das_copy((__mks_1184.left),(nullptr));
                                das_copy((__mks_1184.right),(nullptr));
                                das_copy((__mks_1184.subexpr),(nullptr));
                                das_copy((__mks_1184.cset),((([&]() -> TDim<uint32_t,8> {
                                    TDim<uint32_t,8> __mka_1184;
                                    __mka_1184(0,__context__) = 0x200u;
                                    __mka_1184(1,__context__) = 0x1u;
                                    __mka_1184(2,__context__) = 0x0u;
                                    __mka_1184(3,__context__) = 0x0u;
                                    __mka_1184(4,__context__) = 0x0u;
                                    __mka_1184(5,__context__) = 0x0u;
                                    __mka_1184(6,__context__) = 0x0u;
                                    __mka_1184(7,__context__) = 0x0u;
                                    return __mka_1184;
                                })())));
                                das_copy((__mks_1184.index),(0));
                                return __mks_1184;
                            })()))));
                            das_copy((__mks_1184.cset),((([&]() -> TDim<uint32_t,8> {
                                TDim<uint32_t,8> __mka_1184;
                                __mka_1184(0,__context__) = 0x0u;
                                __mka_1184(1,__context__) = 0x0u;
                                __mka_1184(2,__context__) = 0x0u;
                                __mka_1184(3,__context__) = 0x0u;
                                __mka_1184(4,__context__) = 0x0u;
                                __mka_1184(5,__context__) = 0x0u;
                                __mka_1184(6,__context__) = 0x0u;
                                __mka_1184(7,__context__) = 0x0u;
                                return __mka_1184;
                            })())));
                            das_copy((__mks_1184.index),(0));
                            return __mks_1184;
                        })()))));
                        das_copy((__mks_1184.subexpr),(nullptr));
                        das_copy((__mks_1184.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_1184;
                            __mka_1184(0,__context__) = 0x0u;
                            __mka_1184(1,__context__) = 0x0u;
                            __mka_1184(2,__context__) = 0x0u;
                            __mka_1184(3,__context__) = 0x0u;
                            __mka_1184(4,__context__) = 0x0u;
                            __mka_1184(5,__context__) = 0x0u;
                            __mka_1184(6,__context__) = 0x0u;
                            __mka_1184(7,__context__) = 0x0u;
                            return __mka_1184;
                        })())));
                        das_copy((__mks_1184.index),(0));
                        return __mks_1184;
                    })()))));
                    das_copy((__mks_1184.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_1184;
                        das_zero(__mks_1184);
                        das_copy((__mks_1184.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                        das_copy((__mks_1184.id),(19));
                        das_copy((__mks_1184.at),(range(28,30)));
                        das_copy((__mks_1184.text),(((char *) "(")));
                        das_copy((__mks_1184.textLen),(1));
                        das_move((__mks_1184.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_1184;
                            das_zero(__mks_1184);
                            return __mks_1184;
                        })())));
                        das_copy((__mks_1184.left),(nullptr));
                        das_copy((__mks_1184.right),(nullptr));
                        das_copy((__mks_1184.subexpr),(nullptr));
                        das_copy((__mks_1184.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_1184;
                            __mka_1184(0,__context__) = 0x0u;
                            __mka_1184(1,__context__) = 0x0u;
                            __mka_1184(2,__context__) = 0x0u;
                            __mka_1184(3,__context__) = 0x0u;
                            __mka_1184(4,__context__) = 0x0u;
                            __mka_1184(5,__context__) = 0x0u;
                            __mka_1184(6,__context__) = 0x0u;
                            __mka_1184(7,__context__) = 0x0u;
                            return __mka_1184;
                        })())));
                        das_copy((__mks_1184.index),(0));
                        return __mks_1184;
                    })()))));
                    das_copy((__mks_1184.subexpr),(nullptr));
                    das_copy((__mks_1184.cset),((([&]() -> TDim<uint32_t,8> {
                        TDim<uint32_t,8> __mka_1184;
                        __mka_1184(0,__context__) = 0x0u;
                        __mka_1184(1,__context__) = 0x0u;
                        __mka_1184(2,__context__) = 0x0u;
                        __mka_1184(3,__context__) = 0x0u;
                        __mka_1184(4,__context__) = 0x0u;
                        __mka_1184(5,__context__) = 0x0u;
                        __mka_1184(6,__context__) = 0x0u;
                        __mka_1184(7,__context__) = 0x0u;
                        return __mka_1184;
                    })())));
                    das_copy((__mks_1184.index),(0));
                    return __mks_1184;
                })()))));
                das_copy((__mks_1184.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_1184;
                    das_zero(__mks_1184);
                    das_copy((__mks_1184.op),(DAS_COMMENT(enum) regex::ReOp::Group));
                    das_copy((__mks_1184.id),(20));
                    das_copy((__mks_1184.at),(range(30,46)));
                    das_copy((__mks_1184.text),(nullptr));
                    das_copy((__mks_1184.textLen),(0));
                    das_move((__mks_1184.all),((([&]() -> TArray<regex::ReNode *> {
                        TArray<regex::ReNode *> __mks_1184;
                        das_zero(__mks_1184);
                        return __mks_1184;
                    })())));
                    das_copy((__mks_1184.left),(nullptr));
                    das_copy((__mks_1184.right),(nullptr));
                    das_copy((__mks_1184.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_1184;
                        das_zero(__mks_1184);
                        das_copy((__mks_1184.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                        das_copy((__mks_1184.id),(21));
                        das_copy((__mks_1184.at),(range(31,45)));
                        das_copy((__mks_1184.text),(nullptr));
                        das_copy((__mks_1184.textLen),(0));
                        das_move((__mks_1184.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_1184;
                            das_zero(__mks_1184);
                            return __mks_1184;
                        })())));
                        das_copy((__mks_1184.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                            regex::ReNode __mks_1184;
                            das_zero(__mks_1184);
                            das_copy((__mks_1184.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                            das_copy((__mks_1184.id),(22));
                            das_copy((__mks_1184.at),(range(31,33)));
                            das_copy((__mks_1184.text),(((char *) "0x")));
                            das_copy((__mks_1184.textLen),(2));
                            das_move((__mks_1184.all),((([&]() -> TArray<regex::ReNode *> {
                                TArray<regex::ReNode *> __mks_1184;
                                das_zero(__mks_1184);
                                return __mks_1184;
                            })())));
                            das_copy((__mks_1184.left),(nullptr));
                            das_copy((__mks_1184.right),(nullptr));
                            das_copy((__mks_1184.subexpr),(nullptr));
                            das_copy((__mks_1184.cset),((([&]() -> TDim<uint32_t,8> {
                                TDim<uint32_t,8> __mka_1184;
                                __mka_1184(0,__context__) = 0x0u;
                                __mka_1184(1,__context__) = 0x0u;
                                __mka_1184(2,__context__) = 0x0u;
                                __mka_1184(3,__context__) = 0x0u;
                                __mka_1184(4,__context__) = 0x0u;
                                __mka_1184(5,__context__) = 0x0u;
                                __mka_1184(6,__context__) = 0x0u;
                                __mka_1184(7,__context__) = 0x0u;
                                return __mka_1184;
                            })())));
                            das_copy((__mks_1184.index),(0));
                            return __mks_1184;
                        })()))));
                        das_copy((__mks_1184.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                            regex::ReNode __mks_1184;
                            das_zero(__mks_1184);
                            das_copy((__mks_1184.op),(DAS_COMMENT(enum) regex::ReOp::Plus));
                            das_copy((__mks_1184.id),(23));
                            das_copy((__mks_1184.at),(range(33,45)));
                            das_copy((__mks_1184.text),(nullptr));
                            das_copy((__mks_1184.textLen),(0));
                            das_move((__mks_1184.all),((([&]() -> TArray<regex::ReNode *> {
                                TArray<regex::ReNode *> __mks_1184;
                                das_zero(__mks_1184);
                                return __mks_1184;
                            })())));
                            das_copy((__mks_1184.left),(nullptr));
                            das_copy((__mks_1184.right),(nullptr));
                            das_copy((__mks_1184.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                regex::ReNode __mks_1184;
                                das_zero(__mks_1184);
                                das_copy((__mks_1184.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                                das_copy((__mks_1184.id),(24));
                                das_copy((__mks_1184.at),(range(33,44)));
                                das_copy((__mks_1184.text),(nullptr));
                                das_copy((__mks_1184.textLen),(0));
                                das_move((__mks_1184.all),((([&]() -> TArray<regex::ReNode *> {
                                    TArray<regex::ReNode *> __mks_1184;
                                    das_zero(__mks_1184);
                                    return __mks_1184;
                                })())));
                                das_copy((__mks_1184.left),(nullptr));
                                das_copy((__mks_1184.right),(nullptr));
                                das_copy((__mks_1184.subexpr),(nullptr));
                                das_copy((__mks_1184.cset),((([&]() -> TDim<uint32_t,8> {
                                    TDim<uint32_t,8> __mka_1184;
                                    __mka_1184(0,__context__) = 0x0u;
                                    __mka_1184(1,__context__) = 0x3ff0000u;
                                    __mka_1184(2,__context__) = 0x7fffffeu;
                                    __mka_1184(3,__context__) = 0x7fffffeu;
                                    __mka_1184(4,__context__) = 0x0u;
                                    __mka_1184(5,__context__) = 0x0u;
                                    __mka_1184(6,__context__) = 0x0u;
                                    __mka_1184(7,__context__) = 0x0u;
                                    return __mka_1184;
                                })())));
                                das_copy((__mks_1184.index),(0));
                                return __mks_1184;
                            })()))));
                            das_copy((__mks_1184.cset),((([&]() -> TDim<uint32_t,8> {
                                TDim<uint32_t,8> __mka_1184;
                                __mka_1184(0,__context__) = 0x0u;
                                __mka_1184(1,__context__) = 0x3ff0000u;
                                __mka_1184(2,__context__) = 0x7fffffeu;
                                __mka_1184(3,__context__) = 0x7fffffeu;
                                __mka_1184(4,__context__) = 0x0u;
                                __mka_1184(5,__context__) = 0x0u;
                                __mka_1184(6,__context__) = 0x0u;
                                __mka_1184(7,__context__) = 0x0u;
                                return __mka_1184;
                            })())));
                            das_copy((__mks_1184.index),(0));
                            return __mks_1184;
                        })()))));
                        das_copy((__mks_1184.subexpr),(nullptr));
                        das_copy((__mks_1184.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_1184;
                            __mka_1184(0,__context__) = 0x0u;
                            __mka_1184(1,__context__) = 0x0u;
                            __mka_1184(2,__context__) = 0x0u;
                            __mka_1184(3,__context__) = 0x0u;
                            __mka_1184(4,__context__) = 0x0u;
                            __mka_1184(5,__context__) = 0x0u;
                            __mka_1184(6,__context__) = 0x0u;
                            __mka_1184(7,__context__) = 0x0u;
                            return __mka_1184;
                        })())));
                        das_copy((__mks_1184.index),(0));
                        return __mks_1184;
                    })()))));
                    das_copy((__mks_1184.cset),((([&]() -> TDim<uint32_t,8> {
                        TDim<uint32_t,8> __mka_1184;
                        __mka_1184(0,__context__) = 0x0u;
                        __mka_1184(1,__context__) = 0x0u;
                        __mka_1184(2,__context__) = 0x0u;
                        __mka_1184(3,__context__) = 0x0u;
                        __mka_1184(4,__context__) = 0x0u;
                        __mka_1184(5,__context__) = 0x0u;
                        __mka_1184(6,__context__) = 0x0u;
                        __mka_1184(7,__context__) = 0x0u;
                        return __mka_1184;
                    })())));
                    das_copy((__mks_1184.index),(2));
                    return __mks_1184;
                })()))));
                das_copy((__mks_1184.subexpr),(nullptr));
                das_copy((__mks_1184.cset),((([&]() -> TDim<uint32_t,8> {
                    TDim<uint32_t,8> __mka_1184;
                    __mka_1184(0,__context__) = 0x0u;
                    __mka_1184(1,__context__) = 0x0u;
                    __mka_1184(2,__context__) = 0x0u;
                    __mka_1184(3,__context__) = 0x0u;
                    __mka_1184(4,__context__) = 0x0u;
                    __mka_1184(5,__context__) = 0x0u;
                    __mka_1184(6,__context__) = 0x0u;
                    __mka_1184(7,__context__) = 0x0u;
                    return __mka_1184;
                })())));
                das_copy((__mks_1184.index),(0));
                return __mks_1184;
            })()))));
            das_copy((__mks_1184.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_1184;
                das_zero(__mks_1184);
                das_copy((__mks_1184.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                das_copy((__mks_1184.id),(25));
                das_copy((__mks_1184.at),(range(46,48)));
                das_copy((__mks_1184.text),(((char *) ")")));
                das_copy((__mks_1184.textLen),(1));
                das_move((__mks_1184.all),((([&]() -> TArray<regex::ReNode *> {
                    TArray<regex::ReNode *> __mks_1184;
                    das_zero(__mks_1184);
                    return __mks_1184;
                })())));
                das_copy((__mks_1184.left),(nullptr));
                das_copy((__mks_1184.right),(nullptr));
                das_copy((__mks_1184.subexpr),(nullptr));
                das_copy((__mks_1184.cset),((([&]() -> TDim<uint32_t,8> {
                    TDim<uint32_t,8> __mka_1184;
                    __mka_1184(0,__context__) = 0x0u;
                    __mka_1184(1,__context__) = 0x0u;
                    __mka_1184(2,__context__) = 0x0u;
                    __mka_1184(3,__context__) = 0x0u;
                    __mka_1184(4,__context__) = 0x0u;
                    __mka_1184(5,__context__) = 0x0u;
                    __mka_1184(6,__context__) = 0x0u;
                    __mka_1184(7,__context__) = 0x0u;
                    return __mka_1184;
                })())));
                das_copy((__mks_1184.index),(0));
                return __mks_1184;
            })()))));
            das_copy((__mks_1184.subexpr),(nullptr));
            das_copy((__mks_1184.cset),((([&]() -> TDim<uint32_t,8> {
                TDim<uint32_t,8> __mka_1184;
                __mka_1184(0,__context__) = 0x0u;
                __mka_1184(1,__context__) = 0x0u;
                __mka_1184(2,__context__) = 0x0u;
                __mka_1184(3,__context__) = 0x0u;
                __mka_1184(4,__context__) = 0x0u;
                __mka_1184(5,__context__) = 0x0u;
                __mka_1184(6,__context__) = 0x0u;
                __mka_1184(7,__context__) = 0x0u;
                return __mka_1184;
            })())));
            das_copy((__mks_1184.index),(0));
            return __mks_1184;
        })()))));
        das_move((_temp_make_local_1184_25_1056.groups),(_FuncbuiltinTickto_array_moveTick3185538323411982277_71fbdf75ac58194(__context__,das_arg<TDim<AutoTuple<range,char *>,3>>::pass((([&]() -> TDim<AutoTuple<range,char *>,3>& {
            _temp_make_local_1184_25_1552(0,__context__) = (([&]() -> AutoTuple<range,char *> {
                AutoTuple<range,char *> __mkt_1184;
                das_get_auto_tuple_field<range,0,range,char *>::get(__mkt_1184) = range(0,0);
                das_get_auto_tuple_field<char *,1,range,char *>::get(__mkt_1184) = nullptr;
                return __mkt_1184;
            })());
            _temp_make_local_1184_25_1552(1,__context__) = (([&]() -> AutoTuple<range,char *> {
                AutoTuple<range,char *> __mkt_1184;
                das_get_auto_tuple_field<range,0,range,char *>::get(__mkt_1184) = range(0,0);
                das_get_auto_tuple_field<char *,1,range,char *>::get(__mkt_1184) = ((char *) "1");
                return __mkt_1184;
            })());
            _temp_make_local_1184_25_1552(2,__context__) = (([&]() -> AutoTuple<range,char *> {
                AutoTuple<range,char *> __mkt_1184;
                das_get_auto_tuple_field<range,0,range,char *>::get(__mkt_1184) = range(0,0);
                das_get_auto_tuple_field<char *,1,range,char *>::get(__mkt_1184) = ((char *) "2");
                return __mkt_1184;
            })());
            return _temp_make_local_1184_25_1552;
        })())))));
        das_copy((_temp_make_local_1184_25_1056.earlyOut),((([&]() -> TDim<uint32_t,8> {
            TDim<uint32_t,8> __mka_1184;
            __mka_1184(0,__context__) = 0x0u;
            __mka_1184(1,__context__) = 0x8u;
            __mka_1184(2,__context__) = 0x0u;
            __mka_1184(3,__context__) = 0x0u;
            __mka_1184(4,__context__) = 0x0u;
            __mka_1184(5,__context__) = 0x0u;
            __mka_1184(6,__context__) = 0x0u;
            __mka_1184(7,__context__) = 0x0u;
            return __mka_1184;
        })())));
        das_copy((_temp_make_local_1184_25_1056.canEarlyOut),(true));
        return _temp_make_local_1184_25_1056;
    })()))));
    regex::Regex __reg_def_UINT16_rename_at_1185_350; das_zero(__reg_def_UINT16_rename_at_1185_350); das_move(__reg_def_UINT16_rename_at_1185_350, das_invoke_function<regex::Regex>::invoke_cmres<regex::Regex &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::regex_compile S<regex::Regex>*/ 0x235aa29958cfc4c4)),das_arg<regex::Regex>::pass((([&]() -> regex::Regex& {
        das_zero(_temp_make_local_1185_26_1680);
        das_copy((_temp_make_local_1185_26_1680.root),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
            regex::ReNode __mks_1185;
            das_zero(__mks_1185);
            das_copy((__mks_1185.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
            das_copy((__mks_1185.id),(0));
            das_copy((__mks_1185.at),(range(0,49)));
            das_copy((__mks_1185.text),(nullptr));
            das_copy((__mks_1185.textLen),(0));
            das_move((__mks_1185.all),((([&]() -> TArray<regex::ReNode *> {
                TArray<regex::ReNode *> __mks_1185;
                das_zero(__mks_1185);
                return __mks_1185;
            })())));
            das_copy((__mks_1185.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_1185;
                das_zero(__mks_1185);
                das_copy((__mks_1185.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                das_copy((__mks_1185.id),(1));
                das_copy((__mks_1185.at),(range(0,47)));
                das_copy((__mks_1185.text),(nullptr));
                das_copy((__mks_1185.textLen),(0));
                das_move((__mks_1185.all),((([&]() -> TArray<regex::ReNode *> {
                    TArray<regex::ReNode *> __mks_1185;
                    das_zero(__mks_1185);
                    return __mks_1185;
                })())));
                das_copy((__mks_1185.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_1185;
                    das_zero(__mks_1185);
                    das_copy((__mks_1185.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                    das_copy((__mks_1185.id),(2));
                    das_copy((__mks_1185.at),(range(0,31)));
                    das_copy((__mks_1185.text),(nullptr));
                    das_copy((__mks_1185.textLen),(0));
                    das_move((__mks_1185.all),((([&]() -> TArray<regex::ReNode *> {
                        TArray<regex::ReNode *> __mks_1185;
                        das_zero(__mks_1185);
                        return __mks_1185;
                    })())));
                    das_copy((__mks_1185.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_1185;
                        das_zero(__mks_1185);
                        das_copy((__mks_1185.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                        das_copy((__mks_1185.id),(3));
                        das_copy((__mks_1185.at),(range(0,29)));
                        das_copy((__mks_1185.text),(nullptr));
                        das_copy((__mks_1185.textLen),(0));
                        das_move((__mks_1185.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_1185;
                            das_zero(__mks_1185);
                            return __mks_1185;
                        })())));
                        das_copy((__mks_1185.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                            regex::ReNode __mks_1185;
                            das_zero(__mks_1185);
                            das_copy((__mks_1185.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                            das_copy((__mks_1185.id),(4));
                            das_copy((__mks_1185.at),(range(0,26)));
                            das_copy((__mks_1185.text),(nullptr));
                            das_copy((__mks_1185.textLen),(0));
                            das_move((__mks_1185.all),((([&]() -> TArray<regex::ReNode *> {
                                TArray<regex::ReNode *> __mks_1185;
                                das_zero(__mks_1185);
                                return __mks_1185;
                            })())));
                            das_copy((__mks_1185.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                regex::ReNode __mks_1185;
                                das_zero(__mks_1185);
                                das_copy((__mks_1185.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                                das_copy((__mks_1185.id),(5));
                                das_copy((__mks_1185.at),(range(0,18)));
                                das_copy((__mks_1185.text),(nullptr));
                                das_copy((__mks_1185.textLen),(0));
                                das_move((__mks_1185.all),((([&]() -> TArray<regex::ReNode *> {
                                    TArray<regex::ReNode *> __mks_1185;
                                    das_zero(__mks_1185);
                                    return __mks_1185;
                                })())));
                                das_copy((__mks_1185.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                    regex::ReNode __mks_1185;
                                    das_zero(__mks_1185);
                                    das_copy((__mks_1185.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                                    das_copy((__mks_1185.id),(6));
                                    das_copy((__mks_1185.at),(range(0,15)));
                                    das_copy((__mks_1185.text),(nullptr));
                                    das_copy((__mks_1185.textLen),(0));
                                    das_move((__mks_1185.all),((([&]() -> TArray<regex::ReNode *> {
                                        TArray<regex::ReNode *> __mks_1185;
                                        das_zero(__mks_1185);
                                        return __mks_1185;
                                    })())));
                                    das_copy((__mks_1185.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                        regex::ReNode __mks_1185;
                                        das_zero(__mks_1185);
                                        das_copy((__mks_1185.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                                        das_copy((__mks_1185.id),(7));
                                        das_copy((__mks_1185.at),(range(0,10)));
                                        das_copy((__mks_1185.text),(nullptr));
                                        das_copy((__mks_1185.textLen),(0));
                                        das_move((__mks_1185.all),((([&]() -> TArray<regex::ReNode *> {
                                            TArray<regex::ReNode *> __mks_1185;
                                            das_zero(__mks_1185);
                                            return __mks_1185;
                                        })())));
                                        das_copy((__mks_1185.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                            regex::ReNode __mks_1185;
                                            das_zero(__mks_1185);
                                            das_copy((__mks_1185.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                                            das_copy((__mks_1185.id),(8));
                                            das_copy((__mks_1185.at),(range(0,7)));
                                            das_copy((__mks_1185.text),(((char *) "#define")));
                                            das_copy((__mks_1185.textLen),(7));
                                            das_move((__mks_1185.all),((([&]() -> TArray<regex::ReNode *> {
                                                TArray<regex::ReNode *> __mks_1185;
                                                das_zero(__mks_1185);
                                                return __mks_1185;
                                            })())));
                                            das_copy((__mks_1185.left),(nullptr));
                                            das_copy((__mks_1185.right),(nullptr));
                                            das_copy((__mks_1185.subexpr),(nullptr));
                                            das_copy((__mks_1185.cset),((([&]() -> TDim<uint32_t,8> {
                                                TDim<uint32_t,8> __mka_1185;
                                                __mka_1185(0,__context__) = 0x0u;
                                                __mka_1185(1,__context__) = 0x0u;
                                                __mka_1185(2,__context__) = 0x0u;
                                                __mka_1185(3,__context__) = 0x0u;
                                                __mka_1185(4,__context__) = 0x0u;
                                                __mka_1185(5,__context__) = 0x0u;
                                                __mka_1185(6,__context__) = 0x0u;
                                                __mka_1185(7,__context__) = 0x0u;
                                                return __mka_1185;
                                            })())));
                                            das_copy((__mks_1185.index),(0));
                                            return __mks_1185;
                                        })()))));
                                        das_copy((__mks_1185.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                            regex::ReNode __mks_1185;
                                            das_zero(__mks_1185);
                                            das_copy((__mks_1185.op),(DAS_COMMENT(enum) regex::ReOp::Plus));
                                            das_copy((__mks_1185.id),(9));
                                            das_copy((__mks_1185.at),(range(7,10)));
                                            das_copy((__mks_1185.text),(nullptr));
                                            das_copy((__mks_1185.textLen),(0));
                                            das_move((__mks_1185.all),((([&]() -> TArray<regex::ReNode *> {
                                                TArray<regex::ReNode *> __mks_1185;
                                                das_zero(__mks_1185);
                                                return __mks_1185;
                                            })())));
                                            das_copy((__mks_1185.left),(nullptr));
                                            das_copy((__mks_1185.right),(nullptr));
                                            das_copy((__mks_1185.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                                regex::ReNode __mks_1185;
                                                das_zero(__mks_1185);
                                                das_copy((__mks_1185.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                                                das_copy((__mks_1185.id),(10));
                                                das_copy((__mks_1185.at),(range(7,9)));
                                                das_copy((__mks_1185.text),(nullptr));
                                                das_copy((__mks_1185.textLen),(0));
                                                das_move((__mks_1185.all),((([&]() -> TArray<regex::ReNode *> {
                                                    TArray<regex::ReNode *> __mks_1185;
                                                    das_zero(__mks_1185);
                                                    return __mks_1185;
                                                })())));
                                                das_copy((__mks_1185.left),(nullptr));
                                                das_copy((__mks_1185.right),(nullptr));
                                                das_copy((__mks_1185.subexpr),(nullptr));
                                                das_copy((__mks_1185.cset),((([&]() -> TDim<uint32_t,8> {
                                                    TDim<uint32_t,8> __mka_1185;
                                                    __mka_1185(0,__context__) = 0x200u;
                                                    __mka_1185(1,__context__) = 0x1u;
                                                    __mka_1185(2,__context__) = 0x0u;
                                                    __mka_1185(3,__context__) = 0x0u;
                                                    __mka_1185(4,__context__) = 0x0u;
                                                    __mka_1185(5,__context__) = 0x0u;
                                                    __mka_1185(6,__context__) = 0x0u;
                                                    __mka_1185(7,__context__) = 0x0u;
                                                    return __mka_1185;
                                                })())));
                                                das_copy((__mks_1185.index),(0));
                                                return __mks_1185;
                                            })()))));
                                            das_copy((__mks_1185.cset),((([&]() -> TDim<uint32_t,8> {
                                                TDim<uint32_t,8> __mka_1185;
                                                __mka_1185(0,__context__) = 0x200u;
                                                __mka_1185(1,__context__) = 0x1u;
                                                __mka_1185(2,__context__) = 0x0u;
                                                __mka_1185(3,__context__) = 0x0u;
                                                __mka_1185(4,__context__) = 0x0u;
                                                __mka_1185(5,__context__) = 0x0u;
                                                __mka_1185(6,__context__) = 0x0u;
                                                __mka_1185(7,__context__) = 0x0u;
                                                return __mka_1185;
                                            })())));
                                            das_copy((__mks_1185.index),(0));
                                            return __mks_1185;
                                        })()))));
                                        das_copy((__mks_1185.subexpr),(nullptr));
                                        das_copy((__mks_1185.cset),((([&]() -> TDim<uint32_t,8> {
                                            TDim<uint32_t,8> __mka_1185;
                                            __mka_1185(0,__context__) = 0x0u;
                                            __mka_1185(1,__context__) = 0x0u;
                                            __mka_1185(2,__context__) = 0x0u;
                                            __mka_1185(3,__context__) = 0x0u;
                                            __mka_1185(4,__context__) = 0x0u;
                                            __mka_1185(5,__context__) = 0x0u;
                                            __mka_1185(6,__context__) = 0x0u;
                                            __mka_1185(7,__context__) = 0x0u;
                                            return __mka_1185;
                                        })())));
                                        das_copy((__mks_1185.index),(0));
                                        return __mks_1185;
                                    })()))));
                                    das_copy((__mks_1185.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                        regex::ReNode __mks_1185;
                                        das_zero(__mks_1185);
                                        das_copy((__mks_1185.op),(DAS_COMMENT(enum) regex::ReOp::Group));
                                        das_copy((__mks_1185.id),(11));
                                        das_copy((__mks_1185.at),(range(10,15)));
                                        das_copy((__mks_1185.text),(nullptr));
                                        das_copy((__mks_1185.textLen),(0));
                                        das_move((__mks_1185.all),((([&]() -> TArray<regex::ReNode *> {
                                            TArray<regex::ReNode *> __mks_1185;
                                            das_zero(__mks_1185);
                                            return __mks_1185;
                                        })())));
                                        das_copy((__mks_1185.left),(nullptr));
                                        das_copy((__mks_1185.right),(nullptr));
                                        das_copy((__mks_1185.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                            regex::ReNode __mks_1185;
                                            das_zero(__mks_1185);
                                            das_copy((__mks_1185.op),(DAS_COMMENT(enum) regex::ReOp::Plus));
                                            das_copy((__mks_1185.id),(12));
                                            das_copy((__mks_1185.at),(range(11,14)));
                                            das_copy((__mks_1185.text),(nullptr));
                                            das_copy((__mks_1185.textLen),(0));
                                            das_move((__mks_1185.all),((([&]() -> TArray<regex::ReNode *> {
                                                TArray<regex::ReNode *> __mks_1185;
                                                das_zero(__mks_1185);
                                                return __mks_1185;
                                            })())));
                                            das_copy((__mks_1185.left),(nullptr));
                                            das_copy((__mks_1185.right),(nullptr));
                                            das_copy((__mks_1185.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                                regex::ReNode __mks_1185;
                                                das_zero(__mks_1185);
                                                das_copy((__mks_1185.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                                                das_copy((__mks_1185.id),(13));
                                                das_copy((__mks_1185.at),(range(11,13)));
                                                das_copy((__mks_1185.text),(nullptr));
                                                das_copy((__mks_1185.textLen),(0));
                                                das_move((__mks_1185.all),((([&]() -> TArray<regex::ReNode *> {
                                                    TArray<regex::ReNode *> __mks_1185;
                                                    das_zero(__mks_1185);
                                                    return __mks_1185;
                                                })())));
                                                das_copy((__mks_1185.left),(nullptr));
                                                das_copy((__mks_1185.right),(nullptr));
                                                das_copy((__mks_1185.subexpr),(nullptr));
                                                das_copy((__mks_1185.cset),((([&]() -> TDim<uint32_t,8> {
                                                    TDim<uint32_t,8> __mka_1185;
                                                    __mka_1185(0,__context__) = 0x0u;
                                                    __mka_1185(1,__context__) = 0x3ff0000u;
                                                    __mka_1185(2,__context__) = 0x87fffffeu;
                                                    __mka_1185(3,__context__) = 0x7fffffeu;
                                                    __mka_1185(4,__context__) = 0x0u;
                                                    __mka_1185(5,__context__) = 0x0u;
                                                    __mka_1185(6,__context__) = 0x0u;
                                                    __mka_1185(7,__context__) = 0x0u;
                                                    return __mka_1185;
                                                })())));
                                                das_copy((__mks_1185.index),(0));
                                                return __mks_1185;
                                            })()))));
                                            das_copy((__mks_1185.cset),((([&]() -> TDim<uint32_t,8> {
                                                TDim<uint32_t,8> __mka_1185;
                                                __mka_1185(0,__context__) = 0x0u;
                                                __mka_1185(1,__context__) = 0x3ff0000u;
                                                __mka_1185(2,__context__) = 0x87fffffeu;
                                                __mka_1185(3,__context__) = 0x7fffffeu;
                                                __mka_1185(4,__context__) = 0x0u;
                                                __mka_1185(5,__context__) = 0x0u;
                                                __mka_1185(6,__context__) = 0x0u;
                                                __mka_1185(7,__context__) = 0x0u;
                                                return __mka_1185;
                                            })())));
                                            das_copy((__mks_1185.index),(0));
                                            return __mks_1185;
                                        })()))));
                                        das_copy((__mks_1185.cset),((([&]() -> TDim<uint32_t,8> {
                                            TDim<uint32_t,8> __mka_1185;
                                            __mka_1185(0,__context__) = 0x0u;
                                            __mka_1185(1,__context__) = 0x0u;
                                            __mka_1185(2,__context__) = 0x0u;
                                            __mka_1185(3,__context__) = 0x0u;
                                            __mka_1185(4,__context__) = 0x0u;
                                            __mka_1185(5,__context__) = 0x0u;
                                            __mka_1185(6,__context__) = 0x0u;
                                            __mka_1185(7,__context__) = 0x0u;
                                            return __mka_1185;
                                        })())));
                                        das_copy((__mks_1185.index),(1));
                                        return __mks_1185;
                                    })()))));
                                    das_copy((__mks_1185.subexpr),(nullptr));
                                    das_copy((__mks_1185.cset),((([&]() -> TDim<uint32_t,8> {
                                        TDim<uint32_t,8> __mka_1185;
                                        __mka_1185(0,__context__) = 0x0u;
                                        __mka_1185(1,__context__) = 0x0u;
                                        __mka_1185(2,__context__) = 0x0u;
                                        __mka_1185(3,__context__) = 0x0u;
                                        __mka_1185(4,__context__) = 0x0u;
                                        __mka_1185(5,__context__) = 0x0u;
                                        __mka_1185(6,__context__) = 0x0u;
                                        __mka_1185(7,__context__) = 0x0u;
                                        return __mka_1185;
                                    })())));
                                    das_copy((__mks_1185.index),(0));
                                    return __mks_1185;
                                })()))));
                                das_copy((__mks_1185.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                    regex::ReNode __mks_1185;
                                    das_zero(__mks_1185);
                                    das_copy((__mks_1185.op),(DAS_COMMENT(enum) regex::ReOp::Plus));
                                    das_copy((__mks_1185.id),(14));
                                    das_copy((__mks_1185.at),(range(15,18)));
                                    das_copy((__mks_1185.text),(nullptr));
                                    das_copy((__mks_1185.textLen),(0));
                                    das_move((__mks_1185.all),((([&]() -> TArray<regex::ReNode *> {
                                        TArray<regex::ReNode *> __mks_1185;
                                        das_zero(__mks_1185);
                                        return __mks_1185;
                                    })())));
                                    das_copy((__mks_1185.left),(nullptr));
                                    das_copy((__mks_1185.right),(nullptr));
                                    das_copy((__mks_1185.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                        regex::ReNode __mks_1185;
                                        das_zero(__mks_1185);
                                        das_copy((__mks_1185.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                                        das_copy((__mks_1185.id),(15));
                                        das_copy((__mks_1185.at),(range(15,17)));
                                        das_copy((__mks_1185.text),(nullptr));
                                        das_copy((__mks_1185.textLen),(0));
                                        das_move((__mks_1185.all),((([&]() -> TArray<regex::ReNode *> {
                                            TArray<regex::ReNode *> __mks_1185;
                                            das_zero(__mks_1185);
                                            return __mks_1185;
                                        })())));
                                        das_copy((__mks_1185.left),(nullptr));
                                        das_copy((__mks_1185.right),(nullptr));
                                        das_copy((__mks_1185.subexpr),(nullptr));
                                        das_copy((__mks_1185.cset),((([&]() -> TDim<uint32_t,8> {
                                            TDim<uint32_t,8> __mka_1185;
                                            __mka_1185(0,__context__) = 0x200u;
                                            __mka_1185(1,__context__) = 0x1u;
                                            __mka_1185(2,__context__) = 0x0u;
                                            __mka_1185(3,__context__) = 0x0u;
                                            __mka_1185(4,__context__) = 0x0u;
                                            __mka_1185(5,__context__) = 0x0u;
                                            __mka_1185(6,__context__) = 0x0u;
                                            __mka_1185(7,__context__) = 0x0u;
                                            return __mka_1185;
                                        })())));
                                        das_copy((__mks_1185.index),(0));
                                        return __mks_1185;
                                    })()))));
                                    das_copy((__mks_1185.cset),((([&]() -> TDim<uint32_t,8> {
                                        TDim<uint32_t,8> __mka_1185;
                                        __mka_1185(0,__context__) = 0x200u;
                                        __mka_1185(1,__context__) = 0x1u;
                                        __mka_1185(2,__context__) = 0x0u;
                                        __mka_1185(3,__context__) = 0x0u;
                                        __mka_1185(4,__context__) = 0x0u;
                                        __mka_1185(5,__context__) = 0x0u;
                                        __mka_1185(6,__context__) = 0x0u;
                                        __mka_1185(7,__context__) = 0x0u;
                                        return __mka_1185;
                                    })())));
                                    das_copy((__mks_1185.index),(0));
                                    return __mks_1185;
                                })()))));
                                das_copy((__mks_1185.subexpr),(nullptr));
                                das_copy((__mks_1185.cset),((([&]() -> TDim<uint32_t,8> {
                                    TDim<uint32_t,8> __mka_1185;
                                    __mka_1185(0,__context__) = 0x0u;
                                    __mka_1185(1,__context__) = 0x0u;
                                    __mka_1185(2,__context__) = 0x0u;
                                    __mka_1185(3,__context__) = 0x0u;
                                    __mka_1185(4,__context__) = 0x0u;
                                    __mka_1185(5,__context__) = 0x0u;
                                    __mka_1185(6,__context__) = 0x0u;
                                    __mka_1185(7,__context__) = 0x0u;
                                    return __mka_1185;
                                })())));
                                das_copy((__mks_1185.index),(0));
                                return __mks_1185;
                            })()))));
                            das_copy((__mks_1185.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                regex::ReNode __mks_1185;
                                das_zero(__mks_1185);
                                das_copy((__mks_1185.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                                das_copy((__mks_1185.id),(16));
                                das_copy((__mks_1185.at),(range(18,26)));
                                das_copy((__mks_1185.text),(((char *) "UINT16_C")));
                                das_copy((__mks_1185.textLen),(8));
                                das_move((__mks_1185.all),((([&]() -> TArray<regex::ReNode *> {
                                    TArray<regex::ReNode *> __mks_1185;
                                    das_zero(__mks_1185);
                                    return __mks_1185;
                                })())));
                                das_copy((__mks_1185.left),(nullptr));
                                das_copy((__mks_1185.right),(nullptr));
                                das_copy((__mks_1185.subexpr),(nullptr));
                                das_copy((__mks_1185.cset),((([&]() -> TDim<uint32_t,8> {
                                    TDim<uint32_t,8> __mka_1185;
                                    __mka_1185(0,__context__) = 0x0u;
                                    __mka_1185(1,__context__) = 0x0u;
                                    __mka_1185(2,__context__) = 0x0u;
                                    __mka_1185(3,__context__) = 0x0u;
                                    __mka_1185(4,__context__) = 0x0u;
                                    __mka_1185(5,__context__) = 0x0u;
                                    __mka_1185(6,__context__) = 0x0u;
                                    __mka_1185(7,__context__) = 0x0u;
                                    return __mka_1185;
                                })())));
                                das_copy((__mks_1185.index),(0));
                                return __mks_1185;
                            })()))));
                            das_copy((__mks_1185.subexpr),(nullptr));
                            das_copy((__mks_1185.cset),((([&]() -> TDim<uint32_t,8> {
                                TDim<uint32_t,8> __mka_1185;
                                __mka_1185(0,__context__) = 0x0u;
                                __mka_1185(1,__context__) = 0x0u;
                                __mka_1185(2,__context__) = 0x0u;
                                __mka_1185(3,__context__) = 0x0u;
                                __mka_1185(4,__context__) = 0x0u;
                                __mka_1185(5,__context__) = 0x0u;
                                __mka_1185(6,__context__) = 0x0u;
                                __mka_1185(7,__context__) = 0x0u;
                                return __mka_1185;
                            })())));
                            das_copy((__mks_1185.index),(0));
                            return __mks_1185;
                        })()))));
                        das_copy((__mks_1185.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                            regex::ReNode __mks_1185;
                            das_zero(__mks_1185);
                            das_copy((__mks_1185.op),(DAS_COMMENT(enum) regex::ReOp::Star));
                            das_copy((__mks_1185.id),(17));
                            das_copy((__mks_1185.at),(range(26,29)));
                            das_copy((__mks_1185.text),(nullptr));
                            das_copy((__mks_1185.textLen),(0));
                            das_move((__mks_1185.all),((([&]() -> TArray<regex::ReNode *> {
                                TArray<regex::ReNode *> __mks_1185;
                                das_zero(__mks_1185);
                                return __mks_1185;
                            })())));
                            das_copy((__mks_1185.left),(nullptr));
                            das_copy((__mks_1185.right),(nullptr));
                            das_copy((__mks_1185.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                regex::ReNode __mks_1185;
                                das_zero(__mks_1185);
                                das_copy((__mks_1185.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                                das_copy((__mks_1185.id),(18));
                                das_copy((__mks_1185.at),(range(26,28)));
                                das_copy((__mks_1185.text),(nullptr));
                                das_copy((__mks_1185.textLen),(0));
                                das_move((__mks_1185.all),((([&]() -> TArray<regex::ReNode *> {
                                    TArray<regex::ReNode *> __mks_1185;
                                    das_zero(__mks_1185);
                                    return __mks_1185;
                                })())));
                                das_copy((__mks_1185.left),(nullptr));
                                das_copy((__mks_1185.right),(nullptr));
                                das_copy((__mks_1185.subexpr),(nullptr));
                                das_copy((__mks_1185.cset),((([&]() -> TDim<uint32_t,8> {
                                    TDim<uint32_t,8> __mka_1185;
                                    __mka_1185(0,__context__) = 0x200u;
                                    __mka_1185(1,__context__) = 0x1u;
                                    __mka_1185(2,__context__) = 0x0u;
                                    __mka_1185(3,__context__) = 0x0u;
                                    __mka_1185(4,__context__) = 0x0u;
                                    __mka_1185(5,__context__) = 0x0u;
                                    __mka_1185(6,__context__) = 0x0u;
                                    __mka_1185(7,__context__) = 0x0u;
                                    return __mka_1185;
                                })())));
                                das_copy((__mks_1185.index),(0));
                                return __mks_1185;
                            })()))));
                            das_copy((__mks_1185.cset),((([&]() -> TDim<uint32_t,8> {
                                TDim<uint32_t,8> __mka_1185;
                                __mka_1185(0,__context__) = 0x0u;
                                __mka_1185(1,__context__) = 0x0u;
                                __mka_1185(2,__context__) = 0x0u;
                                __mka_1185(3,__context__) = 0x0u;
                                __mka_1185(4,__context__) = 0x0u;
                                __mka_1185(5,__context__) = 0x0u;
                                __mka_1185(6,__context__) = 0x0u;
                                __mka_1185(7,__context__) = 0x0u;
                                return __mka_1185;
                            })())));
                            das_copy((__mks_1185.index),(0));
                            return __mks_1185;
                        })()))));
                        das_copy((__mks_1185.subexpr),(nullptr));
                        das_copy((__mks_1185.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_1185;
                            __mka_1185(0,__context__) = 0x0u;
                            __mka_1185(1,__context__) = 0x0u;
                            __mka_1185(2,__context__) = 0x0u;
                            __mka_1185(3,__context__) = 0x0u;
                            __mka_1185(4,__context__) = 0x0u;
                            __mka_1185(5,__context__) = 0x0u;
                            __mka_1185(6,__context__) = 0x0u;
                            __mka_1185(7,__context__) = 0x0u;
                            return __mka_1185;
                        })())));
                        das_copy((__mks_1185.index),(0));
                        return __mks_1185;
                    })()))));
                    das_copy((__mks_1185.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_1185;
                        das_zero(__mks_1185);
                        das_copy((__mks_1185.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                        das_copy((__mks_1185.id),(19));
                        das_copy((__mks_1185.at),(range(29,31)));
                        das_copy((__mks_1185.text),(((char *) "(")));
                        das_copy((__mks_1185.textLen),(1));
                        das_move((__mks_1185.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_1185;
                            das_zero(__mks_1185);
                            return __mks_1185;
                        })())));
                        das_copy((__mks_1185.left),(nullptr));
                        das_copy((__mks_1185.right),(nullptr));
                        das_copy((__mks_1185.subexpr),(nullptr));
                        das_copy((__mks_1185.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_1185;
                            __mka_1185(0,__context__) = 0x0u;
                            __mka_1185(1,__context__) = 0x0u;
                            __mka_1185(2,__context__) = 0x0u;
                            __mka_1185(3,__context__) = 0x0u;
                            __mka_1185(4,__context__) = 0x0u;
                            __mka_1185(5,__context__) = 0x0u;
                            __mka_1185(6,__context__) = 0x0u;
                            __mka_1185(7,__context__) = 0x0u;
                            return __mka_1185;
                        })())));
                        das_copy((__mks_1185.index),(0));
                        return __mks_1185;
                    })()))));
                    das_copy((__mks_1185.subexpr),(nullptr));
                    das_copy((__mks_1185.cset),((([&]() -> TDim<uint32_t,8> {
                        TDim<uint32_t,8> __mka_1185;
                        __mka_1185(0,__context__) = 0x0u;
                        __mka_1185(1,__context__) = 0x0u;
                        __mka_1185(2,__context__) = 0x0u;
                        __mka_1185(3,__context__) = 0x0u;
                        __mka_1185(4,__context__) = 0x0u;
                        __mka_1185(5,__context__) = 0x0u;
                        __mka_1185(6,__context__) = 0x0u;
                        __mka_1185(7,__context__) = 0x0u;
                        return __mka_1185;
                    })())));
                    das_copy((__mks_1185.index),(0));
                    return __mks_1185;
                })()))));
                das_copy((__mks_1185.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_1185;
                    das_zero(__mks_1185);
                    das_copy((__mks_1185.op),(DAS_COMMENT(enum) regex::ReOp::Group));
                    das_copy((__mks_1185.id),(20));
                    das_copy((__mks_1185.at),(range(31,47)));
                    das_copy((__mks_1185.text),(nullptr));
                    das_copy((__mks_1185.textLen),(0));
                    das_move((__mks_1185.all),((([&]() -> TArray<regex::ReNode *> {
                        TArray<regex::ReNode *> __mks_1185;
                        das_zero(__mks_1185);
                        return __mks_1185;
                    })())));
                    das_copy((__mks_1185.left),(nullptr));
                    das_copy((__mks_1185.right),(nullptr));
                    das_copy((__mks_1185.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_1185;
                        das_zero(__mks_1185);
                        das_copy((__mks_1185.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                        das_copy((__mks_1185.id),(21));
                        das_copy((__mks_1185.at),(range(32,46)));
                        das_copy((__mks_1185.text),(nullptr));
                        das_copy((__mks_1185.textLen),(0));
                        das_move((__mks_1185.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_1185;
                            das_zero(__mks_1185);
                            return __mks_1185;
                        })())));
                        das_copy((__mks_1185.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                            regex::ReNode __mks_1185;
                            das_zero(__mks_1185);
                            das_copy((__mks_1185.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                            das_copy((__mks_1185.id),(22));
                            das_copy((__mks_1185.at),(range(32,34)));
                            das_copy((__mks_1185.text),(((char *) "0x")));
                            das_copy((__mks_1185.textLen),(2));
                            das_move((__mks_1185.all),((([&]() -> TArray<regex::ReNode *> {
                                TArray<regex::ReNode *> __mks_1185;
                                das_zero(__mks_1185);
                                return __mks_1185;
                            })())));
                            das_copy((__mks_1185.left),(nullptr));
                            das_copy((__mks_1185.right),(nullptr));
                            das_copy((__mks_1185.subexpr),(nullptr));
                            das_copy((__mks_1185.cset),((([&]() -> TDim<uint32_t,8> {
                                TDim<uint32_t,8> __mka_1185;
                                __mka_1185(0,__context__) = 0x0u;
                                __mka_1185(1,__context__) = 0x0u;
                                __mka_1185(2,__context__) = 0x0u;
                                __mka_1185(3,__context__) = 0x0u;
                                __mka_1185(4,__context__) = 0x0u;
                                __mka_1185(5,__context__) = 0x0u;
                                __mka_1185(6,__context__) = 0x0u;
                                __mka_1185(7,__context__) = 0x0u;
                                return __mka_1185;
                            })())));
                            das_copy((__mks_1185.index),(0));
                            return __mks_1185;
                        })()))));
                        das_copy((__mks_1185.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                            regex::ReNode __mks_1185;
                            das_zero(__mks_1185);
                            das_copy((__mks_1185.op),(DAS_COMMENT(enum) regex::ReOp::Plus));
                            das_copy((__mks_1185.id),(23));
                            das_copy((__mks_1185.at),(range(34,46)));
                            das_copy((__mks_1185.text),(nullptr));
                            das_copy((__mks_1185.textLen),(0));
                            das_move((__mks_1185.all),((([&]() -> TArray<regex::ReNode *> {
                                TArray<regex::ReNode *> __mks_1185;
                                das_zero(__mks_1185);
                                return __mks_1185;
                            })())));
                            das_copy((__mks_1185.left),(nullptr));
                            das_copy((__mks_1185.right),(nullptr));
                            das_copy((__mks_1185.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                regex::ReNode __mks_1185;
                                das_zero(__mks_1185);
                                das_copy((__mks_1185.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                                das_copy((__mks_1185.id),(24));
                                das_copy((__mks_1185.at),(range(34,45)));
                                das_copy((__mks_1185.text),(nullptr));
                                das_copy((__mks_1185.textLen),(0));
                                das_move((__mks_1185.all),((([&]() -> TArray<regex::ReNode *> {
                                    TArray<regex::ReNode *> __mks_1185;
                                    das_zero(__mks_1185);
                                    return __mks_1185;
                                })())));
                                das_copy((__mks_1185.left),(nullptr));
                                das_copy((__mks_1185.right),(nullptr));
                                das_copy((__mks_1185.subexpr),(nullptr));
                                das_copy((__mks_1185.cset),((([&]() -> TDim<uint32_t,8> {
                                    TDim<uint32_t,8> __mka_1185;
                                    __mka_1185(0,__context__) = 0x0u;
                                    __mka_1185(1,__context__) = 0x3ff0000u;
                                    __mka_1185(2,__context__) = 0x7fffffeu;
                                    __mka_1185(3,__context__) = 0x7fffffeu;
                                    __mka_1185(4,__context__) = 0x0u;
                                    __mka_1185(5,__context__) = 0x0u;
                                    __mka_1185(6,__context__) = 0x0u;
                                    __mka_1185(7,__context__) = 0x0u;
                                    return __mka_1185;
                                })())));
                                das_copy((__mks_1185.index),(0));
                                return __mks_1185;
                            })()))));
                            das_copy((__mks_1185.cset),((([&]() -> TDim<uint32_t,8> {
                                TDim<uint32_t,8> __mka_1185;
                                __mka_1185(0,__context__) = 0x0u;
                                __mka_1185(1,__context__) = 0x3ff0000u;
                                __mka_1185(2,__context__) = 0x7fffffeu;
                                __mka_1185(3,__context__) = 0x7fffffeu;
                                __mka_1185(4,__context__) = 0x0u;
                                __mka_1185(5,__context__) = 0x0u;
                                __mka_1185(6,__context__) = 0x0u;
                                __mka_1185(7,__context__) = 0x0u;
                                return __mka_1185;
                            })())));
                            das_copy((__mks_1185.index),(0));
                            return __mks_1185;
                        })()))));
                        das_copy((__mks_1185.subexpr),(nullptr));
                        das_copy((__mks_1185.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_1185;
                            __mka_1185(0,__context__) = 0x0u;
                            __mka_1185(1,__context__) = 0x0u;
                            __mka_1185(2,__context__) = 0x0u;
                            __mka_1185(3,__context__) = 0x0u;
                            __mka_1185(4,__context__) = 0x0u;
                            __mka_1185(5,__context__) = 0x0u;
                            __mka_1185(6,__context__) = 0x0u;
                            __mka_1185(7,__context__) = 0x0u;
                            return __mka_1185;
                        })())));
                        das_copy((__mks_1185.index),(0));
                        return __mks_1185;
                    })()))));
                    das_copy((__mks_1185.cset),((([&]() -> TDim<uint32_t,8> {
                        TDim<uint32_t,8> __mka_1185;
                        __mka_1185(0,__context__) = 0x0u;
                        __mka_1185(1,__context__) = 0x0u;
                        __mka_1185(2,__context__) = 0x0u;
                        __mka_1185(3,__context__) = 0x0u;
                        __mka_1185(4,__context__) = 0x0u;
                        __mka_1185(5,__context__) = 0x0u;
                        __mka_1185(6,__context__) = 0x0u;
                        __mka_1185(7,__context__) = 0x0u;
                        return __mka_1185;
                    })())));
                    das_copy((__mks_1185.index),(2));
                    return __mks_1185;
                })()))));
                das_copy((__mks_1185.subexpr),(nullptr));
                das_copy((__mks_1185.cset),((([&]() -> TDim<uint32_t,8> {
                    TDim<uint32_t,8> __mka_1185;
                    __mka_1185(0,__context__) = 0x0u;
                    __mka_1185(1,__context__) = 0x0u;
                    __mka_1185(2,__context__) = 0x0u;
                    __mka_1185(3,__context__) = 0x0u;
                    __mka_1185(4,__context__) = 0x0u;
                    __mka_1185(5,__context__) = 0x0u;
                    __mka_1185(6,__context__) = 0x0u;
                    __mka_1185(7,__context__) = 0x0u;
                    return __mka_1185;
                })())));
                das_copy((__mks_1185.index),(0));
                return __mks_1185;
            })()))));
            das_copy((__mks_1185.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_1185;
                das_zero(__mks_1185);
                das_copy((__mks_1185.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                das_copy((__mks_1185.id),(25));
                das_copy((__mks_1185.at),(range(47,49)));
                das_copy((__mks_1185.text),(((char *) ")")));
                das_copy((__mks_1185.textLen),(1));
                das_move((__mks_1185.all),((([&]() -> TArray<regex::ReNode *> {
                    TArray<regex::ReNode *> __mks_1185;
                    das_zero(__mks_1185);
                    return __mks_1185;
                })())));
                das_copy((__mks_1185.left),(nullptr));
                das_copy((__mks_1185.right),(nullptr));
                das_copy((__mks_1185.subexpr),(nullptr));
                das_copy((__mks_1185.cset),((([&]() -> TDim<uint32_t,8> {
                    TDim<uint32_t,8> __mka_1185;
                    __mka_1185(0,__context__) = 0x0u;
                    __mka_1185(1,__context__) = 0x0u;
                    __mka_1185(2,__context__) = 0x0u;
                    __mka_1185(3,__context__) = 0x0u;
                    __mka_1185(4,__context__) = 0x0u;
                    __mka_1185(5,__context__) = 0x0u;
                    __mka_1185(6,__context__) = 0x0u;
                    __mka_1185(7,__context__) = 0x0u;
                    return __mka_1185;
                })())));
                das_copy((__mks_1185.index),(0));
                return __mks_1185;
            })()))));
            das_copy((__mks_1185.subexpr),(nullptr));
            das_copy((__mks_1185.cset),((([&]() -> TDim<uint32_t,8> {
                TDim<uint32_t,8> __mka_1185;
                __mka_1185(0,__context__) = 0x0u;
                __mka_1185(1,__context__) = 0x0u;
                __mka_1185(2,__context__) = 0x0u;
                __mka_1185(3,__context__) = 0x0u;
                __mka_1185(4,__context__) = 0x0u;
                __mka_1185(5,__context__) = 0x0u;
                __mka_1185(6,__context__) = 0x0u;
                __mka_1185(7,__context__) = 0x0u;
                return __mka_1185;
            })())));
            das_copy((__mks_1185.index),(0));
            return __mks_1185;
        })()))));
        das_move((_temp_make_local_1185_26_1680.groups),(_FuncbuiltinTickto_array_moveTick3185538323411982277_71fbdf75ac58194(__context__,das_arg<TDim<AutoTuple<range,char *>,3>>::pass((([&]() -> TDim<AutoTuple<range,char *>,3>& {
            _temp_make_local_1185_26_2176(0,__context__) = (([&]() -> AutoTuple<range,char *> {
                AutoTuple<range,char *> __mkt_1185;
                das_get_auto_tuple_field<range,0,range,char *>::get(__mkt_1185) = range(0,0);
                das_get_auto_tuple_field<char *,1,range,char *>::get(__mkt_1185) = nullptr;
                return __mkt_1185;
            })());
            _temp_make_local_1185_26_2176(1,__context__) = (([&]() -> AutoTuple<range,char *> {
                AutoTuple<range,char *> __mkt_1185;
                das_get_auto_tuple_field<range,0,range,char *>::get(__mkt_1185) = range(0,0);
                das_get_auto_tuple_field<char *,1,range,char *>::get(__mkt_1185) = ((char *) "1");
                return __mkt_1185;
            })());
            _temp_make_local_1185_26_2176(2,__context__) = (([&]() -> AutoTuple<range,char *> {
                AutoTuple<range,char *> __mkt_1185;
                das_get_auto_tuple_field<range,0,range,char *>::get(__mkt_1185) = range(0,0);
                das_get_auto_tuple_field<char *,1,range,char *>::get(__mkt_1185) = ((char *) "2");
                return __mkt_1185;
            })());
            return _temp_make_local_1185_26_2176;
        })())))));
        das_copy((_temp_make_local_1185_26_1680.earlyOut),((([&]() -> TDim<uint32_t,8> {
            TDim<uint32_t,8> __mka_1185;
            __mka_1185(0,__context__) = 0x0u;
            __mka_1185(1,__context__) = 0x8u;
            __mka_1185(2,__context__) = 0x0u;
            __mka_1185(3,__context__) = 0x0u;
            __mka_1185(4,__context__) = 0x0u;
            __mka_1185(5,__context__) = 0x0u;
            __mka_1185(6,__context__) = 0x0u;
            __mka_1185(7,__context__) = 0x0u;
            return __mka_1185;
        })())));
        das_copy((_temp_make_local_1185_26_1680.canEarlyOut),(true));
        return _temp_make_local_1185_26_1680;
    })()))));
    regex::Regex __reg_def_UINT32_rename_at_1186_351; das_zero(__reg_def_UINT32_rename_at_1186_351); das_move(__reg_def_UINT32_rename_at_1186_351, das_invoke_function<regex::Regex>::invoke_cmres<regex::Regex &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::regex_compile S<regex::Regex>*/ 0x235aa29958cfc4c4)),das_arg<regex::Regex>::pass((([&]() -> regex::Regex& {
        das_zero(_temp_make_local_1186_26_2304);
        das_copy((_temp_make_local_1186_26_2304.root),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
            regex::ReNode __mks_1186;
            das_zero(__mks_1186);
            das_copy((__mks_1186.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
            das_copy((__mks_1186.id),(0));
            das_copy((__mks_1186.at),(range(0,49)));
            das_copy((__mks_1186.text),(nullptr));
            das_copy((__mks_1186.textLen),(0));
            das_move((__mks_1186.all),((([&]() -> TArray<regex::ReNode *> {
                TArray<regex::ReNode *> __mks_1186;
                das_zero(__mks_1186);
                return __mks_1186;
            })())));
            das_copy((__mks_1186.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_1186;
                das_zero(__mks_1186);
                das_copy((__mks_1186.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                das_copy((__mks_1186.id),(1));
                das_copy((__mks_1186.at),(range(0,47)));
                das_copy((__mks_1186.text),(nullptr));
                das_copy((__mks_1186.textLen),(0));
                das_move((__mks_1186.all),((([&]() -> TArray<regex::ReNode *> {
                    TArray<regex::ReNode *> __mks_1186;
                    das_zero(__mks_1186);
                    return __mks_1186;
                })())));
                das_copy((__mks_1186.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_1186;
                    das_zero(__mks_1186);
                    das_copy((__mks_1186.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                    das_copy((__mks_1186.id),(2));
                    das_copy((__mks_1186.at),(range(0,31)));
                    das_copy((__mks_1186.text),(nullptr));
                    das_copy((__mks_1186.textLen),(0));
                    das_move((__mks_1186.all),((([&]() -> TArray<regex::ReNode *> {
                        TArray<regex::ReNode *> __mks_1186;
                        das_zero(__mks_1186);
                        return __mks_1186;
                    })())));
                    das_copy((__mks_1186.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_1186;
                        das_zero(__mks_1186);
                        das_copy((__mks_1186.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                        das_copy((__mks_1186.id),(3));
                        das_copy((__mks_1186.at),(range(0,29)));
                        das_copy((__mks_1186.text),(nullptr));
                        das_copy((__mks_1186.textLen),(0));
                        das_move((__mks_1186.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_1186;
                            das_zero(__mks_1186);
                            return __mks_1186;
                        })())));
                        das_copy((__mks_1186.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                            regex::ReNode __mks_1186;
                            das_zero(__mks_1186);
                            das_copy((__mks_1186.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                            das_copy((__mks_1186.id),(4));
                            das_copy((__mks_1186.at),(range(0,26)));
                            das_copy((__mks_1186.text),(nullptr));
                            das_copy((__mks_1186.textLen),(0));
                            das_move((__mks_1186.all),((([&]() -> TArray<regex::ReNode *> {
                                TArray<regex::ReNode *> __mks_1186;
                                das_zero(__mks_1186);
                                return __mks_1186;
                            })())));
                            das_copy((__mks_1186.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                regex::ReNode __mks_1186;
                                das_zero(__mks_1186);
                                das_copy((__mks_1186.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                                das_copy((__mks_1186.id),(5));
                                das_copy((__mks_1186.at),(range(0,18)));
                                das_copy((__mks_1186.text),(nullptr));
                                das_copy((__mks_1186.textLen),(0));
                                das_move((__mks_1186.all),((([&]() -> TArray<regex::ReNode *> {
                                    TArray<regex::ReNode *> __mks_1186;
                                    das_zero(__mks_1186);
                                    return __mks_1186;
                                })())));
                                das_copy((__mks_1186.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                    regex::ReNode __mks_1186;
                                    das_zero(__mks_1186);
                                    das_copy((__mks_1186.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                                    das_copy((__mks_1186.id),(6));
                                    das_copy((__mks_1186.at),(range(0,15)));
                                    das_copy((__mks_1186.text),(nullptr));
                                    das_copy((__mks_1186.textLen),(0));
                                    das_move((__mks_1186.all),((([&]() -> TArray<regex::ReNode *> {
                                        TArray<regex::ReNode *> __mks_1186;
                                        das_zero(__mks_1186);
                                        return __mks_1186;
                                    })())));
                                    das_copy((__mks_1186.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                        regex::ReNode __mks_1186;
                                        das_zero(__mks_1186);
                                        das_copy((__mks_1186.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                                        das_copy((__mks_1186.id),(7));
                                        das_copy((__mks_1186.at),(range(0,10)));
                                        das_copy((__mks_1186.text),(nullptr));
                                        das_copy((__mks_1186.textLen),(0));
                                        das_move((__mks_1186.all),((([&]() -> TArray<regex::ReNode *> {
                                            TArray<regex::ReNode *> __mks_1186;
                                            das_zero(__mks_1186);
                                            return __mks_1186;
                                        })())));
                                        das_copy((__mks_1186.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                            regex::ReNode __mks_1186;
                                            das_zero(__mks_1186);
                                            das_copy((__mks_1186.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                                            das_copy((__mks_1186.id),(8));
                                            das_copy((__mks_1186.at),(range(0,7)));
                                            das_copy((__mks_1186.text),(((char *) "#define")));
                                            das_copy((__mks_1186.textLen),(7));
                                            das_move((__mks_1186.all),((([&]() -> TArray<regex::ReNode *> {
                                                TArray<regex::ReNode *> __mks_1186;
                                                das_zero(__mks_1186);
                                                return __mks_1186;
                                            })())));
                                            das_copy((__mks_1186.left),(nullptr));
                                            das_copy((__mks_1186.right),(nullptr));
                                            das_copy((__mks_1186.subexpr),(nullptr));
                                            das_copy((__mks_1186.cset),((([&]() -> TDim<uint32_t,8> {
                                                TDim<uint32_t,8> __mka_1186;
                                                __mka_1186(0,__context__) = 0x0u;
                                                __mka_1186(1,__context__) = 0x0u;
                                                __mka_1186(2,__context__) = 0x0u;
                                                __mka_1186(3,__context__) = 0x0u;
                                                __mka_1186(4,__context__) = 0x0u;
                                                __mka_1186(5,__context__) = 0x0u;
                                                __mka_1186(6,__context__) = 0x0u;
                                                __mka_1186(7,__context__) = 0x0u;
                                                return __mka_1186;
                                            })())));
                                            das_copy((__mks_1186.index),(0));
                                            return __mks_1186;
                                        })()))));
                                        das_copy((__mks_1186.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                            regex::ReNode __mks_1186;
                                            das_zero(__mks_1186);
                                            das_copy((__mks_1186.op),(DAS_COMMENT(enum) regex::ReOp::Plus));
                                            das_copy((__mks_1186.id),(9));
                                            das_copy((__mks_1186.at),(range(7,10)));
                                            das_copy((__mks_1186.text),(nullptr));
                                            das_copy((__mks_1186.textLen),(0));
                                            das_move((__mks_1186.all),((([&]() -> TArray<regex::ReNode *> {
                                                TArray<regex::ReNode *> __mks_1186;
                                                das_zero(__mks_1186);
                                                return __mks_1186;
                                            })())));
                                            das_copy((__mks_1186.left),(nullptr));
                                            das_copy((__mks_1186.right),(nullptr));
                                            das_copy((__mks_1186.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                                regex::ReNode __mks_1186;
                                                das_zero(__mks_1186);
                                                das_copy((__mks_1186.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                                                das_copy((__mks_1186.id),(10));
                                                das_copy((__mks_1186.at),(range(7,9)));
                                                das_copy((__mks_1186.text),(nullptr));
                                                das_copy((__mks_1186.textLen),(0));
                                                das_move((__mks_1186.all),((([&]() -> TArray<regex::ReNode *> {
                                                    TArray<regex::ReNode *> __mks_1186;
                                                    das_zero(__mks_1186);
                                                    return __mks_1186;
                                                })())));
                                                das_copy((__mks_1186.left),(nullptr));
                                                das_copy((__mks_1186.right),(nullptr));
                                                das_copy((__mks_1186.subexpr),(nullptr));
                                                das_copy((__mks_1186.cset),((([&]() -> TDim<uint32_t,8> {
                                                    TDim<uint32_t,8> __mka_1186;
                                                    __mka_1186(0,__context__) = 0x200u;
                                                    __mka_1186(1,__context__) = 0x1u;
                                                    __mka_1186(2,__context__) = 0x0u;
                                                    __mka_1186(3,__context__) = 0x0u;
                                                    __mka_1186(4,__context__) = 0x0u;
                                                    __mka_1186(5,__context__) = 0x0u;
                                                    __mka_1186(6,__context__) = 0x0u;
                                                    __mka_1186(7,__context__) = 0x0u;
                                                    return __mka_1186;
                                                })())));
                                                das_copy((__mks_1186.index),(0));
                                                return __mks_1186;
                                            })()))));
                                            das_copy((__mks_1186.cset),((([&]() -> TDim<uint32_t,8> {
                                                TDim<uint32_t,8> __mka_1186;
                                                __mka_1186(0,__context__) = 0x200u;
                                                __mka_1186(1,__context__) = 0x1u;
                                                __mka_1186(2,__context__) = 0x0u;
                                                __mka_1186(3,__context__) = 0x0u;
                                                __mka_1186(4,__context__) = 0x0u;
                                                __mka_1186(5,__context__) = 0x0u;
                                                __mka_1186(6,__context__) = 0x0u;
                                                __mka_1186(7,__context__) = 0x0u;
                                                return __mka_1186;
                                            })())));
                                            das_copy((__mks_1186.index),(0));
                                            return __mks_1186;
                                        })()))));
                                        das_copy((__mks_1186.subexpr),(nullptr));
                                        das_copy((__mks_1186.cset),((([&]() -> TDim<uint32_t,8> {
                                            TDim<uint32_t,8> __mka_1186;
                                            __mka_1186(0,__context__) = 0x0u;
                                            __mka_1186(1,__context__) = 0x0u;
                                            __mka_1186(2,__context__) = 0x0u;
                                            __mka_1186(3,__context__) = 0x0u;
                                            __mka_1186(4,__context__) = 0x0u;
                                            __mka_1186(5,__context__) = 0x0u;
                                            __mka_1186(6,__context__) = 0x0u;
                                            __mka_1186(7,__context__) = 0x0u;
                                            return __mka_1186;
                                        })())));
                                        das_copy((__mks_1186.index),(0));
                                        return __mks_1186;
                                    })()))));
                                    das_copy((__mks_1186.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                        regex::ReNode __mks_1186;
                                        das_zero(__mks_1186);
                                        das_copy((__mks_1186.op),(DAS_COMMENT(enum) regex::ReOp::Group));
                                        das_copy((__mks_1186.id),(11));
                                        das_copy((__mks_1186.at),(range(10,15)));
                                        das_copy((__mks_1186.text),(nullptr));
                                        das_copy((__mks_1186.textLen),(0));
                                        das_move((__mks_1186.all),((([&]() -> TArray<regex::ReNode *> {
                                            TArray<regex::ReNode *> __mks_1186;
                                            das_zero(__mks_1186);
                                            return __mks_1186;
                                        })())));
                                        das_copy((__mks_1186.left),(nullptr));
                                        das_copy((__mks_1186.right),(nullptr));
                                        das_copy((__mks_1186.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                            regex::ReNode __mks_1186;
                                            das_zero(__mks_1186);
                                            das_copy((__mks_1186.op),(DAS_COMMENT(enum) regex::ReOp::Plus));
                                            das_copy((__mks_1186.id),(12));
                                            das_copy((__mks_1186.at),(range(11,14)));
                                            das_copy((__mks_1186.text),(nullptr));
                                            das_copy((__mks_1186.textLen),(0));
                                            das_move((__mks_1186.all),((([&]() -> TArray<regex::ReNode *> {
                                                TArray<regex::ReNode *> __mks_1186;
                                                das_zero(__mks_1186);
                                                return __mks_1186;
                                            })())));
                                            das_copy((__mks_1186.left),(nullptr));
                                            das_copy((__mks_1186.right),(nullptr));
                                            das_copy((__mks_1186.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                                regex::ReNode __mks_1186;
                                                das_zero(__mks_1186);
                                                das_copy((__mks_1186.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                                                das_copy((__mks_1186.id),(13));
                                                das_copy((__mks_1186.at),(range(11,13)));
                                                das_copy((__mks_1186.text),(nullptr));
                                                das_copy((__mks_1186.textLen),(0));
                                                das_move((__mks_1186.all),((([&]() -> TArray<regex::ReNode *> {
                                                    TArray<regex::ReNode *> __mks_1186;
                                                    das_zero(__mks_1186);
                                                    return __mks_1186;
                                                })())));
                                                das_copy((__mks_1186.left),(nullptr));
                                                das_copy((__mks_1186.right),(nullptr));
                                                das_copy((__mks_1186.subexpr),(nullptr));
                                                das_copy((__mks_1186.cset),((([&]() -> TDim<uint32_t,8> {
                                                    TDim<uint32_t,8> __mka_1186;
                                                    __mka_1186(0,__context__) = 0x0u;
                                                    __mka_1186(1,__context__) = 0x3ff0000u;
                                                    __mka_1186(2,__context__) = 0x87fffffeu;
                                                    __mka_1186(3,__context__) = 0x7fffffeu;
                                                    __mka_1186(4,__context__) = 0x0u;
                                                    __mka_1186(5,__context__) = 0x0u;
                                                    __mka_1186(6,__context__) = 0x0u;
                                                    __mka_1186(7,__context__) = 0x0u;
                                                    return __mka_1186;
                                                })())));
                                                das_copy((__mks_1186.index),(0));
                                                return __mks_1186;
                                            })()))));
                                            das_copy((__mks_1186.cset),((([&]() -> TDim<uint32_t,8> {
                                                TDim<uint32_t,8> __mka_1186;
                                                __mka_1186(0,__context__) = 0x0u;
                                                __mka_1186(1,__context__) = 0x3ff0000u;
                                                __mka_1186(2,__context__) = 0x87fffffeu;
                                                __mka_1186(3,__context__) = 0x7fffffeu;
                                                __mka_1186(4,__context__) = 0x0u;
                                                __mka_1186(5,__context__) = 0x0u;
                                                __mka_1186(6,__context__) = 0x0u;
                                                __mka_1186(7,__context__) = 0x0u;
                                                return __mka_1186;
                                            })())));
                                            das_copy((__mks_1186.index),(0));
                                            return __mks_1186;
                                        })()))));
                                        das_copy((__mks_1186.cset),((([&]() -> TDim<uint32_t,8> {
                                            TDim<uint32_t,8> __mka_1186;
                                            __mka_1186(0,__context__) = 0x0u;
                                            __mka_1186(1,__context__) = 0x0u;
                                            __mka_1186(2,__context__) = 0x0u;
                                            __mka_1186(3,__context__) = 0x0u;
                                            __mka_1186(4,__context__) = 0x0u;
                                            __mka_1186(5,__context__) = 0x0u;
                                            __mka_1186(6,__context__) = 0x0u;
                                            __mka_1186(7,__context__) = 0x0u;
                                            return __mka_1186;
                                        })())));
                                        das_copy((__mks_1186.index),(1));
                                        return __mks_1186;
                                    })()))));
                                    das_copy((__mks_1186.subexpr),(nullptr));
                                    das_copy((__mks_1186.cset),((([&]() -> TDim<uint32_t,8> {
                                        TDim<uint32_t,8> __mka_1186;
                                        __mka_1186(0,__context__) = 0x0u;
                                        __mka_1186(1,__context__) = 0x0u;
                                        __mka_1186(2,__context__) = 0x0u;
                                        __mka_1186(3,__context__) = 0x0u;
                                        __mka_1186(4,__context__) = 0x0u;
                                        __mka_1186(5,__context__) = 0x0u;
                                        __mka_1186(6,__context__) = 0x0u;
                                        __mka_1186(7,__context__) = 0x0u;
                                        return __mka_1186;
                                    })())));
                                    das_copy((__mks_1186.index),(0));
                                    return __mks_1186;
                                })()))));
                                das_copy((__mks_1186.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                    regex::ReNode __mks_1186;
                                    das_zero(__mks_1186);
                                    das_copy((__mks_1186.op),(DAS_COMMENT(enum) regex::ReOp::Plus));
                                    das_copy((__mks_1186.id),(14));
                                    das_copy((__mks_1186.at),(range(15,18)));
                                    das_copy((__mks_1186.text),(nullptr));
                                    das_copy((__mks_1186.textLen),(0));
                                    das_move((__mks_1186.all),((([&]() -> TArray<regex::ReNode *> {
                                        TArray<regex::ReNode *> __mks_1186;
                                        das_zero(__mks_1186);
                                        return __mks_1186;
                                    })())));
                                    das_copy((__mks_1186.left),(nullptr));
                                    das_copy((__mks_1186.right),(nullptr));
                                    das_copy((__mks_1186.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                        regex::ReNode __mks_1186;
                                        das_zero(__mks_1186);
                                        das_copy((__mks_1186.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                                        das_copy((__mks_1186.id),(15));
                                        das_copy((__mks_1186.at),(range(15,17)));
                                        das_copy((__mks_1186.text),(nullptr));
                                        das_copy((__mks_1186.textLen),(0));
                                        das_move((__mks_1186.all),((([&]() -> TArray<regex::ReNode *> {
                                            TArray<regex::ReNode *> __mks_1186;
                                            das_zero(__mks_1186);
                                            return __mks_1186;
                                        })())));
                                        das_copy((__mks_1186.left),(nullptr));
                                        das_copy((__mks_1186.right),(nullptr));
                                        das_copy((__mks_1186.subexpr),(nullptr));
                                        das_copy((__mks_1186.cset),((([&]() -> TDim<uint32_t,8> {
                                            TDim<uint32_t,8> __mka_1186;
                                            __mka_1186(0,__context__) = 0x200u;
                                            __mka_1186(1,__context__) = 0x1u;
                                            __mka_1186(2,__context__) = 0x0u;
                                            __mka_1186(3,__context__) = 0x0u;
                                            __mka_1186(4,__context__) = 0x0u;
                                            __mka_1186(5,__context__) = 0x0u;
                                            __mka_1186(6,__context__) = 0x0u;
                                            __mka_1186(7,__context__) = 0x0u;
                                            return __mka_1186;
                                        })())));
                                        das_copy((__mks_1186.index),(0));
                                        return __mks_1186;
                                    })()))));
                                    das_copy((__mks_1186.cset),((([&]() -> TDim<uint32_t,8> {
                                        TDim<uint32_t,8> __mka_1186;
                                        __mka_1186(0,__context__) = 0x200u;
                                        __mka_1186(1,__context__) = 0x1u;
                                        __mka_1186(2,__context__) = 0x0u;
                                        __mka_1186(3,__context__) = 0x0u;
                                        __mka_1186(4,__context__) = 0x0u;
                                        __mka_1186(5,__context__) = 0x0u;
                                        __mka_1186(6,__context__) = 0x0u;
                                        __mka_1186(7,__context__) = 0x0u;
                                        return __mka_1186;
                                    })())));
                                    das_copy((__mks_1186.index),(0));
                                    return __mks_1186;
                                })()))));
                                das_copy((__mks_1186.subexpr),(nullptr));
                                das_copy((__mks_1186.cset),((([&]() -> TDim<uint32_t,8> {
                                    TDim<uint32_t,8> __mka_1186;
                                    __mka_1186(0,__context__) = 0x0u;
                                    __mka_1186(1,__context__) = 0x0u;
                                    __mka_1186(2,__context__) = 0x0u;
                                    __mka_1186(3,__context__) = 0x0u;
                                    __mka_1186(4,__context__) = 0x0u;
                                    __mka_1186(5,__context__) = 0x0u;
                                    __mka_1186(6,__context__) = 0x0u;
                                    __mka_1186(7,__context__) = 0x0u;
                                    return __mka_1186;
                                })())));
                                das_copy((__mks_1186.index),(0));
                                return __mks_1186;
                            })()))));
                            das_copy((__mks_1186.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                regex::ReNode __mks_1186;
                                das_zero(__mks_1186);
                                das_copy((__mks_1186.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                                das_copy((__mks_1186.id),(16));
                                das_copy((__mks_1186.at),(range(18,26)));
                                das_copy((__mks_1186.text),(((char *) "UINT32_C")));
                                das_copy((__mks_1186.textLen),(8));
                                das_move((__mks_1186.all),((([&]() -> TArray<regex::ReNode *> {
                                    TArray<regex::ReNode *> __mks_1186;
                                    das_zero(__mks_1186);
                                    return __mks_1186;
                                })())));
                                das_copy((__mks_1186.left),(nullptr));
                                das_copy((__mks_1186.right),(nullptr));
                                das_copy((__mks_1186.subexpr),(nullptr));
                                das_copy((__mks_1186.cset),((([&]() -> TDim<uint32_t,8> {
                                    TDim<uint32_t,8> __mka_1186;
                                    __mka_1186(0,__context__) = 0x0u;
                                    __mka_1186(1,__context__) = 0x0u;
                                    __mka_1186(2,__context__) = 0x0u;
                                    __mka_1186(3,__context__) = 0x0u;
                                    __mka_1186(4,__context__) = 0x0u;
                                    __mka_1186(5,__context__) = 0x0u;
                                    __mka_1186(6,__context__) = 0x0u;
                                    __mka_1186(7,__context__) = 0x0u;
                                    return __mka_1186;
                                })())));
                                das_copy((__mks_1186.index),(0));
                                return __mks_1186;
                            })()))));
                            das_copy((__mks_1186.subexpr),(nullptr));
                            das_copy((__mks_1186.cset),((([&]() -> TDim<uint32_t,8> {
                                TDim<uint32_t,8> __mka_1186;
                                __mka_1186(0,__context__) = 0x0u;
                                __mka_1186(1,__context__) = 0x0u;
                                __mka_1186(2,__context__) = 0x0u;
                                __mka_1186(3,__context__) = 0x0u;
                                __mka_1186(4,__context__) = 0x0u;
                                __mka_1186(5,__context__) = 0x0u;
                                __mka_1186(6,__context__) = 0x0u;
                                __mka_1186(7,__context__) = 0x0u;
                                return __mka_1186;
                            })())));
                            das_copy((__mks_1186.index),(0));
                            return __mks_1186;
                        })()))));
                        das_copy((__mks_1186.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                            regex::ReNode __mks_1186;
                            das_zero(__mks_1186);
                            das_copy((__mks_1186.op),(DAS_COMMENT(enum) regex::ReOp::Star));
                            das_copy((__mks_1186.id),(17));
                            das_copy((__mks_1186.at),(range(26,29)));
                            das_copy((__mks_1186.text),(nullptr));
                            das_copy((__mks_1186.textLen),(0));
                            das_move((__mks_1186.all),((([&]() -> TArray<regex::ReNode *> {
                                TArray<regex::ReNode *> __mks_1186;
                                das_zero(__mks_1186);
                                return __mks_1186;
                            })())));
                            das_copy((__mks_1186.left),(nullptr));
                            das_copy((__mks_1186.right),(nullptr));
                            das_copy((__mks_1186.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                regex::ReNode __mks_1186;
                                das_zero(__mks_1186);
                                das_copy((__mks_1186.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                                das_copy((__mks_1186.id),(18));
                                das_copy((__mks_1186.at),(range(26,28)));
                                das_copy((__mks_1186.text),(nullptr));
                                das_copy((__mks_1186.textLen),(0));
                                das_move((__mks_1186.all),((([&]() -> TArray<regex::ReNode *> {
                                    TArray<regex::ReNode *> __mks_1186;
                                    das_zero(__mks_1186);
                                    return __mks_1186;
                                })())));
                                das_copy((__mks_1186.left),(nullptr));
                                das_copy((__mks_1186.right),(nullptr));
                                das_copy((__mks_1186.subexpr),(nullptr));
                                das_copy((__mks_1186.cset),((([&]() -> TDim<uint32_t,8> {
                                    TDim<uint32_t,8> __mka_1186;
                                    __mka_1186(0,__context__) = 0x200u;
                                    __mka_1186(1,__context__) = 0x1u;
                                    __mka_1186(2,__context__) = 0x0u;
                                    __mka_1186(3,__context__) = 0x0u;
                                    __mka_1186(4,__context__) = 0x0u;
                                    __mka_1186(5,__context__) = 0x0u;
                                    __mka_1186(6,__context__) = 0x0u;
                                    __mka_1186(7,__context__) = 0x0u;
                                    return __mka_1186;
                                })())));
                                das_copy((__mks_1186.index),(0));
                                return __mks_1186;
                            })()))));
                            das_copy((__mks_1186.cset),((([&]() -> TDim<uint32_t,8> {
                                TDim<uint32_t,8> __mka_1186;
                                __mka_1186(0,__context__) = 0x0u;
                                __mka_1186(1,__context__) = 0x0u;
                                __mka_1186(2,__context__) = 0x0u;
                                __mka_1186(3,__context__) = 0x0u;
                                __mka_1186(4,__context__) = 0x0u;
                                __mka_1186(5,__context__) = 0x0u;
                                __mka_1186(6,__context__) = 0x0u;
                                __mka_1186(7,__context__) = 0x0u;
                                return __mka_1186;
                            })())));
                            das_copy((__mks_1186.index),(0));
                            return __mks_1186;
                        })()))));
                        das_copy((__mks_1186.subexpr),(nullptr));
                        das_copy((__mks_1186.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_1186;
                            __mka_1186(0,__context__) = 0x0u;
                            __mka_1186(1,__context__) = 0x0u;
                            __mka_1186(2,__context__) = 0x0u;
                            __mka_1186(3,__context__) = 0x0u;
                            __mka_1186(4,__context__) = 0x0u;
                            __mka_1186(5,__context__) = 0x0u;
                            __mka_1186(6,__context__) = 0x0u;
                            __mka_1186(7,__context__) = 0x0u;
                            return __mka_1186;
                        })())));
                        das_copy((__mks_1186.index),(0));
                        return __mks_1186;
                    })()))));
                    das_copy((__mks_1186.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_1186;
                        das_zero(__mks_1186);
                        das_copy((__mks_1186.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                        das_copy((__mks_1186.id),(19));
                        das_copy((__mks_1186.at),(range(29,31)));
                        das_copy((__mks_1186.text),(((char *) "(")));
                        das_copy((__mks_1186.textLen),(1));
                        das_move((__mks_1186.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_1186;
                            das_zero(__mks_1186);
                            return __mks_1186;
                        })())));
                        das_copy((__mks_1186.left),(nullptr));
                        das_copy((__mks_1186.right),(nullptr));
                        das_copy((__mks_1186.subexpr),(nullptr));
                        das_copy((__mks_1186.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_1186;
                            __mka_1186(0,__context__) = 0x0u;
                            __mka_1186(1,__context__) = 0x0u;
                            __mka_1186(2,__context__) = 0x0u;
                            __mka_1186(3,__context__) = 0x0u;
                            __mka_1186(4,__context__) = 0x0u;
                            __mka_1186(5,__context__) = 0x0u;
                            __mka_1186(6,__context__) = 0x0u;
                            __mka_1186(7,__context__) = 0x0u;
                            return __mka_1186;
                        })())));
                        das_copy((__mks_1186.index),(0));
                        return __mks_1186;
                    })()))));
                    das_copy((__mks_1186.subexpr),(nullptr));
                    das_copy((__mks_1186.cset),((([&]() -> TDim<uint32_t,8> {
                        TDim<uint32_t,8> __mka_1186;
                        __mka_1186(0,__context__) = 0x0u;
                        __mka_1186(1,__context__) = 0x0u;
                        __mka_1186(2,__context__) = 0x0u;
                        __mka_1186(3,__context__) = 0x0u;
                        __mka_1186(4,__context__) = 0x0u;
                        __mka_1186(5,__context__) = 0x0u;
                        __mka_1186(6,__context__) = 0x0u;
                        __mka_1186(7,__context__) = 0x0u;
                        return __mka_1186;
                    })())));
                    das_copy((__mks_1186.index),(0));
                    return __mks_1186;
                })()))));
                das_copy((__mks_1186.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_1186;
                    das_zero(__mks_1186);
                    das_copy((__mks_1186.op),(DAS_COMMENT(enum) regex::ReOp::Group));
                    das_copy((__mks_1186.id),(20));
                    das_copy((__mks_1186.at),(range(31,47)));
                    das_copy((__mks_1186.text),(nullptr));
                    das_copy((__mks_1186.textLen),(0));
                    das_move((__mks_1186.all),((([&]() -> TArray<regex::ReNode *> {
                        TArray<regex::ReNode *> __mks_1186;
                        das_zero(__mks_1186);
                        return __mks_1186;
                    })())));
                    das_copy((__mks_1186.left),(nullptr));
                    das_copy((__mks_1186.right),(nullptr));
                    das_copy((__mks_1186.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_1186;
                        das_zero(__mks_1186);
                        das_copy((__mks_1186.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                        das_copy((__mks_1186.id),(21));
                        das_copy((__mks_1186.at),(range(32,46)));
                        das_copy((__mks_1186.text),(nullptr));
                        das_copy((__mks_1186.textLen),(0));
                        das_move((__mks_1186.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_1186;
                            das_zero(__mks_1186);
                            return __mks_1186;
                        })())));
                        das_copy((__mks_1186.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                            regex::ReNode __mks_1186;
                            das_zero(__mks_1186);
                            das_copy((__mks_1186.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                            das_copy((__mks_1186.id),(22));
                            das_copy((__mks_1186.at),(range(32,34)));
                            das_copy((__mks_1186.text),(((char *) "0x")));
                            das_copy((__mks_1186.textLen),(2));
                            das_move((__mks_1186.all),((([&]() -> TArray<regex::ReNode *> {
                                TArray<regex::ReNode *> __mks_1186;
                                das_zero(__mks_1186);
                                return __mks_1186;
                            })())));
                            das_copy((__mks_1186.left),(nullptr));
                            das_copy((__mks_1186.right),(nullptr));
                            das_copy((__mks_1186.subexpr),(nullptr));
                            das_copy((__mks_1186.cset),((([&]() -> TDim<uint32_t,8> {
                                TDim<uint32_t,8> __mka_1186;
                                __mka_1186(0,__context__) = 0x0u;
                                __mka_1186(1,__context__) = 0x0u;
                                __mka_1186(2,__context__) = 0x0u;
                                __mka_1186(3,__context__) = 0x0u;
                                __mka_1186(4,__context__) = 0x0u;
                                __mka_1186(5,__context__) = 0x0u;
                                __mka_1186(6,__context__) = 0x0u;
                                __mka_1186(7,__context__) = 0x0u;
                                return __mka_1186;
                            })())));
                            das_copy((__mks_1186.index),(0));
                            return __mks_1186;
                        })()))));
                        das_copy((__mks_1186.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                            regex::ReNode __mks_1186;
                            das_zero(__mks_1186);
                            das_copy((__mks_1186.op),(DAS_COMMENT(enum) regex::ReOp::Plus));
                            das_copy((__mks_1186.id),(23));
                            das_copy((__mks_1186.at),(range(34,46)));
                            das_copy((__mks_1186.text),(nullptr));
                            das_copy((__mks_1186.textLen),(0));
                            das_move((__mks_1186.all),((([&]() -> TArray<regex::ReNode *> {
                                TArray<regex::ReNode *> __mks_1186;
                                das_zero(__mks_1186);
                                return __mks_1186;
                            })())));
                            das_copy((__mks_1186.left),(nullptr));
                            das_copy((__mks_1186.right),(nullptr));
                            das_copy((__mks_1186.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                regex::ReNode __mks_1186;
                                das_zero(__mks_1186);
                                das_copy((__mks_1186.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                                das_copy((__mks_1186.id),(24));
                                das_copy((__mks_1186.at),(range(34,45)));
                                das_copy((__mks_1186.text),(nullptr));
                                das_copy((__mks_1186.textLen),(0));
                                das_move((__mks_1186.all),((([&]() -> TArray<regex::ReNode *> {
                                    TArray<regex::ReNode *> __mks_1186;
                                    das_zero(__mks_1186);
                                    return __mks_1186;
                                })())));
                                das_copy((__mks_1186.left),(nullptr));
                                das_copy((__mks_1186.right),(nullptr));
                                das_copy((__mks_1186.subexpr),(nullptr));
                                das_copy((__mks_1186.cset),((([&]() -> TDim<uint32_t,8> {
                                    TDim<uint32_t,8> __mka_1186;
                                    __mka_1186(0,__context__) = 0x0u;
                                    __mka_1186(1,__context__) = 0x3ff0000u;
                                    __mka_1186(2,__context__) = 0x7fffffeu;
                                    __mka_1186(3,__context__) = 0x7fffffeu;
                                    __mka_1186(4,__context__) = 0x0u;
                                    __mka_1186(5,__context__) = 0x0u;
                                    __mka_1186(6,__context__) = 0x0u;
                                    __mka_1186(7,__context__) = 0x0u;
                                    return __mka_1186;
                                })())));
                                das_copy((__mks_1186.index),(0));
                                return __mks_1186;
                            })()))));
                            das_copy((__mks_1186.cset),((([&]() -> TDim<uint32_t,8> {
                                TDim<uint32_t,8> __mka_1186;
                                __mka_1186(0,__context__) = 0x0u;
                                __mka_1186(1,__context__) = 0x3ff0000u;
                                __mka_1186(2,__context__) = 0x7fffffeu;
                                __mka_1186(3,__context__) = 0x7fffffeu;
                                __mka_1186(4,__context__) = 0x0u;
                                __mka_1186(5,__context__) = 0x0u;
                                __mka_1186(6,__context__) = 0x0u;
                                __mka_1186(7,__context__) = 0x0u;
                                return __mka_1186;
                            })())));
                            das_copy((__mks_1186.index),(0));
                            return __mks_1186;
                        })()))));
                        das_copy((__mks_1186.subexpr),(nullptr));
                        das_copy((__mks_1186.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_1186;
                            __mka_1186(0,__context__) = 0x0u;
                            __mka_1186(1,__context__) = 0x0u;
                            __mka_1186(2,__context__) = 0x0u;
                            __mka_1186(3,__context__) = 0x0u;
                            __mka_1186(4,__context__) = 0x0u;
                            __mka_1186(5,__context__) = 0x0u;
                            __mka_1186(6,__context__) = 0x0u;
                            __mka_1186(7,__context__) = 0x0u;
                            return __mka_1186;
                        })())));
                        das_copy((__mks_1186.index),(0));
                        return __mks_1186;
                    })()))));
                    das_copy((__mks_1186.cset),((([&]() -> TDim<uint32_t,8> {
                        TDim<uint32_t,8> __mka_1186;
                        __mka_1186(0,__context__) = 0x0u;
                        __mka_1186(1,__context__) = 0x0u;
                        __mka_1186(2,__context__) = 0x0u;
                        __mka_1186(3,__context__) = 0x0u;
                        __mka_1186(4,__context__) = 0x0u;
                        __mka_1186(5,__context__) = 0x0u;
                        __mka_1186(6,__context__) = 0x0u;
                        __mka_1186(7,__context__) = 0x0u;
                        return __mka_1186;
                    })())));
                    das_copy((__mks_1186.index),(2));
                    return __mks_1186;
                })()))));
                das_copy((__mks_1186.subexpr),(nullptr));
                das_copy((__mks_1186.cset),((([&]() -> TDim<uint32_t,8> {
                    TDim<uint32_t,8> __mka_1186;
                    __mka_1186(0,__context__) = 0x0u;
                    __mka_1186(1,__context__) = 0x0u;
                    __mka_1186(2,__context__) = 0x0u;
                    __mka_1186(3,__context__) = 0x0u;
                    __mka_1186(4,__context__) = 0x0u;
                    __mka_1186(5,__context__) = 0x0u;
                    __mka_1186(6,__context__) = 0x0u;
                    __mka_1186(7,__context__) = 0x0u;
                    return __mka_1186;
                })())));
                das_copy((__mks_1186.index),(0));
                return __mks_1186;
            })()))));
            das_copy((__mks_1186.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_1186;
                das_zero(__mks_1186);
                das_copy((__mks_1186.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                das_copy((__mks_1186.id),(25));
                das_copy((__mks_1186.at),(range(47,49)));
                das_copy((__mks_1186.text),(((char *) ")")));
                das_copy((__mks_1186.textLen),(1));
                das_move((__mks_1186.all),((([&]() -> TArray<regex::ReNode *> {
                    TArray<regex::ReNode *> __mks_1186;
                    das_zero(__mks_1186);
                    return __mks_1186;
                })())));
                das_copy((__mks_1186.left),(nullptr));
                das_copy((__mks_1186.right),(nullptr));
                das_copy((__mks_1186.subexpr),(nullptr));
                das_copy((__mks_1186.cset),((([&]() -> TDim<uint32_t,8> {
                    TDim<uint32_t,8> __mka_1186;
                    __mka_1186(0,__context__) = 0x0u;
                    __mka_1186(1,__context__) = 0x0u;
                    __mka_1186(2,__context__) = 0x0u;
                    __mka_1186(3,__context__) = 0x0u;
                    __mka_1186(4,__context__) = 0x0u;
                    __mka_1186(5,__context__) = 0x0u;
                    __mka_1186(6,__context__) = 0x0u;
                    __mka_1186(7,__context__) = 0x0u;
                    return __mka_1186;
                })())));
                das_copy((__mks_1186.index),(0));
                return __mks_1186;
            })()))));
            das_copy((__mks_1186.subexpr),(nullptr));
            das_copy((__mks_1186.cset),((([&]() -> TDim<uint32_t,8> {
                TDim<uint32_t,8> __mka_1186;
                __mka_1186(0,__context__) = 0x0u;
                __mka_1186(1,__context__) = 0x0u;
                __mka_1186(2,__context__) = 0x0u;
                __mka_1186(3,__context__) = 0x0u;
                __mka_1186(4,__context__) = 0x0u;
                __mka_1186(5,__context__) = 0x0u;
                __mka_1186(6,__context__) = 0x0u;
                __mka_1186(7,__context__) = 0x0u;
                return __mka_1186;
            })())));
            das_copy((__mks_1186.index),(0));
            return __mks_1186;
        })()))));
        das_move((_temp_make_local_1186_26_2304.groups),(_FuncbuiltinTickto_array_moveTick3185538323411982277_71fbdf75ac58194(__context__,das_arg<TDim<AutoTuple<range,char *>,3>>::pass((([&]() -> TDim<AutoTuple<range,char *>,3>& {
            _temp_make_local_1186_26_2800(0,__context__) = (([&]() -> AutoTuple<range,char *> {
                AutoTuple<range,char *> __mkt_1186;
                das_get_auto_tuple_field<range,0,range,char *>::get(__mkt_1186) = range(0,0);
                das_get_auto_tuple_field<char *,1,range,char *>::get(__mkt_1186) = nullptr;
                return __mkt_1186;
            })());
            _temp_make_local_1186_26_2800(1,__context__) = (([&]() -> AutoTuple<range,char *> {
                AutoTuple<range,char *> __mkt_1186;
                das_get_auto_tuple_field<range,0,range,char *>::get(__mkt_1186) = range(0,0);
                das_get_auto_tuple_field<char *,1,range,char *>::get(__mkt_1186) = ((char *) "1");
                return __mkt_1186;
            })());
            _temp_make_local_1186_26_2800(2,__context__) = (([&]() -> AutoTuple<range,char *> {
                AutoTuple<range,char *> __mkt_1186;
                das_get_auto_tuple_field<range,0,range,char *>::get(__mkt_1186) = range(0,0);
                das_get_auto_tuple_field<char *,1,range,char *>::get(__mkt_1186) = ((char *) "2");
                return __mkt_1186;
            })());
            return _temp_make_local_1186_26_2800;
        })())))));
        das_copy((_temp_make_local_1186_26_2304.earlyOut),((([&]() -> TDim<uint32_t,8> {
            TDim<uint32_t,8> __mka_1186;
            __mka_1186(0,__context__) = 0x0u;
            __mka_1186(1,__context__) = 0x8u;
            __mka_1186(2,__context__) = 0x0u;
            __mka_1186(3,__context__) = 0x0u;
            __mka_1186(4,__context__) = 0x0u;
            __mka_1186(5,__context__) = 0x0u;
            __mka_1186(6,__context__) = 0x0u;
            __mka_1186(7,__context__) = 0x0u;
            return __mka_1186;
        })())));
        das_copy((_temp_make_local_1186_26_2304.canEarlyOut),(true));
        return _temp_make_local_1186_26_2304;
    })()))));
    regex::Regex __reg_def_UINT64_rename_at_1187_352; das_zero(__reg_def_UINT64_rename_at_1187_352); das_move(__reg_def_UINT64_rename_at_1187_352, das_invoke_function<regex::Regex>::invoke_cmres<regex::Regex &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@regex::regex_compile S<regex::Regex>*/ 0x235aa29958cfc4c4)),das_arg<regex::Regex>::pass((([&]() -> regex::Regex& {
        das_zero(_temp_make_local_1187_26_2928);
        das_copy((_temp_make_local_1187_26_2928.root),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
            regex::ReNode __mks_1187;
            das_zero(__mks_1187);
            das_copy((__mks_1187.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
            das_copy((__mks_1187.id),(0));
            das_copy((__mks_1187.at),(range(0,49)));
            das_copy((__mks_1187.text),(nullptr));
            das_copy((__mks_1187.textLen),(0));
            das_move((__mks_1187.all),((([&]() -> TArray<regex::ReNode *> {
                TArray<regex::ReNode *> __mks_1187;
                das_zero(__mks_1187);
                return __mks_1187;
            })())));
            das_copy((__mks_1187.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_1187;
                das_zero(__mks_1187);
                das_copy((__mks_1187.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                das_copy((__mks_1187.id),(1));
                das_copy((__mks_1187.at),(range(0,47)));
                das_copy((__mks_1187.text),(nullptr));
                das_copy((__mks_1187.textLen),(0));
                das_move((__mks_1187.all),((([&]() -> TArray<regex::ReNode *> {
                    TArray<regex::ReNode *> __mks_1187;
                    das_zero(__mks_1187);
                    return __mks_1187;
                })())));
                das_copy((__mks_1187.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_1187;
                    das_zero(__mks_1187);
                    das_copy((__mks_1187.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                    das_copy((__mks_1187.id),(2));
                    das_copy((__mks_1187.at),(range(0,31)));
                    das_copy((__mks_1187.text),(nullptr));
                    das_copy((__mks_1187.textLen),(0));
                    das_move((__mks_1187.all),((([&]() -> TArray<regex::ReNode *> {
                        TArray<regex::ReNode *> __mks_1187;
                        das_zero(__mks_1187);
                        return __mks_1187;
                    })())));
                    das_copy((__mks_1187.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_1187;
                        das_zero(__mks_1187);
                        das_copy((__mks_1187.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                        das_copy((__mks_1187.id),(3));
                        das_copy((__mks_1187.at),(range(0,29)));
                        das_copy((__mks_1187.text),(nullptr));
                        das_copy((__mks_1187.textLen),(0));
                        das_move((__mks_1187.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_1187;
                            das_zero(__mks_1187);
                            return __mks_1187;
                        })())));
                        das_copy((__mks_1187.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                            regex::ReNode __mks_1187;
                            das_zero(__mks_1187);
                            das_copy((__mks_1187.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                            das_copy((__mks_1187.id),(4));
                            das_copy((__mks_1187.at),(range(0,26)));
                            das_copy((__mks_1187.text),(nullptr));
                            das_copy((__mks_1187.textLen),(0));
                            das_move((__mks_1187.all),((([&]() -> TArray<regex::ReNode *> {
                                TArray<regex::ReNode *> __mks_1187;
                                das_zero(__mks_1187);
                                return __mks_1187;
                            })())));
                            das_copy((__mks_1187.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                regex::ReNode __mks_1187;
                                das_zero(__mks_1187);
                                das_copy((__mks_1187.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                                das_copy((__mks_1187.id),(5));
                                das_copy((__mks_1187.at),(range(0,18)));
                                das_copy((__mks_1187.text),(nullptr));
                                das_copy((__mks_1187.textLen),(0));
                                das_move((__mks_1187.all),((([&]() -> TArray<regex::ReNode *> {
                                    TArray<regex::ReNode *> __mks_1187;
                                    das_zero(__mks_1187);
                                    return __mks_1187;
                                })())));
                                das_copy((__mks_1187.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                    regex::ReNode __mks_1187;
                                    das_zero(__mks_1187);
                                    das_copy((__mks_1187.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                                    das_copy((__mks_1187.id),(6));
                                    das_copy((__mks_1187.at),(range(0,15)));
                                    das_copy((__mks_1187.text),(nullptr));
                                    das_copy((__mks_1187.textLen),(0));
                                    das_move((__mks_1187.all),((([&]() -> TArray<regex::ReNode *> {
                                        TArray<regex::ReNode *> __mks_1187;
                                        das_zero(__mks_1187);
                                        return __mks_1187;
                                    })())));
                                    das_copy((__mks_1187.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                        regex::ReNode __mks_1187;
                                        das_zero(__mks_1187);
                                        das_copy((__mks_1187.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                                        das_copy((__mks_1187.id),(7));
                                        das_copy((__mks_1187.at),(range(0,10)));
                                        das_copy((__mks_1187.text),(nullptr));
                                        das_copy((__mks_1187.textLen),(0));
                                        das_move((__mks_1187.all),((([&]() -> TArray<regex::ReNode *> {
                                            TArray<regex::ReNode *> __mks_1187;
                                            das_zero(__mks_1187);
                                            return __mks_1187;
                                        })())));
                                        das_copy((__mks_1187.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                            regex::ReNode __mks_1187;
                                            das_zero(__mks_1187);
                                            das_copy((__mks_1187.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                                            das_copy((__mks_1187.id),(8));
                                            das_copy((__mks_1187.at),(range(0,7)));
                                            das_copy((__mks_1187.text),(((char *) "#define")));
                                            das_copy((__mks_1187.textLen),(7));
                                            das_move((__mks_1187.all),((([&]() -> TArray<regex::ReNode *> {
                                                TArray<regex::ReNode *> __mks_1187;
                                                das_zero(__mks_1187);
                                                return __mks_1187;
                                            })())));
                                            das_copy((__mks_1187.left),(nullptr));
                                            das_copy((__mks_1187.right),(nullptr));
                                            das_copy((__mks_1187.subexpr),(nullptr));
                                            das_copy((__mks_1187.cset),((([&]() -> TDim<uint32_t,8> {
                                                TDim<uint32_t,8> __mka_1187;
                                                __mka_1187(0,__context__) = 0x0u;
                                                __mka_1187(1,__context__) = 0x0u;
                                                __mka_1187(2,__context__) = 0x0u;
                                                __mka_1187(3,__context__) = 0x0u;
                                                __mka_1187(4,__context__) = 0x0u;
                                                __mka_1187(5,__context__) = 0x0u;
                                                __mka_1187(6,__context__) = 0x0u;
                                                __mka_1187(7,__context__) = 0x0u;
                                                return __mka_1187;
                                            })())));
                                            das_copy((__mks_1187.index),(0));
                                            return __mks_1187;
                                        })()))));
                                        das_copy((__mks_1187.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                            regex::ReNode __mks_1187;
                                            das_zero(__mks_1187);
                                            das_copy((__mks_1187.op),(DAS_COMMENT(enum) regex::ReOp::Plus));
                                            das_copy((__mks_1187.id),(9));
                                            das_copy((__mks_1187.at),(range(7,10)));
                                            das_copy((__mks_1187.text),(nullptr));
                                            das_copy((__mks_1187.textLen),(0));
                                            das_move((__mks_1187.all),((([&]() -> TArray<regex::ReNode *> {
                                                TArray<regex::ReNode *> __mks_1187;
                                                das_zero(__mks_1187);
                                                return __mks_1187;
                                            })())));
                                            das_copy((__mks_1187.left),(nullptr));
                                            das_copy((__mks_1187.right),(nullptr));
                                            das_copy((__mks_1187.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                                regex::ReNode __mks_1187;
                                                das_zero(__mks_1187);
                                                das_copy((__mks_1187.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                                                das_copy((__mks_1187.id),(10));
                                                das_copy((__mks_1187.at),(range(7,9)));
                                                das_copy((__mks_1187.text),(nullptr));
                                                das_copy((__mks_1187.textLen),(0));
                                                das_move((__mks_1187.all),((([&]() -> TArray<regex::ReNode *> {
                                                    TArray<regex::ReNode *> __mks_1187;
                                                    das_zero(__mks_1187);
                                                    return __mks_1187;
                                                })())));
                                                das_copy((__mks_1187.left),(nullptr));
                                                das_copy((__mks_1187.right),(nullptr));
                                                das_copy((__mks_1187.subexpr),(nullptr));
                                                das_copy((__mks_1187.cset),((([&]() -> TDim<uint32_t,8> {
                                                    TDim<uint32_t,8> __mka_1187;
                                                    __mka_1187(0,__context__) = 0x200u;
                                                    __mka_1187(1,__context__) = 0x1u;
                                                    __mka_1187(2,__context__) = 0x0u;
                                                    __mka_1187(3,__context__) = 0x0u;
                                                    __mka_1187(4,__context__) = 0x0u;
                                                    __mka_1187(5,__context__) = 0x0u;
                                                    __mka_1187(6,__context__) = 0x0u;
                                                    __mka_1187(7,__context__) = 0x0u;
                                                    return __mka_1187;
                                                })())));
                                                das_copy((__mks_1187.index),(0));
                                                return __mks_1187;
                                            })()))));
                                            das_copy((__mks_1187.cset),((([&]() -> TDim<uint32_t,8> {
                                                TDim<uint32_t,8> __mka_1187;
                                                __mka_1187(0,__context__) = 0x200u;
                                                __mka_1187(1,__context__) = 0x1u;
                                                __mka_1187(2,__context__) = 0x0u;
                                                __mka_1187(3,__context__) = 0x0u;
                                                __mka_1187(4,__context__) = 0x0u;
                                                __mka_1187(5,__context__) = 0x0u;
                                                __mka_1187(6,__context__) = 0x0u;
                                                __mka_1187(7,__context__) = 0x0u;
                                                return __mka_1187;
                                            })())));
                                            das_copy((__mks_1187.index),(0));
                                            return __mks_1187;
                                        })()))));
                                        das_copy((__mks_1187.subexpr),(nullptr));
                                        das_copy((__mks_1187.cset),((([&]() -> TDim<uint32_t,8> {
                                            TDim<uint32_t,8> __mka_1187;
                                            __mka_1187(0,__context__) = 0x0u;
                                            __mka_1187(1,__context__) = 0x0u;
                                            __mka_1187(2,__context__) = 0x0u;
                                            __mka_1187(3,__context__) = 0x0u;
                                            __mka_1187(4,__context__) = 0x0u;
                                            __mka_1187(5,__context__) = 0x0u;
                                            __mka_1187(6,__context__) = 0x0u;
                                            __mka_1187(7,__context__) = 0x0u;
                                            return __mka_1187;
                                        })())));
                                        das_copy((__mks_1187.index),(0));
                                        return __mks_1187;
                                    })()))));
                                    das_copy((__mks_1187.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                        regex::ReNode __mks_1187;
                                        das_zero(__mks_1187);
                                        das_copy((__mks_1187.op),(DAS_COMMENT(enum) regex::ReOp::Group));
                                        das_copy((__mks_1187.id),(11));
                                        das_copy((__mks_1187.at),(range(10,15)));
                                        das_copy((__mks_1187.text),(nullptr));
                                        das_copy((__mks_1187.textLen),(0));
                                        das_move((__mks_1187.all),((([&]() -> TArray<regex::ReNode *> {
                                            TArray<regex::ReNode *> __mks_1187;
                                            das_zero(__mks_1187);
                                            return __mks_1187;
                                        })())));
                                        das_copy((__mks_1187.left),(nullptr));
                                        das_copy((__mks_1187.right),(nullptr));
                                        das_copy((__mks_1187.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                            regex::ReNode __mks_1187;
                                            das_zero(__mks_1187);
                                            das_copy((__mks_1187.op),(DAS_COMMENT(enum) regex::ReOp::Plus));
                                            das_copy((__mks_1187.id),(12));
                                            das_copy((__mks_1187.at),(range(11,14)));
                                            das_copy((__mks_1187.text),(nullptr));
                                            das_copy((__mks_1187.textLen),(0));
                                            das_move((__mks_1187.all),((([&]() -> TArray<regex::ReNode *> {
                                                TArray<regex::ReNode *> __mks_1187;
                                                das_zero(__mks_1187);
                                                return __mks_1187;
                                            })())));
                                            das_copy((__mks_1187.left),(nullptr));
                                            das_copy((__mks_1187.right),(nullptr));
                                            das_copy((__mks_1187.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                                regex::ReNode __mks_1187;
                                                das_zero(__mks_1187);
                                                das_copy((__mks_1187.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                                                das_copy((__mks_1187.id),(13));
                                                das_copy((__mks_1187.at),(range(11,13)));
                                                das_copy((__mks_1187.text),(nullptr));
                                                das_copy((__mks_1187.textLen),(0));
                                                das_move((__mks_1187.all),((([&]() -> TArray<regex::ReNode *> {
                                                    TArray<regex::ReNode *> __mks_1187;
                                                    das_zero(__mks_1187);
                                                    return __mks_1187;
                                                })())));
                                                das_copy((__mks_1187.left),(nullptr));
                                                das_copy((__mks_1187.right),(nullptr));
                                                das_copy((__mks_1187.subexpr),(nullptr));
                                                das_copy((__mks_1187.cset),((([&]() -> TDim<uint32_t,8> {
                                                    TDim<uint32_t,8> __mka_1187;
                                                    __mka_1187(0,__context__) = 0x0u;
                                                    __mka_1187(1,__context__) = 0x3ff0000u;
                                                    __mka_1187(2,__context__) = 0x87fffffeu;
                                                    __mka_1187(3,__context__) = 0x7fffffeu;
                                                    __mka_1187(4,__context__) = 0x0u;
                                                    __mka_1187(5,__context__) = 0x0u;
                                                    __mka_1187(6,__context__) = 0x0u;
                                                    __mka_1187(7,__context__) = 0x0u;
                                                    return __mka_1187;
                                                })())));
                                                das_copy((__mks_1187.index),(0));
                                                return __mks_1187;
                                            })()))));
                                            das_copy((__mks_1187.cset),((([&]() -> TDim<uint32_t,8> {
                                                TDim<uint32_t,8> __mka_1187;
                                                __mka_1187(0,__context__) = 0x0u;
                                                __mka_1187(1,__context__) = 0x3ff0000u;
                                                __mka_1187(2,__context__) = 0x87fffffeu;
                                                __mka_1187(3,__context__) = 0x7fffffeu;
                                                __mka_1187(4,__context__) = 0x0u;
                                                __mka_1187(5,__context__) = 0x0u;
                                                __mka_1187(6,__context__) = 0x0u;
                                                __mka_1187(7,__context__) = 0x0u;
                                                return __mka_1187;
                                            })())));
                                            das_copy((__mks_1187.index),(0));
                                            return __mks_1187;
                                        })()))));
                                        das_copy((__mks_1187.cset),((([&]() -> TDim<uint32_t,8> {
                                            TDim<uint32_t,8> __mka_1187;
                                            __mka_1187(0,__context__) = 0x0u;
                                            __mka_1187(1,__context__) = 0x0u;
                                            __mka_1187(2,__context__) = 0x0u;
                                            __mka_1187(3,__context__) = 0x0u;
                                            __mka_1187(4,__context__) = 0x0u;
                                            __mka_1187(5,__context__) = 0x0u;
                                            __mka_1187(6,__context__) = 0x0u;
                                            __mka_1187(7,__context__) = 0x0u;
                                            return __mka_1187;
                                        })())));
                                        das_copy((__mks_1187.index),(1));
                                        return __mks_1187;
                                    })()))));
                                    das_copy((__mks_1187.subexpr),(nullptr));
                                    das_copy((__mks_1187.cset),((([&]() -> TDim<uint32_t,8> {
                                        TDim<uint32_t,8> __mka_1187;
                                        __mka_1187(0,__context__) = 0x0u;
                                        __mka_1187(1,__context__) = 0x0u;
                                        __mka_1187(2,__context__) = 0x0u;
                                        __mka_1187(3,__context__) = 0x0u;
                                        __mka_1187(4,__context__) = 0x0u;
                                        __mka_1187(5,__context__) = 0x0u;
                                        __mka_1187(6,__context__) = 0x0u;
                                        __mka_1187(7,__context__) = 0x0u;
                                        return __mka_1187;
                                    })())));
                                    das_copy((__mks_1187.index),(0));
                                    return __mks_1187;
                                })()))));
                                das_copy((__mks_1187.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                    regex::ReNode __mks_1187;
                                    das_zero(__mks_1187);
                                    das_copy((__mks_1187.op),(DAS_COMMENT(enum) regex::ReOp::Plus));
                                    das_copy((__mks_1187.id),(14));
                                    das_copy((__mks_1187.at),(range(15,18)));
                                    das_copy((__mks_1187.text),(nullptr));
                                    das_copy((__mks_1187.textLen),(0));
                                    das_move((__mks_1187.all),((([&]() -> TArray<regex::ReNode *> {
                                        TArray<regex::ReNode *> __mks_1187;
                                        das_zero(__mks_1187);
                                        return __mks_1187;
                                    })())));
                                    das_copy((__mks_1187.left),(nullptr));
                                    das_copy((__mks_1187.right),(nullptr));
                                    das_copy((__mks_1187.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                        regex::ReNode __mks_1187;
                                        das_zero(__mks_1187);
                                        das_copy((__mks_1187.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                                        das_copy((__mks_1187.id),(15));
                                        das_copy((__mks_1187.at),(range(15,17)));
                                        das_copy((__mks_1187.text),(nullptr));
                                        das_copy((__mks_1187.textLen),(0));
                                        das_move((__mks_1187.all),((([&]() -> TArray<regex::ReNode *> {
                                            TArray<regex::ReNode *> __mks_1187;
                                            das_zero(__mks_1187);
                                            return __mks_1187;
                                        })())));
                                        das_copy((__mks_1187.left),(nullptr));
                                        das_copy((__mks_1187.right),(nullptr));
                                        das_copy((__mks_1187.subexpr),(nullptr));
                                        das_copy((__mks_1187.cset),((([&]() -> TDim<uint32_t,8> {
                                            TDim<uint32_t,8> __mka_1187;
                                            __mka_1187(0,__context__) = 0x200u;
                                            __mka_1187(1,__context__) = 0x1u;
                                            __mka_1187(2,__context__) = 0x0u;
                                            __mka_1187(3,__context__) = 0x0u;
                                            __mka_1187(4,__context__) = 0x0u;
                                            __mka_1187(5,__context__) = 0x0u;
                                            __mka_1187(6,__context__) = 0x0u;
                                            __mka_1187(7,__context__) = 0x0u;
                                            return __mka_1187;
                                        })())));
                                        das_copy((__mks_1187.index),(0));
                                        return __mks_1187;
                                    })()))));
                                    das_copy((__mks_1187.cset),((([&]() -> TDim<uint32_t,8> {
                                        TDim<uint32_t,8> __mka_1187;
                                        __mka_1187(0,__context__) = 0x200u;
                                        __mka_1187(1,__context__) = 0x1u;
                                        __mka_1187(2,__context__) = 0x0u;
                                        __mka_1187(3,__context__) = 0x0u;
                                        __mka_1187(4,__context__) = 0x0u;
                                        __mka_1187(5,__context__) = 0x0u;
                                        __mka_1187(6,__context__) = 0x0u;
                                        __mka_1187(7,__context__) = 0x0u;
                                        return __mka_1187;
                                    })())));
                                    das_copy((__mks_1187.index),(0));
                                    return __mks_1187;
                                })()))));
                                das_copy((__mks_1187.subexpr),(nullptr));
                                das_copy((__mks_1187.cset),((([&]() -> TDim<uint32_t,8> {
                                    TDim<uint32_t,8> __mka_1187;
                                    __mka_1187(0,__context__) = 0x0u;
                                    __mka_1187(1,__context__) = 0x0u;
                                    __mka_1187(2,__context__) = 0x0u;
                                    __mka_1187(3,__context__) = 0x0u;
                                    __mka_1187(4,__context__) = 0x0u;
                                    __mka_1187(5,__context__) = 0x0u;
                                    __mka_1187(6,__context__) = 0x0u;
                                    __mka_1187(7,__context__) = 0x0u;
                                    return __mka_1187;
                                })())));
                                das_copy((__mks_1187.index),(0));
                                return __mks_1187;
                            })()))));
                            das_copy((__mks_1187.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                regex::ReNode __mks_1187;
                                das_zero(__mks_1187);
                                das_copy((__mks_1187.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                                das_copy((__mks_1187.id),(16));
                                das_copy((__mks_1187.at),(range(18,26)));
                                das_copy((__mks_1187.text),(((char *) "UINT64_C")));
                                das_copy((__mks_1187.textLen),(8));
                                das_move((__mks_1187.all),((([&]() -> TArray<regex::ReNode *> {
                                    TArray<regex::ReNode *> __mks_1187;
                                    das_zero(__mks_1187);
                                    return __mks_1187;
                                })())));
                                das_copy((__mks_1187.left),(nullptr));
                                das_copy((__mks_1187.right),(nullptr));
                                das_copy((__mks_1187.subexpr),(nullptr));
                                das_copy((__mks_1187.cset),((([&]() -> TDim<uint32_t,8> {
                                    TDim<uint32_t,8> __mka_1187;
                                    __mka_1187(0,__context__) = 0x0u;
                                    __mka_1187(1,__context__) = 0x0u;
                                    __mka_1187(2,__context__) = 0x0u;
                                    __mka_1187(3,__context__) = 0x0u;
                                    __mka_1187(4,__context__) = 0x0u;
                                    __mka_1187(5,__context__) = 0x0u;
                                    __mka_1187(6,__context__) = 0x0u;
                                    __mka_1187(7,__context__) = 0x0u;
                                    return __mka_1187;
                                })())));
                                das_copy((__mks_1187.index),(0));
                                return __mks_1187;
                            })()))));
                            das_copy((__mks_1187.subexpr),(nullptr));
                            das_copy((__mks_1187.cset),((([&]() -> TDim<uint32_t,8> {
                                TDim<uint32_t,8> __mka_1187;
                                __mka_1187(0,__context__) = 0x0u;
                                __mka_1187(1,__context__) = 0x0u;
                                __mka_1187(2,__context__) = 0x0u;
                                __mka_1187(3,__context__) = 0x0u;
                                __mka_1187(4,__context__) = 0x0u;
                                __mka_1187(5,__context__) = 0x0u;
                                __mka_1187(6,__context__) = 0x0u;
                                __mka_1187(7,__context__) = 0x0u;
                                return __mka_1187;
                            })())));
                            das_copy((__mks_1187.index),(0));
                            return __mks_1187;
                        })()))));
                        das_copy((__mks_1187.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                            regex::ReNode __mks_1187;
                            das_zero(__mks_1187);
                            das_copy((__mks_1187.op),(DAS_COMMENT(enum) regex::ReOp::Star));
                            das_copy((__mks_1187.id),(17));
                            das_copy((__mks_1187.at),(range(26,29)));
                            das_copy((__mks_1187.text),(nullptr));
                            das_copy((__mks_1187.textLen),(0));
                            das_move((__mks_1187.all),((([&]() -> TArray<regex::ReNode *> {
                                TArray<regex::ReNode *> __mks_1187;
                                das_zero(__mks_1187);
                                return __mks_1187;
                            })())));
                            das_copy((__mks_1187.left),(nullptr));
                            das_copy((__mks_1187.right),(nullptr));
                            das_copy((__mks_1187.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                regex::ReNode __mks_1187;
                                das_zero(__mks_1187);
                                das_copy((__mks_1187.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                                das_copy((__mks_1187.id),(18));
                                das_copy((__mks_1187.at),(range(26,28)));
                                das_copy((__mks_1187.text),(nullptr));
                                das_copy((__mks_1187.textLen),(0));
                                das_move((__mks_1187.all),((([&]() -> TArray<regex::ReNode *> {
                                    TArray<regex::ReNode *> __mks_1187;
                                    das_zero(__mks_1187);
                                    return __mks_1187;
                                })())));
                                das_copy((__mks_1187.left),(nullptr));
                                das_copy((__mks_1187.right),(nullptr));
                                das_copy((__mks_1187.subexpr),(nullptr));
                                das_copy((__mks_1187.cset),((([&]() -> TDim<uint32_t,8> {
                                    TDim<uint32_t,8> __mka_1187;
                                    __mka_1187(0,__context__) = 0x200u;
                                    __mka_1187(1,__context__) = 0x1u;
                                    __mka_1187(2,__context__) = 0x0u;
                                    __mka_1187(3,__context__) = 0x0u;
                                    __mka_1187(4,__context__) = 0x0u;
                                    __mka_1187(5,__context__) = 0x0u;
                                    __mka_1187(6,__context__) = 0x0u;
                                    __mka_1187(7,__context__) = 0x0u;
                                    return __mka_1187;
                                })())));
                                das_copy((__mks_1187.index),(0));
                                return __mks_1187;
                            })()))));
                            das_copy((__mks_1187.cset),((([&]() -> TDim<uint32_t,8> {
                                TDim<uint32_t,8> __mka_1187;
                                __mka_1187(0,__context__) = 0x0u;
                                __mka_1187(1,__context__) = 0x0u;
                                __mka_1187(2,__context__) = 0x0u;
                                __mka_1187(3,__context__) = 0x0u;
                                __mka_1187(4,__context__) = 0x0u;
                                __mka_1187(5,__context__) = 0x0u;
                                __mka_1187(6,__context__) = 0x0u;
                                __mka_1187(7,__context__) = 0x0u;
                                return __mka_1187;
                            })())));
                            das_copy((__mks_1187.index),(0));
                            return __mks_1187;
                        })()))));
                        das_copy((__mks_1187.subexpr),(nullptr));
                        das_copy((__mks_1187.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_1187;
                            __mka_1187(0,__context__) = 0x0u;
                            __mka_1187(1,__context__) = 0x0u;
                            __mka_1187(2,__context__) = 0x0u;
                            __mka_1187(3,__context__) = 0x0u;
                            __mka_1187(4,__context__) = 0x0u;
                            __mka_1187(5,__context__) = 0x0u;
                            __mka_1187(6,__context__) = 0x0u;
                            __mka_1187(7,__context__) = 0x0u;
                            return __mka_1187;
                        })())));
                        das_copy((__mks_1187.index),(0));
                        return __mks_1187;
                    })()))));
                    das_copy((__mks_1187.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_1187;
                        das_zero(__mks_1187);
                        das_copy((__mks_1187.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                        das_copy((__mks_1187.id),(19));
                        das_copy((__mks_1187.at),(range(29,31)));
                        das_copy((__mks_1187.text),(((char *) "(")));
                        das_copy((__mks_1187.textLen),(1));
                        das_move((__mks_1187.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_1187;
                            das_zero(__mks_1187);
                            return __mks_1187;
                        })())));
                        das_copy((__mks_1187.left),(nullptr));
                        das_copy((__mks_1187.right),(nullptr));
                        das_copy((__mks_1187.subexpr),(nullptr));
                        das_copy((__mks_1187.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_1187;
                            __mka_1187(0,__context__) = 0x0u;
                            __mka_1187(1,__context__) = 0x0u;
                            __mka_1187(2,__context__) = 0x0u;
                            __mka_1187(3,__context__) = 0x0u;
                            __mka_1187(4,__context__) = 0x0u;
                            __mka_1187(5,__context__) = 0x0u;
                            __mka_1187(6,__context__) = 0x0u;
                            __mka_1187(7,__context__) = 0x0u;
                            return __mka_1187;
                        })())));
                        das_copy((__mks_1187.index),(0));
                        return __mks_1187;
                    })()))));
                    das_copy((__mks_1187.subexpr),(nullptr));
                    das_copy((__mks_1187.cset),((([&]() -> TDim<uint32_t,8> {
                        TDim<uint32_t,8> __mka_1187;
                        __mka_1187(0,__context__) = 0x0u;
                        __mka_1187(1,__context__) = 0x0u;
                        __mka_1187(2,__context__) = 0x0u;
                        __mka_1187(3,__context__) = 0x0u;
                        __mka_1187(4,__context__) = 0x0u;
                        __mka_1187(5,__context__) = 0x0u;
                        __mka_1187(6,__context__) = 0x0u;
                        __mka_1187(7,__context__) = 0x0u;
                        return __mka_1187;
                    })())));
                    das_copy((__mks_1187.index),(0));
                    return __mks_1187;
                })()))));
                das_copy((__mks_1187.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                    regex::ReNode __mks_1187;
                    das_zero(__mks_1187);
                    das_copy((__mks_1187.op),(DAS_COMMENT(enum) regex::ReOp::Group));
                    das_copy((__mks_1187.id),(20));
                    das_copy((__mks_1187.at),(range(31,47)));
                    das_copy((__mks_1187.text),(nullptr));
                    das_copy((__mks_1187.textLen),(0));
                    das_move((__mks_1187.all),((([&]() -> TArray<regex::ReNode *> {
                        TArray<regex::ReNode *> __mks_1187;
                        das_zero(__mks_1187);
                        return __mks_1187;
                    })())));
                    das_copy((__mks_1187.left),(nullptr));
                    das_copy((__mks_1187.right),(nullptr));
                    das_copy((__mks_1187.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                        regex::ReNode __mks_1187;
                        das_zero(__mks_1187);
                        das_copy((__mks_1187.op),(DAS_COMMENT(enum) regex::ReOp::Concat));
                        das_copy((__mks_1187.id),(21));
                        das_copy((__mks_1187.at),(range(32,46)));
                        das_copy((__mks_1187.text),(nullptr));
                        das_copy((__mks_1187.textLen),(0));
                        das_move((__mks_1187.all),((([&]() -> TArray<regex::ReNode *> {
                            TArray<regex::ReNode *> __mks_1187;
                            das_zero(__mks_1187);
                            return __mks_1187;
                        })())));
                        das_copy((__mks_1187.left),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                            regex::ReNode __mks_1187;
                            das_zero(__mks_1187);
                            das_copy((__mks_1187.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                            das_copy((__mks_1187.id),(22));
                            das_copy((__mks_1187.at),(range(32,34)));
                            das_copy((__mks_1187.text),(((char *) "0x")));
                            das_copy((__mks_1187.textLen),(2));
                            das_move((__mks_1187.all),((([&]() -> TArray<regex::ReNode *> {
                                TArray<regex::ReNode *> __mks_1187;
                                das_zero(__mks_1187);
                                return __mks_1187;
                            })())));
                            das_copy((__mks_1187.left),(nullptr));
                            das_copy((__mks_1187.right),(nullptr));
                            das_copy((__mks_1187.subexpr),(nullptr));
                            das_copy((__mks_1187.cset),((([&]() -> TDim<uint32_t,8> {
                                TDim<uint32_t,8> __mka_1187;
                                __mka_1187(0,__context__) = 0x0u;
                                __mka_1187(1,__context__) = 0x0u;
                                __mka_1187(2,__context__) = 0x0u;
                                __mka_1187(3,__context__) = 0x0u;
                                __mka_1187(4,__context__) = 0x0u;
                                __mka_1187(5,__context__) = 0x0u;
                                __mka_1187(6,__context__) = 0x0u;
                                __mka_1187(7,__context__) = 0x0u;
                                return __mka_1187;
                            })())));
                            das_copy((__mks_1187.index),(0));
                            return __mks_1187;
                        })()))));
                        das_copy((__mks_1187.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                            regex::ReNode __mks_1187;
                            das_zero(__mks_1187);
                            das_copy((__mks_1187.op),(DAS_COMMENT(enum) regex::ReOp::Plus));
                            das_copy((__mks_1187.id),(23));
                            das_copy((__mks_1187.at),(range(34,46)));
                            das_copy((__mks_1187.text),(nullptr));
                            das_copy((__mks_1187.textLen),(0));
                            das_move((__mks_1187.all),((([&]() -> TArray<regex::ReNode *> {
                                TArray<regex::ReNode *> __mks_1187;
                                das_zero(__mks_1187);
                                return __mks_1187;
                            })())));
                            das_copy((__mks_1187.left),(nullptr));
                            das_copy((__mks_1187.right),(nullptr));
                            das_copy((__mks_1187.subexpr),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                                regex::ReNode __mks_1187;
                                das_zero(__mks_1187);
                                das_copy((__mks_1187.op),(DAS_COMMENT(enum) regex::ReOp::Set));
                                das_copy((__mks_1187.id),(24));
                                das_copy((__mks_1187.at),(range(34,45)));
                                das_copy((__mks_1187.text),(nullptr));
                                das_copy((__mks_1187.textLen),(0));
                                das_move((__mks_1187.all),((([&]() -> TArray<regex::ReNode *> {
                                    TArray<regex::ReNode *> __mks_1187;
                                    das_zero(__mks_1187);
                                    return __mks_1187;
                                })())));
                                das_copy((__mks_1187.left),(nullptr));
                                das_copy((__mks_1187.right),(nullptr));
                                das_copy((__mks_1187.subexpr),(nullptr));
                                das_copy((__mks_1187.cset),((([&]() -> TDim<uint32_t,8> {
                                    TDim<uint32_t,8> __mka_1187;
                                    __mka_1187(0,__context__) = 0x0u;
                                    __mka_1187(1,__context__) = 0x3ff0000u;
                                    __mka_1187(2,__context__) = 0x7fffffeu;
                                    __mka_1187(3,__context__) = 0x7fffffeu;
                                    __mka_1187(4,__context__) = 0x0u;
                                    __mka_1187(5,__context__) = 0x0u;
                                    __mka_1187(6,__context__) = 0x0u;
                                    __mka_1187(7,__context__) = 0x0u;
                                    return __mka_1187;
                                })())));
                                das_copy((__mks_1187.index),(0));
                                return __mks_1187;
                            })()))));
                            das_copy((__mks_1187.cset),((([&]() -> TDim<uint32_t,8> {
                                TDim<uint32_t,8> __mka_1187;
                                __mka_1187(0,__context__) = 0x0u;
                                __mka_1187(1,__context__) = 0x3ff0000u;
                                __mka_1187(2,__context__) = 0x7fffffeu;
                                __mka_1187(3,__context__) = 0x7fffffeu;
                                __mka_1187(4,__context__) = 0x0u;
                                __mka_1187(5,__context__) = 0x0u;
                                __mka_1187(6,__context__) = 0x0u;
                                __mka_1187(7,__context__) = 0x0u;
                                return __mka_1187;
                            })())));
                            das_copy((__mks_1187.index),(0));
                            return __mks_1187;
                        })()))));
                        das_copy((__mks_1187.subexpr),(nullptr));
                        das_copy((__mks_1187.cset),((([&]() -> TDim<uint32_t,8> {
                            TDim<uint32_t,8> __mka_1187;
                            __mka_1187(0,__context__) = 0x0u;
                            __mka_1187(1,__context__) = 0x0u;
                            __mka_1187(2,__context__) = 0x0u;
                            __mka_1187(3,__context__) = 0x0u;
                            __mka_1187(4,__context__) = 0x0u;
                            __mka_1187(5,__context__) = 0x0u;
                            __mka_1187(6,__context__) = 0x0u;
                            __mka_1187(7,__context__) = 0x0u;
                            return __mka_1187;
                        })())));
                        das_copy((__mks_1187.index),(0));
                        return __mks_1187;
                    })()))));
                    das_copy((__mks_1187.cset),((([&]() -> TDim<uint32_t,8> {
                        TDim<uint32_t,8> __mka_1187;
                        __mka_1187(0,__context__) = 0x0u;
                        __mka_1187(1,__context__) = 0x0u;
                        __mka_1187(2,__context__) = 0x0u;
                        __mka_1187(3,__context__) = 0x0u;
                        __mka_1187(4,__context__) = 0x0u;
                        __mka_1187(5,__context__) = 0x0u;
                        __mka_1187(6,__context__) = 0x0u;
                        __mka_1187(7,__context__) = 0x0u;
                        return __mka_1187;
                    })())));
                    das_copy((__mks_1187.index),(2));
                    return __mks_1187;
                })()))));
                das_copy((__mks_1187.subexpr),(nullptr));
                das_copy((__mks_1187.cset),((([&]() -> TDim<uint32_t,8> {
                    TDim<uint32_t,8> __mka_1187;
                    __mka_1187(0,__context__) = 0x0u;
                    __mka_1187(1,__context__) = 0x0u;
                    __mka_1187(2,__context__) = 0x0u;
                    __mka_1187(3,__context__) = 0x0u;
                    __mka_1187(4,__context__) = 0x0u;
                    __mka_1187(5,__context__) = 0x0u;
                    __mka_1187(6,__context__) = 0x0u;
                    __mka_1187(7,__context__) = 0x0u;
                    return __mka_1187;
                })())));
                das_copy((__mks_1187.index),(0));
                return __mks_1187;
            })()))));
            das_copy((__mks_1187.right),(das_ascend<regex::ReNode,regex::ReNode>::make(__context__,nullptr,(([&]() -> regex::ReNode {
                regex::ReNode __mks_1187;
                das_zero(__mks_1187);
                das_copy((__mks_1187.op),(DAS_COMMENT(enum) regex::ReOp::Char));
                das_copy((__mks_1187.id),(25));
                das_copy((__mks_1187.at),(range(47,49)));
                das_copy((__mks_1187.text),(((char *) ")")));
                das_copy((__mks_1187.textLen),(1));
                das_move((__mks_1187.all),((([&]() -> TArray<regex::ReNode *> {
                    TArray<regex::ReNode *> __mks_1187;
                    das_zero(__mks_1187);
                    return __mks_1187;
                })())));
                das_copy((__mks_1187.left),(nullptr));
                das_copy((__mks_1187.right),(nullptr));
                das_copy((__mks_1187.subexpr),(nullptr));
                das_copy((__mks_1187.cset),((([&]() -> TDim<uint32_t,8> {
                    TDim<uint32_t,8> __mka_1187;
                    __mka_1187(0,__context__) = 0x0u;
                    __mka_1187(1,__context__) = 0x0u;
                    __mka_1187(2,__context__) = 0x0u;
                    __mka_1187(3,__context__) = 0x0u;
                    __mka_1187(4,__context__) = 0x0u;
                    __mka_1187(5,__context__) = 0x0u;
                    __mka_1187(6,__context__) = 0x0u;
                    __mka_1187(7,__context__) = 0x0u;
                    return __mka_1187;
                })())));
                das_copy((__mks_1187.index),(0));
                return __mks_1187;
            })()))));
            das_copy((__mks_1187.subexpr),(nullptr));
            das_copy((__mks_1187.cset),((([&]() -> TDim<uint32_t,8> {
                TDim<uint32_t,8> __mka_1187;
                __mka_1187(0,__context__) = 0x0u;
                __mka_1187(1,__context__) = 0x0u;
                __mka_1187(2,__context__) = 0x0u;
                __mka_1187(3,__context__) = 0x0u;
                __mka_1187(4,__context__) = 0x0u;
                __mka_1187(5,__context__) = 0x0u;
                __mka_1187(6,__context__) = 0x0u;
                __mka_1187(7,__context__) = 0x0u;
                return __mka_1187;
            })())));
            das_copy((__mks_1187.index),(0));
            return __mks_1187;
        })()))));
        das_move((_temp_make_local_1187_26_2928.groups),(_FuncbuiltinTickto_array_moveTick3185538323411982277_71fbdf75ac58194(__context__,das_arg<TDim<AutoTuple<range,char *>,3>>::pass((([&]() -> TDim<AutoTuple<range,char *>,3>& {
            _temp_make_local_1187_26_3424(0,__context__) = (([&]() -> AutoTuple<range,char *> {
                AutoTuple<range,char *> __mkt_1187;
                das_get_auto_tuple_field<range,0,range,char *>::get(__mkt_1187) = range(0,0);
                das_get_auto_tuple_field<char *,1,range,char *>::get(__mkt_1187) = nullptr;
                return __mkt_1187;
            })());
            _temp_make_local_1187_26_3424(1,__context__) = (([&]() -> AutoTuple<range,char *> {
                AutoTuple<range,char *> __mkt_1187;
                das_get_auto_tuple_field<range,0,range,char *>::get(__mkt_1187) = range(0,0);
                das_get_auto_tuple_field<char *,1,range,char *>::get(__mkt_1187) = ((char *) "1");
                return __mkt_1187;
            })());
            _temp_make_local_1187_26_3424(2,__context__) = (([&]() -> AutoTuple<range,char *> {
                AutoTuple<range,char *> __mkt_1187;
                das_get_auto_tuple_field<range,0,range,char *>::get(__mkt_1187) = range(0,0);
                das_get_auto_tuple_field<char *,1,range,char *>::get(__mkt_1187) = ((char *) "2");
                return __mkt_1187;
            })());
            return _temp_make_local_1187_26_3424;
        })())))));
        das_copy((_temp_make_local_1187_26_2928.earlyOut),((([&]() -> TDim<uint32_t,8> {
            TDim<uint32_t,8> __mka_1187;
            __mka_1187(0,__context__) = 0x0u;
            __mka_1187(1,__context__) = 0x8u;
            __mka_1187(2,__context__) = 0x0u;
            __mka_1187(3,__context__) = 0x0u;
            __mka_1187(4,__context__) = 0x0u;
            __mka_1187(5,__context__) = 0x0u;
            __mka_1187(6,__context__) = 0x0u;
            __mka_1187(7,__context__) = 0x0u;
            return __mka_1187;
        })())));
        das_copy((_temp_make_local_1187_26_2928.canEarlyOut),(true));
        return _temp_make_local_1187_26_2928;
    })()))));
    _FuncfioTickfopenTick3937565566638487747_feeb71cd63fd8eac(__context__,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_105, cast<char * const >::from(__prefix_rename_at_1177_345), cast<char * const >::from(((char *) ".const_inc")))),((char *) "wb"),das_make_block<void,FILE const  * const >(__context__,3520,0,&__func_info__94a6c10524a0a34c,[&](FILE const  * const  __hf_rename_at_1188_353) -> void{
        if ( __hf_rename_at_1188_353 == nullptr )
        {
            builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_106, cast<char * const >::from(((char *) "can't write ")), cast<char * const >::from(__prefix_rename_at_1177_345), cast<char * const >::from(((char *) ".const_inc")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        {
            bool __need_loop_1192 = true;
            // fname: string const&
            das_iterator<TArray<char *> const > __fname_iterator(__fnames_rename_at_1177_344);
            char * const  * __fname_rename_at_1192_354;
            __need_loop_1192 = __fname_iterator.first(__context__,(__fname_rename_at_1192_354)) && __need_loop_1192;
            for ( ; __need_loop_1192 ; __need_loop_1192 = __fname_iterator.next(__context__,(__fname_rename_at_1192_354)) )
            {
                _FuncfioTickfopenTick3937565566638487747_feeb71cd63fd8eac(__context__,(*__fname_rename_at_1192_354),((char *) "rb"),das_make_block<void,FILE const  * const >(__context__,3600,0,&__func_info__ddb31f01d5771c19,[&](FILE const  * const  __f_rename_at_1193_355) -> void{
                    if ( __f_rename_at_1193_355 == nullptr )
                    {
                        builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_107, cast<char * const >::from(((char *) "can't open ")), cast<char * const >::from((*__fname_rename_at_1192_354)))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    };
                    TTable<int32_t,bool> __ofs_rename_at_1197_356;das_zero(__ofs_rename_at_1197_356);
                    char * __data_rename_at_1198_357 = ((char *)(char *)(((char * const )(builtin_fread(__f_rename_at_1193_355,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                    TTable<char *,bool> __dup_rename_at_1199_358;das_zero(__dup_rename_at_1199_358);
                    searchAndGenConst_ec8cac3d091d1a8(__context__,das_arg<regex::Regex>::pass(__reg_def_hex_rename_at_1182_347),((char *) "uint32_t"),((char *) "u"),das_arg<TTable<int32_t,bool>>::pass(__ofs_rename_at_1197_356),__data_rename_at_1198_357,das_arg<TTable<char *,bool>>::pass(__dup_rename_at_1199_358),__hf_rename_at_1188_353);
                    searchAndGenConst_ec8cac3d091d1a8(__context__,das_arg<regex::Regex>::pass(__reg_def_dec_rename_at_1183_348),((char *) "int32_t"),nullptr,das_arg<TTable<int32_t,bool>>::pass(__ofs_rename_at_1197_356),__data_rename_at_1198_357,das_arg<TTable<char *,bool>>::pass(__dup_rename_at_1199_358),__hf_rename_at_1188_353);
                    searchAndGenConst_ec8cac3d091d1a8(__context__,das_arg<regex::Regex>::pass(__reg_def_UINT8_rename_at_1184_349),das_global<char *,0x26b521e3cb75743>(__context__) /*const_uint8_type*/,((char *) "u"),das_arg<TTable<int32_t,bool>>::pass(__ofs_rename_at_1197_356),__data_rename_at_1198_357,das_arg<TTable<char *,bool>>::pass(__dup_rename_at_1199_358),__hf_rename_at_1188_353);
                    searchAndGenConst_ec8cac3d091d1a8(__context__,das_arg<regex::Regex>::pass(__reg_def_UINT16_rename_at_1185_350),das_global<char *,0x27b3ae2f4cd224dd>(__context__) /*const_uint16_type*/,((char *) "u"),das_arg<TTable<int32_t,bool>>::pass(__ofs_rename_at_1197_356),__data_rename_at_1198_357,das_arg<TTable<char *,bool>>::pass(__dup_rename_at_1199_358),__hf_rename_at_1188_353);
                    searchAndGenConst_ec8cac3d091d1a8(__context__,das_arg<regex::Regex>::pass(__reg_def_UINT32_rename_at_1186_351),((char *) "uint32_t"),((char *) "u"),das_arg<TTable<int32_t,bool>>::pass(__ofs_rename_at_1197_356),__data_rename_at_1198_357,das_arg<TTable<char *,bool>>::pass(__dup_rename_at_1199_358),__hf_rename_at_1188_353);
                    searchAndGenConst_ec8cac3d091d1a8(__context__,das_arg<regex::Regex>::pass(__reg_def_UINT64_rename_at_1187_352),((char *) "uint64_t"),((char *) "ul"),das_arg<TTable<int32_t,bool>>::pass(__ofs_rename_at_1197_356),__data_rename_at_1198_357,das_arg<TTable<char *,bool>>::pass(__dup_rename_at_1199_358),__hf_rename_at_1188_353);
                }));
            }
            __fname_iterator.close(__context__,(__fname_rename_at_1192_354));
        };
    }));
}}

inline cpp_gen::ExEnum ExEnum_93a02f0673e373b0 ( Context * __context__ )
{
    return /* <- */ das_auto_cast_move<cpp_gen::ExEnum>::cast((([&]() -> cpp_gen::ExEnum {
        cpp_gen::ExEnum __mks_24;
        das_zero(__mks_24);
        das_copy((__mks_24.removePrefix),(true));
        das_copy((__mks_24.isFlags),(false));
        return __mks_24;
    })()));
}

inline TArray<char *> _FuncbuiltinTickto_array_moveTick3185538323411982277_983e7a1ec42b37c0 ( Context * __context__, TDim<char *,4> &  __a_rename_at_1342_359 )
{
    TArray<char *> __arr_rename_at_1344_360;das_zero(__arr_rename_at_1344_360);
    _FuncbuiltinTickresizeTick4811697762258667383_14682004b38ad2a1(__context__,das_arg<TArray<char *>>::pass(__arr_rename_at_1344_360),4);
    das_copy(das_cast<TDim<char *,4>>::cast(das_ref(__context__,__arr_rename_at_1344_360(0,__context__))),__a_rename_at_1342_359);
    return /* <- */ das_auto_cast_move<TArray<char *>>::cast(__arr_rename_at_1344_360);
}

inline TArray<char *> _FuncbuiltinTickto_array_moveTick3185538323411982277_d6a74d6804670a19 ( Context * __context__, TDim<char *,2> &  __a_rename_at_1342_361 )
{
    TArray<char *> __arr_rename_at_1344_362;das_zero(__arr_rename_at_1344_362);
    _FuncbuiltinTickresizeTick4811697762258667383_14682004b38ad2a1(__context__,das_arg<TArray<char *>>::pass(__arr_rename_at_1344_362),2);
    das_copy(das_cast<TDim<char *,2>>::cast(das_ref(__context__,__arr_rename_at_1344_362(0,__context__))),__a_rename_at_1342_361);
    return /* <- */ das_auto_cast_move<TArray<char *>>::cast(__arr_rename_at_1344_362);
}

inline TArray<AutoTuple<range,char *>> _FuncbuiltinTickto_array_moveTick3185538323411982277_a9d6d54733762ba ( Context * __context__, TDim<AutoTuple<range,char *>,1> &  __a_rename_at_1342_363 )
{
    TArray<AutoTuple<range,char *>> __arr_rename_at_1344_364;das_zero(__arr_rename_at_1344_364);
    _FuncbuiltinTickresizeTick4811697762258667383_401f694502683cef(__context__,das_arg<TArray<AutoTuple<range,char *>>>::pass(__arr_rename_at_1344_364),1);
    das_copy(das_cast<TDim<AutoTuple<range,char *>,1>>::cast(das_ref(__context__,__arr_rename_at_1344_364(0,__context__))),__a_rename_at_1342_363);
    return /* <- */ das_auto_cast_move<TArray<AutoTuple<range,char *>>>::cast(__arr_rename_at_1344_364);
}

inline TArray<AutoTuple<range,char *>> _FuncbuiltinTickto_array_moveTick3185538323411982277_71fbdf75ac58194 ( Context * __context__, TDim<AutoTuple<range,char *>,3> &  __a_rename_at_1342_365 )
{
    TArray<AutoTuple<range,char *>> __arr_rename_at_1344_366;das_zero(__arr_rename_at_1344_366);
    _FuncbuiltinTickresizeTick4811697762258667383_401f694502683cef(__context__,das_arg<TArray<AutoTuple<range,char *>>>::pass(__arr_rename_at_1344_366),3);
    das_copy(das_cast<TDim<AutoTuple<range,char *>,3>>::cast(das_ref(__context__,__arr_rename_at_1344_366(0,__context__))),__a_rename_at_1342_365);
    return /* <- */ das_auto_cast_move<TArray<AutoTuple<range,char *>>>::cast(__arr_rename_at_1344_366);
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x51ab49e441dabce2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_c777430705b7c258>>();
    };
    aotLib[0x3d220d20a9d11ae0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_e79b4e2307f7ae1c>>();
    };
    aotLib[0xd15841113539eb4a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funccpp_genTickonNumericLiteralTick10275515307319155835_73739de28d614e90>>();
    };
    aotLib[0xde6e54abcc6b70c2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_5a77afdd4cbd09e1>>();
    };
    aotLib[0x84a50acbe8dccac2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_at_with_lockcheckTick7807051423786862253_68d1e221d9f9bb60>>();
    };
    aotLib[0x3c395992954dd60d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick14133213201864676143_d626258b591934af>>();
    };
    aotLib[0xff5c322e95e1d464] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_8a84dcb6bb73936b>>();
    };
    aotLib[0xfc84acb5b4365936] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funccpp_genTickonInitArgTick7456162652026714814_980c8ded9aae7f9d>>();
    };
    aotLib[0xb220b0ac7f799284] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_cfad5be86fc7ec48>>();
    };
    aotLib[0xcf7473f96b95ba79] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickemplaceTick13923705686753630697_b7c430019eafb474>>();
    };
    aotLib[0x662080ca29efcfa9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Func_lambda_cpp_gen_543_2Tickfunction_10be92d9c230e7ac>>();
    };
    aotLib[0xa3b06b3c16abb1a2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Func_lambda_cpp_gen_543_2Tickfinalizer_67e83694801db280>>();
    };
    aotLib[0x802c576108eaea4e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickeachTick6002865651812066953_ab768a47fa5edfe2>>();
    };
    aotLib[0xe473f497a092ecde] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcstrings_boostTickjoinTick17792841289284275598_f97bd9e0a3ac85c9>>();
    };
    aotLib[0x17c8cabbf58326b8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_ca4ff32f655e0ba8>>();
    };
    aotLib[0xfd82d3d9f5c42bcf] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_7e2d4453a57c7761>>();
    };
    aotLib[0xfbc5ea08992afdd7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickeach_refTick16137060296763333387_fc747e0a786c7774>>();
    };
    aotLib[0xccbbf2b150b08608] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_3023c4bc268c52d7>>();
    };
    aotLib[0xa13291a4654c583f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_at_with_lockcheckTick7807051423786862253_81b03a6fcc783e7f>>();
    };
    aotLib[0xfcbd75438f3ad0c5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_75085f7298948768>>();
    };
    aotLib[0xbf662ac476af9afa] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_9f21c57cd4b4c83>>();
    };
    aotLib[0xcf09f8eff7245765] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickemplaceTick13923705686753630697_650e65a9cf1ca823>>();
    };
    aotLib[0x1a87893f73e95b9b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funccpp_genTickgetCppNameTick2578297908749988033_7a531127a37dc761>>();
    };
    aotLib[0xaf1ca58ee0134c27] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Func_lambda_cpp_gen_266_1Tickfunction_8e17cb5a29f7eaff>>();
    };
    aotLib[0xbd0c556c458497d9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Func_lambda_cpp_gen_266_1Tickfinalizer_bbb8c1e2c3d5dafe>>();
    };
    aotLib[0x324b175ee9535838] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_at_with_lockcheckTick7807051423786862253_9ef1c864c9d2f80>>();
    };
    aotLib[0x7f6888a99cd3a650] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funccpp_genTickis_valid_locTick10808118537805394975_6bace0942c51e77f>>();
    };
    aotLib[0xba5998beb461511c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick14133213201864676143_f626dd014072c607>>();
    };
    aotLib[0x6d8b4d5a95bb40bc] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_440dfee5ce51ee9b>>();
    };
    aotLib[0x56c0b4f06a2c06e0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpopTick1161079256290593740_76e5d0bb5a9d86cc>>();
    };
    aotLib[0xbebf081d355db03a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpopTick1161079256290593740_15fc7a604b10573f>>();
    };
    aotLib[0xd664e8bab3152fb2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpopTick1161079256290593740_9fb9082827455aec>>();
    };
    aotLib[0xd2971ee4f6216981] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funccpp_genTickgetQualTypeTick1400640308412000890_a58540215c7d0fa9>>();
    };
    aotLib[0x8b5725da62d902c1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funccpp_genTickisBitfieldTick9047106797565157337_d37f3cd228cd993f>>();
    };
    aotLib[0x838c1efad4d065d6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_Funccpp_genTickonFunctionBodyTick983279341569892450_8caee9a44ff10098>>();
    };
    aotLib[0x17c076c38e276651] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcstrings_boostTickjoinTick16475640899284277631_97cbd29033d05835>>();
    };
    aotLib[0x65d0d1d606e714f1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickkey_existsTick16808803843923989214_963e1e05a7a36522>>();
    };
    aotLib[0xafe183a48c7fccc8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funccpp_genTickwith_objectTick7594167909273532910_43fb3bcbe18cb920>>();
    };
    aotLib[0x7111b3c2a927bb0c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funccpp_genTickonStructTick15011585540411908992_8d9737e4ce0fff22>>();
    };
    aotLib[0xb600c07fb02754ce] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funccpp_genTickonNamespaceTick1569694100191165612_31878f48597dada2>>();
    };
    aotLib[0x2cd53f5c9a248283] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funccpp_genTickonFieldTick11712683440231282153_f030e55d3f2f1f92>>();
    };
    aotLib[0x71e5b280a363274c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funccpp_genTickonFunctionTick9237376346942193985_2dab482eabe1f2c3>>();
    };
    aotLib[0x6dcfcbcae29f7804] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funccpp_genTickonMethodTick5355858089746197764_68eed9a736f85c63>>();
    };
    aotLib[0x6b8bdeb1738a288a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funccpp_genTickonEnumTick2734318481451533008_f3a14439e6d126ab>>();
    };
    aotLib[0xece6131182df2409] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funccpp_genTickonVarTick7763998161592485260_1b4b388a8d41c569>>();
    };
    aotLib[0xcef6ba2792aa4843] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funccpp_genTickonTypeDeclTick13685927393030072381_48d18a41d3948b70>>();
    };
    aotLib[0xb6eceb3f3827ce7c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funccpp_genTickonAccessTick18228828068813142789_a461a7e898d3234f>>();
    };
    aotLib[0x333d7f672ecdab61] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funccpp_genTickonLinkageSpecTick9588560724416692279_347cf8f7a9c1d56>>();
    };
    aotLib[0x4f90e655a429a088] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funccpp_genTickwith_arrayTick6272002616190333845_c90f8a054aaf2ef6>>();
    };
    aotLib[0x9fa519969e6ec567] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funccpp_genTickis_strTick14919838023837841642_48d382d152e59de8>>();
    };
    aotLib[0xcd9dacfd7f9d78bd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funccpp_genTickonInnerTick15341579017765610681_62ed76bf0329fb0d>>();
    };
    aotLib[0x2e33de26741ed358] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_4ca5259fa9e9c74e>>();
    };
    aotLib[0x380b7455b0a2f9f5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_14682004b38ad2a1>>();
    };
    aotLib[0xad3d37acbc8291d6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_401f694502683cef>>();
    };
    aotLib[0x99a0e7c1a4bd28e5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_ddf4f34775c3f6ad>>();
    };
    aotLib[0x1d18c10ccd1f35e1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_30458db6e3c9dc4b>>();
    };
    aotLib[0xd36bb4a9944e301b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_3a6bd4fafb6564b7>>();
    };
    aotLib[0xa5b370d581bd401a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_d57f120a5eb06ce3>>();
    };
    aotLib[0x25de1cef3a46999b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncfioTickfreadTick9799048084303762322_c82e6deab5e5b700>>();
    };
    aotLib[0x45ce295307dcac74] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickkey_existsTick16808803843923989214_9d11994217d1865a>>();
    };
    aotLib[0x65f7bf9671b6f2e5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickkey_existsTick16808803843923989214_9b8b56716258076a>>();
    };
    aotLib[0x8b311fbc51669bc8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickgetTick8447005936052527643_c25c472f7c63dd94>>();
    };
    aotLib[0x96e9f325616374f8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funccpp_genTickneedToGenStructTick14158910623600840251_fd93d910f98eae6b>>();
    };
    aotLib[0x15a11bdfd7532172] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickkey_existsTick16808803843923989214_1a54ecb7273c2da1>>();
    };
    aotLib[0x7d6285ee3c6dbb6f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funccpp_genTickneedToGenFieldTick2521814960739047210_6a0d55d67bdff534>>();
    };
    aotLib[0x532c01a28b48288d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funccpp_genTickisBlockedFunTick2478178585869301773_97ae78cd67b212a3>>();
    };
    aotLib[0x8ff205c681819963] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_354a9a0b57d554a0>>();
    };
    aotLib[0x78948a08f0d98f51] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_79f9e841b4434fd8>>();
    };
    aotLib[0x19581459265363be] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick5454204887383796109_9c4083566563a85a>>();
    };
    aotLib[0xfd32f4876dfc3cd4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickto_array_moveTick3185538323411982277_9285ff1255c3d503>>();
    };
    aotLib[0x73b538cf0a27b77] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1351216622833168869_93f4fc8357f87453>>();
    };
    aotLib[0x646189d0845a9472] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1351216622833168869_a5f65aa1a7088367>>();
    };
    aotLib[0xe4cc4ae333785120] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickgetTick8447005936052527643_30d18a39e61d48bc>>();
    };
    aotLib[0x4d1e9a9e9c837f06] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1351216622833168869_7c0785e58fa9645f>>();
    };
    aotLib[0xeefb3e272bbfb4a4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickkeysTick2205854368403803976_2df91d3f4a5c8888>>();
    };
    aotLib[0xa97bafe67041f2ed] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1351216622833168869_16866e32c6607afb>>();
    };
    aotLib[0x72e5a364716a8768] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funccpp_genTickqType2FnTypeClassTick18208979152004666060_cdb79d2da470a00b>>();
    };
    aotLib[0x595d7aad1c94a315] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funccpp_genTickqType2FnTypeTick1482691952773259010_a02b5143c6271bc3>>();
    };
    aotLib[0x70d697cf2073c201] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funccpp_genTickisRefFunTick11632776912375340809_d4070b3dd9da7cd2>>();
    };
    aotLib[0x3a3877024bfcd901] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funccpp_genTickisCmresFunTick16266856454814809519_86372ea3375f1a8c>>();
    };
    aotLib[0xbefccccb4901587d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickkeysTick2205854368403803976_bcec88c808d50381>>();
    };
    aotLib[0xeab0a4f2d29c423a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1351216622833168869_7282d82fa9dd61d5>>();
    };
    aotLib[0x7c2e30b4091b7513] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncfioTickfopenTick3937565566638487747_feeb71cd63fd8eac>>();
    };
    aotLib[0x81618f554fa1a67b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickkey_existsTick16808803843923989214_efda835c0359ec76>>();
    };
    aotLib[0x31fe7314b59bda48] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&isBlockedType_cc1ed1c7968e329c>>();
    };
    aotLib[0x797f4a7d0557659b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&isLocalType_36973d575e8241e5>>();
    };
    aotLib[0xf1a17c6ad744140e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&genEnums_816c948d88e68308>>();
    };
    aotLib[0x12cc1014b81af4e8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&getBindFldName_4490111cebfe248e>>();
    };
    aotLib[0xbb44f5e4a2ef3770] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&genStructs_e7c6b33a2f752b8e>>();
    };
    aotLib[0x91aa16a70960f01] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&methodName_ff1251eb1b75eec2>>();
    };
    aotLib[0xb0c447dab77d8044] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&openSplitFile_ea32c746c410d324>>();
    };
    aotLib[0xc1225911e4d975e7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&closeSplitFile_233b8cc7d4e11a96>>();
    };
    aotLib[0xf9b180ab485ac447] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&getFunctions_26873a00d281aa69>>();
    };
    aotLib[0x334caedd9bcf28b0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&parseQualArgs_3f5a273f6933fa8f>>();
    };
    aotLib[0x3b2381f19cbb2f62] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&getDefinePrefix_68940f3e2e3e9e89>>();
    };
    aotLib[0x9538568bfcb49853] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&genFunction_d3eae2156dccabbe>>();
    };
    aotLib[0xb0bcef0b63c7296b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&genBindings_6b7b54142f4c772b>>();
    };
    aotLib[0xf1548d62afbd3d69] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&bindHeaders_5b0cabfc50efc6fe>>();
    };
    aotLib[0xa1a3a5bba3e6a1f2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&genDefineConstants_9a873908595aec49>>();
    };
    aotLib[0xe78cd469aa0eca13] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&getGenConstType_ba5129bb3b413133>>();
    };
    aotLib[0x1a042eec7d8dc677] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&searchAndGenConst_ec8cac3d091d1a8>>();
    };
    aotLib[0xa9f99a383ad1faf9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&genDefineConstants_7bfbc66c3d4f2529>>();
    };
    aotLib[0xab4fd3dd83def1d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&ExEnum_93a02f0673e373b0>>();
    };
    aotLib[0x8659587e57401db2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickto_array_moveTick3185538323411982277_983e7a1ec42b37c0>>();
    };
    aotLib[0x95171c35a9e75353] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickto_array_moveTick3185538323411982277_d6a74d6804670a19>>();
    };
    aotLib[0xad9ad1fe0ea5cee] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickto_array_moveTick3185538323411982277_a9d6d54733762ba>>();
    };
    aotLib[0xe359b089e1f06bc8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickto_array_moveTick3185538323411982277_71fbdf75ac58194>>();
    };
    // [[ init script ]]
    aotLib[0x3b7f5b8e605b3963] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<AutoSimNode_Aot<&__init_script>>();
        return ctx.aotInitScript;
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_4947418327762536717
AotListBase impl_aot_cpp_gen(_anon_4947418327762536717::registerAotFunctions);
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
