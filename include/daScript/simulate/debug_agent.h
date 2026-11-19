#pragma once

#include "daScript/simulate/simulate.h"

namespace das {

    class DebugAgent : public ptr_ref_count {
    public:
        virtual void onInstall ( DebugAgent * ) {}
        virtual void onUninstall ( DebugAgent * ) {}
        virtual void onCreateContext ( Context * ) {}
        virtual void onDestroyContext ( Context * ) {}
        virtual void onSimulateContext ( Context * ) {}
        virtual void onSingleStep ( Context *, const LineInfo & ) {}
        virtual void onInstrument ( Context *, const LineInfo & ) {}
        virtual void onInstrumentFunction ( Context *, SimFunction *, bool, uint64_t ) {}
        virtual void onBreakpoint ( Context *, const LineInfo &, const char *, const char * ) {}
        virtual void onVariable ( Context *, const char *, const char *, TypeInfo *, void * ) {}
        virtual void onTick () {}
        virtual void onCollect ( Context *, const LineInfo & ) {}
        virtual bool onLog ( Context *, const LineInfo * /*at*/, int /*level*/, const char * /*text*/ ) { return false; }
        virtual void onBreakpointsReset ( const char * /*file*/, int /*breakpointsNum*/ ) {}
        virtual bool isCppOnlyAgent() const { return false; }
        virtual void onBeforeGC ( Context * ) {}
        virtual void onAfterGC ( Context * ) {}
        virtual bool onUserCommand ( const char * /*cmd*/ ) { return false; }
        virtual void onAllocate ( Context *, void *, uint64_t, const LineInfo & ) {}
        virtual void onReallocate ( Context *, void *, uint64_t, void *, uint64_t, const LineInfo & ) {}
        virtual void onFree ( Context *, void *, const LineInfo & ) {}
        virtual void onAllocateString ( Context *, void *, uint64_t, bool, const LineInfo & ) {}
        virtual void onFreeString ( Context *, void *, bool, const LineInfo & ) {}
        bool isThreadLocal = false;
    };
    typedef smart_ptr<DebugAgent> DebugAgentPtr;

    void dapiReportContextState ( Context & ctx, const char * category, const char * name, const TypeInfo * info, void * data );
    DAS_API void dapiSimulateContext ( Context & ctx );
    void dapiUserCommand ( const char * command );
    void dapiOnBeforeGC ( Context & ctx );
    void dapiOnAfterGC ( Context & ctx );
    void dapiInstrumentFunctionThreadLocal ( Context & ctx, SimFunction * sim, bool entering, uint64_t userData );

    struct DebugAgentInstance {
        ContextPtr      debugAgentContext;
        DebugAgentPtr   debugAgent;
        ~DebugAgentInstance() {
            // agent lives on the context heap, so it must be released before the context
            debugAgent.reset();
            debugAgentContext.reset();
        }
    };

    DAS_API DebugAgentInstance *& threadLocalDebugAgent ();
    DAS_API void on_debug_agent_mutex ( const callable<void ()> & lmbd );
    DAS_API void for_each_debug_agent ( const callable<void (const DebugAgentPtr &)> & lmbd );

    DAS_API void tickDebugAgent ( );
    DAS_API void collectDebugAgentState ( Context & ctx, const LineInfo & at );
    DAS_API void onBreakpointsReset ( const char * file, int breakpointsNum );
    DAS_API void tickSpecificDebugAgent ( const char * name );
    DAS_API void installDebugAgent ( DebugAgentPtr newAgent, const char * category, LineInfoArg * at, Context * context );
    DAS_API void installThreadLocalDebugAgent ( DebugAgentPtr newAgent, LineInfoArg * at, Context * context );
    DAS_API void shutdownDebugAgent();
    DAS_API void shutdownThreadLocalDebugAgent();
    DAS_API void deleteDebugAgent ( const char * category, LineInfoArg * at, Context * context );
    DAS_API void forkDebugAgentContext ( Func exFn, Context * context, LineInfoArg * lineinfo );
    DAS_API bool isInDebugAgentCreation();
    DAS_API bool hasDebugAgentContext ( const char * category, LineInfoArg * at, Context * context );
    DAS_API void lockDebugAgent ( const TBlock<void> & blk, Context * context, LineInfoArg * line );
    DAS_API Context & getDebugAgentContext ( const char * category, LineInfoArg * at, Context * context );
    DAS_API void onCreateCppDebugAgent ( const char * category, function<void (Context *)> && );
    DAS_API void onDestroyCppDebugAgent ( const char * category, function<void (Context *)> && );
    DAS_API void onLogCppDebugAgent ( const char * category, function<bool(Context *, const LineInfo * at, int, const char *)> && lmb );
    DAS_API void uninstallCppDebugAgent ( const char * category );
}
