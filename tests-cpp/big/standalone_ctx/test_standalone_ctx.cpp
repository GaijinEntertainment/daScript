#include "daScript/daScript.h"
#include "standalone_init_fixture.das.h"

using namespace das;

int main( int, char * [] ) {
    standalone_init_fixture::Standalone ctx;
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
    standalone_init_fixture::Pair madePair = ctx.make_pair(3, 4);
    expect("make_pair(3,4).a", madePair.a, 3);
    expect("make_pair(3,4).b", madePair.b, 4);
    expect("pair_sum(make_pair(3,4))", ctx.pair_sum(madePair), 7);
    standalone_init_fixture::Pair embedderPair;
    embedderPair.a = 20;
    embedderPair.b = 22;
    expect("pair_sum(embedder-built Pair)", ctx.pair_sum(embedderPair), 42);
    expect("flip(on)", int32_t(ctx.flip(standalone_init_fixture::Mode::on)),
        int32_t(standalone_init_fixture::Mode::off));
    expect("stack.size() honors options stack", ctx.stack.size() >= 262144 ? 1 : 0, 1);
    return failures ? 1 : 0;
}
