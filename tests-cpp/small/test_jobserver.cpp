#include <doctest/doctest.h>
#include "daScript/daScript.h"
#include "daScript/misc/jobserver.h"
#include "daScript/misc/env_cfg.h"

#include <algorithm>
#include <atomic>
#include <thread>
#include <vector>

#if defined(_WIN32)
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <windows.h>
#else
    #include <fcntl.h>
    #include <sys/stat.h>
    #include <unistd.h>
#endif

using namespace das;

namespace {
    std::vector<JobserverToken> acquireAll ( Jobserver & jobserver, size_t count ) {
        std::vector<JobserverToken> tokens;
        for ( size_t i = 0; i != count; ++i ) tokens.push_back(jobserver.acquire());
        return tokens;
    }

    int32_t peakHeld ( Jobserver & jobserver, int32_t threads, int32_t rounds ) {
        std::atomic<int32_t> held{0}, peak{0};
        std::vector<std::thread> workers;
        for ( int32_t t = 0; t != threads; ++t ) {
            workers.emplace_back([&]() {
                for ( int32_t r = 0; r != rounds; ++r ) {
                    auto token = jobserver.acquire();
                    auto now = ++held;
                    for ( auto seen = peak.load(); now > seen && !peak.compare_exchange_weak(seen, now); ) {}
                    --held;
                }
            });
        }
        for ( auto & w : workers ) w.join();
        return peak.load();
    }
}

TEST_CASE("jobserver: with no pool and no limit every acquire returns at once") {
    for ( auto flags : { (const char *) nullptr, "-j8", "--jobserver-auth=x", "--jobserver-auth=1000,1001",
            "--jobserver-auth=fifo:/nonexistent/das_jobserver" } ) {
        Jobserver jobserver(0, flags);
        CHECK_EQ(acquireAll(jobserver, 16).size(), 16);
    }
}

TEST_CASE("jobserver: a limit caps the tokens held at once") {
    Jobserver jobserver(3, nullptr);
    CHECK_LE(peakHeld(jobserver, 8, 200), 3);
}

#if defined(_WIN32)

TEST_CASE("jobserver: a named semaphore pool lends its tokens beside the implicit one and takes them back") {
    auto name = "das_jobserver_test_" + std::to_string(GetCurrentProcessId());
    auto sem = CreateSemaphoreA(nullptr, 2, 2, name.c_str());
    REQUIRE(sem);
    auto flags = " -j3 --jobserver-auth=" + name;
    {
        Jobserver jobserver(0, flags.c_str());
        auto held = acquireAll(jobserver, 3);
        CHECK(WaitForSingleObject(sem, 0) == WAIT_TIMEOUT);
    }
    CHECK(WaitForSingleObject(sem, 0) == WAIT_OBJECT_0);
    CHECK(WaitForSingleObject(sem, 0) == WAIT_OBJECT_0);
    CloseHandle(sem);
}

#else

namespace {
    std::string drain ( int fd ) {
        std::string bytes;
        char c;
        while ( read(fd, &c, 1) == 1 ) bytes += c;
        std::sort(bytes.begin(), bytes.end());
        return bytes;
    }

    struct TestFifo {
        std::string path;
        int         fd = -1;
        explicit TestFifo ( const char * tokens ) {
            auto tmp = das_getenv("TMPDIR");
            path = std::string(tmp && *tmp ? tmp : "/tmp") + "/das_jobserver_test_" + std::to_string(getpid());
            unlink(path.c_str());
            REQUIRE(mkfifo(path.c_str(), 0600) == 0);
            fd = open(path.c_str(), O_RDWR | O_NONBLOCK);
            REQUIRE(fd >= 0);
            REQUIRE(write(fd, tokens, strlen(tokens)) == ssize_t(strlen(tokens)));
        }
        ~TestFifo () {
            close(fd);
            unlink(path.c_str());
        }
        std::string flags () const { return " -j3 --jobserver-auth=fifo:" + path; }
    };
}

TEST_CASE("jobserver: a fifo pool lends its tokens beside the implicit one and takes back the same bytes") {
    TestFifo fifo("ab");
    auto flags = fifo.flags();
    {
        Jobserver jobserver(0, flags.c_str());
        auto held = acquireAll(jobserver, 3);
        CHECK_EQ(drain(fifo.fd), "");
    }
    CHECK_EQ(drain(fifo.fd), "ab");
}

TEST_CASE("jobserver: the last --jobserver-auth wins") {
    TestFifo fifo("c");
    auto flags = "--jobserver-auth=fifo:/nonexistent/das_jobserver --jobserver-auth=fifo:" + fifo.path;
    {
        Jobserver jobserver(0, flags.c_str());
        auto held = acquireAll(jobserver, 2);
        CHECK_EQ(drain(fifo.fd), "");
    }
    CHECK_EQ(drain(fifo.fd), "c");
}

TEST_CASE("jobserver: threads sharing a one-token pool hold at most two tokens and give the token back") {
    TestFifo fifo("+");
    auto flags = fifo.flags();
    {
        Jobserver jobserver(0, flags.c_str());
        CHECK_LE(peakHeld(jobserver, 8, 200), 2);
    }
    CHECK_EQ(drain(fifo.fd), "+");
}

TEST_CASE("jobserver: a parallel compile under a one-token pool finishes and gives the token back") {
    TestFifo fifo("+");
    auto flags = fifo.flags();
    auto saved = das_getenv("MAKEFLAGS");
    std::string savedFlags = saved ? saved : "";
    das_setenv("MAKEFLAGS", flags.c_str());
    {
        auto fAccess = make_smart<FsFileAccess>();
        TextWriter logs;
        ModuleGroup libGroup;
        CodeOfPolicies policies;
        policies.compile_threads = 4;
        auto program = compileDaScript(getDasRoot() + "/tests-cpp/small/test_compile_dag.das", fAccess, logs, libGroup, policies);
        CHECK_MESSAGE(!program->failed(), logs.str());
    }
    if ( saved ) das_setenv("MAKEFLAGS", savedFlags.c_str());
    else das_unsetenv("MAKEFLAGS");
    CHECK_EQ(drain(fifo.fd), "+");
}

#if defined(__linux__)

TEST_CASE("jobserver: an inherited pipe pool reads through its own non-blocking fd") {
    int fds[2];
    REQUIRE(pipe(fds) == 0);
    REQUIRE(write(fds[1], "+++", 3) == 3);
    auto flags = "-j4 --jobserver-fds=" + std::to_string(fds[0]) + "," + std::to_string(fds[1]);
    {
        Jobserver jobserver(0, flags.c_str());
        auto held = acquireAll(jobserver, 4);
        CHECK_EQ(fcntl(fds[0], F_GETFL) & O_NONBLOCK, 0);
    }
    fcntl(fds[0], F_SETFL, O_NONBLOCK);
    CHECK_EQ(drain(fds[0]), "+++");
    close(fds[0]);
    close(fds[1]);
}

#endif
#endif
