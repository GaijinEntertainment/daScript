options gen2

require _mc_common
require daslib/fio
require strings
require daslib/strings_boost
require dastest/testing_boost public
require daslib/rtti
require daslib/ast

[test(description = "A module with a 64-bit annotation argument writes its record on the cold run and serves clean from the cache on the warm run", timeout = 1200)]
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

//! the cache is binary, so the record name is searched as bytes; fread's string stops at its first NUL
def private bytes_have(data : array<uint8> const#; needle : string) : bool {
    var found = false
    peek_data(needle) $(nd) {
        let n = length(nd)
        return if (n > length(data))
        for (i in range(length(data) - n + 1)) {
            var same = true
            for (j in range(n)) {
                if (data[i + j] != nd[j]) {
                    same = false
                    break
                }
            }
            if (same) {
                found = true
                break
            }
        }
    }
    return found
}

def private cache_names(path : string; needle : string) : bool {
    var found = false
    fopen(path, "rb") $(f) {
        return if (f == null)
        fmap(f) $(data) {
            found = bytes_have(data, needle)
        }
    }
    return found
}

//! a builtin module's embedded source compiles with the stream hidden: the cache carries no
//! record of it, so the module's cumulative hash is the same on a cold run and a warm one and a
//! program requiring the module serves clean (src/ast/ARCHITECTURE.md#module-cache-read)
[test(description = "A builtin module's embedded source compiles with the stream hidden, so its hash is the same cold and warm and the program serves clean", timeout = 1200)]
def test_builtin_embedded_not_cached(t : T?) {
    var terr : string
    let tmp = create_temp_directory("das_mc_embedded", terr)
    if (empty(tmp)) {
        t |> failure("create_temp_directory: {terr}")
        return
    }
    let root = get_das_root()
    let drv = path_join(tmp, "mc_embedded_drv.das")
    fwrite(drv, fread("{root}/tests/module_cache/_fixtures/mc_embedded_drv.das"))
    let cache = path_join(tmp, "mc.bin")
    let cmd = "{trace_prefix()}\"{das_exe()}\" -dasroot \"{root}\" -module-cache \"{cache}\" \"{drv}\""
    var cold : string
    let coldRc = run_child(cmd, cold)
    var coldOk = report_child(t, "cold", coldRc, cold, "ser: wrote")
    coldOk = coldOk && report_child(t, "cold", coldRc, cold, "EMBEDDED_DRV=")
    if (cache_names(cache, "unit_test.das")) {
        t |> failure("the cache carries a record of the embedded unit_test.das")
        coldOk = false
    }
    t |> success(coldOk, "cold run wrote the cache without the embedded source")
    var warm : string
    let warmRc = run_child(cmd, warm)
    var warmOk = report_child(t, "warm", warmRc, warm, "deser: clean")
    warmOk = warmOk && report_child(t, "warm", warmRc, warm, "EMBEDDED_DRV=")
    if (find(warm, "cumulative hash") >= 0) {
        t |> failure("the builtin module's cumulative hash differs between the cold run and the warm one")
        warmOk = false
    }
    t |> success(warmOk, "warm run serves every record, the builtin module's hash unchanged")
    var err : string
    rmdir_rec(tmp, err)
}

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
[test(description = "compile_file under module_cache writes on a cold compile, serves every module warm, reparses an edited module and compiles from source with the policy off", timeout = 1200)]
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
    let cacheDir = path_join(tmp, ".cache/daslang/module_cache")
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
    var depth : string
    var depthOk = run_child_reported(t, "depth", "{drv} cache depth", depth, "MC_CF_COMPILED")
    depthOk = depthOk && !served(depth, leafFile) && !served(depth, rootFile)
    t |> success(depthOk, "a fastcall depth cap is a policy the key folds, so it compiles cold:\n{depth}")
    t |> equal(cache_files(cacheDir, "mc_cf_root"), 6, "max_fast_call_depth keys a sixth record")
    var err : string
    rmdir_rec(tmp, err)
}

[test(description = "A program served from the module cache still answers which modules it used through a typedef", timeout = 1200)]
def test_typedef_use_survives_the_cache(t : T?) {
    if (aot_host_skips(t)) return
    var terr : string
    let tmp = create_temp_directory("das_mc_tu", terr)
    if (empty(tmp)) {
        t |> failure("create_temp_directory: {terr}")
        return
    }
    let root = get_das_root()
    let rootFile = path_join(tmp, "mc_tu_root.das")
    var copied = true
    for (name in ["mc_tu_root", "mc_tu_facade", "mc_tu_target"]) {
        copied = fwrite(path_join(tmp, "{name}.das"), fread(fixture(name))) && copied
    }
    t |> success(copied, "the fixtures are copied")
    let enter = get_platform_name() == "windows" ? "cd /d {tmp} &&" : "cd {tmp} &&"
    let host = "{enter} {get_full_file_name(das_exe())} -dasroot {root}"
    let drv = "{host} {fixture("mc_typedef_used_drv")} -- {rootFile} mc_tu_facade"
    let marker = "MC_TYPEDEF_USED mc_tu_facade=true"
    var cold : string
    var coldOk = run_child_reported(t, "cold", drv, cold, marker)
    coldOk = coldOk && !served(cold, rootFile)
    t |> success(coldOk, "the cold compile parses the root and records the module:\n{cold}")
    var warm : string
    var warmOk = run_child_reported(t, "warm", drv, warm, marker)
    warmOk = warmOk && served(warm, rootFile)
    t |> success(warmOk, "the warm compile serves the root and still answers:\n{warm}")
    //! an edited root reparses against a facade the cache serves
    t |> success(fwrite(rootFile, "{fread(rootFile)}\n"), "the root is edited")
    var mixed : string
    var mixedOk = run_child_reported(t, "mixed", drv, mixed, marker)
    let facadeFile = path_join(tmp, "mc_tu_facade.das")
    mixedOk = mixedOk && !served(mixed, rootFile) && served(mixed, facadeFile)
    t |> success(mixedOk, "a reparsed root still answers for a served facade:\n{mixed}")
    var err : string
    rmdir_rec(tmp, err)
}

// A generic instance restored from the module cache must keep its origin generic
// (Function::fromGeneric). The child host restores its own module graph from the cache and
// then compiles a fresh program in the same process - a null origin on any restored
// instance disables generic-instance call recovery library-wide, and the fixture stops
// compiling on the warm run with error[30341] while the cold run is fine.
[test(description = "A generic instance restored from the cache keeps its origin generic, so the warm run compiles as the cold one does", timeout = 1200)]
def test_generic_instance_origin(t : T?) {
    var terr : string
    let tmp = create_temp_directory("das_mc_origin", terr)
    if (empty(tmp)) {
        t |> failure("create_temp_directory: {terr}")
        return
    }
    let root = get_das_root()
    let argv <- [das_exe(), "-dasroot", root, "-module-cache", "{tmp}/mc.bin",
        "{root}/dastest/dastest.das", "--", "--test",
        "{root}/tests/module_cache/_fixtures/mc_generic_origin_drv.das"]
    var cold : string
    let coldRc = run_argv(argv, cold)
    let coldOk = report_child(t, "cold", coldRc, cold, "ser: wrote")
    t |> success(coldOk, "cold run wrote the cache and passed")
    var warm : string
    let warmRc = run_argv(argv, warm)
    // the verdict guards the vacuous pass: a cache that silently fell back to parsing would
    // pass this test without ever restoring a module
    var warmOk = report_child(t, "warm", warmRc, warm, "deser: clean")
    if (find(warm, "error[30341]") >= 0) {
        t |> failure("warm run lost generic-instance recovery (error[30341])")
        warmOk = false
    }
    t |> success(warmOk, "warm run served every module from the cache and passed")
    var err : string
    rmdir_rec(tmp, err)
}

def fixture(name : string) : string => "{get_das_root()}/tests/module_cache/_fixtures/{name}.das"

//! compiles the user fixture and answers how its compile saw mc_noopt_shared: -1 absent, 0 private to the compile, 1 promoted
def compile_user(t : T?; no_opt : bool) : int {
    var seen = -1
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.no_optimizations = no_opt
            compile_file(fixture("mc_noopt_user"), access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                t |> success(ok, "the user fixture compiles: {issues}")
                if (ok) {
                    program_for_each_module(program) $(mod) {
                        if (mod.name == "mc_noopt_shared") {
                            seen = mod.moduleFlags.promoted ? 1 : 0
                        }
                    }
                }
            }
        }
    }
    return seen
}

[test(description = "A compile with optimizations off does not promote the shared module it compiled, while an optimizing compile does and a later unoptimized compile reuses that copy", timeout = 1200)]
def test_no_optimizations_keeps_shared_dependencies_private(t : T?) {
    t |> run("a compile with optimizations off does not promote the shared module it compiled") @(t : T?) {
        t |> equal(compile_user(t, true), 0)
        t |> success(get_module("mc_noopt_shared") == null, "the module died with the compile's ModuleGroup")
    }
    t |> run("an optimizing compile promotes it, and a later compile with optimizations off reuses that copy") @(t : T?) {
        t |> equal(compile_user(t, false), 1)
        t |> success(get_module("mc_noopt_shared") != null, "the promoted module outlives the compile")
        t |> equal(compile_user(t, true), 1)
    }
}

[test(description = "A compound type name in a shared module reads the same on a cold run and on a warm run served clean from the cache", timeout = 1200), arch(at="ARCHITECTURE.md#process-startup")]
def test_shared_typename_cache(t : T?) {
    let tmp = test_temp_dir()
    let root = get_das_root()
    let fixture = path_join(root,
        "tests/module_cache/_fixtures/mc_shared_typename.das")
    let cache = path_join(tmp, "shared_typename.bin")
    let win = get_platform_name() == "windows"
    let args <- get_command_line_arguments()
    let uses_aot = find_index(args, "--use-aot") >= 0
    let host = get_executable_path()
    let exe = (uses_aot
        ? path_join(dir_name(host), win ? "daslang.exe" : "daslang") : host)
    let enter = win ? "cd /d" : "cd"
    let cmd = ("{enter} \"{tmp}\" && \"{exe}\" -dasroot \"{root}\""
        + " -module-cache \"{cache}\" \"{fixture}\"")
    let type_name = "tuple<a:float;b:float;c:float>"
    let marker = "SHARED_TYPENAME={type_name}\n"
    let expected_dll = uses_aot || das_is_dll_build()
    let host_marker = "SHARED_TYPENAME_DLL={expected_dll}\n"
    let clean = "deser: clean - all modules from cache\n"
    for (phase, verdict in ["cold", "warm"], ["ser: wrote", clean]) {
        var out : string
        let ok = run_child_reported(t, phase, cmd, out, marker)
        t |> success(ok && find(out, verdict) >= 0
            && find(out, host_marker) >= 0, "{phase} cache result:\n{out}")
    }
    var bytes : array<uint8>
    fopen(cache, "rb") $(f) {
        t |> strictEqual(f != null, true)
        bytes |> resize(int64(fstat(f).size))
        let read_size = long_fread(f, bytes)
        t |> strictEqual(read_size, long_length(bytes))
    }
    let stream_magic = "\xc4\x82\xcd\xea\x04"
    let version_233 = [0xe9u8, 0x01u8]
    let version_offset = length(stream_magic)
    t |> strictEqual(length(bytes) >= version_offset + 2, true)
    t |> strictEqual(find(bytes, stream_magic), 0)
    t |> strictEqual(int(bytes[version_offset]) & 128, 128)
    t |> strictEqual(int(bytes[version_offset + 1]) & 128, 0)
    let name_offset = find(bytes, type_name)
    t |> strictEqual(name_offset >= 0, true)
    t |> strictEqual(find(bytes, type_name, name_offset + 1), -1)
    let separator = find(type_name, ';')
    bytes[name_offset + separator] = uint8(' ')
    let bad_name = modify_data(type_name) $(var data) {
        data[separator] = uint8(' ')
    }
    let bad_marker = "SHARED_TYPENAME={bad_name}\n"
    for (phase, expected, verdict in ["poisoned", "legacy", "rewritten"],
            [bad_marker, marker, marker],
            [clean, "deser: FALLBACK", clean]) {
        if (phase == "legacy") {
            for (b, i in version_233, count()) {
                bytes[version_offset + i] = b
            }
        }
        if (phase != "rewritten") {
            fopen(cache, "wb") $(f) {
                t |> strictEqual(f != null, true)
                let written = long_fwrite(f, bytes)
                t |> strictEqual(written, long_length(bytes))
            }
        }
        var out : string
        let ok = run_child_reported(t, phase, cmd, out, expected)
        t |> success(ok && find(out, verdict) >= 0
            && find(out, host_marker) >= 0, "{phase} cache result:\n{out}")
    }
}

//! the per-module compile-time line a served main module prints: `cache read took <t>,  (<file>)` -
//! the main module's name is empty, so the line ends in the empty name and the file
def served_main(out, file : string) : bool {
    for (line in split(out, "\n")) {
        if (line |> starts_with("cache read took") && line |> ends_with(",  ({file})")) return true
    }
    return false
}

//! one host process compiles two scripts that require `json_boost`, a `shared` module. On the warm
//! run the first script's compile holds it, so the second script's stream reads its record into a
//! throwaway copy; a main-module type equal to one first written in that record must not reach the
//! copy through the stream's type table, or the second compile reads a freed struct.
//! The children run inside the temp directory, so the default cache they write is under it.
[test(description = "A main-module type equal to one in a shared module's record never reaches the throwaway copy through the stream's type table", timeout = 1200)]
def test_type_backref_stays_in_module_record(t : T?) {
    if (aot_host_skips(t)) return
    var terr : string
    let tmp = create_temp_directory("das_mc_backref", terr)
    if (empty(tmp)) {
        t |> failure("create_temp_directory: {terr}")
        return
    }
    let root = get_das_root()
    let fixtures = "{root}/tests/module_cache/_fixtures"
    let second = "{fixtures}/mc_backref_second.das"
    let enter = get_platform_name() == "windows" ? "cd /d {tmp} &&" : "cd {tmp} &&"
    let cmd = "{enter} {get_full_file_name(das_exe())} -dasroot {root} -log-compile-time {fixtures}/mc_backref_first.das {second}"
    var cold : string
    let coldOk = run_child_reported(t, "cold", cmd, cold, "MC_BACKREF_SECOND ( 1)")
    t |> success(coldOk, "the cold run compiles and runs both scripts")
    var warm : string
    var warmOk = run_child_reported(t, "warm", cmd, warm, "MC_BACKREF_SECOND ( 1)")
    warmOk = warmOk && find(warm, "MC_BACKREF_FIRST") >= 0 && served_main(warm, second)
    t |> success(warmOk, "the warm run serves the second script beside the held json_boost and runs it:\n{warm}")
    var err : string
    rmdir_rec(tmp, err)
}
