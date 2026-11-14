options gen2

require dastest/testing_boost
require daslib/tune
require daslib/module_path
require daslib/strings_boost
require daslib/fio
require strings
require _spawn_common
require llvm/daslib/llvm_tune
require llvm/daslib/llvm_cpu_class

// [tune] framework (llvm/daslib/llvm_tune): normal mode stamps the fallback perm's [llvm_code]
// onto the function itself; the add_plus_k selftest generator emits a+b+k so the stamped perm
// is observable at runtime. Same tolerance contract as llvm_code.das: dastest's in-process
// multi-program execution can serve a reference-tier result with jit instrumentation live, so
// the probe asserts "stamped-perm result or reference tier, never garbage". The grid modes and
// the manifest round-trip are compile-time-env driven (DAS_TUNE_MODE / DAS_TUNE_MANIFEST) and
// are gated by the CLI probes instead — see gemm_generator_plan.md M3.

// companion sibling (the two-function stamp): stamped with tadd_tuned's winner from the same
// entry — k_value emits `return k`, so the stamped perm is observable on the sibling too.
// Declared before the tuned function (the [tune_companion] declare-before rule).
def tadd_kv() : int => -1  // reference tier sentinel

[tune_perm(k = 0), tune_perm(k = 2), tune_perm(suffix = "declined", decline = true),
 tune_companion(fn = "tadd_kv", gen = "llvm_code_selftest::k_value"),
 tune(gen = "llvm_code_selftest::add_plus_k", fallback = "k2")]
def tadd_tuned(a, b : int) : int => a + b

[tune_perm(k = 1), tune(gen = "llvm_code_selftest::add_plus_k")]  // no fallback: reference body
def tadd_reference(a, b : int) : int => a + b

// per-ISA fallback chain: ';'-list walked in order, first perm whose requires= hint passes
// on this box wins. k9 requires a feature no box has (also pins that requires= is stripped
// from the auto-derived suffix — the row still keys as "k9"); k4 has no requires and stamps.
[tune_perm(k = 9, requires = "__no_such_cpuid_feature"), tune_perm(k = 4),
 tune(gen = "llvm_code_selftest::add_plus_k", fallback = "k9;k4")]
def tadd_chain(a, b : int) : int => a + b

[test(description = "a [tune] function runs its stamped fallback tier or the reference body and nothing else, its companion follows the same perm, a fallback chain skips an ineligible perm, and no fallback means the reference body")]
def test_llvm_tune(t : T?) {
    var n = 2  // nolint:LINT003 — var defeats const-fold so the calls survive to codegen
    let r = tadd_tuned(n, 3)
    t |> success(r == 7 || r == 5)  // stamped k2 tier (jit) or reference tier — never anything else
    let kv = tadd_kv()
    t |> success(kv == 2 || kv == -1)  // the companion carries the same winner perm (k2) or reference
    if (!jit_enabled()) {
        t |> equal(r, 5)   // interpreted lane always runs the reference body
        t |> equal(kv, -1)
    }
    t |> equal(tadd_reference(n, 3), 5)  // no manifest + no fallback = reference body everywhere
    let rc = tadd_chain(n, 3)
    // chain skipped ineligible k9 -> stamped k4 (2+3+4=9, jit tier) or reference (5) — a k9
    // stamp would yield 14, so the tolerant pair still pins the chain choice
    t |> success(rc == 9 || rc == 5)
    if (!jit_enabled()) {
        t |> equal(rc, 5)
    }
}

// The same [tune] shape as the grid-mode child fixture
// _llvm_tune_modes_client.das, compiled here in normal mode: the
// [test] below checks its fallback contract. The child keeps its own
// copy because a grid-mode run compiles it as a program root.

// companion sibling - grid modes clone it per perm too (modes_kv__k0)
def modes_kv() : int => -1  // reference tier sentinel

[tune_perm(k = 0), tune_perm(k = 1), tune_perm(k = 2),
 tune_perm(suffix = "declined", decline = true),
 tune_companion(fn = "modes_kv", gen = "llvm_code_selftest::k_value"),
 tune(gen = "llvm_code_selftest::add_plus_k", fallback = "k1")]
def modes_add(a, b : int) : int => a + b

[test(description = "a [tune] function with a declined row and fallback k1 runs the k1 tier or the reference body, and its companion follows the same perm")]
def test_modes_client_fallback(t : T?) {
    var n = 2  // nolint:LINT003 - var defeats const-fold for codegen
    let r = modes_add(n, 3)
    t |> success(r == 6 || r == 5)  // stamped fallback k1 tier (jit) or reference tier
    let kv = modes_kv()
    t |> success(kv == 1 || kv == -1)  // companion carries the same fallback perm (k1) or reference
    if (!jit_enabled()) {
        t |> equal(r, 5)
        t |> equal(kv, -1)
    }
}

// The [tune] grid modes are compile-time-env driven (DAS_TUNE_MODE) —
// a hosted test cannot set that for its own compile, so this test
// spawns a child daslang (the same binary that launched this run,
// argv[0]) on _llvm_tune_modes_client.das with the env set, and
// asserts the streamed per-variant results: reference row, per-perm
// generated values (k0/k1/k2 -> 5/6/7 — distinct values prove
// per-perm args reached the generator), and the declined row falling
// back to the reference body. The manifest round-trip is CLI-gated
// (gen_tune_probe) — see gemm_generator_plan.md M3.

[test(description = "under DAS_TUNE_MODE=test every grid perm runs its own generated body, the reference and declined rows run the reference body, and the companion's grid follows per perm", timeout = 1200)]
def test_llvm_tune_grid_mode(t : T?) {
    if (!jit_enabled()) {
        // the generator only fires under the LLVM JIT; the child needs -jit too
        t |> success(true)
        return
    }
    let args <- get_command_line_arguments()  // locked view — do not delete
    let bin = args[0]
    let client = "{get_this_module_dir()}/_llvm_tune_modes_client.das"
    let inner = "\"{bin}\" -no-module-cache -jit -jit-no-cache \"{client}\""
    // popen goes through cmd.exe / /bin/sh — set the env inside the command line
    let cmd = get_platform_name() == "windows" ? "set DAS_TUNE_MODE=test&& {inner}" : "DAS_TUNE_MODE=test {inner}"
    var lines : array<string>
    let rc = unsafe(popen_timeout(cmd, 300.0) $(f) {
        if (f == null) {
            return
        }
        while (!feof(f)) {
            let ln = fgets(f)
            if (ln |> starts_with("VARIANT ") || ln |> starts_with("KVARIANT ")) {
                lines |> push("{ln}")
            }
        }
    })
    t |> equal(rc, 0)
    var seen : table<string>
    for (ln in lines) {
        seen |> insert(strip(ln))
    }
    t |> success(seen |> key_exists("VARIANT reference -> 5"))
    t |> success(seen |> key_exists("VARIANT k0 -> 5"))
    t |> success(seen |> key_exists("VARIANT k1 -> 6"))
    t |> success(seen |> key_exists("VARIANT k2 -> 7"))
    t |> success(seen |> key_exists("VARIANT declined -> 5"))
    // the [tune_companion] grid: per-perm args reach the companion generator, and the
    // declined row's companion falls back to its reference body in lockstep
    t |> success(seen |> key_exists("KVARIANT reference -> -1"))
    t |> success(seen |> key_exists("KVARIANT k0 -> 0"))
    t |> success(seen |> key_exists("KVARIANT k1 -> 1"))
    t |> success(seen |> key_exists("KVARIANT k2 -> 2"))
    t |> success(seen |> key_exists("KVARIANT declined -> -1"))
    delete seen
    delete lines
}

// The fat mode (DAS_TUNE_MODE=fat under DAS_JIT_BASELINE=<class>, -exe) through child daslang
// processes: the build stamps one clone per class the scope ships a profile for, the exe picks
// the clone from cpuid at startup (the companion switching with it), DAS_TUNE_FAT_CLASS pins the
// pick and refuses a class the box is not in, and the mode refuses a non-exe build and a build
// with no baseline. The fixture set is COPIED into a per-process temp directory, the scope's
// defaults dir redirected into the copy, so the tracked fixtures beside this file are never
// written. Profiles are written for the ladder's lowest class (the baseline, k0) and its top
// class (k2); which clone RESULT reports depends on whether this box is in the top class.

def private tune_env_prefix(mode, baseline, pin : string;
                            forced : string = ""; manifest : string = "") : string =>
    child_env([("DAS_TUNE_MODE", mode), ("DAS_JIT_BASELINE", baseline), ("DAS_TUNE_FAT_CLASS", pin),
        ("DAS_TUNE_FORCE", forced), ("DAS_TUNE_MANIFEST", manifest),
        ("DAS_TUNE_VERBOSITY", "verbose"), ("DAS_TUNE_POLICY", ""), ("DAS_LOG_LEVEL", "info")])

def private has(lines : array<string>; needle : string) : bool {
    for (ln in lines) {
        if (find(ln, needle) >= 0) {
            return true
        }
    }
    return false
}

// the `attributes #N = { ... }` line of the IR function whose `define` line carries `needle`, "" when absent
def private attr_group_of(lines : array<string>; needle : string) : string {
    for (ln in lines) {
        continue if (!(ln |> starts_with("define")) || find(ln, needle) < 0)
        let hashAt = rfind(ln, " #")
        continue if (hashAt < 0)
        let rest = slice(ln, hashAt + 1)
        let sp = find(rest, " ")
        let group = sp < 0 ? rest : slice(rest, 0, sp)
        let key = "attributes {group} = "
        for (al in lines) {
            if (al |> starts_with(key)) {
                return al
            }
        }
    }
    return ""
}

def private write_profile(dir, klass, winner : string) {
    mkdir(dir)
    let features = jit_cpu_class_row(klass).features
    let body = "\{ \"kernels\" : \{ \"fat_add\" : \"{winner}\" \}, \"provenance\" : \{ \"features\" : \"{replace(features, ";", ";")}\" \} \}"
    fwrite(path_join(dir, "{klass}.tune-defaults.json"), body)
}

let private WORK_DEFAULTS = "tune_defaults"
let private SCOPE_FIXTURE = "_llvm_tune_fat_lib.das"
let private DECLARED_DEFAULTS = "defaults = \"./"

def private copy_fixture(t : T?; name, dstdir : string) {
    let text = fread("{get_this_module_dir()}/{name}")
    t |> success(!empty(text), "fixture {name} is unreadable")
    t |> success(fwrite(path_join(dstdir, name), text), "could not copy {name} into {dstdir}")
}

def private redirect_defaults(src : string) : string {
    let at = find(src, DECLARED_DEFAULTS)
    return src if (at < 0)
    let from = at + length(DECLARED_DEFAULTS)
    let close = find(src, "\"", from)
    return src if (close < 0)
    return "{slice(src, 0, at)}{DECLARED_DEFAULTS}{WORK_DEFAULTS}{slice(src, close)}"
}

def private copy_scope_fixture(t : T?; dstdir : string) {
    let src = fread("{get_this_module_dir()}/{SCOPE_FIXTURE}")
    let text = redirect_defaults(src)
    t |> success(text != src, "{SCOPE_FIXTURE} declares no relative {DECLARED_DEFAULTS}...\" for the copy to redirect")
    t |> success(fwrite(path_join(dstdir, SCOPE_FIXTURE), text), "could not copy {SCOPE_FIXTURE} into {dstdir}")
}

[test(description = "a fat -exe stamps one clone per profiled CPU class and dispatches by cpuid at startup, DAS_TUNE_FAT_CLASS pins or refuses the pick, fat mode refuses a -jit run and a build with no baseline, and a fat -lib picks its classes when a host creates it", timeout = 1200)]
def test_llvm_tune_fat(t : T?) { // nolint:STYLE038 - one flat spawn-and-assert rail per scenario; a split would hide the sequence the scenarios build on
    if (!jit_enabled()) {
        t |> success(true)
        return
    }
    let args <- get_command_line_arguments()  // aliases a process-global Array header - do not delete
    let bin = args[0]
    let dirRes = create_temp_directory_result("llvm_tune_fat_")
    if (dirRes is error) {
        t |> failure("could not create a temp dir: {dirRes as error}")
        return
    }
    let work = dirRes as value
    copy_fixture(t, "_llvm_tune_fat_client.das", work)
    copy_scope_fixture(t, work)
    let client = path_join(work, "_llvm_tune_fat_client.das")
    let exe = path_join(work, "_llvm_tune_fat_client")
    let defdir = path_join(work, WORK_DEFAULTS)
    var ladder <- [for (c in split(jit_cpu_class_ladder(get_architecture_name()), ";")); c; where !empty(c)]
    t |> success(length(ladder) >= 2, "this arch has a class ladder to be fat over")
    let top = ladder[0]
    let low = ladder[length(ladder) - 1] == "x86-base" ? "x86-avx2" : ladder[length(ladder) - 1]
    let boxInTop = tune_fat_box_in_class(top)
    write_profile(defdir, low, "k0")
    write_profile(defdir, top, "k2")

    // 1. the fat build: one clone per class with a profile, the baseline's from its own profile;
    //    the IR dump shows each clone a noinline function lowered for its own class
    var lb : array<string>
    let build_rc = spawn_child("{tune_env_prefix("fat", low, "")}\"{bin}\" -no-module-cache -exe "
        + "-output \"{exe}.exe\" \"{client}\" -- --jit-dump", lb)
    t |> equal(build_rc, 0, "case 1: the fat exe builds\n{join(lb, "\n")}")
    t |> success(lb |> has("fat clones {top}=k2/profile, {low}=k0/profile"), "case 1: the build names every class clone and its source")
    t |> success(lb |> has("DAS_JIT_BASELINE={low}"), "case 1: the build targets the baseline class")
    t |> success(!(lb |> has("targeting the current box")), "case 1: a fat exe never targets the box")
    let topAttrs = attr_group_of(lb, "fat_add__fat_{replace(top, "-", "_")}")
    let lowAttrs = attr_group_of(lb, "fat_add__fat_{replace(low, "-", "_")}")
    let topOnly = jit_cpu_class_ladder(get_architecture_name()) |> starts_with("x86") ? "+avx512vnni" : "+i8mm"
    t |> success(find(topAttrs, "\"target-features\"=\"{jit_class_plus_features(top)}\"") >= 0, "case 1: the {top} clone carries its class's features: {topAttrs}")
    t |> success(find(topAttrs, "noinline") >= 0, "case 1: the {top} clone is a function, never inlined into the baseline: {topAttrs}")
    t |> success(find(lowAttrs, "\"target-cpu\"=\"{jit_cpu_class_row(low).cpu}\"") >= 0 && find(lowAttrs, topOnly) < 0, "case 1: the {low} clone carries the baseline's cpu and none of the top class's features: {lowAttrs}")
    t |> success(lb |> has("@das.tune.fat_mask = internal unnamed_addr global i32 0"), "case 1: the dispatch mask is an LLVM-side word, not a das global")

    let can_run = get_platform_name() != "windows" && das_is_dll_build()
    if (can_run && fexist("{exe}.exe")) {
        // 2. the exe dispatches from cpuid: the top clone when this box is in the class, else the baseline's
        var lr : array<string>
        let run_rc = spawn_child("{tune_env_prefix("", "", "")}\"{exe}.exe\"", lr)
        t |> equal(run_rc, 0, "case 2: the fat exe runs\n{join(lr, "\n")}")
        t |> success(lr |> has(boxInTop ? "RESULT 6 KV 2 VOID 2 FORK 6" : "RESULT 4 KV 0 VOID 2 FORK 4"), "case 2: the {boxInTop ? top : low} clone and its companion ran, the void kernel's arm dispatched, and a pooled fork whose init never ran dispatched to the same clone (box in {top}: {boxInTop})\n{join(lr, "\n")}")
        t |> success(lr |> has("STATUS fat_add|{low}|k0|profile|active"), "case 2: the baseline clone is always active")
        t |> success(lr |> has("STATUS fat_add|{top}|k2|profile|{boxInTop ? "active" : "idle"}"), "case 2: the top clone's row says whether this box runs it")
        t |> success(lr |> has("fat kernels - this box runs the"), "case 2: the init announces the classes")
        t |> success(lr |> has("fat_add [{low} <- this box]: k0 (profile)"), "case 2: the status log marks the classes this box runs")
        delete lr

        // 3. DAS_TUNE_FAT_CLASS pins the pick to the baseline whatever cpuid says
        var lp : array<string>
        t |> equal(spawn_child("{tune_env_prefix("", "", low)}\"{exe}.exe\"", lp), 0, "case 3: the pinned exe runs")
        t |> success(lp |> has("RESULT 4 KV 0 VOID 2 FORK 4"), "case 3: the pinned baseline clone ran, in the pooled fork too\n{join(lp, "\n")}")
        t |> success(lp |> has("DAS_TUNE_FAT_CLASS={low} pins the class"), "case 3: the pin announces itself")
        delete lp

        // 4. a pin to a class this box is not in refuses to start
        if (!boxInTop) {
            var lf : array<string>
            t |> success(spawn_child("{tune_env_prefix("", "", top)}\"{exe}.exe\"", lf) != 0, "case 4: a pin outside the box refuses")
            t |> success(lf |> has("pins a CPU class this box is not in"), "case 4: the refusal names the pin\n{join(lf, "\n")}")
            delete lf
        }

        // 4b. a pin to a class the box is in but the exe carries no clone for refuses to start -
        //     only the top and the baseline are profiled, so a middle class of the ladder has none
        if (length(ladder) > 2 && tune_fat_box_in_class(ladder[1])) {
            var lm : array<string>
            t |> success(spawn_child("{tune_env_prefix("", "", ladder[1])}\"{exe}.exe\"", lm) != 0, "case 4b: a pin the exe carries no clone for refuses")
            t |> success(lm |> has("carries no clone for"), "case 4b: the refusal says the exe carries no such clone\n{join(lm, "\n")}")
            delete lm
        }
    }

    // 7. a profile's explicit `reference` entry is a decision: the main's
    //    clone and its companion's both read as profiled, which is what a
    //    fat release and the exe's own gate count
    write_profile(defdir, low, "reference")
    let exeRef = path_join(work, "_llvm_tune_fat_client_ref")
    var lref : array<string>
    let ref_rc = spawn_child("{tune_env_prefix("fat", low, "")}\"{bin}\" -no-module-cache -exe "
        + "-output \"{exeRef}.exe\" \"{client}\"", lref)
    t |> equal(ref_rc, 0, "case 7: the fat exe builds on a reference entry\n{join(lref, "\n")}")
    t |> success(lref |> has("fat clones {top}=k2/profile, {low}=reference/profile"),
        "case 7: the build names the reference entry as the profile's pick\n{join(lref, "\n")}")
    if (can_run && fexist("{exeRef}.exe")) {
        var lrr : array<string>
        let ref_run_rc = spawn_child("{tune_env_prefix("", "", low)}\"{exeRef}.exe\"", lrr)
        t |> equal(ref_run_rc, 0, "case 7: the exe runs pinned to the baseline\n{join(lrr, "\n")}")
        t |> success(lrr |> has("STATUS fat_add|{low}||profile|active"),
            "case 7: the main's reference clone reads as profiled\n{join(lrr, "\n")}")
        t |> success(lrr |> has("STATUS fat_kv|{low}||profile|active"),
            "case 7: the companion's clone follows its main's entry\n{join(lrr, "\n")}")
        delete lrr
    }
    delete lref

    // 5. fat is a standalone mode: a -jit run refuses by name
    var lj : array<string>
    let jit_fat_rc = spawn_child("{tune_env_prefix("fat", low, "")}\"{bin}\" -no-module-cache "
        + "-jit -jit-no-cache \"{client}\"", lj)
    t |> success(jit_fat_rc != 0, "case 5: fat under -jit refuses")
    t |> success(lj |> has("is a standalone mode"),
        "case 5: the refusal names the mode\n{join(lj, "\n")}")

    // 6. fat needs the baseline class
    var ln : array<string>
    let fat_rc = spawn_child("{tune_env_prefix("fat", "", "")}\"{bin}\" -no-module-cache -exe "
        + "-output \"{exe}.exe\" \"{client}\"", ln)
    t |> success(fat_rc != 0, "case 6: fat without a baseline refuses")
    t |> success(ln |> has("needs DAS_JIT_BASELINE"), "case 6: the refusal names the knob\n{join(ln, "\n")}")

    // 8. a fat -lib has no main: the library's own [init] picks
    //    the classes when a host creates it
    if (das_is_dll_build()) {
        copy_fixture(t, "_llvm_tune_fat_clib.das", work)
        copy_fixture(t, "_llvm_tune_fat_clib_host.das", work)
        write_profile(defdir, low, "k0")
        var ll : array<string>
        let lib_rc = build_clib(bin, work, "fatclib", tune_env_prefix("fat", low, ""), ll)
        t |> equal(lib_rc, 0, "case 8: the fat library builds\n{join(ll, "\n")}")
        t |> success(ll |> has("fat clones {top}=k2/profile, {low}=k0/profile"),
            "case 8: the library carries one clone per profiled class\n{join(ll, "\n")}")
        var box_mask = 0
        for (c in ladder) {
            if (tune_fat_box_in_class(c)) {
                box_mask |= tune_fat_class_bit(c)
            }
        }
        let host = path_join(work, "_llvm_tune_fat_clib_host.das")
        var lh : array<string>
        let run_host = "\"{bin}\" -no-module-cache \"{host}\""
        let host_rc = spawn_child("{tune_env_prefix("", "", "")}{run_host}", lh)
        t |> equal(host_rc, 0, "case 8: a host loads the fat library\n{join(lh, "\n")}")
        t |> success(lh |> has("LIBRESULT {13 + (boxInTop ? 2 : 0)} MASK {box_mask} LIBRARY true"),
            "case 8: the library picked this box's classes at create\n{join(lh, "\n")}")
        // a pin to a class of another architecture: the create fails
        // and says why, the host goes on
        let foreign_class = get_architecture_name() == "x86_64" ? "arm-neon" : "x86-avx2"
        var lp : array<string>
        let pinned_rc = spawn_child("{tune_env_prefix("", "", foreign_class)}{run_host}", lp)
        t |> equal(pinned_rc, 1,
            "case 8: a pin this box is not in fails the create\n{join(lp, "\n")}")
        t |> success(lp |> has("create failed") && lp |> has("DAS_TUNE_FAT_CLASS={foreign_class}"),
            "case 8: the failed create names the pin\n{join(lp, "\n")}")
        delete ll
        delete lh
        delete lp
    }

    delete ladder
    rmdir_rec(work)
}

//! The fat-mode C library fixture built with `-lib` under `env`, its
//! bindings beside it.
def private build_clib(bin, work, stem, env : string; var lines : array<string>) : int {
    let source = path_join(work, "_llvm_tune_fat_clib.das")
    let output = path_join(work, stem)
    let bindings = path_join(work, "_{stem}_c.das")
    return spawn_child("{env}\"{bin}\" -no-module-cache -lib \"{source}\" -output \"{output}\""
        + " -- --jit-lib-bindings \"{bindings}\"", lines)
}

[test(description = "DAS_TUNE_FORCE stamps a library and a -jit program from a sidecar another box minted before this binary and says so by name, no saver writes that file, and the same file named as an ordinary sidecar is refused", timeout = 1200)]
def test_llvm_tune_forced_file(t : T?) {
    if (!jit_enabled() || !das_is_dll_build()) {
        t |> success(true)
        return
    }
    // aliases a process-global Array header - do not delete
    let args <- get_command_line_arguments()
    let bin = args[0]
    let dirRes = create_temp_directory_result("llvm_tune_forced_")
    if (dirRes is error) {
        t |> failure("could not create a temp dir: {dirRes as error}")
        return
    }
    let work = dirRes as value
    copy_scope_fixture(t, work)
    copy_fixture(t, "_llvm_tune_fat_clib.das", work)
    copy_fixture(t, "_llvm_tune_fat_clib_host.das", work)
    let ladder_text = jit_cpu_class_ladder(get_architecture_name())
    var ladder <- [for (c in split(ladder_text, ";")); c; where !empty(c)]
    let low = ladder[length(ladder) - 1] == "x86-base" ? "x86-avx2" : ladder[length(ladder) - 1]
    write_profile(path_join(work, WORK_DEFAULTS), low, "k0")
    let foreign = path_join(work, "foreign.tune.json")
    fwrite(foreign, "\{ \"kernels\" : \{ \"fat_add\" : \"k2\" \}, "
        + "\"provenance\" : \{ \"box\" : \"elsewhere|none|another-box|0|no such cpu\" \} \}")
    t |> success(!(set_mtime_result(foreign, mktime(2020, 1, 1, 0, 0, 0)) is error),
        "the forced file is dated before this binary")
    let host = path_join(work, "_llvm_tune_fat_clib_host.das")

    var lf : array<string>
    let forced_rc = build_clib(bin, work, "fatclib", tune_env_prefix("", low, "", foreign), lf)
    t |> equal(forced_rc, 0, "the forced library builds\n{join(lf, "\n")}")
    t |> success(lf |> has("DAS_TUNE_FORCE={foreign} - kernels stamp from this file"),
        "the build names the knob where it takes effect\n{join(lf, "\n")}")
    t |> success(!(lf |> has("was minted on")),
        "a forced file is not judged by its box\n{join(lf, "\n")}")
    let run_host = "{tune_env_prefix("", "", "")}\"{bin}\" -no-module-cache \"{host}\""
    var lh : array<string>
    let forced_host_rc = spawn_child(run_host, lh)
    t |> equal(forced_host_rc, 0, "a host loads it\n{join(lh, "\n")}")
    t |> success(lh |> has("LIBRESULT 15 MASK 0 LIBRARY true"),
        "the library runs the forced file's winner (10+3+2)\n{join(lh, "\n")}")

    var lm : array<string>
    copy_fixture(t, "_llvm_tune_fat_client.das", work)
    var lj : array<string>
    let jit_rc = spawn_child("{tune_env_prefix("", low, "", foreign)}\"{bin}\" -no-module-cache "
        + "-jit -jit-no-cache \"{path_join(work, "_llvm_tune_fat_client.das")}\"", lj)
    t |> equal(jit_rc, 0, "a -jit program runs under the force\n{join(lj, "\n")}")
    t |> success(lj |> has("fat_add <- k2") && lj |> has(" KV 2 "),
        "and stamps and runs the forced file's winner\n{join(lj, "\n")}")

    let before_save = fread(foreign)
    copy_fixture(t, "_llvm_tune_force_save.das", work)
    var ls : array<string>
    let save_rc = spawn_child("{tune_env_prefix("", low, "", foreign)}\"{bin}\" -no-module-cache "
        + "\"{path_join(work, "_llvm_tune_force_save.das")}\"", ls)
    t |> equal(save_rc, 0, "a saver runs under the force\n{join(ls, "\n")}")
    t |> success(ls |> has("SAVED false") && ls |> has("nothing writes it"),
        "the saver is refused by name\n{join(ls, "\n")}")
    t |> equal(fread(foreign), before_save, "and the forced file keeps its bytes")

    let named_rc = build_clib(bin, work, "fatclib", tune_env_prefix("", low, "", "", foreign), lm)
    t |> equal(named_rc, 0, "the same file as an ordinary sidecar still builds\n{join(lm, "\n")}")
    t |> success(lm |> has("was minted on"),
        "an ordinary sidecar from another box is refused by name\n{join(lm, "\n")}")
    var lr : array<string>
    let named_host_rc = spawn_child(run_host, lr)
    t |> equal(named_host_rc, 0, "a host loads it\n{join(lr, "\n")}")
    t |> success(lr |> has("LIBRESULT 13 MASK 0 LIBRARY true"),
        "refused, the class profile's winner stamps (10+3+0)\n{join(lr, "\n")}")

    delete ladder
    delete lf
    delete lh
    delete lm
    delete lr
    rmdir_rec(work)
}

[export]
def main {
    return
}
