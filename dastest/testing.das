options gen2
options indenting = 4

module testing shared

require rtti
require strings // to compare strings/das_strings
require daslib/math_bits
require math
require daslib/unroll

class T {
    name : string
    startTick : int64
    verbose : bool
    onLog : lambda<(msg : string; at : LineInfo) : void>
    onFail : lambda<(now : bool) : void>
    onSkipNow : lambda<() : void>
    onRun : lambda<(name : string; fn : RunT) : void>

    failed : bool = false
    skipped : bool = false

    def T(name_ : string; start_tick : int64; verbose_ : bool) {
        name = name_
        startTick = start_tick
        verbose = verbose_
    }

    def const failNow() {
        onFail |> invoke(true)
        panic("test failed")
    }

    def const fail() {
        onFail |> invoke(false)
    }

    def const error(msg : string) {
        errorAt(msg, get_line_info(1))
    }

    def const errorAt(msg : string; at : LineInfo) {
        logAt(msg, at)
        fail()
    }

    def const fatal(msg : string) {
        fatalAt(msg, get_line_info(1))
    }

    def const fatalAt(msg : string; at : LineInfo) {
        logAt(msg, at)
        failNow()
    }

    def const log(msg : string) {
        logAt(msg, get_line_info(1))
    }

    def const logAt(msg : string; at : LineInfo) {
        onLog |> invoke(msg, at)
    }

    def const skipNow() {
        onSkipNow |> invoke()
        panic("test skipped")
    }

    def const skip(msg : string) {
        skipAt(msg, get_line_info(1))
    }

    def const skipAt(msg : string; at : LineInfo) {
        logAt(msg, at)
        skipNow()
    }

    def const getTimeUsec() : int {
        return get_time_usec(startTick)
    }

    def const getTimeSec() : double {
        return double(getTimeUsec()) / 1000000.0lf
    }
}

class B {
    name : string
    current_sub_name : string
    started : bool = false
    finished : bool = false

    on_fail : lambda<(now : bool) : void>
    on_start : lambda<(sub_name : string) : void>
    on_finished : lambda<(stats : BenchmarkRunStats) : void>

    failed : bool = false

    def const failNow() {
        on_fail |> invoke(true)
        panic("benchmark failed")
    }

    def const fail() {
        on_fail |> invoke(false)
    }
}

// BenchmarkRunStats is a structured result of a single benchmark executed.
// A value of this type is serialized when json output format is used.
struct BenchmarkRunStats {
    name : string
    sub_name : string
    n : int
    time_ns : int64
    allocs : uint64
    heap_bytes : uint64
}


def equal(t : T?; a; b; msg = "") : bool {
    if (a == b) {
        return true
    }
    t->errorAt(!empty(msg) ? msg : "values differ", get_line_info(1))
    t->errorAt("\texpected: {a}", get_line_info(1))
    t->errorAt("\tgot: {b}", get_line_info(1))
    return false
}

[sideeffects]
def accept(t : T?; value) {
    pass
}

def strictEqual(t : T?; a; b; msg = "") : bool {
    if (a == b) {
        return true
    }
    t->errorAt(!empty(msg) ? msg : "values differ", get_line_info(1))
    t->errorAt("\texpected: {a}", get_line_info(1))
    t->fatalAt("\tgot: {b}", get_line_info(1))
    return false
}

def strictEqual(t : T?; a, b : float; msg = "") : bool {
    if (float_bits_to_int(a) == float_bits_to_int(b)) {
        return true
    }
    t->errorAt(!empty(msg) ? msg : "values differ", get_line_info(1))
    t->errorAt("\texpected: {a}", get_line_info(1))
    t->fatalAt("\tgot: {b}", get_line_info(1))
    return false
}

def strictEqual(t : T?; a, b : float2; msg = "") : bool {
    if (float_bits_to_int(a) == float_bits_to_int(b)) {
        return true
    }
    t->errorAt(!empty(msg) ? msg : "values differ", get_line_info(1))
    t->errorAt("\texpected: {a}", get_line_info(1))
    t->fatalAt("\tgot: {b}", get_line_info(1))
    return false
}

def strictEqual(t : T?; a, b : float3; msg = "") : bool {
    if (float_bits_to_int(a) == float_bits_to_int(b)) {
        return true
    }
    t->errorAt(!empty(msg) ? msg : "values differ", get_line_info(1))
    t->errorAt("\texpected: {a}", get_line_info(1))
    t->fatalAt("\tgot: {b}", get_line_info(1))
    return false
}

def strictEqual(t : T?; a, b : float4; msg = "") : bool {
    if (float_bits_to_int(a) == float_bits_to_int(b)) {
        return true
    }
    t->errorAt(!empty(msg) ? msg : "values differ", get_line_info(1))
    t->errorAt("\texpected: {a}", get_line_info(1))
    t->fatalAt("\tgot: {b}", get_line_info(1))
    return false
}


def strictEqual(t : T?; a, b : double; msg = "") : bool {
    if (double_bits_to_int64(a) == double_bits_to_int64(b)) {
        return true
    }
    t->errorAt(!empty(msg) ? msg : "values differ", get_line_info(1))
    t->errorAt("\texpected: {a}", get_line_info(1))
    t->fatalAt("\tgot: {b}", get_line_info(1))
    return false
}

def numericEqual(t : T?; a; b; msg = "") : bool {
    if (a == b) {
        return true
    }
    t->errorAt(!empty(msg) ? msg : "numeric values differ", get_line_info(1))
    t->errorAt("\texpected: {a}", get_line_info(1))
    t->fatalAt("\tgot: {b}", get_line_info(1))
    return false
}

def numericEqual(t : T?; a, b : float; msg = "") : bool {
    if (is_nan(a) && is_nan(b)) {
        return true
    }
    if (a == b) {
        return true
    }
    t->errorAt(!empty(msg) ? msg : "numeric values differ", get_line_info(1))
    t->errorAt("\texpected: {a}", get_line_info(1))
    t->fatalAt("\tgot: {b}", get_line_info(1))
    return false
}

def numericEqual(t : T?; _a, _b : float2; msg = "") : bool {
    var a = _a
    var b = _b
    if (is_nan(a.x) && is_nan(b.x)) {
        a.x = 0.
        b.x = 0.
    }
    if (is_nan(a.y) && is_nan(b.y)) {
        a.y = 0.
        b.y = 0.
    }
    if (a == b) {
        return true
    }
    t->errorAt(!empty(msg) ? msg : "numeric values differ", get_line_info(1))
    t->errorAt("\texpected: {_a}", get_line_info(1))
    t->fatalAt("\tgot: {_b}", get_line_info(1))
    return false
}

def numericEqual(t : T?; _a, _b : float3; msg = "") : bool {
    var a = _a
    var b = _b
    if (is_nan(a.x) && is_nan(b.x)) {
        a.x = 0.
        b.x = 0.
    }
    if (is_nan(a.y) && is_nan(b.y)) {
        a.y = 0.
        b.y = 0.
    }
    if (is_nan(a.z) && is_nan(b.z)) {
        a.z = 0.
        b.z = 0.
    }
    if (a == b) {
        return true
    }
    t->errorAt(!empty(msg) ? msg : "numeric values differ", get_line_info(1))
    t->errorAt("\texpected: {_a}", get_line_info(1))
    t->fatalAt("\tgot: {_b}", get_line_info(1))
    return false
}

def numericEqual(t : T?; _a, _b : float4; msg = "") : bool {
    var a = _a
    var b = _b
    if (is_nan(a.x) && is_nan(b.x)) {
        a.x = 0.
        b.x = 0.
    }
    if (is_nan(a.y) && is_nan(b.y)) {
        a.y = 0.
        b.y = 0.
    }
    if (is_nan(a.z) && is_nan(b.z)) {
        a.z = 0.
        b.z = 0.
    }
    if (is_nan(a.w) && is_nan(b.w)) {
        a.w = 0.
        b.w = 0.
    }
    if (a == b) {
        return true
    }
    t->errorAt(!empty(msg) ? msg : "numeric values differ", get_line_info(1))
    t->errorAt("\texpected: {_a}", get_line_info(1))
    t->fatalAt("\tgot: {_b}", get_line_info(1))
    return false
}

def numericEqual(t : T?; a, b : double; msg = "") : bool {
    if (is_nan(a) && is_nan(b)) {
        return true
    }
    if (a == b) {
        return true
    }
    t->errorAt(!empty(msg) ? msg : "values differ", get_line_info(1))
    t->errorAt("\texpected: {a}", get_line_info(1))
    t->fatalAt("\tgot: {b}", get_line_info(1))
    return false
}

def success(t : T?; a; msg = "") : bool {
    if (a) {
        return true
    }
    t->errorAt(!empty(msg) ? msg : "expected success, got failure", get_line_info(1))
    return false
}

def failure(t : T?; msg = "") : bool {
    t->errorAt(!empty(msg) ? msg : "test failed", get_line_info(1))
    return false
}

typedef RunT = variant<lmd1 : lambda<(t : T?) : void>; func1 : function<(t : T?) : void>>


def run(t : T?; name : string; var cb : lambda<(t : T?) : void>) {
    t.onRun |> invoke(name, RunT(lmd1 <- cb))
}


def run(t : T?; name : string; func : function<(t : T?) : void>) {
    t.onRun |> invoke(name, RunT(func1 = func))
}


// run executes a benchmark of a given name and runs the block in the loop,
// while collecting data like CPU time and heap allocations.
def run(b : B?, name : string; op : block) {
    b.on_start(name)

    let min_runs = 1
    let max_runs = 1_000_000_000

    // The first run serves two purposes: it will
    // find the appropriate N (num_runs) for the op,
    // plus it allows a warmup before we start measuring it.
    var time_ref = ref_time_ticks()
    let run1_start = get_time_nsec(time_ref)
    op()
    let run1_end = get_time_nsec(time_ref)

    let op_time_approx = min(double((run1_end - run1_start)) / 1000000000.0lf, 1.0lf)

    let n_probe = int64(1.0lf / op_time_approx)
    var num_runs = int(clamp(n_probe, int64(min_runs), int64(max_runs)))

    // We will unroll fast-running (small) benchmarks
    // to decrease the loop's own time contribution.
    // A value of 5 gives good results, 10 should be very safe.
    let unroll_count = 10
    let unroll_threshold = 100 * unroll_count

    // Pre-compute all the stuff before the timer has started.
    let do_unroll = num_runs > unroll_threshold
    var loop_runs = num_runs
    if (do_unroll) {
        num_runs = int(round(float(num_runs) / float(unroll_count)) * float(unroll_count))
        loop_runs = num_runs / unroll_count
    }

    let heap_start = heap_allocation_stats()
    let heap_bytes_start = heap_start[0] + heap_start[1]
    let num_allocs_start = heap_allocation_count()
    var time_total = 0l
    if (do_unroll) {
        // A fast/small benchmarking func - run the timers
        // once, unroll the body to minimize the noise.
        let start = get_time_nsec(time_ref)
        for (i in range(loop_runs)) {
            unroll <| $() {
                for (_ in range(unroll_count)) {
                    op()
                }
            }
        }
        let end = get_time_nsec(time_ref)
        time_total = end - start
    } else {
        // A long-running benchmark that should not be unrolled.
        // Its run time will not be dominated by the loop anyway.
        for (i in range(loop_runs)) {
            let start = get_time_nsec(time_ref)
            op()
            let end = get_time_nsec(time_ref)
            time_total += end - start
        }
    }

    let heap_end = heap_allocation_stats()
    let heap_bytes_end = heap_end[0] + heap_end[1]
    let num_allocs_end = heap_allocation_count()

    let stats = BenchmarkRunStats(
        name = b.name,
        sub_name = name,
        n = num_runs,
        time_ns = time_total,
        allocs = num_allocs_end - num_allocs_start,
        heap_bytes = heap_bytes_end - heap_bytes_start,
    )

    b.on_finished(stats)
}
