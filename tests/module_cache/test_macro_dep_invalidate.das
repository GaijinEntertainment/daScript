options gen2
options indenting = 4

require dastest/testing_boost public

require strings
require daslib/fio

//! the daslang binary to spawn - dastest runs as `daslang(.exe) dastest/dastest.das ...`,
//! so argv[0] is the interpreter, not the test script
def das_exe() : string {
    let args <- get_command_line_arguments()
    return empty(args) ? "" : args[0]
}

//! child env rides the command string (the portable set/prefix split popen_argv cannot carry)
def env_run(dep, cmd : string; var output : string&) : int {
    let win = get_platform_name() == "windows"
    let full = win ? "set MC_DEP_FILE={dep}&& {cmd}" : "MC_DEP_FILE={dep} {cmd}"
    return unsafe(popen_timeout(full, 300.0, $(f) {
        if (f != null) {
            output = fread(f)
        }
    }))
}

def report_child(t : T?; phase : string; rc : int; out : string; marker : string) : bool {
    let ok = rc == 0 && find(out, marker) >= 0
    if (!ok) {
        t |> failure("{phase} child: rc={rc}, marker '{marker}' {find(out, marker) >= 0 ? "found" : "MISSING"}")
        t |> failure("{phase} child output follows:\n{out}")
    }
    return ok
}

//! the module cache must gate a macro-registered file input by CONTENT, not mtime: a tune
//! sidecar is rewritten byte-identically on every exit, and a record served stale replays
//! macro output minted against the old file - stale stamps, empty tune_status, unraced winners.
[test]
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
    // compile bypasses the module cache, so only the main-line require chain proves the gate
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
    var chOk = report_child(t, "changed", changedRc, changed, "ser: macro dependency changed")
    if (find(changed, "deser: clean") >= 0) {
        t |> failure("changed run still served the stale record from the cache")
        chOk = false
    }
    t |> success(chOk, "content change invalidated the cached record and the reparse passed")
    var err : string
    rmdir_rec(tmp, err)
}
