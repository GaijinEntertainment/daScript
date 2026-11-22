options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost public
require dastest/worker_policy
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

def run_dastest_passing(args : array<string>; var output : string&; var exit_code : int&) {
    run_dastest_on(passing_fixture(), args, output, exit_code)
}

def private run_annotation_fixture(fixture : string; var output : string&;
                                   extra : array<string>) : int {
    let exe = get_command_line_arguments()[0]
    let root = get_das_root()
    var cmd <- [exe, "-dasroot", root, "{root}/dastest/dastest.das", "--", "--test",
                "{root}/dastest/tests/{fixture}"]
    cmd |> push_from(extra)
    return run_and_capture(cmd, output)
}

def private run_annotation_fixture(fixture : string; var output : string&) : int {
    let none : array<string>
    return run_annotation_fixture(fixture, output, none)
}

[test(description = "a suite that runs past --timeout ends with the timeout message and exit code 124, not a crash", timeout = 1200)]
def test_timeout(tt : T?) {
    tt |> run("a suite that runs past --timeout ends with the timeout message and exit code 124, not a crash") <| @(t : T?) {
        let exe = get_command_line_arguments()[0]
        let cmd <- [exe, "-dasroot", get_das_root(), "{dastest_root()}/dastest.das", "--",
            "--test", "{dastest_root()}/tests/_timeout_fixture", "--timeout", "1"]
        var output : string
        let exit_code = run_and_capture(cmd, output, 60.0)
        t |> equal(exit_code, 124, "expected the timeout exit code, got {exit_code}:\n{output}")
        t |> success(output |> contains("ran past its --timeout of 1s"),
            "expected the timeout message, got:\n{output}")
        t |> success(output |> contains("test_sleep.das (file 1 of 1)"),
            "the watchdog names the file the sweep was on:\n{output}")
        let running = "in test 'test_sleep', subtest 'sleeps past the parent's --timeout'"
        t |> success(output |> contains(running),
            "the watchdog names the test and the subtest that was running:\n{output}")
        t |> success(!(output |> contains("should have ended the process")),
            "the fixture's test ran to its end - the timeout did not end the process:\n{output}")
        t |> success(!(output |> contains("CALL STACK")) && !(output |> contains("[daslang atexit]")),
            "the timeout ended the process as a crash or an unbalanced shutdown, not a plain exit:\n{output}")
    }
}

[test(description = "a test still running past --test-wall ends the process at once with exit code 125 and a line naming the test and its file, and a suite whose tests finish under the wall passes", timeout = 1200)]
def test_wall(tt : T?) {
    tt |> run("a test that outlives the wall ends the process, exit code 125, named") <| @(t : T?) {
        let exe = get_command_line_arguments()[0]
        let cmd <- [exe, "-dasroot", get_das_root(), "{dastest_root()}/dastest.das", "--",
            "--test", "{dastest_root()}/tests/_timeout_fixture", "--test-wall", "1"]
        var output : string
        let exit_code = run_and_capture(cmd, output, 60.0)
        t |> equal(exit_code, 125, "expected the wall's exit code, got {exit_code}:\n{output}")
        t |> success(output |> contains("kill switch: test 'test_sleep' of")
            && output |> contains("test_sleep.das (--test-wall) ran past 1s"),
            "the wall names the test and its file:\n{output}")
        t |> success(!(output |> contains("should have ended the process")),
            "the fixture's test ran to its end - the wall did not end the process:\n{output}")
        t |> success(!(output |> contains("CALL STACK"))
            && !(output |> contains("[daslang atexit]")),
            "the wall ended the process as a crash, not a plain exit:\n{output}")
    }

    tt |> run("a finalize past the wall ends nothing: the wall guards a test") <| @(t : T?) {
        var output : string
        var exit_code : int
        let fixture = "{dastest_root()}/tests/_fixture_wall_idle.das"
        run_dastest_on(fixture, ["--test-wall", "1"], output, exit_code)
        t |> equal(exit_code, 0, "the switch was disarmed as the test ended:\n{output}")
        t |> success(output |> contains("1 tests, 1 passed"),
            "expected the passing summary, got:\n{output}")
    }

    tt |> run("a test that declares a longer budget is held to it, not to the wall") <| @(t : T?) {
        var output : string
        var exit_code : int
        let fixture = "{dastest_root()}/tests/_fixture_wall_budget.das"
        run_dastest_on(fixture, ["--test-wall", "1"], output, exit_code)
        t |> equal(exit_code, 0, "the declared budget raised the wall:\n{output}")
        t |> success(output |> contains("1 tests, 1 passed"),
            "expected the passing summary, got:\n{output}")
    }

    tt |> run("a budget under the wall does not lower it: the test ends, then fails") <| @(t : T?) {
        var output : string
        var exit_code : int
        let fixture = "{dastest_root()}/tests/_fixture_wall_short_budget.das"
        run_dastest_on(fixture, ["--test-wall", "20"], output, exit_code)
        t |> success(exit_code != 125 && exit_code != 0,
            "the wall stayed at the run's, exit code {exit_code}:\n{output}")
        t |> success(output |> contains("FAIL 'test_past_its_budget'")
            && output |> contains("exceeding its timeout=1.000s"),
            "the test ran to its end and failed on the budget it declares:\n{output}")
    }

    tt |> run("tests that finish under the wall pass (the control)") <| @(t : T?) {
        var output : string
        var exit_code : int
        run_dastest_passing(["--test-wall", "3600"], output, exit_code)
        t |> equal(exit_code, 0, "a generous wall fails nothing:\n{output}")
        t |> success(output |> contains("3 tests, 3 passed"),
            "expected the passing summary, got:\n{output}")
    }
}

[test(description = "--max-file-time fails a completed file past the limit in both modes, and a file's declared test budgets raise its limit", timeout = 1200)]
def test_max_file_time(tt : T?) {
    tt |> run("a generous file-time limit preserves a passing suite") <| @(t : T?) {
        var output : string
        var exit_code : int
        run_dastest_passing(["--max-file-time", "3600"], output, exit_code)
        t |> equal(exit_code, 0)
        t |> success(output |> contains("3 tests, 3 passed"),
            "expected the passing summary, got:\n{output}")
    }

    tt |> run("a completed file over the limit fails without sleeps") <| @(t : T?) {
        // 1ms is below the fixture's compile+run cost on every supported host;
        // no elapsed-time wait or frame count drives this test.
        var output : string
        var exit_code : int
        run_dastest_passing(["--max-file-time", "0.001"], output, exit_code)
        t |> success(exit_code != 0, "expected a non-zero exit, got {exit_code}\n{output}")
        t |> success(output |> contains("_fixture_passing.das"),
            "failure should name the slow file, got:\n{output}")
        t |> success(output |> contains("exceeding --max-file-time 0.001s"),
            "failure should name the limit, got:\n{output}")
    }

    tt |> run("isolated mode enforces the same per-file limit") <| @(t : T?) {
        var output : string
        var exit_code : int
        run_dastest_passing(["--max-file-time", "0.001", "--isolated-mode",
            "--isolated-mode-threads", "1"], output, exit_code)
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

[test(description = "[test] carries description and timeout: the claim prints under a FAIL only, an overrun test fails, a mistyped argument does not compile", timeout = 1200)]
def test_annotation_args(tt : T?) {
    var output : string
    let exit_code = run_annotation_fixture("_fixture_annotation_args.das", output)

    tt |> run("the suite fails: one deliberate failure and two overruns, one with an " +
        "integer timeout") <| @(t : T?) {
        t |> success(exit_code != 0, "expected a non-zero exit, got {exit_code}\n{output}")
        t |> success(output |> contains("3 failed"), "expected three failed tests, got:\n{output}")
        t |> success(output |> contains("--- FAIL 'test_over_integer_budget'"),
                     "expected the integer-timeout test to fail, got:\n{output}")
        t |> success(output |> contains("exceeding its timeout=1.000s"),
                     "expected the integer budget in the message, got:\n{output}")
    }

    tt |> run("a failing test's description prints under its FAIL line") <| @(t : T?) {
        let failLine = find(output, "--- FAIL 'test_described_failure'")
        t |> success(failLine >= 0, "expected the FAIL line, got:\n{output}")
        let claim = find(output, "the described failure prints this claim under its FAIL line")
        t |> success(claim > failLine, "expected the claim after the FAIL line, got:\n{output}")
    }

    tt |> run("a passing test's description is not printed") <| @(t : T?) {
        t |> success(!(output |> contains("a passing test's claim is never printed")),
                     "a pass must not print its claim:\n{output}")
    }

    tt |> run("a test past its timeout is reported failed with the timeout named") <| @(t : T?) {
        t |> success(output |> contains("--- FAIL 'test_over_budget'"),
                     "expected the overrun test to fail, got:\n{output}")
        t |> success(output |> contains("exceeding its timeout=0.001s"),
                     "expected the budget in the message, got:\n{output}")
    }

    tt |> run("in isolated mode a declared timeout raises its file's deadline") <| @(t : T?) {
        // the fixture sleeps 300ms under a 50ms per-file budget: dead without
        // the raise, alive with it
        var raisedOutput : string
        let raisedExit = run_annotation_fixture("_fixture_raised_timeout.das", raisedOutput,
                                                ["--isolated-mode", "--isolated-mode-threads", "1",
                                                 "--test-timeout", "0.05"])
        t |> equal(raisedExit, 0, "the raised budget should keep the file alive:\n{raisedOutput}")
        t |> success(raisedOutput |> contains("2 tests, 2 passed"),
                     "expected the pass, got:\n{raisedOutput}")
        var plainOutput : string
        let plainExit = run_annotation_fixture("_fixture_annotation_args.das", plainOutput,
                                               ["--isolated-mode", "--isolated-mode-threads", "1",
                                                "--test-timeout", "0.001"])
        t |> success(plainExit != 0, "a file whose declared timeouts add almost nothing " +
            "is held to the per-file budget:\n{plainOutput}")
        t |> success(plainOutput |> contains("timed out"),
                     "expected the timeout verdict, got:\n{plainOutput}")
    }

    tt |> run("a timeout spelled as a string is a compile error naming the argument") <| @(t : T?) {
        var badOutput : string
        let badExit = run_annotation_fixture("_fixture_bad_annotation_arg.das", badOutput)
        t |> success(badExit != 0, "expected a non-zero exit, got {badExit}\n{badOutput}")
        t |> success(badOutput |> contains("[test(timeout = ...)] takes seconds"),
                     "expected the macro's message, got:\n{badOutput}")
    }

    tt |> run("a description that is not a string, and an argument [test] does not take, " +
        "are compile errors") <| @(t : T?) {
        var kindOutput : string
        let kindExit = run_annotation_fixture("_fixture_bad_annotation_kind.das", kindOutput)
        t |> success(kindExit != 0, "expected a non-zero exit, got {kindExit}\n{kindOutput}")
        t |> success(kindOutput |> contains("[test(description = ...)] takes a string"),
                     "expected the macro's message, got:\n{kindOutput}")
        var unknownOutput : string
        let unknownExit = run_annotation_fixture("_fixture_unknown_annotation_arg.das",
                                                 unknownOutput)
        t |> success(unknownExit != 0,
                     "expected a non-zero exit, got {unknownExit}\n{unknownOutput}")
        t |> success(unknownOutput |> contains("does not take an argument named 'slow'"),
                     "expected the macro's message, got:\n{unknownOutput}")
    }
}

def batch_text(batches : array<array<int>>) : string =>
    join([for (b in batches); join([for (i in b); "{i}"], ",")], " | ")

[test(description = "isolated mode starts every file that declares a test timeout alone and first, the largest budget ahead, and cuts the rest into batches in collection order")]
def test_isolated_batches(t : T?) {
    t |> equal(batch_text(isolated_batches([0.0, 0.0, 0.0, 0.0, 0.0], 2)), "0,1 | 2,3 | 4",
        "no declared budget: collection order, the last batch short")
    let mixed <- isolated_batches([0.0, 60.0, 0.0, 1200.0, 0.0, 60.0, 0.0], 3)
    t |> equal(batch_text(mixed), "3 | 1 | 5 | 0,2,4 | 6",
        "declared files lead one by one, the largest first, equal budgets in collection order")
    t |> equal(batch_text(isolated_batches([5.0, 0.0], 1)), "0 | 1", "batch 1: one file a worker")
    let none : array<float>
    t |> equal(length(isolated_batches(none, 4)), 0, "no files, no batches")
}
