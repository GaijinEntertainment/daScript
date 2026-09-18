options gen2
options indenting = 4

require dastest/testing_boost public

require strings
require daslib/fio
require _mc_common

//! a module whose compile registered a call macro into itself (an annotation's apply) is never
//! served from the cache: the record replays the module's init, not that registration, so the
//! reader reparses it in place while the records around it still serve
//! (src/ast/ARCHITECTURE.md#module-cache-read)
[test]
def test_apply_macro_reparse(t : T?) {
    var terr : string
    let tmp = create_temp_directory("das_mc_apply", terr)
    if (empty(tmp)) {
        t |> failure("create_temp_directory: {terr}")
        return
    }
    let root = get_das_root()
    let fixtures = "{root}/tests/module_cache/_fixtures"
    for (name in ["mc_apply_macro.das", "mc_apply_lib.das", "mc_apply_drv.das"]) {
        fwrite(path_join(tmp, name), fread(path_join(fixtures, name)))
    }
    let drv = path_join(tmp, "mc_apply_drv.das")
    let cmd = "{trace_prefix()}\"{das_exe()}\" -dasroot \"{root}\" -module-cache \"{tmp}/mc.bin\" \"{drv}\""
    // the child prints the path it resolved the module by; the marker holds the name and the reason
    let reparsed = "mc_apply_lib.das' (registers macros at compile time)"
    var cold : string
    let coldRc = run_child(cmd, cold)
    var coldOk = report_child(t, "cold", coldRc, cold, "ser: wrote")
    coldOk = coldOk && report_child(t, "cold", coldRc, cold, "PROBED=42")
    t |> success(coldOk, "cold run wrote the cache and the call macro rewrote the site")
    // warm: every record's header matches, the annotated lib's record is skipped and the lib
    // reparsed in place, the driver still served - and the site still rewritten
    var warm : string
    let warmRc = run_child(cmd, warm)
    var warmOk = report_child(t, "warm", warmRc, warm, reparsed)
    warmOk = warmOk && report_child(t, "warm", warmRc, warm, "deser: partial - 1 module(s) reparsed in place, rest from cache")
    warmOk = warmOk && report_child(t, "warm", warmRc, warm, "PROBED=42")
    t |> success(warmOk, "warm run reparses the annotated lib in place and serves the rest")
    // the driver changes: its record cuts off, the lib still reparses, the driver parses from
    // source against a lib that carries its call macro
    fwrite(drv, "{fread(drv)}\n// the driver changed\n")
    var changed : string
    let changedRc = run_child(cmd, changed)
    var chOk = report_child(t, "changed", changedRc, changed, reparsed)
    chOk = chOk && report_child(t, "changed", changedRc, changed, "deser: FALLBACK")
    chOk = chOk && report_child(t, "changed", changedRc, changed, "PROBED=42")
    if (find(changed, "can't locate") >= 0) {
        t |> failure("the changed run parsed the driver against a served lib that lost its call macro")
        chOk = false
    }
    t |> success(chOk, "a cutoff after the annotated lib still finds the macro the reparse registered")
    var err : string
    rmdir_rec(tmp, err)
}
