options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost public
require daslib/fio
require daslib/strings_boost
require strings

def dastest_root() : string {
    return "{get_das_root()}/dastest"
}

def fixture_dir() : string {
    return "{dastest_root()}/tests/_isolated_fixture"
}

def fixture_dir_pre_dashdash() : string {
    return "{dastest_root()}/tests/_isolated_fixture_pre_dashdash"
}

def fixture_dir_crash() : string {
    return "{dastest_root()}/tests/_isolated_fixture_crash"
}

def run_dastest(cmd : string; var output : string&; var exit_code : int&) {
    output = ""
    unsafe {
        exit_code = popen(cmd) $(f) {
            if (f != null) {
                while (!feof(f)) {
                    output += fgets(f)
                }
            }
        }
    }
}

[test]
def test_isolated_mode(tt : T?) {
    tt |> run("isolated mode spawns 4 workers, fixture suite all passes, --worker-index round-trips") <| @(t : T?) {
        let exe = get_command_line_arguments()[0]
        let cmd = "{exe} {dastest_root()}/dastest.das -- --test {fixture_dir()} --isolated-mode --isolated-mode-threads 4"
        var output : string
        var exit_code : int
        run_dastest(cmd, output, exit_code)
        t |> equal(exit_code, 0)
        t |> success(output |> contains("SUCCESS"), "expected SUCCESS in dastest output, got:\n{output}")
        t |> success(!(output |> contains("subprocess stderr")),
            "a passing isolated run must not emit the crashed-stderr header")
    }

    tt |> run("isolated mode preserves pre-`--` daslang flags (-dasroot) in singleTest cmd") <| @(t : T?) {
        // Pass `-dasroot {existing-dasroot}` — a no-op pre-`--` flag — to the outer
        // dastest. Pre-fix, dastest's singleTest reconstruction used
        // `raw_args[0] raw_args[1]` which dropped everything between exe and
        // dastest.das. The fixture test under fixture_dir_pre_dashdash() asserts
        // -dasroot survives into the spawned subprocess's argv.
        let exe = get_command_line_arguments()[0]
        let cmd = "{exe} -dasroot {get_das_root()} {dastest_root()}/dastest.das -- --test {fixture_dir_pre_dashdash()} --isolated-mode --isolated-mode-threads 1"
        var output : string
        var exit_code : int
        run_dastest(cmd, output, exit_code)
        t |> equal(exit_code, 0)
        t |> success(output |> contains("SUCCESS"), "expected SUCCESS in dastest output, got:\n{output}")
    }

    tt |> run("isolated mode folds a crashed worker's stderr (crash-handler stack) into the failing test's log") <| @(t : T?) {
        // The fixture hard-crashes, so the spawned dastest reports a failure
        // (non-zero exit) — the point is that the crash diagnostic is surfaced
        // under the failing test rather than dropped with the worker's stderr.
        let exe = get_command_line_arguments()[0]
        let cmd = "{exe} {dastest_root()}/dastest.das -- --test {fixture_dir_crash()} --isolated-mode --isolated-mode-threads 1"
        var output : string
        var exit_code : int
        run_dastest(cmd, output, exit_code)
        t |> success(output |> contains("subprocess stderr"),
            "expected the captured-stderr header in output, got:\n{output}")
        t |> success(output |> contains("CRASH"),
            "expected the crash-handler 'CRASH' line folded into the log, got:\n{output}")
    }
}
