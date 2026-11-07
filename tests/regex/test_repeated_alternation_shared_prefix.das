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

[test]
def test_a_repeated_alternation_retries_an_alternative_that_shares_a_prefix(t : T?) {
    t |> equal(match_start("x(aba|ab|b)*c", "xabac"), 0)
    t |> equal(match_start("(aba|ab|b)*c", "abac"), 0)
    t |> equal(match_start("x(aba|ab|b)+c", "xabac"), 0)
    t |> equal(match_start("x(aba|b)*c", "xabac"), 0, "no shared prefix")
    t |> equal(match_start("x(aba|ab|b)c", "xabac"), 0, "not repeated")
}

[test]
def test_every_quantifier_retries_the_alternatives_of_its_subexpression(t : T?) {
    t |> equal(regex_test_match("(ab|aba)?c", "abac"), 4, "greedy ?")
    t |> equal(regex_test_match("(ab|aba)??c", "abac"), 4, "lazy ?")
    t |> equal(regex_test_match("(ab|aba)*?c", "abac"), 4, "lazy *")
    t |> equal(regex_test_match("(ab|aba)+?c", "abac"), 4, "lazy +")
    t |> equal(regex_test_match("(ab|aba)\{0,3\}c", "abac"), 4, "bounded repeat")
    t |> equal(regex_test_match("(ab|aba)\{1,\}c", "abac"), 4, "open repeat")
    t |> equal(regex_test_match("(ab|aba)\{0,3\}?c", "abac"), 4, "lazy bounded repeat")
}

[test]
def test_the_leftmost_alternative_wins(t : T?) {
    t |> equal(regex_test_match("(aba|ab|b)", "aba"), 3)
    t |> equal(regex_test_match("(ab|aba|b)", "aba"), 2)
}

[test]
def test_a_repeated_group_captures_its_last_pass(t : T?) {
    t |> equal(first_group("(a|b)*c", "abc"), "b")
    t |> equal(first_group("x(ab|aba)*c", "xabac"), "aba")
    t |> equal(first_group("(\\w+)\\s", "hi yo"), "hi")
    t |> equal(first_group("((a+)(b+))c", "aabbbc"), "aabbb", "a group that ends in a group")
}

[test]
def test_a_repeated_alternation_runs_over_a_long_input(t : T?) {
    let input = repeat("ab", 20000) + "c"
    t |> equal(regex_test_match("(a|b)*c", input), 40001)
    t |> equal(regex_test_match("(a|b)*d", input), -1)
}

[test]
def test_a_repeated_subexpression_that_matches_empty_ends_the_repetition(t : T?) {
    t |> equal(regex_test_match("(a*)*b", "aac"), -1)
    t |> equal(regex_test_match("(a*)*b", "aab"), 3)
    t |> equal(regex_test_match("(b?|ab)*c", "ababc"), 5,
        "an alternative that matches empty does not hide a longer one")
}

[test]
def test_a_nested_quantifier_is_a_choice_the_outer_one_retries(t : T?) {
    t |> equal(regex_test_match("(ab?)+bc", "abc"), 3)
    t |> equal(regex_test_match("(ab*)+bbc", "abbc"), 4)
}

[test]
def test_a_zero_width_subexpression_under_plus_matches_once(t : T?) {
    t |> equal(regex_test_match("a\\b+", "a"), 1, "at the end of the input")
    t |> equal(regex_test_match("a\\b+ c", "a c"), 3,
        "a pass that matches empty ends the repetition")
    t |> equal(regex_test_match("(\\b)+a", "a"), 1)
}

[test]
def test_a_greedy_quantifier_not_last_in_its_pass_takes_the_shortest_run_the_rest_accepts(t : T?) {
    t |> equal(regex_test_match("a*a", "aaa"), 1)
    t |> equal(regex_test_match("x(ab)*ab", "xababab"), 3)
    t |> equal(regex_test_match("a+a", "aaa"), 2)
}

[test]
def test_a_lazy_quantifier_tries_the_rest_before_another_pass(t : T?) {
    t |> equal(regex_test_match("(ab)+?", "abab"), 2)
    t |> equal(regex_test_match("(ab)??", "ab"), 0)
    t |> equal(regex_test_match("(ab)\{1,3\}?", "ababab"), 2)
    t |> equal(regex_test_match("(a|b)\{0,2\}?c", "aaac"), -1, "a lazy bound still stops")
    t |> equal(regex_test_match("a\{2,3\}?b", "aaaab"), -1, "a lazy character run still stops")
}

[test]
def test_a_pass_cannot_start_at_the_end_of_the_input(t : T?) {
    t |> equal(regex_test_match("x(a?)\{2\}", "x"), -1)
    t |> equal(regex_test_match("(a|$)\{2\}", "a"), -1)
    t |> equal(regex_test_match("a\\b\{1\}", "a"), 1, "a zero-width body may run there")
}

[test]
def test_a_mandatory_pass_that_fails_retries_the_alternatives(t : T?) {
    t |> equal(regex_test_match("(ab|a|b)\{2\}", "ab"), 2)
    t |> equal(regex_test_match("(a|ab)\{2\}", "aba"), 3)
    t |> equal(regex_test_match("(a|ab)\{2\}c", "abc"), -1,
        "the rest is tried only once the minimum is met")
}

[test]
def test_a_bounded_quantifier_takes_no_pass_past_its_maximum(t : T?) {
    t |> equal(regex_test_match("(a|ab)\{1\}c", "aabc"), -1)
    t |> equal(regex_test_match("(a|ab)?c", "aabc"), -1)
    t |> equal(regex_test_match("a\{2\}a", "aa"), -1,
        "a character run never backs below its minimum")
    t |> equal(regex_test_match("a\{2,3\}a", "aa"), -1)
}

[test]
def test_an_empty_pass_ends_the_longest_first_run_and_is_retried_as_a_choice(t : T?) {
    t |> equal(regex_test_match("x(a*)*", "xaa"), 3)
    t |> equal(regex_test_match("x(a*)*ac", "xaac"), 4)
    t |> equal(regex_test_match("(b?|ab)?c", "abc"), 3)
    t |> equal(regex_test_match("(b?|ab)\{0,3\}c", "ababc"), 5)
}

[test]
def test_nested_quantifiers_over_a_failing_tail_finish(t : T?) {
    let input = repeat("ab", 100) + "c"
    t |> equal(regex_test_match("((ab)+)+d", input), -1)
    t |> equal(regex_test_match("((ab)*)*d", input), -1)
    t |> equal(regex_test_match("(a|ab)*(ab|a)*d", input), -1)
}

[test]
def test_a_generated_string_matches_its_quantified_pattern(t : T?) {
    var rnd <- each_random_uint(7)
    for (pat in ["(ab|c)+d", "x(a|bc)*y", "(ab)\{2,4\}"]) {
        var re <- regex_compile(pat)
        for (_ in range(20)) {
            let s = re_gen(re, rnd)
            t |> equal(length(s), regex_match(re, s), "{pat} generated [{s}]")
        }
    }
}
