options gen2
options indenting = 4

require dastest/testing_boost public
require daslib/fio

//! no core dump into the tree from a signal the shell sends itself
def die_of(sig : string) : array<string> => <- ["/bin/sh", "-c", "ulimit -c 0; kill -{sig} $$"]

[test]
def test_a_child_killed_by_a_signal_reads_as_128_plus_the_signal(t : T?) {
    if (get_running_platform_name() == "windows") {
        t |> skip("POSIX signals")
        return
    }
    var out : string
    let exited = run_and_capture(["/bin/sh", "-c", "exit 11"], out)
    t |> equal(11, exited, "a plain exit code comes through as is")
    let segv = run_and_capture(die_of("SEGV"), out)
    t |> equal(128 + 11, segv, "run_and_capture reads SIGSEGV as the shell's $? does")
    t |> success(is_death_exit_code(segv) && !is_death_exit_code(exited),
                 "is_death_exit_code tells the signal death from the plain exit")
    t |> success(!is_death_exit_code(-1) && !is_death_exit_code(popen_timed_out)
                 && !is_death_exit_code(process_running),
                 "a failed spawn, a timeout and a running child are not deaths")
    let missing = run_and_capture(["/nonexistent/daslang"], out)
    t |> success(is_spawn_failure(missing) && !is_death_exit_code(missing),
                 "a child that cannot exec (exit {missing}) is a spawn failure, not a death")
    t |> success(is_spawn_failure(-1) && !is_spawn_failure(0) && !is_spawn_failure(segv),
                 "a failed spawn is one, a plain exit and a signal death are not")
    let trap = unsafe(popen_argv(die_of("TRAP"), 30.0, $(_f) {}))
    t |> equal(128 + 5, trap, "popen_argv with a timeout answers the same")
    let piped = unsafe(popen_argv_pipe(die_of("TERM")) $(_in, _out) {})
    t |> equal(128 + 15, piped, "popen_argv_pipe answers the same")
    let shell = unsafe(popen("kill -KILL $$") $(_f) {})
    t |> equal(128 + 9, shell, "popen answers the same for its shell")
    let timed = unsafe(popen_timeout("kill -HUP $$", 30.0) $(_f) {})
    t |> equal(128 + 1, timed, "popen_timeout answers the same for its shell")
    let noenv : array<string>
    var waited = unsafe(spawn_process(die_of("ABRT"), "", noenv))
    let waitedRc = unsafe(process_wait(waited, 30.0))
    t |> equal(128 + 6, waitedRc, "process_wait answers the same")
    unsafe(close_process(waited))
    var polled = unsafe(spawn_process(die_of("INT"), "", noenv))
    var polledRc = process_running
    for (_ in range(3000)) {
        polledRc = unsafe(process_poll(polled))
        break if (polledRc != process_running)
        sleep(10u)
    }
    t |> equal(128 + 2, polledRc, "process_poll answers the same")
    unsafe(close_process(polled))
}
