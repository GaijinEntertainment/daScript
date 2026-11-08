options gen2

require dastest/testing_boost public
require dastest/file_census

let SUITE_TEXT = ("options gen2\noptions indenting = 4\noptions persistent_heap, " +
    "no_aot\n//! the header line\nrequire dastest/testing_boost public\n\n// a comment " +
        "holding [test] does not count\n[test(description = \"first claim\", timeout = " +
            "2.5)]\ndef test_first(t : T?) \{\n    t |> run(\"arm one\") <| @(t : T?) \{\n" +
            "        t |> equal(1, 1)\n    \}\n    t |> run(\"arm two\") <| @(t : T?) \{\n" +
            "        t |> equal(2, 2)\n    \}\n\}\n\n" +
                "[test, no_jit]\ndef test_second(t : T?) \{\n\}\n")

[test(description = "the census reads a file's tests, arms, descriptions, timeouts, header and fence signature from its text, and classes suites, fixtures and scripts apart")]
def test_file_census(tt : T?) {  // nolint:STYLE038 - one arm per census fact
    tt |> run("a suite: two tests, two arms, one description, one timeout, a header, a " +
        "fence signature") <| @(t : T?) {
        let c <- census_text("fio/test_x.das", SUITE_TEXT)
        t |> equal(c.cls, TestFileClass.suite)
        t |> equal(c.folder, "fio")
        t |> equal(c.stem, "test_x")
        t |> equal(length(c.tests), 2)
        t |> equal(c.tests[0].funcName, "test_first")
        t |> equal(c.tests[0].description, "first claim")
        t |> equal(c.tests[0].timeoutSec, 2.5)
        t |> equal(c.tests[0].line, 8)
        t |> equal(c.tests[1].funcName, "test_second")
        t |> equal(c.tests[1].description, "")
        t |> equal(c.tests[0].standalone, "")
        let alone <- census_text("x.das", "[test(description = \"d\", standalone = " +
            "\"fills a registry\")]\ndef test_a(t : T?) \{\}\n")
        t |> equal(alone.tests[0].standalone, "fills a registry")
        t |> equal(c.arms, 2)
        t |> equal(c.header, "the header line")
        t |> equal(fence_signature(c), "no_aot,persistent_heap")
        t |> success(c.noAot)
        let moduleOption <- census_text("x.das",
            "options _flatten_expand_lerp\noptions _nolint\noptions _function_length = 0\n" +
            "[test]\ndef test_a(t : T?) \{\}\n")
        t |> equal(fence_signature(moduleOption), "_flatten_expand_lerp")
        let valued <- census_text("x.das", "options auto_inline_functions = " +
            "false\noptions stack = 65536\noptions rtti = true\n[test]\ndef test_a(t : T?) \{\}\n")
        t |> equal(fence_signature(valued), "auto_inline_functions=false,rtti,stack=65536")
        let gen1 <- census_text("x.das",
                                "options gen2 = false\n[test]\ndef test_a(t : T?)\n    pass\n")
        t |> equal(fence_signature(gen1), "gen2=false")
        t |> equal(length(c.unparsedAnnotationLines), 0)
    }

    tt |> run("an expect directive or a failed_ name makes a fixture; no test and no " +
        "expect makes a script") <| @(t : T?) {
        let byExpect <- census_text("language/x.das", "expect 30101\ndef main() \{\}\n")
        t |> equal(byExpect.cls, TestFileClass.fixture)
        let byName <- census_text("language/failed_x.das", "def main() \{\}\n")
        t |> equal(byName.cls, TestFileClass.fixture)
        let script <- census_text("language/plain.das", "options gen2\n[export]\ndef main() \{\}\n")
        t |> equal(script.cls, TestFileClass.script)
    }

    tt |> run("comments are stripped, spellings and escapes are read, options are " +
        "deduplicated, bare run calls count") <| @(t : T?) {
        let c <- census_text("x.das", "options rtti // why\noptions " +
            "rtti\n[test(description = \"says \\\"quoted\\\" words\")] // trailing " +
                "note\ndef test_a(t : T?) \{\n    run(t, \"bare arm\")\n\}\n[no_jit, " +
                    "test]\ndef test_b(t : T?) \{\n\}\n")
        t |> equal(fence_signature(c), "rtti")
        t |> equal(length(c.tests), 2)
        t |> equal(c.tests[0].description, "says \\\"quoted\\\" words")
        t |> equal(length(c.unparsedAnnotationLines), 0)
        t |> equal(c.tests[1].funcName, "test_b")
        t |> equal(c.arms, 1)
        let cant <- census_text("language/cant_x.das", "def main() \{\}\n")
        t |> equal(cant.cls, TestFileClass.fixture)
        let invalid <- census_text("language/invalid_x.das", "def main() \{\}\n")
        t |> equal(invalid.cls, TestFileClass.fixture)
    }

    tt |> run("a [test( annotation that does not close on its line is recorded as " +
        "unparsed") <| @(t : T?) {
        let c <- census_text("x.das", "[test(description = \"spills\",\n    timeout = " +
            "3)]\ndef test_a(t : T?) \{\}\n")
        t |> equal(length(c.unparsedAnnotationLines), 1)
        t |> equal(c.unparsedAnnotationLines[0], 1)
        t |> equal(c.folder, ".")
    }

    tt |> run("dastest's run rule: an underscore or dot on any path component excludes " +
        "the file, and so does the gate's name") <| @(t : T?) {
        t |> success(is_run_file("fio/test_x.das"))
        t |> success(!is_run_file("fio/_fixture.das"))
        t |> success(!is_run_file("_fixtures/x.das"))
        t |> success(!is_run_file("fio/.hidden.das"))
        t |> success(!is_run_file("fio/notes.md"))
        t |> success(!is_run_file("REVIEW.das"))
    }

    tt |> run("the header is the first //! line of the leading block; a //! after the " +
        "first declaration is not a header") <| @(t : T?) {
        let late <- census_text("x.das", "options gen2\n[test]\ndef test_a(t : T?) \{\n   " +
            " //! a doc inside the body\n\}\n")
        t |> equal(late.header, "")
        let early <- census_text("x.das", "options gen2\nrequire strings\n//! the " +
            "claim\nexpect 30101\ndef main() \{\}\n")
        t |> equal(early.header, "the claim")
        let wrapped <- census_text("x.das", "//! a claim that\n//! wraps\n\n//! a second " +
            "block is not the header\ndef main() \{\}\n")
        t |> equal(wrapped.header, "a claim that wraps")
    }
}
