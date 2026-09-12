options gen2
options indenting = 4

require dastest/testing_boost public

require strings
require daslib/fio
require daslib/strings_boost
require _mc_common

//! the cache files a child wrote under `dir` for one stem: `<dir>/<stem>-<hash>.dascache`
def cache_files(dir, stem : string) : int {
    var n = 0
    fio::dir(dir) $(name) {
        if (name |> starts_with("{stem}-") && name |> ends_with(".dascache")) {
            n++
        }
    }
    return n
}

//! the per-module compile-time line a served module prints: `cache read took <t>, <module> (<file>)`
//! - matched on the file, since the compiled file's own module has no name
def served(out, file : string) : bool {
    for (line in split(out, "\n")) {
        if (line |> starts_with("cache read took") && find(line, "({file})") >= 0) return true
    }
    return false
}

//! an AOT-consuming host runs the AOT'd program, not the script; spawned as a child it prints no
//! compile-time log, so the cells skip there
def aot_host_skips(t : T?) : bool {
    let hostArgs <- get_command_line_arguments()
    if (find_index(hostArgs, "--use-aot") < 0) return false
    t |> skip("an AOT-consuming host runs no child script")
    return true
}

//! compile_file under `module_cache` reads and refreshes the default module cache the way the
//! daslang host does for its own script: a cold compile writes the record, a warm one serves every
//! module, an edit reparses the edited module while the modules before it still serve, and the
//! policy off compiles from source with the record sitting right there. The children run inside
//! the temp directory, so the default cache they write is under it and goes with it.
[test]
def test_compile_file_module_cache(t : T?) {
    if (aot_host_skips(t)) return
    var terr : string
    let tmp = create_temp_directory("das_mc_cf", terr)
    if (empty(tmp)) {
        t |> failure("create_temp_directory: {terr}")
        return
    }
    let root = get_das_root()
    let rootFile = path_join(tmp, "mc_cf_root.das")
    let leafFile = path_join(tmp, "mc_cf_leaf.das")
    fwrite(leafFile, "options gen2\nmodule mc_cf_leaf\ndef leaf() : int \{\n    return 2\n\}\n")
    fwrite(rootFile, "options gen2\nrequire mc_cf_leaf\n[export]\ndef main \{\n    print(\"\{leaf()\}\\n\")\n\}\n")
    let enter = get_platform_name() == "windows" ? "cd /d {tmp} &&" : "cd {tmp} &&"
    let drv = "{enter} {get_full_file_name(das_exe())} -dasroot {root} {root}/tests/module_cache/_fixtures/mc_cf_drv.das -- {rootFile}"
    let cacheDir = path_join(tmp, ".jitted_scripts/module_cache")
    var cold : string
    var coldOk = run_child_reported(t, "cold", "{drv} cache", cold, "MC_CF_COMPILED")
    coldOk = coldOk && !served(cold, leafFile) && !served(cold, rootFile)
    t |> success(coldOk, "the cold compile parses every module from source:\n{cold}")
    t |> equal(cache_files(cacheDir, "mc_cf_root"), 1, "the cold compile wrote one record for the compiled file")
    var warm : string
    var warmOk = run_child_reported(t, "warm", "{drv} cache", warm, "MC_CF_COMPILED")
    warmOk = warmOk && served(warm, leafFile) && served(warm, rootFile)
    t |> success(warmOk, "the warm compile serves the leaf and the root from the cache:\n{warm}")
    fwrite(rootFile, "options gen2\nrequire mc_cf_leaf\n[export]\ndef main \{\n    print(\"\{leaf() + 1\}\\n\")\n\}\n")
    var edited : string
    var editedOk = run_child_reported(t, "edited", "{drv} cache", edited, "MC_CF_COMPILED")
    editedOk = editedOk && served(edited, leafFile) && !served(edited, rootFile)
    t |> success(editedOk, "an edited root reparses while the leaf before it still serves:\n{edited}")
    var plain : string
    var plainOk = run_child_reported(t, "plain", drv, plain, "MC_CF_COMPILED")
    plainOk = plainOk && !served(plain, leafFile) && !served(plain, rootFile)
    t |> success(plainOk, "with the policy off the compile parses from source beside the record:\n{plain}")
    //! the key's two halves: other policies and other host arguments are other records
    var noopt : string
    var nooptOk = run_child_reported(t, "noopt", "{drv} cache noopt", noopt, "MC_CF_COMPILED")
    nooptOk = nooptOk && !served(noopt, leafFile) && !served(noopt, rootFile)
    t |> success(nooptOk, "other policies compile cold into a record of their own:\n{noopt}")
    t |> equal(cache_files(cacheDir, "mc_cf_root"), 2, "the policies hash keys a second record")
    let hostArgs = "{enter} {get_full_file_name(das_exe())} -dasroot {root} -log-compile-time {root}/tests/module_cache/_fixtures/mc_cf_drv.das -- {rootFile} cache"
    var host : string
    var hostOk = run_child_reported(t, "host", hostArgs, host, "MC_CF_COMPILED")
    hostOk = hostOk && !served(host, leafFile) && !served(host, rootFile)
    t |> success(hostOk, "other host arguments compile cold into a record of their own:\n{host}")
    t |> equal(cache_files(cacheDir, "mc_cf_root"), 3, "the host arguments key a third record")
    //! a --jit-target after the separator is a compile input the key folds too - and folded as
    //! text, not as the NUL-separated occurrence list, or every field after it would leave the key
    var target : string
    var targetOk = run_child_reported(t, "target", "{drv} cache --jit-target x86_64-pc-linux-gnu", target, "MC_CF_COMPILED")
    targetOk = targetOk && !served(target, leafFile) && !served(target, rootFile)
    t |> success(targetOk, "a --jit-target compiles cold into a record of its own:\n{target}")
    t |> equal(cache_files(cacheDir, "mc_cf_root"), 4, "the jit target keys a fourth record")
    var targetNoopt : string
    var targetNooptOk = run_child_reported(t, "target noopt", "{drv} cache noopt --jit-target x86_64-pc-linux-gnu", targetNoopt, "MC_CF_COMPILED")
    targetNooptOk = targetNooptOk && !served(targetNoopt, leafFile) && !served(targetNoopt, rootFile)
    t |> success(targetNooptOk, "other policies beside a --jit-target still compile cold:\n{targetNoopt}")
    t |> equal(cache_files(cacheDir, "mc_cf_root"), 5, "the policies hash survives the jit target in the key")
    var err : string
    rmdir_rec(tmp, err)
}
