options gen2
options persistent_heap

require daslib/jobque_boost
require daslib/fio
require math

// Fork/join dispatch TIMELINE — the instrument for jobque dispatch work. Hand-rolls the exact
// _parallel_for shape (with_wait_group + new_job per chunk) so it can timestamp what parallel_for
// hides, per mode, all in ONE process (interleaved A/B — end-to-end LLM numbers fold in
// DRAM/thermal/model noise and cannot attribute a dispatch change):
//   disp      - dispatch loop duration on the calling thread (serial per-job cost)
//   lastStart - when the LAST worker began its job (the parallelism ramp)
//   rt        - full round trip (dispatch + drain + jobs + join)
// Modes: base / spin (set_jobque_worker_spin) / batch (set_jobque_batch_dispatch) / spin+batch,
// each over empty jobs (pure dispatch tax) and 30us busy jobs (the decode-matmul shape).
// NOTE: spin alone and batch alone measure WORSE than base at high worker counts (spinners storm
// the fifo lock the per-push dispatch needs; batch's notify_all wakes parked workers serially) —
// they are designed to compose: batch keeps the fifo quiet while the dispatcher builds jobs, spin
// keeps workers awake so the flush pays no wakes.
//
//   DAS_JOBQUE_THREADS=<n> bin/daslang -jit benchmarks/core/jobque/dispatch_timeline.das

let WARMUP = 4
let ROUNDS = 12
let SPIN_US = 1000
let BUSY_NS = 30000l

def one_round(njobs : int; spin_ns : int64; var start_ns : array<int64>; var end_ns : array<int64>) : tuple<disp : int64; last_start : int64; rt : int64> {
    for (i in range(njobs)) {
        start_ns[i] = 0l
        end_ns[i] = 0l
    }
    var ps = unsafe(addr(start_ns[0]))
    var pe = unsafe(addr(end_ns[0]))
    var disp_done = 0l
    let t0 = ref_time_ticks()
    with_wait_group(njobs) $(wg) {
        for (i in range(njobs)) {
            let ii = i
            new_job(@ {
                let ts = ref_time_ticks()
                unsafe(ps[ii]) = ts
                if (spin_ns > 0l) {
                    while (ref_time_ticks() - ts < spin_ns) {}
                }
                unsafe(pe[ii]) = ref_time_ticks()
                wg |> notify_and_release
            })
        }
        disp_done = ref_time_ticks()
    }
    let joined = ref_time_ticks()
    var last_start = 0l
    for (i in range(njobs)) {
        last_start = max(last_start, start_ns[i])
    }
    return (disp = disp_done - t0, last_start = last_start - t0, rt = joined - t0)
}

def median(var xs : array<int64>) : double {
    sort(xs)
    return double(xs[length(xs) / 2]) / 1000.0lf
}

def run_mode(mode_name : string; njobs : int; spin_ns : int64; var start_ns : array<int64>; var end_ns : array<int64>) {
    var disps : array<int64>
    var lasts : array<int64>
    var rts : array<int64>
    disps |> reserve(ROUNDS)
    lasts |> reserve(ROUNDS)
    rts |> reserve(ROUNDS)
    for (_w in range(WARMUP)) {
        one_round(njobs, spin_ns, start_ns, end_ns)
    }
    for (_r in range(ROUNDS)) {
        let m = one_round(njobs, spin_ns, start_ns, end_ns)
        disps |> push(m.disp)
        lasts |> push(m.last_start)
        rts |> push(m.rt)
    }
    let d = median(disps)
    let ls = median(lasts)
    let r = median(rts)
    let per_job = d / double(njobs)
    let shape = spin_ns > 0l ? "busy{int(spin_ns / 1000l)}us" : "empty"
    print("{mode_name}/{shape}  N={njobs}  disp={d}us ({per_job}us/job)  lastStart=+{ls}us  rt={r}us  (medians of {ROUNDS})\n")
    delete disps
    delete lasts
    delete rts
}

def run_shape(njobs : int; spin_ns : int64; var start_ns : array<int64>; var end_ns : array<int64>) {
    set_jobque_worker_spin(0)
    set_jobque_batch_dispatch(false)
    run_mode("base      ", njobs, spin_ns, start_ns, end_ns)
    set_jobque_worker_spin(SPIN_US)
    run_mode("spin      ", njobs, spin_ns, start_ns, end_ns)
    set_jobque_worker_spin(0)
    set_jobque_batch_dispatch(true)
    sleep(5u)   // let residual spinners park so the mode starts clean
    run_mode("batch     ", njobs, spin_ns, start_ns, end_ns)
    set_jobque_worker_spin(SPIN_US)
    run_mode("spin+batch", njobs, spin_ns, start_ns, end_ns)
    set_jobque_worker_spin(0)
    set_jobque_batch_dispatch(false)
    sleep(5u)
}

[export]
def main {
    with_job_que() {
        set_jobque_fork_pool(true, true)    // the pure fork/join compute configuration (dasLLAMA-style)
        let njobs = get_total_hw_jobs()
        print("workers: {njobs}\n")
        var start_ns : array<int64>
        var end_ns : array<int64>
        start_ns |> resize(njobs)
        end_ns |> resize(njobs)
        run_shape(njobs, 0l, start_ns, end_ns)
        run_shape(njobs, BUSY_NS, start_ns, end_ns)
        delete start_ns
        delete end_ns
    }
}
