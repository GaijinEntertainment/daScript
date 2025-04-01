#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
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
 // require ast_boost
 // require contracts
 // require templates
 // require array_boost
 // require algorithm
 // require templates_boost
 // require macro_boost
 // require defer
 // require apply
 // require archive

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
namespace _anon_13184248888064695127 {

namespace archive { struct Serializer; };
namespace archive { struct MemSerializer; };
namespace archive { struct Archive; };
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
namespace templates { struct DecltypeMacro; };
namespace templates { struct DecltypeNoRefMacro; };
namespace templates { struct TemplateMacro; };
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
namespace apply { struct ApplyMacro; };
// unused enumeration CompilationError
// unused enumeration ConstMatters
// unused enumeration RefMatters
// unused enumeration TemporaryMatters
// unused enumeration Type
// unused enumeration ConversionResult
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
// unused structure DecltypeMacro
// unused structure DecltypeNoRefMacro
// unused structure TemplateMacro
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
// unused structure ApplyMacro
namespace archive {

struct Serializer {
    void * __rtti;
    Func DAS_COMMENT((void,archive::Serializer)) __finalize;
    Func DAS_COMMENT((bool,archive::Serializer,void * const ,int32_t)) write;
    Func DAS_COMMENT((bool,archive::Serializer,void * const ,int32_t)) read;
    Func DAS_COMMENT((void,archive::Serializer,char * const )) error;
    Func DAS_COMMENT((bool,archive::Serializer)) OK;
};
}
namespace archive {

struct MemSerializer {
    void * __rtti;
    Func DAS_COMMENT((void,archive::Serializer)) __finalize;
    Func DAS_COMMENT((bool,archive::Serializer,void * const ,int32_t)) write;
    Func DAS_COMMENT((bool,archive::Serializer,void * const ,int32_t)) read;
    Func DAS_COMMENT((void,archive::Serializer,char * const )) error;
    Func DAS_COMMENT((bool,archive::Serializer)) OK;
    Func DAS_COMMENT((TArray<uint8_t>,archive::MemSerializer)) extractData;
    Func DAS_COMMENT((TArray<uint8_t>,archive::MemSerializer)) getCopyOfData;
    Func DAS_COMMENT((char * const ,archive::MemSerializer)) getLastError;
    TArray<uint8_t> data;
    int32_t readOffset;
    char * lastError;
};
}
namespace archive {

struct Archive {
    uint32_t version;
    bool reading;
    archive::Serializer * stream;
};
}
extern StructInfo __struct_info__14f90bc296d98088;
extern StructInfo __struct_info__2ea4c52d3e792fcf;
extern TypeInfo __type_info__264a3f04ea74314f;
extern TypeInfo __type_info__4ac1d999a882997b;
extern TypeInfo __type_info__8e776352818d5715;
extern TypeInfo __type_info__3a95295ead7e9066;
extern TypeInfo __type_info__af8afe4c86446b52;
extern TypeInfo __type_info__af90fe4c864e9d52;
extern TypeInfo __type_info__e6261f7908d89619;
extern TypeInfo __type_info__d400c4922cb4cfd0;
extern TypeInfo __type_info__af63df4c8601f1a5;
extern TypeInfo __type_info__af5be84c85f468f0;
extern TypeInfo __type_info__af63eb4c86020609;
extern VarInfo __var_info__508c8ab08466caa4;
extern VarInfo __var_info__ea043962cb59a6d8;
extern VarInfo __var_info__1211c90d55d94687;
extern VarInfo __var_info__5a418ba701cef529;
extern VarInfo __var_info__578d75aa7ddc7c60;
extern VarInfo __var_info__1dc770d15b82733a;
extern VarInfo __var_info__b3774d2580cbad26;
extern VarInfo __var_info__d8f5fa42f01f12fa;
extern VarInfo __var_info__d0369009b194ee3d;
extern VarInfo __var_info__98186f1384e81686;
extern VarInfo __var_info__75ddea9dd7569de7;
extern VarInfo __var_info__fb6691029218b641;
extern VarInfo __var_info__100e564366a028df;
extern VarInfo __var_info__87c4dfde8d1d5ca9;
extern VarInfo __var_info__6ed86c478b9e1349;
extern VarInfo __var_info__d3b4679945da3d7d;
extern VarInfo __var_info__4ba92686dc92032b;
extern VarInfo __var_info__2f90929bfe0e523d;

VarInfo __struct_info__14f90bc296d98088_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0x1211c90d55d94687), "__rtti", offsetof(archive::MemSerializer,__rtti), 9 };
TypeInfo * __type_info__ea043962cb59a6d8_arg_types_var_1511252080341909640[1] = { &__type_info__d400c4922cb4cfd0 };
const char * __type_info__ea043962cb59a6d8_arg_names_var_1511252080341909640[1] = { "self" };
VarInfo __struct_info__14f90bc296d98088_field_1 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__ea043962cb59a6d8_arg_types_var_1511252080341909640, __type_info__ea043962cb59a6d8_arg_names_var_1511252080341909640, 1, 0, nullptr, 12, 8, UINT64_C(0xea043962cb59a6d8), "__finalize", offsetof(archive::MemSerializer,__finalize), 0 };
TypeInfo * __type_info__fb6691029218b641_arg_types_var_1511252080341909640[3] = { &__type_info__d400c4922cb4cfd0, &__type_info__3a95295ead7e9066, &__type_info__af8afe4c86446b52 };
const char * __type_info__fb6691029218b641_arg_names_var_1511252080341909640[3] = { "self", "bytes", "size" };
VarInfo __struct_info__14f90bc296d98088_field_2 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__fb6691029218b641_arg_types_var_1511252080341909640, __type_info__fb6691029218b641_arg_names_var_1511252080341909640, 3, 0, nullptr, 12, 8, UINT64_C(0xfb6691029218b641), "write", offsetof(archive::MemSerializer,write), 0 };
TypeInfo * __type_info__98186f1384e81686_arg_types_var_1511252080341909640[3] = { &__type_info__d400c4922cb4cfd0, &__type_info__3a95295ead7e9066, &__type_info__af8afe4c86446b52 };
const char * __type_info__98186f1384e81686_arg_names_var_1511252080341909640[3] = { "self", "bytes", "size" };
VarInfo __struct_info__14f90bc296d98088_field_3 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__98186f1384e81686_arg_types_var_1511252080341909640, __type_info__98186f1384e81686_arg_names_var_1511252080341909640, 3, 0, nullptr, 12, 8, UINT64_C(0x98186f1384e81686), "read", offsetof(archive::MemSerializer,read), 0 };
TypeInfo * __type_info__578d75aa7ddc7c60_arg_types_var_1511252080341909640[2] = { &__type_info__d400c4922cb4cfd0, &__type_info__af90fe4c864e9d52 };
const char * __type_info__578d75aa7ddc7c60_arg_names_var_1511252080341909640[2] = { "self", "code" };
VarInfo __struct_info__14f90bc296d98088_field_4 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__578d75aa7ddc7c60_arg_types_var_1511252080341909640, __type_info__578d75aa7ddc7c60_arg_names_var_1511252080341909640, 2, 0, nullptr, 12, 8, UINT64_C(0x578d75aa7ddc7c60), "error", offsetof(archive::MemSerializer,error), 0 };
TypeInfo * __type_info__508c8ab08466caa4_arg_types_var_1511252080341909640[1] = { &__type_info__d400c4922cb4cfd0 };
const char * __type_info__508c8ab08466caa4_arg_names_var_1511252080341909640[1] = { "self" };
VarInfo __struct_info__14f90bc296d98088_field_5 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__508c8ab08466caa4_arg_types_var_1511252080341909640, __type_info__508c8ab08466caa4_arg_names_var_1511252080341909640, 1, 0, nullptr, 12, 8, UINT64_C(0x508c8ab08466caa4), "OK", offsetof(archive::MemSerializer,OK), 0 };
TypeInfo * __type_info__1dc770d15b82733a_arg_types_var_1511252080341909640[1] = { &__type_info__e6261f7908d89619 };
const char * __type_info__1dc770d15b82733a_arg_names_var_1511252080341909640[1] = { "self" };
VarInfo __struct_info__14f90bc296d98088_field_6 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__264a3f04ea74314f, nullptr, (TypeInfo **)__type_info__1dc770d15b82733a_arg_types_var_1511252080341909640, __type_info__1dc770d15b82733a_arg_names_var_1511252080341909640, 1, 0, nullptr, 12, 8, UINT64_C(0x1dc770d15b82733a), "extractData", offsetof(archive::MemSerializer,extractData), 0 };
TypeInfo * __type_info__b3774d2580cbad26_arg_types_var_1511252080341909640[1] = { &__type_info__e6261f7908d89619 };
const char * __type_info__b3774d2580cbad26_arg_names_var_1511252080341909640[1] = { "self" };
VarInfo __struct_info__14f90bc296d98088_field_7 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__264a3f04ea74314f, nullptr, (TypeInfo **)__type_info__b3774d2580cbad26_arg_types_var_1511252080341909640, __type_info__b3774d2580cbad26_arg_names_var_1511252080341909640, 1, 0, nullptr, 12, 8, UINT64_C(0xb3774d2580cbad26), "getCopyOfData", offsetof(archive::MemSerializer,getCopyOfData), 0 };
TypeInfo * __type_info__d8f5fa42f01f12fa_arg_types_var_1511252080341909640[1] = { &__type_info__e6261f7908d89619 };
const char * __type_info__d8f5fa42f01f12fa_arg_names_var_1511252080341909640[1] = { "self" };
VarInfo __struct_info__14f90bc296d98088_field_8 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af90fe4c864e9d52, nullptr, (TypeInfo **)__type_info__d8f5fa42f01f12fa_arg_types_var_1511252080341909640, __type_info__d8f5fa42f01f12fa_arg_names_var_1511252080341909640, 1, 0, nullptr, 12, 8, UINT64_C(0xd8f5fa42f01f12fa), "getLastError", offsetof(archive::MemSerializer,getLastError), 0 };
VarInfo __struct_info__14f90bc296d98088_field_9 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 106498, 24, UINT64_C(0x5a418ba701cef529), "data", offsetof(archive::MemSerializer,data), 11 };
VarInfo __struct_info__14f90bc296d98088_field_10 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 65564, 4, UINT64_C(0x75ddea9dd7569de7), "readOffset", offsetof(archive::MemSerializer,readOffset), 0 };
VarInfo __struct_info__14f90bc296d98088_field_11 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 81924, 8, UINT64_C(0xd0369009b194ee3d), "lastError", offsetof(archive::MemSerializer,lastError), 12 };
VarInfo * __struct_info__14f90bc296d98088_fields[12] =  { &__struct_info__14f90bc296d98088_field_0, &__struct_info__14f90bc296d98088_field_1, &__struct_info__14f90bc296d98088_field_2, &__struct_info__14f90bc296d98088_field_3, &__struct_info__14f90bc296d98088_field_4, &__struct_info__14f90bc296d98088_field_5, &__struct_info__14f90bc296d98088_field_6, &__struct_info__14f90bc296d98088_field_7, &__struct_info__14f90bc296d98088_field_8, &__struct_info__14f90bc296d98088_field_9, &__struct_info__14f90bc296d98088_field_10, &__struct_info__14f90bc296d98088_field_11 };
StructInfo __struct_info__14f90bc296d98088 = {"MemSerializer", "archive", 29, __struct_info__14f90bc296d98088_fields, 12, 112, UINT64_C(0x0), nullptr, UINT64_C(0x14f90bc296d98088), 0 };
VarInfo __struct_info__2ea4c52d3e792fcf_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0x6ed86c478b9e1349), "__rtti", offsetof(archive::Serializer,__rtti), 6 };
TypeInfo * __type_info__87c4dfde8d1d5ca9_arg_types_var_3361028020037562319[1] = { &__type_info__d400c4922cb4cfd0 };
const char * __type_info__87c4dfde8d1d5ca9_arg_names_var_3361028020037562319[1] = { "self" };
VarInfo __struct_info__2ea4c52d3e792fcf_field_1 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__87c4dfde8d1d5ca9_arg_types_var_3361028020037562319, __type_info__87c4dfde8d1d5ca9_arg_names_var_3361028020037562319, 1, 0, nullptr, 12, 8, UINT64_C(0x87c4dfde8d1d5ca9), "__finalize", offsetof(archive::Serializer,__finalize), 0 };
TypeInfo * __type_info__2f90929bfe0e523d_arg_types_var_3361028020037562319[3] = { &__type_info__d400c4922cb4cfd0, &__type_info__3a95295ead7e9066, &__type_info__af8afe4c86446b52 };
const char * __type_info__2f90929bfe0e523d_arg_names_var_3361028020037562319[3] = { "self", "bytes", "size" };
VarInfo __struct_info__2ea4c52d3e792fcf_field_2 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__2f90929bfe0e523d_arg_types_var_3361028020037562319, __type_info__2f90929bfe0e523d_arg_names_var_3361028020037562319, 3, 0, nullptr, 12, 8, UINT64_C(0x2f90929bfe0e523d), "write", offsetof(archive::Serializer,write), 0 };
TypeInfo * __type_info__4ba92686dc92032b_arg_types_var_3361028020037562319[3] = { &__type_info__d400c4922cb4cfd0, &__type_info__3a95295ead7e9066, &__type_info__af8afe4c86446b52 };
const char * __type_info__4ba92686dc92032b_arg_names_var_3361028020037562319[3] = { "self", "bytes", "size" };
VarInfo __struct_info__2ea4c52d3e792fcf_field_3 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__4ba92686dc92032b_arg_types_var_3361028020037562319, __type_info__4ba92686dc92032b_arg_names_var_3361028020037562319, 3, 0, nullptr, 12, 8, UINT64_C(0x4ba92686dc92032b), "read", offsetof(archive::Serializer,read), 0 };
TypeInfo * __type_info__d3b4679945da3d7d_arg_types_var_3361028020037562319[2] = { &__type_info__d400c4922cb4cfd0, &__type_info__af90fe4c864e9d52 };
const char * __type_info__d3b4679945da3d7d_arg_names_var_3361028020037562319[2] = { "self", "code" };
VarInfo __struct_info__2ea4c52d3e792fcf_field_4 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__d3b4679945da3d7d_arg_types_var_3361028020037562319, __type_info__d3b4679945da3d7d_arg_names_var_3361028020037562319, 2, 0, nullptr, 12, 8, UINT64_C(0xd3b4679945da3d7d), "error", offsetof(archive::Serializer,error), 0 };
TypeInfo * __type_info__100e564366a028df_arg_types_var_3361028020037562319[1] = { &__type_info__d400c4922cb4cfd0 };
const char * __type_info__100e564366a028df_arg_names_var_3361028020037562319[1] = { "self" };
VarInfo __struct_info__2ea4c52d3e792fcf_field_5 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__100e564366a028df_arg_types_var_3361028020037562319, __type_info__100e564366a028df_arg_names_var_3361028020037562319, 1, 0, nullptr, 12, 8, UINT64_C(0x100e564366a028df), "OK", offsetof(archive::Serializer,OK), 0 };
VarInfo * __struct_info__2ea4c52d3e792fcf_fields[6] =  { &__struct_info__2ea4c52d3e792fcf_field_0, &__struct_info__2ea4c52d3e792fcf_field_1, &__struct_info__2ea4c52d3e792fcf_field_2, &__struct_info__2ea4c52d3e792fcf_field_3, &__struct_info__2ea4c52d3e792fcf_field_4, &__struct_info__2ea4c52d3e792fcf_field_5 };
StructInfo __struct_info__2ea4c52d3e792fcf = {"Serializer", "archive", 13, __struct_info__2ea4c52d3e792fcf_fields, 6, 48, UINT64_C(0x0), nullptr, UINT64_C(0x2ea4c52d3e792fcf), 0 };
TypeInfo __type_info__264a3f04ea74314f = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, 24, UINT64_C(0x264a3f04ea74314f) };
TypeInfo __type_info__4ac1d999a882997b = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, 24, UINT64_C(0x4ac1d999a882997b) };
TypeInfo __type_info__8e776352818d5715 = { Type::tStructure, &__struct_info__14f90bc296d98088, nullptr, nullptr, &__type_info__d400c4922cb4cfd0, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 112, UINT64_C(0x8e776352818d5715) };
TypeInfo __type_info__3a95295ead7e9066 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, nullptr, nullptr, 0, 0, nullptr, 8364, 8, UINT64_C(0x3a95295ead7e9066) };
TypeInfo __type_info__af8afe4c86446b52 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 4, UINT64_C(0xaf8afe4c86446b52) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, 8, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__e6261f7908d89619 = { Type::tStructure, &__struct_info__14f90bc296d98088, nullptr, nullptr, &__type_info__d400c4922cb4cfd0, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 112, UINT64_C(0xe6261f7908d89619) };
TypeInfo __type_info__d400c4922cb4cfd0 = { Type::tStructure, &__struct_info__2ea4c52d3e792fcf, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24590, 48, UINT64_C(0xd400c4922cb4cfd0) };
TypeInfo __type_info__af63df4c8601f1a5 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0xaf63df4c8601f1a5) };
TypeInfo __type_info__af5be84c85f468f0 = { Type::tUInt8, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0xaf5be84c85f468f0) };
TypeInfo __type_info__af63eb4c86020609 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 0, UINT64_C(0xaf63eb4c86020609) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[1] = { &__type_info__4ac1d999a882997b };
TypeInfo * __tinfo_1[1] = { &__type_info__8e776352818d5715 };

inline void _FuncbuiltinTickresizeTick4811697762258667383_2333bfdfd8018176 ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_68_0, int32_t __newSize_rename_at_68_1 );
inline TArray<uint8_t> _FuncbuiltinTickclone_to_moveTick2007252383599261567_5374efae61d17e26 ( Context * __context__, TArray<uint8_t> const  & __clone_src_rename_at_1056_2 );
inline void _FuncbuiltinTickcloneTick3038771811667655495_2edc7087c1b82fce ( Context * __context__, TArray<uint8_t> & __a_rename_at_1080_4, TArray<uint8_t> const  & __b_rename_at_1080_5 );
inline TArray<uint8_t> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_5aac315099132c4e ( Context * __context__, TArray<uint8_t> & __a_rename_at_50_7 );
inline archive::MemSerializer & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_d4c9c59da6d9e4b3 ( Context * __context__, archive::MemSerializer & __a_rename_at_50_8 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb ( Context * __context__, TArray<uint8_t> & __a_rename_at_1182_9 );
inline void finalize_c806b47786a4f1c9 ( Context * __context__, archive::Serializer & ____this_rename_at_16_10 );
inline void finalize_85ad7a5b2a02d95a ( Context * __context__, archive::MemSerializer & ____this_rename_at_24_11 );
inline void _FuncarchiveTickserialize_rawTick346513482259279339_c2da6d567d74f5fb ( Context * __context__, archive::Archive & __arch_rename_at_90_12, float3x3 & __value_rename_at_90_13 );
inline void _FuncarchiveTickserialize_rawTick346513482259279339_a227eb837d918a07 ( Context * __context__, archive::Archive & __arch_rename_at_90_14, float3x4 & __value_rename_at_90_15 );
inline void _FuncarchiveTickserialize_rawTick346513482259279339_1a9c5bf871c817f5 ( Context * __context__, archive::Archive & __arch_rename_at_90_16, float4x4 & __value_rename_at_90_17 );
inline void _FuncarchiveTickread_rawTick6802855309095275289_b9dfe21dac02b654 ( Context * __context__, archive::Archive & __arch_rename_at_99_18, int32_t & __value_rename_at_99_19 );
inline void _FuncbuiltinTickresize_no_initTick14746062268774376747_99c206c17e15b53 ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_114_20, int32_t __newSize_rename_at_114_21 );
inline void _FuncarchiveTickwrite_rawTick9760319972816126958_e9195ce9edbb9e44 ( Context * __context__, archive::Archive & __arch_rename_at_105_22, int32_t & __value_rename_at_105_23 );
inline void _FuncSerializer_0x27___finalize_9fd945918e1827c2 ( Context * __context__, archive::Serializer & __self_rename_at_16_24 );
inline archive::MemSerializer MemSerializer_61e9509e2d784369 ( Context * __context__ );
inline void _FuncMemSerializerTickMemSerializer_59fa6e4b02ed33fb ( Context * __context__, archive::MemSerializer & __self_rename_at_26_26 );
inline archive::MemSerializer MemSerializer_b33baca071b78385 ( Context * __context__, TArray<uint8_t> const  & __from_rename_at_30_27 );
inline void _FuncMemSerializerTickMemSerializer_161b88126e8c4555 ( Context * __context__, archive::MemSerializer & __self_rename_at_30_29, TArray<uint8_t> const  & __from_rename_at_30_30 );
inline TArray<uint8_t> _FuncMemSerializerTickextractData_e2917c6b6b7b6ec4 ( Context * __context__, archive::MemSerializer & __self_rename_at_33_31 );
inline TArray<uint8_t> _FuncMemSerializerTickgetCopyOfData_682f289aa33320f2 ( Context * __context__, archive::MemSerializer & __self_rename_at_37_32 );
inline char * _FuncMemSerializerTickgetLastError_b94e4e8380cef718 ( Context * __context__, archive::MemSerializer & __self_rename_at_42_34 );
inline bool _FuncMemSerializerTickOK_a40cae5e96176170 ( Context * __context__, archive::MemSerializer & __self_rename_at_46_35 );
inline bool _FuncMemSerializerTickwrite_42010d5d4d37e6b4 ( Context * __context__, archive::MemSerializer & __self_rename_at_50_36, void * const  __bytes_rename_at_50_37, int32_t __size_rename_at_50_38 );
inline bool _FuncMemSerializerTickread_e5e9e0c17fa011c0 ( Context * __context__, archive::MemSerializer & __self_rename_at_59_40, void * const  __bytes_rename_at_59_41, int32_t __size_rename_at_59_42 );
inline void _FuncMemSerializerTickerror_408f6b66809c6221 ( Context * __context__, archive::MemSerializer & __self_rename_at_74_45, char * const  __code_rename_at_74_46 );
inline void _FuncMemSerializer_0x27___finalize_eb63259fd4b06d18 ( Context * __context__, archive::MemSerializer & __self_rename_at_24_47 );
inline void serialize_864aec01f72813a5 ( Context * __context__, archive::Archive & __arch_rename_at_133_48, float3x3 & __value_rename_at_133_49 );
inline void serialize_2bdd95a82ea20a0e ( Context * __context__, archive::Archive & __arch_rename_at_137_50, float3x4 & __value_rename_at_137_51 );
inline void serialize_a10311ab5d84440e ( Context * __context__, archive::Archive & __arch_rename_at_141_52, float4x4 & __value_rename_at_141_53 );
inline void serialize_e472b981aa88a5bc ( Context * __context__, archive::Archive & __arch_rename_at_230_54, char * & __value_rename_at_230_55 );
inline archive::Serializer Serializer_3a8e571f3a055b73 ( Context * __context__ );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_2333bfdfd8018176 ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_68_0, int32_t __newSize_rename_at_68_1 )
{
    builtin_array_resize(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_68_0),__newSize_rename_at_68_1,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline TArray<uint8_t> _FuncbuiltinTickclone_to_moveTick2007252383599261567_5374efae61d17e26 ( Context * __context__, TArray<uint8_t> const  &  __clone_src_rename_at_1056_2 )
{
    TArray<uint8_t> __clone_dest_rename_at_1058_3; das_zero(__clone_dest_rename_at_1058_3);
    _FuncbuiltinTickcloneTick3038771811667655495_2edc7087c1b82fce(__context__,das_arg<TArray<uint8_t>>::pass(__clone_dest_rename_at_1058_3),__clone_src_rename_at_1056_2);
    return /* <- */ das_auto_cast_move<TArray<uint8_t>>::cast(__clone_dest_rename_at_1058_3);
}

inline void _FuncbuiltinTickcloneTick3038771811667655495_2edc7087c1b82fce ( Context * __context__, TArray<uint8_t> &  __a_rename_at_1080_4, TArray<uint8_t> const  &  __b_rename_at_1080_5 )
{
    int32_t __ln_rename_at_1081_6 = ((int32_t)builtin_array_size(__b_rename_at_1080_5));
    _FuncbuiltinTickresizeTick4811697762258667383_2333bfdfd8018176(__context__,das_arg<TArray<uint8_t>>::pass(__a_rename_at_1080_4),__ln_rename_at_1081_6);
    if ( __ln_rename_at_1081_6 == 0 )
    {
        return ;
    } else {
        das_memcpy(das_auto_cast<void * const >::cast(das_ref(__context__,__a_rename_at_1080_4(0,__context__))),das_auto_cast<void * const >::cast(das_ref(__context__,__b_rename_at_1080_5(0,__context__))),__ln_rename_at_1081_6 * 1);
    };
}

inline TArray<uint8_t> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_5aac315099132c4e ( Context * __context__, TArray<uint8_t> &  __a_rename_at_50_7 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_0,cast<TArray<uint8_t> &>::from(__a_rename_at_50_7)));
    return das_auto_cast_ref<TArray<uint8_t> &>::cast(__a_rename_at_50_7);
}

inline archive::MemSerializer & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_d4c9c59da6d9e4b3 ( Context * __context__, archive::MemSerializer &  __a_rename_at_50_8 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_1,cast<archive::MemSerializer &>::from(__a_rename_at_50_8)));
    return das_auto_cast_ref<archive::MemSerializer &>::cast(__a_rename_at_50_8);
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb ( Context * __context__, TArray<uint8_t> &  __a_rename_at_1182_9 )
{
    builtin_array_free(das_arg<TArray<uint8_t>>::pass(__a_rename_at_1182_9),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void finalize_c806b47786a4f1c9 ( Context * __context__, archive::Serializer &  ____this_rename_at_16_10 )
{
    memset(&(____this_rename_at_16_10), 0, TypeSize<archive::Serializer>::size);
}

inline void finalize_85ad7a5b2a02d95a ( Context * __context__, archive::MemSerializer &  ____this_rename_at_24_11 )
{
    _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_24_11.data));
    memset(&(____this_rename_at_24_11), 0, TypeSize<archive::MemSerializer>::size);
}

inline void _FuncarchiveTickserialize_rawTick346513482259279339_c2da6d567d74f5fb ( Context * __context__, archive::Archive &  __arch_rename_at_90_12, float3x3 &  __value_rename_at_90_13 )
{
    if ( __arch_rename_at_90_12.reading )
    {
        das_invoke_method<bool,offsetof(archive::Serializer,read)>::invoke<archive::Serializer &,float3x3 *,int32_t>(__context__,nullptr,das_arg<archive::Serializer>::pass(das_cast<archive::Serializer>::cast(das_deref(__context__,__arch_rename_at_90_12.stream))),das_ref(__context__,__value_rename_at_90_13),36);
    } else {
        das_invoke_method<bool,offsetof(archive::Serializer,write)>::invoke<archive::Serializer &,float3x3 *,int32_t>(__context__,nullptr,das_arg<archive::Serializer>::pass(das_cast<archive::Serializer>::cast(das_deref(__context__,__arch_rename_at_90_12.stream))),das_ref(__context__,__value_rename_at_90_13),36);
    };
}

inline void _FuncarchiveTickserialize_rawTick346513482259279339_a227eb837d918a07 ( Context * __context__, archive::Archive &  __arch_rename_at_90_14, float3x4 &  __value_rename_at_90_15 )
{
    if ( __arch_rename_at_90_14.reading )
    {
        das_invoke_method<bool,offsetof(archive::Serializer,read)>::invoke<archive::Serializer &,float3x4 *,int32_t>(__context__,nullptr,das_arg<archive::Serializer>::pass(das_cast<archive::Serializer>::cast(das_deref(__context__,__arch_rename_at_90_14.stream))),das_ref(__context__,__value_rename_at_90_15),48);
    } else {
        das_invoke_method<bool,offsetof(archive::Serializer,write)>::invoke<archive::Serializer &,float3x4 *,int32_t>(__context__,nullptr,das_arg<archive::Serializer>::pass(das_cast<archive::Serializer>::cast(das_deref(__context__,__arch_rename_at_90_14.stream))),das_ref(__context__,__value_rename_at_90_15),48);
    };
}

inline void _FuncarchiveTickserialize_rawTick346513482259279339_1a9c5bf871c817f5 ( Context * __context__, archive::Archive &  __arch_rename_at_90_16, float4x4 &  __value_rename_at_90_17 )
{
    if ( __arch_rename_at_90_16.reading )
    {
        das_invoke_method<bool,offsetof(archive::Serializer,read)>::invoke<archive::Serializer &,float4x4 *,int32_t>(__context__,nullptr,das_arg<archive::Serializer>::pass(das_cast<archive::Serializer>::cast(das_deref(__context__,__arch_rename_at_90_16.stream))),das_ref(__context__,__value_rename_at_90_17),64);
    } else {
        das_invoke_method<bool,offsetof(archive::Serializer,write)>::invoke<archive::Serializer &,float4x4 *,int32_t>(__context__,nullptr,das_arg<archive::Serializer>::pass(das_cast<archive::Serializer>::cast(das_deref(__context__,__arch_rename_at_90_16.stream))),das_ref(__context__,__value_rename_at_90_17),64);
    };
}

inline void _FuncarchiveTickread_rawTick6802855309095275289_b9dfe21dac02b654 ( Context * __context__, archive::Archive &  __arch_rename_at_99_18, int32_t & __value_rename_at_99_19 )
{
    DAS_ASSERT((__arch_rename_at_99_18.reading));
    das_invoke_method<bool,offsetof(archive::Serializer,read)>::invoke<archive::Serializer &,int32_t *,int32_t>(__context__,nullptr,das_arg<archive::Serializer>::pass(das_cast<archive::Serializer>::cast(das_deref(__context__,__arch_rename_at_99_18.stream))),das_ref(__context__,__value_rename_at_99_19),4);
}

inline void _FuncbuiltinTickresize_no_initTick14746062268774376747_99c206c17e15b53 ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_114_20, int32_t __newSize_rename_at_114_21 )
{
    builtin_array_resize_no_init(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_114_20),__newSize_rename_at_114_21,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncarchiveTickwrite_rawTick9760319972816126958_e9195ce9edbb9e44 ( Context * __context__, archive::Archive &  __arch_rename_at_105_22, int32_t & __value_rename_at_105_23 )
{
    DAS_ASSERT((!__arch_rename_at_105_22.reading));
    das_invoke_method<bool,offsetof(archive::Serializer,write)>::invoke<archive::Serializer &,int32_t *,int32_t>(__context__,nullptr,das_arg<archive::Serializer>::pass(das_cast<archive::Serializer>::cast(das_deref(__context__,__arch_rename_at_105_22.stream))),das_ref(__context__,__value_rename_at_105_23),4);
}

inline void _FuncSerializer_0x27___finalize_9fd945918e1827c2 ( Context * __context__, archive::Serializer &  __self_rename_at_16_24 )
{
    finalize_c806b47786a4f1c9(__context__,das_arg<archive::Serializer>::pass(__self_rename_at_16_24));
}

inline archive::MemSerializer MemSerializer_61e9509e2d784369 ( Context * __context__ )
{
    archive::MemSerializer __self_rename_at_26_25; das_zero(__self_rename_at_26_25); das_move(__self_rename_at_26_25, (([&]() -> archive::MemSerializer {
        archive::MemSerializer __mks_26;
        das_zero(__mks_26);
        das_copy((__mks_26.__rtti),(((void *)(&__type_info__e6261f7908d89619))));
        das_copy((__mks_26.__finalize),(das_cast<Func DAS_COMMENT((void,archive::Serializer))>::cast(Func(__context__->fnByMangledName(/*@archive::MemSerializer'__finalize S<archive::MemSerializer>*/ 0x46b725974e047090)))));
        das_copy((__mks_26.write),(das_cast<Func DAS_COMMENT((bool,archive::Serializer,void * const ,int32_t))>::cast(Func(__context__->fnByMangledName(/*@archive::MemSerializer`write S<archive::MemSerializer> CI1<v>? Ci*/ 0x83ff0db8e521d488)))));
        das_copy((__mks_26.read),(das_cast<Func DAS_COMMENT((bool,archive::Serializer,void * const ,int32_t))>::cast(Func(__context__->fnByMangledName(/*@archive::MemSerializer`read S<archive::MemSerializer> CI1<v>? Ci*/ 0x3a4b52526506ab72)))));
        das_copy((__mks_26.error),(das_cast<Func DAS_COMMENT((void,archive::Serializer,char * const ))>::cast(Func(__context__->fnByMangledName(/*@archive::MemSerializer`error S<archive::MemSerializer> Cs*/ 0xff2344812949f51d)))));
        das_copy((__mks_26.OK),(das_cast<Func DAS_COMMENT((bool,archive::Serializer))>::cast(Func(__context__->fnByMangledName(/*@archive::MemSerializer`OK S<archive::MemSerializer>*/ 0x5701cc5ecfef657a)))));
        das_copy((__mks_26.extractData),(Func(__context__->fnByMangledName(/*@archive::MemSerializer`extractData S<archive::MemSerializer>*/ 0xb37189bd1ca32cc9))));
        das_copy((__mks_26.getCopyOfData),(Func(__context__->fnByMangledName(/*@archive::MemSerializer`getCopyOfData S<archive::MemSerializer>*/ 0x8d1174c8496d14ce))));
        das_copy((__mks_26.getLastError),(Func(__context__->fnByMangledName(/*@archive::MemSerializer`getLastError S<archive::MemSerializer>*/ 0x5903e37131c8374))));
        return __mks_26;
    })()));
    return /* <- */ das_auto_cast_move<archive::MemSerializer>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_d4c9c59da6d9e4b3(__context__,das_arg<archive::MemSerializer>::pass(__self_rename_at_26_25)));
}

inline void _FuncMemSerializerTickMemSerializer_59fa6e4b02ed33fb ( Context * __context__, archive::MemSerializer &  __self_rename_at_26_26 )
{
}

inline archive::MemSerializer MemSerializer_b33baca071b78385 ( Context * __context__, TArray<uint8_t> const  &  __from_rename_at_30_27 )
{
    archive::MemSerializer __self_rename_at_30_28; das_zero(__self_rename_at_30_28); das_move(__self_rename_at_30_28, (([&]() -> archive::MemSerializer {
        archive::MemSerializer __mks_30;
        das_zero(__mks_30);
        das_copy((__mks_30.__rtti),(((void *)(&__type_info__e6261f7908d89619))));
        das_copy((__mks_30.__finalize),(das_cast<Func DAS_COMMENT((void,archive::Serializer))>::cast(Func(__context__->fnByMangledName(/*@archive::MemSerializer'__finalize S<archive::MemSerializer>*/ 0x46b725974e047090)))));
        das_copy((__mks_30.write),(das_cast<Func DAS_COMMENT((bool,archive::Serializer,void * const ,int32_t))>::cast(Func(__context__->fnByMangledName(/*@archive::MemSerializer`write S<archive::MemSerializer> CI1<v>? Ci*/ 0x83ff0db8e521d488)))));
        das_copy((__mks_30.read),(das_cast<Func DAS_COMMENT((bool,archive::Serializer,void * const ,int32_t))>::cast(Func(__context__->fnByMangledName(/*@archive::MemSerializer`read S<archive::MemSerializer> CI1<v>? Ci*/ 0x3a4b52526506ab72)))));
        das_copy((__mks_30.error),(das_cast<Func DAS_COMMENT((void,archive::Serializer,char * const ))>::cast(Func(__context__->fnByMangledName(/*@archive::MemSerializer`error S<archive::MemSerializer> Cs*/ 0xff2344812949f51d)))));
        das_copy((__mks_30.OK),(das_cast<Func DAS_COMMENT((bool,archive::Serializer))>::cast(Func(__context__->fnByMangledName(/*@archive::MemSerializer`OK S<archive::MemSerializer>*/ 0x5701cc5ecfef657a)))));
        das_copy((__mks_30.extractData),(Func(__context__->fnByMangledName(/*@archive::MemSerializer`extractData S<archive::MemSerializer>*/ 0xb37189bd1ca32cc9))));
        das_copy((__mks_30.getCopyOfData),(Func(__context__->fnByMangledName(/*@archive::MemSerializer`getCopyOfData S<archive::MemSerializer>*/ 0x8d1174c8496d14ce))));
        das_copy((__mks_30.getLastError),(Func(__context__->fnByMangledName(/*@archive::MemSerializer`getLastError S<archive::MemSerializer>*/ 0x5903e37131c8374))));
        return __mks_30;
    })()));
    _FuncMemSerializerTickMemSerializer_161b88126e8c4555(__context__,das_arg<archive::MemSerializer>::pass(__self_rename_at_30_28),__from_rename_at_30_27);
    return /* <- */ das_auto_cast_move<archive::MemSerializer>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_d4c9c59da6d9e4b3(__context__,das_arg<archive::MemSerializer>::pass(__self_rename_at_30_28)));
}

inline void _FuncMemSerializerTickMemSerializer_161b88126e8c4555 ( Context * __context__, archive::MemSerializer &  __self_rename_at_30_29, TArray<uint8_t> const  &  __from_rename_at_30_30 )
{
    _FuncbuiltinTickcloneTick3038771811667655495_2edc7087c1b82fce(__context__,das_arg<TArray<uint8_t>>::pass(__self_rename_at_30_29.data),__from_rename_at_30_30);
}

inline TArray<uint8_t> _FuncMemSerializerTickextractData_e2917c6b6b7b6ec4 ( Context * __context__, archive::MemSerializer &  __self_rename_at_33_31 )
{
    return /* <- */ das_auto_cast_move<TArray<uint8_t>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_5aac315099132c4e(__context__,das_arg<TArray<uint8_t>>::pass(__self_rename_at_33_31.data)));
}

inline TArray<uint8_t> _FuncMemSerializerTickgetCopyOfData_682f289aa33320f2 ( Context * __context__, archive::MemSerializer &  __self_rename_at_37_32 )
{
    TArray<uint8_t> __cdata_rename_at_39_33; das_zero(__cdata_rename_at_39_33); das_move(__cdata_rename_at_39_33, _FuncbuiltinTickclone_to_moveTick2007252383599261567_5374efae61d17e26(__context__,das_arg<TArray<uint8_t>>::pass(__self_rename_at_37_32.data)));
    return /* <- */ das_auto_cast_move<TArray<uint8_t>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_5aac315099132c4e(__context__,das_arg<TArray<uint8_t>>::pass(__cdata_rename_at_39_33)));
}

inline char * _FuncMemSerializerTickgetLastError_b94e4e8380cef718 ( Context * __context__, archive::MemSerializer &  __self_rename_at_42_34 )
{
    return das_auto_cast<char *>::cast(__self_rename_at_42_34.lastError);
}

inline bool _FuncMemSerializerTickOK_a40cae5e96176170 ( Context * __context__, archive::MemSerializer &  __self_rename_at_46_35 )
{
    return das_auto_cast<bool>::cast(SimPolicy<char *>::Equ(cast<char *>::from(__self_rename_at_46_35.lastError),cast<char *>::from(nullptr),*__context__,nullptr));
}

inline bool _FuncMemSerializerTickwrite_42010d5d4d37e6b4 ( Context * __context__, archive::MemSerializer &  __self_rename_at_50_36, void * const  __bytes_rename_at_50_37, int32_t __size_rename_at_50_38 )
{
    int32_t __pos_rename_at_52_39 = ((int32_t)builtin_array_size(das_arg<TArray<uint8_t>>::pass(__self_rename_at_50_36.data)));
    _FuncbuiltinTickresize_no_initTick14746062268774376747_99c206c17e15b53(__context__,das_arg<TArray<uint8_t>>::pass(__self_rename_at_50_36.data),__pos_rename_at_52_39 + __size_rename_at_50_38);
    das_memcpy(das_auto_cast<void * const >::cast(das_ref(__context__,__self_rename_at_50_36.data(__pos_rename_at_52_39,__context__))),__bytes_rename_at_50_37,__size_rename_at_50_38);
    return das_auto_cast<bool>::cast(true);
}

inline bool _FuncMemSerializerTickread_e5e9e0c17fa011c0 ( Context * __context__, archive::MemSerializer &  __self_rename_at_59_40, void * const  __bytes_rename_at_59_41, int32_t __size_rename_at_59_42 )
{
    int32_t __newOffset_rename_at_61_43 = ((int32_t)(__self_rename_at_59_40.readOffset + __size_rename_at_59_42));
    int32_t __maxOffset_rename_at_62_44 = ((int32_t)builtin_array_size(das_arg<TArray<uint8_t>>::pass(__self_rename_at_59_40.data)));
    if ( __newOffset_rename_at_61_43 > __maxOffset_rename_at_62_44 )
    {
        builtin_error(((char *) "reading past the end of stream"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        das_copy(__self_rename_at_59_40.readOffset,__maxOffset_rename_at_62_44);
        return das_auto_cast<bool>::cast(false);
    } else {
        das_memcpy(__bytes_rename_at_59_41,das_auto_cast<void * const >::cast(das_ref(__context__,__self_rename_at_59_40.data(__self_rename_at_59_40.readOffset,__context__))),__size_rename_at_59_42);
        das_copy(__self_rename_at_59_40.readOffset,__newOffset_rename_at_61_43);
        return das_auto_cast<bool>::cast(true);
    };
}

inline void _FuncMemSerializerTickerror_408f6b66809c6221 ( Context * __context__, archive::MemSerializer &  __self_rename_at_74_45, char * const  __code_rename_at_74_46 )
{
    das_copy(__self_rename_at_74_45.lastError,__code_rename_at_74_46);
}

inline void _FuncMemSerializer_0x27___finalize_eb63259fd4b06d18 ( Context * __context__, archive::MemSerializer &  __self_rename_at_24_47 )
{
    finalize_85ad7a5b2a02d95a(__context__,das_arg<archive::MemSerializer>::pass(__self_rename_at_24_47));
}

inline void serialize_864aec01f72813a5 ( Context * __context__, archive::Archive &  __arch_rename_at_133_48, float3x3 &  __value_rename_at_133_49 )
{
    _FuncarchiveTickserialize_rawTick346513482259279339_c2da6d567d74f5fb(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_133_48),das_arg<float3x3>::pass(__value_rename_at_133_49));
}

inline void serialize_2bdd95a82ea20a0e ( Context * __context__, archive::Archive &  __arch_rename_at_137_50, float3x4 &  __value_rename_at_137_51 )
{
    _FuncarchiveTickserialize_rawTick346513482259279339_a227eb837d918a07(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_137_50),das_arg<float3x4>::pass(__value_rename_at_137_51));
}

inline void serialize_a10311ab5d84440e ( Context * __context__, archive::Archive &  __arch_rename_at_141_52, float4x4 &  __value_rename_at_141_53 )
{
    _FuncarchiveTickserialize_rawTick346513482259279339_1a9c5bf871c817f5(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_141_52),das_arg<float4x4>::pass(__value_rename_at_141_53));
}

inline void serialize_e472b981aa88a5bc ( Context * __context__, archive::Archive &  __arch_rename_at_230_54, char * & __value_rename_at_230_55 )
{
    if ( __arch_rename_at_230_54.reading )
    {
        int32_t __len_rename_at_233_56 = 0;
        _FuncarchiveTickread_rawTick6802855309095275289_b9dfe21dac02b654(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_230_54),__len_rename_at_233_56);
        if ( __len_rename_at_233_56 > 0 )
        {
            TArray<uint8_t> __tbuf_rename_at_236_57; das_zero(__tbuf_rename_at_236_57);
            _FuncbuiltinTickresize_no_initTick14746062268774376747_99c206c17e15b53(__context__,das_arg<TArray<uint8_t>>::pass(__tbuf_rename_at_236_57),__len_rename_at_233_56 + 1);
            das_invoke_method<bool,offsetof(archive::Serializer,read)>::invoke<archive::Serializer &,uint8_t *,int32_t>(__context__,nullptr,das_arg<archive::Serializer>::pass(das_cast<archive::Serializer>::cast(das_deref(__context__,__arch_rename_at_230_54.stream))),das_ref(__context__,__tbuf_rename_at_236_57(0,__context__)),__len_rename_at_233_56);
            das_copy(__tbuf_rename_at_236_57(__len_rename_at_233_56,__context__),0x0);
            das_copy(__value_rename_at_230_55,((char * const )(builtin_string_clone(das_cast<char *>::cast(das_ref(__context__,__tbuf_rename_at_236_57(0,__context__))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
            _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb(__context__,das_arg<TArray<uint8_t>>::pass(__tbuf_rename_at_236_57));
        } else {
            das_copy(__value_rename_at_230_55,nullptr);
        };
    } else {
        int32_t __len_rename_at_246_58 = builtin_string_length(__value_rename_at_230_55,__context__);
        _FuncarchiveTickwrite_rawTick9760319972816126958_e9195ce9edbb9e44(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_230_54),__len_rename_at_246_58);
        if ( __len_rename_at_246_58 > 0 )
        {
            das_invoke_method<bool,offsetof(archive::Serializer,write)>::invoke<archive::Serializer &,void *,int32_t>(__context__,nullptr,das_arg<archive::Serializer>::pass(das_cast<archive::Serializer>::cast(das_deref(__context__,__arch_rename_at_230_54.stream))),das_cast<void *>::cast(__value_rename_at_230_55),__len_rename_at_246_58);
        };
    };
}

inline archive::Serializer Serializer_3a8e571f3a055b73 ( Context * __context__ )
{
    return /* <- */ das_auto_cast_move<archive::Serializer>::cast((([&]() -> archive::Serializer {
        archive::Serializer __mks_16;
        das_zero(__mks_16);
        das_copy((__mks_16.__rtti),(((void *)(&__type_info__d400c4922cb4cfd0))));
        das_copy((__mks_16.__finalize),(Func(__context__->fnByMangledName(/*@archive::Serializer'__finalize S<archive::Serializer>*/ 0x81c3c802e71b7fce))));
        return __mks_16;
    })()));
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0xc502128bbf7c5d21] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_2333bfdfd8018176>>();
    };
    aotLib[0xca1af320824cace5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickclone_to_moveTick2007252383599261567_5374efae61d17e26>>();
    };
    aotLib[0x9ae1e096a795eae2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickcloneTick3038771811667655495_2edc7087c1b82fce>>();
    };
    aotLib[0xa8f929474bbb24c7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_5aac315099132c4e>>();
    };
    aotLib[0x3439bb3602e683e5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_d4c9c59da6d9e4b3>>();
    };
    aotLib[0x1f8eb5798352d074] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb>>();
    };
    aotLib[0x592a7d4951e74f25] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_c806b47786a4f1c9>>();
    };
    aotLib[0x3a1da5e9d200abdc] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_85ad7a5b2a02d95a>>();
    };
    aotLib[0x211ee1572cc4d31] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserialize_rawTick346513482259279339_c2da6d567d74f5fb>>();
    };
    aotLib[0x60cee17d6a7a88c4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserialize_rawTick346513482259279339_a227eb837d918a07>>();
    };
    aotLib[0x7cf87cb8ee7dd6a4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserialize_rawTick346513482259279339_1a9c5bf871c817f5>>();
    };
    aotLib[0x13130f7ba25304c0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickread_rawTick6802855309095275289_b9dfe21dac02b654>>();
    };
    aotLib[0x59646fca104fdabb] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresize_no_initTick14746062268774376747_99c206c17e15b53>>();
    };
    aotLib[0x991c09da6401e817] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickwrite_rawTick9760319972816126958_e9195ce9edbb9e44>>();
    };
    aotLib[0xfaab756b0f2d55a8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncSerializer_0x27___finalize_9fd945918e1827c2>>();
    };
    aotLib[0xdabc50ec882fef0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&MemSerializer_61e9509e2d784369>>();
    };
    aotLib[0xe895d28e29c9a4c4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncMemSerializerTickMemSerializer_59fa6e4b02ed33fb>>();
    };
    aotLib[0x3d3a1066fc2bc2be] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&MemSerializer_b33baca071b78385>>();
    };
    aotLib[0xc5353c3949580f0b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncMemSerializerTickMemSerializer_161b88126e8c4555>>();
    };
    aotLib[0x653a902d3313001a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncMemSerializerTickextractData_e2917c6b6b7b6ec4>>();
    };
    aotLib[0xf5e0a29b44c8f284] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncMemSerializerTickgetCopyOfData_682f289aa33320f2>>();
    };
    aotLib[0xef63b8b3f92572fd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncMemSerializerTickgetLastError_b94e4e8380cef718>>();
    };
    aotLib[0xc67915df15c39d13] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncMemSerializerTickOK_a40cae5e96176170>>();
    };
    aotLib[0xbfb9e4f067f3d2dd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncMemSerializerTickwrite_42010d5d4d37e6b4>>();
    };
    aotLib[0x6036cb960a58c161] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncMemSerializerTickread_e5e9e0c17fa011c0>>();
    };
    aotLib[0xe2ee72e04f66ee4c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncMemSerializerTickerror_408f6b66809c6221>>();
    };
    aotLib[0xbd76d8b56815e9a1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncMemSerializer_0x27___finalize_eb63259fd4b06d18>>();
    };
    aotLib[0x6160a41cea8b03d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&serialize_864aec01f72813a5>>();
    };
    aotLib[0xad0f4dae224f56ef] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&serialize_2bdd95a82ea20a0e>>();
    };
    aotLib[0xd1ad5385f180e8f8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&serialize_a10311ab5d84440e>>();
    };
    aotLib[0xeac25f0c9e287515] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&serialize_e472b981aa88a5bc>>();
    };
    aotLib[0xc8758706775393ff] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&Serializer_3a8e571f3a055b73>>();
    };
	resolveTypeInfoAnnotations();
}

} // namespace _anon_13184248888064695127
AotListBase impl_aot_archive(_anon_13184248888064695127::registerAotFunctions);
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
