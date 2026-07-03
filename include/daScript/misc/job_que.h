#pragma once

#include <condition_variable>
#include <deque>
#include <thread>
#include <atomic>

namespace das {
    struct LineInfo;

    // shared tracking counter for JobStatus and Feature
    DAS_API extern atomic<uint64_t> g_jobque_track_total;
    DAS_API extern atomic<uint64_t> g_jobque_track_id;       // ID to detail-trace (0 = none)
    // single job
    typedef function<void()> Job;
    typedef function<void(int,int)> JobChunk;

    typedef uint32_t JobCategory;

    enum class JobPriority : int32_t {
        Inactive = 0x66,                    // just some high number to indicate the thread is inactive
        Minimum = -2,
        Low = -1,
        Medium = 0,
        High = 1,
        Maximum = 2,
        Default = Medium,
        Realtime = High,
    };

    class DAS_API JobStatus {
    public:
        enum { STATUS_MAGIC = 0xdeadbeef };
        JobStatus();
        JobStatus(uint32_t count);
        JobStatus ( JobStatus && ) = delete;
        JobStatus ( const JobStatus & ) = delete;
        virtual ~JobStatus();
        JobStatus & operator = ( JobStatus && ) = delete;
        JobStatus & operator = ( const JobStatus & ) = delete;
        bool Notify();
        bool NotifyAndRelease( LineInfo * at = nullptr );
        bool isReady();
        void Wait();
        void Clear(uint32_t count = 1);
        int addRef( LineInfo * at = nullptr );
        int releaseRef( LineInfo * at = nullptr );
        int size() const;
        int append(int size);
        bool isValid() const { return mMagic==uint32_t(STATUS_MAGIC); }
    // tracking
    public:
        uint64_t        mTrackId = 0;
        JobStatus *     mTrackNext = nullptr;
        JobStatus *     mTrackPrev = nullptr;
        uint32_t        mTrackMagic = 0;
        string          mCreatedAt;
        static JobStatus *  sTrackHead;
        static mutex        sTrackMutex;
        static constexpr uint32_t TRACK_JOB_STATUS = 0xDA5CA001;
        static constexpr uint32_t TRACK_CHANNEL    = 0xDA5CA002;
        static constexpr uint32_t TRACK_LOCKBOX    = 0xDA5CA003;
        static constexpr uint32_t TRACK_STREAM     = 0xDA5CA004;
        // Join poll level (0 = park on the condvar immediately, the default). Wait() polls
        // mRemaining for level*1024*128 relax-rounds before blocking — the ggml hybrid-poll shape
        // and denomination (their --poll 0..100; 50 = their default ≈ 6.5M rounds). With the
        // calling thread executing parallel_for chunks itself, the join wait is just the workers'
        // tail — the poll removes the last per-fork OS park/wake. Global (all wait groups),
        // opt-in via set_jobque_join_spin: polling burns idle CPU.
        static atomic<int32_t> sJoinSpin;
        static void DumpJobQueLeaks();
        static uint64_t CountJobQueLeaks();
        void trackEvent ( LineInfo * at, bool isAddRef );
    protected:
        void trackInsert();
        void trackRemove();
    protected:
        mutable mutex       mCompleteMutex;
        atomic<uint32_t>    mRemaining{0};  // atomic so Wait()'s spin phase can poll lock-free; all writes stay under mCompleteMutex for the condvar protocol
        condition_variable  mCond;
        atomic<int>         mRef{0};
        uint32_t            mMagic = uint32_t(STATUS_MAGIC);
    };

    class JobQue {
    public:
        JobQue();
        JobQue ( const JobQue & ) = delete;
        JobQue ( JobQue && ) = delete;
        JobQue & operator = ( const JobQue & ) = delete;
        JobQue & operator = ( JobQue && ) = delete;
        ~JobQue ();
        bool isEmpty (bool includingMainThreadJobs = false);
        bool areJobsPending(JobCategory category);
        int getNumberOfQueuedJobs();
        int getTotalHwJobs();
        void push(Job && job, JobCategory category, JobPriority priority);
        void pushBatch(vector<Job> && jobs, JobCategory category, JobPriority priority);   // one lock hold + one notify for the whole batch (workers wake-propagate, see JobQue::job)
        bool tryRunOneJob();    // pop one queued job and execute it on the CALLING thread (dispatcher work-stealing at a fork/join); false when the fifo is empty
        void parallel_for ( JobStatus & status, int from, int to, const JobChunk & chunk, JobCategory category, JobPriority priority, int chunk_count = -1, int step = 1 );
        void parallel_for ( int from, int to, const JobChunk & chunk, JobCategory category, JobPriority priority, int chunk_count = -1, int step = 1 );
        void parallel_for_with_consume (int from, int to, const JobChunk & chunk, const JobChunk & consume, JobCategory category, JobPriority priority, int chunk_count = -1, int step = 1);
        static int get_num_threads();
        void EvalOnMainThread(Job && expr);
        void EvalMainThreadJobs();
        void wait();
        void Reset() { wait( ); }
        // Worker spin-before-park window, microseconds (0 = park on the condvar immediately, the
        // default). An idle worker spin-polls the fifo for this long before blocking, so a
        // fork/join burst (e.g. an LLM decode token's back-to-back matmuls) never pays the OS
        // thread-wake per job — measured ~3-4.5us per notify_one to a PARKED worker vs ~0.1us to a
        // spinning one, paid serially by the DISPATCHING thread. Opt-in: spinning burns idle CPU.
        void setWorkerSpin ( int usec ) { mSpinUs = usec; }
    protected:
        struct JobEntry {
            JobEntry( Job&& _function, JobCategory _category, JobPriority _priority) {
                function = das::move(_function);
                priority = _priority;
                category = _category;
            }
            Job                function = nullptr;
            JobPriority        priority = JobPriority::Inactive;
            JobCategory        category = 0;
        };
        struct ThreadEntry {
            ThreadEntry( unique_ptr<thread> && thread) {
                threadPointer = das::move(thread);
            };
            unique_ptr<thread>  threadPointer;
            JobPriority         currentPriority = JobPriority::Inactive;
            JobPriority         appliedPriority = JobPriority::Inactive;  // last priority actually pushed to the OS thread
            JobCategory         currentCategory = 0;
        };
    protected:
        void join();
        void job(int threadIndex);
        void submit(Job && job, JobCategory category, JobPriority priority);
    protected:
        condition_variable mCond;
        int mSleepMs;
        atomic<bool>    mShutdown{false};
        atomic<int>     mThreadCount{0};
        static thread::id mTheMainThread;
        mutex mFifoMutex;
    protected:
        deque<JobEntry> mFifo;
        vector<ThreadEntry> mThreads;
        atomic<int> mJobsRunning{0};
        atomic<int32_t> mSpinUs{0};
        atomic<int32_t> mFifoCount{0};  // lock-free mirror of mFifo.size() for the spin poll
    protected:
        mutex mEvalMainThreadMutex;
        vector<Job> mEvalMainThread;
    };

    void SetCurrentThreadName ( const string & str );
    void SetCurrentThreadPriority ( JobPriority priority );             // change priority of current thread, 0 - normal, >0 above normal, <0 below normal
}
