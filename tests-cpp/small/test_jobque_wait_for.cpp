#include <doctest/doctest.h>
#include "daScript/daScript.h"
#include "daScript/misc/job_que.h"

#include <chrono>
#include <thread>

using namespace das;

namespace {

    int64_t elapsedMs ( std::chrono::steady_clock::time_point t0 ) {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - t0).count();
    }

    struct JoinSpinGuard {
        explicit JoinSpinGuard ( int32_t level ) : saved(JobStatus::sJoinSpin.load()) {
            JobStatus::sJoinSpin.store(level);
        }
        ~JoinSpinGuard () { JobStatus::sJoinSpin.store(saved); }
        int32_t saved;
    };

}

TEST_CASE("JobStatus::WaitFor times out when nobody notifies") {
    JobStatus status(1);
    auto t0 = std::chrono::steady_clock::now();
    CHECK_FALSE(status.WaitFor(50));
    CHECK_GE(elapsedMs(t0), 40);
    CHECK_FALSE(status.isReady());
}

TEST_CASE("JobStatus::WaitFor returns once the counter reaches zero") {
    JobStatus status(1);
    std::thread worker([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        status.Notify();
    });
    auto t0 = std::chrono::steady_clock::now();
    CHECK(status.WaitFor(5000));
    CHECK_LT(elapsedMs(t0), 2500);
    CHECK(status.isReady());
    worker.join();
}

TEST_CASE("JobStatus::WaitFor on an already ready status returns immediately") {
    JobStatus status(0);
    auto t0 = std::chrono::steady_clock::now();
    CHECK(status.WaitFor(5000));
    CHECK_LT(elapsedMs(t0), 1000);
}

TEST_CASE("JobStatus::WaitFor with a non-positive timeout polls without blocking") {
    JobStatus status(1);
    auto t0 = std::chrono::steady_clock::now();
    CHECK_FALSE(status.WaitFor(0));
    CHECK_FALSE(status.WaitFor(-1));
    CHECK_LT(elapsedMs(t0), 1000);
    status.Notify();
    CHECK(status.WaitFor(0));
}

TEST_CASE("JobStatus::WaitFor with the join spin enabled") {
    SUBCASE("completion during the spin phase is reported") {
        JoinSpinGuard spin(4);
        JobStatus status(1);
        std::thread worker([&]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            status.Notify();
        });
        CHECK(status.WaitFor(5000));
        CHECK(status.isReady());
        worker.join();
    }
    SUBCASE("the timeout bounds the spin window") {
        JoinSpinGuard spin(1000);
        JobStatus status(1);
        auto t0 = std::chrono::steady_clock::now();
        CHECK_FALSE(status.WaitFor(50));
        CHECK_LT(elapsedMs(t0), 3000);
    }
}
