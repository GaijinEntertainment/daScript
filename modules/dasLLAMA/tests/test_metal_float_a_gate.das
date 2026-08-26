options gen2
options stack = 524288

require dastest/testing_boost public
require daslib/fio
require daslib/strings_boost

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
    let rc = run_and_capture([self, "-compile-only", path], out, 120.0)
    rmdir_rec_result(dir)
    return (rc = rc, out = out)
}

[test]
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

