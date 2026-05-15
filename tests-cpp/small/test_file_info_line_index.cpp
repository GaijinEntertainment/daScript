#include <doctest/doctest.h>

#include "daScript/daScript.h"
#include "daScript/simulate/debug_info.h"
#include "daScript/simulate/aot_builtin_rtti.h"

#include <cstring>

using namespace das;

namespace {

// Build a TextFileInfo over a literal source buffer. `own=false` means the
// FileInfo doesn't try to free our string-literal storage.
static TextFileInfo make_info(const char * src) {
    return TextFileInfo(src, uint32_t(strlen(src)), /*own*/false);
}

// Pull `line` out of `info` as a NUL-terminated std::string for easy CHECK comparisons.
static string line_at(FileInfo & info, uint32_t line) {
    const char * begin = nullptr;
    uint32_t len = 0;
    if ( !info.getLine(line, begin, len) ) return "<<miss>>";
    return string(begin, len);
}

}  // namespace

TEST_CASE("FileInfo::getLine basic cases") {
    SUBCASE("three lines with LF endings") {
        auto info = make_info("alpha\nbeta\ngamma\n");
        CHECK(line_at(info, 1) == "alpha");
        CHECK(line_at(info, 2) == "beta");
        CHECK(line_at(info, 3) == "gamma");
    }

    SUBCASE("last line with no trailing newline") {
        auto info = make_info("alpha\nbeta\ngamma");
        CHECK(line_at(info, 3) == "gamma");
    }

    SUBCASE("out-of-range line returns false") {
        auto info = make_info("alpha\nbeta\n");
        const char * begin = nullptr;
        uint32_t len = 0;
        CHECK_FALSE(info.getLine(0, begin, len));
        CHECK_FALSE(info.getLine(99, begin, len));
    }

    SUBCASE("empty source returns false") {
        auto info = make_info("");
        const char * begin = nullptr;
        uint32_t len = 0;
        CHECK_FALSE(info.getLine(1, begin, len));
    }

    SUBCASE("single line, no trailing newline") {
        auto info = make_info("only");
        CHECK(line_at(info, 1) == "only");
        const char * begin = nullptr;
        uint32_t len = 0;
        CHECK_FALSE(info.getLine(2, begin, len));
    }

    SUBCASE("CRLF endings — trailing CR is stripped") {
        auto info = make_info("alpha\r\nbeta\r\n");
        CHECK(line_at(info, 1) == "alpha");
        CHECK(line_at(info, 2) == "beta");
    }

    SUBCASE("buildLineIndex is idempotent") {
        auto info = make_info("a\nb\nc\n");
        info.buildLineIndex();
        info.buildLineIndex();
        CHECK(line_at(info, 2) == "b");
    }
}

TEST_CASE("rtti_is_nolint_suppressed truth table") {
    auto info = make_info(
        // line 1: bare suppression for LINT001
        "let x = 1 // nolint:LINT001\n"
        // line 2: comma-list suppression for LINT001,LINT002
        "let y = 2 // nolint:LINT001,LINT002\n"
        // line 3: whitespace around comma-list tokens
        "let z = 3 // nolint:LINT001 , LINT002\n"
        // line 4: trailing prose after the suppressed code
        "let w = 4 // nolint:PERF003 single access for operator parsing\n"
        // line 5: no `//`
        "let q = 5\n"
        // line 6: `// nolint` without `:CODE`
        "let r = 6 // nolint\n"
        // line 7: substring trap — LINT00 must not match LINT001
        "let s = 7 // nolint:LINT001\n"
        // line 8: repeated `nolint:` directives, space-separated
        "let r2 = 8 // nolint:LINT002 nolint:LINT003\n"
        // line 9: directive mid-comment (lenient match anywhere after //)
        "let r3 = 9 // free prose nolint:PERF018 trailing words\n"
    );

    auto suppressed = [&](uint32_t line, const char * code) {
        return rtti_is_nolint_suppressed(&info, line, code, nullptr, nullptr);
    };

    SUBCASE("single-code line") {
        CHECK(suppressed(1, "LINT001"));
        CHECK_FALSE(suppressed(1, "LINT002"));
    }

    SUBCASE("comma-list matches both") {
        CHECK(suppressed(2, "LINT001"));
        CHECK(suppressed(2, "LINT002"));
        CHECK_FALSE(suppressed(2, "LINT003"));
    }

    SUBCASE("whitespace around tokens tolerated") {
        CHECK(suppressed(3, "LINT001"));
        CHECK(suppressed(3, "LINT002"));
    }

    SUBCASE("trailing prose after code is fine") {
        CHECK(suppressed(4, "PERF003"));
        CHECK_FALSE(suppressed(4, "single"));
    }

    SUBCASE("no comment at all -> false") {
        CHECK_FALSE(suppressed(5, "LINT001"));
    }

    SUBCASE("bare `// nolint` without colon -> false") {
        CHECK_FALSE(suppressed(6, "LINT001"));
    }

    SUBCASE("substring trap: LINT00 must NOT match LINT001") {
        CHECK_FALSE(suppressed(7, "LINT00"));
        CHECK(suppressed(7, "LINT001"));
    }

    SUBCASE("null / empty code -> false") {
        CHECK_FALSE(rtti_is_nolint_suppressed(&info, 1, nullptr, nullptr, nullptr));
        CHECK_FALSE(rtti_is_nolint_suppressed(&info, 1, "", nullptr, nullptr));
    }

    SUBCASE("null info -> false") {
        CHECK_FALSE(rtti_is_nolint_suppressed(nullptr, 1, "LINT001", nullptr, nullptr));
    }

    SUBCASE("repeated `nolint:` directives in one comment, space-separated") {
        CHECK(suppressed(8, "LINT002"));
        CHECK(suppressed(8, "LINT003"));
        CHECK_FALSE(suppressed(8, "LINT001"));
    }

    SUBCASE("directive can appear anywhere after `//`") {
        CHECK(suppressed(9, "PERF018"));
        CHECK_FALSE(suppressed(9, "free"));
        CHECK_FALSE(suppressed(9, "prose"));
    }
}
