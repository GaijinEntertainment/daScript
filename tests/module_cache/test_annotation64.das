options gen2
options indenting = 4
require _mc_common
require daslib/fio
require strings
require daslib/strings_boost
[test]
def annotation64_cache_roundtrip(t : T?) {
    var error : string
    let tmp = create_temp_directory("das_annotation64", error)
    if (empty(tmp)) { t |> failure(error); return }
    let root = get_das_root()
    let args <- [das_exe(), "-dasroot", root, "-module-cache", "{tmp}/wide.bin", "{root}/tests/module_cache/_fixtures/mc_annotation64.das"]
    var cold, warm : string
    let cold_code = run_and_capture(args, cold)
    t |> success(report_child(t, "cold", cold_code, cold, "wide annotation cache verified"))
    t |> success(contains(cold, "ser: wrote"))
    let warm_code = run_and_capture(args, warm)
    t |> success(report_child(t, "warm", warm_code, warm, "wide annotation cache verified"))
    t |> success(contains(warm, "deser: clean"), "warm run must load the cache, not parse again")
    rmdir_rec(tmp, error)
}
