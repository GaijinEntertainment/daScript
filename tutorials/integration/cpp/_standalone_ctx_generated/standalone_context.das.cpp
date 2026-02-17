#include "daScript/simulate/standalone_ctx_utils.h"
 // require builtin
#include "daScript/simulate/bin_serializer.h"
#include "daScript/simulate/runtime_profile.h"
#include "standalone_context.das.h"

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
namespace _anon_3883122988366833237 {

extern TypeInfo __type_info__af90fe4c864e9d52;
extern TypeInfo __type_info__af63e44c8601fa24;
extern TypeInfo __type_info__af63eb4c86020609;

FuncInfo __func_info__7f4e3d0819c1de70 = {"test", "", nullptr, 0, 64, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x7f4e3d0819c1de70), 0x0 };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, 8, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__af63e44c8601fa24 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xaf63e44c8601fa24) };
TypeInfo __type_info__af63eb4c86020609 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 0, UINT64_C(0xaf63eb4c86020609) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24, &__type_info__af90fe4c864e9d52 };

inline void test_40a425e56ca28455 ( Context * __context__ );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void test_40a425e56ca28455 ( Context * __context__ )
{
    int32_t __total_rename_at_9_0 = 0;
    {
        bool __need_loop_10 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,10));
        int32_t __i_rename_at_10_1;
        __need_loop_10 = __i_iterator.first(__context__,(__i_rename_at_10_1)) && __need_loop_10;
        for ( ; __need_loop_10 ; __need_loop_10 = __i_iterator.next(__context__,(__i_rename_at_10_1)) )
        {
            __total_rename_at_9_0 += __i_rename_at_10_1;
        }
        __i_iterator.close(__context__,(__i_rename_at_10_1));
    };
    builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_0, cast<char * const >::from(((char *) "Sum of 0..9 = ")), cast<int32_t>::from(__total_rename_at_9_0), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}
} // namespace _anon_3883122988366833237
using namespace _anon_3883122988366833237;
namespace standalone_context {
static vec4f __wrap_test_40a425e56ca28455 ( Context * __context__ ) {
    test_40a425e56ca28455(__context__);
    return v_zero();
}

#pragma optimize("", off)
struct AotFunction { uint64_t hash; bool is_cmres; void * fn; vec4f (*wrappedFn)(Context*); };
static AotFunction functions[] = {
    { 0x580c190f73ac3d2, false, (void*)&test_40a425e56ca28455, &__wrap_test_40a425e56ca28455 },
};
#pragma optimize("", on)

static void registerAotFunctions ( AotLibrary & aotLib ) {
    for (const auto &[hash, cmres, fn1, fn2] : functions) {
        aotLib.emplace(hash, AotFactory(cmres, fn1, fn2));
    }
    resolveTypeInfoAnnotations();
}

static AotListBase impl(registerAotFunctions);
auto Standalone::test ( ) -> void {
    return test_40a425e56ca28455(this);
}

Standalone::Standalone() {
    auto & context = *this;
    CodeOfPolicies policies;    policies.debugger = false /*policies.debugger*/;
    policies.persistent_heap = false;
    policies.heap_size_hint = 65536;
    policies.string_heap_size_hint = 65536;
    context.setup(0/*totalVariables*/, 20 /*globalStringHeapSize*/, policies, {});
     // start totalVariables
     // end totalVariables

    context.allocateGlobalsAndShared();
    context.totalVariables = 0/*totalVariables*/;
    context.functions = (SimFunction *) context.code->allocate( 1/*totalFunctions*/*sizeof(SimFunction) );
    context.totalFunctions = 1/*totalFunctions*/;
    bool anyPInvoke = false;
    if ( anyPInvoke || false/*(policies.threadlock_context || policies.debugger)*/ ) {
        context.contextMutex = new recursive_mutex;
    }
    context.tabMnLookup = make_shared<das_hash_map<uint64_t,SimFunction *>>();
    context.tabMnLookup->clear();
     // start totalFunctions
    struct FunctionStorage { int idx; FunctionInfo funcInfo; FuncInfo* debugInfo; };
    FunctionStorage usedFunctions[] = {
        {0, FunctionInfo("test", "test", 0x7f4e3d0819c1de70, 0x580c190f73ac3d2, 64, false, false, false, false, false, false), &__func_info__7f4e3d0819c1de70},
    };
    // end totalFunctions
    vector<pair<uint64_t, SimFunction*>> id_to_funcs;
    for (const auto& [index, func_info, debug_info]: usedFunctions) {
        InitAotFunction(context, &context.functions[index], func_info);
        context.functions[index].debugInfo = debug_info;
        (*context.tabMnLookup)[func_info.mnh] = context.functions + index;
        id_to_funcs.emplace_back(func_info.aotHash, &context.functions[index]);
        anyPInvoke |= func_info.pinvoke;
    }
    context.tabGMnLookup = make_shared<das_hash_map<uint64_t,uint32_t>>();
    context.tabGMnLookup->clear();
    for ( int i=0, is=context.totalVariables; i!=is; ++i ) {
        auto mnh = context.globalVariables[i].mangledNameHash;
        (*context.tabGMnLookup)[mnh] = context.globalVariables[i].offset;
    }
    context.tabAdLookup = make_shared<das_hash_map<uint64_t,uint64_t>>();
 FillFunction(context, getGlobalAotLibrary(), id_to_funcs);
    context.runInitScript();
}
#ifdef STANDALONE_CONTEXT_TESTS
static Context * registerStandaloneTest ( ) {
    auto ctx = new StandaloneContext();
    return ctx;
}
StandaloneContextNode node(registerStandaloneTest);
#endif
} // namespace standalone_context
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
