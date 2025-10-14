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
namespace _anon_9563944200698030257 {

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

VarInfo __struct_info__14f90bc296d98088_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, TypeSize<void *>::size, UINT64_C(0x1211c90d55d94687), "__rtti", offsetof(archive::MemSerializer,__rtti), 9 };
TypeInfo * __type_info__ea043962cb59a6d8_arg_types_var_1511252080341909640[1] = { &__type_info__d400c4922cb4cfd0 };
const char * __type_info__ea043962cb59a6d8_arg_names_var_1511252080341909640[1] = { "self" };
VarInfo __struct_info__14f90bc296d98088_field_1 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__ea043962cb59a6d8_arg_types_var_1511252080341909640, __type_info__ea043962cb59a6d8_arg_names_var_1511252080341909640, 1, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,archive::Serializer))>::size, UINT64_C(0xea043962cb59a6d8), "__finalize", offsetof(archive::MemSerializer,__finalize), 0 };
TypeInfo * __type_info__fb6691029218b641_arg_types_var_1511252080341909640[3] = { &__type_info__d400c4922cb4cfd0, &__type_info__3a95295ead7e9066, &__type_info__af8afe4c86446b52 };
const char * __type_info__fb6691029218b641_arg_names_var_1511252080341909640[3] = { "self", "bytes", "size" };
VarInfo __struct_info__14f90bc296d98088_field_2 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__fb6691029218b641_arg_types_var_1511252080341909640, __type_info__fb6691029218b641_arg_names_var_1511252080341909640, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,archive::Serializer,void * const ,int32_t))>::size, UINT64_C(0xfb6691029218b641), "write", offsetof(archive::MemSerializer,write), 0 };
TypeInfo * __type_info__98186f1384e81686_arg_types_var_1511252080341909640[3] = { &__type_info__d400c4922cb4cfd0, &__type_info__3a95295ead7e9066, &__type_info__af8afe4c86446b52 };
const char * __type_info__98186f1384e81686_arg_names_var_1511252080341909640[3] = { "self", "bytes", "size" };
VarInfo __struct_info__14f90bc296d98088_field_3 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__98186f1384e81686_arg_types_var_1511252080341909640, __type_info__98186f1384e81686_arg_names_var_1511252080341909640, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,archive::Serializer,void * const ,int32_t))>::size, UINT64_C(0x98186f1384e81686), "read", offsetof(archive::MemSerializer,read), 0 };
TypeInfo * __type_info__578d75aa7ddc7c60_arg_types_var_1511252080341909640[2] = { &__type_info__d400c4922cb4cfd0, &__type_info__af90fe4c864e9d52 };
const char * __type_info__578d75aa7ddc7c60_arg_names_var_1511252080341909640[2] = { "self", "code" };
VarInfo __struct_info__14f90bc296d98088_field_4 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__578d75aa7ddc7c60_arg_types_var_1511252080341909640, __type_info__578d75aa7ddc7c60_arg_names_var_1511252080341909640, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,archive::Serializer,char * const ))>::size, UINT64_C(0x578d75aa7ddc7c60), "error", offsetof(archive::MemSerializer,error), 0 };
TypeInfo * __type_info__508c8ab08466caa4_arg_types_var_1511252080341909640[1] = { &__type_info__d400c4922cb4cfd0 };
const char * __type_info__508c8ab08466caa4_arg_names_var_1511252080341909640[1] = { "self" };
VarInfo __struct_info__14f90bc296d98088_field_5 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__508c8ab08466caa4_arg_types_var_1511252080341909640, __type_info__508c8ab08466caa4_arg_names_var_1511252080341909640, 1, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,archive::Serializer))>::size, UINT64_C(0x508c8ab08466caa4), "OK", offsetof(archive::MemSerializer,OK), 0 };
TypeInfo * __type_info__1dc770d15b82733a_arg_types_var_1511252080341909640[1] = { &__type_info__e6261f7908d89619 };
const char * __type_info__1dc770d15b82733a_arg_names_var_1511252080341909640[1] = { "self" };
VarInfo __struct_info__14f90bc296d98088_field_6 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__264a3f04ea74314f, nullptr, (TypeInfo **)__type_info__1dc770d15b82733a_arg_types_var_1511252080341909640, __type_info__1dc770d15b82733a_arg_names_var_1511252080341909640, 1, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((TArray<uint8_t>,archive::MemSerializer))>::size, UINT64_C(0x1dc770d15b82733a), "extractData", offsetof(archive::MemSerializer,extractData), 0 };
TypeInfo * __type_info__b3774d2580cbad26_arg_types_var_1511252080341909640[1] = { &__type_info__e6261f7908d89619 };
const char * __type_info__b3774d2580cbad26_arg_names_var_1511252080341909640[1] = { "self" };
VarInfo __struct_info__14f90bc296d98088_field_7 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__264a3f04ea74314f, nullptr, (TypeInfo **)__type_info__b3774d2580cbad26_arg_types_var_1511252080341909640, __type_info__b3774d2580cbad26_arg_names_var_1511252080341909640, 1, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((TArray<uint8_t>,archive::MemSerializer))>::size, UINT64_C(0xb3774d2580cbad26), "getCopyOfData", offsetof(archive::MemSerializer,getCopyOfData), 0 };
TypeInfo * __type_info__d8f5fa42f01f12fa_arg_types_var_1511252080341909640[1] = { &__type_info__e6261f7908d89619 };
const char * __type_info__d8f5fa42f01f12fa_arg_names_var_1511252080341909640[1] = { "self" };
VarInfo __struct_info__14f90bc296d98088_field_8 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af90fe4c864e9d52, nullptr, (TypeInfo **)__type_info__d8f5fa42f01f12fa_arg_types_var_1511252080341909640, __type_info__d8f5fa42f01f12fa_arg_names_var_1511252080341909640, 1, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((char * const ,archive::MemSerializer))>::size, UINT64_C(0xd8f5fa42f01f12fa), "getLastError", offsetof(archive::MemSerializer,getLastError), 0 };
VarInfo __struct_info__14f90bc296d98088_field_9 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 106498, TypeSize<TArray<uint8_t>>::size, UINT64_C(0x5a418ba701cef529), "data", offsetof(archive::MemSerializer,data), 11 };
VarInfo __struct_info__14f90bc296d98088_field_10 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 65564, TypeSize<int32_t>::size, UINT64_C(0x75ddea9dd7569de7), "readOffset", offsetof(archive::MemSerializer,readOffset), 0 };
VarInfo __struct_info__14f90bc296d98088_field_11 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 81924, TypeSize<char *>::size, UINT64_C(0xd0369009b194ee3d), "lastError", offsetof(archive::MemSerializer,lastError), 12 };
VarInfo * __struct_info__14f90bc296d98088_fields[12] =  { &__struct_info__14f90bc296d98088_field_0, &__struct_info__14f90bc296d98088_field_1, &__struct_info__14f90bc296d98088_field_2, &__struct_info__14f90bc296d98088_field_3, &__struct_info__14f90bc296d98088_field_4, &__struct_info__14f90bc296d98088_field_5, &__struct_info__14f90bc296d98088_field_6, &__struct_info__14f90bc296d98088_field_7, &__struct_info__14f90bc296d98088_field_8, &__struct_info__14f90bc296d98088_field_9, &__struct_info__14f90bc296d98088_field_10, &__struct_info__14f90bc296d98088_field_11 };
StructInfo __struct_info__14f90bc296d98088 = {"MemSerializer", "archive", 29, __struct_info__14f90bc296d98088_fields, 12, TypeSize<archive::MemSerializer>::size, UINT64_C(0x0), nullptr, UINT64_C(0x14f90bc296d98088), 0 };
VarInfo __struct_info__2ea4c52d3e792fcf_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, TypeSize<void *>::size, UINT64_C(0x6ed86c478b9e1349), "__rtti", offsetof(archive::Serializer,__rtti), 6 };
TypeInfo * __type_info__87c4dfde8d1d5ca9_arg_types_var_3361028020037562319[1] = { &__type_info__d400c4922cb4cfd0 };
const char * __type_info__87c4dfde8d1d5ca9_arg_names_var_3361028020037562319[1] = { "self" };
VarInfo __struct_info__2ea4c52d3e792fcf_field_1 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__87c4dfde8d1d5ca9_arg_types_var_3361028020037562319, __type_info__87c4dfde8d1d5ca9_arg_names_var_3361028020037562319, 1, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,archive::Serializer))>::size, UINT64_C(0x87c4dfde8d1d5ca9), "__finalize", offsetof(archive::Serializer,__finalize), 0 };
TypeInfo * __type_info__2f90929bfe0e523d_arg_types_var_3361028020037562319[3] = { &__type_info__d400c4922cb4cfd0, &__type_info__3a95295ead7e9066, &__type_info__af8afe4c86446b52 };
const char * __type_info__2f90929bfe0e523d_arg_names_var_3361028020037562319[3] = { "self", "bytes", "size" };
VarInfo __struct_info__2ea4c52d3e792fcf_field_2 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__2f90929bfe0e523d_arg_types_var_3361028020037562319, __type_info__2f90929bfe0e523d_arg_names_var_3361028020037562319, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,archive::Serializer,void * const ,int32_t))>::size, UINT64_C(0x2f90929bfe0e523d), "write", offsetof(archive::Serializer,write), 0 };
TypeInfo * __type_info__4ba92686dc92032b_arg_types_var_3361028020037562319[3] = { &__type_info__d400c4922cb4cfd0, &__type_info__3a95295ead7e9066, &__type_info__af8afe4c86446b52 };
const char * __type_info__4ba92686dc92032b_arg_names_var_3361028020037562319[3] = { "self", "bytes", "size" };
VarInfo __struct_info__2ea4c52d3e792fcf_field_3 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__4ba92686dc92032b_arg_types_var_3361028020037562319, __type_info__4ba92686dc92032b_arg_names_var_3361028020037562319, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,archive::Serializer,void * const ,int32_t))>::size, UINT64_C(0x4ba92686dc92032b), "read", offsetof(archive::Serializer,read), 0 };
TypeInfo * __type_info__d3b4679945da3d7d_arg_types_var_3361028020037562319[2] = { &__type_info__d400c4922cb4cfd0, &__type_info__af90fe4c864e9d52 };
const char * __type_info__d3b4679945da3d7d_arg_names_var_3361028020037562319[2] = { "self", "code" };
VarInfo __struct_info__2ea4c52d3e792fcf_field_4 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__d3b4679945da3d7d_arg_types_var_3361028020037562319, __type_info__d3b4679945da3d7d_arg_names_var_3361028020037562319, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,archive::Serializer,char * const ))>::size, UINT64_C(0xd3b4679945da3d7d), "error", offsetof(archive::Serializer,error), 0 };
TypeInfo * __type_info__100e564366a028df_arg_types_var_3361028020037562319[1] = { &__type_info__d400c4922cb4cfd0 };
const char * __type_info__100e564366a028df_arg_names_var_3361028020037562319[1] = { "self" };
VarInfo __struct_info__2ea4c52d3e792fcf_field_5 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__100e564366a028df_arg_types_var_3361028020037562319, __type_info__100e564366a028df_arg_names_var_3361028020037562319, 1, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,archive::Serializer))>::size, UINT64_C(0x100e564366a028df), "OK", offsetof(archive::Serializer,OK), 0 };
VarInfo * __struct_info__2ea4c52d3e792fcf_fields[6] =  { &__struct_info__2ea4c52d3e792fcf_field_0, &__struct_info__2ea4c52d3e792fcf_field_1, &__struct_info__2ea4c52d3e792fcf_field_2, &__struct_info__2ea4c52d3e792fcf_field_3, &__struct_info__2ea4c52d3e792fcf_field_4, &__struct_info__2ea4c52d3e792fcf_field_5 };
StructInfo __struct_info__2ea4c52d3e792fcf = {"Serializer", "archive", 13, __struct_info__2ea4c52d3e792fcf_fields, 6, TypeSize<archive::Serializer>::size, UINT64_C(0x0), nullptr, UINT64_C(0x2ea4c52d3e792fcf), 0 };
TypeInfo __type_info__264a3f04ea74314f = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, TypeSize<TArray<uint8_t>>::size, UINT64_C(0x264a3f04ea74314f) };
TypeInfo __type_info__4ac1d999a882997b = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, TypeSize<TArray<uint8_t>>::size, UINT64_C(0x4ac1d999a882997b) };
TypeInfo __type_info__3a95295ead7e9066 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, nullptr, nullptr, 0, 0, nullptr, 8364, TypeSize<void *>::size, UINT64_C(0x3a95295ead7e9066) };
TypeInfo __type_info__af8afe4c86446b52 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, TypeSize<int32_t>::size, UINT64_C(0xaf8afe4c86446b52) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, TypeSize<char *>::size, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__e6261f7908d89619 = { Type::tStructure, &__struct_info__14f90bc296d98088, nullptr, nullptr, &__type_info__d400c4922cb4cfd0, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<archive::MemSerializer>::size, UINT64_C(0xe6261f7908d89619) };
TypeInfo __type_info__d400c4922cb4cfd0 = { Type::tStructure, &__struct_info__2ea4c52d3e792fcf, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24590, TypeSize<archive::Serializer>::size, UINT64_C(0xd400c4922cb4cfd0) };
TypeInfo __type_info__af63df4c8601f1a5 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<bool>::size, UINT64_C(0xaf63df4c8601f1a5) };
TypeInfo __type_info__af5be84c85f468f0 = { Type::tUInt8, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<uint8_t>::size, UINT64_C(0xaf5be84c85f468f0) };
TypeInfo __type_info__af63eb4c86020609 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<void>::size, UINT64_C(0xaf63eb4c86020609) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[1] = { &__type_info__4ac1d999a882997b };

inline TArray<uint8_t> _FuncbuiltinTickclone_to_moveTick2007252383599261567_359bffb73f84303b ( Context * __context__, TArray<uint8_t> const  & __clone_src_rename_at_1089_0 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_235acebda28335df ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_68_2, int32_t __newSize_rename_at_68_3 );
inline TArray<uint8_t> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_b13cf2a57c7f809e ( Context * __context__, TArray<uint8_t> & __a_rename_at_50_4 );
inline void _FuncbuiltinTickcloneTick3038771811667655495_5d7fb39f4a94b6 ( Context * __context__, TArray<uint8_t> & __a_rename_at_1113_5, TArray<uint8_t> const  & __b_rename_at_1113_6 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_cf8693e01f654e23 ( Context * __context__, TArray<uint8_t> & __a_rename_at_1234_8 );
inline void finalize_ec9218c84ddc7c41 ( Context * __context__, archive::Serializer & ____this_rename_at_16_9 );
inline void finalize_8e7536909ae820d4 ( Context * __context__, archive::MemSerializer & ____this_rename_at_24_10 );
inline void _FuncarchiveTickserialize_rawTick346513482259279339_f432dbce0a8512d6 ( Context * __context__, archive::Archive & __arch_rename_at_90_11, float3x3 & __value_rename_at_90_12 );
inline void _FuncarchiveTickserialize_rawTick346513482259279339_2e4698a13889843d ( Context * __context__, archive::Archive & __arch_rename_at_90_13, float3x4 & __value_rename_at_90_14 );
inline void _FuncarchiveTickserialize_rawTick346513482259279339_dbf93da0a2b7b5c6 ( Context * __context__, archive::Archive & __arch_rename_at_90_15, float4x4 & __value_rename_at_90_16 );
inline void _FuncarchiveTickread_rawTick6802855309095275289_5ada6eaea03c2d43 ( Context * __context__, archive::Archive & __arch_rename_at_99_17, int32_t & __value_rename_at_99_18 );
inline void _FuncbuiltinTickresize_no_initTick14746062268774376747_9a94138666aaa1f0 ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_114_19, int32_t __newSize_rename_at_114_20 );
inline void _FuncarchiveTickwrite_rawTick9760319972816126958_ccf41d41b9737aa5 ( Context * __context__, archive::Archive & __arch_rename_at_105_21, int32_t & __value_rename_at_105_22 );
inline void _FuncSerializer_0x27___finalize_b57170f7841c5fbe ( Context * __context__, archive::Serializer & __self_rename_at_16_23 );
inline archive::MemSerializer MemSerializer_eedc6c298e8644a7 ( Context * __context__ );
inline void _FuncMemSerializerTickMemSerializer_651498bb4e1220f8 ( Context * __context__, archive::MemSerializer & __self_rename_at_26_25 );
inline archive::MemSerializer MemSerializer_ea328262e30b4519 ( Context * __context__, TArray<uint8_t> const  & __from_rename_at_30_26 );
inline void _FuncMemSerializerTickMemSerializer_1018f16282d2a27f ( Context * __context__, archive::MemSerializer & __self_rename_at_30_28, TArray<uint8_t> const  & __from_rename_at_30_29 );
inline TArray<uint8_t> _FuncMemSerializerTickextractData_679a82cc6ec7ca5b ( Context * __context__, archive::MemSerializer & __self_rename_at_33_30 );
inline TArray<uint8_t> _FuncMemSerializerTickgetCopyOfData_4599022380655ed1 ( Context * __context__, archive::MemSerializer & __self_rename_at_37_31 );
inline char * _FuncMemSerializerTickgetLastError_36b724240a841462 ( Context * __context__, archive::MemSerializer & __self_rename_at_42_33 );
inline bool _FuncMemSerializerTickOK_d9649f9442911044 ( Context * __context__, archive::MemSerializer & __self_rename_at_46_34 );
inline bool _FuncMemSerializerTickwrite_e43800f3e4a97c40 ( Context * __context__, archive::MemSerializer & __self_rename_at_50_35, void * const  __bytes_rename_at_50_36, int32_t __size_rename_at_50_37 );
inline bool _FuncMemSerializerTickread_3955af58030abb80 ( Context * __context__, archive::MemSerializer & __self_rename_at_59_39, void * const  __bytes_rename_at_59_40, int32_t __size_rename_at_59_41 );
inline void _FuncMemSerializerTickerror_9049005b0c023f73 ( Context * __context__, archive::MemSerializer & __self_rename_at_74_44, char * const  __code_rename_at_74_45 );
inline void _FuncMemSerializer_0x27___finalize_ede77b6b9785e805 ( Context * __context__, archive::MemSerializer & __self_rename_at_24_46 );
inline void serialize_507166393832af66 ( Context * __context__, archive::Archive & __arch_rename_at_133_47, float3x3 & __value_rename_at_133_48 );
inline void serialize_9a46ac2edb06a08c ( Context * __context__, archive::Archive & __arch_rename_at_137_49, float3x4 & __value_rename_at_137_50 );
inline void serialize_e6dad884473e595d ( Context * __context__, archive::Archive & __arch_rename_at_141_51, float4x4 & __value_rename_at_141_52 );
inline void serialize_d51d77f15d6914a2 ( Context * __context__, archive::Archive & __arch_rename_at_230_53, char * & __value_rename_at_230_54 );
inline archive::Serializer Serializer_f09410f601464a77 ( Context * __context__ );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline TArray<uint8_t> _FuncbuiltinTickclone_to_moveTick2007252383599261567_359bffb73f84303b ( Context * __context__, TArray<uint8_t> const  &  __clone_src_rename_at_1089_0 )
{
    TArray<uint8_t> __clone_dest_rename_at_1091_1;das_zero(__clone_dest_rename_at_1091_1);
    _FuncbuiltinTickcloneTick3038771811667655495_5d7fb39f4a94b6(__context__,das_arg<TArray<uint8_t>>::pass(__clone_dest_rename_at_1091_1),__clone_src_rename_at_1089_0);
    return /* <- */ das_auto_cast_move<TArray<uint8_t>>::cast(__clone_dest_rename_at_1091_1);
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_235acebda28335df ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_68_2, int32_t __newSize_rename_at_68_3 )
{
    builtin_array_resize(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_68_2),__newSize_rename_at_68_3,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline TArray<uint8_t> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_b13cf2a57c7f809e ( Context * __context__, TArray<uint8_t> &  __a_rename_at_50_4 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_0,cast<TArray<uint8_t> &>::from(__a_rename_at_50_4)));
    return das_auto_cast_ref<TArray<uint8_t> &>::cast(__a_rename_at_50_4);
}

inline void _FuncbuiltinTickcloneTick3038771811667655495_5d7fb39f4a94b6 ( Context * __context__, TArray<uint8_t> &  __a_rename_at_1113_5, TArray<uint8_t> const  &  __b_rename_at_1113_6 )
{
    int32_t __ln_rename_at_1114_7 = ((int32_t)builtin_array_size(__b_rename_at_1113_6));
    _FuncbuiltinTickresizeTick4811697762258667383_235acebda28335df(__context__,das_arg<TArray<uint8_t>>::pass(__a_rename_at_1113_5),__ln_rename_at_1114_7);
    if ( __ln_rename_at_1114_7 == 0 )
    {
        return ;
    } else {
        das_memcpy(das_auto_cast<void * const >::cast(das_ref(__context__,__a_rename_at_1113_5(0,__context__))),das_auto_cast<void * const >::cast(das_ref(__context__,__b_rename_at_1113_6(0,__context__))),__ln_rename_at_1114_7 * 1);
    };
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_cf8693e01f654e23 ( Context * __context__, TArray<uint8_t> &  __a_rename_at_1234_8 )
{
    builtin_array_free(das_arg<TArray<uint8_t>>::pass(__a_rename_at_1234_8),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void finalize_ec9218c84ddc7c41 ( Context * __context__, archive::Serializer &  ____this_rename_at_16_9 )
{
    memset((void*)&(____this_rename_at_16_9), 0, TypeSize<archive::Serializer>::size);
}

inline void finalize_8e7536909ae820d4 ( Context * __context__, archive::MemSerializer &  ____this_rename_at_24_10 )
{
    _FuncbuiltinTickfinalizeTick13836114024949725080_cf8693e01f654e23(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_24_10.data));
    memset((void*)&(____this_rename_at_24_10), 0, TypeSize<archive::MemSerializer>::size);
}

inline void _FuncarchiveTickserialize_rawTick346513482259279339_f432dbce0a8512d6 ( Context * __context__, archive::Archive &  __arch_rename_at_90_11, float3x3 &  __value_rename_at_90_12 )
{
    if ( __arch_rename_at_90_11.reading )
    {
        das_invoke_method<bool,offsetof(archive::Serializer,read)>::invoke<archive::Serializer &,float3x3 *,int32_t>(__context__,nullptr,das_arg<archive::Serializer>::pass(das_cast<archive::Serializer>::cast(das_deref(__context__,__arch_rename_at_90_11.stream))),das_ref(__context__,__value_rename_at_90_12),36);
    } else {
        das_invoke_method<bool,offsetof(archive::Serializer,write)>::invoke<archive::Serializer &,float3x3 *,int32_t>(__context__,nullptr,das_arg<archive::Serializer>::pass(das_cast<archive::Serializer>::cast(das_deref(__context__,__arch_rename_at_90_11.stream))),das_ref(__context__,__value_rename_at_90_12),36);
    };
}

inline void _FuncarchiveTickserialize_rawTick346513482259279339_2e4698a13889843d ( Context * __context__, archive::Archive &  __arch_rename_at_90_13, float3x4 &  __value_rename_at_90_14 )
{
    if ( __arch_rename_at_90_13.reading )
    {
        das_invoke_method<bool,offsetof(archive::Serializer,read)>::invoke<archive::Serializer &,float3x4 *,int32_t>(__context__,nullptr,das_arg<archive::Serializer>::pass(das_cast<archive::Serializer>::cast(das_deref(__context__,__arch_rename_at_90_13.stream))),das_ref(__context__,__value_rename_at_90_14),48);
    } else {
        das_invoke_method<bool,offsetof(archive::Serializer,write)>::invoke<archive::Serializer &,float3x4 *,int32_t>(__context__,nullptr,das_arg<archive::Serializer>::pass(das_cast<archive::Serializer>::cast(das_deref(__context__,__arch_rename_at_90_13.stream))),das_ref(__context__,__value_rename_at_90_14),48);
    };
}

inline void _FuncarchiveTickserialize_rawTick346513482259279339_dbf93da0a2b7b5c6 ( Context * __context__, archive::Archive &  __arch_rename_at_90_15, float4x4 &  __value_rename_at_90_16 )
{
    if ( __arch_rename_at_90_15.reading )
    {
        das_invoke_method<bool,offsetof(archive::Serializer,read)>::invoke<archive::Serializer &,float4x4 *,int32_t>(__context__,nullptr,das_arg<archive::Serializer>::pass(das_cast<archive::Serializer>::cast(das_deref(__context__,__arch_rename_at_90_15.stream))),das_ref(__context__,__value_rename_at_90_16),64);
    } else {
        das_invoke_method<bool,offsetof(archive::Serializer,write)>::invoke<archive::Serializer &,float4x4 *,int32_t>(__context__,nullptr,das_arg<archive::Serializer>::pass(das_cast<archive::Serializer>::cast(das_deref(__context__,__arch_rename_at_90_15.stream))),das_ref(__context__,__value_rename_at_90_16),64);
    };
}

inline void _FuncarchiveTickread_rawTick6802855309095275289_5ada6eaea03c2d43 ( Context * __context__, archive::Archive &  __arch_rename_at_99_17, int32_t & __value_rename_at_99_18 )
{
    DAS_ASSERT((__arch_rename_at_99_17.reading));
    das_invoke_method<bool,offsetof(archive::Serializer,read)>::invoke<archive::Serializer &,int32_t *,int32_t>(__context__,nullptr,das_arg<archive::Serializer>::pass(das_cast<archive::Serializer>::cast(das_deref(__context__,__arch_rename_at_99_17.stream))),das_ref(__context__,__value_rename_at_99_18),4);
}

inline void _FuncbuiltinTickresize_no_initTick14746062268774376747_9a94138666aaa1f0 ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_114_19, int32_t __newSize_rename_at_114_20 )
{
    builtin_array_resize_no_init(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_114_19),__newSize_rename_at_114_20,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncarchiveTickwrite_rawTick9760319972816126958_ccf41d41b9737aa5 ( Context * __context__, archive::Archive &  __arch_rename_at_105_21, int32_t & __value_rename_at_105_22 )
{
    DAS_ASSERT((!__arch_rename_at_105_21.reading));
    das_invoke_method<bool,offsetof(archive::Serializer,write)>::invoke<archive::Serializer &,int32_t *,int32_t>(__context__,nullptr,das_arg<archive::Serializer>::pass(das_cast<archive::Serializer>::cast(das_deref(__context__,__arch_rename_at_105_21.stream))),das_ref(__context__,__value_rename_at_105_22),4);
}

inline void _FuncSerializer_0x27___finalize_b57170f7841c5fbe ( Context * __context__, archive::Serializer &  __self_rename_at_16_23 )
{
    finalize_ec9218c84ddc7c41(__context__,das_arg<archive::Serializer>::pass(__self_rename_at_16_23));
}

inline archive::MemSerializer MemSerializer_eedc6c298e8644a7 ( Context * __context__ )
{
    archive::MemSerializer __self_rename_at_26_24; das_zero(__self_rename_at_26_24); das_move(__self_rename_at_26_24, (([&]() -> archive::MemSerializer {
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
    return /* <- */ das_auto_cast_move<archive::MemSerializer>::cast(__self_rename_at_26_24);
}

inline void _FuncMemSerializerTickMemSerializer_651498bb4e1220f8 ( Context * __context__, archive::MemSerializer &  __self_rename_at_26_25 )
{
}

inline archive::MemSerializer MemSerializer_ea328262e30b4519 ( Context * __context__, TArray<uint8_t> const  &  __from_rename_at_30_26 )
{
    archive::MemSerializer __self_rename_at_30_27; das_zero(__self_rename_at_30_27); das_move(__self_rename_at_30_27, (([&]() -> archive::MemSerializer {
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
    _FuncMemSerializerTickMemSerializer_1018f16282d2a27f(__context__,das_arg<archive::MemSerializer>::pass(__self_rename_at_30_27),__from_rename_at_30_26);
    return /* <- */ das_auto_cast_move<archive::MemSerializer>::cast(__self_rename_at_30_27);
}

inline void _FuncMemSerializerTickMemSerializer_1018f16282d2a27f ( Context * __context__, archive::MemSerializer &  __self_rename_at_30_28, TArray<uint8_t> const  &  __from_rename_at_30_29 )
{
    _FuncbuiltinTickcloneTick3038771811667655495_5d7fb39f4a94b6(__context__,das_arg<TArray<uint8_t>>::pass(__self_rename_at_30_28.data),__from_rename_at_30_29);
}

inline TArray<uint8_t> _FuncMemSerializerTickextractData_679a82cc6ec7ca5b ( Context * __context__, archive::MemSerializer &  __self_rename_at_33_30 )
{
    return /* <- */ das_auto_cast_move<TArray<uint8_t>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_b13cf2a57c7f809e(__context__,das_arg<TArray<uint8_t>>::pass(__self_rename_at_33_30.data)));
}

inline TArray<uint8_t> _FuncMemSerializerTickgetCopyOfData_4599022380655ed1 ( Context * __context__, archive::MemSerializer &  __self_rename_at_37_31 )
{
    TArray<uint8_t> __cdata_rename_at_39_32; das_zero(__cdata_rename_at_39_32); das_move(__cdata_rename_at_39_32, _FuncbuiltinTickclone_to_moveTick2007252383599261567_359bffb73f84303b(__context__,das_arg<TArray<uint8_t>>::pass(__self_rename_at_37_31.data)));
    return /* <- */ das_auto_cast_move<TArray<uint8_t>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_b13cf2a57c7f809e(__context__,das_arg<TArray<uint8_t>>::pass(__cdata_rename_at_39_32)));
}

inline char * _FuncMemSerializerTickgetLastError_36b724240a841462 ( Context * __context__, archive::MemSerializer &  __self_rename_at_42_33 )
{
    return das_auto_cast<char *>::cast(__self_rename_at_42_33.lastError);
}

inline bool _FuncMemSerializerTickOK_d9649f9442911044 ( Context * __context__, archive::MemSerializer &  __self_rename_at_46_34 )
{
    return das_auto_cast<bool>::cast(SimPolicy<char *>::Equ(cast<char *>::from(__self_rename_at_46_34.lastError),cast<char *>::from(nullptr),*__context__,nullptr));
}

inline bool _FuncMemSerializerTickwrite_e43800f3e4a97c40 ( Context * __context__, archive::MemSerializer &  __self_rename_at_50_35, void * const  __bytes_rename_at_50_36, int32_t __size_rename_at_50_37 )
{
    int32_t __pos_rename_at_52_38 = ((int32_t)builtin_array_size(das_arg<TArray<uint8_t>>::pass(__self_rename_at_50_35.data)));
    _FuncbuiltinTickresize_no_initTick14746062268774376747_9a94138666aaa1f0(__context__,das_arg<TArray<uint8_t>>::pass(__self_rename_at_50_35.data),__pos_rename_at_52_38 + __size_rename_at_50_37);
    das_memcpy(das_auto_cast<void * const >::cast(das_ref(__context__,__self_rename_at_50_35.data(__pos_rename_at_52_38,__context__))),__bytes_rename_at_50_36,__size_rename_at_50_37);
    return das_auto_cast<bool>::cast(true);
}

inline bool _FuncMemSerializerTickread_3955af58030abb80 ( Context * __context__, archive::MemSerializer &  __self_rename_at_59_39, void * const  __bytes_rename_at_59_40, int32_t __size_rename_at_59_41 )
{
    int32_t __newOffset_rename_at_61_42 = ((int32_t)(__self_rename_at_59_39.readOffset + __size_rename_at_59_41));
    int32_t __maxOffset_rename_at_62_43 = ((int32_t)builtin_array_size(das_arg<TArray<uint8_t>>::pass(__self_rename_at_59_39.data)));
    if ( __newOffset_rename_at_61_42 > __maxOffset_rename_at_62_43 )
    {
        builtin_error(((char *) "reading past the end of stream"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        das_copy(__self_rename_at_59_39.readOffset,__maxOffset_rename_at_62_43);
        return das_auto_cast<bool>::cast(false);
    } else {
        das_memcpy(__bytes_rename_at_59_40,das_auto_cast<void * const >::cast(das_ref(__context__,__self_rename_at_59_39.data(__self_rename_at_59_39.readOffset,__context__))),__size_rename_at_59_41);
        das_copy(__self_rename_at_59_39.readOffset,__newOffset_rename_at_61_42);
        return das_auto_cast<bool>::cast(true);
    };
}

inline void _FuncMemSerializerTickerror_9049005b0c023f73 ( Context * __context__, archive::MemSerializer &  __self_rename_at_74_44, char * const  __code_rename_at_74_45 )
{
    das_copy(__self_rename_at_74_44.lastError,__code_rename_at_74_45);
}

inline void _FuncMemSerializer_0x27___finalize_ede77b6b9785e805 ( Context * __context__, archive::MemSerializer &  __self_rename_at_24_46 )
{
    finalize_8e7536909ae820d4(__context__,das_arg<archive::MemSerializer>::pass(__self_rename_at_24_46));
}

inline void serialize_507166393832af66 ( Context * __context__, archive::Archive &  __arch_rename_at_133_47, float3x3 &  __value_rename_at_133_48 )
{
    _FuncarchiveTickserialize_rawTick346513482259279339_f432dbce0a8512d6(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_133_47),das_arg<float3x3>::pass(__value_rename_at_133_48));
}

inline void serialize_9a46ac2edb06a08c ( Context * __context__, archive::Archive &  __arch_rename_at_137_49, float3x4 &  __value_rename_at_137_50 )
{
    _FuncarchiveTickserialize_rawTick346513482259279339_2e4698a13889843d(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_137_49),das_arg<float3x4>::pass(__value_rename_at_137_50));
}

inline void serialize_e6dad884473e595d ( Context * __context__, archive::Archive &  __arch_rename_at_141_51, float4x4 &  __value_rename_at_141_52 )
{
    _FuncarchiveTickserialize_rawTick346513482259279339_dbf93da0a2b7b5c6(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_141_51),das_arg<float4x4>::pass(__value_rename_at_141_52));
}

inline void serialize_d51d77f15d6914a2 ( Context * __context__, archive::Archive &  __arch_rename_at_230_53, char * & __value_rename_at_230_54 )
{
    if ( __arch_rename_at_230_53.reading )
    {
        int32_t __len_rename_at_233_55 = 0;
        _FuncarchiveTickread_rawTick6802855309095275289_5ada6eaea03c2d43(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_230_53),__len_rename_at_233_55);
        if ( __len_rename_at_233_55 > 0 )
        {
            TArray<uint8_t> __tbuf_rename_at_236_56;das_zero(__tbuf_rename_at_236_56);
            _FuncbuiltinTickresize_no_initTick14746062268774376747_9a94138666aaa1f0(__context__,das_arg<TArray<uint8_t>>::pass(__tbuf_rename_at_236_56),__len_rename_at_233_55 + 1);
            das_invoke_method<bool,offsetof(archive::Serializer,read)>::invoke<archive::Serializer &,uint8_t *,int32_t>(__context__,nullptr,das_arg<archive::Serializer>::pass(das_cast<archive::Serializer>::cast(das_deref(__context__,__arch_rename_at_230_53.stream))),das_ref(__context__,__tbuf_rename_at_236_56(0,__context__)),__len_rename_at_233_55);
            das_copy(__tbuf_rename_at_236_56(__len_rename_at_233_55,__context__),0x0);
            das_copy(__value_rename_at_230_54,((char * const )(builtin_string_clone(das_cast<char *>::cast(das_ref(__context__,__tbuf_rename_at_236_56(0,__context__))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
            _FuncbuiltinTickfinalizeTick13836114024949725080_cf8693e01f654e23(__context__,das_arg<TArray<uint8_t>>::pass(__tbuf_rename_at_236_56));
        } else {
            das_copy(__value_rename_at_230_54,nullptr);
        };
    } else {
        int32_t __len_rename_at_246_57 = builtin_string_length(__value_rename_at_230_54,__context__);
        _FuncarchiveTickwrite_rawTick9760319972816126958_ccf41d41b9737aa5(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_230_53),__len_rename_at_246_57);
        if ( __len_rename_at_246_57 > 0 )
        {
            das_invoke_method<bool,offsetof(archive::Serializer,write)>::invoke<archive::Serializer &,void *,int32_t>(__context__,nullptr,das_arg<archive::Serializer>::pass(das_cast<archive::Serializer>::cast(das_deref(__context__,__arch_rename_at_230_53.stream))),das_cast<void *>::cast(__value_rename_at_230_54),__len_rename_at_246_57);
        };
    };
}

inline archive::Serializer Serializer_f09410f601464a77 ( Context * __context__ )
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
    aotLib[0xea1162d6c7002d8f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickclone_to_moveTick2007252383599261567_359bffb73f84303b>>();
    };
    aotLib[0xc00ba5922b28bdc6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_235acebda28335df>>();
    };
    aotLib[0x454d17c1def471b2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_b13cf2a57c7f809e>>();
    };
    aotLib[0x5a9d3e5e3b66c8c1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickcloneTick3038771811667655495_5d7fb39f4a94b6>>();
    };
    aotLib[0x2402206a76e3cd1f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_cf8693e01f654e23>>();
    };
    aotLib[0xde7bd1e4ebeb214c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_ec9218c84ddc7c41>>();
    };
    aotLib[0xf234e5dbcf97dda6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_8e7536909ae820d4>>();
    };
    aotLib[0xf5f2c572cb6af2f7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserialize_rawTick346513482259279339_f432dbce0a8512d6>>();
    };
    aotLib[0x46145fb6b3c467bd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserialize_rawTick346513482259279339_2e4698a13889843d>>();
    };
    aotLib[0xe214758b950cb87d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserialize_rawTick346513482259279339_dbf93da0a2b7b5c6>>();
    };
    aotLib[0xc90a696a49fde044] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickread_rawTick6802855309095275289_5ada6eaea03c2d43>>();
    };
    aotLib[0x544319553a4ab3e0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresize_no_initTick14746062268774376747_9a94138666aaa1f0>>();
    };
    aotLib[0x60ccfeff88772ffe] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickwrite_rawTick9760319972816126958_ccf41d41b9737aa5>>();
    };
    aotLib[0x13dfddf185797f46] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncSerializer_0x27___finalize_b57170f7841c5fbe>>();
    };
    aotLib[0xd0718df01c47f852] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&MemSerializer_eedc6c298e8644a7>>();
    };
    aotLib[0x4dc27055756c2d0c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncMemSerializerTickMemSerializer_651498bb4e1220f8>>();
    };
    aotLib[0x72be9b498a91920e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&MemSerializer_ea328262e30b4519>>();
    };
    aotLib[0x973e778092d30111] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncMemSerializerTickMemSerializer_1018f16282d2a27f>>();
    };
    aotLib[0x35af82e448cb16e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncMemSerializerTickextractData_679a82cc6ec7ca5b>>();
    };
    aotLib[0x2a5e151311020411] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncMemSerializerTickgetCopyOfData_4599022380655ed1>>();
    };
    aotLib[0xe5472d04f10f2554] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncMemSerializerTickgetLastError_36b724240a841462>>();
    };
    aotLib[0x9c5a5e8c5ca39741] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncMemSerializerTickOK_d9649f9442911044>>();
    };
    aotLib[0x48fcff447cb32ec4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncMemSerializerTickwrite_e43800f3e4a97c40>>();
    };
    aotLib[0x678ea98dcad3bee2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncMemSerializerTickread_3955af58030abb80>>();
    };
    aotLib[0x92b5ca7a2199c008] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncMemSerializerTickerror_9049005b0c023f73>>();
    };
    aotLib[0xe5d6abf1bdadd7c1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncMemSerializer_0x27___finalize_ede77b6b9785e805>>();
    };
    aotLib[0xbc80d56faa19cfe6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&serialize_507166393832af66>>();
    };
    aotLib[0x276297c953120ef0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&serialize_9a46ac2edb06a08c>>();
    };
    aotLib[0x98ff7e87a2bcf29f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&serialize_e6dad884473e595d>>();
    };
    aotLib[0x70b993b0af855414] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&serialize_d51d77f15d6914a2>>();
    };
    aotLib[0x8d1183c28f501aa8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&Serializer_f09410f601464a77>>();
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_9563944200698030257
AotListBase impl_aot_archive(_anon_9563944200698030257::registerAotFunctions);
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
