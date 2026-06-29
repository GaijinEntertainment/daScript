// CFG construction (ast_cfg.h) - verifies block/edge structure for the core control-flow forms.
#include <doctest/doctest.h>

#include "daScript/daScript.h"
#include "daScript/ast/ast_cfg.h"

using namespace das;

namespace {

static ProgramPtr compileLiteral ( const char * src ) {
    TextPrinter tout;
    ModuleGroup dummyLibGroup;
    auto fAccess = make_smart<FsFileAccess>();
    const string name = "cfg_test.das";
    fAccess->setFileInfo(name, make_unique<TextFileInfo>(src, uint32_t(strlen(src)), /*own*/false));
    return compileDaScript(name, fAccess, tout, dummyLibGroup);
}

static Function * compileFn ( const char * src, const char * fn ) {
    auto prog = compileLiteral(src);
    REQUIRE(prog);
    REQUIRE_FALSE(prog->failed());
    auto f = prog->thisModule->findUniqueFunction(fn);
    REQUIRE(f);
    // keep the program alive for the duration of the test by leaking the smart_ptr into a static.
    static vector<ProgramPtr> keep;
    keep.push_back(prog);
    return f;
}

// proper loop detection: a DFS back-edge (edge to a node currently on the DFS stack). Block ids are
// not topological (the exit block is created early), so an id comparison would misfire.
static bool dfsCycle ( CfgBlock * b, vector<int> & state ) {   // 0 = unvisited, 1 = on-stack, 2 = done
    state[b->id] = 1;
    for ( auto s : b->succ ) {
        if ( state[s->id]==1 ) return true;
        if ( state[s->id]==0 && dfsCycle(s, state) ) return true;
    }
    state[b->id] = 2;
    return false;
}
static bool hasLoop ( const Cfg & cfg ) {
    vector<int> state(cfg.blocks.size(), 0);
    return cfg.entry ? dfsCycle(cfg.entry, state) : false;
}

}  // namespace

TEST_CASE("cfg: straight-line body is entry -> ... -> exit, no branches") {
    gc_guard guard;
    auto fn = compileFn(
        "options gen2\n" "options no_optimization\n"
        "[export] def f(a:int):int { var x = a + 1; var y = x * 2; return y }\n", "f");
    auto cfg = buildCfg(fn);
    CHECK(bool(cfg.entry != nullptr));
    CHECK(bool(cfg.exit != nullptr));
    CHECK_FALSE(hasLoop(cfg));
    CHECK(cfg.exit->pred.size() == 1);          // single return path
}

TEST_CASE("cfg: if/else with two returns branches then converges at exit") {
    gc_guard guard;
    auto fn = compileFn(
        "options gen2\n" "options no_optimization\n"
        "[export] def f(c:bool):int { if (c) { return 1 } else { return 2 } }\n", "f");
    auto cfg = buildCfg(fn);
    CHECK(cfg.entry->succ.size() == 2);         // then + else
    CHECK(cfg.exit->pred.size() == 2);          // both returns reach exit
    CHECK_FALSE(hasLoop(cfg));
}

TEST_CASE("cfg: while loop has a back-edge") {
    gc_guard guard;
    auto fn = compileFn(
        "options gen2\n" "options no_optimization\n"
        "[export] def f(n:int):int { var s = 0; while (s < n) { s += 1 }; return s }\n", "f");
    auto cfg = buildCfg(fn);
    CHECK(hasLoop(cfg));                     // body loops back to the header
    CHECK(cfg.exit->pred.size() >= 1);
}
