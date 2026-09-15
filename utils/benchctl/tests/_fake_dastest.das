options gen2

require daslib/clargs
require daslib/fio
require daslib/json_boost
require strings

[CommandLineArgs]
struct FakeArgs {
    @clarg_doc = "where to write the dastest report"
    json_file : string
    @clarg_doc = "the benchmark file under test"
    test : string
    @clarg_doc = "repeats"
    count : int
    @clarg_doc = "measure"
    bench : bool
    @clarg_doc = "output format"
    bench_format : string
    @clarg_doc = "behaviour: ok | hang | fail | no_report"
    use_aot : bool
}

def private mode(test_file : string) : string {
    let m = strip(fread(path_join(dir_name(dir_name(test_file)), "fake_mode.txt")))
    return empty(m) ? "ok" : m
}

def private stats(name, sub : string; n : int; time_ns : int64) : string {
    return write_json_compact(JV((name = name, sub_name = sub, n = n, time_ns = time_ns,
        allocs = 0, heap_bytes = 0, string_allocs = 0, string_heap_bytes = 0)))
}

[export]
def main : int {
    var r <- parse_args(type<FakeArgs>)
    return 1 if (r |> is_err)
    let args <- r |> move_unwrap
    let how = mode(args.test)
    print("lane={jit_enabled() ? "jit" : "interp"}\n")
    print("{stats("b", "arm", 10, 1000l)}\n")
    if (how == "hang") {
        while (true) {
            sleep(200u)
        }
    }
    if (how == "fail") {
        print("--- FAIL 'b' in '{args.test}'\n")
        fwrite(args.json_file, write_json_compact(JV((success = false, tests = JV([JV((name = "b", passed = false))])))))
        return 1
    }
    if (how != "no_report") {
        fwrite(args.json_file, write_json_compact(JV((success = true, tests = JV([JV((name = "b", passed = true))])))))
    }
    return 0
}
