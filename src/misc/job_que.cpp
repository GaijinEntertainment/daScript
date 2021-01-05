#include "daScript/misc/platform.h"

#include "daScript/misc/job_que.h"

namespace das {

	JobQue::JobQue()
		: mShutdown(false)
        , mThreadCount( 0 )
        , mThreadCountOverflow( 0 )
		, mJobsRunning(0)
		, mJobsRunningOverflow(0)
		, mSleepMs(1000) {
		mThreadCount = max(1,(static_cast<int>(thread::hardware_concurrency())));
		mThreadCountOverflow = max(1,static_cast<int>(thread::hardware_concurrency())); // 0 to disable overflow
		SetCurrentThreadPriority(JobPriority::High);
		set_the_main_thread();
        PostInit();
	}

	void JobQue::PostInit() {
		for (int j = 0; j < mThreadCount; j++) {
			mThreads.emplace_back(new thread([=]() {
				string thread_name = "JobsThread_" + to_string(j);
				SetCurrentThreadName(thread_name);
				auto threadID = this_thread::get_id();
				job(j);
			}));
		}
		for (int j = 0; j < mThreadCountOverflow; j++) {
			mThreadsOverflow.emplace_back( new thread([=]() {
				string thread_name = "JobsThread_Overflow_" + to_string(j);
				SetCurrentThreadName(thread_name);
			 	auto threadID = this_thread::get_id();
				jobOverflow(j);
			}) );
		}
	}

    JobQue::~JobQue () {
        join();
    }

	void JobQue::EvalOnMainThread(Job && expr) {
		lock_guard<mutex> guard(mEvalMainThreadMutex);
		mEvalMainThread.emplace_back(move(expr));
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
        mShutdown = true;
        while ( mThreadCount ) {
            this_thread::yield();
        }
		while (mThreadCountOverflow) {
			this_thread::yield();
        }
		for (auto & th : mThreads) {
			th.threadPointer->join();
        }
		for (auto & th : mThreadsOverflow) {
			th.threadPointer->join();
        }
		mThreads.clear();
		mThreadsOverflow.clear();
	}

    bool JobQue::isEmpty ( bool includingMainThreadJobs ) {
        lock_guard<mutex> lock(mFifoMutex);
		bool queue_is_empty = (mJobsRunning == 0) && (mJobsRunningOverflow == 0) && (mFifo.size() == 0) && (mFifoOverflow.size() == 0);
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
		if (find_if(mFifoOverflow.begin(), mFifoOverflow.end(), [=](const JobEntry& jobEntry) {
                return jobEntry.category == category; }) != mFifoOverflow.end()) {
			return true;
        }
		if (find_if(mThreads.begin(), mThreads.end(), [=](const ThreadEntry& threadEntry) {
                return threadEntry.currentPriority != JobPriority::Inactive && threadEntry.currentCategory == category; }) != mThreads.end()) {
			return true;
        }
		if (find_if(mThreadsOverflow.begin(), mThreadsOverflow.end(), [=](const ThreadEntry& threadEntry) {
                return threadEntry.currentPriority != JobPriority::Inactive && threadEntry.currentCategory == category; }) != mThreadsOverflow.end()) {
			return true;
        }
		return false;
	}

	int JobQue::getNumberOfQueuedJobs() {
		lock_guard<mutex> lock(mFifoMutex);
		return int(mFifo.size() + mFifoOverflow.size());
	}

	inline bool JobQue::pushInternal(Job && job, JobCategory category, JobPriority priority) {
		if (mJobsRunning >= mThreadCount) {
			JobPriority lowestRunningPriority = JobPriority::Maximum;
			for (int i = 0; i < mThreadCount; ++i) {
				DAS_ASSERT(mThreads[i].currentPriority != JobPriority::Inactive);
				lowestRunningPriority = min(lowestRunningPriority, mThreads[i].currentPriority);
			}
			if (priority > lowestRunningPriority) {
				auto  it = lower_bound(mFifoOverflow.begin(), mFifoOverflow.end(), priority, [](const JobEntry& lhs, JobPriority priority) {
					return lhs.priority >= priority; });
				mFifoOverflow.emplace(it, move(job), category, priority);
				return true;
			}
		}
		{
			auto  it = lower_bound(mFifo.begin(), mFifo.end(), priority, [](const JobEntry& lhs, JobPriority priority) {
				return lhs.priority >= priority; });
			mFifo.emplace(it, move(job), category, priority);
			return false;
		}
	}

	void JobQue::push(Job && job, JobCategory category, JobPriority priority) {
		lock_guard<mutex> lock(mFifoMutex);
		bool isPushedOnOverflow = pushInternal(move(job), category, priority);
		if (isPushedOnOverflow) {
			mCondOverflow.notify_one();
        } else {
			mCond.notify_one();
        }
    }

	void JobQue::job(int threadIndex) {
		while (!mShutdown) {
			Job job;
			{
				unique_lock<mutex> lock(mFifoMutex);
				if ( mCond.wait_for(lock, chrono::milliseconds(mSleepMs), [&]() { return mFifo.size() != 0 || mFifoOverflow.size() != 0; }) ) {
					if ( mFifoOverflow.size() > 0 ) {
						job = move(mFifoOverflow.front().function);
						mThreads[threadIndex].currentPriority = mFifoOverflow.front().priority;
						mThreads[threadIndex].currentCategory = mFifoOverflow.front().category;
						mFifoOverflow.pop_front();
					} else {
						DAS_ASSERT(mFifo.size() > 0 && "There must be at least one job available");
						job = move(mFifo.front().function);
						mThreads[threadIndex].currentPriority = mFifo.front().priority;
						mThreads[threadIndex].currentCategory = mFifo.front().category;
						mFifo.pop_front();
					}
					mJobsRunning++;
				} else {
					continue;
                }
			}
			SetCurrentThreadPriority(mThreads[threadIndex].currentPriority);
			job();
			{
				unique_lock<mutex> lock(mFifoMutex);
				mThreads[threadIndex].currentPriority = JobPriority::Inactive;
				mJobsRunning--;
			}
		}
		mThreadCount--;
	}

	void JobQue::jobOverflow(int threadIndex) {
		while (!mShutdown) {
			Job job;
			{
				unique_lock<mutex> lock(mFifoMutex);
				if (mCondOverflow.wait_for(lock, chrono::milliseconds(mSleepMs), [&]() { return mFifoOverflow.size() != 0; })) {
					job = move(mFifoOverflow.front().function);
					mThreadsOverflow[threadIndex].currentPriority = mFifoOverflow.front().priority;
					mThreadsOverflow[threadIndex].currentCategory = mFifoOverflow.front().category;
					mFifoOverflow.pop_front();
					mJobsRunningOverflow++;
				} else {
					continue;
                }
			}
			SetCurrentThreadPriority(mThreadsOverflow[threadIndex].currentPriority);
			job();
			{
				unique_lock<mutex> lock(mFifoMutex);
				mThreadsOverflow[threadIndex].currentPriority = JobPriority::Inactive;
				mJobsRunningOverflow--;
			}
		}
		mThreadCountOverflow--;
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
			bool regularQueueUsed = false;
			bool overflowQueueUsed = false;
			for (int ch = 0; ch < onThreads; ++ch) {
	            int i0 = from + ch * step;
	            int i1 = i0 + step;
	            bool isPushedOnOverflow = pushInternal([=,&status](){
	                chunk(i0, i1);
	                status.Notify();
	            }, category, priority);
				regularQueueUsed |= !isPushedOnOverflow;
				overflowQueueUsed |= isPushedOnOverflow;
	        }
			if (regularQueueUsed) {
				mCond.notify_all();
            }
			if (overflowQueueUsed) {
				mCondOverflow.notify_all();
            }
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
			bool regularQueueUsed = false;
			bool overflowQueueUsed = false;
			for (int ch = 0; ch < numChunks; ++ch) {
	            int i0 = from + ch * step;
	            int i1 = min(i0 + step, to);
	            bool isPushedOnOverflow = pushInternal([=, &chunk, &producerFifoJobs, &producerFifoMutex, &condition]() {
	                chunk(i0, i1);
	                {
                        lock_guard<mutex> producerFifoLock(producerFifoMutex);
                        producerFifoJobs.push_back(([=]() { consume(i0, i1); }));
                        condition.notify_one();
	                }
	            }, category, priority);
				regularQueueUsed |= !isPushedOnOverflow;
				overflowQueueUsed |= isPushedOnOverflow;
			}
			if (regularQueueUsed) {
				mCond.notify_all();
            }
			if (overflowQueueUsed) {
				mCondOverflow.notify_all();
            }
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

	thread::id JobQue::mTheMainThread;

	void JobQue::set_the_main_thread() {
		SetCurrentThreadName( "Main" );
		mTheMainThread = this_thread::get_id();
	}

	bool JobQue::is_the_main_thread() {
		return this_thread::get_id() == mTheMainThread;
	}

	void JobStatus::Notify() {
		lock_guard<mutex> guard(mCompleteMutex);
        DAS_ASSERT(mRemaining != 0 && "Nothing to notify!");
        --mRemaining;
        if ( mRemaining==0 ) {
            mCond.notify_all();
        }
	}

	void JobStatus::Wait() {
        unique_lock<mutex> lock(mCompleteMutex);
        while ( mRemaining ) {
            mCond.wait(lock);
        }
	}

	bool JobStatus::IsReady() {
        lock_guard<mutex> guard(mCompleteMutex);
        return mRemaining==0;
	}

	void JobStatus::Clear(uint32_t count) {
        lock_guard<mutex> guard(mCompleteMutex);
        mRemaining = count;
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
		int platformPriority = (int)(minPlatformPriority + (maxPlatformPriority - minPlatformPriority) * ((float)(priority - JobPriority_Minimum) / (float)(JobPriority_Maximum - JobPriority_Minimum)));
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
		default:
			DAS_ASSERT(0 && "Windows takes prefixed priority values");
			break;
		}
		SetThreadPriority(GetCurrentThread(), winPriority);
	}
}

#else

#include <pthread.h>

namespace das
{    void SetCurrentThreadName ( const string & str ) {
        pthread_setname_np(pthread_self(), str.c_str());
    }

	void SetCurrentThreadPriority ( JobPriority _priority ) {
        int priority = int(_priority);
		float minPlatformPriority = sched_get_priority_min(SCHED_OTHER);
		float maxPlatformPriority = sched_get_priority_max(SCHED_OTHER);
		struct sched_param sched_param;
		int platformPriority = (int)( minPlatformPriority + (maxPlatformPriority - minPlatformPriority) * ((float)(priority - JobPriority_Minimum) / (float)(JobPriority_Maximum - JobPriority_Minimum)) );
		sched_param.sched_priority = platformPriority;
		pthread_setschedparam(pthread_self(), SCHED_OTHER, &sched_param);
	}
}

#endif
