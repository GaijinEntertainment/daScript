options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost public
require dastest/file_census
require dastest/folder_filter
require daslib/fio
require daslib/rtti
require daslib/strings_boost
require strings

let BUDGET_NS_LESS_TIMER_SLACK = 900_000_000l

let SUITE_TEXT = ("options gen2\noptions indenting = 4\noptions persistent_heap, " +
    "no_aot\n//! the header line\nrequire dastest/testing_boost public\n\n// a comment " +
        "holding [test] does not count\n[test(description = \"first claim\", timeout = " +
            "2.5)]\ndef test_first(t : T?) \{\n    t |> run(\"arm one\") <| @(t : T?) \{\n" +
            "        t |> equal(1, 1)\n    \}\n    t |> run(\"arm two\") <| @(t : T?) \{\n" +
            "        t |> equal(2, 2)\n    \}\n\}\n\n" +
                "[test, no_jit]\ndef test_second(t : T?) \{\n\}\n")

var g_stats = BenchmarkRunStats()
var g_starts = 0
var g_sink = 0

def dastest_root() : string => "{get_das_root()}/dastest"

def fixture_dir() : string => "{dastest_root()}/tests/_isolated_fixture"

def fixture_dir_pre_dashdash() : string => "{dastest_root()}/tests/_isolated_fixture_pre_dashdash"

def fixture_dir_crash() : string => "{dastest_root()}/tests/_isolated_fixture_crash"

def fixture_dir_panic() : string => "{dastest_root()}/tests/_isolated_fixture_panic"

def run_dastest(cmd : string; var output : string&; var exit_code : int&) {
    output = ""
    unsafe {    // nolint:STYLE025 - popen takes a trailing block
        exit_code = popen(cmd) $(f) {
            if (f != null) {
                while (!feof(f)) {
                    output += fgets(f)
                }
            }
        }
    }
}

def make_bench(name : string) : B? {
    var b = new B(name = name)
    b.on_start <- @(sub_name : string) {
        g_starts++
    }
    b.on_finished <- @(stats : BenchmarkRunStats) {
        g_stats = stats
    }
    b.on_fail <- @(now : bool) {}
    b.on_log <- @(msg : string; at : LineInfo) {}
    return b
}

[test(description = "the census reads a file's tests, arms, descriptions, timeouts, header and fence signature from its text, and classes suites, fixtures and scripts apart")]
def test_file_census(tt : T?) {  // nolint:STYLE038 - one arm per census fact
    tt |> run("a suite: two tests, two arms, one description, one timeout, a header, a " +
        "fence signature") <| @(t : T?) {
        let c <- census_text("fio/test_x.das", SUITE_TEXT)
        t |> equal(c.cls, TestFileClass.suite)
        t |> equal(c.folder, "fio")
        t |> equal(c.stem, "test_x")
        t |> equal(length(c.tests), 2)
        t |> equal(c.tests[0].funcName, "test_first")
        t |> equal(c.tests[0].description, "first claim")
        t |> equal(c.tests[0].timeoutSec, 2.5)
        t |> equal(c.tests[0].line, 8)
        t |> equal(c.tests[1].funcName, "test_second")
        t |> equal(c.tests[1].description, "")
        t |> equal(c.tests[0].standalone, "")
        let alone <- census_text("x.das", "[test(description = \"d\", standalone = " +
            "\"fills a registry\")]\ndef test_a(t : T?) \{\}\n")
        t |> equal(alone.tests[0].standalone, "fills a registry")
        t |> equal(c.arms, 2)
        t |> equal(c.header, "the header line")
        t |> equal(fence_signature(c), "no_aot,persistent_heap")
        t |> success(c.noAot)
        let moduleOption <- census_text("x.das",
            "options _flatten_expand_lerp\noptions _nolint\noptions _function_length = 0\n" +
            "[test]\ndef test_a(t : T?) \{\}\n")
        t |> equal(fence_signature(moduleOption), "_flatten_expand_lerp")
        let valued <- census_text("x.das", "options auto_inline_functions = " +
            "false\noptions stack = 65536\noptions rtti = true\n[test]\ndef test_a(t : T?) \{\}\n")
        t |> equal(fence_signature(valued), "auto_inline_functions=false,rtti,stack=65536")
        let gen1 <- census_text("x.das",
                                "options gen2 = false\n[test]\ndef test_a(t : T?)\n    pass\n")
        t |> equal(fence_signature(gen1), "gen2=false")
        t |> equal(length(c.unparsedAnnotationLines), 0)
    }

    tt |> run("an expect directive or a failed_ name makes a fixture; no test and no " +
        "expect makes a script") <| @(t : T?) {
        let byExpect <- census_text("language/x.das", "expect 30101\ndef main() \{\}\n")
        t |> equal(byExpect.cls, TestFileClass.fixture)
        let byName <- census_text("language/failed_x.das", "def main() \{\}\n")
        t |> equal(byName.cls, TestFileClass.fixture)
        let script <- census_text("language/plain.das", "options gen2\n[export]\ndef main() \{\}\n")
        t |> equal(script.cls, TestFileClass.script)
    }

    tt |> run("comments are stripped, spellings and escapes are read, options are " +
        "deduplicated, bare run calls count") <| @(t : T?) {
        let c <- census_text("x.das", "options rtti // why\noptions " +
            "rtti\n[test(description = \"says \\\"quoted\\\" words\")] // trailing " +
                "note\ndef test_a(t : T?) \{\n    run(t, \"bare arm\")\n\}\n[no_jit, " +
                    "test]\ndef test_b(t : T?) \{\n\}\n" +
                    "[test(description = \"a // marker and a url http://x/y stay text\")]\n" +
                    "def test_c(t : T?) \{\n\}\n")
        t |> equal(fence_signature(c), "rtti")
        t |> equal(length(c.tests), 3)
        t |> equal(c.tests[0].description, "says \\\"quoted\\\" words")
        t |> equal(length(c.unparsedAnnotationLines), 0)
        t |> equal(c.tests[1].funcName, "test_b")
        t |> equal(c.tests[2].description, "a // marker and a url http://x/y stay text")
        t |> equal(c.arms, 1)
        let cant <- census_text("language/cant_x.das", "def main() \{\}\n")
        t |> equal(cant.cls, TestFileClass.fixture)
        let invalid <- census_text("language/invalid_x.das", "def main() \{\}\n")
        t |> equal(invalid.cls, TestFileClass.fixture)
    }

    tt |> run("a [test( annotation that does not close on its line is recorded as " +
        "unparsed") <| @(t : T?) {
        let c <- census_text("x.das", "[test(description = \"spills\",\n    timeout = " +
            "3)]\ndef test_a(t : T?) \{\}\n")
        t |> equal(length(c.unparsedAnnotationLines), 1)
        t |> equal(c.unparsedAnnotationLines[0], 1)
        t |> equal(c.folder, ".")
    }

    tt |> run("dastest's run rule: an underscore or dot on any path component excludes " +
        "the file, and so does the gate's name") <| @(t : T?) {
        t |> success(is_run_file("fio/test_x.das"))
        t |> success(!is_run_file("fio/_fixture.das"))
        t |> success(!is_run_file("_fixtures/x.das"))
        t |> success(!is_run_file("fio/.hidden.das"))
        t |> success(!is_run_file("fio/notes.md"))
        t |> success(!is_run_file("REVIEW.das"))
    }

    tt |> run("the header is the first //! line of the leading block; a //! after the " +
        "first declaration is not a header") <| @(t : T?) {
        let late <- census_text("x.das", "options gen2\n[test]\ndef test_a(t : T?) \{\n   " +
            " //! a doc inside the body\n\}\n")
        t |> equal(late.header, "")
        let early <- census_text("x.das", "options gen2\nrequire strings\n//! the " +
            "claim\nexpect 30101\ndef main() \{\}\n")
        t |> equal(early.header, "the claim")
        let wrapped <- census_text("x.das", "//! a claim that\n//! wraps\n\n//! a second " +
            "block is not the header\ndef main() \{\}\n")
        t |> equal(wrapped.header, "a claim that wraps")
    }
}

[test(description = "with_folder_filter answers the root's .das_test per folder name, admits every folder where there is none, and refuses a .das_test that does not compile")]
def test_with_folder_filter(t : T?) {
    let tmp = create_temp_directory_result("folder_filter")
    if (!(tmp is value)) {
        t |> failure("could not create temp directory: {tmp as error}")
        return
    }
    let root = tmp as value
    let gated = path_join(root, "gated")
    let broken = path_join(root, "broken")
    mkdir_rec(gated)
    mkdir_rec(broken)
    t |> success(fwrite(path_join(gated, ".das_test"), "options gen2\n[export, pinvoke]\n" +
        "def can_visit_folder(folder_name : string; var result : bool?) \{\n" +
            "    *result = folder_name != \"skipped\"\n\}\n"), "the gating .das_test is written")
    t |> success(fwrite(path_join(broken, ".das_test"), "this is not a daslang program\n"),
        "the broken .das_test is written")
    t |> run("a .das_test at the root decides per folder name") @(t : T?) {
        let answered = with_folder_filter(gated) $(can_visit) {
            t |> success(can_visit |> invoke("kept"), "a folder the filter admits")
            t |> success(!(can_visit |> invoke("skipped")), "a folder the filter declines")
            return true
        }
        t |> success(answered, "the block's answer comes back")
    }
    t |> run("no .das_test admits every folder") @(t : T?) {
        let answered = with_folder_filter(root) $(can_visit) {
            return can_visit |> invoke("skipped")
        }
        t |> success(answered, "every folder is visitable")
    }
    t |> run("the block's own false comes back through a gated root") @(t : T?) {
        let answered = with_folder_filter(gated) $(_can_visit) {
            return false
        }
        t |> success(!answered, "a scan the block reports as failed fails the walk")
    }
    t |> run("a .das_test that does not compile answers false and runs no block") @(t : T?) {
        var ran = false
        let answered = with_folder_filter(broken) $(_can_visit) {
            ran = true
            return true
        }
        t |> success(!answered && !ran, "the walk is refused, not run unfiltered")
    }
    t |> success(remove(path_join(gated, ".das_test")) && remove(path_join(broken, ".das_test")),
        "the two .das_test files are removed")
    t |> success(rmdir(gated) && rmdir(broken) && rmdir(root), "the temp tree is removed")
}

[test(description = "a benchmark reports the batch that met the time budget, for both a fast op and a slow one")]
def test_reported_batch_meets_the_budget(t : T?) {
    t |> run("a fast op grows to a batch spanning the budget less timer slack") @@(t : T?) {
        var b = make_bench("budget")
        g_starts = 0
        b |> run("fast") {
            g_sink++
        }
        t |> equal(g_starts, 1, "the name is announced once")
        t |> success(g_stats.n > 1, "grew past the calibration call: n = {g_stats.n}")
        t |> success(g_stats.n < 1000 || g_stats.n % 10 == 0, "n = {g_stats.n} is a whole number of unrolled groups")
        t |> success(g_stats.time_ns >= BUDGET_NS_LESS_TIMER_SLACK, "reported batch spans {g_stats.time_ns} ns, needs >= {BUDGET_NS_LESS_TIMER_SLACK}")
        unsafe {
            delete b
        }
    }
    t |> run("a slow op is reported from the batch that met the budget") @@(t : T?) {
        var b = make_bench("budget")
        b |> run("slow") {
            var acc = 0
            for (i in range(20_000_000)) {
                acc += i & 7
            }
            g_sink += acc
        }
        t |> success(g_stats.time_ns >= BUDGET_NS_LESS_TIMER_SLACK, "reported batch spans {g_stats.time_ns} ns over n = {g_stats.n}, needs >= {BUDGET_NS_LESS_TIMER_SLACK}")
        unsafe {
            delete b
        }
    }
    feint("{g_sink}")
}

[test(description = "isolated mode runs a suite in workers, keeps pre-`--` flags, folds crash and panic diagnostics into the failing test's log, and honors --failures-only", timeout = 1200)]
def test_isolated_mode(tt : T?) {  // nolint:STYLE038 - one isolated sweep checked arm by arm
    tt |> run("isolated mode spawns 4 workers, fixture suite all passes, --worker-index round-trips") <| @(t : T?) {
        let exe = get_command_line_arguments()[0]
        let cmd = "{exe} {dastest_root()}/dastest.das -- --test {fixture_dir()} --isolated-mode --isolated-mode-threads 4"
        var output : string
        var exit_code : int
        run_dastest(cmd, output, exit_code)
        t |> equal(exit_code, 0)
        t |> success(output |> contains("SUCCESS"),
                     "expected SUCCESS in dastest output, got:\n{output}")
        t |> success(!(output |> contains("subprocess stderr")),
            "a passing isolated run must not emit the crashed-stderr header")
    }

    tt |> run("isolated mode preserves pre-`--` daslang flags (-dasroot) in singleTest cmd") <| @(t : T?) {
        // Pass `-dasroot {existing-dasroot}` — a no-op pre-`--` flag — to the
        // outer dastest. Pre-fix, dastest's singleTest reconstruction used
        // `raw_args[0] raw_args[1]` which dropped everything between exe and
        // dastest.das. The fixture test under fixture_dir_pre_dashdash()
        // asserts -dasroot survives into the spawned subprocess's argv.
        let exe = get_command_line_arguments()[0]
        let cmd = "{exe} -dasroot {get_das_root()} {dastest_root()}/dastest.das -- --test {fixture_dir_pre_dashdash()} --isolated-mode --isolated-mode-threads 1"
        var output : string
        var exit_code : int
        run_dastest(cmd, output, exit_code)
        t |> equal(exit_code, 0)
        t |> success(output |> contains("SUCCESS"),
                     "expected SUCCESS in dastest output, got:\n{output}")
    }

    tt |> run("isolated mode folds a crashed worker's stderr (crash-handler stack) into the failing test's log") <| @(t : T?) {
        // The fixture hard-crashes, so the spawned dastest reports a failure
        // (non-zero exit) — the point is that the crash diagnostic is
        // surfaced under the failing test rather than dropped with the
        // worker's stderr.
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

    tt |> run("--stack-on-exception folds an at-throw call stack into a panicking worker's log") <| @(t : T?) {
        // WITH the flag: the worker sets the test context's
        // alwaysStackWalkOnException, so the panic walks the stack at throw
        // (frames intact) to the worker's stderr, which the isolated runner
        // folds into the failing test's log. The recursive fixture means the
        // deep_panic call chain shows up in the walk.
        let exe = get_command_line_arguments()[0]
        let cmd = "{exe} {dastest_root()}/dastest.das -- --test {fixture_dir_panic()} --isolated-mode --isolated-mode-threads 1 --stack-on-exception"
        var output : string
        var exit_code : int
        run_dastest(cmd, output, exit_code)
        // Positive proof the fixture actually ran and panicked (so an absent
        // CALL STACK below is meaningful, not a subprocess that never ran).
        t |> success(exit_code != 0,
                     "expected non-zero exit from the panicking fixture, got {exit_code}\n{output}")
        t |> success(output |> contains("intentional panic"),
            "expected the fixture's panic message in output, got:\n{output}")
        t |> success(output |> contains("CALL STACK"),
            "expected the at-throw CALL STACK in output, got:\n{output}")
        t |> success(output |> contains("deep_panic"),
            "expected the panicking call chain (deep_panic) in the walk, got:\n{output}")

        // WITHOUT the flag: the fixture still panics (non-zero exit +
        // message), but no stack walk is emitted.
        let cmd2 = "{exe} {dastest_root()}/dastest.das -- --test {fixture_dir_panic()} --isolated-mode --isolated-mode-threads 1"
        var output2 : string
        var exit_code2 : int
        run_dastest(cmd2, output2, exit_code2)
        t |> success(exit_code2 != 0, "expected non-zero exit from the panicking fixture, got {exit_code2}\n{output2}")
        t |> success(output2 |> contains("intentional panic"),
            "expected the fixture's panic message in output, got:\n{output2}")
        t |> success(!(output2 |> contains("CALL STACK")),
            "without --stack-on-exception there must be no stack walk, got:\n{output2}")
    }

    tt |> run("--run worker path honors --failures-only (PASS lines suppressed at the source)") <| @(t : T?) {
        // The worker (--run) path used to return before log::failuresOnly was
        // set, AND the parent never forwarded the flag — so every per-test
        // RUN/PASS line of every passing file streamed through the captured
        // pipe. The JSON report must keep streaming regardless (it is the
        // parent's IPC, emitted raw).
        let exe = get_command_line_arguments()[0]
        let file = "{fixture_dir()}/test_one.das"
        var output : string
        var exit_code : int
        run_dastest("{exe} {dastest_root()}/dastest.das -- --run {file} --failures-only", output,
                    exit_code)
        t |> equal(exit_code, 0)
        t |> success(output |> contains("##dastest##"),
            "JSON report must stream regardless of --failures-only, got:\n{output}")
        t |> success(!(output |> contains("--- PASS")),
            "worker must suppress per-test PASS lines under --failures-only, got:\n{output}")

        // Control: the same invocation WITHOUT the flag does print them
        // (proves the assertion above is meaningful, not a fixture that never
        // logs).
        run_dastest("{exe} {dastest_root()}/dastest.das -- --run {file}", output, exit_code)
        t |> equal(exit_code, 0)
        t |> success(output |> contains("--- PASS"),
            "control run without --failures-only should print PASS lines, got:\n{output}")
    }
}
