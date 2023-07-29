#pragma once

#include "daScript/misc/job_que.h"
#include "daScript/simulate/simulate.h"

#include <queue>

namespace das {

    struct Feature {
        void *              data = nullptr;
        TypeInfo *          type = nullptr;
        shared_ptr<Context> from;
        Feature() {}
        __forceinline Feature ( void * d, TypeInfo * ti, Context * c) : data(d), type(ti), from(c ? c->shared_from_this() : nullptr) {}
        __forceinline void clear() {
            data = nullptr;
            from.reset();
        }
    };

    class LockBox : public JobStatus {
    public:
        LockBox() {}
        virtual ~LockBox();
        void set ( void * data, TypeInfo * ti, Context * context );
        void get ( const TBlock<void,void *> & blk, Context * context, LineInfoArg * at );
    public:
        template <typename TT>
        void peek ( TT && tt ) {
            lock_guard<mutex> guard(mCompleteMutex);
            if ( box.data ) {
                tt(box.data, box.type, box.from.get());
            }
        }
    protected:
        Feature box;
    };

    class Channel : public JobStatus {
    public:
        Channel( Context * ctx ) : owner(ctx) {}
        Channel( Context * ctx, int count) : owner(ctx) { mRemaining = count; }
        virtual ~Channel();
        void push ( void * data, TypeInfo * ti, Context * context );
        void pop ( const TBlock<void,void *> & blk, Context * context, LineInfoArg * at );
        bool isEmpty() const;
        int total() const;
        Context * getOwner() { return owner; }
    public:
        template <typename TT>
        void for_each_item ( TT && tt ) {
            lock_guard<mutex> guard(mCompleteMutex);
            for ( auto & f : pipe ) {
                tt(f.data, f.type, f.from.get());
            }
        }
        template <typename TT>
        void gather ( TT && tt ) {
            lock_guard<mutex> guard(mCompleteMutex);
            for ( auto & f : pipe ) {
                tt(f.data, f.type, f.from.get());
            }
            pipe.clear();
        }
    protected:
        uint32_t            mSleepMs = 1;
        deque<Feature>      pipe;
        Feature             tail;
        Context *           owner = nullptr;
    };

    bool is_job_que_shutting_down();
    void new_job_invoke ( Lambda lambda, Func fn, int32_t lambdaSize, Context * context, LineInfoArg * lineinfo );
    void new_thread_invoke ( Lambda lambda, Func fn, int32_t lambdaSize, Context * context, LineInfoArg * lineinfo );
    void withJobQue ( const TBlock<void> & block, Context * context, LineInfoArg * lineInfo );
    int getTotalHwJobs( Context * context, LineInfoArg * at );
    int getTotalHwThreads ();
    void withJobStatus ( int32_t total, const TBlock<void,JobStatus *> & block, Context * context, LineInfoArg * lineInfo );
    void jobStatusAddRef ( JobStatus * status, Context * context, LineInfoArg * at );
    void jobStatusReleaseRef ( JobStatus * & status, Context * context, LineInfoArg * at );
    JobStatus * jobStatusCreate( Context * context, LineInfoArg * );
    void jobStatusRemove( JobStatus * ch, Context * context, LineInfoArg * at );
    void waitForJob ( JobStatus * status, Context * context, LineInfoArg * at );
    void notifyJob ( JobStatus * status, Context * context, LineInfoArg * at );
    void notifyAndReleaseJob ( JobStatus * & status, Context * context, LineInfoArg * at );
    vec4f channelPush ( Context & context, SimNode_CallBase * call, vec4f * args );
    void channelPop ( Channel * ch, const TBlock<void,void*> & blk, Context * context, LineInfoArg * at );
    int jobAppend ( JobStatus * ch, int size, Context * context, LineInfoArg * at );
    void withChannel ( const TBlock<void,Channel *> & blk, Context * context, LineInfoArg * lineinfo );
    void withChannelEx ( int32_t count, const TBlock<void,Channel *> & blk, Context * context, LineInfoArg * lineinfo );
    Channel* channelCreate( Context * context, LineInfoArg * at);
    void channelRemove(Channel * ch, Context * context, LineInfoArg * at);
    void channelGather ( Channel * ch, const TBlock<void,void *> & blk, Context * context, LineInfoArg * at );
    void channelPeek ( Channel * ch, const TBlock<void,void *> & blk, Context * context, LineInfoArg * at );
    void channelVerify ( Channel * ch, Context * context, LineInfoArg * at );
    LockBox * lockBoxCreate( Context *, LineInfoArg * );
    void lockBoxRemove( LockBox * ch, Context * context, LineInfoArg * at );
    void withLockBox ( const TBlock<void,LockBox *> & blk, Context * context, LineInfoArg * at );
    vec4f lockBoxSet ( Context & context, SimNode_CallBase * call, vec4f * args );
    void lockBoxGet ( LockBox * ch, const TBlock<void,void*> & blk, Context * context, LineInfoArg * at );
}
