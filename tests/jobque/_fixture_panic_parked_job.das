options gen2
require daslib/jobque_boost
require daslib/fio
require daslib/clargs

// The child for the panic-teardown test in uncategorized.das, and the
// root modules/dasLLVM/tests/llvm_exe.das builds as an
// exe. The first argument picks what main leaves running when it
// panics - the shutdown that would have stopped it never runs:
//   (none)   a persistent que whose one job never returns, like a
//            serving slot's worker loop waiting for work
//   thread   a detached new_thread that never returns
//   clean    nothing - the panic finds no job and no thread
// The process exits with -2 either way. The parked worker waits on
// a sleep, not a channel: a `with_channel` scope a panic unwinds out
// of is its own failure (its release guard sees the channel held).
def park_forever() {
    while (true) {
        sleep(10u)
    }
}

[export]
def main : int {
    let args <- get_cli_arguments()
    let mode = empty(args) ? "job" : args[0]
    if (mode == "clean") {
        panic("boom")
    }
    create_job_que()
    if (mode == "thread") {
        new_thread() <| @ {
            park_forever()
        }
    } else {
        new_job() @() {
            park_forever()
        }
    }
    sleep(100u)
    panic("boom")
    return 0
}
