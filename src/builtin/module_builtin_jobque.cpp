#include "daScript/misc/platform.h"

#include "daScript/misc/performance_time.h"
#include "daScript/misc/job_que.h"
// #include "daScript/simulate/aot_builtin_network.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_handle.h"

namespace das {

    unique_ptr<JobQue> g_jobQue;

    void pinvoke ( Func fn, Context * context, LineInfoArg * lineinfo ) {
        if ( !fn.index ) context->throw_error_at(*lineinfo, "can't pinvoke null funciton");
        SimFunction * simFn = context->getFunction(fn.index-1);
        if ( !simFn ) context->throw_error_at(*lineinfo, "can't pinvoke null funciton");
        LineInfo at = *lineinfo;
        Context * forkContext = new Context(*context);
        g_jobQue->push([=]() mutable {
            forkContext->callOrFastcall(simFn, nullptr, &at);
            delete forkContext;
        }, JobCategory::Generic, JobPriority::Default);
    }

    void pfork_invoke ( Lambda lambda, Func fn, int32_t lambdaSize, Context * context, LineInfoArg * lineinfo ) {
        if ( !fn.index ) context->throw_error_at(*lineinfo, "can't pfork_invoke null funciton");
        SimFunction * cloneFn = context->getFunction(fn.index-1);
        if ( !cloneFn ) context->throw_error_at(*lineinfo, "can't pfork_invoke null funciton");
        LineInfo debugInfo = *lineinfo;
        Context * forkContext = new Context(*context);
        auto ptr = forkContext->heap->allocate(lambdaSize);
        forkContext->heap->mark_comment(ptr, "new [[ ]] in pfork");
        memset ( ptr, 0, lambdaSize );
        vec4f args[2];
        args[0] = cast<void *>::from(ptr);
        args[1] = cast<void *>::from(lambda.capture);
        forkContext->callOrFastcall(cloneFn, args, &debugInfo);
        das_delete<Lambda>::clear(context, lambda);
        g_jobQue->push([=]() mutable {
            vec4f argValues[1];
            argValues[0] = cast<void *>::from(ptr);
            int32_t * funIndex = (int32_t *) ptr;
            if (!funIndex) forkContext->throw_error_at(debugInfo,"invoke null lambda");
            SimFunction * simFunc = forkContext->getFunction(*funIndex-1);
            if ( !simFunc ) forkContext->throw_error_at(debugInfo,"invoke null function");
            return forkContext->call(simFunc, argValues, &debugInfo);
            Lambda flambda;
            flambda.capture = ptr;
            das_delete<Lambda>::clear(forkContext, flambda);
            delete forkContext;
        }, JobCategory::Generic, JobPriority::Default);
    }

    Context  * thisContext ( Context * context ) {
        return context;
    }

    class Module_JobQue : public Module {
    public:
        Module_JobQue() : Module("jobque") {
            DAS_PROFILE_SECTION("Module_JobQue");
            ModuleLibrary lib;
            lib.addModule(this);
            lib.addBuiltInModule();
            // debug functions
            addExtern<DAS_BIND_FUN(thisContext)>(*this, lib,  "this_context",
                SideEffects::accessExternal, "thisContext");
            addExtern<DAS_BIND_FUN(pinvoke)>(*this, lib,  "pinvoke",
                SideEffects::modifyExternal, "pinvoke");
            addExtern<DAS_BIND_FUN(pfork_invoke)>(*this, lib,  "pfork_invoke",
                SideEffects::modifyExternal, "pfork_invoke");
            // create job que
            g_jobQue = make_unique<JobQue>();
        }
        virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
            tw << "#include \"daScript/simulate/aot_builtin_jobque.h\"\n";
            return ModuleAotType::cpp;
        }
        virtual ~Module_JobQue() {
            g_jobQue.reset();
        }
    protected:
        // bool needShutdown = false;
    };
}

REGISTER_MODULE_IN_NAMESPACE(Module_JobQue,das);

