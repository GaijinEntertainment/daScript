options gen2
options indenting = 4

require dastest/testing_boost public

require strings
require daslib/fio
require daslib/strings_boost
require _mc_common

let CACHE_DIR = ".jitted_scripts/module_cache"

//! the cache files a child wrote for one stem: `<CACHE_DIR>/<stem>-<hash>.dascache`
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
//! policy off compiles from source with the record sitting right there. The child shares dastest's
//! cwd (argv[0] is a cwd-relative interpreter path), so the cache lands in this tree's own
//! .jitted_scripts/module_cache - the test removes what it wrote, the driver's own record included.
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
    remove_cache_files("mc_cf_root")
    let drv = "{das_exe()} -dasroot {root} {root}/tests/module_cache/_fixtures/mc_cf_drv.das -- {rootFile}"
    var cold : string
    var coldOk = run_child_reported(t, "cold", "{drv} cache", cold, "MC_CF_COMPILED")
    coldOk = coldOk && !served(cold, leafFile) && !served(cold, rootFile)
    t |> success(coldOk, "the cold compile parses every module from source:\n{cold}")
    t |> equal(cache_files("mc_cf_root"), 1, "the cold compile wrote one record for the compiled file")
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
    remove_cache_files("mc_cf_root")
    remove_cache_files("mc_cf_drv")
    var err : string
    rmdir_rec(tmp, err)
}
