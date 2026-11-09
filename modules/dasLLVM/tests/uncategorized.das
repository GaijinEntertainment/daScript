options gen2
require dastest/testing_boost public
require daslib/module_path
require daslib/fio
require daslib/strings_boost
require daslib/ast_boost
require daslib/rtti
require daslib/debugger
require strings
require math
require llvm/daslib/llvm_jit_common
require llvm/daslib/llvm_jit_cli
require llvm/daslib/llvm_jit_run
require _llvm_cache_common
require _spawn_common
require _exe_test_common

// The module cache's corruption self-heal, end to end and interpreter-only (no jit gate -
// this runs in every dastest sweep): a damaged record resumes (skip + reparse in place,
// later records still served) AND triggers the writeback, so the NEXT run reads clean.
// Drift-flavored partials (the dasbind case) deliberately do NOT rewrite - that arm is
// covered by llvm_jit_cache.das's warm rounds; this test is the
// damage arm.

// stomp printable garbage across the first record's PAYLOAD: past the stream header +
// record header (hash+size+filename+deps+len - the temp-dir filename varies in length, hence
// the spread), far before the record's end (records here are >100KB), so the length field
// stays intact and the skip lands true. Many spots on purpose: a single spot could land
// inside a serialized string's CONTENT, which reads back silently - hitting structural
// bytes (lengths, tags, counts) is what forces the THROWING failure flavor
def private corrupt_first_record(path : string) : bool {
    var ok = false
    fopen(path, "rb+") $(f) {
        if (f != null) {
            var at = 300
            while (at < 8000) {
                f |> fseek(int64(at), seek_set)
                f |> fwrite("################################################################")
                at += 400
            }
            ok = true
        }
    }
    return ok
}

[test(description = "a module cache whose first record is corrupted resumes past it, rewrites the stream, and reads clean within two more runs", timeout = 1200)]
def test_cache_corruption_selfheal(t : T?) {
    let args <- get_command_line_arguments()  // locked view - do not delete
    let bin = get_full_file_name(args[0])
    let fixtures = get_this_module_dir()
    var terr : string
    let tmp = create_temp_directory("das_heal", terr)
    if (empty(tmp)) {
        t |> success(false, "create_temp_directory: {terr}")
        return
    }
    t |> success(copy_fixture(fixtures, tmp, "_llvm_split_modules_client.das"), "stage client fixture")
    t |> success(copy_fixture(fixtures, tmp, "_llvm_split_modules_mod_a.das"), "stage mod_a fixture")
    t |> success(copy_fixture(fixtures, tmp, "_llvm_split_modules_mod_b.das"), "stage mod_b fixture")
    // child CWD = tmp: a popen command that STARTS with a quoted
    // path trips cmd.exe's outer-quote stripping; the cd prefix
    // sidesteps it
    let cd_prefix = get_platform_name() == "windows" ? "cd /d \"{tmp |> replace("/", "\\")}\" && " : "cd \"{tmp}\" && "
    let base = "{cd_prefix}\"{bin}\" -dasroot \"{get_das_root()}\" -module-cache \"{tmp}/mc.bin\" -compile-only \"{tmp}/_llvm_split_modules_client.das\""
    // round 1: cold - builds the cache
    expect_round(t, "heal round 1 cold", base, ["ser: wrote"])
    // round 2: damage the first record's payload - the run must resume past it (reparse in
    // place, everything later served) AND repair the stream (corruption-flavored writeback)
    t |> success(corrupt_first_record("{tmp}/mc.bin"), "stomp record payload")
    expect_round(t, "heal round 2 corrupt", base,
        ["ser: reparsing in place", "deser: partial", "ser: wrote"])
    // rounds 3-4: the heal converges in at most TWO rounds - a record re-serialized from a
    // program that was itself served during the corrupt read (second generation) can fail
    // one more read; that round resumes it and rewrites from the fresh parse. Every round
    // still compiles correctly (rc 0) - only the verdict is allowed one extra 'partial'
    expect_round(t, "heal round 3", base, [], ["deser: clean", "deser: partial"])
    expect_round(t, "heal round 4 converged", base, ["deser: clean"])
    var err : string
    rmdir_rec(tmp, err)
}

//! `with_class_target_gates` sets the tier gates to one CPU class's set for its block and hands every one of them back.
struct Gates {
    avx2 : bool
    f16c : bool
    vnni256 : bool
    avx512bw : bool
    avx512vnni : bool
    avx512vbmi : bool
    vnniint8 : bool
    amx : bool
    amx_bf16 : bool
    dotprod : bool
    i8mm : bool
    fullfp16 : bool
}

def read_gates() : Gates {
    return Gates(avx2 = g_target_x64_avx2, f16c = g_target_x64_f16c, vnni256 = g_target_x64_vnni256,
        avx512bw = g_target_x64_avx512bw, avx512vnni = g_target_x64_avx512vnni,
        avx512vbmi = g_target_x64_avx512vbmi, vnniint8 = g_target_x64_vnniint8,
        amx = g_target_x64_amx, amx_bf16 = g_target_x64_amx_bf16, dotprod = g_target_arm64_dotprod,
        i8mm = g_target_arm64_i8mm, fullfp16 = g_target_arm64_fullfp16)
}

def write_gates(g : Gates) {
    g_target_x64_avx2 = g.avx2
    g_target_x64_f16c = g.f16c
    g_target_x64_vnni256 = g.vnni256
    g_target_x64_avx512bw = g.avx512bw
    g_target_x64_avx512vnni = g.avx512vnni
    g_target_x64_avx512vbmi = g.avx512vbmi
    g_target_x64_vnniint8 = g.vnniint8
    g_target_x64_amx = g.amx
    g_target_x64_amx_bf16 = g.amx_bf16
    g_target_arm64_dotprod = g.dotprod
    g_target_arm64_i8mm = g.i8mm
    g_target_arm64_fullfp16 = g.fullfp16
}

def count_set(g : Gates) : int {
    var n = 0
    for (v in [g.avx2, g.f16c, g.vnni256, g.avx512bw, g.avx512vnni, g.avx512vbmi, g.vnniint8,
            g.amx, g.amx_bf16, g.dotprod, g.i8mm, g.fullfp16]) {
        n += v ? 1 : 0
    }
    return n
}

[test(description = "with_class_target_gates holds the twelve tier gates at the x86-avx2 class's set inside its block and restores every gate after it")]
def test_class_target_gates(t : T?) {
    t |> run("every gate set before the block reads set after it") @(t : T?) {
        let saved = read_gates()
        let is_x64 = g_target_is_x64
        let is_aarch64 = g_target_is_aarch64
        g_target_is_x64 = true
        g_target_is_aarch64 = false
        write_gates(Gates(avx2 = true, f16c = true, vnni256 = true, avx512bw = true, avx512vnni = true,
            avx512vbmi = true, vnniint8 = true, amx = true, amx_bf16 = true, dotprod = true,
            i8mm = true, fullfp16 = true))
        var inside : Gates
        with_class_target_gates("x86-avx2") $ {
            inside = read_gates()
        }
        let after = read_gates()
        g_target_is_x64 = is_x64
        g_target_is_aarch64 = is_aarch64
        write_gates(saved)
        t |> equal(count_set(after), 12, "all twelve gates read set after the block")
        t |> success(after.amx_bf16, "the amx-bf16 gate reads set after the block")
        t |> success(!inside.amx_bf16 && !inside.amx && !inside.avx512bw,
            "inside the block the x86-avx2 class has no amx and no avx512 gate")
        t |> success(inside.avx2, "inside the block the x86-avx2 class has its avx2 gate")
    }
}

// Machine-enforces REVIEW.md's GEP rule: no dasLLVM source outside bindings/ may call
// LLVMSetIsInBounds - a constant-folded GEP is a ConstantExpr, and the setter's cast writes
// the flag through the wrong type into it (ARCHITECTURE.md#gep-constant-fold). Build the GEP
// in-bounds with LLVMBuildInBoundsGEP2 instead. Weakening this test is a defect.

//! spelled in two halves so this file does not name its own quarry
let private SET_IS_IN_BOUNDS_CALL = "LLVMSetIsInBounds" + "("

def private scan_dir(dir : string; var offenders : array<string>) {
    fio::dir(dir) $(name : string) {
        if (name == "." || name == "..") {
            return
        }
        let path = "{dir}/{name}"
        let st = stat(path)
        if (st.is_dir) {
            if (name != "bindings") {
                scan_dir(path, offenders)
            }
            return
        }
        if (!(name |> ends_with(".das"))) {
            return
        }
        let text = fread(path)
        var ln = 1
        for (line in split(text, "\n")) {
            let cmt = line |> find("//")
            let code = cmt >= 0 ? slice(line, 0, cmt) : line   // a mention in a comment is not a call
            if (code |> find(SET_IS_IN_BOUNDS_CALL) >= 0) {
                offenders |> push("{path}:{ln}")
            }
            ln++
        }
    }
}

[test(description = "no dasLLVM source outside bindings/ calls LLVMSetIsInBounds, whose cast writes the in-bounds flag into a constant-folded GEP through the wrong type")]
def test_no_llvm_set_is_in_bounds(t : T?) {
    var offenders : array<string>
    scan_dir("{get_this_module_dir()}/..", offenders)
    for (o in offenders) {
        t |> failure("LLVMSetIsInBounds called at {o} - build the GEP in-bounds with LLVMBuildInBoundsGEP2 (REVIEW.md; ARCHITECTURE.md#gep-constant-fold)")
    }
    t |> success(empty(offenders), "no LLVMSetIsInBounds call sites outside bindings/")
}

// A das global reaches JIT code through a jit_get_global_mnh lookup at function entry. The emitter
// resolves each global ONCE per function: every access then shares one base pointer, so LLVM sees
// xs[j] and xs[j + 1] as adjacent (vectorizable, alias-analyzable) and the runtime pays one hash
// lookup per global per call instead of one per access.

def private count_of(s : string; needle : string) : int {
    var n = 0
    var at = find(s, needle, 0)
    while (at >= 0) {
        n++
        at = find(s, needle, at + length(needle))
    }
    return n
}

//! a block label that opens a loop body: daslang's loop header, or
//! the vectorizer's; a preheader of either runs once, before it
def private is_loop_header(block_label : string) : bool {
    return false if (find(block_label, ".preheader") >= 0)
    for (prefix in ["for_start", "while_start", "vector.ph", "vector.body"]) {
        return true if (block_label |> starts_with(prefix))
    }
    return false
}

//! the `callee` lookups of a dumped function from its first loop
//! header on: a hoisted lookup sits in the entry block or in one of
//! the run-once blocks (`.preheader`, `iter.check`) before the loop
def private lookups_inside_loop(fn_ir : string; callee = "jit_get_global_mnh") : int {
    var n = 0
    var in_loop = false
    let blocks <- split(fn_ir, "\n\n")
    for (blk, ix in blocks, count()) {
        let block_label = slice(blk, 0, max(0, find(blk, ":")))
        in_loop = in_loop || (ix > 0 && is_loop_header(block_label))
        if (in_loop) {
            n += count_of(blk, "@{callee}(")
        }
    }
    return n
}

[test(description = "JIT code looks each das global up once per function, never inside a loop or ahead of an untaken branch, and a solid-context global takes no lookup at all", timeout = 1200)]
def test_global_lookup_once_per_function(t : T?) {
    with_child_log_info() {
        t |> run("four accesses to two globals emit one lookup per global") @(t : T?) {
            let out = jit_dump(t, "modules/dasLLVM/tests/_global_lookup_root.das", [])
            t |> success(find(out, "FOUR_SUMS ") >= 0, "the fixture printed its sum: {tail(out)}")
            let ir = function_ir(out, "`four_sums")
            t |> success(!empty(ir), "the dump carries four_sums: {tail(out)}")
            let lookups = count_of(ir, "@jit_get_global_mnh(")
            t |> equal(2, lookups, "one jit_get_global_mnh per global (xs, ys) in four_sums, not one per access:\n{ir}")
            //! the lazy half: a global written on one branch only is never looked up ahead of the branch - the
            //! entry block carries no constant-hash lookup (LLVM may fold both branches into ONE lookup whose
            //! hash is a select, which still probes once per call and never for the untaken global)
            let bir = function_ir(out, "`branchy Cb")
            t |> success(!empty(bir), "the dump carries branchy: {tail(out)}")
            let total = count_of(bir, "@jit_get_global_mnh(")
            t |> success(total >= 1 && total <= 2, "at most one lookup per branch global (ga, gb) in branchy, got {total}:\n{bir}")
            let entry = entry_block_ir(bir)
            let hoisted = count_of(entry, "@jit_get_global_mnh(") - count_of(entry, "@jit_get_global_mnh(i64 %")
            t |> equal(0, hoisted, "no constant-hash lookup in branchy's entry block - an untaken branch pays nothing:\n{bir}")
        }
        t |> run("a lookup under a branch in a loop leaves the loop") @(t : T?) {
            let out = jit_dump(t, "modules/dasLLVM/tests/_global_lookup_root.das", [])
            t |> success(find(out, "SUM_SCALED ") >= 0,
                "the fixture printed its scaled sum: {tail(out)}")
            let ir = function_ir(out, "`sum_scaled Ci")
            t |> success(!empty(ir), "the dump carries sum_scaled: {tail(out)}")
            t |> equal(2, count_of(ir, "@jit_get_global_mnh("),
                "one lookup per global (lut, scale) in sum_scaled:\n{ir}")
            t |> equal(0, lookups_inside_loop(ir),
                "no lookup is left inside the loop or the branch:\n{ir}")
        }
        t |> run("a shared-global lookup under a branch in a loop leaves the loop") @(t : T?) {
            let out = jit_dump(t, "modules/dasLLVM/tests/_global_lookup_root.das", [])
            t |> success(find(out, "SUM_SHARED_SCALED ") >= 0,
                "the fixture printed its shared scaled sum: {tail(out)}")
            let ir = function_ir(out, "`sum_shared_scaled Ci")
            t |> success(!empty(ir), "the dump carries sum_shared_scaled: {tail(out)}")
            t |> equal(1, count_of(ir, "call ptr @jit_get_shared_mnh("),
                "one lookup of shared_scale in sum_shared_scaled:\n{ir}")
            t |> equal(0, lookups_inside_loop(ir, "jit_get_shared_mnh"),
                "no shared lookup is left inside the loop or the branch:\n{ir}")
        }
        t |> run("a solid-context global is one base load per function and no lookup") @(t : T?) {
            let out = jit_dump(t, "modules/dasLLVM/tests/_global_lookup_solid_root.das", [])
            t |> success(find(out, "FOUR_SUMS ") >= 0, "the solid fixture printed its sum: {tail(out)}")
            let ir = function_ir(out, "`four_sums")
            t |> success(!empty(ir), "the dump carries four_sums: {tail(out)}")
            t |> equal(0, count_of(ir, "@jit_get_global_mnh("), "a solid-context global never goes through the hash lookup:\n{ir}")
            t |> equal(1, count_of(ir, "load ptr, ptr"), "one load of the context's globals base serves every access of both globals:\n{ir}")
        }
    }
}

// the hit path and its misses: modules/dasLLVM/ARCHITECTURE_JIT_ENTRY.md sec.1-2

//! the fixture prints its answer and whether the emitter module is in the process; `options
//! log_compile_time` is what turns the backend's phase line on
def private fixture(bound : int) : string =>
    "options gen2\noptions log_compile_time\nrequire daslib/rtti\n\n[export]\ndef main \{\n    var s = 0\n    for (i in range({bound})) \{\n        s += i * i\n    \}\n    print(\"PROBE \{s\}\\n\")\n    print(\"EMITTER \{has_module(\"llvm_macro\")\}\\n\")\n\}\n"

//! a helper the DLL holds; `no_jit` on it after the cold run makes the DLL hold a function the
//! plan disables, which the probe calls a miss
def private fixture_with_helper(no_jit_helper : bool) : string {
    let ann = no_jit_helper ? "[no_jit]\n" : ""
    return "options gen2\noptions log_compile_time\nrequire daslib/rtti\n\n{ann}def sq(i : int) : int \{\n    return i * i\n\}\n\n[export]\ndef main \{\n    var s = 0\n    for (i in range(10)) \{\n        s += sq(i)\n    \}\n    print(\"PROBE \{s\}\\n\")\n    print(\"EMITTER \{has_module(\"llvm_macro\")\}\\n\")\n\}\n"
}

//! nothing to jit: the only function is `[no_jit]`, so the plan's candidate set is empty
def private fixture_nothing_to_jit() : string =>
    "options gen2\noptions log_compile_time\nrequire daslib/rtti\n\n[no_jit, export]\ndef main \{\n    print(\"PROBE none\\n\")\n    print(\"EMITTER \{has_module(\"llvm_macro\")\}\\n\")\n\}\n"

def private jit_argv(script, cache, pinned : string; in_memory : bool) : array<string> {
    var argv <- [das_exe(), "-jit", "-log-compile-time", "-module-cache", cache, "-output", pinned, script]
    if (in_memory) {
        argv |> push("-jit-no-cache")
    }
    return <- argv
}

def private count_prefix(dir : string; prefix : string) : int {
    var n = 0
    fio::dir(dir) $(name) {
        if (name |> starts_with(prefix)) {
            n++
        }
    }
    return n
}

[test(description = "a warm run binds the pinned JIT DLL without loading the emitter, while a cold, stale, disabled-function or other program's DLL is a miss that loads the emitter and emits", timeout = 1200)]
def test_jit_link(t : T?) {
    with_child_log_info() {
        var terr : string
        let tmp = create_temp_directory("das_jit_link", terr)
        if (empty(tmp)) {
            t |> failure("create_temp_directory: {terr}")
            return
        }
        let script = path_join(tmp, "use_jit.das")
        let cache = path_join(tmp, "mc.dascache")
        let pinned = path_join(tmp, "jit_probe")
        fwrite(script, fixture(10))
        t |> run("a cold run misses the pinned DLL, brings the emitter in and emits") @(t : T?) {
            var out = ""
            let rc = run_and_capture(jit_argv(script, cache, pinned, false), out, 300.0)
            t |> equal(0, rc, "the fixture runs: {tail(out)}")
            t |> success(find(out, "PROBE 285") >= 0, "the fixture answered: {tail(out)}")
            t |> success(find(out, "LLVM JIT: DLL cache miss") >= 0 && find(out, "(codegen") >= 0, "the cold run is a miss that emits: {tail(out)}")
            t |> success(find(out, "EMITTER true") >= 0, "the emitter module joined the process for the miss: {tail(out)}")
            t |> equal(0, count_prefix(tmp, "late~"), "the emitter's modules are records of the script's own module cache, not a cache of their own")
            t |> success(find(out, "LLVM JIT time: plan ") >= 0 && find(out, " emitter load ") >= 0, "the link side's plan and the emitter's load print their times: {tail(out)}")
        }
        t |> run("a warm run binds the pinned DLL from the link module alone") @(t : T?) {
            var out = ""
            let rc = run_and_capture(jit_argv(script, cache, pinned, false), out, 300.0)
            t |> equal(0, rc, "the fixture runs: {tail(out)}")
            t |> success(find(out, "PROBE 285") >= 0, "the fixture answered: {tail(out)}")
            t |> success(find(out, "LLVM JIT: DLL cache hit") >= 0 && find(out, "(cache") >= 0, "the warm run is a hit: {tail(out)}")
            t |> success(find(out, "EMITTER false") >= 0, "the emitter module never entered the process on the hit: {tail(out)}")
            t |> success(find(out, "LLVM JIT time: hash ") >= 0 && find(out, " probe ") >= 0 && find(out, " install ") >= 0, "the hit path prints its phases: {tail(out)}")
        }
        t |> run("an in-memory run takes the emitter path and its finalizer frees the engine there") @(t : T?) {
            var out = ""
            let rc = run_and_capture(jit_argv(script, cache, pinned, true), out, 300.0)
            t |> equal(0, rc, "the fixture runs and exits cleanly: {tail(out)}")
            t |> success(find(out, "PROBE 285") >= 0 && find(out, "EMITTER true") >= 0 && find(out, "(codegen") >= 0, "in-memory codegen ran in the emitter: {tail(out)}")
        }
        t |> run("a changed fixture makes the pinned DLL stale, and the miss re-emits") @(t : T?) {
            fwrite(script, fixture(11))
            var out = ""
            let rc = run_and_capture(jit_argv(script, cache, pinned, false), out, 300.0)
            t |> equal(0, rc, "the fixture runs: {tail(out)}")
            t |> success(find(out, "PROBE 385") >= 0, "the new body answered: {tail(out)}")
            t |> success(find(out, "LLVM JIT: DLL cache miss") >= 0 && find(out, "EMITTER true") >= 0, "the stale DLL is a miss that emits: {tail(out)}")
        }
        t |> run("a `[no_jit]` function the DLL holds is a miss, and a program with nothing to jit never loads the emitter") @(t : T?) {
            let helper = path_join(tmp, "use_helper.das")
            let helperPin = path_join(tmp, "jit_helper")
            fwrite(helper, fixture_with_helper(false))
            var out = ""
            var rc = run_and_capture(jit_argv(helper, path_join(tmp, "mc_helper.dascache"), helperPin, false), out, 300.0)
            t |> equal(0, rc, "the helper fixture runs: {tail(out)}")
            t |> success(find(out, "PROBE 285") >= 0 && find(out, "(codegen") >= 0, "the cold run emits the helper: {tail(out)}")
            fwrite(helper, fixture_with_helper(true))
            rc = run_and_capture(jit_argv(helper, path_join(tmp, "mc_helper.dascache"), helperPin, false), out, 300.0)
            t |> equal(0, rc, "the fixture with the disabled helper runs: {tail(out)}")
            t |> success(find(out, "LLVM JIT: DLL cache miss") >= 0 && find(out, "EMITTER true") >= 0, "the DLL holding a now-disabled function is a miss: {tail(out)}")
            t |> success(find(out, "LLVM JIT time: plan ") >= 0 && find(out, " probe ") >= 0 && find(out, " emitter load ") >= 0, "the miss prints the link side's plan and probe times with the emitter's load: {tail(out)}")
            let nothing = path_join(tmp, "use_nothing.das")
            fwrite(nothing, fixture_nothing_to_jit())
            rc = run_and_capture(jit_argv(nothing, path_join(tmp, "mc_nothing.dascache"), path_join(tmp, "jit_nothing"), false), out, 300.0)
            t |> equal(0, rc, "the fixture with nothing to jit runs: {tail(out)}")
            t |> success(find(out, "PROBE none") >= 0 && find(out, "0 functions to jit") >= 0 && find(out, "EMITTER false") >= 0, "an empty candidate set is answered in the link module, no emitter loaded: {tail(out)}")
        }
        t |> run("a pinned DLL another program wrote holds none of this one's functions, and that is a miss") @(t : T?) {
            let other = path_join(tmp, "use_other.das")
            fwrite(other, "options gen2\nrequire daslib/rtti\n\n[export]\ndef main \{\n    var p = 1\n    for (i in range(12)) \{\n        p = (p * 3) % 1000\n    \}\n    print(\"OTHER \{p\}\\n\")\n    print(\"EMITTER \{has_module(\"llvm_macro\")\}\\n\")\n\}\n")
            var out = ""
            let rc = run_and_capture(jit_argv(other, path_join(tmp, "mc_other.dascache"), pinned, false), out, 300.0)
            t |> equal(0, rc, "the other program runs: {tail(out)}")
            t |> success(find(out, "OTHER ") >= 0, "the other program answered: {tail(out)}")
            t |> success(find(out, "LLVM JIT: DLL cache miss") >= 0 && find(out, "EMITTER true") >= 0 && find(out, "0 functions in") < 0, "the DLL of another program is a miss that emits, not a hit binding nothing: {tail(out)}")
        }
        var err : string
        rmdir_rec(tmp, err)
    }
}

//! a pointer difference lowers to i_das_ptr_diff(a, b, stride),
//! which the interpreter and the AOT inline; the dump shows the
//! JIT's form
[test(description = "the JIT lowers a pointer difference to inline subtraction instead of a call through the i_das_ptr_diff slot, and a backward distance divides signed", timeout = 1200)]
def test_a_pointer_difference_is_arithmetic_not_a_call(t : T?) {
    let out = jit_dump(t, "modules/dasLLVM/tests/_ptr_diff_root.das", [])
    t |> success(find(out, "DIST 5 -5") >= 0, "the fixture printed both distances: {tail(out)}")
    t |> success(find(out, "DIST64 -5") >= 0,
        "the unnarrowed backward distance divides signed: {tail(out)}")
    let ir = function_ir(out,
        "`distance C1`Lt`S`Lt`S`Gt``Gt`? C1`Lt`S`Lt`S`Gt``Gt`? implementation")
    t |> success(!empty(ir), "the dump carries distance: {tail(out)}")
    t |> equal(-1, find(ir, "i_das_ptr_diff"),
        "the difference is inline arithmetic, not a call through the i_das_ptr_diff slot:\n{ir}")
    t |> success(find(ir, "sub i64") >= 0, "the difference subtracts the two addresses:\n{ir}")
}

//! the remarks are stderr lines of a child: a second child in the
//! same directory proves they print past the DLL cache the first one
//! minted
[test(description = "--jit-remarks, --jit-remarks-missed and --jit-remarks-analysis each print their vectorizer remarks past a DLL cache hit, and under --jit-debug a remark names the loop's source line", timeout = 1200)]
def test_jit_remarks_print_the_vectorizer_decisions(t : T?) {
    var terr : string
    let dir = create_temp_directory("llvm_jit_remarks", terr)
    if (empty(dir)) {
        t |> failure("create_temp_directory: {terr}")
        return
    }
    let root = get_full_file_name("modules/dasLLVM/tests/_remarks_root.das")
    let noenv : array<string>
    var out = ""
    var rc = run_and_capture([das_exe(), "-no-module-cache", "-jit", root], dir, noenv, out, 300.0)
    t |> equal(0, rc, "the fixture runs under -jit: {tail(out)}")
    t |> success(find(out, "HALVED 1 ") >= 0, "the fixture printed its result: {tail(out)}")
    t |> equal(-1, find(out, "remark:"), "no remark prints without a remark flag: {tail(out)}")
    rc = run_and_capture([das_exe(), "-no-module-cache", "-jit", root, "--",
            "--jit-remarks=loop-vectorize"], dir, noenv, out, 300.0)
    t |> equal(0, rc, "the fixture runs under -jit --jit-remarks alone: {tail(out)}")
    t |> success(find(out, "remark: <unknown>:0:0: vectorized loop") >= 0,
        "--jit-remarks alone prints past the cache: {tail(out)}")
    rc = run_and_capture([das_exe(), "-no-module-cache", "-jit", root, "--",
            "--jit-remarks-missed=loop-vectorize"], dir, noenv, out, 300.0)
    t |> equal(0, rc, "the fixture runs under -jit --jit-remarks-missed alone: {tail(out)}")
    t |> success(find(out, "remark: <unknown>:0:0: loop not vectorized") >= 0,
        "--jit-remarks-missed alone prints past the cache: {tail(out)}")
    t |> equal(-1, find(out, "loop not vectorized:"),
        "--jit-remarks-missed prints no analysis remark: {tail(out)}")
    rc = run_and_capture([das_exe(), "-no-module-cache", "-jit", root, "--",
            "--jit-remarks-analysis=loop-vectorize"], dir, noenv, out, 300.0)
    t |> equal(0, rc, "the fixture runs under -jit --jit-remarks-analysis alone: {tail(out)}")
    t |> success(find(out, "remark: <unknown>:0:0: loop not vectorized: ") >= 0,
        "--jit-remarks-analysis alone prints the analysis remarks past the cache: {tail(out)}")
    rc = run_and_capture(
        [das_exe(), "-no-module-cache", "-jit", root, "--", "--jit-remarks=loop-vectorize",
            "--jit-remarks-missed=loop-vectorize"],
        dir, noenv, out, 300.0)
    t |> equal(0, rc, "the fixture runs under -jit with remarks: {tail(out)}")
    t |> success(find(out, "remark: <unknown>:0:0: vectorized loop") >= 0,
        "the counted loop's remark prints past the cache: {tail(out)}")
    t |> success(find(out, "remark: <unknown>:0:0: loop not vectorized") >= 0,
        "the data-exit loop's missed remark prints: {tail(out)}")
    rc = run_and_capture(
        [das_exe(), "-no-module-cache", "-jit", root, "--", "--jit-debug",
            "--jit-remarks-missed=loop-vectorize"],
        dir, noenv, out, 300.0)
    t |> equal(0, rc, "the fixture runs under -jit --jit-debug with remarks: {tail(out)}")
    t |> success(find(out, "_remarks_root.das:18:") >= 0,
        "under --jit-debug a remark names the loop's .das line: {tail(out)}")
    t |> equal(-1, find(out, "vectorized loop"), "only the missed remarks print: {tail(out)}")
    rmdir_rec(dir, terr)
}

let private SECOND_CONTEXT_FIXTURE = "modules/dasLLVM/tests/_second_context_root.das"

typedef HolesTwice = tuple<compiled : bool; jit_ok : bool; first : int; second : int>

//! the fixture's hole counts from two contexts of one program: the
//! first runs the JIT, generating the DLL into `out_dir`, the second
//! simulates the program again, as dastest's `--together` does
def private holes_twice(out_dir : string) : HolesTwice {
    var res = (compiled = false, jit_ok = false, first = -1, second = -1)
    ast_gc_guard() {
        var inscope access <- make_file_access("")
        access |> add_extra_module("llvm_jit_finalizer",
            "{get_das_root()}/modules/dasLLVM/daslib/llvm_jit_finalizer.das")
        using() $(var mg : ModuleGroup) {
            using() $(var cop : CodeOfPolicies) {
                cop.jit_enabled = true
                cop.version_2_syntax = true
                cop.aot_module = true
                cop.threadlock_context = true
                cop.jit_dll_mode = true
                cop.jit_output_path := "{out_dir}/second_context"
                let root = "{get_das_root()}/{SECOND_CONTEXT_FIXTURE}"
                compile_file(root, access, unsafe(addr(mg)), cop) $(ok, program, _errors) {
                    return if (!ok || program == null)
                    res.compiled = true
                    for (round in range(2)) {
                        simulate(program) $(sok, context, _serrors) {
                            return if (!sok)
                            if (round == 0) {
                                let jit = run_jit(get_ptr(program), get_ptr(context),
                                    JitCliOptions())
                                res.jit_ok = jit.ok
                            }
                            var holes = -1
                            unsafe {
                                invoke_in_context(context, "literal_holes", addr(holes))
                            }
                            if (round == 0) {
                                res.first = holes
                            } else {
                                res.second = holes
                            }
                        }
                    }
                }
            }
        }
    }
    return res
}

[test(description = "a second context of one program, installing the JIT code the first generated, fills every nested literal at init as the first context did", timeout = 1200)]
def test_second_context_literals(t : T?) {
    var terr : string
    let tmp = create_temp_directory("llvm_second_context", terr)
    if (empty(tmp)) {
        t |> failure("create_temp_directory: {terr}")
        return
    }
    let r = holes_twice(tmp)
    var err : string
    rmdir_rec(tmp, err)
    if (!r.compiled) {
        t |> failure("the fixture {SECOND_CONTEXT_FIXTURE} did not compile")
        return
    }
    t |> run("the first context runs the JIT and its init fills every nested literal") @(t : T?) {
        t |> success(r.jit_ok, "run_jit over the fixture")
        t |> equal(r.first, 0, "holes in the context that generated the code")
    }
    t |> run("a second context's init fills them the same: the AST kept its layout") @(t : T?) {
        t |> equal(r.second, 0, "holes in the context that installed the cached code")
    }
}
