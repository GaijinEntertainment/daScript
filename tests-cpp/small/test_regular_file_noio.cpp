#include <doctest/doctest.h>
#include "daScript/daScript.h"
#include "daScript/misc/sysos.h"

#if DAS_NO_FILEIO
TEST_CASE("regular file opener rejects reads when file IO is disabled") {
    uint64_t size = 17;
    CHECK(das::das_fopen_regular_read_utf8("unused", size) == nullptr);
    CHECK(size == 0);
}
#endif
