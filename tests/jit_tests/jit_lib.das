options gen2
options no_aot

require dastest/testing_boost
require daslib/fio
require daslib/strings_boost
require strings

//! Covers `daslang -lib` end to end: the artifact and the header it writes, which functions reach
//! the C boundary, and that a refused `[export_c]` produces no library at all. The C host that
//! CALLS the result is the `lib_capi` ctest - what a dastest can reach is the build.

let OUTPUT_DIR = "{get_das_root()}/build/tests"
let LIB_SCRIPT = "{OUTPUT_DIR}/jit_lib_probe.das"
let LIB_OUT = "{OUTPUT_DIR}/jit_lib_probe"
let BAD_SCRIPT = "{OUTPUT_DIR}/jit_lib_bad.das"
let BAD_OUT = "{OUTPUT_DIR}/jit_lib_bad"
let ALIGN_SCRIPT = "{OUTPUT_DIR}/jit_lib_align.das"
let ALIGN_OUT = "{OUTPUT_DIR}/jit_lib_align"
let EMPTY_SCRIPT = "{OUTPUT_DIR}/jit_lib_empty.das"
let EMPTY_OUT = "{OUTPUT_DIR}/jit_lib_empty"


def private write_script(path, body : string) : bool {
    var ok = false
    mkdir_rec(OUTPUT_DIR)
    fopen(path, "w") $(f) {
        if (f != null) {
            fprint(f, body)
            ok = true
        }
    }
    return ok
}


def private write_probe_script(path : string) : bool {
    // a lambda and a global are here on purpose: both need the AOT-JIT preparation an artifact
    // build turns on, and a library that could not emit them would fail at codegen, not at a call
    var ok = false
    mkdir_rec(OUTPUT_DIR)
    fopen(path, "w") $(f) {
        if (f != null) {
            fprint(f, "options gen2\n")
            fprint(f, "struct Pair \{\n    lo : int\n    hi : int\n\}\n")
            fprint(f, "var g_seen = 0\n")
            fprint(f, "[export_c]\ndef widen(p : Pair) : Pair \{ g_seen++; return Pair(lo = p.lo - 1, hi = p.hi + 1) \}\n")
            fprint(f, "[export_c(name = \"tally\")]\ndef count_seen() : int \{ return g_seen \}\n")
            fprint(f, "[export_c]\ndef mapped(n : int) : int \{ var fx <- @@(x : int) => x * 3; return invoke(fx, n) \}\n")
            fprint(f, "def helper_public(x : int) : int \{ return x + 1 \}\n")
            ok = true
        }
    }
    return ok
}


def private spawn(cmd : string; var lines : array<string>) : int {
    var rc : int
    unsafe {
        rc = popen_timeout("{cmd} 2>&1", 600.0) $(f) {
            if (f == null) {
                return
            }
            while (!feof(f)) {
                let ln = strip(fgets(f))
                if (!empty(ln)) {
                    lines |> push("{ln}")
                }
            }
        }
    }
    return rc
}


def private has(lines : array<string>; needle : string) : bool {
    for (ln in lines) {
        if (find(ln, needle) >= 0) {
            return true
        }
    }
    return false
}


def private read_all(path : string) : string {
    var text = ""
    fopen(path, "rb") $(f) {
        if (f != null) {
            text = fread(f)
        }
    }
    return text
}


def private shared_artifact(stem : string) : string {
    let plat = get_platform_name()
    if (plat == "windows") {
        return "{stem}.dll"
    }
    return plat == "darwin" ? "{stem}.dylib" : "{stem}.so"
}


def private static_artifact(stem : string) : string {
    return get_platform_name() == "windows" ? "{stem}.lib" : "{stem}.a"
}


[test]
def test_jit_lib_shared(t : T?) {
    if (!jit_enabled()) {
        t |> success(true, "-lib lives in the LLVM backend")
        return
    }
    let args <- get_command_line_arguments()   // locked view - do not delete
    let bin = args[0]
    t |> success(write_probe_script(LIB_SCRIPT), "cannot write the probe script")
    var lines : array<string>
    let rc = spawn("\"{bin}\" -lib \"{LIB_SCRIPT}\" -output \"{LIB_OUT}\"", lines)
    t |> success(rc == 0, "daslang -lib exits clean")
    t |> success(lines |> has("LLVM LIB: C entry points generated"), "the C entry points are announced")
    t |> success(fexist("{LIB_OUT}.h"), "the C header is written beside the library")
    if (das_is_dll_build()) {
        t |> success(fexist(shared_artifact(LIB_OUT)), "the shared library is written")
    }

    let header = read_all("{LIB_OUT}.h")
    t |> success(find(header, "jit_lib_probe_ctx * jit_lib_probe_create(void);") >= 0, "create is declared")
    t |> success(find(header, "void jit_lib_probe_widen(jit_lib_probe_ctx * ctx, const jit_lib_probe_Pair * p, jit_lib_probe_Pair * out);") >= 0,
        "a struct in and out crosses by pointer")
    t |> success(find(header, "int32_t jit_lib_probe_tally(jit_lib_probe_ctx * ctx);") >= 0, "the [export_c] name is used")
    t |> success(find(header, "int32_t jit_lib_probe_mapped(jit_lib_probe_ctx * ctx, int32_t n);") >= 0,
        "a function using a lambda still exports")
    t |> success(find(header, "helper_public") < 0, "a bare public function is not exported by default")
}


[test]
def test_jit_lib_static_and_export_all(t : T?) {
    if (!jit_enabled()) {
        t |> success(true, "-lib lives in the LLVM backend")
        return
    }
    let args <- get_command_line_arguments()   // locked view - do not delete
    let bin = args[0]
    t |> success(write_probe_script(LIB_SCRIPT), "cannot write the probe script")
    var lines : array<string>
    let rc = spawn("\"{bin}\" -lib \"{LIB_SCRIPT}\" -output \"{LIB_OUT}_s\" -lib-export-all -- --jit-lib-static", lines)
    t |> success(rc == 0, "a static export-all build exits clean")
    t |> success(fexist(static_artifact("{LIB_OUT}_s")), "the archive is written")
    let header = read_all("{LIB_OUT}_s.h")
    t |> success(find(header, "int32_t jit_lib_probe_helper_public(") >= 0, "export-all reaches a bare public function")
    t |> success(find(header, "Link this archive") >= 0, "the header says what a static host links")
}


def private write_align_script(path : string) : bool {
    var ok = false
    mkdir_rec(OUTPUT_DIR)
    fopen(path, "w") $(f) {
        if (f != null) {
            fprint(f, "options gen2\n")
            fprint(f, "struct Wide \{\n    a : int8\n    b : int64\n    c : float\n\}\n")
            fprint(f, "[export_c]\ndef make(n : int) : Wide \{ return Wide(a = int8(n), b = int64(n), c = float(n)) \}\n")
            ok = true
        }
    }
    return ok
}


[test]
def test_jit_lib_result_slot_is_aligned(t : T?) {
    //! A returned structure is built in an alloca of its OWN alignment, never in a slot of the
    //! argument frame: the frame would align an [N x i8] result to 1, and the das body then stores
    //! through it at the structure's real alignment - a `Wide` here writes its int64 at what LLVM
    //! is told is an 8-aligned address while the frame put it at 4 mod 8.
    if (!jit_enabled()) {
        t |> success(true, "-lib lives in the LLVM backend")
        return
    }
    let args <- get_command_line_arguments()   // locked view - do not delete
    let bin = args[0]
    t |> success(write_align_script(ALIGN_SCRIPT), "cannot write the alignment script")
    var lines : array<string>
    spawn("\"{bin}\" -lib \"{ALIGN_SCRIPT}\" -output \"{ALIGN_OUT}\" -- --jit-dump", lines)
    t |> success(lines |> has("%result = alloca [24 x i8], align 8"),
        "the returned structure gets its own alloca at its own alignment")
    t |> success(!(lines |> has("alloca \{ i32, [24 x i8] }")),
        "the returned structure is not a field of the argument frame")
}


[test]
def test_jit_lib_refuses_unrepresentable(t : T?) {
    if (!jit_enabled()) {
        t |> success(true, "-lib lives in the LLVM backend")
        return
    }
    let args <- get_command_line_arguments()   // locked view - do not delete
    let bin = args[0]
    let body = "options gen2\n[export_c]\ndef takes_array(a : array<int>) : int \{ return length(a) \}\n"
    t |> success(write_script(BAD_SCRIPT, body), "cannot write the refusal script")
    var err : string
    remove(shared_artifact(BAD_OUT), err)
    var lines : array<string>
    let rc = spawn("\"{bin}\" -lib \"{BAD_SCRIPT}\" -output \"{BAD_OUT}\"", lines)
    t |> success(lines |> has("has no C representation"), "the refusal names the type C cannot spell")
    t |> success(!fexist(shared_artifact(BAD_OUT)), "no library is written when an export is refused")
    // a build rule reads the exit code, and a silent success would let it link yesterday's artifact
    t |> success(rc != 0, "a refused export fails the build")

    let empty_body = "options gen2\ndef nothing() \{\}\n"
    t |> success(write_script(EMPTY_SCRIPT, empty_body), "cannot write the empty script")
    var empty_lines : array<string>
    let empty_rc = spawn("\"{bin}\" -lib \"{EMPTY_SCRIPT}\" -output \"{EMPTY_OUT}\"", empty_lines)
    t |> success(empty_lines |> has("nothing to export"), "a program with no exports says so")
    t |> success(empty_rc != 0, "a program with no exports fails the build")
}
