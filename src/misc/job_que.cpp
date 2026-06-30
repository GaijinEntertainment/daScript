#include "daScript/misc/platform.h"

#include "daScript/misc/job_que.h"
#include "daScript/simulate/debug_info.h"
#include "daScript/simulate/aot_builtin_jobque.h"
#include "daScript/misc/string_writer.h"

#if defined(__APPLE__)
#include <sys/sysctl.h>
#endif
#if defined(__EMSCRIPTEN__)
#include <emscripten/threading.h>   // emscripten_num_logical_cores
#endif

// Feature tracking statics
das::Feature *  das::Feature::sTrackHead = nullptr;
das::mutex      das::Feature::sTrackMutex;

namespace das {

#if defined(__APPLE__)
    // Performance ("good") core count on heterogeneous Apple Silicon (P-cores). Returns 0 on homogeneous
    // Macs (Intel) and wherever the keys are missing, so the caller falls back to the generic rule.
    static int apple_perf_core_count() {
        int nperf = 0; size_t len = sizeof(nperf);
        if ( sysctlbyname("hw.nperflevels", &nperf, &len, nullptr, 0) != 0 || nperf < 2 ) return 0;
        int good = 0; len = sizeof(good);
        if ( sysctlbyname("hw.perflevel0.logicalcpu", &good, &len, nullptr, 0) != 0 ) return 0;
        return good;
    }
#endif

    // Worker count. Generic rule: logical cores - 1, capped at DAS_MAX_HW_JOBS — parallel_for has the
    // CALLING (main) thread run a chunk and then wait, so it occupies a core; cores-1 workers + main ==
    // cores avoids the oversubscription that otherwise makes the surplus worker "trickle in" over the
    // first ~37% of every matmul. DAS_MAX_HW_JOBS (default 4; raise via -DDAS_MAX_HW_JOBS=N) keeps a
    // wasm build from spawning one Web Worker per logical core. On heterogeneous Apple Silicon we
    // instead run one worker per PERFORMANCE core (no -1, no cap): the efficiency cores absorb the main
    // thread + OS, whereas cores-1 (logical) spills a worker onto a slow E-core that stalls every
    // parallel_for on the straggler — measured ~1.6x slower on an M-series 8P+2E prefill. DAS_JOBQUE_THREADS
    // is an explicit override that bypasses everything (0/unset = the default).
    static int jobque_thread_count(int hw) {
        static int forced = []{ const char * e = getenv("DAS_JOBQUE_THREADS"); return e ? atoi(e) : 0; }();
        if ( forced > 0 ) return forced;
#if defined(__APPLE__)
        if ( int good = apple_perf_core_count() ) return good;
#endif
        return max(1, min(DAS_MAX_HW_JOBS, hw - 1));
    }

#if defined(_MSC_VER) && !defined(_GAMING_XBOX) && !defined(_DURANGO) && !defined(_M_ARM64)

    int GetLogicalProcessorCountInWindows();

    int JobQue::get_num_threads() {
        int hw = GetLogicalProcessorCountInWindows();
        if ( hw <= 0 ) hw = max(1,static_cast<int>(thread::hardware_concurrency()));
        return jobque_thread_count(hw);
    }


#else

    int JobQue::get_num_threads() {
#if defined(__EMSCRIPTEN__)
        // emscripten's std::thread::hardware_concurrency() does NOT reflect
        // navigator.hardwareConcurrency in the wasm64/memory64 build (it returns a
        // low default ~2); emscripten_num_logical_cores() reads it directly and is
        // correct on both wasm32/64.
        int hw = emscripten_num_logical_cores();
#else
        int hw = static_cast<int>(thread::hardware_concurrency());
#endif
        return jobque_thread_count(max(1, hw));
    }

#endif

    JobQue::JobQue()
        : mSleepMs(1)
        , mShutdown(false)
        , mThreadCount( 0 )
        , mJobsRunning(0) {
        mThreadCount = get_num_threads();
        SetCurrentThreadPriority(JobPriority::High);
        for (int j = 0, js = mThreadCount; j < js; j++) {
            mThreads.emplace_back(make_unique<thread>([this, j]() {
                string thread_name = "JobQue_Job_" + to_string(j);
                SetCurrentThreadName(thread_name);
                job(j);
            }));
        }
    }

    JobQue::~JobQue () {
        join();
    }

    void JobQue::EvalOnMainThread(Job && expr) {
        lock_guard<mutex> guard(mEvalMainThreadMutex);
        mEvalMainThread.emplace_back(das::move(expr));
    }

    void JobQue::EvalMainThreadJobs() {
        vector<Job> results;
        {
            lock_guard<mutex> guard(mEvalMainThreadMutex);
            swap(results, mEvalMainThread);
        }
        for (auto & job : results) {
            job();
        }
    }

    void JobQue::wait() {
        while ( !isEmpty(true) ) {
            while ( !isEmpty() ) {
                this_thread::yield();
            }
            EvalMainThreadJobs();
        }
    }

    void JobQue::join() {
        {
            // Set the flag and wake every parked worker under the lock so none misses the wakeup.
            lock_guard<mutex> lock(mFifoMutex);
            mShutdown = true;
            mCond.notify_all();
        }
        while ( mThreadCount ) {
            this_thread::yield();
        }
        for (auto & th : mThreads) {
            th.threadPointer->join();
        }
        mThreads.clear();
    }

    bool JobQue::isEmpty ( bool includingMainThreadJobs ) {
        lock_guard<mutex> lock(mFifoMutex);
        bool queue_is_empty = (mJobsRunning == 0) && (mFifo.size() == 0);
        if ( includingMainThreadJobs ) {
            lock_guard<mutex> mainThreadLock(mEvalMainThreadMutex);
            return queue_is_empty && mEvalMainThread.empty();
        }
        return queue_is_empty;
    }

    bool JobQue::areJobsPending(JobCategory category) {
        lock_guard<mutex> lock(mFifoMutex);
        if (find_if(mFifo.begin(), mFifo.end(), [=](const JobEntry& jobEntry) {
                return jobEntry.category == category; }) != mFifo.end()) {
            return true;
        }
        if (find_if(mThreads.begin(), mThreads.end(), [=](const ThreadEntry& threadEntry) {
                return threadEntry.currentPriority != JobPriority::Inactive && threadEntry.currentCategory == category; }) != mThreads.end()) {
            return true;
        }
        return false;
    }

    int JobQue::getTotalHwJobs() {
        return mThreadCount;
    }

    int JobQue::getNumberOfQueuedJobs() {
        lock_guard<mutex> lock(mFifoMutex);
        return int(mFifo.size());
    }

    void JobQue::submit(Job && job, JobCategory category, JobPriority priority) {
        auto  it = lower_bound(mFifo.begin(), mFifo.end(), priority, [](const JobEntry& lhs, JobPriority priority) {
            return lhs.priority >= priority; });
        mFifo.emplace(it, das::move(job), category, priority);
    }

    void JobQue::push(Job && job, JobCategory category, JobPriority priority) {
        lock_guard<mutex> lock(mFifoMutex);
        submit(das::move(job), category, priority);
        mCond.notify_one();
    }

    void JobQue::job(int threadIndex) {
        while (!mShutdown) {
            Job job;
            {
                unique_lock<mutex> lock(mFifoMutex);
                // Block until a job is available or we're shutting down. A plain wait (no periodic
                // timeout) means idle workers stay parked instead of waking every mSleepMs to grab
                // mFifoMutex and yield — that periodic wakeup contended on exactly the lock the
                // dispatch path needs, throttling parallel_for when most workers are idle between
                // a token's many small matmuls. push()/parallel_for already notify, so latency is
                // unaffected; join() sets mShutdown under the lock and notifies to wake all workers.
                mCond.wait(lock, [&]() { return mFifo.size() != 0 || mShutdown.load(); });
                if ( mShutdown ) break;
                DAS_VERIFYF(mFifo.size() > 0, "There must be at least one job available");
                job = das::move(mFifo.front().function);
                mThreads[threadIndex].currentPriority = mFifo.front().priority;
                mThreads[threadIndex].currentCategory = mFifo.front().category;
                mFifo.pop_front();
                mJobsRunning++;
            }
            // Only touch the OS thread priority when it actually changes — every job otherwise pays
            // a pthread_setschedparam syscall, and a parallel_for fires the same priority on every
            // chunk (~thousands of redundant syscalls per LLM token). Only this worker writes its own
            // appliedPriority, so no lock needed.
            if ( mThreads[threadIndex].currentPriority != mThreads[threadIndex].appliedPriority ) {
                SetCurrentThreadPriority(mThreads[threadIndex].currentPriority);
                mThreads[threadIndex].appliedPriority = mThreads[threadIndex].currentPriority;
            }
            job();
            {
                unique_lock<mutex> lock(mFifoMutex);
                mThreads[threadIndex].currentPriority = JobPriority::Inactive;
                mJobsRunning--;
            }
        }
        mThreadCount--;
    }

    void JobQue::parallel_for ( JobStatus & status, int from, int to, const JobChunk & chunk,
            JobCategory category, JobPriority priority, int chunk_count, int step ) {
        if ( from >= to ) return;
        if ( chunk_count==-1 ) chunk_count = mThreadCount * 4;
        step = max ( ( to - from + 1 ) / chunk_count, step );
        int numChunks = (to - from + step) / step;
        if ( numChunks==1 ) {
            chunk(from, to);
            return;
        }
        int onMainThread = max ( (numChunks + mThreadCount)  / (mThreadCount+1), 1 );
        int onThreads  = numChunks - onMainThread;
        status.Clear(onThreads);
        {
            lock_guard<mutex> lock(mFifoMutex);
            for (int ch = 0; ch < onThreads; ++ch) {
                int i0 = from + ch * step;
                int i1 = i0 + step;
                submit([=,&status](){
                    chunk(i0, i1);
                    status.Notify();
                }, category, priority);
            }
            mCond.notify_all();
        }
        chunk(from + onThreads * step, to);
    }

    void JobQue::parallel_for ( int from, int to, const JobChunk & chunk, JobCategory category, JobPriority priority, int chunk_count, int step ) {
        JobStatus status;
        parallel_for(status, from, to, chunk, category, priority, chunk_count, step);
        status.Wait();
    }

    void JobQue::parallel_for_with_consume(int from, int to, const JobChunk & chunk, const JobChunk & consume,
        JobCategory category, JobPriority priority, int chunk_count, int step) {
        if (from >= to) return;
        if (chunk_count == -1) chunk_count = mThreadCount * 4;
        step = max((to - from) / chunk_count, step);
        int numChunks = (to - from) / step;
        if (numChunks == 1) {
            chunk(from, to);
            consume(from, to);
            return;
        }
        deque<Job> producerFifoJobs;
        mutex producerFifoMutex;
        condition_variable condition;
        {
            lock_guard<mutex> lock(mFifoMutex);
            for (int ch = 0; ch < numChunks; ++ch) {
                int i0 = from + ch * step;
                int i1 = min(i0 + step, to);
                submit([=, &chunk, &producerFifoJobs, &producerFifoMutex, &condition]() {
                    chunk(i0, i1);
                    {
                        lock_guard<mutex> producerFifoLock(producerFifoMutex);
                        producerFifoJobs.push_back(([=]() { consume(i0, i1); }));
                        condition.notify_one();
                    }
                }, category, priority);
            }
            mCond.notify_all();
        }
        {
            int chunksRemaining = numChunks;
            while (chunksRemaining > 0) {
                deque<Job> consumerFifoJobs;
                {
                    unique_lock<mutex> producerFifoLock(producerFifoMutex);
                    if (condition.wait_for(producerFifoLock, chrono::milliseconds(mSleepMs), [&]() {return producerFifoJobs.size() > 0; })) {
                        consumerFifoJobs.swap(producerFifoJobs);
                    } else {
                        this_thread::yield();
                        continue;
                    }
                }
                for (auto & job : consumerFifoJobs) {
                    job();
                    --chunksRemaining;
                }
            }
        }
    }

    // JobStatus tracking

    void JobStatus::trackInsert() {
        lock_guard<mutex> guard(sTrackMutex);
        mTrackId = ++g_jobque_track_total;
        mTrackNext = sTrackHead;
        mTrackPrev = nullptr;
        if (sTrackHead) sTrackHead->mTrackPrev = this;
        sTrackHead = this;
    }

    void JobStatus::trackRemove() {
        lock_guard<mutex> guard(sTrackMutex);
        if (mTrackPrev) mTrackPrev->mTrackNext = mTrackNext;
        else sTrackHead = mTrackNext;
        if (mTrackNext) mTrackNext->mTrackPrev = mTrackPrev;
    }

    void JobStatus::trackEvent ( LineInfo * at, bool isAddRef ) {
        if ( !at ) return;
        if ( mTrackId != g_jobque_track_id.load() ) return;
        TextPrinter tp;
        tp << "JobStatus #" << HEX << mTrackId << DEC;
        if ( mTrackMagic == TRACK_CHANNEL ) tp << " (Channel)";
        else if ( mTrackMagic == TRACK_LOCKBOX ) tp << " (LockBox)";
        else if ( mTrackMagic == TRACK_STREAM ) tp << " (Stream)";
        else tp << " (JobStatus)";
        tp << (isAddRef ? " addRef" : " releaseRef")
           << " (rc=" << int(mRef.load()) << ")";
        tp << " at " << at->describe() << "\n";
    }

    int JobStatus::addRef( LineInfo * at ) {
        trackEvent(at, true);
        return mRef++;
    }

    int JobStatus::releaseRef( LineInfo * at ) {
        trackEvent(at, false);
        return --mRef;
    }

    // JobStatus constructors/destructor

    JobStatus::JobStatus() {
        mTrackMagic = TRACK_JOB_STATUS;
        trackInsert();
    }

    JobStatus::JobStatus(uint32_t count) {
        mTrackMagic = TRACK_JOB_STATUS;
        trackInsert();
        Clear(count);
    }

    JobStatus::~JobStatus() {
        DAS_VERIFY(mMagic==uint32_t(STATUS_MAGIC));
        DAS_VERIFY(mRef==0);
        mMagic = 0;
        trackRemove();
    }

    bool JobStatus::Notify() {
        lock_guard<mutex> guard(mCompleteMutex);
        if ( mRemaining == 0 ) return false;
        --mRemaining;
        if ( mRemaining==0 ) {
            mCond.notify_all();
        }
        return true;
    }

    bool JobStatus::NotifyAndRelease( LineInfo * at ) {
        lock_guard<mutex> guard(mCompleteMutex);
        trackEvent(at, false);
        mRef--;
        if ( mRemaining == 0 ) return false;
        --mRemaining;
        if ( mRemaining==0 ) {
            mCond.notify_all();
        }
        return true;
    }

    void JobStatus::Wait() {
#if defined(__EMSCRIPTEN__) && !defined(__EMSCRIPTEN_PTHREADS__)
        // Single-threaded wasm (no -pthread): nothing can decrement mRemaining
        // except the calling thread, which is the very thread blocked here — a
        // condition_variable wait would deadlock forever. join()/Wait() exist to
        // fence against a worker thread that does not exist in this build, so the
        // wait is a no-op. (A miniaudio Web Audio callback that would otherwise
        // drain a Stream/LockBox runs as a separate main-thread task and likewise
        // cannot make progress while we block, so there is nothing to wait for.)
        return;
#else
        unique_lock<mutex> lock(mCompleteMutex);
        mCond.wait(lock, [this] {
            return mRemaining==0;
        });
#endif
    }

    bool JobStatus::isReady() {
        lock_guard<mutex> guard(mCompleteMutex);
        return mRemaining==0;
    }

    void JobStatus::Clear(uint32_t count) {
        lock_guard<mutex> guard(mCompleteMutex);
        mRemaining = count;
    }

    void JobStatus::DumpJobQueLeaks() {
        {
            lock_guard<mutex> guard(sTrackMutex);
            TextPrinter tp;
            int total = 0;
            for ( auto p = sTrackHead; p; p = p->mTrackNext ) {
                tp << "  JobStatus #" << HEX << p->mTrackId << DEC
                   << " (rc=" << int(p->mRef.load()) << ")";
                if ( p->mTrackMagic == TRACK_CHANNEL ) tp << " Channel";
                else if ( p->mTrackMagic == TRACK_LOCKBOX ) tp << " LockBox";
                else if ( p->mTrackMagic == TRACK_STREAM ) tp << " Stream";
                else tp << " JobStatus";
                if ( !p->mCreatedAt.empty() ) tp << " created at " << p->mCreatedAt;
                tp << "\n";
                total++;
            }
            if ( total ) tp << "total " << total << " leaked JobStatus objects\n";
        }
        {
            lock_guard<mutex> guard(Feature::sTrackMutex);
            TextPrinter tp;
            int total = 0;
            for ( auto f = Feature::sTrackHead; f; f = f->fTrackNext ) {
                tp << "  Feature #" << HEX << f->fTrackId << DEC;
                if ( f->fOwner ) {
                    tp << " owner=#" << HEX << f->fOwnerTrackId << DEC;
                }
                if ( !f->fCreatedAt.empty() ) tp << " created at " << f->fCreatedAt;
                tp << "\n";
                total++;
            }
            if ( total ) tp << "total " << total << " leaked Feature objects\n";
        }
    }

    uint64_t JobStatus::CountJobQueLeaks() {
        uint64_t total = 0;
        {
            lock_guard<mutex> guard(sTrackMutex);
            for ( auto p = sTrackHead; p; p = p->mTrackNext ) total++;
        }
        {
            lock_guard<mutex> guard(Feature::sTrackMutex);
            for ( auto f = Feature::sTrackHead; f; f = f->fTrackNext ) total++;
        }
        return total;
    }

    // Feature tracking

    void Feature::trackInsert() {
        lock_guard<mutex> guard(sTrackMutex);
        fTrackId = ++g_jobque_track_total;
        fTrackNext = sTrackHead;
        fTrackPrev = nullptr;
        if (sTrackHead) sTrackHead->fTrackPrev = this;
        sTrackHead = this;
    }

    void Feature::trackRemove() {
        lock_guard<mutex> guard(sTrackMutex);
        if (fTrackPrev) fTrackPrev->fTrackNext = fTrackNext;
        else sTrackHead = fTrackNext;
        if (fTrackNext) fTrackNext->fTrackPrev = fTrackPrev;
    }

    Feature::Feature() {
        trackInsert();
    }

    Feature::Feature ( void * d, TypeInfo * ti, Context * c ) : data(d), type(ti) {
        trackInsert();
        setFrom(c);
    }

    Feature::~Feature() {
        trackRemove();
    }

    Feature::Feature ( const Feature & f )
        : data(f.data), type(f.type), from(f.from), fromShared(f.fromShared) {
        fCreatedAt = f.fCreatedAt;
        fOwner = f.fOwner;
        fOwnerTrackId = f.fOwnerTrackId;
        trackInsert();
    }

    Feature::Feature ( Feature && f )
        : data(f.data), type(f.type), from(f.from), fromShared(das::move(f.fromShared)) {
        fCreatedAt = f.fCreatedAt;
        fOwner = f.fOwner;
        fOwnerTrackId = f.fOwnerTrackId;
        f.data = nullptr;
        f.type = nullptr;
        f.from = nullptr;
        trackInsert();
    }

    Feature & Feature::operator = ( const Feature & f ) {
        if ( this != &f ) {
            data = f.data;
            type = f.type;
            from = f.from;
            fromShared = f.fromShared;
            fCreatedAt = f.fCreatedAt;
            fOwner = f.fOwner;
            fOwnerTrackId = f.fOwnerTrackId;
        }
        return *this;
    }

    Feature & Feature::operator = ( Feature && f ) {
        if ( this != &f ) {
            data = f.data;
            type = f.type;
            from = f.from;
            fromShared = das::move(f.fromShared);
            fCreatedAt = f.fCreatedAt;
            fOwner = f.fOwner;
            fOwnerTrackId = f.fOwnerTrackId;
            f.data = nullptr;
            f.type = nullptr;
            f.from = nullptr;
        }
        return *this;
    }

    void Feature::setFrom ( Context * c ) {
        from = c;
        if ( c && c->sharedPtrContext ) {
            // Standalone-AOT/exe contexts are member objects (not make_shared), so their
            // weak_from_this() is empty and shared_from_this() throws bad_weak_ptr. Fall back
            // to a null owner: such a context lives for the whole program, so the Feature never
            // needs to keep it alive. Mirrors debugAgentContextOrNull in context.cpp.
            auto wp = c->weak_from_this();
            fromShared = wp.expired() ? nullptr : wp.lock();
        } else {
            fromShared.reset();
        }
    }

    void Feature::clear() {
        data = nullptr;
        type = nullptr;
        from = nullptr;
        fromShared.reset();
    }

    void Feature::DumpFeatures() {
        lock_guard<mutex> guard(sTrackMutex);
        TextPrinter tp;
        int total = 0;
        for ( auto f = sTrackHead; f; f = f->fTrackNext ) {
            tp << "  Feature #" << HEX << f->fTrackId << DEC;
            if ( f->fOwner ) {
                tp << " owner=#" << HEX << f->fOwnerTrackId << DEC;
            }
            if ( !f->fCreatedAt.empty() ) tp << " created at " << f->fCreatedAt;
            tp << "\n";
            total++;
        }
        if ( total ) tp << "total " << total << " tracked Feature objects\n";
    }
}

#if defined(__APPLE__)

namespace das {
    void SetCurrentThreadName ( const string & str ) {
        pthread_setname_np(str.c_str());
    }

    void SetCurrentThreadPriority ( JobPriority _priority ) {
        int priority = int(_priority);
        float minPlatformPriority = sched_get_priority_min(SCHED_OTHER);
        float maxPlatformPriority = sched_get_priority_max(SCHED_OTHER);
        struct sched_param sched_param;
        int platformPriority = (int)(minPlatformPriority + (maxPlatformPriority - minPlatformPriority)
            * ((float)(priority - int(JobPriority::Minimum)) / (float)(int(JobPriority::Maximum) - int(JobPriority::Minimum))));
        sched_param.sched_priority = platformPriority;
        pthread_setschedparam(pthread_self(), SCHED_OTHER, &sched_param);
    }
}

#elif defined(_MSC_VER)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
    DWORD dwType;       // Must be 0x1000.
    LPCSTR szName;      // Pointer to name (in user addr space).
    DWORD dwThreadID;   // Thread ID (-1=caller thread).
    DWORD dwFlags;      // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

namespace das {
    // http://msdn.microsoft.com/en-us/library/xcb2z8hs(VS.90).aspx
    void SetCurrentThreadName ( const string & str ) {
        DWORD dwThreadID = GetCurrentThreadId();
        THREADNAME_INFO info;
        info.dwType = 0x1000;
        info.szName = str.c_str();
        info.dwThreadID = dwThreadID;
        info.dwFlags = 0;
        __try {
            const DWORD MS_VC_EXCEPTION = 0x406D1388;
            RaiseException( MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info );
        } __except(EXCEPTION_EXECUTE_HANDLER) {
        }
    }

    void SetCurrentThreadPriority ( JobPriority priority ) {
        int winPriority = THREAD_PRIORITY_NORMAL;
        switch (priority) {
        case JobPriority::Minimum:  winPriority = THREAD_PRIORITY_LOWEST; break;
        case JobPriority::Low:      winPriority = THREAD_PRIORITY_BELOW_NORMAL; break;
        case JobPriority::Medium:   winPriority = THREAD_PRIORITY_NORMAL; break;
        case JobPriority::High:     winPriority = THREAD_PRIORITY_ABOVE_NORMAL; break;
        case JobPriority::Maximum:  winPriority = THREAD_PRIORITY_HIGHEST; break;
        default:                    DAS_VERIFYF(0, "Windows takes prefixed priority values"); break;
        }
        SetThreadPriority(GetCurrentThread(), winPriority);
    }
}

#elif defined(__linux__) || defined __HAIKU__

#include <pthread.h>

namespace das
{
    void SetCurrentThreadName ( const string & str ) {
        pthread_setname_np(pthread_self(), str.c_str());
    }

    void SetCurrentThreadPriority ( JobPriority _priority ) {
        int priority = int(_priority);
        float minPlatformPriority = sched_get_priority_min(SCHED_OTHER);
        float maxPlatformPriority = sched_get_priority_max(SCHED_OTHER);
        struct sched_param sched_param;
        int platformPriority = (int)( minPlatformPriority + (maxPlatformPriority - minPlatformPriority)
            * ((float)(priority - int(JobPriority::Minimum)) / (float)(int(JobPriority::Maximum) - int(JobPriority::Minimum))));
        sched_param.sched_priority = platformPriority;
        pthread_setschedparam(pthread_self(), SCHED_OTHER, &sched_param);
    }
}

#else

namespace das
{
    void SetCurrentThreadName ( const string & ) {}
    void SetCurrentThreadPriority ( JobPriority ) {}
}

#endif
