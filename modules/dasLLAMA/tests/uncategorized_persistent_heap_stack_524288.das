options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require _daslang_argv   // daslang_argv, child_disable_flags: a spawned child's module flags
require ../harness/class_mint_text.das
require daslib/json
require daslib/json_boost
require daslib/command_line
require daslib/fio
require daslib/strings_boost
require strings

// The supervisor cancel-minting rail, end to end in a child process: while the file named by
// DAS_TUNE_CONTROL exists, interrupt_gate() aborts between kernel families - the @tune abort
// event fires and nothing past the gate runs (minting is end-gated, so an interrupted run
// writes no sidecar). The env is load-once, so a spawned child is the only honest probe.

def private run_probe(control_env : string) : string {
    let win = get_platform_name() == "windows"
    // an empty control_env is spelled as an explicit UNSET - the parent (the watchdog sets
    // this very variable) must never leak its own value into the "unset" cell
    let envs = (empty(control_env)
        ? (win ? "set \"DAS_TUNE_CONTROL=\"&& " : "env -u DAS_TUNE_CONTROL ")
        : (win ? "set \"DAS_TUNE_CONTROL={control_env}\"&& "
               : "DAS_TUNE_CONTROL=\"{control_env}\" "))
    let exe = "\"{get_das_exe()}\"{child_disable_flags()}"
    let cmd = "{envs}{exe} \"modules/dasLLAMA/tests/_interrupt_probe.das\" -dasroot \"{get_das_root()}\" 2>&1"
    var out = ""
    unsafe(popen(cmd) $(f) {
        if (f != null) {
            out = build_string() $(w) {
                while (!feof(f)) {
                    w |> write(fgets(f))
                }
            }
        }
    })
    return out
}

[test(description = "In a spawned child, an existing DAS_TUNE_CONTROL file makes `interrupt_gate` fire the tune abort event and stop before anything past the gate runs, while a named-but-absent file or an unset variable passes through", timeout = 1200)]
def test_interrupt_aborts_without_minting(t : T?) {
    t |> run("a present control file aborts at the gate") @(tt : T?) {
        let res = create_temp_file_result("tune_control_", ".stop")
        if (res is error) {
            tt |> failure("could not create a temp control file")
            return
        }
        let control = res as value
        let out = run_probe(control)
        tt |> success(out |> find("PROBE: before gate") >= 0, "the probe ran to the gate: {out}")
        tt |> success(out |> find("@tune abort reason=interrupted at=probe_family") >= 0, "the abort event fired: {out}")
        tt |> success(out |> find("gate passed through") < 0, "nothing past the gate ran - minting is end-gated, so nothing was written: {out}")
        remove(control)
    }
    t |> run("a named-but-absent control file passes through") @(tt : T?) {
        let res = create_temp_file_result("tune_control_", ".stop")
        if (res is error) {
            tt |> failure("could not create a temp control file")
            return
        }
        let control = res as value
        remove(control)
        let out = run_probe(control)
        tt |> success(out |> find("gate passed through") >= 0, "the gate is armed only while the file EXISTS: {out}")
        tt |> success(out |> find("@tune abort") < 0, "no abort without the file: {out}")
    }
    t |> run("no DAS_TUNE_CONTROL passes through") @(tt : T?) {
        let out = run_probe("")
        tt |> success(out |> find("gate passed through") >= 0, "unset env never interrupts: {out}")
        tt |> success(out |> find("@tune abort") < 0, "no abort with the env unset: {out}")
    }
}

// The float-A tmm2d gate (msl_emit): an UNLICENSED float A operand in any tmm2d form is a
// compile error - the half twin is measured 8-11% faster per GEMM, so a new family must
// either stamp a float16 XT twin or declare the f32 fallback deliberate with
// [metal_kernel(float_a_ok=true)]. Spawned compile-only fixtures, both directions.

def private fixture_source(flag : string) : string {
    return build_string() $(var w) {
        w |> write("options gen2\n")
        w |> write("require metal/msl_shader\n")
        w |> write("require metal/metal_builtins\n")
        w |> write("class FloatAFx \{\n")
        w |> write("    @ssbo @binding = 0 wsh : array<float16>\n")
        w |> write("    @ssbo @binding = 1 wqb : array<int8>\n")
        w |> write("    @ssbo @binding = 2 xf : array<float>\n")
        w |> write("    @ssbo @binding = 3 y : array<float>\n")
        w |> write("    @uniform @binding = 4 kdim : uint\n")
        w |> write("    @uniform @binding = 5 ndim : uint\n")
        w |> write("    @workgroup twb : float16[2048]\n")
        w |> write("    [metal_kernel{flag}]\n")
        w |> write("    def fx_kernel \{\n")
        w |> write("        let mBase = gl_WorkGroupID.x * 32u\n")
        w |> write("        let nBase = gl_WorkGroupID.y * 64u\n")
        w |> write("        var sp = unsafe(addr(wsh[nBase * (kdim / 32u) * 17u]))\n")
        w |> write("        var wp = unsafe(addr(wqb[nBase * (kdim / 32u) * 34u + 2u]))\n")
        w |> write("        var ap = unsafe(addr(xf[mBase * kdim]))\n")
        w |> write("        var cp = unsafe(addr(y[mBase * ndim + nBase]))\n")
        w |> write("        tmm2d_q8u_f32(32u, 64u, 4u, twb, sp, wp, ap, kdim, cp, ndim, kdim, kdim / 32u, gl_LocalInvocationIndex)\n")
        w |> write("    \}\n")
        w |> write("\}\n")
        w |> write("[export]\n")
        w |> write("def main \{\n")
        w |> write("    print(FloatAFx_fx_kernel_msl)\n")
        w |> write("\}\n")
    }
}

def private spawn_compile(flag, tag : string) : tuple<rc : int; out : string> {
    let dir_r = create_temp_directory_result("metal_float_a_{tag}")
    if (!(dir_r is value)) {
        return (rc = -1, out = "temp dir failed")
    }
    let dir = unsafe(dir_r.value)
    let path = path_join(dir, "prog.das")
    if (!fwrite(path, fixture_source(flag))) {
        return (rc = -1, out = "fwrite failed")
    }
    let argv <- get_command_line_arguments()
    let self = find(to_generic_path(argv[0]), "/") < 0 ? argv[0] : get_full_file_name(normalize(argv[0]))
    var out : string
    var inscope compile <- daslang_argv(self, ["-compile-only", path])
    let rc = run_and_capture(compile, out, 120.0)
    rmdir_rec_result(dir)
    return (rc = rc, out = out)
}

[test(description = "A Metal kernel feeding a float A operand to a tmm2d form fails to compile with an error naming the FLOAT A rule, and the same kernel compiles once marked `float_a_ok=true`", timeout = 1200)]
def test_float_a_gate(t : T?) {
    t |> run("an unlicensed float A operand does not compile") @(t : T?) {
        let r = spawn_compile("", "bare")
        t |> success(r.rc != 0, "bare float-A fixture must fail (rc={r.rc})")
        t |> success(find(r.out, "FLOAT A operand") >= 0, "the failure names the rule: {slice(r.out, 0, 400)}")
    }
    t |> run("float_a_ok licenses the deliberate f32 fallback") @(t : T?) {
        let r = spawn_compile("(float_a_ok=true)", "licensed")
        t |> equal(0, r.rc, "licensed fixture compiles: {slice(r.out, 0, 400)}")
    }
}

[test(description = "class_mint's text is one JSON object: the profile and the sidecar embedded whole where the mint finished, the error alone where a step failed, a quote in the reason escaped")]
def test_class_mint_text(t : T?) {
    var err = ""
    var done = read_json(mint_result_text("\{\"kernels\": \{\"k\": \"nt4\"\}\}", "\{\"race\": \{\"k\": 1\}\}"), err)
    t |> success(done != null, "a finished mint's text parses: {err}")
    t |> equal(done?["profile"]?["kernels"]?["k"] ?? "", "nt4", "the profile rides whole")
    t |> equal(done?["sidecar"]?["race"]?["k"] ?? 0.0lf, 1.0lf, "the sidecar rides whole, race tables included")
    t |> equal(done?["error"] ?? "", "", "a finished mint carries no error")
    delete_json(done)
    var failed = read_json(mint_failure_text("the \"race\" failed"), err)
    t |> success(failed != null, "a failed mint's text parses: {err}")
    t |> equal(failed?["error"] ?? "", "the \"race\" failed", "the reason survives its quotes")
    t |> equal(failed?["profile"]?["kernels"]?["k"] ?? "", "", "a failed mint carries no profile")
    delete_json(failed)
}
