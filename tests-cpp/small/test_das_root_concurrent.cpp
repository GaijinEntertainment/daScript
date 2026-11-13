#include <doctest/doctest.h>
#include "daScript/daScript.h"
#include <atomic>
#include <string>
#include <thread>
#include <vector>

using namespace das;

namespace {

    struct HostRootGuard {
        const string hostRoot = getDasRoot();
        ~HostRootGuard() { setDasRoot(hostRoot); }
    };

    unsigned mismatchedFirstReads ( const string & expected, unsigned workers ) {
        std::vector<string> seen(workers);
        std::atomic<unsigned> waiting(0);
        std::atomic<bool> go(false);
        std::vector<std::thread> pool;
        setDasRoot("");
        for ( unsigned w = 0; w != workers; ++w ) {
            pool.emplace_back([&, w]() {
                waiting.fetch_add(1);
                while ( !go.load() ) std::this_thread::yield();
                seen[w] = getDasRoot();
            });
        }
        while ( waiting.load() != workers ) std::this_thread::yield();
        go = true;
        for ( auto & t : pool ) t.join();
        unsigned mismatched = 0;
        for ( auto & root : seen ) mismatched += root != expected;
        return mismatched;
    }
}

TEST_CASE("threads that read the das root at once, before anything derived it, all get the derived root") {
    HostRootGuard guard;
    setDasRoot("");
    const string derived = getDasRoot();
    REQUIRE_FALSE(derived.empty());
    unsigned mismatched = 0;
    for ( int round = 0; round != 200; ++round ) mismatched += mismatchedFirstReads(derived, 8);
    CHECK_EQ(mismatched, 0u);
}

TEST_CASE("a das root set while threads read it is read whole") {
    HostRootGuard guard;
    const string hostRoot = guard.hostRoot;
    setDasRoot(hostRoot + "/tests-cpp");
    const string other = getDasRoot();
    REQUIRE_NE(other, hostRoot);
    std::atomic<bool> stop(false);
    std::atomic<unsigned> torn(0);
    std::vector<std::thread> readers;
    for ( unsigned r = 0; r != 4; ++r ) {
        readers.emplace_back([&]() {
            while ( !stop.load() ) {
                const string root = getDasRoot();
                if ( root != hostRoot && root != other ) torn.fetch_add(1);
            }
        });
    }
    for ( int flip = 0; flip != 20000; ++flip ) setDasRoot(flip & 1 ? hostRoot : other);
    stop = true;
    for ( auto & t : readers ) t.join();
    CHECK_EQ(torn.load(), 0u);
}
