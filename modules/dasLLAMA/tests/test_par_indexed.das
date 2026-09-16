options gen2
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_par
require dasllama/dasllama_math
require daslib/fio
require strings

// maybe_parallel_for_indexed contract across all three arms (team / fifo / inline):
// exactly-once coverage, slot < get_dispatch_slot_bound(), and per-slot scratch — each
// invocation accumulates into its slot's cell with a plain unsynchronized +=, so a slot
// shared by two in-flight invocations would lose updates and break the checksum.
//
// Single-thread mode is gated here too. Its environment half runs in child processes -
// DASLLAMA_SINGLE_THREAD loads once at context init, so a child started with it set or unset is
// the only instrument - and those children spawn _single_thread_root.das.

def indexed_round(t : T?; nj, first, total : int) {
    let bound = get_dispatch_slot_bound()
    var hits : array<int>
    hits |> resize(total)
    var pr = unsafe(addr(hits[0]))
    var acc : array<int>
    acc |> resize(bound + 1)   // +1 guard cell — out-of-range slots land there
    var pa = unsafe(addr(acc[0]))
    maybe_parallel_for_indexed(first, first + total, nj) $(slot, rb, re) {
        let cell = (slot >= 0 && slot < bound) ? slot : bound
        for (i in range(rb, re)) {
            unsafe(pr[i - first])++
            unsafe(pa[cell]) += i * 3 + 1
        }
    }
    var bad = 0
    for (i in range(total)) {
        if (hits[i] != 1) {
            bad++
        }
    }
    t |> equal(0, bad)
    t |> equal(0, acc[bound])   // no out-of-range slot fired
    var sum = 0
    var want = 0
    for (i in range(first, first + total)) {
        want += i * 3 + 1
    }
    for (s in range(bound)) {
        sum += acc[s]
    }
    t |> equal(want, sum)
    delete hits
    delete acc
}

def single_invoke_round(t : T?; nj, total : int) {
    // inline path: ONE invoke, slot 0, whole range
    var stats : array<int>   // [invocations, non-zero-slot invocations, index checksum]
    stats |> resize(3)
    var ps = unsafe(addr(stats[0]))
    maybe_parallel_for_indexed(0, total, nj) $(slot, rb, re) {
        unsafe(ps[0])++
        if (slot != 0) {
            unsafe(ps[1])++
        }
        for (i in range(rb, re)) {
            unsafe(ps[2]) += i
        }
    }
    t |> equal(1, stats[0])
    t |> equal(0, stats[1])
    t |> equal(total * (total - 1) / 2, stats[2])
    delete stats
}

def indexed_storm(t : T?; rounds : int) {
    for (_r in range(rounds)) {
        indexed_round(t, 4, 0, 16)
        indexed_round(t, 2 * get_total_hw_jobs(), 0, 4 * get_total_hw_jobs())
        indexed_round(t, 8 * get_total_hw_jobs(), 7, 8 * get_total_hw_jobs())   // chunk-per-item, offset range
        indexed_round(t, 64, 3, 7)
        single_invoke_round(t, 1, 32)
    }
}

[test]
def test_maybe_parallel_for_bool_no_que(t : T?) {
    t |> run("no job que - a true flag runs the bool form inline, one invoke over the whole range") @(t : T?) {
        let before = get_disp_inline()
        var acc : array<int>
        acc |> resize(2)
        var pa = unsafe(addr(acc[0]))
        maybe_parallel_for(true, 3, 19, 8) $(rb, re) {
            unsafe {
                pa[0]++
                pa[1] += re - rb
            }
        }
        t |> equal(acc[0], 1, "one invoke")
        t |> equal(acc[1], 16, "the whole range in that invoke")
        t |> equal(get_disp_inline() - before, 1l, "counted as an inline run")
    }
}

//! what a dispatch round asserts whichever form drove it; the caller's hits array carries one cell
//! per index, so the parallel arm's chunks never share a cell and the count needs no synchronization
def private round_verdict(t : T?; tag : string; hits : array<int>; expect_inline : bool; inline_before, disp_before : int64) {
    var covered = 0
    for (h in hits) {
        covered += h == 1 ? 1 : 0
    }
    t |> equal(covered, length(hits), "{tag}: every index covered exactly once")
    t |> equal(get_disp_inline() - inline_before, expect_inline ? 1l : 0l, "{tag}: inline runs")
    t |> equal(get_disp_count() - disp_before, expect_inline ? 0l : 1l, "{tag}: dispatches")
}

//! one bool-form dispatch that WANTS to thread
def private bool_form_round(t : T?; tag : string; expect_inline : bool) {
    let inline_before = get_disp_inline()
    let disp_before = get_disp_count()
    var hits : array<int>
    hits |> resize(64)
    var ph = unsafe(addr(hits[0]))
    maybe_parallel_for(true, 0, 64, 8) $(rb, re) {
        for (i in range(rb, re)) {
            unsafe(ph[i])++
        }
    }
    round_verdict(t, tag, hits, expect_inline, inline_before, disp_before)
    delete hits
}

//! the num_jobs form with a literal count above one - the TTS head loops' shape, which no shaper gates
def private nj_form_round(t : T?; tag : string; expect_inline : bool) {
    let inline_before = get_disp_inline()
    let disp_before = get_disp_count()
    var hits : array<int>
    hits |> resize(48)
    var ph = unsafe(addr(hits[0]))
    maybe_parallel_for(0, 48, 6) $(rb, re) {
        for (i in range(rb, re)) {
            unsafe(ph[i])++
        }
    }
    round_verdict(t, tag, hits, expect_inline, inline_before, disp_before)
    delete hits
}

[test]
def test_single_thread(t : T?) {
    with_job_que() {
        set_jobque_fork_pool(true, true)
        t |> run("single-thread mode runs every form inline on a live team queue, and off again dispatches") @(t : T?) {
            set_jobque_team_mode(true)
            set_single_thread_(true)
            t |> success(get_single_thread_(), "the flag reads back")
            t |> equal(get_dispatch_lanes(), 1, "one lane: every shaper answers inline")
            bool_form_round(t, "team, single", true)
            nj_form_round(t, "team, single", true)
            single_invoke_round(t, 8, 32)
            set_single_thread_(false)
            // the caller lane is a knob of its own, so read it rather than assume its default
            t |> equal(get_dispatch_lanes(), get_total_hw_jobs() + (get_dispatch_caller_lane() ? 1 : 0), "the pool's lanes are back")
            bool_form_round(t, "team, off", false)
            nj_form_round(t, "team, off", false)
            set_jobque_team_mode(false)
        }
        t |> run("the fifo arms honour it too") @(t : T?) {
            set_single_thread_(true)
            bool_form_round(t, "fifo, single", true)
            nj_form_round(t, "fifo, single", true)
            single_invoke_round(t, 8, 32)
            set_single_thread_(false)
            bool_form_round(t, "fifo, off", false)
        }
        t |> run("a hybrid box's phase parking stands down under it - no worker limit is moved") @(t : T?) {
            let workers_was = get_phase_decode_workers()
            set_phase_decode_workers(2)   // the hybrid-box shape: at -1 both dispatch_phase_* calls are inert
            set_jobque_worker_limit(-1)
            let unlimited_readback = get_jobque_worker_limit()
            set_single_thread_(true)
            dispatch_phase_decode()
            t |> equal(get_jobque_worker_limit(), unlimited_readback, "the decode phase parked nobody")
            set_single_thread_(false)
            dispatch_phase_decode()
            t |> equal(get_jobque_worker_limit(), 2, "off, the same call parks the slow tier")
            set_single_thread_(true)   // parked, then the flag on: the batch phase must leave the limit where it is
            dispatch_phase_batch()
            t |> equal(get_jobque_worker_limit(), 2, "the batch phase woke nobody under the flag")
            set_single_thread_(false)
            dispatch_phase_batch()
            t |> equal(get_jobque_worker_limit(), unlimited_readback, "and off it wakes them")
            set_phase_decode_workers(workers_was)
            set_jobque_worker_limit(-1)
        }
    }
}

def private das_exe() : string {
    let args <- get_command_line_arguments()
    return empty(args) ? "" : args[0]
}

def private spawn_root(single : string; var output : string&) : int {
    let cmd = "\"{das_exe()}\" -dasroot \"{get_das_root()}\" \"{get_das_root()}/modules/dasLLAMA/tests/_single_thread_root.das\" 2>&1"
    let win = get_platform_name() == "windows"
    let armed = win ? "set \"DASLLAMA_SINGLE_THREAD={single}\"&& {cmd}" : "DASLLAMA_SINGLE_THREAD={single} {cmd}"
    return unsafe(popen_timeout(armed, 300.0, $(f) {
        if (f != null) {
            output = fread(f)
        }
    }))
}

def private count_of(haystack, needle : string) : int {
    var n = 0
    var at = find(haystack, needle)
    while (at >= 0) {
        n++
        at = find(haystack, needle, at + length(needle))
    }
    return n
}

let ANNOUNCE = "single thread - this context runs every kernel"

[test]
def test_single_thread_env(t : T?) {
    t |> run("DASLLAMA_SINGLE_THREAD=1 reaches the flag at context init, and the announce prints once") @(t : T?) {
        var out : string
        let rc = spawn_root("1", out)
        t |> equal(rc, 0, "the child ran:\n{out}")
        t |> success(find(out, "ROOT_DONE") >= 0, "the child reached its end:\n{out}")
        t |> success(find(out, "SINGLE=true") >= 0, "the facade getter reads the flag the environment set:\n{out}")
        t |> success(find(out, "disp=0") >= 0, "three rounds on a configured queue, no dispatch:\n{out}")
        t |> equal(count_of(out, ANNOUNCE), 1, "one announce for three inline rounds:\n{out}")
    }
    t |> run("unset, the same child dispatches and says nothing") @(t : T?) {
        var out : string
        let rc = spawn_root("0", out)
        t |> equal(rc, 0, "the child ran:\n{out}")
        t |> success(find(out, "SINGLE=false") >= 0, "the flag is off:\n{out}")
        t |> success(find(out, "disp=3") >= 0, "and all three rounds dispatched:\n{out}")
        t |> equal(count_of(out, ANNOUNCE), 0, "a mode nobody asked for announces nothing:\n{out}")
    }
}

[test]
def test_maybe_parallel_for_indexed(t : T?) {
    t |> run("no job que - inline as slot 0") @(t : T?) {
        single_invoke_round(t, 8, 32)
        indexed_round(t, 8, 5, 32)
    }
    with_job_que() {
        t |> run("team mode - slot is the claiming worker") @(t : T?) {
            set_jobque_fork_pool(true, true)
            set_jobque_team_mode(true)
            indexed_storm(t, 10)
            set_jobque_team_mode(false)
        }
        t |> run("fifo mode - slot-domain sub-ranges") @(t : T?) {
            indexed_storm(t, 10)
        }
        t |> run("num_jobs 1 stays inline in both modes") @(t : T?) {
            single_invoke_round(t, 1, 16)
            set_jobque_team_mode(true)
            single_invoke_round(t, 1, 16)
            set_jobque_team_mode(false)
        }
        t |> run("worker limit keeps slots in bounds") @(t : T?) {
            set_jobque_team_mode(true)
            set_jobque_worker_limit(2)
            indexed_round(t, 2 * get_total_hw_jobs(), 0, 4 * get_total_hw_jobs())
            set_jobque_worker_limit(-1)
            indexed_storm(t, 3)
            set_jobque_team_mode(false)
        }
    }
}
