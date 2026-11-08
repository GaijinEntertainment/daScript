options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost public
require daslib/fio
require daslib/strings_boost
require strings

def dastest_root() : string => "{get_das_root()}/dastest"

def passing_fixture() : string => "{dastest_root()}/tests/_fixture_passing.das"

def raised_fixture() : string => "{dastest_root()}/tests/_fixture_raised_timeout.das"

def run_dastest_on(fixture : string; args : array<string>;
                   var output : string&; var exit_code : int&) {
    let exe = get_command_line_arguments()[0]
    var cmd <- [exe, "-dasroot", get_das_root(), "{dastest_root()}/dastest.das", "--",
                "--failures-only", "--test", fixture]
    cmd |> push_from(args)
    exit_code = run_and_capture(cmd, output)
}

def run_dastest(args : array<string>; var output : string&; var exit_code : int&) {
    run_dastest_on(passing_fixture(), args, output, exit_code)
}

[test(description = "--max-file-time fails a completed file past the limit in both modes, and a file's declared test budgets raise its limit")]
def test_max_file_time(tt : T?) {
    tt |> run("a generous file-time limit preserves a passing suite") <| @(t : T?) {
        var output : string
        var exit_code : int
        run_dastest(["--max-file-time", "3600"], output, exit_code)
        t |> equal(exit_code, 0)
        t |> success(output |> contains("3 tests, 3 passed"),
            "expected the passing summary, got:\n{output}")
    }

    tt |> run("a completed file over the limit fails without sleeps") <| @(t : T?) {
        // 1ms is below the fixture's compile+run cost on every supported host;
        // no elapsed-time wait or frame count drives this test.
        var output : string
        var exit_code : int
        run_dastest(["--max-file-time", "0.001"], output, exit_code)
        t |> success(exit_code != 0, "expected a non-zero exit, got {exit_code}\n{output}")
        t |> success(output |> contains("_fixture_passing.das"),
            "failure should name the slow file, got:\n{output}")
        t |> success(output |> contains("exceeding --max-file-time 0.001s"),
            "failure should name the limit, got:\n{output}")
    }

    tt |> run("isolated mode enforces the same per-file limit") <| @(t : T?) {
        var output : string
        var exit_code : int
        run_dastest(["--max-file-time", "0.001", "--isolated-mode", "--isolated-mode-threads", "1"], output, exit_code)
        t |> success(exit_code != 0, "expected a non-zero exit, got {exit_code}\n{output}")
        t |> success(output |> contains("_fixture_passing.das"),
            "isolated failure should name the slow file, got:\n{output}")
        t |> success(output |> contains("exceeding --max-file-time 0.001s"),
            "isolated failure should name the limit, got:\n{output}")
    }

    tt |> run("a declared test budget raises the file's limit by the same amount") <| @(t : T?) {
        // the fixture sleeps 300ms inside a 10s budget; the 1ms limit alone
        // would fail it, the budget lifts the limit to 10.001s
        var output : string
        var exit_code : int
        run_dastest_on(raised_fixture(), ["--max-file-time", "0.001"], output, exit_code)
        t |> equal(exit_code, 0,
            "the declared budget should lift the limit, got {exit_code}:\n{output}")
        t |> success(!(output |> contains("exceeding --max-file-time")),
            "no file-time failure expected, got:\n{output}")
    }
}
