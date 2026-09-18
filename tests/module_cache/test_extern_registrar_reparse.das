options gen2
options indenting = 4

require dastest/testing_boost public

require strings
require daslib/fio
require _mc_common

//! a module whose compile registered an extern proxy into dasbind is never served from the
//! cache: its record is skipped on the mark, without the payload read that used to fail on
//! dasbind's cumulative hash, and it reparses in place while the driver still serves
//! (src/ast/ARCHITECTURE.md#module-cache-read)
[test]
def test_extern_registrar_reparse(t : T?) {
    var terr : string
    let tmp = create_temp_directory("das_mc_extern", terr)
    if (empty(tmp)) {
        t |> failure("create_temp_directory: {terr}")
        return
    }
    let root = get_das_root()
    let fixtures = "{root}/tests/module_cache/_fixtures"
    for (name in ["mc_extern_lib.das", "mc_extern_drv.das"]) {
        fwrite(path_join(tmp, name), fread(path_join(fixtures, name)))
    }
    let drv = path_join(tmp, "mc_extern_drv.das")
    let cmd = "{trace_prefix()}\"{das_exe()}\" -dasroot \"{root}\" -module-cache \"{tmp}/mc.bin\" \"{drv}\""
    var cold : string
    let coldRc = run_child(cmd, cold)
    var coldOk = report_child(t, "cold", coldRc, cold, "ser: wrote")
    coldOk = coldOk && report_child(t, "cold", coldRc, cold, "EXTERN_DRV=7")
    t |> success(coldOk, "cold run wrote the cache")
    var warm : string
    let warmRc = run_child(cmd, warm)
    var warmOk = report_child(t, "warm", warmRc, warm, "mc_extern_lib.das' (registers at compile time)")
    warmOk = warmOk && report_child(t, "warm", warmRc, warm, "deser: partial - 1 module(s) reparsed in place, rest from cache")
    warmOk = warmOk && report_child(t, "warm", warmRc, warm, "EXTERN_DRV=7")
    if (find(warm, "cumulative hash") >= 0 || find(warm, "ser: program failed") >= 0) {
        t |> failure("the warm run read the registrar's payload and failed it on the hash instead of skipping the record")
        warmOk = false
    }
    t |> success(warmOk, "warm run skips the registrar's record on its mark and serves the driver")
    var err : string
    rmdir_rec(tmp, err)
}
