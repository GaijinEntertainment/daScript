#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
 // require rtti
#include "daScript/simulate/aot_builtin_rtti.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_handle.h"
 // require debugapi
#include "daScript/simulate/aot_builtin_debugger.h"
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
 // require ast_debug

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
namespace _anon_1280768645085455701 {

namespace ast_debug { struct SampleStackWalker; };
namespace ast_debug { struct ContextStateAgent; };
namespace debugapi { struct DapiDebugAgent; };
namespace debugapi { struct DapiArray; };
namespace debugapi { struct DapiTable; };
namespace debugapi { struct DapiBlock; };
namespace debugapi { struct DapiFunc; };
namespace debugapi { struct DapiLambda; };
namespace debugapi { struct DapiSequence; };
namespace debugapi { struct DapiDataWalker; };
namespace debugapi { struct DapiStackWalker; };
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
namespace debugapi {

struct DapiDebugAgent {
    void * __rtti;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent)) __finalize;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,DebugAgent * const )) onInstall;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,DebugAgent * const )) onUninstall;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context)) onCreateContext;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context)) onDestroyContext;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context)) onSimulateContext;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,LineInfo const )) onSingleStep;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,LineInfo const )) onInstrument;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,SimFunction * const ,bool,uint64_t)) onInstrumentFunction;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,LineInfo const ,char * const ,char * const )) onBreakpoint;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,char * const ,char * const ,TypeInfo const ,void * const )) onVariable;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,char * const ,int32_t)) onBreakpointsReset;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent)) onTick;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,LineInfo const )) onCollect;
    Func DAS_COMMENT((bool,debugapi::DapiDebugAgent,Context * const ,LineInfo const  * const ,int32_t,char * const )) onLog;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context)) onBeforeGC;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context)) onAfterGC;
    Func DAS_COMMENT((bool,debugapi::DapiDebugAgent,char * const )) onUserCommand;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,void * const ,uint64_t,LineInfo const )) onAllocate;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,void * const ,uint64_t,void * const ,uint64_t,LineInfo const )) onReallocate;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,void * const ,LineInfo const )) onFree;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,void * const ,uint64_t,LineInfo const )) onAllocateString;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,void * const ,LineInfo const )) onFreeString;
    DebugAgent * thisAgent;
};
}
// unused structure DapiArray
// unused structure DapiTable
// unused structure DapiBlock
// unused structure DapiFunc
// unused structure DapiLambda
// unused structure DapiSequence
// unused structure DapiDataWalker
namespace debugapi {

struct DapiStackWalker {
    void * __rtti;
    Func DAS_COMMENT((void,debugapi::DapiStackWalker)) __finalize;
    Func DAS_COMMENT((bool,debugapi::DapiStackWalker)) canWalkArguments;
    Func DAS_COMMENT((bool,debugapi::DapiStackWalker)) canWalkVariables;
    Func DAS_COMMENT((bool,debugapi::DapiStackWalker)) canWalkOutOfScopeVariables;
    Func DAS_COMMENT((void,debugapi::DapiStackWalker,Prologue const ,void * const )) onBeforeCall;
    Func DAS_COMMENT((void,debugapi::DapiStackWalker,Prologue const ,char * const )) onCallAOT;
    Func DAS_COMMENT((void,debugapi::DapiStackWalker,Prologue const ,FuncInfo const ,LineInfo const )) onCallAt;
    Func DAS_COMMENT((void,debugapi::DapiStackWalker,Prologue const ,FuncInfo const )) onCall;
    Func DAS_COMMENT((void,debugapi::DapiStackWalker,Prologue const ,void * const )) onAfterPrologue;
    Func DAS_COMMENT((void,debugapi::DapiStackWalker,FuncInfo const ,int32_t,VarInfo const ,float4)) onArgument;
    Func DAS_COMMENT((void,debugapi::DapiStackWalker)) onBeforeVariables;
    Func DAS_COMMENT((void,debugapi::DapiStackWalker,FuncInfo const ,LocalVariableInfo const ,void * const ,bool)) onVariable;
    Func DAS_COMMENT((bool,debugapi::DapiStackWalker,Prologue const )) onAfterCall;
};
}
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
namespace ast_debug {

struct SampleStackWalker {
    void * __rtti;
    Func DAS_COMMENT((void,debugapi::DapiStackWalker)) __finalize;
    Func DAS_COMMENT((bool,debugapi::DapiStackWalker)) canWalkArguments;
    Func DAS_COMMENT((bool,debugapi::DapiStackWalker)) canWalkVariables;
    Func DAS_COMMENT((bool,debugapi::DapiStackWalker)) canWalkOutOfScopeVariables;
    Func DAS_COMMENT((void,debugapi::DapiStackWalker,Prologue const ,void * const )) onBeforeCall;
    Func DAS_COMMENT((void,debugapi::DapiStackWalker,Prologue const ,char * const )) onCallAOT;
    Func DAS_COMMENT((void,debugapi::DapiStackWalker,Prologue const ,FuncInfo const ,LineInfo const )) onCallAt;
    Func DAS_COMMENT((void,debugapi::DapiStackWalker,Prologue const ,FuncInfo const )) onCall;
    Func DAS_COMMENT((void,debugapi::DapiStackWalker,Prologue const ,void * const )) onAfterPrologue;
    Func DAS_COMMENT((void,debugapi::DapiStackWalker,FuncInfo const ,int32_t,VarInfo const ,float4)) onArgument;
    Func DAS_COMMENT((void,debugapi::DapiStackWalker)) onBeforeVariables;
    Func DAS_COMMENT((void,debugapi::DapiStackWalker,FuncInfo const ,LocalVariableInfo const ,void * const ,bool)) onVariable;
    Func DAS_COMMENT((bool,debugapi::DapiStackWalker,Prologue const )) onAfterCall;
    Context * ctxid;
};
}
namespace ast_debug {

struct ContextStateAgent {
    void * __rtti;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent)) __finalize;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,DebugAgent * const )) onInstall;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,DebugAgent * const )) onUninstall;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context)) onCreateContext;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context)) onDestroyContext;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context)) onSimulateContext;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,LineInfo const )) onSingleStep;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,LineInfo const )) onInstrument;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,SimFunction * const ,bool,uint64_t)) onInstrumentFunction;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,LineInfo const ,char * const ,char * const )) onBreakpoint;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,char * const ,char * const ,TypeInfo const ,void * const )) onVariable;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,char * const ,int32_t)) onBreakpointsReset;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent)) onTick;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,LineInfo const )) onCollect;
    Func DAS_COMMENT((bool,debugapi::DapiDebugAgent,Context * const ,LineInfo const  * const ,int32_t,char * const )) onLog;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context)) onBeforeGC;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context)) onAfterGC;
    Func DAS_COMMENT((bool,debugapi::DapiDebugAgent,char * const )) onUserCommand;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,void * const ,uint64_t,LineInfo const )) onAllocate;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,void * const ,uint64_t,void * const ,uint64_t,LineInfo const )) onReallocate;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,void * const ,LineInfo const )) onFree;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,void * const ,uint64_t,LineInfo const )) onAllocateString;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,void * const ,LineInfo const )) onFreeString;
    DebugAgent * thisAgent;
    smart_ptr_raw<StackWalker> walker_adapter;
    ast_debug::SampleStackWalker * walker;
};
}
extern StructInfo __struct_info__550ac570675ff176;
extern StructInfo __struct_info__f935e99ac014dfbe;
extern StructInfo __struct_info__d91c21afef62b32c;
extern StructInfo __struct_info__31e0f560cc005077;
extern TypeInfo __type_info__921d3d0750982f13;
extern TypeInfo __type_info__a9e212d4f301a82c;
extern TypeInfo __type_info__209f9e4a9562d5c2;
extern TypeInfo __type_info__2d750e15c3790305;
extern TypeInfo __type_info__ed65100a3b73031a;
extern TypeInfo __type_info__9c225ec61b3e6a3c;
extern TypeInfo __type_info__e421b7d32038f816;
extern TypeInfo __type_info__40912bc113ca9a8a;
extern TypeInfo __type_info__246dda13a8a4b104;
extern TypeInfo __type_info__873433ff256e8173;
extern TypeInfo __type_info__bc67beb4aa160fd4;
extern TypeInfo __type_info__c3770d5f54a928b3;
extern TypeInfo __type_info__faddef75a1c73c1f;
extern TypeInfo __type_info__faddb375a1c6d62b;
extern TypeInfo __type_info__af81fe4c86352052;
extern TypeInfo __type_info__7c9b820817d2b152;
extern TypeInfo __type_info__af8afe4c86446b52;
extern TypeInfo __type_info__af90fe4c864e9d52;
extern TypeInfo __type_info__b68d800849332aec;
extern TypeInfo __type_info__44ca287faf79178;
extern TypeInfo __type_info__87688966731f2665;
extern TypeInfo __type_info__767637ee1a337419;
extern TypeInfo __type_info__5ac778689ccf4816;
extern TypeInfo __type_info__2ab2203ab786034d;
extern TypeInfo __type_info__80bd755d94e49a69;
extern TypeInfo __type_info__4e03c6707d012d5b;
extern TypeInfo __type_info__a4d736e8781a7b06;
extern TypeInfo __type_info__af63df4c8601f1a5;
extern TypeInfo __type_info__af63eb4c86020609;
extern VarInfo __var_info__843cfb4112bd698e;
extern VarInfo __var_info__7bd1cab21471ab25;
extern VarInfo __var_info__6a128179deabf0b5;
extern VarInfo __var_info__5e149a6997cfd2da;
extern VarInfo __var_info__f74740f6dafd565a;
extern VarInfo __var_info__da55f2c6b151f305;
extern VarInfo __var_info__cbb06a53b92d54e5;
extern VarInfo __var_info__f99e2e7b8ac97672;
extern VarInfo __var_info__e5dc68dcc6402013;
extern VarInfo __var_info__d8ea5eff76ca2a64;
extern VarInfo __var_info__592d10706f4919e5;
extern VarInfo __var_info__88c8148858bfb714;
extern VarInfo __var_info__d3967362a16cbe84;
extern VarInfo __var_info__2528f5ed5ae1af35;
extern VarInfo __var_info__f77a4661fe2771cb;
extern VarInfo __var_info__d7043f1cd9c75848;
extern VarInfo __var_info__a0092256e3dafcda;
extern VarInfo __var_info__6867962b4cc54c13;
extern VarInfo __var_info__e42fe79e88d98862;
extern VarInfo __var_info__c1cc5c742611c50c;
extern VarInfo __var_info__2f2deacba8a8f559;
extern VarInfo __var_info__a6e377cff2c5dc41;
extern VarInfo __var_info__dd1dcaf58166fe86;
extern VarInfo __var_info__c55b38b4f047d1a;
extern VarInfo __var_info__9123e06308eb280d;
extern VarInfo __var_info__c61d1e69a986c615;
extern VarInfo __var_info__addc2f65febe2541;
extern VarInfo __var_info__6f356ac68231f616;
extern VarInfo __var_info__1a081159f5f4969d;
extern VarInfo __var_info__73ca2bcef448ca39;
extern VarInfo __var_info__42d08aee28095765;
extern VarInfo __var_info__844614b0b3aae54f;
extern VarInfo __var_info__829892f269153876;
extern VarInfo __var_info__3f2826b1f873e317;
extern VarInfo __var_info__da0f60d2fedc5561;
extern VarInfo __var_info__ce0fdf61a430d3ad;
extern VarInfo __var_info__6b14996a7e65a101;
extern VarInfo __var_info__519597779d68e597;
extern VarInfo __var_info__123843feb17460ac;
extern VarInfo __var_info__46966cc843ece6b9;
extern VarInfo __var_info__46e261c8446df808;
extern VarInfo __var_info__b45c2a93b09088d2;
extern VarInfo __var_info__f8c54e03a28c41c;
extern VarInfo __var_info__da073e6d7ab129fb;
extern VarInfo __var_info__d024cad15fe2631f;
extern VarInfo __var_info__8055238a52f31e0c;
extern VarInfo __var_info__f1f0b5451e923dbc;
extern VarInfo __var_info__36c2061312a1876b;
extern VarInfo __var_info__c913774fc6d2e043;
extern VarInfo __var_info__a73424c91982d80c;
extern VarInfo __var_info__913dec62813bfe4d;
extern VarInfo __var_info__99a7ecf15c5ddba;
extern VarInfo __var_info__8b4c2a0827deb533;
extern VarInfo __var_info__6ff5841b5071093e;
extern VarInfo __var_info__c283a5754fe3daf6;
extern VarInfo __var_info__bb113b3aa23055d3;
extern VarInfo __var_info__327f7c9a660b1ab9;
extern VarInfo __var_info__21b69962c22005be;
extern VarInfo __var_info__68ba67f3d12e3c98;
extern VarInfo __var_info__3d1ee78d21cd5f19;
extern VarInfo __var_info__33eddcacfcf4eae8;
extern VarInfo __var_info__d5dfe6dacc8ce102;
extern VarInfo __var_info__edcf7675d0697793;
extern VarInfo __var_info__b6de3e351cfffee7;
extern VarInfo __var_info__df3b4662672293c8;
extern VarInfo __var_info__79d44cbc9a657bf4;
extern VarInfo __var_info__817bf1fe93f2f5d7;
extern VarInfo __var_info__894071b3265e2991;
extern VarInfo __var_info__280f562af3da5a1;
extern VarInfo __var_info__ba24a7dae93892bb;
extern VarInfo __var_info__e7ea6856c665cde7;
extern VarInfo __var_info__cc398c743a2640e0;
extern VarInfo __var_info__3e54f61c3199cebd;
extern VarInfo __var_info__bb7dd188f2b1d03a;
extern VarInfo __var_info__d1b1bc8ffb15d968;
extern VarInfo __var_info__6215cbf9e4be26c;
extern VarInfo __var_info__c0460d6b0b9f90c3;
extern VarInfo __var_info__6e8c47174cd471b0;
extern VarInfo __var_info__f396ff83ffb05655;
extern VarInfo __var_info__ac62bd978c461883;
extern VarInfo __var_info__632c5e0a5c27b74e;

VarInfo __struct_info__550ac570675ff176_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0x7bd1cab21471ab25), "__rtti", offsetof(ast_debug::ContextStateAgent,__rtti), 24 };
TypeInfo * __type_info__843cfb4112bd698e_arg_types_var_6127927329552331126[1] = { &__type_info__4e03c6707d012d5b };
const char * __type_info__843cfb4112bd698e_arg_names_var_6127927329552331126[1] = { "self" };
VarInfo __struct_info__550ac570675ff176_field_1 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__843cfb4112bd698e_arg_types_var_6127927329552331126, __type_info__843cfb4112bd698e_arg_names_var_6127927329552331126, 1, 0, nullptr, 12, 8, UINT64_C(0x843cfb4112bd698e), "__finalize", offsetof(ast_debug::ContextStateAgent,__finalize), 0 };
TypeInfo * __type_info__2528f5ed5ae1af35_arg_types_var_6127927329552331126[2] = { &__type_info__4e03c6707d012d5b, &__type_info__209f9e4a9562d5c2 };
const char * __type_info__2528f5ed5ae1af35_arg_names_var_6127927329552331126[2] = { "self", "agent" };
VarInfo __struct_info__550ac570675ff176_field_2 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__2528f5ed5ae1af35_arg_types_var_6127927329552331126, __type_info__2528f5ed5ae1af35_arg_names_var_6127927329552331126, 2, 0, nullptr, 12, 8, UINT64_C(0x2528f5ed5ae1af35), "onInstall", offsetof(ast_debug::ContextStateAgent,onInstall), 0 };
TypeInfo * __type_info__a6e377cff2c5dc41_arg_types_var_6127927329552331126[2] = { &__type_info__4e03c6707d012d5b, &__type_info__209f9e4a9562d5c2 };
const char * __type_info__a6e377cff2c5dc41_arg_names_var_6127927329552331126[2] = { "self", "agent" };
VarInfo __struct_info__550ac570675ff176_field_3 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__a6e377cff2c5dc41_arg_types_var_6127927329552331126, __type_info__a6e377cff2c5dc41_arg_names_var_6127927329552331126, 2, 0, nullptr, 12, 8, UINT64_C(0xa6e377cff2c5dc41), "onUninstall", offsetof(ast_debug::ContextStateAgent,onUninstall), 0 };
TypeInfo * __type_info__d8ea5eff76ca2a64_arg_types_var_6127927329552331126[2] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419 };
const char * __type_info__d8ea5eff76ca2a64_arg_names_var_6127927329552331126[2] = { "self", "ctx" };
VarInfo __struct_info__550ac570675ff176_field_4 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__d8ea5eff76ca2a64_arg_types_var_6127927329552331126, __type_info__d8ea5eff76ca2a64_arg_names_var_6127927329552331126, 2, 0, nullptr, 12, 8, UINT64_C(0xd8ea5eff76ca2a64), "onCreateContext", offsetof(ast_debug::ContextStateAgent,onCreateContext), 0 };
TypeInfo * __type_info__592d10706f4919e5_arg_types_var_6127927329552331126[2] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419 };
const char * __type_info__592d10706f4919e5_arg_names_var_6127927329552331126[2] = { "self", "ctx" };
VarInfo __struct_info__550ac570675ff176_field_5 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__592d10706f4919e5_arg_types_var_6127927329552331126, __type_info__592d10706f4919e5_arg_names_var_6127927329552331126, 2, 0, nullptr, 12, 8, UINT64_C(0x592d10706f4919e5), "onDestroyContext", offsetof(ast_debug::ContextStateAgent,onDestroyContext), 0 };
TypeInfo * __type_info__e42fe79e88d98862_arg_types_var_6127927329552331126[2] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419 };
const char * __type_info__e42fe79e88d98862_arg_names_var_6127927329552331126[2] = { "self", "ctx" };
VarInfo __struct_info__550ac570675ff176_field_6 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__e42fe79e88d98862_arg_types_var_6127927329552331126, __type_info__e42fe79e88d98862_arg_names_var_6127927329552331126, 2, 0, nullptr, 12, 8, UINT64_C(0xe42fe79e88d98862), "onSimulateContext", offsetof(ast_debug::ContextStateAgent,onSimulateContext), 0 };
TypeInfo * __type_info__c1cc5c742611c50c_arg_types_var_6127927329552331126[3] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__246dda13a8a4b104 };
const char * __type_info__c1cc5c742611c50c_arg_names_var_6127927329552331126[3] = { "self", "ctx", "at" };
VarInfo __struct_info__550ac570675ff176_field_7 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__c1cc5c742611c50c_arg_types_var_6127927329552331126, __type_info__c1cc5c742611c50c_arg_names_var_6127927329552331126, 3, 0, nullptr, 12, 8, UINT64_C(0xc1cc5c742611c50c), "onSingleStep", offsetof(ast_debug::ContextStateAgent,onSingleStep), 0 };
TypeInfo * __type_info__f77a4661fe2771cb_arg_types_var_6127927329552331126[3] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__246dda13a8a4b104 };
const char * __type_info__f77a4661fe2771cb_arg_names_var_6127927329552331126[3] = { "self", "ctx", "at" };
VarInfo __struct_info__550ac570675ff176_field_8 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__f77a4661fe2771cb_arg_types_var_6127927329552331126, __type_info__f77a4661fe2771cb_arg_names_var_6127927329552331126, 3, 0, nullptr, 12, 8, UINT64_C(0xf77a4661fe2771cb), "onInstrument", offsetof(ast_debug::ContextStateAgent,onInstrument), 0 };
TypeInfo * __type_info__d7043f1cd9c75848_arg_types_var_6127927329552331126[5] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__ed65100a3b73031a, &__type_info__af81fe4c86352052, &__type_info__b68d800849332aec };
const char * __type_info__d7043f1cd9c75848_arg_names_var_6127927329552331126[5] = { "self", "ctx", "fn", "entering", "userData" };
VarInfo __struct_info__550ac570675ff176_field_9 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__d7043f1cd9c75848_arg_types_var_6127927329552331126, __type_info__d7043f1cd9c75848_arg_names_var_6127927329552331126, 5, 0, nullptr, 12, 8, UINT64_C(0xd7043f1cd9c75848), "onInstrumentFunction", offsetof(ast_debug::ContextStateAgent,onInstrumentFunction), 0 };
TypeInfo * __type_info__cbb06a53b92d54e5_arg_types_var_6127927329552331126[5] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__246dda13a8a4b104, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
const char * __type_info__cbb06a53b92d54e5_arg_names_var_6127927329552331126[5] = { "self", "ctx", "at", "reason", "text" };
VarInfo __struct_info__550ac570675ff176_field_10 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__cbb06a53b92d54e5_arg_types_var_6127927329552331126, __type_info__cbb06a53b92d54e5_arg_names_var_6127927329552331126, 5, 0, nullptr, 12, 8, UINT64_C(0xcbb06a53b92d54e5), "onBreakpoint", offsetof(ast_debug::ContextStateAgent,onBreakpoint), 0 };
TypeInfo * __type_info__c55b38b4f047d1a_arg_types_var_6127927329552331126[6] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__bc67beb4aa160fd4, &__type_info__9c225ec61b3e6a3c };
const char * __type_info__c55b38b4f047d1a_arg_names_var_6127927329552331126[6] = { "self", "ctx", "category", "name", "info", "data" };
VarInfo __struct_info__550ac570675ff176_field_11 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__c55b38b4f047d1a_arg_types_var_6127927329552331126, __type_info__c55b38b4f047d1a_arg_names_var_6127927329552331126, 6, 0, nullptr, 12, 8, UINT64_C(0xc55b38b4f047d1a), "onVariable", offsetof(ast_debug::ContextStateAgent,onVariable), 0 };
TypeInfo * __type_info__f99e2e7b8ac97672_arg_types_var_6127927329552331126[3] = { &__type_info__4e03c6707d012d5b, &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52 };
const char * __type_info__f99e2e7b8ac97672_arg_names_var_6127927329552331126[3] = { "self", "file", "breakpointsNum" };
VarInfo __struct_info__550ac570675ff176_field_12 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__f99e2e7b8ac97672_arg_types_var_6127927329552331126, __type_info__f99e2e7b8ac97672_arg_names_var_6127927329552331126, 3, 0, nullptr, 12, 8, UINT64_C(0xf99e2e7b8ac97672), "onBreakpointsReset", offsetof(ast_debug::ContextStateAgent,onBreakpointsReset), 0 };
TypeInfo * __type_info__2f2deacba8a8f559_arg_types_var_6127927329552331126[1] = { &__type_info__4e03c6707d012d5b };
const char * __type_info__2f2deacba8a8f559_arg_names_var_6127927329552331126[1] = { "self" };
VarInfo __struct_info__550ac570675ff176_field_13 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__2f2deacba8a8f559_arg_types_var_6127927329552331126, __type_info__2f2deacba8a8f559_arg_names_var_6127927329552331126, 1, 0, nullptr, 12, 8, UINT64_C(0x2f2deacba8a8f559), "onTick", offsetof(ast_debug::ContextStateAgent,onTick), 0 };
TypeInfo * __type_info__e5dc68dcc6402013_arg_types_var_6127927329552331126[3] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__246dda13a8a4b104 };
const char * __type_info__e5dc68dcc6402013_arg_names_var_6127927329552331126[3] = { "self", "ctx", "at" };
VarInfo __struct_info__550ac570675ff176_field_14 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__e5dc68dcc6402013_arg_types_var_6127927329552331126, __type_info__e5dc68dcc6402013_arg_names_var_6127927329552331126, 3, 0, nullptr, 12, 8, UINT64_C(0xe5dc68dcc6402013), "onCollect", offsetof(ast_debug::ContextStateAgent,onCollect), 0 };
TypeInfo * __type_info__a0092256e3dafcda_arg_types_var_6127927329552331126[5] = { &__type_info__4e03c6707d012d5b, &__type_info__2d750e15c3790305, &__type_info__a9e212d4f301a82c, &__type_info__af8afe4c86446b52, &__type_info__921d3d0750982f13 };
const char * __type_info__a0092256e3dafcda_arg_names_var_6127927329552331126[5] = { "self", "context", "at", "level", "text" };
VarInfo __struct_info__550ac570675ff176_field_15 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__a0092256e3dafcda_arg_types_var_6127927329552331126, __type_info__a0092256e3dafcda_arg_names_var_6127927329552331126, 5, 0, nullptr, 12, 8, UINT64_C(0xa0092256e3dafcda), "onLog", offsetof(ast_debug::ContextStateAgent,onLog), 0 };
TypeInfo * __type_info__da55f2c6b151f305_arg_types_var_6127927329552331126[2] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419 };
const char * __type_info__da55f2c6b151f305_arg_names_var_6127927329552331126[2] = { "self", "ctx" };
VarInfo __struct_info__550ac570675ff176_field_16 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__da55f2c6b151f305_arg_types_var_6127927329552331126, __type_info__da55f2c6b151f305_arg_names_var_6127927329552331126, 2, 0, nullptr, 12, 8, UINT64_C(0xda55f2c6b151f305), "onBeforeGC", offsetof(ast_debug::ContextStateAgent,onBeforeGC), 0 };
TypeInfo * __type_info__6a128179deabf0b5_arg_types_var_6127927329552331126[2] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419 };
const char * __type_info__6a128179deabf0b5_arg_names_var_6127927329552331126[2] = { "self", "ctx" };
VarInfo __struct_info__550ac570675ff176_field_17 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__6a128179deabf0b5_arg_types_var_6127927329552331126, __type_info__6a128179deabf0b5_arg_names_var_6127927329552331126, 2, 0, nullptr, 12, 8, UINT64_C(0x6a128179deabf0b5), "onAfterGC", offsetof(ast_debug::ContextStateAgent,onAfterGC), 0 };
TypeInfo * __type_info__dd1dcaf58166fe86_arg_types_var_6127927329552331126[2] = { &__type_info__4e03c6707d012d5b, &__type_info__921d3d0750982f13 };
const char * __type_info__dd1dcaf58166fe86_arg_names_var_6127927329552331126[2] = { "self", "command" };
VarInfo __struct_info__550ac570675ff176_field_18 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__dd1dcaf58166fe86_arg_types_var_6127927329552331126, __type_info__dd1dcaf58166fe86_arg_names_var_6127927329552331126, 2, 0, nullptr, 12, 8, UINT64_C(0xdd1dcaf58166fe86), "onUserCommand", offsetof(ast_debug::ContextStateAgent,onUserCommand), 0 };
TypeInfo * __type_info__5e149a6997cfd2da_arg_types_var_6127927329552331126[5] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__9c225ec61b3e6a3c, &__type_info__b68d800849332aec, &__type_info__246dda13a8a4b104 };
const char * __type_info__5e149a6997cfd2da_arg_names_var_6127927329552331126[5] = { "self", "ctx", "data", "size", "at" };
VarInfo __struct_info__550ac570675ff176_field_19 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__5e149a6997cfd2da_arg_types_var_6127927329552331126, __type_info__5e149a6997cfd2da_arg_names_var_6127927329552331126, 5, 0, nullptr, 12, 8, UINT64_C(0x5e149a6997cfd2da), "onAllocate", offsetof(ast_debug::ContextStateAgent,onAllocate), 0 };
TypeInfo * __type_info__6867962b4cc54c13_arg_types_var_6127927329552331126[7] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__9c225ec61b3e6a3c, &__type_info__b68d800849332aec, &__type_info__9c225ec61b3e6a3c, &__type_info__b68d800849332aec, &__type_info__246dda13a8a4b104 };
const char * __type_info__6867962b4cc54c13_arg_names_var_6127927329552331126[7] = { "self", "ctx", "data", "size", "newData", "newSize", "at" };
VarInfo __struct_info__550ac570675ff176_field_20 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__6867962b4cc54c13_arg_types_var_6127927329552331126, __type_info__6867962b4cc54c13_arg_names_var_6127927329552331126, 7, 0, nullptr, 12, 8, UINT64_C(0x6867962b4cc54c13), "onReallocate", offsetof(ast_debug::ContextStateAgent,onReallocate), 0 };
TypeInfo * __type_info__88c8148858bfb714_arg_types_var_6127927329552331126[4] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__9c225ec61b3e6a3c, &__type_info__246dda13a8a4b104 };
const char * __type_info__88c8148858bfb714_arg_names_var_6127927329552331126[4] = { "self", "ctx", "data", "at" };
VarInfo __struct_info__550ac570675ff176_field_21 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__88c8148858bfb714_arg_types_var_6127927329552331126, __type_info__88c8148858bfb714_arg_names_var_6127927329552331126, 4, 0, nullptr, 12, 8, UINT64_C(0x88c8148858bfb714), "onFree", offsetof(ast_debug::ContextStateAgent,onFree), 0 };
TypeInfo * __type_info__f74740f6dafd565a_arg_types_var_6127927329552331126[5] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__9c225ec61b3e6a3c, &__type_info__b68d800849332aec, &__type_info__246dda13a8a4b104 };
const char * __type_info__f74740f6dafd565a_arg_names_var_6127927329552331126[5] = { "self", "ctx", "data", "size", "at" };
VarInfo __struct_info__550ac570675ff176_field_22 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__f74740f6dafd565a_arg_types_var_6127927329552331126, __type_info__f74740f6dafd565a_arg_names_var_6127927329552331126, 5, 0, nullptr, 12, 8, UINT64_C(0xf74740f6dafd565a), "onAllocateString", offsetof(ast_debug::ContextStateAgent,onAllocateString), 0 };
TypeInfo * __type_info__d3967362a16cbe84_arg_types_var_6127927329552331126[4] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__9c225ec61b3e6a3c, &__type_info__246dda13a8a4b104 };
const char * __type_info__d3967362a16cbe84_arg_names_var_6127927329552331126[4] = { "self", "ctx", "data", "at" };
VarInfo __struct_info__550ac570675ff176_field_23 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__d3967362a16cbe84_arg_types_var_6127927329552331126, __type_info__d3967362a16cbe84_arg_names_var_6127927329552331126, 4, 0, nullptr, 12, 8, UINT64_C(0xd3967362a16cbe84), "onFreeString", offsetof(ast_debug::ContextStateAgent,onFreeString), 0 };
VarInfo __struct_info__550ac570675ff176_field_24 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__44ca287faf79178, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0x9123e06308eb280d), "thisAgent", offsetof(ast_debug::ContextStateAgent,thisAgent), 25 };
VarInfo __struct_info__550ac570675ff176_field_25 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__87688966731f2665, nullptr, nullptr, nullptr, 0, 0, nullptr, 9216, 8, UINT64_C(0xaddc2f65febe2541), "walker_adapter", offsetof(ast_debug::ContextStateAgent,walker_adapter), 26 };
VarInfo __struct_info__550ac570675ff176_field_26 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__80bd755d94e49a69, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0xc61d1e69a986c615), "walker", offsetof(ast_debug::ContextStateAgent,walker), 27 };
VarInfo * __struct_info__550ac570675ff176_fields[27] =  { &__struct_info__550ac570675ff176_field_0, &__struct_info__550ac570675ff176_field_1, &__struct_info__550ac570675ff176_field_2, &__struct_info__550ac570675ff176_field_3, &__struct_info__550ac570675ff176_field_4, &__struct_info__550ac570675ff176_field_5, &__struct_info__550ac570675ff176_field_6, &__struct_info__550ac570675ff176_field_7, &__struct_info__550ac570675ff176_field_8, &__struct_info__550ac570675ff176_field_9, &__struct_info__550ac570675ff176_field_10, &__struct_info__550ac570675ff176_field_11, &__struct_info__550ac570675ff176_field_12, &__struct_info__550ac570675ff176_field_13, &__struct_info__550ac570675ff176_field_14, &__struct_info__550ac570675ff176_field_15, &__struct_info__550ac570675ff176_field_16, &__struct_info__550ac570675ff176_field_17, &__struct_info__550ac570675ff176_field_18, &__struct_info__550ac570675ff176_field_19, &__struct_info__550ac570675ff176_field_20, &__struct_info__550ac570675ff176_field_21, &__struct_info__550ac570675ff176_field_22, &__struct_info__550ac570675ff176_field_23, &__struct_info__550ac570675ff176_field_24, &__struct_info__550ac570675ff176_field_25, &__struct_info__550ac570675ff176_field_26 };
StructInfo __struct_info__550ac570675ff176 = {"ContextStateAgent", "ast_debug", 13, __struct_info__550ac570675ff176_fields, 27, 216, UINT64_C(0x0), nullptr, UINT64_C(0x550ac570675ff176), 0 };
VarInfo __struct_info__f935e99ac014dfbe_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0x1a081159f5f4969d), "__rtti", offsetof(ast_debug::SampleStackWalker,__rtti), 14 };
TypeInfo * __type_info__6f356ac68231f616_arg_types_var_17957515940066549694[1] = { &__type_info__a4d736e8781a7b06 };
const char * __type_info__6f356ac68231f616_arg_names_var_17957515940066549694[1] = { "self" };
VarInfo __struct_info__f935e99ac014dfbe_field_1 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__6f356ac68231f616_arg_types_var_17957515940066549694, __type_info__6f356ac68231f616_arg_names_var_17957515940066549694, 1, 0, nullptr, 12, 8, UINT64_C(0x6f356ac68231f616), "__finalize", offsetof(ast_debug::SampleStackWalker,__finalize), 0 };
TypeInfo * __type_info__73ca2bcef448ca39_arg_types_var_17957515940066549694[1] = { &__type_info__a4d736e8781a7b06 };
const char * __type_info__73ca2bcef448ca39_arg_names_var_17957515940066549694[1] = { "self" };
VarInfo __struct_info__f935e99ac014dfbe_field_2 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__73ca2bcef448ca39_arg_types_var_17957515940066549694, __type_info__73ca2bcef448ca39_arg_names_var_17957515940066549694, 1, 0, nullptr, 12, 8, UINT64_C(0x73ca2bcef448ca39), "canWalkArguments", offsetof(ast_debug::SampleStackWalker,canWalkArguments), 0 };
TypeInfo * __type_info__844614b0b3aae54f_arg_types_var_17957515940066549694[1] = { &__type_info__a4d736e8781a7b06 };
const char * __type_info__844614b0b3aae54f_arg_names_var_17957515940066549694[1] = { "self" };
VarInfo __struct_info__f935e99ac014dfbe_field_3 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__844614b0b3aae54f_arg_types_var_17957515940066549694, __type_info__844614b0b3aae54f_arg_names_var_17957515940066549694, 1, 0, nullptr, 12, 8, UINT64_C(0x844614b0b3aae54f), "canWalkVariables", offsetof(ast_debug::SampleStackWalker,canWalkVariables), 0 };
TypeInfo * __type_info__42d08aee28095765_arg_types_var_17957515940066549694[1] = { &__type_info__a4d736e8781a7b06 };
const char * __type_info__42d08aee28095765_arg_names_var_17957515940066549694[1] = { "self" };
VarInfo __struct_info__f935e99ac014dfbe_field_4 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__42d08aee28095765_arg_types_var_17957515940066549694, __type_info__42d08aee28095765_arg_names_var_17957515940066549694, 1, 0, nullptr, 12, 8, UINT64_C(0x42d08aee28095765), "canWalkOutOfScopeVariables", offsetof(ast_debug::SampleStackWalker,canWalkOutOfScopeVariables), 0 };
TypeInfo * __type_info__6b14996a7e65a101_arg_types_var_17957515940066549694[3] = { &__type_info__a4d736e8781a7b06, &__type_info__e421b7d32038f816, &__type_info__9c225ec61b3e6a3c };
const char * __type_info__6b14996a7e65a101_arg_names_var_17957515940066549694[3] = { "self", "pp", "sp" };
VarInfo __struct_info__f935e99ac014dfbe_field_5 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__6b14996a7e65a101_arg_types_var_17957515940066549694, __type_info__6b14996a7e65a101_arg_names_var_17957515940066549694, 3, 0, nullptr, 12, 8, UINT64_C(0x6b14996a7e65a101), "onBeforeCall", offsetof(ast_debug::SampleStackWalker,onBeforeCall), 0 };
TypeInfo * __type_info__46966cc843ece6b9_arg_types_var_17957515940066549694[3] = { &__type_info__a4d736e8781a7b06, &__type_info__e421b7d32038f816, &__type_info__921d3d0750982f13 };
const char * __type_info__46966cc843ece6b9_arg_names_var_17957515940066549694[3] = { "self", "pp", "fileName" };
VarInfo __struct_info__f935e99ac014dfbe_field_6 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__46966cc843ece6b9_arg_types_var_17957515940066549694, __type_info__46966cc843ece6b9_arg_names_var_17957515940066549694, 3, 0, nullptr, 12, 8, UINT64_C(0x46966cc843ece6b9), "onCallAOT", offsetof(ast_debug::SampleStackWalker,onCallAOT), 0 };
TypeInfo * __type_info__46e261c8446df808_arg_types_var_17957515940066549694[4] = { &__type_info__a4d736e8781a7b06, &__type_info__e421b7d32038f816, &__type_info__40912bc113ca9a8a, &__type_info__246dda13a8a4b104 };
const char * __type_info__46e261c8446df808_arg_names_var_17957515940066549694[4] = { "self", "pp", "info", "at" };
VarInfo __struct_info__f935e99ac014dfbe_field_7 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__46e261c8446df808_arg_types_var_17957515940066549694, __type_info__46e261c8446df808_arg_names_var_17957515940066549694, 4, 0, nullptr, 12, 8, UINT64_C(0x46e261c8446df808), "onCallAt", offsetof(ast_debug::SampleStackWalker,onCallAt), 0 };
TypeInfo * __type_info__123843feb17460ac_arg_types_var_17957515940066549694[3] = { &__type_info__a4d736e8781a7b06, &__type_info__e421b7d32038f816, &__type_info__40912bc113ca9a8a };
const char * __type_info__123843feb17460ac_arg_names_var_17957515940066549694[3] = { "self", "pp", "info" };
VarInfo __struct_info__f935e99ac014dfbe_field_8 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__123843feb17460ac_arg_types_var_17957515940066549694, __type_info__123843feb17460ac_arg_names_var_17957515940066549694, 3, 0, nullptr, 12, 8, UINT64_C(0x123843feb17460ac), "onCall", offsetof(ast_debug::SampleStackWalker,onCall), 0 };
TypeInfo * __type_info__da0f60d2fedc5561_arg_types_var_17957515940066549694[3] = { &__type_info__a4d736e8781a7b06, &__type_info__e421b7d32038f816, &__type_info__9c225ec61b3e6a3c };
const char * __type_info__da0f60d2fedc5561_arg_names_var_17957515940066549694[3] = { "self", "pp", "sp" };
VarInfo __struct_info__f935e99ac014dfbe_field_9 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__da0f60d2fedc5561_arg_types_var_17957515940066549694, __type_info__da0f60d2fedc5561_arg_names_var_17957515940066549694, 3, 0, nullptr, 12, 8, UINT64_C(0xda0f60d2fedc5561), "onAfterPrologue", offsetof(ast_debug::SampleStackWalker,onAfterPrologue), 0 };
TypeInfo * __type_info__ce0fdf61a430d3ad_arg_types_var_17957515940066549694[5] = { &__type_info__a4d736e8781a7b06, &__type_info__40912bc113ca9a8a, &__type_info__af8afe4c86446b52, &__type_info__c3770d5f54a928b3, &__type_info__7c9b820817d2b152 };
const char * __type_info__ce0fdf61a430d3ad_arg_names_var_17957515940066549694[5] = { "self", "info", "index", "vinfo", "arg" };
VarInfo __struct_info__f935e99ac014dfbe_field_10 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__ce0fdf61a430d3ad_arg_types_var_17957515940066549694, __type_info__ce0fdf61a430d3ad_arg_names_var_17957515940066549694, 5, 0, nullptr, 12, 8, UINT64_C(0xce0fdf61a430d3ad), "onArgument", offsetof(ast_debug::SampleStackWalker,onArgument), 0 };
TypeInfo * __type_info__519597779d68e597_arg_types_var_17957515940066549694[1] = { &__type_info__a4d736e8781a7b06 };
const char * __type_info__519597779d68e597_arg_names_var_17957515940066549694[1] = { "self" };
VarInfo __struct_info__f935e99ac014dfbe_field_11 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__519597779d68e597_arg_types_var_17957515940066549694, __type_info__519597779d68e597_arg_names_var_17957515940066549694, 1, 0, nullptr, 12, 8, UINT64_C(0x519597779d68e597), "onBeforeVariables", offsetof(ast_debug::SampleStackWalker,onBeforeVariables), 0 };
TypeInfo * __type_info__b45c2a93b09088d2_arg_types_var_17957515940066549694[5] = { &__type_info__a4d736e8781a7b06, &__type_info__40912bc113ca9a8a, &__type_info__873433ff256e8173, &__type_info__9c225ec61b3e6a3c, &__type_info__af81fe4c86352052 };
const char * __type_info__b45c2a93b09088d2_arg_names_var_17957515940066549694[5] = { "self", "inf", "vinfo", "arg", "inScope" };
VarInfo __struct_info__f935e99ac014dfbe_field_12 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__b45c2a93b09088d2_arg_types_var_17957515940066549694, __type_info__b45c2a93b09088d2_arg_names_var_17957515940066549694, 5, 0, nullptr, 12, 8, UINT64_C(0xb45c2a93b09088d2), "onVariable", offsetof(ast_debug::SampleStackWalker,onVariable), 0 };
TypeInfo * __type_info__3f2826b1f873e317_arg_types_var_17957515940066549694[2] = { &__type_info__a4d736e8781a7b06, &__type_info__e421b7d32038f816 };
const char * __type_info__3f2826b1f873e317_arg_names_var_17957515940066549694[2] = { "self", "pp" };
VarInfo __struct_info__f935e99ac014dfbe_field_13 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__3f2826b1f873e317_arg_types_var_17957515940066549694, __type_info__3f2826b1f873e317_arg_names_var_17957515940066549694, 2, 0, nullptr, 12, 8, UINT64_C(0x3f2826b1f873e317), "onAfterCall", offsetof(ast_debug::SampleStackWalker,onAfterCall), 0 };
VarInfo __struct_info__f935e99ac014dfbe_field_14 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__767637ee1a337419, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0x829892f269153876), "ctxid", offsetof(ast_debug::SampleStackWalker,ctxid), 15 };
VarInfo * __struct_info__f935e99ac014dfbe_fields[15] =  { &__struct_info__f935e99ac014dfbe_field_0, &__struct_info__f935e99ac014dfbe_field_1, &__struct_info__f935e99ac014dfbe_field_2, &__struct_info__f935e99ac014dfbe_field_3, &__struct_info__f935e99ac014dfbe_field_4, &__struct_info__f935e99ac014dfbe_field_5, &__struct_info__f935e99ac014dfbe_field_6, &__struct_info__f935e99ac014dfbe_field_7, &__struct_info__f935e99ac014dfbe_field_8, &__struct_info__f935e99ac014dfbe_field_9, &__struct_info__f935e99ac014dfbe_field_10, &__struct_info__f935e99ac014dfbe_field_11, &__struct_info__f935e99ac014dfbe_field_12, &__struct_info__f935e99ac014dfbe_field_13, &__struct_info__f935e99ac014dfbe_field_14 };
StructInfo __struct_info__f935e99ac014dfbe = {"SampleStackWalker", "ast_debug", 13, __struct_info__f935e99ac014dfbe_fields, 15, 120, UINT64_C(0x0), nullptr, UINT64_C(0xf935e99ac014dfbe), 0 };
VarInfo __struct_info__d91c21afef62b32c_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0xda073e6d7ab129fb), "__rtti", offsetof(debugapi::DapiDebugAgent,__rtti), 24 };
TypeInfo * __type_info__f8c54e03a28c41c_arg_types_var_15644416245097476908[1] = { &__type_info__4e03c6707d012d5b };
const char * __type_info__f8c54e03a28c41c_arg_names_var_15644416245097476908[1] = { "self" };
VarInfo __struct_info__d91c21afef62b32c_field_1 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__f8c54e03a28c41c_arg_types_var_15644416245097476908, __type_info__f8c54e03a28c41c_arg_names_var_15644416245097476908, 1, 0, nullptr, 12, 8, UINT64_C(0xf8c54e03a28c41c), "__finalize", offsetof(debugapi::DapiDebugAgent,__finalize), 0 };
TypeInfo * __type_info__bb113b3aa23055d3_arg_types_var_15644416245097476908[2] = { &__type_info__4e03c6707d012d5b, &__type_info__209f9e4a9562d5c2 };
const char * __type_info__bb113b3aa23055d3_arg_names_var_15644416245097476908[2] = { "self", "agent" };
VarInfo __struct_info__d91c21afef62b32c_field_2 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__bb113b3aa23055d3_arg_types_var_15644416245097476908, __type_info__bb113b3aa23055d3_arg_names_var_15644416245097476908, 2, 0, nullptr, 12, 8, UINT64_C(0xbb113b3aa23055d3), "onInstall", offsetof(debugapi::DapiDebugAgent,onInstall), 0 };
TypeInfo * __type_info__b6de3e351cfffee7_arg_types_var_15644416245097476908[2] = { &__type_info__4e03c6707d012d5b, &__type_info__209f9e4a9562d5c2 };
const char * __type_info__b6de3e351cfffee7_arg_names_var_15644416245097476908[2] = { "self", "agent" };
VarInfo __struct_info__d91c21afef62b32c_field_3 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__b6de3e351cfffee7_arg_types_var_15644416245097476908, __type_info__b6de3e351cfffee7_arg_names_var_15644416245097476908, 2, 0, nullptr, 12, 8, UINT64_C(0xb6de3e351cfffee7), "onUninstall", offsetof(debugapi::DapiDebugAgent,onUninstall), 0 };
TypeInfo * __type_info__99a7ecf15c5ddba_arg_types_var_15644416245097476908[2] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419 };
const char * __type_info__99a7ecf15c5ddba_arg_names_var_15644416245097476908[2] = { "self", "ctx" };
VarInfo __struct_info__d91c21afef62b32c_field_4 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__99a7ecf15c5ddba_arg_types_var_15644416245097476908, __type_info__99a7ecf15c5ddba_arg_names_var_15644416245097476908, 2, 0, nullptr, 12, 8, UINT64_C(0x99a7ecf15c5ddba), "onCreateContext", offsetof(debugapi::DapiDebugAgent,onCreateContext), 0 };
TypeInfo * __type_info__8b4c2a0827deb533_arg_types_var_15644416245097476908[2] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419 };
const char * __type_info__8b4c2a0827deb533_arg_names_var_15644416245097476908[2] = { "self", "ctx" };
VarInfo __struct_info__d91c21afef62b32c_field_5 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__8b4c2a0827deb533_arg_types_var_15644416245097476908, __type_info__8b4c2a0827deb533_arg_names_var_15644416245097476908, 2, 0, nullptr, 12, 8, UINT64_C(0x8b4c2a0827deb533), "onDestroyContext", offsetof(debugapi::DapiDebugAgent,onDestroyContext), 0 };
TypeInfo * __type_info__33eddcacfcf4eae8_arg_types_var_15644416245097476908[2] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419 };
const char * __type_info__33eddcacfcf4eae8_arg_names_var_15644416245097476908[2] = { "self", "ctx" };
VarInfo __struct_info__d91c21afef62b32c_field_6 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__33eddcacfcf4eae8_arg_types_var_15644416245097476908, __type_info__33eddcacfcf4eae8_arg_names_var_15644416245097476908, 2, 0, nullptr, 12, 8, UINT64_C(0x33eddcacfcf4eae8), "onSimulateContext", offsetof(debugapi::DapiDebugAgent,onSimulateContext), 0 };
TypeInfo * __type_info__d5dfe6dacc8ce102_arg_types_var_15644416245097476908[3] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__246dda13a8a4b104 };
const char * __type_info__d5dfe6dacc8ce102_arg_names_var_15644416245097476908[3] = { "self", "ctx", "at" };
VarInfo __struct_info__d91c21afef62b32c_field_7 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__d5dfe6dacc8ce102_arg_types_var_15644416245097476908, __type_info__d5dfe6dacc8ce102_arg_names_var_15644416245097476908, 3, 0, nullptr, 12, 8, UINT64_C(0xd5dfe6dacc8ce102), "onSingleStep", offsetof(debugapi::DapiDebugAgent,onSingleStep), 0 };
TypeInfo * __type_info__327f7c9a660b1ab9_arg_types_var_15644416245097476908[3] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__246dda13a8a4b104 };
const char * __type_info__327f7c9a660b1ab9_arg_names_var_15644416245097476908[3] = { "self", "ctx", "at" };
VarInfo __struct_info__d91c21afef62b32c_field_8 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__327f7c9a660b1ab9_arg_types_var_15644416245097476908, __type_info__327f7c9a660b1ab9_arg_names_var_15644416245097476908, 3, 0, nullptr, 12, 8, UINT64_C(0x327f7c9a660b1ab9), "onInstrument", offsetof(debugapi::DapiDebugAgent,onInstrument), 0 };
TypeInfo * __type_info__21b69962c22005be_arg_types_var_15644416245097476908[5] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__ed65100a3b73031a, &__type_info__af81fe4c86352052, &__type_info__b68d800849332aec };
const char * __type_info__21b69962c22005be_arg_names_var_15644416245097476908[5] = { "self", "ctx", "fn", "entering", "userData" };
VarInfo __struct_info__d91c21afef62b32c_field_9 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__21b69962c22005be_arg_types_var_15644416245097476908, __type_info__21b69962c22005be_arg_names_var_15644416245097476908, 5, 0, nullptr, 12, 8, UINT64_C(0x21b69962c22005be), "onInstrumentFunction", offsetof(debugapi::DapiDebugAgent,onInstrumentFunction), 0 };
TypeInfo * __type_info__c913774fc6d2e043_arg_types_var_15644416245097476908[5] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__246dda13a8a4b104, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
const char * __type_info__c913774fc6d2e043_arg_names_var_15644416245097476908[5] = { "self", "ctx", "at", "reason", "text" };
VarInfo __struct_info__d91c21afef62b32c_field_10 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__c913774fc6d2e043_arg_types_var_15644416245097476908, __type_info__c913774fc6d2e043_arg_names_var_15644416245097476908, 5, 0, nullptr, 12, 8, UINT64_C(0xc913774fc6d2e043), "onBreakpoint", offsetof(debugapi::DapiDebugAgent,onBreakpoint), 0 };
TypeInfo * __type_info__79d44cbc9a657bf4_arg_types_var_15644416245097476908[6] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__bc67beb4aa160fd4, &__type_info__9c225ec61b3e6a3c };
const char * __type_info__79d44cbc9a657bf4_arg_names_var_15644416245097476908[6] = { "self", "ctx", "category", "name", "info", "data" };
VarInfo __struct_info__d91c21afef62b32c_field_11 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__79d44cbc9a657bf4_arg_types_var_15644416245097476908, __type_info__79d44cbc9a657bf4_arg_names_var_15644416245097476908, 6, 0, nullptr, 12, 8, UINT64_C(0x79d44cbc9a657bf4), "onVariable", offsetof(debugapi::DapiDebugAgent,onVariable), 0 };
TypeInfo * __type_info__a73424c91982d80c_arg_types_var_15644416245097476908[3] = { &__type_info__4e03c6707d012d5b, &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52 };
const char * __type_info__a73424c91982d80c_arg_names_var_15644416245097476908[3] = { "self", "file", "breakpointsNum" };
VarInfo __struct_info__d91c21afef62b32c_field_12 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__a73424c91982d80c_arg_types_var_15644416245097476908, __type_info__a73424c91982d80c_arg_names_var_15644416245097476908, 3, 0, nullptr, 12, 8, UINT64_C(0xa73424c91982d80c), "onBreakpointsReset", offsetof(debugapi::DapiDebugAgent,onBreakpointsReset), 0 };
TypeInfo * __type_info__edcf7675d0697793_arg_types_var_15644416245097476908[1] = { &__type_info__4e03c6707d012d5b };
const char * __type_info__edcf7675d0697793_arg_names_var_15644416245097476908[1] = { "self" };
VarInfo __struct_info__d91c21afef62b32c_field_13 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__edcf7675d0697793_arg_types_var_15644416245097476908, __type_info__edcf7675d0697793_arg_names_var_15644416245097476908, 1, 0, nullptr, 12, 8, UINT64_C(0xedcf7675d0697793), "onTick", offsetof(debugapi::DapiDebugAgent,onTick), 0 };
TypeInfo * __type_info__913dec62813bfe4d_arg_types_var_15644416245097476908[3] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__246dda13a8a4b104 };
const char * __type_info__913dec62813bfe4d_arg_names_var_15644416245097476908[3] = { "self", "ctx", "at" };
VarInfo __struct_info__d91c21afef62b32c_field_14 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__913dec62813bfe4d_arg_types_var_15644416245097476908, __type_info__913dec62813bfe4d_arg_names_var_15644416245097476908, 3, 0, nullptr, 12, 8, UINT64_C(0x913dec62813bfe4d), "onCollect", offsetof(debugapi::DapiDebugAgent,onCollect), 0 };
TypeInfo * __type_info__68ba67f3d12e3c98_arg_types_var_15644416245097476908[5] = { &__type_info__4e03c6707d012d5b, &__type_info__2d750e15c3790305, &__type_info__a9e212d4f301a82c, &__type_info__af8afe4c86446b52, &__type_info__921d3d0750982f13 };
const char * __type_info__68ba67f3d12e3c98_arg_names_var_15644416245097476908[5] = { "self", "context", "at", "level", "text" };
VarInfo __struct_info__d91c21afef62b32c_field_15 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__68ba67f3d12e3c98_arg_types_var_15644416245097476908, __type_info__68ba67f3d12e3c98_arg_names_var_15644416245097476908, 5, 0, nullptr, 12, 8, UINT64_C(0x68ba67f3d12e3c98), "onLog", offsetof(debugapi::DapiDebugAgent,onLog), 0 };
TypeInfo * __type_info__36c2061312a1876b_arg_types_var_15644416245097476908[2] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419 };
const char * __type_info__36c2061312a1876b_arg_names_var_15644416245097476908[2] = { "self", "ctx" };
VarInfo __struct_info__d91c21afef62b32c_field_16 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__36c2061312a1876b_arg_types_var_15644416245097476908, __type_info__36c2061312a1876b_arg_names_var_15644416245097476908, 2, 0, nullptr, 12, 8, UINT64_C(0x36c2061312a1876b), "onBeforeGC", offsetof(debugapi::DapiDebugAgent,onBeforeGC), 0 };
TypeInfo * __type_info__d024cad15fe2631f_arg_types_var_15644416245097476908[2] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419 };
const char * __type_info__d024cad15fe2631f_arg_names_var_15644416245097476908[2] = { "self", "ctx" };
VarInfo __struct_info__d91c21afef62b32c_field_17 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__d024cad15fe2631f_arg_types_var_15644416245097476908, __type_info__d024cad15fe2631f_arg_names_var_15644416245097476908, 2, 0, nullptr, 12, 8, UINT64_C(0xd024cad15fe2631f), "onAfterGC", offsetof(debugapi::DapiDebugAgent,onAfterGC), 0 };
TypeInfo * __type_info__df3b4662672293c8_arg_types_var_15644416245097476908[2] = { &__type_info__4e03c6707d012d5b, &__type_info__921d3d0750982f13 };
const char * __type_info__df3b4662672293c8_arg_names_var_15644416245097476908[2] = { "self", "command" };
VarInfo __struct_info__d91c21afef62b32c_field_18 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__df3b4662672293c8_arg_types_var_15644416245097476908, __type_info__df3b4662672293c8_arg_names_var_15644416245097476908, 2, 0, nullptr, 12, 8, UINT64_C(0xdf3b4662672293c8), "onUserCommand", offsetof(debugapi::DapiDebugAgent,onUserCommand), 0 };
TypeInfo * __type_info__8055238a52f31e0c_arg_types_var_15644416245097476908[5] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__9c225ec61b3e6a3c, &__type_info__b68d800849332aec, &__type_info__246dda13a8a4b104 };
const char * __type_info__8055238a52f31e0c_arg_names_var_15644416245097476908[5] = { "self", "ctx", "data", "size", "at" };
VarInfo __struct_info__d91c21afef62b32c_field_19 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__8055238a52f31e0c_arg_types_var_15644416245097476908, __type_info__8055238a52f31e0c_arg_names_var_15644416245097476908, 5, 0, nullptr, 12, 8, UINT64_C(0x8055238a52f31e0c), "onAllocate", offsetof(debugapi::DapiDebugAgent,onAllocate), 0 };
TypeInfo * __type_info__3d1ee78d21cd5f19_arg_types_var_15644416245097476908[7] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__9c225ec61b3e6a3c, &__type_info__b68d800849332aec, &__type_info__9c225ec61b3e6a3c, &__type_info__b68d800849332aec, &__type_info__246dda13a8a4b104 };
const char * __type_info__3d1ee78d21cd5f19_arg_names_var_15644416245097476908[7] = { "self", "ctx", "data", "size", "newData", "newSize", "at" };
VarInfo __struct_info__d91c21afef62b32c_field_20 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__3d1ee78d21cd5f19_arg_types_var_15644416245097476908, __type_info__3d1ee78d21cd5f19_arg_names_var_15644416245097476908, 7, 0, nullptr, 12, 8, UINT64_C(0x3d1ee78d21cd5f19), "onReallocate", offsetof(debugapi::DapiDebugAgent,onReallocate), 0 };
TypeInfo * __type_info__6ff5841b5071093e_arg_types_var_15644416245097476908[4] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__9c225ec61b3e6a3c, &__type_info__246dda13a8a4b104 };
const char * __type_info__6ff5841b5071093e_arg_names_var_15644416245097476908[4] = { "self", "ctx", "data", "at" };
VarInfo __struct_info__d91c21afef62b32c_field_21 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__6ff5841b5071093e_arg_types_var_15644416245097476908, __type_info__6ff5841b5071093e_arg_names_var_15644416245097476908, 4, 0, nullptr, 12, 8, UINT64_C(0x6ff5841b5071093e), "onFree", offsetof(debugapi::DapiDebugAgent,onFree), 0 };
TypeInfo * __type_info__f1f0b5451e923dbc_arg_types_var_15644416245097476908[5] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__9c225ec61b3e6a3c, &__type_info__b68d800849332aec, &__type_info__246dda13a8a4b104 };
const char * __type_info__f1f0b5451e923dbc_arg_names_var_15644416245097476908[5] = { "self", "ctx", "data", "size", "at" };
VarInfo __struct_info__d91c21afef62b32c_field_22 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__f1f0b5451e923dbc_arg_types_var_15644416245097476908, __type_info__f1f0b5451e923dbc_arg_names_var_15644416245097476908, 5, 0, nullptr, 12, 8, UINT64_C(0xf1f0b5451e923dbc), "onAllocateString", offsetof(debugapi::DapiDebugAgent,onAllocateString), 0 };
TypeInfo * __type_info__c283a5754fe3daf6_arg_types_var_15644416245097476908[4] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__9c225ec61b3e6a3c, &__type_info__246dda13a8a4b104 };
const char * __type_info__c283a5754fe3daf6_arg_names_var_15644416245097476908[4] = { "self", "ctx", "data", "at" };
VarInfo __struct_info__d91c21afef62b32c_field_23 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__c283a5754fe3daf6_arg_types_var_15644416245097476908, __type_info__c283a5754fe3daf6_arg_names_var_15644416245097476908, 4, 0, nullptr, 12, 8, UINT64_C(0xc283a5754fe3daf6), "onFreeString", offsetof(debugapi::DapiDebugAgent,onFreeString), 0 };
VarInfo __struct_info__d91c21afef62b32c_field_24 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__44ca287faf79178, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0x817bf1fe93f2f5d7), "thisAgent", offsetof(debugapi::DapiDebugAgent,thisAgent), 25 };
VarInfo * __struct_info__d91c21afef62b32c_fields[25] =  { &__struct_info__d91c21afef62b32c_field_0, &__struct_info__d91c21afef62b32c_field_1, &__struct_info__d91c21afef62b32c_field_2, &__struct_info__d91c21afef62b32c_field_3, &__struct_info__d91c21afef62b32c_field_4, &__struct_info__d91c21afef62b32c_field_5, &__struct_info__d91c21afef62b32c_field_6, &__struct_info__d91c21afef62b32c_field_7, &__struct_info__d91c21afef62b32c_field_8, &__struct_info__d91c21afef62b32c_field_9, &__struct_info__d91c21afef62b32c_field_10, &__struct_info__d91c21afef62b32c_field_11, &__struct_info__d91c21afef62b32c_field_12, &__struct_info__d91c21afef62b32c_field_13, &__struct_info__d91c21afef62b32c_field_14, &__struct_info__d91c21afef62b32c_field_15, &__struct_info__d91c21afef62b32c_field_16, &__struct_info__d91c21afef62b32c_field_17, &__struct_info__d91c21afef62b32c_field_18, &__struct_info__d91c21afef62b32c_field_19, &__struct_info__d91c21afef62b32c_field_20, &__struct_info__d91c21afef62b32c_field_21, &__struct_info__d91c21afef62b32c_field_22, &__struct_info__d91c21afef62b32c_field_23, &__struct_info__d91c21afef62b32c_field_24 };
StructInfo __struct_info__d91c21afef62b32c = {"DapiDebugAgent", "debugapi", 13, __struct_info__d91c21afef62b32c_fields, 25, 200, UINT64_C(0x0), nullptr, UINT64_C(0xd91c21afef62b32c), 0 };
VarInfo __struct_info__31e0f560cc005077_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0x280f562af3da5a1), "__rtti", offsetof(debugapi::DapiStackWalker,__rtti), 14 };
TypeInfo * __type_info__894071b3265e2991_arg_types_var_3594142298729894007[1] = { &__type_info__a4d736e8781a7b06 };
const char * __type_info__894071b3265e2991_arg_names_var_3594142298729894007[1] = { "self" };
VarInfo __struct_info__31e0f560cc005077_field_1 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__894071b3265e2991_arg_types_var_3594142298729894007, __type_info__894071b3265e2991_arg_names_var_3594142298729894007, 1, 0, nullptr, 12, 8, UINT64_C(0x894071b3265e2991), "__finalize", offsetof(debugapi::DapiStackWalker,__finalize), 0 };
TypeInfo * __type_info__ba24a7dae93892bb_arg_types_var_3594142298729894007[1] = { &__type_info__a4d736e8781a7b06 };
const char * __type_info__ba24a7dae93892bb_arg_names_var_3594142298729894007[1] = { "self" };
VarInfo __struct_info__31e0f560cc005077_field_2 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__ba24a7dae93892bb_arg_types_var_3594142298729894007, __type_info__ba24a7dae93892bb_arg_names_var_3594142298729894007, 1, 0, nullptr, 12, 8, UINT64_C(0xba24a7dae93892bb), "canWalkArguments", offsetof(debugapi::DapiStackWalker,canWalkArguments), 0 };
TypeInfo * __type_info__cc398c743a2640e0_arg_types_var_3594142298729894007[1] = { &__type_info__a4d736e8781a7b06 };
const char * __type_info__cc398c743a2640e0_arg_names_var_3594142298729894007[1] = { "self" };
VarInfo __struct_info__31e0f560cc005077_field_3 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__cc398c743a2640e0_arg_types_var_3594142298729894007, __type_info__cc398c743a2640e0_arg_names_var_3594142298729894007, 1, 0, nullptr, 12, 8, UINT64_C(0xcc398c743a2640e0), "canWalkVariables", offsetof(debugapi::DapiStackWalker,canWalkVariables), 0 };
TypeInfo * __type_info__e7ea6856c665cde7_arg_types_var_3594142298729894007[1] = { &__type_info__a4d736e8781a7b06 };
const char * __type_info__e7ea6856c665cde7_arg_names_var_3594142298729894007[1] = { "self" };
VarInfo __struct_info__31e0f560cc005077_field_4 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__e7ea6856c665cde7_arg_types_var_3594142298729894007, __type_info__e7ea6856c665cde7_arg_names_var_3594142298729894007, 1, 0, nullptr, 12, 8, UINT64_C(0xe7ea6856c665cde7), "canWalkOutOfScopeVariables", offsetof(debugapi::DapiStackWalker,canWalkOutOfScopeVariables), 0 };
TypeInfo * __type_info__6215cbf9e4be26c_arg_types_var_3594142298729894007[3] = { &__type_info__a4d736e8781a7b06, &__type_info__e421b7d32038f816, &__type_info__9c225ec61b3e6a3c };
const char * __type_info__6215cbf9e4be26c_arg_names_var_3594142298729894007[3] = { "self", "pp", "sp" };
VarInfo __struct_info__31e0f560cc005077_field_5 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__6215cbf9e4be26c_arg_types_var_3594142298729894007, __type_info__6215cbf9e4be26c_arg_names_var_3594142298729894007, 3, 0, nullptr, 12, 8, UINT64_C(0x6215cbf9e4be26c), "onBeforeCall", offsetof(debugapi::DapiStackWalker,onBeforeCall), 0 };
TypeInfo * __type_info__f396ff83ffb05655_arg_types_var_3594142298729894007[3] = { &__type_info__a4d736e8781a7b06, &__type_info__e421b7d32038f816, &__type_info__921d3d0750982f13 };
const char * __type_info__f396ff83ffb05655_arg_names_var_3594142298729894007[3] = { "self", "pp", "fileName" };
VarInfo __struct_info__31e0f560cc005077_field_6 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__f396ff83ffb05655_arg_types_var_3594142298729894007, __type_info__f396ff83ffb05655_arg_names_var_3594142298729894007, 3, 0, nullptr, 12, 8, UINT64_C(0xf396ff83ffb05655), "onCallAOT", offsetof(debugapi::DapiStackWalker,onCallAOT), 0 };
TypeInfo * __type_info__ac62bd978c461883_arg_types_var_3594142298729894007[4] = { &__type_info__a4d736e8781a7b06, &__type_info__e421b7d32038f816, &__type_info__40912bc113ca9a8a, &__type_info__246dda13a8a4b104 };
const char * __type_info__ac62bd978c461883_arg_names_var_3594142298729894007[4] = { "self", "pp", "info", "at" };
VarInfo __struct_info__31e0f560cc005077_field_7 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__ac62bd978c461883_arg_types_var_3594142298729894007, __type_info__ac62bd978c461883_arg_names_var_3594142298729894007, 4, 0, nullptr, 12, 8, UINT64_C(0xac62bd978c461883), "onCallAt", offsetof(debugapi::DapiStackWalker,onCallAt), 0 };
TypeInfo * __type_info__6e8c47174cd471b0_arg_types_var_3594142298729894007[3] = { &__type_info__a4d736e8781a7b06, &__type_info__e421b7d32038f816, &__type_info__40912bc113ca9a8a };
const char * __type_info__6e8c47174cd471b0_arg_names_var_3594142298729894007[3] = { "self", "pp", "info" };
VarInfo __struct_info__31e0f560cc005077_field_8 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__6e8c47174cd471b0_arg_types_var_3594142298729894007, __type_info__6e8c47174cd471b0_arg_names_var_3594142298729894007, 3, 0, nullptr, 12, 8, UINT64_C(0x6e8c47174cd471b0), "onCall", offsetof(debugapi::DapiStackWalker,onCall), 0 };
TypeInfo * __type_info__bb7dd188f2b1d03a_arg_types_var_3594142298729894007[3] = { &__type_info__a4d736e8781a7b06, &__type_info__e421b7d32038f816, &__type_info__9c225ec61b3e6a3c };
const char * __type_info__bb7dd188f2b1d03a_arg_names_var_3594142298729894007[3] = { "self", "pp", "sp" };
VarInfo __struct_info__31e0f560cc005077_field_9 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__bb7dd188f2b1d03a_arg_types_var_3594142298729894007, __type_info__bb7dd188f2b1d03a_arg_names_var_3594142298729894007, 3, 0, nullptr, 12, 8, UINT64_C(0xbb7dd188f2b1d03a), "onAfterPrologue", offsetof(debugapi::DapiStackWalker,onAfterPrologue), 0 };
TypeInfo * __type_info__d1b1bc8ffb15d968_arg_types_var_3594142298729894007[5] = { &__type_info__a4d736e8781a7b06, &__type_info__40912bc113ca9a8a, &__type_info__af8afe4c86446b52, &__type_info__c3770d5f54a928b3, &__type_info__7c9b820817d2b152 };
const char * __type_info__d1b1bc8ffb15d968_arg_names_var_3594142298729894007[5] = { "self", "info", "index", "vinfo", "arg" };
VarInfo __struct_info__31e0f560cc005077_field_10 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__d1b1bc8ffb15d968_arg_types_var_3594142298729894007, __type_info__d1b1bc8ffb15d968_arg_names_var_3594142298729894007, 5, 0, nullptr, 12, 8, UINT64_C(0xd1b1bc8ffb15d968), "onArgument", offsetof(debugapi::DapiStackWalker,onArgument), 0 };
TypeInfo * __type_info__c0460d6b0b9f90c3_arg_types_var_3594142298729894007[1] = { &__type_info__a4d736e8781a7b06 };
const char * __type_info__c0460d6b0b9f90c3_arg_names_var_3594142298729894007[1] = { "self" };
VarInfo __struct_info__31e0f560cc005077_field_11 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__c0460d6b0b9f90c3_arg_types_var_3594142298729894007, __type_info__c0460d6b0b9f90c3_arg_names_var_3594142298729894007, 1, 0, nullptr, 12, 8, UINT64_C(0xc0460d6b0b9f90c3), "onBeforeVariables", offsetof(debugapi::DapiStackWalker,onBeforeVariables), 0 };
TypeInfo * __type_info__632c5e0a5c27b74e_arg_types_var_3594142298729894007[5] = { &__type_info__a4d736e8781a7b06, &__type_info__40912bc113ca9a8a, &__type_info__873433ff256e8173, &__type_info__9c225ec61b3e6a3c, &__type_info__af81fe4c86352052 };
const char * __type_info__632c5e0a5c27b74e_arg_names_var_3594142298729894007[5] = { "self", "inf", "vinfo", "arg", "inScope" };
VarInfo __struct_info__31e0f560cc005077_field_12 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__632c5e0a5c27b74e_arg_types_var_3594142298729894007, __type_info__632c5e0a5c27b74e_arg_names_var_3594142298729894007, 5, 0, nullptr, 12, 8, UINT64_C(0x632c5e0a5c27b74e), "onVariable", offsetof(debugapi::DapiStackWalker,onVariable), 0 };
TypeInfo * __type_info__3e54f61c3199cebd_arg_types_var_3594142298729894007[2] = { &__type_info__a4d736e8781a7b06, &__type_info__e421b7d32038f816 };
const char * __type_info__3e54f61c3199cebd_arg_names_var_3594142298729894007[2] = { "self", "pp" };
VarInfo __struct_info__31e0f560cc005077_field_13 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__3e54f61c3199cebd_arg_types_var_3594142298729894007, __type_info__3e54f61c3199cebd_arg_names_var_3594142298729894007, 2, 0, nullptr, 12, 8, UINT64_C(0x3e54f61c3199cebd), "onAfterCall", offsetof(debugapi::DapiStackWalker,onAfterCall), 0 };
VarInfo * __struct_info__31e0f560cc005077_fields[14] =  { &__struct_info__31e0f560cc005077_field_0, &__struct_info__31e0f560cc005077_field_1, &__struct_info__31e0f560cc005077_field_2, &__struct_info__31e0f560cc005077_field_3, &__struct_info__31e0f560cc005077_field_4, &__struct_info__31e0f560cc005077_field_5, &__struct_info__31e0f560cc005077_field_6, &__struct_info__31e0f560cc005077_field_7, &__struct_info__31e0f560cc005077_field_8, &__struct_info__31e0f560cc005077_field_9, &__struct_info__31e0f560cc005077_field_10, &__struct_info__31e0f560cc005077_field_11, &__struct_info__31e0f560cc005077_field_12, &__struct_info__31e0f560cc005077_field_13 };
StructInfo __struct_info__31e0f560cc005077 = {"DapiStackWalker", "debugapi", 13, __struct_info__31e0f560cc005077_fields, 14, 112, UINT64_C(0x0), nullptr, UINT64_C(0x31e0f560cc005077), 0 };
TypeInfo __type_info__921d3d0750982f13 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16484, 8, UINT64_C(0x921d3d0750982f13) };
TypeInfo __type_info__a9e212d4f301a82c = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__246dda13a8a4b104, nullptr, nullptr, nullptr, 0, 0, nullptr, 8236, 8, UINT64_C(0xa9e212d4f301a82c) };
TypeInfo __type_info__209f9e4a9562d5c2 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__44ca287faf79178, nullptr, nullptr, nullptr, 0, 0, nullptr, 8236, 8, UINT64_C(0x209f9e4a9562d5c2) };
TypeInfo __type_info__2d750e15c3790305 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__767637ee1a337419, nullptr, nullptr, nullptr, 0, 0, nullptr, 24620, 8, UINT64_C(0x2d750e15c3790305) };
TypeInfo __type_info__ed65100a3b73031a = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__5ac778689ccf4816, nullptr, nullptr, nullptr, 0, 0, nullptr, 24620, 8, UINT64_C(0xed65100a3b73031a) };
TypeInfo __type_info__9c225ec61b3e6a3c = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, nullptr, nullptr, 0, 0, nullptr, 8236, 8, UINT64_C(0x9c225ec61b3e6a3c) };
TypeInfo __type_info__e421b7d32038f816 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~debugapi::Prologue"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24622, 32, UINT64_C(0xe421b7d32038f816) };
TypeInfo __type_info__40912bc113ca9a8a = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~rtti::FuncInfo"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24622, 80, UINT64_C(0x40912bc113ca9a8a) };
TypeInfo __type_info__246dda13a8a4b104 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~rtti::LineInfo"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 8230, 24, UINT64_C(0x246dda13a8a4b104) };
TypeInfo __type_info__873433ff256e8173 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~rtti::LocalVariableInfo"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24614, 120, UINT64_C(0x873433ff256e8173) };
TypeInfo __type_info__bc67beb4aa160fd4 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~rtti::TypeInfo"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24622, 80, UINT64_C(0xbc67beb4aa160fd4) };
TypeInfo __type_info__c3770d5f54a928b3 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~rtti::VarInfo"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24614, 128, UINT64_C(0xc3770d5f54a928b3) };
TypeInfo __type_info__faddef75a1c73c1f = { Type::tPointer, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 8236, 8, UINT64_C(0xfaddef75a1c73c1f) };
TypeInfo __type_info__faddb375a1c6d62b = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, 8, UINT64_C(0xfaddb375a1c6d62b) };
TypeInfo __type_info__af81fe4c86352052 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 1, UINT64_C(0xaf81fe4c86352052) };
TypeInfo __type_info__7c9b820817d2b152 = { Type::tFloat4, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 16, UINT64_C(0x7c9b820817d2b152) };
TypeInfo __type_info__af8afe4c86446b52 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 4, UINT64_C(0xaf8afe4c86446b52) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, 8, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__b68d800849332aec = { Type::tUInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 8, UINT64_C(0xb68d800849332aec) };
TypeInfo __type_info__44ca287faf79178 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~debugapi::DebugAgent"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 2, 24, UINT64_C(0x44ca287faf79178) };
TypeInfo __type_info__87688966731f2665 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~debugapi::StackWalker"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 2, 16, UINT64_C(0x87688966731f2665) };
TypeInfo __type_info__767637ee1a337419 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~rtti::Context"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, 576, UINT64_C(0x767637ee1a337419) };
TypeInfo __type_info__5ac778689ccf4816 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~rtti::SimFunction"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24590, 56, UINT64_C(0x5ac778689ccf4816) };
TypeInfo __type_info__2ab2203ab786034d = { Type::tStructure, &__struct_info__550ac570675ff176, nullptr, nullptr, &__type_info__4e03c6707d012d5b, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, 216, UINT64_C(0x2ab2203ab786034d) };
TypeInfo __type_info__80bd755d94e49a69 = { Type::tStructure, &__struct_info__f935e99ac014dfbe, nullptr, nullptr, &__type_info__a4d736e8781a7b06, nullptr, nullptr, nullptr, 0, 0, nullptr, 24590, 120, UINT64_C(0x80bd755d94e49a69) };
TypeInfo __type_info__4e03c6707d012d5b = { Type::tStructure, &__struct_info__d91c21afef62b32c, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24590, 200, UINT64_C(0x4e03c6707d012d5b) };
TypeInfo __type_info__a4d736e8781a7b06 = { Type::tStructure, &__struct_info__31e0f560cc005077, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24590, 112, UINT64_C(0xa4d736e8781a7b06) };
TypeInfo __type_info__af63df4c8601f1a5 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0xaf63df4c8601f1a5) };
TypeInfo __type_info__af63eb4c86020609 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 0, UINT64_C(0xaf63eb4c86020609) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {__type_info__e421b7d32038f816, __type_info__40912bc113ca9a8a, __type_info__246dda13a8a4b104, __type_info__873433ff256e8173, __type_info__bc67beb4aa160fd4, __type_info__c3770d5f54a928b3, __type_info__44ca287faf79178, __type_info__87688966731f2665, __type_info__767637ee1a337419, __type_info__5ac778689ccf4816, };
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_c81a84221fb6bad2 ( Context * __context__, ast_debug::SampleStackWalker const  & __cl_rename_at_116_0 );
inline bool _Funcast_debugTickisModulePtrTypeTick12629776342530398480_56257da3a0e7b243 ( Context * __context__, VarInfo const  & ___vinfo_rename_at_36_1, char * const  __mod_rename_at_36_2, char * const  __what_rename_at_36_3 );
inline bool _Funcast_debugTickisModulePtrTypeTick12629776342530398480_a210fcb9569bff8c ( Context * __context__, LocalVariableInfo const  & ___vinfo_rename_at_36_5, char * const  __mod_rename_at_36_6, char * const  __what_rename_at_36_7 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_7c5d3c9bad07d43c ( Context * __context__, ast_debug::ContextStateAgent const  & __cl_rename_at_116_9 );
inline bool _Funcast_debugTickisRttiTypeTick4516636421360898655_d3b3aacafe01c827 ( Context * __context__, VarInfo const  & __vinfo_rename_at_61_10, char * const  __what_rename_at_61_11 );
inline void _Funcast_debugTickreport_to_debuggerTick2408265038224314806_2643678afa1dfd37 ( Context * __context__, Context & __ctx_rename_at_13_12, char * const  __category_rename_at_13_13, char * const  __name_rename_at_13_14, char * const  __value_rename_at_13_15 );
inline void _Funcast_debugTickreport_to_debuggerTick2408265038224314806_48ecdbe80a06d95e ( Context * __context__, Context & __ctx_rename_at_13_18, char * const  __category_rename_at_13_19, char * const  __name_rename_at_13_20, void * const  __value_rename_at_13_21 );
inline bool _Funcast_debugTickisExpressionTypeTick1683607289072304170_59fb59f45b96aa56 ( Context * __context__, VarInfo const  & ___vinfo_rename_at_19_24 );
inline char * _FuncastTickdescribeTick842554968825501494_452cfc8955963a18 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_48_26 );
inline bool _Funcast_debugTickisAstPtrTypeTick3276036939226023356_3b7ab890c635e212 ( Context * __context__, VarInfo const  & ___vinfo_rename_at_53_27, char * const  __what_rename_at_53_28 );
inline char * _FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __decl_rename_at_38_29, bool __extra_rename_at_38_30, bool __contracts_rename_at_38_31, bool __modules_rename_at_38_32 );
inline char * _FuncastTickdescribeTick16391739697169902387_22f6094444e3262d ( Context * __context__, smart_ptr_raw<Function> const  __expr_rename_at_53_33 );
inline bool _Funcast_debugTickisRttiPtrTypeTick806345736983820832_48d56f4f80ba3b53 ( Context * __context__, VarInfo const  & ___vinfo_rename_at_57_34, char * const  __what_rename_at_57_35 );
inline bool _Funcast_debugTickisRttiTypeTick4516636421360898655_2f19fa27e91344c2 ( Context * __context__, LocalVariableInfo const  & __vinfo_rename_at_61_36, char * const  __what_rename_at_61_37 );
inline bool _Funcast_debugTickisExpressionTypeTick1683607289072304170_967b88ce98476055 ( Context * __context__, LocalVariableInfo const  & ___vinfo_rename_at_19_38 );
inline bool _Funcast_debugTickisAstPtrTypeTick3276036939226023356_1d28eff7d398c8a3 ( Context * __context__, LocalVariableInfo const  & ___vinfo_rename_at_53_40, char * const  __what_rename_at_53_41 );
inline bool _Funcast_debugTickisRttiPtrTypeTick806345736983820832_515c5b2443eb4ef0 ( Context * __context__, LocalVariableInfo const  & ___vinfo_rename_at_57_42, char * const  __what_rename_at_57_43 );
inline void _Funcast_debugTickdescribe_argTick8826490271305289944_efb00f1647d906b2 ( Context * __context__, Context & __ctxid_rename_at_74_44, VarInfo const  & __vinfo_rename_at_74_45, void * const  __arg_rename_at_74_46 );
inline void _Funcast_debugTickdescribe_argTick8826490271305289944_af045ee043c050cf ( Context * __context__, Context & __ctxid_rename_at_74_48, LocalVariableInfo const  & __vinfo_rename_at_74_49, void * const  __arg_rename_at_74_50 );
inline smart_ptr_raw<StackWalker> _FuncdebugapiTickmake_stack_walkerTick10101906578839871846_229ea554e31d57ca ( Context * __context__, ast_debug::SampleStackWalker * const  __classPtr_rename_at_216_52 );
inline void finalize_500fce7609128ffb ( Context * __context__, ast_debug::SampleStackWalker * & ____this_rename_at_143_54 );
inline void finalize_5733f4cfa043e920 ( Context * __context__, ast_debug::SampleStackWalker & ____this_rename_at_103_56 );
inline void _FuncdebugapiTickinstall_new_debug_agentTick3897311473844341055_3e1c20224cd19f2e ( Context * __context__, ast_debug::ContextStateAgent * __agentPtr_rename_at_37_57, char * const  __category_rename_at_37_58 );
inline void _FuncSampleStackWalkerTickonArgument_b3c1b48f676b9bbd ( Context * __context__, ast_debug::SampleStackWalker & __self_rename_at_105_61, FuncInfo const  & __info_rename_at_105_62, int32_t __index_rename_at_105_63, VarInfo const  & __vinfo_rename_at_105_64, float4 __arg_rename_at_105_65 );
inline void _FuncSampleStackWalkerTickonVariable_b047f0cc7c7030d6 ( Context * __context__, ast_debug::SampleStackWalker & __self_rename_at_114_66, FuncInfo const  & __inf_rename_at_114_67, LocalVariableInfo const  & __vinfo_rename_at_114_68, void * const  __arg_rename_at_114_69, bool __inScope_rename_at_114_70 );
inline bool _FuncSampleStackWalkerTickonAfterCall_eedb864288cb4a8a ( Context * __context__, ast_debug::SampleStackWalker & __self_rename_at_126_71, Prologue const  & __pp_rename_at_126_72 );
inline void _FuncSampleStackWalker_0x27___finalize_af21cd893b46a727 ( Context * __context__, ast_debug::SampleStackWalker & __self_rename_at_103_73 );
inline ast_debug::ContextStateAgent ContextStateAgent_e10fdff26046cd6f ( Context * __context__ );
inline void _FuncContextStateAgentTickContextStateAgent_ce0829614ebac3e5 ( Context * __context__, ast_debug::ContextStateAgent & __self_rename_at_134_75 );
inline void finalize_dcfff9e2129da1aa ( Context * __context__, ast_debug::ContextStateAgent & __self_rename_at_140_76 );
inline void _FuncContextStateAgentTickonCollect_d5b055adc3fcc695 ( Context * __context__, ast_debug::ContextStateAgent & __self_rename_at_146_77, Context & __ctx_rename_at_146_78, LineInfo const  & __at_rename_at_146_79 );
inline void _FuncContextStateAgent_0x27___finalize_c2bf11f8c2d8aef ( Context * __context__, ast_debug::ContextStateAgent & __self_rename_at_131_80 );
inline void state_agent_a7d0d7fe180b2c1a ( Context * __context__, Context const  & __ctx_rename_at_153_81 );
inline ast_debug::SampleStackWalker SampleStackWalker_b9ed6b4c0fa80af2 ( Context * __context__ );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_c81a84221fb6bad2 ( Context * __context__, ast_debug::SampleStackWalker const  &  __cl_rename_at_116_0 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_0.__rtti))).getStructType())));
}

inline bool _Funcast_debugTickisModulePtrTypeTick12629776342530398480_56257da3a0e7b243 ( Context * __context__, VarInfo const  &  ___vinfo_rename_at_36_1, char * const  __mod_rename_at_36_2, char * const  __what_rename_at_36_3 )
{
    if ( (___vinfo_rename_at_36_1.type /*basicType*/ != DAS_COMMENT(bound_enum) das::Type::tPointer) || (___vinfo_rename_at_36_1.firstType /*firstType*/ == nullptr) )
    {
        return das_auto_cast<bool>::cast(false);
    } else {
        TypeInfo * __vinfo_rename_at_40_4 = ((TypeInfo *)___vinfo_rename_at_36_1.firstType /*firstType*/);
        return das_auto_cast<bool>::cast((__vinfo_rename_at_40_4->type /*basicType*/ != DAS_COMMENT(bound_enum) das::Type::tHandle) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((neq_dstr_str(das_arg<das::string>::pass(((das_deref(__context__,__vinfo_rename_at_40_4)).getAnnotation())->module /*_module*/->name /*name*/),__mod_rename_at_36_2)) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((neq_dstr_str(das_arg<das::string>::pass(((das_deref(__context__,__vinfo_rename_at_40_4)).getAnnotation())->name /*name*/),__what_rename_at_36_3)) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(true))))));
    };
}

inline bool _Funcast_debugTickisModulePtrTypeTick12629776342530398480_a210fcb9569bff8c ( Context * __context__, LocalVariableInfo const  &  ___vinfo_rename_at_36_5, char * const  __mod_rename_at_36_6, char * const  __what_rename_at_36_7 )
{
    if ( (___vinfo_rename_at_36_5.type /*basicType*/ != DAS_COMMENT(bound_enum) das::Type::tPointer) || (___vinfo_rename_at_36_5.firstType /*firstType*/ == nullptr) )
    {
        return das_auto_cast<bool>::cast(false);
    } else {
        TypeInfo * __vinfo_rename_at_40_8 = ((TypeInfo *)___vinfo_rename_at_36_5.firstType /*firstType*/);
        return das_auto_cast<bool>::cast((__vinfo_rename_at_40_8->type /*basicType*/ != DAS_COMMENT(bound_enum) das::Type::tHandle) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((neq_dstr_str(das_arg<das::string>::pass(((das_deref(__context__,__vinfo_rename_at_40_8)).getAnnotation())->module /*_module*/->name /*name*/),__mod_rename_at_36_6)) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((neq_dstr_str(das_arg<das::string>::pass(((das_deref(__context__,__vinfo_rename_at_40_8)).getAnnotation())->name /*name*/),__what_rename_at_36_7)) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(true))))));
    };
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_7c5d3c9bad07d43c ( Context * __context__, ast_debug::ContextStateAgent const  &  __cl_rename_at_116_9 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_9.__rtti))).getStructType())));
}

inline bool _Funcast_debugTickisRttiTypeTick4516636421360898655_d3b3aacafe01c827 ( Context * __context__, VarInfo const  &  __vinfo_rename_at_61_10, char * const  __what_rename_at_61_11 )
{
    return das_auto_cast<bool>::cast((__vinfo_rename_at_61_10.type /*basicType*/ != DAS_COMMENT(bound_enum) das::Type::tHandle) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((neq_dstr_str(das_arg<das::string>::pass(((__vinfo_rename_at_61_10).getAnnotation())->module /*_module*/->name /*name*/),((char *) "rtti"))) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((neq_dstr_str(das_arg<das::string>::pass(((__vinfo_rename_at_61_10).getAnnotation())->name /*name*/),__what_rename_at_61_11)) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(true))))));
}

inline void _Funcast_debugTickreport_to_debuggerTick2408265038224314806_2643678afa1dfd37 ( Context * __context__, Context &  __ctx_rename_at_13_12, char * const  __category_rename_at_13_13, char * const  __name_rename_at_13_14, char * const  __value_rename_at_13_15 )
{
    TypeInfo __tinfo_rename_at_14_16_ConstRef = ((TypeInfo)(__type_info__faddb375a1c6d62b));
    TypeInfo const  & __tinfo_rename_at_14_16 = __tinfo_rename_at_14_16_ConstRef; ;
    char * const  * __pdata_rename_at_15_17 = ((char * const  *)das_ref(__context__,__value_rename_at_13_15));
    dapiReportContextState(das_arg<Context>::pass(__ctx_rename_at_13_12),__category_rename_at_13_13,__name_rename_at_13_14,das_ref(__context__,__tinfo_rename_at_14_16),das_auto_cast<void * const >::cast(__pdata_rename_at_15_17));
}

inline void _Funcast_debugTickreport_to_debuggerTick2408265038224314806_48ecdbe80a06d95e ( Context * __context__, Context &  __ctx_rename_at_13_18, char * const  __category_rename_at_13_19, char * const  __name_rename_at_13_20, void * const  __value_rename_at_13_21 )
{
    TypeInfo __tinfo_rename_at_14_22_ConstRef = ((TypeInfo)(__type_info__faddef75a1c73c1f));
    TypeInfo const  & __tinfo_rename_at_14_22 = __tinfo_rename_at_14_22_ConstRef; ;
    void * const  * __pdata_rename_at_15_23 = ((void * const  *)das_ref(__context__,__value_rename_at_13_21));
    dapiReportContextState(das_arg<Context>::pass(__ctx_rename_at_13_18),__category_rename_at_13_19,__name_rename_at_13_20,das_ref(__context__,__tinfo_rename_at_14_22),das_auto_cast<void * const >::cast(__pdata_rename_at_15_23));
}

inline bool _Funcast_debugTickisExpressionTypeTick1683607289072304170_59fb59f45b96aa56 ( Context * __context__, VarInfo const  &  ___vinfo_rename_at_19_24 )
{
    if ( (___vinfo_rename_at_19_24.type /*basicType*/ != DAS_COMMENT(bound_enum) das::Type::tPointer) || (___vinfo_rename_at_19_24.firstType /*firstType*/ == nullptr) )
    {
        return das_auto_cast<bool>::cast(false);
    } else {
        TypeInfo * __vinfo_rename_at_23_25 = ((TypeInfo *)___vinfo_rename_at_19_24.firstType /*firstType*/);
        return das_auto_cast<bool>::cast((__vinfo_rename_at_23_25->type /*basicType*/ != DAS_COMMENT(bound_enum) das::Type::tHandle) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((neq_dstr_str(das_arg<das::string>::pass(((das_deref(__context__,__vinfo_rename_at_23_25)).getAnnotation())->module /*_module*/->name /*name*/),((char *) "ast"))) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast((!(builtin_string_startswith(((char * const )(to_das_string(das_arg<das::string>::pass(((das_deref(__context__,__vinfo_rename_at_23_25)).getAnnotation())->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))),((char *) "Expr"),__context__)) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(true))))));
    };
}

inline char * _FuncastTickdescribeTick842554968825501494_452cfc8955963a18 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_48_26 )
{
    return das_auto_cast<char *>::cast(((char * const )(ast_describe_expression(__expr_rename_at_48_26,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline bool _Funcast_debugTickisAstPtrTypeTick3276036939226023356_3b7ab890c635e212 ( Context * __context__, VarInfo const  &  ___vinfo_rename_at_53_27, char * const  __what_rename_at_53_28 )
{
    return das_auto_cast<bool>::cast(_Funcast_debugTickisModulePtrTypeTick12629776342530398480_56257da3a0e7b243(__context__,___vinfo_rename_at_53_27,((char *) "ast"),__what_rename_at_53_28));
}

inline char * _FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __decl_rename_at_38_29, bool __extra_rename_at_38_30, bool __contracts_rename_at_38_31, bool __modules_rename_at_38_32 )
{
    return das_auto_cast<char *>::cast(((char * const )(ast_describe_typedecl(__decl_rename_at_38_29,__extra_rename_at_38_30,__contracts_rename_at_38_31,__modules_rename_at_38_32,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline char * _FuncastTickdescribeTick16391739697169902387_22f6094444e3262d ( Context * __context__, smart_ptr_raw<Function> const  __expr_rename_at_53_33 )
{
    return das_auto_cast<char *>::cast(((char * const )(ast_describe_function(__expr_rename_at_53_33,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline bool _Funcast_debugTickisRttiPtrTypeTick806345736983820832_48d56f4f80ba3b53 ( Context * __context__, VarInfo const  &  ___vinfo_rename_at_57_34, char * const  __what_rename_at_57_35 )
{
    return das_auto_cast<bool>::cast(_Funcast_debugTickisModulePtrTypeTick12629776342530398480_56257da3a0e7b243(__context__,___vinfo_rename_at_57_34,((char *) "rtti"),__what_rename_at_57_35));
}

inline bool _Funcast_debugTickisRttiTypeTick4516636421360898655_2f19fa27e91344c2 ( Context * __context__, LocalVariableInfo const  &  __vinfo_rename_at_61_36, char * const  __what_rename_at_61_37 )
{
    return das_auto_cast<bool>::cast((__vinfo_rename_at_61_36.type /*basicType*/ != DAS_COMMENT(bound_enum) das::Type::tHandle) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((neq_dstr_str(das_arg<das::string>::pass(((__vinfo_rename_at_61_36).getAnnotation())->module /*_module*/->name /*name*/),((char *) "rtti"))) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((neq_dstr_str(das_arg<das::string>::pass(((__vinfo_rename_at_61_36).getAnnotation())->name /*name*/),__what_rename_at_61_37)) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(true))))));
}

inline bool _Funcast_debugTickisExpressionTypeTick1683607289072304170_967b88ce98476055 ( Context * __context__, LocalVariableInfo const  &  ___vinfo_rename_at_19_38 )
{
    if ( (___vinfo_rename_at_19_38.type /*basicType*/ != DAS_COMMENT(bound_enum) das::Type::tPointer) || (___vinfo_rename_at_19_38.firstType /*firstType*/ == nullptr) )
    {
        return das_auto_cast<bool>::cast(false);
    } else {
        TypeInfo * __vinfo_rename_at_23_39 = ((TypeInfo *)___vinfo_rename_at_19_38.firstType /*firstType*/);
        return das_auto_cast<bool>::cast((__vinfo_rename_at_23_39->type /*basicType*/ != DAS_COMMENT(bound_enum) das::Type::tHandle) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((neq_dstr_str(das_arg<das::string>::pass(((das_deref(__context__,__vinfo_rename_at_23_39)).getAnnotation())->module /*_module*/->name /*name*/),((char *) "ast"))) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast((!(builtin_string_startswith(((char * const )(to_das_string(das_arg<das::string>::pass(((das_deref(__context__,__vinfo_rename_at_23_39)).getAnnotation())->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))),((char *) "Expr"),__context__)) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(true))))));
    };
}

inline bool _Funcast_debugTickisAstPtrTypeTick3276036939226023356_1d28eff7d398c8a3 ( Context * __context__, LocalVariableInfo const  &  ___vinfo_rename_at_53_40, char * const  __what_rename_at_53_41 )
{
    return das_auto_cast<bool>::cast(_Funcast_debugTickisModulePtrTypeTick12629776342530398480_a210fcb9569bff8c(__context__,___vinfo_rename_at_53_40,((char *) "ast"),__what_rename_at_53_41));
}

inline bool _Funcast_debugTickisRttiPtrTypeTick806345736983820832_515c5b2443eb4ef0 ( Context * __context__, LocalVariableInfo const  &  ___vinfo_rename_at_57_42, char * const  __what_rename_at_57_43 )
{
    return das_auto_cast<bool>::cast(_Funcast_debugTickisModulePtrTypeTick12629776342530398480_a210fcb9569bff8c(__context__,___vinfo_rename_at_57_42,((char *) "rtti"),__what_rename_at_57_43));
}

inline void _Funcast_debugTickdescribe_argTick8826490271305289944_efb00f1647d906b2 ( Context * __context__, Context &  __ctxid_rename_at_74_44, VarInfo const  &  __vinfo_rename_at_74_45, void * const  __arg_rename_at_74_46 )
{
    if ( _Funcast_debugTickisRttiTypeTick4516636421360898655_d3b3aacafe01c827(__context__,__vinfo_rename_at_74_45,((char *) "AnnotationList")) )
    {
        _Funcast_debugTickreport_to_debuggerTick2408265038224314806_2643678afa1dfd37(__context__,das_arg<Context>::pass(__ctxid_rename_at_74_44),((char *) "ast_debug"),((char * const )(__vinfo_rename_at_74_45.name /*name*/)),das_invoke_function<char * const >::invoke<AnnotationList const  &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::describe CH<rtti::AnnotationList>*/ 0x32c9eba4a62fbeaf)),das_arg<AnnotationList>::pass(das_deref(__context__,das_cast<AnnotationList *>::cast(__arg_rename_at_74_46)))));
        return ;
    } else if ( _Funcast_debugTickisRttiTypeTick4516636421360898655_d3b3aacafe01c827(__context__,__vinfo_rename_at_74_45,((char *) "AnnotationArgumentList")) )
    {
        _Funcast_debugTickreport_to_debuggerTick2408265038224314806_2643678afa1dfd37(__context__,das_arg<Context>::pass(__ctxid_rename_at_74_44),((char *) "ast_debug"),((char * const )(__vinfo_rename_at_74_45.name /*name*/)),das_invoke_function<char * const >::invoke<AnnotationArgumentList const  &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::describe CH<rtti::AnnotationArgumentList>*/ 0xc00b5d1b29d92686)),das_arg<AnnotationArgumentList>::pass(das_deref(__context__,das_cast<AnnotationArgumentList *>::cast(__arg_rename_at_74_46)))));
        return ;
    };
    void * * __parg_rename_at_83_47 = ((void * *)das_cast<void * *>::cast(__arg_rename_at_74_46));
    if ( (__parg_rename_at_83_47 == nullptr) || (das_deref(__context__,__parg_rename_at_83_47) == nullptr) )
    {
        _Funcast_debugTickreport_to_debuggerTick2408265038224314806_48ecdbe80a06d95e(__context__,das_arg<Context>::pass(__ctxid_rename_at_74_44),((char *) "ast_debug"),((char * const )(__vinfo_rename_at_74_45.name /*name*/)),das_auto_cast<void * const >::cast(nullptr));
        return ;
    } else {
        if ( _Funcast_debugTickisExpressionTypeTick1683607289072304170_59fb59f45b96aa56(__context__,__vinfo_rename_at_74_45) )
        {
            _Funcast_debugTickreport_to_debuggerTick2408265038224314806_2643678afa1dfd37(__context__,das_arg<Context>::pass(__ctxid_rename_at_74_44),((char *) "ast_debug"),((char * const )(__vinfo_rename_at_74_45.name /*name*/)),_FuncastTickdescribeTick842554968825501494_452cfc8955963a18(__context__,das_deref(__context__,das_cast<smart_ptr_raw<Expression> *>::cast(__arg_rename_at_74_46))));
            _Funcast_debugTickreport_to_debuggerTick2408265038224314806_2643678afa1dfd37(__context__,das_arg<Context>::pass(__ctxid_rename_at_74_44),((char *) "ast_debug"),cast<char *>::to(SimPolicy<char *>::Add(cast<char *>::from(((char * const )(__vinfo_rename_at_74_45.name /*name*/))),cast<char *>::from(((char *) ".detailed")),*__context__,nullptr)),das_invoke_function<char * const >::invoke<smart_ptr_raw<Expression> const ,Bitfield>(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::debug_expression CY<ExpressionPtr>1<H<ast::Expression>>?M CY<DebugExpressionFlags>N<refCount>t*/ 0x28d96126c4ea29b4)),das_deref(__context__,das_cast<smart_ptr_raw<Expression> *>::cast(__arg_rename_at_74_46)),0x0u));
        } else if ( _Funcast_debugTickisAstPtrTypeTick3276036939226023356_3b7ab890c635e212(__context__,__vinfo_rename_at_74_45,((char *) "TypeDecl")) )
        {
            _Funcast_debugTickreport_to_debuggerTick2408265038224314806_2643678afa1dfd37(__context__,das_arg<Context>::pass(__ctxid_rename_at_74_44),((char *) "ast_debug"),((char * const )(__vinfo_rename_at_74_45.name /*name*/)),_FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89(__context__,das_deref(__context__,das_cast<smart_ptr_raw<TypeDecl> *>::cast(__arg_rename_at_74_46)),true,true,true));
        } else if ( _Funcast_debugTickisAstPtrTypeTick3276036939226023356_3b7ab890c635e212(__context__,__vinfo_rename_at_74_45,((char *) "Function")) )
        {
            _Funcast_debugTickreport_to_debuggerTick2408265038224314806_2643678afa1dfd37(__context__,das_arg<Context>::pass(__ctxid_rename_at_74_44),((char *) "ast_debug"),((char * const )(__vinfo_rename_at_74_45.name /*name*/)),_FuncastTickdescribeTick16391739697169902387_22f6094444e3262d(__context__,das_deref(__context__,das_cast<smart_ptr_raw<Function> *>::cast(__arg_rename_at_74_46))));
        } else if ( _Funcast_debugTickisAstPtrTypeTick3276036939226023356_3b7ab890c635e212(__context__,__vinfo_rename_at_74_45,((char *) "Variable")) )
        {
            _Funcast_debugTickreport_to_debuggerTick2408265038224314806_2643678afa1dfd37(__context__,das_arg<Context>::pass(__ctxid_rename_at_74_44),((char *) "ast_debug"),((char * const )(__vinfo_rename_at_74_45.name /*name*/)),das_invoke_function<char * const >::invoke<smart_ptr_raw<Variable> const >(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::describe CY<VariablePtr>1<H<ast::Variable>>?M*/ 0x64d824b4153a1526)),das_deref(__context__,das_cast<smart_ptr_raw<Variable> *>::cast(__arg_rename_at_74_46))));
        } else if ( _Funcast_debugTickisRttiPtrTypeTick806345736983820832_48d56f4f80ba3b53(__context__,__vinfo_rename_at_74_45,((char *) "AnnotationDeclaration")) )
        {
            _Funcast_debugTickreport_to_debuggerTick2408265038224314806_2643678afa1dfd37(__context__,das_arg<Context>::pass(__ctxid_rename_at_74_44),((char *) "ast_debug"),((char * const )(__vinfo_rename_at_74_45.name /*name*/)),das_invoke_function<char * const >::invoke<AnnotationDeclaration const  &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::describe CH<rtti::AnnotationDeclaration>*/ 0x3208529d31c498ae)),das_arg<AnnotationDeclaration>::pass(das_deref(__context__,das_deref(__context__,das_cast<smart_ptr_raw<AnnotationDeclaration> *>::cast(__arg_rename_at_74_46))))));
        };
    };
}

inline void _Funcast_debugTickdescribe_argTick8826490271305289944_af045ee043c050cf ( Context * __context__, Context &  __ctxid_rename_at_74_48, LocalVariableInfo const  &  __vinfo_rename_at_74_49, void * const  __arg_rename_at_74_50 )
{
    if ( _Funcast_debugTickisRttiTypeTick4516636421360898655_2f19fa27e91344c2(__context__,__vinfo_rename_at_74_49,((char *) "AnnotationList")) )
    {
        _Funcast_debugTickreport_to_debuggerTick2408265038224314806_2643678afa1dfd37(__context__,das_arg<Context>::pass(__ctxid_rename_at_74_48),((char *) "ast_debug"),((char * const )(__vinfo_rename_at_74_49.name /*name*/)),das_invoke_function<char * const >::invoke<AnnotationList const  &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::describe CH<rtti::AnnotationList>*/ 0x32c9eba4a62fbeaf)),das_arg<AnnotationList>::pass(das_deref(__context__,das_cast<AnnotationList *>::cast(__arg_rename_at_74_50)))));
        return ;
    } else if ( _Funcast_debugTickisRttiTypeTick4516636421360898655_2f19fa27e91344c2(__context__,__vinfo_rename_at_74_49,((char *) "AnnotationArgumentList")) )
    {
        _Funcast_debugTickreport_to_debuggerTick2408265038224314806_2643678afa1dfd37(__context__,das_arg<Context>::pass(__ctxid_rename_at_74_48),((char *) "ast_debug"),((char * const )(__vinfo_rename_at_74_49.name /*name*/)),das_invoke_function<char * const >::invoke<AnnotationArgumentList const  &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::describe CH<rtti::AnnotationArgumentList>*/ 0xc00b5d1b29d92686)),das_arg<AnnotationArgumentList>::pass(das_deref(__context__,das_cast<AnnotationArgumentList *>::cast(__arg_rename_at_74_50)))));
        return ;
    };
    void * * __parg_rename_at_83_51 = ((void * *)das_cast<void * *>::cast(__arg_rename_at_74_50));
    if ( (__parg_rename_at_83_51 == nullptr) || (das_deref(__context__,__parg_rename_at_83_51) == nullptr) )
    {
        _Funcast_debugTickreport_to_debuggerTick2408265038224314806_48ecdbe80a06d95e(__context__,das_arg<Context>::pass(__ctxid_rename_at_74_48),((char *) "ast_debug"),((char * const )(__vinfo_rename_at_74_49.name /*name*/)),das_auto_cast<void * const >::cast(nullptr));
        return ;
    } else {
        if ( _Funcast_debugTickisExpressionTypeTick1683607289072304170_967b88ce98476055(__context__,__vinfo_rename_at_74_49) )
        {
            _Funcast_debugTickreport_to_debuggerTick2408265038224314806_2643678afa1dfd37(__context__,das_arg<Context>::pass(__ctxid_rename_at_74_48),((char *) "ast_debug"),((char * const )(__vinfo_rename_at_74_49.name /*name*/)),_FuncastTickdescribeTick842554968825501494_452cfc8955963a18(__context__,das_deref(__context__,das_cast<smart_ptr_raw<Expression> *>::cast(__arg_rename_at_74_50))));
            _Funcast_debugTickreport_to_debuggerTick2408265038224314806_2643678afa1dfd37(__context__,das_arg<Context>::pass(__ctxid_rename_at_74_48),((char *) "ast_debug"),cast<char *>::to(SimPolicy<char *>::Add(cast<char *>::from(((char * const )(__vinfo_rename_at_74_49.name /*name*/))),cast<char *>::from(((char *) ".detailed")),*__context__,nullptr)),das_invoke_function<char * const >::invoke<smart_ptr_raw<Expression> const ,Bitfield>(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::debug_expression CY<ExpressionPtr>1<H<ast::Expression>>?M CY<DebugExpressionFlags>N<refCount>t*/ 0x28d96126c4ea29b4)),das_deref(__context__,das_cast<smart_ptr_raw<Expression> *>::cast(__arg_rename_at_74_50)),0x0u));
        } else if ( _Funcast_debugTickisAstPtrTypeTick3276036939226023356_1d28eff7d398c8a3(__context__,__vinfo_rename_at_74_49,((char *) "TypeDecl")) )
        {
            _Funcast_debugTickreport_to_debuggerTick2408265038224314806_2643678afa1dfd37(__context__,das_arg<Context>::pass(__ctxid_rename_at_74_48),((char *) "ast_debug"),((char * const )(__vinfo_rename_at_74_49.name /*name*/)),_FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89(__context__,das_deref(__context__,das_cast<smart_ptr_raw<TypeDecl> *>::cast(__arg_rename_at_74_50)),true,true,true));
        } else if ( _Funcast_debugTickisAstPtrTypeTick3276036939226023356_1d28eff7d398c8a3(__context__,__vinfo_rename_at_74_49,((char *) "Function")) )
        {
            _Funcast_debugTickreport_to_debuggerTick2408265038224314806_2643678afa1dfd37(__context__,das_arg<Context>::pass(__ctxid_rename_at_74_48),((char *) "ast_debug"),((char * const )(__vinfo_rename_at_74_49.name /*name*/)),_FuncastTickdescribeTick16391739697169902387_22f6094444e3262d(__context__,das_deref(__context__,das_cast<smart_ptr_raw<Function> *>::cast(__arg_rename_at_74_50))));
        } else if ( _Funcast_debugTickisAstPtrTypeTick3276036939226023356_1d28eff7d398c8a3(__context__,__vinfo_rename_at_74_49,((char *) "Variable")) )
        {
            _Funcast_debugTickreport_to_debuggerTick2408265038224314806_2643678afa1dfd37(__context__,das_arg<Context>::pass(__ctxid_rename_at_74_48),((char *) "ast_debug"),((char * const )(__vinfo_rename_at_74_49.name /*name*/)),das_invoke_function<char * const >::invoke<smart_ptr_raw<Variable> const >(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::describe CY<VariablePtr>1<H<ast::Variable>>?M*/ 0x64d824b4153a1526)),das_deref(__context__,das_cast<smart_ptr_raw<Variable> *>::cast(__arg_rename_at_74_50))));
        } else if ( _Funcast_debugTickisRttiPtrTypeTick806345736983820832_515c5b2443eb4ef0(__context__,__vinfo_rename_at_74_49,((char *) "AnnotationDeclaration")) )
        {
            _Funcast_debugTickreport_to_debuggerTick2408265038224314806_2643678afa1dfd37(__context__,das_arg<Context>::pass(__ctxid_rename_at_74_48),((char *) "ast_debug"),((char * const )(__vinfo_rename_at_74_49.name /*name*/)),das_invoke_function<char * const >::invoke<AnnotationDeclaration const  &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::describe CH<rtti::AnnotationDeclaration>*/ 0x3208529d31c498ae)),das_arg<AnnotationDeclaration>::pass(das_deref(__context__,das_deref(__context__,das_cast<smart_ptr_raw<AnnotationDeclaration> *>::cast(__arg_rename_at_74_50))))));
        };
    };
}

inline smart_ptr_raw<StackWalker> _FuncdebugapiTickmake_stack_walkerTick10101906578839871846_229ea554e31d57ca ( Context * __context__, ast_debug::SampleStackWalker * const  __classPtr_rename_at_216_52 )
{
    StructInfo const  * __classInfo_rename_at_218_53 = ((StructInfo const  *)_FuncrttiTickclass_infoTick15801393167907430156_c81a84221fb6bad2(__context__,das_deref(__context__,__classPtr_rename_at_216_52)));
    return /* <- */ das_auto_cast_move<smart_ptr_raw<StackWalker>>::cast(makeStackWalker(das_auto_cast<void * const >::cast(__classPtr_rename_at_216_52),__classInfo_rename_at_218_53,__context__));
}

inline void finalize_500fce7609128ffb ( Context * __context__, ast_debug::SampleStackWalker * & ____this_rename_at_143_54 )
{
    if ( ____this_rename_at_143_54 != nullptr )
    {
        int32_t ____size_rename_at_143_55 = ((int32_t)class_rtti_size(das_auto_cast<void * const >::cast(____this_rename_at_143_54)));
        das_invoke_function<void>::invoke<debugapi::DapiStackWalker &>(__context__,nullptr,____this_rename_at_143_54->__finalize,das_arg<debugapi::DapiStackWalker>::pass(das_cast<debugapi::DapiStackWalker>::cast(das_deref(__context__,____this_rename_at_143_54))));
        das_delete<ast_debug::SampleStackWalker *>::clear(__context__,____this_rename_at_143_54,____size_rename_at_143_55);
        das_copy(____this_rename_at_143_54,nullptr);
    };
}

inline void finalize_5733f4cfa043e920 ( Context * __context__, ast_debug::SampleStackWalker &  ____this_rename_at_103_56 )
{
    memset((void*)&(____this_rename_at_103_56), 0, TypeSize<ast_debug::SampleStackWalker>::size);
}

inline void _FuncdebugapiTickinstall_new_debug_agentTick3897311473844341055_3e1c20224cd19f2e ( Context * __context__, ast_debug::ContextStateAgent * __agentPtr_rename_at_37_57, char * const  __category_rename_at_37_58 )
{
    StructInfo const  * __agentInfo_rename_at_39_59; memset((void*)&__agentInfo_rename_at_39_59,0,sizeof(__agentInfo_rename_at_39_59));
    smart_ptr_raw<DebugAgent> __agentAdapter_rename_at_40_60; memset((void*)&__agentAdapter_rename_at_40_60,0,sizeof(__agentAdapter_rename_at_40_60));
    {
        /* finally */ auto __finally_38= das_finally([&](){
        das_delete_handle<smart_ptr_raw<DebugAgent>>::clear(__context__,__agentAdapter_rename_at_40_60);
        /* end finally */ });
        __agentInfo_rename_at_39_59 = ((StructInfo const  *)_FuncrttiTickclass_infoTick15801393167907430156_7c5d3c9bad07d43c(__context__,das_arg<ast_debug::ContextStateAgent>::pass(das_deref(__context__,__agentPtr_rename_at_37_57))));
        __agentAdapter_rename_at_40_60; das_zero(__agentAdapter_rename_at_40_60); das_move(__agentAdapter_rename_at_40_60, makeDebugAgent(das_auto_cast<void * const >::cast(__agentPtr_rename_at_37_57),__agentInfo_rename_at_39_59,__context__));
        das_copy(__agentPtr_rename_at_37_57->thisAgent,das_cast<DebugAgent *>::cast(__agentAdapter_rename_at_40_60));
        installDebugAgent(__agentAdapter_rename_at_40_60,__category_rename_at_37_58,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)),__context__);
        set_das_string(das_arg<das::string>::pass(thisContext(__context__).name /*name*/),das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_0, cast<char * const >::from(((char *) "debug agent ")), cast<char * const >::from(__category_rename_at_37_58))));
    };
}

inline void _FuncSampleStackWalkerTickonArgument_b3c1b48f676b9bbd ( Context * __context__, ast_debug::SampleStackWalker &  __self_rename_at_105_61, FuncInfo const  &  __info_rename_at_105_62, int32_t __index_rename_at_105_63, VarInfo const  &  __vinfo_rename_at_105_64, float4 __arg_rename_at_105_65 )
{
    if ( das_get_bitfield(__vinfo_rename_at_105_64.flags /*flags*/,1u << 0) || das_get_bitfield(__vinfo_rename_at_105_64.flags /*flags*/,1u << 1) )
    {
        _Funcast_debugTickdescribe_argTick8826490271305289944_efb00f1647d906b2(__context__,das_arg<Context>::pass(das_deref(__context__,__self_rename_at_105_61.ctxid)),__vinfo_rename_at_105_64,das_deref(__context__,das_cast<void * *>::cast(das_ref(__context__,__arg_rename_at_105_65))));
    } else {
        _Funcast_debugTickdescribe_argTick8826490271305289944_efb00f1647d906b2(__context__,das_arg<Context>::pass(das_deref(__context__,__self_rename_at_105_61.ctxid)),__vinfo_rename_at_105_64,das_auto_cast<void * const >::cast(das_ref(__context__,__arg_rename_at_105_65)));
    };
}

inline void _FuncSampleStackWalkerTickonVariable_b047f0cc7c7030d6 ( Context * __context__, ast_debug::SampleStackWalker &  __self_rename_at_114_66, FuncInfo const  &  __inf_rename_at_114_67, LocalVariableInfo const  &  __vinfo_rename_at_114_68, void * const  __arg_rename_at_114_69, bool __inScope_rename_at_114_70 )
{
    if ( !__inScope_rename_at_114_70 )
    {
        return ;
    } else {
        if ( das_get_bitfield(__vinfo_rename_at_114_68.flags /*flags*/,1u << 0) || das_get_bitfield(__vinfo_rename_at_114_68.flags /*flags*/,1u << 1) )
        {
            _Funcast_debugTickdescribe_argTick8826490271305289944_af045ee043c050cf(__context__,das_arg<Context>::pass(das_deref(__context__,__self_rename_at_114_66.ctxid)),__vinfo_rename_at_114_68,das_deref(__context__,das_cast<void * *>::cast(__arg_rename_at_114_69)));
        } else {
            _Funcast_debugTickdescribe_argTick8826490271305289944_af045ee043c050cf(__context__,das_arg<Context>::pass(das_deref(__context__,__self_rename_at_114_66.ctxid)),__vinfo_rename_at_114_68,__arg_rename_at_114_69);
        };
    };
}

inline bool _FuncSampleStackWalkerTickonAfterCall_eedb864288cb4a8a ( Context * __context__, ast_debug::SampleStackWalker &  __self_rename_at_126_71, Prologue const  &  __pp_rename_at_126_72 )
{
    return das_auto_cast<bool>::cast(false);
}

inline void _FuncSampleStackWalker_0x27___finalize_af21cd893b46a727 ( Context * __context__, ast_debug::SampleStackWalker &  __self_rename_at_103_73 )
{
    finalize_5733f4cfa043e920(__context__,das_arg<ast_debug::SampleStackWalker>::pass(__self_rename_at_103_73));
}

inline ast_debug::ContextStateAgent ContextStateAgent_e10fdff26046cd6f ( Context * __context__ )
{
    ast_debug::ContextStateAgent __self_rename_at_134_74; das_zero(__self_rename_at_134_74); das_move(__self_rename_at_134_74, (([&]() -> ast_debug::ContextStateAgent {
        ast_debug::ContextStateAgent __mks_134;
        das_zero(__mks_134);
        das_copy((__mks_134.__rtti),(((void *)(&__type_info__2ab2203ab786034d))));
        das_copy((__mks_134.__finalize),(das_cast<Func DAS_COMMENT((void,debugapi::DapiDebugAgent))>::cast(Func(__context__->fnByMangledName(/*@ast_debug::ContextStateAgent'__finalize S<ast_debug::ContextStateAgent>*/ 0xff67a1e20e88fbfe)))));
        das_copy((__mks_134.onCollect),(das_cast<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,LineInfo const ))>::cast(Func(__context__->fnByMangledName(/*@ast_debug::ContextStateAgent`onCollect S<ast_debug::ContextStateAgent> H<rtti::Context> CH<rtti::LineInfo>*/ 0x44e865880fb63b63)))));
        return __mks_134;
    })()));
    _FuncContextStateAgentTickContextStateAgent_ce0829614ebac3e5(__context__,das_arg<ast_debug::ContextStateAgent>::pass(__self_rename_at_134_74));
    return /* <- */ das_auto_cast_move<ast_debug::ContextStateAgent>::cast(__self_rename_at_134_74);
}

inline void _FuncContextStateAgentTickContextStateAgent_ce0829614ebac3e5 ( Context * __context__, ast_debug::ContextStateAgent &  __self_rename_at_134_75 )
{
    das_copy(__self_rename_at_134_75.walker,das_new<ast_debug::SampleStackWalker>::make_and_init(__context__,[&]() { return SampleStackWalker_b9ed6b4c0fa80af2(__context__); }));
    das_move(__self_rename_at_134_75.walker_adapter,_FuncdebugapiTickmake_stack_walkerTick10101906578839871846_229ea554e31d57ca(__context__,__self_rename_at_134_75.walker));
}

inline void finalize_dcfff9e2129da1aa ( Context * __context__, ast_debug::ContextStateAgent &  __self_rename_at_140_76 )
{
    das_delete_handle<smart_ptr_raw<StackWalker>>::clear(__context__,__self_rename_at_140_76.walker_adapter);
    finalize_500fce7609128ffb(__context__,__self_rename_at_140_76.walker);
}

inline void _FuncContextStateAgentTickonCollect_d5b055adc3fcc695 ( Context * __context__, ast_debug::ContextStateAgent &  __self_rename_at_146_77, Context &  __ctx_rename_at_146_78, LineInfo const  &  __at_rename_at_146_79 )
{
    das_copy(__self_rename_at_146_77.walker->ctxid,das_ref(__context__,__ctx_rename_at_146_78));
    dapiStackWalk(__self_rename_at_146_77.walker_adapter,das_arg<Context>::pass(__ctx_rename_at_146_78),__at_rename_at_146_79);
}

inline void _FuncContextStateAgent_0x27___finalize_c2bf11f8c2d8aef ( Context * __context__, ast_debug::ContextStateAgent &  __self_rename_at_131_80 )
{
    finalize_dcfff9e2129da1aa(__context__,das_arg<ast_debug::ContextStateAgent>::pass(__self_rename_at_131_80));
}

inline void state_agent_a7d0d7fe180b2c1a ( Context * __context__, Context const  &  __ctx_rename_at_153_81 )
{
    _FuncdebugapiTickinstall_new_debug_agentTick3897311473844341055_3e1c20224cd19f2e(__context__,das_new<ast_debug::ContextStateAgent>::make_and_init(__context__,[&]() { return ContextStateAgent_e10fdff26046cd6f(__context__); }),((char *) "ast_debug"));
}

inline ast_debug::SampleStackWalker SampleStackWalker_b9ed6b4c0fa80af2 ( Context * __context__ )
{
    return /* <- */ das_auto_cast_move<ast_debug::SampleStackWalker>::cast((([&]() -> ast_debug::SampleStackWalker {
        ast_debug::SampleStackWalker __mks_103;
        das_zero(__mks_103);
        das_copy((__mks_103.__rtti),(((void *)(&__type_info__80bd755d94e49a69))));
        das_copy((__mks_103.__finalize),(das_cast<Func DAS_COMMENT((void,debugapi::DapiStackWalker))>::cast(Func(__context__->fnByMangledName(/*@ast_debug::SampleStackWalker'__finalize S<ast_debug::SampleStackWalker>*/ 0xaf91bcf2512cb268)))));
        das_copy((__mks_103.onArgument),(das_cast<Func DAS_COMMENT((void,debugapi::DapiStackWalker,FuncInfo const ,int32_t,VarInfo const ,float4))>::cast(Func(__context__->fnByMangledName(/*@ast_debug::SampleStackWalker`onArgument S<ast_debug::SampleStackWalker> CH<rtti::FuncInfo> Ci CH<rtti::VarInfo> Cf4*/ 0xcdbed7d98b8f0a44)))));
        das_copy((__mks_103.onVariable),(das_cast<Func DAS_COMMENT((void,debugapi::DapiStackWalker,FuncInfo const ,LocalVariableInfo const ,void * const ,bool))>::cast(Func(__context__->fnByMangledName(/*@ast_debug::SampleStackWalker`onVariable S<ast_debug::SampleStackWalker> CH<rtti::FuncInfo> CH<rtti::LocalVariableInfo> C1<v>? Cb*/ 0xed778efea49905d3)))));
        das_copy((__mks_103.onAfterCall),(das_cast<Func DAS_COMMENT((bool,debugapi::DapiStackWalker,Prologue const ))>::cast(Func(__context__->fnByMangledName(/*@ast_debug::SampleStackWalker`onAfterCall S<ast_debug::SampleStackWalker> CH<debugapi::Prologue>*/ 0x6ace7af035c2f944)))));
        return __mks_103;
    })()));
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0xd944b240f1dadce5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_c81a84221fb6bad2>>();
    };
    aotLib[0xcf481776efa54cc0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_debugTickisModulePtrTypeTick12629776342530398480_56257da3a0e7b243>>();
    };
    aotLib[0xec046597df2ce265] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_debugTickisModulePtrTypeTick12629776342530398480_a210fcb9569bff8c>>();
    };
    aotLib[0x1abaf737439ed2ef] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_7c5d3c9bad07d43c>>();
    };
    aotLib[0x3961f53f76a6dc70] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_debugTickisRttiTypeTick4516636421360898655_d3b3aacafe01c827>>();
    };
    aotLib[0xd6d7cce01249a4bc] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_debugTickreport_to_debuggerTick2408265038224314806_2643678afa1dfd37>>();
    };
    aotLib[0x5c4b96d7c8bcb6e5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_debugTickreport_to_debuggerTick2408265038224314806_48ecdbe80a06d95e>>();
    };
    aotLib[0xf5044d10f8587bb5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_debugTickisExpressionTypeTick1683607289072304170_59fb59f45b96aa56>>();
    };
    aotLib[0xfbfe800d5700e08f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickdescribeTick842554968825501494_452cfc8955963a18>>();
    };
    aotLib[0xf1f22832564e2f65] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_debugTickisAstPtrTypeTick3276036939226023356_3b7ab890c635e212>>();
    };
    aotLib[0xb9c50cbd3f619a9d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89>>();
    };
    aotLib[0x6559b2976faba9bc] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickdescribeTick16391739697169902387_22f6094444e3262d>>();
    };
    aotLib[0x712caa686b4e9cf1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_debugTickisRttiPtrTypeTick806345736983820832_48d56f4f80ba3b53>>();
    };
    aotLib[0x40f0c191a75f02eb] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_debugTickisRttiTypeTick4516636421360898655_2f19fa27e91344c2>>();
    };
    aotLib[0xc12611ddde3da5af] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_debugTickisExpressionTypeTick1683607289072304170_967b88ce98476055>>();
    };
    aotLib[0x44b91979f7cd3be2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_debugTickisAstPtrTypeTick3276036939226023356_1d28eff7d398c8a3>>();
    };
    aotLib[0x89914d144ab3775e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_debugTickisRttiPtrTypeTick806345736983820832_515c5b2443eb4ef0>>();
    };
    aotLib[0x9585d59a27adf27b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_debugTickdescribe_argTick8826490271305289944_efb00f1647d906b2>>();
    };
    aotLib[0x1bc38274120e1354] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_debugTickdescribe_argTick8826490271305289944_af045ee043c050cf>>();
    };
    aotLib[0x86519d383a236a85] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncdebugapiTickmake_stack_walkerTick10101906578839871846_229ea554e31d57ca>>();
    };
    aotLib[0x9e24c1a5d7a1647e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_500fce7609128ffb>>();
    };
    aotLib[0x16be1c7d7571ba3d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_5733f4cfa043e920>>();
    };
    aotLib[0xc8fd86923428452f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncdebugapiTickinstall_new_debug_agentTick3897311473844341055_3e1c20224cd19f2e>>();
    };
    aotLib[0x955a91178f2a8500] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncSampleStackWalkerTickonArgument_b3c1b48f676b9bbd>>();
    };
    aotLib[0x1d8e0b6a1c5583c6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncSampleStackWalkerTickonVariable_b047f0cc7c7030d6>>();
    };
    aotLib[0xbe942b4a73c4cd55] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncSampleStackWalkerTickonAfterCall_eedb864288cb4a8a>>();
    };
    aotLib[0x6c77ecfde1d92d7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncSampleStackWalker_0x27___finalize_af21cd893b46a727>>();
    };
    aotLib[0x3fbf295fb400101b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&ContextStateAgent_e10fdff26046cd6f>>();
    };
    aotLib[0x954f3558ded65d8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncContextStateAgentTickContextStateAgent_ce0829614ebac3e5>>();
    };
    aotLib[0x2af306b9895d5228] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_dcfff9e2129da1aa>>();
    };
    aotLib[0x30e88d3b4dd372e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncContextStateAgentTickonCollect_d5b055adc3fcc695>>();
    };
    aotLib[0x2ae71cff664419fd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncContextStateAgent_0x27___finalize_c2bf11f8c2d8aef>>();
    };
    aotLib[0xe4e00ea64d552d51] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&state_agent_a7d0d7fe180b2c1a>>();
    };
    aotLib[0xbddc52318f02e163] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&SampleStackWalker_b9ed6b4c0fa80af2>>();
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_1280768645085455701
AotListBase impl_aot_ast_debug(_anon_1280768645085455701::registerAotFunctions);
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
