options gen2
options no_aot // a subprocess fixture with no [test] - an AOT build of it tests nothing

require daslib/fio

// Child for test_process.das: writes a line with no newline, flushes it into the pipe, then waits
// until the parent's timeout kills it (the elapsed-time check is only a deadlock guard).
[export]
def main() : int {
    fprint(fstdout(), "partial")
    fflush(fstdout())
    let started = ref_time_ticks()
    while (get_time_usec(started) < 30000000) {
        sleep(10u)
    }
    return 0
}
