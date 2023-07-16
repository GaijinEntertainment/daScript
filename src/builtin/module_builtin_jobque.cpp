#include "daScript/misc/platform.h"

#include "daScript/misc/performance_time.h"
#include "daScript/simulate/aot_builtin_jobque.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_handle.h"

#include "daScript/misc/job_que.h"

MAKE_TYPE_FACTORY(JobStatus, JobStatus)
MAKE_TYPE_FACTORY(Channel, Channel)

namespace das {
    Channel::~Channel() {
        lock_guard<mutex> guard(mCompleteMutex);
        pipe = {};
        tail.clear();
        DAS_ASSERT(mRef==0);
    }

    void Channel::push ( void * data, TypeInfo * ti, Context * context ) {
        lock_guard<mutex> guard(mCompleteMutex);
        pipe.emplace_back(data, ti, context!=owner ? context : nullptr);
        mCond.notify_all();  // notify_one??
    }

    void Channel::pop ( const TBlock<void,void *> & blk, Context * context, LineInfoArg * at ) {
        while ( true ) {
            unique_lock<mutex> uguard(mCompleteMutex);
            if ( !mCond.wait_for(uguard, chrono::milliseconds(mSleepMs), [&]() {
                bool continue_waiting = (mRemaining>0) && pipe.empty();
                return !continue_waiting;
            }) ) {
                this_thread::yield();
            } else {
                break;
            }
        }
        lock_guard<mutex> guard(mCompleteMutex);
        if ( pipe.empty() ) {
            tail.clear();
        } else {
            tail = das::move(pipe.front());
            pipe.pop_front();
        }
        das_invoke<void>::invoke<void *>(context, at, blk, tail.data);
    }

    bool Channel::isEmpty() const {
        lock_guard<mutex> guard(mCompleteMutex);
        return pipe.empty();
    }

    int32_t JobStatus::size() const {
        lock_guard<mutex> guard(mCompleteMutex);
        return (int32_t) mRemaining;
    }

    int32_t Channel::total() const {
        lock_guard<mutex> guard(mCompleteMutex);
        return (int32_t) pipe.size();
    }

    int JobStatus::append(int size) {
        lock_guard<mutex> guard(mCompleteMutex);
        mRemaining += size;
        return mRemaining;
    }

    void channelGather ( Channel * ch, const TBlock<void,void *> & blk, Context * context, LineInfoArg * at ) {
        if ( !ch ) context->throw_error_at(at, "channelGather: channel is null");
        ch->gather([&](void * data, TypeInfo *, Context *) {
            das_invoke<void>::invoke<void *>(context, at, blk, data);
        });
    }

    void channelPeek ( Channel * ch, const TBlock<void,void *> & blk, Context * context, LineInfoArg * at ) {
        if ( !ch ) context->throw_error_at(at, "channelPeek: channel is null");
        ch->for_each_item([&](void * data, TypeInfo *, Context *) {
            das_invoke<void>::invoke<void *>(context, at, blk, data);
        });
    }

    void channelVerify ( Channel * ch, Context * context, LineInfoArg * at ) {
        if ( !ch ) context->throw_error_at(at, "channelVerify: channel is null");
        ch->for_each_item([&](void * data, TypeInfo * ti, Context * ctx) {
            Context * vctx = ctx ? ctx : ch->getOwner();
            auto size = ti->firstType->size;
            size = (size + 15) & ~15;
            printf("verify %p of size %i (%i)\n", data, int(ti->firstType->size), int(size));
            if ( !vctx->heap->isValidPtr((char *) data, size) ) {
                os_debug_break();
                context->throw_error_at(at, "channelVerify: channel contains non-owned pointer");
            }
        });
    }

    vec4f channelPush ( Context & context, SimNode_CallBase * call, vec4f * args ) {
        auto ch = cast<Channel *>::to(args[0]);
        if ( !ch ) context.throw_error_at(call->debugInfo, "channelPush: channel is null");
        void * data = cast<void *>::to(args[1]);
        TypeInfo * ti = call->types[1];
        ch->push(data, ti, &context);
        return v_zero();
    }

    void channelPop ( Channel * ch, const TBlock<void,void*> & blk, Context * context, LineInfoArg * at ) {
        if ( !ch ) context->throw_error_at(at, "channelPop: channel is null");
        ch->pop(blk,context,at);
    }

    int channelAppend ( Channel * ch, int size, Context * context, LineInfoArg * at ) {
        if ( !ch ) context->throw_error_at(at, "channelAppend: channel is null");
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

    struct ChannelAnnotation : ManagedStructureAnnotation<Channel,false> {
        ChannelAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("Channel", ml) {
            addProperty<DAS_BIND_MANAGED_PROP(isEmpty)>("isEmpty");
            addProperty<DAS_BIND_MANAGED_PROP(isReady)>("isReady");
            addProperty<DAS_BIND_MANAGED_PROP(size)>("size");
            addProperty<DAS_BIND_MANAGED_PROP(total)>("total");
        }
        virtual int32_t getGcFlags(das_set<Structure *> &, das_set<Annotation *> &) const override {
            return TypeDecl::gcFlag_heap | TypeDecl::gcFlag_stringHeap;
        }
        virtual void walk(DataWalker & walker, void * data) override {
            BasicStructureAnnotation::walk(walker, data);
            Channel * ch = (Channel *) data;
            ch->for_each_item([&](void * data, TypeInfo * ti, Context *) {
                walker.walk((char *)&data, ti);
            });
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
            // types
            addAnnotation(make_smart<JobStatusAnnotation>(lib));
            auto cha = make_smart<ChannelAnnotation>(lib);
            cha->from("JobStatus");
            addAnnotation(cha);
            // channel
            addInterop<channelPush,void,Channel *,vec4f>(*this, lib,  "_builtin_channel_push",
                SideEffects::modifyArgumentAndExternal, "channelPush")
                    ->args({"channel","data"});
            addExtern<DAS_BIND_FUN(channelPop)>(*this, lib,  "_builtin_channel_pop",
                SideEffects::modifyArgumentAndExternal, "channelPop")
                    ->args({"channel","block","context","line"});
            addExtern<DAS_BIND_FUN(channelGather)>(*this, lib,  "_builtin_channel_gather",
                SideEffects::modifyArgumentAndExternal, "channelGather")
                    ->args({"channel","block","context","line"});
            addExtern<DAS_BIND_FUN(channelPeek)>(*this, lib,  "_builtin_channel_peek",
                SideEffects::modifyArgumentAndExternal, "channelPeek")
                    ->args({"channel","block","context","line"});
            addExtern<DAS_BIND_FUN(channelVerify)>(*this, lib,  "_builtin_channel_verify",
                SideEffects::modifyArgumentAndExternal, "channelGather")
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
            // job
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

