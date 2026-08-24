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
    return failures ? 1 : 0;
}
