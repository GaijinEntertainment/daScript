#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
 // require math
#include "daScript/simulate/aot_builtin_math.h"
 // require strings
#include "daScript/simulate/aot_builtin_string.h"
 // require rtti
#include "daScript/simulate/aot_builtin_rtti.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_handle.h"
 // require ast
#include "daScript/ast/ast.h"
#include "daScript/simulate/aot_builtin_ast.h"
#include "daScript/ast/ast_generate.h"
 // require debugapi
#include "daScript/simulate/aot_builtin_debugger.h"
 // require strings_boost
 // require ast_boost
 // require templates
 // require contracts
 // require array_boost
 // require algorithm
 // require templates_boost
 // require macro_boost
 // require defer
 // require match
 // require math_bits
 // require debug_eval

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
namespace _anon_12798696619525958528 {

namespace debug_eval { struct Result; };
namespace debug_eval { struct TokenStream; };
namespace debug_eval { struct _lambda_debug_eval_26_1; };
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
namespace debugapi { struct DapiDebugAgent; };
namespace debugapi { struct DapiArray; };
namespace debugapi { struct DapiTable; };
namespace debugapi { struct DapiBlock; };
namespace debugapi { struct DapiFunc; };
namespace debugapi { struct DapiLambda; };
namespace debugapi { struct DapiSequence; };
namespace debugapi { struct DapiDataWalker; };
namespace debugapi { struct DapiStackWalker; };
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
namespace match { struct MatchError; };
namespace match { struct MatchTo; };
namespace match { struct MatchMacro; };
namespace match { struct StaticMatchMacro; };
namespace match { struct MultiMatchMacro; };
namespace match { struct StaticMultiMatchMacro; };
namespace match { struct MatchAsIs; };
namespace match { struct MatchCopy; };
// unused enumeration ConversionResult
// unused enumeration CompilationError
// unused enumeration ConstMatters
// unused enumeration RefMatters
// unused enumeration TemporaryMatters
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
// unused structure DapiDebugAgent
namespace debugapi {

struct DapiArray {
    void * data;
    uint32_t size;
    uint32_t capacity;
    uint32_t lock;
    Bitfield flags;
};
}
namespace debugapi {

struct DapiTable {
    void * data;
    uint32_t size;
    uint32_t capacity;
    uint32_t lock;
    Bitfield flags;
    void * keys;
    uint32_t * hashes;
};
}
// unused structure DapiBlock
// unused structure DapiFunc
// unused structure DapiLambda
// unused structure DapiSequence
// unused structure DapiDataWalker
// unused structure DapiStackWalker
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
// unused enumeration MatchType
// unused structure MatchError
// unused structure MatchTo
// unused structure MatchMacro
// unused structure StaticMatchMacro
// unused structure MultiMatchMacro
// unused structure StaticMultiMatchMacro
// unused structure MatchAsIs
// unused structure MatchCopy
namespace debug_eval {

struct Result {
    TypeInfo tinfo;
    float4 value;
    void const  * data;
    char * error;
};
}
namespace debug_eval {

struct TokenStream {
    Sequence DAS_COMMENT((AutoVariant<int64_t,int32_t,char *,char *,char *>)) tokens;
    TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>> stream;
    TArray<char *> errors;
    TTable<char *,debug_eval::Result> names;
};
}
namespace debug_eval {

struct _lambda_debug_eval_26_1 {
    Func DAS_COMMENT((bool,debug_eval::_lambda_debug_eval_26_1,AutoVariant<int64_t,int32_t,char *,char *,char *>)) __lambda;
    Func DAS_COMMENT((void,debug_eval::_lambda_debug_eval_26_1 *)) __finalize;
    int32_t __yield;
    char * st;
    char * tst;
    Sequence DAS_COMMENT((int32_t)) chars;
    int32_t ahead;
    TArray<uint8_t> __str_rename_at_42_20;
    TArray<uint8_t> __str_rename_at_57_20;
};
}
extern StructInfo __struct_info__bd90204d3de3945;
extern StructInfo __struct_info__acf09986cbe796dd;
extern StructInfo __struct_info__5c760594c2c1ce52;
extern TypeInfo __type_info__1822028605008c6c;
extern TypeInfo __type_info__51bb14dd602168c2;
extern TypeInfo __type_info__7a089ab9e22fd5f;
extern TypeInfo __type_info__4fd1011fade7876f;
extern TypeInfo __type_info__af81fe4c86352052;
extern TypeInfo __type_info__af8afe4c86446b52;
extern TypeInfo __type_info__8d8d8008262e16ec;
extern TypeInfo __type_info__af90fe4c864e9d52;
extern TypeInfo __type_info__af96fe4c8658cf52;
extern TypeInfo __type_info__af95fe4c86571c52;
extern TypeInfo __type_info__a7159d402feecb0a;
extern TypeInfo __type_info__203fbf2d6d7ad716;
extern TypeInfo __type_info__64e2d0c6d216e20d;
extern TypeInfo __type_info__f8bbdc9319696978;
extern TypeInfo __type_info__af63e44c8601fa24;
extern TypeInfo __type_info__d922fe078cefab30;
extern TypeInfo __type_info__af63ee4c86020b22;
extern TypeInfo __type_info__af5be84c85f468f0;
extern TypeInfo __type_info__af63eb4c86020609;
extern VarInfo __var_info__f388c6a2fc596964;
extern VarInfo __var_info__312bbc9c4c7cd171;
extern VarInfo __var_info__86dad552ee0e6c29;
extern VarInfo __var_info__8421d25d4efe44c8;
extern VarInfo __var_info__e0bed6337d486f47;
extern VarInfo __var_info__ddc1b5e9e4d20aaa;
extern VarInfo __var_info__71caf94fe7458508;
extern VarInfo __var_info__485520cdbba56729;
extern VarInfo __var_info__7205e8a10f980068;
extern VarInfo __var_info__6362ab4c2cfcea07;
extern VarInfo __var_info__6391bed0a86864a0;
extern VarInfo __var_info__9aaba7e91e30058b;
extern VarInfo __var_info__b280c6258bfe0c24;
extern VarInfo __var_info__5f2fbded7d6eabf5;
extern VarInfo __var_info__9a31f8edaaa938a6;
extern VarInfo __var_info__379ccd50c8d67a9a;
extern VarInfo __var_info__66d8d04552d43f9d;
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
VarInfo __struct_info__bd90204d3de3945_field_0 =  { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~rtti::TypeInfo"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24590, TypeSize<TypeInfo>::size, UINT64_C(0x86dad552ee0e6c29), "tinfo", offsetof(debug_eval::Result,tinfo), 2 };
VarInfo __struct_info__bd90204d3de3945_field_1 =  { Type::tFloat4, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<float4>::size, UINT64_C(0x8421d25d4efe44c8), "value", offsetof(debug_eval::Result,value), 0 };
VarInfo __struct_info__bd90204d3de3945_field_2 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af95fe4c86571c52, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, TypeSize<void const  *>::size, UINT64_C(0xf388c6a2fc596964), "data", offsetof(debug_eval::Result,data), 3 };
VarInfo __struct_info__bd90204d3de3945_field_3 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, TypeSize<char *>::size, UINT64_C(0x312bbc9c4c7cd171), "error", offsetof(debug_eval::Result,error), 4 };
VarInfo * __struct_info__bd90204d3de3945_fields[4] =  { &__struct_info__bd90204d3de3945_field_0, &__struct_info__bd90204d3de3945_field_1, &__struct_info__bd90204d3de3945_field_2, &__struct_info__bd90204d3de3945_field_3 };
StructInfo __struct_info__bd90204d3de3945 = {"Result", "debug_eval", 12, __struct_info__bd90204d3de3945_fields, 4, TypeSize<debug_eval::Result>::size, UINT64_C(0x0), nullptr, UINT64_C(0xbd90204d3de3945), 0 };
VarInfo __struct_info__acf09986cbe796dd_field_0 =  { Type::tIterator, nullptr, nullptr, nullptr, &__type_info__f8bbdc9319696978, nullptr, nullptr, nullptr, 0, 0, nullptr, 24602, TypeSize<Sequence DAS_COMMENT((AutoVariant<int64_t,int32_t,char *,char *,char *>))>::size, UINT64_C(0x485520cdbba56729), "tokens", offsetof(debug_eval::TokenStream,tokens), 1 };
VarInfo __struct_info__acf09986cbe796dd_field_1 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__f8bbdc9319696978, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>>>::size, UINT64_C(0x71caf94fe7458508), "stream", offsetof(debug_eval::TokenStream,stream), 2 };
VarInfo __struct_info__acf09986cbe796dd_field_2 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<TArray<char *>>::size, UINT64_C(0xe0bed6337d486f47), "errors", offsetof(debug_eval::TokenStream,errors), 3 };
VarInfo __struct_info__acf09986cbe796dd_field_3 =  { Type::tTable, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, &__type_info__203fbf2d6d7ad716, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<TTable<char *,debug_eval::Result>>::size, UINT64_C(0xddc1b5e9e4d20aaa), "names", offsetof(debug_eval::TokenStream,names), 4 };
VarInfo * __struct_info__acf09986cbe796dd_fields[4] =  { &__struct_info__acf09986cbe796dd_field_0, &__struct_info__acf09986cbe796dd_field_1, &__struct_info__acf09986cbe796dd_field_2, &__struct_info__acf09986cbe796dd_field_3 };
StructInfo __struct_info__acf09986cbe796dd = {"TokenStream", "debug_eval", 28, __struct_info__acf09986cbe796dd_fields, 4, TypeSize<debug_eval::TokenStream>::size, UINT64_C(0x0), nullptr, UINT64_C(0xacf09986cbe796dd), 0 };
TypeInfo * __type_info__6362ab4c2cfcea07_arg_types_var_6662518835222007378[2] = { &__type_info__64e2d0c6d216e20d, &__type_info__f8bbdc9319696978 };
const char * __type_info__6362ab4c2cfcea07_arg_names_var_6662518835222007378[2] = { "__this", "_yield_26" };
VarInfo __struct_info__5c760594c2c1ce52_field_0 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af81fe4c86352052, nullptr, (TypeInfo **)__type_info__6362ab4c2cfcea07_arg_types_var_6662518835222007378, __type_info__6362ab4c2cfcea07_arg_names_var_6662518835222007378, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,debug_eval::_lambda_debug_eval_26_1,AutoVariant<int64_t,int32_t,char *,char *,char *>))>::size, UINT64_C(0x6362ab4c2cfcea07), "__lambda", offsetof(debug_eval::_lambda_debug_eval_26_1,__lambda), 0 };
TypeInfo * __type_info__7205e8a10f980068_arg_types_var_6662518835222007378[1] = { &__type_info__1822028605008c6c };
const char * __type_info__7205e8a10f980068_arg_names_var_6662518835222007378[1] = { "__this" };
VarInfo __struct_info__5c760594c2c1ce52_field_1 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__7205e8a10f980068_arg_types_var_6662518835222007378, __type_info__7205e8a10f980068_arg_names_var_6662518835222007378, 1, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debug_eval::_lambda_debug_eval_26_1 *))>::size, UINT64_C(0x7205e8a10f980068), "__finalize", offsetof(debug_eval::_lambda_debug_eval_26_1,__finalize), 0 };
VarInfo __struct_info__5c760594c2c1ce52_field_2 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int32_t>::size, UINT64_C(0xb280c6258bfe0c24), "__yield", offsetof(debug_eval::_lambda_debug_eval_26_1,__yield), 0 };
VarInfo __struct_info__5c760594c2c1ce52_field_3 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, TypeSize<char *>::size, UINT64_C(0x379ccd50c8d67a9a), "st", offsetof(debug_eval::_lambda_debug_eval_26_1,st), 4 };
VarInfo __struct_info__5c760594c2c1ce52_field_4 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, TypeSize<char *>::size, UINT64_C(0x66d8d04552d43f9d), "tst", offsetof(debug_eval::_lambda_debug_eval_26_1,tst), 5 };
VarInfo __struct_info__5c760594c2c1ce52_field_5 =  { Type::tIterator, nullptr, nullptr, nullptr, &__type_info__af63e44c8601fa24, nullptr, nullptr, nullptr, 0, 0, nullptr, 24602, TypeSize<Sequence DAS_COMMENT((int32_t))>::size, UINT64_C(0x9a31f8edaaa938a6), "chars", offsetof(debug_eval::_lambda_debug_eval_26_1,chars), 7 };
VarInfo __struct_info__5c760594c2c1ce52_field_6 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int32_t>::size, UINT64_C(0x5f2fbded7d6eabf5), "ahead", offsetof(debug_eval::_lambda_debug_eval_26_1,ahead), 0 };
VarInfo __struct_info__5c760594c2c1ce52_field_7 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, TypeSize<TArray<uint8_t>>::size, UINT64_C(0x6391bed0a86864a0), "__str_rename_at_42_20", offsetof(debug_eval::_lambda_debug_eval_26_1,__str_rename_at_42_20), 8 };
VarInfo __struct_info__5c760594c2c1ce52_field_8 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, TypeSize<TArray<uint8_t>>::size, UINT64_C(0x9aaba7e91e30058b), "__str_rename_at_57_20", offsetof(debug_eval::_lambda_debug_eval_26_1,__str_rename_at_57_20), 9 };
VarInfo * __struct_info__5c760594c2c1ce52_fields[9] =  { &__struct_info__5c760594c2c1ce52_field_0, &__struct_info__5c760594c2c1ce52_field_1, &__struct_info__5c760594c2c1ce52_field_2, &__struct_info__5c760594c2c1ce52_field_3, &__struct_info__5c760594c2c1ce52_field_4, &__struct_info__5c760594c2c1ce52_field_5, &__struct_info__5c760594c2c1ce52_field_6, &__struct_info__5c760594c2c1ce52_field_7, &__struct_info__5c760594c2c1ce52_field_8 };
StructInfo __struct_info__5c760594c2c1ce52 = {"_lambda_debug_eval_26_1", "debug_eval", 30, __struct_info__5c760594c2c1ce52_fields, 9, TypeSize<debug_eval::_lambda_debug_eval_26_1>::size, UINT64_C(0x0), nullptr, UINT64_C(0x5c760594c2c1ce52), 3 };
TypeInfo __type_info__1822028605008c6c = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__64e2d0c6d216e20d, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, TypeSize<debug_eval::_lambda_debug_eval_26_1 *>::size, UINT64_C(0x1822028605008c6c) };
TypeInfo __type_info__51bb14dd602168c2 = { Type::tStructure, &__struct_info__acf09986cbe796dd, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<debug_eval::TokenStream>::size, UINT64_C(0x51bb14dd602168c2) };
TypeInfo * __type_info__7a089ab9e22fd5f_arg_types[5] = { &__type_info__d922fe078cefab30, &__type_info__af63e44c8601fa24, &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
const char * __type_info__7a089ab9e22fd5f_arg_names[5] = { "number", "punkt", "ident", "invalid", "eos" };
TypeInfo __type_info__7a089ab9e22fd5f = { Type::tVariant, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__7a089ab9e22fd5f_arg_types, __type_info__7a089ab9e22fd5f_arg_names, 5, 0, nullptr, 16678, TypeSize<AutoVariant<int64_t,int32_t,char *,char *,char *>>::size, UINT64_C(0x7a089ab9e22fd5f) };
TypeInfo __type_info__4fd1011fade7876f = { Type::tEnumeration, nullptr, &__enum_info__c897fe55afe7f727, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, TypeSize<DAS_COMMENT(bound_enum) das::Type>::size, UINT64_C(0x4fd1011fade7876f) };
TypeInfo __type_info__af81fe4c86352052 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, TypeSize<bool>::size, UINT64_C(0xaf81fe4c86352052) };
TypeInfo __type_info__af8afe4c86446b52 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, TypeSize<int32_t>::size, UINT64_C(0xaf8afe4c86446b52) };
TypeInfo __type_info__8d8d8008262e16ec = { Type::tInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, TypeSize<int64_t>::size, UINT64_C(0x8d8d8008262e16ec) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, TypeSize<char *>::size, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__af96fe4c8658cf52 = { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, TypeSize<uint32_t>::size, UINT64_C(0xaf96fe4c8658cf52) };
TypeInfo __type_info__af95fe4c86571c52 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, TypeSize<void>::size, UINT64_C(0xaf95fe4c86571c52) };
TypeInfo __type_info__a7159d402feecb0a = { Type::tEnumeration, nullptr, &__enum_info__c897fe55afe7f727, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<DAS_COMMENT(bound_enum) das::Type>::size, UINT64_C(0xa7159d402feecb0a) };
TypeInfo __type_info__203fbf2d6d7ad716 = { Type::tStructure, &__struct_info__bd90204d3de3945, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24582, TypeSize<debug_eval::Result>::size, UINT64_C(0x203fbf2d6d7ad716) };
TypeInfo __type_info__64e2d0c6d216e20d = { Type::tStructure, &__struct_info__5c760594c2c1ce52, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<debug_eval::_lambda_debug_eval_26_1>::size, UINT64_C(0x64e2d0c6d216e20d) };
TypeInfo * __type_info__f8bbdc9319696978_arg_types[5] = { &__type_info__d922fe078cefab30, &__type_info__af63e44c8601fa24, &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
const char * __type_info__f8bbdc9319696978_arg_names[5] = { "number", "punkt", "ident", "invalid", "eos" };
TypeInfo __type_info__f8bbdc9319696978 = { Type::tVariant, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__f8bbdc9319696978_arg_types, __type_info__f8bbdc9319696978_arg_names, 5, 0, nullptr, 16390, TypeSize<AutoVariant<int64_t,int32_t,char *,char *,char *>>::size, UINT64_C(0xf8bbdc9319696978) };
TypeInfo __type_info__af63e44c8601fa24 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int32_t>::size, UINT64_C(0xaf63e44c8601fa24) };
TypeInfo __type_info__d922fe078cefab30 = { Type::tInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int64_t>::size, UINT64_C(0xd922fe078cefab30) };
TypeInfo __type_info__af63ee4c86020b22 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, TypeSize<char *>::size, UINT64_C(0xaf63ee4c86020b22) };
TypeInfo __type_info__af5be84c85f468f0 = { Type::tUInt8, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<uint8_t>::size, UINT64_C(0xaf5be84c85f468f0) };
TypeInfo __type_info__af63eb4c86020609 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<void>::size, UINT64_C(0xaf63eb4c86020609) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[1] = { &__type_info__51bb14dd602168c2 };
TypeInfo * __tinfo_1[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_2[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af96fe4c8658cf52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_3[2] = { &__type_info__af90fe4c864e9d52, &__type_info__4fd1011fade7876f };
TypeInfo * __tinfo_4[2] = { &__type_info__af90fe4c864e9d52, &__type_info__4fd1011fade7876f };
TypeInfo * __tinfo_5[2] = { &__type_info__af90fe4c864e9d52, &__type_info__4fd1011fade7876f };
TypeInfo * __tinfo_6[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_7[3] = { &__type_info__af90fe4c864e9d52, &__type_info__7a089ab9e22fd5f, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_8[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_9[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_10[3] = { &__type_info__af90fe4c864e9d52, &__type_info__7a089ab9e22fd5f, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_11[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52 };
TypeInfo * __tinfo_12[4] = { &__type_info__af90fe4c864e9d52, &__type_info__7a089ab9e22fd5f, &__type_info__af90fe4c864e9d52, &__type_info__a7159d402feecb0a };
TypeInfo * __tinfo_13[5] = { &__type_info__af90fe4c864e9d52, &__type_info__8d8d8008262e16ec, &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_14[2] = { &__type_info__af90fe4c864e9d52, &__type_info__7a089ab9e22fd5f };

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_27c833578baf1b20 ( Context * __context__, TArray<uint8_t> & __a_rename_at_1234_0 );
inline bool _FuncbuiltinTicknextTick17450348357676149856_8b0be63689a99d63 ( Context * __context__, Sequence DAS_COMMENT((int32_t)) & __it_rename_at_1275_1, int32_t & __value_rename_at_1275_2 );
inline void finalize_9eb6d916b60f25b7 ( Context * __context__, debug_eval::_lambda_debug_eval_26_1 & ____this_rename_at_26_3 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_2182b904ebc5e479 ( Context * __context__, TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>> & __Arr_rename_at_68_4, int32_t __newSize_rename_at_68_5 );
inline void _FuncbuiltinTickclearTick13739625760977891612_6523d27c25fcd504 ( Context * __context__, TTable<char *,debug_eval::Result> & __t_rename_at_1912_6 );
inline Sequence DAS_COMMENT((char *)) _FuncbuiltinTickkeysTick2205854368403803976_b9582efb4c0d7da2 ( Context * __context__, TTable<char *,debug_eval::Result> const  & __a_rename_at_1180_7 );
inline Sequence DAS_COMMENT((debug_eval::Result &)) _FuncbuiltinTickvaluesTick1935193042646774172_a3ae7bd938ff3d33 ( Context * __context__, TTable<char *,debug_eval::Result> const  & __a_rename_at_1195_9 );
inline Sequence DAS_COMMENT((AutoVariant<int64_t,int32_t,char *,char *,char *>)) _FuncbuiltinTickeachTick9663565701927713696_1f90241636846e ( Context * __context__, Lambda DAS_COMMENT((bool,AutoVariant<int64_t,int32_t,char *,char *,char *>)) const  __lam_rename_at_1341_11 );
inline debug_eval::TokenStream & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_70b51b4c8469b8c5 ( Context * __context__, debug_eval::TokenStream & __a_rename_at_50_13 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_26b930b48ef88e1b ( Context * __context__, TTable<char *,debug_eval::Result> const  & __Tab_rename_at_1047_14, char * const  __at_rename_at_1047_15 );
inline debug_eval::Result _FuncbuiltinTickget_valueTick6803070933163225147_86faf0edd3fcebfe ( Context * __context__, TTable<char *,debug_eval::Result> & __Tab_rename_at_741_16, char * const  __at_rename_at_741_17 );
inline Sequence DAS_COMMENT((int32_t)) _FuncbuiltinTickeachTick4044332007967089362_e3640663f3ad1647 ( Context * __context__, char * const  __str_rename_at_1308_18 );
inline void _FuncbuiltinTickpushTick14133213201864676143_fb343dce7d417ea7 ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_165_20, uint8_t __value_rename_at_165_21 );
inline bool _Func_lambda_debug_eval_26_1Tickfunction_96d8425d73290f4c ( Context * __context__, debug_eval::_lambda_debug_eval_26_1 & ____this_rename_at_26_22, AutoVariant<int64_t,int32_t,char *,char *,char *> & ___yield_26_rename_at_26_23 );
inline void _Func_lambda_debug_eval_26_1Tickfinalizer_c96bf990405645ee ( Context * __context__, debug_eval::_lambda_debug_eval_26_1 * ____this_rename_at_26_24 );
inline bool _FuncbuiltinTickemptyTick15399874715904164783_f42072dd005df0e5 ( Context * __context__, TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>> const  & __a_rename_at_585_25 );
inline bool _FuncbuiltinTicknextTick17450348357676149856_5b50b5e96d75f745 ( Context * __context__, Sequence DAS_COMMENT((AutoVariant<int64_t,int32_t,char *,char *,char *>)) & __it_rename_at_1275_26, AutoVariant<int64_t,int32_t,char *,char *,char *> & __value_rename_at_1275_27 );
inline AutoVariant<int64_t,int32_t,char *,char *,char *> & _FuncbuiltinTickbackTick18296309835877697278_ef4cec92e917281d ( Context * __context__, TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>> & __a_rename_at_473_28 );
inline void _FuncbuiltinTickpopTick1161079256290593740_474fdde50584fe9b ( Context * __context__, TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>> & __Arr_rename_at_132_30 );
inline void _FuncbuiltinTickpushTick14133213201864676143_e139faaef2376ad9 ( Context * __context__, TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>> & __Arr_rename_at_165_31, AutoVariant<int64_t,int32_t,char *,char *,char *> const  & __value_rename_at_165_32 );
inline void _FuncbuiltinTickpushTick14133213201864676143_9ba16521459d7b1c ( Context * __context__, TArray<char *> & __Arr_rename_at_165_33, char * const  __value_rename_at_165_34 );
inline int64_t _Funcdebug_evalTickloadITick653575658332832056_4fd755c4a7fd2bd9 ( Context * __context__, void * const  __data_rename_at_135_35, int64_t __t_rename_at_135_36 );
inline int32_t _Funcdebug_evalTickloadITick653575658332832056_ef3b66a87599345e ( Context * __context__, void * const  __data_rename_at_135_37, int32_t __t_rename_at_135_38 );
inline int16_t _Funcdebug_evalTickloadITick653575658332832056_83c53155ffab2f2d ( Context * __context__, void * const  __data_rename_at_135_39, int16_t __t_rename_at_135_40 );
inline int8_t _Funcdebug_evalTickloadITick653575658332832056_c36fa462805fa187 ( Context * __context__, void * const  __data_rename_at_135_41, int8_t __t_rename_at_135_42 );
inline bool _Funcdebug_evalTickloadITick653575658332832056_d898bb5d27c76a23 ( Context * __context__, void * const  __data_rename_at_135_43, bool __t_rename_at_135_44 );
inline void _FuncbuiltinTickcloneTick3147220447302434744_70013ebd41b82ce5 ( Context * __context__, TTable<char *,debug_eval::Result> & __a_rename_at_1152_45, TTable<char *,debug_eval::Result> const  & __b_rename_at_1152_46 );
inline bool _FuncbuiltinTickemptyTick15399874715904164783_e1ebf91008e61bc3 ( Context * __context__, TArray<char *> const  & __a_rename_at_585_52 );
inline Sequence DAS_COMMENT((AutoVariant<int64_t,int32_t,char *,char *,char *>)) each_token_e5227dee8193fe81 ( Context * __context__, char * const  __st_rename_at_25_53 );
inline debug_eval::TokenStream TokenStream_ae362449c106dfa ( Context * __context__, char * const  __st_rename_at_89_54 );
inline AutoVariant<int64_t,int32_t,char *,char *,char *> token_905886d3b1cc992c ( Context * __context__, debug_eval::TokenStream & __st_rename_at_93_55 );
inline void unput_9f6508d6fd04dc86 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_106_57, AutoVariant<int64_t,int32_t,char *,char *,char *> const  & __token_rename_at_106_58 );
inline debug_eval::Result Result_1f04b6f1f7809164 ( Context * __context__, int64_t __value_rename_at_110_59 );
inline debug_eval::Result Result_73cdb19a79e62102 ( Context * __context__, bool __value_rename_at_117_61 );
inline debug_eval::Result InvalidResult_9b1b21e34eba155d ( Context * __context__ );
inline debug_eval::Result error_e5bb3044bbd89a1d ( Context * __context__, debug_eval::TokenStream & __st_rename_at_130_64, char * const  __error_rename_at_130_65 );
inline void * getPD_4da28c77bbe6c3d7 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_141_66, debug_eval::Result const  & __result_rename_at_141_67, int32_t __offset_rename_at_141_68 );
inline int64_t getI_5e1fded1da82fe11 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_167_72, debug_eval::Result const  & __result_rename_at_167_73 );
inline char * getT_9627d57179cddca6 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_187_76, debug_eval::Result const  & __result_rename_at_187_77 );
inline debug_eval::Result func_call_length_17a2d2ec17789881 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_198_78, debug_eval::Result const  & __result_rename_at_198_79 );
inline debug_eval::Result func_call_34538813fbb8fc0f ( Context * __context__, debug_eval::TokenStream & __st_rename_at_220_84, char * const  __name_rename_at_220_85, debug_eval::Result const  & __arg_rename_at_220_86 );
inline debug_eval::Result expr_value_ae10ccbfc8e1ce31 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_225_87 );
inline AutoTuple<int32_t,TypeInfo> getStructFieldOffset_13b5c35ad8bad1b1 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_272_92, char * const  __ident_rename_at_272_93, debug_eval::Result & __result_rename_at_272_94 );
inline AutoTuple<int32_t,TypeInfo> getTupleFieldOffset_843b15bad51bff07 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_289_99, char * const  __ident_rename_at_289_100, debug_eval::Result & __result_rename_at_289_101 );
inline AutoTuple<int32_t,TypeInfo> getVectorOffset_dcf7cf4570b94b24 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_303_106, char * const  __ident_rename_at_303_107, debug_eval::Result & __result_rename_at_303_108 );
inline AutoTuple<int32_t,TypeInfo> getHandleFieldOffset_4b8713168d14b603 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_314_112, char * const  __ident_rename_at_314_113, debug_eval::Result & __result_rename_at_314_114 );
inline debug_eval::Result expr_field_5dfcd45b1ecd4499 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_329_118 );
inline debug_eval::Result opDim_b0bf78e5c93259f1 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_370_124, int64_t __index_rename_at_370_125, debug_eval::Result & __result_rename_at_370_126 );
inline debug_eval::Result opDimString_6ac245c533760d26 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_381_129, int64_t __index_rename_at_381_130, debug_eval::Result & __result_rename_at_381_131 );
inline debug_eval::Result opArray_f74b354d6cd2ce02 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_397_134, int64_t __index_rename_at_397_135, debug_eval::Result & __result_rename_at_397_136 );
inline debug_eval::Result opTable_b6c6c7c404d92dc9 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_416_142, debug_eval::Result const  & __index_rename_at_416_143, debug_eval::Result & __result_rename_at_416_144 );
inline debug_eval::Result expr_at_56cc60f4051344c2 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_440_152 );
inline debug_eval::Result expr_unary_a06fc7e0b3c8f97 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_473_157 );
inline debug_eval::Result expr_mul_div_d2e41a242a5717c8 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_493_159 );
inline debug_eval::Result expr_add_sub_98f296ed1fed65d7 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_514_163 );
inline bool try_accept_punkt_36a058d72081e56e ( Context * __context__, debug_eval::TokenStream & __st_rename_at_531_166, int32_t __punkt_rename_at_531_167 );
inline debug_eval::Result expr_and_d5d8333e25dfcbe4 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_540_169 );
inline debug_eval::Result expr_xor_98afb583f1a5a9a5 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_552_171 );
inline debug_eval::Result expr_or_d504d6b12b7ceea7 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_564_173 );
inline debug_eval::Result expr_bool_op_c0cbada86a1585de ( Context * __context__, debug_eval::TokenStream & __st_rename_at_576_175 );
inline debug_eval::Result expr_251518313273ca50 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_609_179 );
inline debug_eval::Result debug_eval_25c7c1861470fbfd ( Context * __context__, TTable<char *,debug_eval::Result> const  & __context_rename_at_613_180, char * const  __expr_rename_at_613_181 );

void __init_script ( Context * __context__, bool __init_shared )
{
    das_global<int64_t,0x9bae386ba6f944e9>(__context__) = INT64_MIN;/*INVALID_VALUE*/
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_27c833578baf1b20 ( Context * __context__, TArray<uint8_t> &  __a_rename_at_1234_0 )
{
    builtin_array_free(das_arg<TArray<uint8_t>>::pass(__a_rename_at_1234_0),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline bool _FuncbuiltinTicknextTick17450348357676149856_8b0be63689a99d63 ( Context * __context__, Sequence DAS_COMMENT((int32_t)) &  __it_rename_at_1275_1, int32_t & __value_rename_at_1275_2 )
{
    return das_auto_cast<bool>::cast(builtin_iterator_iterate(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(__it_rename_at_1275_1),das_auto_cast<void * const >::cast(das_ref(__context__,__value_rename_at_1275_2)),__context__));
}

inline void finalize_9eb6d916b60f25b7 ( Context * __context__, debug_eval::_lambda_debug_eval_26_1 &  ____this_rename_at_26_3 )
{
    builtin_iterator_delete(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_26_3.chars),__context__);
    _FuncbuiltinTickfinalizeTick13836114024949725080_27c833578baf1b20(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_26_3.__str_rename_at_42_20));
    _FuncbuiltinTickfinalizeTick13836114024949725080_27c833578baf1b20(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_26_3.__str_rename_at_57_20));
    memset((void*)&(____this_rename_at_26_3), 0, TypeSize<debug_eval::_lambda_debug_eval_26_1>::size);
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_2182b904ebc5e479 ( Context * __context__, TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>> &  __Arr_rename_at_68_4, int32_t __newSize_rename_at_68_5 )
{
    builtin_array_resize(das_arg<TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>>>::pass(__Arr_rename_at_68_4),__newSize_rename_at_68_5,16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickclearTick13739625760977891612_6523d27c25fcd504 ( Context * __context__, TTable<char *,debug_eval::Result> &  __t_rename_at_1912_6 )
{
    builtin_table_clear(das_arg<TTable<char *,debug_eval::Result>>::pass(__t_rename_at_1912_6),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline Sequence DAS_COMMENT((char *)) _FuncbuiltinTickkeysTick2205854368403803976_b9582efb4c0d7da2 ( Context * __context__, TTable<char *,debug_eval::Result> const  &  __a_rename_at_1180_7 )
{
    Sequence DAS_COMMENT((char *)) __it_rename_at_1181_8;das_zero(__it_rename_at_1181_8);
    builtin_table_keys(das_arg<Sequence DAS_COMMENT((char *))>::pass(__it_rename_at_1181_8),__a_rename_at_1180_7,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((char *))>::cast(__it_rename_at_1181_8);
}

inline Sequence DAS_COMMENT((debug_eval::Result &)) _FuncbuiltinTickvaluesTick1935193042646774172_a3ae7bd938ff3d33 ( Context * __context__, TTable<char *,debug_eval::Result> const  &  __a_rename_at_1195_9 )
{
    Sequence DAS_COMMENT((debug_eval::Result *)) __it_rename_at_1196_10;das_zero(__it_rename_at_1196_10);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((debug_eval::Result const ))>::pass(__it_rename_at_1196_10),__a_rename_at_1195_9,112,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((debug_eval::Result &))>::cast(__it_rename_at_1196_10);
}

inline Sequence DAS_COMMENT((AutoVariant<int64_t,int32_t,char *,char *,char *>)) _FuncbuiltinTickeachTick9663565701927713696_1f90241636846e ( Context * __context__, Lambda DAS_COMMENT((bool,AutoVariant<int64_t,int32_t,char *,char *,char *>)) const  __lam_rename_at_1341_11 )
{
    Sequence DAS_COMMENT((AutoVariant<int64_t,int32_t,char *,char *,char *>)) __it_rename_at_1343_12;das_zero(__it_rename_at_1343_12);
    builtin_make_lambda_iterator(das_arg<Sequence DAS_COMMENT((AutoVariant<int64_t,int32_t,char *,char *,char *>))>::pass(__it_rename_at_1343_12),__lam_rename_at_1341_11,16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((AutoVariant<int64_t,int32_t,char *,char *,char *>))>::cast(__it_rename_at_1343_12);
}

inline debug_eval::TokenStream & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_70b51b4c8469b8c5 ( Context * __context__, debug_eval::TokenStream &  __a_rename_at_50_13 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_0,cast<debug_eval::TokenStream &>::from(__a_rename_at_50_13)));
    return das_auto_cast_ref<debug_eval::TokenStream &>::cast(__a_rename_at_50_13);
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_26b930b48ef88e1b ( Context * __context__, TTable<char *,debug_eval::Result> const  &  __Tab_rename_at_1047_14, char * const  __at_rename_at_1047_15 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1047_14,__at_rename_at_1047_15));
}

inline debug_eval::Result _FuncbuiltinTickget_valueTick6803070933163225147_86faf0edd3fcebfe ( Context * __context__, TTable<char *,debug_eval::Result> &  __Tab_rename_at_741_16, char * const  __at_rename_at_741_17 )
{
    return das_auto_cast_ref<debug_eval::Result>::cast(__Tab_rename_at_741_16(__at_rename_at_741_17,__context__));
}

inline Sequence DAS_COMMENT((int32_t)) _FuncbuiltinTickeachTick4044332007967089362_e3640663f3ad1647 ( Context * __context__, char * const  __str_rename_at_1308_18 )
{
    Sequence DAS_COMMENT((int32_t)) __it_rename_at_1309_19;das_zero(__it_rename_at_1309_19);
    builtin_make_string_iterator(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(__it_rename_at_1309_19),__str_rename_at_1308_18,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((int32_t))>::cast(__it_rename_at_1309_19);
}

inline void _FuncbuiltinTickpushTick14133213201864676143_fb343dce7d417ea7 ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_165_20, uint8_t __value_rename_at_165_21 )
{
    das_copy(__Arr_rename_at_165_20(builtin_array_push_back(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_165_20),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_165_21);
}

inline bool _Func_lambda_debug_eval_26_1Tickfunction_96d8425d73290f4c ( Context * __context__, debug_eval::_lambda_debug_eval_26_1 &  ____this_rename_at_26_22, AutoVariant<int64_t,int32_t,char *,char *,char *> &  ___yield_26_rename_at_26_23 )
{
    switch (____this_rename_at_26_22.__yield) {
    case 0: goto label_0;
    case 14: goto label_14;
    case 1: goto label_1;
    case 12: goto label_12;
    case 2: goto label_2;
    case 4: goto label_4;
    case 3: goto label_3;
    case 5: goto label_5;
    case 10: goto label_10;
    case 6: goto label_6;
    case 8: goto label_8;
    case 7: goto label_7;
    case 9: goto label_9;
    case 11: goto label_11;
    case 13: goto label_13;
    case 15: goto label_15;
    case 16: goto label_16;
    default: __context__->throw_error("invalid label");
    };
    label_0:;;
    das_copy(____this_rename_at_26_22.tst,____this_rename_at_26_22.st);
    das_move(____this_rename_at_26_22.chars,_FuncbuiltinTickeachTick4044332007967089362_e3640663f3ad1647(__context__,____this_rename_at_26_22.tst));
    das_copy(____this_rename_at_26_22.ahead,32);
    label_14:;;
    if ( !!(builtin_iterator_empty(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_26_22.chars))) )
    {
        goto label_15;
    };
    while ( is_white_space(____this_rename_at_26_22.ahead) && _FuncbuiltinTicknextTick17450348357676149856_8b0be63689a99d63(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_26_22.chars),____this_rename_at_26_22.ahead) )
    {
    };
    if ( builtin_iterator_empty(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_26_22.chars)) )
    {
        goto label_15;
    };
    if ( !(((((((((((((((((____this_rename_at_26_22.ahead == 43) || (____this_rename_at_26_22.ahead == 45)) || (____this_rename_at_26_22.ahead == 42)) || (____this_rename_at_26_22.ahead == 47)) || (____this_rename_at_26_22.ahead == 91)) || (____this_rename_at_26_22.ahead == 93)) || (____this_rename_at_26_22.ahead == 40)) || (____this_rename_at_26_22.ahead == 41)) || (____this_rename_at_26_22.ahead == 46)) || (____this_rename_at_26_22.ahead == 33)) || (____this_rename_at_26_22.ahead == 126)) || (____this_rename_at_26_22.ahead == 38)) || (____this_rename_at_26_22.ahead == 124)) || (____this_rename_at_26_22.ahead == 94)) || (____this_rename_at_26_22.ahead == 60)) || (____this_rename_at_26_22.ahead == 62)) || (____this_rename_at_26_22.ahead == 61)) )
    {
        goto label_12;
    };
    das_copy(___yield_26_rename_at_26_23,(([&]() -> AutoVariant<int64_t,int32_t,char *,char *,char *> {
        AutoVariant<int64_t,int32_t,char *,char *,char *> __mkv_39;
        das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::set(__mkv_39) = ____this_rename_at_26_22.ahead;
        return __mkv_39;
    })()));
    das_copy(____this_rename_at_26_22.__yield,1);
    return das_auto_cast<bool>::cast(true);
    label_1:;;
    _FuncbuiltinTicknextTick17450348357676149856_8b0be63689a99d63(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_26_22.chars),____this_rename_at_26_22.ahead);
    goto label_13;
    label_12:;;
    if ( !is_number(____this_rename_at_26_22.ahead) )
    {
        goto label_10;
    };
    memset((void*)&(____this_rename_at_26_22.__str_rename_at_42_20), 0, TypeSize<TArray<uint8_t>>::size);
    _FuncbuiltinTickpushTick14133213201864676143_fb343dce7d417ea7(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_26_22.__str_rename_at_42_20),uint8_t(____this_rename_at_26_22.ahead));
    while ( _FuncbuiltinTicknextTick17450348357676149856_8b0be63689a99d63(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_26_22.chars),____this_rename_at_26_22.ahead) && is_number(____this_rename_at_26_22.ahead) )
    {
        _FuncbuiltinTickpushTick14133213201864676143_fb343dce7d417ea7(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_26_22.__str_rename_at_42_20),uint8_t(____this_rename_at_26_22.ahead));
    };
    if ( !(((builtin_array_size(das_arg<TArray<uint8_t>>::pass(____this_rename_at_26_22.__str_rename_at_42_20)) == 1) && (das_equ_val(____this_rename_at_26_22.__str_rename_at_42_20(0,__context__),0x30))) && ((____this_rename_at_26_22.ahead == 120) || (____this_rename_at_26_22.ahead == 88))) )
    {
        goto label_4;
    };
    _FuncbuiltinTickpushTick14133213201864676143_fb343dce7d417ea7(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_26_22.__str_rename_at_42_20),uint8_t(____this_rename_at_26_22.ahead));
    while ( _FuncbuiltinTicknextTick17450348357676149856_8b0be63689a99d63(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_26_22.chars),____this_rename_at_26_22.ahead) && ((is_number(____this_rename_at_26_22.ahead) || ((____this_rename_at_26_22.ahead >= 97) && (____this_rename_at_26_22.ahead <= 102))) || ((____this_rename_at_26_22.ahead >= 65) && (____this_rename_at_26_22.ahead <= 70))) )
    {
        _FuncbuiltinTickpushTick14133213201864676143_fb343dce7d417ea7(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_26_22.__str_rename_at_42_20),uint8_t(____this_rename_at_26_22.ahead));
    };
    das_copy(___yield_26_rename_at_26_23,(([&]() -> AutoVariant<int64_t,int32_t,char *,char *,char *> {
        AutoVariant<int64_t,int32_t,char *,char *,char *> __mkv_52;
        das_get_auto_variant_field<int64_t,0,int64_t,int32_t,char *,char *,char *>::set(__mkv_52) = fast_to_int64(((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass(____this_rename_at_26_22.__str_rename_at_42_20),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))),true);
        return __mkv_52;
    })()));
    das_copy(____this_rename_at_26_22.__yield,2);
    return das_auto_cast<bool>::cast(true);
    label_2:;;
    goto label_5;
    label_4:;;
    das_copy(___yield_26_rename_at_26_23,(([&]() -> AutoVariant<int64_t,int32_t,char *,char *,char *> {
        AutoVariant<int64_t,int32_t,char *,char *,char *> __mkv_54;
        das_get_auto_variant_field<int64_t,0,int64_t,int32_t,char *,char *,char *>::set(__mkv_54) = fast_to_int64(((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass(____this_rename_at_26_22.__str_rename_at_42_20),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))),false);
        return __mkv_54;
    })()));
    das_copy(____this_rename_at_26_22.__yield,3);
    return das_auto_cast<bool>::cast(true);
    label_3:;;
    label_5:;;
    goto label_11;
    label_10:;;
    if ( !((is_alpha(____this_rename_at_26_22.ahead) || (____this_rename_at_26_22.ahead == 95)) || (____this_rename_at_26_22.ahead == 96)) )
    {
        goto label_8;
    };
    memset((void*)&(____this_rename_at_26_22.__str_rename_at_57_20), 0, TypeSize<TArray<uint8_t>>::size);
    _FuncbuiltinTickpushTick14133213201864676143_fb343dce7d417ea7(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_26_22.__str_rename_at_57_20),uint8_t(____this_rename_at_26_22.ahead));
    while ( _FuncbuiltinTicknextTick17450348357676149856_8b0be63689a99d63(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_26_22.chars),____this_rename_at_26_22.ahead) && (((is_alpha(____this_rename_at_26_22.ahead) || is_number(____this_rename_at_26_22.ahead)) || (____this_rename_at_26_22.ahead == 95)) || (____this_rename_at_26_22.ahead == 96)) )
    {
        _FuncbuiltinTickpushTick14133213201864676143_fb343dce7d417ea7(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_26_22.__str_rename_at_57_20),uint8_t(____this_rename_at_26_22.ahead));
    };
    das_copy(___yield_26_rename_at_26_23,(([&]() -> AutoVariant<int64_t,int32_t,char *,char *,char *> {
        AutoVariant<int64_t,int32_t,char *,char *,char *> __mkv_62;
        das_get_auto_variant_field<char *,2,int64_t,int32_t,char *,char *,char *>::set(__mkv_62) = ((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass(____this_rename_at_26_22.__str_rename_at_57_20),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))));
        return __mkv_62;
    })()));
    das_copy(____this_rename_at_26_22.__yield,6);
    return das_auto_cast<bool>::cast(true);
    label_6:;;
    goto label_9;
    label_8:;;
    das_copy(___yield_26_rename_at_26_23,(([&]() -> AutoVariant<int64_t,int32_t,char *,char *,char *> {
        AutoVariant<int64_t,int32_t,char *,char *,char *> __mkv_64;
        das_get_auto_variant_field<char *,3,int64_t,int32_t,char *,char *,char *>::set(__mkv_64) = das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_1, cast<char * const >::from(((char *) "invalid character ")), cast<char * const >::from(((char * const )(to_string_char(____this_rename_at_26_22.ahead,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))))));
        return __mkv_64;
    })()));
    das_copy(____this_rename_at_26_22.__yield,7);
    return das_auto_cast<bool>::cast(true);
    label_7:;;
    return das_auto_cast<bool>::cast(false);
    label_9:;;
    label_11:;;
    label_13:;;
    goto label_14;
    label_15:;;
    das_copy(___yield_26_rename_at_26_23,(([&]() -> AutoVariant<int64_t,int32_t,char *,char *,char *> {
        AutoVariant<int64_t,int32_t,char *,char *,char *> __mkv_68;
        das_get_auto_variant_field<char *,4,int64_t,int32_t,char *,char *,char *>::set(__mkv_68) = ((char *) "eos");
        return __mkv_68;
    })()));
    das_copy(____this_rename_at_26_22.__yield,16);
    return das_auto_cast<bool>::cast(true);
    label_16:;;
    return das_auto_cast<bool>::cast(false);
}

inline void _Func_lambda_debug_eval_26_1Tickfinalizer_c96bf990405645ee ( Context * __context__, debug_eval::_lambda_debug_eval_26_1 * ____this_rename_at_26_24 )
{
    finalize_9eb6d916b60f25b7(__context__,das_arg<debug_eval::_lambda_debug_eval_26_1>::pass(das_deref(__context__,____this_rename_at_26_24)));
    das_delete_lambda_struct<debug_eval::_lambda_debug_eval_26_1 *>::clear(__context__,____this_rename_at_26_24);
}

inline bool _FuncbuiltinTickemptyTick15399874715904164783_f42072dd005df0e5 ( Context * __context__, TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>> const  &  __a_rename_at_585_25 )
{
    return das_auto_cast<bool>::cast(builtin_array_size(__a_rename_at_585_25) == 0);
}

inline bool _FuncbuiltinTicknextTick17450348357676149856_5b50b5e96d75f745 ( Context * __context__, Sequence DAS_COMMENT((AutoVariant<int64_t,int32_t,char *,char *,char *>)) &  __it_rename_at_1275_26, AutoVariant<int64_t,int32_t,char *,char *,char *> &  __value_rename_at_1275_27 )
{
    return das_auto_cast<bool>::cast(builtin_iterator_iterate(das_arg<Sequence DAS_COMMENT((AutoVariant<int64_t,int32_t,char *,char *,char *>))>::pass(__it_rename_at_1275_26),das_auto_cast<void * const >::cast(das_ref(__context__,__value_rename_at_1275_27)),__context__));
}

inline AutoVariant<int64_t,int32_t,char *,char *,char *> & _FuncbuiltinTickbackTick18296309835877697278_ef4cec92e917281d ( Context * __context__, TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>> &  __a_rename_at_473_28 )
{
    int32_t __l_rename_at_474_29 = ((int32_t)builtin_array_size(das_arg<TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>>>::pass(__a_rename_at_473_28)));
    if ( __l_rename_at_474_29 == 0 )
    {
        builtin_throw(((char *) "back empty array"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    return das_auto_cast_ref<AutoVariant<int64_t,int32_t,char *,char *,char *> &>::cast(__a_rename_at_473_28((__l_rename_at_474_29 - 1),__context__));
}

inline void _FuncbuiltinTickpopTick1161079256290593740_474fdde50584fe9b ( Context * __context__, TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>> &  __Arr_rename_at_132_30 )
{
    _FuncbuiltinTickresizeTick4811697762258667383_2182b904ebc5e479(__context__,das_arg<TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>>>::pass(__Arr_rename_at_132_30),builtin_array_size(das_arg<TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>>>::pass(__Arr_rename_at_132_30)) - 1);
}

inline void _FuncbuiltinTickpushTick14133213201864676143_e139faaef2376ad9 ( Context * __context__, TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>> &  __Arr_rename_at_165_31, AutoVariant<int64_t,int32_t,char *,char *,char *> const  &  __value_rename_at_165_32 )
{
    das_copy(__Arr_rename_at_165_31(builtin_array_push_back(das_arg<TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>>>::pass(__Arr_rename_at_165_31),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_165_32);
}

inline void _FuncbuiltinTickpushTick14133213201864676143_9ba16521459d7b1c ( Context * __context__, TArray<char *> &  __Arr_rename_at_165_33, char * const  __value_rename_at_165_34 )
{
    das_copy(__Arr_rename_at_165_33(builtin_array_push_back(das_arg<TArray<char *>>::pass(__Arr_rename_at_165_33),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_165_34);
}

inline int64_t _Funcdebug_evalTickloadITick653575658332832056_4fd755c4a7fd2bd9 ( Context * __context__, void * const  __data_rename_at_135_35, int64_t __t_rename_at_135_36 )
{
    return das_auto_cast<int64_t>::cast(das_deref(__context__,das_cast<int64_t const  *>::cast(__data_rename_at_135_35)));
}

inline int32_t _Funcdebug_evalTickloadITick653575658332832056_ef3b66a87599345e ( Context * __context__, void * const  __data_rename_at_135_37, int32_t __t_rename_at_135_38 )
{
    return das_auto_cast<int32_t>::cast(das_deref(__context__,das_cast<int32_t const  *>::cast(__data_rename_at_135_37)));
}

inline int16_t _Funcdebug_evalTickloadITick653575658332832056_83c53155ffab2f2d ( Context * __context__, void * const  __data_rename_at_135_39, int16_t __t_rename_at_135_40 )
{
    return das_auto_cast<int16_t>::cast(das_deref(__context__,das_cast<int16_t const  *>::cast(__data_rename_at_135_39)));
}

inline int8_t _Funcdebug_evalTickloadITick653575658332832056_c36fa462805fa187 ( Context * __context__, void * const  __data_rename_at_135_41, int8_t __t_rename_at_135_42 )
{
    return das_auto_cast<int8_t>::cast(das_deref(__context__,das_cast<int8_t const  *>::cast(__data_rename_at_135_41)));
}

inline bool _Funcdebug_evalTickloadITick653575658332832056_d898bb5d27c76a23 ( Context * __context__, void * const  __data_rename_at_135_43, bool __t_rename_at_135_44 )
{
    return das_auto_cast<bool>::cast(das_deref(__context__,das_cast<bool const  *>::cast(__data_rename_at_135_43)));
}

inline void _FuncbuiltinTickcloneTick3147220447302434744_70013ebd41b82ce5 ( Context * __context__, TTable<char *,debug_eval::Result> &  __a_rename_at_1152_45, TTable<char *,debug_eval::Result> const  &  __b_rename_at_1152_46 )
{
    Sequence DAS_COMMENT((char *)) _temp_make_local_1154_17_0; _temp_make_local_1154_17_0;
    Sequence DAS_COMMENT((debug_eval::Result *)) _temp_make_local_1154_26_1; _temp_make_local_1154_26_1;
    _FuncbuiltinTickclearTick13739625760977891612_6523d27c25fcd504(__context__,das_arg<TTable<char *,debug_eval::Result>>::pass(__a_rename_at_1152_45));
    {
        bool __need_loop_1154 = true;
        // k: string
        das_iterator<Sequence DAS_COMMENT((char *))> __k_iterator((_temp_make_local_1154_17_0 = (_FuncbuiltinTickkeysTick2205854368403803976_b9582efb4c0d7da2(__context__,__b_rename_at_1152_46))));
        char * __k_rename_at_1154_49;
        __need_loop_1154 = __k_iterator.first(__context__,(__k_rename_at_1154_49)) && __need_loop_1154;
        // v: debug_eval::Result const&
        das_iterator<Sequence DAS_COMMENT((debug_eval::Result const ))> __v_iterator((_temp_make_local_1154_26_1 = (_FuncbuiltinTickvaluesTick1935193042646774172_a3ae7bd938ff3d33(__context__,__b_rename_at_1152_46))));
        debug_eval::Result const  * __v_rename_at_1154_50;
        __need_loop_1154 = __v_iterator.first(__context__,(__v_rename_at_1154_50)) && __need_loop_1154;
        for ( ; __need_loop_1154 ; __need_loop_1154 = __k_iterator.next(__context__,(__k_rename_at_1154_49)) && __v_iterator.next(__context__,(__v_rename_at_1154_50)) )
        {
            char * __kk_rename_at_1155_51 = ((char *)(char *)(__k_rename_at_1154_49));
            das_copy(__a_rename_at_1152_45(__kk_rename_at_1155_51,__context__),(*__v_rename_at_1154_50));
        }
        __k_iterator.close(__context__,(__k_rename_at_1154_49));
        __v_iterator.close(__context__,(__v_rename_at_1154_50));
    };
}

inline bool _FuncbuiltinTickemptyTick15399874715904164783_e1ebf91008e61bc3 ( Context * __context__, TArray<char *> const  &  __a_rename_at_585_52 )
{
    return das_auto_cast<bool>::cast(builtin_array_size(__a_rename_at_585_52) == 0);
}

inline Sequence DAS_COMMENT((AutoVariant<int64_t,int32_t,char *,char *,char *>)) each_token_e5227dee8193fe81 ( Context * __context__, char * const  __st_rename_at_25_53 )
{
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((AutoVariant<int64_t,int32_t,char *,char *,char *>))>::cast(_FuncbuiltinTickeachTick9663565701927713696_1f90241636846e(__context__,das_ascend<bool,debug_eval::_lambda_debug_eval_26_1>::make(__context__,&__type_info__64e2d0c6d216e20d,(([&]() -> debug_eval::_lambda_debug_eval_26_1 {
        debug_eval::_lambda_debug_eval_26_1 __mks_26;
        das_zero(__mks_26);
        das_copy((__mks_26.__lambda),(Func(__context__->fnByMangledName(/*@debug_eval::_lambda_debug_eval_26_1`function XS<debug_eval::_lambda_debug_eval_26_1> Y<Token>N<number;punkt;ident;invalid;eos>0<i64;i;s;s;s>V*/ 0x564819c6df0852e7))));
        das_copy((__mks_26.__finalize),(Func(__context__->fnByMangledName(/*@debug_eval::_lambda_debug_eval_26_1`finalizer X1<S<debug_eval::_lambda_debug_eval_26_1>>?*/ 0x456a0534fda3a25d))));
        das_copy((__mks_26.st),(__st_rename_at_25_53));
        return __mks_26;
    })()))));
}

inline debug_eval::TokenStream TokenStream_ae362449c106dfa ( Context * __context__, char * const  __st_rename_at_89_54 )
{
    debug_eval::TokenStream _temp_make_local_90_14_2; _temp_make_local_90_14_2;
    return /* <- */ das_auto_cast_move<debug_eval::TokenStream>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_70b51b4c8469b8c5(__context__,das_arg<debug_eval::TokenStream>::pass((([&]() -> debug_eval::TokenStream& {
        das_zero(_temp_make_local_90_14_2);
        das_move((_temp_make_local_90_14_2.tokens),(each_token_e5227dee8193fe81(__context__,__st_rename_at_89_54)));
        return _temp_make_local_90_14_2;
    })()))));
}

inline AutoVariant<int64_t,int32_t,char *,char *,char *> token_905886d3b1cc992c ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_93_55 )
{
    AutoVariant<int64_t,int32_t,char *,char *,char *> __token_rename_at_94_56;das_zero(__token_rename_at_94_56);
    if ( _FuncbuiltinTickemptyTick15399874715904164783_f42072dd005df0e5(__context__,das_arg<TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>>>::pass(__st_rename_at_93_55.stream)) )
    {
        if ( !_FuncbuiltinTicknextTick17450348357676149856_5b50b5e96d75f745(__context__,das_arg<Sequence DAS_COMMENT((AutoVariant<int64_t,int32_t,char *,char *,char *>))>::pass(__st_rename_at_93_55.tokens),das_arg<AutoVariant<int64_t,int32_t,char *,char *,char *>>::pass(__token_rename_at_94_56)) )
        {
            das_copy(__token_rename_at_94_56,(([&]() -> AutoVariant<int64_t,int32_t,char *,char *,char *> {
                AutoVariant<int64_t,int32_t,char *,char *,char *> __mkv_97;
                das_get_auto_variant_field<char *,4,int64_t,int32_t,char *,char *,char *>::set(__mkv_97) = ((char *) "eos");
                return __mkv_97;
            })()));
        };
    } else {
        das_copy(__token_rename_at_94_56,_FuncbuiltinTickbackTick18296309835877697278_ef4cec92e917281d(__context__,das_arg<TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>>>::pass(__st_rename_at_93_55.stream)));
        _FuncbuiltinTickpopTick1161079256290593740_474fdde50584fe9b(__context__,das_arg<TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>>>::pass(__st_rename_at_93_55.stream));
    };
    return das_auto_cast_ref<AutoVariant<int64_t,int32_t,char *,char *,char *>>::cast(__token_rename_at_94_56);
}

inline void unput_9f6508d6fd04dc86 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_106_57, AutoVariant<int64_t,int32_t,char *,char *,char *> const  &  __token_rename_at_106_58 )
{
    _FuncbuiltinTickpushTick14133213201864676143_e139faaef2376ad9(__context__,das_arg<TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>>>::pass(__st_rename_at_106_57.stream),__token_rename_at_106_58);
}

inline debug_eval::Result Result_1f04b6f1f7809164 ( Context * __context__, int64_t __value_rename_at_110_59 )
{
    debug_eval::Result __self_rename_at_111_60;das_zero(__self_rename_at_111_60);
    das_copy(__self_rename_at_111_60.tinfo.type /*basicType*/,DAS_COMMENT(bound_enum) das::Type::tInt64);
    das_copy(__self_rename_at_111_60.value,das_invoke_function<float4>::invoke<int64_t>(__context__,nullptr,Func(__context__->fnByMangledName(/*@math_bits::cast_to_vec4f Ci64*/ 0x3e76e6463b5d2a78)),__value_rename_at_110_59));
    return das_auto_cast_ref<debug_eval::Result>::cast(__self_rename_at_111_60);
}

inline debug_eval::Result Result_73cdb19a79e62102 ( Context * __context__, bool __value_rename_at_117_61 )
{
    debug_eval::Result __self_rename_at_118_62;das_zero(__self_rename_at_118_62);
    das_copy(__self_rename_at_118_62.tinfo.type /*basicType*/,DAS_COMMENT(bound_enum) das::Type::tBool);
    das_copy(__self_rename_at_118_62.value,das_invoke_function<float4>::invoke<bool>(__context__,nullptr,Func(__context__->fnByMangledName(/*@math_bits::cast_to_vec4f Cb*/ 0x3d6f2ea296bbc09e)),__value_rename_at_117_61));
    return das_auto_cast_ref<debug_eval::Result>::cast(__self_rename_at_118_62);
}

inline debug_eval::Result InvalidResult_9b1b21e34eba155d ( Context * __context__ )
{
    debug_eval::Result __self_rename_at_125_63;das_zero(__self_rename_at_125_63);
    das_copy(__self_rename_at_125_63.tinfo.type /*basicType*/,DAS_COMMENT(bound_enum) das::Type::tVoid);
    return das_auto_cast_ref<debug_eval::Result>::cast(__self_rename_at_125_63);
}

inline debug_eval::Result error_e5bb3044bbd89a1d ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_130_64, char * const  __error_rename_at_130_65 )
{
    _FuncbuiltinTickpushTick14133213201864676143_9ba16521459d7b1c(__context__,das_arg<TArray<char *>>::pass(__st_rename_at_130_64.errors),__error_rename_at_130_65);
    return das_auto_cast_ref<debug_eval::Result>::cast(InvalidResult_9b1b21e34eba155d(__context__));
}

inline void * getPD_4da28c77bbe6c3d7 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_141_66, debug_eval::Result const  &  __result_rename_at_141_67, int32_t __offset_rename_at_141_68 )
{
    void const  * __data_rename_at_143_69 = 0;
    if ( __result_rename_at_141_67.data != nullptr )
    {
        das_copy(__data_rename_at_143_69,__result_rename_at_141_67.data);
    } else {
        das_copy(__data_rename_at_143_69,das_ref(__context__,__result_rename_at_141_67.value));
        if ( das_get_bitfield(__result_rename_at_141_67.tinfo.flags /*flags*/,1u << 1) )
        {
            void * * __pdata_rename_at_149_70 = ((void * *)das_cast<void * *>::cast(__data_rename_at_143_69));
            if ( __pdata_rename_at_149_70 == nullptr )
            {
                return das_auto_cast<void *>::cast(nullptr);
            } else {
                das_copy(__data_rename_at_143_69,das_deref(__context__,__pdata_rename_at_149_70));
            };
        };
    };
    if ( das_get_bitfield(__result_rename_at_141_67.tinfo.flags /*flags*/,1u << 0) )
    {
        void * * __pdata_rename_at_157_71 = ((void * *)das_cast<void * *>::cast(__data_rename_at_143_69));
        if ( __pdata_rename_at_157_71 == nullptr )
        {
            return das_auto_cast<void *>::cast(nullptr);
        } else {
            das_copy(__data_rename_at_143_69,das_deref(__context__,__pdata_rename_at_157_71));
        };
    };
    return das_auto_cast<void *>::cast(das_ptr_add_int32(das_cast<uint8_t const  *>::cast(__data_rename_at_143_69),__offset_rename_at_141_68,1));
}

inline int64_t getI_5e1fded1da82fe11 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_167_72, debug_eval::Result const  &  __result_rename_at_167_73 )
{
    if ( __result_rename_at_167_73.tinfo.dimSize /*dimSize*/ != 0x0u )
    {
        debug_eval::Result _temp_make_local_169_14_3; _temp_make_local_169_14_3;
        (_temp_make_local_169_14_3 = (error_e5bb3044bbd89a1d(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_167_72),das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_2, cast<char * const >::from(((char *) "can't get value of [")), cast<uint32_t>::from(__result_rename_at_167_73.tinfo.dimSize /*dimSize*/), cast<char * const >::from(((char *) "]")))))));
        return das_auto_cast<int64_t>::cast(INT64_MIN);
    } else {
        DAS_COMMENT(bound_enum) das::Type __bt_rename_at_172_74 = ((DAS_COMMENT(bound_enum) das::Type)__result_rename_at_167_73.tinfo.type /*basicType*/);
        void * __data_rename_at_173_75 = ((void *)getPD_4da28c77bbe6c3d7(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_167_72),__result_rename_at_167_73,0));
        if ( __data_rename_at_173_75 == nullptr )
        {
            debug_eval::Result _temp_make_local_175_14_4; _temp_make_local_175_14_4;
            (_temp_make_local_175_14_4 = (error_e5bb3044bbd89a1d(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_167_72),das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_3, cast<char * const >::from(((char *) "null pointer dereference for ")), cast<DAS_COMMENT(bound_enum) das::Type>::from(__result_rename_at_167_73.tinfo.type /*basicType*/))))));
            return das_auto_cast<int64_t>::cast(INT64_MIN);
        } else {
            if ( (__bt_rename_at_172_74 == DAS_COMMENT(bound_enum) das::Type::tInt64) || (__bt_rename_at_172_74 == DAS_COMMENT(bound_enum) das::Type::tUInt64) )
            {
                return das_auto_cast<int64_t>::cast(_Funcdebug_evalTickloadITick653575658332832056_4fd755c4a7fd2bd9(__context__,__data_rename_at_173_75,das_typedecl_value<int64_t>()()));
            } else {
                if ( (__bt_rename_at_172_74 == DAS_COMMENT(bound_enum) das::Type::tInt) || (__bt_rename_at_172_74 == DAS_COMMENT(bound_enum) das::Type::tUInt) )
                {
                    return das_auto_cast<int64_t>::cast(int64_t(_Funcdebug_evalTickloadITick653575658332832056_ef3b66a87599345e(__context__,__data_rename_at_173_75,das_typedecl_value<int32_t>()())));
                } else {
                    if ( (__bt_rename_at_172_74 == DAS_COMMENT(bound_enum) das::Type::tInt16) || (__bt_rename_at_172_74 == DAS_COMMENT(bound_enum) das::Type::tUInt16) )
                    {
                        return das_auto_cast<int64_t>::cast(int64_t(_Funcdebug_evalTickloadITick653575658332832056_83c53155ffab2f2d(__context__,__data_rename_at_173_75,das_typedecl_value<int16_t>()())));
                    } else {
                        if ( (__bt_rename_at_172_74 == DAS_COMMENT(bound_enum) das::Type::tInt8) || (__bt_rename_at_172_74 == DAS_COMMENT(bound_enum) das::Type::tUInt8) )
                        {
                            return das_auto_cast<int64_t>::cast(int64_t(_Funcdebug_evalTickloadITick653575658332832056_c36fa462805fa187(__context__,__data_rename_at_173_75,das_typedecl_value<int8_t>()())));
                        } else {
                            if ( __bt_rename_at_172_74 == DAS_COMMENT(bound_enum) das::Type::tBool )
                            {
                                return das_auto_cast<int64_t>::cast(int64_t(_Funcdebug_evalTickloadITick653575658332832056_d898bb5d27c76a23(__context__,__data_rename_at_173_75,das_typedecl_value<bool>()()) ? das_auto_cast<int64_t>::cast(INT64_C(1)) : das_auto_cast<int64_t>::cast(INT64_C(0))));
                            } else {
                                debug_eval::Result _temp_make_local_183_10_5; _temp_make_local_183_10_5;
                                (_temp_make_local_183_10_5 = (error_e5bb3044bbd89a1d(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_167_72),das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_4, cast<char * const >::from(((char *) "can't get value of ")), cast<DAS_COMMENT(bound_enum) das::Type>::from(__result_rename_at_167_73.tinfo.type /*basicType*/))))));
                                return das_auto_cast<int64_t>::cast(INT64_MIN);
                            };
                        };
                    };
                };
            };
        };
    };
}

inline char * getT_9627d57179cddca6 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_187_76, debug_eval::Result const  &  __result_rename_at_187_77 )
{
    return das_auto_cast<char *>::cast(!(builtin_empty(__result_rename_at_187_77.error)) ? das_auto_cast<char * const >::cast(nullptr) : das_auto_cast<char * const >::cast(((__result_rename_at_187_77.data != nullptr) ? das_auto_cast<char * const >::cast(((char * const )(builtin_print_data(__result_rename_at_187_77.data,das_ref(__context__,__result_rename_at_187_77.tinfo),0x10u,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) : das_auto_cast<char * const >::cast(((char * const )(builtin_print_data_v(__result_rename_at_187_77.value,das_ref(__context__,__result_rename_at_187_77.tinfo),0x10u,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))))));
}

inline debug_eval::Result func_call_length_17a2d2ec17789881 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_198_78, debug_eval::Result const  &  __result_rename_at_198_79 )
{
    if ( __result_rename_at_198_79.tinfo.dimSize /*dimSize*/ != 0x0u )
    {
        int32_t __idim_rename_at_201_80 = ((int32_t)rtti_getDimTypeInfo(__result_rename_at_198_79.tinfo,int32_t(__result_rename_at_198_79.tinfo.dimSize /*dimSize*/ - 0x1u),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
        return /* <- */ das_auto_cast_move<debug_eval::Result>::cast(Result_1f04b6f1f7809164(__context__,int64_t(__idim_rename_at_201_80)));
    } else if ( __result_rename_at_198_79.tinfo.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tString )
    {
        char * * __pstr_rename_at_205_81 = ((char * *)das_cast<char * *>::cast(getPD_4da28c77bbe6c3d7(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_198_78),__result_rename_at_198_79,0)));
        return /* <- */ das_auto_cast_move<debug_eval::Result>::cast(Result_1f04b6f1f7809164(__context__,int64_t(builtin_string_length(das_deref(__context__,__pstr_rename_at_205_81),__context__))));
    } else if ( __result_rename_at_198_79.tinfo.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tArray )
    {
        debugapi::DapiArray * __parr_rename_at_209_82 = ((debugapi::DapiArray *)das_cast<debugapi::DapiArray *>::cast(getPD_4da28c77bbe6c3d7(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_198_78),__result_rename_at_198_79,0)));
        return /* <- */ das_auto_cast_move<debug_eval::Result>::cast(Result_1f04b6f1f7809164(__context__,int64_t(__parr_rename_at_209_82->size)));
    } else if ( __result_rename_at_198_79.tinfo.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tTable )
    {
        debugapi::DapiTable * __ptab_rename_at_213_83 = ((debugapi::DapiTable *)das_cast<debugapi::DapiTable *>::cast(getPD_4da28c77bbe6c3d7(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_198_78),__result_rename_at_198_79,0)));
        return /* <- */ das_auto_cast_move<debug_eval::Result>::cast(Result_1f04b6f1f7809164(__context__,int64_t(__ptab_rename_at_213_83->size)));
    };
    return das_auto_cast_ref<debug_eval::Result>::cast(error_e5bb3044bbd89a1d(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_198_78),das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_5, cast<char * const >::from(((char *) "unsupported length for type ")), cast<DAS_COMMENT(bound_enum) das::Type>::from(__result_rename_at_198_79.tinfo.type /*basicType*/)))));
}

inline debug_eval::Result func_call_34538813fbb8fc0f ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_220_84, char * const  __name_rename_at_220_85, debug_eval::Result const  &  __arg_rename_at_220_86 )
{
    if ( SimPolicy<char *>::Equ(cast<char *>::from(__name_rename_at_220_85),cast<char *>::from(((char *) "length")),*__context__,nullptr) )
    {
        return das_auto_cast_ref<debug_eval::Result>::cast(func_call_length_17a2d2ec17789881(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_220_84),__arg_rename_at_220_86));
    } else {
        return das_auto_cast_ref<debug_eval::Result>::cast(error_e5bb3044bbd89a1d(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_220_84),das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_6, cast<char * const >::from(((char *) "unknown function ")), cast<char * const >::from(__name_rename_at_220_85)))));
    };
}

inline debug_eval::Result expr_value_ae10ccbfc8e1ce31 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_225_87 )
{
    AutoVariant<int64_t,int32_t,char *,char *,char *> __token_rename_at_226_88_ConstRef = ((AutoVariant<int64_t,int32_t,char *,char *,char *>)token_905886d3b1cc992c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_225_87)));
    AutoVariant<int64_t,int32_t,char *,char *,char *> const  & __token_rename_at_226_88 = __token_rename_at_226_88_ConstRef; ;
    if ( das_get_auto_variant_field<char *,3,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_226_88) )
    {
        return das_auto_cast_ref<debug_eval::Result>::cast(error_e5bb3044bbd89a1d(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_225_87),das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_7, cast<char * const >::from(((char *) "invalid token ")), cast<AutoVariant<int64_t,int32_t,char *,char *,char *> const  &>::from(__token_rename_at_226_88), cast<char * const >::from(((char *) ", expecting number or ("))))));
    } else if ( das_get_auto_variant_field<char *,4,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_226_88) )
    {
        return das_auto_cast_ref<debug_eval::Result>::cast(error_e5bb3044bbd89a1d(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_225_87),((char *) "unexpected end of stream")));
    } else if ( das_get_auto_variant_field<int64_t,0,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_226_88) )
    {
        return /* <- */ das_auto_cast_move<debug_eval::Result>::cast(Result_1f04b6f1f7809164(__context__,das_get_auto_variant_field<int64_t,0,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_226_88,__context__)));
    } else if ( das_get_auto_variant_field<char *,2,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_226_88) )
    {
        AutoVariant<int64_t,int32_t,char *,char *,char *> __ntoken_rename_at_238_89 = token_905886d3b1cc992c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_225_87));
        if ( das_null_coalescing<int32_t>::get(das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::safe_as(__ntoken_rename_at_238_89),0) == 40 )
        {
            debug_eval::Result __arg_rename_at_240_90_ConstRef = ((debug_eval::Result)expr_251518313273ca50(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_225_87)));
    debug_eval::Result const  & __arg_rename_at_240_90 = __arg_rename_at_240_90_ConstRef; ;
            das_copy(__ntoken_rename_at_238_89,token_905886d3b1cc992c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_225_87)));
            if ( das_null_coalescing<int32_t>::get(das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::safe_as(__ntoken_rename_at_238_89),0) != 41 )
            {
                return das_auto_cast_ref<debug_eval::Result>::cast(error_e5bb3044bbd89a1d(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_225_87),((char *) "expecting )")));
            } else {
                return das_auto_cast_ref<debug_eval::Result>::cast(func_call_34538813fbb8fc0f(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_225_87),das_get_auto_variant_field<char *,2,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_226_88,__context__),__arg_rename_at_240_90));
            };
        };
        unput_9f6508d6fd04dc86(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_225_87),das_arg<AutoVariant<int64_t,int32_t,char *,char *,char *>>::pass(__ntoken_rename_at_238_89));
        if ( SimPolicy<char *>::Equ(cast<char *>::from(das_get_auto_variant_field<char *,2,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_226_88,__context__)),cast<char *>::from(((char *) "true")),*__context__,nullptr) )
        {
            return /* <- */ das_auto_cast_move<debug_eval::Result>::cast(Result_73cdb19a79e62102(__context__,true));
        } else if ( SimPolicy<char *>::Equ(cast<char *>::from(das_get_auto_variant_field<char *,2,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_226_88,__context__)),cast<char *>::from(((char *) "false")),*__context__,nullptr) )
        {
            return /* <- */ das_auto_cast_move<debug_eval::Result>::cast(Result_73cdb19a79e62102(__context__,false));
        };
        if ( _FuncbuiltinTickkey_existsTick16808803843923989214_26b930b48ef88e1b(__context__,das_arg<TTable<char *,debug_eval::Result>>::pass(__st_rename_at_225_87.names),das_get_auto_variant_field<char *,2,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_226_88,__context__)) )
        {
            return das_auto_cast_ref<debug_eval::Result>::cast(_FuncbuiltinTickget_valueTick6803070933163225147_86faf0edd3fcebfe(__context__,das_arg<TTable<char *,debug_eval::Result>>::pass(__st_rename_at_225_87.names),das_get_auto_variant_field<char *,2,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_226_88,__context__)));
        } else {
            return das_auto_cast_ref<debug_eval::Result>::cast(error_e5bb3044bbd89a1d(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_225_87),das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_8, cast<char * const >::from(((char *) "unknown variable ")), cast<char * const >::from(das_get_auto_variant_field<char *,2,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_226_88,__context__))))));
        };
    } else if ( das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_226_88) && (das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_226_88,__context__) == 40) )
    {
        AutoVariant<int64_t,int32_t,char *,char *,char *> _temp_make_local_260_16_6; _temp_make_local_260_16_6;
        debug_eval::Result __res_rename_at_259_91 = expr_251518313273ca50(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_225_87));
        if ( das_null_coalescing<int32_t>::get(das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::safe_as((_temp_make_local_260_16_6 = (token_905886d3b1cc992c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_225_87))))),0) != 41 )
        {
            return das_auto_cast_ref<debug_eval::Result>::cast(error_e5bb3044bbd89a1d(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_225_87),((char *) "expecting )")));
        } else {
            return /* <- */ das_auto_cast_move<debug_eval::Result>::cast(__res_rename_at_259_91);
        };
    } else if ( das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_226_88) )
    {
        return das_auto_cast_ref<debug_eval::Result>::cast(error_e5bb3044bbd89a1d(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_225_87),das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_9, cast<char * const >::from(((char *) "unexpected character ")), cast<char * const >::from(((char * const )(to_string_char(das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_226_88,__context__),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) ", expecting number or ("))))));
    };
    return das_auto_cast_ref<debug_eval::Result>::cast(error_e5bb3044bbd89a1d(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_225_87),das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_10, cast<char * const >::from(((char *) "unexpected token ")), cast<AutoVariant<int64_t,int32_t,char *,char *,char *> const  &>::from(__token_rename_at_226_88), cast<char * const >::from(((char *) ", expecting number or ("))))));
}

inline AutoTuple<int32_t,TypeInfo> getStructFieldOffset_13b5c35ad8bad1b1 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_272_92, char * const  __ident_rename_at_272_93, debug_eval::Result &  __result_rename_at_272_94 )
{
    int32_t __size_rename_at_273_95 = 0;
    {
        bool __need_loop_274 = true;
        // field: rtti::VarInfo&
        das_iterator<StructInfo> __field_iterator(das_deref(__context__,((__result_rename_at_272_94.tinfo).getStructType())));
        VarInfo * __field_rename_at_274_96;
        __need_loop_274 = __field_iterator.first(__context__,(__field_rename_at_274_96)) && __need_loop_274;
        for ( ; __need_loop_274 ; __need_loop_274 = __field_iterator.next(__context__,(__field_rename_at_274_96)) )
        {
            TypeInfo * __tinfo_rename_at_275_97 = 0;
            das_copy(__tinfo_rename_at_275_97,das_cast<TypeInfo *>::cast(das_ref(__context__,(*__field_rename_at_274_96))));
            if ( SimPolicy<char *>::Equ(cast<char *>::from(((char * const )((*__field_rename_at_274_96).name /*name*/))),cast<char *>::from(__ident_rename_at_272_93),*__context__,nullptr) )
            {
                return das_auto_cast_ref<AutoTuple<int32_t,TypeInfo>>::cast((([&]() -> AutoTuple<int32_t,TypeInfo> {
                    AutoTuple<int32_t,TypeInfo> __mkt_280;
                    das_get_auto_tuple_field<int32_t,0,int32_t,TypeInfo>::get(__mkt_280) = __size_rename_at_273_95;
                    das_get_auto_tuple_field<TypeInfo,1,int32_t,TypeInfo>::get(__mkt_280) = das_deref(__context__,__tinfo_rename_at_275_97);
                    return __mkt_280;
                })()));
            } else {
                int32_t __al_rename_at_282_98 = ((int32_t)(getTypeAlign(__tinfo_rename_at_275_97) - 1));
                das_copy(__size_rename_at_273_95,(__size_rename_at_273_95 + __al_rename_at_282_98) & ~__al_rename_at_282_98);
                __size_rename_at_273_95 += getTypeSize(__tinfo_rename_at_275_97);
            };
        }
        __field_iterator.close(__context__,(__field_rename_at_274_96));
    };
    return das_auto_cast_ref<AutoTuple<int32_t,TypeInfo>>::cast((([&]() -> AutoTuple<int32_t,TypeInfo> {
        AutoTuple<int32_t,TypeInfo> __mkt_286;
        das_zero(__mkt_286);
        das_get_auto_tuple_field<int32_t,0,int32_t,TypeInfo>::get(__mkt_286) = -1;
        das_get_auto_tuple_field<TypeInfo,1,int32_t,TypeInfo>::get(__mkt_286) = (([&]() -> TypeInfo {
            TypeInfo __mks_286;
            das_zero(__mks_286);
            return __mks_286;
        })());
        return __mkt_286;
    })()));
}

inline AutoTuple<int32_t,TypeInfo> getTupleFieldOffset_843b15bad51bff07 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_289_99, char * const  __ident_rename_at_289_100, debug_eval::Result &  __result_rename_at_289_101 )
{
    int32_t __size_rename_at_290_102 = 0;
    {
        bool __need_loop_291 = true;
        // cnt: int const
        das_iterator<range> __cnt_iterator(mk_range(__result_rename_at_289_101.tinfo.argCount /*argCount*/));
        int32_t __cnt_rename_at_291_103;
        __need_loop_291 = __cnt_iterator.first(__context__,(__cnt_rename_at_291_103)) && __need_loop_291;
        for ( ; __need_loop_291 ; __need_loop_291 = __cnt_iterator.next(__context__,(__cnt_rename_at_291_103)) )
        {
            TypeInfo * __tinfo_rename_at_292_104 = ((TypeInfo *)das_index<TypeInfo * *>::at(__result_rename_at_289_101.tinfo.argTypes /*argTypes*/,__cnt_rename_at_291_103,__context__));
            if ( SimPolicy<char *>::Equ(cast<char *>::from(__ident_rename_at_289_100),cast<char *>::from(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_11, cast<char * const >::from(((char *) "_")), cast<int32_t>::from(__cnt_rename_at_291_103)))),*__context__,nullptr) )
            {
                return das_auto_cast_ref<AutoTuple<int32_t,TypeInfo>>::cast((([&]() -> AutoTuple<int32_t,TypeInfo> {
                    AutoTuple<int32_t,TypeInfo> __mkt_294;
                    das_get_auto_tuple_field<int32_t,0,int32_t,TypeInfo>::get(__mkt_294) = __size_rename_at_290_102;
                    das_get_auto_tuple_field<TypeInfo,1,int32_t,TypeInfo>::get(__mkt_294) = das_deref(__context__,__tinfo_rename_at_292_104);
                    return __mkt_294;
                })()));
            } else {
                int32_t __al_rename_at_296_105 = ((int32_t)(getTypeAlign(__tinfo_rename_at_292_104) - 1));
                das_copy(__size_rename_at_290_102,(__size_rename_at_290_102 + __al_rename_at_296_105) & ~__al_rename_at_296_105);
                __size_rename_at_290_102 += getTypeSize(__tinfo_rename_at_292_104);
            };
        }
        __cnt_iterator.close(__context__,(__cnt_rename_at_291_103));
    };
    return das_auto_cast_ref<AutoTuple<int32_t,TypeInfo>>::cast((([&]() -> AutoTuple<int32_t,TypeInfo> {
        AutoTuple<int32_t,TypeInfo> __mkt_300;
        das_zero(__mkt_300);
        das_get_auto_tuple_field<int32_t,0,int32_t,TypeInfo>::get(__mkt_300) = -1;
        das_get_auto_tuple_field<TypeInfo,1,int32_t,TypeInfo>::get(__mkt_300) = (([&]() -> TypeInfo {
            TypeInfo __mks_300;
            das_zero(__mks_300);
            return __mks_300;
        })());
        return __mkt_300;
    })()));
}

inline AutoTuple<int32_t,TypeInfo> getVectorOffset_dcf7cf4570b94b24 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_303_106, char * const  __ident_rename_at_303_107, debug_eval::Result &  __result_rename_at_303_108 )
{
    int32_t __ofs_rename_at_304_109 = ((int32_t)das_invoke_function<int32_t>::invoke<DAS_COMMENT(bound_enum) das::Type,char * const >(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::getVectorOffset CE<rtti::Type> Cs*/ 0x116dce136b7efa03)),__result_rename_at_303_108.tinfo.type /*basicType*/,__ident_rename_at_303_107));
    if ( __ofs_rename_at_304_109 < 0 )
    {
        return das_auto_cast_ref<AutoTuple<int32_t,TypeInfo>>::cast((([&]() -> AutoTuple<int32_t,TypeInfo> {
            AutoTuple<int32_t,TypeInfo> __mkt_305;
            das_zero(__mkt_305);
            das_get_auto_tuple_field<int32_t,0,int32_t,TypeInfo>::get(__mkt_305) = -1;
            das_get_auto_tuple_field<TypeInfo,1,int32_t,TypeInfo>::get(__mkt_305) = (([&]() -> TypeInfo {
                TypeInfo __mks_305;
                das_zero(__mks_305);
                return __mks_305;
            })());
            return __mkt_305;
        })()));
    } else {
        int32_t __fieldSize_rename_at_306_110 = ((int32_t)das_invoke_function<int32_t>::invoke<DAS_COMMENT(bound_enum) das::Type>(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::getVectorElementSize CE<rtti::Type>*/ 0x65e065298f6e4b80)),__result_rename_at_303_108.tinfo.type /*basicType*/));
        TypeInfo __tinfo_rename_at_307_111_ConstRef = ((TypeInfo)(([&]() -> TypeInfo {
            TypeInfo __mks_307;
            das_zero(__mks_307);
            das_copy((__mks_307.type /*basicType*/),(das_invoke_function<DAS_COMMENT(bound_enum) das::Type>::invoke<DAS_COMMENT(bound_enum) das::Type>(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::getVectorElementType CE<rtti::Type>*/ 0xf2e79698fe66ab15)),__result_rename_at_303_108.tinfo.type /*basicType*/)));
            das_copy((__mks_307.size /*size*/),(uint32_t(__fieldSize_rename_at_306_110)));
            return __mks_307;
        })()));
    TypeInfo const  & __tinfo_rename_at_307_111 = __tinfo_rename_at_307_111_ConstRef; ;
        return das_auto_cast_ref<AutoTuple<int32_t,TypeInfo>>::cast((([&]() -> AutoTuple<int32_t,TypeInfo> {
            AutoTuple<int32_t,TypeInfo> __mkt_311;
            das_get_auto_tuple_field<int32_t,0,int32_t,TypeInfo>::get(__mkt_311) = (__ofs_rename_at_304_109 * __fieldSize_rename_at_306_110);
            das_get_auto_tuple_field<TypeInfo,1,int32_t,TypeInfo>::get(__mkt_311) = __tinfo_rename_at_307_111;
            return __mkt_311;
        })()));
    };
}

inline AutoTuple<int32_t,TypeInfo> getHandleFieldOffset_4b8713168d14b603 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_314_112, char * const  __ident_rename_at_314_113, debug_eval::Result &  __result_rename_at_314_114 )
{
    smart_ptr_raw<TypeAnnotation> __ann_rename_at_316_115; das_zero(__ann_rename_at_316_115); das_move(__ann_rename_at_316_115, das_cast<smart_ptr_raw<TypeAnnotation>>::cast(((__result_rename_at_314_114.tinfo).getAnnotation())));
    if ( equ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__ann_rename_at_316_115),das_auto_cast<void * const >::cast(nullptr)) )
    {
        return das_auto_cast_ref<AutoTuple<int32_t,TypeInfo>>::cast((([&]() -> AutoTuple<int32_t,TypeInfo> {
            AutoTuple<int32_t,TypeInfo> __mkt_318;
            das_zero(__mkt_318);
            das_get_auto_tuple_field<int32_t,0,int32_t,TypeInfo>::get(__mkt_318) = -1;
            das_get_auto_tuple_field<TypeInfo,1,int32_t,TypeInfo>::get(__mkt_318) = (([&]() -> TypeInfo {
                TypeInfo __mks_318;
                das_zero(__mks_318);
                return __mks_318;
            })());
            return __mkt_318;
        })()));
    } else {
        int32_t __ofs_rename_at_320_116 = ((int32_t)int32_t(getHandledTypeFieldOffset(__ann_rename_at_316_115,__ident_rename_at_314_113,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))));
        if ( __ofs_rename_at_320_116 == -1 )
        {
            return das_auto_cast_ref<AutoTuple<int32_t,TypeInfo>>::cast((([&]() -> AutoTuple<int32_t,TypeInfo> {
                AutoTuple<int32_t,TypeInfo> __mkt_322;
                das_zero(__mkt_322);
                das_get_auto_tuple_field<int32_t,0,int32_t,TypeInfo>::get(__mkt_322) = -1;
                das_get_auto_tuple_field<TypeInfo,1,int32_t,TypeInfo>::get(__mkt_322) = (([&]() -> TypeInfo {
                    TypeInfo __mks_322;
                    das_zero(__mks_322);
                    return __mks_322;
                })());
                return __mkt_322;
            })()));
        } else {
            TypeInfo * __tinfo_rename_at_324_117 = ((TypeInfo *)getHandledTypeFieldType(__ann_rename_at_316_115,__ident_rename_at_314_113,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
            return das_auto_cast_ref<AutoTuple<int32_t,TypeInfo>>::cast((([&]() -> AutoTuple<int32_t,TypeInfo> {
                AutoTuple<int32_t,TypeInfo> __mkt_325;
                das_get_auto_tuple_field<int32_t,0,int32_t,TypeInfo>::get(__mkt_325) = __ofs_rename_at_320_116;
                das_get_auto_tuple_field<TypeInfo,1,int32_t,TypeInfo>::get(__mkt_325) = das_deref(__context__,__tinfo_rename_at_324_117);
                return __mkt_325;
            })()));
        };
    };
}

inline debug_eval::Result expr_field_5dfcd45b1ecd4499 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_329_118 )
{
    debug_eval::Result __result_rename_at_330_119 = expr_value_ae10ccbfc8e1ce31(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_329_118));
    AutoVariant<int64_t,int32_t,char *,char *,char *> __token_rename_at_331_120_ConstRef = ((AutoVariant<int64_t,int32_t,char *,char *,char *>)token_905886d3b1cc992c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_329_118)));
    AutoVariant<int64_t,int32_t,char *,char *,char *> const  & __token_rename_at_331_120 = __token_rename_at_331_120_ConstRef; ;
    if ( das_null_coalescing<int32_t>::get(das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::safe_as(__token_rename_at_331_120),0) == 46 )
    {
        AutoVariant<int64_t,int32_t,char *,char *,char *> __field_rename_at_333_121_ConstRef = ((AutoVariant<int64_t,int32_t,char *,char *,char *>)token_905886d3b1cc992c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_329_118)));
    AutoVariant<int64_t,int32_t,char *,char *,char *> const  & __field_rename_at_333_121 = __field_rename_at_333_121_ConstRef; ;
        if ( das_get_auto_variant_field<char *,2,int64_t,int32_t,char *,char *,char *>::is(__field_rename_at_333_121) )
        {
            AutoTuple<int32_t,TypeInfo> __oti_rename_at_335_122;das_zero(__oti_rename_at_335_122);
            das_copy(das_get_auto_tuple_field<int32_t,0,int32_t,TypeInfo>::get(__oti_rename_at_335_122),-1);
            if ( __result_rename_at_330_119.tinfo.dimSize /*dimSize*/ != 0x0u )
            {
                return das_auto_cast_ref<debug_eval::Result>::cast(error_e5bb3044bbd89a1d(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_329_118),((char *) "can't access field of array")));
            } else if ( __result_rename_at_330_119.tinfo.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tStructure )
            {
                das_copy(__oti_rename_at_335_122,getStructFieldOffset_13b5c35ad8bad1b1(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_329_118),das_get_auto_variant_field<char *,2,int64_t,int32_t,char *,char *,char *>::as(__field_rename_at_333_121,__context__),das_arg<debug_eval::Result>::pass(__result_rename_at_330_119)));
            } else if ( __result_rename_at_330_119.tinfo.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tTuple )
            {
                das_copy(__oti_rename_at_335_122,getTupleFieldOffset_843b15bad51bff07(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_329_118),das_get_auto_variant_field<char *,2,int64_t,int32_t,char *,char *,char *>::as(__field_rename_at_333_121,__context__),das_arg<debug_eval::Result>::pass(__result_rename_at_330_119)));
            } else if ( das_invoke_function<int32_t>::invoke<DAS_COMMENT(bound_enum) das::Type>(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::getVectorElementCount CE<rtti::Type>*/ 0x42f0d692c6e099b8)),__result_rename_at_330_119.tinfo.type /*basicType*/) != 0 )
            {
                das_copy(__oti_rename_at_335_122,getVectorOffset_dcf7cf4570b94b24(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_329_118),das_get_auto_variant_field<char *,2,int64_t,int32_t,char *,char *,char *>::as(__field_rename_at_333_121,__context__),das_arg<debug_eval::Result>::pass(__result_rename_at_330_119)));
            } else if ( __result_rename_at_330_119.tinfo.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tHandle )
            {
                das_copy(__oti_rename_at_335_122,getHandleFieldOffset_4b8713168d14b603(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_329_118),das_get_auto_variant_field<char *,2,int64_t,int32_t,char *,char *,char *>::as(__field_rename_at_333_121,__context__),das_arg<debug_eval::Result>::pass(__result_rename_at_330_119)));
            };
            if ( das_get_auto_tuple_field<int32_t,0,int32_t,TypeInfo>::get(__oti_rename_at_335_122) < 0 )
            {
                return das_auto_cast_ref<debug_eval::Result>::cast(error_e5bb3044bbd89a1d(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_329_118),das_string_builder(__context__,SimNode_AotInterop<4>(__tinfo_12, cast<char * const >::from(((char *) "unknown field ")), cast<AutoVariant<int64_t,int32_t,char *,char *,char *> const  &>::from(__field_rename_at_333_121), cast<char * const >::from(((char *) " in type ")), cast<DAS_COMMENT(bound_enum) das::Type>::from(__result_rename_at_330_119.tinfo.type /*basicType*/)))));
            } else {
                void * __newData_rename_at_357_123 = ((void *)getPD_4da28c77bbe6c3d7(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_329_118),das_arg<debug_eval::Result>::pass(__result_rename_at_330_119),das_get_auto_tuple_field<int32_t,0,int32_t,TypeInfo>::get(__oti_rename_at_335_122)));
                das_copy(__result_rename_at_330_119.data,__newData_rename_at_357_123);
                das_copy(__result_rename_at_330_119.tinfo,das_get_auto_tuple_field<TypeInfo,1,int32_t,TypeInfo>::get(__oti_rename_at_335_122));
                return das_auto_cast_ref<debug_eval::Result>::cast(__result_rename_at_330_119);
            };
        } else {
            return das_auto_cast_ref<debug_eval::Result>::cast(error_e5bb3044bbd89a1d(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_329_118),((char *) "expecting field name")));
        };
    } else {
        unput_9f6508d6fd04dc86(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_329_118),__token_rename_at_331_120);
    };
    return das_auto_cast_ref<debug_eval::Result>::cast(__result_rename_at_330_119);
}

inline debug_eval::Result opDim_b0bf78e5c93259f1 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_370_124, int64_t __index_rename_at_370_125, debug_eval::Result &  __result_rename_at_370_126 )
{
    int32_t __dim_rename_at_371_127 = ((int32_t)rtti_getDimTypeInfo(das_arg<TypeInfo>::pass(__result_rename_at_370_126.tinfo),int32_t(__result_rename_at_370_126.tinfo.dimSize /*dimSize*/ - 0x1u),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
    if ( (__index_rename_at_370_125 < INT64_C(0)) || (__index_rename_at_370_125 >= int64_t(__dim_rename_at_371_127)) )
    {
        return das_auto_cast_ref<debug_eval::Result>::cast(error_e5bb3044bbd89a1d(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_370_124),((char *) "index out of range")));
    } else {
        --__result_rename_at_370_126.tinfo.dimSize /*dimSize*/;
        int32_t __elemSize_rename_at_376_128 = ((int32_t)getTypeSize(das_ref(__context__,__result_rename_at_370_126.tinfo)));
        das_copy(__result_rename_at_370_126.data,getPD_4da28c77bbe6c3d7(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_370_124),das_arg<debug_eval::Result>::pass(__result_rename_at_370_126),int32_t(__index_rename_at_370_125) * __elemSize_rename_at_376_128));
        return das_auto_cast_ref<debug_eval::Result>::cast(__result_rename_at_370_126);
    };
}

inline debug_eval::Result opDimString_6ac245c533760d26 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_381_129, int64_t __index_rename_at_381_130, debug_eval::Result &  __result_rename_at_381_131 )
{
    char * __str_rename_at_382_132 = 0;
    if ( __result_rename_at_381_131.data != nullptr )
    {
        das_copy(__str_rename_at_382_132,das_deref(__context__,das_cast<char * *>::cast(__result_rename_at_381_131.data)));
    } else {
        das_copy(__str_rename_at_382_132,das_invoke_function<char * const >::invoke<float4>(__context__,nullptr,Func(__context__->fnByMangledName(/*@math_bits::cast_to_string Cf4*/ 0x3c261093455fad31)),__result_rename_at_381_131.value));
    };
    int32_t __len_rename_at_390_133 = ((int32_t)builtin_string_length(__str_rename_at_382_132,__context__));
    if ( __index_rename_at_381_130 >= int64_t(__len_rename_at_390_133) )
    {
        return das_auto_cast_ref<debug_eval::Result>::cast(error_e5bb3044bbd89a1d(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_381_129),das_string_builder(__context__,SimNode_AotInterop<5>(__tinfo_13, cast<char * const >::from(((char *) "index out of range ")), cast<int64_t>::from(__index_rename_at_381_130), cast<char * const >::from(((char *) " of ")), cast<int32_t>::from(__len_rename_at_390_133), cast<char * const >::from(((char *) " (string too short)"))))));
    } else {
        return das_auto_cast_ref<debug_eval::Result>::cast(Result_1f04b6f1f7809164(__context__,int64_t(get_character_at(__str_rename_at_382_132,int32_t(__index_rename_at_381_130),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
    };
}

inline debug_eval::Result opArray_f74b354d6cd2ce02 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_397_134, int64_t __index_rename_at_397_135, debug_eval::Result &  __result_rename_at_397_136 )
{
    debugapi::DapiArray * __pArray_rename_at_399_137 = ((debugapi::DapiArray *)das_cast<debugapi::DapiArray *>::cast(getPD_4da28c77bbe6c3d7(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_397_134),das_arg<debug_eval::Result>::pass(__result_rename_at_397_136),0)));
    if ( __pArray_rename_at_399_137 == nullptr )
    {
        return das_auto_cast_ref<debug_eval::Result>::cast(error_e5bb3044bbd89a1d(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_397_134),((char *) "null pointer")));
    } else {
        uint32_t __dim_rename_at_403_138 = ((uint32_t)__pArray_rename_at_399_137->size);
        if ( (__index_rename_at_397_135 < INT64_C(0)) || (__index_rename_at_397_135 >= int64_t(__dim_rename_at_403_138)) )
        {
            return das_auto_cast_ref<debug_eval::Result>::cast(error_e5bb3044bbd89a1d(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_397_134),((char *) "index out of range")));
        } else {
            int8_t * __pData_rename_at_407_139 = ((int8_t *)das_cast<int8_t *>::cast(__pArray_rename_at_399_137->data));
            int32_t __elemSize_rename_at_408_140 = ((int32_t)getTypeSize(__result_rename_at_397_136.tinfo.firstType /*firstType*/));
            int32_t __offset_rename_at_409_141 = ((int32_t)(int32_t(__index_rename_at_397_135) * __elemSize_rename_at_408_140));
            das_copy(__result_rename_at_397_136.data,das_ptr_add_int32(__pData_rename_at_407_139,__offset_rename_at_409_141,1));
            das_copy(__result_rename_at_397_136.tinfo,das_deref(__context__,__result_rename_at_397_136.tinfo.firstType /*firstType*/));
            return das_auto_cast_ref<debug_eval::Result>::cast(__result_rename_at_397_136);
        };
    };
}

inline debug_eval::Result opTable_b6c6c7c404d92dc9 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_416_142, debug_eval::Result const  &  __index_rename_at_416_143, debug_eval::Result &  __result_rename_at_416_144 )
{
    debugapi::DapiTable * __pTable_rename_at_418_145 = ((debugapi::DapiTable *)das_cast<debugapi::DapiTable *>::cast(getPD_4da28c77bbe6c3d7(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_416_142),das_arg<debug_eval::Result>::pass(__result_rename_at_416_144),0)));
    if ( __pTable_rename_at_418_145 == nullptr )
    {
        return das_auto_cast_ref<debug_eval::Result>::cast(error_e5bb3044bbd89a1d(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_416_142),((char *) "table - null pointer")));
    } else {
        float4 * __pKey_rename_at_422_146 = ((float4 *)das_cast<float4 *>::cast(getPD_4da28c77bbe6c3d7(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_416_142),__index_rename_at_416_143,0)));
        if ( __pKey_rename_at_422_146 == nullptr )
        {
            return das_auto_cast_ref<debug_eval::Result>::cast(error_e5bb3044bbd89a1d(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_416_142),((char *) "key - null pointer")));
        } else {
            float4 __key_rename_at_426_147 = ((float4)das_deref(__context__,__pKey_rename_at_422_146));
            DAS_COMMENT(bound_enum) das::Type __keyBaseType_rename_at_427_148 = ((DAS_COMMENT(bound_enum) das::Type)__result_rename_at_416_144.tinfo.firstType /*firstType*/->type /*basicType*/);
            int32_t __valueTypeSize_rename_at_428_149 = ((int32_t)getTypeSize(__result_rename_at_416_144.tinfo.secondType /*secondType*/));
            int32_t __elementIndex_rename_at_429_150 = ((int32_t)rtti_getTablePtr(das_auto_cast<void * const >::cast(__pTable_rename_at_418_145),cast<float4>::from(__key_rename_at_426_147),__keyBaseType_rename_at_427_148,__valueTypeSize_rename_at_428_149,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
            if ( __elementIndex_rename_at_429_150 == -1 )
            {
                return das_auto_cast_ref<debug_eval::Result>::cast(error_e5bb3044bbd89a1d(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_416_142),((char *) "key not found")));
            } else {
                uint8_t * __pValue_rename_at_433_151 = ((uint8_t *)das_ptr_add_int32(das_cast<uint8_t *>::cast(__pTable_rename_at_418_145->data),__elementIndex_rename_at_429_150 * __valueTypeSize_rename_at_428_149,1));
                das_copy(__result_rename_at_416_144.data,__pValue_rename_at_433_151);
                das_copy(__result_rename_at_416_144.tinfo,das_deref(__context__,__result_rename_at_416_144.tinfo.secondType /*secondType*/));
                return das_auto_cast_ref<debug_eval::Result>::cast(__result_rename_at_416_144);
            };
        };
    };
}

inline debug_eval::Result expr_at_56cc60f4051344c2 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_440_152 )
{
    debug_eval::Result __result_rename_at_441_153 = expr_field_5dfcd45b1ecd4499(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_440_152));
    AutoVariant<int64_t,int32_t,char *,char *,char *> __token_rename_at_442_154 = token_905886d3b1cc992c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_440_152));
    while ( das_null_coalescing<int32_t>::get(das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::safe_as(__token_rename_at_442_154),0) == 91 )
    {
        AutoVariant<int64_t,int32_t,char *,char *,char *> _temp_make_local_445_12_7; _temp_make_local_445_12_7;
        debug_eval::Result __eindex_rename_at_444_155_ConstRef = ((debug_eval::Result)expr_251518313273ca50(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_440_152)));
    debug_eval::Result const  & __eindex_rename_at_444_155 = __eindex_rename_at_444_155_ConstRef; ;
        if ( das_null_coalescing<int32_t>::get(das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::safe_as((_temp_make_local_445_12_7 = (token_905886d3b1cc992c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_440_152))))),0) != 93 )
        {
            return das_auto_cast_ref<debug_eval::Result>::cast(error_e5bb3044bbd89a1d(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_440_152),((char *) "expecting ]")));
        } else {
            debug_eval::Result __nresult_rename_at_448_156;das_zero(__nresult_rename_at_448_156);
            if ( (__result_rename_at_441_153.tinfo.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tString) && (__result_rename_at_441_153.tinfo.dimSize /*dimSize*/ == 0x0u) )
            {
                das_copy(__nresult_rename_at_448_156,opDimString_6ac245c533760d26(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_440_152),getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_440_152),__eindex_rename_at_444_155),das_arg<debug_eval::Result>::pass(__result_rename_at_441_153)));
            } else if ( (__result_rename_at_441_153.tinfo.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tArray) && (__result_rename_at_441_153.tinfo.dimSize /*dimSize*/ == 0x0u) )
            {
                das_copy(__nresult_rename_at_448_156,opArray_f74b354d6cd2ce02(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_440_152),getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_440_152),__eindex_rename_at_444_155),das_arg<debug_eval::Result>::pass(__result_rename_at_441_153)));
            } else if ( (__result_rename_at_441_153.tinfo.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tTable) && (__result_rename_at_441_153.tinfo.dimSize /*dimSize*/ == 0x0u) )
            {
                das_copy(__nresult_rename_at_448_156,opTable_b6c6c7c404d92dc9(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_440_152),__eindex_rename_at_444_155,das_arg<debug_eval::Result>::pass(__result_rename_at_441_153)));
            } else if ( __result_rename_at_441_153.tinfo.dimSize /*dimSize*/ == 0x0u )
            {
                return das_auto_cast_ref<debug_eval::Result>::cast(error_e5bb3044bbd89a1d(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_440_152),((char *) "can't index non-array")));
            } else {
                das_copy(__nresult_rename_at_448_156,opDim_b0bf78e5c93259f1(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_440_152),getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_440_152),__eindex_rename_at_444_155),das_arg<debug_eval::Result>::pass(__result_rename_at_441_153)));
            };
            das_copy(__result_rename_at_441_153,__nresult_rename_at_448_156);
            das_copy(__token_rename_at_442_154,token_905886d3b1cc992c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_440_152)));
        };
    };
    unput_9f6508d6fd04dc86(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_440_152),das_arg<AutoVariant<int64_t,int32_t,char *,char *,char *>>::pass(__token_rename_at_442_154));
    return das_auto_cast_ref<debug_eval::Result>::cast(__result_rename_at_441_153);
}

inline debug_eval::Result expr_unary_a06fc7e0b3c8f97 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_473_157 )
{
    AutoVariant<int64_t,int32_t,char *,char *,char *> __token_rename_at_474_158_ConstRef = ((AutoVariant<int64_t,int32_t,char *,char *,char *>)token_905886d3b1cc992c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_473_157)));
    AutoVariant<int64_t,int32_t,char *,char *,char *> const  & __token_rename_at_474_158 = __token_rename_at_474_158_ConstRef; ;
    if ( das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_474_158) && (das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_474_158,__context__) == 43) )
    {
        debug_eval::Result _temp_make_local_477_37_8; _temp_make_local_477_37_8;
        return das_auto_cast_ref<debug_eval::Result>::cast(Result_1f04b6f1f7809164(__context__,getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_473_157),das_arg<debug_eval::Result>::pass((_temp_make_local_477_37_8 = (expr_unary_a06fc7e0b3c8f97(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_473_157))))))));
    } else if ( das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_474_158) && (das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_474_158,__context__) == 45) )
    {
        debug_eval::Result _temp_make_local_480_38_9; _temp_make_local_480_38_9;
        return das_auto_cast_ref<debug_eval::Result>::cast(Result_1f04b6f1f7809164(__context__,-getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_473_157),das_arg<debug_eval::Result>::pass((_temp_make_local_480_38_9 = (expr_unary_a06fc7e0b3c8f97(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_473_157))))))));
    } else if ( das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_474_158) && (das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_474_158,__context__) == 126) )
    {
        debug_eval::Result _temp_make_local_483_38_10; _temp_make_local_483_38_10;
        return das_auto_cast_ref<debug_eval::Result>::cast(Result_1f04b6f1f7809164(__context__,~getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_473_157),das_arg<debug_eval::Result>::pass((_temp_make_local_483_38_10 = (expr_unary_a06fc7e0b3c8f97(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_473_157))))))));
    } else if ( das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_474_158) && (das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_474_158,__context__) == 33) )
    {
        debug_eval::Result _temp_make_local_486_37_11; _temp_make_local_486_37_11;
        return das_auto_cast_ref<debug_eval::Result>::cast(Result_73cdb19a79e62102(__context__,getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_473_157),das_arg<debug_eval::Result>::pass((_temp_make_local_486_37_11 = (expr_unary_a06fc7e0b3c8f97(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_473_157)))))) == INT64_C(0)));
    };
    unput_9f6508d6fd04dc86(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_473_157),__token_rename_at_474_158);
    return das_auto_cast_ref<debug_eval::Result>::cast(expr_at_56cc60f4051344c2(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_473_157)));
}

inline debug_eval::Result expr_mul_div_d2e41a242a5717c8 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_493_159 )
{
    debug_eval::Result __leftValue_rename_at_494_160 = expr_unary_a06fc7e0b3c8f97(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_493_159));
    AutoVariant<int64_t,int32_t,char *,char *,char *> __token_rename_at_495_161_ConstRef = ((AutoVariant<int64_t,int32_t,char *,char *,char *>)token_905886d3b1cc992c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_493_159)));
    AutoVariant<int64_t,int32_t,char *,char *,char *> const  & __token_rename_at_495_161 = __token_rename_at_495_161_ConstRef; ;
    if ( das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_495_161) && (das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_495_161,__context__) == 42) )
    {
        debug_eval::Result _temp_make_local_498_61_12; _temp_make_local_498_61_12;
        return das_auto_cast_ref<debug_eval::Result>::cast(Result_1f04b6f1f7809164(__context__,getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_493_159),das_arg<debug_eval::Result>::pass(__leftValue_rename_at_494_160)) * getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_493_159),das_arg<debug_eval::Result>::pass((_temp_make_local_498_61_12 = (expr_mul_div_d2e41a242a5717c8(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_493_159))))))));
    } else if ( das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_495_161) && (das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_495_161,__context__) == 47) )
    {
        debug_eval::Result _temp_make_local_501_40_13; _temp_make_local_501_40_13;
        int64_t __rightValue_rename_at_501_162 = ((int64_t)getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_493_159),das_arg<debug_eval::Result>::pass((_temp_make_local_501_40_13 = (expr_mul_div_d2e41a242a5717c8(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_493_159)))))));
        if ( __rightValue_rename_at_501_162 == INT64_C(0) )
        {
            return das_auto_cast_ref<debug_eval::Result>::cast(error_e5bb3044bbd89a1d(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_493_159),((char *) "runtime error, division by 0")));
        } else {
            return das_auto_cast_ref<debug_eval::Result>::cast(Result_1f04b6f1f7809164(__context__,getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_493_159),das_arg<debug_eval::Result>::pass(__leftValue_rename_at_494_160)) * __rightValue_rename_at_501_162));
        };
    } else {
        unput_9f6508d6fd04dc86(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_493_159),__token_rename_at_495_161);
    };
    return /* <- */ das_auto_cast_move<debug_eval::Result>::cast(__leftValue_rename_at_494_160);
}

inline debug_eval::Result expr_add_sub_98f296ed1fed65d7 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_514_163 )
{
    debug_eval::Result __leftValue_rename_at_515_164 = expr_mul_div_d2e41a242a5717c8(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_514_163));
    AutoVariant<int64_t,int32_t,char *,char *,char *> __token_rename_at_516_165_ConstRef = ((AutoVariant<int64_t,int32_t,char *,char *,char *>)token_905886d3b1cc992c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_514_163)));
    AutoVariant<int64_t,int32_t,char *,char *,char *> const  & __token_rename_at_516_165 = __token_rename_at_516_165_ConstRef; ;
    if ( das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_516_165) && (das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_516_165,__context__) == 43) )
    {
        debug_eval::Result _temp_make_local_519_61_14; _temp_make_local_519_61_14;
        return das_auto_cast_ref<debug_eval::Result>::cast(Result_1f04b6f1f7809164(__context__,getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_514_163),das_arg<debug_eval::Result>::pass(__leftValue_rename_at_515_164)) + getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_514_163),das_arg<debug_eval::Result>::pass((_temp_make_local_519_61_14 = (expr_add_sub_98f296ed1fed65d7(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_514_163))))))));
    } else if ( das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_516_165) && (das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_516_165,__context__) == 45) )
    {
        debug_eval::Result _temp_make_local_522_61_15; _temp_make_local_522_61_15;
        return das_auto_cast_ref<debug_eval::Result>::cast(Result_1f04b6f1f7809164(__context__,getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_514_163),das_arg<debug_eval::Result>::pass(__leftValue_rename_at_515_164)) - getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_514_163),das_arg<debug_eval::Result>::pass((_temp_make_local_522_61_15 = (expr_add_sub_98f296ed1fed65d7(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_514_163))))))));
    } else {
        unput_9f6508d6fd04dc86(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_514_163),__token_rename_at_516_165);
    };
    return /* <- */ das_auto_cast_move<debug_eval::Result>::cast(__leftValue_rename_at_515_164);
}

inline bool try_accept_punkt_36a058d72081e56e ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_531_166, int32_t __punkt_rename_at_531_167 )
{
    AutoVariant<int64_t,int32_t,char *,char *,char *> __token_rename_at_532_168_ConstRef = ((AutoVariant<int64_t,int32_t,char *,char *,char *>)token_905886d3b1cc992c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_531_166)));
    AutoVariant<int64_t,int32_t,char *,char *,char *> const  & __token_rename_at_532_168 = __token_rename_at_532_168_ConstRef; ;
    if ( das_null_coalescing<int32_t>::get(das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::safe_as(__token_rename_at_532_168),0) != __punkt_rename_at_531_167 )
    {
        unput_9f6508d6fd04dc86(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_531_166),__token_rename_at_532_168);
        return das_auto_cast<bool>::cast(false);
    } else {
        return das_auto_cast<bool>::cast(true);
    };
}

inline debug_eval::Result expr_and_d5d8333e25dfcbe4 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_540_169 )
{
    debug_eval::Result __leftValue_rename_at_541_170 = expr_add_sub_98f296ed1fed65d7(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_540_169));
    if ( try_accept_punkt_36a058d72081e56e(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_540_169),38) )
    {
        if ( try_accept_punkt_36a058d72081e56e(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_540_169),38) )
        {
            debug_eval::Result _temp_make_local_544_62_16; _temp_make_local_544_62_16;
            return das_auto_cast_ref<debug_eval::Result>::cast(Result_73cdb19a79e62102(__context__,(getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_540_169),das_arg<debug_eval::Result>::pass(__leftValue_rename_at_541_170)) & getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_540_169),das_arg<debug_eval::Result>::pass((_temp_make_local_544_62_16 = (expr_and_d5d8333e25dfcbe4(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_540_169))))))) != INT64_C(0)));
        } else {
            debug_eval::Result _temp_make_local_546_61_17; _temp_make_local_546_61_17;
            return das_auto_cast_ref<debug_eval::Result>::cast(Result_1f04b6f1f7809164(__context__,getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_540_169),das_arg<debug_eval::Result>::pass(__leftValue_rename_at_541_170)) & getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_540_169),das_arg<debug_eval::Result>::pass((_temp_make_local_546_61_17 = (expr_and_d5d8333e25dfcbe4(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_540_169))))))));
        };
    };
    return /* <- */ das_auto_cast_move<debug_eval::Result>::cast(__leftValue_rename_at_541_170);
}

inline debug_eval::Result expr_xor_98afb583f1a5a9a5 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_552_171 )
{
    debug_eval::Result __leftValue_rename_at_553_172 = expr_and_d5d8333e25dfcbe4(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_552_171));
    if ( try_accept_punkt_36a058d72081e56e(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_552_171),94) )
    {
        if ( try_accept_punkt_36a058d72081e56e(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_552_171),94) )
        {
            debug_eval::Result _temp_make_local_556_62_18; _temp_make_local_556_62_18;
            return das_auto_cast_ref<debug_eval::Result>::cast(Result_73cdb19a79e62102(__context__,(getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_552_171),das_arg<debug_eval::Result>::pass(__leftValue_rename_at_553_172)) ^ getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_552_171),das_arg<debug_eval::Result>::pass((_temp_make_local_556_62_18 = (expr_xor_98afb583f1a5a9a5(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_552_171))))))) != INT64_C(0)));
        } else {
            debug_eval::Result _temp_make_local_558_61_19; _temp_make_local_558_61_19;
            return das_auto_cast_ref<debug_eval::Result>::cast(Result_1f04b6f1f7809164(__context__,getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_552_171),das_arg<debug_eval::Result>::pass(__leftValue_rename_at_553_172)) ^ getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_552_171),das_arg<debug_eval::Result>::pass((_temp_make_local_558_61_19 = (expr_xor_98afb583f1a5a9a5(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_552_171))))))));
        };
    };
    return /* <- */ das_auto_cast_move<debug_eval::Result>::cast(__leftValue_rename_at_553_172);
}

inline debug_eval::Result expr_or_d504d6b12b7ceea7 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_564_173 )
{
    debug_eval::Result __leftValue_rename_at_565_174 = expr_xor_98afb583f1a5a9a5(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_564_173));
    if ( try_accept_punkt_36a058d72081e56e(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_564_173),124) )
    {
        if ( try_accept_punkt_36a058d72081e56e(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_564_173),124) )
        {
            debug_eval::Result _temp_make_local_568_62_20; _temp_make_local_568_62_20;
            return das_auto_cast_ref<debug_eval::Result>::cast(Result_73cdb19a79e62102(__context__,(getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_564_173),das_arg<debug_eval::Result>::pass(__leftValue_rename_at_565_174)) | getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_564_173),das_arg<debug_eval::Result>::pass((_temp_make_local_568_62_20 = (expr_or_d504d6b12b7ceea7(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_564_173))))))) != INT64_C(0)));
        } else {
            debug_eval::Result _temp_make_local_570_61_21; _temp_make_local_570_61_21;
            return das_auto_cast_ref<debug_eval::Result>::cast(Result_1f04b6f1f7809164(__context__,getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_564_173),das_arg<debug_eval::Result>::pass(__leftValue_rename_at_565_174)) | getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_564_173),das_arg<debug_eval::Result>::pass((_temp_make_local_570_61_21 = (expr_or_d504d6b12b7ceea7(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_564_173))))))));
        };
    };
    return /* <- */ das_auto_cast_move<debug_eval::Result>::cast(__leftValue_rename_at_565_174);
}

inline debug_eval::Result expr_bool_op_c0cbada86a1585de ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_576_175 )
{
    debug_eval::Result __leftValue_rename_at_577_176 = expr_or_d504d6b12b7ceea7(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175));
    AutoVariant<int64_t,int32_t,char *,char *,char *> __token_rename_at_578_177_ConstRef = ((AutoVariant<int64_t,int32_t,char *,char *,char *>)token_905886d3b1cc992c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175)));
    AutoVariant<int64_t,int32_t,char *,char *,char *> const  & __token_rename_at_578_177 = __token_rename_at_578_177_ConstRef; ;
    AutoVariant<int64_t,int32_t,char *,char *,char *> __subtoken_rename_at_579_178_ConstRef = ((AutoVariant<int64_t,int32_t,char *,char *,char *>)token_905886d3b1cc992c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175)));
    AutoVariant<int64_t,int32_t,char *,char *,char *> const  & __subtoken_rename_at_579_178 = __subtoken_rename_at_579_178_ConstRef; ;
    if ( das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_578_177) && ((das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_578_177,__context__) == 33) && (das_null_coalescing<int32_t>::get(das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::safe_as(__subtoken_rename_at_579_178),0) == 61)) )
    {
        debug_eval::Result _temp_make_local_582_62_22; _temp_make_local_582_62_22;
        return das_auto_cast_ref<debug_eval::Result>::cast(Result_73cdb19a79e62102(__context__,SimPolicy<char *>::NotEqu(cast<char *>::from(getT_9627d57179cddca6(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175),das_arg<debug_eval::Result>::pass(__leftValue_rename_at_577_176))),cast<char *>::from(getT_9627d57179cddca6(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175),das_arg<debug_eval::Result>::pass((_temp_make_local_582_62_22 = (expr_bool_op_c0cbada86a1585de(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175))))))),*__context__,nullptr)));
    } else if ( das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_578_177) && ((das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_578_177,__context__) == 61) && (das_null_coalescing<int32_t>::get(das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::safe_as(__subtoken_rename_at_579_178),0) == 61)) )
    {
        debug_eval::Result _temp_make_local_585_62_23; _temp_make_local_585_62_23;
        return das_auto_cast_ref<debug_eval::Result>::cast(Result_73cdb19a79e62102(__context__,SimPolicy<char *>::Equ(cast<char *>::from(getT_9627d57179cddca6(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175),das_arg<debug_eval::Result>::pass(__leftValue_rename_at_577_176))),cast<char *>::from(getT_9627d57179cddca6(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175),das_arg<debug_eval::Result>::pass((_temp_make_local_585_62_23 = (expr_bool_op_c0cbada86a1585de(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175))))))),*__context__,nullptr)));
    } else if ( das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_578_177) && ((das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_578_177,__context__) == 60) && (das_null_coalescing<int32_t>::get(das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::safe_as(__subtoken_rename_at_579_178),0) == 61)) )
    {
        debug_eval::Result _temp_make_local_588_62_24; _temp_make_local_588_62_24;
        return das_auto_cast_ref<debug_eval::Result>::cast(Result_73cdb19a79e62102(__context__,getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175),das_arg<debug_eval::Result>::pass(__leftValue_rename_at_577_176)) <= getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175),das_arg<debug_eval::Result>::pass((_temp_make_local_588_62_24 = (expr_bool_op_c0cbada86a1585de(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175))))))));
    } else if ( das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_578_177) && (das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_578_177,__context__) == 60) )
    {
        debug_eval::Result _temp_make_local_592_61_25; _temp_make_local_592_61_25;
        unput_9f6508d6fd04dc86(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175),__subtoken_rename_at_579_178);
        return das_auto_cast_ref<debug_eval::Result>::cast(Result_73cdb19a79e62102(__context__,getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175),das_arg<debug_eval::Result>::pass(__leftValue_rename_at_577_176)) < getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175),das_arg<debug_eval::Result>::pass((_temp_make_local_592_61_25 = (expr_bool_op_c0cbada86a1585de(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175))))))));
    } else if ( das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_578_177) && ((das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_578_177,__context__) == 62) && (das_null_coalescing<int32_t>::get(das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::safe_as(__subtoken_rename_at_579_178),0) == 61)) )
    {
        debug_eval::Result _temp_make_local_595_62_26; _temp_make_local_595_62_26;
        return das_auto_cast_ref<debug_eval::Result>::cast(Result_73cdb19a79e62102(__context__,getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175),das_arg<debug_eval::Result>::pass(__leftValue_rename_at_577_176)) >= getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175),das_arg<debug_eval::Result>::pass((_temp_make_local_595_62_26 = (expr_bool_op_c0cbada86a1585de(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175))))))));
    } else if ( das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_578_177) && (das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_578_177,__context__) == 62) )
    {
        debug_eval::Result _temp_make_local_599_61_27; _temp_make_local_599_61_27;
        unput_9f6508d6fd04dc86(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175),__subtoken_rename_at_579_178);
        return das_auto_cast_ref<debug_eval::Result>::cast(Result_73cdb19a79e62102(__context__,getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175),das_arg<debug_eval::Result>::pass(__leftValue_rename_at_577_176)) > getI_5e1fded1da82fe11(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175),das_arg<debug_eval::Result>::pass((_temp_make_local_599_61_27 = (expr_bool_op_c0cbada86a1585de(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175))))))));
    } else {
        unput_9f6508d6fd04dc86(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175),__subtoken_rename_at_579_178);
        unput_9f6508d6fd04dc86(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175),__token_rename_at_578_177);
    };
    return /* <- */ das_auto_cast_move<debug_eval::Result>::cast(__leftValue_rename_at_577_176);
}

inline debug_eval::Result expr_251518313273ca50 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_609_179 )
{
    return das_auto_cast_ref<debug_eval::Result>::cast(expr_bool_op_c0cbada86a1585de(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_609_179)));
}

inline debug_eval::Result debug_eval_25c7c1861470fbfd ( Context * __context__, TTable<char *,debug_eval::Result> const  &  __context_rename_at_613_180, char * const  __expr_rename_at_613_181 )
{
    debug_eval::TokenStream __st_rename_at_614_182; das_zero(__st_rename_at_614_182); das_move(__st_rename_at_614_182, TokenStream_ae362449c106dfa(__context__,__expr_rename_at_613_181));
    _FuncbuiltinTickcloneTick3147220447302434744_70013ebd41b82ce5(__context__,das_arg<TTable<char *,debug_eval::Result>>::pass(__st_rename_at_614_182.names),__context_rename_at_613_180);
    debug_eval::Result __result_rename_at_616_183 = expr_251518313273ca50(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_614_182));
    AutoVariant<int64_t,int32_t,char *,char *,char *> __token_rename_at_617_184_ConstRef = ((AutoVariant<int64_t,int32_t,char *,char *,char *>)token_905886d3b1cc992c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_614_182)));
    AutoVariant<int64_t,int32_t,char *,char *,char *> const  & __token_rename_at_617_184 = __token_rename_at_617_184_ConstRef; ;
    if ( !das_get_auto_variant_field<char *,4,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_617_184) )
    {
        return das_auto_cast_ref<debug_eval::Result>::cast((([&]() -> debug_eval::Result {
            debug_eval::Result __mks_619;
            das_zero(__mks_619);
            das_copy((__mks_619.error),(das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_14, cast<char * const >::from(((char *) "unexpected token ")), cast<AutoVariant<int64_t,int32_t,char *,char *,char *> const  &>::from(__token_rename_at_617_184)))));
            return __mks_619;
        })()));
    } else {
        if ( !_FuncbuiltinTickemptyTick15399874715904164783_e1ebf91008e61bc3(__context__,das_arg<TArray<char *>>::pass(__st_rename_at_614_182.errors)) )
        {
            return das_auto_cast_ref<debug_eval::Result>::cast((([&]() -> debug_eval::Result {
                debug_eval::Result __mks_622;
                das_zero(__mks_622);
                das_copy((__mks_622.error),(__st_rename_at_614_182.errors(0,__context__)));
                return __mks_622;
            })()));
        } else {
            return /* <- */ das_auto_cast_move<debug_eval::Result>::cast(__result_rename_at_616_183);
        };
    };
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x3064482770f873f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_27c833578baf1b20>>();
    };
    aotLib[0x18a81095ec1dc5c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicknextTick17450348357676149856_8b0be63689a99d63>>();
    };
    aotLib[0xc401756aa93a8971] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_9eb6d916b60f25b7>>();
    };
    aotLib[0xfc2bb60986600e36] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_2182b904ebc5e479>>();
    };
    aotLib[0x9561c37eb8e6c7c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickclearTick13739625760977891612_6523d27c25fcd504>>();
    };
    aotLib[0x5eb0d019f2bf0bab] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickkeysTick2205854368403803976_b9582efb4c0d7da2>>();
    };
    aotLib[0xa05242cf12b993fc] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1935193042646774172_a3ae7bd938ff3d33>>();
    };
    aotLib[0x1254988cea61a1ff] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickeachTick9663565701927713696_1f90241636846e>>();
    };
    aotLib[0x8e947a8fbf380450] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_70b51b4c8469b8c5>>();
    };
    aotLib[0xd9ad7be370010939] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickkey_existsTick16808803843923989214_26b930b48ef88e1b>>();
    };
    aotLib[0x9324733346291808] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickget_valueTick6803070933163225147_86faf0edd3fcebfe>>();
    };
    aotLib[0x2f8e1919b0da4d76] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickeachTick4044332007967089362_e3640663f3ad1647>>();
    };
    aotLib[0xe3dab00cc63b5b3e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick14133213201864676143_fb343dce7d417ea7>>();
    };
    aotLib[0xf762de9af1246b7f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Func_lambda_debug_eval_26_1Tickfunction_96d8425d73290f4c>>();
    };
    aotLib[0xcf210e3648b8332d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Func_lambda_debug_eval_26_1Tickfinalizer_c96bf990405645ee>>();
    };
    aotLib[0xdbbf413b0effc66c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickemptyTick15399874715904164783_f42072dd005df0e5>>();
    };
    aotLib[0x9073c4efbfab31dc] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicknextTick17450348357676149856_5b50b5e96d75f745>>();
    };
    aotLib[0xe8c225b14cd33497] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickbackTick18296309835877697278_ef4cec92e917281d>>();
    };
    aotLib[0x653603c9211b5d7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpopTick1161079256290593740_474fdde50584fe9b>>();
    };
    aotLib[0x816ff8df98c71375] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick14133213201864676143_e139faaef2376ad9>>();
    };
    aotLib[0x2a4c59cf73ce841e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick14133213201864676143_9ba16521459d7b1c>>();
    };
    aotLib[0x8817c9f4046b0b6e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcdebug_evalTickloadITick653575658332832056_4fd755c4a7fd2bd9>>();
    };
    aotLib[0x17e3a67e09cbab4c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcdebug_evalTickloadITick653575658332832056_ef3b66a87599345e>>();
    };
    aotLib[0x5d82a01d4ee777a1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcdebug_evalTickloadITick653575658332832056_83c53155ffab2f2d>>();
    };
    aotLib[0xada0e5a12325d04b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcdebug_evalTickloadITick653575658332832056_c36fa462805fa187>>();
    };
    aotLib[0xe611d6645cd895de] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcdebug_evalTickloadITick653575658332832056_d898bb5d27c76a23>>();
    };
    aotLib[0x2c7cea6059829d9b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickcloneTick3147220447302434744_70013ebd41b82ce5>>();
    };
    aotLib[0xe706c1fa3cb11ee0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickemptyTick15399874715904164783_e1ebf91008e61bc3>>();
    };
    aotLib[0xf36891c5e3c08470] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&each_token_e5227dee8193fe81>>();
    };
    aotLib[0xe20c93095a034513] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&TokenStream_ae362449c106dfa>>();
    };
    aotLib[0xea6f55bbe165818a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&token_905886d3b1cc992c>>();
    };
    aotLib[0x9a93399250d4d63a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&unput_9f6508d6fd04dc86>>();
    };
    aotLib[0x47feaf4bbafd228f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&Result_1f04b6f1f7809164>>();
    };
    aotLib[0x9c229862fb29121c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&Result_73cdb19a79e62102>>();
    };
    aotLib[0x7870d8f52972b72c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&InvalidResult_9b1b21e34eba155d>>();
    };
    aotLib[0xf69645d18cff1af4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&error_e5bb3044bbd89a1d>>();
    };
    aotLib[0x3edf9c9700ce8b61] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&getPD_4da28c77bbe6c3d7>>();
    };
    aotLib[0x4a15981ca731f4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&getI_5e1fded1da82fe11>>();
    };
    aotLib[0xf8859c8ca9c141e3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&getT_9627d57179cddca6>>();
    };
    aotLib[0x417b97408498ddf5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&func_call_length_17a2d2ec17789881>>();
    };
    aotLib[0xed71919c666ea4e0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&func_call_34538813fbb8fc0f>>();
    };
    aotLib[0x3ad3b9e8fda3c8a2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&expr_value_ae10ccbfc8e1ce31>>();
    };
    aotLib[0xdc21e52ecdd0c43] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&getStructFieldOffset_13b5c35ad8bad1b1>>();
    };
    aotLib[0x9d433889a1059421] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&getTupleFieldOffset_843b15bad51bff07>>();
    };
    aotLib[0xdf46ce8e99ee6ef5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&getVectorOffset_dcf7cf4570b94b24>>();
    };
    aotLib[0xe38ee7fecb6759ad] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&getHandleFieldOffset_4b8713168d14b603>>();
    };
    aotLib[0x4d42225320636044] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&expr_field_5dfcd45b1ecd4499>>();
    };
    aotLib[0x203e46026cbf7d1b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&opDim_b0bf78e5c93259f1>>();
    };
    aotLib[0x8fa0e2e28ce8cdf4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&opDimString_6ac245c533760d26>>();
    };
    aotLib[0xe08d83ec76a6cb0e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&opArray_f74b354d6cd2ce02>>();
    };
    aotLib[0x6d046566ed6cf458] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&opTable_b6c6c7c404d92dc9>>();
    };
    aotLib[0x1a4ae1cba46b931] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&expr_at_56cc60f4051344c2>>();
    };
    aotLib[0xafe42001ca5af36] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&expr_unary_a06fc7e0b3c8f97>>();
    };
    aotLib[0x5497a816134f3a99] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&expr_mul_div_d2e41a242a5717c8>>();
    };
    aotLib[0xb9046f9b6e4cf346] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&expr_add_sub_98f296ed1fed65d7>>();
    };
    aotLib[0xbe9853f9ef3a8afb] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&try_accept_punkt_36a058d72081e56e>>();
    };
    aotLib[0xc0627b41a5af77b4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&expr_and_d5d8333e25dfcbe4>>();
    };
    aotLib[0x363cac2f4e901be0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&expr_xor_98afb583f1a5a9a5>>();
    };
    aotLib[0xd7152eeea0c6c102] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&expr_or_d504d6b12b7ceea7>>();
    };
    aotLib[0x3b5e99ad1d3a324] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&expr_bool_op_c0cbada86a1585de>>();
    };
    aotLib[0xe2e9b001f6a9eb85] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&expr_251518313273ca50>>();
    };
    aotLib[0x8d54cbbad5bb43e4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&debug_eval_25c7c1861470fbfd>>();
    };
    // [[ init script ]]
    aotLib[0xaba998aaf897b3b6] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<AutoSimNode_Aot<&__init_script>>();
        return ctx.aotInitScript;
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_12798696619525958528
AotListBase impl_aot_debug_eval(_anon_12798696619525958528::registerAotFunctions);
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
