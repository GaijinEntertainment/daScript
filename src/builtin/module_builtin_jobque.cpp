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
        DAS_ASSERT(mRef==0);
    }

    void Channel::push ( void * data, Context * context ) {
        lock_guard<mutex> guard(lock);
        pipe.emplace(data, context!=owner ? context : nullptr);
        cond.notify_all();  // notify_one??
    }

    void * Channel::pop () {
        while ( true ) {
            unique_lock<mutex> uguard(lock);
            if ( !cond.wait_for(uguard, chrono::milliseconds(mSleepMs), [&]() {
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

    int Channel::size() const {
        lock_guard<mutex> guard(lock);
        return remaining;
    }

    void Channel::notify() {
        lock_guard<mutex> guard(lock);
        DAS_ASSERTF(remaining != 0, "Nothing to notify!");
        --remaining;
        if ( remaining==0 ) {
            cond.notify_all();
        }
    }

    void Channel::notifyAndRelease() {
        lock_guard<mutex> guard(lock);
        mRef--;
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

    int Channel::append(int size) {
        lock_guard<mutex> guard(lock);
        remaining += size;
        return remaining;
    }

    bool Channel::isReady() const {
        lock_guard<mutex> guard(lock);
        return remaining==0;
    }

    void channelPush ( Channel * ch, void * data, Context * context, LineInfoArg * at ) {
        if ( !ch ) context->throw_error_at(at, "channelPush: channel is null");
        ch->push(data, context);
    }

    void * channelPop ( Channel * ch, Context * context, LineInfoArg * at ) {
        if ( !ch ) context->throw_error_at(at, "channelPop: channel is null");
        return ch->pop();
    }

    int channelAppend ( Channel * ch, int size, Context * context, LineInfoArg * at ) {
        if ( !ch ) context->throw_error_at(at, "channelPop: channel is null");
        return ch->append(size);
    }

    void withChannel ( const TBlock<void,Channel *> & blk, Context * context, LineInfoArg * at ) {
        Channel ch(context);
        ch.addRef();
        das_invoke<void>::invoke<Channel *>(context, at, blk, &ch);
        if ( ch.releaseRef() ) {
            context->throw_error_at(at, "channel beeing deleted while being used");
        }
    }

    void withChannelEx ( int32_t count, const TBlock<void,Channel *> & blk, Context * context, LineInfoArg * at ) {
        Channel ch(context,count);
        ch.addRef();
        das_invoke<void>::invoke<Channel *>(context, at, blk, &ch);
        if ( ch.releaseRef() ) {
            context->throw_error_at(at, "channel beeing deleted while being used");
        }
    }

    Channel * channelCreate( Context * context, LineInfoArg * ) {
        Channel * ch = new Channel(context);
        ch->addRef();
        return ch;
    }

    void channelRemove( Channel * ch, Context * context, LineInfoArg * at ) {
        if (ch->releaseRef()) {
            context->throw_error_at(at, "channel beeing deleted while being used");
        }
        delete ch;
    }

    void channelAddRef ( Channel * ch, Context * context, LineInfoArg * at ) {
        if ( !ch ) context->throw_error_at(at, "channelAddRef: channel is null");
        ch->addRef();
    }

    void channelReleaseRef ( Channel * & ch, Context * context, LineInfoArg * at ) {
        if ( !ch ) context->throw_error_at(at, "channelReleaseRef: channel is null");
        ch->releaseRef();
        ch = nullptr;
    }

    void waitForChannel ( Channel * status, Context * context, LineInfoArg * at ) {
        if ( !status ) context->throw_error_at(at, "waitForChannel: channel is null");
        status->wait();
    }

    void notifyChannel ( Channel * status, Context * context, LineInfoArg * at ) {
        if ( !status ) context->throw_error_at(at, "notifyChannel: channel is null");
        status->notify();
    }

    void notifyAndReleaseChannel ( Channel * & status, Context * context, LineInfoArg * at ) {
        if ( !status ) context->throw_error_at(at, "notifyAndReleaseChannel: channel is null");
        status->notifyAndRelease();
        status = nullptr;
    }

    struct ChannelAnnotation : ManagedStructureAnnotation<Channel,false> {
        ChannelAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("Channel", ml) {
            addProperty<DAS_BIND_MANAGED_PROP(isEmpty)>("isEmpty");
            addProperty<DAS_BIND_MANAGED_PROP(isReady)>("isReady");
            addProperty<DAS_BIND_MANAGED_PROP(size)>("size");
        }
    };

    struct JobStatusAnnotation : ManagedStructureAnnotation<JobStatus,false> {
        JobStatusAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("JobStatus", ml) {
            addProperty<DAS_BIND_MANAGED_PROP(isReady)>("isReady");
        }
    };

    mutex              g_jobQueMutex;
    shared_ptr<JobQue> g_jobQue;

}

das::Context* get_clone_context( das::Context * ctx, uint32_t category );//link time resolved dependencies

namespace das {

    void new_job_invoke ( Lambda lambda, Func fn, int32_t lambdaSize, Context * context, LineInfoArg * lineinfo ) {
        if ( !g_jobQue ) context->throw_error_at(lineinfo, "need to be in 'with_job_que' block");
        shared_ptr<Context> forkContext;
        forkContext.reset(get_clone_context(context, uint32_t(ContextCategory::job_clone)));
        auto ptr = forkContext->heap->allocate(lambdaSize + 16);
        forkContext->heap->mark_comment(ptr, "new [[ ]] in new_job");
        memset ( ptr, 0, lambdaSize + 16 );
        ptr += 16;
        das_invoke_function<void>::invoke(forkContext.get(), lineinfo, fn, ptr, lambda.capture);
        das_delete<Lambda>::clear(context, lambda);
        auto bound = daScriptEnvironment::bound;
        g_jobQue->push([=]() mutable {
            daScriptEnvironment::bound = bound;
            Lambda flambda(ptr);
            das_invoke_lambda<void>::invoke(forkContext.get(), lineinfo, flambda);
            das_delete<Lambda>::clear(forkContext.get(), flambda);
        }, 0, JobPriority::Default);
    }

    static atomic<int32_t> g_jobQueAvailable{0};
    static atomic<int32_t> g_jobQueTotalThreads{0};

    bool is_job_que_shutting_down () {
        return g_jobQueAvailable == 0;
    }

    void new_thread_invoke ( Lambda lambda, Func fn, int32_t lambdaSize, Context * context, LineInfoArg * lineinfo ) {
        shared_ptr<Context> forkContext;
        forkContext.reset(get_clone_context(context, uint32_t(ContextCategory::thread_clone)));
        auto ptr = forkContext->heap->allocate(lambdaSize + 16);
        forkContext->heap->mark_comment(ptr, "new [[ ]] in new_thread");
        memset ( ptr, 0, lambdaSize + 16 );
        ptr += 16;
        das_invoke_function<void>::invoke(forkContext.get(), lineinfo, fn, ptr, lambda.capture);
        das_delete<Lambda>::clear(context, lambda);
        g_jobQueTotalThreads ++;
        auto bound = daScriptEnvironment::bound;
        thread([=]() mutable {
            daScriptEnvironment::bound = bound;
            Lambda flambda(ptr);
            das_invoke_lambda<void>::invoke(forkContext.get(), lineinfo, flambda);
            das_delete<Lambda>::clear(forkContext.get(), flambda);
            g_jobQueTotalThreads --;
        }).detach();
    }

    void withJobQue ( const TBlock<void> & block, Context * context, LineInfoArg * lineInfo ) {
        if ( !g_jobQue ) {
            lock_guard<mutex> guard(g_jobQueMutex);
            g_jobQue = make_shared<JobQue>();
        }
        {
            shared_ptr<JobQue> jq = g_jobQue;
            context->invoke(block, nullptr, nullptr, lineInfo);
        }
        {
            lock_guard<mutex> guard(g_jobQueMutex);
            if ( g_jobQue.use_count()==1 ) g_jobQue.reset();
        }
    }

    void jobStatusAddRef ( JobStatus * status, Context * context, LineInfoArg * at ) {
        if ( !status ) context->throw_error_at(at, "jobStatusAddRef: status is null");
        status->addRef();
    }

    void jobStatusReleaseRef ( JobStatus * & status, Context * context, LineInfoArg * at ) {
        if ( !status ) context->throw_error_at(at, "jobStatusReleaseRef: status is null");
        status->releaseRef();
        status = nullptr;
    }

    void withJobStatus ( int32_t total, const TBlock<void,JobStatus *> & block, Context * context, LineInfoArg * lineInfo ) {
        JobStatus status(total);
        status.addRef();
        vec4f args[1];
        args[0] = cast<JobStatus *>::from(&status);
        context->invoke(block,args,nullptr,lineInfo);
        if ( int ref = status.releaseRef() ) {
            context->throw_error_at(lineInfo, "job status beeing deleted while being used (ref %i)", ref);
        }
    }

    void waitForJob ( JobStatus * status, Context * context, LineInfoArg * at ) {
        if ( !status ) context->throw_error_at(at, "waitForJob: status is null");
        status->Wait();
    }

    void notifyJob ( JobStatus * status, Context * context, LineInfoArg * at ) {
        if ( !status ) context->throw_error_at(at, "notifyJob: status is null");
        status->Notify();
    }

    void notifyAndReleaseJob ( JobStatus * & status, Context * context, LineInfoArg * at ) {
        if ( !status ) context->throw_error_at(at, "notifyAndReleaseJob: status is null");
        status->NotifyAndRelease();
        status = nullptr;
    }

    int getTotalHwJobs( Context * context, LineInfoArg * at ) {
        if ( !g_jobQue ) context->throw_error_at(at, "need to be in 'with_job_que' block");
        return g_jobQue->getTotalHwJobs();
    }

    int getTotalHwThreads () {
        return thread::hardware_concurrency();
    }

    class Module_JobQue : public Module {
    public:
        Module_JobQue() : Module("jobque") {
            DAS_PROFILE_SECTION("Module_JobQue");
            g_jobQueAvailable++;
            // libs
            ModuleLibrary lib;
            lib.addModule(this);
            lib.addBuiltInModule();
            // channel
            addAnnotation(make_smart<ChannelAnnotation>(lib));
            addExtern<DAS_BIND_FUN(channelPush)>(*this, lib,  "_builtin_channel_push",
                SideEffects::modifyArgumentAndExternal, "channelPush")
                    ->args({"channel","data","context","line"});
            addExtern<DAS_BIND_FUN(channelPop)>(*this, lib,  "_builtin_channel_pop",
                SideEffects::modifyArgumentAndExternal, "channelPop")
                    ->args({"channel","context","line"});
            addExtern<DAS_BIND_FUN(channelAppend)>(*this, lib, "append",
                SideEffects::modifyArgument, "channelAppend")
                    ->args({"channel","size","context","line"});
            addExtern<DAS_BIND_FUN(withChannel)>(*this, lib,  "with_channel",
                SideEffects::invoke, "withChannel")
                    ->args({"block","context","line"});
            addExtern<DAS_BIND_FUN(withChannelEx)>(*this, lib,  "with_channel",
                SideEffects::invoke, "withChannelEx")
                    ->args({"count","block","context","line"});
            addExtern<DAS_BIND_FUN(channelCreate)>(*this, lib, "channel_create",
                SideEffects::invoke, "channelCreate")
                    ->args({ "context","line" })->unsafeOperation = true;
            addExtern<DAS_BIND_FUN(channelRemove)>(*this, lib, "channel_remove",
                SideEffects::invoke, "channelRemove")
                    ->args({ "channel", "context","line" })->unsafeOperation = true;;
            addExtern<DAS_BIND_FUN(channelAddRef)>(*this, lib,  "add_ref",
                SideEffects::modifyArgumentAndAccessExternal, "channelAddRef")
                    ->args({"channel","context","line"});
            addExtern<DAS_BIND_FUN(channelReleaseRef)>(*this, lib,  "release",
                SideEffects::modifyArgumentAndAccessExternal, "channelReleaseRef")
                    ->args({"channel","context","line"});
            addExtern<DAS_BIND_FUN(waitForChannel)>(*this, lib,  "join",
                SideEffects::modifyExternal, "waitForChannel")
                    ->args({"channel","context","line"});
            addExtern<DAS_BIND_FUN(notifyChannel)>(*this, lib,  "notify",
                SideEffects::modifyExternal, "notifyChannel")
                    ->args({"channel","context","line"});
            addExtern<DAS_BIND_FUN(notifyAndReleaseChannel)>(*this, lib,  "notify_and_release",
                SideEffects::modifyExternal, "notifyAndReleaseChannel")
                    ->args({"channel","context","line"});
            // job
            addAnnotation(make_smart<JobStatusAnnotation>(lib));
            addExtern<DAS_BIND_FUN(withJobStatus)>(*this, lib,  "with_job_status",
                SideEffects::modifyExternal, "withJobStatus")
                    ->args({"total","block","context","line"});
            addExtern<DAS_BIND_FUN(jobStatusAddRef)>(*this, lib, "add_ref",
                SideEffects::modifyArgumentAndAccessExternal, "jobStatusAddRef")
                    ->args({"status","context","line"});
            addExtern<DAS_BIND_FUN(jobStatusReleaseRef)>(*this, lib, "release",
                SideEffects::modifyArgumentAndAccessExternal, "jobStatusReleaseRef")
                    ->args({"status","context","line"});
            addExtern<DAS_BIND_FUN(waitForJob)>(*this, lib,  "join",
                SideEffects::modifyExternal, "waitForJob")
                    ->args({"job","context","line"});
            addExtern<DAS_BIND_FUN(notifyJob)>(*this, lib,  "notify",
                SideEffects::modifyExternal, "notifyJob")
                    ->args({"job","context","line"});
            addExtern<DAS_BIND_FUN(notifyAndReleaseJob)>(*this, lib,  "notify_and_release",
                SideEffects::modifyExternal, "notifyAndReleaseJob")
                    ->args({"job","context","line"});
            // fork \ invoke \ etc
            addExtern<DAS_BIND_FUN(new_job_invoke)>(*this, lib,  "new_job_invoke",
                SideEffects::modifyExternal, "new_job_invoke")
                    ->args({"lambda","function","lambdaSize","context","line"});
            addExtern<DAS_BIND_FUN(withJobQue)>(*this, lib,  "with_job_que",
                SideEffects::modifyExternal, "withJobQue")
                    ->args({"block","context","line"});
            addExtern<DAS_BIND_FUN(getTotalHwJobs)>(*this, lib,  "get_total_hw_jobs",
                SideEffects::accessExternal, "getTotalHwJobs")
                    ->args({"context","line"});
            addExtern<DAS_BIND_FUN(getTotalHwThreads)>(*this, lib,  "get_total_hw_threads",
                SideEffects::accessExternal, "getTotalHwThreads");
            addExtern<DAS_BIND_FUN(new_thread_invoke)>(*this, lib,  "new_thread_invoke",
                SideEffects::modifyExternal, "new_thread_invoke")
                    ->args({"lambda","function","lambdaSize","context","line"});
            addExtern<DAS_BIND_FUN(is_job_que_shutting_down)>(*this, lib,  "is_job_que_shutting_down",
                SideEffects::modifyExternal, "is_job_que_shutting_down");
        }
        virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
            tw << "#include \"daScript/simulate/aot_builtin_jobque.h\"\n";
            return ModuleAotType::cpp;
        }
        virtual ~Module_JobQue() {
            g_jobQueAvailable--;
            if ( g_jobQueAvailable == 0 ) {
                while ( g_jobQueTotalThreads ) {
                    builtin_sleep(0);
                }
                lock_guard<mutex> guard(g_jobQueMutex);
                g_jobQue.reset();
            }
        }
    protected:

        // bool needShutdown = false;
    };
}

REGISTER_MODULE_IN_NAMESPACE(Module_JobQue,das);

