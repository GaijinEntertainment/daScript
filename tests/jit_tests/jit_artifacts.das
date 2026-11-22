options gen2
//! no_aot: every test here drives the JIT or a JIT-built artifact
options no_aot

require daslib/ast
require daslib/fio
require strings
require dastest/testing_boost
require daslib/strings_boost
require daslib/clargs
require _jit_children

// Tests JIT DLL content-addressed caching:
// - recompile with identical source        -> cache hit  (same DLL
// filename) - recompile with different source        -> cache miss
// (new filename, old artifacts GC'd) Runs only under -jit on a
// dll-build binary; in-process via compile_file + simulate.


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
            fprint(f, "def through_block(blk : block<(x : int) : int>) : int \{\n")
            fprint(f, "    return invoke(blk, 20) + 1\n")
            fprint(f, "\}\n")
            fprint(f, "let g_from_block = through_block() $(x) \{ return x * 2 \}\n")
            fprint(f, "[export]\n")
            fprint(f, "def main() \{\n")
            fprint(f, "    var seen = -1\n")
            fprint(f, "    with_job_que() \{\n")
            fprint(f, "        with_channel(1) $(ch) \{\n")
            fprint(f, "            new_job() @() \{\n")
            fprint(f, "                let v = read_marker() * 1000 + g_from_block\n")
            fprint(f, "                ch |> push_clone(IntVal(v = v))\n")
            fprint(f, "                ch |> notify_and_release\n")
            fprint(f, "            \}\n")
            fprint(f, "            ch |> for_each_clone() $(val : IntVal#) \{\n")
            fprint(f, "                seen = val.v\n")
            fprint(f, "            \}\n")
            fprint(f, "        \}\n")
            fprint(f, "    \}\n")
            fprint(f, "    print(\"CLONE_INIT_MARKER=[\{seen / 1000\}]\\n\")\n")
            fprint(f, "    print(\"CLONE_BLOCK_GLOBAL=[\{seen % 1000\}]\\n\")\n")
            fprint(f, "    print(\"MAIN_BLOCK_GLOBAL=[\{g_from_block\}]\\n\")\n")
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
    t |> success(find(output, "MAIN_BLOCK_GLOBAL=[41]") != -1,
        "a global initialized through a block: {output}")
    t |> success(find(output, "CLONE_BLOCK_GLOBAL=[41]") != -1,
        "the same global in a cloned context: {output}")
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

[test(description = "JIT-compiled executables for hello world, builtin global init, an init and a block-initialized global in a cloned context, and a function-pointer global run and print the expected output", timeout = 1200)]
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

def private is_wasm_object(path : string) : bool {
    var magic : array<uint8>
    magic |> resize(4)
    var read = 0
    fopen(path, "rb") $(f) {
        if (f != null) {
            read = fread(f, magic)
        }
    }
    return (read == 4 && magic[0] == 0u8 && magic[1] == uint8('a') && magic[2] == uint8('s')
        && magic[3] == uint8('m'))
}


def private build_cross_lib(stem, triple : string; var lines : array<string>) : int {
    mkdir_rec(OUTPUT_DIR)
    let bin = get_host_binary()
    return spawn("cd \"{get_das_root()}\" && \"{bin}\" -lib \"{FIXTURE_DIR}/_jit_lib_probe.das\" " +
        "-output \"{stem}\" -- --jit-target={triple} --jit-static " +
        "--jit-lib-bindings \"{stem}_c.das\"", lines)
}


[test(description = "a wasm64 -lib build writes one wasm object and a header naming its emcc link, ignores --jit-static and --jit-lib-bindings, and a wasm32 library is refused", timeout = 1200)]
def test_jit_lib_wasm(tt : T?) {
    if (!jit_enabled()) {
        tt |> success(true, "-lib needs the LLVM backend")
        return
    }
    tt |> run("a wasm64 library is one wasm object and its header") @(t : T?) {
        let stem = path_join(OUTPUT_DIR, "jit_lib_wasm64")
        var lines : array<string>
        let rc = build_cross_lib(stem, "wasm64-unknown-emscripten", lines)
        let output = join(lines, "\n")
        t |> equal(rc, 0, output)
        t |> success(is_wasm_object("{stem}.o"), "{stem}.o is a wasm object:\n{output}")
        t |> success(!fexist(static_artifact(stem)) && !fexist(shared_artifact(stem)),
            "no host archive or shared library is written")
        t |> success(!fexist("{stem}_c.das"), "no dasbind bindings are written")
        t |> success(find(output, "--jit-static ignored") >= 0, output)
        t |> success(find(output, "--jit-lib-bindings names a shared library") >= 0, output)
        let header = fread("{stem}.h")
        t |> success(find(header, "-fwasm-exceptions") >= 0 && find(header, "-sMEMORY64=2") >= 0
            && find(header, "liblibDaScript_runtime.a") >= 0, header)
    }
    tt |> run("a wasm32 library is refused") @(t : T?) {
        let stem = path_join(OUTPUT_DIR, "jit_lib_wasm32")
        var lines : array<string>
        let rc = build_cross_lib(stem, "wasm32-unknown-emscripten", lines)
        let output = join(lines, "\n")
        t |> success(rc != 0, output)
        t |> success(find(output, "64-bit only") >= 0, output)
        t |> success(!fexist("{stem}.o"), "no object is written")
    }
}


def private elf_machine(path : string) : int {
    var head : array<uint8>
    head |> resize(20)
    var read = 0
    fopen(path, "rb") $(f) {
        if (f != null) {
            read = fread(f, head)
        }
    }
    let elf = (head[0] == 0x7Fu8 && head[1] == uint8('E') && head[2] == uint8('L')
        && head[3] == uint8('F'))
    return -1 if (read != 20 || !elf)
    return int(head[18]) | (int(head[19]) << 8)
}


let ELF_AARCH64 = int(0xB7)


[test(description = "an aarch64 Android -lib build writes one aarch64 ELF object and its header, and a 32-bit arm library is refused", timeout = 1200)]
def test_jit_lib_android(tt : T?) {
    if (!jit_enabled()) {
        tt |> success(true, "-lib needs the LLVM backend")
        return
    }
    tt |> run("an aarch64 library is one aarch64 object and its header") @(t : T?) {
        let stem = path_join(OUTPUT_DIR, "jit_lib_aarch64")
        var lines : array<string>
        let rc = build_cross_lib(stem, "aarch64-linux-android", lines)
        let output = join(lines, "\n")
        t |> equal(rc, 0, output)
        t |> equal(elf_machine("{stem}.o"), ELF_AARCH64, output)
        t |> success(fexist("{stem}.h"), "the C header is written")
        t |> success(!fexist(static_artifact(stem)) && !fexist(shared_artifact(stem)),
            "no host archive or shared library is written")
        t |> success(!fexist("{stem}_c.das"), "no dasbind bindings are written")
    }
    tt |> run("a 32-bit arm library is refused") @(t : T?) {
        let stem = path_join(OUTPUT_DIR, "jit_lib_armv7")
        var lines : array<string>
        let rc = build_cross_lib(stem, "armv7-linux-androideabi", lines)
        t |> success(rc != 0, join(lines, "\n"))
        t |> success(!fexist("{stem}.o"), "no object is written")
    }
}
