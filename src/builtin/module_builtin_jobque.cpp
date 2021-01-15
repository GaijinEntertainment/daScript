#include "daScript/misc/platform.h"

#include "daScript/misc/performance_time.h"
#include "daScript/simulate/aot_builtin_jobque.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_handle.h"

MAKE_TYPE_FACTORY(JobStatus, JobStatus)
MAKE_TYPE_FACTORY(Channel, Channel)

namespace das {

    Channel::~Channel() {
        lock_guard<mutex> guard(lock);
        pipe = {};
        tail.clear();
    }

    void Channel::push ( void * data, Context * context ) {
        lock_guard<mutex> guard(lock);
        pipe.emplace(data, context!=owner ? context : nullptr);
        cond.notify_all();  // notify_one??
    }

    void * Channel::pop () {
        while ( true ) {
            unique_lock<mutex> uguard(lock);
            if ( !cond.wait_for(uguard, chrono::milliseconds(0), [&]() {
                bool continue_waiting = (remaining>0) && pipe.empty();
                return !continue_waiting;
            }) ) {
                this_thread::yield();
            } else {
                break;
            }
        }
        lock_guard<mutex> guard(lock);
        if ( pipe.empty() ) {
            tail.clear();
        } else {
            tail = move ( pipe.front() );
            pipe.pop();
        }
        return tail.data;
    }

    bool Channel::isEmpty() const {
        lock_guard<mutex> guard(lock);
        return pipe.empty();
    }

    void Channel::notify() {
        lock_guard<mutex> guard(lock);
        DAS_ASSERTF(remaining != 0, "Nothing to notify!");
        --remaining;
        if ( remaining==0 ) {
            cond.notify_all();
        }
    }

    void Channel::wait() {
        unique_lock<mutex> uguard(lock);
        while ( remaining ) {
            cond.wait(uguard);
        }
    }

    bool Channel::isReady() const {
        lock_guard<mutex> guard(lock);
        return remaining==0;
    }

    void channelPush ( Channel * ch, void * data, Context * ctx ) {
        ch->push(data, ctx);
    }

    void * channelPop ( Channel * ch ) {
        return ch->pop();
    }

    void withChannel ( const TBlock<void,Channel *> & blk, Context * context ) {
        Channel ch(context);
        das_invoke<void>::invoke<Channel *>(context, blk, &ch);
    }

    void withChannelEx ( int32_t count, const TBlock<void,Channel *> & blk, Context * context ) {
        Channel ch(context,count);
        das_invoke<void>::invoke<Channel *>(context, blk, &ch);
    }

    void waitForChannel ( Channel * status ) {
        if ( !status ) return;
        status->wait();
    }

    void notifyChannel ( Channel * status ) {
        if ( !status ) return;
        status->notify();
    }

    struct ChannelAnnotation : ManagedStructureAnnotation<Channel,false> {
        ChannelAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("Channel", ml) {
            addProperty<DAS_BIND_MANAGED_PROP(isEmpty)>("isEmpty");
            addProperty<DAS_BIND_MANAGED_PROP(isReady)>("isReady");
        }
    };


    struct JobStatusAnnotation : ManagedStructureAnnotation<JobStatus,false> {
        JobStatusAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("JobStatus", ml) {
            addProperty<DAS_BIND_MANAGED_PROP(isReady)>("isReady");
        }
    };

    mutex              g_jobQueMutex;
    shared_ptr<JobQue> g_jobQue;

    void new_job_invoke ( Lambda lambda, Func fn, int32_t lambdaSize, Context * context, LineInfoArg * lineinfo ) {
        if ( !g_jobQue ) context->throw_error_at(*lineinfo, "need to be in 'with_job_que' block");
        auto forkContext = make_shared<Context>(*context);
        auto ptr = forkContext->heap->allocate(lambdaSize);
        forkContext->heap->mark_comment(ptr, "new [[ ]] in new_job");
        memset ( ptr, 0, lambdaSize );
        das_invoke_function<void>::invoke(forkContext.get(), fn, ptr, lambda.capture);
        das_delete<Lambda>::clear(context, lambda);
        g_jobQue->push([=]() mutable {
            Lambda flambda(ptr);
            das_invoke_lambda<void>::invoke(forkContext.get(), flambda);
            das_delete<Lambda>::clear(forkContext.get(), flambda);
        }, 0, JobPriority::Default);
    }

    void new_thread_invoke ( Lambda lambda, Func fn, int32_t lambdaSize, Context * context ) {
        auto forkContext = make_shared<Context>(*context);
        auto ptr = forkContext->heap->allocate(lambdaSize);
        forkContext->heap->mark_comment(ptr, "new [[ ]] in new_thread");
        memset ( ptr, 0, lambdaSize );
        das_invoke_function<void>::invoke(forkContext.get(), fn, ptr, lambda.capture);
        das_delete<Lambda>::clear(context, lambda);
        thread([=]() mutable {
            Lambda flambda(ptr);
            das_invoke_lambda<void>::invoke(forkContext.get(), flambda);
            das_delete<Lambda>::clear(forkContext.get(), flambda);
        }).detach();
    }

    void withJobQue ( const TBlock<void> & block, Context * context, LineInfoArg * lineInfo ) {
        if ( !g_jobQue ) {
            lock_guard<mutex> guard(g_jobQueMutex);
            g_jobQue = make_shared<JobQue>();
        }
        {
            shared_ptr<JobQue> jq = g_jobQue;
            context->invoke(block, nullptr, lineInfo);
        }
        {
            lock_guard<mutex> guard(g_jobQueMutex);
            if ( g_jobQue.use_count()==1 ) g_jobQue.reset();
        }
    }

    void withJobStatus ( int32_t total, const TBlock<void,JobStatus *> & block, Context * context, LineInfoArg * lineInfo ) {
        JobStatus status(total);
        vec4f args[1];
        args[0] = cast<JobStatus *>::from(&status);
        context->invoke(block,args,lineInfo);
    }

    void waitForJob ( JobStatus * status ) {
        if ( !status ) return;
        status->Wait();
    }

    void notifyJob ( JobStatus * status ) {
        if ( !status ) return;
        status->Notify();
    }

    int getTotalHwJobs( Context * context, LineInfoArg * at ) {
        if ( !g_jobQue ) context->throw_error_at(*at, "need to be in 'with_job_que' block");
        return g_jobQue->getTotalHwJobs();
    }

    int getTotalHwThreads () {
        return thread::hardware_concurrency();
    }

    class Module_JobQue : public Module {
    public:
        Module_JobQue() : Module("jobque") {
            DAS_PROFILE_SECTION("Module_JobQue");
            ModuleLibrary lib;
            lib.addModule(this);
            lib.addBuiltInModule();
            // channel
            addAnnotation(make_smart<ChannelAnnotation>(lib));
            addExtern<DAS_BIND_FUN(channelPush)>(*this, lib,  "_builtin_channel_push",
                SideEffects::modifyArgumentAndExternal, "channelPush");
            addExtern<DAS_BIND_FUN(channelPop)>(*this, lib,  "_builtin_channel_pop",
                SideEffects::modifyArgumentAndExternal, "channelPop");
            addExtern<DAS_BIND_FUN(withChannel)>(*this, lib,  "with_channel",
                SideEffects::invoke, "withChannel");
            addExtern<DAS_BIND_FUN(withChannelEx)>(*this, lib,  "with_channel",
                SideEffects::invoke, "withChannelEx");
            addExtern<DAS_BIND_FUN(waitForChannel)>(*this, lib,  "join",
                SideEffects::modifyExternal, "waitForChannel");
            addExtern<DAS_BIND_FUN(notifyChannel)>(*this, lib,  "notify",
                SideEffects::modifyExternal, "notifyChannel");
            // job
            addAnnotation(make_smart<JobStatusAnnotation>(lib));
            addExtern<DAS_BIND_FUN(withJobStatus)>(*this, lib,  "with_job_status",
                SideEffects::modifyExternal, "withJobStatus");
            addExtern<DAS_BIND_FUN(waitForJob)>(*this, lib,  "join",
                SideEffects::modifyExternal, "waitForJob");
            addExtern<DAS_BIND_FUN(notifyJob)>(*this, lib,  "notify",
                SideEffects::modifyExternal, "notifyJob");
            // context
            addExtern<DAS_BIND_FUN(thisContext)>(*this, lib,  "this_context",
                SideEffects::accessExternal, "thisContext");
            // fork \ invoke \ etc
            addExtern<DAS_BIND_FUN(new_job_invoke)>(*this, lib,  "new_job_invoke",
                SideEffects::modifyExternal, "new_job_invoke");
            addExtern<DAS_BIND_FUN(withJobQue)>(*this, lib,  "with_job_que",
                SideEffects::modifyExternal, "withJobQue");
            addExtern<DAS_BIND_FUN(getTotalHwJobs)>(*this, lib,  "get_total_hw_jobs",
                SideEffects::accessExternal, "getTotalHwJobs");
            addExtern<DAS_BIND_FUN(getTotalHwThreads)>(*this, lib,  "get_total_hw_threads",
                SideEffects::accessExternal, "getTotalHwThreads");
            addExtern<DAS_BIND_FUN(new_thread_invoke)>(*this, lib,  "new_thread_invoke",
                SideEffects::modifyExternal, "new_thread_invoke");
        }
        virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
            tw << "#include \"daScript/simulate/aot_builtin_jobque.h\"\n";
            return ModuleAotType::cpp;
        }
        virtual ~Module_JobQue() {
            lock_guard<mutex> guard(g_jobQueMutex);
            g_jobQue.reset();
        }
    protected:
        // bool needShutdown = false;
    };
}

REGISTER_MODULE_IN_NAMESPACE(Module_JobQue,das);

