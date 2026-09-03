options gen2
options no_aot

require dastest/testing_boost
require daslib/fio
require daslib/strings_boost
require strings


let OUTPUT_DIR = "{get_das_root()}/build/tests"
let LIB_SCRIPT = "{OUTPUT_DIR}/jit_lib_probe.das"
let LIB_OUT = "{OUTPUT_DIR}/jit_lib_probe"
let BAD_SCRIPT = "{OUTPUT_DIR}/jit_lib_bad.das"
let BAD_OUT = "{OUTPUT_DIR}/jit_lib_bad"
let ALIGN_SCRIPT = "{OUTPUT_DIR}/jit_lib_align.das"
let ALIGN_OUT = "{OUTPUT_DIR}/jit_lib_align"
let BIND_SCRIPT = "{OUTPUT_DIR}/bindlib.das"
let BIND_OUT = "{OUTPUT_DIR}/bindlib"
let BINDER_SCRIPT = "{OUTPUT_DIR}/jit_lib_binder.das"
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
    let args <- get_command_line_arguments()
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
    let args <- get_command_line_arguments()
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
    if (!jit_enabled()) {
        t |> success(true, "-lib lives in the LLVM backend")
        return
    }
    let args <- get_command_line_arguments()
    let bin = args[0]
    t |> success(write_align_script(ALIGN_SCRIPT), "cannot write the alignment script")
    var lines : array<string>
    spawn("\"{bin}\" -lib \"{ALIGN_SCRIPT}\" -output \"{ALIGN_OUT}\" -- --jit-dump", lines)
    t |> success(lines |> has("%result = alloca [24 x i8], align 8"),
        "the returned structure gets its own alloca at its own alignment")
    t |> success(!(lines |> has("alloca \{ i32, [24 x i8] }")),
        "the returned structure is not a field of the argument frame")
}


def private write_bind_library(path : string) : bool {
    var body = "options gen2\n"
    body += "struct Pair \{\n    lo : int\n    hi : int\n\}\n"
    body += "var g_calls = 0\n"
    body += "[export_c]\ndef bump() : int \{ g_calls++; return g_calls \}\n"
    body += "[export_c]\ndef widen(p : Pair) : Pair \{ return Pair(lo = p.lo - 1, hi = p.hi + 1) \}\n"
    body += "[export_c]\ndef greet(who : string) : string \{ return \"hola \{who}\" \}\n"
    body += "[export_c(name = \"double_or_raise\")]\ndef boom(n : int) : int \{ if (n < 0) \{ panic(\"negative: \{n}\") \}; return n * 2 \}\n"
    return write_script(path, body)
}


def private write_binder(path, lib : string) : bool {
    var ok = false
    fopen(path, "w") $(f) {
        if (f != null) {
            fprint(f, "options gen2\nrequire dasbind\nrequire daslib/safe_addr\n")
            fprint(f, "struct Pair \{\n    lo : int\n    hi : int\n\}\n")
            let externs <- [
                ("bindlib_create", "bind_create() : void? \{ return null \}"),
                ("bindlib_destroy", "bind_destroy(ctx : void?) : void \{\}"),
                ("bindlib_last_error", "bind_last_error(ctx : void?) : string \{ return \"\" \}"),
                ("bindlib_bump", "bind_bump(ctx : void?) : int \{ return 0 \}"),
                ("bindlib_widen", "bind_widen(ctx, p, out : void?) : void \{\}"),
                ("bindlib_greet", "bind_greet(ctx : void?; who : string) : string \{ return \"\" \}"),
                ("bindlib_double_or_raise", "bind_double(ctx : void?; n : int) : int \{ return 0 \}")
            ]
            for (ext in externs) {
                fprint(f, "[extern(cdecl, late, name=\"{ext._0}\", library=\"{lib}\", linux_library=\"{lib}\", macos_library=\"{lib}\", windows_library=\"{lib}\")]\ndef {ext._1}\n")
            }
            fprint(f, "[export]\ndef main() \{\n")
            fprint(f, "    var ctx = bind_create()\n")
            fprint(f, "    if (ctx == null) \{ print(\"create failed: \{bind_last_error(null)}\\n\"); return \}\n")
            fprint(f, "    var p = Pair(lo = 10, hi = 20)\n    var out = Pair(lo = 0, hi = 0)\n")
            fprint(f, "    unsafe \{ bind_widen(ctx, reinterpret<void?>(safe_addr(p)), reinterpret<void?>(safe_addr(out))) \}\n")
            fprint(f, "    let raised = bind_double(ctx, -5)\n    let err = bind_last_error(ctx)\n")
            fprint(f, "    print(\"BUMP=\{bind_bump(ctx)}\{bind_bump(ctx)} WIDEN=\{out.lo},\{out.hi} GREET=\{bind_greet(ctx, \"mundo\")} DOUBLE=\{bind_double(ctx, 21)} RAISED=\{raised} ERR=\{err} AFTER=\{bind_double(ctx, 3)}\\n\")\n")
            fprint(f, "    bind_destroy(ctx)\n    print(\"BIND OK\\n\")\n\}\n")
            ok = true
        }
    }
    return ok
}


[test]
def test_jit_lib_binds_into_a_daslang_host(t : T?) {
    if (!jit_enabled() || !das_is_dll_build()) {
        t |> success(true, "-lib needs the LLVM backend and a shared runtime to link against")
        return
    }
    let args <- get_command_line_arguments()
    let bin = args[0]
    t |> success(write_bind_library(BIND_SCRIPT), "cannot write the bind library source")
    var build_lines : array<string>
    let build_rc = spawn("\"{bin}\" -lib \"{BIND_SCRIPT}\" -output \"{BIND_OUT}\"", build_lines)
    t |> success(build_rc == 0, "the bind library builds")
    let artifact = shared_artifact(BIND_OUT)
    t |> success(fexist(artifact), "the shared library exists at {artifact}")
    t |> success(write_binder(BINDER_SCRIPT, artifact), "cannot write the binder script")
    var lines : array<string>
    let rc = spawn("\"{bin}\" \"{BINDER_SCRIPT}\"", lines)
    t |> success(rc == 0, "the binder script exits clean")
    t |> success(lines |> has("BIND OK"), "the binder reached the end")
    t |> success(lines |> has("BUMP=12"), "a global persists across two calls")
    t |> success(lines |> has("WIDEN=9,21"), "a structure crosses in and out")
    t |> success(lines |> has("GREET=hola mundo"), "a string crosses in and out")
    t |> success(lines |> has("DOUBLE=42"), "a scalar crosses in and out")
    t |> success(lines |> has("RAISED=0"), "a raising call answers zero")
    t |> success(lines |> has("ERR=negative: -5"), "the panic text reaches the caller")
    t |> success(lines |> has("AFTER=6"), "the instance still works after a panic")
}


[test]
def test_jit_lib_refuses_unrepresentable(t : T?) {
    if (!jit_enabled()) {
        t |> success(true, "-lib lives in the LLVM backend")
        return
    }
    let args <- get_command_line_arguments()
    let bin = args[0]
    let body = "options gen2\n[export_c]\ndef takes_array(a : array<int>) : int \{ return length(a) \}\n"
    t |> success(write_script(BAD_SCRIPT, body), "cannot write the refusal script")
    var err : string
    remove(shared_artifact(BAD_OUT), err)
    var lines : array<string>
    let rc = spawn("\"{bin}\" -lib \"{BAD_SCRIPT}\" -output \"{BAD_OUT}\"", lines)
    t |> success(lines |> has("has no C representation"), "the refusal names the type C cannot spell")
    t |> success(!fexist(shared_artifact(BAD_OUT)), "no library is written when an export is refused")
    t |> success(rc != 0, "a refused export fails the build")

    let empty_body = "options gen2\ndef nothing() \{\}\n"
    t |> success(write_script(EMPTY_SCRIPT, empty_body), "cannot write the empty script")
    var empty_lines : array<string>
    let empty_rc = spawn("\"{bin}\" -lib \"{EMPTY_SCRIPT}\" -output \"{EMPTY_OUT}\"", empty_lines)
    t |> success(empty_lines |> has("nothing to export"), "a program with no exports says so")
    t |> success(empty_rc != 0, "a program with no exports fails the build")
}
