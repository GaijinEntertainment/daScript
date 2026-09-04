options gen2
options indenting = 4

require dastest/testing_boost public

require strings
require math
require daslib/fio
require daslib/strings_boost

//! the daslang binary to spawn - dastest runs as `daslang(.exe) dastest/dastest.das ...`,
//! so argv[0] is the interpreter, not the test script
def das_exe() : string {
    let args <- get_command_line_arguments()
    return empty(args) ? "" : args[0]
}

//! the child shares dastest's cwd (argv[0] is a cwd-relative interpreter path), so the default
//! cache lands in this tree's own .jitted_scripts/module_cache - the test removes what it wrote.
//! Commands stay unquoted: a leading quote breaks Windows _popen (cmd /c quote stripping)
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

def cache_file_path(stem : string) : string {
    var found = ""
    fio::dir(CACHE_DIR) $(name) {
        if (name |> starts_with("{stem}-") && name |> ends_with(".dascache")) {
            found = path_join(CACHE_DIR, name)
        }
    }
    return found
}

//! keeps the first half of the file - the stream header survives, a record in the middle does not
def truncate_file(path : string) {
    var half : array<uint8>
    fopen(path, "rb") $(f) {
        if (f != null) {
            half |> resize(int64(fstat(f).size / 2ul))
            fread(f, half)
        }
    }
    fopen(path, "wb") $(f) {
        if (f != null) {
            fwrite(f, half)
        }
    }
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

//! an explicit -module-cache says how many modules the cache served and where the cutoff fell
[test]
def test_fallback_verdict_names_the_cutoff(t : T?) {
    var terr : string
    let tmp = create_temp_directory("das_mc_verdict", terr)
    if (empty(tmp)) {
        t |> failure("create_temp_directory: {terr}")
        return
    }
    let leafB = path_join(tmp, "mc_leaf_b.das")
    fwrite(path_join(tmp, "mc_leaf_a.das"), "options gen2\nmodule mc_leaf_a\ndef leaf_a() : int \{\n    return 1\n\}\n")
    fwrite(leafB, "options gen2\nmodule mc_leaf_b\ndef leaf_b() : int \{\n    return 2\n\}\n")
    fwrite(path_join(tmp, "mc_root.das"), "options gen2\nrequire mc_leaf_a\nrequire mc_leaf_b\n[export]\ndef main \{\n    print(\"MARK_verdict \{leaf_a() + leaf_b()\}\\n\")\n\}\n")
    let cmd = "{das_exe()} -dasroot {get_das_root()} -module-cache {path_join(tmp, "explicit.dascache")} {path_join(tmp, "mc_root.das")}"
    var cold : string
    t |> success(report_child(t, "verdict cold", run(cmd, cold), cold, "ser: wrote"), "the cold run writes the explicit cache")
    fwrite(leafB, "options gen2\nmodule mc_leaf_b\ndef leaf_b() : int \{\n    return 20\n\}\n")
    var warm : string
    t |> success(report_child(t, "verdict warm", run(cmd, warm), warm, "MARK_verdict 21"), "the edited leaf is recompiled")
    t |> success(find(warm, "deser: FALLBACK - 1 module(s) served from cache") >= 0, "the verdict counts the module served before the cutoff:\n{warm}")
    t |> success(find(warm, "mc_leaf_b.das") >= 0 && find(warm, "(file changed)") >= 0, "the verdict names the cutoff file and its reason:\n{warm}")
    var err : string
    rmdir_rec(tmp, err)
}

//! a record written under one set of compile policies serves nothing to a compile under another:
//! the same explicit cache read by a -no-lint run stops at the first module and is rewritten
[test]
def test_policy_change_refuses_the_cache(t : T?) {
    var terr : string
    let tmp = create_temp_directory("das_mc_policy", terr)
    if (empty(tmp)) {
        t |> failure("create_temp_directory: {terr}")
        return
    }
    fwrite(path_join(tmp, "mc_pol_leaf.das"), "options gen2\nmodule mc_pol_leaf\ndef leaf() : int \{\n    return 3\n\}\n")
    fwrite(path_join(tmp, "mc_pol_root.das"), "options gen2\nrequire mc_pol_leaf\n[export]\ndef main \{\n    print(\"MARK_policy \{leaf()\}\\n\")\n\}\n")
    let base = "{das_exe()} -dasroot {get_das_root()}"
    let tail = "-module-cache {path_join(tmp, "explicit.dascache")} {path_join(tmp, "mc_pol_root.das")}"
    var cold : string
    t |> success(report_child(t, "policy cold", run("{base} {tail}", cold), cold, "ser: wrote"), "the cold run writes the explicit cache")
    var same : string
    t |> success(report_child(t, "policy same", run("{base} {tail}", same), same, "deser: clean"), "the same policies serve every module")
    var changed : string
    t |> success(report_child(t, "policy changed", run("{base} -no-lint {tail}", changed), changed, "MARK_policy 3"), "a -no-lint run over the plain run's cache still executes")
    t |> success(find(changed, "compile policies changed at") >= 0 && find(changed, "mc_pol_leaf.das") >= 0, "the verdict names the policy cutoff at the first module:\n{changed}")
    t |> success(find(changed, "ser: wrote") >= 0, "the cache is rewritten under the new policies")
    var rewritten : string
    t |> success(report_child(t, "policy rewritten", run("{base} -no-lint {tail}", rewritten), rewritten, "deser: clean"), "the rewritten cache serves the -no-lint compile")
    var err : string
    rmdir_rec(tmp, err)
}

//! the modes that never install the default cache: the opt-out flag, a compile with no run, an AOT-consuming run
def check_off_modes(t : T?; base, b, c : string) {
    var off : string
    let offRc = run("{base} -no-module-cache {b}", off)
    t |> success(report_child(t, "no-module-cache", offRc, off, "MARK_mc_default_b"), "-no-module-cache still executes")
    t |> equal(cache_files("mc_default_b"), 0, "-no-module-cache writes no cache")
    var co : string
    let coRc = run("{base} -compile-only {c}", co)
    t |> success(coRc == 0, "-compile-only compiles:\n{co}")
    t |> equal(cache_files("mc_default_c"), 0, "-compile-only writes no cache")
    var aot : string
    run("{base} -use-aot {c}", aot)
    t |> equal(cache_files("mc_default_c"), 0, "-use-aot writes no cache, whether or not the binary carries AOT stubs")
}

def count_lines_with(text, needle : string) : int {
    var n = 0
    for (ln in split(text, "\n")) {
        if (find(ln, needle) >= 0) {
            n++
        }
    }
    return n
}

//! the AOT generator is a daslang script whose shared modules a warm default cache serves; a
//! deserialized module must still read as AOT-capable, or every stub it emits is empty
[test]
def test_aot_generation_reads_cached_modules(t : T?) {
    var terr : string
    let tmp = create_temp_directory("das_mc_aot", terr)
    if (empty(tmp)) {
        t |> failure("create_temp_directory: {terr}")
        return
    }
    let root = get_das_root()
    let fixture = path_join(tmp, "mc_aot_fixture.das")
    fwrite(fixture, "options gen2\nrequire daslib/strings_boost\n[export]\ndef main \{\n    print(join([\"a\", \"b\"], \",\"))\n\}\n")
    let gen = "{das_exe()} -dasroot {root} {path_join(root, "utils/aot/main.das")} -- -aot {fixture}"
    var cold : string
    t |> equal(run("{gen} {path_join(tmp, "cold.cpp")}", cold), 0, "the cold generator run exits clean:\n{cold}")
    let coldCpp = fread(path_join(tmp, "cold.cpp"))
    let coldHashes = count_lines_with(coldCpp, "hash=")
    t |> success(coldHashes > 0, "the cold run emits functions")
    t |> success(find(coldCpp, "AOT disabled due to this module") < 0, "no module reads as AOT-disabled on the cold run")
    var warm : string
    t |> equal(run("{gen} {path_join(tmp, "warm.cpp")}", warm), 0, "the warm generator run exits clean:\n{warm}")
    let warmCpp = fread(path_join(tmp, "warm.cpp"))
    t |> success(find(warmCpp, "AOT disabled due to this module") < 0, "no module reads as AOT-disabled when the generator's modules came from the cache")
    t |> equal(count_lines_with(warmCpp, "hash="), coldHashes, "the warm run emits the same functions as the cold run")
    var err : string
    rmdir_rec(tmp, err)
}

//! overwrites the file's first 64 bytes - the stream header and version - with 0xff
def scramble_header(path : string) {
    var bytes : array<uint8>
    fopen(path, "rb") $(f) {
        if (f != null) {
            bytes |> resize(int64(fstat(f).size))
            fread(f, bytes)
        }
    }
    for (i in range(min(64, length(bytes)))) {
        bytes[i] = 0xffu8
    }
    fopen(path, "wb") $(f) {
        if (f != null) {
            fwrite(f, bytes)
        }
    }
}

//! a served module keeps its source lines: the lint reads `nolint:` markers off them, so a
//! warm run of a module whose only defence against a lint finding is the marker must still compile
[test]
def test_source_lines_survive_the_cache(t : T?) {
    var terr : string
    let tmp = create_temp_directory("das_mc_src", terr)
    if (empty(tmp)) {
        t |> failure("create_temp_directory: {terr}")
        return
    }
    fwrite(path_join(tmp, "mc_marked.das"), "options gen2\nmodule mc_marked\ndef marked(x) \{\n    var v = x   // nolint:LINT003 - the marker is what this generic lives on, instantiated in the root\n    return v\n\}\n")
    let root = path_join(tmp, "mc_src_root.das")
    fwrite(root, "options gen2\nrequire daslib/lint\nrequire mc_marked\n[export]\ndef main \{\n    print(\"MARK_src \{marked(7)\}\\n\")\n\}\n")
    remove_cache_files("mc_src_root")
    let cmd = "{das_exe()} -dasroot {get_das_root()} {root}"
    var cold : string
    t |> success(report_child(t, "marked cold", run(cmd, cold), cold, "MARK_src 7"), "the cold run compiles past the marked variable")
    // the root changes so it re-parses and its lint pass runs, while the marked module is served
    fwrite(root, "options gen2\nrequire daslib/lint\nrequire mc_marked\n[export]\ndef main \{\n    print(\"MARK_src2 \{marked(7)\}\\n\")\n\}\n")
    var warm : string
    t |> success(report_child(t, "marked warm", run(cmd, warm), warm, "MARK_src2 7"), "the warm run still sees the nolint marker on the served module")
    t |> success(find(warm, "LINT003") < 0, "no LINT003 leaks through a served module:\n{warm}")
    remove_cache_files("mc_src_root")
    var err : string
    rmdir_rec(tmp, err)
}

//! a cache file cut in the middle, or one whose header is garbage, is read as far as it goes,
//! silently, and rewritten
def check_damaged_cache(t : T?; base, a : string) {
    truncate_file(cache_file_path("mc_default_a"))
    var damaged : string
    let damagedRc = run("{base} {a}", damaged)
    t |> success(report_child(t, "damaged cache", damagedRc, damaged, "MARK_mc_default_a"), "a run over a truncated cache file executes")
    t |> success(quiet(damaged), "the default cache says nothing over a truncated cache file:\n{damaged}")
    t |> equal(cache_files("mc_default_a"), 1, "the truncated file is rewritten in place")
    scramble_header(cache_file_path("mc_default_a"))
    var garbage : string
    let garbageRc = run("{base} {a}", garbage)
    t |> success(report_child(t, "garbage header", garbageRc, garbage, "MARK_mc_default_a"), "a run over a cache file with a garbage header executes")
    t |> success(quiet(garbage) && find(garbage, "das: ") < 0, "the default cache says nothing over a garbage header - no verdict, no serializer warning:\n{garbage}")
    t |> equal(cache_files("mc_default_a"), 1, "the garbage file is rewritten in place")
}

//! the AST module cache is on by default - silent, at a path keyed by script, host flags and DAS* env - and the flags that turn it off or make it loud
[test]
def test_default_module_cache(t : T?) {
    let hostArgs <- get_command_line_arguments()
    if (find_index(hostArgs, "--use-aot") >= 0) {
        t |> skip("an AOT-consuming host never installs the default cache")
        return
    }
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
    let base = "{das_exe()} -dasroot {root}"
    let a = "{path_join(tmp, "mc_default_a.das")}"
    let b = "{path_join(tmp, "mc_default_b.das")}"
    let c = "{path_join(tmp, "mc_default_c.das")}"
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
    check_damaged_cache(t, base, a)
    check_off_modes(t, base, b, c)
    var loud : string
    let loudRc = run("{base} -module-cache {path_join(tmp, "explicit.bin")} {a}", loud)
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
    let bothRc = run("{base} -module-cache {ignored} -no-module-cache {b}", both)
    t |> success(report_child(t, "explicit+off", bothRc, both, "MARK_mc_default_b"), "-module-cache with -no-module-cache still executes")
    t |> success(quiet(both), "-no-module-cache wins over -module-cache - no verdict line:\n{both}")
    var ignoredStat : FStat
    t |> success(!stat(ignored, ignoredStat), "-no-module-cache wins over -module-cache - the explicit file is not written")
    var serOff : string
    let serOffRc = run("{base} -no-module-cache -ser {path_join(tmp, "explicit.ser")} {c}", serOff)
    t |> success(serOffRc != 0, "-no-module-cache beside -ser is rejected, not silently honoured either way")
    t |> success(find(serOff, "do not combine") >= 0, "the rejection names the conflict:\n{serOff}")
    let binDir = dir_name(das_exe())
    let exeName = base_name(das_exe())
    let viaPath = win ? "set \"PATH={binDir};%PATH%\"&& {exeName} -dasroot {root}" : "PATH={binDir}:$PATH {exeName} -dasroot {root}"
    var pathRun : string
    let pathRc = run("{viaPath} {a}", pathRun)
    t |> success(report_child(t, "via PATH", pathRc, pathRun, "MARK_mc_default_a"), "a PATH launch executes")
    t |> equal(cache_files("mc_default_a"), 3, "a PATH launch keys the same file as the path launch - the binary is resolved, not read off argv[0]")
    for (stem in stems) {
        remove_cache_files(stem)
    }
    var err : string
    rmdir_rec(tmp, err)
}
