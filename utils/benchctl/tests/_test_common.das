options gen2
module _test_common public

require dastest/testing_boost
require daslib/fio
require daslib/json_boost

def with_temp_dir(prefix : string; blk : block<(dir : string) : void>) {
    var err = ""
    let dir = create_temp_directory(prefix, err)
    verify(!empty(dir), "temp dir created")
    invoke(blk, dir)
    rmdir_rec(dir)
}

def fake_repo(root, mode : string) {
    mkdir_rec(path_join(root, "dastest"))
    mkdir_rec(path_join(root, "benchmarks/x"))
    fwrite(path_join(root, "dastest/dastest.das"), fread(path_join(get_das_root(), "utils/benchctl/tests/_fake_dastest.das")))
    fwrite(path_join(root, "benchmarks/x/y.das"), "options gen2\n")
    fwrite(path_join(root, "benchmarks/fake_mode.txt"), mode)
}

def report_json(names, failed : array<string>) : string {
    var tests <- [for (n in names); JV((name = n, passed = find_index(failed, n) < 0))]
    return write_json_compact(JV((success = empty(failed), tests = JV(tests))))
}

def stats_line(name, sub : string; n : int; time_ns : int64; allocs : int = 0) : string {
    return write_json_compact(JV((name = name, sub_name = sub, n = n, time_ns = time_ns,
        allocs = allocs, heap_bytes = allocs * 16, string_allocs = 0, string_heap_bytes = 0)))
}
