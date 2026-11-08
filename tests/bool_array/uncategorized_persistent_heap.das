options gen2
options persistent_heap

require dastest/testing_boost
require daslib/bool_array
require daslib/rtti

// Repro: a benchmark arm once ran in the wrong context. The benchmark
// keeps the context-name probe for `--bench`; the test pins that a
// BoolArray built and iterated in a test's own context counts its set
// bits.

[benchmark]
def benchmark_context_check(b : B?) {
    to_log(LOG_INFO, "benchmark_context_check: context name = '{this_context().name}'\n")
    var a : BoolArray
    for (i in 0 .. 10) {
        a.push(i % 2 == 0)
    }
    b |> run("for_each", 10) {
        var count = 0
        for (e in a) {
            if (e) {
                count ++
            }
        }
        feint("{count}\n")
    }
}

[test(description = "iterating a BoolArray in the test's own context visits all ten pushed bits and counts the five set ones")]
def test_iterate_bool_array_in_own_context(t : T?) {
    var a : BoolArray
    for (i in 0 .. 10) {
        a.push(i % 2 == 0)
    }
    var visited = 0
    var set_bits = 0
    for (e in a) {
        visited ++
        if (e) {
            set_bits ++
        }
    }
    t |> equal(visited, 10)
    t |> equal(set_bits, 5)
}
