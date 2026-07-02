options gen2
options persistent_heap

require dastest/testing_boost
require daslib/jobque_boost
require math

// Fork/join dispatch tax in isolation — the empty parallel_for round trip — across the jobque
// dispatch knobs (worker spin-before-park, batched dispatch). This is the instrument for jobque
// dispatch work: end-to-end LLM decode folds in DRAM/thermal/model noise and cannot attribute a
// dispatch change. ns/op = ONE job's share of a round trip (chunk_size = job count).
// Pin the worker count with DAS_JOBQUE_THREADS when recording numbers.
//
//   bin/daslang -jit dastest/dastest.das -- --bench --test benchmarks/core/jobque/dispatch.das

let SPIN_US = 200

def fork_join_round(nj : int) {
    parallel_for(0, nj, nj) $(_rb, _re, wg) {
        wg |> notify_and_release
    }
}

def bench_modes(b : B?; nj : int) {
    set_jobque_worker_spin(0)
    set_jobque_batch_dispatch(false)
    b |> run("fork_join/{nj}/base", nj) {
        fork_join_round(nj)
    }
    set_jobque_worker_spin(SPIN_US)
    b |> run("fork_join/{nj}/spin", nj) {
        fork_join_round(nj)
    }
    set_jobque_worker_spin(0)
    set_jobque_batch_dispatch(true)
    b |> run("fork_join/{nj}/batch", nj) {
        fork_join_round(nj)
    }
    set_jobque_worker_spin(SPIN_US)
    b |> run("fork_join/{nj}/spin_batch", nj) {
        fork_join_round(nj)
    }
    set_jobque_worker_spin(0)
    set_jobque_batch_dispatch(false)
}

[benchmark]
def jobque_dispatch(b : B?) {
    with_job_que() {
        set_jobque_fork_pool(true, true)    // the pure fork/join compute configuration (dasLLAMA-style)
        bench_modes(b, min(8, get_total_hw_jobs()))
        if (get_total_hw_jobs() > 8) {
            bench_modes(b, get_total_hw_jobs())
        }
    }
}
