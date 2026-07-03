options gen2
options persistent_heap

// Attribute the ~4.2us/worker empty parallel_for round-trip tax (dasLLAMA decode leg).
// Hand-rolls the exact _parallel_for shape (with_wait_group + new_job per chunk) so we can
// timestamp phases the real parallel_for hides:
//   push_ns[i]  - dispatch thread, right after new_job(i) returns (serial dispatch cost)
//   start_ns[i] - worker, first statement of job i (wake latency = start - push)
//   end_ns[i]   - worker, right before notify (job body is empty, start==end)
//   disp/joined - dispatch-loop-done / wait-group-join-returned
// Run: DAS_JOBQUE_THREADS=32 bin/Release/daslang.exe -jit dispatch_probe.das
// Knobs: DAS_FORK_SKIP_HEAP_RESET=1 (skip restartHeaps on pooled reuse), DAS_DETAIL=1 (per-job dump)

require daslib/jobque_boost
require daslib/fio
require math
require strings

let MAXJ = 64

def one_round(njobs : int; spin_ns : int64; var push_ns : array<int64>; var start_ns : array<int64>; var end_ns : array<int64>; detail : bool) {
    for (i in range(njobs)) {
        start_ns[i] = 0l
        end_ns[i] = 0l
        push_ns[i] = 0l
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
            push_ns[i] = ref_time_ticks()
        }
        disp_done = ref_time_ticks()
    }
    let joined = ref_time_ticks()
    // analysis
    var last_start = 0l
    var last_end = 0l
    var max_wake = 0l
    var sum_wake = 0l
    for (i in range(njobs)) {
        last_start = max(last_start, start_ns[i])
        last_end = max(last_end, end_ns[i])
        let wake = start_ns[i] - push_ns[i]
        max_wake = max(max_wake, wake)
        sum_wake += wake
    }
    let us = 1000.0lf
    let rt = double(joined - t0) / us
    let disp = double(disp_done - t0) / us
    let per_push = disp / double(njobs)
    let lstart = double(last_start - t0) / us
    let jtail = double(joined - last_end) / us
    let mwake = double(max_wake) / us
    let awake = double(sum_wake) / double(njobs) / us
    print("N={njobs}  rt={rt}us  disp={disp}us ({per_push}us/job)  lastStart=+{lstart}us  joinTail={jtail}us  wake avg={awake}us max={mwake}us\n")
    if (detail) {
        for (i in range(njobs)) {
            let pu = double(push_ns[i] - t0) / us
            let su = double(start_ns[i] - t0) / us
            let eu = double(end_ns[i] - t0) / us
            let wk = double(start_ns[i] - push_ns[i]) / us
            print("  job {i}: push=+{pu}  start=+{su}  wake={wk}  end=+{eu}\n")
        }
    }
}

// Two-wave attribution: wave A parks→wakes all workers into 500us spins; wave B dispatches while
// every worker is awake and busy (no parked waiter, no fifo traffic). Wave B per-push isolates the
// intrinsic push cost (lock + insert + notify-with-no-waiter + fork acquire from warm pool).
def two_wave_round(njobs : int; var push_ns : array<int64>; var start_ns : array<int64>; var end_ns : array<int64>) {
    let spin = 500000l  // 500us — outlasts wave-B dispatch
    for (i in range(2 * njobs)) {
        start_ns[i] = 0l
        end_ns[i] = 0l
        push_ns[i] = 0l
    }
    var ps = unsafe(addr(start_ns[0]))
    var pe = unsafe(addr(end_ns[0]))
    let t0 = ref_time_ticks()
    var dispA = 0l
    var dispB = 0l
    var tB0 = 0l
    with_wait_group(2 * njobs) $(wg) {
        for (i in range(njobs)) {
            let ii = i
            new_job(@ {
                let ts = ref_time_ticks()
                unsafe(ps[ii]) = ts
                while (ref_time_ticks() - ts < spin) {}
                unsafe(pe[ii]) = ref_time_ticks()
                wg |> notify_and_release
            })
            push_ns[i] = ref_time_ticks()
        }
        dispA = ref_time_ticks()
        tB0 = dispA
        for (i in range(njobs)) {
            let ii = njobs + i
            new_job(@ {
                let ts = ref_time_ticks()
                unsafe(ps[ii]) = ts
                while (ref_time_ticks() - ts < spin) {}
                unsafe(pe[ii]) = ref_time_ticks()
                wg |> notify_and_release
            })
            push_ns[njobs + i] = ref_time_ticks()
        }
        dispB = ref_time_ticks()
    }
    let joined = ref_time_ticks()
    let us = 1000.0lf
    let dA = double(dispA - t0) / us
    let dB = double(dispB - tB0) / us
    let ppA = dA / double(njobs)
    let ppB = dB / double(njobs)
    let rt = double(joined - t0) / us
    print("N={njobs}x2  waveA disp={dA}us ({ppA}us/job)  waveB disp={dB}us ({ppB}us/job)  rt={rt}us\n")
}

[export]
def main {
    let detail_all = has_env_variable("DAS_DETAIL")
    var spin_ns = 0l
    if (has_env_variable("DAS_SPIN_NS")) {
        spin_ns = int64(to_int(get_env_variable("DAS_SPIN_NS")))
        print("spin per job: {spin_ns}ns\n")
    }
    with_job_que() {
        set_jobque_fork_pool(true, true)
        if (has_env_variable("DAS_FORK_SKIP_HEAP_RESET")) {
            set_jobque_fork_skip_heap_reset(true)
            print("fork_skip_heap_reset ON\n")
        }
        if (has_env_variable("DAS_WORKER_SPIN_US")) {
            let spin_us = to_int(get_env_variable("DAS_WORKER_SPIN_US"))
            set_jobque_worker_spin(spin_us)
            print("worker spin: {spin_us}us\n")
        }
        if (has_env_variable("DAS_BATCH_DISPATCH")) {
            set_jobque_batch_dispatch(true)
            print("batch dispatch ON\n")
        }
        print("workers: {get_total_hw_jobs()}\n")
        var push_ns : array<int64>
        var start_ns : array<int64>
        var end_ns : array<int64>
        push_ns |> resize(MAXJ)
        start_ns |> resize(MAXJ)
        end_ns |> resize(MAXJ)
        if (has_env_variable("DAS_TWO_WAVE")) {
            let nj = get_total_hw_jobs()
            print("--- two-wave {nj}+{nj} (500us spin jobs) ---\n")
            for (_w in range(3)) {
                two_wave_round(nj, push_ns, start_ns, end_ns)
            }
            print("(measured)\n")
            for (_r in range(8)) {
                two_wave_round(nj, push_ns, start_ns, end_ns)
            }
            delete push_ns
            delete start_ns
            delete end_ns
            return
        }
        for (nj in [1, 2, 4, 8, 16, 24, 32, 48]) {
            print("--- {nj} jobs ---\n")
            for (_w in range(3)) {   // warmup: populate the fork pool, page in
                one_round(nj, spin_ns, push_ns, start_ns, end_ns, false)
            }
            print("(measured)\n")
            for (r in range(8)) {
                let last = r == 7
                one_round(nj, spin_ns, push_ns, start_ns, end_ns, detail_all || (last && nj == 32))
            }
        }
        delete push_ns
        delete start_ns
        delete end_ns
    }
}
