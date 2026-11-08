options gen2
options indenting = 4

require daslib/ast
require daslib/ast_match
require dastest/testing_boost public

// ``qn`` is a pure macro-context helper that builds a backtick-quoted qname
// from a prefix + LineInfo. These tests pin the byte-output contract so
// downstream macros can rely on it.

[test(description = "qn builds a name from the prefix and the hex line and column")]
def test_basic(t : T?) {
    var at = LineInfo()
    at.line = 42u
    at.column = 7u
    // ``{uint}`` formats as hex by default — matches the existing linq_fold qname idiom.
    t |> equal(qn("src", at), "`src`0x2a`0x7")
}

[test(description = "qn gives distinct names to distinct prefixes at one position")]
def test_prefix_distinct(t : T?) {
    var at = LineInfo()
    at.line = 100u
    at.column = 1u
    t |> success(qn("buf", at) != qn("acc", at), "distinct prefixes must produce distinct qnames")
}

[test(description = "qn gives distinct names to one prefix at distinct positions")]
def test_position_distinct(t : T?) {
    var at1 = LineInfo()
    at1.line = 1u
    at1.column = 1u
    var at2 = LineInfo()
    at2.line = 1u
    at2.column = 2u
    t |> success(qn("x", at1) != qn("x", at2), "same prefix at distinct columns must produce distinct qnames")
}

[test(description = "qn of a zero LineInfo yields the zero-position name")]
def test_zero_lineinfo(t : T?) {
    // Documented behavior: synthesized LineInfo (line=0, column=0) produces
    // `prefix`0`0 and WILL collide across distinct synth sites with the same
    // prefix. Pinned here so future code knows to build a synth-specific name.
    let at = LineInfo()
    t |> equal(qn("synth", at), "`synth`0x0`0x0")
    let at2 = LineInfo()
    t |> equal(qn("synth", at), qn("synth", at2))
}
