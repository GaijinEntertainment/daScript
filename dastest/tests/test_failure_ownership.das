options gen2

require dastest/testing_boost public
require daslib/clargs
require daslib/fio
require daslib/json_boost
require strings

//! Isolate intentional failures and allocator crashes in a child.
[test]
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
