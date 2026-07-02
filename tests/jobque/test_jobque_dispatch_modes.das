options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false
require dastest/testing_boost public
require daslib/jobque_boost

// Correctness of the jobque dispatch knobs — set_jobque_worker_spin (spin-before-park) and
// set_jobque_batch_dispatch (publish all new_jobs under one lock at join) — in every combination:
// parallel_for produces the same results, and back-to-back storms neither hang nor drop jobs.

def sum_round(t : T?; nj, total : int) {
    var results : array<int>
    results |> resize(total)
    var pr = unsafe(addr(results[0]))
    parallel_for(0, total, nj) $(job_begin, job_end, wg) {
        for (i in range(job_begin, job_end)) {
            unsafe(pr[i]) = i * 3 + 1
        }
        wg |> notify_and_release
    }
    var sum = 0
    for (i in range(total)) {
        sum += results[i]
    }
    t |> equal(3 * total * (total - 1) / 2 + total, sum)
    delete results
}

def storm(t : T?; rounds : int) {
    // back-to-back fork/join rounds at several shapes: 1 job, fewer jobs than items,
    // more jobs than workers — exercises spin re-entry and the batch flush repeatedly
    for (_r in range(rounds)) {
        sum_round(t, 1, 4)
        sum_round(t, 4, 16)
        sum_round(t, 2 * get_total_hw_jobs(), 4 * get_total_hw_jobs())
    }
}

[test]
def test_dispatch_modes(t : T?) {
    with_job_que() {
        set_jobque_fork_pool(true, true)
        t |> run("baseline") @(t : T?) {
            storm(t, 10)
        }
        t |> run("spin") @(t : T?) {
            set_jobque_worker_spin(200)
            storm(t, 10)
            set_jobque_worker_spin(0)
        }
        t |> run("batch") @(t : T?) {
            set_jobque_batch_dispatch(true)
            storm(t, 10)
            set_jobque_batch_dispatch(false)
        }
        t |> run("spin+batch") @(t : T?) {
            set_jobque_worker_spin(200)
            set_jobque_batch_dispatch(true)
            storm(t, 10)
            set_jobque_worker_spin(0)
            set_jobque_batch_dispatch(false)
        }
        t |> run("baseline after reset") @(t : T?) {
            storm(t, 5)
        }
    }
}
