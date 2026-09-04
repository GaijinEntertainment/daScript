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

//! the child shares dastest's cwd (argv[0] is a cwd-relative interpreter path), so the default
//! cache lands in this tree's own .jitted_scripts/module_cache - the test removes what it wrote
def run(cmd : string; var output : string&) : int {
    return unsafe(popen_timeout(cmd, 300.0, $(f) {
        if (f != null) {
            output = fread(f)
        }
    }))
}

let CACHE_DIR = ".jitted_scripts/module_cache"

def report_child(t : T?; phase : string; rc : int; out : string; marker : string) : bool {
    let ok = rc == 0 && find(out, marker) >= 0
    if (!ok) {
        t |> failure("{phase} child: rc={rc}, marker '{marker}' {find(out, marker) >= 0 ? "found" : "MISSING"}")
        t |> failure("{phase} child output follows:\n{out}")
    }
    return ok
}

//! the cache files written for one root: `<CACHE_DIR>/<stem>-<hash>.dascache`
def cache_files(stem : string) : int {
    var n = 0
    fio::dir(CACHE_DIR) $(name) {
        if (name |> starts_with("{stem}-") && name |> ends_with(".dascache")) {
            n++
        }
    }
    return n
}

def remove_cache_files(stem : string) {
    var victims : array<string>
    fio::dir(CACHE_DIR) $(name) {
        if (name |> starts_with("{stem}-") && name |> ends_with(".dascache")) {
            victims |> push(path_join(CACHE_DIR, name))
        }
    }
    for (v in victims) {
        remove(v)
    }
}

def quiet(out : string) : bool {
    return find(out, "deser:") < 0 && find(out, "ser:") < 0
}

//! the AST module cache is on by default - silent, at a path keyed by script, host flags and DAS* env - and the flags that turn it off or make it loud
[test]
def test_default_module_cache(t : T?) {
    var terr : string
    let tmp = create_temp_directory("das_mc_default", terr)
    if (empty(tmp)) {
        t |> failure("create_temp_directory: {terr}")
        return
    }
    let root = get_das_root()
    let stems <- ["mc_default_a", "mc_default_b", "mc_default_c"]
    for (stem in stems) {
        remove_cache_files(stem)
        fwrite(path_join(tmp, "{stem}.das"), "options gen2\n[export]\ndef main \{\n    print(\"MARK_{stem}\\n\")\n\}\n")
    }
    let base = "\"{das_exe()}\" -dasroot \"{root}\""
    let a = "\"{path_join(tmp, "mc_default_a.das")}\""
    let b = "\"{path_join(tmp, "mc_default_b.das")}\""
    let c = "\"{path_join(tmp, "mc_default_c.das")}\""
    var cold : string
    let coldRc = run("{base} {a}", cold)
    t |> success(report_child(t, "default cold", coldRc, cold, "MARK_mc_default_a"), "a plain run executes")
    t |> success(quiet(cold), "the default cache says nothing on the cold run:\n{cold}")
    t |> equal(cache_files("mc_default_a"), 1, "the cold run wrote one cache file for the root")
    var warm : string
    let warmRc = run("{base} {a}", warm)
    t |> success(report_child(t, "default warm", warmRc, warm, "MARK_mc_default_a"), "the warm run executes")
    t |> success(quiet(warm), "the default cache says nothing on the warm run:\n{warm}")
    t |> equal(cache_files("mc_default_a"), 1, "the warm run adds no second file")
    var off : string
    let offRc = run("{base} -no-module-cache {b}", off)
    t |> success(report_child(t, "no-module-cache", offRc, off, "MARK_mc_default_b"), "-no-module-cache still executes")
    t |> equal(cache_files("mc_default_b"), 0, "-no-module-cache writes no cache")
    var co : string
    let coRc = run("{base} -compile-only {c}", co)
    t |> success(coRc == 0, "-compile-only compiles:\n{co}")
    t |> equal(cache_files("mc_default_c"), 0, "-compile-only writes no cache")
    var loud : string
    let loudRc = run("{base} -module-cache \"{path_join(tmp, "explicit.bin")}\" {a}", loud)
    t |> success(report_child(t, "explicit", loudRc, loud, "ser: wrote"), "an explicit -module-cache keeps the loud verdict")
    let win = get_platform_name() == "windows"
    var envRun : string
    let envRc = run(win ? "set \"DAS_MC_PROBE=1\"&& {base} {a}" : "DAS_MC_PROBE=1 {base} {a}", envRun)
    t |> success(report_child(t, "env-keyed", envRc, envRun, "MARK_mc_default_a"), "a DAS* environment change still executes")
    t |> equal(cache_files("mc_default_a"), 2, "a DAS* environment change keys a second cache file")
    var flagged : string
    let flaggedRc = run("{base} -v2syntax {a}", flagged)
    t |> success(report_child(t, "flag-keyed", flaggedRc, flagged, "MARK_mc_default_a"), "a different host flag set still executes")
    t |> equal(cache_files("mc_default_a"), 3, "a host flag before the script keys a third cache file")
    var userArgs : string
    let userRc = run("{base} {a} -- --probe", userArgs)
    t |> success(report_child(t, "user-args", userRc, userArgs, "MARK_mc_default_a"), "script arguments after -- still execute")
    t |> equal(cache_files("mc_default_a"), 3, "script arguments after -- do not key a cache file")
    let ignored = path_join(tmp, "ignored.bin")
    var both : string
    let bothRc = run("{base} -module-cache \"{ignored}\" -no-module-cache {b}", both)
    t |> success(report_child(t, "explicit+off", bothRc, both, "MARK_mc_default_b"), "-module-cache with -no-module-cache still executes")
    t |> success(quiet(both), "-no-module-cache wins over -module-cache - no verdict line:\n{both}")
    var ignoredStat : FStat
    t |> success(!stat(ignored, ignoredStat), "-no-module-cache wins over -module-cache - the explicit file is not written")
    for (stem in stems) {
        remove_cache_files(stem)
    }
    var err : string
    rmdir_rec(tmp, err)
}
