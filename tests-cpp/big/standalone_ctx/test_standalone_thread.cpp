#include "daScript/daScript.h"
#include "standalone_thread_fixture.das.h"

using namespace das;

int main( int, char * [] ) {
    ctx_standalone_thread_fixture::Standalone ctx;
    int32_t have = ctx.threaded_sum();
    if ( have != 100 ) {
        TextPrinter tout;
        tout << "threaded_sum() = " << have << ", expected 100\n";
        return 1;
    }
    return 0;
}
