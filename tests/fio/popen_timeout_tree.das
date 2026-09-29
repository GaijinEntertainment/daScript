options gen2
options no_aot

require dastest/testing_boost public
require daslib/fio
require daslib/strings_boost

let GRANDCHILD_SECONDS = 20
let LIMIT_SECONDS = 1.0
let RETURN_GUARD_SECONDS = 10.0

//! `sleep N; true` keeps the shell a parent: a shell given one command execs it and leaves no grandchild
def shell_with_grandchild() : string {
    return "sleep {GRANDCHILD_SECONDS}; true"
}

//! `spawn` starts the shell under LIMIT_SECONDS, drains its pipe through `drain` and answers the exit code
def expect_tree_killed(t : T?; spawn : block<(drain : block<(f : FILE const?) : void>) : int>) {
    if (get_running_platform_name() == "windows") {
        t |> skip("the shell command is POSIX")
        return
    }
    let started = ref_time_ticks()
    let rc = invoke(spawn) $(f : FILE const?) {
        if (f != null) {
            fread(f)
        }
    }
    let seconds = float(get_time_usec(started)) / 1000000.0
    t |> equal(rc, popen_timed_out)
    t |> success(seconds < RETURN_GUARD_SECONDS,
        "the call returned after {seconds} s: the grandchild outlived the timeout and held the pipe")
}

//! the shell's own pid and its process group, as the shell itself reads them
def pid_and_group(limit : float) : tuple<pid : int; group : int> {
    var text = ""
    unsafe(popen_argv(["/bin/sh", "-c", "echo $$; ps -o pgid= -p $$"], limit, $(f) {
        if (f != null) {
            text = fread(f)
        }
    }))
    var words <- [for (w in split(text, "\n")); strip(w); where !empty(strip(w))]
    let res = length(words) == 2 ? (pid = to_int(words[0]), group = to_int(words[1])) : (pid = -1, group = -2)
    delete words
    return res
}

[test]
def test_only_a_timed_child_leads_a_group(t : T?) {
    if (get_running_platform_name() == "windows") {
        t |> skip("the shell command is POSIX")
        return
    }
    t |> run("a timed popen_argv child leads its own process group") @(t : T?) {
        let timed = pid_and_group(30.0)
        t |> success(timed.pid > 0, "the shell reported its pid and group")
        t |> equal(timed.group, timed.pid)
    }
    t |> run("an untimed popen_argv child stays in its caller's group") @(t : T?) {
        let untimed = pid_and_group(0.0)
        t |> success(untimed.pid > 0 && untimed.group > 0, "the shell reported its pid and group")
        t |> success(untimed.group != untimed.pid, "the child's group {untimed.group} is not its own pid {untimed.pid}")
    }
}

[test]
def test_popen_timeout_kills_the_tree(t : T?) {
    t |> run("a timed popen returns once its limit passes, whatever the shell started") @(t : T?) {
        expect_tree_killed(t) $(drain) {
            return unsafe(popen_timeout(shell_with_grandchild(), LIMIT_SECONDS, drain))
        }
    }
}

[test]
def test_popen_argv_timeout_kills_the_tree(t : T?) {
    t |> run("a timed popen_argv returns once its limit passes, whatever its child started") @(t : T?) {
        expect_tree_killed(t) $(drain) {
            return unsafe(popen_argv(["/bin/sh", "-c", shell_with_grandchild()], LIMIT_SECONDS, drain))
        }
    }
}
