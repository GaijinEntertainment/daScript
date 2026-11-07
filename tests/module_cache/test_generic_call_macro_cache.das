options gen2
options indenting = 4

require dastest/testing_boost public

require daslib/fio
require _mc_common

//! the cold run writes the record, the warm run reparses the script
[test]
def test_a_same_module_call_macro_in_a_generic_body_writes_to_the_cache(t : T?) {
    var terr : string
    let tmp = create_temp_directory("das_mc_generic_call_macro", terr)
    if (empty(tmp)) {
        t |> failure("create_temp_directory: {terr}")
        return
    }
    let fixture = path_join(get_das_root(),
                            "tests/module_cache/_fixtures/mc_generic_call_macro.das")
    let cache = path_join(tmp, "mc.bin")
    let argv <- [das_exe(), "-dasroot", get_das_root(), "-module-cache", cache, fixture]
    var cold : string
    let coldRc = run_argv(argv, cold)
    var coldOk = report_child(t, "cold", coldRc, cold, "outer=42")
    coldOk = coldOk && report_child(t, "cold", coldRc, cold, "ser: wrote")
    t |> success(coldOk, "the cold run writes the cache and runs the program")
    var warm : string
    let warmRc = run_argv(argv, warm)
    var warmOk = report_child(t, "warm", warmRc, warm, "outer=42")
    warmOk = warmOk && report_child(t, "warm", warmRc, warm,
                                    "mc_generic_call_macro.das' (registers at compile time)")
    t |> success(warmOk, "the warm run reparses the script in place and runs it")
    var err : string
    rmdir_rec(tmp, err)
}

//! the marked module is a reference in the driver's record,
//! so the record is written
[test]
def test_a_script_that_requires_a_marked_module_writes_its_record(t : T?) {
    var terr : string
    let tmp = create_temp_directory("das_mc_gcm_drv", terr)
    if (empty(tmp)) {
        t |> failure("create_temp_directory: {terr}")
        return
    }
    let driver = path_join(get_das_root(), "tests/module_cache/_fixtures/mc_gcm_drv.das")
    let cache = path_join(tmp, "mc.bin")
    let argv <- [das_exe(), "-dasroot", get_das_root(), "-module-cache", cache, driver]
    var cold : string
    let coldRc = run_argv(argv, cold)
    var coldOk = report_child(t, "cold", coldRc, cold, "outer=42")
    coldOk = coldOk && report_child(t, "cold", coldRc, cold, "ser: wrote")
    t |> success(coldOk, "the cold run writes the record with the marked module as a reference")
    var warm : string
    let warmRc = run_argv(argv, warm)
    t |> success(report_child(t, "warm", warmRc, warm, "outer=42"), "the warm run serves and runs")
    var err : string
    rmdir_rec(tmp, err)
}
