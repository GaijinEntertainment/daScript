#pragma once

#include <condition_variable>
#include <mutex>
#include <deque>

namespace das {
	// single job
    typedef function<void()> Job;
    typedef function<void(int,int)> JobChunk;

    enum class JobCategory {
        None,
        Generic,
        Realtime,
        Streaming,
        Cache,
    };

    enum class JobPriority {
        Inactive = 0x66,                    // just some high number to indicate the thread is inactive
        Minimum = -2,
        Low = -1,
        Medium = 0,
        High = 1,
        Maximum = 2,
        Default = Medium,
        Realtime = High,
    };

	class JobStatus {
	public:
		JobStatus() {};
		JobStatus(uint32_t count) { Clear( count); };
		JobStatus ( JobStatus && ) = delete;
		JobStatus ( const JobStatus & ) = delete;
		JobStatus & operator = ( JobStatus && ) = delete;
		JobStatus & operator = ( const JobStatus & ) = delete;
		void Notify();
		bool IsReady();
		void Wait();
		void Clear(uint32_t count = 1);
	protected:
		mutex				mCompleteMutex;
		uint32_t			mRemaining = 0;
		condition_variable	mCond;
	};

    class JobQue {
    public:
		JobQue();
        JobQue ( const JobQue & ) = delete;
        JobQue ( JobQue && ) = delete;
        JobQue & operator = ( const JobQue & ) = delete;
        JobQue & operator = ( JobQue && ) = delete;
        ~JobQue ();
		void PostInit();
        bool	isEmpty (bool includingMainThreadJobs = false);
		bool	areJobsPending(JobCategory category);
		int		getNumberOfQueuedJobs();
		void	push(Job && job, JobCategory category, JobPriority priority);
        void parallel_for ( JobStatus & status, int from, int to, const JobChunk & chunk, JobCategory category, JobPriority priority, int chunk_count = -1, int step = 1 );
        void parallel_for ( int from, int to, const JobChunk & chunk, JobCategory category, JobPriority priority, int chunk_count = -1, int step = 1 );
		void parallel_for_with_consume (int from, int to, const JobChunk & chunk, const JobChunk & consume, JobCategory category, JobPriority priority, int chunk_count = -1, int step = 1);
		static void set_the_main_thread();
		static bool is_the_main_thread();
		static int get_num_threads() { return   max(1,static_cast<int>(thread::hardware_concurrency())); }
		void EvalOnMainThread(Job && expr);
		void EvalMainThreadJobs();
		void wait();
		void Reset() { wait( ); }
	protected:
		struct JobEntry {
			JobEntry( Job&& _function, JobCategory _category, JobPriority _priority) {
				function = move(_function);
				priority = _priority;
				category = _category;
			}
			Job				function = nullptr;
			JobPriority		priority = JobPriority::Inactive;
			JobCategory		category = JobCategory::None;
		};
		struct ThreadEntry {
			ThreadEntry( thread* thread) {
				threadPointer.reset(thread);
			};
			unique_ptr<thread>	threadPointer;
			JobPriority			currentPriority = JobPriority::Inactive;
			JobCategory			currentCategory = JobCategory::None;
		};

		void	join();
		void	job(int threadIndex);
		void	jobOverflow(int threadIndex);
		bool	pushInternal(Job && job, JobCategory category, JobPriority priority);
		condition_variable mCond;
		condition_variable mCondOverflow;
		int mSleepMs;
		atomic<bool>	mShutdown;
		atomic<int>		mThreadCount;
		atomic<int>		mThreadCountOverflow;
		static thread::id mTheMainThread;
		mutex mFifoMutex;
    protected:
        deque<JobEntry>	mFifo;
        deque<JobEntry>	mFifoOverflow;
        vector<ThreadEntry>		mThreads;
        vector<ThreadEntry >	mThreadsOverflow;
        atomic<int> mJobsRunning;
        atomic<int> mJobsRunningOverflow;
	protected:
		mutex mEvalMainThreadMutex;
		vector<Job> mEvalMainThread;
    };

    void SetCurrentThreadName ( const string & str );
	void SetCurrentThreadPriority ( JobPriority priority );             // change priority of current thread, 0 - normal, >0 above normal, <0 below normal
}
