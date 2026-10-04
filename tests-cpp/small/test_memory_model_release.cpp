#include <doctest/doctest.h>

#include "daScript/misc/platform.h"
#include "daScript/misc/memory_model.h"

#include <vector>

using namespace das;

// src/misc/ARCHITECTURE.md#empty-deck-release
#if !DAS_ASAN
namespace {
    std::vector<char*> fillDecks ( MemoryModel & mm, uint64_t slotSize, uint32_t minDecks ) {
        std::vector<char*> ptrs;
        while ( mm.shoe.totalChunks() < minDecks ) {
            char * p = mm.allocate(slotSize);
            REQUIRE(p != nullptr);
            ptrs.push_back(p);
        }
        return ptrs;
    }
    void collectKeeping ( MemoryModel & mm, const std::vector<char*> & survivors, uint64_t slotSize ) {
        mm.shoe.beforeGC();
        for ( auto p : survivors ) mm.shoe.mark(p, uint32_t(slotSize));
        mm.sweep();
    }
}

TEST_CASE("MemoryModel sweep releases empty decks and keeps the head deck") {
    MemoryModel mm;
    mm.setInitialSize(4096);
    const uint64_t slotSize = 32;
    auto ptrs = fillDecks(mm, slotSize, 4);
    const uint32_t decksBefore = mm.shoe.totalChunks();
    const uint64_t reservedBefore = mm.totalAlignedMemoryAllocated();
    REQUIRE(decksBefore >= 4);

    Deck * head = mm.shoe.chunks[(slotSize >> 4) - 1];
    std::vector<char*> survivors;
    char * doomed = nullptr;
    for ( auto p : ptrs ) {
        if ( head->isOwnPtr(p) ) survivors.push_back(p);
        else doomed = p;
    }
    REQUIRE(!survivors.empty());
    REQUIRE(doomed != nullptr);
    REQUIRE(mm.shoe.isOwnPtr(doomed, uint32_t(slotSize)));
    REQUIRE(mm.shoe.lastChunk != nullptr);

    collectKeeping(mm, survivors, slotSize);
    CHECK(mm.shoe.lastChunk == nullptr);
    CHECK(mm.shoe.totalChunks() == 1);
    CHECK(mm.shoe.chunks[(slotSize >> 4) - 1] == head);
    CHECK(mm.totalAlignedMemoryAllocated() < reservedBefore);
    CHECK(mm.bytesAllocated() == uint64_t(survivors.size()) * slotSize);
    for ( auto p : survivors ) {
        CHECK(mm.isOwnPtr(p, slotSize));
        CHECK(mm.isAllocatedPtr(p, slotSize));
    }
    auto more = fillDecks(mm, slotSize, 2);
    CHECK(mm.shoe.totalChunks() == 2);
    for ( auto p : more ) CHECK(mm.isOwnPtr(p, slotSize));

    collectKeeping(mm, {}, slotSize);
    CHECK(mm.shoe.totalChunks() == 1);
    CHECK(mm.bytesAllocated() == 0);
    CHECK(mm.totalAlignedMemoryAllocated() > 0);
}

TEST_CASE("MemoryModel shrink releases every empty deck, the head included") {
    MemoryModel mm;
    mm.setInitialSize(4096);
    const uint64_t slotSize = 48;
    fillDecks(mm, slotSize, 3);
    collectKeeping(mm, {}, slotSize);
    REQUIRE(mm.shoe.totalChunks() == 1);
    mm.shrink();
    CHECK(mm.shoe.totalChunks() == 0);
    CHECK(mm.totalAlignedMemoryAllocated() == 0);
    char * p = mm.allocate(slotSize);
    REQUIRE(p != nullptr);
    mm.shrink();
    CHECK(mm.shoe.totalChunks() == 1);
    CHECK(mm.isAllocatedPtr(p, slotSize));
}
#endif
