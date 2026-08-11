#include <doctest/doctest.h>
#include "daScript/daScript.h"
#include "daScript/misc/job_que.h"
#include "daScript/simulate/aot_builtin_jobque.h"

#include <atomic>
#include <chrono>
#include <thread>

using namespace das;

// SeqBox's odd/even counter is a seqlock, which admits exactly one writer. Two writers bumping it
// concurrently could leave it EVEN while the payload was half-written, and a reader accepted that as
// a stable snapshot -- reachable from the audio module, where the mixer publishes on the device
// thread while clear_status/set_status_update write from the caller's. Writers now claim the box
// with a CAS, so publish either owns it outright or declines.
//
// Every published snapshot carries the same generation counter in all eight words, so a snapshot
// whose words disagree can only have been assembled from two different publishes.
TEST_CASE("seq box survives concurrent writers") {
    SeqBox box;

    std::atomic<bool> start{false};
    std::atomic<bool> stop{false};
    std::atomic<uint64_t> reads{0};
    std::atomic<uint64_t> torn{0};
    std::atomic<uint64_t> published{0};
    std::atomic<uint64_t> declined{0};

    std::thread publisher([&]{
        while (!start.load(std::memory_order_acquire)) std::this_thread::yield();
        uint64_t gen = 1;
        while (!stop.load(std::memory_order_relaxed)) {
            uint64_t words[SeqBox::PAYLOAD_WORDS];
            for (int w=0; w!=SeqBox::PAYLOAD_WORDS; ++w) words[w] = gen;
            if (box.publish(words, SeqBox::PAYLOAD_BYTES)) published.fetch_add(1, std::memory_order_relaxed);
            else declined.fetch_add(1, std::memory_order_relaxed);
            ++gen;
        }
    });
    std::thread clearer([&]{
        while (!start.load(std::memory_order_acquire)) std::this_thread::yield();
        while (!stop.load(std::memory_order_relaxed)) box.clear();
    });
    std::thread reader([&]{
        while (!start.load(std::memory_order_acquire)) std::this_thread::yield();
        while (!stop.load(std::memory_order_relaxed)) {
            uint64_t out[SeqBox::PAYLOAD_WORDS];
            if (!box.read(out, SeqBox::PAYLOAD_BYTES)) continue;
            reads.fetch_add(1, std::memory_order_relaxed);
            for (int w=1; w!=SeqBox::PAYLOAD_WORDS; ++w) {
                if (out[w]!=out[0]) { torn.fetch_add(1, std::memory_order_relaxed); break; }
            }
        }
    });

    start.store(true, std::memory_order_release);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    stop.store(true, std::memory_order_relaxed);
    publisher.join(); clearer.join(); reader.join();

    CHECK(torn.load() == 0);
    // guards against a vacuous pass: all three threads have to have made progress for the assertion
    // above to mean anything
    CHECK(published.load() > 0);
    CHECK(reads.load() > 0);
}
