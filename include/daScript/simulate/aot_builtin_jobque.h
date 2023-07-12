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

    class Channel {
    public:
        Channel( Context * ctx ) : owner(ctx) {}
        Channel( Context * ctx, int count) : remaining(count), owner(ctx) {}
        ~Channel();
        Channel ( Channel && ) = delete;
        Channel ( const Channel & ) = delete;
        Channel & operator = ( const Channel & ) = delete;
        Channel & operator = ( Channel && ) = delete;
        void push ( void * data, TypeInfo * ti, Context * context );
        void pop ( const TBlock<void,void *> & blk, Context * context, LineInfoArg * at );
        bool isEmpty() const;
        int size() const;
        int total() const;
        bool isReady() const;
        void notify();
        void notifyAndRelease();
        void wait();
        int append(int size);
        int addRef() { return mRef++; }
        int releaseRef() { return --mRef; }
        Context * getOwner() { return owner; }
    public:
        template <typename TT>
        void for_each_item ( const TT & tt ) {
            lock_guard<mutex> guard(lock);
            for ( auto & f : pipe ) {
                tt(f.data, f.type, f.from.get());
            }
        }
        template <typename TT>
        void gather ( const TT & tt ) {
            lock_guard<mutex> guard(lock);
            for ( auto & f : pipe ) {
                tt(f.data, f.type, f.from.get());
            }
            pipe.clear();
        }

    protected:
        uint32_t            mSleepMs = 1;
        mutable mutex       lock;
        deque<Feature>      pipe;
        Feature             tail;
        uint32_t			remaining = 0;
        condition_variable	cond;
        Context *           owner = nullptr;
        atomic<int>         mRef{0};
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
    void waitForJob ( JobStatus * status, Context * context, LineInfoArg * at );
    void notifyJob ( JobStatus * status, Context * context, LineInfoArg * at );
    void notifyAndReleaseJob ( JobStatus * & status, Context * context, LineInfoArg * at );
    vec4f channelPush ( Context & context, SimNode_CallBase * call, vec4f * args );
    void channelPop ( Channel * ch, const TBlock<void,void*> & blk, Context * context, LineInfoArg * at );
    int channelAppend ( Channel * ch, int size, Context * context, LineInfoArg * at );
    void withChannel ( const TBlock<void,Channel *> & blk, Context * context, LineInfoArg * lineinfo );
    void withChannelEx ( int32_t count, const TBlock<void,Channel *> & blk, Context * context, LineInfoArg * lineinfo );
    Channel* channelCreate( Context * context, LineInfoArg * at);
    void channelRemove(Channel * ch, Context * context, LineInfoArg * at);
    void channelAddRef ( Channel * ch, Context * context, LineInfoArg * at );
    void channelReleaseRef ( Channel * & ch, Context * context, LineInfoArg * at );
    void waitForChannel ( Channel * status, Context * context, LineInfoArg * at );
    void notifyChannel ( Channel * status, Context * context, LineInfoArg * at );
    void notifyAndReleaseChannel ( Channel * & status, Context * context, LineInfoArg * at );
    void channelGather ( Channel * ch, const TBlock<void,void *> & blk, Context * context, LineInfoArg * at );
    void channelPeek ( Channel * ch, const TBlock<void,void *> & blk, Context * context, LineInfoArg * at );
    void channelVerify ( Channel * ch, Context * context, LineInfoArg * at );
}
