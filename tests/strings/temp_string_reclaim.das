options gen2
options persistent_heap
require dastest/testing_boost
require strings

// temp-string reclaim: [temp_string_result] calls consumed directly by a non-capturing call
// are wrapped in _temp_string_result and ride the 1-slot dispose queue (persistent heap only).
// Also covers the single-site-per-call soundness rule for the older builder temp marking.

def make_str(x : int) : string {
    return string(x) // return position - never wrapped
}

[test]
def test_temp_string_reclaim_flatness(t : T?) {
    t |> run("conversion consumed in a call is reclaimed") @@(t) {
        let before = string_heap_bytes_allocated()
        var summ = 0
        for (i in range(100000)) {
            summ += length(string(i))
        }
        let after = string_heap_bytes_allocated()
        t |> success(summ > 0)
        // only the final parked temp may remain
        t |> success(after - before < 256ul, "string heap should stay flat")
    }

    t |> run("flagged-call chain is reclaimed link by link") @@(t) {
        let before = string_heap_bytes_allocated()
        var summ = 0
        for (i in range(50000)) {
            summ += length(to_upper(strip("  padded{i}  ")))
        }
        let after = string_heap_bytes_allocated()
        t |> success(summ > 0)
        t |> success(after - before < 256ul, "chain temps should all be reclaimed")
    }
}

[test]
def test_temp_string_reclaim_correctness(t : T?) {
    t |> run("chain values stay correct") @@(t) {
        for (i in range(100)) {
            let expected_len = length("PADDED{i}")
            t |> equal(length(to_upper(strip("  padded{i}  "))), expected_len)
        }
    }

    t |> run("two flagged args - only the rightmost is queued") @@(t) {
        for (i in range(10000)) {
            let a = 100000 + i
            let b = 900000 + i
            // if string(a) were also queued, string(b)'s flush would free it mid-call
            t |> success(compare_ignore_case(string(a), string(b)) != 0)
        }
    }

    t |> run("nested builder cannot flush a sibling site (the compare/to_upper corruption)") @@(t) {
        for (i in range(10000)) {
            let a = 100000 + (i % 80000)
            let b = 300000 + (i % 70000)
            // regression: the old rightmost-builder rule freed "{a}" mid-call via the nested
            // builder's flush under right-to-left extern arg evaluation - equal on every iteration
            t |> success(compare_ignore_case(to_upper("{b}"), "{a}") != 0)
        }
    }

    t |> run("return-position conversion is not wrapped") @@(t) {
        let keep = make_str(424242)
        var churn = 0
        for (i in range(1000)) {
            churn += length(string(7777000 + i))
        }
        t |> success(churn > 0)
        t |> equal(keep, "424242")
    }

    t |> run("capturing consumers keep their strings") @@(t) {
        var tab : table<string; int>
        for (i in range(100)) {
            tab |> insert(string(i), i)
        }
        var arr : array<string>
        for (i in range(100)) {
            arr |> push(string(1000000 + i))
        }
        var churn = 0
        for (i in range(1000)) {
            churn += length(string(8888000 + i))
        }
        t |> success(churn > 0)
        for (i in range(100)) {
            let expected_key = string(i)
            t |> success(key_exists(tab, expected_key), "table keys must survive temp churn")
            let expected_elem = string(1000000 + i)
            t |> equal(arr[i], expected_elem)
        }
    }

    t |> run("wrapper passthrough on non-heap strings is a no-op") @@(t) {
        t |> equal(_temp_string_result("literal"), "literal")
    }
}
