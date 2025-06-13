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
 // require fio
#include "daScript/misc/performance_time.h"
#include "daScript/simulate/aot_builtin_fio.h"
 // require uriparser
#include "daScript/simulate/aot_builtin_uriparser.h"
 // require ast
#include "daScript/ast/ast.h"
#include "daScript/simulate/aot_builtin_ast.h"
#include "daScript/ast/ast_generate.h"
 // require profiler

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
namespace _anon_14082698819804048119 {

namespace profiler { struct PerfNode; };
namespace profiler { struct PerfEvent; };
namespace profiler { struct PerfContext; };
namespace profiler { struct ProfilerDebugAgent; };
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
// unused enumeration CompilationError
// unused enumeration ConstMatters
// unused enumeration RefMatters
// unused enumeration TemporaryMatters
// unused enumeration Type
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
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,void * const ,uint64_t,bool,LineInfo const )) onAllocateString;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,void * const ,bool,LineInfo const )) onFreeString;
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
// unused structure DapiStackWalker
// unused enumeration ConversionResult
// unused structure df_header
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
namespace profiler {

struct PerfNode {
    SimFunction * fun;
    uint64_t count;
    int64_t total_time;
    int64_t enter_time;
    uint64_t total_heap_bytes;
    uint64_t enter_heap_bytes;
    uint64_t total_string_heap_bytes;
    uint64_t enter_string_heap_bytes;
    TTable<SimFunction *,profiler::PerfNode *> children;
};
}
namespace profiler {

struct PerfEvent {
    SimFunction * fun;
    int64_t ts;
    bool entering;
    uint64_t heapBytes;
    uint64_t stringHeapBytes;
};
}
namespace profiler {

struct PerfContext {
    TArray<profiler::PerfEvent> events;
    bool enabled;
};
}
namespace profiler {

struct ProfilerDebugAgent {
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
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,void * const ,uint64_t,bool,LineInfo const )) onAllocateString;
    Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,void * const ,bool,LineInfo const )) onFreeString;
    DebugAgent * thisAgent;
    int64_t us0;
    TTable<uint64_t,profiler::PerfContext> events;
    FILE const  * out;
    bool firstRecord;
    bool manual;
    bool report_memory;
    TTable<uint64_t,void> instrumented;
    TTable<char *,int64_t> map_time_units;
    char * time_unit;
    Func DAS_COMMENT((Func DAS_COMMENT((void,uint64_t,bool)),profiler::ProfilerDebugAgent)) dependency;
    Func DAS_COMMENT((bool,profiler::ProfilerDebugAgent,char * const )) is_time_unit_correct;
    Func DAS_COMMENT((int64_t,profiler::ProfilerDebugAgent,int64_t,char * const )) convert_ns_to_unit;
    Func DAS_COMMENT((bool,profiler::ProfilerDebugAgent,Context)) isProfileable;
    Func DAS_COMMENT((void,profiler::ProfilerDebugAgent,uint64_t,bool)) enable_profiler;
    Func DAS_COMMENT((void,profiler::ProfilerDebugAgent,Context,SimFunction *,bool,uint64_t)) onInstrumentFunctionWithMemory;
    Func DAS_COMMENT((void,profiler::ProfilerDebugAgent,char * const )) dump;
    Func DAS_COMMENT((void,profiler::ProfilerDebugAgent,char * const )) write;
    Func DAS_COMMENT((void,profiler::ProfilerDebugAgent,profiler::PerfEvent const ,uint64_t)) dump_event;
    Func DAS_COMMENT((void,profiler::ProfilerDebugAgent,profiler::PerfNode * const ,int32_t)) dump_node;
    Func DAS_COMMENT((void,profiler::ProfilerDebugAgent,uint64_t)) dump_context_stack;
    Func DAS_COMMENT((void,profiler::ProfilerDebugAgent,Context,uint64_t)) dump_meta;
    Func DAS_COMMENT((void,profiler::ProfilerDebugAgent,uint64_t)) dump_events;
    Func DAS_COMMENT((void,profiler::ProfilerDebugAgent,Context,uint64_t)) dump_context;
};
}
extern StructInfo __struct_info__d91c21afef62b32c;
extern StructInfo __struct_info__540344a4a2e38e82;
extern StructInfo __struct_info__217389ff4ed5b430;
extern StructInfo __struct_info__5a07f965532a3ee4;
extern StructInfo __struct_info__574b31acc906991f;
extern TypeInfo __type_info__6b1c7db4b71a781f;
extern TypeInfo __type_info__831390c0482e333b;
extern TypeInfo __type_info__9ef92bcca2cb528b;
extern TypeInfo __type_info__921d3d0750982f13;
extern TypeInfo __type_info__a9e212d4f301a82c;
extern TypeInfo __type_info__209f9e4a9562d5c2;
extern TypeInfo __type_info__2d750e15c3790305;
extern TypeInfo __type_info__ed65100a3b73031a;
extern TypeInfo __type_info__bf9a0c4f15947355;
extern TypeInfo __type_info__9c225ec61b3e6a3c;
extern TypeInfo __type_info__16d0aa3dd6b69257;
extern TypeInfo __type_info__246dda13a8a4b104;
extern TypeInfo __type_info__bc67beb4aa160fd4;
extern TypeInfo __type_info__d1a453a44e333f31;
extern TypeInfo __type_info__af81fe4c86352052;
extern TypeInfo __type_info__af8afe4c86446b52;
extern TypeInfo __type_info__8d8d8008262e16ec;
extern TypeInfo __type_info__af90fe4c864e9d52;
extern TypeInfo __type_info__b68d800849332aec;
extern TypeInfo __type_info__44ca287faf79178;
extern TypeInfo __type_info__767637ee1a337419;
extern TypeInfo __type_info__5ac778689ccf4816;
extern TypeInfo __type_info__d6eadf66eea309ce;
extern TypeInfo __type_info__4e03c6707d012d5b;
extern TypeInfo __type_info__3a369d6747c19883;
extern TypeInfo __type_info__a066e05f8b62d82b;
extern TypeInfo __type_info__786ea93274f6826d;
extern TypeInfo __type_info__216e8f5cf53cc8a6;
extern TypeInfo __type_info__326921ccb9993fde;
extern TypeInfo __type_info__af63df4c8601f1a5;
extern TypeInfo __type_info__d922fe078cefab30;
extern TypeInfo __type_info__af63ee4c86020b22;
extern TypeInfo __type_info__d9307e078cfb0f0c;
extern TypeInfo __type_info__af63eb4c86020609;
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
extern VarInfo __var_info__c520db4aaaaf1653;
extern VarInfo __var_info__f43b147ac1a7fc79;
extern VarInfo __var_info__bd98cfe33a401a3a;
extern VarInfo __var_info__2518118bebc91673;
extern VarInfo __var_info__3e5bc1e454ff87df;
extern VarInfo __var_info__960d426a54660a92;
extern VarInfo __var_info__12c6b1f833ad265;
extern VarInfo __var_info__dd5d207611eb42b;
extern VarInfo __var_info__9a776d9448d67666;
extern VarInfo __var_info__6efbf0c62a11b261;
extern VarInfo __var_info__4470c1f2e173529;
extern VarInfo __var_info__cd25bd76dbc4b9e1;
extern VarInfo __var_info__af2cc453608f439a;
extern VarInfo __var_info__bbd181d24ec96740;
extern VarInfo __var_info__5a4468c8ea4b9f0e;
extern VarInfo __var_info__615fefeb6580392;
extern VarInfo __var_info__37a12b8313bf8dcd;
extern VarInfo __var_info__d0b84c3839aa73b6;
extern VarInfo __var_info__bda9c2ded41fc3c;
extern VarInfo __var_info__3e2343ef50b19acb;
extern VarInfo __var_info__cb7283133fe45f6d;
extern VarInfo __var_info__78f81aa6c5a74e4d;
extern VarInfo __var_info__bab2883f721d865e;
extern VarInfo __var_info__3110e727e22189c9;
extern VarInfo __var_info__3123e727e241d2c9;
extern VarInfo __var_info__ceb8028124c3af1c;
extern VarInfo __var_info__f0c4248141b60967;
extern VarInfo __var_info__70020a28b4ca2c0;
extern VarInfo __var_info__6b0be594474def95;
extern VarInfo __var_info__d5bb05923125aff;
extern VarInfo __var_info__553a3df51114fd10;
extern VarInfo __var_info__7a1acedae32682d6;
extern VarInfo __var_info__3953895e68f7fcc5;
extern VarInfo __var_info__f724df2c564e5d7a;
extern VarInfo __var_info__6d53a32104c74859;
extern VarInfo __var_info__53acf42a9c806f3c;
extern VarInfo __var_info__f684a95a6ba6bf55;
extern VarInfo __var_info__735aba0d8f7e4eab;
extern VarInfo __var_info__84019f70d7412afe;
extern VarInfo __var_info__c558da89a705eba4;
extern VarInfo __var_info__aa05b11c36ac3c35;
extern VarInfo __var_info__feceeddaf4049fbe;
extern VarInfo __var_info__b530a2a341319c0b;
extern VarInfo __var_info__c40fa3d28ebbfc50;
extern VarInfo __var_info__f3a6ea626c8cbed7;
extern VarInfo __var_info__fb747bebfe3f3e55;
extern VarInfo __var_info__6076ae3d2f9a46c;
extern VarInfo __var_info__817eb2964f5a2216;
extern VarInfo __var_info__16636b23c74d8eb1;
extern VarInfo __var_info__75ae552aca9561cb;
extern VarInfo __var_info__4bd6643d5162e7df;
extern VarInfo __var_info__ac4299ef8241130e;
extern VarInfo __var_info__1d3f6245146bd6c7;
extern VarInfo __var_info__2aec03a5856eaa71;
extern VarInfo __var_info__db6d000acf6b1696;
extern VarInfo __var_info__fe1aa7a83218828e;
extern VarInfo __var_info__a1cd057ea1e464a7;
extern VarInfo __var_info__16642f0ac9ace2e1;
extern VarInfo __var_info__55bd2f29eced282f;
extern VarInfo __var_info__cfa971b639d18578;
extern VarInfo __var_info__49d531818b0ffac4;
extern VarInfo __var_info__4293b9d029c3fd4c;
extern VarInfo __var_info__fd852d29a21826c9;
extern VarInfo __var_info__c8ec6cc996ce3186;

VarInfo __struct_info__d91c21afef62b32c_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, TypeSize<void *>::size, UINT64_C(0xda073e6d7ab129fb), "__rtti", offsetof(debugapi::DapiDebugAgent,__rtti), 24 };
TypeInfo * __type_info__f8c54e03a28c41c_arg_types_var_15644416245097476908[1] = { &__type_info__4e03c6707d012d5b };
const char * __type_info__f8c54e03a28c41c_arg_names_var_15644416245097476908[1] = { "self" };
VarInfo __struct_info__d91c21afef62b32c_field_1 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__f8c54e03a28c41c_arg_types_var_15644416245097476908, __type_info__f8c54e03a28c41c_arg_names_var_15644416245097476908, 1, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent))>::size, UINT64_C(0xf8c54e03a28c41c), "__finalize", offsetof(debugapi::DapiDebugAgent,__finalize), 0 };
TypeInfo * __type_info__bb113b3aa23055d3_arg_types_var_15644416245097476908[2] = { &__type_info__4e03c6707d012d5b, &__type_info__209f9e4a9562d5c2 };
const char * __type_info__bb113b3aa23055d3_arg_names_var_15644416245097476908[2] = { "self", "agent" };
VarInfo __struct_info__d91c21afef62b32c_field_2 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__bb113b3aa23055d3_arg_types_var_15644416245097476908, __type_info__bb113b3aa23055d3_arg_names_var_15644416245097476908, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,DebugAgent * const ))>::size, UINT64_C(0xbb113b3aa23055d3), "onInstall", offsetof(debugapi::DapiDebugAgent,onInstall), 0 };
TypeInfo * __type_info__b6de3e351cfffee7_arg_types_var_15644416245097476908[2] = { &__type_info__4e03c6707d012d5b, &__type_info__209f9e4a9562d5c2 };
const char * __type_info__b6de3e351cfffee7_arg_names_var_15644416245097476908[2] = { "self", "agent" };
VarInfo __struct_info__d91c21afef62b32c_field_3 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__b6de3e351cfffee7_arg_types_var_15644416245097476908, __type_info__b6de3e351cfffee7_arg_names_var_15644416245097476908, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,DebugAgent * const ))>::size, UINT64_C(0xb6de3e351cfffee7), "onUninstall", offsetof(debugapi::DapiDebugAgent,onUninstall), 0 };
TypeInfo * __type_info__99a7ecf15c5ddba_arg_types_var_15644416245097476908[2] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419 };
const char * __type_info__99a7ecf15c5ddba_arg_names_var_15644416245097476908[2] = { "self", "ctx" };
VarInfo __struct_info__d91c21afef62b32c_field_4 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__99a7ecf15c5ddba_arg_types_var_15644416245097476908, __type_info__99a7ecf15c5ddba_arg_names_var_15644416245097476908, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context))>::size, UINT64_C(0x99a7ecf15c5ddba), "onCreateContext", offsetof(debugapi::DapiDebugAgent,onCreateContext), 0 };
TypeInfo * __type_info__8b4c2a0827deb533_arg_types_var_15644416245097476908[2] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419 };
const char * __type_info__8b4c2a0827deb533_arg_names_var_15644416245097476908[2] = { "self", "ctx" };
VarInfo __struct_info__d91c21afef62b32c_field_5 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__8b4c2a0827deb533_arg_types_var_15644416245097476908, __type_info__8b4c2a0827deb533_arg_names_var_15644416245097476908, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context))>::size, UINT64_C(0x8b4c2a0827deb533), "onDestroyContext", offsetof(debugapi::DapiDebugAgent,onDestroyContext), 0 };
TypeInfo * __type_info__33eddcacfcf4eae8_arg_types_var_15644416245097476908[2] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419 };
const char * __type_info__33eddcacfcf4eae8_arg_names_var_15644416245097476908[2] = { "self", "ctx" };
VarInfo __struct_info__d91c21afef62b32c_field_6 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__33eddcacfcf4eae8_arg_types_var_15644416245097476908, __type_info__33eddcacfcf4eae8_arg_names_var_15644416245097476908, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context))>::size, UINT64_C(0x33eddcacfcf4eae8), "onSimulateContext", offsetof(debugapi::DapiDebugAgent,onSimulateContext), 0 };
TypeInfo * __type_info__d5dfe6dacc8ce102_arg_types_var_15644416245097476908[3] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__246dda13a8a4b104 };
const char * __type_info__d5dfe6dacc8ce102_arg_names_var_15644416245097476908[3] = { "self", "ctx", "at" };
VarInfo __struct_info__d91c21afef62b32c_field_7 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__d5dfe6dacc8ce102_arg_types_var_15644416245097476908, __type_info__d5dfe6dacc8ce102_arg_names_var_15644416245097476908, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,LineInfo const ))>::size, UINT64_C(0xd5dfe6dacc8ce102), "onSingleStep", offsetof(debugapi::DapiDebugAgent,onSingleStep), 0 };
TypeInfo * __type_info__327f7c9a660b1ab9_arg_types_var_15644416245097476908[3] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__246dda13a8a4b104 };
const char * __type_info__327f7c9a660b1ab9_arg_names_var_15644416245097476908[3] = { "self", "ctx", "at" };
VarInfo __struct_info__d91c21afef62b32c_field_8 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__327f7c9a660b1ab9_arg_types_var_15644416245097476908, __type_info__327f7c9a660b1ab9_arg_names_var_15644416245097476908, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,LineInfo const ))>::size, UINT64_C(0x327f7c9a660b1ab9), "onInstrument", offsetof(debugapi::DapiDebugAgent,onInstrument), 0 };
TypeInfo * __type_info__21b69962c22005be_arg_types_var_15644416245097476908[5] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__ed65100a3b73031a, &__type_info__af81fe4c86352052, &__type_info__b68d800849332aec };
const char * __type_info__21b69962c22005be_arg_names_var_15644416245097476908[5] = { "self", "ctx", "fn", "entering", "userData" };
VarInfo __struct_info__d91c21afef62b32c_field_9 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__21b69962c22005be_arg_types_var_15644416245097476908, __type_info__21b69962c22005be_arg_names_var_15644416245097476908, 5, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,SimFunction * const ,bool,uint64_t))>::size, UINT64_C(0x21b69962c22005be), "onInstrumentFunction", offsetof(debugapi::DapiDebugAgent,onInstrumentFunction), 0 };
TypeInfo * __type_info__c913774fc6d2e043_arg_types_var_15644416245097476908[5] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__246dda13a8a4b104, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
const char * __type_info__c913774fc6d2e043_arg_names_var_15644416245097476908[5] = { "self", "ctx", "at", "reason", "text" };
VarInfo __struct_info__d91c21afef62b32c_field_10 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__c913774fc6d2e043_arg_types_var_15644416245097476908, __type_info__c913774fc6d2e043_arg_names_var_15644416245097476908, 5, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,LineInfo const ,char * const ,char * const ))>::size, UINT64_C(0xc913774fc6d2e043), "onBreakpoint", offsetof(debugapi::DapiDebugAgent,onBreakpoint), 0 };
TypeInfo * __type_info__79d44cbc9a657bf4_arg_types_var_15644416245097476908[6] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__bc67beb4aa160fd4, &__type_info__9c225ec61b3e6a3c };
const char * __type_info__79d44cbc9a657bf4_arg_names_var_15644416245097476908[6] = { "self", "ctx", "category", "name", "info", "data" };
VarInfo __struct_info__d91c21afef62b32c_field_11 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__79d44cbc9a657bf4_arg_types_var_15644416245097476908, __type_info__79d44cbc9a657bf4_arg_names_var_15644416245097476908, 6, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,char * const ,char * const ,TypeInfo const ,void * const ))>::size, UINT64_C(0x79d44cbc9a657bf4), "onVariable", offsetof(debugapi::DapiDebugAgent,onVariable), 0 };
TypeInfo * __type_info__a73424c91982d80c_arg_types_var_15644416245097476908[3] = { &__type_info__4e03c6707d012d5b, &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52 };
const char * __type_info__a73424c91982d80c_arg_names_var_15644416245097476908[3] = { "self", "file", "breakpointsNum" };
VarInfo __struct_info__d91c21afef62b32c_field_12 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__a73424c91982d80c_arg_types_var_15644416245097476908, __type_info__a73424c91982d80c_arg_names_var_15644416245097476908, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,char * const ,int32_t))>::size, UINT64_C(0xa73424c91982d80c), "onBreakpointsReset", offsetof(debugapi::DapiDebugAgent,onBreakpointsReset), 0 };
TypeInfo * __type_info__edcf7675d0697793_arg_types_var_15644416245097476908[1] = { &__type_info__4e03c6707d012d5b };
const char * __type_info__edcf7675d0697793_arg_names_var_15644416245097476908[1] = { "self" };
VarInfo __struct_info__d91c21afef62b32c_field_13 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__edcf7675d0697793_arg_types_var_15644416245097476908, __type_info__edcf7675d0697793_arg_names_var_15644416245097476908, 1, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent))>::size, UINT64_C(0xedcf7675d0697793), "onTick", offsetof(debugapi::DapiDebugAgent,onTick), 0 };
TypeInfo * __type_info__913dec62813bfe4d_arg_types_var_15644416245097476908[3] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__246dda13a8a4b104 };
const char * __type_info__913dec62813bfe4d_arg_names_var_15644416245097476908[3] = { "self", "ctx", "at" };
VarInfo __struct_info__d91c21afef62b32c_field_14 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__913dec62813bfe4d_arg_types_var_15644416245097476908, __type_info__913dec62813bfe4d_arg_names_var_15644416245097476908, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,LineInfo const ))>::size, UINT64_C(0x913dec62813bfe4d), "onCollect", offsetof(debugapi::DapiDebugAgent,onCollect), 0 };
TypeInfo * __type_info__68ba67f3d12e3c98_arg_types_var_15644416245097476908[5] = { &__type_info__4e03c6707d012d5b, &__type_info__2d750e15c3790305, &__type_info__a9e212d4f301a82c, &__type_info__af8afe4c86446b52, &__type_info__921d3d0750982f13 };
const char * __type_info__68ba67f3d12e3c98_arg_names_var_15644416245097476908[5] = { "self", "context", "at", "level", "text" };
VarInfo __struct_info__d91c21afef62b32c_field_15 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__68ba67f3d12e3c98_arg_types_var_15644416245097476908, __type_info__68ba67f3d12e3c98_arg_names_var_15644416245097476908, 5, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,debugapi::DapiDebugAgent,Context * const ,LineInfo const  * const ,int32_t,char * const ))>::size, UINT64_C(0x68ba67f3d12e3c98), "onLog", offsetof(debugapi::DapiDebugAgent,onLog), 0 };
TypeInfo * __type_info__36c2061312a1876b_arg_types_var_15644416245097476908[2] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419 };
const char * __type_info__36c2061312a1876b_arg_names_var_15644416245097476908[2] = { "self", "ctx" };
VarInfo __struct_info__d91c21afef62b32c_field_16 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__36c2061312a1876b_arg_types_var_15644416245097476908, __type_info__36c2061312a1876b_arg_names_var_15644416245097476908, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context))>::size, UINT64_C(0x36c2061312a1876b), "onBeforeGC", offsetof(debugapi::DapiDebugAgent,onBeforeGC), 0 };
TypeInfo * __type_info__d024cad15fe2631f_arg_types_var_15644416245097476908[2] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419 };
const char * __type_info__d024cad15fe2631f_arg_names_var_15644416245097476908[2] = { "self", "ctx" };
VarInfo __struct_info__d91c21afef62b32c_field_17 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__d024cad15fe2631f_arg_types_var_15644416245097476908, __type_info__d024cad15fe2631f_arg_names_var_15644416245097476908, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context))>::size, UINT64_C(0xd024cad15fe2631f), "onAfterGC", offsetof(debugapi::DapiDebugAgent,onAfterGC), 0 };
TypeInfo * __type_info__df3b4662672293c8_arg_types_var_15644416245097476908[2] = { &__type_info__4e03c6707d012d5b, &__type_info__921d3d0750982f13 };
const char * __type_info__df3b4662672293c8_arg_names_var_15644416245097476908[2] = { "self", "command" };
VarInfo __struct_info__d91c21afef62b32c_field_18 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__df3b4662672293c8_arg_types_var_15644416245097476908, __type_info__df3b4662672293c8_arg_names_var_15644416245097476908, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,debugapi::DapiDebugAgent,char * const ))>::size, UINT64_C(0xdf3b4662672293c8), "onUserCommand", offsetof(debugapi::DapiDebugAgent,onUserCommand), 0 };
TypeInfo * __type_info__8055238a52f31e0c_arg_types_var_15644416245097476908[5] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__9c225ec61b3e6a3c, &__type_info__b68d800849332aec, &__type_info__246dda13a8a4b104 };
const char * __type_info__8055238a52f31e0c_arg_names_var_15644416245097476908[5] = { "self", "ctx", "data", "size", "at" };
VarInfo __struct_info__d91c21afef62b32c_field_19 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__8055238a52f31e0c_arg_types_var_15644416245097476908, __type_info__8055238a52f31e0c_arg_names_var_15644416245097476908, 5, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,void * const ,uint64_t,LineInfo const ))>::size, UINT64_C(0x8055238a52f31e0c), "onAllocate", offsetof(debugapi::DapiDebugAgent,onAllocate), 0 };
TypeInfo * __type_info__3d1ee78d21cd5f19_arg_types_var_15644416245097476908[7] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__9c225ec61b3e6a3c, &__type_info__b68d800849332aec, &__type_info__9c225ec61b3e6a3c, &__type_info__b68d800849332aec, &__type_info__246dda13a8a4b104 };
const char * __type_info__3d1ee78d21cd5f19_arg_names_var_15644416245097476908[7] = { "self", "ctx", "data", "size", "newData", "newSize", "at" };
VarInfo __struct_info__d91c21afef62b32c_field_20 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__3d1ee78d21cd5f19_arg_types_var_15644416245097476908, __type_info__3d1ee78d21cd5f19_arg_names_var_15644416245097476908, 7, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,void * const ,uint64_t,void * const ,uint64_t,LineInfo const ))>::size, UINT64_C(0x3d1ee78d21cd5f19), "onReallocate", offsetof(debugapi::DapiDebugAgent,onReallocate), 0 };
TypeInfo * __type_info__6ff5841b5071093e_arg_types_var_15644416245097476908[4] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__9c225ec61b3e6a3c, &__type_info__246dda13a8a4b104 };
const char * __type_info__6ff5841b5071093e_arg_names_var_15644416245097476908[4] = { "self", "ctx", "data", "at" };
VarInfo __struct_info__d91c21afef62b32c_field_21 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__6ff5841b5071093e_arg_types_var_15644416245097476908, __type_info__6ff5841b5071093e_arg_names_var_15644416245097476908, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,void * const ,LineInfo const ))>::size, UINT64_C(0x6ff5841b5071093e), "onFree", offsetof(debugapi::DapiDebugAgent,onFree), 0 };
TypeInfo * __type_info__f1f0b5451e923dbc_arg_types_var_15644416245097476908[6] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__9c225ec61b3e6a3c, &__type_info__b68d800849332aec, &__type_info__af81fe4c86352052, &__type_info__246dda13a8a4b104 };
const char * __type_info__f1f0b5451e923dbc_arg_names_var_15644416245097476908[6] = { "self", "ctx", "data", "size", "tempString", "at" };
VarInfo __struct_info__d91c21afef62b32c_field_22 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__f1f0b5451e923dbc_arg_types_var_15644416245097476908, __type_info__f1f0b5451e923dbc_arg_names_var_15644416245097476908, 6, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,void * const ,uint64_t,bool,LineInfo const ))>::size, UINT64_C(0xf1f0b5451e923dbc), "onAllocateString", offsetof(debugapi::DapiDebugAgent,onAllocateString), 0 };
TypeInfo * __type_info__c283a5754fe3daf6_arg_types_var_15644416245097476908[5] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__9c225ec61b3e6a3c, &__type_info__af81fe4c86352052, &__type_info__246dda13a8a4b104 };
const char * __type_info__c283a5754fe3daf6_arg_names_var_15644416245097476908[5] = { "self", "ctx", "data", "tempString", "at" };
VarInfo __struct_info__d91c21afef62b32c_field_23 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__c283a5754fe3daf6_arg_types_var_15644416245097476908, __type_info__c283a5754fe3daf6_arg_names_var_15644416245097476908, 5, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,void * const ,bool,LineInfo const ))>::size, UINT64_C(0xc283a5754fe3daf6), "onFreeString", offsetof(debugapi::DapiDebugAgent,onFreeString), 0 };
VarInfo __struct_info__d91c21afef62b32c_field_24 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__44ca287faf79178, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, TypeSize<DebugAgent *>::size, UINT64_C(0x817bf1fe93f2f5d7), "thisAgent", offsetof(debugapi::DapiDebugAgent,thisAgent), 25 };
VarInfo * __struct_info__d91c21afef62b32c_fields[25] =  { &__struct_info__d91c21afef62b32c_field_0, &__struct_info__d91c21afef62b32c_field_1, &__struct_info__d91c21afef62b32c_field_2, &__struct_info__d91c21afef62b32c_field_3, &__struct_info__d91c21afef62b32c_field_4, &__struct_info__d91c21afef62b32c_field_5, &__struct_info__d91c21afef62b32c_field_6, &__struct_info__d91c21afef62b32c_field_7, &__struct_info__d91c21afef62b32c_field_8, &__struct_info__d91c21afef62b32c_field_9, &__struct_info__d91c21afef62b32c_field_10, &__struct_info__d91c21afef62b32c_field_11, &__struct_info__d91c21afef62b32c_field_12, &__struct_info__d91c21afef62b32c_field_13, &__struct_info__d91c21afef62b32c_field_14, &__struct_info__d91c21afef62b32c_field_15, &__struct_info__d91c21afef62b32c_field_16, &__struct_info__d91c21afef62b32c_field_17, &__struct_info__d91c21afef62b32c_field_18, &__struct_info__d91c21afef62b32c_field_19, &__struct_info__d91c21afef62b32c_field_20, &__struct_info__d91c21afef62b32c_field_21, &__struct_info__d91c21afef62b32c_field_22, &__struct_info__d91c21afef62b32c_field_23, &__struct_info__d91c21afef62b32c_field_24 };
StructInfo __struct_info__d91c21afef62b32c = {"DapiDebugAgent", "debugapi", 13, __struct_info__d91c21afef62b32c_fields, 25, TypeSize<debugapi::DapiDebugAgent>::size, UINT64_C(0x0), nullptr, UINT64_C(0xd91c21afef62b32c), 0 };
VarInfo __struct_info__540344a4a2e38e82_field_0 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__a066e05f8b62d82b, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<TArray<profiler::PerfEvent>>::size, UINT64_C(0xf43b147ac1a7fc79), "events", offsetof(profiler::PerfContext,events), 2 };
VarInfo __struct_info__540344a4a2e38e82_field_1 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<bool>::size, UINT64_C(0xc520db4aaaaf1653), "enabled", offsetof(profiler::PerfContext,enabled), 0 };
VarInfo * __struct_info__540344a4a2e38e82_fields[2] =  { &__struct_info__540344a4a2e38e82_field_0, &__struct_info__540344a4a2e38e82_field_1 };
StructInfo __struct_info__540344a4a2e38e82 = {"PerfContext", "profiler", 12, __struct_info__540344a4a2e38e82_fields, 2, TypeSize<profiler::PerfContext>::size, UINT64_C(0x0), nullptr, UINT64_C(0x540344a4a2e38e82), 0 };
VarInfo __struct_info__217389ff4ed5b430_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__5ac778689ccf4816, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, TypeSize<SimFunction *>::size, UINT64_C(0x2518118bebc91673), "fun", offsetof(profiler::PerfEvent,fun), 5 };
VarInfo __struct_info__217389ff4ed5b430_field_1 =  { Type::tInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int64_t>::size, UINT64_C(0x12c6b1f833ad265), "ts", offsetof(profiler::PerfEvent,ts), 0 };
VarInfo __struct_info__217389ff4ed5b430_field_2 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<bool>::size, UINT64_C(0xbd98cfe33a401a3a), "entering", offsetof(profiler::PerfEvent,entering), 0 };
VarInfo __struct_info__217389ff4ed5b430_field_3 =  { Type::tUInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<uint64_t>::size, UINT64_C(0x3e5bc1e454ff87df), "heapBytes", offsetof(profiler::PerfEvent,heapBytes), 0 };
VarInfo __struct_info__217389ff4ed5b430_field_4 =  { Type::tUInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<uint64_t>::size, UINT64_C(0x960d426a54660a92), "stringHeapBytes", offsetof(profiler::PerfEvent,stringHeapBytes), 0 };
VarInfo * __struct_info__217389ff4ed5b430_fields[5] =  { &__struct_info__217389ff4ed5b430_field_0, &__struct_info__217389ff4ed5b430_field_1, &__struct_info__217389ff4ed5b430_field_2, &__struct_info__217389ff4ed5b430_field_3, &__struct_info__217389ff4ed5b430_field_4 };
StructInfo __struct_info__217389ff4ed5b430 = {"PerfEvent", "profiler", 12, __struct_info__217389ff4ed5b430_fields, 5, TypeSize<profiler::PerfEvent>::size, UINT64_C(0x0), nullptr, UINT64_C(0x217389ff4ed5b430), 0 };
VarInfo __struct_info__5a07f965532a3ee4_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__5ac778689ccf4816, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, TypeSize<SimFunction *>::size, UINT64_C(0xaf2cc453608f439a), "fun", offsetof(profiler::PerfNode,fun), 8 };
VarInfo __struct_info__5a07f965532a3ee4_field_1 =  { Type::tUInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<uint64_t>::size, UINT64_C(0x9a776d9448d67666), "count", offsetof(profiler::PerfNode,count), 0 };
VarInfo __struct_info__5a07f965532a3ee4_field_2 =  { Type::tInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int64_t>::size, UINT64_C(0x615fefeb6580392), "total_time", offsetof(profiler::PerfNode,total_time), 0 };
VarInfo __struct_info__5a07f965532a3ee4_field_3 =  { Type::tInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int64_t>::size, UINT64_C(0xcd25bd76dbc4b9e1), "enter_time", offsetof(profiler::PerfNode,enter_time), 0 };
VarInfo __struct_info__5a07f965532a3ee4_field_4 =  { Type::tUInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<uint64_t>::size, UINT64_C(0xbbd181d24ec96740), "total_heap_bytes", offsetof(profiler::PerfNode,total_heap_bytes), 0 };
VarInfo __struct_info__5a07f965532a3ee4_field_5 =  { Type::tUInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<uint64_t>::size, UINT64_C(0x6efbf0c62a11b261), "enter_heap_bytes", offsetof(profiler::PerfNode,enter_heap_bytes), 0 };
VarInfo __struct_info__5a07f965532a3ee4_field_6 =  { Type::tUInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<uint64_t>::size, UINT64_C(0x5a4468c8ea4b9f0e), "total_string_heap_bytes", offsetof(profiler::PerfNode,total_string_heap_bytes), 0 };
VarInfo __struct_info__5a07f965532a3ee4_field_7 =  { Type::tUInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<uint64_t>::size, UINT64_C(0x4470c1f2e173529), "enter_string_heap_bytes", offsetof(profiler::PerfNode,enter_string_heap_bytes), 0 };
VarInfo __struct_info__5a07f965532a3ee4_field_8 =  { Type::tTable, nullptr, nullptr, nullptr, &__type_info__831390c0482e333b, &__type_info__9ef92bcca2cb528b, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<TTable<SimFunction *,profiler::PerfNode *>>::size, UINT64_C(0xdd5d207611eb42b), "children", offsetof(profiler::PerfNode,children), 9 };
VarInfo * __struct_info__5a07f965532a3ee4_fields[9] =  { &__struct_info__5a07f965532a3ee4_field_0, &__struct_info__5a07f965532a3ee4_field_1, &__struct_info__5a07f965532a3ee4_field_2, &__struct_info__5a07f965532a3ee4_field_3, &__struct_info__5a07f965532a3ee4_field_4, &__struct_info__5a07f965532a3ee4_field_5, &__struct_info__5a07f965532a3ee4_field_6, &__struct_info__5a07f965532a3ee4_field_7, &__struct_info__5a07f965532a3ee4_field_8 };
StructInfo __struct_info__5a07f965532a3ee4 = {"PerfNode", "profiler", 12, __struct_info__5a07f965532a3ee4_fields, 9, TypeSize<profiler::PerfNode>::size, UINT64_C(0x0), nullptr, UINT64_C(0x5a07f965532a3ee4), 0 };
VarInfo __struct_info__574b31acc906991f_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, TypeSize<void *>::size, UINT64_C(0xd0b84c3839aa73b6), "__rtti", offsetof(profiler::ProfilerDebugAgent,__rtti), 24 };
TypeInfo * __type_info__37a12b8313bf8dcd_arg_types_var_6290175922729818399[1] = { &__type_info__4e03c6707d012d5b };
const char * __type_info__37a12b8313bf8dcd_arg_names_var_6290175922729818399[1] = { "self" };
VarInfo __struct_info__574b31acc906991f_field_1 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__37a12b8313bf8dcd_arg_types_var_6290175922729818399, __type_info__37a12b8313bf8dcd_arg_names_var_6290175922729818399, 1, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent))>::size, UINT64_C(0x37a12b8313bf8dcd), "__finalize", offsetof(profiler::ProfilerDebugAgent,__finalize), 0 };
TypeInfo * __type_info__6076ae3d2f9a46c_arg_types_var_6290175922729818399[2] = { &__type_info__4e03c6707d012d5b, &__type_info__209f9e4a9562d5c2 };
const char * __type_info__6076ae3d2f9a46c_arg_names_var_6290175922729818399[2] = { "self", "agent" };
VarInfo __struct_info__574b31acc906991f_field_2 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__6076ae3d2f9a46c_arg_types_var_6290175922729818399, __type_info__6076ae3d2f9a46c_arg_names_var_6290175922729818399, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,DebugAgent * const ))>::size, UINT64_C(0x6076ae3d2f9a46c), "onInstall", offsetof(profiler::ProfilerDebugAgent,onInstall), 0 };
TypeInfo * __type_info__fe1aa7a83218828e_arg_types_var_6290175922729818399[2] = { &__type_info__4e03c6707d012d5b, &__type_info__209f9e4a9562d5c2 };
const char * __type_info__fe1aa7a83218828e_arg_names_var_6290175922729818399[2] = { "self", "agent" };
VarInfo __struct_info__574b31acc906991f_field_3 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__fe1aa7a83218828e_arg_types_var_6290175922729818399, __type_info__fe1aa7a83218828e_arg_names_var_6290175922729818399, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,DebugAgent * const ))>::size, UINT64_C(0xfe1aa7a83218828e), "onUninstall", offsetof(profiler::ProfilerDebugAgent,onUninstall), 0 };
TypeInfo * __type_info__b530a2a341319c0b_arg_types_var_6290175922729818399[2] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419 };
const char * __type_info__b530a2a341319c0b_arg_names_var_6290175922729818399[2] = { "self", "ctx" };
VarInfo __struct_info__574b31acc906991f_field_4 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__b530a2a341319c0b_arg_types_var_6290175922729818399, __type_info__b530a2a341319c0b_arg_names_var_6290175922729818399, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context))>::size, UINT64_C(0xb530a2a341319c0b), "onCreateContext", offsetof(profiler::ProfilerDebugAgent,onCreateContext), 0 };
TypeInfo * __type_info__c40fa3d28ebbfc50_arg_types_var_6290175922729818399[2] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419 };
const char * __type_info__c40fa3d28ebbfc50_arg_names_var_6290175922729818399[2] = { "self", "ctx" };
VarInfo __struct_info__574b31acc906991f_field_5 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__c40fa3d28ebbfc50_arg_types_var_6290175922729818399, __type_info__c40fa3d28ebbfc50_arg_names_var_6290175922729818399, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context))>::size, UINT64_C(0xc40fa3d28ebbfc50), "onDestroyContext", offsetof(profiler::ProfilerDebugAgent,onDestroyContext), 0 };
TypeInfo * __type_info__1d3f6245146bd6c7_arg_types_var_6290175922729818399[2] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419 };
const char * __type_info__1d3f6245146bd6c7_arg_names_var_6290175922729818399[2] = { "self", "ctx" };
VarInfo __struct_info__574b31acc906991f_field_6 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__1d3f6245146bd6c7_arg_types_var_6290175922729818399, __type_info__1d3f6245146bd6c7_arg_names_var_6290175922729818399, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context))>::size, UINT64_C(0x1d3f6245146bd6c7), "onSimulateContext", offsetof(profiler::ProfilerDebugAgent,onSimulateContext), 0 };
TypeInfo * __type_info__2aec03a5856eaa71_arg_types_var_6290175922729818399[3] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__246dda13a8a4b104 };
const char * __type_info__2aec03a5856eaa71_arg_names_var_6290175922729818399[3] = { "self", "ctx", "at" };
VarInfo __struct_info__574b31acc906991f_field_7 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__2aec03a5856eaa71_arg_types_var_6290175922729818399, __type_info__2aec03a5856eaa71_arg_names_var_6290175922729818399, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,LineInfo const ))>::size, UINT64_C(0x2aec03a5856eaa71), "onSingleStep", offsetof(profiler::ProfilerDebugAgent,onSingleStep), 0 };
TypeInfo * __type_info__817eb2964f5a2216_arg_types_var_6290175922729818399[3] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__246dda13a8a4b104 };
const char * __type_info__817eb2964f5a2216_arg_names_var_6290175922729818399[3] = { "self", "ctx", "at" };
VarInfo __struct_info__574b31acc906991f_field_8 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__817eb2964f5a2216_arg_types_var_6290175922729818399, __type_info__817eb2964f5a2216_arg_names_var_6290175922729818399, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,LineInfo const ))>::size, UINT64_C(0x817eb2964f5a2216), "onInstrument", offsetof(profiler::ProfilerDebugAgent,onInstrument), 0 };
TypeInfo * __type_info__16636b23c74d8eb1_arg_types_var_6290175922729818399[5] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__ed65100a3b73031a, &__type_info__af81fe4c86352052, &__type_info__b68d800849332aec };
const char * __type_info__16636b23c74d8eb1_arg_names_var_6290175922729818399[5] = { "self", "ctx", "fn", "entering", "userData" };
VarInfo __struct_info__574b31acc906991f_field_9 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__16636b23c74d8eb1_arg_types_var_6290175922729818399, __type_info__16636b23c74d8eb1_arg_names_var_6290175922729818399, 5, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,SimFunction * const ,bool,uint64_t))>::size, UINT64_C(0x16636b23c74d8eb1), "onInstrumentFunction", offsetof(profiler::ProfilerDebugAgent,onInstrumentFunction), 0 };
TypeInfo * __type_info__c558da89a705eba4_arg_types_var_6290175922729818399[5] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__246dda13a8a4b104, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
const char * __type_info__c558da89a705eba4_arg_names_var_6290175922729818399[5] = { "self", "ctx", "at", "reason", "text" };
VarInfo __struct_info__574b31acc906991f_field_10 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__c558da89a705eba4_arg_types_var_6290175922729818399, __type_info__c558da89a705eba4_arg_names_var_6290175922729818399, 5, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,LineInfo const ,char * const ,char * const ))>::size, UINT64_C(0xc558da89a705eba4), "onBreakpoint", offsetof(profiler::ProfilerDebugAgent,onBreakpoint), 0 };
TypeInfo * __type_info__16642f0ac9ace2e1_arg_types_var_6290175922729818399[6] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__bc67beb4aa160fd4, &__type_info__9c225ec61b3e6a3c };
const char * __type_info__16642f0ac9ace2e1_arg_names_var_6290175922729818399[6] = { "self", "ctx", "category", "name", "info", "data" };
VarInfo __struct_info__574b31acc906991f_field_11 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__16642f0ac9ace2e1_arg_types_var_6290175922729818399, __type_info__16642f0ac9ace2e1_arg_names_var_6290175922729818399, 6, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,char * const ,char * const ,TypeInfo const ,void * const ))>::size, UINT64_C(0x16642f0ac9ace2e1), "onVariable", offsetof(profiler::ProfilerDebugAgent,onVariable), 0 };
TypeInfo * __type_info__aa05b11c36ac3c35_arg_types_var_6290175922729818399[3] = { &__type_info__4e03c6707d012d5b, &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52 };
const char * __type_info__aa05b11c36ac3c35_arg_names_var_6290175922729818399[3] = { "self", "file", "breakpointsNum" };
VarInfo __struct_info__574b31acc906991f_field_12 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__aa05b11c36ac3c35_arg_types_var_6290175922729818399, __type_info__aa05b11c36ac3c35_arg_names_var_6290175922729818399, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,char * const ,int32_t))>::size, UINT64_C(0xaa05b11c36ac3c35), "onBreakpointsReset", offsetof(profiler::ProfilerDebugAgent,onBreakpointsReset), 0 };
TypeInfo * __type_info__db6d000acf6b1696_arg_types_var_6290175922729818399[1] = { &__type_info__4e03c6707d012d5b };
const char * __type_info__db6d000acf6b1696_arg_names_var_6290175922729818399[1] = { "self" };
VarInfo __struct_info__574b31acc906991f_field_13 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__db6d000acf6b1696_arg_types_var_6290175922729818399, __type_info__db6d000acf6b1696_arg_names_var_6290175922729818399, 1, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent))>::size, UINT64_C(0xdb6d000acf6b1696), "onTick", offsetof(profiler::ProfilerDebugAgent,onTick), 0 };
TypeInfo * __type_info__feceeddaf4049fbe_arg_types_var_6290175922729818399[3] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__246dda13a8a4b104 };
const char * __type_info__feceeddaf4049fbe_arg_names_var_6290175922729818399[3] = { "self", "ctx", "at" };
VarInfo __struct_info__574b31acc906991f_field_14 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__feceeddaf4049fbe_arg_types_var_6290175922729818399, __type_info__feceeddaf4049fbe_arg_names_var_6290175922729818399, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,LineInfo const ))>::size, UINT64_C(0xfeceeddaf4049fbe), "onCollect", offsetof(profiler::ProfilerDebugAgent,onCollect), 0 };
TypeInfo * __type_info__4bd6643d5162e7df_arg_types_var_6290175922729818399[5] = { &__type_info__4e03c6707d012d5b, &__type_info__2d750e15c3790305, &__type_info__a9e212d4f301a82c, &__type_info__af8afe4c86446b52, &__type_info__921d3d0750982f13 };
const char * __type_info__4bd6643d5162e7df_arg_names_var_6290175922729818399[5] = { "self", "context", "at", "level", "text" };
VarInfo __struct_info__574b31acc906991f_field_15 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__4bd6643d5162e7df_arg_types_var_6290175922729818399, __type_info__4bd6643d5162e7df_arg_names_var_6290175922729818399, 5, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,debugapi::DapiDebugAgent,Context * const ,LineInfo const  * const ,int32_t,char * const ))>::size, UINT64_C(0x4bd6643d5162e7df), "onLog", offsetof(profiler::ProfilerDebugAgent,onLog), 0 };
TypeInfo * __type_info__84019f70d7412afe_arg_types_var_6290175922729818399[2] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419 };
const char * __type_info__84019f70d7412afe_arg_names_var_6290175922729818399[2] = { "self", "ctx" };
VarInfo __struct_info__574b31acc906991f_field_16 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__84019f70d7412afe_arg_types_var_6290175922729818399, __type_info__84019f70d7412afe_arg_names_var_6290175922729818399, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context))>::size, UINT64_C(0x84019f70d7412afe), "onBeforeGC", offsetof(profiler::ProfilerDebugAgent,onBeforeGC), 0 };
TypeInfo * __type_info__53acf42a9c806f3c_arg_types_var_6290175922729818399[2] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419 };
const char * __type_info__53acf42a9c806f3c_arg_names_var_6290175922729818399[2] = { "self", "ctx" };
VarInfo __struct_info__574b31acc906991f_field_17 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__53acf42a9c806f3c_arg_types_var_6290175922729818399, __type_info__53acf42a9c806f3c_arg_names_var_6290175922729818399, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context))>::size, UINT64_C(0x53acf42a9c806f3c), "onAfterGC", offsetof(profiler::ProfilerDebugAgent,onAfterGC), 0 };
TypeInfo * __type_info__a1cd057ea1e464a7_arg_types_var_6290175922729818399[2] = { &__type_info__4e03c6707d012d5b, &__type_info__921d3d0750982f13 };
const char * __type_info__a1cd057ea1e464a7_arg_names_var_6290175922729818399[2] = { "self", "command" };
VarInfo __struct_info__574b31acc906991f_field_18 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63df4c8601f1a5, nullptr, (TypeInfo **)__type_info__a1cd057ea1e464a7_arg_types_var_6290175922729818399, __type_info__a1cd057ea1e464a7_arg_names_var_6290175922729818399, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,debugapi::DapiDebugAgent,char * const ))>::size, UINT64_C(0xa1cd057ea1e464a7), "onUserCommand", offsetof(profiler::ProfilerDebugAgent,onUserCommand), 0 };
TypeInfo * __type_info__f684a95a6ba6bf55_arg_types_var_6290175922729818399[5] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__9c225ec61b3e6a3c, &__type_info__b68d800849332aec, &__type_info__246dda13a8a4b104 };
const char * __type_info__f684a95a6ba6bf55_arg_names_var_6290175922729818399[5] = { "self", "ctx", "data", "size", "at" };
VarInfo __struct_info__574b31acc906991f_field_19 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__f684a95a6ba6bf55_arg_types_var_6290175922729818399, __type_info__f684a95a6ba6bf55_arg_names_var_6290175922729818399, 5, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,void * const ,uint64_t,LineInfo const ))>::size, UINT64_C(0xf684a95a6ba6bf55), "onAllocate", offsetof(profiler::ProfilerDebugAgent,onAllocate), 0 };
TypeInfo * __type_info__ac4299ef8241130e_arg_types_var_6290175922729818399[7] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__9c225ec61b3e6a3c, &__type_info__b68d800849332aec, &__type_info__9c225ec61b3e6a3c, &__type_info__b68d800849332aec, &__type_info__246dda13a8a4b104 };
const char * __type_info__ac4299ef8241130e_arg_names_var_6290175922729818399[7] = { "self", "ctx", "data", "size", "newData", "newSize", "at" };
VarInfo __struct_info__574b31acc906991f_field_20 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__ac4299ef8241130e_arg_types_var_6290175922729818399, __type_info__ac4299ef8241130e_arg_names_var_6290175922729818399, 7, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,void * const ,uint64_t,void * const ,uint64_t,LineInfo const ))>::size, UINT64_C(0xac4299ef8241130e), "onReallocate", offsetof(profiler::ProfilerDebugAgent,onReallocate), 0 };
TypeInfo * __type_info__f3a6ea626c8cbed7_arg_types_var_6290175922729818399[4] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__9c225ec61b3e6a3c, &__type_info__246dda13a8a4b104 };
const char * __type_info__f3a6ea626c8cbed7_arg_names_var_6290175922729818399[4] = { "self", "ctx", "data", "at" };
VarInfo __struct_info__574b31acc906991f_field_21 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__f3a6ea626c8cbed7_arg_types_var_6290175922729818399, __type_info__f3a6ea626c8cbed7_arg_names_var_6290175922729818399, 4, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,void * const ,LineInfo const ))>::size, UINT64_C(0xf3a6ea626c8cbed7), "onFree", offsetof(profiler::ProfilerDebugAgent,onFree), 0 };
TypeInfo * __type_info__735aba0d8f7e4eab_arg_types_var_6290175922729818399[6] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__9c225ec61b3e6a3c, &__type_info__b68d800849332aec, &__type_info__af81fe4c86352052, &__type_info__246dda13a8a4b104 };
const char * __type_info__735aba0d8f7e4eab_arg_names_var_6290175922729818399[6] = { "self", "ctx", "data", "size", "tempString", "at" };
VarInfo __struct_info__574b31acc906991f_field_22 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__735aba0d8f7e4eab_arg_types_var_6290175922729818399, __type_info__735aba0d8f7e4eab_arg_names_var_6290175922729818399, 6, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,void * const ,uint64_t,bool,LineInfo const ))>::size, UINT64_C(0x735aba0d8f7e4eab), "onAllocateString", offsetof(profiler::ProfilerDebugAgent,onAllocateString), 0 };
TypeInfo * __type_info__fb747bebfe3f3e55_arg_types_var_6290175922729818399[5] = { &__type_info__4e03c6707d012d5b, &__type_info__767637ee1a337419, &__type_info__9c225ec61b3e6a3c, &__type_info__af81fe4c86352052, &__type_info__246dda13a8a4b104 };
const char * __type_info__fb747bebfe3f3e55_arg_names_var_6290175922729818399[5] = { "self", "ctx", "data", "tempString", "at" };
VarInfo __struct_info__574b31acc906991f_field_23 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__fb747bebfe3f3e55_arg_types_var_6290175922729818399, __type_info__fb747bebfe3f3e55_arg_names_var_6290175922729818399, 5, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,void * const ,bool,LineInfo const ))>::size, UINT64_C(0xfb747bebfe3f3e55), "onFreeString", offsetof(profiler::ProfilerDebugAgent,onFreeString), 0 };
VarInfo __struct_info__574b31acc906991f_field_24 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__44ca287faf79178, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, TypeSize<DebugAgent *>::size, UINT64_C(0x49d531818b0ffac4), "thisAgent", offsetof(profiler::ProfilerDebugAgent,thisAgent), 26 };
VarInfo __struct_info__574b31acc906991f_field_25 =  { Type::tInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int64_t>::size, UINT64_C(0xfd852d29a21826c9), "us0", offsetof(profiler::ProfilerDebugAgent,us0), 0 };
VarInfo __struct_info__574b31acc906991f_field_26 =  { Type::tTable, nullptr, nullptr, nullptr, &__type_info__d9307e078cfb0f0c, &__type_info__3a369d6747c19883, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<TTable<uint64_t,profiler::PerfContext>>::size, UINT64_C(0x6b0be594474def95), "events", offsetof(profiler::ProfilerDebugAgent,events), 27 };
VarInfo __struct_info__574b31acc906991f_field_27 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__16d0aa3dd6b69257, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, TypeSize<FILE const  *>::size, UINT64_C(0x55bd2f29eced282f), "out", offsetof(profiler::ProfilerDebugAgent,out), 31 };
VarInfo __struct_info__574b31acc906991f_field_28 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<bool>::size, UINT64_C(0xd5bb05923125aff), "firstRecord", offsetof(profiler::ProfilerDebugAgent,firstRecord), 0 };
VarInfo __struct_info__574b31acc906991f_field_29 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<bool>::size, UINT64_C(0xf724df2c564e5d7a), "manual", offsetof(profiler::ProfilerDebugAgent,manual), 0 };
VarInfo __struct_info__574b31acc906991f_field_30 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<bool>::size, UINT64_C(0xcfa971b639d18578), "report_memory", offsetof(profiler::ProfilerDebugAgent,report_memory), 0 };
VarInfo __struct_info__574b31acc906991f_field_31 =  { Type::tTable, nullptr, nullptr, nullptr, &__type_info__d9307e078cfb0f0c, &__type_info__af63eb4c86020609, nullptr, nullptr, 0, 0, nullptr, 40962, TypeSize<TTable<uint64_t,void>>::size, UINT64_C(0x553a3df51114fd10), "instrumented", offsetof(profiler::ProfilerDebugAgent,instrumented), 32 };
VarInfo __struct_info__574b31acc906991f_field_32 =  { Type::tTable, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, &__type_info__d922fe078cefab30, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<TTable<char *,int64_t>>::size, UINT64_C(0x6d53a32104c74859), "map_time_units", offsetof(profiler::ProfilerDebugAgent,map_time_units), 33 };
VarInfo __struct_info__574b31acc906991f_field_33 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, TypeSize<char *>::size, UINT64_C(0x4293b9d029c3fd4c), "time_unit", offsetof(profiler::ProfilerDebugAgent,time_unit), 48 };
TypeInfo * __type_info__3e2343ef50b19acb_arg_types_var_6290175922729818399[1] = { &__type_info__216e8f5cf53cc8a6 };
const char * __type_info__3e2343ef50b19acb_arg_names_var_6290175922729818399[1] = { "self" };
VarInfo __struct_info__574b31acc906991f_field_34 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__d6eadf66eea309ce, nullptr, (TypeInfo **)__type_info__3e2343ef50b19acb_arg_types_var_6290175922729818399, __type_info__3e2343ef50b19acb_arg_names_var_6290175922729818399, 1, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((Func DAS_COMMENT((void,uint64_t,bool)),profiler::ProfilerDebugAgent))>::size, UINT64_C(0x3e2343ef50b19acb), "dependency", offsetof(profiler::ProfilerDebugAgent,dependency), 0 };
TypeInfo * __type_info__3953895e68f7fcc5_arg_types_var_6290175922729818399[2] = { &__type_info__216e8f5cf53cc8a6, &__type_info__af90fe4c864e9d52 };
const char * __type_info__3953895e68f7fcc5_arg_names_var_6290175922729818399[2] = { "self", "unit" };
VarInfo __struct_info__574b31acc906991f_field_35 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af81fe4c86352052, nullptr, (TypeInfo **)__type_info__3953895e68f7fcc5_arg_types_var_6290175922729818399, __type_info__3953895e68f7fcc5_arg_names_var_6290175922729818399, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,profiler::ProfilerDebugAgent,char * const ))>::size, UINT64_C(0x3953895e68f7fcc5), "is_time_unit_correct", offsetof(profiler::ProfilerDebugAgent,is_time_unit_correct), 0 };
TypeInfo * __type_info__bda9c2ded41fc3c_arg_types_var_6290175922729818399[3] = { &__type_info__216e8f5cf53cc8a6, &__type_info__8d8d8008262e16ec, &__type_info__af90fe4c864e9d52 };
const char * __type_info__bda9c2ded41fc3c_arg_names_var_6290175922729818399[3] = { "self", "time", "to" };
VarInfo __struct_info__574b31acc906991f_field_36 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__8d8d8008262e16ec, nullptr, (TypeInfo **)__type_info__bda9c2ded41fc3c_arg_types_var_6290175922729818399, __type_info__bda9c2ded41fc3c_arg_names_var_6290175922729818399, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((int64_t,profiler::ProfilerDebugAgent,int64_t,char * const ))>::size, UINT64_C(0xbda9c2ded41fc3c), "convert_ns_to_unit", offsetof(profiler::ProfilerDebugAgent,convert_ns_to_unit), 0 };
TypeInfo * __type_info__7a1acedae32682d6_arg_types_var_6290175922729818399[2] = { &__type_info__216e8f5cf53cc8a6, &__type_info__767637ee1a337419 };
const char * __type_info__7a1acedae32682d6_arg_names_var_6290175922729818399[2] = { "self", "ctx" };
VarInfo __struct_info__574b31acc906991f_field_37 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af81fe4c86352052, nullptr, (TypeInfo **)__type_info__7a1acedae32682d6_arg_types_var_6290175922729818399, __type_info__7a1acedae32682d6_arg_names_var_6290175922729818399, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((bool,profiler::ProfilerDebugAgent,Context))>::size, UINT64_C(0x7a1acedae32682d6), "isProfileable", offsetof(profiler::ProfilerDebugAgent,isProfileable), 0 };
TypeInfo * __type_info__70020a28b4ca2c0_arg_types_var_6290175922729818399[3] = { &__type_info__216e8f5cf53cc8a6, &__type_info__b68d800849332aec, &__type_info__af81fe4c86352052 };
const char * __type_info__70020a28b4ca2c0_arg_names_var_6290175922729818399[3] = { "self", "ctxId", "enabled" };
VarInfo __struct_info__574b31acc906991f_field_38 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__70020a28b4ca2c0_arg_types_var_6290175922729818399, __type_info__70020a28b4ca2c0_arg_names_var_6290175922729818399, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,profiler::ProfilerDebugAgent,uint64_t,bool))>::size, UINT64_C(0x70020a28b4ca2c0), "enable_profiler", offsetof(profiler::ProfilerDebugAgent,enable_profiler), 0 };
TypeInfo * __type_info__75ae552aca9561cb_arg_types_var_6290175922729818399[5] = { &__type_info__216e8f5cf53cc8a6, &__type_info__767637ee1a337419, &__type_info__831390c0482e333b, &__type_info__af81fe4c86352052, &__type_info__b68d800849332aec };
const char * __type_info__75ae552aca9561cb_arg_names_var_6290175922729818399[5] = { "self", "ctx", "fun", "entering", "userData" };
VarInfo __struct_info__574b31acc906991f_field_39 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__75ae552aca9561cb_arg_types_var_6290175922729818399, __type_info__75ae552aca9561cb_arg_names_var_6290175922729818399, 5, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,profiler::ProfilerDebugAgent,Context,SimFunction *,bool,uint64_t))>::size, UINT64_C(0x75ae552aca9561cb), "onInstrumentFunctionWithMemory", offsetof(profiler::ProfilerDebugAgent,onInstrumentFunctionWithMemory), 0 };
TypeInfo * __type_info__cb7283133fe45f6d_arg_types_var_6290175922729818399[2] = { &__type_info__216e8f5cf53cc8a6, &__type_info__af90fe4c864e9d52 };
const char * __type_info__cb7283133fe45f6d_arg_names_var_6290175922729818399[2] = { "self", "text" };
VarInfo __struct_info__574b31acc906991f_field_40 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__cb7283133fe45f6d_arg_types_var_6290175922729818399, __type_info__cb7283133fe45f6d_arg_names_var_6290175922729818399, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,profiler::ProfilerDebugAgent,char * const ))>::size, UINT64_C(0xcb7283133fe45f6d), "dump", offsetof(profiler::ProfilerDebugAgent,dump), 0 };
TypeInfo * __type_info__c8ec6cc996ce3186_arg_types_var_6290175922729818399[2] = { &__type_info__216e8f5cf53cc8a6, &__type_info__af90fe4c864e9d52 };
const char * __type_info__c8ec6cc996ce3186_arg_names_var_6290175922729818399[2] = { "self", "text" };
VarInfo __struct_info__574b31acc906991f_field_41 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__c8ec6cc996ce3186_arg_types_var_6290175922729818399, __type_info__c8ec6cc996ce3186_arg_names_var_6290175922729818399, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,profiler::ProfilerDebugAgent,char * const ))>::size, UINT64_C(0xc8ec6cc996ce3186), "write", offsetof(profiler::ProfilerDebugAgent,write), 0 };
TypeInfo * __type_info__3110e727e22189c9_arg_types_var_6290175922729818399[3] = { &__type_info__216e8f5cf53cc8a6, &__type_info__d1a453a44e333f31, &__type_info__b68d800849332aec };
const char * __type_info__3110e727e22189c9_arg_names_var_6290175922729818399[3] = { "self", "ev", "tid" };
VarInfo __struct_info__574b31acc906991f_field_42 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__3110e727e22189c9_arg_types_var_6290175922729818399, __type_info__3110e727e22189c9_arg_names_var_6290175922729818399, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,profiler::ProfilerDebugAgent,profiler::PerfEvent const ,uint64_t))>::size, UINT64_C(0x3110e727e22189c9), "dump_event", offsetof(profiler::ProfilerDebugAgent,dump_event), 0 };
TypeInfo * __type_info__f0c4248141b60967_arg_types_var_6290175922729818399[3] = { &__type_info__216e8f5cf53cc8a6, &__type_info__bf9a0c4f15947355, &__type_info__af8afe4c86446b52 };
const char * __type_info__f0c4248141b60967_arg_names_var_6290175922729818399[3] = { "self", "node", "tab" };
VarInfo __struct_info__574b31acc906991f_field_43 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__f0c4248141b60967_arg_types_var_6290175922729818399, __type_info__f0c4248141b60967_arg_names_var_6290175922729818399, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,profiler::ProfilerDebugAgent,profiler::PerfNode * const ,int32_t))>::size, UINT64_C(0xf0c4248141b60967), "dump_node", offsetof(profiler::ProfilerDebugAgent,dump_node), 0 };
TypeInfo * __type_info__bab2883f721d865e_arg_types_var_6290175922729818399[2] = { &__type_info__216e8f5cf53cc8a6, &__type_info__b68d800849332aec };
const char * __type_info__bab2883f721d865e_arg_names_var_6290175922729818399[2] = { "self", "tid" };
VarInfo __struct_info__574b31acc906991f_field_44 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__bab2883f721d865e_arg_types_var_6290175922729818399, __type_info__bab2883f721d865e_arg_names_var_6290175922729818399, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,profiler::ProfilerDebugAgent,uint64_t))>::size, UINT64_C(0xbab2883f721d865e), "dump_context_stack", offsetof(profiler::ProfilerDebugAgent,dump_context_stack), 0 };
TypeInfo * __type_info__ceb8028124c3af1c_arg_types_var_6290175922729818399[3] = { &__type_info__216e8f5cf53cc8a6, &__type_info__767637ee1a337419, &__type_info__b68d800849332aec };
const char * __type_info__ceb8028124c3af1c_arg_names_var_6290175922729818399[3] = { "self", "ctx", "tid" };
VarInfo __struct_info__574b31acc906991f_field_45 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__ceb8028124c3af1c_arg_types_var_6290175922729818399, __type_info__ceb8028124c3af1c_arg_names_var_6290175922729818399, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,profiler::ProfilerDebugAgent,Context,uint64_t))>::size, UINT64_C(0xceb8028124c3af1c), "dump_meta", offsetof(profiler::ProfilerDebugAgent,dump_meta), 0 };
TypeInfo * __type_info__3123e727e241d2c9_arg_types_var_6290175922729818399[2] = { &__type_info__216e8f5cf53cc8a6, &__type_info__b68d800849332aec };
const char * __type_info__3123e727e241d2c9_arg_names_var_6290175922729818399[2] = { "self", "tid" };
VarInfo __struct_info__574b31acc906991f_field_46 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__3123e727e241d2c9_arg_types_var_6290175922729818399, __type_info__3123e727e241d2c9_arg_names_var_6290175922729818399, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,profiler::ProfilerDebugAgent,uint64_t))>::size, UINT64_C(0x3123e727e241d2c9), "dump_events", offsetof(profiler::ProfilerDebugAgent,dump_events), 0 };
TypeInfo * __type_info__78f81aa6c5a74e4d_arg_types_var_6290175922729818399[3] = { &__type_info__216e8f5cf53cc8a6, &__type_info__767637ee1a337419, &__type_info__b68d800849332aec };
const char * __type_info__78f81aa6c5a74e4d_arg_names_var_6290175922729818399[3] = { "self", "ctx", "tid" };
VarInfo __struct_info__574b31acc906991f_field_47 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__78f81aa6c5a74e4d_arg_types_var_6290175922729818399, __type_info__78f81aa6c5a74e4d_arg_names_var_6290175922729818399, 3, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,profiler::ProfilerDebugAgent,Context,uint64_t))>::size, UINT64_C(0x78f81aa6c5a74e4d), "dump_context", offsetof(profiler::ProfilerDebugAgent,dump_context), 0 };
VarInfo * __struct_info__574b31acc906991f_fields[48] =  { &__struct_info__574b31acc906991f_field_0, &__struct_info__574b31acc906991f_field_1, &__struct_info__574b31acc906991f_field_2, &__struct_info__574b31acc906991f_field_3, &__struct_info__574b31acc906991f_field_4, &__struct_info__574b31acc906991f_field_5, &__struct_info__574b31acc906991f_field_6, &__struct_info__574b31acc906991f_field_7, &__struct_info__574b31acc906991f_field_8, &__struct_info__574b31acc906991f_field_9, &__struct_info__574b31acc906991f_field_10, &__struct_info__574b31acc906991f_field_11, &__struct_info__574b31acc906991f_field_12, &__struct_info__574b31acc906991f_field_13, &__struct_info__574b31acc906991f_field_14, &__struct_info__574b31acc906991f_field_15, &__struct_info__574b31acc906991f_field_16, &__struct_info__574b31acc906991f_field_17, &__struct_info__574b31acc906991f_field_18, &__struct_info__574b31acc906991f_field_19, &__struct_info__574b31acc906991f_field_20, &__struct_info__574b31acc906991f_field_21, &__struct_info__574b31acc906991f_field_22, &__struct_info__574b31acc906991f_field_23, &__struct_info__574b31acc906991f_field_24, &__struct_info__574b31acc906991f_field_25, &__struct_info__574b31acc906991f_field_26, &__struct_info__574b31acc906991f_field_27, &__struct_info__574b31acc906991f_field_28, &__struct_info__574b31acc906991f_field_29, &__struct_info__574b31acc906991f_field_30, &__struct_info__574b31acc906991f_field_31, &__struct_info__574b31acc906991f_field_32, &__struct_info__574b31acc906991f_field_33, &__struct_info__574b31acc906991f_field_34, &__struct_info__574b31acc906991f_field_35, &__struct_info__574b31acc906991f_field_36, &__struct_info__574b31acc906991f_field_37, &__struct_info__574b31acc906991f_field_38, &__struct_info__574b31acc906991f_field_39, &__struct_info__574b31acc906991f_field_40, &__struct_info__574b31acc906991f_field_41, &__struct_info__574b31acc906991f_field_42, &__struct_info__574b31acc906991f_field_43, &__struct_info__574b31acc906991f_field_44, &__struct_info__574b31acc906991f_field_45, &__struct_info__574b31acc906991f_field_46, &__struct_info__574b31acc906991f_field_47 };
StructInfo __struct_info__574b31acc906991f = {"ProfilerDebugAgent", "profiler", 29, __struct_info__574b31acc906991f_fields, 48, TypeSize<profiler::ProfilerDebugAgent>::size, UINT64_C(0x0), nullptr, UINT64_C(0x574b31acc906991f), 0 };
TypeInfo __type_info__6b1c7db4b71a781f = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~$::das_string"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 258, TypeSize<das::string>::size, UINT64_C(0x6b1c7db4b71a781f) };
TypeInfo __type_info__831390c0482e333b = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__5ac778689ccf4816, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, TypeSize<SimFunction *>::size, UINT64_C(0x831390c0482e333b) };
TypeInfo __type_info__9ef92bcca2cb528b = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__786ea93274f6826d, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, TypeSize<profiler::PerfNode *>::size, UINT64_C(0x9ef92bcca2cb528b) };
TypeInfo __type_info__921d3d0750982f13 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16484, TypeSize<char *>::size, UINT64_C(0x921d3d0750982f13) };
TypeInfo __type_info__a9e212d4f301a82c = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__246dda13a8a4b104, nullptr, nullptr, nullptr, 0, 0, nullptr, 8236, TypeSize<LineInfo const  *>::size, UINT64_C(0xa9e212d4f301a82c) };
TypeInfo __type_info__209f9e4a9562d5c2 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__44ca287faf79178, nullptr, nullptr, nullptr, 0, 0, nullptr, 8236, TypeSize<DebugAgent *>::size, UINT64_C(0x209f9e4a9562d5c2) };
TypeInfo __type_info__2d750e15c3790305 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__767637ee1a337419, nullptr, nullptr, nullptr, 0, 0, nullptr, 24620, TypeSize<Context *>::size, UINT64_C(0x2d750e15c3790305) };
TypeInfo __type_info__ed65100a3b73031a = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__5ac778689ccf4816, nullptr, nullptr, nullptr, 0, 0, nullptr, 24620, TypeSize<SimFunction *>::size, UINT64_C(0xed65100a3b73031a) };
TypeInfo __type_info__bf9a0c4f15947355 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__786ea93274f6826d, nullptr, nullptr, nullptr, 0, 0, nullptr, 24620, TypeSize<profiler::PerfNode *>::size, UINT64_C(0xbf9a0c4f15947355) };
TypeInfo __type_info__9c225ec61b3e6a3c = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, nullptr, nullptr, 0, 0, nullptr, 8236, TypeSize<void *>::size, UINT64_C(0x9c225ec61b3e6a3c) };
TypeInfo __type_info__16d0aa3dd6b69257 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~fio::FILE"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 34, TypeSize<FILE>::size, UINT64_C(0x16d0aa3dd6b69257) };
TypeInfo __type_info__246dda13a8a4b104 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~rtti::LineInfo"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 8230, TypeSize<LineInfo>::size, UINT64_C(0x246dda13a8a4b104) };
TypeInfo __type_info__bc67beb4aa160fd4 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~rtti::TypeInfo"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24622, TypeSize<TypeInfo>::size, UINT64_C(0xbc67beb4aa160fd4) };
TypeInfo __type_info__d1a453a44e333f31 = { Type::tStructure, &__struct_info__217389ff4ed5b430, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24622, TypeSize<profiler::PerfEvent>::size, UINT64_C(0xd1a453a44e333f31) };
TypeInfo __type_info__af81fe4c86352052 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, TypeSize<bool>::size, UINT64_C(0xaf81fe4c86352052) };
TypeInfo __type_info__af8afe4c86446b52 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, TypeSize<int32_t>::size, UINT64_C(0xaf8afe4c86446b52) };
TypeInfo __type_info__8d8d8008262e16ec = { Type::tInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, TypeSize<int64_t>::size, UINT64_C(0x8d8d8008262e16ec) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, TypeSize<char *>::size, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__b68d800849332aec = { Type::tUInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, TypeSize<uint64_t>::size, UINT64_C(0xb68d800849332aec) };
TypeInfo __type_info__44ca287faf79178 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~debugapi::DebugAgent"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 2, TypeSize<DebugAgent>::size, UINT64_C(0x44ca287faf79178) };
TypeInfo __type_info__767637ee1a337419 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~rtti::Context"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<Context>::size, UINT64_C(0x767637ee1a337419) };
TypeInfo __type_info__5ac778689ccf4816 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~rtti::SimFunction"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24590, TypeSize<SimFunction>::size, UINT64_C(0x5ac778689ccf4816) };
TypeInfo * __type_info__d6eadf66eea309ce_arg_types[2] = { &__type_info__d9307e078cfb0f0c, &__type_info__af81fe4c86352052 };
const char * __type_info__d6eadf66eea309ce_arg_names[2] = { "ctxId", "enabled" };
TypeInfo __type_info__d6eadf66eea309ce = { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__d6eadf66eea309ce_arg_types, __type_info__d6eadf66eea309ce_arg_names, 2, 0, nullptr, 12, TypeSize<Func DAS_COMMENT((void,uint64_t,bool))>::size, UINT64_C(0xd6eadf66eea309ce) };
TypeInfo __type_info__4e03c6707d012d5b = { Type::tStructure, &__struct_info__d91c21afef62b32c, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24590, TypeSize<debugapi::DapiDebugAgent>::size, UINT64_C(0x4e03c6707d012d5b) };
TypeInfo __type_info__3a369d6747c19883 = { Type::tStructure, &__struct_info__540344a4a2e38e82, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<profiler::PerfContext>::size, UINT64_C(0x3a369d6747c19883) };
TypeInfo __type_info__a066e05f8b62d82b = { Type::tStructure, &__struct_info__217389ff4ed5b430, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24590, TypeSize<profiler::PerfEvent>::size, UINT64_C(0xa066e05f8b62d82b) };
TypeInfo __type_info__786ea93274f6826d = { Type::tStructure, &__struct_info__5a07f965532a3ee4, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, TypeSize<profiler::PerfNode>::size, UINT64_C(0x786ea93274f6826d) };
TypeInfo __type_info__216e8f5cf53cc8a6 = { Type::tStructure, &__struct_info__574b31acc906991f, nullptr, nullptr, &__type_info__4e03c6707d012d5b, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<profiler::ProfilerDebugAgent>::size, UINT64_C(0x216e8f5cf53cc8a6) };
const char * __type_info__326921ccb9993fde_arg_names[10] = { "dead", "debug_context", "thread_clone", "job_clone", "opengl", "debugger_tick", "debugger_attached", "macro_context", "folding_context", "audio" };
TypeInfo __type_info__326921ccb9993fde = { Type::tBitfield, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, __type_info__326921ccb9993fde_arg_names, 10, 0, nullptr, 28, TypeSize<Bitfield>::size, UINT64_C(0x326921ccb9993fde) };
TypeInfo __type_info__af63df4c8601f1a5 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<bool>::size, UINT64_C(0xaf63df4c8601f1a5) };
TypeInfo __type_info__d922fe078cefab30 = { Type::tInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int64_t>::size, UINT64_C(0xd922fe078cefab30) };
TypeInfo __type_info__af63ee4c86020b22 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, TypeSize<char *>::size, UINT64_C(0xaf63ee4c86020b22) };
TypeInfo __type_info__d9307e078cfb0f0c = { Type::tUInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<uint64_t>::size, UINT64_C(0xd9307e078cfb0f0c) };
TypeInfo __type_info__af63eb4c86020609 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<void>::size, UINT64_C(0xaf63eb4c86020609) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {__type_info__6b1c7db4b71a781f, __type_info__16d0aa3dd6b69257, __type_info__246dda13a8a4b104, __type_info__bc67beb4aa160fd4, __type_info__44ca287faf79178, __type_info__767637ee1a337419, __type_info__5ac778689ccf4816, };
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_1[9] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__8d8d8008262e16ec, &__type_info__af90fe4c864e9d52, &__type_info__8d8d8008262e16ec, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_2[12] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__8d8d8008262e16ec, &__type_info__af90fe4c864e9d52, &__type_info__8d8d8008262e16ec, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__8d8d8008262e16ec, &__type_info__af90fe4c864e9d52, &__type_info__8d8d8008262e16ec, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_3[8] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__8d8d8008262e16ec, &__type_info__af90fe4c864e9d52, &__type_info__8d8d8008262e16ec, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_4[3] = { &__type_info__af90fe4c864e9d52, &__type_info__6b1c7db4b71a781f, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_5[4] = { &__type_info__b68d800849332aec, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__326921ccb9993fde };
TypeInfo * __tinfo_6[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__8d8d8008262e16ec, &__type_info__af90fe4c864e9d52 };

inline Sequence DAS_COMMENT((profiler::PerfNode * &)) _FuncbuiltinTickvaluesTick1351216622833168869_a4d809f70d013740 ( Context * __context__, TTable<SimFunction *,profiler::PerfNode *> & __a_rename_at_1202_0 );
inline Sequence DAS_COMMENT((profiler::PerfContext &)) _FuncbuiltinTickvaluesTick1351216622833168869_58e95cad5e33a23b ( Context * __context__, TTable<uint64_t,profiler::PerfContext> & __a_rename_at_1202_2 );
inline void finalize_7ec6032b9dc0345b ( Context * __context__, profiler::PerfEvent & ____this_rename_at_29_4 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_23175f5c66395481 ( Context * __context__, profiler::ProfilerDebugAgent const  & __cl_rename_at_116_5 );
inline void _FuncbuiltinTickfinalizeTick5454204887383796109_b84e8f61cdd6cfbe ( Context * __context__, TTable<SimFunction *,profiler::PerfNode *> & __a_rename_at_1245_6 );
inline void _FuncbuiltinTickfinalizeTick5454204887383796109_a90a12b574f1d50e ( Context * __context__, TTable<uint64_t,profiler::PerfContext> & __a_rename_at_1245_8 );
inline void _FuncbuiltinTickfinalizeTick5454204887383796109_13a27acb0aa845f7 ( Context * __context__, TTable<uint64_t,void> & __a_rename_at_1245_10 );
inline void _FuncbuiltinTickfinalizeTick5454204887383796109_1b617b3dea3b44eb ( Context * __context__, TTable<char *,int64_t> & __a_rename_at_1245_11 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_f33ad2da0da0fc40 ( Context * __context__, TArray<profiler::PerfEvent> & __a_rename_at_1234_12 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_509e2dba1d78f7f3 ( Context * __context__, TArray<profiler::PerfNode *> & __Arr_rename_at_68_14, int32_t __newSize_rename_at_68_15 );
inline void finalize_d5d88071a1a2b702 ( Context * __context__, profiler::PerfNode & ____this_rename_at_17_16 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_9a4a041519f183d5 ( Context * __context__, TTable<char *,int64_t> const  & __Tab_rename_at_1047_17, char * const  __at_rename_at_1047_18 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_fb9a18d52170edb4 ( Context * __context__, TTable<uint64_t,void> const  & __Tab_rename_at_1047_19, uint64_t __at_rename_at_1047_20 );
inline void _FuncbuiltinTickinsertTick10959621454228962049_297e40a85937efcc ( Context * __context__, TTable<uint64_t,void> & __Tab_rename_at_895_21, uint64_t __at_rename_at_895_22 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_8480a4ada694a4a ( Context * __context__, TTable<uint64_t,profiler::PerfContext> const  & __Tab_rename_at_1047_23, uint64_t __at_rename_at_1047_24 );
inline void finalize_76f4fe90df874c6 ( Context * __context__, profiler::PerfContext & ____this_rename_at_38_25 );
inline bool _FuncbuiltinTickeraseTick5639988512056021548_735549c230142cdc ( Context * __context__, TTable<uint64_t,void> & __Tab_rename_at_888_26, uint64_t __at_rename_at_888_27 );
inline void _FuncbuiltinTickpushTick10769833213962245646_561ff816281a0367 ( Context * __context__, TArray<profiler::PerfEvent> & __Arr_rename_at_181_28, profiler::PerfEvent & __value_rename_at_181_29 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_59d0b9e50e586e4 ( Context * __context__, TTable<SimFunction *,profiler::PerfNode *> const  & __Tab_rename_at_1047_30, SimFunction * const  __at_rename_at_1047_31 );
inline bool _FuncbuiltinTickeraseTick5639988512056021548_e7e3b7c59d77dc1b ( Context * __context__, TTable<uint64_t,profiler::PerfContext> & __Tab_rename_at_888_32, uint64_t __at_rename_at_888_33 );
inline TArray<char *> _FuncbuiltinTickget_command_line_argumentsTick14327939727965569528_f662626f31496e07 ( Context * __context__ );
inline uint64_t _FuncbuiltinTickintptrTick11320822648609276562_1714d144c7c46b3a ( Context * __context__, void * const  __p_rename_at_15_35 );
inline Sequence DAS_COMMENT((profiler::PerfNode * &)) _FuncbuiltinTickvaluesTick1935193042646774172_75d79b4b60ddfd81 ( Context * __context__, TTable<SimFunction *,profiler::PerfNode *> const  & __a_rename_at_1195_36 );
inline void _FuncbuiltinTickpushTick10769833213962245646_de2b37813d4362bf ( Context * __context__, TArray<profiler::PerfNode *> & __Arr_rename_at_181_38, profiler::PerfNode * __value_rename_at_181_39 );
inline profiler::PerfNode * & _FuncbuiltinTickbackTick18296309835877697278_7d600bb6b963e354 ( Context * __context__, TArray<profiler::PerfNode *> & __a_rename_at_473_40 );
inline void _FuncbuiltinTickpopTick1161079256290593740_e8064abe124c4e76 ( Context * __context__, TArray<profiler::PerfNode *> & __Arr_rename_at_132_42 );
inline void finalize_898c8b885e54e57e ( Context * __context__, profiler::PerfNode * & ____this_rename_at_233_43 );
inline void finalize_5f2b6f1cedc818a3 ( Context * __context__, profiler::ProfilerDebugAgent & ____this_rename_at_51_44 );
inline void _FuncdebugapiTickinstall_new_thread_local_debug_agentTick15124441463860820031_53778b6d1c803b14 ( Context * __context__, profiler::ProfilerDebugAgent * __agentPtr_rename_at_51_45 );
inline void set_enable_profiler_124bae90f2713c60 ( Context * __context__, uint64_t __ctxId_rename_at_45_48, bool __enabled_rename_at_45_49 );
inline Func DAS_COMMENT((void,uint64_t,bool)) _FuncProfilerDebugAgentTickdependency_c5b8c1449dcf4ec2 ( Context * __context__, profiler::ProfilerDebugAgent & __self_rename_at_62_50 );
inline bool _FuncProfilerDebugAgentTickis_time_unit_correct_e5df0855959b48d6 ( Context * __context__, profiler::ProfilerDebugAgent & __self_rename_at_66_51, char * const  __unit_rename_at_66_52 );
inline int64_t _FuncProfilerDebugAgentTickconvert_ns_to_unit_e8b63a47d67ad5c9 ( Context * __context__, profiler::ProfilerDebugAgent & __self_rename_at_70_53, int64_t __time_rename_at_70_54, char * const  __to_rename_at_70_55 );
inline void _FuncProfilerDebugAgentTickonInstall_f555c07af16faf7b ( Context * __context__, profiler::ProfilerDebugAgent & __self_rename_at_74_56, DebugAgent * const  __agent_rename_at_74_57 );
inline void _FuncProfilerDebugAgentTickonUninstall_298304d79d058498 ( Context * __context__, profiler::ProfilerDebugAgent & __self_rename_at_100_64, DebugAgent * const  __agent_rename_at_100_65 );
inline bool _FuncProfilerDebugAgentTickisProfileable_26822e4e90fe1652 ( Context * __context__, profiler::ProfilerDebugAgent & __self_rename_at_107_66, Context & __ctx_rename_at_107_67 );
inline void _FuncProfilerDebugAgentTickonCreateContext_d8f358d68c58d9ef ( Context * __context__, profiler::ProfilerDebugAgent & __self_rename_at_111_68, Context & __ctx_rename_at_111_69 );
inline void _FuncProfilerDebugAgentTickenable_profiler_979c874d994314a0 ( Context * __context__, profiler::ProfilerDebugAgent & __self_rename_at_119_70, uint64_t __ctxId_rename_at_119_71, bool __enabled_rename_at_119_72 );
inline void _FuncProfilerDebugAgentTickonDestroyContext_7df236229babeb37 ( Context * __context__, profiler::ProfilerDebugAgent & __self_rename_at_122_73, Context & __ctx_rename_at_122_74 );
inline void _FuncProfilerDebugAgentTickonInstrumentFunctionWithMemory_267dd430d221cc59 ( Context * __context__, profiler::ProfilerDebugAgent & __self_rename_at_133_76, Context & __ctx_rename_at_133_77, SimFunction * __fun_rename_at_133_78, bool __entering_rename_at_133_79, uint64_t __userData_rename_at_133_80 );
inline void _FuncProfilerDebugAgentTickonInstrumentFunction_9b1fb13d70c13403 ( Context * __context__, profiler::ProfilerDebugAgent & __self_rename_at_149_84, Context & __ctx_rename_at_149_85, SimFunction * __fun_rename_at_149_86, bool __entering_rename_at_149_87, uint64_t __userData_rename_at_149_88 );
inline void _FuncProfilerDebugAgentTickdump_cc84c3c1cbb56745 ( Context * __context__, profiler::ProfilerDebugAgent & __self_rename_at_161_91, char * const  __text_rename_at_161_92 );
inline void _FuncProfilerDebugAgentTickwrite_73b913f7789c5bd7 ( Context * __context__, profiler::ProfilerDebugAgent & __self_rename_at_169_93, char * const  __text_rename_at_169_94 );
inline void _FuncProfilerDebugAgentTickdump_event_5782bd96d61864da ( Context * __context__, profiler::ProfilerDebugAgent & __self_rename_at_174_95, profiler::PerfEvent const  & __ev_rename_at_174_96, uint64_t __tid_rename_at_174_97 );
inline void _FuncProfilerDebugAgentTickdump_node_7a5e6d3b88420fc2 ( Context * __context__, profiler::ProfilerDebugAgent & __self_rename_at_179_100, profiler::PerfNode * const  __node_rename_at_179_101, int32_t __tab_rename_at_179_102 );
inline void _FuncProfilerDebugAgentTickdump_context_stack_77f3b24d86f88464 ( Context * __context__, profiler::ProfilerDebugAgent & __self_rename_at_202_109, uint64_t __tid_rename_at_202_110 );
inline void _FuncProfilerDebugAgentTickdump_meta_ee131d376f01d903 ( Context * __context__, profiler::ProfilerDebugAgent & __self_rename_at_236_115, Context & __ctx_rename_at_236_116, uint64_t __tid_rename_at_236_117 );
inline void _FuncProfilerDebugAgentTickdump_events_b4b8dcaa6b59cfc6 ( Context * __context__, profiler::ProfilerDebugAgent & __self_rename_at_245_121, uint64_t __tid_rename_at_245_122 );
inline void _FuncProfilerDebugAgentTickdump_context_31251310253bebb1 ( Context * __context__, profiler::ProfilerDebugAgent & __self_rename_at_250_124, Context & __ctx_rename_at_250_125, uint64_t __tid_rename_at_250_126 );
inline void _FuncProfilerDebugAgent_0x27___finalize_1e7e1b45f5a9fe62 ( Context * __context__, profiler::ProfilerDebugAgent & __self_rename_at_51_127 );
inline void debug_agent_d264bf1ee1e8402e ( Context * __context__, Context const  & __ctx_rename_at_261_128 );
inline profiler::ProfilerDebugAgent ProfilerDebugAgent_5839a39d4ce45680 ( Context * __context__ );
inline TTable<char *,int64_t> _FuncbuiltinTickto_table_moveTick5858896087460481804_7ed588f7d03f97e5 ( Context * __context__, TDim<AutoTuple<char *,int64_t>,4> & __a_rename_at_1507_129 );

void __init_script ( Context * __context__, bool __init_shared )
{
    das_global_zero<profiler::ProfilerDebugAgent *,0x971ce3908ca8e1fe>(__context__);/*g_d_agent*/
}

inline Sequence DAS_COMMENT((profiler::PerfNode * &)) _FuncbuiltinTickvaluesTick1351216622833168869_a4d809f70d013740 ( Context * __context__, TTable<SimFunction *,profiler::PerfNode *> &  __a_rename_at_1202_0 )
{
    Sequence DAS_COMMENT((profiler::PerfNode * *)) __it_rename_at_1203_1;das_zero(__it_rename_at_1203_1);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((profiler::PerfNode *))>::pass(__it_rename_at_1203_1),das_arg<TTable<SimFunction *,profiler::PerfNode *>>::pass(__a_rename_at_1202_0),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((profiler::PerfNode * &))>::cast(__it_rename_at_1203_1);
}

inline Sequence DAS_COMMENT((profiler::PerfContext &)) _FuncbuiltinTickvaluesTick1351216622833168869_58e95cad5e33a23b ( Context * __context__, TTable<uint64_t,profiler::PerfContext> &  __a_rename_at_1202_2 )
{
    Sequence DAS_COMMENT((profiler::PerfContext *)) __it_rename_at_1203_3;das_zero(__it_rename_at_1203_3);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((profiler::PerfContext))>::pass(__it_rename_at_1203_3),das_arg<TTable<uint64_t,profiler::PerfContext>>::pass(__a_rename_at_1202_2),32,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((profiler::PerfContext &))>::cast(__it_rename_at_1203_3);
}

inline void finalize_7ec6032b9dc0345b ( Context * __context__, profiler::PerfEvent &  ____this_rename_at_29_4 )
{
    memset((void*)&(____this_rename_at_29_4), 0, TypeSize<profiler::PerfEvent>::size);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_23175f5c66395481 ( Context * __context__, profiler::ProfilerDebugAgent const  &  __cl_rename_at_116_5 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_5.__rtti))).getStructType())));
}

inline void _FuncbuiltinTickfinalizeTick5454204887383796109_b84e8f61cdd6cfbe ( Context * __context__, TTable<SimFunction *,profiler::PerfNode *> &  __a_rename_at_1245_6 )
{
    Sequence DAS_COMMENT((profiler::PerfNode * *)) _temp_make_local_1247_19_0; _temp_make_local_1247_19_0;
    {
        bool __need_loop_1247 = true;
        // aV: profiler::PerfNode?&
        das_iterator<Sequence DAS_COMMENT((profiler::PerfNode *))> __aV_iterator((_temp_make_local_1247_19_0 = (_FuncbuiltinTickvaluesTick1351216622833168869_a4d809f70d013740(__context__,das_arg<TTable<SimFunction *,profiler::PerfNode *>>::pass(__a_rename_at_1245_6)))));
        profiler::PerfNode * * __aV_rename_at_1247_7;
        __need_loop_1247 = __aV_iterator.first(__context__,(__aV_rename_at_1247_7)) && __need_loop_1247;
        for ( ; __need_loop_1247 ; __need_loop_1247 = __aV_iterator.next(__context__,(__aV_rename_at_1247_7)) )
        {
            finalize_898c8b885e54e57e(__context__,(*__aV_rename_at_1247_7));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1247_7));
    };
    builtin_table_free(das_arg<TTable<SimFunction *,profiler::PerfNode *>>::pass(__a_rename_at_1245_6),8,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick5454204887383796109_a90a12b574f1d50e ( Context * __context__, TTable<uint64_t,profiler::PerfContext> &  __a_rename_at_1245_8 )
{
    Sequence DAS_COMMENT((profiler::PerfContext *)) _temp_make_local_1247_19_1; _temp_make_local_1247_19_1;
    {
        bool __need_loop_1247 = true;
        // aV: profiler::PerfContext&
        das_iterator<Sequence DAS_COMMENT((profiler::PerfContext))> __aV_iterator((_temp_make_local_1247_19_1 = (_FuncbuiltinTickvaluesTick1351216622833168869_58e95cad5e33a23b(__context__,das_arg<TTable<uint64_t,profiler::PerfContext>>::pass(__a_rename_at_1245_8)))));
        profiler::PerfContext * __aV_rename_at_1247_9;
        __need_loop_1247 = __aV_iterator.first(__context__,(__aV_rename_at_1247_9)) && __need_loop_1247;
        for ( ; __need_loop_1247 ; __need_loop_1247 = __aV_iterator.next(__context__,(__aV_rename_at_1247_9)) )
        {
            finalize_76f4fe90df874c6(__context__,das_arg<profiler::PerfContext>::pass((*__aV_rename_at_1247_9)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1247_9));
    };
    builtin_table_free(das_arg<TTable<uint64_t,profiler::PerfContext>>::pass(__a_rename_at_1245_8),8,32,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick5454204887383796109_13a27acb0aa845f7 ( Context * __context__, TTable<uint64_t,void> &  __a_rename_at_1245_10 )
{
    builtin_table_free(das_arg<TTable<uint64_t,void>>::pass(__a_rename_at_1245_10),8,0,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick5454204887383796109_1b617b3dea3b44eb ( Context * __context__, TTable<char *,int64_t> &  __a_rename_at_1245_11 )
{
    builtin_table_free(das_arg<TTable<char *,int64_t>>::pass(__a_rename_at_1245_11),8,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_f33ad2da0da0fc40 ( Context * __context__, TArray<profiler::PerfEvent> &  __a_rename_at_1234_12 )
{
    {
        bool __need_loop_1236 = true;
        // aV: profiler::PerfEvent aka TT&
        das_iterator<TArray<profiler::PerfEvent>> __aV_iterator(__a_rename_at_1234_12);
        profiler::PerfEvent * __aV_rename_at_1236_13;
        __need_loop_1236 = __aV_iterator.first(__context__,(__aV_rename_at_1236_13)) && __need_loop_1236;
        for ( ; __need_loop_1236 ; __need_loop_1236 = __aV_iterator.next(__context__,(__aV_rename_at_1236_13)) )
        {
            finalize_7ec6032b9dc0345b(__context__,das_arg<profiler::PerfEvent>::pass((*__aV_rename_at_1236_13)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1236_13));
    };
    builtin_array_free(das_arg<TArray<profiler::PerfEvent>>::pass(__a_rename_at_1234_12),40,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_509e2dba1d78f7f3 ( Context * __context__, TArray<profiler::PerfNode *> &  __Arr_rename_at_68_14, int32_t __newSize_rename_at_68_15 )
{
    builtin_array_resize(das_arg<TArray<profiler::PerfNode *>>::pass(__Arr_rename_at_68_14),__newSize_rename_at_68_15,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void finalize_d5d88071a1a2b702 ( Context * __context__, profiler::PerfNode &  ____this_rename_at_17_16 )
{
    _FuncbuiltinTickfinalizeTick5454204887383796109_b84e8f61cdd6cfbe(__context__,das_arg<TTable<SimFunction *,profiler::PerfNode *>>::pass(____this_rename_at_17_16.children));
    memset((void*)&(____this_rename_at_17_16), 0, TypeSize<profiler::PerfNode>::size);
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_9a4a041519f183d5 ( Context * __context__, TTable<char *,int64_t> const  &  __Tab_rename_at_1047_17, char * const  __at_rename_at_1047_18 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1047_17,__at_rename_at_1047_18));
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_fb9a18d52170edb4 ( Context * __context__, TTable<uint64_t,void> const  &  __Tab_rename_at_1047_19, uint64_t __at_rename_at_1047_20 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1047_19,__at_rename_at_1047_20));
}

inline void _FuncbuiltinTickinsertTick10959621454228962049_297e40a85937efcc ( Context * __context__, TTable<uint64_t,void> &  __Tab_rename_at_895_21, uint64_t __at_rename_at_895_22 )
{
    __builtin_table_set_insert(__context__,__Tab_rename_at_895_21,__at_rename_at_895_22);
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_8480a4ada694a4a ( Context * __context__, TTable<uint64_t,profiler::PerfContext> const  &  __Tab_rename_at_1047_23, uint64_t __at_rename_at_1047_24 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1047_23,__at_rename_at_1047_24));
}

inline void finalize_76f4fe90df874c6 ( Context * __context__, profiler::PerfContext &  ____this_rename_at_38_25 )
{
    _FuncbuiltinTickfinalizeTick13836114024949725080_f33ad2da0da0fc40(__context__,das_arg<TArray<profiler::PerfEvent>>::pass(____this_rename_at_38_25.events));
    memset((void*)&(____this_rename_at_38_25), 0, TypeSize<profiler::PerfContext>::size);
}

inline bool _FuncbuiltinTickeraseTick5639988512056021548_735549c230142cdc ( Context * __context__, TTable<uint64_t,void> &  __Tab_rename_at_888_26, uint64_t __at_rename_at_888_27 )
{
    return das_auto_cast<bool>::cast(__builtin_table_erase(__context__,__Tab_rename_at_888_26,__at_rename_at_888_27));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_561ff816281a0367 ( Context * __context__, TArray<profiler::PerfEvent> &  __Arr_rename_at_181_28, profiler::PerfEvent &  __value_rename_at_181_29 )
{
    das_copy(__Arr_rename_at_181_28(builtin_array_push_back(das_arg<TArray<profiler::PerfEvent>>::pass(__Arr_rename_at_181_28),40,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_29);
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_59d0b9e50e586e4 ( Context * __context__, TTable<SimFunction *,profiler::PerfNode *> const  &  __Tab_rename_at_1047_30, SimFunction * const  __at_rename_at_1047_31 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1047_30,__at_rename_at_1047_31));
}

inline bool _FuncbuiltinTickeraseTick5639988512056021548_e7e3b7c59d77dc1b ( Context * __context__, TTable<uint64_t,profiler::PerfContext> &  __Tab_rename_at_888_32, uint64_t __at_rename_at_888_33 )
{
    return das_auto_cast<bool>::cast(__builtin_table_erase(__context__,__Tab_rename_at_888_32,__at_rename_at_888_33));
}

inline TArray<char *> _FuncbuiltinTickget_command_line_argumentsTick14327939727965569528_f662626f31496e07 ( Context * __context__ )
{
    TArray<char *> __args_rename_at_1805_34;das_zero(__args_rename_at_1805_34);
    getCommandLineArguments(das_arg<TArray<char *>>::pass(__args_rename_at_1805_34));
    return /* <- */ das_auto_cast_move<TArray<char *>>::cast(__args_rename_at_1805_34);
}

inline uint64_t _FuncbuiltinTickintptrTick11320822648609276562_1714d144c7c46b3a ( Context * __context__, void * const  __p_rename_at_15_35 )
{
    return das_auto_cast<uint64_t>::cast(das_cast<uint64_t>::cast(__p_rename_at_15_35));
}

inline Sequence DAS_COMMENT((profiler::PerfNode * &)) _FuncbuiltinTickvaluesTick1935193042646774172_75d79b4b60ddfd81 ( Context * __context__, TTable<SimFunction *,profiler::PerfNode *> const  &  __a_rename_at_1195_36 )
{
    Sequence DAS_COMMENT((profiler::PerfNode * *)) __it_rename_at_1196_37;das_zero(__it_rename_at_1196_37);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((profiler::PerfNode * const ))>::pass(__it_rename_at_1196_37),__a_rename_at_1195_36,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((profiler::PerfNode * &))>::cast(__it_rename_at_1196_37);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_de2b37813d4362bf ( Context * __context__, TArray<profiler::PerfNode *> &  __Arr_rename_at_181_38, profiler::PerfNode * __value_rename_at_181_39 )
{
    das_copy(__Arr_rename_at_181_38(builtin_array_push_back(das_arg<TArray<profiler::PerfNode *>>::pass(__Arr_rename_at_181_38),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_39);
}

inline profiler::PerfNode * & _FuncbuiltinTickbackTick18296309835877697278_7d600bb6b963e354 ( Context * __context__, TArray<profiler::PerfNode *> &  __a_rename_at_473_40 )
{
    int32_t __l_rename_at_474_41 = ((int32_t)builtin_array_size(das_arg<TArray<profiler::PerfNode *>>::pass(__a_rename_at_473_40)));
    if ( __l_rename_at_474_41 == 0 )
    {
        builtin_throw(((char *) "back empty array"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
    return das_auto_cast_ref<profiler::PerfNode * &>::cast(__a_rename_at_473_40((__l_rename_at_474_41 - 1),__context__));
}

inline void _FuncbuiltinTickpopTick1161079256290593740_e8064abe124c4e76 ( Context * __context__, TArray<profiler::PerfNode *> &  __Arr_rename_at_132_42 )
{
    _FuncbuiltinTickresizeTick4811697762258667383_509e2dba1d78f7f3(__context__,das_arg<TArray<profiler::PerfNode *>>::pass(__Arr_rename_at_132_42),builtin_array_size(das_arg<TArray<profiler::PerfNode *>>::pass(__Arr_rename_at_132_42)) - 1);
}

inline void finalize_898c8b885e54e57e ( Context * __context__, profiler::PerfNode * & ____this_rename_at_233_43 )
{
    if ( ____this_rename_at_233_43 != nullptr )
    {
        finalize_d5d88071a1a2b702(__context__,das_arg<profiler::PerfNode>::pass(das_deref(__context__,____this_rename_at_233_43)));
        das_delete<profiler::PerfNode *>::clear(__context__,____this_rename_at_233_43);
        das_copy(____this_rename_at_233_43,nullptr);
    };
}

inline void finalize_5f2b6f1cedc818a3 ( Context * __context__, profiler::ProfilerDebugAgent &  ____this_rename_at_51_44 )
{
    _FuncbuiltinTickfinalizeTick5454204887383796109_a90a12b574f1d50e(__context__,das_arg<TTable<uint64_t,profiler::PerfContext>>::pass(____this_rename_at_51_44.events));
    _FuncbuiltinTickfinalizeTick5454204887383796109_13a27acb0aa845f7(__context__,das_arg<TTable<uint64_t,void>>::pass(____this_rename_at_51_44.instrumented));
    _FuncbuiltinTickfinalizeTick5454204887383796109_1b617b3dea3b44eb(__context__,das_arg<TTable<char *,int64_t>>::pass(____this_rename_at_51_44.map_time_units));
    memset((void*)&(____this_rename_at_51_44), 0, TypeSize<profiler::ProfilerDebugAgent>::size);
}

inline void _FuncdebugapiTickinstall_new_thread_local_debug_agentTick15124441463860820031_53778b6d1c803b14 ( Context * __context__, profiler::ProfilerDebugAgent * __agentPtr_rename_at_51_45 )
{
    StructInfo const  * __agentInfo_rename_at_53_46; memset((void*)&__agentInfo_rename_at_53_46,0,sizeof(__agentInfo_rename_at_53_46));
    smart_ptr_raw<DebugAgent> __agentAdapter_rename_at_54_47; memset((void*)&__agentAdapter_rename_at_54_47,0,sizeof(__agentAdapter_rename_at_54_47));
    {
        /* finally */ auto __finally_52= das_finally([&](){
        das_delete_handle<smart_ptr_raw<DebugAgent>>::clear(__context__,__agentAdapter_rename_at_54_47);
        /* end finally */ });
        __agentInfo_rename_at_53_46 = ((StructInfo const  *)_FuncrttiTickclass_infoTick15801393167907430156_23175f5c66395481(__context__,das_arg<profiler::ProfilerDebugAgent>::pass(das_deref(__context__,__agentPtr_rename_at_51_45))));
        __agentAdapter_rename_at_54_47; das_zero(__agentAdapter_rename_at_54_47); das_move(__agentAdapter_rename_at_54_47, makeDebugAgent(das_auto_cast<void * const >::cast(__agentPtr_rename_at_51_45),__agentInfo_rename_at_53_46,__context__));
        das_copy(__agentPtr_rename_at_51_45->thisAgent,das_cast<DebugAgent *>::cast(__agentAdapter_rename_at_54_47));
        installThreadLocalDebugAgent(__agentAdapter_rename_at_54_47,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)),__context__);
        set_das_string(das_arg<das::string>::pass(thisContext(__context__).name /*name*/),((char *) "thread local debug agent"));
    };
}

inline void set_enable_profiler_124bae90f2713c60 ( Context * __context__, uint64_t __ctxId_rename_at_45_48, bool __enabled_rename_at_45_49 )
{
    if ( das_global<profiler::ProfilerDebugAgent *,0x971ce3908ca8e1fe>(__context__) /*g_d_agent*/ != nullptr )
    {
        das_invoke_method<void,offsetof(profiler::ProfilerDebugAgent,enable_profiler)>::invoke<profiler::ProfilerDebugAgent &,uint64_t,bool>(__context__,nullptr,das_arg<profiler::ProfilerDebugAgent>::pass(das_cast<profiler::ProfilerDebugAgent>::cast(das_deref(__context__,das_global<profiler::ProfilerDebugAgent *,0x971ce3908ca8e1fe>(__context__) /*g_d_agent*/))),__ctxId_rename_at_45_48,__enabled_rename_at_45_49);
    };
}

inline Func DAS_COMMENT((void,uint64_t,bool)) _FuncProfilerDebugAgentTickdependency_c5b8c1449dcf4ec2 ( Context * __context__, profiler::ProfilerDebugAgent &  __self_rename_at_62_50 )
{
    return das_auto_cast<Func DAS_COMMENT((void,uint64_t,bool))>::cast(Func(__context__->fnByMangledName(/*@profiler::set_enable_profiler u64 Cb*/ 0x141fa5e0c4b2e16b)));
}

inline bool _FuncProfilerDebugAgentTickis_time_unit_correct_e5df0855959b48d6 ( Context * __context__, profiler::ProfilerDebugAgent &  __self_rename_at_66_51, char * const  __unit_rename_at_66_52 )
{
    return das_auto_cast<bool>::cast(_FuncbuiltinTickkey_existsTick16808803843923989214_9a4a041519f183d5(__context__,das_arg<TTable<char *,int64_t>>::pass(__self_rename_at_66_51.map_time_units),__unit_rename_at_66_52));
}

inline int64_t _FuncProfilerDebugAgentTickconvert_ns_to_unit_e8b63a47d67ad5c9 ( Context * __context__, profiler::ProfilerDebugAgent &  __self_rename_at_70_53, int64_t __time_rename_at_70_54, char * const  __to_rename_at_70_55 )
{
    return das_auto_cast<int64_t>::cast(SimPolicy<int64_t>::Div(__time_rename_at_70_54,__self_rename_at_70_53.map_time_units(__to_rename_at_70_55,__context__),*__context__,nullptr));
}

inline void _FuncProfilerDebugAgentTickonInstall_f555c07af16faf7b ( Context * __context__, profiler::ProfilerDebugAgent &  __self_rename_at_74_56, DebugAgent * const  __agent_rename_at_74_57 )
{
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_77_30_2; _temp_make_local_77_30_2;
    set_das_string(das_arg<das::string>::pass(thisContext(__context__).name /*name*/),((char *) "__PROFILER__"));
    TArray<char *> __args_rename_at_76_58; das_zero(__args_rename_at_76_58); das_move(__args_rename_at_76_58, _FuncbuiltinTickget_command_line_argumentsTick14327939727965569528_f662626f31496e07(__context__));
    {
        bool __need_loop_77 = true;
        // argv: string&
        das_iterator<TArray<char *>> __argv_iterator(__args_rename_at_76_58);
        char * * __argv_rename_at_77_61;
        __need_loop_77 = __argv_iterator.first(__context__,(__argv_rename_at_77_61)) && __need_loop_77;
        // i: int
        das_iterator_count DAS_COMMENT((_temp_make_local_77_30_2 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __i_iterator(0,1);
        int32_t __i_rename_at_77_62;
        __need_loop_77 = __i_iterator.first(__context__,(__i_rename_at_77_62)) && __need_loop_77;
        for ( ; __need_loop_77 ; __need_loop_77 = __argv_iterator.next(__context__,(__argv_rename_at_77_61)) && __i_iterator.next(__context__,(__i_rename_at_77_62)) )
        {
            if ( ((SimPolicy<char *>::Equ(cast<char *>::from((*__argv_rename_at_77_61)),cast<char *>::from(((char *) "--das-profiler-log-file")),*__context__,nullptr)) && (__self_rename_at_74_56.out == nullptr)) && ((__i_rename_at_77_62 + 1) < builtin_array_size(das_arg<TArray<char *>>::pass(__args_rename_at_76_58))) )
            {
                das_copy(__self_rename_at_74_56.out,builtin_fopen(__args_rename_at_76_58((__i_rename_at_77_62 + 1),__context__),((char *) "wb")));
            } else if ( SimPolicy<char *>::Equ(cast<char *>::from((*__argv_rename_at_77_61)),cast<char *>::from(((char *) "--das-profiler-manual")),*__context__,nullptr) )
            {
                das_copy(__self_rename_at_74_56.manual,true);
            } else if ( SimPolicy<char *>::Equ(cast<char *>::from((*__argv_rename_at_77_61)),cast<char *>::from(((char *) "--das-profiler-memory")),*__context__,nullptr) )
            {
                das_copy(__self_rename_at_74_56.report_memory,true);
                das_copy(__self_rename_at_74_56.onInstrumentFunction,das_cast<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,SimFunction * const ,bool,uint64_t))>::cast(__self_rename_at_74_56.onInstrumentFunctionWithMemory));
            } else if ( (SimPolicy<char *>::Equ(cast<char *>::from((*__argv_rename_at_77_61)),cast<char *>::from(((char *) "--das-profiler-time-unit")),*__context__,nullptr)) && ((__i_rename_at_77_62 + 1) < builtin_array_size(das_arg<TArray<char *>>::pass(__args_rename_at_76_58))) )
            {
                char * __unit_rename_at_88_63 = ((char *)(char *)(__args_rename_at_76_58((__i_rename_at_77_62 + 1),__context__)));
                if ( !das_invoke_method<bool,offsetof(profiler::ProfilerDebugAgent,is_time_unit_correct)>::invoke<profiler::ProfilerDebugAgent &,char * const >(__context__,nullptr,das_arg<profiler::ProfilerDebugAgent>::pass(__self_rename_at_74_56),__unit_rename_at_88_63) )
                {
                    toLog(40000,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_0, cast<char * const >::from(((char *) "Time unit '")), cast<char * const >::from(__unit_rename_at_88_63), cast<char * const >::from(((char *) "' is not defined!")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                } else {
                    das_copy(__self_rename_at_74_56.time_unit,__unit_rename_at_88_63);
                };
            };
        }
        __argv_iterator.close(__context__,(__argv_rename_at_77_61));
        __i_iterator.close(__context__,(__i_rename_at_77_62));
    };
    das_invoke_method<void,offsetof(profiler::ProfilerDebugAgent,write)>::invoke<profiler::ProfilerDebugAgent &,char * const >(__context__,nullptr,das_arg<profiler::ProfilerDebugAgent>::pass(__self_rename_at_74_56),((char *) "["));
    das_copy(das_global<profiler::ProfilerDebugAgent *,0x971ce3908ca8e1fe>(__context__) /*g_d_agent*/,das_ref(__context__,__self_rename_at_74_56));
    das_copy(__self_rename_at_74_56.us0,ref_time_ticks());
}

inline void _FuncProfilerDebugAgentTickonUninstall_298304d79d058498 ( Context * __context__, profiler::ProfilerDebugAgent &  __self_rename_at_100_64, DebugAgent * const  __agent_rename_at_100_65 )
{
    if ( __self_rename_at_100_64.out != nullptr )
    {
        das_invoke_method<void,offsetof(profiler::ProfilerDebugAgent,write)>::invoke<profiler::ProfilerDebugAgent &,char * const >(__context__,nullptr,das_arg<profiler::ProfilerDebugAgent>::pass(__self_rename_at_100_64),((char *) "]"));
        builtin_fclose(__self_rename_at_100_64.out,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        das_copy(__self_rename_at_100_64.out,nullptr);
    };
}

inline bool _FuncProfilerDebugAgentTickisProfileable_26822e4e90fe1652 ( Context * __context__, profiler::ProfilerDebugAgent &  __self_rename_at_107_66, Context &  __ctx_rename_at_107_67 )
{
    return das_auto_cast<bool>::cast(!((((das_get_bitfield(__ctx_rename_at_107_67.category /*category*/,1u << 1) || das_get_bitfield(__ctx_rename_at_107_67.category /*category*/,1u << 7)) || das_get_bitfield(__ctx_rename_at_107_67.category /*category*/,1u << 8)) || das_get_bitfield(__ctx_rename_at_107_67.category /*category*/,1u << 5)) || das_get_bitfield(__ctx_rename_at_107_67.category /*category*/,1u << 6)));
}

inline void _FuncProfilerDebugAgentTickonCreateContext_d8f358d68c58d9ef ( Context * __context__, profiler::ProfilerDebugAgent &  __self_rename_at_111_68, Context &  __ctx_rename_at_111_69 )
{
    if ( !(das_invoke_method<bool,offsetof(profiler::ProfilerDebugAgent,isProfileable)>::invoke<profiler::ProfilerDebugAgent &,Context &>(__context__,nullptr,das_arg<profiler::ProfilerDebugAgent>::pass(__self_rename_at_111_68),das_arg<Context>::pass(__ctx_rename_at_111_69))) || _FuncbuiltinTickkey_existsTick16808803843923989214_fb9a18d52170edb4(__context__,das_arg<TTable<uint64_t,void>>::pass(__self_rename_at_111_68.instrumented),((__ctx_rename_at_111_69).getCodeAllocatorId())) )
    {
        return ;
    } else {
        _FuncbuiltinTickinsertTick10959621454228962049_297e40a85937efcc(__context__,das_arg<TTable<uint64_t,void>>::pass(__self_rename_at_111_68.instrumented),((__ctx_rename_at_111_69).getCodeAllocatorId()));
        das_copy(__self_rename_at_111_68.events(_FuncbuiltinTickintptrTick11320822648609276562_1714d144c7c46b3a(__context__,das_auto_cast<void * const >::cast(das_ref(__context__,__ctx_rename_at_111_69))),__context__).enabled,!__self_rename_at_111_68.manual);
        instrument_all_functions_thread_local(das_arg<Context>::pass(__ctx_rename_at_111_69));
    };
}

inline void _FuncProfilerDebugAgentTickenable_profiler_979c874d994314a0 ( Context * __context__, profiler::ProfilerDebugAgent &  __self_rename_at_119_70, uint64_t __ctxId_rename_at_119_71, bool __enabled_rename_at_119_72 )
{
    das_copy(__self_rename_at_119_70.events(__ctxId_rename_at_119_71,__context__).enabled,__enabled_rename_at_119_72);
}

inline void _FuncProfilerDebugAgentTickonDestroyContext_7df236229babeb37 ( Context * __context__, profiler::ProfilerDebugAgent &  __self_rename_at_122_73, Context &  __ctx_rename_at_122_74 )
{
    if ( !das_invoke_method<bool,offsetof(profiler::ProfilerDebugAgent,isProfileable)>::invoke<profiler::ProfilerDebugAgent &,Context &>(__context__,nullptr,das_arg<profiler::ProfilerDebugAgent>::pass(__self_rename_at_122_73),das_arg<Context>::pass(__ctx_rename_at_122_74)) )
    {
        return ;
    } else {
        uint64_t __tid_rename_at_126_75 = ((uint64_t)_FuncbuiltinTickintptrTick11320822648609276562_1714d144c7c46b3a(__context__,das_auto_cast<void * const >::cast(das_ref(__context__,__ctx_rename_at_122_74))));
        if ( _FuncbuiltinTickkey_existsTick16808803843923989214_8480a4ada694a4a(__context__,das_arg<TTable<uint64_t,profiler::PerfContext>>::pass(__self_rename_at_122_73.events),__tid_rename_at_126_75) )
        {
            das_invoke_method<void,offsetof(profiler::ProfilerDebugAgent,dump_context)>::invoke<profiler::ProfilerDebugAgent &,Context &,uint64_t>(__context__,nullptr,das_arg<profiler::ProfilerDebugAgent>::pass(__self_rename_at_122_73),das_arg<Context>::pass(__ctx_rename_at_122_74),__tid_rename_at_126_75);
            finalize_76f4fe90df874c6(__context__,das_arg<profiler::PerfContext>::pass(__self_rename_at_122_73.events(__tid_rename_at_126_75,__context__)));
        };
        _FuncbuiltinTickeraseTick5639988512056021548_735549c230142cdc(__context__,das_arg<TTable<uint64_t,void>>::pass(__self_rename_at_122_73.instrumented),((__ctx_rename_at_122_74).getCodeAllocatorId()));
    };
}

inline void _FuncProfilerDebugAgentTickonInstrumentFunctionWithMemory_267dd430d221cc59 ( Context * __context__, profiler::ProfilerDebugAgent &  __self_rename_at_133_76, Context &  __ctx_rename_at_133_77, SimFunction * __fun_rename_at_133_78, bool __entering_rename_at_133_79, uint64_t __userData_rename_at_133_80 )
{
    uint64_t __tid_rename_at_135_81 = ((uint64_t)_FuncbuiltinTickintptrTick11320822648609276562_1714d144c7c46b3a(__context__,das_auto_cast<void * const >::cast(das_ref(__context__,__ctx_rename_at_133_77))));
    profiler::PerfContext * __ev_rename_at_136_82 = &(__self_rename_at_133_76.events(__tid_rename_at_135_81,__context__));
    if ( (*__ev_rename_at_136_82).enabled )
    {
        profiler::PerfEvent _temp_make_local_140_34_3; _temp_make_local_140_34_3;
        TDim<uint64_t,2> __hb_shb_rename_at_138_83;das_zero(__hb_shb_rename_at_138_83);
        heap_stats(das_arg<Context>::pass(__ctx_rename_at_133_77),das_ref(__context__,__hb_shb_rename_at_138_83(0,__context__)));
        _FuncbuiltinTickpushTick10769833213962245646_561ff816281a0367(__context__,das_arg<TArray<profiler::PerfEvent>>::pass((*__ev_rename_at_136_82).events),das_arg<profiler::PerfEvent>::pass((([&]() -> profiler::PerfEvent& {
            das_copy((_temp_make_local_140_34_3.fun),(__fun_rename_at_133_78));
            das_copy((_temp_make_local_140_34_3.entering),(__entering_rename_at_133_79));
            das_copy((_temp_make_local_140_34_3.ts),(get_time_nsec(__self_rename_at_133_76.us0)));
            das_copy((_temp_make_local_140_34_3.heapBytes),(__hb_shb_rename_at_138_83(0,__context__)));
            das_copy((_temp_make_local_140_34_3.stringHeapBytes),(__hb_shb_rename_at_138_83(1,__context__)));
            return _temp_make_local_140_34_3;
        })())));
    };
}

inline void _FuncProfilerDebugAgentTickonInstrumentFunction_9b1fb13d70c13403 ( Context * __context__, profiler::ProfilerDebugAgent &  __self_rename_at_149_84, Context &  __ctx_rename_at_149_85, SimFunction * __fun_rename_at_149_86, bool __entering_rename_at_149_87, uint64_t __userData_rename_at_149_88 )
{
    uint64_t __tid_rename_at_151_89 = ((uint64_t)_FuncbuiltinTickintptrTick11320822648609276562_1714d144c7c46b3a(__context__,das_auto_cast<void * const >::cast(das_ref(__context__,__ctx_rename_at_149_85))));
    profiler::PerfContext * __ev_rename_at_152_90 = &(__self_rename_at_149_84.events(__tid_rename_at_151_89,__context__));
    if ( (*__ev_rename_at_152_90).enabled )
    {
        profiler::PerfEvent _temp_make_local_154_34_4; _temp_make_local_154_34_4;
        _FuncbuiltinTickpushTick10769833213962245646_561ff816281a0367(__context__,das_arg<TArray<profiler::PerfEvent>>::pass((*__ev_rename_at_152_90).events),das_arg<profiler::PerfEvent>::pass((([&]() -> profiler::PerfEvent& {
            das_zero(_temp_make_local_154_34_4);
            das_copy((_temp_make_local_154_34_4.fun),(__fun_rename_at_149_86));
            das_copy((_temp_make_local_154_34_4.entering),(__entering_rename_at_149_87));
            das_copy((_temp_make_local_154_34_4.ts),(get_time_nsec(__self_rename_at_149_84.us0)));
            return _temp_make_local_154_34_4;
        })())));
    };
}

inline void _FuncProfilerDebugAgentTickdump_cc84c3c1cbb56745 ( Context * __context__, profiler::ProfilerDebugAgent &  __self_rename_at_161_91, char * const  __text_rename_at_161_92 )
{
    if ( __self_rename_at_161_91.firstRecord )
    {
        das_copy(__self_rename_at_161_91.firstRecord,false);
    } else {
        das_invoke_method<void,offsetof(profiler::ProfilerDebugAgent,write)>::invoke<profiler::ProfilerDebugAgent &,char * const >(__context__,nullptr,das_arg<profiler::ProfilerDebugAgent>::pass(__self_rename_at_161_91),((char *) ",\n"));
    };
    das_invoke_method<void,offsetof(profiler::ProfilerDebugAgent,write)>::invoke<profiler::ProfilerDebugAgent &,char * const >(__context__,nullptr,das_arg<profiler::ProfilerDebugAgent>::pass(__self_rename_at_161_91),__text_rename_at_161_92);
}

inline void _FuncProfilerDebugAgentTickwrite_73b913f7789c5bd7 ( Context * __context__, profiler::ProfilerDebugAgent &  __self_rename_at_169_93, char * const  __text_rename_at_169_94 )
{
    if ( __self_rename_at_169_93.out != nullptr )
    {
        builtin_fprint(__self_rename_at_169_93.out,__text_rename_at_169_94,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}

inline void _FuncProfilerDebugAgentTickdump_event_5782bd96d61864da ( Context * __context__, profiler::ProfilerDebugAgent &  __self_rename_at_174_95, profiler::PerfEvent const  &  __ev_rename_at_174_96, uint64_t __tid_rename_at_174_97 )
{
    char * __phase_rename_at_175_98 = ((char *)(char *)((__ev_rename_at_174_96.entering ? das_auto_cast<char * const >::cast(((char *) "B")) : das_auto_cast<char * const >::cast(((char *) "E")))));
    char * __fnName_rename_at_176_99 = ((char *)(char *)(((char * const )(builtin_string_escape(__ev_rename_at_174_96.fun->mangledName /*mangledName*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    das_invoke_method<void,offsetof(profiler::ProfilerDebugAgent,dump)>::invoke<profiler::ProfilerDebugAgent &,char *>(__context__,nullptr,das_arg<profiler::ProfilerDebugAgent>::pass(__self_rename_at_174_95),das_string_builder(__context__,SimNode_AotInterop<9>(__tinfo_1, cast<char * const >::from(((char *) "{\"name\":\"")), cast<char * const >::from(__fnName_rename_at_176_99), cast<char * const >::from(((char *) "\",\"cat\": \"PERF\",\"ph\":\"")), cast<char * const >::from(__phase_rename_at_175_98), cast<char * const >::from(((char *) "\",\"pid\": 0,\"tid\":")), cast<int64_t>::from(int64_t(__tid_rename_at_174_97)), cast<char * const >::from(((char *) ",\"ts\":")), cast<int64_t>::from((SimPolicy<int64_t>::Div(__ev_rename_at_174_96.ts,INT64_C(1000),*__context__,nullptr))), cast<char * const >::from(((char *) "}")))));
}

inline void _FuncProfilerDebugAgentTickdump_node_7a5e6d3b88420fc2 ( Context * __context__, profiler::ProfilerDebugAgent &  __self_rename_at_179_100, profiler::PerfNode * const  __node_rename_at_179_101, int32_t __tab_rename_at_179_102 )
{
    if ( __node_rename_at_179_101 == nullptr )
    {
        return ;
    } else {
        Sequence DAS_COMMENT((profiler::PerfNode * *)) _temp_make_local_198_19_5; _temp_make_local_198_19_5;
        if ( __node_rename_at_179_101->fun != nullptr )
        {
            int64_t __total_time_rename_at_184_103 = ((int64_t)das_invoke_method<int64_t,offsetof(profiler::ProfilerDebugAgent,convert_ns_to_unit)>::invoke<profiler::ProfilerDebugAgent &,int64_t,char *>(__context__,nullptr,das_arg<profiler::ProfilerDebugAgent>::pass(__self_rename_at_179_100),__node_rename_at_179_101->total_time,__self_rename_at_179_100.time_unit));
            char * __tabs_rename_at_185_104 = ((char *)(char *)(((char * const )(string_repeat(((char *) "  "),__tab_rename_at_179_102,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
            if ( __self_rename_at_179_100.report_memory )
            {
                Sequence DAS_COMMENT((profiler::PerfNode * *)) _temp_make_local_189_27_6; _temp_make_local_189_27_6;
                uint64_t __hb_rename_at_187_105 = __node_rename_at_179_101->total_heap_bytes;
                uint64_t __shb_rename_at_188_106 = __node_rename_at_179_101->total_string_heap_bytes;
                {
                    bool __need_loop_189 = true;
                    // ch: profiler::PerfNode? const&
                    das_iterator<Sequence DAS_COMMENT((profiler::PerfNode * const ))> __ch_iterator((_temp_make_local_189_27_6 = (_FuncbuiltinTickvaluesTick1935193042646774172_75d79b4b60ddfd81(__context__,__node_rename_at_179_101->children))));
                    profiler::PerfNode * const  * __ch_rename_at_189_107;
                    __need_loop_189 = __ch_iterator.first(__context__,(__ch_rename_at_189_107)) && __need_loop_189;
                    for ( ; __need_loop_189 ; __need_loop_189 = __ch_iterator.next(__context__,(__ch_rename_at_189_107)) )
                    {
                        __hb_rename_at_187_105 -= (*__ch_rename_at_189_107)->total_heap_bytes;
                        __shb_rename_at_188_106 -= (*__ch_rename_at_189_107)->total_string_heap_bytes;
                    }
                    __ch_iterator.close(__context__,(__ch_rename_at_189_107));
                };
                toLog(20000,das_string_builder_temp(__context__,SimNode_AotInterop<12>(__tinfo_2, cast<char * const >::from(__tabs_rename_at_185_104), cast<char * const >::from(__node_rename_at_179_101->fun->mangledName /*mangledName*/), cast<char * const >::from(((char *) " ")), cast<int64_t>::from(int64_t(__node_rename_at_179_101->count)), cast<char * const >::from(((char *) " ")), cast<int64_t>::from(__total_time_rename_at_184_103), cast<char *>::from(__self_rename_at_179_100.time_unit), cast<char * const >::from(((char *) " heap=")), cast<int64_t>::from(int64_t(__hb_rename_at_187_105)), cast<char * const >::from(((char *) " string_heap=")), cast<int64_t>::from(int64_t(__shb_rename_at_188_106)), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            } else {
                toLog(20000,das_string_builder_temp(__context__,SimNode_AotInterop<8>(__tinfo_3, cast<char * const >::from(__tabs_rename_at_185_104), cast<char * const >::from(__node_rename_at_179_101->fun->mangledName /*mangledName*/), cast<char * const >::from(((char *) " ")), cast<int64_t>::from(int64_t(__node_rename_at_179_101->count)), cast<char * const >::from(((char *) " ")), cast<int64_t>::from(__total_time_rename_at_184_103), cast<char *>::from(__self_rename_at_179_100.time_unit), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            };
        };
        {
            bool __need_loop_198 = true;
            // ch: profiler::PerfNode? const&
            das_iterator<Sequence DAS_COMMENT((profiler::PerfNode * const ))> __ch_iterator((_temp_make_local_198_19_5 = (_FuncbuiltinTickvaluesTick1935193042646774172_75d79b4b60ddfd81(__context__,__node_rename_at_179_101->children))));
            profiler::PerfNode * const  * __ch_rename_at_198_108;
            __need_loop_198 = __ch_iterator.first(__context__,(__ch_rename_at_198_108)) && __need_loop_198;
            for ( ; __need_loop_198 ; __need_loop_198 = __ch_iterator.next(__context__,(__ch_rename_at_198_108)) )
            {
                das_invoke_method<void,offsetof(profiler::ProfilerDebugAgent,dump_node)>::invoke<profiler::ProfilerDebugAgent &,profiler::PerfNode * const ,int32_t>(__context__,nullptr,das_arg<profiler::ProfilerDebugAgent>::pass(__self_rename_at_179_100),(*__ch_rename_at_198_108),__tab_rename_at_179_102 + 1);
            }
            __ch_iterator.close(__context__,(__ch_rename_at_198_108));
        };
    };
}

inline void _FuncProfilerDebugAgentTickdump_context_stack_77f3b24d86f88464 ( Context * __context__, profiler::ProfilerDebugAgent &  __self_rename_at_202_109, uint64_t __tid_rename_at_202_110 )
{
    profiler::PerfNode * __root_rename_at_204_111 = das_new<profiler::PerfNode>::make(__context__);
    TArray<profiler::PerfNode *> __stack_rename_at_205_112;das_zero(__stack_rename_at_205_112);
    profiler::PerfNode * __sp_rename_at_206_113 = __root_rename_at_204_111;
    {
        bool __need_loop_207 = true;
        // ev: profiler::PerfEvent&
        das_iterator<TArray<profiler::PerfEvent>> __ev_iterator(__self_rename_at_202_109.events(__tid_rename_at_202_110,__context__).events);
        profiler::PerfEvent * __ev_rename_at_207_114;
        __need_loop_207 = __ev_iterator.first(__context__,(__ev_rename_at_207_114)) && __need_loop_207;
        for ( ; __need_loop_207 ; __need_loop_207 = __ev_iterator.next(__context__,(__ev_rename_at_207_114)) )
        {
            if ( __self_rename_at_202_109.manual && builtin_string_startswith((*__ev_rename_at_207_114).fun->mangledName /*mangledName*/,((char *) "@profiler_boost::"),__context__) )
            {
                continue;
            } else {
                if ( (*__ev_rename_at_207_114).entering )
                {
                    _FuncbuiltinTickpushTick10769833213962245646_de2b37813d4362bf(__context__,das_arg<TArray<profiler::PerfNode *>>::pass(__stack_rename_at_205_112),__sp_rename_at_206_113);
                    if ( !_FuncbuiltinTickkey_existsTick16808803843923989214_59d0b9e50e586e4(__context__,das_arg<TTable<SimFunction *,profiler::PerfNode *>>::pass(__sp_rename_at_206_113->children),(*__ev_rename_at_207_114).fun) )
                    {
                        das_copy(__sp_rename_at_206_113->children((*__ev_rename_at_207_114).fun,__context__),das_ascend<profiler::PerfNode,profiler::PerfNode>::make(__context__,nullptr,(([&]() -> profiler::PerfNode {
                            profiler::PerfNode __mks_214;
                            das_zero(__mks_214);
                            das_copy((__mks_214.fun),((*__ev_rename_at_207_114).fun));
                            return __mks_214;
                        })())));
                    };
                    das_copy(__sp_rename_at_206_113,__sp_rename_at_206_113->children((*__ev_rename_at_207_114).fun,__context__));
                    ++__sp_rename_at_206_113->count;
                    das_copy(__sp_rename_at_206_113->enter_time,(*__ev_rename_at_207_114).ts);
                    das_copy(__sp_rename_at_206_113->enter_heap_bytes,(*__ev_rename_at_207_114).heapBytes);
                    das_copy(__sp_rename_at_206_113->enter_string_heap_bytes,(*__ev_rename_at_207_114).stringHeapBytes);
                } else {
                    __sp_rename_at_206_113->total_time += ((*__ev_rename_at_207_114).ts - __sp_rename_at_206_113->enter_time);
                    __sp_rename_at_206_113->total_heap_bytes += ((*__ev_rename_at_207_114).heapBytes - __sp_rename_at_206_113->enter_heap_bytes);
                    __sp_rename_at_206_113->total_string_heap_bytes += ((*__ev_rename_at_207_114).stringHeapBytes - __sp_rename_at_206_113->enter_string_heap_bytes);
                    das_copy(__sp_rename_at_206_113,(builtin_array_size(das_arg<TArray<profiler::PerfNode *>>::pass(__stack_rename_at_205_112)) != 0) ? das_auto_cast_ref<profiler::PerfNode * &>::cast(_FuncbuiltinTickbackTick18296309835877697278_7d600bb6b963e354(__context__,das_arg<TArray<profiler::PerfNode *>>::pass(__stack_rename_at_205_112))) : das_auto_cast_ref<profiler::PerfNode * &>::cast(__root_rename_at_204_111));
                    if ( builtin_array_size(das_arg<TArray<profiler::PerfNode *>>::pass(__stack_rename_at_205_112)) > 0 )
                    {
                        _FuncbuiltinTickpopTick1161079256290593740_e8064abe124c4e76(__context__,das_arg<TArray<profiler::PerfNode *>>::pass(__stack_rename_at_205_112));
                    };
                };
            };
        }
        __ev_iterator.close(__context__,(__ev_rename_at_207_114));
    };
    das_invoke_method<void,offsetof(profiler::ProfilerDebugAgent,dump_node)>::invoke<profiler::ProfilerDebugAgent &,profiler::PerfNode *,int32_t>(__context__,nullptr,das_arg<profiler::ProfilerDebugAgent>::pass(__self_rename_at_202_109),__root_rename_at_204_111,-1);
    finalize_898c8b885e54e57e(__context__,__root_rename_at_204_111);
}

inline void _FuncProfilerDebugAgentTickdump_meta_ee131d376f01d903 ( Context * __context__, profiler::ProfilerDebugAgent &  __self_rename_at_236_115, Context &  __ctx_rename_at_236_116, uint64_t __tid_rename_at_236_117 )
{
    char * __ctxName_rename_at_237_118 = ((char *)(char *)((builtin_empty_das_string(das_arg<das::string>::pass(__ctx_rename_at_236_116.name /*name*/)) ? das_auto_cast<char * const >::cast(nullptr) : das_auto_cast<char * const >::cast(das_string_builder(__context__,SimNode_AotInterop<3>(__tinfo_4, cast<char * const >::from(((char *) " '")), cast<das::string &>::from(__ctx_rename_at_236_116.name /*name*/), cast<char * const >::from(((char *) "'"))))))));
    bool __wasDead_rename_at_238_119 = ((bool)((__ctx_rename_at_236_116.category /*category*/ & 0x1u) == 0x1u));
    __ctx_rename_at_236_116.category /*category*/ &= 0xfffffffeu;
    char * __ctxDesc_rename_at_240_120 = ((char *)(char *)(((char * const )(builtin_string_escape(das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_5, cast<uint64_t>::from(__tid_rename_at_236_117), cast<char * const >::from(__ctxName_rename_at_237_118), cast<char * const >::from(((char *) " cat:")), cast<Bitfield>::from(__ctx_rename_at_236_116.category /*category*/))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    das_invoke_method<void,offsetof(profiler::ProfilerDebugAgent,dump)>::invoke<profiler::ProfilerDebugAgent &,char *>(__context__,nullptr,das_arg<profiler::ProfilerDebugAgent>::pass(__self_rename_at_236_115),das_string_builder(__context__,SimNode_AotInterop<5>(__tinfo_6, cast<char * const >::from(((char *) "{\"args\": { \"name\": \"")), cast<char * const >::from(__ctxDesc_rename_at_240_120), cast<char * const >::from(((char *) "\" }, \"name\": \"thread_name\", \"cat\": \"__metadata\", \"ph\": \"M\", \"pid\": 0, \"tid\": ")), cast<int64_t>::from(int64_t(__tid_rename_at_236_117)), cast<char * const >::from(((char *) ", \"ts\": 0 }")))));
    if ( __wasDead_rename_at_238_119 )
    {
        __ctx_rename_at_236_116.category /*category*/ |= 0x1u;
    };
}

inline void _FuncProfilerDebugAgentTickdump_events_b4b8dcaa6b59cfc6 ( Context * __context__, profiler::ProfilerDebugAgent &  __self_rename_at_245_121, uint64_t __tid_rename_at_245_122 )
{
    {
        bool __need_loop_246 = true;
        // ev: profiler::PerfEvent&
        das_iterator<TArray<profiler::PerfEvent>> __ev_iterator(__self_rename_at_245_121.events(__tid_rename_at_245_122,__context__).events);
        profiler::PerfEvent * __ev_rename_at_246_123;
        __need_loop_246 = __ev_iterator.first(__context__,(__ev_rename_at_246_123)) && __need_loop_246;
        for ( ; __need_loop_246 ; __need_loop_246 = __ev_iterator.next(__context__,(__ev_rename_at_246_123)) )
        {
            das_invoke_method<void,offsetof(profiler::ProfilerDebugAgent,dump_event)>::invoke<profiler::ProfilerDebugAgent &,profiler::PerfEvent &,uint64_t>(__context__,nullptr,das_arg<profiler::ProfilerDebugAgent>::pass(__self_rename_at_245_121),das_arg<profiler::PerfEvent>::pass((*__ev_rename_at_246_123)),__tid_rename_at_245_122);
        }
        __ev_iterator.close(__context__,(__ev_rename_at_246_123));
    };
}

inline void _FuncProfilerDebugAgentTickdump_context_31251310253bebb1 ( Context * __context__, profiler::ProfilerDebugAgent &  __self_rename_at_250_124, Context &  __ctx_rename_at_250_125, uint64_t __tid_rename_at_250_126 )
{
    das_invoke_method<void,offsetof(profiler::ProfilerDebugAgent,dump_context_stack)>::invoke<profiler::ProfilerDebugAgent &,uint64_t>(__context__,nullptr,das_arg<profiler::ProfilerDebugAgent>::pass(__self_rename_at_250_124),__tid_rename_at_250_126);
    das_invoke_method<void,offsetof(profiler::ProfilerDebugAgent,dump_meta)>::invoke<profiler::ProfilerDebugAgent &,Context &,uint64_t>(__context__,nullptr,das_arg<profiler::ProfilerDebugAgent>::pass(__self_rename_at_250_124),das_arg<Context>::pass(__ctx_rename_at_250_125),__tid_rename_at_250_126);
    das_invoke_method<void,offsetof(profiler::ProfilerDebugAgent,dump_events)>::invoke<profiler::ProfilerDebugAgent &,uint64_t>(__context__,nullptr,das_arg<profiler::ProfilerDebugAgent>::pass(__self_rename_at_250_124),__tid_rename_at_250_126);
    _FuncbuiltinTickfinalizeTick13836114024949725080_f33ad2da0da0fc40(__context__,das_arg<TArray<profiler::PerfEvent>>::pass(__self_rename_at_250_124.events(__tid_rename_at_250_126,__context__).events));
    _FuncbuiltinTickeraseTick5639988512056021548_e7e3b7c59d77dc1b(__context__,das_arg<TTable<uint64_t,profiler::PerfContext>>::pass(__self_rename_at_250_124.events),__tid_rename_at_250_126);
}

inline void _FuncProfilerDebugAgent_0x27___finalize_1e7e1b45f5a9fe62 ( Context * __context__, profiler::ProfilerDebugAgent &  __self_rename_at_51_127 )
{
    finalize_5f2b6f1cedc818a3(__context__,das_arg<profiler::ProfilerDebugAgent>::pass(__self_rename_at_51_127));
}

inline void debug_agent_d264bf1ee1e8402e ( Context * __context__, Context const  &  __ctx_rename_at_261_128 )
{
    DAS_ASSERT((das_get_bitfield(thisContext(__context__).category /*category*/,1u << 1)));
    _FuncdebugapiTickinstall_new_thread_local_debug_agentTick15124441463860820031_53778b6d1c803b14(__context__,das_new<profiler::ProfilerDebugAgent>::make_and_init(__context__,[&]() { return ProfilerDebugAgent_5839a39d4ce45680(__context__); }));
}

inline profiler::ProfilerDebugAgent ProfilerDebugAgent_5839a39d4ce45680 ( Context * __context__ )
{
    TDim<AutoTuple<char *,int64_t>,4> _temp_make_local_59_45_7; _temp_make_local_59_45_7;
    return /* <- */ das_auto_cast_move<profiler::ProfilerDebugAgent>::cast((([&]() -> profiler::ProfilerDebugAgent {
        profiler::ProfilerDebugAgent __mks_51;
        das_zero(__mks_51);
        das_copy((__mks_51.__rtti),(((void *)(&__type_info__216e8f5cf53cc8a6))));
        das_copy((__mks_51.__finalize),(das_cast<Func DAS_COMMENT((void,debugapi::DapiDebugAgent))>::cast(Func(__context__->fnByMangledName(/*@profiler::ProfilerDebugAgent'__finalize S<profiler::ProfilerDebugAgent>*/ 0xf09ee7130a694ec2)))));
        das_copy((__mks_51.onInstall),(das_cast<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,DebugAgent * const ))>::cast(Func(__context__->fnByMangledName(/*@profiler::ProfilerDebugAgent`onInstall S<profiler::ProfilerDebugAgent> C1<H<debugapi::DebugAgent>>?*/ 0x39a91423d92da437)))));
        das_copy((__mks_51.onUninstall),(das_cast<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,DebugAgent * const ))>::cast(Func(__context__->fnByMangledName(/*@profiler::ProfilerDebugAgent`onUninstall S<profiler::ProfilerDebugAgent> C1<H<debugapi::DebugAgent>>?*/ 0x636ff0e8b7bf5db8)))));
        das_copy((__mks_51.onCreateContext),(das_cast<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context))>::cast(Func(__context__->fnByMangledName(/*@profiler::ProfilerDebugAgent`onCreateContext S<profiler::ProfilerDebugAgent> H<rtti::Context>*/ 0xaa7ffa2efe654cc5)))));
        das_copy((__mks_51.onDestroyContext),(das_cast<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context))>::cast(Func(__context__->fnByMangledName(/*@profiler::ProfilerDebugAgent`onDestroyContext S<profiler::ProfilerDebugAgent> H<rtti::Context>*/ 0x3fa80de30376a415)))));
        das_copy((__mks_51.onInstrumentFunction),(das_cast<Func DAS_COMMENT((void,debugapi::DapiDebugAgent,Context,SimFunction * const ,bool,uint64_t))>::cast(Func(__context__->fnByMangledName(/*@profiler::ProfilerDebugAgent`onInstrumentFunction S<profiler::ProfilerDebugAgent> H<rtti::Context> 1<H<rtti::SimFunction>>? Cb Cu64*/ 0x9c564c07fa802db3)))));
        das_copy((__mks_51.us0),(INT64_C(0)));
        das_copy((__mks_51.firstRecord),(true));
        das_copy((__mks_51.manual),(false));
        das_copy((__mks_51.report_memory),(false));
        das_move((__mks_51.map_time_units),(_FuncbuiltinTickto_table_moveTick5858896087460481804_7ed588f7d03f97e5(__context__,das_arg<TDim<AutoTuple<char *,int64_t>,4>>::pass((([&]() -> TDim<AutoTuple<char *,int64_t>,4>& {
            _temp_make_local_59_45_7(0,__context__) = (([&]() -> AutoTuple<char *,int64_t> {
                AutoTuple<char *,int64_t> __mkt_59;
                das_get_auto_tuple_field<char *,0,char *,int64_t>::get(__mkt_59) = ((char *) "ns");
                das_get_auto_tuple_field<int64_t,1,char *,int64_t>::get(__mkt_59) = INT64_C(1);
                return __mkt_59;
            })());
            _temp_make_local_59_45_7(1,__context__) = (([&]() -> AutoTuple<char *,int64_t> {
                AutoTuple<char *,int64_t> __mkt_59;
                das_get_auto_tuple_field<char *,0,char *,int64_t>::get(__mkt_59) = ((char *) "us");
                das_get_auto_tuple_field<int64_t,1,char *,int64_t>::get(__mkt_59) = INT64_C(1000);
                return __mkt_59;
            })());
            _temp_make_local_59_45_7(2,__context__) = (([&]() -> AutoTuple<char *,int64_t> {
                AutoTuple<char *,int64_t> __mkt_59;
                das_get_auto_tuple_field<char *,0,char *,int64_t>::get(__mkt_59) = ((char *) "ms");
                das_get_auto_tuple_field<int64_t,1,char *,int64_t>::get(__mkt_59) = INT64_C(1000000);
                return __mkt_59;
            })());
            _temp_make_local_59_45_7(3,__context__) = (([&]() -> AutoTuple<char *,int64_t> {
                AutoTuple<char *,int64_t> __mkt_59;
                das_get_auto_tuple_field<char *,0,char *,int64_t>::get(__mkt_59) = ((char *) "s");
                das_get_auto_tuple_field<int64_t,1,char *,int64_t>::get(__mkt_59) = INT64_C(1000000000);
                return __mkt_59;
            })());
            return _temp_make_local_59_45_7;
        })())))));
        das_copy((__mks_51.time_unit),(((char *) "ns")));
        das_copy((__mks_51.dependency),(Func(__context__->fnByMangledName(/*@profiler::ProfilerDebugAgent`dependency S<profiler::ProfilerDebugAgent>*/ 0xec69536cf8555d37))));
        das_copy((__mks_51.is_time_unit_correct),(Func(__context__->fnByMangledName(/*@profiler::ProfilerDebugAgent`is_time_unit_correct S<profiler::ProfilerDebugAgent> Cs*/ 0x3b9a63160073f617))));
        das_copy((__mks_51.convert_ns_to_unit),(Func(__context__->fnByMangledName(/*@profiler::ProfilerDebugAgent`convert_ns_to_unit S<profiler::ProfilerDebugAgent> Ci64 Cs*/ 0xa5315b19f725f395))));
        das_copy((__mks_51.isProfileable),(Func(__context__->fnByMangledName(/*@profiler::ProfilerDebugAgent`isProfileable S<profiler::ProfilerDebugAgent> H<rtti::Context>*/ 0xc27ca255a42c3b91))));
        das_copy((__mks_51.enable_profiler),(Func(__context__->fnByMangledName(/*@profiler::ProfilerDebugAgent`enable_profiler S<profiler::ProfilerDebugAgent> Cu64 Cb*/ 0xf9e0a3a350220fd0))));
        das_copy((__mks_51.onInstrumentFunctionWithMemory),(Func(__context__->fnByMangledName(/*@profiler::ProfilerDebugAgent`onInstrumentFunctionWithMemory S<profiler::ProfilerDebugAgent> H<rtti::Context> 1<H<rtti::SimFunction>>? Cb Cu64*/ 0xad5eebba40785914))));
        das_copy((__mks_51.dump),(Func(__context__->fnByMangledName(/*@profiler::ProfilerDebugAgent`dump S<profiler::ProfilerDebugAgent> Cs*/ 0x800cbe3c04e9e4a1))));
        das_copy((__mks_51.write),(Func(__context__->fnByMangledName(/*@profiler::ProfilerDebugAgent`write S<profiler::ProfilerDebugAgent> Cs*/ 0x4be9753ec91f8f10))));
        das_copy((__mks_51.dump_event),(Func(__context__->fnByMangledName(/*@profiler::ProfilerDebugAgent`dump_event S<profiler::ProfilerDebugAgent> CS<profiler::PerfEvent> Cu64*/ 0xd9e69d3ecb7a5c6e))));
        das_copy((__mks_51.dump_node),(Func(__context__->fnByMangledName(/*@profiler::ProfilerDebugAgent`dump_node S<profiler::ProfilerDebugAgent> C1<S<profiler::PerfNode>>? Ci*/ 0x76e5a372a94353c9))));
        das_copy((__mks_51.dump_context_stack),(Func(__context__->fnByMangledName(/*@profiler::ProfilerDebugAgent`dump_context_stack S<profiler::ProfilerDebugAgent> Cu64*/ 0x3d15cdb2a17919eb))));
        das_copy((__mks_51.dump_meta),(Func(__context__->fnByMangledName(/*@profiler::ProfilerDebugAgent`dump_meta S<profiler::ProfilerDebugAgent> H<rtti::Context> Cu64*/ 0xc801da724efb043f))));
        das_copy((__mks_51.dump_events),(Func(__context__->fnByMangledName(/*@profiler::ProfilerDebugAgent`dump_events S<profiler::ProfilerDebugAgent> Cu64*/ 0xafce747c04dc4e))));
        das_copy((__mks_51.dump_context),(Func(__context__->fnByMangledName(/*@profiler::ProfilerDebugAgent`dump_context S<profiler::ProfilerDebugAgent> H<rtti::Context> Cu64*/ 0xa3057690bae3c56d))));
        return __mks_51;
    })()));
}

inline TTable<char *,int64_t> _FuncbuiltinTickto_table_moveTick5858896087460481804_7ed588f7d03f97e5 ( Context * __context__, TDim<AutoTuple<char *,int64_t>,4> &  __a_rename_at_1507_129 )
{
    TTable<char *,int64_t> __tab_rename_at_1508_130;das_zero(__tab_rename_at_1508_130);
    {
        bool __need_loop_1510 = true;
        // x: tuple<string aka keyT;int64 aka valT>&
        das_iterator<TDim<AutoTuple<char *,int64_t>,4>> __x_iterator(__a_rename_at_1507_129);
        AutoTuple<char *,int64_t> * __x_rename_at_1510_131;
        __need_loop_1510 = __x_iterator.first(__context__,(__x_rename_at_1510_131)) && __need_loop_1510;
        for ( ; __need_loop_1510 ; __need_loop_1510 = __x_iterator.next(__context__,(__x_rename_at_1510_131)) )
        {
            das_copy(__tab_rename_at_1508_130(das_get_auto_tuple_field<char *,0,char *,int64_t>::get((*__x_rename_at_1510_131)),__context__),das_get_auto_tuple_field<int64_t,1,char *,int64_t>::get((*__x_rename_at_1510_131)));
        }
        __x_iterator.close(__context__,(__x_rename_at_1510_131));
    };
    return /* <- */ das_auto_cast_move<TTable<char *,int64_t>>::cast(__tab_rename_at_1508_130);
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x26b7ff8eb779afd4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1351216622833168869_a4d809f70d013740>>();
    };
    aotLib[0x74a7e4f5fdc9d152] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1351216622833168869_58e95cad5e33a23b>>();
    };
    aotLib[0xce1e3f6c002decd4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_7ec6032b9dc0345b>>();
    };
    aotLib[0x2aeef8bda3366b70] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_23175f5c66395481>>();
    };
    aotLib[0x137609e95e5b15ae] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick5454204887383796109_b84e8f61cdd6cfbe>>();
    };
    aotLib[0x613f84ca1b867b48] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick5454204887383796109_a90a12b574f1d50e>>();
    };
    aotLib[0x6bb3666275da6ba7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick5454204887383796109_13a27acb0aa845f7>>();
    };
    aotLib[0xef2024831f12e76d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick5454204887383796109_1b617b3dea3b44eb>>();
    };
    aotLib[0x3582b38cfaa84522] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_f33ad2da0da0fc40>>();
    };
    aotLib[0x589c456b9f166464] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_509e2dba1d78f7f3>>();
    };
    aotLib[0x4ac439f4de45dd8e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_d5d88071a1a2b702>>();
    };
    aotLib[0xdf2e58009a9c5f22] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickkey_existsTick16808803843923989214_9a4a041519f183d5>>();
    };
    aotLib[0x4e2546db7b491ae9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickkey_existsTick16808803843923989214_fb9a18d52170edb4>>();
    };
    aotLib[0x5b1c35d0be68057] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickinsertTick10959621454228962049_297e40a85937efcc>>();
    };
    aotLib[0xb94fbde00d051cc8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickkey_existsTick16808803843923989214_8480a4ada694a4a>>();
    };
    aotLib[0xf80c2fea4ee59fb2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_76f4fe90df874c6>>();
    };
    aotLib[0xddb02825891e19a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickeraseTick5639988512056021548_735549c230142cdc>>();
    };
    aotLib[0xc3bbd68ff657de89] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_561ff816281a0367>>();
    };
    aotLib[0x43cbe980eebb3f47] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickkey_existsTick16808803843923989214_59d0b9e50e586e4>>();
    };
    aotLib[0xe855ffb163baadf6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickeraseTick5639988512056021548_e7e3b7c59d77dc1b>>();
    };
    aotLib[0x5505d1b16b74a35b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickget_command_line_argumentsTick14327939727965569528_f662626f31496e07>>();
    };
    aotLib[0xa6f3495ed1b6a629] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickintptrTick11320822648609276562_1714d144c7c46b3a>>();
    };
    aotLib[0x2b7f511bfbd7c4de] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1935193042646774172_75d79b4b60ddfd81>>();
    };
    aotLib[0x831429e9c35554f1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_de2b37813d4362bf>>();
    };
    aotLib[0x5032995e9d8b6174] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickbackTick18296309835877697278_7d600bb6b963e354>>();
    };
    aotLib[0xa3174e3473d1b04b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpopTick1161079256290593740_e8064abe124c4e76>>();
    };
    aotLib[0x7fcb1d2cc506508d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_898c8b885e54e57e>>();
    };
    aotLib[0xf05a4ac03f3ffa3e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_5f2b6f1cedc818a3>>();
    };
    aotLib[0x1e1b2f553abc4b6b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncdebugapiTickinstall_new_thread_local_debug_agentTick15124441463860820031_53778b6d1c803b14>>();
    };
    aotLib[0xab514ac20b7b242] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&set_enable_profiler_124bae90f2713c60>>();
    };
    aotLib[0xea71208569a13784] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncProfilerDebugAgentTickdependency_c5b8c1449dcf4ec2>>();
    };
    aotLib[0x430ead7ac6249d33] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncProfilerDebugAgentTickis_time_unit_correct_e5df0855959b48d6>>();
    };
    aotLib[0x7a629de0a1810e82] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncProfilerDebugAgentTickconvert_ns_to_unit_e8b63a47d67ad5c9>>();
    };
    aotLib[0xbc31cc3796c51f40] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncProfilerDebugAgentTickonInstall_f555c07af16faf7b>>();
    };
    aotLib[0x26f26e549b7a2886] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncProfilerDebugAgentTickonUninstall_298304d79d058498>>();
    };
    aotLib[0xa0a1cc234afba017] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncProfilerDebugAgentTickisProfileable_26822e4e90fe1652>>();
    };
    aotLib[0xda4145f5a608328c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncProfilerDebugAgentTickonCreateContext_d8f358d68c58d9ef>>();
    };
    aotLib[0xede767712c1a847b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncProfilerDebugAgentTickenable_profiler_979c874d994314a0>>();
    };
    aotLib[0x5ae970cf466a9aa5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncProfilerDebugAgentTickonDestroyContext_7df236229babeb37>>();
    };
    aotLib[0x51d9600c3edcca5b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncProfilerDebugAgentTickonInstrumentFunctionWithMemory_267dd430d221cc59>>();
    };
    aotLib[0x9e8be2e2042cd430] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncProfilerDebugAgentTickonInstrumentFunction_9b1fb13d70c13403>>();
    };
    aotLib[0xf154a39bbfb45bbd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncProfilerDebugAgentTickdump_cc84c3c1cbb56745>>();
    };
    aotLib[0xc14d5bb0435e1a99] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncProfilerDebugAgentTickwrite_73b913f7789c5bd7>>();
    };
    aotLib[0xee811e7034a47821] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncProfilerDebugAgentTickdump_event_5782bd96d61864da>>();
    };
    aotLib[0x3732ea2a0e01e84b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncProfilerDebugAgentTickdump_node_7a5e6d3b88420fc2>>();
    };
    aotLib[0xd0bb56c51bdb333f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncProfilerDebugAgentTickdump_context_stack_77f3b24d86f88464>>();
    };
    aotLib[0xed01fa6493c43875] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncProfilerDebugAgentTickdump_meta_ee131d376f01d903>>();
    };
    aotLib[0x21d6d7a5c8f61ad] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncProfilerDebugAgentTickdump_events_b4b8dcaa6b59cfc6>>();
    };
    aotLib[0x47e0439e68300090] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncProfilerDebugAgentTickdump_context_31251310253bebb1>>();
    };
    aotLib[0xefcab0265b5d9060] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncProfilerDebugAgent_0x27___finalize_1e7e1b45f5a9fe62>>();
    };
    aotLib[0xcea977cc8f8bd6fb] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&debug_agent_d264bf1ee1e8402e>>();
    };
    aotLib[0x7f9faae19d3a61d1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&ProfilerDebugAgent_5839a39d4ce45680>>();
    };
    aotLib[0xab24380bf3f68207] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickto_table_moveTick5858896087460481804_7ed588f7d03f97e5>>();
    };
    // [[ init script ]]
    aotLib[0x6e0e597b34278779] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<AutoSimNode_Aot<&__init_script>>();
        return ctx.aotInitScript;
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_14082698819804048119
AotListBase impl_aot_profiler(_anon_14082698819804048119::registerAotFunctions);
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
