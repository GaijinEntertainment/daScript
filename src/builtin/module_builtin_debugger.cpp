#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate_nodes.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_policy_types.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/simulate/aot_builtin_debugger.h"
#include "module_builtin_rtti.h"
#include "daScript/misc/performance_time.h"

using namespace das;

MAKE_TYPE_FACTORY(DebugAgent,DebugAgent)

namespace das
{
    struct DebugAgentAdapter : DebugAgent {
        DebugAgentAdapter ( char * pClass, const StructInfo * info, Context * ctx )
            : classPtr(pClass), context(ctx) {
            fnOnInstall = adapt("onInstall",pClass,info);
            fnOnUninstall = adapt("onUninstall",pClass,info);
            fnOnCreateContext = adapt("onCreateContext",pClass,info);
            fnOnDestroyContext = adapt("onDestroyContext",pClass,info);
            fnOnSingleStep = adapt("onSingleStep",pClass,info);
            fnOnBreakpoint = adapt("onBreakpoint",pClass,info);
            fnOnTick = adapt("onTick",pClass,info);
        }

        virtual void onInstall ( DebugAgent * agent ) override {
            if ( fnOnInstall ) {
                das_invoke_function<void>::invoke<void *,DebugAgentPtr>(context,fnOnInstall,classPtr,agent);
            }
        }
        virtual void onUninstall ( DebugAgent * agent ) override {
            if ( fnOnInstall ) {
                das_invoke_function<void>::invoke<void *,DebugAgentPtr>(context,fnOnUninstall,classPtr,agent);
            }
        }
        virtual void onCreateContext ( Context * ctx ) override {
            if ( fnOnCreateContext ) {
                das_invoke_function<void>::invoke<void *,Context *>(context,fnOnCreateContext,classPtr,ctx);
            }
        }
        virtual void onDestroyContext ( Context * ctx ) override {
            if ( fnOnDestroyContext ) {
                das_invoke_function<void>::invoke<void *,Context *>(context,fnOnDestroyContext,classPtr,ctx);
            }
        }
        virtual void onSingleStep ( Context * ctx, const LineInfo & at ) override {
            if ( fnOnSingleStep ) {
                das_invoke_function<void>::invoke<void *,Context *,const LineInfo &>(context,fnOnSingleStep,classPtr,ctx,at);
            }
        }
        virtual void onBreakpoint ( Context * ctx, const LineInfo & at ) override {
            if ( fnOnBreakpoint ) {
                das_invoke_function<void>::invoke<void *,Context *,const LineInfo &>(context,fnOnBreakpoint,classPtr,ctx,at);
            }
        }
        virtual void onTick () override {
            if ( fnOnTick ) {
                das_invoke_function<void>::invoke<void *>(context,fnOnTick,classPtr);
            }
        }
    protected:
        void *      classPtr;
        Context *   context;
    protected:
        Func    fnOnInstall;
        Func    fnOnUninstall;
        Func    fnOnCreateContext;
        Func    fnOnDestroyContext;
        Func    fnOnSingleStep;
        Func    fnOnBreakpoint;
        Func    fnOnTick;
    };

    struct AstDebugAgentAnnotation : ManagedStructureAnnotation<DebugAgent,false,true> {
        AstDebugAgentAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("DebugAgent", ml) {
        }
    };

    #include "debugger.das.inc"

    DebugAgentPtr makeDebugAgent ( const void * pClass, const StructInfo * info, Context * context ) {
        return make_smart<DebugAgentAdapter>((char *)pClass,info,context);
    }

    void debuggerSetContextSingleStep ( Context & context, bool step ) {
        context.setSingleStep(step);
    }

    void debuggerStackWalk ( Context & context, const LineInfo & lineInfo ) {
        context.stackWalk(&lineInfo, true, true);
    }

    class Module_Debugger : public Module {
    public:
        Module_Debugger() : Module("debugapi") {
            DAS_PROFILE_SECTION("Module_Debugger");
            ModuleLibrary lib;
            lib.addModule(this);
            lib.addBuiltInModule();
            lib.addModule(Module::require("rtti"));
            // annotations
            addAnnotation(make_smart<AstDebugAgentAnnotation>(lib));
            // functions
            addExtern<DAS_BIND_FUN(makeDebugAgent)>(*this, lib,  "make_debug_agent",
                SideEffects::modifyExternal, "makeDebugAgent");
            addExtern<DAS_BIND_FUN(tickDebugAgent)>(*this, lib,  "tick_debug_agent",
                SideEffects::modifyExternal, "tickDebugAgent");
            addExtern<DAS_BIND_FUN(installDebugAgent)>(*this, lib,  "install_debug_agent",
                SideEffects::modifyExternal, "installDebugAgent");
            addExtern<DAS_BIND_FUN(forkDebugAgentContext)>(*this, lib,  "fork_debug_agent_context",
                SideEffects::modifyExternal, "forkDebugAgentContext");
            addExtern<DAS_BIND_FUN(debuggerSetContextSingleStep)>(*this, lib,  "set_single_step",
                SideEffects::modifyExternal, "debuggerSetContextSingleStep");
            addExtern<DAS_BIND_FUN(debuggerStackWalk)>(*this, lib, "stackwalk",
                SideEffects::modifyExternal, "debuggerStackWalk");
            // add builtin module
            compileBuiltinModule("debugger.das",debugger_das,sizeof(debugger_das));
            // lets make sure its all aot ready
            verifyAotReady();
        }
        virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
            tw << "#include \"daScript/simulate/aot_builtin_debugger.h\"\n";
            return ModuleAotType::cpp;
        }
    };
}

REGISTER_MODULE_IN_NAMESPACE(Module_Debugger,das);
