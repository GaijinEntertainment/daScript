options gen2
require dastest/testing_boost
require daslib/rtti

var g_stats = BenchmarkRunStats()
var g_sink = 0

def make_bench(name : string) : B? {
    var b = new B(name = name)
    b.on_start <- @(sub_name : string) {}
    b.on_finished <- @(stats : BenchmarkRunStats) {
        g_stats = stats
    }
    b.on_fail <- @(now : bool) {}
    b.on_log <- @(msg : string; at : LineInfo) {}
    return b
}

[test]
def test_reported_batch_meets_the_budget(t : T?) {
    t |> run("a fast op grows to a batch of at least the budget") @@(t : T?) {
        var b = make_bench("budget")
        b |> run("fast") {
            g_sink++
        }
        t |> success(g_stats.n > 1, "grew past the calibration call: n = {g_stats.n}")
        t |> success(g_stats.time_ns >= 900_000_000l, "reported batch spans {g_stats.time_ns} ns")
        unsafe {
            delete b
        }
    }
    t |> run("a slow op is reported from the batch that met the budget") @@(t : T?) {
        var b = make_bench("budget")
        b |> run("slow") {
            var acc = 0
            for (i in range(20_000_000)) {
                acc += i & 7
            }
            g_sink += acc
        }
        t |> success(g_stats.time_ns >= 900_000_000l, "reported batch spans {g_stats.time_ns} ns over n = {g_stats.n}")
        unsafe {
            delete b
        }
    }
    feint("{g_sink}")
}
