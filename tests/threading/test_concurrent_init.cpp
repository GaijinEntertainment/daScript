// Test for issue #2027: Race condition in concurrent module registration.
//
// Multiple threads call NEED_ALL_DEFAULT_MODULES + Module::Initialize()
// simultaneously, which triggers SIGSEGV / access violations due to
// unprotected shared state inside module constructors.
//
// Build:  cmake --build build --config Release --target test_concurrent_init
// Run:    bin/Release/test_concurrent_init.exe

#include "daScript/daScript.h"
#include "daScript/misc/platform.h"

#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>
#include <mutex>

using namespace das;

// ── helpers ────────────────────────────────────────────────────────────
static std::mutex coutMtx;

template <typename... Args>
static void log(Args&&... args) {
    std::lock_guard<std::mutex> lk(coutMtx);
    (std::cout << ... << std::forward<Args>(args)) << "\n" << std::flush;
}

// ── worker ─────────────────────────────────────────────────────────────
// Register every module that daslang_static uses — the same set the real
// compiler loads.  This maximises the chance of hitting shared-state races
// inside module constructors.
static void workerThread(int id, std::atomic<int>& ok, std::atomic<int>& fail) {
    try {
        // Use the same macro as the issue #2027 reporter's repro case
        NEED_ALL_DEFAULT_MODULES;
        NEED_MODULE(Module_UriParser);
        NEED_MODULE(Module_JobQue);
        Module::Initialize();

        // Quick sanity: make sure the "$" (builtin) module exists.
        auto *m = Module::require("$");
        if (!m) {
            log("[Thread ", id, "] FAIL – builtin module not found");
            ++fail;
        } else {
            ++ok;
        }

        Module::Shutdown();
    } catch (const std::exception& e) {
        log("[Thread ", id, "] EXCEPTION: ", e.what());
        ++fail;
    } catch (...) {
        log("[Thread ", id, "] UNKNOWN EXCEPTION");
        ++fail;
    }
}

// ── main ───────────────────────────────────────────────────────────────
int main() {
    // ── Test 1: sequential (baseline) ──────────────────────────────────
    {
        log("=== Test 1: sequential init (10 iterations) ===");
        std::atomic<int> ok{0}, fail{0};
        for (int i = 0; i < 10; ++i) {
            workerThread(i, ok, fail);
        }
        log("sequential: ", ok.load(), " ok, ", fail.load(), " fail");
        if (fail.load() != 0) {
            log("FAILED (sequential should never fail)");
            return 1;
        }
        log("=== Test 1 PASSED ===\n");
    }

    // ── Test 2: concurrent (should crash without the fix) ──────────────
    for (int round = 0; round < 3; ++round) {
        constexpr int N = 64;
        log("=== Test 2 round ", round, ": concurrent init (", N, " threads) ===");
        std::atomic<int> ok{0}, fail{0};
        std::vector<std::thread> threads;
        threads.reserve(N);

        for (int i = 0; i < N; ++i) {
            threads.emplace_back(workerThread, 1000 + round * N + i, std::ref(ok), std::ref(fail));
        }
        for (auto& t : threads) {
            t.join();
        }

        log("concurrent: ", ok.load(), " ok, ", fail.load(), " fail");
        if (fail.load() != 0) {
            log("FAILED at round ", round);
            return 1;
        }
        log("=== Test 2 round ", round, " PASSED ===\n");
    }

    log("\nAll tests passed.");
    return 0;
}
