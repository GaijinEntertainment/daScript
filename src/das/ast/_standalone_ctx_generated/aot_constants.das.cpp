// Module aot_constants
#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

#include "daScript/simulate/standalone_ctx_utils.h"
 // require builtin
 // require aot_constants
#include "aot_constants.das.h"

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
namespace _anon_13529571403243701795 {



static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}



void __init_script ( Context * __context__, bool __init_shared )
{
    das_global<char *,0xaad7adf214d4fb60>(__context__) = ((char *) "#include \"daScript/misc/platform.h\"\n\n#include \"daScript/simulate/simulate.h\"\n#include \"daScript/simulate/aot.h\"\n#include \"daScript/simulate/aot_library.h\"\n\n");/*AOT_INCLUDES*/
    das_global<char *,0xdd9e1917ad07c08>(__context__) = ((char *) "#if defined(_MSC_VER)\n#pragma warning(push)\n#pragma warning(disable:4100)   // unreferenced formal parameter\n#pragma warning(disable:4189)   // local variable is initialized but not referenced\n#pragma warning(disable:4244)   // conversion from 'int32_t' to 'float', possible loss of data\n#pragma warning(disable:4114)   // same qualifier more than once\n#pragma warning(disable:4623)   // default constructor was implicitly defined as deleted\n#pragma warning(disable:4946)   // reinterpret_cast used between related classes\n#pragma warning(disable:4269)   // 'const' automatic data initialized with compiler generated default constructor produces unreliable results\n#pragma warning(disable:4555)   // result of expression not used\n#endif\n#if defined(__EDG__)\n#pragma diag_suppress 826\n#elif defined(__GNUC__) && !defined(__clang__)\n#pragma GCC diagnostic push\n#pragma GCC diagnostic ignored \"-Wunused-parameter\"\n#pragma GCC diagnostic ignored \"-Wunused-variable\"\n#pragma GCC diagnostic ignored \"-Wunused-function\"\n#pragma GCC diagnostic ignored \"-Wwrite-strings\"\n#pragma GCC diagnostic ignored \"-Wreturn-local-addr\"\n#pragma GCC diagnostic ignored \"-Wignored-qualifiers\"\n#pragma GCC diagnostic ignored \"-Wsign-compare\"\n#pragma GCC diagnostic ignored \"-Wsubobject-linkage\"\n#endif\n#if defined(__clang__)\n#pragma clang diagnostic push\n#pragma clang diagnostic ignored \"-Wunused-parameter\"\n#pragma clang diagnostic ignored \"-Wwritable-strings\"\n#pragma clang diagnostic ignored \"-Wunused-variable\"\n#pragma clang diagnostic ignored \"-Wunused-but-set-variable\"\n#pragma clang diagnostic ignored \"-Wunsequenced\"\n#pragma clang diagnostic ignored \"-Wunused-function\"\n#endif\n\n");/*AOT_HEADERS*/
    das_global<char *,0x51562779c28fd648>(__context__) = ((char *) "\n#if defined(_MSC_VER)\n#pragma warning(pop)\n#endif\n#if defined(__EDG__)\n#pragma diag_default 826\n#elif defined(__GNUC__) && !defined(__clang__)\n#pragma GCC diagnostic pop\n#endif\n#if defined(__clang__)\n#pragma clang diagnostic pop\n#endif\n");/*AOT_FOOTER*/
}
} // namespace _anon_13529571403243701795
using namespace _anon_13529571403243701795;
namespace aot_constants {

static void registerAotFunctions ( AotLibrary & aotLib ) {
    // [[ init script ]]
    aotLib[0xfb2101726506b8c3] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<AutoSimNode_Aot<&__init_script>>();
        return ctx.aotInitScript;
    };
    resolveTypeInfoAnnotations();
};

static AotListBase impl(registerAotFunctions);
Standalone::Standalone() {
    auto & context = *this;
    CodeOfPolicies policies;    policies.debugger = false /*policies.debugger*/;
    policies.persistent_heap = false;
    policies.heap_size_hint = 65536;
    policies.string_heap_size_hint = 65536;
    context.setup(3/*totalVariables*/, 2064 /*globalStringHeapSize*/, policies, {});
     // start totalVariables
    InitGlobalVar(context, &context.globalVariables[0/*pvar->index*/], GlobalVarInfo("AOT_INCLUDES", "aot_constants::AOT_INCLUDES Cs", TypeSize<char * const >::size, false));
    InitGlobalVar(context, &context.globalVariables[1/*pvar->index*/], GlobalVarInfo("AOT_HEADERS", "aot_constants::AOT_HEADERS Cs", TypeSize<char * const >::size, false));
    InitGlobalVar(context, &context.globalVariables[2/*pvar->index*/], GlobalVarInfo("AOT_FOOTER", "aot_constants::AOT_FOOTER Cs", TypeSize<char * const >::size, false));
     // end totalVariables

    context.allocateGlobalsAndShared();
    context.totalVariables = 3/*totalVariables*/;
    context.functions = (SimFunction *) context.code->allocate( 0/*totalFunctions*/*sizeof(SimFunction) );
    context.totalFunctions = 0/*totalFunctions*/;
    bool anyPInvoke = false;
    if ( anyPInvoke || false/*(policies.threadlock_context || policies.debugger)*/ ) {
        context.contextMutex = new recursive_mutex;
    }
    context.tabMnLookup = make_shared<das_hash_map<uint64_t,SimFunction *>>();
    context.tabMnLookup->clear();
    context.tabGMnLookup = make_shared<das_hash_map<uint64_t,uint32_t>>();
    context.tabGMnLookup->clear();
    for ( int i=0, is=context.totalVariables; i!=is; ++i ) {
        auto mnh = context.globalVariables[i].mangledNameHash;
        (*context.tabGMnLookup)[mnh] = context.globalVariables[i].offset;
    }
    context.tabAdLookup = make_shared<das_hash_map<uint64_t,uint64_t>>();
    {
        auto it = getGlobalAotLibrary().find(0xfb2101726506b8c3/*initSemanticHashWithDep*/);
        if ( it != getGlobalAotLibrary().end() ) {
            (it->second)(context);
        }
    }
    context.runInitScript();
}
#ifdef STANDALONE_CONTEXT_TESTS
static Context * registerStandaloneTest ( ) {
    auto ctx = new StandaloneContext();
    return ctx;
}
StandaloneContextNode node(registerStandaloneTest);
#endif
} // namespace aot_constants
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
