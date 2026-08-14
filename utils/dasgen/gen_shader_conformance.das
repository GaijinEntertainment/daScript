options gen2

require daslib/fio
require strings

//! Generates the per-type SHADER conformance harness: for every type row, a dasSpirv
//! `[compute_shader]` fixture test (tests/spirv/test_conf_<name>.das) and a dasGlsl
//! `[fragment_program]` fixture test (tests/glsl/test_conf_<name>.das), exercising locals,
//! ctors, splats, swizzle reads, and (where the type computes) arithmetic — validated by
//! spirv-val / glslang. `reject` rows generate _fail_closed fixtures + a runner per rail
//! (compile must FAIL with the row's needle — the emitters' fail-closed contract).
//! Run: daslang utils/dasgen/gen_shader_conformance.das
//! The generated files are checked in; regenerate whenever the spec table changes.

struct ShaderTypeSpec {
    name : string           // das type name: "float4", "int2", "half4"
    glsl_name : string      // expected GLSL spelling: "vec4", "ivec2", "f16vec4"
    arity : int             // 1 for scalars
    kind : string           // ctor-argument literal spelling: "float" | "int" | "uint" | "double" | "half"; "memzero" = reject-control row
    has_arith : bool        // + - * work on the type in shader code
    spv_mode : string       // "emit" | "reject" | "skip" — the dasSpirv rail's expectation
    glsl_mode : string      // "emit" | "reject" | "skip" — the dasGlsl rail's expectation
    spv_needle : string     // required substring of the SPIR-V emitter diagnostic (reject rows)
    glsl_needle : string    // required substring of the GLSL emitter diagnostic (reject rows)
}

let LANE_LETTERS = fixed_array("x", "y", "z", "w")

def lit(kind : string; v : int) : string {
    if (kind == "float") {
        return "{v}."
    }
    if (kind == "half") {
        return "{v}h"
    }
    if (kind == "double") {
        return "{v}.lf"
    }
    return kind == "uint" ? "{v}u" : "{v}"
}

def ctor_gen(spec : ShaderTypeSpec; lane : block<(i : int) : int>) : string {
    if (spec.arity == 1 && spec.kind == "half") {
        return lit(spec.kind, invoke(lane, 0))      // bare literal: no float16(float16) ctor exists
    }
    if (spec.arity == 1) {
        // scalars whose literal already IS the type (double's `1.lf` too) — a same-type ctor is PERF020 noise
        if (spec.name == "float" || spec.name == "int" || spec.name == "uint" || spec.name == "double") {
            return lit(spec.kind, invoke(lane, 0))
        }
        return "{spec.name}({lit(spec.kind, invoke(lane, 0))})"
    }
    return build_string() $(var writer) {
        writer |> write(spec.name)
        writer |> write("(")
        for (i in range(spec.arity)) {
            if (i != 0) {
                writer |> write(", ")
            }
            writer |> write(lit(spec.kind, invoke(lane, i)))
        }
        writer |> write(")")
    }
}

def ctor(spec : ShaderTypeSpec; base : int) : string {
    return ctor_gen(spec) $(i) => base + i
}

// the shader body shared by both rails: local ctor + splat + (arith) + swizzle read,
// reduced to one scalar so the value flows into the output (nothing dead-code-elided)
def emit_body(var writer; spec : ShaderTypeSpec; seed : string) {
    // storage rows never mutate v (no closed arithmetic) — declare with let
    let vkw = spec.has_arith ? "var" : "let"
    if (spec.arity == 1 && spec.kind == "half") {
        writer |> write("    {vkw} v = 1.5h\n")   // scalar float16 has no float16(float16) ctor
    } else {
        writer |> write("    {vkw} v = {ctor(spec, 1)}\n")
    }
    if (spec.arity > 1) {
        writer |> write("    let s = {spec.name}({seed})\n")
    } elif (spec.name == "float" || spec.name == "int" || spec.name == "uint") {
        writer |> write("    let s = {seed}\n")
    } else {
        writer |> write("    let s = {spec.name}({seed})\n")    // scalar convert, e.g. float16(f32)
    }
    if (spec.has_arith) {
        writer |> write("    v = v + s\n")
        writer |> write("    v = v * {ctor_gen(spec) $(_i) => 2}\n")
        writer |> write("    v = v - s\n")
        if (spec.arity > 1) {
            let last = LANE_LETTERS[spec.arity - 1]
            writer |> write("    let r = v.x + v.{last}\n")
        } else {
            writer |> write("    let r = v\n")
        }
    } else {
        // storage rows: consume BOTH the literal ctor and the runtime-convert seed — s is the
        // convert-coverage half; widen the elements first (storage scalars have no operators)
        let wide = spec.kind == "uint" ? "uint" : "int"
        if (spec.arity > 1) {
            writer |> write("    let r = {wide}(v.x) + {wide}(s.x)\n")
        } else {
            writer |> write("    let r = {wide}(v) + {wide}(s)\n")
        }
    }
}

// fp16 and fp64 rows need the sink narrowed to the float SSBO element (float16 / double r -> float
// elem); storage rows pre-widen to int/uint and 32-bit arith rows already produce the element type
// (PERF020: same-type cast)
def ssbo_sink(spec : ShaderTypeSpec) : string {
    return (spec.kind == "half" || spec.kind == "double") ? "{ssbo_elem(spec)}(r)" : "r"
}

def ssbo_elem(spec : ShaderTypeSpec) : string {
    if (spec.kind == "uint") {
        return "uint"
    }
    return spec.kind == "int" ? "int" : "float"
}

def emit_spirv_test(spec : ShaderTypeSpec) {
    let path = get_das_root() + "/tests/spirv/test_conf_{spec.name}.das"
    let t0 = ref_time_ticks()
    to_log(LOG_INFO, "[gen_shader_conformance] START {path}\n")
    fopen(path, "wb") $(f) {
        let body = build_string() $(var writer) {
            writer |> write("// GENERATED by utils/dasgen/gen_shader_conformance.das — DO NOT EDIT\n")
            writer |> write("// regen: daslang utils/dasgen/gen_shader_conformance.das\n")
            writer |> write("options gen2\n")
            writer |> write("options indenting = 4\n")
            writer |> write("\n")
            writer |> write("require dastest/testing_boost public\n")
            writer |> write("require _spirv_common\n")
            writer |> write("require spirv/spirv_shader\n")
            writer |> write("require spirv/spirv_dis\n")
            writer |> write("require spirv/spirv_grammar\n")
            writer |> write("\n")
            writer |> write("var @ssbo @binding = 0 conf_{spec.name}_data : array<{ssbo_elem(spec)}>\n")
            writer |> write("\n")
            writer |> write("[compute_shader(local_size_x=64, name=\"conf_{spec.name}_spv\"), marker(no_coverage)]\n")
            writer |> write("def conf_{spec.name}_kernel \{\n")
            writer |> write("    let gi = gl_GlobalInvocationID.x\n")
            emit_body(writer, spec, "conf_{spec.name}_data[gi]")
            writer |> write("    conf_{spec.name}_data[gi] = {ssbo_sink(spec)}\n")
            writer |> write("\}\n")
            writer |> write("\n")
            writer |> write("[test]\n")
            writer |> write("def test_conf_{spec.name}_spirv(t : T?) \{\n")
            writer |> write("    t |> run(\"conformance {spec.name}: emits and validates\") <| @(t : T?) \{\n")
            writer |> write("        var words <- clone_to_move(conf_{spec.name}_spv)\n")
            writer |> write("        t |> success(length(words) > 5, \"has words\")\n")
            writer |> write("        t |> equal(words[0], SPV_MAGIC)\n")
            writer |> write("        t |> equal(count_op(words, SpvOp.EntryPoint), 1)\n")
            writer |> write("        let r = validate_spirv(words)\n")
            writer |> write("        if (r.ran) \{\n")
            writer |> write("            t |> success(r.ok, \"spirv-val: \{r.msg\}\")\n")
            writer |> write("        \} else \{\n")
            writer |> write("            feint(\"spirv-val not found locally; skipping (CI enforces)\\n\")\n")
            writer |> write("        \}\n")
            writer |> write("        delete words\n")
            writer |> write("    \}\n")
            writer |> write("\}\n")
        }
        f |> fwrite(body)
    }
    to_log(LOG_INFO, "[gen_shader_conformance] DONE {path} ({get_time_usec(t0)}us)\n")
}

// GLSL fragment seed: derive the working value from the varying so nothing constant-folds away
def seed_expr(spec : ShaderTypeSpec) : string {
    if (spec.kind == "float" || spec.kind == "half") {
        return "conf_{spec.name}_uv.x"
    }
    if (spec.kind == "uint") {
        return "uint(conf_{spec.name}_uv.x)"
    }
    return "int(conf_{spec.name}_uv.x)"
}

// reduce the result scalar back to a float lane for the color output
def out_expr(spec : ShaderTypeSpec) : string {
    return spec.kind == "float" ? "r" : "float(r)"
}

// the GL_EXT_* line expected in the guarded extension chain's #else branch ("" = 32-bit row).
// real desktop drivers take the GL_NV_gpu_shader5 / AMD branch instead — same type spellings
def glsl_ext_needle(spec : ShaderTypeSpec) : string {
    let g = spec.glsl_name
    if (g == "float16_t" || starts_with(g, "f16vec")) {
        return "GL_EXT_shader_explicit_arithmetic_types_float16"
    }
    if (g == "int16_t" || g == "uint16_t" || starts_with(g, "i16vec") || starts_with(g, "u16vec")) {
        return "GL_EXT_shader_explicit_arithmetic_types_int16"
    }
    if (g == "int8_t" || g == "uint8_t" || starts_with(g, "i8vec") || starts_with(g, "u8vec")) {
        return "GL_EXT_shader_explicit_arithmetic_types_int8"
    }
    return ""
}

def emit_glsl_test(spec : ShaderTypeSpec) {
    let path = get_das_root() + "/tests/glsl/test_conf_{spec.name}.das"
    let t0 = ref_time_ticks()
    to_log(LOG_INFO, "[gen_shader_conformance] START {path}\n")
    fopen(path, "wb") $(f) {
        let body = build_string() $(var writer) {
            writer |> write("// GENERATED by utils/dasgen/gen_shader_conformance.das — DO NOT EDIT\n")
            writer |> write("// regen: daslang utils/dasgen/gen_shader_conformance.das\n")
            writer |> write("options gen2\n")
            writer |> write("options indenting = 4\n")
            writer |> write("\n")
            writer |> write("require dastest/testing_boost public\n")
            writer |> write("require _glsl_common\n")
            writer |> write("require glsl/glsl_opengl\n")
            writer |> write("\n")
            writer |> write("var @inout conf_{spec.name}_uv : float2\n")
            writer |> write("var @out conf_{spec.name}_color : float4\n")
            writer |> write("\n")
            writer |> write("[fragment_program(name=\"conf_{spec.name}_glsl\")]\n")
            writer |> write("def conf_{spec.name}_fragment \{\n")
            emit_body(writer, spec, seed_expr(spec))
            writer |> write("    conf_{spec.name}_color = float4({out_expr(spec)}, 0., 0., 1.)\n")
            writer |> write("\}\n")
            writer |> write("\n")
            writer |> write("[test]\n")
            writer |> write("def test_conf_{spec.name}_glsl(t : T?) \{\n")
            writer |> write("    t |> run(\"conformance {spec.name}: emits and validates\") <| @(t : T?) \{\n")
            writer |> write("        t |> success(has(conf_{spec.name}_glsl, \"{spec.glsl_name}\"), \"spells {spec.glsl_name}\")\n")
            let extn = glsl_ext_needle(spec)
            if (extn != "") {
                writer |> write("        t |> success(has(conf_{spec.name}_glsl, \"#if defined(GL_NV_gpu_shader5)\"), \"guarded NV chain\")\n")
                writer |> write("        t |> success(has(conf_{spec.name}_glsl, \"{extn}\"), \"EXT line in the #else branch\")\n")
            }
            writer |> write("        let r = validate_glsl(conf_{spec.name}_glsl, \"frag\")\n")
            writer |> write("        if (r.ran) \{\n")
            writer |> write("            t |> success(r.ok, \"glslang: \{r.msg\}\")\n")
            writer |> write("        \} else \{\n")
            writer |> write("            feint(\"glslangValidator not found locally; skipping (CI enforces)\\n\")\n")
            writer |> write("        \}\n")
            writer |> write("    \}\n")
            writer |> write("\}\n")
        }
        f |> fwrite(body)
    }
    to_log(LOG_INFO, "[gen_shader_conformance] DONE {path} ({get_time_usec(t0)}us)\n")
}

def emit_reject_fixture(spec : ShaderTypeSpec; rail : string) {
    let dir = get_das_root() + "/tests/{rail}/_fail_closed"
    mkdir(dir)
    let path = "{dir}/_fc_conf_{spec.name}.das"
    let t0 = ref_time_ticks()
    to_log(LOG_INFO, "[gen_shader_conformance] START {path}\n")
    fopen(path, "wb") $(f) {
        let body = build_string() $(var writer) {
            writer |> write("// GENERATED by utils/dasgen/gen_shader_conformance.das — DO NOT EDIT\n")
            writer |> write("expect 50501\n")
            writer |> write("\n")
            writer |> write("options gen2\n")
            writer |> write("\n")
            if (rail == "spirv") {
                writer |> write("require spirv/spirv_shader\n")
                writer |> write("require spirv/spirv_builtins public\n")
                writer |> write("\n")
                writer |> write("[compute_shader(local_size_x=64, name=\"fc_conf_{spec.name}_spv\")]\n")
                writer |> write("def fc_conf_{spec.name}_kernel \{\n")
            } else {
                writer |> write("require glsl/glsl_opengl\n")
                writer |> write("\n")
                writer |> write("var @inout fc_conf_{spec.name}_uv : float2\n")
                writer |> write("var @out fc_conf_{spec.name}_color : float4\n")
                writer |> write("\n")
                writer |> write("[fragment_program(name=\"fc_conf_{spec.name}_glsl\")]\n")
                writer |> write("def fc_conf_{spec.name}_fragment \{\n")
            }
            // the lattice value must FLOW into the shader output: dasGlsl's fixup runs
            // post-optimization, so an unused local is DCE'd before the emitter can reject it
            // (dasSpirv's patch runs pre-DCE — but keep both rails uniform)
            var elem = "v"
            if (spec.arity > 4) {
                elem = "v.s0"       // xyzw stays <= 4 lanes; .s namespace covers 8/16
            } elif (spec.arity > 1) {
                elem = "v.x"
            }
            if (spec.kind == "memzero") {
                // side-effecting statement on a consumed value — optimizer-proof
                writer |> write("    var v = float4(0., 0., 0., 1.)\n")
                writer |> write("    memzero(v)\n")
            } elif (rail == "glsl" && spec.arity == 1) {
                // scalar lattice consts FOLD (float(1.5h) -> 1.5f at compile time) — seed from
                // the varying so the value is runtime-dependent and survives to the emitter
                writer |> write("    var v = {spec.name}(fc_conf_{spec.name}_uv.x)\n")
            } elif (spec.arity == 1 && spec.kind == "half") {
                writer |> write("    var v = 1.5h\n")          // scalar float16 has no float16(float16) ctor
            } else {
                writer |> write("    var v = {ctor(spec, 1)}\n")
            }
            if (rail == "glsl") {
                if (spec.kind == "memzero") {
                    writer |> write("    fc_conf_{spec.name}_color = v\n")
                } else {
                    writer |> write("    fc_conf_{spec.name}_color = float4(float({elem}), 0., 0., 1.)\n")
                }
            } else {
                if (spec.kind != "memzero") {
                    writer |> write("    let r = {elem}\n")
                }
            }
            writer |> write("\}\n")
            if (rail == "glsl") {
                // the shader-text global must be CONSUMED or the unused fragment function never
                // reaches the annotation's fixup (no emission -> no rejection) — probe-verified
                writer |> write("\n")
                writer |> write("[export]\n")
                writer |> write("def main \{\n")
                writer |> write("    print(\"\{fc_conf_{spec.name}_glsl\}\\n\")\n")
                writer |> write("\}\n")
            }
        }
        f |> fwrite(body)
    }
    to_log(LOG_INFO, "[gen_shader_conformance] DONE {path} ({get_time_usec(t0)}us)\n")
}

// context fixtures: lattice types in interface blocks (UBO / SSBO / push constants) must fail
// closed per context — these pin the per-context diagnostics on both rails
struct ContextSpec {
    name : string
    rail : string
    body : string
    needle : string
    code : int = 50501      // GlslExport/spirv error() surface as 50501; macro_error as 50503
}

def context_specs : array<ContextSpec> {  // nolint:STYLE038 — flat spec table, one entry per fail-closed case
    return <- [
        ContextSpec(name = "_fc_lattice_ubo", rail = "spirv", needle = "field 'h': type tHalf4 is not supported",
            body = build_string() $(var w) {
                w |> write("struct FcUboLat \{\n    a : float4\n    h : half4\n\}\n\n")
                w |> write("var @uniform @binding = 0 fc_u_lat : FcUboLat\n")
                w |> write("var @ssbo @binding = 1 fc_u_out : array<float>\n\n")
                w |> write("[compute_shader(local_size_x=64, name=\"fc_lat_ubo_spv\")]\n")
                w |> write("def fc_lat_ubo_kernel \{\n")
                w |> write("    let gi = gl_GlobalInvocationID.x\n")
                w |> write("    fc_u_out[gi] = fc_u_lat.a.x\n")
                w |> write("\}\n")
            }),
        // _fc_lattice_ssbo was deleted with the 8/16-bit SSBO storage wave: array<half4> SSBO
        // elements are now legal (std430, StorageBuffer16BitAccess) — positive coverage lives in
        // tests/spirv/test_storage_8_16.das. @uniform/@push_constant sub-32 stays fail-closed (std140).
        ContextSpec(name = "_fc_lattice_push", rail = "spirv", needle = "field 's': type tShort2 is not supported",
            body = build_string() $(var w) {
                w |> write("struct FcPushLat \{\n    a : float4\n    s : short2\n\}\n\n")
                w |> write("var @push_constant fc_p_lat : FcPushLat\n")
                w |> write("var @ssbo @binding = 0 fc_p_out : array<float>\n\n")
                w |> write("[compute_shader(local_size_x=64, name=\"fc_lat_push_spv\")]\n")
                w |> write("def fc_lat_push_kernel \{\n")
                w |> write("    let gi = gl_GlobalInvocationID.x\n")
                w |> write("    fc_p_out[gi] = fc_p_lat.a.x\n")
                w |> write("\}\n")
            }),
        ContextSpec(name = "_fc_lattice_ubo", rail = "glsl", needle = "16/8-bit lattice members have no std140 layout",
            body = build_string() $(var w) {
                w |> write("options no_unused_function_arguments = false\n\n")
                w |> write("[unused_argument(program, blockName, size)] def std140_reset(program : uint; blockName : string; size : int) \{\}\n")
                w |> write("[unused_argument(program, blockName, offset, value)] def std140_set(program : uint; blockName : string; offset : uint; value) \{\}\n")
                w |> write("[unused_argument(program, blockName, bindingPoint)] def std140_flush(program : uint; blockName : string; bindingPoint : int) \{\}\n\n")
                w |> write("struct FcUboLat \{\n    a : float4\n    h : half4\n\}\n\n")
                w |> write("var @uniform fc_ug_lat : FcUboLat\n")
                w |> write("var @out fc_ug_color : float4\n\n")
                w |> write("[fragment_program(name=\"fc_lat_ubo_glsl\")]\n")
                w |> write("def fc_lat_ubo_fragment \{\n")
                w |> write("    fc_ug_color = fc_ug_lat.a\n")
                w |> write("\}\n\n")
                w |> write("[export]\ndef main \{\n    print(\"\{fc_lat_ubo_glsl\}\\n\")\n\}\n")
            }),
        // block members must peel array dims to the lattice element (Copilot R2)
        ContextSpec(name = "_fc_lattice_ubo_array", rail = "glsl", needle = "16/8-bit lattice members have no std140 layout",
            body = build_string() $(var w) {
                w |> write("options no_unused_function_arguments = false\n\n")
                w |> write("[unused_argument(program, blockName, size)] def std140_reset(program : uint; blockName : string; size : int) \{\}\n")
                w |> write("[unused_argument(program, blockName, offset, value)] def std140_set(program : uint; blockName : string; offset : uint; value) \{\}\n")
                w |> write("[unused_argument(program, blockName, bindingPoint)] def std140_flush(program : uint; blockName : string; bindingPoint : int) \{\}\n\n")
                w |> write("struct FcUboFaLat \{\n    a : float4\n    h : half4[4]\n\}\n\n")
                w |> write("var @uniform fc_uga_lat : FcUboFaLat\n")
                w |> write("var @out fc_uga_color : float4\n\n")
                w |> write("[fragment_program(name=\"fc_lat_ubo_array_glsl\")]\n")
                w |> write("def fc_lat_ubo_array_fragment \{\n")
                w |> write("    fc_uga_color = fc_uga_lat.a\n")
                w |> write("\}\n\n")
                w |> write("[export]\ndef main \{\n    print(\"\{fc_lat_ubo_array_glsl\}\\n\")\n\}\n")
            }),
        // members hidden behind nested structs must be found by the recursive scan (Copilot R3)
        ContextSpec(name = "_fc_lattice_ubo_nested", rail = "glsl", needle = "16/8-bit lattice members have no std140 layout",
            body = build_string() $(var w) {
                w |> write("options no_unused_function_arguments = false\n\n")
                w |> write("[unused_argument(program, blockName, size)] def std140_reset(program : uint; blockName : string; size : int) \{\}\n")
                w |> write("[unused_argument(program, blockName, offset, value)] def std140_set(program : uint; blockName : string; offset : uint; value) \{\}\n")
                w |> write("[unused_argument(program, blockName, bindingPoint)] def std140_flush(program : uint; blockName : string; bindingPoint : int) \{\}\n\n")
                w |> write("struct FcInnerLat \{\n    h : half4\n\}\n\n")
                w |> write("struct FcUboNestLat \{\n    a : float4\n    inner : FcInnerLat\n\}\n\n")
                w |> write("var @uniform fc_ugn_lat : FcUboNestLat\n")
                w |> write("var @out fc_ugn_color : float4\n\n")
                w |> write("[fragment_program(name=\"fc_lat_ubo_nested_glsl\")]\n")
                w |> write("def fc_lat_ubo_nested_fragment \{\n")
                w |> write("    fc_ugn_color = fc_ugn_lat.a\n")
                w |> write("\}\n\n")
                w |> write("[export]\ndef main \{\n    print(\"\{fc_lat_ubo_nested_glsl\}\\n\")\n\}\n")
            }),
        // loose lattice uniforms must get the designed interface diagnostic from the BIND
        // generator (glsl_opengl), not a typer error on the generated glUniformAny call
        ContextSpec(name = "_fc_lattice_uniform", rail = "glsl", needle = "not supported in shader interfaces", code = 50503,
            body = build_string() $(var w) {
                w |> write("var @uniform fc_lu_lat : half4\n")
                w |> write("var @out fc_lu_color : float4\n\n")
                w |> write("[fragment_program(name=\"fc_lat_uniform_glsl\")]\n")
                w |> write("def fc_lat_uniform_fragment \{\n")
                w |> write("    fc_lu_color = float4(float(fc_lu_lat.x), 0., 0., 1.)\n")
                w |> write("\}\n\n")
                w |> write("[export]\ndef main \{\n    print(\"\{fc_lat_uniform_glsl\}\\n\")\n\}\n")
            }),
        // fixed-array shape: the array dims must peel down to the lattice element (Copilot R1)
        ContextSpec(name = "_fc_lattice_fixed_array", rail = "glsl", needle = "not supported in shader interfaces", code = 50503,
            body = build_string() $(var w) {
                w |> write("var @uniform fc_fa_lat : half4[4]\n")
                w |> write("var @out fc_fa_color : float4\n\n")
                w |> write("[fragment_program(name=\"fc_lat_fixed_array_glsl\")]\n")
                w |> write("def fc_lat_fixed_array_fragment \{\n")
                w |> write("    fc_fa_color = float4(float(fc_fa_lat[0].x), 0., 0., 1.)\n")
                w |> write("\}\n\n")
                w |> write("[export]\ndef main \{\n    print(\"\{fc_lat_fixed_array_glsl\}\\n\")\n\}\n")
            }),
        ContextSpec(name = "_fc_lattice_ssbo", rail = "glsl", needle = "not supported in shader interfaces",
            body = build_string() $(var w) {
                w |> write("var @ssbo fc_sg_lat : array<half4>\n")
                w |> write("var @out fc_sg_color : float4\n\n")
                w |> write("[fragment_program(name=\"fc_lat_ssbo_glsl\")]\n")
                w |> write("def fc_lat_ssbo_fragment \{\n")
                w |> write("    fc_sg_color = float4(float(fc_sg_lat[0].x), 0., 0., 1.)\n")
                w |> write("\}\n\n")
                w |> write("[export]\ndef main \{\n    print(\"\{fc_lat_ssbo_glsl\}\\n\")\n\}\n")
            }),
        // the ES/WebGL2 rail has no GL_EXT_shader_explicit_arithmetic_types — any lattice use
        // rejects (es300=true annotation arg = the test-drivable rail override)
        ContextSpec(name = "_fc_lattice_es300", rail = "glsl", needle = "not available on the ES/WebGL2 rail",
            body = build_string() $(var w) {
                w |> write("var @inout fc_es_uv : float2\n")
                w |> write("var @out fc_es_color : float4\n\n")
                w |> write("[fragment_program(name=\"fc_lat_es300_glsl\", es300=true)]\n")
                w |> write("def fc_lat_es300_fragment \{\n")
                w |> write("    var v = half4(1h, 2h, 3h, 4h)\n")
                w |> write("    fc_es_color = float4(float(v.x), 0., 0., 1.)\n")
                w |> write("\}\n\n")
                w |> write("[export]\ndef main \{\n    print(\"\{fc_lat_es300_glsl\}\\n\")\n\}\n")
            })
    ]
}

def emit_context_fixture(cs : ContextSpec) {
    let dir = get_das_root() + "/tests/{cs.rail}/_fail_closed"
    mkdir(dir)
    let path = "{dir}/{cs.name}.das"
    to_log(LOG_INFO, "[gen_shader_conformance] START {path}\n")
    fopen(path, "wb") $(f) {
        let body = build_string() $(var writer) {
            writer |> write("// GENERATED by utils/dasgen/gen_shader_conformance.das — DO NOT EDIT\n")
            writer |> write("expect {cs.code}\n")
            writer |> write("\n")
            writer |> write("options gen2\n")
            if (cs.rail == "spirv") {
                writer |> write("\nrequire spirv/spirv_shader\nrequire spirv/spirv_builtins public\n\n")
            } else {
                writer |> write("\nrequire glsl/glsl_opengl\n\n")
            }
            writer |> write(cs.body)
        }
        f |> fwrite(body)
    }
    to_log(LOG_INFO, "[gen_shader_conformance] DONE {path}\n")
}

def emit_reject_runner(specs : array<ShaderTypeSpec>; rail : string) {
    let path = get_das_root() + "/tests/{rail}/test_conf_rejects.das"
    let t0 = ref_time_ticks()
    to_log(LOG_INFO, "[gen_shader_conformance] START {path}\n")
    fopen(path, "wb") $(f) {
        let body = build_string() $(var writer) {
            writer |> write("// GENERATED by utils/dasgen/gen_shader_conformance.das — DO NOT EDIT\n")
            writer |> write("// regen: daslang utils/dasgen/gen_shader_conformance.das\n")
            writer |> write("options gen2\n")
            writer |> write("options indenting = 4\n")
            writer |> write("options no_aot     // invokes the compiler at runtime (compile_file) — not AOT-linkable\n")
            writer |> write("\n")
            writer |> write("// Fail-closed conformance rows: each fixture uses one shader-illegal type; the emitter\n")
            writer |> write("// must reject with a clean diagnostic (same contract as tests/spirv/test_fail_closed.das).\n")
            writer |> write("\n")
            writer |> write("require dastest/testing_boost public\n")
            writer |> write("require daslib/ast\n")
            writer |> write("require strings\n")
            writer |> write("\n")
            writer |> write("def private compile_issues(file_path : string) : tuple<ok : bool; issues : string> \{\n")
            writer |> write("    var okFlag = false\n")
            writer |> write("    var issuesText = \"\"\n")
            writer |> write("    var inscope access <- make_file_access(\"\")\n")
            writer |> write("    using() $(var mg : ModuleGroup) \{\n")
            writer |> write("        using() $(var cop : CodeOfPolicies) \{\n")
            writer |> write("            cop.threadlock_context = true\n")
            writer |> write("            compile_file(file_path, access, unsafe(addr(mg)), cop) $(ok, _program, issues) \{\n")
            writer |> write("                okFlag = ok\n")
            writer |> write("                issuesText := issues\n")
            writer |> write("            \}\n")
            writer |> write("        \}\n")
            writer |> write("    \}\n")
            writer |> write("    return (ok = okFlag, issues = issuesText)\n")
            writer |> write("\}\n")
            writer |> write("\n")
            writer |> write("def private check_rejects(t : T?; fixture, needle : string) \{\n")
            writer |> write("    let r = compile_issues(\"\{get_das_root()\}/tests/{rail}/_fail_closed/\{fixture\}.das\")\n")
            writer |> write("    t |> success(!r.ok, \"\{fixture\}: compilation fails (fail-closed)\")\n")
            writer |> write("    t |> success(find(r.issues, needle) >= 0, \"\{fixture\}: error mentions '\{needle\}'\")\n")
            writer |> write("    if (r.ok || find(r.issues, needle) < 0) \{\n")
            writer |> write("        to_log(LOG_ERROR, \"FAIL-CLOSED \{fixture\}: ok=\{r.ok\}, issues=\\n\{r.issues\}\\n\")\n")
            writer |> write("    \}\n")
            writer |> write("\}\n")
            writer |> write("\n")
            writer |> write("[test]\n")
            writer |> write("def test_conf_rejects_{rail}(t : T?) \{\n")
            writer |> write("    t |> run(\"conformance reject rows fail closed\") <| @(t : T?) \{\n")
            for (spec in specs) {
                let mode = rail == "spirv" ? spec.spv_mode : spec.glsl_mode
                if (mode == "reject") {
                    let needle = rail == "spirv" ? spec.spv_needle : spec.glsl_needle
                    writer |> write("        check_rejects(t, \"_fc_conf_{spec.name}\", \"{needle}\")\n")
                }
            }
            var ctx <- context_specs()
            for (cs in ctx) {
                if (cs.rail == rail) {
                    writer |> write("        check_rejects(t, \"{cs.name}\", \"{cs.needle}\")\n")
                }
            }
            delete ctx
            writer |> write("    \}\n")
            writer |> write("\}\n")
        }
        f |> fwrite(body)
    }
    to_log(LOG_INFO, "[gen_shader_conformance] DONE {path} ({get_time_usec(t0)}us)\n")
}

// one row per lattice type. Shader-legal widths EMIT on the SPIR-V rail (types/consts/ctors/
// converts/arith landed) and stay reject on GLSL until its wave; CPU-only widths (8/16-lane)
// reject on both rails with the dedicated diagnostic, permanently.
[unused_argument(enum_name)]
def push_lattice_row(var specs : array<ShaderTypeSpec>; name, glsl_name, kind : string; arity : int; cpu_only, arith : bool; enum_name : string) {
    if (cpu_only) {
        specs |> push(ShaderTypeSpec(name = name, glsl_name = "", arity = arity, kind = kind, has_arith = false,
            spv_mode = "reject", glsl_mode = "reject",
            spv_needle = "is CPU-only", glsl_needle = "is CPU-only"))
    } else {
        specs |> push(ShaderTypeSpec(name = name, glsl_name = glsl_name, arity = arity, kind = kind, has_arith = arith,
            spv_mode = "emit", glsl_mode = "emit",
            spv_needle = "", glsl_needle = ""))
    }
}

def push_lattice_rows(var specs : array<ShaderTypeSpec>) {
    // fp16 family — closed arithmetic
    push_lattice_row(specs, "float16", "float16_t", "half", 1, false, true, "tFloat16")
    push_lattice_row(specs, "half2", "f16vec2", "half", 2, false, true, "tHalf2")
    push_lattice_row(specs, "half3", "f16vec3", "half", 3, false, true, "tHalf3")
    push_lattice_row(specs, "half4", "f16vec4", "half", 4, false, true, "tHalf4")
    push_lattice_row(specs, "half8", "", "half", 8, true, false, "tHalf8")
    // pre-lattice small scalars — STORAGE types (error 30230: no numeric operators), convert-only
    push_lattice_row(specs, "int8", "int8_t", "int", 1, false, false, "tInt8")
    push_lattice_row(specs, "uint8", "uint8_t", "uint", 1, false, false, "tUInt8")
    push_lattice_row(specs, "int16", "int16_t", "int", 1, false, false, "tInt16")
    push_lattice_row(specs, "uint16", "uint16_t", "uint", 1, false, false, "tUInt16")
    // int lattice vectors — storage + converts, no closed arithmetic
    push_lattice_row(specs, "short2", "i16vec2", "int", 2, false, false, "tShort2")
    push_lattice_row(specs, "short3", "i16vec3", "int", 3, false, false, "tShort3")
    push_lattice_row(specs, "short4", "i16vec4", "int", 4, false, false, "tShort4")
    push_lattice_row(specs, "short8", "", "int", 8, true, false, "tShort8")
    push_lattice_row(specs, "ushort2", "u16vec2", "uint", 2, false, false, "tUShort2")
    push_lattice_row(specs, "ushort3", "u16vec3", "uint", 3, false, false, "tUShort3")
    push_lattice_row(specs, "ushort4", "u16vec4", "uint", 4, false, false, "tUShort4")
    push_lattice_row(specs, "ushort8", "", "uint", 8, true, false, "tUShort8")
    push_lattice_row(specs, "byte2", "i8vec2", "int", 2, false, false, "tByte2")
    push_lattice_row(specs, "byte3", "i8vec3", "int", 3, false, false, "tByte3")
    push_lattice_row(specs, "byte4", "i8vec4", "int", 4, false, false, "tByte4")
    push_lattice_row(specs, "byte8", "", "int", 8, true, false, "tByte8")
    push_lattice_row(specs, "byte16", "", "int", 16, true, false, "tByte16")
    push_lattice_row(specs, "ubyte2", "u8vec2", "uint", 2, false, false, "tUByte2")
    push_lattice_row(specs, "ubyte3", "u8vec3", "uint", 3, false, false, "tUByte3")
    push_lattice_row(specs, "ubyte4", "u8vec4", "uint", 4, false, false, "tUByte4")
    push_lattice_row(specs, "ubyte8", "", "uint", 8, true, false, "tUByte8")
    push_lattice_row(specs, "ubyte16", "", "uint", 16, true, false, "tUByte16")
}

[export]
def main {
    // W0: 32-bit controls prove the harness (incl. the glslang gate) before any lattice
    // feature work; `double` proves the reject rail end to end on both emitters.
    // Lattice rows land in W1 (reject) and flip to emit as the feature waves ship.
    var specs <- [
        ShaderTypeSpec(name = "float", glsl_name = "float", arity = 1, kind = "float", has_arith = true,
            spv_mode = "emit", glsl_mode = "emit", spv_needle = "", glsl_needle = ""),
        ShaderTypeSpec(name = "float2", glsl_name = "vec2", arity = 2, kind = "float", has_arith = true,
            spv_mode = "emit", glsl_mode = "emit", spv_needle = "", glsl_needle = ""),
        ShaderTypeSpec(name = "float4", glsl_name = "vec4", arity = 4, kind = "float", has_arith = true,
            spv_mode = "emit", glsl_mode = "emit", spv_needle = "", glsl_needle = ""),
        ShaderTypeSpec(name = "int", glsl_name = "int", arity = 1, kind = "int", has_arith = true,
            spv_mode = "emit", glsl_mode = "emit", spv_needle = "", glsl_needle = ""),
        ShaderTypeSpec(name = "int2", glsl_name = "ivec2", arity = 2, kind = "int", has_arith = true,
            spv_mode = "emit", glsl_mode = "emit", spv_needle = "", glsl_needle = ""),
        ShaderTypeSpec(name = "uint", glsl_name = "uint", arity = 1, kind = "uint", has_arith = true,
            spv_mode = "emit", glsl_mode = "emit", spv_needle = "", glsl_needle = ""),
        ShaderTypeSpec(name = "uint4", glsl_name = "uvec4", arity = 4, kind = "uint", has_arith = true,
            spv_mode = "emit", glsl_mode = "emit", spv_needle = "", glsl_needle = ""),
        // double proves the scalar fp64 arithmetic rail: dasSpirv lowers it to OpTypeFloat 64 (a GLSL
        // dvec3 closure ports as three doubles); dasGlsl's fp64 support is not gated here (skip)
        ShaderTypeSpec(name = "double", glsl_name = "", arity = 1, kind = "double", has_arith = true,
            spv_mode = "emit", glsl_mode = "skip", spv_needle = "", glsl_needle = ""),
        ShaderTypeSpec(name = "memzero", glsl_name = "", arity = 1, kind = "memzero", has_arith = false,
            spv_mode = "skip", glsl_mode = "reject", spv_needle = "", glsl_needle = "memzero is not supported")
    ]
    // 16/8-bit lattice: shader-legal rows EMIT on SPIR-V (this arc's waves 3+4), stay reject on
    // GLSL until its wave; the 8/16-lane forms are PERMANENT rejects (CPU-only widths)
    push_lattice_rows(specs)
    for (spec in specs) {
        if (spec.spv_mode == "emit") {
            emit_spirv_test(spec)
        } elif (spec.spv_mode == "reject") {
            emit_reject_fixture(spec, "spirv")
        }
        if (spec.glsl_mode == "emit") {
            emit_glsl_test(spec)
        } elif (spec.glsl_mode == "reject") {
            emit_reject_fixture(spec, "glsl")
        }
    }
    var ctx <- context_specs()
    for (cs in ctx) {
        emit_context_fixture(cs)
    }
    delete ctx
    emit_reject_runner(specs, "spirv")
    emit_reject_runner(specs, "glsl")
    to_log(LOG_INFO, "[gen_shader_conformance] generated {length(specs)} row(s)\n")
    delete specs
    return true
}
