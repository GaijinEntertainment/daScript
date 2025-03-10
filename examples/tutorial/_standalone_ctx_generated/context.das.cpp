// Module 
#include "context.das.h"
 // require builtin
 // require tutorial_02
#include "tutorial02aot.h"

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

extern TypeInfo __type_info__af63e44c8601fa24;
extern TypeInfo __type_info__af90fe4c864e9d52;
extern TypeInfo __type_info__af63eb4c86020609;
extern TypeInfo __type_info__af8afe4c86446b52;
extern VarInfo __var_info__9c35250759777ebd;
extern FuncInfo __func_info__8259aa08d12f30b1;
extern FuncInfo __func_info__d427bf00e15116d5;
extern FuncInfo __func_info__955909f57bd61177;

VarInfo __func_info__8259aa08d12f30b1_field_0 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 4, UINT64_C(0x9c35250759777ebd), "a", 0, 0 };
VarInfo * __func_info__8259aa08d12f30b1_fields[1] =  { &__func_info__8259aa08d12f30b1_field_0 };
FuncInfo __func_info__8259aa08d12f30b1 = {"invoke block<(a:int const):int const> const", "", __func_info__8259aa08d12f30b1_fields, 1, 32, &__type_info__af8afe4c86446b52, nullptr,0,UINT64_C(0x8259aa08d12f30b1), 0x0 };
FuncInfo __func_info__d427bf00e15116d5 = {"invoke block<int const> const", "", nullptr, 0, 32, &__type_info__af8afe4c86446b52, nullptr,0,UINT64_C(0xd427bf00e15116d5), 0x0 };
FuncInfo __func_info__955909f57bd61177 = {"invoke block<void> const", "", nullptr, 0, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x955909f57bd61177), 0x0 };
TypeInfo __type_info__af63e44c8601fa24 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xaf63e44c8601fa24) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, 8, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__af63eb4c86020609 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 0, UINT64_C(0xaf63eb4c86020609) };
TypeInfo __type_info__af8afe4c86446b52 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 4, UINT64_C(0xaf8afe4c86446b52) };

static void resolveTypeInfoAnnotations()
{
}

TypeInfo * __tinfo_0[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_1[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_2[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af90fe4c864e9d52 };

inline void test_29946536011366b7 ( Context * __context__ );

void __init_script ( Context * __context__, bool __init_shared )
{
	das_global<int32_t,0x9a2fb2e6e2a13157>(__context__) = 2;/*a*/
}

inline void test_29946536011366b7 ( Context * __context__ ) { das_stack_prologue __prologue(__context__,240,"test " DAS_FILE_LINE);
{
	builtin_print(((char *) "this tutorial utilizes basic builin module with constant and function\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
	builtin_print(((char *) "sq2 = 1.4142135 // expecting sqrt(2), 1.41421\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
	builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_0, cast<char * const >::from(((char *) "a   = ")), cast<int32_t>::from(das_global<int32_t,0x9a2fb2e6e2a13157>(__context__) /*a*/), cast<char * const >::from(((char *) "   // expecting var a initialized with 2\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
	builtin_print(((char *) "xma = 3 // expecting 3\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
	bool __isTps_rename_at_15_0 = false;
	das_copy(__isTps_rename_at_15_0,true);
	int32_t __offset_rename_at_17_1 = ((int32_t)das_invoke<int32_t>::invoke(__context__,nullptr,das_make_block<int32_t>(__context__,0,0,&__func_info__d427bf00e15116d5,[&]() -> int32_t{
		return das_auto_cast<int32_t>::cast(__isTps_rename_at_15_0 ? das_auto_cast<int32_t>::cast(1) : das_auto_cast<int32_t>::cast(2));
	})));
	builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_1, cast<char * const >::from(((char *) "offset = ")), cast<int32_t>::from(__offset_rename_at_17_1), cast<char * const >::from(((char *) " // expecting 2.0\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
	int32_t __offset2_rename_at_23_2 = ((int32_t)das_invoke<int32_t>::invoke<int32_t>(__context__,nullptr,das_make_block<int32_t,int32_t>(__context__,176,0,&__func_info__8259aa08d12f30b1,[&](int32_t const  __a_rename_at_23_3) -> int32_t{
		return das_auto_cast<int32_t>::cast(__a_rename_at_23_3 + 1);
	}),13));
	builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_2, cast<char * const >::from(((char *) "offset2 = ")), cast<int32_t>::from(__offset2_rename_at_23_2), cast<char * const >::from(((char *) " // expecting 14\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
	das_invoke<void>::invoke(__context__,nullptr,das_make_block<void>(__context__,0,0,&__func_info__955909f57bd61177,[&]() -> void{
		builtin_print(((char *) "this is an anonymous invoke\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
	}));
}}

static void registerAotFunctions ( AotLibrary & aotLib ) {
	// test_29946536011366b7
	aotLib[0xa2feef601256f9ec] = +[](Context & ctx) -> SimNode* {
		return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__ ),&test_29946536011366b7>>();
	};
	// [[ init script ]]
	aotLib[0xd9e9b0755c21f011] = +[](Context & ctx) -> SimNode* {
		ctx.aotInitScript = ctx.code->makeNode<SimNode_Aot<void (*)(Context *, bool),&__init_script>>();
		return ctx.aotInitScript;
	};
	resolveTypeInfoAnnotations();
};

AotListBase impl(registerAotFunctions);
namespace context {
    auto Standalone::test (  ) -> void {
        return test_29946536011366b7(this); 
    }

Standalone::Standalone() {
    auto & context = *this;
    context.breakOnException |= false /*policies.debugger*/;
    context.persistent = false /*options.getBoolOption("persistent_heap", policies.persistent_heap)*/;
    if ( context.persistent ) {
        context.heap = make_smart<PersistentHeapAllocator>();
        context.stringHeap = make_smart<PersistentStringAllocator>();
    } else {
        context.heap = make_smart<LinearHeapAllocator>();
        context.stringHeap = make_smart<LinearStringAllocator>();
    }
    context.heap->setInitialSize ( 65536 /*options.getIntOption("heap_size_hint", policies.heap_size_hint)*/);
    context.stringHeap->setInitialSize ( 65536 /*options.getIntOption("string_heap_size_hint", policies.string_heap_size_hint)*/);
    context.constStringHeap = make_shared<ConstStringAllocator>();
    if ( 292 /*globalStringHeapSize*/) {
        context.constStringHeap->setInitialSize(292/*globalStringHeapSize*/);
    }
    context.globalVariables = (GlobalVariable *) context.code->allocate( 1/*totalVariables*/*sizeof(GlobalVariable) );
    context.globalsSize = 0;
    context.sharedSize = 0;
     // totalVariables  
    {
        auto & gvar = context.globalVariables[0/*pvar->index*/];
        gvar.name = context.code->allocateName("a"/*pvar->name*/);
        gvar.size = 4/*pvar->type->getSizeOf()*/;
        gvar.flags = 0;
        if ( false /*pvar->global_shared*/) {
            gvar.offset = context.sharedSize;
            gvar.shared = true;
            context.sharedSize = (context.sharedSize + gvar.size + 0xf) & ~0xf;
        } else {
            gvar.offset = context.globalsSize;
            context.globalsSize = (context.globalsSize + gvar.size + 0xf) & ~0xf;
        }
        gvar.mangledNameHash = 0x9a2fb2e6e2a13157/*pvar->getMangledNameHash()*/;
        gvar.init = nullptr;
    }
    context.globals = (char *) das_aligned_alloc16(context.globalsSize);
    context.shared = (char *) das_aligned_alloc16(context.sharedSize);
    context.sharedOwner = true;
    context.totalVariables = 1/*totalVariables*/;
    context.functions = (SimFunction *) context.code->allocate( 1/*totalFunctions*/*sizeof(SimFunction) );
    context.totalFunctions = 1/*totalFunctions*/;
    auto debuggerOrGC = false/*getDebugger()*/ || false/*options.getBoolOption("gc", false)*/;
    bool anyPInvoke = false;
     // totalFunctions  
    {
        string mangledName = "test"/*pfun->getMangledName()*/;
        auto MNH = hash_blockz64((uint8_t *)mangledName.c_str());
        auto & gfun = context.functions[0/*pfun->index*/];
        gfun.name = context.code->allocateName("test"/*pfun->name*/);
        gfun.mangledName = context.code->allocateName(mangledName);
        gfun.stackSize = 240/*pfun->totalStackSize*/;
        gfun.mangledNameHash = MNH;
        gfun.aotFunction = nullptr;
        gfun.flags = 0;
        gfun.fastcall = false/*pfun->fastCall*/;
        gfun.unsafe = false/*pfun->unsafeOperation*/;
        if ( false/*(pfun->result->isRefType() && !pfun->result->ref)*/ ) {
            gfun.cmres = true;
        }
        if ( false/*(pfun->module->builtIn && !pfun->module->promoted)*/ ) {
            gfun.builtin = true;
        }
        if ( false/*pfun->pinvoke*/ ) {
            anyPInvoke = true;
            gfun.pinvoke = true;
        }
    }
    if ( anyPInvoke || false/*(policies.threadlock_context || policies.debugger)*/ ) {
        context.contextMutex = new recursive_mutex;
    }
    context.tabMnLookup = make_shared<das_hash_map<uint64_t,SimFunction *>>();
    context.tabMnLookup->clear();
    // test
    (*context.tabMnLookup)[9173336596001447536/*mnh*/] = context.functions + 0/*fn->index*/;
    context.tabGMnLookup = make_shared<das_hash_map<uint64_t,uint32_t>>();
    context.tabGMnLookup->clear();
    for ( int i=0, is=context.totalVariables; i!=is; ++i ) {
        auto mnh = context.globalVariables[i].mangledNameHash;
        (*context.tabGMnLookup)[mnh] = context.globalVariables[i].offset;
    }
    for ( int i=0, is=context.totalVariables; i!=is; ++i ) {
        auto & gvar = context.globalVariables[i];
        uint32_t voffset = context.globalOffsetByMangledName(gvar.mangledNameHash);
    }
    context.tabAdLookup = make_shared<das_hash_map<uint64_t,uint64_t>>();
    auto & aotLib = getGlobalAotLibrary();
    SimFunction * fn = nullptr;
 // fnis = 0
    fn = &context.functions[0/*fni*/];
    {
        // test
        uint64_t semHash = 0xa2feef601256f9ec/*fnn[fni]*/;
        auto it = aotLib.find(semHash);
        if ( it != aotLib.end() ) {
            fn->code = (it->second)(context);
            fn->aot = true;
            auto fcb = (SimNode_CallBase *) fn->code;
            fn->aotFunction = fcb->aotFunction;
        }
    }
    {
        uint64_t semHash = 0xd9e9b0755c21f011/*initSemanticHashWithDep*/;
        auto it = aotLib.find(semHash);
        if ( it != aotLib.end() ) {
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
} // namespace context
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
