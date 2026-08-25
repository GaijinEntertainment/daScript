options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false
require dastest/testing_boost public
require daslib/jobque_boost
require daslib/fio

[test]
def test_join_timeout_expires(t : T?) {
    t |> run("join times out when the wait group is never notified") @(t : T?) {
        with_job_status(1) $(status) {
            let done = status |> join(50)
            t |> equal(done, false)
            t |> equal(status.isReady, false)
        }
    }
}

[test]
def test_join_timeout_completes(t : T?) {
    t |> run("join returns true once every job has notified") @(t : T?) {
        with_job_que() {
            with_job_status(2) $(status) {
                for (_i in range(2)) {
                    new_job() @() {
                        status |> notify_and_release
                    }
                }
                let done = status |> join(10000)
                t |> equal(done, true)
                t |> equal(status.isReady, true)
            }
        }
    }
}

[test]
def test_join_timeout_then_success(t : T?) {
    t |> run("a short join times out, a later join still sees the completion") @(t : T?) {
        with_job_que() {
            with_job_status(1) $(status) {
                new_job() @() {
                    sleep(250u)
                    status |> notify_and_release
                }
                let tooEarly = status |> join(10)
                t |> equal(tooEarly, false)
                let done = status |> join(10000)
                t |> equal(done, true)
            }
        }
    }
}

[test]
def test_join_timeout_flushes_batched_dispatch(t : T?) {
    t |> run("join publishes the pending batch, same as the untimed join") @(t : T?) {
        with_job_que() {
            set_jobque_batch_dispatch(true)
            with_job_status(2) $(status) {
                for (_i in range(2)) {
                    new_job() @() {
                        status |> notify_and_release
                    }
                }
                let done = status |> join(10000)
                t |> equal(done, true)
            }
            set_jobque_batch_dispatch(false)
        }
    }
}

[test]
def test_join_zero_timeout_polls(t : T?) {
    t |> run("join with a non-positive timeout polls instead of waiting") @(t : T?) {
        with_job_status(1) $(status) {
            let pending = status |> join(0)
            t |> equal(pending, false)
            let stillPending = status |> join(-1)
            t |> equal(stillPending, false)
            status |> notify
            let ready = status |> join(0)
            t |> equal(ready, true)
        }
    }
}

[test]
def test_join_timeout_with_join_spin(t : T?) {
    t |> run("the timeout bounds the wait with the join spin enabled") @(t : T?) {
        set_jobque_join_spin(2)
        with_job_status(1) $(status) {
            let done = status |> join(50)
            t |> equal(done, false)
        }
        set_jobque_join_spin(0)
    }
}

[test]
def test_join_timeout_on_filled_lock_box(t : T?) {
    t |> run("a filled lock box nobody grabs times out instead of waiting forever") @(t : T?) {
        with_lock_box() $(box) {
            t |> equal(box.isReady, true, "a fresh box holds nothing")
            var payload <- [for (i in range(4)); float(i) * 1.5]
            box |> fill(payload)
            t |> equal(box.isReady, false, "fill marks the box full")
            let taken = box |> join(50)
            t |> equal(taken, false, "nobody grabbed, so the bounded wait must report failure")
            var seen = 0.0
            box |> grab() $(var v : array<float>) {
                for (x in v) {
                    seen += x
                }
            }
            t |> equal(seen, 9.0)
            t |> equal(box.isReady, true)
            t |> equal(box |> join(50), true, "an emptied box joins immediately")
        }
    }
}
