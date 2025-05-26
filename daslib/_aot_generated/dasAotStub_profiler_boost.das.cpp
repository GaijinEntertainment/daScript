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
 // require math
#include "daScript/simulate/aot_builtin_math.h"
 // require profiler
 // require strings_boost
 // require ast_boost
 // require templates
 // require contracts
 // require array_boost
 // require algorithm
 // require templates_boost
 // require macro_boost
 // require defer
 // require apply_in_context
 // require profiler_boost

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
namespace _anon_122503766534862016 {

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
namespace profiler { struct PerfNode; };
namespace profiler { struct PerfEvent; };
namespace profiler { struct PerfContext; };
namespace profiler { struct ProfilerDebugAgent; };
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
namespace apply_in_context { struct AppendCondAnnotation; };
// unused enumeration CompilationError
// unused enumeration ConstMatters
// unused enumeration RefMatters
// unused enumeration TemporaryMatters
// unused enumeration Type
// unused structure DapiDebugAgent
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
// unused structure PerfNode
// unused structure PerfEvent
// unused structure PerfContext
// unused structure ProfilerDebugAgent
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
// unused structure AppendCondAnnotation
extern TypeInfo __type_info__3bdf8339ef37753;
extern TypeInfo __type_info__af81fe4c86352052;
extern TypeInfo __type_info__d6eadf66eea309ce;
extern TypeInfo __type_info__d9307e078cfb0f0c;
extern TypeInfo __type_info__af63eb4c86020609;
extern FuncInfo __func_info__955909f57bd61177;

FuncInfo __func_info__955909f57bd61177 = {"invoke block<void> const", "", nullptr, 0, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x955909f57bd61177), 0x0 };
TypeInfo __type_info__3bdf8339ef37753 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~rtti::Context"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24834, 576, UINT64_C(0x3bdf8339ef37753) };
TypeInfo __type_info__af81fe4c86352052 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 1, UINT64_C(0xaf81fe4c86352052) };
TypeInfo * __type_info__d6eadf66eea309ce_arg_types[2] = { &__type_info__d9307e078cfb0f0c, &__type_info__af81fe4c86352052 };
const char * __type_info__d6eadf66eea309ce_arg_names[2] = { "ctxId", "enabled" };
TypeInfo __type_info__d6eadf66eea309ce = { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__d6eadf66eea309ce_arg_types, __type_info__d6eadf66eea309ce_arg_names, 2, 0, nullptr, 12, 8, UINT64_C(0xd6eadf66eea309ce) };
TypeInfo __type_info__d9307e078cfb0f0c = { Type::tUInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 8, UINT64_C(0xd9307e078cfb0f0c) };
TypeInfo __type_info__af63eb4c86020609 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 0, UINT64_C(0xaf63eb4c86020609) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {__type_info__3bdf8339ef37753, };
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[4] = { &__type_info__3bdf8339ef37753, &__type_info__d6eadf66eea309ce, &__type_info__d9307e078cfb0f0c, &__type_info__af81fe4c86352052 };

inline void _FuncCONTEXTTickinternal_enable_profiler_9259ac347efac90d ( Context * __context__, uint64_t __ctxId_rename_at_90_0, bool __enabled_rename_at_90_1 );
inline void _FuncCONTEXT_CLONETickinternal_enable_profiler_efa02bb71ea20bcd ( Context * __context__, uint64_t __ctxId_rename_at_14_2, bool __enabled_rename_at_14_3 );
inline uint64_t _FuncbuiltinTickintptrTick11320822648609276562_814d48688d3fb11d ( Context * __context__, void * const  __p_rename_at_15_4 );
inline void internal_enable_profiler_49af0bcaf84865fa ( Context * __context__, uint64_t __ctxId_rename_at_14_5, bool __enabled_rename_at_14_6 );
inline void enable_profiler_f1a7d4ae3b806df7 ( Context * __context__, Context & __ctx_rename_at_22_7 );
inline void disable_profiler_32fa37bfb0ad4434 ( Context * __context__, Context & __ctx_rename_at_26_8 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void _FuncCONTEXTTickinternal_enable_profiler_9259ac347efac90d ( Context * __context__, uint64_t __ctxId_rename_at_90_0, bool __enabled_rename_at_90_1 )
{
    DAS_VERIFYF((hasDebugAgentContext(((char *) "profiler"),((LineInfoArg *)(&LineInfo::g_LineInfoNULL)),__context__)),(((char *) "debug agent is not installed")));
    DAS_VERIFYF((das_ref(__context__,getDebugAgentContext(((char *) "profiler"),((LineInfoArg *)(&LineInfo::g_LineInfoNULL)),__context__)) == das_ref(__context__,thisContext(__context__))),(((char *) "agent context mismatch")));
    _FuncCONTEXT_CLONETickinternal_enable_profiler_efa02bb71ea20bcd(__context__,__ctxId_rename_at_90_0,__enabled_rename_at_90_1);
}

inline void _FuncCONTEXT_CLONETickinternal_enable_profiler_efa02bb71ea20bcd ( Context * __context__, uint64_t __ctxId_rename_at_14_2, bool __enabled_rename_at_14_3 ) { das_stack_prologue __prologue(__context__,80,"CONTEXT_CLONE`internal_enable_profiler " DAS_FILE_LINE);
{
    lockDebugAgent(das_make_block<void>(__context__,0,0,&__func_info__955909f57bd61177,[&]() -> void{
        das_invoke_function<void>::invoke<uint64_t,bool>(__context__,nullptr,Func(__context__->fnByMangledName(/*@profiler::set_enable_profiler u64 Cb*/ 0x141fa5e0c4b2e16b)),__ctxId_rename_at_14_2,__enabled_rename_at_14_3);
    }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}}

inline uint64_t _FuncbuiltinTickintptrTick11320822648609276562_814d48688d3fb11d ( Context * __context__, void * const  __p_rename_at_15_4 )
{
    return das_auto_cast<uint64_t>::cast(das_cast<uint64_t>::cast(__p_rename_at_15_4));
}

inline void internal_enable_profiler_49af0bcaf84865fa ( Context * __context__, uint64_t __ctxId_rename_at_14_5, bool __enabled_rename_at_14_6 )
{
    DAS_VERIFYF((hasDebugAgentContext(((char *) "profiler"),((LineInfoArg *)(&LineInfo::g_LineInfoNULL)),__context__)),(((char *) "debug agent is not installed")));
    DAS_VERIFYF((das_ref(__context__,getDebugAgentContext(((char *) "profiler"),((LineInfoArg *)(&LineInfo::g_LineInfoNULL)),__context__)) != das_ref(__context__,thisContext(__context__))),(((char *) "agent context mismatch")));
    das_call_interop<void>::call(&pinvoke_impl2,__context__,SimNode_AotInterop<4>(__tinfo_0,cast<Context &>::from(getDebugAgentContext(((char *) "profiler"),((LineInfoArg *)(&LineInfo::g_LineInfoNULL)),__context__)),cast<Func DAS_COMMENT((void,uint64_t,bool))>::from(Func(__context__->fnByMangledName(/*@profiler_boost::CONTEXT`internal_enable_profiler u64 Cb*/ 0xa0d31a805c19abf5))),cast<uint64_t>::from(__ctxId_rename_at_14_5),cast<bool>::from(__enabled_rename_at_14_6)));
}

inline void enable_profiler_f1a7d4ae3b806df7 ( Context * __context__, Context &  __ctx_rename_at_22_7 )
{
    internal_enable_profiler_49af0bcaf84865fa(__context__,_FuncbuiltinTickintptrTick11320822648609276562_814d48688d3fb11d(__context__,das_auto_cast<void * const >::cast(das_ref(__context__,__ctx_rename_at_22_7))),true);
}

inline void disable_profiler_32fa37bfb0ad4434 ( Context * __context__, Context &  __ctx_rename_at_26_8 )
{
    internal_enable_profiler_49af0bcaf84865fa(__context__,_FuncbuiltinTickintptrTick11320822648609276562_814d48688d3fb11d(__context__,das_auto_cast<void * const >::cast(das_ref(__context__,__ctx_rename_at_26_8))),false);
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0xfca7001704070a79] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncCONTEXTTickinternal_enable_profiler_9259ac347efac90d>>();
    };
    aotLib[0xe9eeed31e8090051] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncCONTEXT_CLONETickinternal_enable_profiler_efa02bb71ea20bcd>>();
    };
    aotLib[0x4577f8df0b21841b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickintptrTick11320822648609276562_814d48688d3fb11d>>();
    };
    aotLib[0x6bf987a594e1454a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&internal_enable_profiler_49af0bcaf84865fa>>();
    };
    aotLib[0xae293b0b849a6913] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&enable_profiler_f1a7d4ae3b806df7>>();
    };
    aotLib[0x92e9ad71dd13f815] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&disable_profiler_32fa37bfb0ad4434>>();
    };
    // [[ init script ]]
    aotLib[0x6e0e597b34278779] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<AutoSimNode_Aot<&__init_script>>();
        return ctx.aotInitScript;
    };
	resolveTypeInfoAnnotations();
}

} // namespace _anon_122503766534862016
AotListBase impl_aot_profiler_boost(_anon_122503766534862016::registerAotFunctions);
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
