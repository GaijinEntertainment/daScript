options gen2
options no_aot

require daslib/ast
require daslib/fio
require strings
require dastest/testing_boost
require daslib/strings_boost
require daslib/clargs
require jit
require math

// Tests JIT DLL content-addressed caching:
// - recompile with identical source        -> cache hit  (same DLL
// filename) - recompile with different source        -> cache miss
// (new filename, old artifacts GC'd) Runs only under -jit on a
// dll-build binary; in-process via compile_file + simulate.


let OUTPUT_DIR = test_temp_dir()
let PROBE_PATH = "{OUTPUT_DIR}/dll_cache_probe.das"

def write_probe(table_value : int) : bool {
    // Probe uses table[k] so jit_table_at_tString lands in the emitted
    // globals, exercising the codegen path that #2802 fixed (DLLExport
    // linkage on the table accessor slots so ResolveExternVisitor can fix
    // them up on reload).
    mkdir_rec(OUTPUT_DIR)
    var ok = false
    fopen(PROBE_PATH, "w") $(f) {
        if (f != null) {
            fprint(f, "options gen2\n")
            fprint(f, "require daslib/just_in_time\n\n")
            fprint(f, "[jit, sideeffects, export]\n")
            fprint(f, "def probe(x : int) : int \{\n")
            fprint(f, "    var tab : table<string; int>\n")
            fprint(f, "    tab[\"a\"] = {table_value}\n")
            fprint(f, "    return x + tab[\"a\"]\n")
            fprint(f, "\}\n")
            fprint(f, "\n[export]\ndef main() \{\n    let _ = probe(0)\n\}\n")
            ok = true
        }
    }
    return ok
}

def compile_probe() : string {
    var ns : string
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.jit_enabled = true
            cop.jit_dll_mode = true
            access |> add_extra_module("just_in_time", "{get_das_root()}/daslib/just_in_time.das")
            compile_file(PROBE_PATH, access, unsafe(addr(mg)), cop) $(ok, program, _errors) {
                if (ok && program != null) {
                    ns = string(program.thisNamespace)
                    simulate(program) $(_sok, _context, _serrors) {}
                }
            }
        }
    }
    return ns
}

def list_dlls(subdir : string) : array<string> {
    var res : array<string>
    dir(subdir) $(fname) {
        if (fname |> ends_with(".dll")) {
            res |> push(fname)
        }
    }
    return <- res
}

def clear_subdir(subdir : string) {
    dir(subdir) $(fname) {
        return if (fname == "." || fname == "..")
        var err : string
        remove("{subdir}/{fname}", err)
    }
}

def test_dll_cache(t : T?) {
    return if (!jit_enabled() || !das_is_dll_build())
    t |> equal(write_probe(1), true)
    let ns = compile_probe()
    t |> equal(empty(ns), false)
    let subdir = ".cache/daslang/{ns}"
    clear_subdir(subdir)

    // compile 1: cache miss, one DLL appears
    compile_probe()
    let dlls1 <- list_dlls(subdir)
    t |> equal(length(dlls1), 1)
    let first_dll = dlls1[0]

    // compile 2 with same source: cache hit, same filename, count still 1
    compile_probe()
    let dlls2 <- list_dlls(subdir)
    t |> equal(length(dlls2), 1)
    t |> strictEqual(dlls2[0], first_dll, "cache hit keeps same DLL filename")

    // compile 3 with different body: miss, new DLL, old artifacts GC'd
    t |> equal(write_probe(2), true)
    compile_probe()
    let dlls3 <- list_dlls(subdir)
    t |> equal(length(dlls3), 1)
    t |> equal(dlls3[0] != first_dll, true)
    rmdir_rec(subdir)
}

[test(description = "the JIT DLL cache reuses its DLL for an unchanged script and compiles a new one when the script changes")]
def test_main(t : T?) {
    t |> run("dll_cache") @(t : T?) {
        test_dll_cache(t)
    }
}

// This file tests creating JIT executable.


let SCRIPT_PATH = "{OUTPUT_DIR}/jit_exe_test_hello.das"
let EXE_PATH = "{OUTPUT_DIR}/jit_exe_test_hello"
let GLOBAL_INIT_SCRIPT = "{OUTPUT_DIR}/jit_exe_test_global_init.das"
let GLOBAL_INIT_EXE = "{OUTPUT_DIR}/jit_exe_test_global_init"
let CLONE_INIT_SCRIPT = "{OUTPUT_DIR}/jit_exe_test_clone_init.das"
let CLONE_INIT_EXE = "{OUTPUT_DIR}/jit_exe_test_clone_init"
let FNPTR_GLOBAL_SCRIPT = "{OUTPUT_DIR}/jit_exe_test_fnptr_global.das"
let FNPTR_GLOBAL_EXE = "{OUTPUT_DIR}/jit_exe_test_fnptr_global"

def write_hello_script() : bool {
    var ok = false
    mkdir_rec(OUTPUT_DIR)
    fopen(SCRIPT_PATH, "w") $(f) {
        if (f != null) {
            fprint(f, "options gen2\n")
            fprint(f, "[export] def main() \{ print(\"Hello World!\\n\"); \}\n")
            ok = true
        }
    }
    return ok
}

// Emits a script whose only top-level state is `let MAIN_DIR =
// get_das_root()`, then prints it bracketed so an empty value is
// observable. Reproducer for issue #2582 — without the fix the global
// initializer's call gets registered nowhere, LLVM folds the null
// function-pointer load, init_globals never runs the call, and the
// printed value is empty.
def write_global_init_script() : bool {
    var ok = false
    mkdir_rec(OUTPUT_DIR)
    fopen(GLOBAL_INIT_SCRIPT, "w") $(f) {
        if (f != null) {
            fprint(f, "options gen2\n")
            fprint(f, "let MAIN_DIR = get_das_root()\n")
            fprint(f, "[export] def main() \{ print(\"MAIN_DIR=[\{MAIN_DIR}]\\n\"); \}\n")
            ok = true
        }
    }
    return ok
}

// Cloned-context [init] reproducer. A standalone -exe creates its
// context via jit_create_standalone_ctx (NOT simulate), so the
// runtime initFunctions array is empty; a CLONED context (jobque
// worker) relies entirely on jitInitScript to re-run init. Without
// the fix, jitInitScript registered only init_globals, never the
// [init] functions, so g_init_marker (set ONLY by [init]) stays 0 in
// the clone — the exact failure that silenced cross-compiled dasAudio
// (its limiter is set up by an [init] gated on the audio context,
// which only exists on the clone). Fixed: the clone runs the full
// init -> the value read in the worker is 777, matching the
// interpreter.
def write_clone_init_script() : bool {
    var ok = false
    mkdir_rec(OUTPUT_DIR)
    fopen(CLONE_INIT_SCRIPT, "w") $(f) {
        if (f != null) {
            fprint(f, "options gen2\n")
            fprint(f, "require jobque\n")
            fprint(f, "require daslib/jobque_boost\n")
            fprint(f, "struct IntVal \{ v : int \}\n")
            fprint(f, "var g_init_marker = 0\n")
            fprint(f, "[init]\n")
            fprint(f, "def mark_init() \{ g_init_marker = 777 \}\n")
            fprint(f, "def read_marker() : int \{ return g_init_marker \}\n")
            fprint(f, "[export]\n")
            fprint(f, "def main() \{\n")
            fprint(f, "    var seen = -1\n")
            fprint(f, "    with_job_que() \{\n")
            fprint(f, "        with_channel(1) $(ch) \{\n")
            fprint(f, "            new_job() @() \{\n")
            fprint(f, "                ch |> push_clone(IntVal(v = read_marker()))\n")
            fprint(f, "                ch |> notify_and_release\n")
            fprint(f, "            \}\n")
            fprint(f, "            ch |> for_each_clone() $(val : IntVal#) \{\n")
            fprint(f, "                seen = val.v\n")
            fprint(f, "            \}\n")
            fprint(f, "        \}\n")
            fprint(f, "    \}\n")
            fprint(f, "    print(\"CLONE_INIT_MARKER=[\{seen\}]\\n\")\n")
            fprint(f, "\}\n")
            ok = true
        }
    }
    return ok
}

// A function-pointer global initialized with `@@fn` (an ExprAddr) is
// a UID-bearing node inside a global-variable initializer. A
// standalone -exe codegens those
// (generate_globals_initialization_fn), unlike -jit where the
// interpreter runs global inits. Without the fix, create_uid_nodes
// never gave the ExprAddr an id and get_base panicked "Key not
// found"; with it, the exe runs and prints 42.
def write_fnptr_global_script() : bool {
    var ok = false
    mkdir_rec(OUTPUT_DIR)
    fopen(FNPTR_GLOBAL_SCRIPT, "w") $(f) {
        if (f != null) {
            fprint(f, "options gen2\n")
            fprint(f, "def my_kernel(x : int) : int \{ return x + 1 \}\n")
            fprint(f, "var g_fn = @@my_kernel\n")
            fprint(f,
                   "[export] def main() \{ print(\"FNPTR_RESULT=[\{invoke(g_fn, 41)}]\\n\"); \}\n")
            ok = true
        }
    }
    return ok
}

// Returns true if the standalone .exe was actually produced. Returns
// false when the link step failed — typically on the sanitizer matrix
// (the runtime .so is built with -fsanitize=ubsan/asan/tsan and
// references __ubsan_handle_* etc. that daslang's -exe link command
// doesn't propagate).
def compile_to_exe(t : T?; src : string; out : string) : bool {
    let exe_path = "{out}.exe"
    // Wipe any leftover from a previous run so fexist below reliably
    // reflects whether THIS compile produced a binary.
    if (fexist(exe_path)) {
        remove_result(exe_path)
    }
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.jit_enabled = true
            cop.jit_exe_mode = true
            cop.jit_dll_mode = false
            access |> add_extra_module("just_in_time", "{get_das_root()}/daslib/just_in_time.das")
            cop.jit_output_path := exe_path
            compile_file(src, access, unsafe(addr(mg)), cop) $(ok, program, _errors) {
                t |> equal(ok, true)
                simulate(program) $(sok, _context, _serrors) {
                    t |> equal(sok, true)
                    // No need to execute, we already created binary.
                }
            }
        }
    }
    return fexist(exe_path)
}

def run_exe(exe : string) : string {
    var output = ""
    unsafe(popen("{exe}.exe") $(f) {
        while (!feof(f)) {
            output += fgets(f)
        }
    })
    return output
}

def test_hello_world(t : T?) {
    t |> equal(write_hello_script(), true)
    if (!compile_to_exe(t, SCRIPT_PATH, EXE_PATH)) { return ; }
    t |> equal("Hello World!\n", run_exe(EXE_PATH))
}

// Issue #2582: top-level `let X = builtin()` initializers must
// register their builtins so init_globals can call them. Without the
// fix, `output` would be "MAIN_DIR=[]\n" (the global initializer's
// call is constant-folded away).
def test_global_init_builtin(t : T?) {
    t |> equal(write_global_init_script(), true)
    if (!compile_to_exe(t, GLOBAL_INIT_SCRIPT, GLOBAL_INIT_EXE)) { return ; }
    let output = run_exe(GLOBAL_INIT_EXE)
    t |> success(starts_with(output, "MAIN_DIR=["))
    t |> success(!starts_with(output, "MAIN_DIR=[]"))
}

// A [init] function must run in a CLONED context (jobque worker) in a
// standalone -exe, matching the interpreter. Regression for the
// cross-compiled-dasAudio silence: without the fix the worker reads
// g_init_marker == 0 (clone never ran [init]); with it, 777.
def test_clone_init_in_thread(t : T?) {
    t |> equal(write_clone_init_script(), true)
    if (!compile_to_exe(t, CLONE_INIT_SCRIPT, CLONE_INIT_EXE)) { return ; }
    let output = run_exe(CLONE_INIT_EXE)
    t |> success(find(output, "CLONE_INIT_MARKER=[777]") != -1)
    t |> success(find(output, "CLONE_INIT_MARKER=[0]") == -1)
}

// A function-pointer global (`var g = @@fn`) must codegen + run in a
// standalone -exe. Regression for the "Key not found" / "unresolved
// expression" panic on UID-bearing nodes in global initializers.
def test_fnptr_global(t : T?) {
    t |> equal(write_fnptr_global_script(), true)
    if (!compile_to_exe(t, FNPTR_GLOBAL_SCRIPT, FNPTR_GLOBAL_EXE)) { return ; }
    let output = run_exe(FNPTR_GLOBAL_EXE)
    t |> success(find(output, "FNPTR_RESULT=[42]") != -1)
}

[test(description = "JIT-compiled executables for hello world, builtin global init, an init in a cloned context and a function-pointer global run and print the expected output", timeout = 1200)]
def test_executable(t : T?) {
    // Skip on Windows: standalone exes built here link against the
    // daslang shared lib, and Windows lacks RPATH so the spawned process
    // can't find libDaScript.dll. Skip without daslang JIT or a
    // shared-lib build for the same reason — there's no library to call
    // into at runtime.
    if (get_platform_name() == "windows" || !jit_enabled() || !das_is_dll_build()) { return ; }
    t |> run("hello world") @(t : T?) {
        test_hello_world(t)
    }
    t |> run("global init builtin (issue #2582)") @(t : T?) {
        test_global_init_builtin(t)
    }
    t |> run("[init] runs in cloned context") @(t : T?) {
        test_clone_init_in_thread(t)
    }
    t |> run("function-pointer global (@@fn in global init)") @(t : T?) {
        test_fnptr_global(t)
    }
}

let FIXTURE_DIR = "{get_das_root()}/tests/jit_tests"
let SHARED_FIXTURE = "{get_das_root()}/tests-cpp/big/standalone_ctx/standalone_init_fixture.das"


def private spawn(cmd : string; var lines : array<string>) : int {
    let rc = unsafe(popen_timeout("{cmd} 2>&1", 600.0) $(f) {
        if (f == null) {
            return
        }
        while (!feof(f)) {
            let ln = strip(fgets(f))
            if (!(ln |> empty())) {
                lines |> push("{ln}")
            }
        }
    })
    return rc
}


def private shared_artifact(stem : string) : string {
    let plat = get_platform_name()
    if (plat == "windows") {
        return "{stem}.dll"
    }
    return plat == "darwin" ? "{stem}.dylib" : "{stem}.so"
}


def private static_artifact(stem : string) : string =>
    get_platform_name() == "windows" ? "{stem}.lib" : "{stem}.a"


def private build_lib(bin, source, stem, flags, jit_flags : string;
                      var lines : array<string>) : int {
    mkdir_rec(OUTPUT_DIR)
    return spawn("cd \"{get_das_root()}\" && \"{bin}\" -lib \"{source}\" -output \"{OUTPUT_DIR}/{stem}\" {flags}" +
        " -- {jit_flags} --jit-lib-bindings \"{OUTPUT_DIR}/_{stem}_c.das\"", lines)
}


//! the host is copied beside its generated bindings, because
//! `require` resolves relative to the requiring file - and because
//! writing either into tests/jit_tests would have the sweep reading
//! that directory while this test writes it, which Windows answers
//! with a sharing violation
def private run_host(bin, host : string; var lines : array<string>) : int {
    mkdir_rec(OUTPUT_DIR)
    var copy_error : string
    copy_file("{FIXTURE_DIR}/{host}", "{OUTPUT_DIR}/{host}", true, copy_error)
    if (!empty(copy_error)) {
        lines |> push("cannot stage {host}: {copy_error}")
        return -1
    }
    return spawn("cd \"{get_das_root()}\" && \"{bin}\" \"{OUTPUT_DIR}/{host}\"", lines)
}


def private report(lines : array<string>; what : string) {
    for (ln in lines) {
        to_log(LOG_ERROR, "{what}: {ln}\n")
    }
}


def private lib_tier_ready(t : T?) : bool {
    if (!jit_enabled() || !das_is_dll_build()) {
        to_log(LOG_WARNING, "jit_lib: SKIPPED - -lib needs the LLVM backend and a shared runtime\n")
        t |> success(true, "-lib needs the LLVM backend and a shared runtime to link against")
        return false
    }
    return true
}


[test(description = "daslang -lib builds a shared library with a C header that a daslang host loads and calls correctly", timeout = 1200)]
def test_jit_lib_shared(t : T?) {
    if (!lib_tier_ready(t)) {
        return
    }
    let bin = get_host_binary()
    var lines : array<string>
    let rc = build_lib(bin, "{FIXTURE_DIR}/_jit_lib_probe.das", "jit_lib_probe", "", "", lines)
    report(lines, "build")
    t |> success(rc == 0, "daslang -lib exits clean")
    t |> success(fexist("{OUTPUT_DIR}/jit_lib_probe.h"),
                 "the C header is written beside the library")
    t |> success(fexist(shared_artifact("{OUTPUT_DIR}/jit_lib_probe")),
                 "the shared library is written")
    var host_lines : array<string>
    let host_rc = run_host(bin, "_jit_lib_probe_host.das", host_lines)
    report(host_lines, "host")
    t |> success(host_rc == 0,
        "a struct, a renamed call, a lambda-using body and a table all answer through C")
    var lib = load_dynamic_library(shared_artifact("{OUTPUT_DIR}/jit_lib_probe"))
    t |> success(lib != null, "the shared library loads")
    if (lib != null) {
        t |> success(get_function_address(lib, "jit_lib_probe_tally") != null,
            "jit_lib_probe_tally is exported")
        t |> success(get_function_address(lib, "`jit`table_at`tInt` glob") == null,
            "the int-key table_at slot is not exported")
        t |> success(get_function_address(lib, "__das_clone_init_script") == null,
            "__das_clone_init_script is not exported")
        close_dynamic_library(lib)
    }
}


[test(description = "a static -lib-export-all build writes its archive and exports every function to its host", timeout = 1200)]
def test_jit_lib_static_and_export_all(t : T?) {
    if (!lib_tier_ready(t)) {
        return
    }
    let bin = get_host_binary()
    var static_lines : array<string>
    let static_rc = build_lib(bin, "{FIXTURE_DIR}/_jit_lib_probe.das", "jit_lib_probe_s",
        "-lib-export-all", "--jit-static", static_lines)
    report(static_lines, "static build")
    t |> success(static_rc == 0, "a static export-all build exits clean")
    t |> success(fexist(static_artifact("{OUTPUT_DIR}/jit_lib_probe_s")), "the archive is written")

    var lines : array<string>
    let rc = build_lib(bin, "{FIXTURE_DIR}/_jit_lib_probe.das", "jit_lib_probe_all",
        "-lib-export-all", "", lines)
    report(lines, "build")
    t |> success(rc == 0, "a shared export-all build exits clean")
    var host_lines : array<string>
    let host_rc = run_host(bin, "_jit_lib_export_all_host.das", host_lines)
    report(host_lines, "host")
    t |> success(host_rc == 0,
                 "export-all reaches a bare public function, under the output name's prefix")
}


[test(description = "a structure whose alignment exceeds the argument frame's crosses the library boundary intact", timeout = 1200)]
def test_jit_lib_result_slot_is_aligned(t : T?) {
    if (!lib_tier_ready(t)) {
        return
    }
    let bin = get_host_binary()
    var lines : array<string>
    let rc = build_lib(bin, "{FIXTURE_DIR}/_jit_lib_align.das", "jit_lib_align", "", "", lines)
    report(lines, "build")
    t |> success(rc == 0, "the alignment fixture builds")
    var host_lines : array<string>
    let host_rc = run_host(bin, "_jit_lib_align_host.das", host_lines)
    report(host_lines, "host")
    t |> success(host_rc == 0,
                 "a structure whose alignment exceeds the argument frame's crosses intact")
}


[test(description = "a library's export_c comments reach both the C header and the generated daslang bindings, and a daslang host calls it", timeout = 1200)]
def test_jit_lib_binds_into_a_daslang_host(t : T?) {
    if (!lib_tier_ready(t)) {
        return
    }
    let bin = get_host_binary()
    var lines : array<string>
    let rc = build_lib(bin, "{FIXTURE_DIR}/_jit_lib_bind.das", "bindlib", "", "", lines)
    report(lines, "build")
    t |> success(rc == 0, "the bind library builds")
    t |> success(fexist("{OUTPUT_DIR}/_bindlib_c.das"), "the daslang bindings are written")
    let comment = "// Doubles n.\n// Raises on a negative n.\n"
    t |> success(fread("{OUTPUT_DIR}/bindlib.h") |> find(comment) >= 0,
                 "the C header carries the export_c comment")
    t |> success(fread("{OUTPUT_DIR}/_bindlib_c.das") |> find(comment) >= 0,
                 "the daslang bindings carry the export_c comment")
    var host_lines : array<string>
    let host_rc = run_host(bin, "_jit_lib_bind_host.das", host_lines)
    report(host_lines, "host")
    t |> success(host_rc == 0,
                 "a global, a struct, a string and a panic all behave through the bindings")
}


[test(description = "the AOT standalone fixture also builds as a JIT library and answers the same values", timeout = 1200)]
def test_jit_lib_runs_the_standalone_fixture(t : T?) {
    if (!lib_tier_ready(t)) {
        return
    }
    let bin = get_host_binary()
    var lines : array<string>
    let rc = build_lib(bin, SHARED_FIXTURE, "standalone_init_fixture", "",
                       "--jit-lib-export-marked", lines)
    report(lines, "build")
    t |> success(rc == 0, "the AOT standalone fixture also builds as a JIT library")
    var host_lines : array<string>
    let host_rc = run_host(bin, "_jit_lib_shared_host.das", host_lines)
    report(host_lines, "host")
    t |> success(host_rc == 0, "both tiers answer the same values from the same source")
}


[test(description = "a library reaching its own modules works inside a host runtime that already holds modules", timeout = 1200)]
def test_jit_lib_is_a_guest_of_a_populated_runtime(t : T?) {
    if (!lib_tier_ready(t)) {
        return
    }
    let bin = get_host_binary()
    var lines : array<string>
    let rc = build_lib(bin, "{FIXTURE_DIR}/_jit_lib_guest.das", "jit_lib_guest", "", "", lines)
    report(lines, "build")
    t |> success(rc == 0, "the guest library builds")
    var host_lines : array<string>
    let host_rc = run_host(bin, "_jit_lib_guest_host.das", host_lines)
    report(host_lines, "host")
    t |> success(host_rc == 0, "a library reaching its own modules survives a populated runtime")
}


[test(description = "two JIT libraries with different module sets coexist in one host", timeout = 1200)]
def test_jit_lib_carries_two_module_sets(t : T?) {
    if (!lib_tier_ready(t)) {
        return
    }
    let bin = get_host_binary()
    var fio_lines : array<string>
    let fio_rc = build_lib(bin, "{get_das_root()}/tests-cpp/big/standalone_ctx/standalone_modules_fixture.das",
        "standalone_modules_fixture", "", "--jit-lib-export-marked", fio_lines)
    report(fio_lines, "fio library")
    t |> success(fio_rc == 0, "the AOT modules fixture also builds as a JIT library")
    var guest_lines : array<string>
    let guest_rc = build_lib(bin, "{FIXTURE_DIR}/_jit_lib_guest.das", "jit_lib_guest", "", "",
                             guest_lines)
    report(guest_lines, "ast library")
    t |> success(guest_rc == 0, "the ast library builds")
    var host_lines : array<string>
    let host_rc = run_host(bin, "_jit_lib_two_module_sets_host.das", host_lines)
    report(host_lines, "host")
    t |> success(host_rc == 0, "two libraries with different module sets answer in one process")
}

// A return, break or continue that leaves through a `finally`
// dispatches back to its continuation through a switch on an index;
// the wasm64 cross compile of that shape emits its object (the
// indirectbr form left IR the wasm backend crashed on), and a native
// run reaches every continuation.


let WASM_FINALLY_FIXTURE = "{get_das_root()}/tests/jit_tests/_wasm_finally_fixture.das"


[test(description = "a native run takes every exit through a finally once, and the wasm64 cross compile of that shape emits its object", timeout = 1200)]
def test_wasm_finally_return(tt : T?) {
    if (!jit_enabled()) {
        tt |> success(true)
        return
    }
    let args <- get_command_line_arguments()  // locked view - do not delete
    let bin = args[0]
    tt |> run("a native run takes every exit through its finally once") @(t : T?) {
        var lines : array<string>
        let rc = spawn_child("\"{bin}\" -jit \"{WASM_FINALLY_FIXTURE}\"", lines)
        let output = join(lines, "\n")
        t |> equal(rc, 0, output)
        // pick: 2 finally runs; loop_exits(10): i = 0..4 enter the block, 4
        // breaks out -> 5 runs
        t |> success(find(output, "values 7 9 3 5 finally 7") >= 0, output)
    }
    tt |> run("the wasm64 cross compile emits its object") @(t : T?) {
        let obj = path_join(test_temp_dir(), "wasm_finally.o")
        var lines : array<string>
        let rc = spawn_child("\"{bin}\" -exe -output \"{obj}\" \"{WASM_FINALLY_FIXTURE}\" " +
            "-- --jit-target=wasm64-unknown-emscripten --jit-emit-object", lines)
        let output = join(lines, "\n")
        t |> equal(rc, 0, output)
        let st = stat(obj)
        t |> success(st.is_reg && st.size > 0ul, "{obj} written:\n{output}")
    }
}

// The signed idot family on the wasm64 cross target lowers to the
// ISA's exact int8 dot halves - the i16x8 extmul products summed by
// i32x4.extadd_pairwise_i16x8_s and folded even/odd - and never to
// the generic widen-multiply chain that runs a fifth as fast there;
// the relaxed 7-bit dot is refused by design (it wraps at -128, and
// the feature string carries no +relaxed-simd - WebKit runs none of
// it), so the dump carries no relaxed intrinsic either. A host
// compile of the same fixture emits no wasm intrinsic. Compile-only
// keeps both compiles on this host and never touches the artifact
// cache, so --jit-dump prints every time.


let WASM_IDOT_FIXTURE = "{get_das_root()}/tests/jit_tests/_wasm_idot_fixture.das"
let PAIRWISE_ADD = "llvm.wasm.extadd.pairwise.signed"
let RELAXED_DOT = "llvm.wasm.relaxed.dot"


def private count_with(lines : array<string>; needle : string) : int {
    var n = 0
    for (ln in lines) {
        if (find(ln, needle) >= 0) {
            n++
        }
    }
    return n
}

[test(description = "the wasm64 cross compile lowers idot4 to the pairwise int8 dot with simd128 and no relaxed-simd, and a host compile emits no wasm intrinsic", timeout = 1200)]
def test_wasm_idot_lowering(t : T?) {
    if (!jit_enabled()) {
        t |> success(true)
        return
    }
    let args <- get_command_line_arguments()  // locked view — do not delete
    let bin = args[0]
    var terr : string
    let dump = path_join(temp_directory(terr), "wasm_idot_{ref_time_ticks()}")
    t |> run("the wasm64 cross compile lowers idot4 to the exact pairwise int8 dot") @(t : T?) {
        var out : array<string>
        spawn_child("\"{bin}\" -jit \"{WASM_IDOT_FIXTURE}\" -- " +
            "--jit-target=wasm64-unknown-emscripten "
            + "--jit-compile-only --jit-dump=\"{dump}.wasm\"", out)
        let ln <- split(fread("{dump}.wasm"), "\n")
        let pairs = count_with(ln, PAIRWISE_ADD)
        t |> success(pairs >= 3, "the declaration and the two half calls of {PAIRWISE_ADD} are in the dump ({pairs} lines, {length(ln)} lines total)")
        t |> success(count_with(ln, RELAXED_DOT) == 0, "the relaxed 7-bit dot is never emitted")
        t |> success(count_with(ln, "idot_prod") == 0, "no generic widen-multiply chain remains")
        // the unsigned-by-signed form takes the same path, widening its first
        // operand with ZExt: drop that arm and these two halves become the
        // 16-lane chain the line above bans
        let zexts = count_with(ln, "zext <8 x i8>")
        t |> success(zexts >= 2,
                     "both halves of the unsigned operand widen with ZExt ({zexts} lines)")
    }
    t |> run("a host compile emits no wasm intrinsic") @(t : T?) {
        var out : array<string>
        spawn_child("\"{bin}\" -jit \"{WASM_IDOT_FIXTURE}\" -- --jit-compile-only "
            + "--jit-dump=\"{dump}.host\"", out)
        let ln <- split(fread("{dump}.host"), "\n")
        t |> success(length(ln) > 20, "the host dump printed ({length(ln)} lines)")
        t |> success(count_with(ln, "llvm.wasm.") == 0, "no wasm intrinsic on the host")
    }
    t |> run("the wasm object's target features carry simd128 and no relaxed-simd") @(t : T?) {
        let dir_r = create_temp_directory_result("wasm_idot_features")
        if (!(dir_r is value)) {
            t |> failure("temp dir failed")
            return
        }
        let dir = unsafe(dir_r.value)
        let obj = "{dir}/idot.o"
        var ln : array<string>
        spawn_child("\"{bin}\" -exe -output \"{obj}\" \"{WASM_IDOT_FIXTURE}\" -- " +
            "--jit-target=wasm64-unknown-emscripten --jit-emit-object", ln)
        let features = object_target_features(obj)
        t |> success(find(features, "simd128") >= 0,
                     "the object's target_features section names simd128: {features}")
        t |> success(find(features, "relaxed-simd") < 0,
                     "no relaxed opcode can be in a module WebKit must run: {features}")
        rmdir_rec_result(dir)
    }
    remove("{dump}.wasm", terr)
    remove("{dump}.host", terr)
}

// the object's `target_features` custom section as one text line -
// each feature name is ASCII inside a binary file, so the bytes are
// read into an array and every non-printable byte becomes a space
// before the search
def private object_target_features(path : string) : string {
    var bytes : array<uint8>
    fopen(path, "rb") $(f) {
        if (f == null) {
            return
        }
        bytes |> resize(int64(stat(path).size))
        fread(f, bytes)
    }
    let text = build_string() $(w) {
        for (b in bytes) {
            let c = int(b)
            w |> write(c >= 0x20 && c < 0x7f ? to_char(c) : " ")
        }
    }
    let at = find(text, "target_features")
    return at < 0 ? "" : slice(text, at, min(at + 256, length(text)))
}

// The target-side folds: get_target_triple /
// get_target_architecture_name answer for the artifact a compile
// EMITS (the --jit-target triple), the host's values with no triple,
// and a static_if over the triple selects the cross arm (the tune
// framework's inert gate makes the same test), so a wasm or console
// artifact built on a tuned box carries reference bodies and no
// sidecar stamps. Compile-only keeps the cross run on this host: the
// folds are decided from argv before the first module parses, and
// main runs interpreted. The children run with the default module
// cache on purpose: a --jit-target after the separator keys the
// cache, or a cached native compile serves the cross one with the
// host's folds.


let CROSS_TARGET_FIXTURE = "{get_das_root()}/tests/jit_tests/_cross_target_fixture.das"

def private spawn_child(cmd : string; var lines : array<string>) : int {
    var rc : int
    var full = "{cmd} 2>&1"
    if (get_platform_name() == "windows") {
        // cmd.exe strips the first and last quote of a line that opens with
        // one
        full = "\"{full}\""
    }
    rc = unsafe(popen_timeout(full, 300.0) $(f) {
        if (f == null) {
            return
        }
        while (!feof(f)) {
            let ln = strip(fgets(f))
            if (!empty(ln)) {
                lines |> push("{ln}")
            }
        }
    })
    return rc
}

def private has(lines : array<string>; needle : string) : bool {
    for (ln in lines) {
        if (find(ln, needle) >= 0) return true
    }
    return false
}

[test(description = "the target triple and architecture name fold to the cross target's values for a --jit-target compile and to the host's otherwise", timeout = 1200)]
def test_cross_target_folds(t : T?) {
    if (!jit_enabled()) {
        t |> success(true)
        return
    }
    let args <- get_command_line_arguments()  // locked view — do not delete
    let bin = args[0]
    t |> run("a native compile answers the host and keeps the framework live") @(t : T?) {
        var ln : array<string>
        spawn_child("\"{bin}\" -jit \"{CROSS_TARGET_FIXTURE}\"", ln)
        t |> success(ln |> has("TRIPLE=[]"), "no triple on a native compile: {join(ln, " | ")}")
        t |> success(ln |> has("TARGET_ARCH=[{get_architecture_name()}]"),
                     "the target arch is the host's")
        t |> success(ln |> has("CROSS_PLATFORM=[]"), "no cross platform")
        t |> success(ln |> has("CROSS=[no]"), "a native compile takes the native arm")
        t |> success(ln |> has("LET_WASM=[false]"),
                     "a module-level let over the target folds false natively")
    }
    t |> run("a wasm64 cross target answers wasm64 and turns the framework inert") @(t : T?) {
        var ln : array<string>
        spawn_child("\"{bin}\" -jit \"{CROSS_TARGET_FIXTURE}\" -- " +
            "--jit-target=wasm64-unknown-emscripten --jit-compile-only", ln)
        t |> success(ln |> has("TRIPLE=[wasm64-unknown-emscripten]"),
                     "the triple folds: {join(ln, " | ")}")
        t |> success(ln |> has("TARGET_ARCH=[wasm64]"), "the target arch is the triple's")
        t |> success(ln |> has("HOST_ARCH=[{get_architecture_name()}]"),
                     "the host arch stays the host's")
        t |> success(ln |> has("CROSS_PLATFORM=[emscripten]"), "the cross platform is emscripten")
        t |> success(ln |> has("CROSS=[yes]"), "a cross compile takes the cross arm")
        t |> success(ln |> has("LET_WASM=[true]"),
                     "a module-level let over the target folds true for wasm64")
    }
    t |> run("the last --jit-target on the command line wins, as every other argv reader takes it") @(t : T?) {
        var ln : array<string>
        spawn_child("\"{bin}\" -jit \"{CROSS_TARGET_FIXTURE}\" -- " +
            "--jit-target=x86_64-unknown-linux-gnu --jit-target=wasm64-unknown-emscripten " +
                "--jit-compile-only", ln)
        t |> success(ln |> has("TRIPLE=[wasm64-unknown-emscripten]"),
                     "the last triple folds: {join(ln, " | ")}")
    }
    t |> run("an x86_64 linux cross target answers x86_64") @(t : T?) {
        var ln : array<string>
        spawn_child("\"{bin}\" -jit \"{CROSS_TARGET_FIXTURE}\" -- " +
            "--jit-target=x86_64-unknown-linux-gnu --jit-compile-only", ln)
        t |> success(ln |> has("TARGET_ARCH=[x86_64]"),
                     "the target arch is the triple's: {join(ln, " | ")}")
        t |> success(ln |> has("CROSS_PLATFORM=[]"), "linux is not a cross platform name")
        t |> success(ln |> has("CROSS=[yes]"), "any cross target takes the cross arm")
    }
}

//! the shell prefix that makes `dir` the child's working directory;
//! on Windows a plain `cd` stays on the current drive
def private cd_to(dir : string) : string =>
    get_platform_name() == "windows" ? "cd /d \"{dir}\"" : "cd \"{dir}\""

[test(description = "a break inside a try arm of a loop whose recover arm can panic leaves the loop under -jit on a cold cache and again on the warm one", timeout = 1200)]
def test_break_inside_a_try_arm_leaves_the_loop_under_jit(t : T?) {
    if (!jit_enabled()) {
        t |> skip("the child runs under -jit, which this build does not carry")
        return
    }
    let bin = get_host_binary()
    let cwd = "{OUTPUT_DIR}/break_in_try_{ref_time_ticks()}"   // fresh: round one's cache is cold
    mkdir_rec(cwd)
    let fixture = "{FIXTURE_DIR}/_fixture_break_in_try.das"
    let root = get_das_root()
    for (round in range(2)) {
        var inscope lines : array<string>
        let rc = spawn("{cd_to(cwd)} && \"{bin}\" -dasroot \"{root}\" -jit \"{fixture}\"", lines)
        let out = join(lines, "\n")
        t |> equal(0, rc, "round {round}: the child runs under -jit, got:\n{out}")
        t |> success(lines |> has("BREAK_IN_TRY 2"),
                     "round {round}: the loop leaves at i == 2, got:\n{out}")
    }
}

[test(description = "a load through an odd int? that a parity test guards is skipped under -jit: the pointer parameter carries no alignment the fold could use", timeout = 1200)]
def test_guarded_load_through_an_odd_pointer_is_skipped(t : T?) {
    if (!jit_enabled()) {
        t |> skip("the child runs under -jit, which this build does not carry")
        return
    }
    let bin = get_host_binary()
    let fixture = "{FIXTURE_DIR}/_fixture_misaligned_pointer.das"
    let cwd = "{OUTPUT_DIR}/misaligned_pointer"
    mkdir_rec(cwd)
    var lines : array<string>
    let flags = "-dasroot \"{get_das_root()}\" -no-module-cache -jit -jit-no-cache"
    let rc = spawn("{cd_to(cwd)} && \"{bin}\" {flags} \"{fixture}\"", lines)
    let out = join(lines, "\n")
    t |> equal(0, rc, "the guarded load through an odd pointer is skipped, got:\n{out}")
}

def private launder_calls(dump : string) : int {
    let ln <- split(fread(dump), "\n")
    return count_with(ln, "call ptr @llvm.launder.invariant.group")
}

[test(description = "under solid_context the address of a global array a loop indexes, shared or not, reaches code generation hidden in the one-module and the split build, and a scalar global's does not", timeout = 1200)]
def test_solid_array_base_is_hidden_from_codegen(t : T?) {
    if (!jit_enabled()) {
        t |> skip("the child runs under -jit, which this build does not carry")
        return
    }
    let bin = get_host_binary()
    let root = get_das_root()
    let cwd = "{OUTPUT_DIR}/solid_base_{ref_time_ticks()}"   // fresh: a cold cache
    mkdir_rec(cwd)
    var out : array<string>
    let fixture = "{FIXTURE_DIR}/_fixture_solid_array_base.das"
    let rc_one = spawn_child("\"{bin}\" -no-module-cache -jit -jit-no-cache \"{fixture}\" "
        + "-- --jit-dump=\"{cwd}/one.ll\"", out)
    t |> equal(rc_one, 0, "the one-module child runs, got:\n{join(out, "\n")}")
    t |> equal(launder_calls("{cwd}/one.ll"), 2, "the one-module build hides the two arrays alone")
    var split_out : array<string>
    let rc_split = spawn("{cd_to(cwd)} && \"{bin}\" -dasroot \"{root}\" -no-module-cache "
        + "-jit \"{fixture}\" -- --jit-dump=split.ll", split_out)
    t |> equal(rc_split, 0, "the split child runs, got:\n{join(split_out, "\n")}")
    t |> equal(launder_calls("{cwd}/split.ll"), 2, "the split build hides them too")
    rmdir_rec_result(cwd)
}
