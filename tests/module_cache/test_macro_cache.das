options gen2

require dastest/testing_boost public
require strings
require daslib/fio
require _mc_common

//! a module whose compile registered a call macro into itself (an annotation's apply) is never
//! served from the cache: the record replays the module's init, not that registration, so the
//! reader reparses it in place while the records around it still serve
//! (src/ast/ARCHITECTURE.md#module-cache-read)
[test(description = "A module whose compile registered a call macro into itself is never served from the cache and is reparsed in place while the records around it still serve", timeout = 1200)]
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
    let reparsed = "mc_apply_lib.das' (registers at compile time)"
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

//! a module whose compile registered an extern proxy into dasbind is never served from the
//! cache: its record is skipped on the mark, without the payload read that used to fail on
//! dasbind's cumulative hash, and it reparses in place while the driver still serves
//! (src/ast/ARCHITECTURE.md#module-cache-read)
[test(description = "A module whose compile registered an extern proxy into dasbind is never served from the cache and reparses in place while the driver still serves", timeout = 1200)]
def test_extern_registrar_reparse(t : T?) {
    if (typeinfo sizeof(type<void?>) != 8) {
        t |> skip("a 32-bit host builds no extern binder, so nothing registers at compile time")
        return
    }
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

//! a module compiled between two registrars is served: dasbind's proxies are not part of its hash,
//! so the later registrar's proxies do not fail it and the cache is not rewritten. A child process:
//! in this one dasbind keeps the cold compile's proxies, so the warm read never sees fewer
[test(description = "A module compiled between two dasbind registrars is served from the cache and the cache is not rewritten", timeout = 1200)]
def test_extern_between_registrars_served(t : T?) {
    if (typeinfo sizeof(type<void?>) != 8) {
        t |> skip("a 32-bit host builds no extern binder, so nothing registers at compile time")
        return
    }
    var terr : string
    let tmp = create_temp_directory("das_mc_extern2", terr)
    if (empty(tmp)) {
        t |> failure("create_temp_directory: {terr}")
        return
    }
    let root = get_das_root()
    let fixtures = "{root}/tests/module_cache/_fixtures"
    for (name in ["mc_extern_lib.das", "mc_extern_dep.das", "mc_extern2_drv.das"]) {
        fwrite(path_join(tmp, name), fread(path_join(fixtures, name)))
    }
    let drv = path_join(tmp, "mc_extern2_drv.das")
    let cmd = "{trace_prefix()}\"{das_exe()}\" -dasroot \"{root}\" -module-cache \"{tmp}/mc.bin\" \"{drv}\""
    var cold : string
    t |> success(run_child_reported(t, "cold", cmd, cold, "EXTERN2_DRV=8"), "cold run")
    var warm : string
    var warmOk = run_child_reported(t, "warm", cmd, warm, "deser: partial - 2 module(s) reparsed in place, rest from cache")
    if (find(warm, "cumulative hash") >= 0 || find(warm, "ser: wrote") >= 0) {
        t |> failure("the warm run failed the middle module on dasbind's hash and rewrote the cache:\n{warm}")
        warmOk = false
    }
    t |> success(warmOk && find(warm, "EXTERN2_DRV=8") >= 0, "warm run serves the module between the registrars")
    var err : string
    rmdir_rec(tmp, err)
}

//! child env rides the command string (the portable set/prefix split popen_argv cannot carry)
def env_run(dep, cmd : string; var output : string&) : int {
    let win = get_platform_name() == "windows"
    let full = win ? "set \"MC_DEP_FILE={dep}\"&& {cmd}" : "MC_DEP_FILE='{dep}' {cmd}"
    return unsafe(popen_timeout(full, 300.0, $(f) {
        if (f != null) {
            output = fread(f)
        }
    }))
}

//! the module cache must gate a macro-registered file input by CONTENT, not mtime: a tune
//! sidecar is rewritten byte-identically on every exit, and a record served stale replays
//! macro output minted against the old file - stale stamps, empty tune_status, unraced winners.
[test(description = "The module cache gates a macro-registered file input by content, not mtime, so a rewritten identical sidecar keeps serving and a changed one invalidates", timeout = 1200)]
def test_macro_dep_invalidate(t : T?) {
    var terr : string
    let tmp = create_temp_directory("das_mc_dep", terr)
    if (empty(tmp)) {
        t |> failure("create_temp_directory: {terr}")
        return
    }
    let dep = "{tmp}/dep_input.txt"
    fwrite(dep, "content v1")
    let root = get_das_root()
    // the driver runs as the child's MAIN program (not a dastest child): a nested runtime
    // compile caches only under its own module_cache policy, so the main-line require chain
    // is what proves the host's gate
    let cmd = "\"{das_exe()}\" -dasroot \"{root}\" -module-cache \"{tmp}/mc.bin\" \"{root}/tests/module_cache/_fixtures/mc_dep_drv.das\""
    var cold : string
    let coldRc = env_run(dep, cmd, cold)
    var coldOk = report_child(t, "cold", coldRc, cold, "ser: wrote")
    coldOk = coldOk && report_child(t, "cold", coldRc, cold, "PROBED=42")
    t |> success(coldOk, "cold run wrote the cache and passed")
    var warm : string
    let warmRc = env_run(dep, cmd, warm)
    t |> success(report_child(t, "warm", warmRc, warm, "deser: clean"), "warm run served every module from the cache")
    fwrite(dep, "content v1")
    var rewrite : string
    let rewriteRc = env_run(dep, cmd, rewrite)
    t |> success(report_child(t, "rewrite", rewriteRc, rewrite, "deser: clean"), "byte-identical rewrite stays cached")
    fwrite(dep, "content v2 - re-minted")
    var changed : string
    let changedRc = env_run(dep, cmd, changed)
    let registeredDep = (get_platform_name() == "windows"
        ? get_full_file_name(dep) : dep)
    var chOk = report_child(t, "changed", changedRc, changed, "ser: macro dependency changed")
    chOk = report_child(t, "changed", changedRc, changed,
        "(macro dependency '{registeredDep}' changed)\n") && chOk
    if (find(changed, "deser: clean") >= 0) {
        t |> failure("changed run still served the stale record from the cache")
        chOk = false
    }
    t |> success(chOk, "content change invalidated the cached record and the reparse passed")
    // absent -> present, through a RELATIVE dep path: the case the architecture doc calls the
    // one that matters most (a first mint has no sidecar), plus the builtin's cwd-absolutize
    // arm - a relative registration must still validate from the reader's cwd
    let depRel = "._mc_dep_probe_rel_{ref_time_ticks()}.txt"
    remove(depRel)
    // a FRESH cache: the phase-1 cache still validates its own (existing) dep and would serve clean
    let cmd2 = "\"{das_exe()}\" -dasroot \"{root}\" -module-cache \"{tmp}/mc2.bin\" \"{root}/tests/module_cache/_fixtures/mc_dep_drv.das\""
    var absent : string
    let absentRc = env_run(depRel, cmd2, absent)
    t |> success(report_child(t, "absent", absentRc, absent, "ser: wrote"), "cold run with an ABSENT dep wrote the cache")
    fwrite(depRel, "now it exists")
    var appeared : string
    let appearedRc = env_run(depRel, cmd2, appeared)
    var apOk = report_child(t, "appeared", appearedRc, appeared, "ser: macro dependency changed")
    if (find(appeared, "deser: clean") >= 0) {
        t |> failure("the appearing dep did not invalidate the cached record")
        apOk = false
    }
    t |> success(apOk, "a dep that APPEARS invalidates the record minted against its absence")
    remove(depRel)
    var err : string
    rmdir_rec(tmp, err)
}

//! the cold run writes the record, the warm run reparses the script
[test(description = "A cold run with a same-module call macro in a generic body writes its record and the warm run reparses the script", timeout = 1200)]
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
[test(description = "A script that requires a marked module still writes its own record because the marked module is a reference in it", timeout = 1200)]
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
