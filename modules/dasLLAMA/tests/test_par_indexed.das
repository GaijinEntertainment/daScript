options gen2
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_par
require dasllama/dasllama_math

// maybe_parallel_for_indexed contract across all three arms (team / fifo / inline):
// exactly-once coverage, slot < get_dispatch_slot_bound(), and per-slot scratch — each
// invocation accumulates into its slot's cell with a plain unsynchronized +=, so a slot
// shared by two in-flight invocations would lose updates and break the checksum.

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
