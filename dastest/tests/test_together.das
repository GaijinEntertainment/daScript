options gen2

require dastest/testing_boost
require dastest/suite_result
require daslib/json
require daslib/json_boost
require daslib/strings_boost
require daslib/fio

// --together: the listed fixtures compiled as one program, each still
// reporting as its own file

struct JsonReport {
    total : int
    passed : int
    failed : int
    errors : int
    skipped : int
    tests : array<SuiteTestResult>
}

def run_together(fixtures : array<string>;
                 var output : string&) : tuple<exit_code : int; report : JsonReport> {
    let das_root = get_das_root()
    let json_file = path_join(test_temp_dir(), "together_{join(fixtures, "_")}.json")
    var args <- [get_command_line_arguments()[0], "-dasroot", das_root,
        "{das_root}/dastest/dastest.das", "--"]
    args |> reserve(length(args) + 2 * length(fixtures) + 4)
    for (f in fixtures) {
        args |> push("--together")
        args |> push("{das_root}/dastest/tests/_fixture_{f}.das")
    }
    // the child's generated root and its AST record land in this test's
    // own temp folder, never the system one
    args |> push("--module-cache-dir")
    args |> push(test_temp_dir())
    args |> push("--json-file")
    args |> push(json_file)
    let exit_code = run_and_capture(args, output)
    var json_text = ""
    fopen(json_file, "rb") $(f) {
        if (f != null) {
            json_text = fread(f)
        }
    }
    remove(json_file)
    var error = ""
    let json = read_json(json_text, error)
    if (json == null) {
        to_log(LOG_ERROR, "Command output ({exit_code}):\n\n{output}\n")
        panic("Failed to parse JSON from {json_file}: {error}")
    }
    return <- (exit_code = exit_code, report <- from_JV(json, type<JsonReport>))
}

def has_test(report : JsonReport; name : string) : bool {
    for (tr in report.tests) {
        return true if (tr.name == name)
    }
    return false
}

[test(description = "--together compiles the listed fixtures as one program yet reports each file's tests, results and output lines separately", timeout = 1200)]
def test_together_runs_every_file(tt : T?) {
    var output : string
    let r <- run_together(["passing", "skipping", "failing"], output)
    tt |> equal(r.report.total, 7,
        "the three fixtures' tests: passing's two and its sub-run, two each after")
    tt |> equal(r.report.passed, 5, "passing's three, skipping's one, failing's one")
    tt |> equal(r.report.failed, 1, "failing's failing test")
    tt |> equal(r.report.skipped, 1, "skipping's skipped test")
    tt |> equal(r.report.errors, 0, "no compile or run error")
    tt |> success(r.exit_code != 0, "a failing test fails the run: exit {r.exit_code}")
    tt |> success(has_test(r.report, "test_pass_one") && has_test(r.report, "test_skipped")
        && has_test(r.report, "test_will_fail"), "every file's tests are in the report")
    tt |> success(find(output, "PASS ") >= 0 && find(output, "_fixture_passing.das") >= 0,
        "the passing file gets its PASS line")
    tt |> success(find(output, "FAIL ") >= 0 && find(output, "_fixture_failing.das") >= 0,
        "the failing file gets its FAIL line")
    tt |> success(find(output, "_fixture_skipping.das") >= 0, "the skipping file gets its line too")
}

[test(description = "--together runs each file in a fresh context, so one file's write to a shared global never reaches the next", timeout = 1200)]
def test_together_gives_each_file_a_fresh_context(tt : T?) {
    var output : string
    let r <- run_together(["state_a", "state_b"], output)
    tt |> equal(r.report.total, 2, "the two fixtures' tests")
    tt |> equal(r.report.passed, 2,
        "each file reads the shared global fresh: the first file's write died with its context")
    tt |> equal(r.exit_code, 0, "a clean run:\n{output}")
}

[test(description = "--together simulates its program once: a file whose [init] panics fails every file of the batch with one error each and runs no test", timeout = 1200)]
def test_together_fails_every_file_when_the_simulate_fails(tt : T?) {
    var output : string
    let r <- run_together(["init_panics", "passing"], output)
    tt |> success(r.exit_code != 0, "a failed simulate fails the run:\n{output}")
    tt |> equal(r.report.errors, 2, "each file of the batch is one error")
    tt |> equal(r.report.passed, 0, "no test ran")
    tt |> success(find(output, "Failed to simulate the together root") >= 0,
        "the failure is the root's simulate:\n{output}")
}

[test(description = "under --together a test past --test-wall ends the process with exit code 125 and a line naming the test's own file of the batch", timeout = 1200)]
def test_together_wall_names_the_file(tt : T?) {
    let das_root = get_das_root()
    var output : string
    let exit_code = run_and_capture([get_command_line_arguments()[0], "-dasroot", das_root,
        "{das_root}/dastest/dastest.das", "--",
        "--together", "{das_root}/dastest/tests/_fixture_passing.das",
        "--together", "{das_root}/dastest/tests/_timeout_fixture/test_sleep.das",
        "--module-cache-dir", test_temp_dir(), "--test-wall", "1"], output, 120.0)
    tt |> equal(exit_code, 125, "the wall's exit code:\n{output}")
    tt |> success(find(output, "kill switch: test 'test_sleep' of") >= 0
        && find(output, "test_sleep.das (--test-wall) ran past 1s") >= 0,
        "the wall names the batch's file the test is in:\n{output}")
}

[test(description = "--together refuses a batch whose files disagree on options, reporting one error per file and naming both values", timeout = 1200)]
def test_together_refuses_disagreeing_options(tt : T?) {
    var output : string
    let r <- run_together(["opt_a", "opt_b"], output)
    tt |> equal(r.report.errors, 2, "each file of a refused batch is one error")
    tt |> equal(r.report.passed, 0, "nothing ran")
    tt |> success(r.exit_code != 0, "the refusal fails the run")
    tt |> success(find(output, "options disagree") >= 0,
        "the refusal names the disagreement: {output}")
    tt |> success(find(output, "options stack = 2048") >= 0
        && find(output, "options stack = 1024") >= 0, "both values are named")
}
