options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false
require dastest/testing_boost public
require daslib/regex

def private collect_spans(var re : Regex; str : string) {
    var spans : array<int>
    regex_foreach(re, str) $(at) {
        spans |> push(at.x)
        spans |> push(at.y)
        return true
    }
    return <- spans
}

def private same_as_scan(t : T?; pattern : string; str : string) {
    var withEarlyOut <- regex_compile(pattern)
    var fullScan <- regex_compile(pattern)
    fullScan.canEarlyOut = false
    var fast <- collect_spans(withEarlyOut, str)
    var slow <- collect_spans(fullScan, str)
    t |> equal(length(fast), length(slow), "match count for `{pattern}` on `{str}`")
    if (length(fast) == length(slow)) {
        for (a, b in fast, slow) {
            t |> equal(a, b, "span bound for `{pattern}` on `{str}`")
        }
    }
}

[test]
def test_empty_matchable_patterns_scan_every_position(t : T?) {
    t |> run("star") @(t : T?) {
        same_as_scan(t, "a*", "bab")
        same_as_scan(t, "a*", "xyz")
    }
    t |> run("optional group") @(t : T?) {
        same_as_scan(t, "(abc)?", "xabcx")
    }
    t |> run("repeat with zero minimum") @(t : T?) {
        same_as_scan(t, "x\{0,3}", "aaxxa")
    }
    t |> run("alternation with empty branch") @(t : T?) {
        same_as_scan(t, "a*|b", "cbc")
    }
}

[test]
def test_zero_length_matches_are_reported(t : T?) {
    t |> run("a* on bab yields three matches") @(t : T?) {
        var re <- regex_compile("a*")
        let spans <- collect_spans(re, "bab")
        t |> equal(length(spans), 6)
        if (length(spans) == 6) {
            t |> equal(spans[0], 0)
            t |> equal(spans[1], 0)
            t |> equal(spans[2], 1)
            t |> equal(spans[3], 2)
            t |> equal(spans[4], 2)
            t |> equal(spans[5], 2)
        }
    }
}

[test]
def test_early_out_survives_where_it_is_sound(t : T?) {
    t |> run("empty-matchable patterns clear the early-out") @(t : T?) {
        var star <- regex_compile("a*")
        t |> success(!star.canEarlyOut, "a* must not early-out")
        var opt <- regex_compile("(abc)?")
        t |> success(!opt.canEarlyOut, "(abc)? must not early-out")
    }
    t |> run("mandatory patterns keep the early-out") @(t : T?) {
        var lit <- regex_compile("abc")
        t |> success(lit.canEarlyOut, "abc must early-out")
        var plus <- regex_compile("a+")
        t |> success(plus.canEarlyOut, "a+ must early-out")
        var tail <- regex_compile("a*b")
        t |> success(tail.canEarlyOut, "a*b must early-out")
    }
}
