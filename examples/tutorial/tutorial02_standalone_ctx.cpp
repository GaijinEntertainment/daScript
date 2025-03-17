#include "daScript/daScript.h"
#include "_standalone_ctx_generated/context.das.h"

int main( int, char * [] ) {
    auto ctx = das::context::Standalone();
    ctx.test();
    return 0;
}
