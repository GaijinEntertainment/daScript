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

#if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_IX86))
#include <emmintrin.h>  // _mm_pause
#elif defined(_MSC_VER) && defined(_M_ARM64)
#include <intrin.h>     // __yield
#endif

namespace das {
    // A single CPU spin-wait hint — the poll-loop relax (one PAUSE/YIELD, the ggml
    // ggml_thread_cpu_relax shape; the loop itself is a counted number of these).
    static inline void jobque_spin_relax() {
#if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_IX86))
        _mm_pause();
#elif defined(_MSC_VER) && defined(_M_ARM64)
        __yield();
#elif defined(__x86_64__) || defined(__i386__)
        __builtin_ia32_pause();
#elif defined(__aarch64__)
        __asm__ __volatile__("yield");
#else
        ;
#endif
    }

    // One short burst of the CPU's spin-wait hint (worker spin-before-park, see JobQue::job).
    static inline void jobque_spin_pause() {
#if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_IX86))
        for ( int i = 0; i != 64; ++i ) _mm_pause();
#elif defined(_MSC_VER) && defined(_M_ARM64)
        for ( int i = 0; i != 64; ++i ) __yield();
#elif defined(__x86_64__) || defined(__i386__)
        for ( int i = 0; i != 64; ++i ) __builtin_ia32_pause();
#elif defined(__aarch64__)
        for ( int i = 0; i != 64; ++i ) __asm__ __volatile__("yield");
#else
        this_thread::yield();
#endif
    }
}

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

    // Worker count. Generic rule: logical cores - 1, capped at DAS_MAX_HW_JOBS — parallel_for's
    // CALLING (main) thread now executes queued chunks itself (jobque_try_run_one), so it IS a
    // compute thread and occupies a core; cores-1 workers + main == cores avoids the oversubscription
    // that otherwise makes the surplus thread "trickle in" over the first ~37% of every matmul.
    // DAS_MAX_HW_JOBS (4 on wasm so a web build doesn't spawn one Web Worker per logical core;
    // effectively uncapped elsewhere). On heterogeneous Apple Silicon the same -1 applies to the
    // PERFORMANCE-core count: P-1 workers + the computing main == P. Landing any compute thread on a
    // slow E-core stalls every parallel_for on its straggler chunk (measured ~1.6x slower on an
    // M-series 8P+2E prefill vs P-only) — pre-main-steal that ruled out logical cores-1 here; now
    // that the main thread computes too, the -1 is what keeps every compute thread on a P-core.
    // DAS_JOBQUE_THREADS is an explicit override that bypasses everything (0/unset = the default).
    static int jobque_thread_count(int hw) {
        static int forced = []{ const char * e = getenv("DAS_JOBQUE_THREADS"); return e ? atoi(e) : 0; }();
        if ( forced > 0 ) return forced;
#if defined(__APPLE__)
        if ( int good = apple_perf_core_count() ) return max(1, good - 1);
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
        mFifoCount.fetch_add(1, std::memory_order_relaxed);
    }

    void JobQue::push(Job && job, JobCategory category, JobPriority priority) {
        lock_guard<mutex> lock(mFifoMutex);
        submit(das::move(job), category, priority);
        mCond.notify_one();
    }

    void JobQue::pushBatch(vector<Job> && jobs, JobCategory category, JobPriority priority) {
        {
            lock_guard<mutex> lock(mFifoMutex);
            for ( auto & j : jobs ) {
                submit(das::move(j), category, priority);
            }
        }
        // ONE wake, not notify_all: waking N parked threads is ~N serialized OS wakes (~5-8us each)
        // no matter who calls it. Workers propagate instead — every worker that pops and leaves work
        // behind wakes the next (see job()), so the wake tree fans out with log depth and the wake
        // syscalls are paid by the (parallel) workers, not the flushing thread.
        mCond.notify_one();
        jobs.clear();
    }

    bool JobQue::tryRunOneJob() {
        // Dispatcher-side work stealing: a thread sitting at a fork/join (parallel_for) pops queued
        // jobs and runs them itself instead of sleeping — the job closures are thread-agnostic (the
        // same closures any worker would run), so the only difference is who executes. Wake
        // propagation mirrors job(): if our pop left work behind, kick parked workers.
        Job job;
        size_t moreWork = 0;
        {
            lock_guard<mutex> lock(mFifoMutex);
            if ( mFifo.empty() ) return false;
            job = das::move(mFifo.front().function);
            mFifo.pop_front();
            mFifoCount.fetch_sub(1, std::memory_order_relaxed);
            mJobsRunning++;
            moreWork = mFifo.size();
        }
        if ( moreWork >= 1 ) mCond.notify_one();
        if ( moreWork >= 2 ) mCond.notify_one();
        job();
        --mJobsRunning;
        return true;
    }

    void JobQue::job(int threadIndex) {
        while (!mShutdown) {
            Job job;
            bool gotJob = false;
            size_t moreWork = 0;    // work left behind by our pop → wake-propagate (see pushBatch)
            // Spin-before-park (opt-in via setWorkerSpin): poll the fifo for mSpinUs before blocking
            // on the condvar. notify_one to a PARKED worker costs the DISPATCHING thread an OS
            // thread-wake (~3-4.5us, serially per job — the dominant term of the fork/join tax); to
            // a spinning worker it's a ~0.1us no-waiter check. A fork/join burst (LLM decode token =
            // dozens of back-to-back parallel_for matmuls) keeps workers in the spin window the whole
            // time, so only the burst's first dispatch pays the wakes.
            int spinUs = mSpinUs.load(std::memory_order_relaxed);
            if ( spinUs > 0 && !mShutdown.load(std::memory_order_relaxed) ) {
                auto deadline = chrono::steady_clock::now() + chrono::microseconds(spinUs);
                for (;;) {
                    if ( mShutdown.load(std::memory_order_relaxed) ) break;
                    // try_lock, NOT lock: every spinner that sees the same count blip races here, and
                    // blocking losers would queue on the mutex — a convoy the dispatcher's next push
                    // waits behind (measured 45us push stalls). try_lock losers just keep spinning,
                    // and a raided worker resumes the SAME spin window rather than parking, so the
                    // rest of the burst still finds it awake.
                    if ( mFifoCount.load(std::memory_order_relaxed) != 0 && mFifoMutex.try_lock() ) {
                        {
                            lock_guard<mutex> lock(mFifoMutex, std::adopt_lock);
                            if ( !mFifo.empty() ) {
                                job = das::move(mFifo.front().function);
                                mThreads[threadIndex].currentPriority = mFifo.front().priority;
                                mThreads[threadIndex].currentCategory = mFifo.front().category;
                                mFifo.pop_front();
                                mFifoCount.fetch_sub(1, std::memory_order_relaxed);
                                mJobsRunning++;
                                gotJob = true;
                                moreWork = mFifo.size();
                            }
                        }
                        if ( gotJob ) break;
                    }
                    jobque_spin_pause();
                    if ( chrono::steady_clock::now() >= deadline ) break;
                }
            }
            if ( !gotJob ) {
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
                mFifoCount.fetch_sub(1, std::memory_order_relaxed);
                mJobsRunning++;
                moreWork = mFifo.size();
            }
            // Wake propagation (outside the lock so the wakee doesn't bounce off it): if our pop left
            // work behind, wake up to TWO more workers before running the job. A batch flush wakes
            // only one worker; each wakee doubling the wake front fans the backlog out as a binary
            // tree (log2 depth ≈ 5 wake latencies for 32 jobs, vs 32 serialized wakes for a
            // notify_all), paid in parallel by the workers instead of serially by the dispatcher.
            // No-op when no one is parked (spinners pick work up themselves).
            if ( moreWork >= 1 ) mCond.notify_one();
            if ( moreWork >= 2 ) mCond.notify_one();
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
        // Join spin-before-park (opt-in via set_jobque_join_spin, default 0 = park immediately):
        // poll the counter before blocking — the ggml hybrid-poll shape verbatim
        // (ggml_graph_compute_poll_for_work): a COUNTED loop of one relaxed load + one cpu-relax
        // per round, no clock anywhere in the loop (a steady_clock read per round costs more than
        // the pause and caps the poll rate). level*1024*128 rounds, level 50 ≈ ggml's default
        // window. The final seq_cst load reading 0 synchronizes with every notifier's decrement
        // (RMW release sequence), so the workers' results are visible without taking mCompleteMutex.
        int level = sJoinSpin.load(std::memory_order_relaxed);
        if ( level > 0 ) {
            const uint64_t nRounds = uint64_t(level) * 1024ull * 128ull;
            for ( uint64_t i = 0; mRemaining.load(std::memory_order_relaxed) != 0 && i < nRounds; ++i ) {
                jobque_spin_relax();
            }
            if ( mRemaining.load() == 0 ) {
                // Lifetime handshake: the last notifier decrements under mCompleteMutex and can
                // still be inside notify_all/unlock ON THIS OBJECT when the lock-free load sees 0.
                // Callers destroy the JobStatus right after Wait returns (with_wait_group holds it
                // on the stack), so take the lock once to wait the notifier out of its locked
                // region. Uncontended in the common case — the notifier is nanoseconds from done.
                lock_guard<mutex> guard(mCompleteMutex);
                return;
            }
        }
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
