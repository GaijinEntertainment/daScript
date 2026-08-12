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
// with a CAS, so a writer either owns it outright or declines.
//
// Two publishers are the competing writers, and the clearer runs spaced out rather than flat out.
// That is deliberate: a clearer in a tight loop leaves the box EMPTY most of the time, so reads
// mostly fail and the number of snapshots actually validated collapses to whatever the scheduler
// allows -- measured as low as 31 in this window, against ~130k for the shape below. Two publishers
// keep the box full, and a spaced clearer still covers the publish-vs-clear path the audio module
// hits. The old protocol also tears far more visibly under this shape (~85k-155k torn reads here
// versus ~31k for one publisher against a hot clearer).
//
// Every published snapshot carries one generation counter in all eight words, and the two
// publishers draw from disjoint generations, so a snapshot whose words disagree can only have been
// assembled from two different publishes. Equal words therefore never report a false tear.
TEST_CASE("seq box survives concurrent writers") {
    SeqBox box;

    std::atomic<bool> start{false};
    std::atomic<bool> stop{false};
    std::atomic<uint64_t> reads{0};
    std::atomic<uint64_t> torn{0};
    std::atomic<uint64_t> published{0};

    auto publish_loop = [&](uint64_t firstGeneration){
        while (!start.load(std::memory_order_acquire)) std::this_thread::yield();
        uint64_t gen = firstGeneration;
        while (!stop.load(std::memory_order_relaxed)) {
            uint64_t words[SeqBox::PAYLOAD_WORDS];
            for (int w=0; w!=SeqBox::PAYLOAD_WORDS; ++w) words[w] = gen;
            if (box.publish(words, SeqBox::PAYLOAD_BYTES)) published.fetch_add(1, std::memory_order_relaxed);
            gen += 2;   // disjoint from the other publisher's generations
        }
    };
    std::thread publisherA(publish_loop, 1);
    std::thread publisherB(publish_loop, 2);
    std::thread clearer([&]{
        while (!start.load(std::memory_order_acquire)) std::this_thread::yield();
        uint64_t spin = 0;
        while (!stop.load(std::memory_order_relaxed)) {
            if (++spin % 64 == 0) box.clear();
        }
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
    publisherA.join(); publisherB.join(); clearer.join(); reader.join();

    CHECK(torn.load() == 0);
    // guards against a vacuous pass: the writers have to have landed snapshots and the reader has
    // to have validated some, or `torn == 0` above says nothing
    CHECK(published.load() > 0);
    CHECK(reads.load() > 0);
}
