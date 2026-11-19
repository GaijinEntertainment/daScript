options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false
require dastest/testing_boost public
require daslib/jobque_boost

struct IntVal {
    v : int
}

// ---- Single-item edge case ----

[test(description = "A single pushed item is visited by for_each_clone")]
def test_channel_single_item(t : T?) {
    t |> run("single push then for_each_clone") @(t : T?) {
        with_channel() $(ch) {
            ch |> push_clone(IntVal(v = 777))
            var results : array<int>
            ch |> for_each_clone() $(val : IntVal#) {
                results |> push(val.v)
            }
            t |> equal(1, length(results))
            t |> equal(777, results[0])
        }
    }
}

// ---- Large batch ----

[test(description = "100 pushed items all pop back")]
def test_channel_large_batch(t : T?) {
    t |> run("push and pop 100 items") @(t : T?) {
        let N = 100
        with_channel() $(ch) {
            for (i in range(N)) {
                ch |> push_clone(IntVal(v = i))
            }
            t |> equal(N, ch.total)
            var results : array<int>
            ch |> for_each_clone() $(val : IntVal#) {
                results |> push(val.v)
            }
            t |> equal(N, length(results))
            for (i in range(N)) {
                t |> equal(i, results[i])
            }
        }
    }
}

// ---- Empty channel pop ----

[test(description = "Popping an empty channel returns false")]
def test_channel_empty_pop(t : T?) {
    t |> run("pop from empty channel returns false") @(t : T?) {
        with_channel() $(ch) {
            var found = false
            ch |> pop_and_clone_one() $(val : IntVal#) {
                found = true
            }
            t |> equal(false, found)
        }
    }
}

// ---- Lockbox create/destroy ----

[test(description = "with_lock_box creates and destroys a lock box whose handle is valid inside the block")]
def test_lockbox_basics(t : T?) {
    t |> run("with_lock_box creates and destroys") @(t : T?) {
        var entered = false
        with_lock_box() $(box) {
            entered = true
            t |> equal(true, box.isValid)
        }
        t |> equal(true, entered)
    }
}

// ---- Channel properties tracking ----

[test(description = "isEmpty and total track pushes and pops")]
def test_channel_properties(t : T?) {
    t |> run("isEmpty and total track correctly") @(t : T?) {
        with_channel() $(ch) {
            t |> equal(true, ch.isEmpty)
            t |> equal(0, ch.total)
            ch |> push_clone(IntVal(v = 1))
            t |> equal(false, ch.isEmpty)
            t |> equal(1, ch.total)
            ch |> push_clone(IntVal(v = 2))
            t |> equal(2, ch.total)
            ch |> push_clone(IntVal(v = 3))
            t |> equal(3, ch.total)
        }
    }
}

// ---- gather_and_forward ----

[test(description = "gather_and_forward copies data from one channel to another")]
def test_gather_and_forward(t : T?) {
    t |> run("gather_and_forward copies data between channels") @(t : T?) {
        with_channel() $(src) {
            with_channel() $(dst) {
                src |> push_clone(IntVal(v = 10))
                src |> push_clone(IntVal(v = 20))
                src |> push_clone(IntVal(v = 30))
                var gathered : array<int>
                src |> gather_and_forward(dst) $(val : IntVal#) {
                    gathered |> push(val.v)
                }
                t |> equal(3, length(gathered))
                // dst should now have the data
                t |> equal(3, dst.total)
                var forwarded : array<int>
                dst |> for_each_clone() $(val : IntVal#) {
                    forwarded |> push(val.v)
                }
                t |> equal(3, length(forwarded))
            }
        }
    }
}

// ---- Concurrent jobs pushing to same channel ----

[test(description = "Multiple jobs can push to the same channel concurrently")]
def test_concurrent_jobs(t : T?) {
    t |> run("multiple jobs pushing to same channel") @(t : T?) {
        with_job_que() {
            let NUM_JOBS = 8
            with_channel(NUM_JOBS) $(ch) {
                for (i in range(NUM_JOBS)) {
                    new_job() @() {
                        ch |> push_clone(IntVal(v = i))
                        ch |> notify_and_release
                    }
                }
                var results : array<int>
                ch |> for_each_clone() $(val : IntVal#) {
                    results |> push(val.v)
                }
                t |> equal(NUM_JOBS, length(results))
                sort(results)
                for (i in range(NUM_JOBS)) {
                    t |> equal(i, results[i])
                }
            }
        }
    }
}


[test(description = "A panic of the with_job_que block's own body, recovered outside the block, leaves no job queue in the process, and a later with_job_que opens one and releases it")]
def test_block_panic_releases_the_queue(t : T?) {
    t |> success(!is_job_que_available(), "no queue stands before the block")
    var caught = false
    try {
        with_job_que() {
            panic("the block's own panic")
        }
    } recover {
        caught = true
    }
    t |> success(caught, "the block's panic reaches the recover around it")
    t |> success(!is_job_que_available(), "the queue the block opened is released")
    with_job_que() {
        t |> success(is_job_que_available(), "a later block opens a queue")
    }
    t |> success(!is_job_que_available(), "and releases it")
}

[test(description = "A job's panic recorded while a with_job_que block runs is dropped when the block itself panics: one panic reaches the recover around the block, the queue is released, and the next with_job_que block ends clean")]
def test_block_panic_drops_a_recorded_job_panic(t : T?) {
    var recovered = false
    try {
        with_job_que() {
            var status = job_status_create()
            status |> append(1)
            new_job() @() {
                //! the worker records the panic, then notifies the
                //! captured status for the job
                if (status != null) {
                    panic("the job's panic")
                }
                status |> notify_and_release
            }
            status |> join
            unsafe(job_status_remove(status))
            panic("the block's own panic")
        }
    } recover {
        recovered = true
    }
    t |> success(recovered, "the block's panic reaches the recover around it")
    t |> success(!is_job_que_available(), "the queue the block opened is released")
    var later = true
    try {
        with_job_que() {
            later = true
        }
    } recover {
        later = false
    }
    t |> success(later, "a later block does not end on the earlier job's panic")
}
