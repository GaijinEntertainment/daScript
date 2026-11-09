options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost public
require daslib/clargs
require daslib/fio
require daslib/json_boost
require daslib/strings_boost
require strings

def dastest_root() : string => "{get_das_root()}/dastest"

def fixture_path(name : string) : string => "{dastest_root()}/tests/{name}"

def json_out_path() : string => "{get_das_root()}/.dastest_json_test.json"

def run_dastest_json(test_file : string; var json_content : string&; var exit_code : int&) {
    let exe = get_command_line_arguments()[0]
    let json_path = json_out_path()
    let cmd = "{exe} {dastest_root()}/dastest.das -- --test {test_file} --json-file {json_path}"
    var output : string
    unsafe {    // nolint:STYLE025 - popen takes a trailing block
        exit_code = popen(cmd) $(f) {
            if (f != null) {
                while (!feof(f)) {
                    output += fgets(f)
                }
            }
        }
    }
    json_content = ""
    fopen(json_path, "rb") $(f) {
        if (f != null) {
            json_content = fread(f)
        }
    }
    remove(json_path)
}

def private run_dastest_color(cmd : string; var output : string&; var exit_code : int&) {
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

[test(description = "--json-file writes a valid report whose required fields, counts, per-test entries and timings match the passing fixture", timeout = 1200)]
def test_json_file_passing(tt : T?) {  // nolint:STYLE038 - one JSON report checked field by field
    tt |> run("produces valid JSON for passing tests") <| @(t : T?) {
        var json_content : string
        var exit_code : int
        run_dastest_json(fixture_path("_fixture_passing.das"), json_content, exit_code)
        t |> equal(exit_code, 0)
        t |> success(!empty(json_content), "JSON file should not be empty")

        var error : string
        var js <- read_json(json_content, error)
        t |> success(js != null, "JSON parse failed: {error}")
        if (js == null) {
            return
        }

        // Check all required fields exist
        t |> success(js?.file != null, "should have 'file' field")
        t |> success(js?.total != null, "should have 'total' field")
        t |> success(js?.passed != null, "should have 'passed' field")
        t |> success(js?.failed != null, "should have 'failed' field")
        t |> success(js?.errors != null, "should have 'errors' field")
        t |> success(js?.skipped != null, "should have 'skipped' field")
        t |> success(js?.success != null, "should have 'success' field")
        t |> success(js?.time_usec != null, "should have 'time_usec' field")
        t |> success(js?.tests != null, "should have 'tests' field")

        // Check values
        let success_val = js?.success ?? false
        t |> success(success_val, "success should be true")
        let total = js?.total ?? -1
        let passed_count = js?.passed ?? -1
        let failed_count = js?.failed ?? -1
        let errors_count = js?.errors ?? -1
        t |> equal(total, 3)  // test_pass_one has sub_a, test_pass_two = 3 tests total
        t |> equal(passed_count, 3)
        t |> equal(failed_count, 0)
        t |> equal(errors_count, 0)
    }

    tt |> run("individual test results have correct fields") <| @(t : T?) {
        var json_content : string
        var exit_code : int
        run_dastest_json(fixture_path("_fixture_passing.das"), json_content, exit_code)
        if (exit_code != 0 || empty(json_content)) {
            t |> failure("dastest did not produce JSON output")
            return
        }

        var error : string
        var js <- read_json(json_content, error)
        if (js == null) {
            t |> failure("JSON parse failed: {error}")
            return
        }

        let tests_jv = js?.tests
        t |> success(tests_jv != null, "tests field should exist")
        if (tests_jv == null) {
            return
        }

        let tests & = unsafe(tests_jv.value as _array)
        t |> equal(length(tests), 3)
        if (empty(tests)) {
            return
        }

        // Check first test entry has required fields
        let first = tests[0]
        t |> success(first?.name != null, "test entry should have 'name'")
        t |> success(first?.passed != null, "test entry should have 'passed'")
        t |> success(first?.skipped != null, "test entry should have 'skipped'")
        t |> success(first?.time != null, "test entry should have 'time' (renamed from time_usec)")

        // All tests should pass
        for (test_jv in tests) {
            let test_passed = test_jv?.passed ?? false
            let test_name = test_jv?.name ?? "unknown"
            t |> success(test_passed, "test '{test_name}' should have passed")
        }
    }

    tt |> run("time_usec is positive") <| @(t : T?) {
        var json_content : string
        var exit_code : int
        run_dastest_json(fixture_path("_fixture_passing.das"), json_content, exit_code)
        if (exit_code != 0 || empty(json_content)) {
            t |> failure("dastest did not produce JSON output")
            return
        }

        var error : string
        var js <- read_json(json_content, error)
        if (js == null) {
            t |> failure("JSON parse failed")
            return
        }

        let time_usec = (js?.time_usec ?? 0)
        t |> success(time_usec > 0, "time_usec should be positive")

        let tests_jv = js?.tests
        if (tests_jv == null) {
            return
        }
        let tests & = unsafe(tests_jv.value as _array)
        for (test_jv in tests) {
            let test_time = (test_jv?.time ?? 0)
            let test_name = test_jv?.name ?? "unknown"
            t |> success(test_time >= 0, "test '{test_name}' time should be >= 0")
        }
    }
}

[test(description = "a run without --json-file leaves no JSON report file behind", timeout = 1200)]
def test_json_file_not_created_without_flag(tt : T?) {
    tt |> run("no JSON file without --json-file flag") <| @(t : T?) {
        let json_path = json_out_path()
        remove(json_path)
        let exe = get_command_line_arguments()[0]
        let fix_path = fixture_path("_fixture_passing.das")
        let cmd = "{exe} {dastest_root()}/dastest.das -- --test {fix_path}"
        unsafe {    // nolint:STYLE025 - popen takes a trailing block
            popen(cmd) $(f) {
                if (f != null) {
                    while (!feof(f)) {
                        fgets(f)
                    }
                }
            }
        }
        let st = stat(json_path)
        t |> success(!st.is_reg, "JSON file should not exist without --json-file flag")
    }
}

// A colored log line must end "<text><reset>\n", never
// "<text>\n<reset>": a reset escape carried past the newline lands at
// the head of the NEXT stdout line and corrupts machine-read output
// (--bench-format json lines after a colored SUCCESS banner).
[test(description = "under --color no captured line starts with a reset escape, so a colored line never carries its newline inside the color wrap", timeout = 1200)]
def test_log_color_framing(tt : T?) {
    tt |> run("no captured line starts with a reset escape under --color") <| @(t : T?) {
        let exe = get_command_line_arguments()[0]
        let cmd = "{exe} {get_das_root()}/dastest/dastest.das -- --color --test {get_das_root()}/dastest/tests/_fixture_passing.das"
        var output : string
        var exit_code : int
        run_dastest_color(cmd, output, exit_code)
        t |> equal(exit_code, 0)
        let esc = "{to_char(27)}"
        t |> success(output |> contains("{esc}["),
                     "--color must force color escapes on, or this test checks nothing")
        var bled = 0
        for (line in split(output, "\n")) {
            if (starts_with(line, "{esc}[0m")) {
                bled++
            }
        }
        t |> equal(bled, 0, "lines beginning with the reset escape — a colored line carried its newline inside the color wrap")
    }
}

//! Isolate intentional failures and allocator crashes in a child.
[test(description = "failure reports written to the JSON file outlive the test heap: messages, locations and counts survive a collection and a crash", timeout = 1200)]
def failure_reports_outlive_the_test_heap(t : T?) {
    let report = path_join(test_temp_dir(), "failure.json")
    let arguments <- [get_host_binary(), "-dasroot", get_das_root(),
        "-no-module-cache", path_join(get_das_root(), "dastest/dastest.das"),
        "--", "--test", path_join(get_das_root(),
            "dastest/tests/_fixture_failure_gc.das"), "--json-file", report]
    var output : string
    let code = run_and_capture(arguments, output, 120.0)
    t |> equal(code, 4, output)
    var error : string
    var result = read_json(fread(report), error)
    t |> strictEqual(error, "", output)
    if (result == null) { return }
    t |> equal(result?.failed ?? -1, 3)
    t |> equal(result?.errors ?? -1, 1)
    t |> success(ends_with(result?.tests?[0]?.messages?[0] ?? "",
        ": ordinary log before collection"))
    let message = result?.tests?[0]?.messages?[1] ?? ""
    t |> success(ends_with(message, ": retained failure before collection"), message)
    t |> success(find(result?.tests?[1]?.messages?[0] ?? "", "fatal failure after collection") >= 0)
    t |> success(find(result?.tests?[2]?.messages?[0] ?? "", "runtime error after collection") >= 0)
    t |> equal(result?.tests?[3]?.name ?? "", "nested failure")
    t |> success(ends_with(result?.tests?[3]?.messages?[0] ?? "", ": nested retained failure"))
    for (i in range(4)) {
        let location = result?.tests?[i]?.location ?? ""
        t |> success(find(location, "_fixture_failure_gc.das:") >= 0, location)
        t |> success(starts_with(result?.tests?[i]?.messages?[0] ?? "", location + ": "))
        let entries = result?.tests?[i]?.messages
        t |> success(entries.value is _array)
        if (entries.value is _array) { t |> equal(length(entries.value as _array), i == 0 ? 2 : 1) }
    }
    delete_json(result)
}
