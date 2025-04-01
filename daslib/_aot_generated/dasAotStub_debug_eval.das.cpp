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
namespace _anon_2611991765220838030 {

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
VarInfo __struct_info__bd90204d3de3945_field_0 =  { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~rtti::TypeInfo"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24590, 80, UINT64_C(0x86dad552ee0e6c29), "tinfo", offsetof(debug_eval::Result,tinfo), 2 };
VarInfo __struct_info__bd90204d3de3945_field_1 =  { Type::tFloat4, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 16, UINT64_C(0x8421d25d4efe44c8), "value", offsetof(debug_eval::Result,value), 0 };
VarInfo __struct_info__bd90204d3de3945_field_2 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af95fe4c86571c52, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0xf388c6a2fc596964), "data", offsetof(debug_eval::Result,data), 3 };
VarInfo __struct_info__bd90204d3de3945_field_3 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x312bbc9c4c7cd171), "error", offsetof(debug_eval::Result,error), 4 };
VarInfo * __struct_info__bd90204d3de3945_fields[4] =  { &__struct_info__bd90204d3de3945_field_0, &__struct_info__bd90204d3de3945_field_1, &__struct_info__bd90204d3de3945_field_2, &__struct_info__bd90204d3de3945_field_3 };
StructInfo __struct_info__bd90204d3de3945 = {"Result", "debug_eval", 12, __struct_info__bd90204d3de3945_fields, 4, 112, UINT64_C(0x0), nullptr, UINT64_C(0xbd90204d3de3945), 0 };
VarInfo __struct_info__acf09986cbe796dd_field_0 =  { Type::tIterator, nullptr, nullptr, nullptr, &__type_info__f8bbdc9319696978, nullptr, nullptr, nullptr, 0, 0, nullptr, 24602, 8, UINT64_C(0x485520cdbba56729), "tokens", offsetof(debug_eval::TokenStream,tokens), 1 };
VarInfo __struct_info__acf09986cbe796dd_field_1 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__f8bbdc9319696978, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x71caf94fe7458508), "stream", offsetof(debug_eval::TokenStream,stream), 2 };
VarInfo __struct_info__acf09986cbe796dd_field_2 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0xe0bed6337d486f47), "errors", offsetof(debug_eval::TokenStream,errors), 3 };
VarInfo __struct_info__acf09986cbe796dd_field_3 =  { Type::tTable, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, &__type_info__203fbf2d6d7ad716, nullptr, nullptr, 0, 0, nullptr, 57346, 48, UINT64_C(0xddc1b5e9e4d20aaa), "names", offsetof(debug_eval::TokenStream,names), 4 };
VarInfo * __struct_info__acf09986cbe796dd_fields[4] =  { &__struct_info__acf09986cbe796dd_field_0, &__struct_info__acf09986cbe796dd_field_1, &__struct_info__acf09986cbe796dd_field_2, &__struct_info__acf09986cbe796dd_field_3 };
StructInfo __struct_info__acf09986cbe796dd = {"TokenStream", "debug_eval", 28, __struct_info__acf09986cbe796dd_fields, 4, 104, UINT64_C(0x0), nullptr, UINT64_C(0xacf09986cbe796dd), 0 };
TypeInfo * __type_info__6362ab4c2cfcea07_arg_types_var_6662518835222007378[2] = { &__type_info__64e2d0c6d216e20d, &__type_info__f8bbdc9319696978 };
const char * __type_info__6362ab4c2cfcea07_arg_names_var_6662518835222007378[2] = { "__this", "_yield_26" };
VarInfo __struct_info__5c760594c2c1ce52_field_0 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af81fe4c86352052, nullptr, (TypeInfo **)__type_info__6362ab4c2cfcea07_arg_types_var_6662518835222007378, __type_info__6362ab4c2cfcea07_arg_names_var_6662518835222007378, 2, 0, nullptr, 12, 8, UINT64_C(0x6362ab4c2cfcea07), "__lambda", offsetof(debug_eval::_lambda_debug_eval_26_1,__lambda), 0 };
TypeInfo * __type_info__7205e8a10f980068_arg_types_var_6662518835222007378[1] = { &__type_info__1822028605008c6c };
const char * __type_info__7205e8a10f980068_arg_names_var_6662518835222007378[1] = { "__this" };
VarInfo __struct_info__5c760594c2c1ce52_field_1 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__7205e8a10f980068_arg_types_var_6662518835222007378, __type_info__7205e8a10f980068_arg_names_var_6662518835222007378, 1, 0, nullptr, 12, 8, UINT64_C(0x7205e8a10f980068), "__finalize", offsetof(debug_eval::_lambda_debug_eval_26_1,__finalize), 0 };
VarInfo __struct_info__5c760594c2c1ce52_field_2 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xb280c6258bfe0c24), "__yield", offsetof(debug_eval::_lambda_debug_eval_26_1,__yield), 0 };
VarInfo __struct_info__5c760594c2c1ce52_field_3 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x379ccd50c8d67a9a), "st", offsetof(debug_eval::_lambda_debug_eval_26_1,st), 4 };
VarInfo __struct_info__5c760594c2c1ce52_field_4 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x66d8d04552d43f9d), "tst", offsetof(debug_eval::_lambda_debug_eval_26_1,tst), 5 };
VarInfo __struct_info__5c760594c2c1ce52_field_5 =  { Type::tIterator, nullptr, nullptr, nullptr, &__type_info__af63e44c8601fa24, nullptr, nullptr, nullptr, 0, 0, nullptr, 24602, 8, UINT64_C(0x9a31f8edaaa938a6), "chars", offsetof(debug_eval::_lambda_debug_eval_26_1,chars), 7 };
VarInfo __struct_info__5c760594c2c1ce52_field_6 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x5f2fbded7d6eabf5), "ahead", offsetof(debug_eval::_lambda_debug_eval_26_1,ahead), 0 };
VarInfo __struct_info__5c760594c2c1ce52_field_7 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, 24, UINT64_C(0x6391bed0a86864a0), "__str_rename_at_42_20", offsetof(debug_eval::_lambda_debug_eval_26_1,__str_rename_at_42_20), 8 };
VarInfo __struct_info__5c760594c2c1ce52_field_8 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, 24, UINT64_C(0x9aaba7e91e30058b), "__str_rename_at_57_20", offsetof(debug_eval::_lambda_debug_eval_26_1,__str_rename_at_57_20), 9 };
VarInfo * __struct_info__5c760594c2c1ce52_fields[9] =  { &__struct_info__5c760594c2c1ce52_field_0, &__struct_info__5c760594c2c1ce52_field_1, &__struct_info__5c760594c2c1ce52_field_2, &__struct_info__5c760594c2c1ce52_field_3, &__struct_info__5c760594c2c1ce52_field_4, &__struct_info__5c760594c2c1ce52_field_5, &__struct_info__5c760594c2c1ce52_field_6, &__struct_info__5c760594c2c1ce52_field_7, &__struct_info__5c760594c2c1ce52_field_8 };
StructInfo __struct_info__5c760594c2c1ce52 = {"_lambda_debug_eval_26_1", "debug_eval", 30, __struct_info__5c760594c2c1ce52_fields, 9, 104, UINT64_C(0x0), nullptr, UINT64_C(0x5c760594c2c1ce52), 3 };
TypeInfo __type_info__1822028605008c6c = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__64e2d0c6d216e20d, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0x1822028605008c6c) };
TypeInfo __type_info__51bb14dd602168c2 = { Type::tStructure, &__struct_info__acf09986cbe796dd, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 104, UINT64_C(0x51bb14dd602168c2) };
TypeInfo * __type_info__7a089ab9e22fd5f_arg_types[5] = { &__type_info__d922fe078cefab30, &__type_info__af63e44c8601fa24, &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
const char * __type_info__7a089ab9e22fd5f_arg_names[5] = { "number", "punkt", "ident", "invalid", "eos" };
TypeInfo __type_info__7a089ab9e22fd5f = { Type::tVariant, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__7a089ab9e22fd5f_arg_types, __type_info__7a089ab9e22fd5f_arg_names, 5, 0, nullptr, 16678, 16, UINT64_C(0x7a089ab9e22fd5f) };
TypeInfo __type_info__4fd1011fade7876f = { Type::tEnumeration, nullptr, &__enum_info__c897fe55afe7f727, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 4, UINT64_C(0x4fd1011fade7876f) };
TypeInfo __type_info__af81fe4c86352052 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 1, UINT64_C(0xaf81fe4c86352052) };
TypeInfo __type_info__af8afe4c86446b52 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 4, UINT64_C(0xaf8afe4c86446b52) };
TypeInfo __type_info__8d8d8008262e16ec = { Type::tInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 8, UINT64_C(0x8d8d8008262e16ec) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, 8, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__af96fe4c8658cf52 = { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 4, UINT64_C(0xaf96fe4c8658cf52) };
TypeInfo __type_info__af95fe4c86571c52 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 0, UINT64_C(0xaf95fe4c86571c52) };
TypeInfo __type_info__a7159d402feecb0a = { Type::tEnumeration, nullptr, &__enum_info__c897fe55afe7f727, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xa7159d402feecb0a) };
TypeInfo __type_info__203fbf2d6d7ad716 = { Type::tStructure, &__struct_info__bd90204d3de3945, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24582, 112, UINT64_C(0x203fbf2d6d7ad716) };
TypeInfo __type_info__64e2d0c6d216e20d = { Type::tStructure, &__struct_info__5c760594c2c1ce52, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 104, UINT64_C(0x64e2d0c6d216e20d) };
TypeInfo * __type_info__f8bbdc9319696978_arg_types[5] = { &__type_info__d922fe078cefab30, &__type_info__af63e44c8601fa24, &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
const char * __type_info__f8bbdc9319696978_arg_names[5] = { "number", "punkt", "ident", "invalid", "eos" };
TypeInfo __type_info__f8bbdc9319696978 = { Type::tVariant, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__f8bbdc9319696978_arg_types, __type_info__f8bbdc9319696978_arg_names, 5, 0, nullptr, 16390, 16, UINT64_C(0xf8bbdc9319696978) };
TypeInfo __type_info__af63e44c8601fa24 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xaf63e44c8601fa24) };
TypeInfo __type_info__d922fe078cefab30 = { Type::tInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 8, UINT64_C(0xd922fe078cefab30) };
TypeInfo __type_info__af63ee4c86020b22 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xaf63ee4c86020b22) };
TypeInfo __type_info__af5be84c85f468f0 = { Type::tUInt8, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0xaf5be84c85f468f0) };
TypeInfo __type_info__af63eb4c86020609 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 0, UINT64_C(0xaf63eb4c86020609) };

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

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb ( Context * __context__, TArray<uint8_t> & __a_rename_at_1182_0 );
inline void _FuncbuiltinTickclearTick13739625760977891612_bc21a7c19d5d96ed ( Context * __context__, TTable<char *,debug_eval::Result> & __t_rename_at_1860_1 );
inline Sequence DAS_COMMENT((char *)) _FuncbuiltinTickkeysTick2205854368403803976_d3d21e2c8adb2e77 ( Context * __context__, TTable<char *,debug_eval::Result> const  & __a_rename_at_1128_2 );
inline Sequence DAS_COMMENT((debug_eval::Result &)) _FuncbuiltinTickvaluesTick1935193042646774172_fa9f9c260fe9a102 ( Context * __context__, TTable<char *,debug_eval::Result> const  & __a_rename_at_1143_4 );
inline bool _FuncbuiltinTicknextTick17450348357676149856_ba3f4a54e51a0d57 ( Context * __context__, Sequence DAS_COMMENT((int32_t)) & __it_rename_at_1223_6, int32_t & __value_rename_at_1223_7 );
inline void finalize_c2f42de354cd5bce ( Context * __context__, debug_eval::_lambda_debug_eval_26_1 & ____this_rename_at_26_8 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_f497868bddc80ac1 ( Context * __context__, TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>> & __Arr_rename_at_68_9, int32_t __newSize_rename_at_68_10 );
inline Sequence DAS_COMMENT((AutoVariant<int64_t,int32_t,char *,char *,char *>)) _FuncbuiltinTickeachTick9663565701927713696_acc6981303ddd856 ( Context * __context__, Lambda DAS_COMMENT((bool,AutoVariant<int64_t,int32_t,char *,char *,char *>)) const  __lam_rename_at_1289_11 );
inline debug_eval::TokenStream & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_32ef8117beffbe31 ( Context * __context__, debug_eval::TokenStream & __a_rename_at_50_13 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_f403d698c1fdad14 ( Context * __context__, TTable<char *,debug_eval::Result> const  & __Tab_rename_at_1014_14, char * const  __at_rename_at_1014_15 );
inline debug_eval::Result _FuncbuiltinTickget_valueTick6803070933163225147_25700dfc8547814 ( Context * __context__, TTable<char *,debug_eval::Result> & __Tab_rename_at_741_16, char * const  __at_rename_at_741_17 );
inline void _FuncbuiltinTickcloneTick3147220447302434744_caf9a18f00151e46 ( Context * __context__, TTable<char *,debug_eval::Result> & __a_rename_at_1119_18, TTable<char *,debug_eval::Result> const  & __b_rename_at_1119_19 );
inline Sequence DAS_COMMENT((int32_t)) _FuncbuiltinTickeachTick4044332007967089362_d13e7572e5849aa5 ( Context * __context__, char * const  __str_rename_at_1256_25 );
inline void _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67 ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_165_27, uint8_t __value_rename_at_165_28 );
inline bool _Func_lambda_debug_eval_26_1Tickfunction_f6c2387720b0ce3f ( Context * __context__, debug_eval::_lambda_debug_eval_26_1 & ____this_rename_at_26_29, AutoVariant<int64_t,int32_t,char *,char *,char *> & ___yield_26_rename_at_26_30 );
inline void _Func_lambda_debug_eval_26_1Tickfinalizer_5b0173a727d79cf1 ( Context * __context__, debug_eval::_lambda_debug_eval_26_1 * ____this_rename_at_26_31 );
inline bool _FuncbuiltinTickemptyTick15399874715904164783_4d77f401303a908c ( Context * __context__, TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>> const  & __a_rename_at_585_32 );
inline bool _FuncbuiltinTicknextTick17450348357676149856_5cb3f1fd943d9108 ( Context * __context__, Sequence DAS_COMMENT((AutoVariant<int64_t,int32_t,char *,char *,char *>)) & __it_rename_at_1223_33, AutoVariant<int64_t,int32_t,char *,char *,char *> & __value_rename_at_1223_34 );
inline AutoVariant<int64_t,int32_t,char *,char *,char *> & _FuncbuiltinTickbackTick18296309835877697278_237276f31a84753f ( Context * __context__, TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>> & __a_rename_at_473_35 );
inline void _FuncbuiltinTickpopTick1161079256290593740_7a972024f8437b3f ( Context * __context__, TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>> & __Arr_rename_at_132_37 );
inline void _FuncbuiltinTickpushTick14133213201864676143_e9749418e214318c ( Context * __context__, TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>> & __Arr_rename_at_165_38, AutoVariant<int64_t,int32_t,char *,char *,char *> const  & __value_rename_at_165_39 );
inline void _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af ( Context * __context__, TArray<char *> & __Arr_rename_at_165_40, char * const  __value_rename_at_165_41 );
inline int64_t _Funcdebug_evalTickloadITick653575658332832056_745a57955fa753cb ( Context * __context__, void * const  __data_rename_at_135_42, int64_t __t_rename_at_135_43 );
inline int32_t _Funcdebug_evalTickloadITick653575658332832056_d8697a77a47f3f6e ( Context * __context__, void * const  __data_rename_at_135_44, int32_t __t_rename_at_135_45 );
inline int16_t _Funcdebug_evalTickloadITick653575658332832056_62319ed67a086d7d ( Context * __context__, void * const  __data_rename_at_135_46, int16_t __t_rename_at_135_47 );
inline int8_t _Funcdebug_evalTickloadITick653575658332832056_f5e0b20ee0adc9d8 ( Context * __context__, void * const  __data_rename_at_135_48, int8_t __t_rename_at_135_49 );
inline bool _Funcdebug_evalTickloadITick653575658332832056_efd880e7e70477bb ( Context * __context__, void * const  __data_rename_at_135_50, bool __t_rename_at_135_51 );
inline bool _FuncbuiltinTickemptyTick15399874715904164783_e7d23d30bed4f24e ( Context * __context__, TArray<char *> const  & __a_rename_at_585_52 );
inline Sequence DAS_COMMENT((AutoVariant<int64_t,int32_t,char *,char *,char *>)) each_token_96e82efcebf13d0c ( Context * __context__, char * const  __st_rename_at_25_53 );
inline debug_eval::TokenStream TokenStream_59176a02fe830623 ( Context * __context__, char * const  __st_rename_at_89_54 );
inline AutoVariant<int64_t,int32_t,char *,char *,char *> token_36ce8667ed49d425 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_93_55 );
inline void unput_c08f1d95eea68a34 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_106_57, AutoVariant<int64_t,int32_t,char *,char *,char *> const  & __token_rename_at_106_58 );
inline debug_eval::Result Result_38240f4c3f9f19dc ( Context * __context__, int64_t __value_rename_at_110_59 );
inline debug_eval::Result Result_b262a84f440eb5c9 ( Context * __context__, bool __value_rename_at_117_61 );
inline debug_eval::Result InvalidResult_e6549533a9b44e33 ( Context * __context__ );
inline debug_eval::Result error_3b51aee67e872094 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_130_64, char * const  __error_rename_at_130_65 );
inline void * getPD_950137d699d78aca ( Context * __context__, debug_eval::TokenStream & __st_rename_at_141_66, debug_eval::Result const  & __result_rename_at_141_67, int32_t __offset_rename_at_141_68 );
inline int64_t getI_e470f5a746b3541c ( Context * __context__, debug_eval::TokenStream & __st_rename_at_167_72, debug_eval::Result const  & __result_rename_at_167_73 );
inline char * getT_e38a914ed5762f78 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_187_76, debug_eval::Result const  & __result_rename_at_187_77 );
inline debug_eval::Result func_call_length_e620fd2edb88dae3 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_198_78, debug_eval::Result const  & __result_rename_at_198_79 );
inline debug_eval::Result func_call_e8069ada942ecd18 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_220_84, char * const  __name_rename_at_220_85, debug_eval::Result const  & __arg_rename_at_220_86 );
inline debug_eval::Result expr_value_eb120b4e3ca163eb ( Context * __context__, debug_eval::TokenStream & __st_rename_at_225_87 );
inline AutoTuple<int32_t,TypeInfo> getStructFieldOffset_5201839c067f2b1b ( Context * __context__, debug_eval::TokenStream & __st_rename_at_272_92, char * const  __ident_rename_at_272_93, debug_eval::Result & __result_rename_at_272_94 );
inline AutoTuple<int32_t,TypeInfo> getTupleFieldOffset_82d2cca0fc285ff0 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_289_99, char * const  __ident_rename_at_289_100, debug_eval::Result & __result_rename_at_289_101 );
inline AutoTuple<int32_t,TypeInfo> getVectorOffset_f396a4629b54077a ( Context * __context__, debug_eval::TokenStream & __st_rename_at_303_106, char * const  __ident_rename_at_303_107, debug_eval::Result & __result_rename_at_303_108 );
inline AutoTuple<int32_t,TypeInfo> getHandleFieldOffset_17e7898b27c06a3b ( Context * __context__, debug_eval::TokenStream & __st_rename_at_314_112, char * const  __ident_rename_at_314_113, debug_eval::Result & __result_rename_at_314_114 );
inline debug_eval::Result expr_field_7b9abb4b48818ee6 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_329_118 );
inline debug_eval::Result opDim_e93722f23b2b225d ( Context * __context__, debug_eval::TokenStream & __st_rename_at_370_124, int64_t __index_rename_at_370_125, debug_eval::Result & __result_rename_at_370_126 );
inline debug_eval::Result opDimString_2d98ae0b9aba33b ( Context * __context__, debug_eval::TokenStream & __st_rename_at_381_129, int64_t __index_rename_at_381_130, debug_eval::Result & __result_rename_at_381_131 );
inline debug_eval::Result opArray_dfcf64278e28052b ( Context * __context__, debug_eval::TokenStream & __st_rename_at_397_134, int64_t __index_rename_at_397_135, debug_eval::Result & __result_rename_at_397_136 );
inline debug_eval::Result opTable_2c3c0211d77b2294 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_416_142, debug_eval::Result const  & __index_rename_at_416_143, debug_eval::Result & __result_rename_at_416_144 );
inline debug_eval::Result expr_at_eb04195e4bb4b7e4 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_440_152 );
inline debug_eval::Result expr_unary_7eafff562dd91c36 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_473_157 );
inline debug_eval::Result expr_mul_div_a01816d0548689b4 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_493_159 );
inline debug_eval::Result expr_add_sub_958f6e8350a25a01 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_514_163 );
inline bool try_accept_punkt_c1565174265ef7b3 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_531_166, int32_t __punkt_rename_at_531_167 );
inline debug_eval::Result expr_and_9edde39c3aad1048 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_540_169 );
inline debug_eval::Result expr_xor_466b6dbbcc14397c ( Context * __context__, debug_eval::TokenStream & __st_rename_at_552_171 );
inline debug_eval::Result expr_or_bf9fadca84c5914c ( Context * __context__, debug_eval::TokenStream & __st_rename_at_564_173 );
inline debug_eval::Result expr_bool_op_a803ca1de5c0f120 ( Context * __context__, debug_eval::TokenStream & __st_rename_at_576_175 );
inline debug_eval::Result expr_553a8faea7e2c9cd ( Context * __context__, debug_eval::TokenStream & __st_rename_at_609_179 );
inline debug_eval::Result debug_eval_cad59dc2d7e9b0bb ( Context * __context__, TTable<char *,debug_eval::Result> const  & __context_rename_at_613_180, char * const  __expr_rename_at_613_181 );

void __init_script ( Context * __context__, bool __init_shared )
{
    das_global<int64_t,0x9bae386ba6f944e9>(__context__) = INT64_MIN;/*INVALID_VALUE*/
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb ( Context * __context__, TArray<uint8_t> &  __a_rename_at_1182_0 )
{
    builtin_array_free(das_arg<TArray<uint8_t>>::pass(__a_rename_at_1182_0),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickclearTick13739625760977891612_bc21a7c19d5d96ed ( Context * __context__, TTable<char *,debug_eval::Result> &  __t_rename_at_1860_1 )
{
    builtin_table_clear(das_arg<TTable<char *,debug_eval::Result>>::pass(__t_rename_at_1860_1),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline Sequence DAS_COMMENT((char *)) _FuncbuiltinTickkeysTick2205854368403803976_d3d21e2c8adb2e77 ( Context * __context__, TTable<char *,debug_eval::Result> const  &  __a_rename_at_1128_2 )
{
    Sequence DAS_COMMENT((char *)) __it_rename_at_1129_3; das_zero(__it_rename_at_1129_3);
    builtin_table_keys(das_arg<Sequence DAS_COMMENT((char *))>::pass(__it_rename_at_1129_3),__a_rename_at_1128_2,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((char *))>::cast(__it_rename_at_1129_3);
}

inline Sequence DAS_COMMENT((debug_eval::Result &)) _FuncbuiltinTickvaluesTick1935193042646774172_fa9f9c260fe9a102 ( Context * __context__, TTable<char *,debug_eval::Result> const  &  __a_rename_at_1143_4 )
{
    Sequence DAS_COMMENT((debug_eval::Result *)) __it_rename_at_1144_5; das_zero(__it_rename_at_1144_5);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((debug_eval::Result const ))>::pass(__it_rename_at_1144_5),__a_rename_at_1143_4,112,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((debug_eval::Result &))>::cast(__it_rename_at_1144_5);
}

inline bool _FuncbuiltinTicknextTick17450348357676149856_ba3f4a54e51a0d57 ( Context * __context__, Sequence DAS_COMMENT((int32_t)) &  __it_rename_at_1223_6, int32_t & __value_rename_at_1223_7 )
{
    return das_auto_cast<bool>::cast(builtin_iterator_iterate(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(__it_rename_at_1223_6),das_auto_cast<void * const >::cast(das_ref(__context__,__value_rename_at_1223_7)),__context__));
}

inline void finalize_c2f42de354cd5bce ( Context * __context__, debug_eval::_lambda_debug_eval_26_1 &  ____this_rename_at_26_8 )
{
    builtin_iterator_delete(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_26_8.chars),__context__);
    _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_26_8.__str_rename_at_42_20));
    _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_26_8.__str_rename_at_57_20));
    memset(&(____this_rename_at_26_8), 0, TypeSize<debug_eval::_lambda_debug_eval_26_1>::size);
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_f497868bddc80ac1 ( Context * __context__, TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>> &  __Arr_rename_at_68_9, int32_t __newSize_rename_at_68_10 )
{
    builtin_array_resize(das_arg<TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>>>::pass(__Arr_rename_at_68_9),__newSize_rename_at_68_10,16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline Sequence DAS_COMMENT((AutoVariant<int64_t,int32_t,char *,char *,char *>)) _FuncbuiltinTickeachTick9663565701927713696_acc6981303ddd856 ( Context * __context__, Lambda DAS_COMMENT((bool,AutoVariant<int64_t,int32_t,char *,char *,char *>)) const  __lam_rename_at_1289_11 )
{
    Sequence DAS_COMMENT((AutoVariant<int64_t,int32_t,char *,char *,char *>)) __it_rename_at_1291_12; das_zero(__it_rename_at_1291_12);
    builtin_make_lambda_iterator(das_arg<Sequence DAS_COMMENT((AutoVariant<int64_t,int32_t,char *,char *,char *>))>::pass(__it_rename_at_1291_12),__lam_rename_at_1289_11,16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((AutoVariant<int64_t,int32_t,char *,char *,char *>))>::cast(__it_rename_at_1291_12);
}

inline debug_eval::TokenStream & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_32ef8117beffbe31 ( Context * __context__, debug_eval::TokenStream &  __a_rename_at_50_13 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_0,cast<debug_eval::TokenStream &>::from(__a_rename_at_50_13)));
    return das_auto_cast_ref<debug_eval::TokenStream &>::cast(__a_rename_at_50_13);
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_f403d698c1fdad14 ( Context * __context__, TTable<char *,debug_eval::Result> const  &  __Tab_rename_at_1014_14, char * const  __at_rename_at_1014_15 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1014_14,__at_rename_at_1014_15));
}

inline debug_eval::Result _FuncbuiltinTickget_valueTick6803070933163225147_25700dfc8547814 ( Context * __context__, TTable<char *,debug_eval::Result> &  __Tab_rename_at_741_16, char * const  __at_rename_at_741_17 )
{
    return das_auto_cast_ref<debug_eval::Result>::cast(__Tab_rename_at_741_16(__at_rename_at_741_17,__context__));
}

inline void _FuncbuiltinTickcloneTick3147220447302434744_caf9a18f00151e46 ( Context * __context__, TTable<char *,debug_eval::Result> &  __a_rename_at_1119_18, TTable<char *,debug_eval::Result> const  &  __b_rename_at_1119_19 )
{
    Sequence DAS_COMMENT((char *)) _temp_make_local_1121_17_32; _temp_make_local_1121_17_32;
    Sequence DAS_COMMENT((debug_eval::Result *)) _temp_make_local_1121_26_48; _temp_make_local_1121_26_48;
    _FuncbuiltinTickclearTick13739625760977891612_bc21a7c19d5d96ed(__context__,das_arg<TTable<char *,debug_eval::Result>>::pass(__a_rename_at_1119_18));
    {
        bool __need_loop_1121 = true;
        // k: string
        das_iterator<Sequence DAS_COMMENT((char *))> __k_iterator((_temp_make_local_1121_17_32 = (_FuncbuiltinTickkeysTick2205854368403803976_d3d21e2c8adb2e77(__context__,__b_rename_at_1119_19))));
        char * __k_rename_at_1121_22;
        __need_loop_1121 = __k_iterator.first(__context__,(__k_rename_at_1121_22)) && __need_loop_1121;
        // v: debug_eval::Result const&
        das_iterator<Sequence DAS_COMMENT((debug_eval::Result const ))> __v_iterator((_temp_make_local_1121_26_48 = (_FuncbuiltinTickvaluesTick1935193042646774172_fa9f9c260fe9a102(__context__,__b_rename_at_1119_19))));
        debug_eval::Result const  * __v_rename_at_1121_23;
        __need_loop_1121 = __v_iterator.first(__context__,(__v_rename_at_1121_23)) && __need_loop_1121;
        for ( ; __need_loop_1121 ; __need_loop_1121 = __k_iterator.next(__context__,(__k_rename_at_1121_22)) && __v_iterator.next(__context__,(__v_rename_at_1121_23)) )
        {
            char * __kk_rename_at_1122_24 = ((char *)(char *)(__k_rename_at_1121_22));
            das_copy(__a_rename_at_1119_18(__kk_rename_at_1122_24,__context__),(*__v_rename_at_1121_23));
        }
        __k_iterator.close(__context__,(__k_rename_at_1121_22));
        __v_iterator.close(__context__,(__v_rename_at_1121_23));
    };
}

inline Sequence DAS_COMMENT((int32_t)) _FuncbuiltinTickeachTick4044332007967089362_d13e7572e5849aa5 ( Context * __context__, char * const  __str_rename_at_1256_25 )
{
    Sequence DAS_COMMENT((int32_t)) __it_rename_at_1257_26; das_zero(__it_rename_at_1257_26);
    builtin_make_string_iterator(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(__it_rename_at_1257_26),__str_rename_at_1256_25,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((int32_t))>::cast(__it_rename_at_1257_26);
}

inline void _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67 ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_165_27, uint8_t __value_rename_at_165_28 )
{
    das_copy(__Arr_rename_at_165_27(builtin_array_push_back(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_165_27),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_165_28);
}

inline bool _Func_lambda_debug_eval_26_1Tickfunction_f6c2387720b0ce3f ( Context * __context__, debug_eval::_lambda_debug_eval_26_1 &  ____this_rename_at_26_29, AutoVariant<int64_t,int32_t,char *,char *,char *> &  ___yield_26_rename_at_26_30 )
{
    switch (____this_rename_at_26_29.__yield) {
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
    das_copy(____this_rename_at_26_29.tst,____this_rename_at_26_29.st);
    das_move(____this_rename_at_26_29.chars,_FuncbuiltinTickeachTick4044332007967089362_d13e7572e5849aa5(__context__,____this_rename_at_26_29.tst));
    das_copy(____this_rename_at_26_29.ahead,32);
    label_14:;;
    if ( !!(builtin_iterator_empty(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_26_29.chars))) )
    {
        goto label_15;
    };
    while ( is_white_space(____this_rename_at_26_29.ahead) && _FuncbuiltinTicknextTick17450348357676149856_ba3f4a54e51a0d57(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_26_29.chars),____this_rename_at_26_29.ahead) )
    {
    };
    if ( builtin_iterator_empty(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_26_29.chars)) )
    {
        goto label_15;
    };
    if ( !(((((((((((((((((____this_rename_at_26_29.ahead == 43) || (____this_rename_at_26_29.ahead == 45)) || (____this_rename_at_26_29.ahead == 42)) || (____this_rename_at_26_29.ahead == 47)) || (____this_rename_at_26_29.ahead == 91)) || (____this_rename_at_26_29.ahead == 93)) || (____this_rename_at_26_29.ahead == 40)) || (____this_rename_at_26_29.ahead == 41)) || (____this_rename_at_26_29.ahead == 46)) || (____this_rename_at_26_29.ahead == 33)) || (____this_rename_at_26_29.ahead == 126)) || (____this_rename_at_26_29.ahead == 38)) || (____this_rename_at_26_29.ahead == 124)) || (____this_rename_at_26_29.ahead == 94)) || (____this_rename_at_26_29.ahead == 60)) || (____this_rename_at_26_29.ahead == 62)) || (____this_rename_at_26_29.ahead == 61)) )
    {
        goto label_12;
    };
    das_copy(___yield_26_rename_at_26_30,(([&]() -> AutoVariant<int64_t,int32_t,char *,char *,char *> {
        AutoVariant<int64_t,int32_t,char *,char *,char *> __mkv_39;
        das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::set(__mkv_39) = ____this_rename_at_26_29.ahead;
        return __mkv_39;
    })()));
    das_copy(____this_rename_at_26_29.__yield,1);
    return das_auto_cast<bool>::cast(true);
    label_1:;;
    _FuncbuiltinTicknextTick17450348357676149856_ba3f4a54e51a0d57(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_26_29.chars),____this_rename_at_26_29.ahead);
    goto label_13;
    label_12:;;
    if ( !is_number(____this_rename_at_26_29.ahead) )
    {
        goto label_10;
    };
    memset(&(____this_rename_at_26_29.__str_rename_at_42_20), 0, TypeSize<TArray<uint8_t>>::size);
    _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_26_29.__str_rename_at_42_20),uint8_t(____this_rename_at_26_29.ahead));
    while ( _FuncbuiltinTicknextTick17450348357676149856_ba3f4a54e51a0d57(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_26_29.chars),____this_rename_at_26_29.ahead) && is_number(____this_rename_at_26_29.ahead) )
    {
        _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_26_29.__str_rename_at_42_20),uint8_t(____this_rename_at_26_29.ahead));
    };
    if ( !(((builtin_array_size(das_arg<TArray<uint8_t>>::pass(____this_rename_at_26_29.__str_rename_at_42_20)) == 1) && (das_equ_val(____this_rename_at_26_29.__str_rename_at_42_20(0,__context__),0x30))) && ((____this_rename_at_26_29.ahead == 120) || (____this_rename_at_26_29.ahead == 88))) )
    {
        goto label_4;
    };
    _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_26_29.__str_rename_at_42_20),uint8_t(____this_rename_at_26_29.ahead));
    while ( _FuncbuiltinTicknextTick17450348357676149856_ba3f4a54e51a0d57(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_26_29.chars),____this_rename_at_26_29.ahead) && ((is_number(____this_rename_at_26_29.ahead) || ((____this_rename_at_26_29.ahead >= 97) && (____this_rename_at_26_29.ahead <= 102))) || ((____this_rename_at_26_29.ahead >= 65) && (____this_rename_at_26_29.ahead <= 70))) )
    {
        _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_26_29.__str_rename_at_42_20),uint8_t(____this_rename_at_26_29.ahead));
    };
    das_copy(___yield_26_rename_at_26_30,(([&]() -> AutoVariant<int64_t,int32_t,char *,char *,char *> {
        AutoVariant<int64_t,int32_t,char *,char *,char *> __mkv_52;
        das_get_auto_variant_field<int64_t,0,int64_t,int32_t,char *,char *,char *>::set(__mkv_52) = fast_to_int64(((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass(____this_rename_at_26_29.__str_rename_at_42_20),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))),true);
        return __mkv_52;
    })()));
    das_copy(____this_rename_at_26_29.__yield,2);
    return das_auto_cast<bool>::cast(true);
    label_2:;;
    goto label_5;
    label_4:;;
    das_copy(___yield_26_rename_at_26_30,(([&]() -> AutoVariant<int64_t,int32_t,char *,char *,char *> {
        AutoVariant<int64_t,int32_t,char *,char *,char *> __mkv_54;
        das_get_auto_variant_field<int64_t,0,int64_t,int32_t,char *,char *,char *>::set(__mkv_54) = fast_to_int64(((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass(____this_rename_at_26_29.__str_rename_at_42_20),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))),false);
        return __mkv_54;
    })()));
    das_copy(____this_rename_at_26_29.__yield,3);
    return das_auto_cast<bool>::cast(true);
    label_3:;;
    label_5:;;
    goto label_11;
    label_10:;;
    if ( !((is_alpha(____this_rename_at_26_29.ahead) || (____this_rename_at_26_29.ahead == 95)) || (____this_rename_at_26_29.ahead == 96)) )
    {
        goto label_8;
    };
    memset(&(____this_rename_at_26_29.__str_rename_at_57_20), 0, TypeSize<TArray<uint8_t>>::size);
    _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_26_29.__str_rename_at_57_20),uint8_t(____this_rename_at_26_29.ahead));
    while ( _FuncbuiltinTicknextTick17450348357676149856_ba3f4a54e51a0d57(__context__,das_arg<Sequence DAS_COMMENT((int32_t))>::pass(____this_rename_at_26_29.chars),____this_rename_at_26_29.ahead) && (((is_alpha(____this_rename_at_26_29.ahead) || is_number(____this_rename_at_26_29.ahead)) || (____this_rename_at_26_29.ahead == 95)) || (____this_rename_at_26_29.ahead == 96)) )
    {
        _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_26_29.__str_rename_at_57_20),uint8_t(____this_rename_at_26_29.ahead));
    };
    das_copy(___yield_26_rename_at_26_30,(([&]() -> AutoVariant<int64_t,int32_t,char *,char *,char *> {
        AutoVariant<int64_t,int32_t,char *,char *,char *> __mkv_62;
        das_get_auto_variant_field<char *,2,int64_t,int32_t,char *,char *,char *>::set(__mkv_62) = ((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass(____this_rename_at_26_29.__str_rename_at_57_20),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))));
        return __mkv_62;
    })()));
    das_copy(____this_rename_at_26_29.__yield,6);
    return das_auto_cast<bool>::cast(true);
    label_6:;;
    goto label_9;
    label_8:;;
    das_copy(___yield_26_rename_at_26_30,(([&]() -> AutoVariant<int64_t,int32_t,char *,char *,char *> {
        AutoVariant<int64_t,int32_t,char *,char *,char *> __mkv_64;
        das_get_auto_variant_field<char *,3,int64_t,int32_t,char *,char *,char *>::set(__mkv_64) = das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_1, cast<char * const >::from(((char *) "invalid character ")), cast<char * const >::from(((char * const )(to_string_char(____this_rename_at_26_29.ahead,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))))));
        return __mkv_64;
    })()));
    das_copy(____this_rename_at_26_29.__yield,7);
    return das_auto_cast<bool>::cast(true);
    label_7:;;
    return das_auto_cast<bool>::cast(false);
    label_9:;;
    label_11:;;
    label_13:;;
    goto label_14;
    label_15:;;
    das_copy(___yield_26_rename_at_26_30,(([&]() -> AutoVariant<int64_t,int32_t,char *,char *,char *> {
        AutoVariant<int64_t,int32_t,char *,char *,char *> __mkv_68;
        das_get_auto_variant_field<char *,4,int64_t,int32_t,char *,char *,char *>::set(__mkv_68) = ((char *) "eos");
        return __mkv_68;
    })()));
    das_copy(____this_rename_at_26_29.__yield,16);
    return das_auto_cast<bool>::cast(true);
    label_16:;;
    return das_auto_cast<bool>::cast(false);
}

inline void _Func_lambda_debug_eval_26_1Tickfinalizer_5b0173a727d79cf1 ( Context * __context__, debug_eval::_lambda_debug_eval_26_1 * ____this_rename_at_26_31 )
{
    finalize_c2f42de354cd5bce(__context__,das_arg<debug_eval::_lambda_debug_eval_26_1>::pass(das_deref(__context__,____this_rename_at_26_31)));
    das_delete_lambda_struct<debug_eval::_lambda_debug_eval_26_1 *>::clear(__context__,____this_rename_at_26_31);
}

inline bool _FuncbuiltinTickemptyTick15399874715904164783_4d77f401303a908c ( Context * __context__, TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>> const  &  __a_rename_at_585_32 )
{
    return das_auto_cast<bool>::cast(builtin_array_size(__a_rename_at_585_32) == 0);
}

inline bool _FuncbuiltinTicknextTick17450348357676149856_5cb3f1fd943d9108 ( Context * __context__, Sequence DAS_COMMENT((AutoVariant<int64_t,int32_t,char *,char *,char *>)) &  __it_rename_at_1223_33, AutoVariant<int64_t,int32_t,char *,char *,char *> &  __value_rename_at_1223_34 )
{
    return das_auto_cast<bool>::cast(builtin_iterator_iterate(das_arg<Sequence DAS_COMMENT((AutoVariant<int64_t,int32_t,char *,char *,char *>))>::pass(__it_rename_at_1223_33),das_auto_cast<void * const >::cast(das_ref(__context__,__value_rename_at_1223_34)),__context__));
}

inline AutoVariant<int64_t,int32_t,char *,char *,char *> & _FuncbuiltinTickbackTick18296309835877697278_237276f31a84753f ( Context * __context__, TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>> &  __a_rename_at_473_35 )
{
    int32_t __l_rename_at_474_36 = ((int32_t)builtin_array_size(das_arg<TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>>>::pass(__a_rename_at_473_35)));
    if ( __l_rename_at_474_36 == 0 )
    {
        builtin_throw(((char *) "back empty array"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    return das_auto_cast_ref<AutoVariant<int64_t,int32_t,char *,char *,char *> &>::cast(__a_rename_at_473_35((__l_rename_at_474_36 - 1),__context__));
}

inline void _FuncbuiltinTickpopTick1161079256290593740_7a972024f8437b3f ( Context * __context__, TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>> &  __Arr_rename_at_132_37 )
{
    _FuncbuiltinTickresizeTick4811697762258667383_f497868bddc80ac1(__context__,das_arg<TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>>>::pass(__Arr_rename_at_132_37),builtin_array_size(das_arg<TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>>>::pass(__Arr_rename_at_132_37)) - 1);
}

inline void _FuncbuiltinTickpushTick14133213201864676143_e9749418e214318c ( Context * __context__, TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>> &  __Arr_rename_at_165_38, AutoVariant<int64_t,int32_t,char *,char *,char *> const  &  __value_rename_at_165_39 )
{
    das_copy(__Arr_rename_at_165_38(builtin_array_push_back(das_arg<TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>>>::pass(__Arr_rename_at_165_38),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_165_39);
}

inline void _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af ( Context * __context__, TArray<char *> &  __Arr_rename_at_165_40, char * const  __value_rename_at_165_41 )
{
    das_copy(__Arr_rename_at_165_40(builtin_array_push_back(das_arg<TArray<char *>>::pass(__Arr_rename_at_165_40),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_165_41);
}

inline int64_t _Funcdebug_evalTickloadITick653575658332832056_745a57955fa753cb ( Context * __context__, void * const  __data_rename_at_135_42, int64_t __t_rename_at_135_43 )
{
    return das_auto_cast<int64_t>::cast(das_deref(__context__,das_cast<int64_t const  *>::cast(__data_rename_at_135_42)));
}

inline int32_t _Funcdebug_evalTickloadITick653575658332832056_d8697a77a47f3f6e ( Context * __context__, void * const  __data_rename_at_135_44, int32_t __t_rename_at_135_45 )
{
    return das_auto_cast<int32_t>::cast(das_deref(__context__,das_cast<int32_t const  *>::cast(__data_rename_at_135_44)));
}

inline int16_t _Funcdebug_evalTickloadITick653575658332832056_62319ed67a086d7d ( Context * __context__, void * const  __data_rename_at_135_46, int16_t __t_rename_at_135_47 )
{
    return das_auto_cast<int16_t>::cast(das_deref(__context__,das_cast<int16_t const  *>::cast(__data_rename_at_135_46)));
}

inline int8_t _Funcdebug_evalTickloadITick653575658332832056_f5e0b20ee0adc9d8 ( Context * __context__, void * const  __data_rename_at_135_48, int8_t __t_rename_at_135_49 )
{
    return das_auto_cast<int8_t>::cast(das_deref(__context__,das_cast<int8_t const  *>::cast(__data_rename_at_135_48)));
}

inline bool _Funcdebug_evalTickloadITick653575658332832056_efd880e7e70477bb ( Context * __context__, void * const  __data_rename_at_135_50, bool __t_rename_at_135_51 )
{
    return das_auto_cast<bool>::cast(das_deref(__context__,das_cast<bool const  *>::cast(__data_rename_at_135_50)));
}

inline bool _FuncbuiltinTickemptyTick15399874715904164783_e7d23d30bed4f24e ( Context * __context__, TArray<char *> const  &  __a_rename_at_585_52 )
{
    return das_auto_cast<bool>::cast(builtin_array_size(__a_rename_at_585_52) == 0);
}

inline Sequence DAS_COMMENT((AutoVariant<int64_t,int32_t,char *,char *,char *>)) each_token_96e82efcebf13d0c ( Context * __context__, char * const  __st_rename_at_25_53 )
{
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((AutoVariant<int64_t,int32_t,char *,char *,char *>))>::cast(_FuncbuiltinTickeachTick9663565701927713696_acc6981303ddd856(__context__,das_ascend<bool,debug_eval::_lambda_debug_eval_26_1>::make(__context__,&__type_info__64e2d0c6d216e20d,(([&]() -> debug_eval::_lambda_debug_eval_26_1 {
        debug_eval::_lambda_debug_eval_26_1 __mks_26;
        das_zero(__mks_26);
        das_copy((__mks_26.__lambda),(Func(__context__->fnByMangledName(/*@debug_eval::_lambda_debug_eval_26_1`function XS<debug_eval::_lambda_debug_eval_26_1> Y<Token>N<number;punkt;ident;invalid;eos>0<i64;i;s;s;s>V*/ 0x564819c6df0852e7))));
        das_copy((__mks_26.__finalize),(Func(__context__->fnByMangledName(/*@debug_eval::_lambda_debug_eval_26_1`finalizer X1<S<debug_eval::_lambda_debug_eval_26_1>>?*/ 0x456a0534fda3a25d))));
        das_copy((__mks_26.st),(__st_rename_at_25_53));
        return __mks_26;
    })()))));
}

inline debug_eval::TokenStream TokenStream_59176a02fe830623 ( Context * __context__, char * const  __st_rename_at_89_54 )
{
    debug_eval::TokenStream _temp_make_local_90_14_32; _temp_make_local_90_14_32;
    return /* <- */ das_auto_cast_move<debug_eval::TokenStream>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_32ef8117beffbe31(__context__,das_arg<debug_eval::TokenStream>::pass((([&]() -> debug_eval::TokenStream& {
        das_zero(_temp_make_local_90_14_32);
        das_move((_temp_make_local_90_14_32.tokens),(each_token_96e82efcebf13d0c(__context__,__st_rename_at_89_54)));
        return _temp_make_local_90_14_32;
    })()))));
}

inline AutoVariant<int64_t,int32_t,char *,char *,char *> token_36ce8667ed49d425 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_93_55 )
{
    AutoVariant<int64_t,int32_t,char *,char *,char *> __token_rename_at_94_56; das_zero(__token_rename_at_94_56);
    if ( _FuncbuiltinTickemptyTick15399874715904164783_4d77f401303a908c(__context__,das_arg<TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>>>::pass(__st_rename_at_93_55.stream)) )
    {
        if ( !_FuncbuiltinTicknextTick17450348357676149856_5cb3f1fd943d9108(__context__,das_arg<Sequence DAS_COMMENT((AutoVariant<int64_t,int32_t,char *,char *,char *>))>::pass(__st_rename_at_93_55.tokens),das_arg<AutoVariant<int64_t,int32_t,char *,char *,char *>>::pass(__token_rename_at_94_56)) )
        {
            das_copy(__token_rename_at_94_56,(([&]() -> AutoVariant<int64_t,int32_t,char *,char *,char *> {
                AutoVariant<int64_t,int32_t,char *,char *,char *> __mkv_97;
                das_get_auto_variant_field<char *,4,int64_t,int32_t,char *,char *,char *>::set(__mkv_97) = ((char *) "eos");
                return __mkv_97;
            })()));
        };
    } else {
        das_copy(__token_rename_at_94_56,_FuncbuiltinTickbackTick18296309835877697278_237276f31a84753f(__context__,das_arg<TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>>>::pass(__st_rename_at_93_55.stream)));
        _FuncbuiltinTickpopTick1161079256290593740_7a972024f8437b3f(__context__,das_arg<TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>>>::pass(__st_rename_at_93_55.stream));
    };
    return das_auto_cast_ref<AutoVariant<int64_t,int32_t,char *,char *,char *>>::cast(__token_rename_at_94_56);
}

inline void unput_c08f1d95eea68a34 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_106_57, AutoVariant<int64_t,int32_t,char *,char *,char *> const  &  __token_rename_at_106_58 )
{
    _FuncbuiltinTickpushTick14133213201864676143_e9749418e214318c(__context__,das_arg<TArray<AutoVariant<int64_t,int32_t,char *,char *,char *>>>::pass(__st_rename_at_106_57.stream),__token_rename_at_106_58);
}

inline debug_eval::Result Result_38240f4c3f9f19dc ( Context * __context__, int64_t __value_rename_at_110_59 )
{
    debug_eval::Result __self_rename_at_111_60; das_zero(__self_rename_at_111_60);
    das_copy(__self_rename_at_111_60.tinfo.type /*basicType*/,DAS_COMMENT(bound_enum) das::Type::tInt64);
    das_copy(__self_rename_at_111_60.value,das_invoke_function<float4>::invoke<int64_t>(__context__,nullptr,Func(__context__->fnByMangledName(/*@math_bits::cast_to_vec4f Ci64*/ 0x3e76e6463b5d2a78)),__value_rename_at_110_59));
    return das_auto_cast_ref<debug_eval::Result>::cast(__self_rename_at_111_60);
}

inline debug_eval::Result Result_b262a84f440eb5c9 ( Context * __context__, bool __value_rename_at_117_61 )
{
    debug_eval::Result __self_rename_at_118_62; das_zero(__self_rename_at_118_62);
    das_copy(__self_rename_at_118_62.tinfo.type /*basicType*/,DAS_COMMENT(bound_enum) das::Type::tBool);
    das_copy(__self_rename_at_118_62.value,das_invoke_function<float4>::invoke<bool>(__context__,nullptr,Func(__context__->fnByMangledName(/*@math_bits::cast_to_vec4f Cb*/ 0x3d6f2ea296bbc09e)),__value_rename_at_117_61));
    return das_auto_cast_ref<debug_eval::Result>::cast(__self_rename_at_118_62);
}

inline debug_eval::Result InvalidResult_e6549533a9b44e33 ( Context * __context__ )
{
    debug_eval::Result __self_rename_at_125_63; das_zero(__self_rename_at_125_63);
    das_copy(__self_rename_at_125_63.tinfo.type /*basicType*/,DAS_COMMENT(bound_enum) das::Type::tVoid);
    return das_auto_cast_ref<debug_eval::Result>::cast(__self_rename_at_125_63);
}

inline debug_eval::Result error_3b51aee67e872094 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_130_64, char * const  __error_rename_at_130_65 )
{
    _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__st_rename_at_130_64.errors),__error_rename_at_130_65);
    return das_auto_cast_ref<debug_eval::Result>::cast(InvalidResult_e6549533a9b44e33(__context__));
}

inline void * getPD_950137d699d78aca ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_141_66, debug_eval::Result const  &  __result_rename_at_141_67, int32_t __offset_rename_at_141_68 )
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

inline int64_t getI_e470f5a746b3541c ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_167_72, debug_eval::Result const  &  __result_rename_at_167_73 )
{
    if ( __result_rename_at_167_73.tinfo.dimSize /*dimSize*/ != 0x0u )
    {
        debug_eval::Result _temp_make_local_169_14_32; _temp_make_local_169_14_32;
        (_temp_make_local_169_14_32 = (error_3b51aee67e872094(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_167_72),das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_2, cast<char * const >::from(((char *) "can't get value of [")), cast<uint32_t>::from(__result_rename_at_167_73.tinfo.dimSize /*dimSize*/), cast<char * const >::from(((char *) "]")))))));
        return das_auto_cast<int64_t>::cast(INT64_MIN);
    } else {
        DAS_COMMENT(bound_enum) das::Type __bt_rename_at_172_74 = ((DAS_COMMENT(bound_enum) das::Type)__result_rename_at_167_73.tinfo.type /*basicType*/);
        void * __data_rename_at_173_75 = ((void *)getPD_950137d699d78aca(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_167_72),__result_rename_at_167_73,0));
        if ( __data_rename_at_173_75 == nullptr )
        {
            debug_eval::Result _temp_make_local_175_14_176; _temp_make_local_175_14_176;
            (_temp_make_local_175_14_176 = (error_3b51aee67e872094(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_167_72),das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_3, cast<char * const >::from(((char *) "null pointer dereference for ")), cast<DAS_COMMENT(bound_enum) das::Type>::from(__result_rename_at_167_73.tinfo.type /*basicType*/))))));
            return das_auto_cast<int64_t>::cast(INT64_MIN);
        } else {
            if ( (__bt_rename_at_172_74 == DAS_COMMENT(bound_enum) das::Type::tInt64) || (__bt_rename_at_172_74 == DAS_COMMENT(bound_enum) das::Type::tUInt64) )
            {
                return das_auto_cast<int64_t>::cast(_Funcdebug_evalTickloadITick653575658332832056_745a57955fa753cb(__context__,__data_rename_at_173_75,das_typedecl_value<int64_t>()()));
            } else {
                if ( (__bt_rename_at_172_74 == DAS_COMMENT(bound_enum) das::Type::tInt) || (__bt_rename_at_172_74 == DAS_COMMENT(bound_enum) das::Type::tUInt) )
                {
                    return das_auto_cast<int64_t>::cast(int64_t(_Funcdebug_evalTickloadITick653575658332832056_d8697a77a47f3f6e(__context__,__data_rename_at_173_75,das_typedecl_value<int32_t>()())));
                } else {
                    if ( (__bt_rename_at_172_74 == DAS_COMMENT(bound_enum) das::Type::tInt16) || (__bt_rename_at_172_74 == DAS_COMMENT(bound_enum) das::Type::tUInt16) )
                    {
                        return das_auto_cast<int64_t>::cast(int64_t(_Funcdebug_evalTickloadITick653575658332832056_62319ed67a086d7d(__context__,__data_rename_at_173_75,das_typedecl_value<int16_t>()())));
                    } else {
                        if ( (__bt_rename_at_172_74 == DAS_COMMENT(bound_enum) das::Type::tInt8) || (__bt_rename_at_172_74 == DAS_COMMENT(bound_enum) das::Type::tUInt8) )
                        {
                            return das_auto_cast<int64_t>::cast(int64_t(_Funcdebug_evalTickloadITick653575658332832056_f5e0b20ee0adc9d8(__context__,__data_rename_at_173_75,das_typedecl_value<int8_t>()())));
                        } else {
                            if ( __bt_rename_at_172_74 == DAS_COMMENT(bound_enum) das::Type::tBool )
                            {
                                return das_auto_cast<int64_t>::cast(int64_t(_Funcdebug_evalTickloadITick653575658332832056_efd880e7e70477bb(__context__,__data_rename_at_173_75,das_typedecl_value<bool>()()) ? das_auto_cast<int64_t>::cast(INT64_C(1)) : das_auto_cast<int64_t>::cast(INT64_C(0))));
                            } else {
                                debug_eval::Result _temp_make_local_183_10_288; _temp_make_local_183_10_288;
                                (_temp_make_local_183_10_288 = (error_3b51aee67e872094(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_167_72),das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_4, cast<char * const >::from(((char *) "can't get value of ")), cast<DAS_COMMENT(bound_enum) das::Type>::from(__result_rename_at_167_73.tinfo.type /*basicType*/))))));
                                return das_auto_cast<int64_t>::cast(INT64_MIN);
                            };
                        };
                    };
                };
            };
        };
    };
}

inline char * getT_e38a914ed5762f78 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_187_76, debug_eval::Result const  &  __result_rename_at_187_77 )
{
    return das_auto_cast<char *>::cast(!(builtin_empty(__result_rename_at_187_77.error)) ? das_auto_cast<char * const >::cast(nullptr) : das_auto_cast<char * const >::cast(((__result_rename_at_187_77.data != nullptr) ? das_auto_cast<char * const >::cast(((char * const )(builtin_print_data(__result_rename_at_187_77.data,das_ref(__context__,__result_rename_at_187_77.tinfo),0x10u,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) : das_auto_cast<char * const >::cast(((char * const )(builtin_print_data_v(__result_rename_at_187_77.value,das_ref(__context__,__result_rename_at_187_77.tinfo),0x10u,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))))));
}

inline debug_eval::Result func_call_length_e620fd2edb88dae3 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_198_78, debug_eval::Result const  &  __result_rename_at_198_79 )
{
    if ( __result_rename_at_198_79.tinfo.dimSize /*dimSize*/ != 0x0u )
    {
        int32_t __idim_rename_at_201_80 = ((int32_t)rtti_getDimTypeInfo(__result_rename_at_198_79.tinfo,int32_t(__result_rename_at_198_79.tinfo.dimSize /*dimSize*/ - 0x1u),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
        return /* <- */ das_auto_cast_move<debug_eval::Result>::cast(Result_38240f4c3f9f19dc(__context__,int64_t(__idim_rename_at_201_80)));
    } else if ( __result_rename_at_198_79.tinfo.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tString )
    {
        char * * __pstr_rename_at_205_81 = ((char * *)das_cast<char * *>::cast(getPD_950137d699d78aca(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_198_78),__result_rename_at_198_79,0)));
        return /* <- */ das_auto_cast_move<debug_eval::Result>::cast(Result_38240f4c3f9f19dc(__context__,int64_t(builtin_string_length(das_deref(__context__,__pstr_rename_at_205_81),__context__))));
    } else if ( __result_rename_at_198_79.tinfo.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tArray )
    {
        debugapi::DapiArray * __parr_rename_at_209_82 = ((debugapi::DapiArray *)das_cast<debugapi::DapiArray *>::cast(getPD_950137d699d78aca(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_198_78),__result_rename_at_198_79,0)));
        return /* <- */ das_auto_cast_move<debug_eval::Result>::cast(Result_38240f4c3f9f19dc(__context__,int64_t(__parr_rename_at_209_82->size)));
    } else if ( __result_rename_at_198_79.tinfo.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tTable )
    {
        debugapi::DapiTable * __ptab_rename_at_213_83 = ((debugapi::DapiTable *)das_cast<debugapi::DapiTable *>::cast(getPD_950137d699d78aca(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_198_78),__result_rename_at_198_79,0)));
        return /* <- */ das_auto_cast_move<debug_eval::Result>::cast(Result_38240f4c3f9f19dc(__context__,int64_t(__ptab_rename_at_213_83->size)));
    };
    return das_auto_cast_ref<debug_eval::Result>::cast(error_3b51aee67e872094(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_198_78),das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_5, cast<char * const >::from(((char *) "unsupported length for type ")), cast<DAS_COMMENT(bound_enum) das::Type>::from(__result_rename_at_198_79.tinfo.type /*basicType*/)))));
}

inline debug_eval::Result func_call_e8069ada942ecd18 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_220_84, char * const  __name_rename_at_220_85, debug_eval::Result const  &  __arg_rename_at_220_86 )
{
    if ( SimPolicy<char *>::Equ(cast<char *>::from(__name_rename_at_220_85),cast<char *>::from(((char *) "length")),*__context__,nullptr) )
    {
        return das_auto_cast_ref<debug_eval::Result>::cast(func_call_length_e620fd2edb88dae3(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_220_84),__arg_rename_at_220_86));
    } else {
        return das_auto_cast_ref<debug_eval::Result>::cast(error_3b51aee67e872094(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_220_84),das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_6, cast<char * const >::from(((char *) "unknown function ")), cast<char * const >::from(__name_rename_at_220_85)))));
    };
}

inline debug_eval::Result expr_value_eb120b4e3ca163eb ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_225_87 )
{
    AutoVariant<int64_t,int32_t,char *,char *,char *> __token_rename_at_226_88_ConstRef = ((AutoVariant<int64_t,int32_t,char *,char *,char *>)token_36ce8667ed49d425(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_225_87)));
    AutoVariant<int64_t,int32_t,char *,char *,char *> const  & __token_rename_at_226_88 = __token_rename_at_226_88_ConstRef; ;
    if ( das_get_auto_variant_field<char *,3,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_226_88) )
    {
        return das_auto_cast_ref<debug_eval::Result>::cast(error_3b51aee67e872094(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_225_87),das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_7, cast<char * const >::from(((char *) "invalid token ")), cast<AutoVariant<int64_t,int32_t,char *,char *,char *> const  &>::from(__token_rename_at_226_88), cast<char * const >::from(((char *) ", expecting number or ("))))));
    } else if ( das_get_auto_variant_field<char *,4,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_226_88) )
    {
        return das_auto_cast_ref<debug_eval::Result>::cast(error_3b51aee67e872094(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_225_87),((char *) "unexpected end of stream")));
    } else if ( das_get_auto_variant_field<int64_t,0,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_226_88) )
    {
        return /* <- */ das_auto_cast_move<debug_eval::Result>::cast(Result_38240f4c3f9f19dc(__context__,das_get_auto_variant_field<int64_t,0,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_226_88,__context__)));
    } else if ( das_get_auto_variant_field<char *,2,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_226_88) )
    {
        AutoVariant<int64_t,int32_t,char *,char *,char *> __ntoken_rename_at_238_89 = token_36ce8667ed49d425(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_225_87));
        if ( das_null_coalescing<int32_t>::get(das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::safe_as(__ntoken_rename_at_238_89),0) == 40 )
        {
            debug_eval::Result __arg_rename_at_240_90_ConstRef = ((debug_eval::Result)expr_553a8faea7e2c9cd(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_225_87)));
    debug_eval::Result const  & __arg_rename_at_240_90 = __arg_rename_at_240_90_ConstRef; ;
            das_copy(__ntoken_rename_at_238_89,token_36ce8667ed49d425(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_225_87)));
            if ( das_null_coalescing<int32_t>::get(das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::safe_as(__ntoken_rename_at_238_89),0) != 41 )
            {
                return das_auto_cast_ref<debug_eval::Result>::cast(error_3b51aee67e872094(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_225_87),((char *) "expecting )")));
            } else {
                return das_auto_cast_ref<debug_eval::Result>::cast(func_call_e8069ada942ecd18(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_225_87),das_get_auto_variant_field<char *,2,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_226_88,__context__),__arg_rename_at_240_90));
            };
        };
        unput_c08f1d95eea68a34(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_225_87),das_arg<AutoVariant<int64_t,int32_t,char *,char *,char *>>::pass(__ntoken_rename_at_238_89));
        if ( SimPolicy<char *>::Equ(cast<char *>::from(das_get_auto_variant_field<char *,2,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_226_88,__context__)),cast<char *>::from(((char *) "true")),*__context__,nullptr) )
        {
            return /* <- */ das_auto_cast_move<debug_eval::Result>::cast(Result_b262a84f440eb5c9(__context__,true));
        } else if ( SimPolicy<char *>::Equ(cast<char *>::from(das_get_auto_variant_field<char *,2,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_226_88,__context__)),cast<char *>::from(((char *) "false")),*__context__,nullptr) )
        {
            return /* <- */ das_auto_cast_move<debug_eval::Result>::cast(Result_b262a84f440eb5c9(__context__,false));
        };
        if ( _FuncbuiltinTickkey_existsTick16808803843923989214_f403d698c1fdad14(__context__,das_arg<TTable<char *,debug_eval::Result>>::pass(__st_rename_at_225_87.names),das_get_auto_variant_field<char *,2,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_226_88,__context__)) )
        {
            return das_auto_cast_ref<debug_eval::Result>::cast(_FuncbuiltinTickget_valueTick6803070933163225147_25700dfc8547814(__context__,das_arg<TTable<char *,debug_eval::Result>>::pass(__st_rename_at_225_87.names),das_get_auto_variant_field<char *,2,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_226_88,__context__)));
        } else {
            return das_auto_cast_ref<debug_eval::Result>::cast(error_3b51aee67e872094(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_225_87),das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_8, cast<char * const >::from(((char *) "unknown variable ")), cast<char * const >::from(das_get_auto_variant_field<char *,2,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_226_88,__context__))))));
        };
    } else if ( das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_226_88) && (das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_226_88,__context__) == 40) )
    {
        AutoVariant<int64_t,int32_t,char *,char *,char *> _temp_make_local_260_16_176; _temp_make_local_260_16_176;
        debug_eval::Result __res_rename_at_259_91 = expr_553a8faea7e2c9cd(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_225_87));
        if ( das_null_coalescing<int32_t>::get(das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::safe_as((_temp_make_local_260_16_176 = (token_36ce8667ed49d425(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_225_87))))),0) != 41 )
        {
            return das_auto_cast_ref<debug_eval::Result>::cast(error_3b51aee67e872094(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_225_87),((char *) "expecting )")));
        } else {
            return /* <- */ das_auto_cast_move<debug_eval::Result>::cast(__res_rename_at_259_91);
        };
    } else if ( das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_226_88) )
    {
        return das_auto_cast_ref<debug_eval::Result>::cast(error_3b51aee67e872094(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_225_87),das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_9, cast<char * const >::from(((char *) "unexpected character ")), cast<char * const >::from(((char * const )(to_string_char(das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_226_88,__context__),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) ", expecting number or ("))))));
    };
    return das_auto_cast_ref<debug_eval::Result>::cast(error_3b51aee67e872094(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_225_87),das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_10, cast<char * const >::from(((char *) "unexpected token ")), cast<AutoVariant<int64_t,int32_t,char *,char *,char *> const  &>::from(__token_rename_at_226_88), cast<char * const >::from(((char *) ", expecting number or ("))))));
}

inline AutoTuple<int32_t,TypeInfo> getStructFieldOffset_5201839c067f2b1b ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_272_92, char * const  __ident_rename_at_272_93, debug_eval::Result &  __result_rename_at_272_94 )
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

inline AutoTuple<int32_t,TypeInfo> getTupleFieldOffset_82d2cca0fc285ff0 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_289_99, char * const  __ident_rename_at_289_100, debug_eval::Result &  __result_rename_at_289_101 )
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

inline AutoTuple<int32_t,TypeInfo> getVectorOffset_f396a4629b54077a ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_303_106, char * const  __ident_rename_at_303_107, debug_eval::Result &  __result_rename_at_303_108 )
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

inline AutoTuple<int32_t,TypeInfo> getHandleFieldOffset_17e7898b27c06a3b ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_314_112, char * const  __ident_rename_at_314_113, debug_eval::Result &  __result_rename_at_314_114 )
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

inline debug_eval::Result expr_field_7b9abb4b48818ee6 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_329_118 )
{
    debug_eval::Result __result_rename_at_330_119 = expr_value_eb120b4e3ca163eb(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_329_118));
    AutoVariant<int64_t,int32_t,char *,char *,char *> __token_rename_at_331_120_ConstRef = ((AutoVariant<int64_t,int32_t,char *,char *,char *>)token_36ce8667ed49d425(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_329_118)));
    AutoVariant<int64_t,int32_t,char *,char *,char *> const  & __token_rename_at_331_120 = __token_rename_at_331_120_ConstRef; ;
    if ( das_null_coalescing<int32_t>::get(das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::safe_as(__token_rename_at_331_120),0) == 46 )
    {
        AutoVariant<int64_t,int32_t,char *,char *,char *> __field_rename_at_333_121_ConstRef = ((AutoVariant<int64_t,int32_t,char *,char *,char *>)token_36ce8667ed49d425(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_329_118)));
    AutoVariant<int64_t,int32_t,char *,char *,char *> const  & __field_rename_at_333_121 = __field_rename_at_333_121_ConstRef; ;
        if ( das_get_auto_variant_field<char *,2,int64_t,int32_t,char *,char *,char *>::is(__field_rename_at_333_121) )
        {
            AutoTuple<int32_t,TypeInfo> __oti_rename_at_335_122; das_zero(__oti_rename_at_335_122);
            das_copy(das_get_auto_tuple_field<int32_t,0,int32_t,TypeInfo>::get(__oti_rename_at_335_122),-1);
            if ( __result_rename_at_330_119.tinfo.dimSize /*dimSize*/ != 0x0u )
            {
                return das_auto_cast_ref<debug_eval::Result>::cast(error_3b51aee67e872094(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_329_118),((char *) "can't access field of array")));
            } else if ( __result_rename_at_330_119.tinfo.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tStructure )
            {
                das_copy(__oti_rename_at_335_122,getStructFieldOffset_5201839c067f2b1b(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_329_118),das_get_auto_variant_field<char *,2,int64_t,int32_t,char *,char *,char *>::as(__field_rename_at_333_121,__context__),das_arg<debug_eval::Result>::pass(__result_rename_at_330_119)));
            } else if ( __result_rename_at_330_119.tinfo.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tTuple )
            {
                das_copy(__oti_rename_at_335_122,getTupleFieldOffset_82d2cca0fc285ff0(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_329_118),das_get_auto_variant_field<char *,2,int64_t,int32_t,char *,char *,char *>::as(__field_rename_at_333_121,__context__),das_arg<debug_eval::Result>::pass(__result_rename_at_330_119)));
            } else if ( das_invoke_function<int32_t>::invoke<DAS_COMMENT(bound_enum) das::Type>(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::getVectorElementCount CE<rtti::Type>*/ 0x42f0d692c6e099b8)),__result_rename_at_330_119.tinfo.type /*basicType*/) != 0 )
            {
                das_copy(__oti_rename_at_335_122,getVectorOffset_f396a4629b54077a(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_329_118),das_get_auto_variant_field<char *,2,int64_t,int32_t,char *,char *,char *>::as(__field_rename_at_333_121,__context__),das_arg<debug_eval::Result>::pass(__result_rename_at_330_119)));
            } else if ( __result_rename_at_330_119.tinfo.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tHandle )
            {
                das_copy(__oti_rename_at_335_122,getHandleFieldOffset_17e7898b27c06a3b(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_329_118),das_get_auto_variant_field<char *,2,int64_t,int32_t,char *,char *,char *>::as(__field_rename_at_333_121,__context__),das_arg<debug_eval::Result>::pass(__result_rename_at_330_119)));
            };
            if ( das_get_auto_tuple_field<int32_t,0,int32_t,TypeInfo>::get(__oti_rename_at_335_122) < 0 )
            {
                return das_auto_cast_ref<debug_eval::Result>::cast(error_3b51aee67e872094(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_329_118),das_string_builder(__context__,SimNode_AotInterop<4>(__tinfo_12, cast<char * const >::from(((char *) "unknown field ")), cast<AutoVariant<int64_t,int32_t,char *,char *,char *> const  &>::from(__field_rename_at_333_121), cast<char * const >::from(((char *) " in type ")), cast<DAS_COMMENT(bound_enum) das::Type>::from(__result_rename_at_330_119.tinfo.type /*basicType*/)))));
            } else {
                void * __newData_rename_at_357_123 = ((void *)getPD_950137d699d78aca(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_329_118),das_arg<debug_eval::Result>::pass(__result_rename_at_330_119),das_get_auto_tuple_field<int32_t,0,int32_t,TypeInfo>::get(__oti_rename_at_335_122)));
                das_copy(__result_rename_at_330_119.data,__newData_rename_at_357_123);
                das_copy(__result_rename_at_330_119.tinfo,das_get_auto_tuple_field<TypeInfo,1,int32_t,TypeInfo>::get(__oti_rename_at_335_122));
                return das_auto_cast_ref<debug_eval::Result>::cast(__result_rename_at_330_119);
            };
        } else {
            return das_auto_cast_ref<debug_eval::Result>::cast(error_3b51aee67e872094(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_329_118),((char *) "expecting field name")));
        };
    } else {
        unput_c08f1d95eea68a34(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_329_118),__token_rename_at_331_120);
    };
    return das_auto_cast_ref<debug_eval::Result>::cast(__result_rename_at_330_119);
}

inline debug_eval::Result opDim_e93722f23b2b225d ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_370_124, int64_t __index_rename_at_370_125, debug_eval::Result &  __result_rename_at_370_126 )
{
    int32_t __dim_rename_at_371_127 = ((int32_t)rtti_getDimTypeInfo(das_arg<TypeInfo>::pass(__result_rename_at_370_126.tinfo),int32_t(__result_rename_at_370_126.tinfo.dimSize /*dimSize*/ - 0x1u),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
    if ( (__index_rename_at_370_125 < INT64_C(0)) || (__index_rename_at_370_125 >= int64_t(__dim_rename_at_371_127)) )
    {
        return das_auto_cast_ref<debug_eval::Result>::cast(error_3b51aee67e872094(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_370_124),((char *) "index out of range")));
    } else {
        --__result_rename_at_370_126.tinfo.dimSize /*dimSize*/;
        int32_t __elemSize_rename_at_376_128 = ((int32_t)getTypeSize(das_ref(__context__,__result_rename_at_370_126.tinfo)));
        das_copy(__result_rename_at_370_126.data,getPD_950137d699d78aca(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_370_124),das_arg<debug_eval::Result>::pass(__result_rename_at_370_126),int32_t(__index_rename_at_370_125) * __elemSize_rename_at_376_128));
        return das_auto_cast_ref<debug_eval::Result>::cast(__result_rename_at_370_126);
    };
}

inline debug_eval::Result opDimString_2d98ae0b9aba33b ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_381_129, int64_t __index_rename_at_381_130, debug_eval::Result &  __result_rename_at_381_131 )
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
        return das_auto_cast_ref<debug_eval::Result>::cast(error_3b51aee67e872094(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_381_129),das_string_builder(__context__,SimNode_AotInterop<5>(__tinfo_13, cast<char * const >::from(((char *) "index out of range ")), cast<int64_t>::from(__index_rename_at_381_130), cast<char * const >::from(((char *) " of ")), cast<int32_t>::from(__len_rename_at_390_133), cast<char * const >::from(((char *) " (string too short)"))))));
    } else {
        return das_auto_cast_ref<debug_eval::Result>::cast(Result_38240f4c3f9f19dc(__context__,int64_t(get_character_at(__str_rename_at_382_132,int32_t(__index_rename_at_381_130),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
    };
}

inline debug_eval::Result opArray_dfcf64278e28052b ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_397_134, int64_t __index_rename_at_397_135, debug_eval::Result &  __result_rename_at_397_136 )
{
    debugapi::DapiArray * __pArray_rename_at_399_137 = ((debugapi::DapiArray *)das_cast<debugapi::DapiArray *>::cast(getPD_950137d699d78aca(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_397_134),das_arg<debug_eval::Result>::pass(__result_rename_at_397_136),0)));
    if ( __pArray_rename_at_399_137 == nullptr )
    {
        return das_auto_cast_ref<debug_eval::Result>::cast(error_3b51aee67e872094(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_397_134),((char *) "null pointer")));
    } else {
        uint32_t __dim_rename_at_403_138 = ((uint32_t)__pArray_rename_at_399_137->size);
        if ( (__index_rename_at_397_135 < INT64_C(0)) || (__index_rename_at_397_135 >= int64_t(__dim_rename_at_403_138)) )
        {
            return das_auto_cast_ref<debug_eval::Result>::cast(error_3b51aee67e872094(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_397_134),((char *) "index out of range")));
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

inline debug_eval::Result opTable_2c3c0211d77b2294 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_416_142, debug_eval::Result const  &  __index_rename_at_416_143, debug_eval::Result &  __result_rename_at_416_144 )
{
    debugapi::DapiTable * __pTable_rename_at_418_145 = ((debugapi::DapiTable *)das_cast<debugapi::DapiTable *>::cast(getPD_950137d699d78aca(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_416_142),das_arg<debug_eval::Result>::pass(__result_rename_at_416_144),0)));
    if ( __pTable_rename_at_418_145 == nullptr )
    {
        return das_auto_cast_ref<debug_eval::Result>::cast(error_3b51aee67e872094(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_416_142),((char *) "table - null pointer")));
    } else {
        float4 * __pKey_rename_at_422_146 = ((float4 *)das_cast<float4 *>::cast(getPD_950137d699d78aca(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_416_142),__index_rename_at_416_143,0)));
        if ( __pKey_rename_at_422_146 == nullptr )
        {
            return das_auto_cast_ref<debug_eval::Result>::cast(error_3b51aee67e872094(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_416_142),((char *) "key - null pointer")));
        } else {
            float4 __key_rename_at_426_147 = ((float4)das_deref(__context__,__pKey_rename_at_422_146));
            DAS_COMMENT(bound_enum) das::Type __keyBaseType_rename_at_427_148 = ((DAS_COMMENT(bound_enum) das::Type)__result_rename_at_416_144.tinfo.firstType /*firstType*/->type /*basicType*/);
            int32_t __valueTypeSize_rename_at_428_149 = ((int32_t)getTypeSize(__result_rename_at_416_144.tinfo.secondType /*secondType*/));
            int32_t __elementIndex_rename_at_429_150 = ((int32_t)rtti_getTablePtr(das_auto_cast<void * const >::cast(__pTable_rename_at_418_145),cast<float4>::from(__key_rename_at_426_147),__keyBaseType_rename_at_427_148,__valueTypeSize_rename_at_428_149,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
            if ( __elementIndex_rename_at_429_150 == -1 )
            {
                return das_auto_cast_ref<debug_eval::Result>::cast(error_3b51aee67e872094(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_416_142),((char *) "key not found")));
            } else {
                uint8_t * __pValue_rename_at_433_151 = ((uint8_t *)das_ptr_add_int32(das_cast<uint8_t *>::cast(__pTable_rename_at_418_145->data),__elementIndex_rename_at_429_150 * __valueTypeSize_rename_at_428_149,1));
                das_copy(__result_rename_at_416_144.data,__pValue_rename_at_433_151);
                das_copy(__result_rename_at_416_144.tinfo,das_deref(__context__,__result_rename_at_416_144.tinfo.secondType /*secondType*/));
                return das_auto_cast_ref<debug_eval::Result>::cast(__result_rename_at_416_144);
            };
        };
    };
}

inline debug_eval::Result expr_at_eb04195e4bb4b7e4 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_440_152 )
{
    debug_eval::Result __result_rename_at_441_153 = expr_field_7b9abb4b48818ee6(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_440_152));
    AutoVariant<int64_t,int32_t,char *,char *,char *> __token_rename_at_442_154 = token_36ce8667ed49d425(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_440_152));
    while ( das_null_coalescing<int32_t>::get(das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::safe_as(__token_rename_at_442_154),0) == 91 )
    {
        AutoVariant<int64_t,int32_t,char *,char *,char *> _temp_make_local_445_12_160; _temp_make_local_445_12_160;
        debug_eval::Result __eindex_rename_at_444_155_ConstRef = ((debug_eval::Result)expr_553a8faea7e2c9cd(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_440_152)));
    debug_eval::Result const  & __eindex_rename_at_444_155 = __eindex_rename_at_444_155_ConstRef; ;
        if ( das_null_coalescing<int32_t>::get(das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::safe_as((_temp_make_local_445_12_160 = (token_36ce8667ed49d425(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_440_152))))),0) != 93 )
        {
            return das_auto_cast_ref<debug_eval::Result>::cast(error_3b51aee67e872094(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_440_152),((char *) "expecting ]")));
        } else {
            debug_eval::Result __nresult_rename_at_448_156; das_zero(__nresult_rename_at_448_156);
            if ( (__result_rename_at_441_153.tinfo.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tString) && (__result_rename_at_441_153.tinfo.dimSize /*dimSize*/ == 0x0u) )
            {
                das_copy(__nresult_rename_at_448_156,opDimString_2d98ae0b9aba33b(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_440_152),getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_440_152),__eindex_rename_at_444_155),das_arg<debug_eval::Result>::pass(__result_rename_at_441_153)));
            } else if ( (__result_rename_at_441_153.tinfo.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tArray) && (__result_rename_at_441_153.tinfo.dimSize /*dimSize*/ == 0x0u) )
            {
                das_copy(__nresult_rename_at_448_156,opArray_dfcf64278e28052b(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_440_152),getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_440_152),__eindex_rename_at_444_155),das_arg<debug_eval::Result>::pass(__result_rename_at_441_153)));
            } else if ( (__result_rename_at_441_153.tinfo.type /*basicType*/ == DAS_COMMENT(bound_enum) das::Type::tTable) && (__result_rename_at_441_153.tinfo.dimSize /*dimSize*/ == 0x0u) )
            {
                das_copy(__nresult_rename_at_448_156,opTable_2c3c0211d77b2294(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_440_152),__eindex_rename_at_444_155,das_arg<debug_eval::Result>::pass(__result_rename_at_441_153)));
            } else if ( __result_rename_at_441_153.tinfo.dimSize /*dimSize*/ == 0x0u )
            {
                return das_auto_cast_ref<debug_eval::Result>::cast(error_3b51aee67e872094(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_440_152),((char *) "can't index non-array")));
            } else {
                das_copy(__nresult_rename_at_448_156,opDim_e93722f23b2b225d(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_440_152),getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_440_152),__eindex_rename_at_444_155),das_arg<debug_eval::Result>::pass(__result_rename_at_441_153)));
            };
            das_copy(__result_rename_at_441_153,__nresult_rename_at_448_156);
            das_copy(__token_rename_at_442_154,token_36ce8667ed49d425(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_440_152)));
        };
    };
    unput_c08f1d95eea68a34(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_440_152),das_arg<AutoVariant<int64_t,int32_t,char *,char *,char *>>::pass(__token_rename_at_442_154));
    return das_auto_cast_ref<debug_eval::Result>::cast(__result_rename_at_441_153);
}

inline debug_eval::Result expr_unary_7eafff562dd91c36 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_473_157 )
{
    AutoVariant<int64_t,int32_t,char *,char *,char *> __token_rename_at_474_158_ConstRef = ((AutoVariant<int64_t,int32_t,char *,char *,char *>)token_36ce8667ed49d425(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_473_157)));
    AutoVariant<int64_t,int32_t,char *,char *,char *> const  & __token_rename_at_474_158 = __token_rename_at_474_158_ConstRef; ;
    if ( das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_474_158) && (das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_474_158,__context__) == 43) )
    {
        debug_eval::Result _temp_make_local_477_37_48; _temp_make_local_477_37_48;
        return das_auto_cast_ref<debug_eval::Result>::cast(Result_38240f4c3f9f19dc(__context__,getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_473_157),das_arg<debug_eval::Result>::pass((_temp_make_local_477_37_48 = (expr_unary_7eafff562dd91c36(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_473_157))))))));
    } else if ( das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_474_158) && (das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_474_158,__context__) == 45) )
    {
        debug_eval::Result _temp_make_local_480_38_160; _temp_make_local_480_38_160;
        return das_auto_cast_ref<debug_eval::Result>::cast(Result_38240f4c3f9f19dc(__context__,-getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_473_157),das_arg<debug_eval::Result>::pass((_temp_make_local_480_38_160 = (expr_unary_7eafff562dd91c36(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_473_157))))))));
    } else if ( das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_474_158) && (das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_474_158,__context__) == 126) )
    {
        debug_eval::Result _temp_make_local_483_38_272; _temp_make_local_483_38_272;
        return das_auto_cast_ref<debug_eval::Result>::cast(Result_38240f4c3f9f19dc(__context__,~getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_473_157),das_arg<debug_eval::Result>::pass((_temp_make_local_483_38_272 = (expr_unary_7eafff562dd91c36(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_473_157))))))));
    } else if ( das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_474_158) && (das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_474_158,__context__) == 33) )
    {
        debug_eval::Result _temp_make_local_486_37_384; _temp_make_local_486_37_384;
        return das_auto_cast_ref<debug_eval::Result>::cast(Result_b262a84f440eb5c9(__context__,getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_473_157),das_arg<debug_eval::Result>::pass((_temp_make_local_486_37_384 = (expr_unary_7eafff562dd91c36(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_473_157)))))) == INT64_C(0)));
    };
    unput_c08f1d95eea68a34(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_473_157),__token_rename_at_474_158);
    return das_auto_cast_ref<debug_eval::Result>::cast(expr_at_eb04195e4bb4b7e4(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_473_157)));
}

inline debug_eval::Result expr_mul_div_a01816d0548689b4 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_493_159 )
{
    debug_eval::Result __leftValue_rename_at_494_160 = expr_unary_7eafff562dd91c36(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_493_159));
    AutoVariant<int64_t,int32_t,char *,char *,char *> __token_rename_at_495_161_ConstRef = ((AutoVariant<int64_t,int32_t,char *,char *,char *>)token_36ce8667ed49d425(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_493_159)));
    AutoVariant<int64_t,int32_t,char *,char *,char *> const  & __token_rename_at_495_161 = __token_rename_at_495_161_ConstRef; ;
    if ( das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_495_161) && (das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_495_161,__context__) == 42) )
    {
        debug_eval::Result _temp_make_local_498_61_48; _temp_make_local_498_61_48;
        return das_auto_cast_ref<debug_eval::Result>::cast(Result_38240f4c3f9f19dc(__context__,getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_493_159),das_arg<debug_eval::Result>::pass(__leftValue_rename_at_494_160)) * getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_493_159),das_arg<debug_eval::Result>::pass((_temp_make_local_498_61_48 = (expr_mul_div_a01816d0548689b4(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_493_159))))))));
    } else if ( das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_495_161) && (das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_495_161,__context__) == 47) )
    {
        debug_eval::Result _temp_make_local_501_40_176; _temp_make_local_501_40_176;
        int64_t __rightValue_rename_at_501_162 = ((int64_t)getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_493_159),das_arg<debug_eval::Result>::pass((_temp_make_local_501_40_176 = (expr_mul_div_a01816d0548689b4(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_493_159)))))));
        if ( __rightValue_rename_at_501_162 == INT64_C(0) )
        {
            return das_auto_cast_ref<debug_eval::Result>::cast(error_3b51aee67e872094(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_493_159),((char *) "runtime error, division by 0")));
        } else {
            return das_auto_cast_ref<debug_eval::Result>::cast(Result_38240f4c3f9f19dc(__context__,getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_493_159),das_arg<debug_eval::Result>::pass(__leftValue_rename_at_494_160)) * __rightValue_rename_at_501_162));
        };
    } else {
        unput_c08f1d95eea68a34(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_493_159),__token_rename_at_495_161);
    };
    return /* <- */ das_auto_cast_move<debug_eval::Result>::cast(__leftValue_rename_at_494_160);
}

inline debug_eval::Result expr_add_sub_958f6e8350a25a01 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_514_163 )
{
    debug_eval::Result __leftValue_rename_at_515_164 = expr_mul_div_a01816d0548689b4(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_514_163));
    AutoVariant<int64_t,int32_t,char *,char *,char *> __token_rename_at_516_165_ConstRef = ((AutoVariant<int64_t,int32_t,char *,char *,char *>)token_36ce8667ed49d425(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_514_163)));
    AutoVariant<int64_t,int32_t,char *,char *,char *> const  & __token_rename_at_516_165 = __token_rename_at_516_165_ConstRef; ;
    if ( das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_516_165) && (das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_516_165,__context__) == 43) )
    {
        debug_eval::Result _temp_make_local_519_61_48; _temp_make_local_519_61_48;
        return das_auto_cast_ref<debug_eval::Result>::cast(Result_38240f4c3f9f19dc(__context__,getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_514_163),das_arg<debug_eval::Result>::pass(__leftValue_rename_at_515_164)) + getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_514_163),das_arg<debug_eval::Result>::pass((_temp_make_local_519_61_48 = (expr_add_sub_958f6e8350a25a01(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_514_163))))))));
    } else if ( das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_516_165) && (das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_516_165,__context__) == 45) )
    {
        debug_eval::Result _temp_make_local_522_61_160; _temp_make_local_522_61_160;
        return das_auto_cast_ref<debug_eval::Result>::cast(Result_38240f4c3f9f19dc(__context__,getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_514_163),das_arg<debug_eval::Result>::pass(__leftValue_rename_at_515_164)) - getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_514_163),das_arg<debug_eval::Result>::pass((_temp_make_local_522_61_160 = (expr_add_sub_958f6e8350a25a01(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_514_163))))))));
    } else {
        unput_c08f1d95eea68a34(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_514_163),__token_rename_at_516_165);
    };
    return /* <- */ das_auto_cast_move<debug_eval::Result>::cast(__leftValue_rename_at_515_164);
}

inline bool try_accept_punkt_c1565174265ef7b3 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_531_166, int32_t __punkt_rename_at_531_167 )
{
    AutoVariant<int64_t,int32_t,char *,char *,char *> __token_rename_at_532_168_ConstRef = ((AutoVariant<int64_t,int32_t,char *,char *,char *>)token_36ce8667ed49d425(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_531_166)));
    AutoVariant<int64_t,int32_t,char *,char *,char *> const  & __token_rename_at_532_168 = __token_rename_at_532_168_ConstRef; ;
    if ( das_null_coalescing<int32_t>::get(das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::safe_as(__token_rename_at_532_168),0) != __punkt_rename_at_531_167 )
    {
        unput_c08f1d95eea68a34(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_531_166),__token_rename_at_532_168);
        return das_auto_cast<bool>::cast(false);
    } else {
        return das_auto_cast<bool>::cast(true);
    };
}

inline debug_eval::Result expr_and_9edde39c3aad1048 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_540_169 )
{
    debug_eval::Result __leftValue_rename_at_541_170 = expr_add_sub_958f6e8350a25a01(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_540_169));
    if ( try_accept_punkt_c1565174265ef7b3(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_540_169),38) )
    {
        if ( try_accept_punkt_c1565174265ef7b3(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_540_169),38) )
        {
            debug_eval::Result _temp_make_local_544_62_32; _temp_make_local_544_62_32;
            return das_auto_cast_ref<debug_eval::Result>::cast(Result_b262a84f440eb5c9(__context__,(getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_540_169),das_arg<debug_eval::Result>::pass(__leftValue_rename_at_541_170)) & getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_540_169),das_arg<debug_eval::Result>::pass((_temp_make_local_544_62_32 = (expr_and_9edde39c3aad1048(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_540_169))))))) != INT64_C(0)));
        } else {
            debug_eval::Result _temp_make_local_546_61_144; _temp_make_local_546_61_144;
            return das_auto_cast_ref<debug_eval::Result>::cast(Result_38240f4c3f9f19dc(__context__,getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_540_169),das_arg<debug_eval::Result>::pass(__leftValue_rename_at_541_170)) & getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_540_169),das_arg<debug_eval::Result>::pass((_temp_make_local_546_61_144 = (expr_and_9edde39c3aad1048(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_540_169))))))));
        };
    };
    return /* <- */ das_auto_cast_move<debug_eval::Result>::cast(__leftValue_rename_at_541_170);
}

inline debug_eval::Result expr_xor_466b6dbbcc14397c ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_552_171 )
{
    debug_eval::Result __leftValue_rename_at_553_172 = expr_and_9edde39c3aad1048(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_552_171));
    if ( try_accept_punkt_c1565174265ef7b3(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_552_171),94) )
    {
        if ( try_accept_punkt_c1565174265ef7b3(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_552_171),94) )
        {
            debug_eval::Result _temp_make_local_556_62_32; _temp_make_local_556_62_32;
            return das_auto_cast_ref<debug_eval::Result>::cast(Result_b262a84f440eb5c9(__context__,(getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_552_171),das_arg<debug_eval::Result>::pass(__leftValue_rename_at_553_172)) ^ getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_552_171),das_arg<debug_eval::Result>::pass((_temp_make_local_556_62_32 = (expr_xor_466b6dbbcc14397c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_552_171))))))) != INT64_C(0)));
        } else {
            debug_eval::Result _temp_make_local_558_61_144; _temp_make_local_558_61_144;
            return das_auto_cast_ref<debug_eval::Result>::cast(Result_38240f4c3f9f19dc(__context__,getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_552_171),das_arg<debug_eval::Result>::pass(__leftValue_rename_at_553_172)) ^ getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_552_171),das_arg<debug_eval::Result>::pass((_temp_make_local_558_61_144 = (expr_xor_466b6dbbcc14397c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_552_171))))))));
        };
    };
    return /* <- */ das_auto_cast_move<debug_eval::Result>::cast(__leftValue_rename_at_553_172);
}

inline debug_eval::Result expr_or_bf9fadca84c5914c ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_564_173 )
{
    debug_eval::Result __leftValue_rename_at_565_174 = expr_xor_466b6dbbcc14397c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_564_173));
    if ( try_accept_punkt_c1565174265ef7b3(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_564_173),124) )
    {
        if ( try_accept_punkt_c1565174265ef7b3(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_564_173),124) )
        {
            debug_eval::Result _temp_make_local_568_62_32; _temp_make_local_568_62_32;
            return das_auto_cast_ref<debug_eval::Result>::cast(Result_b262a84f440eb5c9(__context__,(getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_564_173),das_arg<debug_eval::Result>::pass(__leftValue_rename_at_565_174)) | getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_564_173),das_arg<debug_eval::Result>::pass((_temp_make_local_568_62_32 = (expr_or_bf9fadca84c5914c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_564_173))))))) != INT64_C(0)));
        } else {
            debug_eval::Result _temp_make_local_570_61_144; _temp_make_local_570_61_144;
            return das_auto_cast_ref<debug_eval::Result>::cast(Result_38240f4c3f9f19dc(__context__,getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_564_173),das_arg<debug_eval::Result>::pass(__leftValue_rename_at_565_174)) | getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_564_173),das_arg<debug_eval::Result>::pass((_temp_make_local_570_61_144 = (expr_or_bf9fadca84c5914c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_564_173))))))));
        };
    };
    return /* <- */ das_auto_cast_move<debug_eval::Result>::cast(__leftValue_rename_at_565_174);
}

inline debug_eval::Result expr_bool_op_a803ca1de5c0f120 ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_576_175 )
{
    debug_eval::Result __leftValue_rename_at_577_176 = expr_or_bf9fadca84c5914c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175));
    AutoVariant<int64_t,int32_t,char *,char *,char *> __token_rename_at_578_177_ConstRef = ((AutoVariant<int64_t,int32_t,char *,char *,char *>)token_36ce8667ed49d425(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175)));
    AutoVariant<int64_t,int32_t,char *,char *,char *> const  & __token_rename_at_578_177 = __token_rename_at_578_177_ConstRef; ;
    AutoVariant<int64_t,int32_t,char *,char *,char *> __subtoken_rename_at_579_178_ConstRef = ((AutoVariant<int64_t,int32_t,char *,char *,char *>)token_36ce8667ed49d425(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175)));
    AutoVariant<int64_t,int32_t,char *,char *,char *> const  & __subtoken_rename_at_579_178 = __subtoken_rename_at_579_178_ConstRef; ;
    if ( das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_578_177) && ((das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_578_177,__context__) == 33) && (das_null_coalescing<int32_t>::get(das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::safe_as(__subtoken_rename_at_579_178),0) == 61)) )
    {
        debug_eval::Result _temp_make_local_582_62_64; _temp_make_local_582_62_64;
        return das_auto_cast_ref<debug_eval::Result>::cast(Result_b262a84f440eb5c9(__context__,SimPolicy<char *>::NotEqu(cast<char *>::from(getT_e38a914ed5762f78(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175),das_arg<debug_eval::Result>::pass(__leftValue_rename_at_577_176))),cast<char *>::from(getT_e38a914ed5762f78(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175),das_arg<debug_eval::Result>::pass((_temp_make_local_582_62_64 = (expr_bool_op_a803ca1de5c0f120(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175))))))),*__context__,nullptr)));
    } else if ( das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_578_177) && ((das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_578_177,__context__) == 61) && (das_null_coalescing<int32_t>::get(das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::safe_as(__subtoken_rename_at_579_178),0) == 61)) )
    {
        debug_eval::Result _temp_make_local_585_62_176; _temp_make_local_585_62_176;
        return das_auto_cast_ref<debug_eval::Result>::cast(Result_b262a84f440eb5c9(__context__,SimPolicy<char *>::Equ(cast<char *>::from(getT_e38a914ed5762f78(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175),das_arg<debug_eval::Result>::pass(__leftValue_rename_at_577_176))),cast<char *>::from(getT_e38a914ed5762f78(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175),das_arg<debug_eval::Result>::pass((_temp_make_local_585_62_176 = (expr_bool_op_a803ca1de5c0f120(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175))))))),*__context__,nullptr)));
    } else if ( das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_578_177) && ((das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_578_177,__context__) == 60) && (das_null_coalescing<int32_t>::get(das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::safe_as(__subtoken_rename_at_579_178),0) == 61)) )
    {
        debug_eval::Result _temp_make_local_588_62_288; _temp_make_local_588_62_288;
        return das_auto_cast_ref<debug_eval::Result>::cast(Result_b262a84f440eb5c9(__context__,getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175),das_arg<debug_eval::Result>::pass(__leftValue_rename_at_577_176)) <= getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175),das_arg<debug_eval::Result>::pass((_temp_make_local_588_62_288 = (expr_bool_op_a803ca1de5c0f120(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175))))))));
    } else if ( das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_578_177) && (das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_578_177,__context__) == 60) )
    {
        debug_eval::Result _temp_make_local_592_61_400; _temp_make_local_592_61_400;
        unput_c08f1d95eea68a34(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175),__subtoken_rename_at_579_178);
        return das_auto_cast_ref<debug_eval::Result>::cast(Result_b262a84f440eb5c9(__context__,getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175),das_arg<debug_eval::Result>::pass(__leftValue_rename_at_577_176)) < getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175),das_arg<debug_eval::Result>::pass((_temp_make_local_592_61_400 = (expr_bool_op_a803ca1de5c0f120(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175))))))));
    } else if ( das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_578_177) && ((das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_578_177,__context__) == 62) && (das_null_coalescing<int32_t>::get(das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::safe_as(__subtoken_rename_at_579_178),0) == 61)) )
    {
        debug_eval::Result _temp_make_local_595_62_512; _temp_make_local_595_62_512;
        return das_auto_cast_ref<debug_eval::Result>::cast(Result_b262a84f440eb5c9(__context__,getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175),das_arg<debug_eval::Result>::pass(__leftValue_rename_at_577_176)) >= getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175),das_arg<debug_eval::Result>::pass((_temp_make_local_595_62_512 = (expr_bool_op_a803ca1de5c0f120(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175))))))));
    } else if ( das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::is(__token_rename_at_578_177) && (das_get_auto_variant_field<int32_t,1,int64_t,int32_t,char *,char *,char *>::as(__token_rename_at_578_177,__context__) == 62) )
    {
        debug_eval::Result _temp_make_local_599_61_624; _temp_make_local_599_61_624;
        unput_c08f1d95eea68a34(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175),__subtoken_rename_at_579_178);
        return das_auto_cast_ref<debug_eval::Result>::cast(Result_b262a84f440eb5c9(__context__,getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175),das_arg<debug_eval::Result>::pass(__leftValue_rename_at_577_176)) > getI_e470f5a746b3541c(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175),das_arg<debug_eval::Result>::pass((_temp_make_local_599_61_624 = (expr_bool_op_a803ca1de5c0f120(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175))))))));
    } else {
        unput_c08f1d95eea68a34(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175),__subtoken_rename_at_579_178);
        unput_c08f1d95eea68a34(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_576_175),__token_rename_at_578_177);
    };
    return /* <- */ das_auto_cast_move<debug_eval::Result>::cast(__leftValue_rename_at_577_176);
}

inline debug_eval::Result expr_553a8faea7e2c9cd ( Context * __context__, debug_eval::TokenStream &  __st_rename_at_609_179 )
{
    return das_auto_cast_ref<debug_eval::Result>::cast(expr_bool_op_a803ca1de5c0f120(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_609_179)));
}

inline debug_eval::Result debug_eval_cad59dc2d7e9b0bb ( Context * __context__, TTable<char *,debug_eval::Result> const  &  __context_rename_at_613_180, char * const  __expr_rename_at_613_181 )
{
    debug_eval::TokenStream __st_rename_at_614_182; das_zero(__st_rename_at_614_182); das_move(__st_rename_at_614_182, TokenStream_59176a02fe830623(__context__,__expr_rename_at_613_181));
    _FuncbuiltinTickcloneTick3147220447302434744_caf9a18f00151e46(__context__,das_arg<TTable<char *,debug_eval::Result>>::pass(__st_rename_at_614_182.names),__context_rename_at_613_180);
    debug_eval::Result __result_rename_at_616_183 = expr_553a8faea7e2c9cd(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_614_182));
    AutoVariant<int64_t,int32_t,char *,char *,char *> __token_rename_at_617_184_ConstRef = ((AutoVariant<int64_t,int32_t,char *,char *,char *>)token_36ce8667ed49d425(__context__,das_arg<debug_eval::TokenStream>::pass(__st_rename_at_614_182)));
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
        if ( !_FuncbuiltinTickemptyTick15399874715904164783_e7d23d30bed4f24e(__context__,das_arg<TArray<char *>>::pass(__st_rename_at_614_182.errors)) )
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
    aotLib[0x1f8eb5798352d074] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb>>();
    };
    aotLib[0xe55374de1b7aa0a7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickclearTick13739625760977891612_bc21a7c19d5d96ed>>();
    };
    aotLib[0x7b002e2201d6f482] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickkeysTick2205854368403803976_d3d21e2c8adb2e77>>();
    };
    aotLib[0xd326d6f30877a35a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1935193042646774172_fa9f9c260fe9a102>>();
    };
    aotLib[0x56aed9f0fde0dd26] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicknextTick17450348357676149856_ba3f4a54e51a0d57>>();
    };
    aotLib[0x1cac7143c9cb3d64] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_c2f42de354cd5bce>>();
    };
    aotLib[0x3611d31809ac9af] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_f497868bddc80ac1>>();
    };
    aotLib[0xcfe9411d2674cd24] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickeachTick9663565701927713696_acc6981303ddd856>>();
    };
    aotLib[0x6a304785603b7a5a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_32ef8117beffbe31>>();
    };
    aotLib[0x99f47e31f6347dab] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickkey_existsTick16808803843923989214_f403d698c1fdad14>>();
    };
    aotLib[0xef47bba27d899465] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickget_valueTick6803070933163225147_25700dfc8547814>>();
    };
    aotLib[0x65d385b545473069] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickcloneTick3147220447302434744_caf9a18f00151e46>>();
    };
    aotLib[0x9e683f3840fb4e50] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickeachTick4044332007967089362_d13e7572e5849aa5>>();
    };
    aotLib[0xcba4b4c693ad27e9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67>>();
    };
    aotLib[0x690d07de2c41d4fc] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Func_lambda_debug_eval_26_1Tickfunction_f6c2387720b0ce3f>>();
    };
    aotLib[0xcdf520b38ddb084a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Func_lambda_debug_eval_26_1Tickfinalizer_5b0173a727d79cf1>>();
    };
    aotLib[0x4978fd41c995fb2e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickemptyTick15399874715904164783_4d77f401303a908c>>();
    };
    aotLib[0x1c52ced26acd0c1d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicknextTick17450348357676149856_5cb3f1fd943d9108>>();
    };
    aotLib[0xa2c2e5788842f84d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickbackTick18296309835877697278_237276f31a84753f>>();
    };
    aotLib[0xc0005b2dd5939e59] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpopTick1161079256290593740_7a972024f8437b3f>>();
    };
    aotLib[0x83f96c6def0df1ec] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick14133213201864676143_e9749418e214318c>>();
    };
    aotLib[0x3c395992954dd60d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick14133213201864676143_d626258b591934af>>();
    };
    aotLib[0x8b5ab2f45b02d6ff] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcdebug_evalTickloadITick653575658332832056_745a57955fa753cb>>();
    };
    aotLib[0xedcb1c8b6a92a86a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcdebug_evalTickloadITick653575658332832056_d8697a77a47f3f6e>>();
    };
    aotLib[0xd286deff98766854] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcdebug_evalTickloadITick653575658332832056_62319ed67a086d7d>>();
    };
    aotLib[0x9f86584c3d6c6771] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcdebug_evalTickloadITick653575658332832056_f5e0b20ee0adc9d8>>();
    };
    aotLib[0xa7f95fa66c9f690e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcdebug_evalTickloadITick653575658332832056_efd880e7e70477bb>>();
    };
    aotLib[0x637326493f6fb41c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickemptyTick15399874715904164783_e7d23d30bed4f24e>>();
    };
    aotLib[0x738d9e9deff9e9d2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&each_token_96e82efcebf13d0c>>();
    };
    aotLib[0xff4ab1d1c808b55e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&TokenStream_59176a02fe830623>>();
    };
    aotLib[0xabeb188039d63864] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&token_36ce8667ed49d425>>();
    };
    aotLib[0xcbdd97a58389d1d7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&unput_c08f1d95eea68a34>>();
    };
    aotLib[0x141ce1f082098a12] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&Result_38240f4c3f9f19dc>>();
    };
    aotLib[0xc3701292bc5c983b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&Result_b262a84f440eb5c9>>();
    };
    aotLib[0x27beb3bc346b2338] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&InvalidResult_e6549533a9b44e33>>();
    };
    aotLib[0x5cc262f932d84597] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&error_3b51aee67e872094>>();
    };
    aotLib[0xa6269dff87688e8d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&getPD_950137d699d78aca>>();
    };
    aotLib[0x70667c0b0a1bdf90] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&getI_e470f5a746b3541c>>();
    };
    aotLib[0x18da356005b9e967] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&getT_e38a914ed5762f78>>();
    };
    aotLib[0x22476e3bf8dd68e4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&func_call_length_e620fd2edb88dae3>>();
    };
    aotLib[0x767a68a4b2e068a1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&func_call_e8069ada942ecd18>>();
    };
    aotLib[0x55a425bcd1de71c2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&expr_value_eb120b4e3ca163eb>>();
    };
    aotLib[0x5083a97212c3dd15] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&getStructFieldOffset_5201839c067f2b1b>>();
    };
    aotLib[0xe17bcb4778a95f12] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&getTupleFieldOffset_82d2cca0fc285ff0>>();
    };
    aotLib[0x6757e625ec4f37af] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&getVectorOffset_f396a4629b54077a>>();
    };
    aotLib[0xdf37cd901bc6adb1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&getHandleFieldOffset_17e7898b27c06a3b>>();
    };
    aotLib[0x3d70fc4b10a52574] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&expr_field_7b9abb4b48818ee6>>();
    };
    aotLib[0x894eba1f71f973aa] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&opDim_e93722f23b2b225d>>();
    };
    aotLib[0x7c7372303d744bdc] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&opDimString_2d98ae0b9aba33b>>();
    };
    aotLib[0x19a68a43844531f2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&opArray_dfcf64278e28052b>>();
    };
    aotLib[0xdaef6995e8852276] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&opTable_2c3c0211d77b2294>>();
    };
    aotLib[0x8b8365ce3bbbea0b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&expr_at_eb04195e4bb4b7e4>>();
    };
    aotLib[0x3860b8d72e1a66bd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&expr_unary_7eafff562dd91c36>>();
    };
    aotLib[0xdb3e390e2f2cfb46] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&expr_mul_div_a01816d0548689b4>>();
    };
    aotLib[0x788a18fb081c9e5d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&expr_add_sub_958f6e8350a25a01>>();
    };
    aotLib[0x8b2de92aa1cdea8a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&try_accept_punkt_c1565174265ef7b3>>();
    };
    aotLib[0xf6908204c847e37] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&expr_and_9edde39c3aad1048>>();
    };
    aotLib[0xa518daa1fb61b7fe] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&expr_xor_466b6dbbcc14397c>>();
    };
    aotLib[0xa5b47b9aa799c9fd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&expr_or_bf9fadca84c5914c>>();
    };
    aotLib[0xf63fa1e2388437c2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&expr_bool_op_a803ca1de5c0f120>>();
    };
    aotLib[0x34fdfe9a1af1973f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&expr_553a8faea7e2c9cd>>();
    };
    aotLib[0xe577f0a1bef28361] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&debug_eval_cad59dc2d7e9b0bb>>();
    };
    // [[ init script ]]
    aotLib[0x9e75b5e0d855ce21] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<AutoSimNode_Aot<&__init_script>>();
        return ctx.aotInitScript;
    };
	resolveTypeInfoAnnotations();
}

} // namespace _anon_2611991765220838030
AotListBase impl_aot_debug_eval(_anon_2611991765220838030::registerAotFunctions);
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
