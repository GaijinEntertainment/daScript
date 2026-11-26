#include "daScript/misc/platform.h"

#include "daScript/misc/jobserver.h"
#include "daScript/misc/env_cfg.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <string>

#if defined(_WIN32)
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <windows.h>
#else
    #include <cerrno>
    #include <fcntl.h>
    #include <sys/stat.h>
    #include <unistd.h>
#endif

namespace das {

    namespace {
        std::string authFromFlags ( const char * makeflags ) {
            if ( !makeflags ) return "";
            std::string flags = makeflags;
            for ( auto key : { "--jobserver-auth=", "--jobserver-fds=" } ) {
                auto at = flags.rfind(key);
                if ( at == std::string::npos ) continue;
                at += strlen(key);
                return flags.substr(at, flags.find(' ', at) - at);
            }
            return "";
        }

        const char * makeflagsFromEnvironment () {
            for ( auto var : { "CARGO_MAKEFLAGS", "MAKEFLAGS", "MFLAGS" } ) {
                auto flags = das_getenv(var);
                if ( !authFromFlags(flags).empty() ) return flags;
            }
            return nullptr;
        }

#if !defined(_WIN32) && !DAS_TARGET_PS
        bool isInheritedPipe ( int fd ) {
            struct stat st;
            return fd >= 0 && fcntl(fd, F_GETFD) != -1 && fstat(fd, &st) == 0 && S_ISFIFO(st.st_mode);
        }
#endif
    }

#if defined(_WIN32)

    class JobserverPool {
    public:
        static std::unique_ptr<JobserverPool> open ( const std::string & auth ) {
            auto h = OpenSemaphoreA(SYNCHRONIZE | SEMAPHORE_MODIFY_STATE, FALSE, auth.c_str());
            if ( !h ) return nullptr;
            return std::unique_ptr<JobserverPool>(new JobserverPool(h));
        }
        ~JobserverPool () { CloseHandle(sem); }
        bool take ( char & byte ) {
            byte = '+';
            return WaitForSingleObject(sem, 0) == WAIT_OBJECT_0;
        }
        void giveBack ( char ) { ReleaseSemaphore(sem, 1, nullptr); }
    private:
        explicit JobserverPool ( HANDLE h ) : sem(h) {}
        HANDLE  sem;
    };

#elif DAS_TARGET_PS

    class JobserverPool {
    public:
        static std::unique_ptr<JobserverPool> open ( const std::string & ) { return nullptr; }
        bool take ( char & ) { return false; }
        void giveBack ( char ) {}
    };

#else

    class JobserverPool {
    public:
        static std::unique_ptr<JobserverPool> open ( const std::string & auth ) {
            if ( auth.compare(0, 5, "fifo:") == 0 ) {
                int fd = ::open(auth.c_str() + 5, O_RDWR | O_NONBLOCK | O_CLOEXEC);
                if ( fd < 0 ) return nullptr;
                return std::unique_ptr<JobserverPool>(new JobserverPool(fd, fd));
            }
            int r = -1, w = -1;
            if ( sscanf(auth.c_str(), "%d,%d", &r, &w) != 2 ) return nullptr;
            if ( !isInheritedPipe(r) || !isInheritedPipe(w) ) return nullptr;
            int ownRead = ::open(("/proc/self/fd/" + std::to_string(r)).c_str(), O_RDONLY | O_NONBLOCK | O_CLOEXEC);
            if ( ownRead < 0 ) return nullptr;
            int ownWrite = dup(w);
            if ( ownWrite < 0 ) {
                close(ownRead);
                return nullptr;
            }
            return std::unique_ptr<JobserverPool>(new JobserverPool(ownRead, ownWrite));
        }
        ~JobserverPool () {
            close(readFd);
            if ( writeFd != readFd ) close(writeFd);
        }
        bool take ( char & byte ) {
            for ( ;; ) {
                auto n = read(readFd, &byte, 1);
                if ( n == 1 ) return true;
                if ( n < 0 && errno == EINTR ) continue;
                return false;
            }
        }
        void giveBack ( char byte ) {
            while ( write(writeFd, &byte, 1) < 0 && errno == EINTR ) {}
        }
    private:
        JobserverPool ( int r, int w ) : readFd(r), writeFd(w) {}
        int readFd;
        int writeFd;
    };

#endif

    class JobserverState {
    public:
        std::unique_ptr<JobserverPool>  pool;
        int32_t                         limit = 0;
        int32_t                         running = 0;
        bool                            implicitBusy = false;
        std::mutex                      lock;
        std::condition_variable         freed;

        void release ( bool implicitToken, char byte ) {
            {
                std::lock_guard<std::mutex> guard(lock);
                running --;
                if ( implicitToken ) implicitBusy = false;
                else if ( pool ) pool->giveBack(byte);
            }
            freed.notify_one();
        }
    };

    JobserverToken::JobserverToken ( JobserverToken && other ) noexcept
        : state(other.state), implicitToken(other.implicitToken), byte(other.byte) {
        other.state = nullptr;
    }

    JobserverToken & JobserverToken::operator = ( JobserverToken && other ) noexcept {
        if ( this != &other ) {
            if ( state ) state->release(implicitToken, byte);
            state = other.state;
            implicitToken = other.implicitToken;
            byte = other.byte;
            other.state = nullptr;
        }
        return *this;
    }

    JobserverToken::~JobserverToken () {
        if ( state ) state->release(implicitToken, byte);
    }

    Jobserver::Jobserver ( int32_t limit ) : Jobserver(limit, makeflagsFromEnvironment()) {}

    Jobserver::Jobserver ( int32_t limit, const char * makeflags ) : state(std::make_unique<JobserverState>()) {
        state->limit = limit;
        auto auth = authFromFlags(makeflags);
        if ( !auth.empty() ) state->pool = JobserverPool::open(auth);
    }

    Jobserver::~Jobserver () = default;

    JobserverToken Jobserver::acquire () {
        if ( !state->pool && state->limit <= 0 ) return JobserverToken(nullptr, false, 0);
        std::unique_lock<std::mutex> guard(state->lock);
        for ( ;; ) {
            if ( state->limit <= 0 || state->running < state->limit ) {
                char byte = 0;
                bool implicitToken = !state->implicitBusy;
                if ( implicitToken || !state->pool || state->pool->take(byte) ) {
                    if ( implicitToken ) state->implicitBusy = true;
                    state->running ++;
                    return JobserverToken(state.get(), implicitToken, byte);
                }
            }
            state->freed.wait_for(guard, std::chrono::milliseconds(10));
        }
    }
}
