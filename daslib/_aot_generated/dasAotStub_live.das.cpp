#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
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
 // require math
#include "daScript/simulate/aot_builtin_math.h"
 // require debugapi
#include "daScript/simulate/aot_builtin_debugger.h"
 // require fio
#include "daScript/misc/performance_time.h"
#include "daScript/simulate/aot_builtin_fio.h"
 // require strings_boost
 // require ast_boost
 // require templates
 // require contracts
 // require array_boost
 // require algorithm
 // require functional
 // require templates_boost
 // require macro_boost
 // require defer
 // require apply
 // require archive
 // require decs
 // require static_let
 // require live

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
namespace _anon_513224431237640881 {

namespace live { struct _lambda_live_205_1; };
namespace live { struct _lambda_live_225_2; };
namespace live { struct _lambda_live_237_3; };
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
namespace fio { struct df_header; };
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
namespace apply { struct ApplyMacro; };
namespace archive { struct Serializer; };
namespace archive { struct MemSerializer; };
namespace archive { struct Archive; };
namespace decs { struct CTypeInfo; };
namespace decs { struct Component; };
namespace decs { struct EntityId; };
namespace decs { struct Archetype; };
namespace decs { struct ComponentValue; };
namespace decs { struct DeferAction; };
namespace decs { struct EcsRequestPos; };
namespace decs { struct EcsRequest; };
namespace decs { struct DecsState; };
namespace decs { struct DecsPass; };
namespace decs { struct _lambda_decs_781_1; };
namespace decs { struct _lambda_decs_789_2; };
namespace decs { struct _lambda_decs_799_3; };
namespace decs { struct _lambda_decs_885_6; };
namespace decs { struct _lambda_decs_885_10; };
namespace decs { struct _lambda_decs_885_14; };
namespace decs { struct _lambda_decs_885_18; };
namespace decs { struct _lambda_decs_885_22; };
namespace decs { struct _lambda_decs_885_26; };
namespace decs { struct _lambda_decs_885_30; };
namespace decs { struct _lambda_decs_885_34; };
namespace decs { struct _lambda_decs_885_38; };
namespace decs { struct _lambda_decs_885_42; };
namespace decs { struct _lambda_decs_885_46; };
namespace decs { struct _lambda_decs_885_50; };
namespace decs { struct _lambda_decs_885_54; };
namespace decs { struct _lambda_decs_885_58; };
namespace decs { struct _lambda_decs_885_62; };
namespace decs { struct _lambda_decs_885_66; };
namespace decs { struct _lambda_decs_885_70; };
namespace decs { struct _lambda_decs_885_74; };
namespace decs { struct _lambda_decs_885_78; };
namespace decs { struct _lambda_decs_885_82; };
namespace decs { struct _lambda_decs_885_86; };
namespace decs { struct _lambda_decs_885_90; };
namespace decs { struct _lambda_decs_885_94; };
namespace decs { struct _lambda_decs_885_98; };
namespace decs { struct _lambda_decs_885_102; };
namespace decs { struct _lambda_decs_885_106; };
namespace decs { struct _lambda_decs_885_110; };
namespace decs { struct _lambda_decs_885_114; };
namespace decs { struct _lambda_decs_885_118; };
namespace static_let { struct StaticLetMacro; };
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
// unused structure DapiArray
// unused structure DapiTable
// unused structure DapiBlock
// unused structure DapiFunc
// unused structure DapiLambda
// unused structure DapiSequence
// unused structure DapiDataWalker
// unused structure DapiStackWalker
// unused structure df_header
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
namespace decs {

struct CTypeInfo {
    DAS_COMMENT(bound_enum) das::Type basicType;
    char * mangledName;
    char * fullName;
    uint64_t hash;
    uint32_t size;
    Func DAS_COMMENT((void,TArray<uint8_t>)) eraser;
    Func DAS_COMMENT((void,TArray<uint8_t>,TArray<uint8_t> const )) clonner;
    Func DAS_COMMENT((void,archive::Archive,TArray<uint8_t>,char * const )) serializer;
    Func DAS_COMMENT((char *,void * const )) dumper;
    Func DAS_COMMENT((TypeInfo const  *)) mkTypeInfo;
    Func DAS_COMMENT((Lambda DAS_COMMENT((void)),TArray<uint8_t>)) gc;
};
}
namespace decs {

struct Component {
    char * name;
    uint64_t hash;
    int32_t stride;
    TArray<uint8_t> data;
    decs::CTypeInfo info;
    Lambda DAS_COMMENT((void)) gc_dummy;
};
}
namespace decs {

struct EntityId {
    uint32_t id;
    int32_t generation;
};
}
namespace decs {

struct Archetype {
    uint64_t hash;
    TArray<decs::Component> components;
    int32_t size;
    int32_t eidIndex;
};
}
// unused structure ComponentValue
// unused structure DeferAction
namespace decs {

struct EcsRequestPos {
    char * file;
    uint32_t line;
};
}
namespace decs {

struct EcsRequest {
    uint64_t hash;
    TArray<char *> req;
    TArray<char *> reqn;
    TArray<int32_t> archetypes;
    decs::EcsRequestPos at;
};
}
namespace decs {

struct DecsState {
    TTable<uint64_t,int32_t> archetypeLookup;
    TArray<decs::Archetype> allArchetypes;
    TArray<decs::EntityId> entityFreeList;
    TArray<AutoTuple<int32_t,uint64_t,int32_t>> entityLookup;
    TTable<char *,decs::CTypeInfo> componentTypeCheck;
    TArray<decs::EcsRequest> ecsQueries;
    TTable<uint64_t,int32_t> queryLookup;
};
}
// unused structure DecsPass
// unused structure _lambda_decs_781_1
// unused structure _lambda_decs_789_2
// unused structure _lambda_decs_799_3
// unused structure _lambda_decs_885_6
// unused structure _lambda_decs_885_10
// unused structure _lambda_decs_885_14
// unused structure _lambda_decs_885_18
// unused structure _lambda_decs_885_22
// unused structure _lambda_decs_885_26
// unused structure _lambda_decs_885_30
// unused structure _lambda_decs_885_34
// unused structure _lambda_decs_885_38
// unused structure _lambda_decs_885_42
// unused structure _lambda_decs_885_46
// unused structure _lambda_decs_885_50
// unused structure _lambda_decs_885_54
// unused structure _lambda_decs_885_58
// unused structure _lambda_decs_885_62
// unused structure _lambda_decs_885_66
// unused structure _lambda_decs_885_70
// unused structure _lambda_decs_885_74
// unused structure _lambda_decs_885_78
// unused structure _lambda_decs_885_82
// unused structure _lambda_decs_885_86
// unused structure _lambda_decs_885_90
// unused structure _lambda_decs_885_94
// unused structure _lambda_decs_885_98
// unused structure _lambda_decs_885_102
// unused structure _lambda_decs_885_106
// unused structure _lambda_decs_885_110
// unused structure _lambda_decs_885_114
// unused structure _lambda_decs_885_118
// unused structure StaticLetMacro
namespace live {

struct _lambda_live_205_1 {
    Func DAS_COMMENT((void,live::_lambda_live_205_1)) __lambda;
    Func DAS_COMMENT((void,live::_lambda_live_205_1 *)) __finalize;
};
}
namespace live {

struct _lambda_live_225_2 {
    Func DAS_COMMENT((void,live::_lambda_live_225_2,TArray<uint8_t> const )) __lambda;
    Func DAS_COMMENT((void,live::_lambda_live_225_2 *)) __finalize;
};
}
namespace live {

struct _lambda_live_237_3 {
    Func DAS_COMMENT((void,live::_lambda_live_237_3,decs::DecsState * const )) __lambda;
    Func DAS_COMMENT((void,live::_lambda_live_237_3 *)) __finalize;
    smart_ptr_raw<Context> lContext;
};
}
extern StructInfo __struct_info__e48f7ed30ac22e75;
extern StructInfo __struct_info__2ea4c52d3e792fcf;
extern StructInfo __struct_info__f9485bd96ad1fe2b;
extern StructInfo __struct_info__adf7f0b255ff2702;
extern StructInfo __struct_info__62dc6fcf66cdc7b2;
extern StructInfo __struct_info__99004e5160f37e36;
extern StructInfo __struct_info__3ad3d98059b6e265;
extern StructInfo __struct_info__5cade38857dd55b4;
extern StructInfo __struct_info__1d9403e5c2c237d8;
extern StructInfo __struct_info__e120f67bdeeb9788;
extern StructInfo __struct_info__e126367bdeeebcf6;
extern StructInfo __struct_info__da18367bd8bb281f;
extern TypeInfo __type_info__86304441d7b56b0c;
extern TypeInfo __type_info__b5c5f09960a9d827;
extern TypeInfo __type_info__f4b9fc3e25a5c7c1;
extern TypeInfo __type_info__b3327a1376bd9760;
extern TypeInfo __type_info__fc81ca08b970563f;
extern TypeInfo __type_info__2e81ca1ee570f19d;
extern TypeInfo __type_info__e346383245d26f5c;
extern TypeInfo __type_info__15192774a936dc4d;
extern TypeInfo __type_info__92aa643dc780b0b0;
extern TypeInfo __type_info__8ed317d13fd59c13;
extern TypeInfo __type_info__264a3f04ea74314f;
extern TypeInfo __type_info__12283b04d98e775a;
extern TypeInfo __type_info__4ac1d999a882997b;
extern TypeInfo __type_info__af63fd4c8602249f;
extern TypeInfo __type_info__3884b34b905bfd2e;
extern TypeInfo __type_info__b9975a5e7e222a13;
extern TypeInfo __type_info__d83b3ba409a17e95;
extern TypeInfo __type_info__9c225ec61b3e6a3c;
extern TypeInfo __type_info__624d371c76b25aa4;
extern TypeInfo __type_info__bc67beb4aa160fd4;
extern TypeInfo __type_info__3a95295ead7e9066;
extern TypeInfo __type_info__af81fe4c86352052;
extern TypeInfo __type_info__af87fe4c863f5252;
extern TypeInfo __type_info__af85fe4c863bec52;
extern TypeInfo __type_info__af8afe4c86446b52;
extern TypeInfo __type_info__af90fe4c864e9d52;
extern TypeInfo __type_info__af96fe4c8658cf52;
extern TypeInfo __type_info__b68d800849332aec;
extern TypeInfo __type_info__767637ee1a337419;
extern TypeInfo __type_info__125855d9cd771ead;
extern TypeInfo __type_info__54c9a4192a31329c;
extern TypeInfo __type_info__26c336131e645000;
extern TypeInfo __type_info__da6749c16df8cbcb;
extern TypeInfo __type_info__f8ab3594a8b7b5e8;
extern TypeInfo __type_info__d400c4922cb4cfd0;
extern TypeInfo __type_info__cf711d1260800988;
extern TypeInfo __type_info__a1bfdcf22cffade1;
extern TypeInfo __type_info__34d6cc421a9bb601;
extern TypeInfo __type_info__a30ba508046366f1;
extern TypeInfo __type_info__cef1c98e992968cc;
extern TypeInfo __type_info__979842bcb7689575;
extern TypeInfo __type_info__8d785db1354cca5f;
extern TypeInfo __type_info__dd290bb17de94649;
extern TypeInfo __type_info__bc41b2a6fc7fa7ce;
extern TypeInfo __type_info__6f8c1343480cc5e4;
extern TypeInfo __type_info__3fb1dab7ab02f59;
extern TypeInfo __type_info__3452466d210e34e2;
extern TypeInfo __type_info__29e8184e4772e4bc;
extern TypeInfo __type_info__a8d2247d5b8ed91f;
extern TypeInfo __type_info__7dcc2c583c294a1f;
extern TypeInfo __type_info__af63df4c8601f1a5;
extern TypeInfo __type_info__af63e44c8601fa24;
extern TypeInfo __type_info__af63ee4c86020b22;
extern TypeInfo __type_info__af5be84c85f468f0;
extern TypeInfo __type_info__af63eb4c86020609;
extern VarInfo __var_info__f97ad1255edf7b73;
extern VarInfo __var_info__3a675c02b861f0d4;
extern VarInfo __var_info__dc5ad52ba61c2cb;
extern VarInfo __var_info__100e564366a028df;
extern VarInfo __var_info__87c4dfde8d1d5ca9;
extern VarInfo __var_info__6ed86c478b9e1349;
extern VarInfo __var_info__d3b4679945da3d7d;
extern VarInfo __var_info__4ba92686dc92032b;
extern VarInfo __var_info__2f90929bfe0e523d;
extern VarInfo __var_info__991b098814c694e3;
extern VarInfo __var_info__7627c88541b6ee9a;
extern VarInfo __var_info__64592ca1dee9371a;
extern VarInfo __var_info__6735754341b3daeb;
extern VarInfo __var_info__4b3b7643298b859f;
extern VarInfo __var_info__afd965db25eda39;
extern VarInfo __var_info__4ccf70bfd75cbb93;
extern VarInfo __var_info__659a06ad7ed41a45;
extern VarInfo __var_info__d833ed9c9b0916ea;
extern VarInfo __var_info__51020a1b4c525442;
extern VarInfo __var_info__b3053fdf47c5638e;
extern VarInfo __var_info__f01a8566edfe99b4;
extern VarInfo __var_info__9945d3aa334a3ba4;
extern VarInfo __var_info__72fa5fa2aaf87433;
extern VarInfo __var_info__e66213a2bae19899;
extern VarInfo __var_info__d50d7466d734e3f0;
extern VarInfo __var_info__d34716bb369e080d;
extern VarInfo __var_info__54b80d9198d4ac8d;
extern VarInfo __var_info__d34017bb369db164;
extern VarInfo __var_info__e43a02bb450971de;
extern VarInfo __var_info__d32f19bb3686981c;
extern VarInfo __var_info__2bbe845219c139b4;
extern VarInfo __var_info__13102b962e7b8e6b;
extern VarInfo __var_info__e1496f9b35a41b0;
extern VarInfo __var_info__386ec463b1bc1dd0;
extern VarInfo __var_info__79cc2a49405f8801;
extern VarInfo __var_info__b967929fca60b3d4;
extern VarInfo __var_info__36e461fc6934e180;
extern VarInfo __var_info__5eac5551762d72bc;
extern VarInfo __var_info__1c1c2b9a5d9b2e09;
extern VarInfo __var_info__b2b7258794e68bde;
extern VarInfo __var_info__fc9f59842addd61f;
extern VarInfo __var_info__725636875e5de0c1;
extern VarInfo __var_info__a65f57055984c95d;
extern VarInfo __var_info__f8091607bf8f4d5d;
extern VarInfo __var_info__f82d1407bfaf965d;
extern VarInfo __var_info__5e7f7d8b3878d997;
extern VarInfo __var_info__abf120f8491329a9;
extern VarInfo __var_info__c2d42c7deb7bdbd6;
extern VarInfo __var_info__73878c966ce41f3f;
extern VarInfo __var_info__eac8189bc1bc80e1;
extern VarInfo __var_info__e2a573727e1bac42;
extern VarInfo __var_info__d63b6b9131892694;
extern VarInfo __var_info__dc2bafdc4641403e;
extern VarInfo __var_info__8b0d17b41f76f7e4;
extern VarInfo __var_info__ea7703fdd48b8362;
extern VarInfo __var_info__ea7d03fdd495b562;
extern VarInfo __var_info__ea8303fdd49fe762;
extern VarInfo __var_info__1c731c504b9417bc;
extern VarInfo __var_info__dc1dbd882425d194;
extern VarInfo __var_info__30b54910b1aefa4;
extern VarInfo __var_info__5021a0a702c559cb;
extern VarInfo __var_info__be7128724a4f3f31;
extern VarInfo __var_info__e466752a547036d2;
extern VarInfo __var_info__30ac1da2c9bdee4d;
extern VarInfo __var_info__4214a1d6c9be75dd;
extern VarInfo __var_info__80697368a766432;
extern VarInfo __var_info__41bc45d6c97363b3;
extern VarInfo __var_info__fbb7d318fa1b3508;
extern VarInfo __var_info__50239cf28bd067fc;
extern VarInfo __var_info__e632c403d42f243a;
extern VarInfo __var_info__bb98dcfd4a2fffd8;
extern VarInfo __var_info__64a3a1bb96d4295e;
extern VarInfo __var_info__755fcff7f192780e;
extern VarInfo __var_info__e743ca9c6691fda4;
extern VarInfo __var_info__be1d38b71e3c6ecd;
extern VarInfo __var_info__252cce99bfee75a9;
extern VarInfo __var_info__436d8545bf0997ac;
extern VarInfo __var_info__865026081ff8017a;
extern VarInfo __var_info__601b19a6ca455003;
extern VarInfo __var_info__7c5077e54e2863f8;
extern VarInfo __var_info__3a3585e2284c6c6b;
extern FuncInfo __func_info__a4e792ea11638c41;
extern FuncInfo __func_info__baddab894dba73e8;
extern FuncInfo __func_info__19af4d1c08ef72b2;
extern FuncInfo __func_info__91507666520860cd;
extern FuncInfo __func_info__2c3aa2fcec87dc8f;
extern FuncInfo __func_info__f45f30cc74c8f4a6;
extern FuncInfo __func_info__1bcbcef990d4075b;
extern FuncInfo __func_info__1dcd71ef3006d28f;
extern FuncInfo __func_info__faa6e7b3f28885e9;
extern FuncInfo __func_info__dbcb75dfde121c0e;
extern FuncInfo __func_info__6b8a23cd42de6854;
extern FuncInfo __func_info__41a35bb8cc7508cc;
extern FuncInfo __func_info__c9f4d529bdde6142;
extern FuncInfo __func_info__97ee470ea66a6afd;
extern FuncInfo __func_info__766dae3d6a2a7d14;
extern FuncInfo __func_info__9e7ffb69e4895cc9;
extern FuncInfo __func_info__d7138e6aaa104191;
extern FuncInfo __func_info__c85cb34a7aabfce2;
extern FuncInfo __func_info__88667fa8dd2c245b;
extern FuncInfo __func_info__e43d9eedb8768372;
extern FuncInfo __func_info__e5601d0b9c7536bc;
extern FuncInfo __func_info__214fcb6dd261a4ee;
extern FuncInfo __func_info__fe02a5da011a6cb4;
extern FuncInfo __func_info__8eb7d9f42d992591;
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
VarInfo __struct_info__e48f7ed30ac22e75_field_0 =  { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xdc5ad52ba61c2cb), "version", offsetof(archive::Archive,version), 0 };
VarInfo __struct_info__e48f7ed30ac22e75_field_1 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0xf97ad1255edf7b73), "reading", offsetof(archive::Archive,reading), 0 };
VarInfo __struct_info__e48f7ed30ac22e75_field_2 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__d400c4922cb4cfd0, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0x3a675c02b861f0d4), "stream", offsetof(archive::Archive,stream), 3 };
VarInfo * __struct_info__e48f7ed30ac22e75_fields[3] =  { &__struct_info__e48f7ed30ac22e75_field_0, &__struct_info__e48f7ed30ac22e75_field_1, &__struct_info__e48f7ed30ac22e75_field_2 };
StructInfo __struct_info__e48f7ed30ac22e75 = {"Archive", "archive", 12, __struct_info__e48f7ed30ac22e75_fields, 3, 16, UINT64_C(0x0), nullptr, UINT64_C(0xe48f7ed30ac22e75), 2 };
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
VarInfo __struct_info__f9485bd96ad1fe2b_field_0 =  { Type::tUInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 8, UINT64_C(0x6735754341b3daeb), "hash", offsetof(decs::Archetype,hash), 0 };
VarInfo __struct_info__f9485bd96ad1fe2b_field_1 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__34d6cc421a9bb601, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x7627c88541b6ee9a), "components", offsetof(decs::Archetype,components), 4 };
VarInfo __struct_info__f9485bd96ad1fe2b_field_2 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x4b3b7643298b859f), "size", offsetof(decs::Archetype,size), 0 };
VarInfo __struct_info__f9485bd96ad1fe2b_field_3 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x64592ca1dee9371a), "eidIndex", offsetof(decs::Archetype,eidIndex), 0 };
VarInfo * __struct_info__f9485bd96ad1fe2b_fields[4] =  { &__struct_info__f9485bd96ad1fe2b_field_0, &__struct_info__f9485bd96ad1fe2b_field_1, &__struct_info__f9485bd96ad1fe2b_field_2, &__struct_info__f9485bd96ad1fe2b_field_3 };
StructInfo __struct_info__f9485bd96ad1fe2b = {"Archetype", "decs", 28, __struct_info__f9485bd96ad1fe2b_fields, 4, 40, UINT64_C(0x0), nullptr, UINT64_C(0xf9485bd96ad1fe2b), 1 };
VarInfo __struct_info__adf7f0b255ff2702_field_0 =  { Type::tEnumeration, nullptr, &__enum_info__c897fe55afe7f727, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xafd965db25eda39), "basicType", offsetof(decs::CTypeInfo,basicType), 0 };
VarInfo __struct_info__adf7f0b255ff2702_field_1 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x9945d3aa334a3ba4), "mangledName", offsetof(decs::CTypeInfo,mangledName), 2 };
VarInfo __struct_info__adf7f0b255ff2702_field_2 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x51020a1b4c525442), "fullName", offsetof(decs::CTypeInfo,fullName), 11 };
VarInfo __struct_info__adf7f0b255ff2702_field_3 =  { Type::tUInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 8, UINT64_C(0xf01a8566edfe99b4), "hash", offsetof(decs::CTypeInfo,hash), 0 };
VarInfo __struct_info__adf7f0b255ff2702_field_4 =  { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xd50d7466d734e3f0), "size", offsetof(decs::CTypeInfo,size), 0 };
TypeInfo * __type_info__d833ed9c9b0916ea_arg_types_var_12535752736546694914[1] = { &__type_info__264a3f04ea74314f };
const char * __type_info__d833ed9c9b0916ea_arg_names_var_12535752736546694914[1] = { "arr" };
VarInfo __struct_info__adf7f0b255ff2702_field_5 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__d833ed9c9b0916ea_arg_types_var_12535752736546694914, __type_info__d833ed9c9b0916ea_arg_names_var_12535752736546694914, 1, 0, nullptr, 12, 8, UINT64_C(0xd833ed9c9b0916ea), "eraser", offsetof(decs::CTypeInfo,eraser), 0 };
TypeInfo * __type_info__4ccf70bfd75cbb93_arg_types_var_12535752736546694914[2] = { &__type_info__264a3f04ea74314f, &__type_info__d83b3ba409a17e95 };
const char * __type_info__4ccf70bfd75cbb93_arg_names_var_12535752736546694914[2] = { "dst", "src" };
VarInfo __struct_info__adf7f0b255ff2702_field_6 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__4ccf70bfd75cbb93_arg_types_var_12535752736546694914, __type_info__4ccf70bfd75cbb93_arg_names_var_12535752736546694914, 2, 0, nullptr, 12, 8, UINT64_C(0x4ccf70bfd75cbb93), "clonner", offsetof(decs::CTypeInfo,clonner), 0 };
TypeInfo * __type_info__e66213a2bae19899_arg_types_var_12535752736546694914[3] = { &__type_info__f8ab3594a8b7b5e8, &__type_info__264a3f04ea74314f, &__type_info__af90fe4c864e9d52 };
const char * __type_info__e66213a2bae19899_arg_names_var_12535752736546694914[3] = { "arch", "arr", "name" };
VarInfo __struct_info__adf7f0b255ff2702_field_7 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__e66213a2bae19899_arg_types_var_12535752736546694914, __type_info__e66213a2bae19899_arg_names_var_12535752736546694914, 3, 0, nullptr, 12, 8, UINT64_C(0xe66213a2bae19899), "serializer", offsetof(decs::CTypeInfo,serializer), 0 };
TypeInfo * __type_info__659a06ad7ed41a45_arg_types_var_12535752736546694914[1] = { &__type_info__9c225ec61b3e6a3c };
const char * __type_info__659a06ad7ed41a45_arg_names_var_12535752736546694914[1] = { "elem" };
VarInfo __struct_info__adf7f0b255ff2702_field_8 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, nullptr, (TypeInfo **)__type_info__659a06ad7ed41a45_arg_types_var_12535752736546694914, __type_info__659a06ad7ed41a45_arg_names_var_12535752736546694914, 1, 0, nullptr, 12, 8, UINT64_C(0x659a06ad7ed41a45), "dumper", offsetof(decs::CTypeInfo,dumper), 0 };
VarInfo __struct_info__adf7f0b255ff2702_field_9 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__b5c5f09960a9d827, nullptr, nullptr, nullptr, 0, 0, nullptr, 12, 8, UINT64_C(0x72fa5fa2aaf87433), "mkTypeInfo", offsetof(decs::CTypeInfo,mkTypeInfo), 0 };
TypeInfo * __type_info__b3053fdf47c5638e_arg_types_var_12535752736546694914[1] = { &__type_info__264a3f04ea74314f };
const char * __type_info__b3053fdf47c5638e_arg_names_var_12535752736546694914[1] = { "src" };
VarInfo __struct_info__adf7f0b255ff2702_field_10 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63fd4c8602249f, nullptr, (TypeInfo **)__type_info__b3053fdf47c5638e_arg_types_var_12535752736546694914, __type_info__b3053fdf47c5638e_arg_names_var_12535752736546694914, 1, 0, nullptr, 12, 8, UINT64_C(0xb3053fdf47c5638e), "gc", offsetof(decs::CTypeInfo,gc), 0 };
VarInfo * __struct_info__adf7f0b255ff2702_fields[11] =  { &__struct_info__adf7f0b255ff2702_field_0, &__struct_info__adf7f0b255ff2702_field_1, &__struct_info__adf7f0b255ff2702_field_2, &__struct_info__adf7f0b255ff2702_field_3, &__struct_info__adf7f0b255ff2702_field_4, &__struct_info__adf7f0b255ff2702_field_5, &__struct_info__adf7f0b255ff2702_field_6, &__struct_info__adf7f0b255ff2702_field_7, &__struct_info__adf7f0b255ff2702_field_8, &__struct_info__adf7f0b255ff2702_field_9, &__struct_info__adf7f0b255ff2702_field_10 };
StructInfo __struct_info__adf7f0b255ff2702 = {"CTypeInfo", "decs", 8, __struct_info__adf7f0b255ff2702_fields, 11, 88, UINT64_C(0x0), nullptr, UINT64_C(0xadf7f0b255ff2702), 1 };
VarInfo __struct_info__62dc6fcf66cdc7b2_field_0 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xd32f19bb3686981c), "name", offsetof(decs::Component,name), 3 };
VarInfo __struct_info__62dc6fcf66cdc7b2_field_1 =  { Type::tUInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 8, UINT64_C(0xd34017bb369db164), "hash", offsetof(decs::Component,hash), 0 };
VarInfo __struct_info__62dc6fcf66cdc7b2_field_2 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x2bbe845219c139b4), "stride", offsetof(decs::Component,stride), 0 };
VarInfo __struct_info__62dc6fcf66cdc7b2_field_3 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, 24, UINT64_C(0xd34716bb369e080d), "data", offsetof(decs::Component,data), 4 };
VarInfo __struct_info__62dc6fcf66cdc7b2_field_4 =  { Type::tStructure, &__struct_info__adf7f0b255ff2702, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16390, 88, UINT64_C(0xe43a02bb450971de), "info", offsetof(decs::Component,info), 5 };
VarInfo __struct_info__62dc6fcf66cdc7b2_field_5 =  { Type::tLambda, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24576, 8, UINT64_C(0x54b80d9198d4ac8d), "gc_dummy", offsetof(decs::Component,gc_dummy), 6 };
VarInfo * __struct_info__62dc6fcf66cdc7b2_fields[6] =  { &__struct_info__62dc6fcf66cdc7b2_field_0, &__struct_info__62dc6fcf66cdc7b2_field_1, &__struct_info__62dc6fcf66cdc7b2_field_2, &__struct_info__62dc6fcf66cdc7b2_field_3, &__struct_info__62dc6fcf66cdc7b2_field_4, &__struct_info__62dc6fcf66cdc7b2_field_5 };
StructInfo __struct_info__62dc6fcf66cdc7b2 = {"Component", "decs", 28, __struct_info__62dc6fcf66cdc7b2_fields, 6, 144, UINT64_C(0x0), nullptr, UINT64_C(0x62dc6fcf66cdc7b2), 0 };
VarInfo __struct_info__99004e5160f37e36_field_0 =  { Type::tTable, nullptr, nullptr, nullptr, &__type_info__6f8c1343480cc5e4, &__type_info__af63e44c8601fa24, nullptr, nullptr, 0, 0, nullptr, 40962, 48, UINT64_C(0xe1496f9b35a41b0), "archetypeLookup", offsetof(decs::DecsState,archetypeLookup), 1 };
VarInfo __struct_info__99004e5160f37e36_field_1 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__cf711d1260800988, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x13102b962e7b8e6b), "allArchetypes", offsetof(decs::DecsState,allArchetypes), 2 };
VarInfo __struct_info__99004e5160f37e36_field_2 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__979842bcb7689575, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, 24, UINT64_C(0xb967929fca60b3d4), "entityFreeList", offsetof(decs::DecsState,entityFreeList), 3 };
VarInfo __struct_info__99004e5160f37e36_field_3 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__26c336131e645000, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, 24, UINT64_C(0x36e461fc6934e180), "entityLookup", offsetof(decs::DecsState,entityLookup), 4 };
VarInfo __struct_info__99004e5160f37e36_field_4 =  { Type::tTable, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, &__type_info__a1bfdcf22cffade1, nullptr, nullptr, 0, 0, nullptr, 57346, 48, UINT64_C(0x386ec463b1bc1dd0), "componentTypeCheck", offsetof(decs::DecsState,componentTypeCheck), 5 };
VarInfo __struct_info__99004e5160f37e36_field_5 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__cef1c98e992968cc, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x79cc2a49405f8801), "ecsQueries", offsetof(decs::DecsState,ecsQueries), 6 };
VarInfo __struct_info__99004e5160f37e36_field_6 =  { Type::tTable, nullptr, nullptr, nullptr, &__type_info__6f8c1343480cc5e4, &__type_info__af63e44c8601fa24, nullptr, nullptr, 0, 0, nullptr, 40962, 48, UINT64_C(0x5eac5551762d72bc), "queryLookup", offsetof(decs::DecsState,queryLookup), 7 };
VarInfo * __struct_info__99004e5160f37e36_fields[7] =  { &__struct_info__99004e5160f37e36_field_0, &__struct_info__99004e5160f37e36_field_1, &__struct_info__99004e5160f37e36_field_2, &__struct_info__99004e5160f37e36_field_3, &__struct_info__99004e5160f37e36_field_4, &__struct_info__99004e5160f37e36_field_5, &__struct_info__99004e5160f37e36_field_6 };
StructInfo __struct_info__99004e5160f37e36 = {"DecsState", "decs", 28, __struct_info__99004e5160f37e36_fields, 7, 240, UINT64_C(0x0), nullptr, UINT64_C(0x99004e5160f37e36), 0 };
VarInfo __struct_info__3ad3d98059b6e265_field_0 =  { Type::tUInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 8, UINT64_C(0xfc9f59842addd61f), "hash", offsetof(decs::EcsRequest,hash), 0 };
VarInfo __struct_info__3ad3d98059b6e265_field_1 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x725636875e5de0c1), "req", offsetof(decs::EcsRequest,req), 2 };
VarInfo __struct_info__3ad3d98059b6e265_field_2 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0xa65f57055984c95d), "reqn", offsetof(decs::EcsRequest,reqn), 3 };
VarInfo __struct_info__3ad3d98059b6e265_field_3 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63e44c8601fa24, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, 24, UINT64_C(0x1c1c2b9a5d9b2e09), "archetypes", offsetof(decs::EcsRequest,archetypes), 4 };
VarInfo __struct_info__3ad3d98059b6e265_field_4 =  { Type::tStructure, &__struct_info__5cade38857dd55b4, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16390, 16, UINT64_C(0xb2b7258794e68bde), "at", offsetof(decs::EcsRequest,at), 5 };
VarInfo * __struct_info__3ad3d98059b6e265_fields[5] =  { &__struct_info__3ad3d98059b6e265_field_0, &__struct_info__3ad3d98059b6e265_field_1, &__struct_info__3ad3d98059b6e265_field_2, &__struct_info__3ad3d98059b6e265_field_3, &__struct_info__3ad3d98059b6e265_field_4 };
StructInfo __struct_info__3ad3d98059b6e265 = {"EcsRequest", "decs", 28, __struct_info__3ad3d98059b6e265_fields, 5, 96, UINT64_C(0x0), nullptr, UINT64_C(0x3ad3d98059b6e265), 1 };
VarInfo __struct_info__5cade38857dd55b4_field_0 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xf8091607bf8f4d5d), "file", offsetof(decs::EcsRequestPos,file), 2 };
VarInfo __struct_info__5cade38857dd55b4_field_1 =  { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xf82d1407bfaf965d), "line", offsetof(decs::EcsRequestPos,line), 0 };
VarInfo * __struct_info__5cade38857dd55b4_fields[2] =  { &__struct_info__5cade38857dd55b4_field_0, &__struct_info__5cade38857dd55b4_field_1 };
StructInfo __struct_info__5cade38857dd55b4 = {"EcsRequestPos", "decs", 8, __struct_info__5cade38857dd55b4_fields, 2, 16, UINT64_C(0x0), nullptr, UINT64_C(0x5cade38857dd55b4), 0 };
VarInfo __struct_info__1d9403e5c2c237d8_field_0 =  { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xabf120f8491329a9), "id", offsetof(decs::EntityId,id), 0 };
VarInfo __struct_info__1d9403e5c2c237d8_field_1 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x5e7f7d8b3878d997), "generation", offsetof(decs::EntityId,generation), 0 };
VarInfo * __struct_info__1d9403e5c2c237d8_fields[2] =  { &__struct_info__1d9403e5c2c237d8_field_0, &__struct_info__1d9403e5c2c237d8_field_1 };
StructInfo __struct_info__1d9403e5c2c237d8 = {"EntityId", "decs", 0, __struct_info__1d9403e5c2c237d8_fields, 2, 8, UINT64_C(0x0), nullptr, UINT64_C(0x1d9403e5c2c237d8), 2 };
TypeInfo * __type_info__64a3a1bb96d4295e_arg_types_var_16222236869669918600[1] = { &__type_info__8d785db1354cca5f };
const char * __type_info__64a3a1bb96d4295e_arg_names_var_16222236869669918600[1] = { "__this" };
VarInfo __struct_info__e120f67bdeeb9788_field_0 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__64a3a1bb96d4295e_arg_types_var_16222236869669918600, __type_info__64a3a1bb96d4295e_arg_names_var_16222236869669918600, 1, 0, nullptr, 12, 8, UINT64_C(0x64a3a1bb96d4295e), "__lambda", offsetof(live::_lambda_live_205_1,__lambda), 0 };
TypeInfo * __type_info__bb98dcfd4a2fffd8_arg_types_var_16222236869669918600[1] = { &__type_info__fc81ca08b970563f };
const char * __type_info__bb98dcfd4a2fffd8_arg_names_var_16222236869669918600[1] = { "__this" };
VarInfo __struct_info__e120f67bdeeb9788_field_1 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__bb98dcfd4a2fffd8_arg_types_var_16222236869669918600, __type_info__bb98dcfd4a2fffd8_arg_names_var_16222236869669918600, 1, 0, nullptr, 12, 8, UINT64_C(0xbb98dcfd4a2fffd8), "__finalize", offsetof(live::_lambda_live_205_1,__finalize), 0 };
VarInfo * __struct_info__e120f67bdeeb9788_fields[2] =  { &__struct_info__e120f67bdeeb9788_field_0, &__struct_info__e120f67bdeeb9788_field_1 };
StructInfo __struct_info__e120f67bdeeb9788 = {"_lambda_live_205_1", "live", 14, __struct_info__e120f67bdeeb9788_fields, 2, 16, UINT64_C(0x0), nullptr, UINT64_C(0xe120f67bdeeb9788), 2 };
TypeInfo * __type_info__e743ca9c6691fda4_arg_types_var_16223714613297855734[2] = { &__type_info__dd290bb17de94649, &__type_info__3884b34b905bfd2e };
const char * __type_info__e743ca9c6691fda4_arg_names_var_16223714613297855734[2] = { "__this", "data" };
VarInfo __struct_info__e126367bdeeebcf6_field_0 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__e743ca9c6691fda4_arg_types_var_16223714613297855734, __type_info__e743ca9c6691fda4_arg_names_var_16223714613297855734, 2, 0, nullptr, 12, 8, UINT64_C(0xe743ca9c6691fda4), "__lambda", offsetof(live::_lambda_live_225_2,__lambda), 0 };
TypeInfo * __type_info__755fcff7f192780e_arg_types_var_16223714613297855734[1] = { &__type_info__2e81ca1ee570f19d };
const char * __type_info__755fcff7f192780e_arg_names_var_16223714613297855734[1] = { "__this" };
VarInfo __struct_info__e126367bdeeebcf6_field_1 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__755fcff7f192780e_arg_types_var_16223714613297855734, __type_info__755fcff7f192780e_arg_names_var_16223714613297855734, 1, 0, nullptr, 12, 8, UINT64_C(0x755fcff7f192780e), "__finalize", offsetof(live::_lambda_live_225_2,__finalize), 0 };
VarInfo * __struct_info__e126367bdeeebcf6_fields[2] =  { &__struct_info__e126367bdeeebcf6_field_0, &__struct_info__e126367bdeeebcf6_field_1 };
StructInfo __struct_info__e126367bdeeebcf6 = {"_lambda_live_225_2", "live", 14, __struct_info__e126367bdeeebcf6_fields, 2, 16, UINT64_C(0x0), nullptr, UINT64_C(0xe126367bdeeebcf6), 2 };
TypeInfo * __type_info__252cce99bfee75a9_arg_types_var_15715370805254367263[2] = { &__type_info__bc41b2a6fc7fa7ce, &__type_info__b9975a5e7e222a13 };
const char * __type_info__252cce99bfee75a9_arg_names_var_15715370805254367263[2] = { "__this", "state" };
VarInfo __struct_info__da18367bd8bb281f_field_0 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__252cce99bfee75a9_arg_types_var_15715370805254367263, __type_info__252cce99bfee75a9_arg_names_var_15715370805254367263, 2, 0, nullptr, 12, 8, UINT64_C(0x252cce99bfee75a9), "__lambda", offsetof(live::_lambda_live_237_3,__lambda), 0 };
TypeInfo * __type_info__be1d38b71e3c6ecd_arg_types_var_15715370805254367263[1] = { &__type_info__e346383245d26f5c };
const char * __type_info__be1d38b71e3c6ecd_arg_names_var_15715370805254367263[1] = { "__this" };
VarInfo __struct_info__da18367bd8bb281f_field_1 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__be1d38b71e3c6ecd_arg_types_var_15715370805254367263, __type_info__be1d38b71e3c6ecd_arg_names_var_15715370805254367263, 1, 0, nullptr, 12, 8, UINT64_C(0xbe1d38b71e3c6ecd), "__finalize", offsetof(live::_lambda_live_237_3,__finalize), 0 };
VarInfo __struct_info__da18367bd8bb281f_field_2 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__767637ee1a337419, nullptr, nullptr, nullptr, 0, 0, nullptr, 25600, 8, UINT64_C(0x436d8545bf0997ac), "lContext", offsetof(live::_lambda_live_237_3,lContext), 3 };
VarInfo * __struct_info__da18367bd8bb281f_fields[3] =  { &__struct_info__da18367bd8bb281f_field_0, &__struct_info__da18367bd8bb281f_field_1, &__struct_info__da18367bd8bb281f_field_2 };
StructInfo __struct_info__da18367bd8bb281f = {"_lambda_live_237_3", "live", 14, __struct_info__da18367bd8bb281f_fields, 3, 24, UINT64_C(0x0), nullptr, UINT64_C(0xda18367bd8bb281f), 2 };
VarInfo __func_info__a4e792ea11638c41_field_0 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, 8, UINT64_C(0x50239cf28bd067fc), "fname", 0, 0 };
VarInfo * __func_info__a4e792ea11638c41_fields[1] =  { &__func_info__a4e792ea11638c41_field_0 };
FuncInfo __func_info__a4e792ea11638c41 = {"invoke block<(fname:string const):void> const", "", __func_info__a4e792ea11638c41_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xa4e792ea11638c41), 0x0 };
VarInfo __func_info__baddab894dba73e8_field_0 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__cf711d1260800988, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x30b54910b1aefa4), "field", 0, 0 };
VarInfo * __func_info__baddab894dba73e8_fields[1] =  { &__func_info__baddab894dba73e8_field_0 };
FuncInfo __func_info__baddab894dba73e8 = {"invoke block<(var field:array<decs::Archetype>):void> const", "", __func_info__baddab894dba73e8_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xbaddab894dba73e8), 0x0 };
VarInfo __func_info__19af4d1c08ef72b2_field_0 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__34d6cc421a9bb601, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x5021a0a702c559cb), "field", 0, 0 };
VarInfo * __func_info__19af4d1c08ef72b2_fields[1] =  { &__func_info__19af4d1c08ef72b2_field_0 };
FuncInfo __func_info__19af4d1c08ef72b2 = {"invoke block<(var field:array<decs::Component>):void> const", "", __func_info__19af4d1c08ef72b2_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x19af4d1c08ef72b2), 0x0 };
VarInfo __func_info__91507666520860cd_field_0 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__cef1c98e992968cc, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0xbe7128724a4f3f31), "field", 0, 0 };
VarInfo * __func_info__91507666520860cd_fields[1] =  { &__func_info__91507666520860cd_field_0 };
FuncInfo __func_info__91507666520860cd = {"invoke block<(var field:array<decs::EcsRequest>):void> const", "", __func_info__91507666520860cd_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x91507666520860cd), 0x0 };
VarInfo __func_info__2c3aa2fcec87dc8f_field_0 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__979842bcb7689575, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, 24, UINT64_C(0xe466752a547036d2), "field", 0, 0 };
VarInfo * __func_info__2c3aa2fcec87dc8f_fields[1] =  { &__func_info__2c3aa2fcec87dc8f_field_0 };
FuncInfo __func_info__2c3aa2fcec87dc8f = {"invoke block<(var field:array<decs::EntityId>):void> const", "", __func_info__2c3aa2fcec87dc8f_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x2c3aa2fcec87dc8f), 0x0 };
VarInfo __func_info__f45f30cc74c8f4a6_field_0 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63e44c8601fa24, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, 24, UINT64_C(0x4214a1d6c9be75dd), "field", 0, 0 };
VarInfo * __func_info__f45f30cc74c8f4a6_fields[1] =  { &__func_info__f45f30cc74c8f4a6_field_0 };
FuncInfo __func_info__f45f30cc74c8f4a6 = {"invoke block<(var field:array<int>):void> const", "", __func_info__f45f30cc74c8f4a6_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xf45f30cc74c8f4a6), 0x0 };
VarInfo __func_info__1bcbcef990d4075b_field_0 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x41bc45d6c97363b3), "field", 0, 0 };
VarInfo * __func_info__1bcbcef990d4075b_fields[1] =  { &__func_info__1bcbcef990d4075b_field_0 };
FuncInfo __func_info__1bcbcef990d4075b = {"invoke block<(var field:array<string>):void> const", "", __func_info__1bcbcef990d4075b_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x1bcbcef990d4075b), 0x0 };
VarInfo __func_info__1dcd71ef3006d28f_field_0 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__26c336131e645000, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, 24, UINT64_C(0xdc1dbd882425d194), "field", 0, 0 };
VarInfo * __func_info__1dcd71ef3006d28f_fields[1] =  { &__func_info__1dcd71ef3006d28f_field_0 };
FuncInfo __func_info__1dcd71ef3006d28f = {"invoke block<(var field:array<tuple<generation:int;archetype:uint64 aka ComponentHash;index:int>>):void> const", "", __func_info__1dcd71ef3006d28f_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x1dcd71ef3006d28f), 0x0 };
VarInfo __func_info__faa6e7b3f28885e9_field_0 =  { Type::tStructure, &__struct_info__5cade38857dd55b4, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16390, 16, UINT64_C(0xfbb7d318fa1b3508), "field", 0, 0 };
VarInfo * __func_info__faa6e7b3f28885e9_fields[1] =  { &__func_info__faa6e7b3f28885e9_field_0 };
FuncInfo __func_info__faa6e7b3f28885e9 = {"invoke block<(var field:decs::EcsRequestPos):void> const", "", __func_info__faa6e7b3f28885e9_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xfaa6e7b3f28885e9), 0x0 };
TypeInfo * __type_info__dc2bafdc4641403e_arg_types_var_15837882118942366734[1] = { &__type_info__9c225ec61b3e6a3c };
const char * __type_info__dc2bafdc4641403e_arg_names_var_15837882118942366734[1] = { "elem" };
VarInfo __func_info__dbcb75dfde121c0e_field_0 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, nullptr, (TypeInfo **)__type_info__dc2bafdc4641403e_arg_types_var_15837882118942366734, __type_info__dc2bafdc4641403e_arg_names_var_15837882118942366734, 1, 0, nullptr, 269, 8, UINT64_C(0xdc2bafdc4641403e), "field", 0, 0 };
VarInfo * __func_info__dbcb75dfde121c0e_fields[1] =  { &__func_info__dbcb75dfde121c0e_field_0 };
FuncInfo __func_info__dbcb75dfde121c0e = {"invoke block<(var field:function<(elem:void? const):string>&):void> const", "", __func_info__dbcb75dfde121c0e_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xdbcb75dfde121c0e), 0x0 };
TypeInfo * __type_info__eac8189bc1bc80e1_arg_types_var_7749045473341499476[3] = { &__type_info__f8ab3594a8b7b5e8, &__type_info__264a3f04ea74314f, &__type_info__af90fe4c864e9d52 };
const char * __type_info__eac8189bc1bc80e1_arg_names_var_7749045473341499476[3] = { "arch", "arr", "name" };
VarInfo __func_info__6b8a23cd42de6854_field_0 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__eac8189bc1bc80e1_arg_types_var_7749045473341499476, __type_info__eac8189bc1bc80e1_arg_names_var_7749045473341499476, 3, 0, nullptr, 269, 8, UINT64_C(0xeac8189bc1bc80e1), "field", 0, 0 };
VarInfo * __func_info__6b8a23cd42de6854_fields[1] =  { &__func_info__6b8a23cd42de6854_field_0 };
FuncInfo __func_info__6b8a23cd42de6854 = {"invoke block<(var field:function<(var arch:archive::Archive -const;var arr:array<uint8> -const;name:string const):void>&):void> const", "", __func_info__6b8a23cd42de6854_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x6b8a23cd42de6854), 0x0 };
TypeInfo * __type_info__e2a573727e1bac42_arg_types_var_4729724882931484876[1] = { &__type_info__264a3f04ea74314f };
const char * __type_info__e2a573727e1bac42_arg_names_var_4729724882931484876[1] = { "arr" };
VarInfo __func_info__41a35bb8cc7508cc_field_0 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__e2a573727e1bac42_arg_types_var_4729724882931484876, __type_info__e2a573727e1bac42_arg_names_var_4729724882931484876, 1, 0, nullptr, 269, 8, UINT64_C(0xe2a573727e1bac42), "field", 0, 0 };
VarInfo * __func_info__41a35bb8cc7508cc_fields[1] =  { &__func_info__41a35bb8cc7508cc_field_0 };
FuncInfo __func_info__41a35bb8cc7508cc = {"invoke block<(var field:function<(var arr:array<uint8> -const):void>&):void> const", "", __func_info__41a35bb8cc7508cc_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x41a35bb8cc7508cc), 0x0 };
TypeInfo * __type_info__d63b6b9131892694_arg_types_var_14552490671196758338[2] = { &__type_info__264a3f04ea74314f, &__type_info__d83b3ba409a17e95 };
const char * __type_info__d63b6b9131892694_arg_names_var_14552490671196758338[2] = { "dst", "src" };
VarInfo __func_info__c9f4d529bdde6142_field_0 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__d63b6b9131892694_arg_types_var_14552490671196758338, __type_info__d63b6b9131892694_arg_names_var_14552490671196758338, 2, 0, nullptr, 269, 8, UINT64_C(0xd63b6b9131892694), "field", 0, 0 };
VarInfo * __func_info__c9f4d529bdde6142_fields[1] =  { &__func_info__c9f4d529bdde6142_field_0 };
FuncInfo __func_info__c9f4d529bdde6142 = {"invoke block<(var field:function<(var dst:array<uint8> -const;src:array<uint8> const):void>&):void> const", "", __func_info__c9f4d529bdde6142_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xc9f4d529bdde6142), 0x0 };
TypeInfo * __type_info__8b0d17b41f76f7e4_arg_types_var_10947765872431360765[1] = { &__type_info__264a3f04ea74314f };
const char * __type_info__8b0d17b41f76f7e4_arg_names_var_10947765872431360765[1] = { "src" };
VarInfo __func_info__97ee470ea66a6afd_field_0 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63fd4c8602249f, nullptr, (TypeInfo **)__type_info__8b0d17b41f76f7e4_arg_types_var_10947765872431360765, __type_info__8b0d17b41f76f7e4_arg_names_var_10947765872431360765, 1, 0, nullptr, 269, 8, UINT64_C(0x8b0d17b41f76f7e4), "field", 0, 0 };
VarInfo * __func_info__97ee470ea66a6afd_fields[1] =  { &__func_info__97ee470ea66a6afd_field_0 };
FuncInfo __func_info__97ee470ea66a6afd = {"invoke block<(var field:function<(var src:array<uint8> -const):lambda<>>&):void> const", "", __func_info__97ee470ea66a6afd_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x97ee470ea66a6afd), 0x0 };
VarInfo __func_info__766dae3d6a2a7d14_field_0 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__b5c5f09960a9d827, nullptr, nullptr, nullptr, 0, 0, nullptr, 269, 8, UINT64_C(0xc2d42c7deb7bdbd6), "field", 0, 0 };
VarInfo * __func_info__766dae3d6a2a7d14_fields[1] =  { &__func_info__766dae3d6a2a7d14_field_0 };
FuncInfo __func_info__766dae3d6a2a7d14 = {"invoke block<(var field:function<rtti::TypeInfo const?>&):void> const", "", __func_info__766dae3d6a2a7d14_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x766dae3d6a2a7d14), 0x0 };
VarInfo __func_info__9e7ffb69e4895cc9_field_0 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 285, 4, UINT64_C(0xea7703fdd48b8362), "field", 0, 0 };
VarInfo * __func_info__9e7ffb69e4895cc9_fields[1] =  { &__func_info__9e7ffb69e4895cc9_field_0 };
FuncInfo __func_info__9e7ffb69e4895cc9 = {"invoke block<(var field:int&):void> const", "", __func_info__9e7ffb69e4895cc9_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x9e7ffb69e4895cc9), 0x0 };
VarInfo __func_info__d7138e6aaa104191_field_0 =  { Type::tEnumeration, nullptr, &__enum_info__c897fe55afe7f727, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 285, 4, UINT64_C(0x73878c966ce41f3f), "field", 0, 0 };
VarInfo * __func_info__d7138e6aaa104191_fields[1] =  { &__func_info__d7138e6aaa104191_field_0 };
FuncInfo __func_info__d7138e6aaa104191 = {"invoke block<(var field:rtti::Type&):void> const", "", __func_info__d7138e6aaa104191_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xd7138e6aaa104191), 0x0 };
VarInfo __func_info__c85cb34a7aabfce2_field_0 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16645, 8, UINT64_C(0xea7d03fdd495b562), "field", 0, 0 };
VarInfo * __func_info__c85cb34a7aabfce2_fields[1] =  { &__func_info__c85cb34a7aabfce2_field_0 };
FuncInfo __func_info__c85cb34a7aabfce2 = {"invoke block<(var field:string&):void> const", "", __func_info__c85cb34a7aabfce2_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xc85cb34a7aabfce2), 0x0 };
VarInfo __func_info__88667fa8dd2c245b_field_0 =  { Type::tTable, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, &__type_info__a1bfdcf22cffade1, nullptr, nullptr, 0, 0, nullptr, 57346, 48, UINT64_C(0x80697368a766432), "field", 0, 0 };
VarInfo * __func_info__88667fa8dd2c245b_fields[1] =  { &__func_info__88667fa8dd2c245b_field_0 };
FuncInfo __func_info__88667fa8dd2c245b = {"invoke block<(var field:table<string;decs::CTypeInfo>):void> const", "", __func_info__88667fa8dd2c245b_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x88667fa8dd2c245b), 0x0 };
VarInfo __func_info__e43d9eedb8768372_field_0 =  { Type::tTable, nullptr, nullptr, nullptr, &__type_info__6f8c1343480cc5e4, &__type_info__af63e44c8601fa24, nullptr, nullptr, 0, 0, nullptr, 40962, 48, UINT64_C(0x30ac1da2c9bdee4d), "field", 0, 0 };
VarInfo * __func_info__e43d9eedb8768372_fields[1] =  { &__func_info__e43d9eedb8768372_field_0 };
FuncInfo __func_info__e43d9eedb8768372 = {"invoke block<(var field:table<uint64 aka ComponentHash;int>):void> const", "", __func_info__e43d9eedb8768372_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xe43d9eedb8768372), 0x0 };
VarInfo __func_info__e5601d0b9c7536bc_field_0 =  { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 285, 4, UINT64_C(0xea8303fdd49fe762), "field", 0, 0 };
VarInfo * __func_info__e5601d0b9c7536bc_fields[1] =  { &__func_info__e5601d0b9c7536bc_field_0 };
FuncInfo __func_info__e5601d0b9c7536bc = {"invoke block<(var field:uint&):void> const", "", __func_info__e5601d0b9c7536bc_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xe5601d0b9c7536bc), 0x0 };
VarInfo __func_info__214fcb6dd261a4ee_field_0 =  { Type::tUInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 285, 8, UINT64_C(0x1c731c504b9417bc), "field", 0, 0 };
VarInfo * __func_info__214fcb6dd261a4ee_fields[1] =  { &__func_info__214fcb6dd261a4ee_field_0 };
FuncInfo __func_info__214fcb6dd261a4ee = {"invoke block<(var field:uint64&):void> const", "", __func_info__214fcb6dd261a4ee_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x214fcb6dd261a4ee), 0x0 };
VarInfo __func_info__fe02a5da011a6cb4_field_0 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0x865026081ff8017a), "ok", 0, 0 };
VarInfo __func_info__fe02a5da011a6cb4_field_1 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__125855d9cd771ead, nullptr, nullptr, nullptr, 0, 0, nullptr, 11264, 8, UINT64_C(0x601b19a6ca455003), "program", 0, 0 };
VarInfo __func_info__fe02a5da011a6cb4_field_2 =  { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~$::das_string"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 34, 32, UINT64_C(0xe632c403d42f243a), "issues", 0, 0 };
VarInfo * __func_info__fe02a5da011a6cb4_fields[3] =  { &__func_info__fe02a5da011a6cb4_field_0, &__func_info__fe02a5da011a6cb4_field_1, &__func_info__fe02a5da011a6cb4_field_2 };
FuncInfo __func_info__fe02a5da011a6cb4 = {"invoke block<(var ok:bool;var program:smart_ptr<rtti::Program>;issues:$::das_string const):void> const", "", __func_info__fe02a5da011a6cb4_fields, 3, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xfe02a5da011a6cb4), 0x0 };
VarInfo __func_info__8eb7d9f42d992591_field_0 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0x3a3585e2284c6c6b), "sok", 0, 0 };
VarInfo __func_info__8eb7d9f42d992591_field_1 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__767637ee1a337419, nullptr, nullptr, nullptr, 0, 0, nullptr, 27648, 8, UINT64_C(0x991b098814c694e3), "context", 0, 0 };
VarInfo __func_info__8eb7d9f42d992591_field_2 =  { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~$::das_string"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 2, 32, UINT64_C(0x7c5077e54e2863f8), "serrors", 0, 0 };
VarInfo * __func_info__8eb7d9f42d992591_fields[3] =  { &__func_info__8eb7d9f42d992591_field_0, &__func_info__8eb7d9f42d992591_field_1, &__func_info__8eb7d9f42d992591_field_2 };
FuncInfo __func_info__8eb7d9f42d992591 = {"invoke block<(var sok:bool;var context:smart_ptr<rtti::Context>;var serrors:$::das_string):void> const", "", __func_info__8eb7d9f42d992591_fields, 3, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x8eb7d9f42d992591), 0x0 };
TypeInfo __type_info__86304441d7b56b0c = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 41218, 24, UINT64_C(0x86304441d7b56b0c) };
TypeInfo __type_info__b5c5f09960a9d827 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__bc67beb4aa160fd4, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0xb5c5f09960a9d827) };
TypeInfo __type_info__f4b9fc3e25a5c7c1 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__767637ee1a337419, nullptr, nullptr, nullptr, 0, 0, nullptr, 25600, 8, UINT64_C(0xf4b9fc3e25a5c7c1) };
TypeInfo __type_info__b3327a1376bd9760 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__a30ba508046366f1, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0xb3327a1376bd9760) };
TypeInfo __type_info__fc81ca08b970563f = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__8d785db1354cca5f, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0xfc81ca08b970563f) };
TypeInfo __type_info__2e81ca1ee570f19d = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__dd290bb17de94649, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0x2e81ca1ee570f19d) };
TypeInfo __type_info__e346383245d26f5c = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__bc41b2a6fc7fa7ce, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0xe346383245d26f5c) };
TypeInfo __type_info__15192774a936dc4d = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__3fb1dab7ab02f59, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x15192774a936dc4d) };
TypeInfo __type_info__92aa643dc780b0b0 = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__3452466d210e34e2, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x92aa643dc780b0b0) };
TypeInfo __type_info__8ed317d13fd59c13 = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__29e8184e4772e4bc, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x8ed317d13fd59c13) };
TypeInfo __type_info__264a3f04ea74314f = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, 24, UINT64_C(0x264a3f04ea74314f) };
TypeInfo __type_info__12283b04d98e775a = { Type::tLambda, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, nullptr, nullptr, 0, 0, nullptr, 24576, 8, UINT64_C(0x12283b04d98e775a) };
TypeInfo __type_info__4ac1d999a882997b = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, 24, UINT64_C(0x4ac1d999a882997b) };
TypeInfo __type_info__af63fd4c8602249f = { Type::tLambda, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24576, 8, UINT64_C(0xaf63fd4c8602249f) };
TypeInfo __type_info__3884b34b905bfd2e = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 41058, 24, UINT64_C(0x3884b34b905bfd2e) };
TypeInfo __type_info__b9975a5e7e222a13 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__a30ba508046366f1, nullptr, nullptr, nullptr, 0, 0, nullptr, 24620, 8, UINT64_C(0xb9975a5e7e222a13) };
TypeInfo __type_info__d83b3ba409a17e95 = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 40994, 24, UINT64_C(0xd83b3ba409a17e95) };
TypeInfo __type_info__9c225ec61b3e6a3c = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, nullptr, nullptr, 0, 0, nullptr, 8236, 8, UINT64_C(0x9c225ec61b3e6a3c) };
TypeInfo __type_info__624d371c76b25aa4 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~$::das_string"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 34, 32, UINT64_C(0x624d371c76b25aa4) };
TypeInfo __type_info__bc67beb4aa160fd4 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~rtti::TypeInfo"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24622, 80, UINT64_C(0xbc67beb4aa160fd4) };
TypeInfo __type_info__3a95295ead7e9066 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, nullptr, nullptr, 0, 0, nullptr, 8364, 8, UINT64_C(0x3a95295ead7e9066) };
TypeInfo __type_info__af81fe4c86352052 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 1, UINT64_C(0xaf81fe4c86352052) };
TypeInfo __type_info__af87fe4c863f5252 = { Type::tDouble, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 8, UINT64_C(0xaf87fe4c863f5252) };
TypeInfo __type_info__af85fe4c863bec52 = { Type::tFloat, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 4, UINT64_C(0xaf85fe4c863bec52) };
TypeInfo __type_info__af8afe4c86446b52 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 4, UINT64_C(0xaf8afe4c86446b52) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, 8, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__af96fe4c8658cf52 = { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 4, UINT64_C(0xaf96fe4c8658cf52) };
TypeInfo __type_info__b68d800849332aec = { Type::tUInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 8, UINT64_C(0xb68d800849332aec) };
TypeInfo __type_info__767637ee1a337419 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~rtti::Context"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, 576, UINT64_C(0x767637ee1a337419) };
TypeInfo __type_info__125855d9cd771ead = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~rtti::Program"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 8194, 632, UINT64_C(0x125855d9cd771ead) };
TypeInfo * __type_info__54c9a4192a31329c_arg_types[1] = { &__type_info__3884b34b905bfd2e };
const char * __type_info__54c9a4192a31329c_arg_names[1] = { "data" };
TypeInfo __type_info__54c9a4192a31329c = { Type::tLambda, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__54c9a4192a31329c_arg_types, __type_info__54c9a4192a31329c_arg_names, 1, 0, nullptr, 24576, 8, UINT64_C(0x54c9a4192a31329c) };
TypeInfo * __type_info__26c336131e645000_arg_types[3] = { &__type_info__af63e44c8601fa24, &__type_info__6f8c1343480cc5e4, &__type_info__af63e44c8601fa24 };
const char * __type_info__26c336131e645000_arg_names[3] = { "generation", "archetype", "index" };
TypeInfo __type_info__26c336131e645000 = { Type::tTuple, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__26c336131e645000_arg_types, __type_info__26c336131e645000_arg_names, 3, 0, nullptr, 30, 24, UINT64_C(0x26c336131e645000) };
TypeInfo * __type_info__da6749c16df8cbcb_arg_types[1] = { &__type_info__b9975a5e7e222a13 };
const char * __type_info__da6749c16df8cbcb_arg_names[1] = { "state" };
TypeInfo __type_info__da6749c16df8cbcb = { Type::tLambda, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__da6749c16df8cbcb_arg_types, __type_info__da6749c16df8cbcb_arg_names, 1, 0, nullptr, 24576, 8, UINT64_C(0xda6749c16df8cbcb) };
TypeInfo __type_info__f8ab3594a8b7b5e8 = { Type::tStructure, &__struct_info__e48f7ed30ac22e75, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24590, 16, UINT64_C(0xf8ab3594a8b7b5e8) };
TypeInfo __type_info__d400c4922cb4cfd0 = { Type::tStructure, &__struct_info__2ea4c52d3e792fcf, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24590, 48, UINT64_C(0xd400c4922cb4cfd0) };
TypeInfo __type_info__cf711d1260800988 = { Type::tStructure, &__struct_info__f9485bd96ad1fe2b, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 40, UINT64_C(0xcf711d1260800988) };
TypeInfo __type_info__a1bfdcf22cffade1 = { Type::tStructure, &__struct_info__adf7f0b255ff2702, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16390, 88, UINT64_C(0xa1bfdcf22cffade1) };
TypeInfo __type_info__34d6cc421a9bb601 = { Type::tStructure, &__struct_info__62dc6fcf66cdc7b2, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 144, UINT64_C(0x34d6cc421a9bb601) };
TypeInfo __type_info__a30ba508046366f1 = { Type::tStructure, &__struct_info__99004e5160f37e36, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 240, UINT64_C(0xa30ba508046366f1) };
TypeInfo __type_info__cef1c98e992968cc = { Type::tStructure, &__struct_info__3ad3d98059b6e265, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 96, UINT64_C(0xcef1c98e992968cc) };
TypeInfo __type_info__979842bcb7689575 = { Type::tStructure, &__struct_info__1d9403e5c2c237d8, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 30, 8, UINT64_C(0x979842bcb7689575) };
TypeInfo __type_info__8d785db1354cca5f = { Type::tStructure, &__struct_info__e120f67bdeeb9788, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24590, 16, UINT64_C(0x8d785db1354cca5f) };
TypeInfo __type_info__dd290bb17de94649 = { Type::tStructure, &__struct_info__e126367bdeeebcf6, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24590, 16, UINT64_C(0xdd290bb17de94649) };
TypeInfo __type_info__bc41b2a6fc7fa7ce = { Type::tStructure, &__struct_info__da18367bd8bb281f, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, 24, UINT64_C(0xbc41b2a6fc7fa7ce) };
TypeInfo __type_info__6f8c1343480cc5e4 = { Type::tUInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 8, UINT64_C(0x6f8c1343480cc5e4) };
TypeInfo __type_info__3fb1dab7ab02f59 = { Type::tStructure, &__struct_info__f9485bd96ad1fe2b, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 40, UINT64_C(0x3fb1dab7ab02f59) };
TypeInfo __type_info__3452466d210e34e2 = { Type::tStructure, &__struct_info__62dc6fcf66cdc7b2, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 144, UINT64_C(0x3452466d210e34e2) };
TypeInfo __type_info__29e8184e4772e4bc = { Type::tStructure, &__struct_info__3ad3d98059b6e265, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 96, UINT64_C(0x29e8184e4772e4bc) };
TypeInfo __type_info__a8d2247d5b8ed91f = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, 24, UINT64_C(0xa8d2247d5b8ed91f) };
TypeInfo __type_info__7dcc2c583c294a1f = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, 24, UINT64_C(0x7dcc2c583c294a1f) };
TypeInfo __type_info__af63df4c8601f1a5 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0xaf63df4c8601f1a5) };
TypeInfo __type_info__af63e44c8601fa24 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xaf63e44c8601fa24) };
TypeInfo __type_info__af63ee4c86020b22 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xaf63ee4c86020b22) };
TypeInfo __type_info__af5be84c85f468f0 = { Type::tUInt8, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0xaf5be84c85f468f0) };
TypeInfo __type_info__af63eb4c86020609 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 0, UINT64_C(0xaf63eb4c86020609) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {__type_info__624d371c76b25aa4, __type_info__bc67beb4aa160fd4, __type_info__767637ee1a337419, __type_info__125855d9cd771ead, };
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[1] = { &__type_info__92aa643dc780b0b0 };
TypeInfo * __tinfo_1[1] = { &__type_info__15192774a936dc4d };
TypeInfo * __tinfo_2[1] = { &__type_info__8ed317d13fd59c13 };
TypeInfo * __tinfo_3[1] = { &__type_info__4ac1d999a882997b };
TypeInfo * __tinfo_4[1] = { &__type_info__a8d2247d5b8ed91f };
TypeInfo * __tinfo_5[1] = { &__type_info__7dcc2c583c294a1f };
TypeInfo * __tinfo_6[3] = { &__type_info__f4b9fc3e25a5c7c1, &__type_info__af90fe4c864e9d52, &__type_info__af81fe4c86352052 };
TypeInfo * __tinfo_7[9] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af81fe4c86352052, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_8[6] = { &__type_info__f4b9fc3e25a5c7c1, &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af8afe4c86446b52, &__type_info__af8afe4c86446b52, &__type_info__af8afe4c86446b52 };
TypeInfo * __tinfo_9[15] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_10[3] = { &__type_info__f4b9fc3e25a5c7c1, &__type_info__af90fe4c864e9d52, &__type_info__af96fe4c8658cf52 };
TypeInfo * __tinfo_11[9] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af96fe4c8658cf52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_12[4] = { &__type_info__f4b9fc3e25a5c7c1, &__type_info__af90fe4c864e9d52, &__type_info__af87fe4c863f5252, &__type_info__af87fe4c863f5252 };
TypeInfo * __tinfo_13[11] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af87fe4c863f5252, &__type_info__af90fe4c864e9d52, &__type_info__af87fe4c863f5252, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_14[5] = { &__type_info__f4b9fc3e25a5c7c1, &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af8afe4c86446b52, &__type_info__af8afe4c86446b52 };
TypeInfo * __tinfo_15[13] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_16[3] = { &__type_info__f4b9fc3e25a5c7c1, &__type_info__af90fe4c864e9d52, &__type_info__af85fe4c863bec52 };
TypeInfo * __tinfo_17[9] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af85fe4c863bec52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_18[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_19[2] = { &__type_info__f4b9fc3e25a5c7c1, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_20[7] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_21[3] = { &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_22[3] = { &__type_info__af90fe4c864e9d52, &__type_info__b68d800849332aec, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_23[2] = { &__type_info__f4b9fc3e25a5c7c1, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_24[2] = { &__type_info__f4b9fc3e25a5c7c1, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_25[2] = { &__type_info__f4b9fc3e25a5c7c1, &__type_info__12283b04d98e775a };
TypeInfo * __tinfo_26[2] = { &__type_info__f4b9fc3e25a5c7c1, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_27[2] = { &__type_info__f4b9fc3e25a5c7c1, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_28[3] = { &__type_info__f4b9fc3e25a5c7c1, &__type_info__54c9a4192a31329c, &__type_info__86304441d7b56b0c };
TypeInfo * __tinfo_29[2] = { &__type_info__f4b9fc3e25a5c7c1, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_30[3] = { &__type_info__f4b9fc3e25a5c7c1, &__type_info__da6749c16df8cbcb, &__type_info__b3327a1376bd9760 };
TypeInfo * __tinfo_31[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af81fe4c86352052, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_32[3] = { &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_33[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__624d371c76b25aa4, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_34[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__624d371c76b25aa4, &__type_info__af90fe4c864e9d52 };

inline void _FuncbuiltinTickresizeTick4811697762258667383_559ca47f392751ff ( Context * __context__, TArray<decs::Component> & __Arr_rename_at_68_0, int32_t __newSize_rename_at_68_1 );
inline void _FuncarchiveTickserialize_rawTick346513482259279339_2cb15e82f6a052bb ( Context * __context__, archive::Archive & __arch_rename_at_90_2, uint32_t & __value_rename_at_90_3 );
inline void _FuncarchiveTickserialize_rawTick346513482259279339_877086b5c2b9e6d8 ( Context * __context__, archive::Archive & __arch_rename_at_90_4, DAS_COMMENT(bound_enum) das::Type & __value_rename_at_90_5 );
inline void _FuncarchiveTickread_rawTick6802855309095275289_59f27d4ca7f63255 ( Context * __context__, archive::Archive & __arch_rename_at_99_6, uint64_t & __value_rename_at_99_7 );
inline void _FuncarchiveTickwrite_rawTick9760319972816126958_7e827e6b1de76fcd ( Context * __context__, archive::Archive & __arch_rename_at_105_8, uint64_t & __value_rename_at_105_9 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_14682004b38ad2a1 ( Context * __context__, TArray<char *> & __Arr_rename_at_68_10, int32_t __newSize_rename_at_68_11 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_1bdb9995fa062a51 ( Context * __context__, TArray<int32_t> & __Arr_rename_at_68_12, int32_t __newSize_rename_at_68_13 );
inline void _FuncapplyTickstructTickEcsRequestPosTick0x96Tick0Tick2_cf2646de905784ac ( Context * __context__, decs::EcsRequestPos & ___Var_Tick_self_rename_at_62_14, Block DAS_COMMENT((void,char * &)) const  & ____arg_file_rename_at_62_15, Block DAS_COMMENT((void,uint32_t &)) const  & ____arg_line_rename_at_62_16 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_345f02cc82623620 ( Context * __context__, TArray<decs::Component> & __a_rename_at_1182_17 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_79f9e841b4434fd8 ( Context * __context__, TArray<char *> & __a_rename_at_1182_19 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_c2c15811c6d720e8 ( Context * __context__, TArray<int32_t> & __a_rename_at_1182_20 );
inline void _FuncarchiveTickserializeTick1002677114065211290_838b04f3311f9ee8 ( Context * __context__, archive::Archive & __arch_rename_at_189_21, TArray<decs::Component> & __value_rename_at_189_22 );
inline void _FuncarchiveTickserializeTick6962985013043226618_c51e37371f3d267a ( Context * __context__, archive::Archive & __arch_rename_at_112_27, uint32_t & __value_rename_at_112_28 );
inline void _FuncarchiveTickserializeTick6962985013043226618_ed7dae2a007ca602 ( Context * __context__, archive::Archive & __arch_rename_at_112_29, DAS_COMMENT(bound_enum) das::Type & __value_rename_at_112_30 );
inline void _FuncarchiveTickserializeTick4429977757536996025_2b74a39202406253 ( Context * __context__, archive::Archive & __arch_rename_at_118_31, Func DAS_COMMENT((void,TArray<uint8_t>)) & __value_rename_at_118_32 );
inline void _FuncarchiveTickserializeTick4429977757536996025_1ff57546f6343a0a ( Context * __context__, archive::Archive & __arch_rename_at_118_35, Func DAS_COMMENT((void,TArray<uint8_t>,TArray<uint8_t> const )) & __value_rename_at_118_36 );
inline void _FuncarchiveTickserializeTick4429977757536996025_beb22678ca166fa4 ( Context * __context__, archive::Archive & __arch_rename_at_118_39, Func DAS_COMMENT((void,archive::Archive,TArray<uint8_t>,char * const )) & __value_rename_at_118_40 );
inline void _FuncarchiveTickserializeTick4429977757536996025_3b53a58effc6756a ( Context * __context__, archive::Archive & __arch_rename_at_118_43, Func DAS_COMMENT((char *,void * const )) & __value_rename_at_118_44 );
inline void _FuncarchiveTickserializeTick4429977757536996025_190cc1e99493f13c ( Context * __context__, archive::Archive & __arch_rename_at_118_47, Func DAS_COMMENT((TypeInfo const  *)) & __value_rename_at_118_48 );
inline void _FuncarchiveTickserializeTick4429977757536996025_25683f2273767516 ( Context * __context__, archive::Archive & __arch_rename_at_118_51, Func DAS_COMMENT((Lambda DAS_COMMENT((void)),TArray<uint8_t>)) & __value_rename_at_118_52 );
inline void _FuncarchiveTickserializeTick1002677114065211290_bef6bf5e85c51d3b ( Context * __context__, archive::Archive & __arch_rename_at_189_55, TArray<char *> & __value_rename_at_189_56 );
inline void _FuncarchiveTickserializeTick1002677114065211290_591dc6f784de6b57 ( Context * __context__, archive::Archive & __arch_rename_at_189_61, TArray<int32_t> & __value_rename_at_189_62 );
inline void _FuncarchiveTickserializeTick8013964856239694639_cac39795b36f7c56 ( Context * __context__, archive::Archive & __arch_rename_at_146_67, decs::EcsRequestPos & __value_rename_at_146_68 );
inline void finalize_9a888ca44a018013 ( Context * __context__, decs::EcsRequestPos & ____this_rename_at_81_71 );
inline void _FuncarchiveTickserialize_rawTick346513482259279339_83ff526b8b4a3feb ( Context * __context__, archive::Archive & __arch_rename_at_90_72, uint64_t & __value_rename_at_90_73 );
inline void _FuncarchiveTickserialize_rawTick346513482259279339_344032bd32dfc282 ( Context * __context__, archive::Archive & __arch_rename_at_90_74, int32_t & __value_rename_at_90_75 );
inline void finalize_37451350471f63d7 ( Context * __context__, decs::Archetype & ____this_rename_at_57_76 );
inline void _FuncapplyTickstructTickArchetypeTick0x96Tick0Tick4_86443a66f85f8e05 ( Context * __context__, decs::Archetype & ___Var_Tick_self_rename_at_62_77, Block DAS_COMMENT((void,uint64_t &)) const  & ____arg_hash_rename_at_62_78, Block DAS_COMMENT((void,TArray<decs::Component>)) const  & ____arg_components_rename_at_62_79, Block DAS_COMMENT((void,int32_t &)) const  & ____arg_size_rename_at_62_80, Block DAS_COMMENT((void,int32_t &)) const  & ____arg_eidIndex_rename_at_62_81 );
inline void finalize_aed1eae423063a73 ( Context * __context__, decs::EntityId & ____this_rename_at_51_82 );
inline void _FuncapplyTickstructTickEntityIdTick0x96Tick0Tick2_ceabfe323854d15e ( Context * __context__, decs::EntityId & ___Var_Tick_self_rename_at_62_83, Block DAS_COMMENT((void,uint32_t &)) const  & ____arg_id_rename_at_62_84, Block DAS_COMMENT((void,int32_t &)) const  & ____arg_generation_rename_at_62_85 );
inline void finalize_f87ddce7a3ad05ec ( Context * __context__, AutoTuple<int32_t,uint64_t,int32_t> & ____this_rename_at_158_86 );
inline void _FuncapplyTicktupleTick0x6f4dd0adfbe8abacTick0xa0Tick0Tick3_69d210316d1d7319 ( Context * __context__, AutoTuple<int32_t,uint64_t,int32_t> & ___Var_Tick_self_rename_at_112_87, Block DAS_COMMENT((void,int32_t &)) const  & ____arg_generation_rename_at_112_88, Block DAS_COMMENT((void,uint64_t &)) const  & ____arg_archetype_rename_at_112_89, Block DAS_COMMENT((void,int32_t &)) const  & ____arg_index_rename_at_112_90 );
inline void finalize_c2cf16c62ac9b1f5 ( Context * __context__, decs::CTypeInfo & ____this_rename_at_25_91 );
inline void _FuncapplyTickstructTickCTypeInfoTick0x96Tick0Tick11_50e2ea000b6a5668 ( Context * __context__, decs::CTypeInfo & ___Var_Tick_self_rename_at_62_92, Block DAS_COMMENT((void,DAS_COMMENT(bound_enum) das::Type &)) const  & ____arg_basicType_rename_at_62_93, Block DAS_COMMENT((void,char * &)) const  & ____arg_mangledName_rename_at_62_94, Block DAS_COMMENT((void,char * &)) const  & ____arg_fullName_rename_at_62_95, Block DAS_COMMENT((void,uint64_t &)) const  & ____arg_hash_rename_at_62_96, Block DAS_COMMENT((void,uint32_t &)) const  & ____arg_size_rename_at_62_97, Block DAS_COMMENT((void,Func DAS_COMMENT((void,TArray<uint8_t>)) &)) const  & ____arg_eraser_rename_at_62_98, Block DAS_COMMENT((void,Func DAS_COMMENT((void,TArray<uint8_t>,TArray<uint8_t> const )) &)) const  & ____arg_clonner_rename_at_62_99, Block DAS_COMMENT((void,Func DAS_COMMENT((void,archive::Archive,TArray<uint8_t>,char * const )) &)) const  & ____arg_serializer_rename_at_62_100, Block DAS_COMMENT((void,Func DAS_COMMENT((char *,void * const )) &)) const  & ____arg_dumper_rename_at_62_101, Block DAS_COMMENT((void,Func DAS_COMMENT((TypeInfo const  *)) &)) const  & ____arg_mkTypeInfo_rename_at_62_102, Block DAS_COMMENT((void,Func DAS_COMMENT((Lambda DAS_COMMENT((void)),TArray<uint8_t>)) &)) const  & ____arg_gc_rename_at_62_103 );
inline void finalize_723506a070851f1e ( Context * __context__, decs::EcsRequest & ____this_rename_at_92_104 );
inline void _FuncapplyTickstructTickEcsRequestTick0x96Tick0Tick5_1da32bb85bdaa226 ( Context * __context__, decs::EcsRequest & ___Var_Tick_self_rename_at_62_105, Block DAS_COMMENT((void,uint64_t &)) const  & ____arg_hash_rename_at_62_106, Block DAS_COMMENT((void,TArray<char *>)) const  & ____arg_req_rename_at_62_107, Block DAS_COMMENT((void,TArray<char *>)) const  & ____arg_reqn_rename_at_62_108, Block DAS_COMMENT((void,TArray<int32_t>)) const  & ____arg_archetypes_rename_at_62_109, Block DAS_COMMENT((void,decs::EcsRequestPos)) const  & ____arg_at_rename_at_62_110 );
inline void _FuncarchiveTickread_rawTick6802855309095275289_b9dfe21dac02b654 ( Context * __context__, archive::Archive & __arch_rename_at_99_111, int32_t & __value_rename_at_99_112 );
inline void _FuncarchiveTickserializeTick6962985013043226618_125aa1d245ce9a68 ( Context * __context__, archive::Archive & __arch_rename_at_112_113, uint64_t & __value_rename_at_112_114 );
inline void _FuncarchiveTickserializeTick6962985013043226618_8e0595ec66b9d4fd ( Context * __context__, archive::Archive & __arch_rename_at_112_115, int32_t & __value_rename_at_112_116 );
inline void _FuncarchiveTickwrite_rawTick9760319972816126958_e9195ce9edbb9e44 ( Context * __context__, archive::Archive & __arch_rename_at_105_117, int32_t & __value_rename_at_105_118 );
inline Sequence DAS_COMMENT((uint64_t)) _FuncbuiltinTickkeysTick2205854368403803976_137bf91714c0645e ( Context * __context__, TTable<uint64_t,int32_t> const  & __a_rename_at_1128_119 );
inline Sequence DAS_COMMENT((int32_t &)) _FuncbuiltinTickvaluesTick1351216622833168869_38542bdf50c4ab32 ( Context * __context__, TTable<uint64_t,int32_t> & __a_rename_at_1150_121 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_aeca051a2e04cb07 ( Context * __context__, TArray<decs::Archetype> & __Arr_rename_at_68_123, int32_t __newSize_rename_at_68_124 );
inline void _FuncarchiveTickserializeTick8013964856239694639_731eb25edf591333 ( Context * __context__, archive::Archive & __arch_rename_at_146_125, decs::Archetype & __value_rename_at_146_126 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_be5e4db5cc62bbe2 ( Context * __context__, TArray<decs::EntityId> & __Arr_rename_at_68_131, int32_t __newSize_rename_at_68_132 );
inline void _FuncarchiveTickserializeTick8013964856239694639_9f9391ad59b315bc ( Context * __context__, archive::Archive & __arch_rename_at_146_133, decs::EntityId & __value_rename_at_146_134 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_deb20fe608939269 ( Context * __context__, TArray<AutoTuple<int32_t,uint64_t,int32_t>> & __Arr_rename_at_68_137, int32_t __newSize_rename_at_68_138 );
inline void _FuncarchiveTickserializeTick10018991383941125049_67930047264965f1 ( Context * __context__, archive::Archive & __arch_rename_at_156_139, AutoTuple<int32_t,uint64_t,int32_t> & __value_rename_at_156_140 );
inline void _FuncarchiveTickserializeTick8013964856239694639_a8ff9e2247269964 ( Context * __context__, archive::Archive & __arch_rename_at_146_144, decs::CTypeInfo & __value_rename_at_146_145 );
inline Sequence DAS_COMMENT((char *)) _FuncbuiltinTickkeysTick2205854368403803976_23a113e85e5887ab ( Context * __context__, TTable<char *,decs::CTypeInfo> const  & __a_rename_at_1128_157 );
inline Sequence DAS_COMMENT((decs::CTypeInfo &)) _FuncbuiltinTickvaluesTick1351216622833168869_61c0f20a0f69d8bd ( Context * __context__, TTable<char *,decs::CTypeInfo> & __a_rename_at_1150_159 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_d65f5162f71fde6a ( Context * __context__, TArray<decs::EcsRequest> & __Arr_rename_at_68_161, int32_t __newSize_rename_at_68_162 );
inline void _FuncarchiveTickserializeTick8013964856239694639_95732ecdb0114b69 ( Context * __context__, archive::Archive & __arch_rename_at_146_163, decs::EcsRequest & __value_rename_at_146_164 );
inline void _FuncbuiltinTickfinalizeTick5454204887383796109_7b1c641088884a8c ( Context * __context__, TTable<uint64_t,int32_t> & __a_rename_at_1193_170 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_ea18cb8662f307c1 ( Context * __context__, TArray<decs::Archetype> & __a_rename_at_1182_171 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_5fbae9e7389a3856 ( Context * __context__, TArray<decs::EntityId> & __a_rename_at_1182_173 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_ba685a3ffd2b9e77 ( Context * __context__, TArray<AutoTuple<int32_t,uint64_t,int32_t>> & __a_rename_at_1182_175 );
inline void _FuncbuiltinTickfinalizeTick5454204887383796109_1f7f0c3546fa051e ( Context * __context__, TTable<char *,decs::CTypeInfo> & __a_rename_at_1193_177 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_9a747408a2a6d0ff ( Context * __context__, TArray<decs::EcsRequest> & __a_rename_at_1182_179 );
inline void _FuncarchiveTickserializeTick18117489833716462019_10e000d138e4ac0f ( Context * __context__, archive::Archive & __arch_rename_at_209_181, TTable<uint64_t,int32_t> & __value_rename_at_209_182 );
inline void _FuncarchiveTickserializeTick1002677114065211290_339f3460927c659a ( Context * __context__, archive::Archive & __arch_rename_at_189_192, TArray<decs::Archetype> & __value_rename_at_189_193 );
inline void _FuncarchiveTickserializeTick1002677114065211290_2ebd95325aaa7a86 ( Context * __context__, archive::Archive & __arch_rename_at_189_198, TArray<decs::EntityId> & __value_rename_at_189_199 );
inline void _FuncarchiveTickserializeTick1002677114065211290_7eba8f9beff4a0ef ( Context * __context__, archive::Archive & __arch_rename_at_189_204, TArray<AutoTuple<int32_t,uint64_t,int32_t>> & __value_rename_at_189_205 );
inline void _FuncarchiveTickserializeTick18117489833716462019_82336b17b91a4160 ( Context * __context__, archive::Archive & __arch_rename_at_209_210, TTable<char *,decs::CTypeInfo> & __value_rename_at_209_211 );
inline void _FuncarchiveTickserializeTick1002677114065211290_94274d93de23df5 ( Context * __context__, archive::Archive & __arch_rename_at_189_221, TArray<decs::EcsRequest> & __value_rename_at_189_222 );
inline TArray<uint8_t> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_5aac315099132c4e ( Context * __context__, TArray<uint8_t> & __a_rename_at_50_227 );
inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_f314c103a27e7f0c ( Context * __context__, TArray<uint8_t> & __a_rename_at_32_228, TArray<uint8_t> & __b_rename_at_32_229 );
inline void finalize_81b0d19e466110c8 ( Context * __context__, decs::DecsState & ____this_rename_at_102_230 );
inline void _FuncapplyTickstructTickDecsStateTick0x96Tick0Tick7_84c01a5244f15f9e ( Context * __context__, decs::DecsState & ___Var_Tick_self_rename_at_62_231, Block DAS_COMMENT((void,TTable<uint64_t,int32_t>)) const  & ____arg_archetypeLookup_rename_at_62_232, Block DAS_COMMENT((void,TArray<decs::Archetype>)) const  & ____arg_allArchetypes_rename_at_62_233, Block DAS_COMMENT((void,TArray<decs::EntityId>)) const  & ____arg_entityFreeList_rename_at_62_234, Block DAS_COMMENT((void,TArray<AutoTuple<int32_t,uint64_t,int32_t>>)) const  & ____arg_entityLookup_rename_at_62_235, Block DAS_COMMENT((void,TTable<char *,decs::CTypeInfo>)) const  & ____arg_componentTypeCheck_rename_at_62_236, Block DAS_COMMENT((void,TArray<decs::EcsRequest>)) const  & ____arg_ecsQueries_rename_at_62_237, Block DAS_COMMENT((void,TTable<uint64_t,int32_t>)) const  & ____arg_queryLookup_rename_at_62_238 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_2333bfdfd8018176 ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_68_239, int32_t __newSize_rename_at_68_240 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_1b59bf517600dbc5 ( Context * __context__, TTable<char *,das::Time> const  & __Tab_rename_at_1014_241, char * const  __at_rename_at_1014_242 );
inline das::Time _FuncbuiltinTickget_valueTick6803070933163225147_6f53cf5131c26ea6 ( Context * __context__, TTable<char *,das::Time> & __Tab_rename_at_741_243, char * const  __at_rename_at_741_244 );
inline void _FuncbuiltinTickinsertTick4246857231018487965_53ce123b607bdf56 ( Context * __context__, TTable<char *,das::Time> & __Tab_rename_at_939_245, char * const  __at_rename_at_939_246, das::Time __val_rename_at_939_247 );
inline void _FuncarchiveTickserializeTick8013964856239694639_a9d1c2f2f9d21c11 ( Context * __context__, archive::Archive & __arch_rename_at_146_248, decs::DecsState & __value_rename_at_146_249 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_efda835c0359ec76 ( Context * __context__, TTable<char *,bool> const  & __Tab_rename_at_1014_257, char * const  __at_rename_at_1014_258 );
inline void _FuncbuiltinTickinsertTick12964066441666329206_963edeee5917200b ( Context * __context__, TTable<char *,bool> & __Tab_rename_at_947_259, char * const  __at_rename_at_947_260, bool __val_rename_at_947_261 );
inline bool _FuncbuiltinTickget_valueTick6803070933163225147_c79cd68da0d29c45 ( Context * __context__, TTable<char *,bool> & __Tab_rename_at_741_262, char * const  __at_rename_at_741_263 );
inline void finalize_801ffbcd37fa63c8 ( Context * __context__, live::_lambda_live_205_1 & ____this_rename_at_205_264 );
inline void _FuncbuiltinTickcloneTick3038771811667655495_1107976c81aa121a ( Context * __context__, TArray<uint8_t> & __a_rename_at_1080_265, TArray<uint8_t> const  & __b_rename_at_1080_266 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb ( Context * __context__, TArray<uint8_t> & __a_rename_at_1182_268 );
inline void finalize_3e056b096d7418b6 ( Context * __context__, live::_lambda_live_225_2 & ____this_rename_at_225_269 );
inline void finalize_82b0f923cb1a5c28 ( Context * __context__, live::_lambda_live_237_3 & ____this_rename_at_237_270 );
inline uint64_t _FuncbuiltinTickintptrTick11320822648609276562_c1cab2e197c26e65 ( Context * __context__, void * const  __p_rename_at_15_271 );
inline void _FuncbuiltinTickcloneTick3038771811667655495_74d08f53d44ae194 ( Context * __context__, TArray<uint8_t> & __a_rename_at_1080_272, TArray<uint8_t> const  & __b_rename_at_1080_273 );
inline void _FuncbuiltinTickfinalizeTick5454204887383796109_9c4083566563a85a ( Context * __context__, TTable<char *,bool> & __a_rename_at_1193_275 );
inline TArray<uint8_t> _FuncarchiveTickmem_archive_saveTick1217508036218320246_8093c94b06c5b1d3 ( Context * __context__, decs::DecsState & __t_rename_at_261_276 );
inline bool _FuncarchiveTickmem_archive_loadTick9860235419630369707_520cdbc5db538a78 ( Context * __context__, TArray<uint8_t> & __data_rename_at_269_279, decs::DecsState & __t_rename_at_269_280, bool __canfail_rename_at_269_281 );
inline void clone_bccc36eddc052dcb ( Context * __context__, smart_ptr_raw<Context> & __dest_rename_at_61_284, void * const  __src_rename_at_61_285 );
inline void clone_5a60439f7fb04300 ( Context * __context__, smart_ptr_raw<Context> & __dest_rename_at_78_286, smart_ptr_raw<Context> const  __src_rename_at_78_287 );
inline bool _FuncliveTicklive_lookupTick637029202016062135_11576884b544c838 ( Context * __context__, char * const  __fn_rename_at_92_288 );
inline void _FuncfioTickdirTick17131698203006957834_186fcc74e85e93aa ( Context * __context__, char * const  __path_rename_at_21_289, Block DAS_COMMENT((void,char * const )) const  & __blk_rename_at_21_290 );
inline Context * _FuncbuiltinTickget_ptrTick5807679485210906136_91295c6251ff26c5 ( Context * __context__, smart_ptr_raw<Context> __src_rename_at_1732_291 );
inline void _Func_lambda_live_205_1Tickfunction_b3b7a30c2cb432a6 ( Context * __context__, live::_lambda_live_205_1 & ____this_rename_at_205_292 );
inline void _Func_lambda_live_205_1Tickfinalizer_88431e65ddbac842 ( Context * __context__, live::_lambda_live_205_1 * ____this_rename_at_205_293 );
inline void _Func_lambda_live_225_2Tickfunction_2c0f2ecd65895fe4 ( Context * __context__, live::_lambda_live_225_2 & ____this_rename_at_225_294, TArray<uint8_t> const  & __data_rename_at_225_295 );
inline void _Func_lambda_live_225_2Tickfinalizer_4b5c91408d22a4a6 ( Context * __context__, live::_lambda_live_225_2 * ____this_rename_at_225_296 );
inline void _Func_lambda_live_237_3Tickfunction_1dd03a040db0eba6 ( Context * __context__, live::_lambda_live_237_3 & ____this_rename_at_237_297, decs::DecsState * const  __state_rename_at_237_298 );
inline void _Func_lambda_live_237_3Tickfinalizer_7c78462166aa88ea ( Context * __context__, live::_lambda_live_237_3 * ____this_rename_at_237_299 );
inline void _FuncliveTickinvoke_liveTick12947590458897769171_8b2be572fbf2e075 ( Context * __context__, char * const  __fn_rename_at_115_300, bool __a1_rename_at_115_301 );
inline void _FuncliveTickinvoke_liveTick2886249264218566728_c3b9177bd0f8a8bc ( Context * __context__, char * const  __fn_rename_at_154_302, int32_t __a1_rename_at_154_303, int32_t __a2_rename_at_154_304, int32_t __a3_rename_at_154_305, int32_t __a4_rename_at_154_306 );
inline void _FuncliveTickinvoke_liveTick12947590458897769171_8571e9b30efbd8d7 ( Context * __context__, char * const  __fn_rename_at_115_307, uint32_t __a1_rename_at_115_308 );
inline void _FuncliveTickinvoke_liveTick8381256433725501221_ebcbd4eac3367a58 ( Context * __context__, char * const  __fn_rename_at_128_309, double __a1_rename_at_128_310, double __a2_rename_at_128_311 );
inline void _FuncliveTickinvoke_liveTick16287916205268058194_beee970120602d17 ( Context * __context__, char * const  __fn_rename_at_141_312, int32_t __a1_rename_at_141_313, int32_t __a2_rename_at_141_314, int32_t __a3_rename_at_141_315 );
inline void _FuncliveTickinvoke_liveTick12947590458897769171_1753e685298c5c2a ( Context * __context__, char * const  __fn_rename_at_115_316, float __a1_rename_at_115_317 );
inline smart_ptr_raw<Context> get_live_context_e44470f3748ec318 ( Context * __context__ );
inline void saveLiveContext_efcae6d209280047 ( Context * __context__ );
inline void restoreLiveContext_49c6d9153d13659f ( Context * __context__ );
inline void go_offline_778dbe0dd3b703b8 ( Context * __context__ );
inline bool is_live_92bab00e5c617532 ( Context * __context__ );
inline bool is_app_live_b15dd0f7bf4a041d ( Context * __context__ );
inline void go_main_b78776d702f7ae9b ( Context * __context__ );
inline void go_live_b8a103eaa74d382f ( Context * __context__, char * const  __appf_rename_at_82_319, char * const  __appd_rename_at_82_320 );
inline void invoke_live_94051986a48f719e ( Context * __context__, char * const  __fn_rename_at_102_321 );
inline bool watch_files_cfe33bf10ac8df9b ( Context * __context__ );
inline void set_new_context_a7a4593ab51c9d0c ( Context * __context__, smart_ptr_raw<Context> __ptr_rename_at_195_327, bool __full_restart_rename_at_195_328 );
inline void recompile_18a8bac272d4f62d ( Context * __context__, bool __full_restart_rename_at_250_338 );
inline void live_keypressed_c93934ab7b43c5fc ( Context * __context__, int32_t __keycode_rename_at_277_348, int32_t __scancode_rename_at_277_349, int32_t __action_rename_at_277_350, int32_t __mods_rename_at_277_351 );
inline void live_charpressed_7ed6cbfc39a60863 ( Context * __context__, uint32_t __charcode_rename_at_286_352 );
inline void live_mousemoved_30e11d11222838c6 ( Context * __context__, double __x_rename_at_290_353, double __y_rename_at_290_354 );
inline void live_mouseenter_3eb091e967328196 ( Context * __context__, bool __yes_rename_at_294_355 );
inline void live_mousebutton_65494a8acfebcfbd ( Context * __context__, int32_t __button_rename_at_298_356, int32_t __action_rename_at_298_357, int32_t __mods_rename_at_298_358 );
inline void live_mousescroll_3abccc465cea7be8 ( Context * __context__, double __x_rename_at_302_359, double __y_rename_at_302_360 );
inline void live_update_48a547de881c9a0a ( Context * __context__, double __appTime_rename_at_306_361 );

void __init_script ( Context * __context__, bool __init_shared )
{
    das_global_zero<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__);/*appPtr*/
    das_global<char *,0x86e5ae912531f3a1>(__context__) = ((char *) "app.das");/*appFile*/
    das_global<char *,0xb0f39eaf391fc167>(__context__) = (cast<char *>::to(SimPolicy<char *>::Add(cast<char *>::from(((char * const )(builtin_das_root(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))),cast<char *>::from(((char *) "/modules/dasGlfw/framework")),*__context__,nullptr)));/*appDir*/
    das_global<bool,0x67076871139e3abe>(__context__) = false;/*appLive*/
    das_global_zero<TTable<char *,das::Time>,0xfca7703c86890e7>(__context__);/*appTime*/
    das_global<das::Time,0xe2c0ab7b4123c2d9>(__context__) = /*c-tor*/ builtin_clock(/*end-c-tor*/);/*watchTime*/
    das_global_zero<TTable<char *,bool>,0x7955fcc436037958>(__context__);/*liveFunctionLookup*/
    das_global_zero<TArray<uint8_t>,0x4b711ceb6d62f997>(__context__);/*decsLiveData*/
    das_global_zero<smart_ptr_raw<Context>,0xec8e0452d46569f1>(__context__);/*liveContext*/
    das_global<double,0x81fa513f37cd5e>(__context__) = 0.000000e+00;/*`static`at_line_307`t*/
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_559ca47f392751ff ( Context * __context__, TArray<decs::Component> &  __Arr_rename_at_68_0, int32_t __newSize_rename_at_68_1 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_0,cast<TArray<decs::Component> &>::from(__Arr_rename_at_68_0)));
    builtin_array_resize(das_arg<TArray<decs::Component>>::pass(__Arr_rename_at_68_0),__newSize_rename_at_68_1,144,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncarchiveTickserialize_rawTick346513482259279339_2cb15e82f6a052bb ( Context * __context__, archive::Archive &  __arch_rename_at_90_2, uint32_t & __value_rename_at_90_3 )
{
    if ( __arch_rename_at_90_2.reading )
    {
        das_invoke_method<bool,offsetof(archive::Serializer,read)>::invoke<archive::Serializer &,uint32_t *,int32_t>(__context__,nullptr,das_arg<archive::Serializer>::pass(das_cast<archive::Serializer>::cast(das_deref(__context__,__arch_rename_at_90_2.stream))),das_ref(__context__,__value_rename_at_90_3),4);
    } else {
        das_invoke_method<bool,offsetof(archive::Serializer,write)>::invoke<archive::Serializer &,uint32_t *,int32_t>(__context__,nullptr,das_arg<archive::Serializer>::pass(das_cast<archive::Serializer>::cast(das_deref(__context__,__arch_rename_at_90_2.stream))),das_ref(__context__,__value_rename_at_90_3),4);
    };
}

inline void _FuncarchiveTickserialize_rawTick346513482259279339_877086b5c2b9e6d8 ( Context * __context__, archive::Archive &  __arch_rename_at_90_4, DAS_COMMENT(bound_enum) das::Type & __value_rename_at_90_5 )
{
    if ( __arch_rename_at_90_4.reading )
    {
        das_invoke_method<bool,offsetof(archive::Serializer,read)>::invoke<archive::Serializer &,DAS_COMMENT(bound_enum) das::Type *,int32_t>(__context__,nullptr,das_arg<archive::Serializer>::pass(das_cast<archive::Serializer>::cast(das_deref(__context__,__arch_rename_at_90_4.stream))),das_ref(__context__,__value_rename_at_90_5),4);
    } else {
        das_invoke_method<bool,offsetof(archive::Serializer,write)>::invoke<archive::Serializer &,DAS_COMMENT(bound_enum) das::Type *,int32_t>(__context__,nullptr,das_arg<archive::Serializer>::pass(das_cast<archive::Serializer>::cast(das_deref(__context__,__arch_rename_at_90_4.stream))),das_ref(__context__,__value_rename_at_90_5),4);
    };
}

inline void _FuncarchiveTickread_rawTick6802855309095275289_59f27d4ca7f63255 ( Context * __context__, archive::Archive &  __arch_rename_at_99_6, uint64_t & __value_rename_at_99_7 )
{
    DAS_ASSERT((__arch_rename_at_99_6.reading));
    das_invoke_method<bool,offsetof(archive::Serializer,read)>::invoke<archive::Serializer &,uint64_t *,int32_t>(__context__,nullptr,das_arg<archive::Serializer>::pass(das_cast<archive::Serializer>::cast(das_deref(__context__,__arch_rename_at_99_6.stream))),das_ref(__context__,__value_rename_at_99_7),8);
}

inline void _FuncarchiveTickwrite_rawTick9760319972816126958_7e827e6b1de76fcd ( Context * __context__, archive::Archive &  __arch_rename_at_105_8, uint64_t & __value_rename_at_105_9 )
{
    DAS_ASSERT((!__arch_rename_at_105_8.reading));
    das_invoke_method<bool,offsetof(archive::Serializer,write)>::invoke<archive::Serializer &,uint64_t *,int32_t>(__context__,nullptr,das_arg<archive::Serializer>::pass(das_cast<archive::Serializer>::cast(das_deref(__context__,__arch_rename_at_105_8.stream))),das_ref(__context__,__value_rename_at_105_9),8);
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_14682004b38ad2a1 ( Context * __context__, TArray<char *> &  __Arr_rename_at_68_10, int32_t __newSize_rename_at_68_11 )
{
    builtin_array_resize(das_arg<TArray<char *>>::pass(__Arr_rename_at_68_10),__newSize_rename_at_68_11,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_1bdb9995fa062a51 ( Context * __context__, TArray<int32_t> &  __Arr_rename_at_68_12, int32_t __newSize_rename_at_68_13 )
{
    builtin_array_resize(das_arg<TArray<int32_t>>::pass(__Arr_rename_at_68_12),__newSize_rename_at_68_13,4,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncapplyTickstructTickEcsRequestPosTick0x96Tick0Tick2_cf2646de905784ac ( Context * __context__, decs::EcsRequestPos &  ___Var_Tick_self_rename_at_62_14, Block DAS_COMMENT((void,char * &)) const  &  ____arg_file_rename_at_62_15, Block DAS_COMMENT((void,uint32_t &)) const  &  ____arg_line_rename_at_62_16 )
{
    das_invoke<void>::invoke<char * &>(__context__,nullptr,____arg_file_rename_at_62_15,___Var_Tick_self_rename_at_62_14.file);
    das_invoke<void>::invoke<uint32_t &>(__context__,nullptr,____arg_line_rename_at_62_16,___Var_Tick_self_rename_at_62_14.line);
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_345f02cc82623620 ( Context * __context__, TArray<decs::Component> &  __a_rename_at_1182_17 )
{
    {
        bool __need_loop_1184 = true;
        // aV: decs::Component aka TT&
        das_iterator<TArray<decs::Component>> __aV_iterator(__a_rename_at_1182_17);
        decs::Component * __aV_rename_at_1184_18;
        __need_loop_1184 = __aV_iterator.first(__context__,(__aV_rename_at_1184_18)) && __need_loop_1184;
        for ( ; __need_loop_1184 ; __need_loop_1184 = __aV_iterator.next(__context__,(__aV_rename_at_1184_18)) )
        {
            das_invoke_function<void>::invoke<decs::Component &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@decs::finalize S<decs::Component>*/ 0x8c569da32e1a59bb)),das_arg<decs::Component>::pass((*__aV_rename_at_1184_18)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1184_18));
    };
    builtin_array_free(das_arg<TArray<decs::Component>>::pass(__a_rename_at_1182_17),144,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_79f9e841b4434fd8 ( Context * __context__, TArray<char *> &  __a_rename_at_1182_19 )
{
    builtin_array_free(das_arg<TArray<char *>>::pass(__a_rename_at_1182_19),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_c2c15811c6d720e8 ( Context * __context__, TArray<int32_t> &  __a_rename_at_1182_20 )
{
    builtin_array_free(das_arg<TArray<int32_t>>::pass(__a_rename_at_1182_20),4,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncarchiveTickserializeTick1002677114065211290_838b04f3311f9ee8 ( Context * __context__, archive::Archive &  __arch_rename_at_189_21, TArray<decs::Component> &  __value_rename_at_189_22 )
{
    if ( __arch_rename_at_189_21.reading )
    {
        int32_t __len_rename_at_191_23 = 0;
        _FuncbuiltinTickfinalizeTick13836114024949725080_345f02cc82623620(__context__,das_arg<TArray<decs::Component>>::pass(__value_rename_at_189_22));
        _FuncarchiveTickread_rawTick6802855309095275289_b9dfe21dac02b654(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_189_21),__len_rename_at_191_23);
        _FuncbuiltinTickresizeTick4811697762258667383_559ca47f392751ff(__context__,das_arg<TArray<decs::Component>>::pass(__value_rename_at_189_22),__len_rename_at_191_23);
        {
            bool __need_loop_197 = true;
            // element: decs::Component aka TT&
            das_iterator<TArray<decs::Component>> __element_iterator(__value_rename_at_189_22);
            decs::Component * __element_rename_at_197_24;
            __need_loop_197 = __element_iterator.first(__context__,(__element_rename_at_197_24)) && __need_loop_197;
            for ( ; __need_loop_197 ; __need_loop_197 = __element_iterator.next(__context__,(__element_rename_at_197_24)) )
            {
                das_invoke_function<void>::invoke<archive::Archive &,decs::Component &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@decs::serialize S<archive::Archive> S<decs::Component>*/ 0x76bf5795e0a03d42)),das_arg<archive::Archive>::pass(__arch_rename_at_189_21),das_arg<decs::Component>::pass((*__element_rename_at_197_24)));
            }
            __element_iterator.close(__context__,(__element_rename_at_197_24));
        };
    } else {
        int32_t __len_rename_at_201_25 = builtin_array_size(das_arg<TArray<decs::Component>>::pass(__value_rename_at_189_22));
        _FuncarchiveTickwrite_rawTick9760319972816126958_e9195ce9edbb9e44(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_189_21),__len_rename_at_201_25);
        {
            bool __need_loop_203 = true;
            // element: decs::Component aka TT&
            das_iterator<TArray<decs::Component>> __element_iterator(__value_rename_at_189_22);
            decs::Component * __element_rename_at_203_26;
            __need_loop_203 = __element_iterator.first(__context__,(__element_rename_at_203_26)) && __need_loop_203;
            for ( ; __need_loop_203 ; __need_loop_203 = __element_iterator.next(__context__,(__element_rename_at_203_26)) )
            {
                das_invoke_function<void>::invoke<archive::Archive &,decs::Component &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@decs::serialize S<archive::Archive> S<decs::Component>*/ 0x76bf5795e0a03d42)),das_arg<archive::Archive>::pass(__arch_rename_at_189_21),das_arg<decs::Component>::pass((*__element_rename_at_203_26)));
            }
            __element_iterator.close(__context__,(__element_rename_at_203_26));
        };
    };
}

inline void _FuncarchiveTickserializeTick6962985013043226618_c51e37371f3d267a ( Context * __context__, archive::Archive &  __arch_rename_at_112_27, uint32_t & __value_rename_at_112_28 )
{
    _FuncarchiveTickserialize_rawTick346513482259279339_2cb15e82f6a052bb(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_112_27),__value_rename_at_112_28);
}

inline void _FuncarchiveTickserializeTick6962985013043226618_ed7dae2a007ca602 ( Context * __context__, archive::Archive &  __arch_rename_at_112_29, DAS_COMMENT(bound_enum) das::Type & __value_rename_at_112_30 )
{
    _FuncarchiveTickserialize_rawTick346513482259279339_877086b5c2b9e6d8(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_112_29),__value_rename_at_112_30);
}

inline void _FuncarchiveTickserializeTick4429977757536996025_2b74a39202406253 ( Context * __context__, archive::Archive &  __arch_rename_at_118_31, Func DAS_COMMENT((void,TArray<uint8_t>)) & __value_rename_at_118_32 )
{
    if ( __arch_rename_at_118_31.reading )
    {
        uint64_t __mnh_rename_at_120_33 = 0;
        _FuncarchiveTickread_rawTick6802855309095275289_59f27d4ca7f63255(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_118_31),__mnh_rename_at_120_33);
        if ( __mnh_rename_at_120_33 != UINT64_C(0x0) )
        {
            das_copy(__value_rename_at_118_32,das_cast<Func DAS_COMMENT((void,TArray<uint8_t>)) &>::cast(builtin_getFunctionByMnh(__mnh_rename_at_120_33,__context__)));
        };
    } else {
        uint64_t __mnh_rename_at_128_34 = builtin_getFunctionMnh(__value_rename_at_118_32,__context__);
        _FuncarchiveTickwrite_rawTick9760319972816126958_7e827e6b1de76fcd(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_118_31),__mnh_rename_at_128_34);
    };
}

inline void _FuncarchiveTickserializeTick4429977757536996025_1ff57546f6343a0a ( Context * __context__, archive::Archive &  __arch_rename_at_118_35, Func DAS_COMMENT((void,TArray<uint8_t>,TArray<uint8_t> const )) & __value_rename_at_118_36 )
{
    if ( __arch_rename_at_118_35.reading )
    {
        uint64_t __mnh_rename_at_120_37 = 0;
        _FuncarchiveTickread_rawTick6802855309095275289_59f27d4ca7f63255(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_118_35),__mnh_rename_at_120_37);
        if ( __mnh_rename_at_120_37 != UINT64_C(0x0) )
        {
            das_copy(__value_rename_at_118_36,das_cast<Func DAS_COMMENT((void,TArray<uint8_t>,TArray<uint8_t> const )) &>::cast(builtin_getFunctionByMnh(__mnh_rename_at_120_37,__context__)));
        };
    } else {
        uint64_t __mnh_rename_at_128_38 = builtin_getFunctionMnh(__value_rename_at_118_36,__context__);
        _FuncarchiveTickwrite_rawTick9760319972816126958_7e827e6b1de76fcd(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_118_35),__mnh_rename_at_128_38);
    };
}

inline void _FuncarchiveTickserializeTick4429977757536996025_beb22678ca166fa4 ( Context * __context__, archive::Archive &  __arch_rename_at_118_39, Func DAS_COMMENT((void,archive::Archive,TArray<uint8_t>,char * const )) & __value_rename_at_118_40 )
{
    if ( __arch_rename_at_118_39.reading )
    {
        uint64_t __mnh_rename_at_120_41 = 0;
        _FuncarchiveTickread_rawTick6802855309095275289_59f27d4ca7f63255(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_118_39),__mnh_rename_at_120_41);
        if ( __mnh_rename_at_120_41 != UINT64_C(0x0) )
        {
            das_copy(__value_rename_at_118_40,das_cast<Func DAS_COMMENT((void,archive::Archive,TArray<uint8_t>,char * const )) &>::cast(builtin_getFunctionByMnh(__mnh_rename_at_120_41,__context__)));
        };
    } else {
        uint64_t __mnh_rename_at_128_42 = builtin_getFunctionMnh(__value_rename_at_118_40,__context__);
        _FuncarchiveTickwrite_rawTick9760319972816126958_7e827e6b1de76fcd(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_118_39),__mnh_rename_at_128_42);
    };
}

inline void _FuncarchiveTickserializeTick4429977757536996025_3b53a58effc6756a ( Context * __context__, archive::Archive &  __arch_rename_at_118_43, Func DAS_COMMENT((char *,void * const )) & __value_rename_at_118_44 )
{
    if ( __arch_rename_at_118_43.reading )
    {
        uint64_t __mnh_rename_at_120_45 = 0;
        _FuncarchiveTickread_rawTick6802855309095275289_59f27d4ca7f63255(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_118_43),__mnh_rename_at_120_45);
        if ( __mnh_rename_at_120_45 != UINT64_C(0x0) )
        {
            das_copy(__value_rename_at_118_44,das_cast<Func DAS_COMMENT((char *,void * const )) &>::cast(builtin_getFunctionByMnh(__mnh_rename_at_120_45,__context__)));
        };
    } else {
        uint64_t __mnh_rename_at_128_46 = builtin_getFunctionMnh(__value_rename_at_118_44,__context__);
        _FuncarchiveTickwrite_rawTick9760319972816126958_7e827e6b1de76fcd(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_118_43),__mnh_rename_at_128_46);
    };
}

inline void _FuncarchiveTickserializeTick4429977757536996025_190cc1e99493f13c ( Context * __context__, archive::Archive &  __arch_rename_at_118_47, Func DAS_COMMENT((TypeInfo const  *)) & __value_rename_at_118_48 )
{
    if ( __arch_rename_at_118_47.reading )
    {
        uint64_t __mnh_rename_at_120_49 = 0;
        _FuncarchiveTickread_rawTick6802855309095275289_59f27d4ca7f63255(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_118_47),__mnh_rename_at_120_49);
        if ( __mnh_rename_at_120_49 != UINT64_C(0x0) )
        {
            das_copy(__value_rename_at_118_48,das_cast<Func DAS_COMMENT((TypeInfo const  *)) &>::cast(builtin_getFunctionByMnh(__mnh_rename_at_120_49,__context__)));
        };
    } else {
        uint64_t __mnh_rename_at_128_50 = builtin_getFunctionMnh(__value_rename_at_118_48,__context__);
        _FuncarchiveTickwrite_rawTick9760319972816126958_7e827e6b1de76fcd(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_118_47),__mnh_rename_at_128_50);
    };
}

inline void _FuncarchiveTickserializeTick4429977757536996025_25683f2273767516 ( Context * __context__, archive::Archive &  __arch_rename_at_118_51, Func DAS_COMMENT((Lambda DAS_COMMENT((void)),TArray<uint8_t>)) & __value_rename_at_118_52 )
{
    if ( __arch_rename_at_118_51.reading )
    {
        uint64_t __mnh_rename_at_120_53 = 0;
        _FuncarchiveTickread_rawTick6802855309095275289_59f27d4ca7f63255(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_118_51),__mnh_rename_at_120_53);
        if ( __mnh_rename_at_120_53 != UINT64_C(0x0) )
        {
            das_copy(__value_rename_at_118_52,das_cast<Func DAS_COMMENT((Lambda DAS_COMMENT((void)),TArray<uint8_t>)) &>::cast(builtin_getFunctionByMnh(__mnh_rename_at_120_53,__context__)));
        };
    } else {
        uint64_t __mnh_rename_at_128_54 = builtin_getFunctionMnh(__value_rename_at_118_52,__context__);
        _FuncarchiveTickwrite_rawTick9760319972816126958_7e827e6b1de76fcd(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_118_51),__mnh_rename_at_128_54);
    };
}

inline void _FuncarchiveTickserializeTick1002677114065211290_bef6bf5e85c51d3b ( Context * __context__, archive::Archive &  __arch_rename_at_189_55, TArray<char *> &  __value_rename_at_189_56 )
{
    if ( __arch_rename_at_189_55.reading )
    {
        int32_t __len_rename_at_191_57 = 0;
        _FuncbuiltinTickfinalizeTick13836114024949725080_79f9e841b4434fd8(__context__,das_arg<TArray<char *>>::pass(__value_rename_at_189_56));
        _FuncarchiveTickread_rawTick6802855309095275289_b9dfe21dac02b654(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_189_55),__len_rename_at_191_57);
        _FuncbuiltinTickresizeTick4811697762258667383_14682004b38ad2a1(__context__,das_arg<TArray<char *>>::pass(__value_rename_at_189_56),__len_rename_at_191_57);
        {
            bool __need_loop_197 = true;
            // element: string aka TT&
            das_iterator<TArray<char *>> __element_iterator(__value_rename_at_189_56);
            char * * __element_rename_at_197_58;
            __need_loop_197 = __element_iterator.first(__context__,(__element_rename_at_197_58)) && __need_loop_197;
            for ( ; __need_loop_197 ; __need_loop_197 = __element_iterator.next(__context__,(__element_rename_at_197_58)) )
            {
                das_invoke_function<void>::invoke<archive::Archive &,char * &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@archive::serialize S<archive::Archive> &s*/ 0x27563976f74188a2)),das_arg<archive::Archive>::pass(__arch_rename_at_189_55),(*__element_rename_at_197_58));
            }
            __element_iterator.close(__context__,(__element_rename_at_197_58));
        };
    } else {
        int32_t __len_rename_at_201_59 = builtin_array_size(das_arg<TArray<char *>>::pass(__value_rename_at_189_56));
        _FuncarchiveTickwrite_rawTick9760319972816126958_e9195ce9edbb9e44(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_189_55),__len_rename_at_201_59);
        {
            bool __need_loop_203 = true;
            // element: string aka TT&
            das_iterator<TArray<char *>> __element_iterator(__value_rename_at_189_56);
            char * * __element_rename_at_203_60;
            __need_loop_203 = __element_iterator.first(__context__,(__element_rename_at_203_60)) && __need_loop_203;
            for ( ; __need_loop_203 ; __need_loop_203 = __element_iterator.next(__context__,(__element_rename_at_203_60)) )
            {
                das_invoke_function<void>::invoke<archive::Archive &,char * &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@archive::serialize S<archive::Archive> &s*/ 0x27563976f74188a2)),das_arg<archive::Archive>::pass(__arch_rename_at_189_55),(*__element_rename_at_203_60));
            }
            __element_iterator.close(__context__,(__element_rename_at_203_60));
        };
    };
}

inline void _FuncarchiveTickserializeTick1002677114065211290_591dc6f784de6b57 ( Context * __context__, archive::Archive &  __arch_rename_at_189_61, TArray<int32_t> &  __value_rename_at_189_62 )
{
    if ( __arch_rename_at_189_61.reading )
    {
        int32_t __len_rename_at_191_63 = 0;
        _FuncbuiltinTickfinalizeTick13836114024949725080_c2c15811c6d720e8(__context__,das_arg<TArray<int32_t>>::pass(__value_rename_at_189_62));
        _FuncarchiveTickread_rawTick6802855309095275289_b9dfe21dac02b654(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_189_61),__len_rename_at_191_63);
        _FuncbuiltinTickresizeTick4811697762258667383_1bdb9995fa062a51(__context__,das_arg<TArray<int32_t>>::pass(__value_rename_at_189_62),__len_rename_at_191_63);
        {
            bool __need_loop_197 = true;
            // element: int aka TT&
            das_iterator<TArray<int32_t>> __element_iterator(__value_rename_at_189_62);
            int32_t * __element_rename_at_197_64;
            __need_loop_197 = __element_iterator.first(__context__,(__element_rename_at_197_64)) && __need_loop_197;
            for ( ; __need_loop_197 ; __need_loop_197 = __element_iterator.next(__context__,(__element_rename_at_197_64)) )
            {
                _FuncarchiveTickserializeTick6962985013043226618_8e0595ec66b9d4fd(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_189_61),(*__element_rename_at_197_64));
            }
            __element_iterator.close(__context__,(__element_rename_at_197_64));
        };
    } else {
        int32_t __len_rename_at_201_65 = builtin_array_size(das_arg<TArray<int32_t>>::pass(__value_rename_at_189_62));
        _FuncarchiveTickwrite_rawTick9760319972816126958_e9195ce9edbb9e44(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_189_61),__len_rename_at_201_65);
        {
            bool __need_loop_203 = true;
            // element: int aka TT&
            das_iterator<TArray<int32_t>> __element_iterator(__value_rename_at_189_62);
            int32_t * __element_rename_at_203_66;
            __need_loop_203 = __element_iterator.first(__context__,(__element_rename_at_203_66)) && __need_loop_203;
            for ( ; __need_loop_203 ; __need_loop_203 = __element_iterator.next(__context__,(__element_rename_at_203_66)) )
            {
                _FuncarchiveTickserializeTick6962985013043226618_8e0595ec66b9d4fd(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_189_61),(*__element_rename_at_203_66));
            }
            __element_iterator.close(__context__,(__element_rename_at_203_66));
        };
    };
}

inline void _FuncarchiveTickserializeTick8013964856239694639_cac39795b36f7c56 ( Context * __context__, archive::Archive &  __arch_rename_at_146_67, decs::EcsRequestPos &  __value_rename_at_146_68 ) { das_stack_prologue __prologue(__context__,160,"archive`serialize`8013964856239694639 " DAS_FILE_LINE);
{
    if ( __arch_rename_at_146_67.reading )
    {
        finalize_9a888ca44a018013(__context__,das_arg<decs::EcsRequestPos>::pass(__value_rename_at_146_68));
    };
    _FuncapplyTickstructTickEcsRequestPosTick0x96Tick0Tick2_cf2646de905784ac(__context__,das_arg<decs::EcsRequestPos>::pass(__value_rename_at_146_68),das_make_block<void,char * &>(__context__,80,0,&__func_info__c85cb34a7aabfce2,[&](char * & __field_rename_at_150_69) -> void{
        das_invoke_function<void>::invoke<archive::Archive &,char * &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@archive::serialize S<archive::Archive> &s*/ 0x27563976f74188a2)),das_arg<archive::Archive>::pass(__arch_rename_at_146_67),__field_rename_at_150_69);
    }),das_make_block<void,uint32_t &>(__context__,144,0,&__func_info__e5601d0b9c7536bc,[&](uint32_t & __field_rename_at_150_70) -> void{
        _FuncarchiveTickserializeTick6962985013043226618_c51e37371f3d267a(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_146_67),__field_rename_at_150_70);
    }));
}}

inline void finalize_9a888ca44a018013 ( Context * __context__, decs::EcsRequestPos &  ____this_rename_at_81_71 )
{
    memset(&(____this_rename_at_81_71), 0, TypeSize<decs::EcsRequestPos>::size);
}

inline void _FuncarchiveTickserialize_rawTick346513482259279339_83ff526b8b4a3feb ( Context * __context__, archive::Archive &  __arch_rename_at_90_72, uint64_t & __value_rename_at_90_73 )
{
    if ( __arch_rename_at_90_72.reading )
    {
        das_invoke_method<bool,offsetof(archive::Serializer,read)>::invoke<archive::Serializer &,uint64_t *,int32_t>(__context__,nullptr,das_arg<archive::Serializer>::pass(das_cast<archive::Serializer>::cast(das_deref(__context__,__arch_rename_at_90_72.stream))),das_ref(__context__,__value_rename_at_90_73),8);
    } else {
        das_invoke_method<bool,offsetof(archive::Serializer,write)>::invoke<archive::Serializer &,uint64_t *,int32_t>(__context__,nullptr,das_arg<archive::Serializer>::pass(das_cast<archive::Serializer>::cast(das_deref(__context__,__arch_rename_at_90_72.stream))),das_ref(__context__,__value_rename_at_90_73),8);
    };
}

inline void _FuncarchiveTickserialize_rawTick346513482259279339_344032bd32dfc282 ( Context * __context__, archive::Archive &  __arch_rename_at_90_74, int32_t & __value_rename_at_90_75 )
{
    if ( __arch_rename_at_90_74.reading )
    {
        das_invoke_method<bool,offsetof(archive::Serializer,read)>::invoke<archive::Serializer &,int32_t *,int32_t>(__context__,nullptr,das_arg<archive::Serializer>::pass(das_cast<archive::Serializer>::cast(das_deref(__context__,__arch_rename_at_90_74.stream))),das_ref(__context__,__value_rename_at_90_75),4);
    } else {
        das_invoke_method<bool,offsetof(archive::Serializer,write)>::invoke<archive::Serializer &,int32_t *,int32_t>(__context__,nullptr,das_arg<archive::Serializer>::pass(das_cast<archive::Serializer>::cast(das_deref(__context__,__arch_rename_at_90_74.stream))),das_ref(__context__,__value_rename_at_90_75),4);
    };
}

inline void finalize_37451350471f63d7 ( Context * __context__, decs::Archetype &  ____this_rename_at_57_76 )
{
    _FuncbuiltinTickfinalizeTick13836114024949725080_345f02cc82623620(__context__,das_arg<TArray<decs::Component>>::pass(____this_rename_at_57_76.components));
    memset(&(____this_rename_at_57_76), 0, TypeSize<decs::Archetype>::size);
}

inline void _FuncapplyTickstructTickArchetypeTick0x96Tick0Tick4_86443a66f85f8e05 ( Context * __context__, decs::Archetype &  ___Var_Tick_self_rename_at_62_77, Block DAS_COMMENT((void,uint64_t &)) const  &  ____arg_hash_rename_at_62_78, Block DAS_COMMENT((void,TArray<decs::Component>)) const  &  ____arg_components_rename_at_62_79, Block DAS_COMMENT((void,int32_t &)) const  &  ____arg_size_rename_at_62_80, Block DAS_COMMENT((void,int32_t &)) const  &  ____arg_eidIndex_rename_at_62_81 )
{
    das_invoke<void>::invoke<uint64_t &>(__context__,nullptr,____arg_hash_rename_at_62_78,___Var_Tick_self_rename_at_62_77.hash);
    das_invoke<void>::invoke<TArray<decs::Component> &>(__context__,nullptr,____arg_components_rename_at_62_79,das_arg<TArray<decs::Component>>::pass(___Var_Tick_self_rename_at_62_77.components));
    das_invoke<void>::invoke<int32_t &>(__context__,nullptr,____arg_size_rename_at_62_80,___Var_Tick_self_rename_at_62_77.size);
    das_invoke<void>::invoke<int32_t &>(__context__,nullptr,____arg_eidIndex_rename_at_62_81,___Var_Tick_self_rename_at_62_77.eidIndex);
}

inline void finalize_aed1eae423063a73 ( Context * __context__, decs::EntityId &  ____this_rename_at_51_82 )
{
    memset(&(____this_rename_at_51_82), 0, TypeSize<decs::EntityId>::size);
}

inline void _FuncapplyTickstructTickEntityIdTick0x96Tick0Tick2_ceabfe323854d15e ( Context * __context__, decs::EntityId &  ___Var_Tick_self_rename_at_62_83, Block DAS_COMMENT((void,uint32_t &)) const  &  ____arg_id_rename_at_62_84, Block DAS_COMMENT((void,int32_t &)) const  &  ____arg_generation_rename_at_62_85 )
{
    das_invoke<void>::invoke<uint32_t &>(__context__,nullptr,____arg_id_rename_at_62_84,___Var_Tick_self_rename_at_62_83.id);
    das_invoke<void>::invoke<int32_t &>(__context__,nullptr,____arg_generation_rename_at_62_85,___Var_Tick_self_rename_at_62_83.generation);
}

inline void finalize_f87ddce7a3ad05ec ( Context * __context__, AutoTuple<int32_t,uint64_t,int32_t> &  ____this_rename_at_158_86 )
{
    memset(&(____this_rename_at_158_86), 0, TypeSize<AutoTuple<int32_t,uint64_t,int32_t>>::size);
}

inline void _FuncapplyTicktupleTick0x6f4dd0adfbe8abacTick0xa0Tick0Tick3_69d210316d1d7319 ( Context * __context__, AutoTuple<int32_t,uint64_t,int32_t> &  ___Var_Tick_self_rename_at_112_87, Block DAS_COMMENT((void,int32_t &)) const  &  ____arg_generation_rename_at_112_88, Block DAS_COMMENT((void,uint64_t &)) const  &  ____arg_archetype_rename_at_112_89, Block DAS_COMMENT((void,int32_t &)) const  &  ____arg_index_rename_at_112_90 )
{
    das_invoke<void>::invoke<int32_t &>(__context__,nullptr,____arg_generation_rename_at_112_88,das_get_auto_tuple_field<int32_t,0,int32_t,uint64_t,int32_t>::get(___Var_Tick_self_rename_at_112_87));
    das_invoke<void>::invoke<uint64_t &>(__context__,nullptr,____arg_archetype_rename_at_112_89,das_get_auto_tuple_field<uint64_t,1,int32_t,uint64_t,int32_t>::get(___Var_Tick_self_rename_at_112_87));
    das_invoke<void>::invoke<int32_t &>(__context__,nullptr,____arg_index_rename_at_112_90,das_get_auto_tuple_field<int32_t,2,int32_t,uint64_t,int32_t>::get(___Var_Tick_self_rename_at_112_87));
}

inline void finalize_c2cf16c62ac9b1f5 ( Context * __context__, decs::CTypeInfo &  ____this_rename_at_25_91 )
{
    memset(&(____this_rename_at_25_91), 0, TypeSize<decs::CTypeInfo>::size);
}

inline void _FuncapplyTickstructTickCTypeInfoTick0x96Tick0Tick11_50e2ea000b6a5668 ( Context * __context__, decs::CTypeInfo &  ___Var_Tick_self_rename_at_62_92, Block DAS_COMMENT((void,DAS_COMMENT(bound_enum) das::Type &)) const  &  ____arg_basicType_rename_at_62_93, Block DAS_COMMENT((void,char * &)) const  &  ____arg_mangledName_rename_at_62_94, Block DAS_COMMENT((void,char * &)) const  &  ____arg_fullName_rename_at_62_95, Block DAS_COMMENT((void,uint64_t &)) const  &  ____arg_hash_rename_at_62_96, Block DAS_COMMENT((void,uint32_t &)) const  &  ____arg_size_rename_at_62_97, Block DAS_COMMENT((void,Func DAS_COMMENT((void,TArray<uint8_t>)) &)) const  &  ____arg_eraser_rename_at_62_98, Block DAS_COMMENT((void,Func DAS_COMMENT((void,TArray<uint8_t>,TArray<uint8_t> const )) &)) const  &  ____arg_clonner_rename_at_62_99, Block DAS_COMMENT((void,Func DAS_COMMENT((void,archive::Archive,TArray<uint8_t>,char * const )) &)) const  &  ____arg_serializer_rename_at_62_100, Block DAS_COMMENT((void,Func DAS_COMMENT((char *,void * const )) &)) const  &  ____arg_dumper_rename_at_62_101, Block DAS_COMMENT((void,Func DAS_COMMENT((TypeInfo const  *)) &)) const  &  ____arg_mkTypeInfo_rename_at_62_102, Block DAS_COMMENT((void,Func DAS_COMMENT((Lambda DAS_COMMENT((void)),TArray<uint8_t>)) &)) const  &  ____arg_gc_rename_at_62_103 )
{
    das_invoke<void>::invoke<DAS_COMMENT(bound_enum) das::Type &>(__context__,nullptr,____arg_basicType_rename_at_62_93,___Var_Tick_self_rename_at_62_92.basicType);
    das_invoke<void>::invoke<char * &>(__context__,nullptr,____arg_mangledName_rename_at_62_94,___Var_Tick_self_rename_at_62_92.mangledName);
    das_invoke<void>::invoke<char * &>(__context__,nullptr,____arg_fullName_rename_at_62_95,___Var_Tick_self_rename_at_62_92.fullName);
    das_invoke<void>::invoke<uint64_t &>(__context__,nullptr,____arg_hash_rename_at_62_96,___Var_Tick_self_rename_at_62_92.hash);
    das_invoke<void>::invoke<uint32_t &>(__context__,nullptr,____arg_size_rename_at_62_97,___Var_Tick_self_rename_at_62_92.size);
    das_invoke<void>::invoke<Func DAS_COMMENT((void,TArray<uint8_t>)) &>(__context__,nullptr,____arg_eraser_rename_at_62_98,___Var_Tick_self_rename_at_62_92.eraser);
    das_invoke<void>::invoke<Func DAS_COMMENT((void,TArray<uint8_t>,TArray<uint8_t> const )) &>(__context__,nullptr,____arg_clonner_rename_at_62_99,___Var_Tick_self_rename_at_62_92.clonner);
    das_invoke<void>::invoke<Func DAS_COMMENT((void,archive::Archive,TArray<uint8_t>,char * const )) &>(__context__,nullptr,____arg_serializer_rename_at_62_100,___Var_Tick_self_rename_at_62_92.serializer);
    das_invoke<void>::invoke<Func DAS_COMMENT((char *,void * const )) &>(__context__,nullptr,____arg_dumper_rename_at_62_101,___Var_Tick_self_rename_at_62_92.dumper);
    das_invoke<void>::invoke<Func DAS_COMMENT((TypeInfo const  *)) &>(__context__,nullptr,____arg_mkTypeInfo_rename_at_62_102,___Var_Tick_self_rename_at_62_92.mkTypeInfo);
    das_invoke<void>::invoke<Func DAS_COMMENT((Lambda DAS_COMMENT((void)),TArray<uint8_t>)) &>(__context__,nullptr,____arg_gc_rename_at_62_103,___Var_Tick_self_rename_at_62_92.gc);
}

inline void finalize_723506a070851f1e ( Context * __context__, decs::EcsRequest &  ____this_rename_at_92_104 )
{
    _FuncbuiltinTickfinalizeTick13836114024949725080_79f9e841b4434fd8(__context__,das_arg<TArray<char *>>::pass(____this_rename_at_92_104.req));
    _FuncbuiltinTickfinalizeTick13836114024949725080_79f9e841b4434fd8(__context__,das_arg<TArray<char *>>::pass(____this_rename_at_92_104.reqn));
    _FuncbuiltinTickfinalizeTick13836114024949725080_c2c15811c6d720e8(__context__,das_arg<TArray<int32_t>>::pass(____this_rename_at_92_104.archetypes));
    finalize_9a888ca44a018013(__context__,das_arg<decs::EcsRequestPos>::pass(____this_rename_at_92_104.at));
    memset(&(____this_rename_at_92_104), 0, TypeSize<decs::EcsRequest>::size);
}

inline void _FuncapplyTickstructTickEcsRequestTick0x96Tick0Tick5_1da32bb85bdaa226 ( Context * __context__, decs::EcsRequest &  ___Var_Tick_self_rename_at_62_105, Block DAS_COMMENT((void,uint64_t &)) const  &  ____arg_hash_rename_at_62_106, Block DAS_COMMENT((void,TArray<char *>)) const  &  ____arg_req_rename_at_62_107, Block DAS_COMMENT((void,TArray<char *>)) const  &  ____arg_reqn_rename_at_62_108, Block DAS_COMMENT((void,TArray<int32_t>)) const  &  ____arg_archetypes_rename_at_62_109, Block DAS_COMMENT((void,decs::EcsRequestPos)) const  &  ____arg_at_rename_at_62_110 )
{
    das_invoke<void>::invoke<uint64_t &>(__context__,nullptr,____arg_hash_rename_at_62_106,___Var_Tick_self_rename_at_62_105.hash);
    das_invoke<void>::invoke<TArray<char *> &>(__context__,nullptr,____arg_req_rename_at_62_107,das_arg<TArray<char *>>::pass(___Var_Tick_self_rename_at_62_105.req));
    das_invoke<void>::invoke<TArray<char *> &>(__context__,nullptr,____arg_reqn_rename_at_62_108,das_arg<TArray<char *>>::pass(___Var_Tick_self_rename_at_62_105.reqn));
    das_invoke<void>::invoke<TArray<int32_t> &>(__context__,nullptr,____arg_archetypes_rename_at_62_109,das_arg<TArray<int32_t>>::pass(___Var_Tick_self_rename_at_62_105.archetypes));
    das_invoke<void>::invoke<decs::EcsRequestPos &>(__context__,nullptr,____arg_at_rename_at_62_110,das_arg<decs::EcsRequestPos>::pass(___Var_Tick_self_rename_at_62_105.at));
}

inline void _FuncarchiveTickread_rawTick6802855309095275289_b9dfe21dac02b654 ( Context * __context__, archive::Archive &  __arch_rename_at_99_111, int32_t & __value_rename_at_99_112 )
{
    DAS_ASSERT((__arch_rename_at_99_111.reading));
    das_invoke_method<bool,offsetof(archive::Serializer,read)>::invoke<archive::Serializer &,int32_t *,int32_t>(__context__,nullptr,das_arg<archive::Serializer>::pass(das_cast<archive::Serializer>::cast(das_deref(__context__,__arch_rename_at_99_111.stream))),das_ref(__context__,__value_rename_at_99_112),4);
}

inline void _FuncarchiveTickserializeTick6962985013043226618_125aa1d245ce9a68 ( Context * __context__, archive::Archive &  __arch_rename_at_112_113, uint64_t & __value_rename_at_112_114 )
{
    _FuncarchiveTickserialize_rawTick346513482259279339_83ff526b8b4a3feb(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_112_113),__value_rename_at_112_114);
}

inline void _FuncarchiveTickserializeTick6962985013043226618_8e0595ec66b9d4fd ( Context * __context__, archive::Archive &  __arch_rename_at_112_115, int32_t & __value_rename_at_112_116 )
{
    _FuncarchiveTickserialize_rawTick346513482259279339_344032bd32dfc282(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_112_115),__value_rename_at_112_116);
}

inline void _FuncarchiveTickwrite_rawTick9760319972816126958_e9195ce9edbb9e44 ( Context * __context__, archive::Archive &  __arch_rename_at_105_117, int32_t & __value_rename_at_105_118 )
{
    DAS_ASSERT((!__arch_rename_at_105_117.reading));
    das_invoke_method<bool,offsetof(archive::Serializer,write)>::invoke<archive::Serializer &,int32_t *,int32_t>(__context__,nullptr,das_arg<archive::Serializer>::pass(das_cast<archive::Serializer>::cast(das_deref(__context__,__arch_rename_at_105_117.stream))),das_ref(__context__,__value_rename_at_105_118),4);
}

inline Sequence DAS_COMMENT((uint64_t)) _FuncbuiltinTickkeysTick2205854368403803976_137bf91714c0645e ( Context * __context__, TTable<uint64_t,int32_t> const  &  __a_rename_at_1128_119 )
{
    Sequence DAS_COMMENT((uint64_t)) __it_rename_at_1129_120; das_zero(__it_rename_at_1129_120);
    builtin_table_keys(das_arg<Sequence DAS_COMMENT((uint64_t))>::pass(__it_rename_at_1129_120),__a_rename_at_1128_119,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((uint64_t))>::cast(__it_rename_at_1129_120);
}

inline Sequence DAS_COMMENT((int32_t &)) _FuncbuiltinTickvaluesTick1351216622833168869_38542bdf50c4ab32 ( Context * __context__, TTable<uint64_t,int32_t> &  __a_rename_at_1150_121 )
{
    Sequence DAS_COMMENT((int32_t *)) __it_rename_at_1151_122; das_zero(__it_rename_at_1151_122);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((int32_t))>::pass(__it_rename_at_1151_122),das_arg<TTable<uint64_t,int32_t>>::pass(__a_rename_at_1150_121),4,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((int32_t &))>::cast(__it_rename_at_1151_122);
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_aeca051a2e04cb07 ( Context * __context__, TArray<decs::Archetype> &  __Arr_rename_at_68_123, int32_t __newSize_rename_at_68_124 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_1,cast<TArray<decs::Archetype> &>::from(__Arr_rename_at_68_123)));
    builtin_array_resize(das_arg<TArray<decs::Archetype>>::pass(__Arr_rename_at_68_123),__newSize_rename_at_68_124,40,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncarchiveTickserializeTick8013964856239694639_731eb25edf591333 ( Context * __context__, archive::Archive &  __arch_rename_at_146_125, decs::Archetype &  __value_rename_at_146_126 ) { das_stack_prologue __prologue(__context__,288,"archive`serialize`8013964856239694639 " DAS_FILE_LINE);
{
    if ( __arch_rename_at_146_125.reading )
    {
        finalize_37451350471f63d7(__context__,das_arg<decs::Archetype>::pass(__value_rename_at_146_126));
    };
    _FuncapplyTickstructTickArchetypeTick0x96Tick0Tick4_86443a66f85f8e05(__context__,das_arg<decs::Archetype>::pass(__value_rename_at_146_126),das_make_block<void,uint64_t &>(__context__,80,0,&__func_info__214fcb6dd261a4ee,[&](uint64_t & __field_rename_at_150_127) -> void{
        _FuncarchiveTickserializeTick6962985013043226618_125aa1d245ce9a68(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_146_125),__field_rename_at_150_127);
    }),das_make_block<void,TArray<decs::Component> &>(__context__,144,0,&__func_info__19af4d1c08ef72b2,[&](TArray<decs::Component> & __field_rename_at_150_128) -> void{
        _FuncarchiveTickserializeTick1002677114065211290_838b04f3311f9ee8(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_146_125),das_arg<TArray<decs::Component>>::pass(__field_rename_at_150_128));
    }),das_make_block<void,int32_t &>(__context__,208,0,&__func_info__9e7ffb69e4895cc9,[&](int32_t & __field_rename_at_150_129) -> void{
        _FuncarchiveTickserializeTick6962985013043226618_8e0595ec66b9d4fd(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_146_125),__field_rename_at_150_129);
    }),das_make_block<void,int32_t &>(__context__,272,0,&__func_info__9e7ffb69e4895cc9,[&](int32_t & __field_rename_at_150_130) -> void{
        _FuncarchiveTickserializeTick6962985013043226618_8e0595ec66b9d4fd(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_146_125),__field_rename_at_150_130);
    }));
}}

inline void _FuncbuiltinTickresizeTick4811697762258667383_be5e4db5cc62bbe2 ( Context * __context__, TArray<decs::EntityId> &  __Arr_rename_at_68_131, int32_t __newSize_rename_at_68_132 )
{
    builtin_array_resize(das_arg<TArray<decs::EntityId>>::pass(__Arr_rename_at_68_131),__newSize_rename_at_68_132,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncarchiveTickserializeTick8013964856239694639_9f9391ad59b315bc ( Context * __context__, archive::Archive &  __arch_rename_at_146_133, decs::EntityId &  __value_rename_at_146_134 ) { das_stack_prologue __prologue(__context__,160,"archive`serialize`8013964856239694639 " DAS_FILE_LINE);
{
    if ( __arch_rename_at_146_133.reading )
    {
        finalize_aed1eae423063a73(__context__,das_arg<decs::EntityId>::pass(__value_rename_at_146_134));
    };
    _FuncapplyTickstructTickEntityIdTick0x96Tick0Tick2_ceabfe323854d15e(__context__,das_arg<decs::EntityId>::pass(__value_rename_at_146_134),das_make_block<void,uint32_t &>(__context__,80,0,&__func_info__e5601d0b9c7536bc,[&](uint32_t & __field_rename_at_150_135) -> void{
        _FuncarchiveTickserializeTick6962985013043226618_c51e37371f3d267a(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_146_133),__field_rename_at_150_135);
    }),das_make_block<void,int32_t &>(__context__,144,0,&__func_info__9e7ffb69e4895cc9,[&](int32_t & __field_rename_at_150_136) -> void{
        _FuncarchiveTickserializeTick6962985013043226618_8e0595ec66b9d4fd(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_146_133),__field_rename_at_150_136);
    }));
}}

inline void _FuncbuiltinTickresizeTick4811697762258667383_deb20fe608939269 ( Context * __context__, TArray<AutoTuple<int32_t,uint64_t,int32_t>> &  __Arr_rename_at_68_137, int32_t __newSize_rename_at_68_138 )
{
    builtin_array_resize(das_arg<TArray<AutoTuple<int32_t,uint64_t,int32_t>>>::pass(__Arr_rename_at_68_137),__newSize_rename_at_68_138,24,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncarchiveTickserializeTick10018991383941125049_67930047264965f1 ( Context * __context__, archive::Archive &  __arch_rename_at_156_139, AutoTuple<int32_t,uint64_t,int32_t> &  __value_rename_at_156_140 ) { das_stack_prologue __prologue(__context__,224,"archive`serialize`10018991383941125049 " DAS_FILE_LINE);
{
    if ( __arch_rename_at_156_139.reading )
    {
        finalize_f87ddce7a3ad05ec(__context__,das_arg<AutoTuple<int32_t,uint64_t,int32_t>>::pass(__value_rename_at_156_140));
    };
    _FuncapplyTicktupleTick0x6f4dd0adfbe8abacTick0xa0Tick0Tick3_69d210316d1d7319(__context__,das_arg<AutoTuple<int32_t,uint64_t,int32_t>>::pass(__value_rename_at_156_140),das_make_block<void,int32_t &>(__context__,80,0,&__func_info__9e7ffb69e4895cc9,[&](int32_t & __field_rename_at_160_141) -> void{
        _FuncarchiveTickserializeTick6962985013043226618_8e0595ec66b9d4fd(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_156_139),__field_rename_at_160_141);
    }),das_make_block<void,uint64_t &>(__context__,144,0,&__func_info__214fcb6dd261a4ee,[&](uint64_t & __field_rename_at_160_142) -> void{
        _FuncarchiveTickserializeTick6962985013043226618_125aa1d245ce9a68(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_156_139),__field_rename_at_160_142);
    }),das_make_block<void,int32_t &>(__context__,208,0,&__func_info__9e7ffb69e4895cc9,[&](int32_t & __field_rename_at_160_143) -> void{
        _FuncarchiveTickserializeTick6962985013043226618_8e0595ec66b9d4fd(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_156_139),__field_rename_at_160_143);
    }));
}}

inline void _FuncarchiveTickserializeTick8013964856239694639_a8ff9e2247269964 ( Context * __context__, archive::Archive &  __arch_rename_at_146_144, decs::CTypeInfo &  __value_rename_at_146_145 ) { das_stack_prologue __prologue(__context__,736,"archive`serialize`8013964856239694639 " DAS_FILE_LINE);
{
    if ( __arch_rename_at_146_144.reading )
    {
        finalize_c2cf16c62ac9b1f5(__context__,das_arg<decs::CTypeInfo>::pass(__value_rename_at_146_145));
    };
    _FuncapplyTickstructTickCTypeInfoTick0x96Tick0Tick11_50e2ea000b6a5668(__context__,das_arg<decs::CTypeInfo>::pass(__value_rename_at_146_145),das_make_block<void,DAS_COMMENT(bound_enum) das::Type &>(__context__,80,0,&__func_info__d7138e6aaa104191,[&](DAS_COMMENT(bound_enum) das::Type & __field_rename_at_150_146) -> void{
        _FuncarchiveTickserializeTick6962985013043226618_ed7dae2a007ca602(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_146_144),__field_rename_at_150_146);
    }),das_make_block<void,char * &>(__context__,144,0,&__func_info__c85cb34a7aabfce2,[&](char * & __field_rename_at_150_147) -> void{
        das_invoke_function<void>::invoke<archive::Archive &,char * &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@archive::serialize S<archive::Archive> &s*/ 0x27563976f74188a2)),das_arg<archive::Archive>::pass(__arch_rename_at_146_144),__field_rename_at_150_147);
    }),das_make_block<void,char * &>(__context__,208,0,&__func_info__c85cb34a7aabfce2,[&](char * & __field_rename_at_150_148) -> void{
        das_invoke_function<void>::invoke<archive::Archive &,char * &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@archive::serialize S<archive::Archive> &s*/ 0x27563976f74188a2)),das_arg<archive::Archive>::pass(__arch_rename_at_146_144),__field_rename_at_150_148);
    }),das_make_block<void,uint64_t &>(__context__,272,0,&__func_info__214fcb6dd261a4ee,[&](uint64_t & __field_rename_at_150_149) -> void{
        _FuncarchiveTickserializeTick6962985013043226618_125aa1d245ce9a68(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_146_144),__field_rename_at_150_149);
    }),das_make_block<void,uint32_t &>(__context__,336,0,&__func_info__e5601d0b9c7536bc,[&](uint32_t & __field_rename_at_150_150) -> void{
        _FuncarchiveTickserializeTick6962985013043226618_c51e37371f3d267a(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_146_144),__field_rename_at_150_150);
    }),das_make_block<void,Func DAS_COMMENT((void,TArray<uint8_t>)) &>(__context__,400,0,&__func_info__41a35bb8cc7508cc,[&](Func DAS_COMMENT((void,TArray<uint8_t>)) & __field_rename_at_150_151) -> void{
        _FuncarchiveTickserializeTick4429977757536996025_2b74a39202406253(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_146_144),__field_rename_at_150_151);
    }),das_make_block<void,Func DAS_COMMENT((void,TArray<uint8_t>,TArray<uint8_t> const )) &>(__context__,464,0,&__func_info__c9f4d529bdde6142,[&](Func DAS_COMMENT((void,TArray<uint8_t>,TArray<uint8_t> const )) & __field_rename_at_150_152) -> void{
        _FuncarchiveTickserializeTick4429977757536996025_1ff57546f6343a0a(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_146_144),__field_rename_at_150_152);
    }),das_make_block<void,Func DAS_COMMENT((void,archive::Archive,TArray<uint8_t>,char * const )) &>(__context__,528,0,&__func_info__6b8a23cd42de6854,[&](Func DAS_COMMENT((void,archive::Archive,TArray<uint8_t>,char * const )) & __field_rename_at_150_153) -> void{
        _FuncarchiveTickserializeTick4429977757536996025_beb22678ca166fa4(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_146_144),__field_rename_at_150_153);
    }),das_make_block<void,Func DAS_COMMENT((char *,void * const )) &>(__context__,592,0,&__func_info__dbcb75dfde121c0e,[&](Func DAS_COMMENT((char *,void * const )) & __field_rename_at_150_154) -> void{
        _FuncarchiveTickserializeTick4429977757536996025_3b53a58effc6756a(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_146_144),__field_rename_at_150_154);
    }),das_make_block<void,Func DAS_COMMENT((TypeInfo const  *)) &>(__context__,656,0,&__func_info__766dae3d6a2a7d14,[&](Func DAS_COMMENT((TypeInfo const  *)) & __field_rename_at_150_155) -> void{
        _FuncarchiveTickserializeTick4429977757536996025_190cc1e99493f13c(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_146_144),__field_rename_at_150_155);
    }),das_make_block<void,Func DAS_COMMENT((Lambda DAS_COMMENT((void)),TArray<uint8_t>)) &>(__context__,720,0,&__func_info__97ee470ea66a6afd,[&](Func DAS_COMMENT((Lambda DAS_COMMENT((void)),TArray<uint8_t>)) & __field_rename_at_150_156) -> void{
        _FuncarchiveTickserializeTick4429977757536996025_25683f2273767516(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_146_144),__field_rename_at_150_156);
    }));
}}

inline Sequence DAS_COMMENT((char *)) _FuncbuiltinTickkeysTick2205854368403803976_23a113e85e5887ab ( Context * __context__, TTable<char *,decs::CTypeInfo> const  &  __a_rename_at_1128_157 )
{
    Sequence DAS_COMMENT((char *)) __it_rename_at_1129_158; das_zero(__it_rename_at_1129_158);
    builtin_table_keys(das_arg<Sequence DAS_COMMENT((char *))>::pass(__it_rename_at_1129_158),__a_rename_at_1128_157,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((char *))>::cast(__it_rename_at_1129_158);
}

inline Sequence DAS_COMMENT((decs::CTypeInfo &)) _FuncbuiltinTickvaluesTick1351216622833168869_61c0f20a0f69d8bd ( Context * __context__, TTable<char *,decs::CTypeInfo> &  __a_rename_at_1150_159 )
{
    Sequence DAS_COMMENT((decs::CTypeInfo *)) __it_rename_at_1151_160; das_zero(__it_rename_at_1151_160);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((decs::CTypeInfo))>::pass(__it_rename_at_1151_160),das_arg<TTable<char *,decs::CTypeInfo>>::pass(__a_rename_at_1150_159),88,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((decs::CTypeInfo &))>::cast(__it_rename_at_1151_160);
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_d65f5162f71fde6a ( Context * __context__, TArray<decs::EcsRequest> &  __Arr_rename_at_68_161, int32_t __newSize_rename_at_68_162 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_2,cast<TArray<decs::EcsRequest> &>::from(__Arr_rename_at_68_161)));
    builtin_array_resize(das_arg<TArray<decs::EcsRequest>>::pass(__Arr_rename_at_68_161),__newSize_rename_at_68_162,96,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncarchiveTickserializeTick8013964856239694639_95732ecdb0114b69 ( Context * __context__, archive::Archive &  __arch_rename_at_146_163, decs::EcsRequest &  __value_rename_at_146_164 ) { das_stack_prologue __prologue(__context__,352,"archive`serialize`8013964856239694639 " DAS_FILE_LINE);
{
    if ( __arch_rename_at_146_163.reading )
    {
        finalize_723506a070851f1e(__context__,das_arg<decs::EcsRequest>::pass(__value_rename_at_146_164));
    };
    _FuncapplyTickstructTickEcsRequestTick0x96Tick0Tick5_1da32bb85bdaa226(__context__,das_arg<decs::EcsRequest>::pass(__value_rename_at_146_164),das_make_block<void,uint64_t &>(__context__,80,0,&__func_info__214fcb6dd261a4ee,[&](uint64_t & __field_rename_at_150_165) -> void{
        _FuncarchiveTickserializeTick6962985013043226618_125aa1d245ce9a68(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_146_163),__field_rename_at_150_165);
    }),das_make_block<void,TArray<char *> &>(__context__,144,0,&__func_info__1bcbcef990d4075b,[&](TArray<char *> & __field_rename_at_150_166) -> void{
        _FuncarchiveTickserializeTick1002677114065211290_bef6bf5e85c51d3b(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_146_163),das_arg<TArray<char *>>::pass(__field_rename_at_150_166));
    }),das_make_block<void,TArray<char *> &>(__context__,208,0,&__func_info__1bcbcef990d4075b,[&](TArray<char *> & __field_rename_at_150_167) -> void{
        _FuncarchiveTickserializeTick1002677114065211290_bef6bf5e85c51d3b(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_146_163),das_arg<TArray<char *>>::pass(__field_rename_at_150_167));
    }),das_make_block<void,TArray<int32_t> &>(__context__,272,0,&__func_info__f45f30cc74c8f4a6,[&](TArray<int32_t> & __field_rename_at_150_168) -> void{
        _FuncarchiveTickserializeTick1002677114065211290_591dc6f784de6b57(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_146_163),das_arg<TArray<int32_t>>::pass(__field_rename_at_150_168));
    }),das_make_block<void,decs::EcsRequestPos &>(__context__,336,0,&__func_info__faa6e7b3f28885e9,[&](decs::EcsRequestPos & __field_rename_at_150_169) -> void{
        _FuncarchiveTickserializeTick8013964856239694639_cac39795b36f7c56(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_146_163),das_arg<decs::EcsRequestPos>::pass(__field_rename_at_150_169));
    }));
}}

inline void _FuncbuiltinTickfinalizeTick5454204887383796109_7b1c641088884a8c ( Context * __context__, TTable<uint64_t,int32_t> &  __a_rename_at_1193_170 )
{
    builtin_table_free(das_arg<TTable<uint64_t,int32_t>>::pass(__a_rename_at_1193_170),8,4,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_ea18cb8662f307c1 ( Context * __context__, TArray<decs::Archetype> &  __a_rename_at_1182_171 )
{
    {
        bool __need_loop_1184 = true;
        // aV: decs::Archetype aka TT&
        das_iterator<TArray<decs::Archetype>> __aV_iterator(__a_rename_at_1182_171);
        decs::Archetype * __aV_rename_at_1184_172;
        __need_loop_1184 = __aV_iterator.first(__context__,(__aV_rename_at_1184_172)) && __need_loop_1184;
        for ( ; __need_loop_1184 ; __need_loop_1184 = __aV_iterator.next(__context__,(__aV_rename_at_1184_172)) )
        {
            finalize_37451350471f63d7(__context__,das_arg<decs::Archetype>::pass((*__aV_rename_at_1184_172)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1184_172));
    };
    builtin_array_free(das_arg<TArray<decs::Archetype>>::pass(__a_rename_at_1182_171),40,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_5fbae9e7389a3856 ( Context * __context__, TArray<decs::EntityId> &  __a_rename_at_1182_173 )
{
    {
        bool __need_loop_1184 = true;
        // aV: decs::EntityId aka TT&
        das_iterator<TArray<decs::EntityId>> __aV_iterator(__a_rename_at_1182_173);
        decs::EntityId * __aV_rename_at_1184_174;
        __need_loop_1184 = __aV_iterator.first(__context__,(__aV_rename_at_1184_174)) && __need_loop_1184;
        for ( ; __need_loop_1184 ; __need_loop_1184 = __aV_iterator.next(__context__,(__aV_rename_at_1184_174)) )
        {
            finalize_aed1eae423063a73(__context__,das_arg<decs::EntityId>::pass((*__aV_rename_at_1184_174)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1184_174));
    };
    builtin_array_free(das_arg<TArray<decs::EntityId>>::pass(__a_rename_at_1182_173),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_ba685a3ffd2b9e77 ( Context * __context__, TArray<AutoTuple<int32_t,uint64_t,int32_t>> &  __a_rename_at_1182_175 )
{
    {
        bool __need_loop_1184 = true;
        // aV: tuple<generation:int;archetype:uint64 aka ComponentHash;index:int> aka TT&
        das_iterator<TArray<AutoTuple<int32_t,uint64_t,int32_t>>> __aV_iterator(__a_rename_at_1182_175);
        AutoTuple<int32_t,uint64_t,int32_t> * __aV_rename_at_1184_176;
        __need_loop_1184 = __aV_iterator.first(__context__,(__aV_rename_at_1184_176)) && __need_loop_1184;
        for ( ; __need_loop_1184 ; __need_loop_1184 = __aV_iterator.next(__context__,(__aV_rename_at_1184_176)) )
        {
            finalize_f87ddce7a3ad05ec(__context__,das_arg<AutoTuple<int32_t,uint64_t,int32_t>>::pass((*__aV_rename_at_1184_176)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1184_176));
    };
    builtin_array_free(das_arg<TArray<AutoTuple<int32_t,uint64_t,int32_t>>>::pass(__a_rename_at_1182_175),24,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick5454204887383796109_1f7f0c3546fa051e ( Context * __context__, TTable<char *,decs::CTypeInfo> &  __a_rename_at_1193_177 )
{
    Sequence DAS_COMMENT((decs::CTypeInfo *)) _temp_make_local_1195_19_32; _temp_make_local_1195_19_32;
    {
        bool __need_loop_1195 = true;
        // aV: decs::CTypeInfo&
        das_iterator<Sequence DAS_COMMENT((decs::CTypeInfo))> __aV_iterator((_temp_make_local_1195_19_32 = (_FuncbuiltinTickvaluesTick1351216622833168869_61c0f20a0f69d8bd(__context__,das_arg<TTable<char *,decs::CTypeInfo>>::pass(__a_rename_at_1193_177)))));
        decs::CTypeInfo * __aV_rename_at_1195_178;
        __need_loop_1195 = __aV_iterator.first(__context__,(__aV_rename_at_1195_178)) && __need_loop_1195;
        for ( ; __need_loop_1195 ; __need_loop_1195 = __aV_iterator.next(__context__,(__aV_rename_at_1195_178)) )
        {
            finalize_c2cf16c62ac9b1f5(__context__,das_arg<decs::CTypeInfo>::pass((*__aV_rename_at_1195_178)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1195_178));
    };
    builtin_table_free(das_arg<TTable<char *,decs::CTypeInfo>>::pass(__a_rename_at_1193_177),8,88,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_9a747408a2a6d0ff ( Context * __context__, TArray<decs::EcsRequest> &  __a_rename_at_1182_179 )
{
    {
        bool __need_loop_1184 = true;
        // aV: decs::EcsRequest aka TT&
        das_iterator<TArray<decs::EcsRequest>> __aV_iterator(__a_rename_at_1182_179);
        decs::EcsRequest * __aV_rename_at_1184_180;
        __need_loop_1184 = __aV_iterator.first(__context__,(__aV_rename_at_1184_180)) && __need_loop_1184;
        for ( ; __need_loop_1184 ; __need_loop_1184 = __aV_iterator.next(__context__,(__aV_rename_at_1184_180)) )
        {
            finalize_723506a070851f1e(__context__,das_arg<decs::EcsRequest>::pass((*__aV_rename_at_1184_180)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1184_180));
    };
    builtin_array_free(das_arg<TArray<decs::EcsRequest>>::pass(__a_rename_at_1182_179),96,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncarchiveTickserializeTick18117489833716462019_10e000d138e4ac0f ( Context * __context__, archive::Archive &  __arch_rename_at_209_181, TTable<uint64_t,int32_t> &  __value_rename_at_209_182 )
{
    if ( __arch_rename_at_209_181.reading )
    {
        int32_t __len_rename_at_211_183 = 0;
        _FuncarchiveTickread_rawTick6802855309095275289_b9dfe21dac02b654(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_209_181),__len_rename_at_211_183);
        _FuncbuiltinTickfinalizeTick5454204887383796109_7b1c641088884a8c(__context__,das_arg<TTable<uint64_t,int32_t>>::pass(__value_rename_at_209_182));
        {
            bool __need_loop_214 = true;
            // _: int const
            das_iterator<range> ____iterator(mk_range(__len_rename_at_211_183));
            int32_t ____rename_at_214_184;
            __need_loop_214 = ____iterator.first(__context__,(____rename_at_214_184)) && __need_loop_214;
            for ( ; __need_loop_214 ; __need_loop_214 = ____iterator.next(__context__,(____rename_at_214_184)) )
            {
                uint64_t __k_rename_at_215_185 = 0;
                _FuncarchiveTickserializeTick6962985013043226618_125aa1d245ce9a68(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_209_181),__k_rename_at_215_185);
                _FuncarchiveTickserializeTick6962985013043226618_8e0595ec66b9d4fd(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_209_181),__value_rename_at_209_182(__k_rename_at_215_185,__context__));
            }
            ____iterator.close(__context__,(____rename_at_214_184));
        };
    } else {
        Sequence DAS_COMMENT((uint64_t)) _temp_make_local_222_21_96; _temp_make_local_222_21_96;
        Sequence DAS_COMMENT((int32_t *)) _temp_make_local_222_34_112; _temp_make_local_222_34_112;
        int32_t __len_rename_at_220_186 = builtin_table_size(das_arg<TTable<uint64_t,int32_t>>::pass(__value_rename_at_209_182));
        _FuncarchiveTickwrite_rawTick9760319972816126958_e9195ce9edbb9e44(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_209_181),__len_rename_at_220_186);
        {
            bool __need_loop_222 = true;
            // k: uint64
            das_iterator<Sequence DAS_COMMENT((uint64_t))> __k_iterator((_temp_make_local_222_21_96 = (_FuncbuiltinTickkeysTick2205854368403803976_137bf91714c0645e(__context__,das_arg<TTable<uint64_t,int32_t>>::pass(__value_rename_at_209_182)))));
            uint64_t __k_rename_at_222_189;
            __need_loop_222 = __k_iterator.first(__context__,(__k_rename_at_222_189)) && __need_loop_222;
            // v: int&
            das_iterator<Sequence DAS_COMMENT((int32_t))> __v_iterator((_temp_make_local_222_34_112 = (_FuncbuiltinTickvaluesTick1351216622833168869_38542bdf50c4ab32(__context__,das_arg<TTable<uint64_t,int32_t>>::pass(__value_rename_at_209_182)))));
            int32_t * __v_rename_at_222_190;
            __need_loop_222 = __v_iterator.first(__context__,(__v_rename_at_222_190)) && __need_loop_222;
            for ( ; __need_loop_222 ; __need_loop_222 = __k_iterator.next(__context__,(__k_rename_at_222_189)) && __v_iterator.next(__context__,(__v_rename_at_222_190)) )
            {
                uint64_t __K_rename_at_223_191 = __k_rename_at_222_189;
                _FuncarchiveTickserializeTick6962985013043226618_125aa1d245ce9a68(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_209_181),__K_rename_at_223_191);
                _FuncarchiveTickserializeTick6962985013043226618_8e0595ec66b9d4fd(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_209_181),(*__v_rename_at_222_190));
            }
            __k_iterator.close(__context__,(__k_rename_at_222_189));
            __v_iterator.close(__context__,(__v_rename_at_222_190));
        };
    };
}

inline void _FuncarchiveTickserializeTick1002677114065211290_339f3460927c659a ( Context * __context__, archive::Archive &  __arch_rename_at_189_192, TArray<decs::Archetype> &  __value_rename_at_189_193 )
{
    if ( __arch_rename_at_189_192.reading )
    {
        int32_t __len_rename_at_191_194 = 0;
        _FuncbuiltinTickfinalizeTick13836114024949725080_ea18cb8662f307c1(__context__,das_arg<TArray<decs::Archetype>>::pass(__value_rename_at_189_193));
        _FuncarchiveTickread_rawTick6802855309095275289_b9dfe21dac02b654(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_189_192),__len_rename_at_191_194);
        _FuncbuiltinTickresizeTick4811697762258667383_aeca051a2e04cb07(__context__,das_arg<TArray<decs::Archetype>>::pass(__value_rename_at_189_193),__len_rename_at_191_194);
        {
            bool __need_loop_197 = true;
            // element: decs::Archetype aka TT&
            das_iterator<TArray<decs::Archetype>> __element_iterator(__value_rename_at_189_193);
            decs::Archetype * __element_rename_at_197_195;
            __need_loop_197 = __element_iterator.first(__context__,(__element_rename_at_197_195)) && __need_loop_197;
            for ( ; __need_loop_197 ; __need_loop_197 = __element_iterator.next(__context__,(__element_rename_at_197_195)) )
            {
                _FuncarchiveTickserializeTick8013964856239694639_731eb25edf591333(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_189_192),das_arg<decs::Archetype>::pass((*__element_rename_at_197_195)));
            }
            __element_iterator.close(__context__,(__element_rename_at_197_195));
        };
    } else {
        int32_t __len_rename_at_201_196 = builtin_array_size(das_arg<TArray<decs::Archetype>>::pass(__value_rename_at_189_193));
        _FuncarchiveTickwrite_rawTick9760319972816126958_e9195ce9edbb9e44(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_189_192),__len_rename_at_201_196);
        {
            bool __need_loop_203 = true;
            // element: decs::Archetype aka TT&
            das_iterator<TArray<decs::Archetype>> __element_iterator(__value_rename_at_189_193);
            decs::Archetype * __element_rename_at_203_197;
            __need_loop_203 = __element_iterator.first(__context__,(__element_rename_at_203_197)) && __need_loop_203;
            for ( ; __need_loop_203 ; __need_loop_203 = __element_iterator.next(__context__,(__element_rename_at_203_197)) )
            {
                _FuncarchiveTickserializeTick8013964856239694639_731eb25edf591333(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_189_192),das_arg<decs::Archetype>::pass((*__element_rename_at_203_197)));
            }
            __element_iterator.close(__context__,(__element_rename_at_203_197));
        };
    };
}

inline void _FuncarchiveTickserializeTick1002677114065211290_2ebd95325aaa7a86 ( Context * __context__, archive::Archive &  __arch_rename_at_189_198, TArray<decs::EntityId> &  __value_rename_at_189_199 )
{
    if ( __arch_rename_at_189_198.reading )
    {
        int32_t __len_rename_at_191_200 = 0;
        _FuncbuiltinTickfinalizeTick13836114024949725080_5fbae9e7389a3856(__context__,das_arg<TArray<decs::EntityId>>::pass(__value_rename_at_189_199));
        _FuncarchiveTickread_rawTick6802855309095275289_b9dfe21dac02b654(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_189_198),__len_rename_at_191_200);
        _FuncbuiltinTickresizeTick4811697762258667383_be5e4db5cc62bbe2(__context__,das_arg<TArray<decs::EntityId>>::pass(__value_rename_at_189_199),__len_rename_at_191_200);
        {
            bool __need_loop_197 = true;
            // element: decs::EntityId aka TT&
            das_iterator<TArray<decs::EntityId>> __element_iterator(__value_rename_at_189_199);
            decs::EntityId * __element_rename_at_197_201;
            __need_loop_197 = __element_iterator.first(__context__,(__element_rename_at_197_201)) && __need_loop_197;
            for ( ; __need_loop_197 ; __need_loop_197 = __element_iterator.next(__context__,(__element_rename_at_197_201)) )
            {
                _FuncarchiveTickserializeTick8013964856239694639_9f9391ad59b315bc(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_189_198),das_arg<decs::EntityId>::pass((*__element_rename_at_197_201)));
            }
            __element_iterator.close(__context__,(__element_rename_at_197_201));
        };
    } else {
        int32_t __len_rename_at_201_202 = builtin_array_size(das_arg<TArray<decs::EntityId>>::pass(__value_rename_at_189_199));
        _FuncarchiveTickwrite_rawTick9760319972816126958_e9195ce9edbb9e44(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_189_198),__len_rename_at_201_202);
        {
            bool __need_loop_203 = true;
            // element: decs::EntityId aka TT&
            das_iterator<TArray<decs::EntityId>> __element_iterator(__value_rename_at_189_199);
            decs::EntityId * __element_rename_at_203_203;
            __need_loop_203 = __element_iterator.first(__context__,(__element_rename_at_203_203)) && __need_loop_203;
            for ( ; __need_loop_203 ; __need_loop_203 = __element_iterator.next(__context__,(__element_rename_at_203_203)) )
            {
                _FuncarchiveTickserializeTick8013964856239694639_9f9391ad59b315bc(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_189_198),das_arg<decs::EntityId>::pass((*__element_rename_at_203_203)));
            }
            __element_iterator.close(__context__,(__element_rename_at_203_203));
        };
    };
}

inline void _FuncarchiveTickserializeTick1002677114065211290_7eba8f9beff4a0ef ( Context * __context__, archive::Archive &  __arch_rename_at_189_204, TArray<AutoTuple<int32_t,uint64_t,int32_t>> &  __value_rename_at_189_205 )
{
    if ( __arch_rename_at_189_204.reading )
    {
        int32_t __len_rename_at_191_206 = 0;
        _FuncbuiltinTickfinalizeTick13836114024949725080_ba685a3ffd2b9e77(__context__,das_arg<TArray<AutoTuple<int32_t,uint64_t,int32_t>>>::pass(__value_rename_at_189_205));
        _FuncarchiveTickread_rawTick6802855309095275289_b9dfe21dac02b654(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_189_204),__len_rename_at_191_206);
        _FuncbuiltinTickresizeTick4811697762258667383_deb20fe608939269(__context__,das_arg<TArray<AutoTuple<int32_t,uint64_t,int32_t>>>::pass(__value_rename_at_189_205),__len_rename_at_191_206);
        {
            bool __need_loop_197 = true;
            // element: tuple<generation:int;archetype:uint64 aka ComponentHash;index:int> aka TT&
            das_iterator<TArray<AutoTuple<int32_t,uint64_t,int32_t>>> __element_iterator(__value_rename_at_189_205);
            AutoTuple<int32_t,uint64_t,int32_t> * __element_rename_at_197_207;
            __need_loop_197 = __element_iterator.first(__context__,(__element_rename_at_197_207)) && __need_loop_197;
            for ( ; __need_loop_197 ; __need_loop_197 = __element_iterator.next(__context__,(__element_rename_at_197_207)) )
            {
                _FuncarchiveTickserializeTick10018991383941125049_67930047264965f1(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_189_204),das_arg<AutoTuple<int32_t,uint64_t,int32_t>>::pass((*__element_rename_at_197_207)));
            }
            __element_iterator.close(__context__,(__element_rename_at_197_207));
        };
    } else {
        int32_t __len_rename_at_201_208 = builtin_array_size(das_arg<TArray<AutoTuple<int32_t,uint64_t,int32_t>>>::pass(__value_rename_at_189_205));
        _FuncarchiveTickwrite_rawTick9760319972816126958_e9195ce9edbb9e44(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_189_204),__len_rename_at_201_208);
        {
            bool __need_loop_203 = true;
            // element: tuple<generation:int;archetype:uint64 aka ComponentHash;index:int> aka TT&
            das_iterator<TArray<AutoTuple<int32_t,uint64_t,int32_t>>> __element_iterator(__value_rename_at_189_205);
            AutoTuple<int32_t,uint64_t,int32_t> * __element_rename_at_203_209;
            __need_loop_203 = __element_iterator.first(__context__,(__element_rename_at_203_209)) && __need_loop_203;
            for ( ; __need_loop_203 ; __need_loop_203 = __element_iterator.next(__context__,(__element_rename_at_203_209)) )
            {
                _FuncarchiveTickserializeTick10018991383941125049_67930047264965f1(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_189_204),das_arg<AutoTuple<int32_t,uint64_t,int32_t>>::pass((*__element_rename_at_203_209)));
            }
            __element_iterator.close(__context__,(__element_rename_at_203_209));
        };
    };
}

inline void _FuncarchiveTickserializeTick18117489833716462019_82336b17b91a4160 ( Context * __context__, archive::Archive &  __arch_rename_at_209_210, TTable<char *,decs::CTypeInfo> &  __value_rename_at_209_211 )
{
    if ( __arch_rename_at_209_210.reading )
    {
        int32_t __len_rename_at_211_212 = 0;
        _FuncarchiveTickread_rawTick6802855309095275289_b9dfe21dac02b654(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_209_210),__len_rename_at_211_212);
        _FuncbuiltinTickfinalizeTick5454204887383796109_1f7f0c3546fa051e(__context__,das_arg<TTable<char *,decs::CTypeInfo>>::pass(__value_rename_at_209_211));
        {
            bool __need_loop_214 = true;
            // _: int const
            das_iterator<range> ____iterator(mk_range(__len_rename_at_211_212));
            int32_t ____rename_at_214_213;
            __need_loop_214 = ____iterator.first(__context__,(____rename_at_214_213)) && __need_loop_214;
            for ( ; __need_loop_214 ; __need_loop_214 = ____iterator.next(__context__,(____rename_at_214_213)) )
            {
                char * __k_rename_at_215_214 = 0;
                das_invoke_function<void>::invoke<archive::Archive &,char * &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@archive::serialize S<archive::Archive> &s*/ 0x27563976f74188a2)),das_arg<archive::Archive>::pass(__arch_rename_at_209_210),__k_rename_at_215_214);
                _FuncarchiveTickserializeTick8013964856239694639_a8ff9e2247269964(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_209_210),das_arg<decs::CTypeInfo>::pass(__value_rename_at_209_211(__k_rename_at_215_214,__context__)));
            }
            ____iterator.close(__context__,(____rename_at_214_213));
        };
    } else {
        Sequence DAS_COMMENT((char *)) _temp_make_local_222_21_96; _temp_make_local_222_21_96;
        Sequence DAS_COMMENT((decs::CTypeInfo *)) _temp_make_local_222_34_112; _temp_make_local_222_34_112;
        int32_t __len_rename_at_220_215 = builtin_table_size(das_arg<TTable<char *,decs::CTypeInfo>>::pass(__value_rename_at_209_211));
        _FuncarchiveTickwrite_rawTick9760319972816126958_e9195ce9edbb9e44(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_209_210),__len_rename_at_220_215);
        {
            bool __need_loop_222 = true;
            // k: string
            das_iterator<Sequence DAS_COMMENT((char *))> __k_iterator((_temp_make_local_222_21_96 = (_FuncbuiltinTickkeysTick2205854368403803976_23a113e85e5887ab(__context__,das_arg<TTable<char *,decs::CTypeInfo>>::pass(__value_rename_at_209_211)))));
            char * __k_rename_at_222_218;
            __need_loop_222 = __k_iterator.first(__context__,(__k_rename_at_222_218)) && __need_loop_222;
            // v: decs::CTypeInfo&
            das_iterator<Sequence DAS_COMMENT((decs::CTypeInfo))> __v_iterator((_temp_make_local_222_34_112 = (_FuncbuiltinTickvaluesTick1351216622833168869_61c0f20a0f69d8bd(__context__,das_arg<TTable<char *,decs::CTypeInfo>>::pass(__value_rename_at_209_211)))));
            decs::CTypeInfo * __v_rename_at_222_219;
            __need_loop_222 = __v_iterator.first(__context__,(__v_rename_at_222_219)) && __need_loop_222;
            for ( ; __need_loop_222 ; __need_loop_222 = __k_iterator.next(__context__,(__k_rename_at_222_218)) && __v_iterator.next(__context__,(__v_rename_at_222_219)) )
            {
                char * __K_rename_at_223_220 = (char *)(__k_rename_at_222_218);
                das_invoke_function<void>::invoke<archive::Archive &,char * &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@archive::serialize S<archive::Archive> &s*/ 0x27563976f74188a2)),das_arg<archive::Archive>::pass(__arch_rename_at_209_210),__K_rename_at_223_220);
                _FuncarchiveTickserializeTick8013964856239694639_a8ff9e2247269964(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_209_210),das_arg<decs::CTypeInfo>::pass((*__v_rename_at_222_219)));
            }
            __k_iterator.close(__context__,(__k_rename_at_222_218));
            __v_iterator.close(__context__,(__v_rename_at_222_219));
        };
    };
}

inline void _FuncarchiveTickserializeTick1002677114065211290_94274d93de23df5 ( Context * __context__, archive::Archive &  __arch_rename_at_189_221, TArray<decs::EcsRequest> &  __value_rename_at_189_222 )
{
    if ( __arch_rename_at_189_221.reading )
    {
        int32_t __len_rename_at_191_223 = 0;
        _FuncbuiltinTickfinalizeTick13836114024949725080_9a747408a2a6d0ff(__context__,das_arg<TArray<decs::EcsRequest>>::pass(__value_rename_at_189_222));
        _FuncarchiveTickread_rawTick6802855309095275289_b9dfe21dac02b654(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_189_221),__len_rename_at_191_223);
        _FuncbuiltinTickresizeTick4811697762258667383_d65f5162f71fde6a(__context__,das_arg<TArray<decs::EcsRequest>>::pass(__value_rename_at_189_222),__len_rename_at_191_223);
        {
            bool __need_loop_197 = true;
            // element: decs::EcsRequest aka TT&
            das_iterator<TArray<decs::EcsRequest>> __element_iterator(__value_rename_at_189_222);
            decs::EcsRequest * __element_rename_at_197_224;
            __need_loop_197 = __element_iterator.first(__context__,(__element_rename_at_197_224)) && __need_loop_197;
            for ( ; __need_loop_197 ; __need_loop_197 = __element_iterator.next(__context__,(__element_rename_at_197_224)) )
            {
                _FuncarchiveTickserializeTick8013964856239694639_95732ecdb0114b69(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_189_221),das_arg<decs::EcsRequest>::pass((*__element_rename_at_197_224)));
            }
            __element_iterator.close(__context__,(__element_rename_at_197_224));
        };
    } else {
        int32_t __len_rename_at_201_225 = builtin_array_size(das_arg<TArray<decs::EcsRequest>>::pass(__value_rename_at_189_222));
        _FuncarchiveTickwrite_rawTick9760319972816126958_e9195ce9edbb9e44(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_189_221),__len_rename_at_201_225);
        {
            bool __need_loop_203 = true;
            // element: decs::EcsRequest aka TT&
            das_iterator<TArray<decs::EcsRequest>> __element_iterator(__value_rename_at_189_222);
            decs::EcsRequest * __element_rename_at_203_226;
            __need_loop_203 = __element_iterator.first(__context__,(__element_rename_at_203_226)) && __need_loop_203;
            for ( ; __need_loop_203 ; __need_loop_203 = __element_iterator.next(__context__,(__element_rename_at_203_226)) )
            {
                _FuncarchiveTickserializeTick8013964856239694639_95732ecdb0114b69(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_189_221),das_arg<decs::EcsRequest>::pass((*__element_rename_at_203_226)));
            }
            __element_iterator.close(__context__,(__element_rename_at_203_226));
        };
    };
}

inline TArray<uint8_t> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_5aac315099132c4e ( Context * __context__, TArray<uint8_t> &  __a_rename_at_50_227 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_3,cast<TArray<uint8_t> &>::from(__a_rename_at_50_227)));
    return das_auto_cast_ref<TArray<uint8_t> &>::cast(__a_rename_at_50_227);
}

inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_f314c103a27e7f0c ( Context * __context__, TArray<uint8_t> &  __a_rename_at_32_228, TArray<uint8_t> &  __b_rename_at_32_229 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_4,cast<TArray<uint8_t> &>::from(__a_rename_at_32_228)));
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_5,cast<TArray<uint8_t> &>::from(__b_rename_at_32_229)));
    das_move(__a_rename_at_32_228,__b_rename_at_32_229);
}

inline void finalize_81b0d19e466110c8 ( Context * __context__, decs::DecsState &  ____this_rename_at_102_230 )
{
    _FuncbuiltinTickfinalizeTick5454204887383796109_7b1c641088884a8c(__context__,das_arg<TTable<uint64_t,int32_t>>::pass(____this_rename_at_102_230.archetypeLookup));
    _FuncbuiltinTickfinalizeTick13836114024949725080_ea18cb8662f307c1(__context__,das_arg<TArray<decs::Archetype>>::pass(____this_rename_at_102_230.allArchetypes));
    _FuncbuiltinTickfinalizeTick13836114024949725080_5fbae9e7389a3856(__context__,das_arg<TArray<decs::EntityId>>::pass(____this_rename_at_102_230.entityFreeList));
    _FuncbuiltinTickfinalizeTick13836114024949725080_ba685a3ffd2b9e77(__context__,das_arg<TArray<AutoTuple<int32_t,uint64_t,int32_t>>>::pass(____this_rename_at_102_230.entityLookup));
    _FuncbuiltinTickfinalizeTick5454204887383796109_1f7f0c3546fa051e(__context__,das_arg<TTable<char *,decs::CTypeInfo>>::pass(____this_rename_at_102_230.componentTypeCheck));
    _FuncbuiltinTickfinalizeTick13836114024949725080_9a747408a2a6d0ff(__context__,das_arg<TArray<decs::EcsRequest>>::pass(____this_rename_at_102_230.ecsQueries));
    _FuncbuiltinTickfinalizeTick5454204887383796109_7b1c641088884a8c(__context__,das_arg<TTable<uint64_t,int32_t>>::pass(____this_rename_at_102_230.queryLookup));
    memset(&(____this_rename_at_102_230), 0, TypeSize<decs::DecsState>::size);
}

inline void _FuncapplyTickstructTickDecsStateTick0x96Tick0Tick7_84c01a5244f15f9e ( Context * __context__, decs::DecsState &  ___Var_Tick_self_rename_at_62_231, Block DAS_COMMENT((void,TTable<uint64_t,int32_t>)) const  &  ____arg_archetypeLookup_rename_at_62_232, Block DAS_COMMENT((void,TArray<decs::Archetype>)) const  &  ____arg_allArchetypes_rename_at_62_233, Block DAS_COMMENT((void,TArray<decs::EntityId>)) const  &  ____arg_entityFreeList_rename_at_62_234, Block DAS_COMMENT((void,TArray<AutoTuple<int32_t,uint64_t,int32_t>>)) const  &  ____arg_entityLookup_rename_at_62_235, Block DAS_COMMENT((void,TTable<char *,decs::CTypeInfo>)) const  &  ____arg_componentTypeCheck_rename_at_62_236, Block DAS_COMMENT((void,TArray<decs::EcsRequest>)) const  &  ____arg_ecsQueries_rename_at_62_237, Block DAS_COMMENT((void,TTable<uint64_t,int32_t>)) const  &  ____arg_queryLookup_rename_at_62_238 )
{
    das_invoke<void>::invoke<TTable<uint64_t,int32_t> &>(__context__,nullptr,____arg_archetypeLookup_rename_at_62_232,das_arg<TTable<uint64_t,int32_t>>::pass(___Var_Tick_self_rename_at_62_231.archetypeLookup));
    das_invoke<void>::invoke<TArray<decs::Archetype> &>(__context__,nullptr,____arg_allArchetypes_rename_at_62_233,das_arg<TArray<decs::Archetype>>::pass(___Var_Tick_self_rename_at_62_231.allArchetypes));
    das_invoke<void>::invoke<TArray<decs::EntityId> &>(__context__,nullptr,____arg_entityFreeList_rename_at_62_234,das_arg<TArray<decs::EntityId>>::pass(___Var_Tick_self_rename_at_62_231.entityFreeList));
    das_invoke<void>::invoke<TArray<AutoTuple<int32_t,uint64_t,int32_t>> &>(__context__,nullptr,____arg_entityLookup_rename_at_62_235,das_arg<TArray<AutoTuple<int32_t,uint64_t,int32_t>>>::pass(___Var_Tick_self_rename_at_62_231.entityLookup));
    das_invoke<void>::invoke<TTable<char *,decs::CTypeInfo> &>(__context__,nullptr,____arg_componentTypeCheck_rename_at_62_236,das_arg<TTable<char *,decs::CTypeInfo>>::pass(___Var_Tick_self_rename_at_62_231.componentTypeCheck));
    das_invoke<void>::invoke<TArray<decs::EcsRequest> &>(__context__,nullptr,____arg_ecsQueries_rename_at_62_237,das_arg<TArray<decs::EcsRequest>>::pass(___Var_Tick_self_rename_at_62_231.ecsQueries));
    das_invoke<void>::invoke<TTable<uint64_t,int32_t> &>(__context__,nullptr,____arg_queryLookup_rename_at_62_238,das_arg<TTable<uint64_t,int32_t>>::pass(___Var_Tick_self_rename_at_62_231.queryLookup));
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_2333bfdfd8018176 ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_68_239, int32_t __newSize_rename_at_68_240 )
{
    builtin_array_resize(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_68_239),__newSize_rename_at_68_240,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_1b59bf517600dbc5 ( Context * __context__, TTable<char *,das::Time> const  &  __Tab_rename_at_1014_241, char * const  __at_rename_at_1014_242 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1014_241,__at_rename_at_1014_242));
}

inline das::Time _FuncbuiltinTickget_valueTick6803070933163225147_6f53cf5131c26ea6 ( Context * __context__, TTable<char *,das::Time> &  __Tab_rename_at_741_243, char * const  __at_rename_at_741_244 )
{
    return das_auto_cast<das::Time>::cast(__Tab_rename_at_741_243(__at_rename_at_741_244,__context__));
}

inline void _FuncbuiltinTickinsertTick4246857231018487965_53ce123b607bdf56 ( Context * __context__, TTable<char *,das::Time> &  __Tab_rename_at_939_245, char * const  __at_rename_at_939_246, das::Time __val_rename_at_939_247 )
{
    das_copy(__Tab_rename_at_939_245(__at_rename_at_939_246,__context__),__val_rename_at_939_247);
}

inline void _FuncarchiveTickserializeTick8013964856239694639_a9d1c2f2f9d21c11 ( Context * __context__, archive::Archive &  __arch_rename_at_146_248, decs::DecsState &  __value_rename_at_146_249 ) { das_stack_prologue __prologue(__context__,480,"archive`serialize`8013964856239694639 " DAS_FILE_LINE);
{
    if ( __arch_rename_at_146_248.reading )
    {
        finalize_81b0d19e466110c8(__context__,das_arg<decs::DecsState>::pass(__value_rename_at_146_249));
    };
    _FuncapplyTickstructTickDecsStateTick0x96Tick0Tick7_84c01a5244f15f9e(__context__,das_arg<decs::DecsState>::pass(__value_rename_at_146_249),das_make_block<void,TTable<uint64_t,int32_t> &>(__context__,80,0,&__func_info__e43d9eedb8768372,[&](TTable<uint64_t,int32_t> & __field_rename_at_150_250) -> void{
        _FuncarchiveTickserializeTick18117489833716462019_10e000d138e4ac0f(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_146_248),das_arg<TTable<uint64_t,int32_t>>::pass(__field_rename_at_150_250));
    }),das_make_block<void,TArray<decs::Archetype> &>(__context__,144,0,&__func_info__baddab894dba73e8,[&](TArray<decs::Archetype> & __field_rename_at_150_251) -> void{
        _FuncarchiveTickserializeTick1002677114065211290_339f3460927c659a(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_146_248),das_arg<TArray<decs::Archetype>>::pass(__field_rename_at_150_251));
    }),das_make_block<void,TArray<decs::EntityId> &>(__context__,208,0,&__func_info__2c3aa2fcec87dc8f,[&](TArray<decs::EntityId> & __field_rename_at_150_252) -> void{
        _FuncarchiveTickserializeTick1002677114065211290_2ebd95325aaa7a86(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_146_248),das_arg<TArray<decs::EntityId>>::pass(__field_rename_at_150_252));
    }),das_make_block<void,TArray<AutoTuple<int32_t,uint64_t,int32_t>> &>(__context__,272,0,&__func_info__1dcd71ef3006d28f,[&](TArray<AutoTuple<int32_t,uint64_t,int32_t>> & __field_rename_at_150_253) -> void{
        _FuncarchiveTickserializeTick1002677114065211290_7eba8f9beff4a0ef(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_146_248),das_arg<TArray<AutoTuple<int32_t,uint64_t,int32_t>>>::pass(__field_rename_at_150_253));
    }),das_make_block<void,TTable<char *,decs::CTypeInfo> &>(__context__,336,0,&__func_info__88667fa8dd2c245b,[&](TTable<char *,decs::CTypeInfo> & __field_rename_at_150_254) -> void{
        _FuncarchiveTickserializeTick18117489833716462019_82336b17b91a4160(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_146_248),das_arg<TTable<char *,decs::CTypeInfo>>::pass(__field_rename_at_150_254));
    }),das_make_block<void,TArray<decs::EcsRequest> &>(__context__,400,0,&__func_info__91507666520860cd,[&](TArray<decs::EcsRequest> & __field_rename_at_150_255) -> void{
        _FuncarchiveTickserializeTick1002677114065211290_94274d93de23df5(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_146_248),das_arg<TArray<decs::EcsRequest>>::pass(__field_rename_at_150_255));
    }),das_make_block<void,TTable<uint64_t,int32_t> &>(__context__,464,0,&__func_info__e43d9eedb8768372,[&](TTable<uint64_t,int32_t> & __field_rename_at_150_256) -> void{
        _FuncarchiveTickserializeTick18117489833716462019_10e000d138e4ac0f(__context__,das_arg<archive::Archive>::pass(__arch_rename_at_146_248),das_arg<TTable<uint64_t,int32_t>>::pass(__field_rename_at_150_256));
    }));
}}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_efda835c0359ec76 ( Context * __context__, TTable<char *,bool> const  &  __Tab_rename_at_1014_257, char * const  __at_rename_at_1014_258 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1014_257,__at_rename_at_1014_258));
}

inline void _FuncbuiltinTickinsertTick12964066441666329206_963edeee5917200b ( Context * __context__, TTable<char *,bool> &  __Tab_rename_at_947_259, char * const  __at_rename_at_947_260, bool __val_rename_at_947_261 )
{
    das_copy(__Tab_rename_at_947_259(__at_rename_at_947_260,__context__),__val_rename_at_947_261);
}

inline bool _FuncbuiltinTickget_valueTick6803070933163225147_c79cd68da0d29c45 ( Context * __context__, TTable<char *,bool> &  __Tab_rename_at_741_262, char * const  __at_rename_at_741_263 )
{
    return das_auto_cast<bool>::cast(__Tab_rename_at_741_262(__at_rename_at_741_263,__context__));
}

inline void finalize_801ffbcd37fa63c8 ( Context * __context__, live::_lambda_live_205_1 &  ____this_rename_at_205_264 )
{
    memset(&(____this_rename_at_205_264), 0, TypeSize<live::_lambda_live_205_1>::size);
}

inline void _FuncbuiltinTickcloneTick3038771811667655495_1107976c81aa121a ( Context * __context__, TArray<uint8_t> &  __a_rename_at_1080_265, TArray<uint8_t> const  &  __b_rename_at_1080_266 )
{
    int32_t __ln_rename_at_1081_267 = ((int32_t)builtin_array_size(__b_rename_at_1080_266));
    _FuncbuiltinTickresizeTick4811697762258667383_2333bfdfd8018176(__context__,das_arg<TArray<uint8_t>>::pass(__a_rename_at_1080_265),__ln_rename_at_1081_267);
    if ( __ln_rename_at_1081_267 == 0 )
    {
        return ;
    } else {
        das_memcpy(das_auto_cast<void * const >::cast(das_ref(__context__,__a_rename_at_1080_265(0,__context__))),das_auto_cast<void * const >::cast(das_ref(__context__,__b_rename_at_1080_266(0,__context__))),__ln_rename_at_1081_267 * 1);
    };
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb ( Context * __context__, TArray<uint8_t> &  __a_rename_at_1182_268 )
{
    builtin_array_free(das_arg<TArray<uint8_t>>::pass(__a_rename_at_1182_268),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void finalize_3e056b096d7418b6 ( Context * __context__, live::_lambda_live_225_2 &  ____this_rename_at_225_269 )
{
    memset(&(____this_rename_at_225_269), 0, TypeSize<live::_lambda_live_225_2>::size);
}

inline void finalize_82b0f923cb1a5c28 ( Context * __context__, live::_lambda_live_237_3 &  ____this_rename_at_237_270 )
{
    memset(&(____this_rename_at_237_270), 0, TypeSize<live::_lambda_live_237_3>::size);
}

inline uint64_t _FuncbuiltinTickintptrTick11320822648609276562_c1cab2e197c26e65 ( Context * __context__, void * const  __p_rename_at_15_271 )
{
    return das_auto_cast<uint64_t>::cast(das_cast<uint64_t>::cast(__p_rename_at_15_271));
}

inline void _FuncbuiltinTickcloneTick3038771811667655495_74d08f53d44ae194 ( Context * __context__, TArray<uint8_t> &  __a_rename_at_1080_272, TArray<uint8_t> const  &  __b_rename_at_1080_273 )
{
    int32_t __ln_rename_at_1081_274 = ((int32_t)builtin_array_size(__b_rename_at_1080_273));
    _FuncbuiltinTickresizeTick4811697762258667383_2333bfdfd8018176(__context__,das_arg<TArray<uint8_t>>::pass(__a_rename_at_1080_272),__ln_rename_at_1081_274);
    if ( __ln_rename_at_1081_274 == 0 )
    {
        return ;
    } else {
        das_memcpy(das_auto_cast<void * const >::cast(das_ref(__context__,__a_rename_at_1080_272(0,__context__))),das_auto_cast<void * const >::cast(das_ref(__context__,__b_rename_at_1080_273(0,__context__))),__ln_rename_at_1081_274 * 1);
    };
}

inline void _FuncbuiltinTickfinalizeTick5454204887383796109_9c4083566563a85a ( Context * __context__, TTable<char *,bool> &  __a_rename_at_1193_275 )
{
    builtin_table_free(das_arg<TTable<char *,bool>>::pass(__a_rename_at_1193_275),8,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline TArray<uint8_t> _FuncarchiveTickmem_archive_saveTick1217508036218320246_8093c94b06c5b1d3 ( Context * __context__, decs::DecsState &  __t_rename_at_261_276 )
{
    archive::MemSerializer * __mems_rename_at_263_277 = das_new<archive::MemSerializer>::make_and_init(__context__,[&]() { return das_invoke_function<archive::MemSerializer>::invoke_cmres(__context__,nullptr,Func(__context__->fnByMangledName(/*@archive::MemSerializer*/ 0x7d9e31723ea66247))); });
    archive::Archive __sarch_rename_at_264_278 = (([&]() -> archive::Archive {
        archive::Archive __mks_264;
        das_zero(__mks_264);
        das_copy((__mks_264.reading),(false));
        das_copy((__mks_264.stream),(__mems_rename_at_263_277));
        return __mks_264;
    })());
    _FuncarchiveTickserializeTick8013964856239694639_a9d1c2f2f9d21c11(__context__,das_arg<archive::Archive>::pass(__sarch_rename_at_264_278),das_arg<decs::DecsState>::pass(__t_rename_at_261_276));
    return /* <- */ das_auto_cast_move<TArray<uint8_t>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_5aac315099132c4e(__context__,das_arg<TArray<uint8_t>>::pass(das_invoke_method<TArray<uint8_t>,offsetof(archive::MemSerializer,extractData)>::invoke_cmres<archive::MemSerializer &>(__context__,nullptr,das_arg<archive::MemSerializer>::pass(das_cast<archive::MemSerializer>::cast(das_deref(__context__,__mems_rename_at_263_277)))))));
}

inline bool _FuncarchiveTickmem_archive_loadTick9860235419630369707_520cdbc5db538a78 ( Context * __context__, TArray<uint8_t> &  __data_rename_at_269_279, decs::DecsState &  __t_rename_at_269_280, bool __canfail_rename_at_269_281 )
{
    archive::MemSerializer * __memr_rename_at_271_282 = das_new<archive::MemSerializer>::make_and_init(__context__,[&]() { return das_invoke_function<archive::MemSerializer>::invoke_cmres<TArray<uint8_t> const  &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@archive::MemSerializer C1<u8>A*/ 0x64b988c1cab1fe9b)),das_arg<TArray<uint8_t>>::pass(__data_rename_at_269_279)); });
    archive::Archive __rarch_rename_at_272_283 = (([&]() -> archive::Archive {
        archive::Archive __mks_272;
        das_zero(__mks_272);
        das_copy((__mks_272.reading),(true));
        das_copy((__mks_272.stream),(__memr_rename_at_271_282));
        return __mks_272;
    })());
    _FuncarchiveTickserializeTick8013964856239694639_a9d1c2f2f9d21c11(__context__,das_arg<archive::Archive>::pass(__rarch_rename_at_272_283),das_arg<decs::DecsState>::pass(__t_rename_at_269_280));
    if ( !das_invoke_method<bool,offsetof(archive::Serializer,OK)>::invoke<archive::Serializer &>(__context__,nullptr,das_arg<archive::Serializer>::pass(das_cast<archive::Serializer>::cast(das_deref(__context__,__memr_rename_at_271_282)))) )
    {
        if ( !__canfail_rename_at_269_281 )
        {
            builtin_throw(das_invoke_method<char * const ,offsetof(archive::MemSerializer,getLastError)>::invoke<archive::MemSerializer &>(__context__,nullptr,das_arg<archive::MemSerializer>::pass(das_cast<archive::MemSerializer>::cast(das_deref(__context__,__memr_rename_at_271_282)))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
        return das_auto_cast<bool>::cast(false);
    } else {
        return das_auto_cast<bool>::cast(true);
    };
}

inline void clone_bccc36eddc052dcb ( Context * __context__, smart_ptr_raw<Context> & __dest_rename_at_61_284, void * const  __src_rename_at_61_285 )
{
    builtin_smart_ptr_clone_ptr(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_61_284),__src_rename_at_61_285,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void clone_5a60439f7fb04300 ( Context * __context__, smart_ptr_raw<Context> & __dest_rename_at_78_286, smart_ptr_raw<Context> const  __src_rename_at_78_287 )
{
    builtin_smart_ptr_clone(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_78_286),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_rename_at_78_287),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline bool _FuncliveTicklive_lookupTick637029202016062135_11576884b544c838 ( Context * __context__, char * const  __fn_rename_at_92_288 )
{
    if ( equ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/),das_auto_cast<void * const >::cast(nullptr)) )
    {
        return das_auto_cast<bool>::cast(false);
    } else {
        if ( !_FuncbuiltinTickkey_existsTick16808803843923989214_efda835c0359ec76(__context__,das_arg<TTable<char *,bool>>::pass(das_global<TTable<char *,bool>,0x7955fcc436037958>(__context__) /*liveFunctionLookup*/),__fn_rename_at_92_288) )
        {
            _FuncbuiltinTickinsertTick12964066441666329206_963edeee5917200b(__context__,das_arg<TTable<char *,bool>>::pass(das_global<TTable<char *,bool>,0x7955fcc436037958>(__context__) /*liveFunctionLookup*/),__fn_rename_at_92_288,has_function(das_arg<Context>::pass(das_deref(__context__,das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/)),__fn_rename_at_92_288));
        };
        return das_auto_cast<bool>::cast(_FuncbuiltinTickget_valueTick6803070933163225147_c79cd68da0d29c45(__context__,das_arg<TTable<char *,bool>>::pass(das_global<TTable<char *,bool>,0x7955fcc436037958>(__context__) /*liveFunctionLookup*/),__fn_rename_at_92_288));
    };
}

inline void _FuncfioTickdirTick17131698203006957834_186fcc74e85e93aa ( Context * __context__, char * const  __path_rename_at_21_289, Block DAS_COMMENT((void,char * const )) const  &  __blk_rename_at_21_290 )
{
    builtin_dir(__path_rename_at_21_289,__blk_rename_at_21_290,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline Context * _FuncbuiltinTickget_ptrTick5807679485210906136_91295c6251ff26c5 ( Context * __context__, smart_ptr_raw<Context> __src_rename_at_1732_291 )
{
    return das_auto_cast<Context *>::cast(das_cast<Context *>::cast(__src_rename_at_1732_291));
}

inline void _Func_lambda_live_205_1Tickfunction_b3b7a30c2cb432a6 ( Context * __context__, live::_lambda_live_205_1 &  ____this_rename_at_205_292 )
{
    saveLiveContext_efcae6d209280047(__context__);
}

inline void _Func_lambda_live_205_1Tickfinalizer_88431e65ddbac842 ( Context * __context__, live::_lambda_live_205_1 * ____this_rename_at_205_293 )
{
    finalize_801ffbcd37fa63c8(__context__,das_arg<live::_lambda_live_205_1>::pass(das_deref(__context__,____this_rename_at_205_293)));
    das_delete_lambda_struct<live::_lambda_live_205_1 *>::clear(__context__,____this_rename_at_205_293);
}

inline void _Func_lambda_live_225_2Tickfunction_2c0f2ecd65895fe4 ( Context * __context__, live::_lambda_live_225_2 &  ____this_rename_at_225_294, TArray<uint8_t> const  &  __data_rename_at_225_295 )
{
    _FuncbuiltinTickcloneTick3038771811667655495_1107976c81aa121a(__context__,das_arg<TArray<uint8_t>>::pass(das_global<TArray<uint8_t>,0x4b711ceb6d62f997>(__context__) /*decsLiveData*/),__data_rename_at_225_295);
    restoreLiveContext_49c6d9153d13659f(__context__);
    _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb(__context__,das_arg<TArray<uint8_t>>::pass(das_global<TArray<uint8_t>,0x4b711ceb6d62f997>(__context__) /*decsLiveData*/));
}

inline void _Func_lambda_live_225_2Tickfinalizer_4b5c91408d22a4a6 ( Context * __context__, live::_lambda_live_225_2 * ____this_rename_at_225_296 )
{
    finalize_3e056b096d7418b6(__context__,das_arg<live::_lambda_live_225_2>::pass(das_deref(__context__,____this_rename_at_225_296)));
    das_delete_lambda_struct<live::_lambda_live_225_2 *>::clear(__context__,____this_rename_at_225_296);
}

inline void _Func_lambda_live_237_3Tickfunction_1dd03a040db0eba6 ( Context * __context__, live::_lambda_live_237_3 &  ____this_rename_at_237_297, decs::DecsState * const  __state_rename_at_237_298 )
{
    das_move(das_global<smart_ptr_raw<Context>,0xec8e0452d46569f1>(__context__) /*liveContext*/,____this_rename_at_237_297.lContext);
}

inline void _Func_lambda_live_237_3Tickfinalizer_7c78462166aa88ea ( Context * __context__, live::_lambda_live_237_3 * ____this_rename_at_237_299 )
{
    finalize_82b0f923cb1a5c28(__context__,das_arg<live::_lambda_live_237_3>::pass(das_deref(__context__,____this_rename_at_237_299)));
    das_delete_lambda_struct<live::_lambda_live_237_3 *>::clear(__context__,____this_rename_at_237_299);
}

inline void _FuncliveTickinvoke_liveTick12947590458897769171_8b2be572fbf2e075 ( Context * __context__, char * const  __fn_rename_at_115_300, bool __a1_rename_at_115_301 )
{
    if ( _FuncliveTicklive_lookupTick637029202016062135_11576884b544c838(__context__,__fn_rename_at_115_300) )
    {
        das_try_recover(__context__, [&]()
        {
            das_call_interop<void>::call(&pinvoke_impl,__context__,SimNode_AotInterop<3>(__tinfo_6,cast<smart_ptr_raw<Context>>::from(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/),cast<char * const >::from(__fn_rename_at_115_300),cast<bool>::from(__a1_rename_at_115_301)));
        }, [&]()
        {
            toLog(40000,das_string_builder_temp(__context__,SimNode_AotInterop<9>(__tinfo_7, cast<char * const >::from(((char *) "exception during '")), cast<char * const >::from(__fn_rename_at_115_300), cast<char * const >::from(((char *) "(")), cast<bool>::from(__a1_rename_at_115_301), cast<char * const >::from(((char *) ")'\n")), cast<char * const >::from(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/->last_exception /*last_exception*/), cast<char * const >::from(((char *) "\n")), cast<char * const >::from(((char * const )(builtin_debug_line(das_arg<LineInfo>::pass(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/->exceptionAt /*exceptionAt*/),false,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            clone_bccc36eddc052dcb(__context__,das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/,das_auto_cast<void * const >::cast(nullptr));
        });
    };
}

inline void _FuncliveTickinvoke_liveTick2886249264218566728_c3b9177bd0f8a8bc ( Context * __context__, char * const  __fn_rename_at_154_302, int32_t __a1_rename_at_154_303, int32_t __a2_rename_at_154_304, int32_t __a3_rename_at_154_305, int32_t __a4_rename_at_154_306 )
{
    if ( _FuncliveTicklive_lookupTick637029202016062135_11576884b544c838(__context__,__fn_rename_at_154_302) )
    {
        das_try_recover(__context__, [&]()
        {
            das_call_interop<void>::call(&pinvoke_impl,__context__,SimNode_AotInterop<6>(__tinfo_8,cast<smart_ptr_raw<Context>>::from(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/),cast<char * const >::from(__fn_rename_at_154_302),cast<int32_t>::from(__a1_rename_at_154_303),cast<int32_t>::from(__a2_rename_at_154_304),cast<int32_t>::from(__a3_rename_at_154_305),cast<int32_t>::from(__a4_rename_at_154_306)));
        }, [&]()
        {
            toLog(40000,das_string_builder_temp(__context__,SimNode_AotInterop<15>(__tinfo_9, cast<char * const >::from(((char *) "exception during '")), cast<char * const >::from(__fn_rename_at_154_302), cast<char * const >::from(((char *) "(")), cast<int32_t>::from(__a1_rename_at_154_303), cast<char * const >::from(((char *) ",")), cast<int32_t>::from(__a2_rename_at_154_304), cast<char * const >::from(((char *) ",")), cast<int32_t>::from(__a3_rename_at_154_305), cast<char * const >::from(((char *) ",")), cast<int32_t>::from(__a4_rename_at_154_306), cast<char * const >::from(((char *) ")'\n")), cast<char * const >::from(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/->last_exception /*last_exception*/), cast<char * const >::from(((char *) "\n")), cast<char * const >::from(((char * const )(builtin_debug_line(das_arg<LineInfo>::pass(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/->exceptionAt /*exceptionAt*/),false,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            clone_bccc36eddc052dcb(__context__,das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/,das_auto_cast<void * const >::cast(nullptr));
        });
    };
}

inline void _FuncliveTickinvoke_liveTick12947590458897769171_8571e9b30efbd8d7 ( Context * __context__, char * const  __fn_rename_at_115_307, uint32_t __a1_rename_at_115_308 )
{
    if ( _FuncliveTicklive_lookupTick637029202016062135_11576884b544c838(__context__,__fn_rename_at_115_307) )
    {
        das_try_recover(__context__, [&]()
        {
            das_call_interop<void>::call(&pinvoke_impl,__context__,SimNode_AotInterop<3>(__tinfo_10,cast<smart_ptr_raw<Context>>::from(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/),cast<char * const >::from(__fn_rename_at_115_307),cast<uint32_t>::from(__a1_rename_at_115_308)));
        }, [&]()
        {
            toLog(40000,das_string_builder_temp(__context__,SimNode_AotInterop<9>(__tinfo_11, cast<char * const >::from(((char *) "exception during '")), cast<char * const >::from(__fn_rename_at_115_307), cast<char * const >::from(((char *) "(")), cast<uint32_t>::from(__a1_rename_at_115_308), cast<char * const >::from(((char *) ")'\n")), cast<char * const >::from(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/->last_exception /*last_exception*/), cast<char * const >::from(((char *) "\n")), cast<char * const >::from(((char * const )(builtin_debug_line(das_arg<LineInfo>::pass(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/->exceptionAt /*exceptionAt*/),false,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            clone_bccc36eddc052dcb(__context__,das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/,das_auto_cast<void * const >::cast(nullptr));
        });
    };
}

inline void _FuncliveTickinvoke_liveTick8381256433725501221_ebcbd4eac3367a58 ( Context * __context__, char * const  __fn_rename_at_128_309, double __a1_rename_at_128_310, double __a2_rename_at_128_311 )
{
    if ( _FuncliveTicklive_lookupTick637029202016062135_11576884b544c838(__context__,__fn_rename_at_128_309) )
    {
        das_try_recover(__context__, [&]()
        {
            das_call_interop<void>::call(&pinvoke_impl,__context__,SimNode_AotInterop<4>(__tinfo_12,cast<smart_ptr_raw<Context>>::from(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/),cast<char * const >::from(__fn_rename_at_128_309),cast<double>::from(__a1_rename_at_128_310),cast<double>::from(__a2_rename_at_128_311)));
        }, [&]()
        {
            toLog(40000,das_string_builder_temp(__context__,SimNode_AotInterop<11>(__tinfo_13, cast<char * const >::from(((char *) "exception during '")), cast<char * const >::from(__fn_rename_at_128_309), cast<char * const >::from(((char *) "(")), cast<double>::from(__a1_rename_at_128_310), cast<char * const >::from(((char *) ",")), cast<double>::from(__a2_rename_at_128_311), cast<char * const >::from(((char *) ")'\n")), cast<char * const >::from(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/->last_exception /*last_exception*/), cast<char * const >::from(((char *) "\n")), cast<char * const >::from(((char * const )(builtin_debug_line(das_arg<LineInfo>::pass(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/->exceptionAt /*exceptionAt*/),false,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            clone_bccc36eddc052dcb(__context__,das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/,das_auto_cast<void * const >::cast(nullptr));
        });
    };
}

inline void _FuncliveTickinvoke_liveTick16287916205268058194_beee970120602d17 ( Context * __context__, char * const  __fn_rename_at_141_312, int32_t __a1_rename_at_141_313, int32_t __a2_rename_at_141_314, int32_t __a3_rename_at_141_315 )
{
    if ( _FuncliveTicklive_lookupTick637029202016062135_11576884b544c838(__context__,__fn_rename_at_141_312) )
    {
        das_try_recover(__context__, [&]()
        {
            das_call_interop<void>::call(&pinvoke_impl,__context__,SimNode_AotInterop<5>(__tinfo_14,cast<smart_ptr_raw<Context>>::from(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/),cast<char * const >::from(__fn_rename_at_141_312),cast<int32_t>::from(__a1_rename_at_141_313),cast<int32_t>::from(__a2_rename_at_141_314),cast<int32_t>::from(__a3_rename_at_141_315)));
        }, [&]()
        {
            toLog(40000,das_string_builder_temp(__context__,SimNode_AotInterop<13>(__tinfo_15, cast<char * const >::from(((char *) "exception during '")), cast<char * const >::from(__fn_rename_at_141_312), cast<char * const >::from(((char *) "(")), cast<int32_t>::from(__a1_rename_at_141_313), cast<char * const >::from(((char *) ",")), cast<int32_t>::from(__a2_rename_at_141_314), cast<char * const >::from(((char *) ",")), cast<int32_t>::from(__a3_rename_at_141_315), cast<char * const >::from(((char *) ")'\n")), cast<char * const >::from(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/->last_exception /*last_exception*/), cast<char * const >::from(((char *) "\n")), cast<char * const >::from(((char * const )(builtin_debug_line(das_arg<LineInfo>::pass(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/->exceptionAt /*exceptionAt*/),false,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            clone_bccc36eddc052dcb(__context__,das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/,das_auto_cast<void * const >::cast(nullptr));
        });
    };
}

inline void _FuncliveTickinvoke_liveTick12947590458897769171_1753e685298c5c2a ( Context * __context__, char * const  __fn_rename_at_115_316, float __a1_rename_at_115_317 )
{
    if ( _FuncliveTicklive_lookupTick637029202016062135_11576884b544c838(__context__,__fn_rename_at_115_316) )
    {
        das_try_recover(__context__, [&]()
        {
            das_call_interop<void>::call(&pinvoke_impl,__context__,SimNode_AotInterop<3>(__tinfo_16,cast<smart_ptr_raw<Context>>::from(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/),cast<char * const >::from(__fn_rename_at_115_316),cast<float>::from(__a1_rename_at_115_317)));
        }, [&]()
        {
            toLog(40000,das_string_builder_temp(__context__,SimNode_AotInterop<9>(__tinfo_17, cast<char * const >::from(((char *) "exception during '")), cast<char * const >::from(__fn_rename_at_115_316), cast<char * const >::from(((char *) "(")), cast<float>::from(__a1_rename_at_115_317), cast<char * const >::from(((char *) ")'\n")), cast<char * const >::from(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/->last_exception /*last_exception*/), cast<char * const >::from(((char *) "\n")), cast<char * const >::from(((char * const )(builtin_debug_line(das_arg<LineInfo>::pass(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/->exceptionAt /*exceptionAt*/),false,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            clone_bccc36eddc052dcb(__context__,das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/,das_auto_cast<void * const >::cast(nullptr));
        });
    };
}

inline smart_ptr_raw<Context> get_live_context_e44470f3748ec318 ( Context * __context__ )
{
    return das_auto_cast<smart_ptr_raw<Context>>::cast(das_global<smart_ptr_raw<Context>,0xec8e0452d46569f1>(__context__) /*liveContext*/);
}

inline void saveLiveContext_efcae6d209280047 ( Context * __context__ )
{
    TArray<uint8_t> _temp_make_local_43_20_32; _temp_make_local_43_20_32;
    toLog(0,((char *) "LIVE: saveLiveContext\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_f314c103a27e7f0c(__context__,das_arg<TArray<uint8_t>>::pass(das_global<TArray<uint8_t>,0x4b711ceb6d62f997>(__context__) /*decsLiveData*/),das_arg<TArray<uint8_t>>::pass((_temp_make_local_43_20_32 = (_FuncarchiveTickmem_archive_saveTick1217508036218320246_8093c94b06c5b1d3(__context__,das_arg<decs::DecsState>::pass(das_global<decs::DecsState,0xca9708f97d9e3208>(__context__) /*decsState*/))))));
}

inline void restoreLiveContext_49c6d9153d13659f ( Context * __context__ )
{
    toLog(0,((char *) "LIVE: restoreLiveContext\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( builtin_array_size(das_arg<TArray<uint8_t>>::pass(das_global<TArray<uint8_t>,0x4b711ceb6d62f997>(__context__) /*decsLiveData*/)) > 0 )
    {
        das_try_recover(__context__, [&]()
        {
            _FuncarchiveTickmem_archive_loadTick9860235419630369707_520cdbc5db538a78(__context__,das_arg<TArray<uint8_t>>::pass(das_global<TArray<uint8_t>,0x4b711ceb6d62f997>(__context__) /*decsLiveData*/),das_arg<decs::DecsState>::pass(das_global<decs::DecsState,0xca9708f97d9e3208>(__context__) /*decsState*/),false);
        }, [&]()
        {
            toLog(40000,((char *) "exception during restoreLiveContext\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        });
    };
}

inline void go_offline_778dbe0dd3b703b8 ( Context * __context__ )
{
    toLog(0,((char *) "LIVE: go_offline\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    clone_bccc36eddc052dcb(__context__,das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/,das_auto_cast<void * const >::cast(nullptr));
}

inline bool is_live_92bab00e5c617532 ( Context * __context__ )
{
    return das_auto_cast<bool>::cast(das_global<bool,0x67076871139e3abe>(__context__) /*appLive*/);
}

inline bool is_app_live_b15dd0f7bf4a041d ( Context * __context__ )
{
    return das_auto_cast<bool>::cast(nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/),das_auto_cast<void * const >::cast(nullptr)));
}

inline void go_main_b78776d702f7ae9b ( Context * __context__ )
{
    toLog(0,((char *) "LIVE: go_main\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb(__context__,das_arg<TArray<uint8_t>>::pass(das_global<TArray<uint8_t>,0x4b711ceb6d62f997>(__context__) /*decsLiveData*/));
    das_copy(das_global<bool,0x67076871139e3abe>(__context__) /*appLive*/,false);
    smart_ptr_raw<Context> __ctx_rename_at_77_318; das_zero(__ctx_rename_at_77_318); das_move(__ctx_rename_at_77_318, das_cast<smart_ptr_raw<Context>>::cast(das_ref(__context__,thisContext(__context__))));
    clone_5a60439f7fb04300(__context__,das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/,__ctx_rename_at_77_318);
}

inline void go_live_b8a103eaa74d382f ( Context * __context__, char * const  __appf_rename_at_82_319, char * const  __appd_rename_at_82_320 )
{
    toLog(0,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_18, cast<char * const >::from(((char *) "LIVE: go_life appFile=")), cast<char * const >::from(__appf_rename_at_82_319), cast<char * const >::from(((char *) " appDir=")), cast<char * const >::from(__appd_rename_at_82_320), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    das_copy(das_global<bool,0x67076871139e3abe>(__context__) /*appLive*/,true);
    set_das_string(das_arg<das::string>::pass(thisContext(__context__).name /*name*/),((char *) "Live context"));
    das_copy(das_global<char *,0x86e5ae912531f3a1>(__context__) /*appFile*/,__appf_rename_at_82_319);
    das_copy(das_global<char *,0xb0f39eaf391fc167>(__context__) /*appDir*/,__appd_rename_at_82_320);
    _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb(__context__,das_arg<TArray<uint8_t>>::pass(das_global<TArray<uint8_t>,0x4b711ceb6d62f997>(__context__) /*decsLiveData*/));
    recompile_18a8bac272d4f62d(__context__,false);
}

inline void invoke_live_94051986a48f719e ( Context * __context__, char * const  __fn_rename_at_102_321 )
{
    if ( _FuncliveTicklive_lookupTick637029202016062135_11576884b544c838(__context__,__fn_rename_at_102_321) )
    {
        das_try_recover(__context__, [&]()
        {
            das_call_interop<void>::call(&pinvoke_impl,__context__,SimNode_AotInterop<2>(__tinfo_19,cast<smart_ptr_raw<Context>>::from(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/),cast<char * const >::from(__fn_rename_at_102_321)));
        }, [&]()
        {
            toLog(40000,das_string_builder_temp(__context__,SimNode_AotInterop<7>(__tinfo_20, cast<char * const >::from(((char *) "exception during '")), cast<char * const >::from(__fn_rename_at_102_321), cast<char * const >::from(((char *) "()'\n")), cast<char * const >::from(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/->last_exception /*last_exception*/), cast<char * const >::from(((char *) "\n")), cast<char * const >::from(((char * const )(builtin_debug_line(das_arg<LineInfo>::pass(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/->exceptionAt /*exceptionAt*/),false,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            clone_bccc36eddc052dcb(__context__,das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/,das_auto_cast<void * const >::cast(nullptr));
        });
    };
}

inline bool watch_files_cfe33bf10ac8df9b ( Context * __context__ ) { das_stack_prologue __prologue(__context__,304,"watch_files " DAS_FILE_LINE);
{
    das::Time __clk_rename_at_168_322 = /*c-tor*/ builtin_clock(/*end-c-tor*/);
    if ( (time_sub(__clk_rename_at_168_322,das_global<das::Time,0xe2c0ab7b4123c2d9>(__context__) /*watchTime*/)) <= 0.000000e+00 )
    {
        return das_auto_cast<bool>::cast(false);
    } else {
        das_copy(das_global<das::Time,0xe2c0ab7b4123c2d9>(__context__) /*watchTime*/,__clk_rename_at_168_322);
        bool __any_rename_at_173_323 = false;
        _FuncfioTickdirTick17131698203006957834_186fcc74e85e93aa(__context__,das_global<char *,0xb0f39eaf391fc167>(__context__) /*appDir*/,das_make_block<void,char * const >(__context__,112,0,&__func_info__a4e792ea11638c41,[&](char * const  __fname_rename_at_174_324) -> void{
            if ( !(builtin_string_endswith(__fname_rename_at_174_324,((char *) ".das"),__context__) || builtin_string_endswith(__fname_rename_at_174_324,((char *) ".das_project"),__context__)) )
            {
                return ;
            };
            char * __fileName_rename_at_178_325 = ((char *)(char *)(das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_21, cast<char *>::from(das_global<char *,0xb0f39eaf391fc167>(__context__) /*appDir*/), cast<char * const >::from(((char *) "/")), cast<char * const >::from(__fname_rename_at_174_324)))));
            FStat __st_rename_at_179_326; das_zero(__st_rename_at_179_326);
            if ( builtin_stat(__fileName_rename_at_178_325,das_arg<FStat>::pass(__st_rename_at_179_326)) )
            {
                if ( ((__st_rename_at_179_326).is_reg()) )
                {
                    if ( _FuncbuiltinTickkey_existsTick16808803843923989214_1b59bf517600dbc5(__context__,das_arg<TTable<char *,das::Time>>::pass(das_global<TTable<char *,das::Time>,0xfca7703c86890e7>(__context__) /*appTime*/),__fname_rename_at_174_324) )
                    {
                        if ( time_nequal(_FuncbuiltinTickget_valueTick6803070933163225147_6f53cf5131c26ea6(__context__,das_arg<TTable<char *,das::Time>>::pass(das_global<TTable<char *,das::Time>,0xfca7703c86890e7>(__context__) /*appTime*/),__fname_rename_at_174_324),((__st_rename_at_179_326).mtime())) )
                        {
                            das_copy(__any_rename_at_173_323,true);
                        };
                    };
                    _FuncbuiltinTickinsertTick4246857231018487965_53ce123b607bdf56(__context__,das_arg<TTable<char *,das::Time>>::pass(das_global<TTable<char *,das::Time>,0xfca7703c86890e7>(__context__) /*appTime*/),__fname_rename_at_174_324,((__st_rename_at_179_326).mtime()));
                };
            };
        }));
        return das_auto_cast<bool>::cast(__any_rename_at_173_323);
    };
}}

inline void set_new_context_a7a4593ab51c9d0c ( Context * __context__, smart_ptr_raw<Context> __ptr_rename_at_195_327, bool __full_restart_rename_at_195_328 )
{
    toLog(0,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_22, cast<char * const >::from(((char *) "LIVE: set new context ")), cast<uint64_t>::from(_FuncbuiltinTickintptrTick11320822648609276562_c1cab2e197c26e65(__context__,das_auto_cast<void * const >::cast(_FuncbuiltinTickget_ptrTick5807679485210906136_91295c6251ff26c5(__context__,__ptr_rename_at_195_327)))), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    invoke_live_94051986a48f719e(__context__,((char *) "shutdown"));
    if ( nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/),das_auto_cast<void * const >::cast(nullptr)) )
    {
        void * __pvar_rename_at_201_329 = das_call_interop<void *>::call(&get_global_variable,__context__,SimNode_AotInterop<2>(__tinfo_23,cast<smart_ptr_raw<Context>>::from(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/),cast<char * const >::from(((char *) "decsState"))));
        if ( __pvar_rename_at_201_329 != nullptr )
        {
            TArray<uint8_t> * __pstate_rename_at_203_330 = das_cast<TArray<uint8_t> *>::cast(das_call_interop<void *>::call(&get_global_variable,__context__,SimNode_AotInterop<2>(__tinfo_24,cast<smart_ptr_raw<Context>>::from(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/),cast<char * const >::from(((char *) "decsLiveData")))));
            DAS_ASSERT((__pstate_rename_at_203_330 != nullptr));
            Lambda DAS_COMMENT((void)) __css_rename_at_205_331; das_zero(__css_rename_at_205_331); das_move(__css_rename_at_205_331, das_ascend<void,live::_lambda_live_205_1>::make(__context__,&__type_info__8d785db1354cca5f,(([&]() -> live::_lambda_live_205_1 {
                live::_lambda_live_205_1 __mks_205;
                das_copy((__mks_205.__lambda),(Func(__context__->fnByMangledName(/*@live::_lambda_live_205_1`function XS<live::_lambda_live_205_1>*/ 0x951ba59be467efbc))));
                das_copy((__mks_205.__finalize),(Func(__context__->fnByMangledName(/*@live::_lambda_live_205_1`finalizer X1<S<live::_lambda_live_205_1>>?*/ 0xb0aa18ace305983f))));
                return __mks_205;
            })())));
            das_call_interop<void>::call(&pinvoke_impl3,__context__,SimNode_AotInterop<2>(__tinfo_25,cast<smart_ptr_raw<Context>>::from(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/),cast<Lambda DAS_COMMENT((void))>::from(__css_rename_at_205_331)));
            _FuncbuiltinTickcloneTick3038771811667655495_74d08f53d44ae194(__context__,das_arg<TArray<uint8_t>>::pass(das_global<TArray<uint8_t>,0x4b711ceb6d62f997>(__context__) /*decsLiveData*/),das_arg<TArray<uint8_t>>::pass(das_deref(__context__,__pstate_rename_at_203_330)));
            das_delete<Lambda DAS_COMMENT((void))>::clear(__context__,__css_rename_at_205_331);
        };
    };
    clone_5a60439f7fb04300(__context__,das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/,__ptr_rename_at_195_327);
    _FuncbuiltinTickfinalizeTick5454204887383796109_9c4083566563a85a(__context__,das_arg<TTable<char *,bool>>::pass(das_global<TTable<char *,bool>,0x7955fcc436037958>(__context__) /*liveFunctionLookup*/));
    if ( nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/),das_auto_cast<void * const >::cast(nullptr)) )
    {
        das_copy(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/->alwaysErrorOnException /*alwaysErrorOnException*/,true);
        das_copy(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/->alwaysStackWalkOnException /*alwaysStackWalkOnException*/,true);
        void * __pvar_rename_at_221_332 = das_call_interop<void *>::call(&get_global_variable,__context__,SimNode_AotInterop<2>(__tinfo_26,cast<smart_ptr_raw<Context>>::from(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/),cast<char * const >::from(((char *) "decsState"))));
        if ( __pvar_rename_at_221_332 != nullptr )
        {
            TArray<uint8_t> * __pstate_rename_at_223_333 = das_cast<TArray<uint8_t> *>::cast(das_call_interop<void *>::call(&get_global_variable,__context__,SimNode_AotInterop<2>(__tinfo_27,cast<smart_ptr_raw<Context>>::from(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/),cast<char * const >::from(((char *) "decsLiveData")))));
            DAS_ASSERT((__pstate_rename_at_223_333 != nullptr));
            Lambda DAS_COMMENT((void,TArray<uint8_t> const )) __css_rename_at_225_334; das_zero(__css_rename_at_225_334); das_move(__css_rename_at_225_334, das_ascend<void,live::_lambda_live_225_2>::make(__context__,&__type_info__dd290bb17de94649,(([&]() -> live::_lambda_live_225_2 {
                live::_lambda_live_225_2 __mks_225;
                das_copy((__mks_225.__lambda),(Func(__context__->fnByMangledName(/*@live::_lambda_live_225_2`function XS<live::_lambda_live_225_2> C#1<u8>A*/ 0x8aa434c8a4fe903a))));
                das_copy((__mks_225.__finalize),(Func(__context__->fnByMangledName(/*@live::_lambda_live_225_2`finalizer X1<S<live::_lambda_live_225_2>>?*/ 0xe405124b6d89cc2f))));
                return __mks_225;
            })())));
            das_call_interop<void>::call(&pinvoke_impl3,__context__,SimNode_AotInterop<3>(__tinfo_28,cast<smart_ptr_raw<Context>>::from(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/),cast<Lambda DAS_COMMENT((void,TArray<uint8_t> const ))>::from(__css_rename_at_225_334),cast<TArray<uint8_t> &>::from(das_global<TArray<uint8_t>,0x4b711ceb6d62f997>(__context__) /*decsLiveData*/)));
            das_delete<Lambda DAS_COMMENT((void,TArray<uint8_t> const ))>::clear(__context__,__css_rename_at_225_334);
        };
        void * __pctx_var_rename_at_234_335 = das_call_interop<void *>::call(&get_global_variable,__context__,SimNode_AotInterop<2>(__tinfo_29,cast<smart_ptr_raw<Context>>::from(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/),cast<char * const >::from(((char *) "liveContext"))));
        if ( __pctx_var_rename_at_234_335 != nullptr )
        {
            smart_ptr_raw<Context> __lContext_rename_at_236_336; das_zero(__lContext_rename_at_236_336); das_move(__lContext_rename_at_236_336, das_cast<smart_ptr_raw<Context>>::cast(das_ref(__context__,thisContext(__context__))));
            Lambda DAS_COMMENT((void,decs::DecsState * const )) __css_rename_at_237_337; das_zero(__css_rename_at_237_337); das_move(__css_rename_at_237_337, das_ascend<void,live::_lambda_live_237_3>::make(__context__,&__type_info__bc41b2a6fc7fa7ce,(([&]() -> live::_lambda_live_237_3 {
                live::_lambda_live_237_3 __mks_237;
                das_copy((__mks_237.__lambda),(Func(__context__->fnByMangledName(/*@live::_lambda_live_237_3`function XS<live::_lambda_live_237_3> C1<S<decs::DecsState>>?*/ 0x1e067235f39c4e03))));
                das_copy((__mks_237.__finalize),(Func(__context__->fnByMangledName(/*@live::_lambda_live_237_3`finalizer X1<S<live::_lambda_live_237_3>>?*/ 0x650d62850c745ab7))));
                das_move((__mks_237.lContext),(__lContext_rename_at_236_336));
                return __mks_237;
            })())));
            das_call_interop<void>::call(&pinvoke_impl3,__context__,SimNode_AotInterop<3>(__tinfo_30,cast<smart_ptr_raw<Context>>::from(das_global<smart_ptr_raw<Context>,0x55a761c45f683855>(__context__) /*appPtr*/),cast<Lambda DAS_COMMENT((void,decs::DecsState * const ))>::from(__css_rename_at_237_337),cast<decs::DecsState *>::from(das_ref(__context__,das_global<decs::DecsState,0xca9708f97d9e3208>(__context__) /*decsState*/))));
            das_delete<Lambda DAS_COMMENT((void,decs::DecsState * const ))>::clear(__context__,__css_rename_at_237_337);
        };
        _FuncliveTickinvoke_liveTick12947590458897769171_8b2be572fbf2e075(__context__,((char *) "initialize"),__full_restart_rename_at_195_328);
    };
}

inline void recompile_18a8bac272d4f62d ( Context * __context__, bool __full_restart_rename_at_250_338 ) { das_stack_prologue __prologue(__context__,304,"recompile " DAS_FILE_LINE);
{
    smart_ptr_raw<FileAccess> __access_rename_at_252_339; memset(&__access_rename_at_252_339,0,sizeof(__access_rename_at_252_339));
    /* finally */ auto __finally_250= das_finally([&](){
    das_delete_handle<smart_ptr_raw<FileAccess>>::clear(__context__,__access_rename_at_252_339);
    /* end finally */ });
    toLog(0,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_31, cast<char * const >::from(((char *) "LIVE: recompile full_restart=")), cast<bool>::from(__full_restart_rename_at_250_338), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    __access_rename_at_252_339; das_zero(__access_rename_at_252_339); das_move(__access_rename_at_252_339, makeFileAccess(nullptr,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
    das_using<ModuleGroup>::use([&](ModuleGroup & __mg_rename_at_253_340) DAS_AOT_INLINE_LAMBDA -> void{
        das_using<CodeOfPolicies>::use([&](CodeOfPolicies & __cop_rename_at_254_341) DAS_AOT_INLINE_LAMBDA -> void{
            das_copy(__cop_rename_at_254_341.threadlock_context /*threadlock_context*/,true);
            rtti_builtin_compile_file(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_32, cast<char *>::from(das_global<char *,0xb0f39eaf391fc167>(__context__) /*appDir*/), cast<char * const >::from(((char *) "/")), cast<char *>::from(das_global<char *,0x86e5ae912531f3a1>(__context__) /*appFile*/))),__access_rename_at_252_339,das_ref(__context__,__mg_rename_at_253_340),das_arg<CodeOfPolicies>::pass(__cop_rename_at_254_341),das_make_block<void,bool,smart_ptr_raw<Program>,das::string const  &>(__context__,224,0,&__func_info__fe02a5da011a6cb4,[&](bool __ok_rename_at_256_342, smart_ptr_raw<Program> __program_rename_at_256_343, das::string const  & __issues_rename_at_256_344) -> void{
                if ( __ok_rename_at_256_342 )
                {
                    rtti_builtin_simulate(__program_rename_at_256_343,das_make_block<void,bool,smart_ptr_raw<Context>,das::string &>(__context__,288,0,&__func_info__8eb7d9f42d992591,[&](bool __sok_rename_at_258_345, smart_ptr_raw<Context> __context_rename_at_258_346, das::string & __serrors_rename_at_258_347) -> void{
                        if ( __sok_rename_at_258_345 )
                        {
                            toLog(0,((char *) "LIVE: reloaded\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                            set_new_context_a7a4593ab51c9d0c(__context__,__context_rename_at_258_346,__full_restart_rename_at_250_338);
                        } else {
                            toLog(40000,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_33, cast<char * const >::from(((char *) "LIVE: ")), cast<char *>::from(das_global<char *,0x86e5ae912531f3a1>(__context__) /*appFile*/), cast<char * const >::from(((char *) " failed to simulate:\n")), cast<das::string const  &>::from(__issues_rename_at_256_344), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                            set_new_context_a7a4593ab51c9d0c(__context__,das_auto_cast<smart_ptr_raw<Context>>::cast(nullptr),__full_restart_rename_at_250_338);
                        };
                    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                } else {
                    toLog(40000,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_34, cast<char * const >::from(((char *) "LIVE: ")), cast<char *>::from(das_global<char *,0x86e5ae912531f3a1>(__context__) /*appFile*/), cast<char * const >::from(((char *) " failed to compile:\n")), cast<das::string const  &>::from(__issues_rename_at_256_344), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                    set_new_context_a7a4593ab51c9d0c(__context__,das_auto_cast<smart_ptr_raw<Context>>::cast(nullptr),__full_restart_rename_at_250_338);
                };
            }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        });
    });
}}

inline void live_keypressed_c93934ab7b43c5fc ( Context * __context__, int32_t __keycode_rename_at_277_348, int32_t __scancode_rename_at_277_349, int32_t __action_rename_at_277_350, int32_t __mods_rename_at_277_351 )
{
    _FuncliveTickinvoke_liveTick2886249264218566728_c3b9177bd0f8a8bc(__context__,((char *) "keypressed"),__keycode_rename_at_277_348,__scancode_rename_at_277_349,__action_rename_at_277_350,__mods_rename_at_277_351);
    if ( is_live_92bab00e5c617532(__context__) )
    {
        if ( ((__scancode_rename_at_277_349 == 19) && (__action_rename_at_277_350 == 1)) && (__mods_rename_at_277_351 == 2) )
        {
            recompile_18a8bac272d4f62d(__context__,true);
        };
    };
}

inline void live_charpressed_7ed6cbfc39a60863 ( Context * __context__, uint32_t __charcode_rename_at_286_352 )
{
    _FuncliveTickinvoke_liveTick12947590458897769171_8571e9b30efbd8d7(__context__,((char *) "charpressed"),__charcode_rename_at_286_352);
}

inline void live_mousemoved_30e11d11222838c6 ( Context * __context__, double __x_rename_at_290_353, double __y_rename_at_290_354 )
{
    _FuncliveTickinvoke_liveTick8381256433725501221_ebcbd4eac3367a58(__context__,((char *) "mousemoved"),__x_rename_at_290_353,__y_rename_at_290_354);
}

inline void live_mouseenter_3eb091e967328196 ( Context * __context__, bool __yes_rename_at_294_355 )
{
    _FuncliveTickinvoke_liveTick12947590458897769171_8b2be572fbf2e075(__context__,((char *) "mouseenter"),__yes_rename_at_294_355);
}

inline void live_mousebutton_65494a8acfebcfbd ( Context * __context__, int32_t __button_rename_at_298_356, int32_t __action_rename_at_298_357, int32_t __mods_rename_at_298_358 )
{
    _FuncliveTickinvoke_liveTick16287916205268058194_beee970120602d17(__context__,((char *) "mousebutton"),__button_rename_at_298_356,__action_rename_at_298_357,__mods_rename_at_298_358);
}

inline void live_mousescroll_3abccc465cea7be8 ( Context * __context__, double __x_rename_at_302_359, double __y_rename_at_302_360 )
{
    _FuncliveTickinvoke_liveTick8381256433725501221_ebcbd4eac3367a58(__context__,((char *) "mousescroll"),__x_rename_at_302_359,__y_rename_at_302_360);
}

inline void live_update_48a547de881c9a0a ( Context * __context__, double __appTime_rename_at_306_361 )
{
    float __dt_rename_at_310_362 = ((float)float(__appTime_rename_at_306_361 - das_global<double,0x81fa513f37cd5e>(__context__) /*`static`at_line_307`t*/));
    das_copy(das_global<double,0x81fa513f37cd5e>(__context__) /*`static`at_line_307`t*/,__appTime_rename_at_306_361);
    _FuncliveTickinvoke_liveTick12947590458897769171_1753e685298c5c2a(__context__,((char *) "act"),__dt_rename_at_310_362);
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0xadb76bc7a4dda36a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_559ca47f392751ff>>();
    };
    aotLib[0x71ae0ff76282f070] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserialize_rawTick346513482259279339_2cb15e82f6a052bb>>();
    };
    aotLib[0xd0efd05b4423d002] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserialize_rawTick346513482259279339_877086b5c2b9e6d8>>();
    };
    aotLib[0x4e874e2fb81679da] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickread_rawTick6802855309095275289_59f27d4ca7f63255>>();
    };
    aotLib[0x1475235b1d218b09] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickwrite_rawTick9760319972816126958_7e827e6b1de76fcd>>();
    };
    aotLib[0x380b7455b0a2f9f5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_14682004b38ad2a1>>();
    };
    aotLib[0xd5b298fc293f15ad] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_1bdb9995fa062a51>>();
    };
    aotLib[0xf80d77138a37535] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncapplyTickstructTickEcsRequestPosTick0x96Tick0Tick2_cf2646de905784ac>>();
    };
    aotLib[0x17199d64e8a85d54] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_345f02cc82623620>>();
    };
    aotLib[0x78948a08f0d98f51] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_79f9e841b4434fd8>>();
    };
    aotLib[0x439426c0e62e2f81] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_c2c15811c6d720e8>>();
    };
    aotLib[0x29f0f1ae43c0d92e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserializeTick1002677114065211290_838b04f3311f9ee8>>();
    };
    aotLib[0x4bd10b60a8d673a7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserializeTick6962985013043226618_c51e37371f3d267a>>();
    };
    aotLib[0xdb9f562bb6357ab2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserializeTick6962985013043226618_ed7dae2a007ca602>>();
    };
    aotLib[0x5b31b41f15c74888] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserializeTick4429977757536996025_2b74a39202406253>>();
    };
    aotLib[0x7042ba8a45301ef2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserializeTick4429977757536996025_1ff57546f6343a0a>>();
    };
    aotLib[0x90f9afd25d501013] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserializeTick4429977757536996025_beb22678ca166fa4>>();
    };
    aotLib[0x41b9a6def22e2f3d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserializeTick4429977757536996025_3b53a58effc6756a>>();
    };
    aotLib[0x9273b1cd5e9260e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserializeTick4429977757536996025_190cc1e99493f13c>>();
    };
    aotLib[0xf695457d508814af] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserializeTick4429977757536996025_25683f2273767516>>();
    };
    aotLib[0xe460a6d8011f7804] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserializeTick1002677114065211290_bef6bf5e85c51d3b>>();
    };
    aotLib[0x493a46b80fb13156] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserializeTick1002677114065211290_591dc6f784de6b57>>();
    };
    aotLib[0x16138765c1fd27fa] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserializeTick8013964856239694639_cac39795b36f7c56>>();
    };
    aotLib[0x2ea41817c63e772d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_9a888ca44a018013>>();
    };
    aotLib[0xfad1bd58c349c13f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserialize_rawTick346513482259279339_83ff526b8b4a3feb>>();
    };
    aotLib[0x33bbef1b3475a876] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserialize_rawTick346513482259279339_344032bd32dfc282>>();
    };
    aotLib[0xa84d4ffd7a73dafe] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_37451350471f63d7>>();
    };
    aotLib[0x7a8c7b3c06128b3e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncapplyTickstructTickArchetypeTick0x96Tick0Tick4_86443a66f85f8e05>>();
    };
    aotLib[0xed0add122e0bab7f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_aed1eae423063a73>>();
    };
    aotLib[0x7f165b69e38475ac] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncapplyTickstructTickEntityIdTick0x96Tick0Tick2_ceabfe323854d15e>>();
    };
    aotLib[0x8957589138f093d0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_f87ddce7a3ad05ec>>();
    };
    aotLib[0x32a5b519d63a68cf] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncapplyTicktupleTick0x6f4dd0adfbe8abacTick0xa0Tick0Tick3_69d210316d1d7319>>();
    };
    aotLib[0x292a78251254e501] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_c2cf16c62ac9b1f5>>();
    };
    aotLib[0x226c6be0555d0619] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncapplyTickstructTickCTypeInfoTick0x96Tick0Tick11_50e2ea000b6a5668>>();
    };
    aotLib[0xe4221d2b079955fe] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_723506a070851f1e>>();
    };
    aotLib[0x6c184cf3a84b6677] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncapplyTickstructTickEcsRequestTick0x96Tick0Tick5_1da32bb85bdaa226>>();
    };
    aotLib[0x13130f7ba25304c0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickread_rawTick6802855309095275289_b9dfe21dac02b654>>();
    };
    aotLib[0xe4ad0e53b0f7340b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserializeTick6962985013043226618_125aa1d245ce9a68>>();
    };
    aotLib[0x45463604db5aa25b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserializeTick6962985013043226618_8e0595ec66b9d4fd>>();
    };
    aotLib[0x991c09da6401e817] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickwrite_rawTick9760319972816126958_e9195ce9edbb9e44>>();
    };
    aotLib[0x830a57ff0a67a1cb] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickkeysTick2205854368403803976_137bf91714c0645e>>();
    };
    aotLib[0xd43e9642eedc5a7c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1351216622833168869_38542bdf50c4ab32>>();
    };
    aotLib[0xfca19b58b45b1ea1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_aeca051a2e04cb07>>();
    };
    aotLib[0x1018916d234af400] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserializeTick8013964856239694639_731eb25edf591333>>();
    };
    aotLib[0x4c8adc2de907c86] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_be5e4db5cc62bbe2>>();
    };
    aotLib[0xbbd5bdbccf7cba79] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserializeTick8013964856239694639_9f9391ad59b315bc>>();
    };
    aotLib[0xa0c4b4744856ced6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_deb20fe608939269>>();
    };
    aotLib[0xa6fff85a2901f5a7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserializeTick10018991383941125049_67930047264965f1>>();
    };
    aotLib[0xbe9e31ad1122dda3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserializeTick8013964856239694639_a8ff9e2247269964>>();
    };
    aotLib[0x8255bb5ee889b80d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickkeysTick2205854368403803976_23a113e85e5887ab>>();
    };
    aotLib[0xa1c786531a6bfbc] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1351216622833168869_61c0f20a0f69d8bd>>();
    };
    aotLib[0xd64e73b938721c73] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_d65f5162f71fde6a>>();
    };
    aotLib[0x8ebe00d7b489818c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserializeTick8013964856239694639_95732ecdb0114b69>>();
    };
    aotLib[0x19242bfc4de66ee4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick5454204887383796109_7b1c641088884a8c>>();
    };
    aotLib[0xe75f8145d2820a26] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_ea18cb8662f307c1>>();
    };
    aotLib[0x72d901b4c38d27b7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_5fbae9e7389a3856>>();
    };
    aotLib[0x8bf6e90a54fd6dca] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_ba685a3ffd2b9e77>>();
    };
    aotLib[0xcdc7d84276838fd6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick5454204887383796109_1f7f0c3546fa051e>>();
    };
    aotLib[0xa30ab4db3724ff20] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_9a747408a2a6d0ff>>();
    };
    aotLib[0xb0eb9163ada09350] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserializeTick18117489833716462019_10e000d138e4ac0f>>();
    };
    aotLib[0x54d1787656f02f9a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserializeTick1002677114065211290_339f3460927c659a>>();
    };
    aotLib[0x3bd85fcf6f8613d8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserializeTick1002677114065211290_2ebd95325aaa7a86>>();
    };
    aotLib[0xadd82cecc7f4ec98] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserializeTick1002677114065211290_7eba8f9beff4a0ef>>();
    };
    aotLib[0xca3b26db9b509641] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserializeTick18117489833716462019_82336b17b91a4160>>();
    };
    aotLib[0x4a6ac1b667cba367] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserializeTick1002677114065211290_94274d93de23df5>>();
    };
    aotLib[0xa8f929474bbb24c7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_5aac315099132c4e>>();
    };
    aotLib[0x8532fc80bb7a983f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_f314c103a27e7f0c>>();
    };
    aotLib[0xa6c8bff2b25e1054] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_81b0d19e466110c8>>();
    };
    aotLib[0x5c74091d38a0876d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncapplyTickstructTickDecsStateTick0x96Tick0Tick7_84c01a5244f15f9e>>();
    };
    aotLib[0xc502128bbf7c5d21] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_2333bfdfd8018176>>();
    };
    aotLib[0x5dc9b40a4178ffd1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickkey_existsTick16808803843923989214_1b59bf517600dbc5>>();
    };
    aotLib[0x5f91d303cace51c1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_valueTick6803070933163225147_6f53cf5131c26ea6>>();
    };
    aotLib[0xf4ffee0ce77d71af] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickinsertTick4246857231018487965_53ce123b607bdf56>>();
    };
    aotLib[0x1b817c2b9b8206e7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickserializeTick8013964856239694639_a9d1c2f2f9d21c11>>();
    };
    aotLib[0x81618f554fa1a67b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickkey_existsTick16808803843923989214_efda835c0359ec76>>();
    };
    aotLib[0xec1fda1174e8c717] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickinsertTick12964066441666329206_963edeee5917200b>>();
    };
    aotLib[0xf7214cd218a61fdf] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_valueTick6803070933163225147_c79cd68da0d29c45>>();
    };
    aotLib[0x31b52c746ef61d68] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_801ffbcd37fa63c8>>();
    };
    aotLib[0xbbfa1b270677fcde] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickcloneTick3038771811667655495_1107976c81aa121a>>();
    };
    aotLib[0x1f8eb5798352d074] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb>>();
    };
    aotLib[0xe6742d4c3f884384] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_3e056b096d7418b6>>();
    };
    aotLib[0x88312717d651d984] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_82b0f923cb1a5c28>>();
    };
    aotLib[0x2594cb0948e99447] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickintptrTick11320822648609276562_c1cab2e197c26e65>>();
    };
    aotLib[0xbfacbbcec6173c0f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickcloneTick3038771811667655495_74d08f53d44ae194>>();
    };
    aotLib[0x19581459265363be] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick5454204887383796109_9c4083566563a85a>>();
    };
    aotLib[0xa7ba1ff5da336a77] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncarchiveTickmem_archive_saveTick1217508036218320246_8093c94b06c5b1d3>>();
    };
    aotLib[0x1e2b3523e83f5cdf] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncarchiveTickmem_archive_loadTick9860235419630369707_520cdbc5db538a78>>();
    };
    aotLib[0x482687c30dbab01f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_bccc36eddc052dcb>>();
    };
    aotLib[0xdd4d573d1899ee3c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_5a60439f7fb04300>>();
    };
    aotLib[0xd11798612d63b90c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncliveTicklive_lookupTick637029202016062135_11576884b544c838>>();
    };
    aotLib[0xb2d43dda7522315c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncfioTickdirTick17131698203006957834_186fcc74e85e93aa>>();
    };
    aotLib[0x1acab6e2ab20bedd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_ptrTick5807679485210906136_91295c6251ff26c5>>();
    };
    aotLib[0x280e46e82cd2155c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Func_lambda_live_205_1Tickfunction_b3b7a30c2cb432a6>>();
    };
    aotLib[0x4c31a56ad1f80c25] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Func_lambda_live_205_1Tickfinalizer_88431e65ddbac842>>();
    };
    aotLib[0x4978ac8f36909853] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Func_lambda_live_225_2Tickfunction_2c0f2ecd65895fe4>>();
    };
    aotLib[0x1dc0780f9bc3bc08] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Func_lambda_live_225_2Tickfinalizer_4b5c91408d22a4a6>>();
    };
    aotLib[0x6194b7f6b9413ba] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Func_lambda_live_237_3Tickfunction_1dd03a040db0eba6>>();
    };
    aotLib[0x34a3c2a9fcac7cb0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Func_lambda_live_237_3Tickfinalizer_7c78462166aa88ea>>();
    };
    aotLib[0x75527bda7de2bfe0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncliveTickinvoke_liveTick12947590458897769171_8b2be572fbf2e075>>();
    };
    aotLib[0x5aa405b6a504eb26] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncliveTickinvoke_liveTick2886249264218566728_c3b9177bd0f8a8bc>>();
    };
    aotLib[0xf9ad7c230c08666c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncliveTickinvoke_liveTick12947590458897769171_8571e9b30efbd8d7>>();
    };
    aotLib[0xb132b122d394c6d0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncliveTickinvoke_liveTick8381256433725501221_ebcbd4eac3367a58>>();
    };
    aotLib[0x1293cf42f4f12a27] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncliveTickinvoke_liveTick16287916205268058194_beee970120602d17>>();
    };
    aotLib[0x14c761f4a17941a6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncliveTickinvoke_liveTick12947590458897769171_1753e685298c5c2a>>();
    };
    aotLib[0x1d458b60dc16a8e2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&get_live_context_e44470f3748ec318>>();
    };
    aotLib[0xcb32672e5a4f57fc] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&saveLiveContext_efcae6d209280047>>();
    };
    aotLib[0xe31d0ab40ff5a5e3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&restoreLiveContext_49c6d9153d13659f>>();
    };
    aotLib[0xfd067d20b103d34] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&go_offline_778dbe0dd3b703b8>>();
    };
    aotLib[0x6ca2f90079b6d4e4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&is_live_92bab00e5c617532>>();
    };
    aotLib[0x408f116fe204931b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&is_app_live_b15dd0f7bf4a041d>>();
    };
    aotLib[0x37c57de20aabcbeb] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&go_main_b78776d702f7ae9b>>();
    };
    aotLib[0x71c71fdba853521b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&go_live_b8a103eaa74d382f>>();
    };
    aotLib[0xfbd85a12ca911c11] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&invoke_live_94051986a48f719e>>();
    };
    aotLib[0xb351aa6d186d9638] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&watch_files_cfe33bf10ac8df9b>>();
    };
    aotLib[0xbaaca5f617502fb1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&set_new_context_a7a4593ab51c9d0c>>();
    };
    aotLib[0x144c8a45014c65b0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&recompile_18a8bac272d4f62d>>();
    };
    aotLib[0xac225147a4e3f271] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&live_keypressed_c93934ab7b43c5fc>>();
    };
    aotLib[0x3c3b5f75b72ab198] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&live_charpressed_7ed6cbfc39a60863>>();
    };
    aotLib[0x405a02bc1219f38] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&live_mousemoved_30e11d11222838c6>>();
    };
    aotLib[0x6683d6cc0c4861d2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&live_mouseenter_3eb091e967328196>>();
    };
    aotLib[0x5ad2f8fc5bc4ea76] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&live_mousebutton_65494a8acfebcfbd>>();
    };
    aotLib[0xc34f03a25f4b33d1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&live_mousescroll_3abccc465cea7be8>>();
    };
    aotLib[0xfae8fe2f6bfde39e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&live_update_48a547de881c9a0a>>();
    };
    // [[ init script ]]
    aotLib[0x3bd945a073c7dfc] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<AutoSimNode_Aot<&__init_script>>();
        return ctx.aotInitScript;
    };
	resolveTypeInfoAnnotations();
}

} // namespace _anon_513224431237640881
AotListBase impl_aot_live(_anon_513224431237640881::registerAotFunctions);
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
