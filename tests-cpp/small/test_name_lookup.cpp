#include <doctest/doctest.h>

#include "daScript/daScript.h"
#include "daScript/simulate/name_lookup.h"

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

using namespace das;

#define SCRIPT_PATH "/tests-cpp/small/test_name_lookup.das"

namespace {

std::string mangledOf ( uint32_t i ) {
    return "fn_" + std::to_string(i / 4) + " (" + std::to_string(i % 4) + ")";
}

}

TEST_CASE("NameLookup - sealed perfect hash answers every key and no other") {
    const uint32_t N = 10000;
    std::vector<std::string> names, mangled;
    std::vector<uint64_t> mnhs;
    for ( uint32_t i=0; i!=N; ++i ) {
        names.push_back("fn_" + std::to_string(i / 4));
        mangled.push_back(mangledOf(i));
        mnhs.push_back(NameLookup::hashName(mangled.back().c_str()));
    }
    NameLookup lookup;
    for ( uint32_t i=0; i!=N; ++i ) lookup.insert(mnhs[i], names[i].c_str(), i, i * 16);
    CHECK_FALSE(lookup.isSealed());
    string failure;
    REQUIRE_MESSAGE(lookup.seal(&failure), failure);
    CHECK(lookup.isSealed());
    CHECK_EQ(lookup.size(), N);
    for ( uint32_t i=0; i!=N; ++i ) {
        CHECK_EQ(lookup.valueByMnh(mnhs[i]), i * 16);
    }
    CHECK_EQ(lookup.valueByMnh(0), NameLookup::NOT_FOUND);
    CHECK_EQ(lookup.valueByMnh(NameLookup::hashName("not a mangled name")), NameLookup::NOT_FOUND);
    for ( uint32_t g=0; g!=N / 4; ++g ) {
        std::string name = "fn_" + std::to_string(g);
        int32_t slot = lookup.headByName(name.c_str());
        REQUIRE_MESSAGE(slot >= 0, name);
        uint32_t seen = 0;
        for ( ; slot >= 0; slot = lookup.nextSameName(slot), ++seen ) {
            CHECK_EQ(lookup.indexAt(slot), g * 4 + seen);
            CHECK_EQ(lookup.valueAt(slot), (g * 4 + seen) * 16);
        }
        CHECK_EQ(seen, 4u);
    }
    CHECK_EQ(lookup.headByName("fn_"), -1);
    CHECK_EQ(lookup.headByName("nope"), -1);
    CHECK_EQ(lookup.headByName(""), -1);
    CHECK_EQ(lookup.headByName(nullptr), -1);
}

TEST_CASE("NameLookup - empty and single-entry tables") {
    SUBCASE("empty") {
        NameLookup lookup;
        REQUIRE(lookup.seal());
        CHECK_EQ(lookup.size(), 0u);
        CHECK_EQ(lookup.valueByMnh(12345), NameLookup::NOT_FOUND);
        CHECK_EQ(lookup.headByName("x"), -1);
    }
    SUBCASE("single") {
        NameLookup lookup;
        lookup.insert(777, "only", 0, 99);
        REQUIRE(lookup.seal());
        CHECK_EQ(lookup.valueByMnh(777), 99u);
        CHECK_EQ(lookup.valueByMnh(778), NameLookup::NOT_FOUND);
        int32_t slot = lookup.headByName("only");
        REQUIRE(slot >= 0);
        CHECK_EQ(lookup.indexAt(slot), 0u);
        CHECK_EQ(lookup.nextSameName(slot), -1);
    }
}

TEST_CASE("NameLookup - an adopted table answers like the one that built it and owns nothing") {
    NameLookup built;
    std::vector<std::string> names, mangled;
    for ( uint32_t i=0; i!=300; ++i ) {
        names.push_back("g_" + std::to_string(i / 3));
        mangled.push_back(names.back() + "#" + std::to_string(i % 3));
        built.insert(NameLookup::hashName(mangled.back().c_str()), names.back().c_str(), i, i * 16);
    }
    REQUIRE(built.seal());
    CHECK(built.isOwned());
    // the arrays an emitter would write out, held here in the caller's own storage
    std::vector<NameLookup::Entry> entries(built.entries, built.entries + built.byMnh.nslots);
    std::vector<NameLookup::NameSlot> slots(built.names, built.names + built.byName.nslots);
    std::vector<uint32_t> mnhDisp(built.byMnh.disp, built.byMnh.disp + built.byMnh.nbuckets);
    std::vector<uint32_t> nameDisp(built.byName.disp, built.byName.disp + built.byName.nbuckets);
    NameLookup::StaticTable table = { built.byMnh.nbuckets, built.byMnh.nslots, built.byName.nbuckets, built.byName.nslots,
        built.count, 0, mnhDisp.data(), nameDisp.data(), entries.data(), slots.data() };
    {
        NameLookup adopted;
        adopted.adopt(table);
        CHECK(adopted.isSealed());
        CHECK_FALSE(adopted.isOwned());
        CHECK_EQ(adopted.size(), built.size());
        for ( uint32_t i=0; i!=300; ++i ) {
            auto mnh = NameLookup::hashName(mangled[i].c_str());
            CHECK_EQ(adopted.valueByMnh(mnh), built.valueByMnh(mnh));
        }
        for ( uint32_t g=0; g!=100; ++g ) {
            std::string name = "g_" + std::to_string(g);
            int32_t a = adopted.headByName(name.c_str()), b = built.headByName(name.c_str());
            for ( ; a >= 0 && b >= 0; a = adopted.nextSameName(a), b = built.nextSameName(b) ) {
                CHECK_EQ(adopted.indexAt(a), built.indexAt(b));
            }
            CHECK_EQ(a, b);
        }
        CHECK_EQ(adopted.headByName("g_100"), -1);
        CHECK_EQ(adopted.valueByMnh(1), NameLookup::NOT_FOUND);
    }
    // the adopted object is gone; the storage it pointed at is untouched
    CHECK_EQ(entries.size(), size_t(built.byMnh.nslots));
    CHECK_EQ(built.valueByMnh(NameLookup::hashName(mangled[7].c_str())), 7u * 16);
}

TEST_CASE("NameLookup - an inserted name need not outlive the insert") {
    NameLookup lookup;
    char scratch[32];
    for ( uint32_t i=0; i!=40; ++i ) {
        // one buffer, rewritten before every insert: two overloads of `same`, then names that differ
        snprintf(scratch, sizeof(scratch), i < 2 ? "same" : "other_%u", i);
        lookup.insert(1000 + i, scratch, i, i);
        memset(scratch, 'x', sizeof(scratch) - 1);
        scratch[sizeof(scratch) - 1] = 0;
    }
    string failure;
    REQUIRE_MESSAGE(lookup.seal(&failure), failure);
    int32_t slot = lookup.headByName("same");
    REQUIRE(slot >= 0);
    CHECK_EQ(lookup.indexAt(slot), 0u);
    REQUIRE(lookup.nextSameName(slot) >= 0);
    CHECK_EQ(lookup.indexAt(lookup.nextSameName(slot)), 1u);
    CHECK_EQ(lookup.nextSameName(lookup.nextSameName(slot)), -1);
    CHECK(lookup.headByName("other_7") >= 0);
    CHECK_EQ(lookup.headByName("xxxx"), -1);
}

TEST_CASE("NameLookup - a duplicate mangled-name hash refuses to seal and names both entries") {
    NameLookup lookup;
    lookup.insert(1, "first", 0, 0);
    lookup.insert(2, "second", 1, 1);
    lookup.insert(1, "third", 2, 2);
    string failure;
    CHECK_FALSE(lookup.seal(&failure));
    CHECK(failure.find("first") != string::npos);
    CHECK(failure.find("third") != string::npos);
    // the failed object answers every probe with a miss, and a second seal fails the same way
    CHECK_FALSE(lookup.isSealed());
    CHECK_EQ(lookup.size(), 0u);
    CHECK_EQ(lookup.valueByMnh(1), NameLookup::NOT_FOUND);
    CHECK_EQ(lookup.valueByMnh(2), NameLookup::NOT_FOUND);
    CHECK_EQ(lookup.headByName("first"), -1);
    string again;
    CHECK_FALSE(lookup.seal(&again));
    CHECK_EQ(again, failure);
}

TEST_CASE("NameLookup - a fresh object answers every probe with a miss") {
    NameLookup fresh;
    CHECK_FALSE(fresh.isSealed());
    CHECK_FALSE(fresh.isOwned());
    CHECK_EQ(fresh.size(), 0u);
    CHECK_EQ(fresh.valueByMnh(0), NameLookup::NOT_FOUND);
    CHECK_EQ(fresh.valueByMnh(0x1234567887654321ull), NameLookup::NOT_FOUND);
    CHECK_EQ(fresh.headByName("anything"), -1);
    CHECK_EQ(fresh.headByName(""), -1);
}

TEST_CASE("NameLookup - a sealed table refuses a second seal and keeps answering") {
    NameLookup lookup;
    lookup.insert(5, "only", 0, 7);
    REQUIRE(lookup.seal());
    string failure;
    CHECK_FALSE(lookup.seal(&failure));
    CHECK(failure.find("sealed twice") != string::npos);
    CHECK(lookup.isSealed());
    CHECK_EQ(lookup.valueByMnh(5), 7u);
}

TEST_CASE("NameLookup - a null name inserts as the empty name") {
    NameLookup lookup;
    lookup.insert(9, nullptr, 3, 3);
    lookup.insert(10, "named", 4, 4);
    REQUIRE(lookup.seal());
    int32_t slot = lookup.headByName("");
    REQUIRE(slot >= 0);
    CHECK_EQ(lookup.indexAt(slot), 3u);
    CHECK_EQ(lookup.headByName(nullptr), slot);
    CHECK_EQ(lookup.nextSameName(slot), -1);
}

TEST_CASE("Context - a context that never simulated misses every by-name lookup") {
    Context bare(4096);
    CHECK_EQ(bare.findFunction("main"), nullptr);
    bool unique = true;
    CHECK_EQ(bare.findFunction("main", unique), nullptr);
    CHECK_FALSE(unique);
    CHECK(bare.findFunctions("main").empty());
    CHECK(bare.findFunctions(nullptr).empty());
    CHECK_EQ(bare.findVariable("g"), -1);
    TextWriter report;
    bare.logMemInfo(report);
    CHECK(string(report.str()).find("functionLookup[0]") != string::npos);
}

TEST_CASE("Context - by-name and by-hash function and global lookups") {
    TextPrinter tout;
    ModuleGroup dummyLibGroup;
    auto fAccess = make_smart<FsFileAccess>();
    auto program = compileDaScript(getDasRoot() + SCRIPT_PATH, fAccess, tout, dummyLibGroup);
    REQUIRE_FALSE(program->failed());
    Context ctx(program->getContextStackSize());
    REQUIRE(program->simulate(ctx, tout));

    SUBCASE("overloads share a name") {
        auto foo = ctx.findFunction("foo");
        REQUIRE(foo != nullptr);
        CHECK(strcmp(foo->name, "foo") == 0);
        auto foos = ctx.findFunctions("foo");
        REQUIRE_EQ(foos.size(), 3u);
        CHECK_EQ(foos[0], foo);
        for ( size_t i=0; i!=foos.size(); ++i ) {
            CHECK(strcmp(foos[i]->name, "foo") == 0);
            for ( size_t j=i + 1; j!=foos.size(); ++j ) {
                CHECK(strcmp(foos[i]->mangledName, foos[j]->mangledName) != 0);
            }
            CHECK_EQ(ctx.fnByMangledName(foos[i]->mangledNameHash), foos[i]);
        }
        bool unique = true;
        CHECK_EQ(ctx.findFunction("foo", unique), foo);
        CHECK_FALSE(unique);
    }
    SUBCASE("a unique name") {
        bool unique = false;
        auto bar = ctx.findFunction("bar", unique);
        REQUIRE(bar != nullptr);
        CHECK(unique);
        CHECK(strcmp(bar->name, "bar") == 0);
        CHECK_EQ(ctx.findFunctions("bar").size(), 1u);
        CHECK_EQ(ctx.fnByMangledName(bar->mangledNameHash), bar);
    }
    SUBCASE("a missing name") {
        bool unique = true;
        CHECK_EQ(ctx.findFunction("nope"), nullptr);
        CHECK_EQ(ctx.findFunction("nope", unique), nullptr);
        CHECK_FALSE(unique);
        CHECK(ctx.findFunctions("nope").empty());
        CHECK_EQ(ctx.findFunction("fo"), nullptr);
        CHECK_EQ(ctx.findFunction("fooo"), nullptr);
        CHECK_EQ(ctx.findFunction(""), nullptr);
        CHECK_EQ(ctx.findFunction(nullptr), nullptr);
        CHECK_EQ(ctx.fnByMangledName(0), nullptr);
        CHECK_EQ(ctx.fnByMangledName(NameLookup::hashName("no such mangled name")), nullptr);
    }
    SUBCASE("globals by name and by hash") {
        int counter = ctx.findVariable("g_counter");
        REQUIRE(counter >= 0);
        CHECK(strcmp(ctx.getVariableInfo(counter)->name, "g_counter") == 0);
        int label = ctx.findVariable("g_label");
        REQUIRE(label >= 0);
        CHECK(strcmp(ctx.getVariableInfo(label)->name, "g_label") == 0);
        CHECK_NE(counter, label);
        CHECK_EQ(ctx.findVariable("nope"), -1);
        CHECK_EQ(ctx.findVariable("g_"), -1);
        CHECK_EQ(ctx.findVariable(nullptr), -1);
        for ( int i=0; i!=ctx.getTotalVariables(); ++i ) {
            auto gv = ctx.getGlobalVariable(i);
            CHECK_EQ(ctx.globalOffsetByMangledName(gv.mangledNameHash), gv.offset);
            CHECK_EQ(ctx.findVariable(gv.name), i);
        }
    }
    SUBCASE("a fork shares the sealed tables") {
        Context::CopyOptions opts;
        Context fork(ctx, opts);
        CHECK_EQ(fork.functionLookup.get(), ctx.functionLookup.get());
        CHECK_EQ(fork.variableLookup.get(), ctx.variableLookup.get());
        CHECK_EQ(fork.findFunction("bar"), ctx.findFunction("bar"));
        CHECK_EQ(fork.findFunctions("foo").size(), 3u);
        CHECK_EQ(fork.findVariable("g_scale"), ctx.findVariable("g_scale"));
    }
}
