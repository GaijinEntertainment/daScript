#include "daScript/daScript.h"
#include "standalone_init_fixture.das.h"

using namespace das;

int main( int, char * [] ) {
    ctx_standalone_init_fixture::Standalone ctx;
    TextPrinter tout;
    int failures = 0;
    auto expect = [&]( const char * name, int32_t have, int32_t want ) {
        if ( have != want ) {
            tout << name << " = " << have << ", expected " << want << "\n";
            failures ++;
        }
    };
    expect("get_first()", ctx.get_first(), 31);
    expect("get_second()", ctx.get_second(), 2);
    expect("get_init_fn_stamp()", ctx.get_init_fn_stamp(), 3);
    expect("get_reads_forward()", ctx.get_reads_forward(), 100);
    expect("get_later()", ctx.get_later(), 7);
    ctx_standalone_init_fixture::Pair madePair = ctx.make_pair(3, 4);
    expect("make_pair(3,4).a", madePair.a, 3);
    expect("make_pair(3,4).b", madePair.b, 4);
    expect("pair_sum(make_pair(3,4))", ctx.pair_sum(madePair), 7);
    ctx_standalone_init_fixture::Pair embedderPair;
    embedderPair.a = 20;
    embedderPair.b = 22;
    expect("pair_sum(embedder-built Pair)", ctx.pair_sum(embedderPair), 42);
    expect("flip(on)", int32_t(ctx.flip(ctx_standalone_init_fixture::Mode::on)),
        int32_t(ctx_standalone_init_fixture::Mode::off));
    constexpr uint32_t kFixtureStackSize = 262144;
    expect("stack.size() exceeds options stack by the init headroom",
        ctx.stack.size() > kFixtureStackSize ? 1 : 0, 1);
    expect("apply_lambda(10)", ctx.apply_lambda(10), 16);
    expect("call_through_pointer(21)", ctx.call_through_pointer(21), 42);
    expect("sum_generator(5)", ctx.sum_generator(5), 10);
    auto getFirst = ctx.findFunction("get_first");
    expect("findFunction(get_first)", getFirst != nullptr ? 1 : 0, 1);
    expect("fnByMangledName(get_first)", getFirst && ctx.fnByMangledName(getFirst->mangledNameHash) == getFirst ? 1 : 0, 1);
    bool unique = false;
    expect("findFunction(get_first, unique)", ctx.findFunction("get_first", unique) == getFirst && unique ? 1 : 0, 1);
    expect("findFunctions(get_first).size()", int32_t(ctx.findFunctions("get_first").size()), 1);
    expect("findFunction(nope)", ctx.findFunction("nope") == nullptr ? 1 : 0, 1);
    expect("findFunctions(nope).size()", int32_t(ctx.findFunctions("nope").size()), 0);
    int later = ctx.findVariable("g_later");
    expect("findVariable(g_later)", later >= 0 ? 1 : 0, 1);
    expect("getVariable(g_later)", later >= 0 ? *(int32_t *) ctx.getVariable(later) : -1, 7);
    expect("findVariable(nope)", ctx.findVariable("nope"), -1);
    // a shared global sits in the shared block: its emitted offset is the shared running size
    expect("get_shared_total()", ctx.get_shared_total(), 6);
    expect("renamed_sum(20,22)", ctx.renamed_sum(20, 22), 42);
    int sharedTaps = ctx.findVariable("g_shared_taps");
    expect("findVariable(g_shared_taps)", sharedTaps >= 0 ? 1 : 0, 1);
    expect("getVariable(g_shared_taps)[0]", sharedTaps >= 0 ? ((int32_t *) ctx.getVariable(sharedTaps))[0] : -1, 1);
    expect("getVariable(g_shared_taps)[2]", sharedTaps >= 0 ? ((int32_t *) ctx.getVariable(sharedTaps))[2] : -1, 3);
    return failures ? 1 : 0;
}
