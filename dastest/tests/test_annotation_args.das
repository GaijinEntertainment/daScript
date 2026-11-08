options gen2

require dastest/testing_boost public
require daslib/fio
require daslib/strings_boost
require strings

def private run_dastest(fixture : string; var output : string&; extra : array<string>) : int {
    let exe = get_command_line_arguments()[0]
    let root = get_das_root()
    var cmd <- [exe, "-dasroot", root, "{root}/dastest/dastest.das", "--", "--test",
                "{root}/dastest/tests/{fixture}"]
    cmd |> push_from(extra)
    return run_and_capture(cmd, output)
}

def private run_dastest(fixture : string; var output : string&) : int {
    let none : array<string>
    return run_dastest(fixture, output, none)
}

[test(description = "[test] carries description and timeout: the claim prints under a FAIL only, an overrun test fails, a mistyped argument does not compile")]
def test_annotation_args(tt : T?) {
    var output : string
    let exit_code = run_dastest("_fixture_annotation_args.das", output)

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
        let raisedExit = run_dastest("_fixture_raised_timeout.das", raisedOutput,
                                     ["--isolated-mode", "--isolated-mode-threads", "1",
                                      "--test-timeout", "0.05"])
        t |> equal(raisedExit, 0, "the raised budget should keep the file alive:\n{raisedOutput}")
        t |> success(raisedOutput |> contains("2 tests, 2 passed"),
                     "expected the pass, got:\n{raisedOutput}")
        var plainOutput : string
        let plainExit = run_dastest("_fixture_annotation_args.das", plainOutput,
                                    ["--isolated-mode", "--isolated-mode-threads", "1",
                                     "--test-timeout", "0.001"])
        t |> success(plainExit != 0, "a file whose declared timeouts add almost nothing " +
            "is held to the per-file budget:\n{plainOutput}")
        t |> success(plainOutput |> contains("timed out"),
                     "expected the timeout verdict, got:\n{plainOutput}")
    }

    tt |> run("a timeout spelled as a string is a compile error naming the argument") <| @(t : T?) {
        var badOutput : string
        let badExit = run_dastest("_fixture_bad_annotation_arg.das", badOutput)
        t |> success(badExit != 0, "expected a non-zero exit, got {badExit}\n{badOutput}")
        t |> success(badOutput |> contains("[test(timeout = ...)] takes seconds"),
                     "expected the macro's message, got:\n{badOutput}")
    }

    tt |> run("a description that is not a string, and an argument [test] does not take, " +
        "are compile errors") <| @(t : T?) {
        var kindOutput : string
        let kindExit = run_dastest("_fixture_bad_annotation_kind.das", kindOutput)
        t |> success(kindExit != 0, "expected a non-zero exit, got {kindExit}\n{kindOutput}")
        t |> success(kindOutput |> contains("[test(description = ...)] takes a string"),
                     "expected the macro's message, got:\n{kindOutput}")
        var unknownOutput : string
        let unknownExit = run_dastest("_fixture_unknown_annotation_arg.das", unknownOutput)
        t |> success(unknownExit != 0,
                     "expected a non-zero exit, got {unknownExit}\n{unknownOutput}")
        t |> success(unknownOutput |> contains("does not take an argument named 'slow'"),
                     "expected the macro's message, got:\n{unknownOutput}")
    }
}
