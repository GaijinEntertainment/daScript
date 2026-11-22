options gen2
require dastest/testing_boost public
require daslib/regex
require _regex_test_helpers
require daslib/regex_boost
require daslib/random
require strings

def match_start(pat : string; s : string) : int {
    var r <- regex_compile(pat)
    return regex_search(r, s).x
}

def first_group(pat : string; s : string) : string {
    var r <- regex_compile(pat)
    if (regex_match(r, s) < 0) return "<no match>"
    return regex_group(r, 1, s)
}

[test(description = "a repeated alternation backs off to a shorter alternative that shares a prefix with the longer one")]
def test_a_repeated_alternation_retries_an_alternative_that_shares_a_prefix(t : T?) {
    t |> equal(match_start("x(aba|ab|b)*c", "xabac"), 0)
    t |> equal(match_start("(aba|ab|b)*c", "abac"), 0)
    t |> equal(match_start("x(aba|ab|b)+c", "xabac"), 0)
    t |> equal(match_start("x(aba|b)*c", "xabac"), 0, "no shared prefix")
    t |> equal(match_start("x(aba|ab|b)c", "xabac"), 0, "not repeated")
}

[test(description = "every quantifier form retries the alternatives of its subexpression when the rest fails")]
def test_every_quantifier_retries_the_alternatives_of_its_subexpression(t : T?) {
    t |> equal(regex_test_match("(ab|aba)?c", "abac"), 4, "greedy ?")
    t |> equal(regex_test_match("(ab|aba)??c", "abac"), 4, "lazy ?")
    t |> equal(regex_test_match("(ab|aba)*?c", "abac"), 4, "lazy *")
    t |> equal(regex_test_match("(ab|aba)+?c", "abac"), 4, "lazy +")
    t |> equal(regex_test_match("(ab|aba)\{0,3\}c", "abac"), 4, "bounded repeat")
    t |> equal(regex_test_match("(ab|aba)\{1,\}c", "abac"), 4, "open repeat")
    t |> equal(regex_test_match("(ab|aba)\{0,3\}?c", "abac"), 4, "lazy bounded repeat")
}

[test(description = "among alternatives that all match, the leftmost one decides the match")]
def test_the_leftmost_alternative_wins(t : T?) {
    t |> equal(regex_test_match("(aba|ab|b)", "aba"), 3)
    t |> equal(regex_test_match("(ab|aba|b)", "aba"), 2)
}

[test(description = "a repeated group captures the text of its last pass")]
def test_a_repeated_group_captures_its_last_pass(t : T?) {
    t |> equal(first_group("(a|b)*c", "abc"), "b")
    t |> equal(first_group("x(ab|aba)*c", "xabac"), "aba")
    t |> equal(first_group("(\\w+)\\s", "hi yo"), "hi")
    t |> equal(first_group("((a+)(b+))c", "aabbbc"), "aabbb", "a group that ends in a group")
}

[test(description = "a repeated alternation matches and fails correctly over a 40000-character input")]
def test_a_repeated_alternation_runs_over_a_long_input(t : T?) {
    let input = repeat("ab", 20000) + "c"
    t |> equal(regex_test_match("(a|b)*c", input), 40001)
    t |> equal(regex_test_match("(a|b)*d", input), -1)
}

[test(description = "a subexpression that matches empty ends the repetition without hiding a longer alternative")]
def test_a_repeated_subexpression_that_matches_empty_ends_the_repetition(t : T?) {
    t |> equal(regex_test_match("(a*)*b", "aac"), -1)
    t |> equal(regex_test_match("(a*)*b", "aab"), 3)
    t |> equal(regex_test_match("(b?|ab)*c", "ababc"), 5,
        "an alternative that matches empty does not hide a longer one")
}

[test(description = "an inner quantifier is a choice point the outer quantifier retries")]
def test_a_nested_quantifier_is_a_choice_the_outer_one_retries(t : T?) {
    t |> equal(regex_test_match("(ab?)+bc", "abc"), 3)
    t |> equal(regex_test_match("(ab*)+bbc", "abbc"), 4)
}

[test(description = "a zero-width subexpression under plus matches once and ends the repetition")]
def test_a_zero_width_subexpression_under_plus_matches_once(t : T?) {
    t |> equal(regex_test_match("a\\b+", "a"), 1, "at the end of the input")
    t |> equal(regex_test_match("a\\b+ c", "a c"), 3,
        "a pass that matches empty ends the repetition")
    t |> equal(regex_test_match("(\\b)+a", "a"), 1)
}

[test(description = "a greedy quantifier takes the longest run and gives back only until the rest of the pattern accepts")]
def test_a_greedy_quantifier_takes_the_longest_run_the_rest_accepts(t : T?) {
    t |> equal(regex_test_match("a*a", "aaa"), 3)
    t |> equal(regex_test_match("x(ab)*ab", "xababab"), 7)
    t |> equal(regex_test_match("a+a", "aaa"), 3)
    t |> equal(regex_test_match(".*b", "abab"), 4, "the last b, not the first")
    t |> equal(regex_test_match("a.+b", "axbxb"), 5)
    t |> equal(regex_test_match("(ab)+a", "ababab"), 5, "one pass is given back")
    t |> equal(regex_test_match("a+b", "aaac"), -1, "giving back does not invent a match")
}

[test(description = "a greedy quantifier before an element that can match empty still takes the longest run")]
def test_a_greedy_quantifier_before_an_optional_element_takes_the_longest_run(t : T?) {
    t |> equal(regex_test_match("a+b?", "aaab"), 4)
    t |> equal(regex_test_match("a+(b)?", "aaab"), 4)
    t |> equal(regex_test_match("a+b*", "aaab"), 4)
    t |> equal(regex_test_match("a*b?", "aaab"), 4)
    t |> equal(regex_test_match("[a-z]+[xy]?", "abcd"), 4)
    t |> equal(regex_test_match("[a-z]*[xy]?", "abcd"), 4)
    t |> equal(regex_test_match("[1-9][0-9]*(u|U)?", "2048"), 4)
    t |> equal(regex_test_match("[0-9]+[uUlL]?", "2048u"), 5,
        "the optional suffix is taken when it is there")
    t |> equal(regex_test_match("\\w+\\s*", "abc   d"), 6)
    var re <- regex_compile("(a+)(b?)")
    t |> equal(regex_match(re, "aaab"), 4)
    t |> equal(regex_group(re, 1, "aaab"), "aaa", "the run is captured whole")
    t |> equal(regex_group(re, 2, "aaab"), "b")
    var quoted <- regex_compile("\".*\"")
    let span = regex_search(quoted, "say \"hi\" and \"bye\"")
    t |> equal(span.x, 4)
    t |> equal(span.y, 18, "a greedy search runs to the last quote")
}

[test(description = "a greedy group that gives passes back captures what the passes it keeps captured, and a run that fails leaves the captures as it found them")]
def test_a_greedy_group_that_gives_back_keeps_the_captures_of_the_kept_passes(t : T?) {
    t |> run("the last kept pass, not the one given back") @(t : T?) {
        var re <- regex_compile("(a|b)*a")
        t |> equal(regex_match(re, "aba"), 3)
        t |> equal(re[1], range(1, 2))
        var pairs <- regex_compile("(ab)+a")
        t |> equal(regex_match(pairs, "ababab"), 5)
        t |> equal(re_group_text(pairs, 1, "ababab"), "ab")
        t |> equal(pairs[1], range(2, 4))
        var counted <- regex_compile("(a|b)\{1,\}a")
        t |> equal(regex_match(counted, "aba"), 3)
        t |> equal(counted[1], range(1, 2))
    }
    t |> run("a group only an earlier kept pass wrote") @(t : T?) {
        var re <- regex_compile("((a)|b)*a")
        t |> equal(regex_match(re, "aba"), 3)
        t |> equal(re[1], range(1, 2))
        t |> equal(re[2], range(0, 1), "the pass given back wrote it too")
    }
    t |> run("every pass given back leaves the group empty") @(t : T?) {
        var re <- regex_compile("(a|b)*a")
        t |> equal(regex_match(re, "a"), 1)
        t |> equal(re[1], range(0))
    }
    t |> run("a run that fails leaves no capture behind") @(t : T?) {
        var re <- regex_compile("((a)*b|ac)")
        t |> equal(regex_match(re, "ac"), 2)
        t |> equal(re[2], range(0))
    }
    t |> run("a match leaves the stacks of every quantifier empty") @(t : T?) {
        var re <- regex_compile("x((ab)*a)+b")
        t |> equal(regex_match(re, "xabaab"), 6)
        var held = 0
        visit_top_down(re.root) $(var node) {
            held += length(node.run_stops) + length(node.run_captures)
        }
        t |> equal(held, 0)
    }
}

def re_group_text(re : Regex; index : int; text : string) : string => regex_group(re, index, text)

[test(description = "a lazy quantifier tries the rest of the pattern before taking another pass and still stops at its bound")]
def test_a_lazy_quantifier_tries_the_rest_before_another_pass(t : T?) {
    t |> equal(regex_test_match("(ab)+?", "abab"), 2)
    t |> equal(regex_test_match("(ab)??", "ab"), 0)
    t |> equal(regex_test_match("(ab)\{1,3\}?", "ababab"), 2)
    t |> equal(regex_test_match("(a|b)\{0,2\}?c", "aaac"), -1, "a lazy bound still stops")
    t |> equal(regex_test_match("a\{2,3\}?b", "aaaab"), -1, "a lazy character run still stops")
}

[test(description = "a consuming pass cannot start at the end of input while a zero-width body may")]
def test_a_pass_cannot_start_at_the_end_of_the_input(t : T?) {
    t |> equal(regex_test_match("x(a?)\{2\}", "x"), -1)
    t |> equal(regex_test_match("(a|$)\{2\}", "a"), -1)
    t |> equal(regex_test_match("a\\b\{1\}", "a"), 1, "a zero-width body may run there")
}

[test(description = "a mandatory pass that fails retries the other alternatives before the rest is tried")]
def test_a_mandatory_pass_that_fails_retries_the_alternatives(t : T?) {
    t |> equal(regex_test_match("(ab|a|b)\{2\}", "ab"), 2)
    t |> equal(regex_test_match("(a|ab)\{2\}", "aba"), 3)
    t |> equal(regex_test_match("(a|ab)\{2\}c", "abc"), -1,
        "the rest is tried only once the minimum is met")
}

[test(description = "a bounded quantifier never takes a pass past its maximum and a character run never backs below its minimum")]
def test_a_bounded_quantifier_takes_no_pass_past_its_maximum(t : T?) {
    t |> equal(regex_test_match("(a|ab)\{1\}c", "aabc"), -1)
    t |> equal(regex_test_match("(a|ab)?c", "aabc"), -1)
    t |> equal(regex_test_match("a\{2\}a", "aa"), -1,
        "a character run never backs below its minimum")
    t |> equal(regex_test_match("a\{2,3\}a", "aa"), -1)
}

[test(description = "an empty pass ends the longest-first run and is retried as a choice")]
def test_an_empty_pass_ends_the_longest_first_run_and_is_retried_as_a_choice(t : T?) {
    t |> equal(regex_test_match("x(a*)*", "xaa"), 3)
    t |> equal(regex_test_match("x(a*)*ac", "xaac"), 4)
    t |> equal(regex_test_match("(b?|ab)?c", "abc"), 3)
    t |> equal(regex_test_match("(b?|ab)\{0,3\}c", "ababc"), 5)
}

[test(description = "nested quantifiers over a failing tail terminate with no match")]
def test_nested_quantifiers_over_a_failing_tail_finish(t : T?) {
    let input = repeat("ab", 100) + "c"
    t |> equal(regex_test_match("((ab)+)+d", input), -1)
    t |> equal(regex_test_match("((ab)*)*d", input), -1)
    t |> equal(regex_test_match("(a|ab)*(ab|a)*d", input), -1)
}

[test(description = "strings generated from a quantified pattern are fully matched by it")]
def test_a_generated_string_matches_its_quantified_pattern(t : T?) {
    var rnd <- each_random_uint(7)
    for (pat in ["(ab|c)+d", "x(a|bc)*y", "(ab)\{2,4\}", "x(a)y", "(a)(b)c"]) {
        var re <- regex_compile(pat)
        for (_ in range(20)) {
            let s = re_gen(re, rnd)
            t |> equal(length(s), regex_match(re, s), "{pat} generated [{s}]")
        }
    }
}
