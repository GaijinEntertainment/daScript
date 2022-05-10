#pragma once

#include "daScript/misc/job_que.h"
#include "daScript/simulate/simulate.h"

#include <queue>

namespace das {

    struct Feature {
        void *              data = nullptr;
        shared_ptr<Context> from;
        Feature() {}
        __forceinline Feature ( void * d, Context * c) : data(d), from(c ? c->shared_from_this() : nullptr) {}
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
        void push ( void * data, Context * context );
        void * pop();
        bool isEmpty() const;
        int size() const;
        bool isReady() const;
        void notify();
        void wait();
        int append(int size);
    protected:
        uint32_t            mSleepMs = 1;
        mutable mutex       lock;
        queue<Feature>      pipe;
        Feature             tail;
        uint32_t			remaining = 0;
        condition_variable	cond;
        Context *           owner = nullptr;
    };

    bool is_job_que_shutting_down();
    void new_job_invoke ( Lambda lambda, Func fn, int32_t lambdaSize, Context * context, LineInfoArg * lineinfo );
    void new_thread_invoke ( Lambda lambda, Func fn, int32_t lambdaSize, Context * context, LineInfoArg * lineinfo );
    void withJobQue ( const TBlock<void> & block, Context * context, LineInfoArg * lineInfo );
    int getTotalHwJobs( Context * context, LineInfoArg * at );
    int getTotalHwThreads ();
    void withJobStatus ( int32_t total, const TBlock<void,JobStatus *> & block, Context * context, LineInfoArg * lineInfo );
    void waitForJob ( JobStatus * status );
    void notifyJob ( JobStatus * status );
    void channelPush ( Channel * ch, void * data, Context * ctx );
    void * channelPop ( Channel * ch );
    int channelAppend ( Channel * ch, int size );
    void withChannel ( const TBlock<void,Channel *> & blk, Context * context, LineInfoArg * lineinfo );
    void withChannelEx ( int32_t count, const TBlock<void,Channel *> & blk, Context * context, LineInfoArg * lineinfo );
    void waitForChannel ( Channel * status );
    void notifyChannel ( Channel * status );
}
