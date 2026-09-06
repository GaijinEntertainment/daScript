options gen2
options no_aot

require dastest/testing_boost public
require daslib/fio

// argv[0] is the running interpreter (dastest is launched as `daslang dastest/dastest.das ...`),
// so it spawns the daslang we want the child to run. Same trick as popen_argv.das.
def das_exe() : string {
    let args <- get_command_line_arguments()
    return empty(args) ? "" : args[0]
}

def has_line(lines : array<string>; s : string) : bool {
    for (l in lines) {
        if (l == s) return true
    }
    return false
}

def child_argv(fixture, ready, release : string) : array<string> {
    return [das_exe(), "-dasroot", get_das_root(), fixture, "--", ready, release]
}

[test]
def test_process_lifecycle(t : T?) {
    t |> run("spawn, drain lines, poll running, wait exit code, apply env + cwd") @(t : T?) {
        let tmp = create_temp_directory_result("das process test ")
        if (!(tmp is value)) {
            t |> failure("could not create temp directory: {tmp as error}")
            return
        }
        let root_dir = tmp as value
        let ready = path_join(root_dir, "ready")
        let release = path_join(root_dir, "release")
        let fixture = path_join(get_das_root(), "tests/fio/_fixture_process_child.das")
        var p = unsafe(spawn_process(child_argv(fixture, ready, release), root_dir, ["DAS_PROC_TEST=marker42"]))
        t |> success(p != null, "spawn_process returned a handle")
        // Drain the child's stdout until it signals ready; the file event drives completion,
        // the elapsed check is only a guard.
        var lines : array<string>
        let started = ref_time_ticks()
        while (!fexist(ready)) {
            unsafe(process_drain(p) $(line) {
                lines |> push(clone_string(line))
            })
            if (get_time_usec(started) > 15000000) {
                t |> failure("child never signaled ready")
                break
            }
            sleep(5u)
        }
        unsafe(process_drain(p) $(line) {
            lines |> push(clone_string(line))
        })
        t |> success(has_line(lines, "env=marker42"), "env override reached the child: {lines}")
        t |> success(has_line(lines, "cwd={base_name(root_dir)}"), "cwd applied to the child: {lines}")
        t |> success(has_line(lines, "hello one") && has_line(lines, "hello two"),
            "both stdout lines drained one per line: {lines}")
        t |> equal(unsafe(process_poll(p)), process_running, "child is still running while parked on release")
        let pid = unsafe(process_pid(p))
        t |> success(pid > 0, "pid is positive: {pid}")
        t |> success(unsafe(process_alive(pid)), "process_alive is true while running")
        t |> success(fwrite(release, "go"), "wrote the release event")
        t |> equal(unsafe(process_wait(p, 5.0)), 7, "child exited with its own code")
        t |> success(!unsafe(process_alive(pid)), "process_alive is false after exit")
        unsafe(close_process(p))
        let cleanup = rmdir_rec_result(root_dir)
        t |> success(cleanup is value && cleanup as value, "temp dir removed")
    }
}

[test]
def test_process_terminate(t : T?) {
    t |> run("terminate stops a child parked on a release that never comes") @(t : T?) {
        let tmp = create_temp_directory_result("das process kill ")
        if (!(tmp is value)) {
            t |> failure("could not create temp directory: {tmp as error}")
            return
        }
        let root_dir = tmp as value
        let ready = path_join(root_dir, "ready")
        let release = path_join(root_dir, "release")   // deliberately never written
        let fixture = path_join(get_das_root(), "tests/fio/_fixture_process_child.das")
        let noenv : array<string>
        var p = unsafe(spawn_process(child_argv(fixture, ready, release), "", noenv))
        t |> success(p != null, "spawn_process returned a handle")
        let started = ref_time_ticks()
        while (!fexist(ready)) {
            unsafe(process_drain(p) $(line) {})
            if (get_time_usec(started) > 15000000) {
                t |> failure("child never signaled ready")
                break
            }
            sleep(5u)
        }
        let pid = unsafe(process_pid(p))
        unsafe(process_terminate(p))
        let rc = unsafe(process_wait(p, 5.0))
        t |> success(rc != process_running, "child stopped after terminate (rc={rc})")
        t |> success(!unsafe(process_alive(pid)), "process_alive is false after terminate")
        unsafe(close_process(p))
        let cleanup = rmdir_rec_result(root_dir)
        t |> success(cleanup is value && cleanup as value, "temp dir removed")
    }
}
