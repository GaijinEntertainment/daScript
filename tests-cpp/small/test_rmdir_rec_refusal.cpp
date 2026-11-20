#include <doctest/doctest.h>
#include "daScript/daScript.h"
#include "daScript/simulate/aot_builtin_fio.h"

using namespace das;

#if !DAS_NO_FILEIO

static string refusal_of ( const char * path ) {
    auto reason = rmdir_rec_refusal(path);
    return reason ? string(reason) : string();
}

TEST_CASE("rmdir_rec refuses an empty path") {
    CHECK_EQ(refusal_of(nullptr), "the path is empty");
    CHECK_EQ(refusal_of(""), "the path is empty");
}

TEST_CASE("rmdir_rec refuses a filesystem root") {
    for ( auto path : { "/", "//" } ) {
        INFO(path);
        CHECK_EQ(refusal_of(path), "the path is a filesystem root");
    }
}

TEST_CASE("rmdir_rec refuses a path whose last component is . or ..") {
    for ( auto path : { ".", "..", "./", "a/.", "a/..", "a/../" } ) {
        INFO(path);
        CHECK_EQ(refusal_of(path), "the path's last component is '.' or '..'");
    }
}

TEST_CASE("rmdir_rec allows a path that names a directory below a root") {
    for ( auto path : { "a", "a/b", "a/b/", "/tmp/x" } ) {
        INFO(path);
        CHECK(rmdir_rec_refusal(path) == nullptr);
    }
}

#if defined(_WIN32)
TEST_CASE("rmdir_rec refuses a drive and a drive root") {
    for ( auto path : { "C:", "C:\\" } ) {
        INFO(path);
        CHECK_EQ(refusal_of(path), "the path is a filesystem root");
    }
}

TEST_CASE("rmdir_rec refuses a share root") {
    for ( auto path : { "\\\\server\\share", "\\\\server\\share\\" } ) {
        INFO(path);
        CHECK_EQ(refusal_of(path), "the path is a filesystem root");
    }
}

TEST_CASE("rmdir_rec refuses a root under the \\\\?\\ prefix") {
    for ( auto path : { "\\\\?\\C:\\", "\\\\?\\UNC\\server\\share\\" } ) {
        INFO(path);
        CHECK_EQ(refusal_of(path), "the path is a filesystem root");
    }
}

TEST_CASE("rmdir_rec refuses . and .. relative to a drive") {
    for ( auto path : { "C:.", "C:.." } ) {
        INFO(path);
        CHECK_EQ(refusal_of(path), "the path's last component is '.' or '..'");
    }
}

TEST_CASE("rmdir_rec allows a directory below a drive or a share") {
    for ( auto path : { "C:\\dir", "\\\\server\\share\\dir", "\\\\?\\C:\\dir" } ) {
        INFO(path);
        CHECK(rmdir_rec_refusal(path) == nullptr);
    }
}
#endif

#endif
