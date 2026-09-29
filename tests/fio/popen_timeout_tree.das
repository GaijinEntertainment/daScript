options gen2
options no_aot

require dastest/testing_boost public
require daslib/fio

let GRANDCHILD_SECONDS = 20
let LIMIT_SECONDS = 1.0
let RETURN_GUARD_SECONDS = 10.0

//! `sleep N; true` keeps the shell a parent: a shell given one command execs it and leaves no grandchild
def shell_with_grandchild() : string {
    return "sleep {GRANDCHILD_SECONDS}; true"
}

def seconds_since(started : int64) : float {
    return float(get_time_usec(started)) / 1000000.0
}

def expect_tree_killed(t : T?; rc : int; seconds : float) {
    t |> equal(rc, popen_timed_out)
    t |> success(seconds < RETURN_GUARD_SECONDS,
        "the call returned after {seconds} s: the grandchild outlived the timeout and held the pipe")
}

[test]
def test_popen_timeout_kills_the_tree(t : T?) {
    if (get_running_platform_name() == "windows") {
        t |> skip("the shell command is POSIX")
        return
    }
    t |> run("a timed popen returns once its limit passes, whatever the shell started") @(t : T?) {
        let started = ref_time_ticks()
        let rc = unsafe(popen_timeout(shell_with_grandchild(), LIMIT_SECONDS, $(f) {
            if (f != null) {
                fread(f)
            }
        }))
        expect_tree_killed(t, rc, seconds_since(started))
    }
}

[test]
def test_popen_argv_timeout_kills_the_tree(t : T?) {
    if (get_running_platform_name() == "windows") {
        t |> skip("the shell command is POSIX")
        return
    }
    t |> run("a timed popen_argv returns once its limit passes, whatever its child started") @(t : T?) {
        let started = ref_time_ticks()
        let rc = unsafe(popen_argv(["/bin/sh", "-c", shell_with_grandchild()], LIMIT_SECONDS, $(f) {
            if (f != null) {
                fread(f)
            }
        }))
        expect_tree_killed(t, rc, seconds_since(started))
    }
}
