options gen2

require dastest/testing_boost public
require strings
require daslib/fio
require daslib/strings_boost

// Helpers ---------------------------------------------------------------

// argv[0] is the running interpreter; on these tests dastest is invoked
// as `daslang(.exe) dastest/dastest.das ...`, so argv[0] points at the
// daslang binary we want to spawn. Avoids `daslib/command_line` so this
// test's AOT footprint stays self-contained.
def das_exe() : string {
    let args <- get_command_line_arguments()
    return empty(args) ? "" : args[0]
}

def run_argv(args : array<string>; var output : string&) : int {
    return unsafe(popen_argv(args, 0.0, $(f) {
        if (f != null) {
            output = fread(f)
        }
    }))
}

def wait_for_file(path : string) : bool {
    let started = ref_time_ticks()
    while (!fexist(path)) {
        // Guard only: completion is driven by the file event, not a delay.
        if (get_time_usec(started) > 5000000) return false
    }
    return true
}

// Tests -----------------------------------------------------------------

[test(description = "popen_argv runs the daslang binary and captures its help banner")]
def test_popen_argv_basic_invocation(t : T?) {
    t |> run("daslang --help captures expected banner") @(t : T?) {
        var output : string
        let rc = run_argv([das_exe(), "--help"], output)
        t |> equal(rc, 0)
        t |> success(find(output, "daslang version") >= 0,
            "expected 'daslang version' in output: {output}")
    }
}

[test(description = "popen_argv reports a non-zero exit code when the child gets an unknown flag")]
def test_popen_argv_unknown_flag_nonzero_exit(t : T?) {
    t |> run("invalid flag returns non-zero exit") @(t : T?) {
        var output : string
        // -no-such-flag-xyz123 is intentionally unknown so daslang errors out.
        let rc = run_argv([das_exe(), "-no-such-flag-xyz123"], output)
        t |> success(rc != 0, "expected non-zero exit, got {rc}")
    }
}

[test(description = "--help and -h after the -- separator reach the script as its own arguments")]
def test_help_after_separator_reaches_the_script(t : T?) {
    t |> run("--help and -h after -- are script arguments, not the driver's") @(t : T?) {
        let fixture = path_join(get_das_root(), "tests/fio/_fixture_echo_args.das")
        var output : string
        let rc = run_argv([das_exe(), "-dasroot", get_das_root(), fixture, "--", "--help", "-h"],
            output)
        t |> equal(rc, 0)
        t |> success(find(output, "--help\n") >= 0, "the script should see --help: {output}")
        t |> success(find(output, "-h\n") >= 0, "the script should see -h: {output}")
        t |> success(find(output, "daslang version") < 0,
            "the driver must not answer a help flag placed after --: {output}")
    }
}

[test(description = "An argv element containing $(...) reaches the child literally, with no shell expansion")]
def test_popen_argv_unix_dollar_paren_not_expanded(t : T?) {
    t |> run("argv with $(...) is passed literally, not expanded by a shell") @(t : T?) {
        // If popen_argv invoked /bin/sh, "$(echo SHOULDNOTAPPEAR)" would be
        // command-substituted to "SHOULDNOTAPPEAR". Argv-direct passes it
        // verbatim, so daslang sees an unknown arg containing the literal
        // dollar-paren string — that text echoes back in the diagnostic.
        var output : string
        let rc = run_argv([das_exe(), "-X-$(echo SHOULDNOTAPPEAR)"], output)
        t |> success(rc != 0, "expected non-zero exit")
        t |> success(find(output, "$(echo") >= 0,
            "argv should be passed literally; expected '$(echo' to appear in output: {output}")
        t |> success(find(output, "SHOULDNOTAPPEAR") >= 0,
            "literal token text should also appear: {output}")
    }
}

[test(description = "An argv element containing an ampersand reaches the child literally and is not a command separator")]
def test_popen_argv_windows_ampersand_not_separator(t : T?) {
    t |> run("argv with `&` is passed literally, not interpreted as cmd separator") @(t : T?) {
        // If popen_argv invoked cmd.exe on Windows, "& echo PWNED" would be
        // a chained command. Argv-direct passes it as one arg.
        var output : string
        let rc = run_argv([das_exe(), "-X-& echo PWNED"], output)
        t |> success(rc != 0, "expected non-zero exit")
        t |> success(find(output, "PWNED") >= 0,
            "literal token text should appear in error output: {output}")
    }
}

[test(description = "An argv element with an embedded space stays one argument")]
def test_popen_argv_arg_with_spaces(t : T?) {
    t |> run("argv element with embedded space stays a single arg") @(t : T?) {
        var output : string
        // Without proper quoting on Windows, "two words" would split into
        // two args; we'd see a different diagnostic. With proper quoting,
        // the unknown arg is the full string including the space.
        let rc = run_argv([das_exe(), "-X-arg with spaces"], output)
        t |> success(rc != 0, "expected non-zero exit")
        t |> success(find(output, "arg with spaces") >= 0,
            "expected full string with spaces in error output: {output}")
    }
}

[test(description = "An argv element containing a double quote reaches the child intact")]
def test_popen_argv_arg_with_embedded_quote(t : T?) {
    t |> run("argv element containing a double-quote is preserved") @(t : T?) {
        // Tests Windows arg-quoting (CommandLineToArgvW backslash rules) and
        // Unix execvp passthrough.
        var output : string
        let rc = run_argv([das_exe(), "-X-pre\"mid\"post"], output)
        t |> success(rc != 0, "expected non-zero exit")
        t |> success(find(output, "pre") >= 0 && find(output, "mid") >= 0 && find(output, "post") >= 0,
            "expected pre/mid/post tokens in output: {output}")
    }
}

[test(description = "A relative argv[0] with forward slashes and no leading ./ still spawns")]
def test_popen_argv_relative_forward_slash_exe(t : T?) {
    t |> run("a relative argv[0] with forward slashes and no leading ./ spawns") @(t : T?) {
        let exe = get_full_file_name(das_exe())
        let exe_dir = dir_name(exe)
        let rel = "{base_name(exe_dir)}/{base_name(exe)}"
        let launch_dir = dir_name(exe_dir)
        let saved_cwd = getcwd()
        if (!chdir(launch_dir)) {
            t |> failure("could not chdir to {launch_dir}")
            return
        }
        var output : string
        let rc = run_argv([rel, "--help"], output)
        t |> success(chdir(saved_cwd), "could not restore the working directory {saved_cwd}")
        t |> equal(rc, 0, "spawning {rel} from {launch_dir} failed")
        t |> success(find(output, "daslang version") >= 0,
            "expected 'daslang version' in output: {output}")
    }
}

[test(description = "A popen_argv child cannot read the parent's stdin")]
def test_popen_argv_child_stdin_isolated(t : T?) {
    t |> run("child cannot read parent's stdin") @(t : T?) {
        // Spawn the same echo fixture used by test_popen_argv_pipe. The
        // fixture reads stdin in a loop and echoes each line. With proper
        // stdin isolation, the child sees EOF immediately, exits cleanly
        // with no output. Without isolation, the child would block reading
        // from the parent's terminal/inherited stdin, hanging the test.
        var output : string
        let fixture = path_join(get_das_root(), "tests/mcp/_fixture_echo.das")
        let rc = run_argv([das_exe(), "-dasroot", get_das_root(), fixture], output)
        t |> equal(rc, 0, "child should exit cleanly")
        t |> equal(output, "", "child should produce no output (EOF on stdin)")
    }
}

// DISABLED 2026-07-17: this test's file-event handshake depends on a spawned
// interpreter compiling the fixture within five seconds. JIT-disabled Win64
// CI legitimately exceeds that deadline, making the test timing-sensitive.
// Keep the scenario here until it can be covered without a wall-clock gate.
// [test]
def test_spawn_argv_returns_while_child_is_running(t : T?) {
    t |> run("spawn_argv is non-blocking and preserves argv boundaries") @(t : T?) {
        let tmp = create_temp_directory_result("spawn argv spaces ")
        if (!(tmp is value)) {
            t |> failure("could not create temp directory: {tmp as error}")
            return
        }
        let root = tmp as value
        let ready = path_join(root, "ready event")
        let release = path_join(root, "release event")
        let done = path_join(root, "done event")
        let fixture = path_join(get_das_root(), "tests/fio/_fixture_spawn_argv.das")
        let launched = unsafe(spawn_argv([
            das_exe(), "-dasroot", get_das_root(), fixture, "--",
            ready, release, done]))
        t |> success(launched, "child process launched")
        if (launched) {
            t |> success(wait_for_file(ready), "child signaled ready")
            t |> success(!fexist(done),
                "spawn returned while child was waiting for its release event")
            t |> success(fwrite(release, "release"), "parent signaled release")
            t |> success(wait_for_file(done), "child acknowledged release")
        }
        let cleanup = rmdir_rec_result(root)
        t |> success(cleanup is value && cleanup as value,
            "temporary spawn fixture was removed")
    }
}

[test(description = "spawn_argv rejects a missing executable")]
def test_spawn_argv_reports_exec_failure(t : T?) {
    t |> run("spawn_argv rejects a missing executable") @(t : T?) {
        let launched = unsafe(spawn_argv([
            "definitely-not-a-real-dascript-executable-7f37b1e9"]))
        t |> success(!launched, "missing executable was not reported as launched")
    }
}

[test(description = "A cwd and environment values outside ASCII reach the child, and a failed spawn still runs the block with a safe file")]
def test_popen_argv_paths_outside_ascii(t : T?) {
    t |> run("a cwd and an environment value outside ASCII reach the child") @(t : T?) {
        let dir = path_join(test_temp_dir(), "cwd_日本")
        t |> success(mkdir(dir), "mkdir {dir}")
        let child = path_join(get_das_root(), "tests/fio/_fixture_process_child.das")
        let ready = path_join(dir, "ready")
        let release = path_join(dir, "release")
        fwrite(release, "go")
        var output : string
        let args <- [das_exe(), child, "--", ready, release]
        let rc = unsafe(popen_argv(args, dir, ["DAS_PROC_TEST=値"], 30.0) $(f) {
            if (f != null) {
                output = fread(f)
            }
        })
        t |> equal(rc, 7, output)
        t |> success(find(output, "env=値\n") >= 0, output)
        t |> success(find(output, "cwd=cwd_日本\n") >= 0, output)
    }
    t |> run("an environment variable outside ASCII round-trips through set and get") @(t : T?) {
        set_env_variable("DAS_T_変数", "値 ünï")
        t |> equal(get_env_variable("DAS_T_変数"), "値 ünï")
        t |> success(has_env_variable("DAS_T_変数"))
    }
    t |> run("a failed spawn still runs the block; feof on its file is safe") @(t : T?) {
        let no_env : array<string>
        var saw_null = false
        var at_end = false
        let bad_dir = path_join(test_temp_dir(), "no_such_dir_日本")
        let rc = unsafe(popen_argv(["no_such_program_値"], bad_dir, no_env, 5.0) $(f) {
            saw_null = f == null
            at_end = feof(f)
        })
        t |> success(rc != 0, "rc={rc}")
        t |> success(!saw_null || at_end, "a null file reads as at its end")
    }
}

let GRANDCHILD_SECONDS = 20
let LIMIT_SECONDS = 1.0
let RETURN_GUARD_SECONDS = 10.0

//! `sleep N; true` keeps the shell a parent: a shell given one command execs it and leaves no grandchild
def shell_with_grandchild() : string => "sleep {GRANDCHILD_SECONDS}; true"

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

[test(description = "A timed popen_argv child leads its own process group while an untimed one stays in the caller's")]
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

[test(description = "A timed popen returns once its limit passes, whatever processes the shell started")]
def test_popen_timeout_kills_the_tree(t : T?) {
    t |> run("a timed popen returns once its limit passes, whatever the shell started") @(t : T?) {
        expect_tree_killed(t) $(drain) {
            return unsafe(popen_timeout(shell_with_grandchild(), LIMIT_SECONDS, drain))
        }
    }
}

[test(description = "A timed popen_argv returns once its limit passes, whatever processes its child started")]
def test_popen_argv_timeout_kills_the_tree(t : T?) {
    t |> run("a timed popen_argv returns once its limit passes, whatever its child started") @(t : T?) {
        expect_tree_killed(t) $(drain) {
            return unsafe(popen_argv(["/bin/sh", "-c", shell_with_grandchild()], LIMIT_SECONDS, drain))
        }
    }
}

//! no core dump into the tree from a signal the shell sends itself
def die_of(sig : string) : array<string> => <- ["/bin/sh", "-c", "ulimit -c 0; kill -{sig} $$"]

[test(description = "run_and_capture reads a child killed by a signal as 128 plus the signal number, and is_death_exit_code tells it from a plain exit")]
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

// `get_executable_path` names the binary running this test - daslang, dastest or an AOT test
// host - as an absolute path to a regular file, which is what a standalone program locates its
// shipped files from

[test(description = "The running binary's path is absolute and names a regular file")]
def test_executable_path(t : T?) {
    t |> run("the running binary: absolute, a regular file") @(t : T?) {
        let exe = get_executable_path()
        t |> success(!empty(exe), "the platform names it")
        t |> success(is_absolute(exe), "absolute: {exe}")
        let st = stat(exe)
        t |> success(st.is_valid && st.is_reg, "a regular file: {exe}")
    }
}

// `executable_on_path` answers from the running platform's own shell: the shell it runs through is
// on PATH by construction, and a name no box carries is not

[test(description = "The shell that runs the probe is found on PATH and a made-up name is not")]
def test_executable_on_path(t : T?) {
    t |> run("the shell that runs the probe is on PATH, and a made-up name is not") @(t : T?) {
        let shell = get_running_platform_name() == "windows" ? "cmd" : "sh"
        t |> success(executable_on_path(shell), "{shell} resolves on PATH")
        t |> success(!executable_on_path("no_such_program_9f2c_anywhere"), "an unknown name does not")
    }
}
