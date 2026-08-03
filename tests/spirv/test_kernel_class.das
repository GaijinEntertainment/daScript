options gen2
options indenting = 4

// [spirv_kernel] class-frontend gate: the a*b class kernel vs its module-globals twin (same body)
// — same word count, identical per-opcode counts (ids permute only: members classify in field
// order, globals in reference order), spirv-val clean on both, and the class blob's reflection
// carries the two bindings + the push range. A second class exercises the @workgroup /
// @private_global member classifications.

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_shader
require spirv/spirv_dis
require spirv/spirv_grammar
require spirv/spirv_reflect

struct KcArgs {
    n : uint
    gain : float
}

class KcKern {
    @ssbo @binding = 0 xs : array<float>
    @ssbo @binding = 1 ys : array<float>
    @push_constant pa : KcArgs

    [spirv_kernel(local_size_x=64, name="kc_spv")]
    def kc {
        let i = gl_GlobalInvocationID.x
        if (i < pa.n) {
            ys[i] = xs[i] * pa.gain
        }
    }
}

// the twin: the same body over module globals — the equivalence target
var @ssbo @binding = 0 g_xs : array<float>
var @ssbo @binding = 1 g_ys : array<float>
var @push_constant g_pa : KcArgs

[compute_shader(local_size_x=64, name="kc_twin_spv")]
def kc_twin {
    let i = gl_GlobalInvocationID.x
    if (i < g_pa.n) {
        g_ys[i] = g_xs[i] * g_pa.gain
    }
}

class KwKern {
    @ssbo @binding = 0 acc : array<float>
    @workgroup tile : float[64]
    @private_global scratch : float

    [spirv_kernel(local_size_x=64, name="kw_spv")]
    def kw {
        let li = gl_LocalInvocationID.x
        tile[li] = acc[gl_GlobalInvocationID.x]
        barrier()
        scratch = tile[li ^ 1u]
        acc[gl_GlobalInvocationID.x] = scratch + tile[li]
    }
}

class KmKern {
    @ssbo @binding = 0 xs : array<float>
    @ssbo @binding = 1 ys : array<float>

    def gain : float {
        return 3.0
    }

    def scale(v : float) : float {
        return v * gain()
    }

    [spirv_kernel(local_size_x=64, name="km_spv")]
    def km {
        let i = gl_GlobalInvocationID.x
        ys[i] = scale(xs[i])
    }
}

def private opcode_counts(words : array<uint>) : table<uint; int> {
    var counts : table<uint; int>
    foreach_inst(words) $(opcode : uint; _w : array<uint>; _s, _n : int) {
        counts[opcode]++
    }
    return <- counts
}

def private check_valid(t : T?; words : array<uint>; tag : string) {
    let r = validate_spirv(words)
    if (r.ran) {
        t |> success(r.ok, "{tag} spirv-val: {r.msg}")
    } else {
        feint("spirv-val not found locally; skipping (CI enforces)\n")
    }
}

[test]
def test_kernel_class_shape(t : T?) {
    t |> run("class kernel: opcode shape + decorations") <| @(t : T?) {
        t |> equal(kc_spv[0], SPV_MAGIC)
        t |> equal(count_op(kc_spv, SpvOp.EntryPoint), 1)
        t |> equal(count_op(kc_spv, SpvOp.Variable), 4)   // gid (Input) + xs + ys (StorageBuffer) + pa (PushConstant)
        t |> success(op_has_operand_pair(kc_spv, SpvOp.Decorate, uint(SpvDecoration.Binding), 0u), "Binding = 0")
        t |> success(op_has_operand_pair(kc_spv, SpvOp.Decorate, uint(SpvDecoration.Binding), 1u), "Binding = 1")
        t |> success(op_has_operand_pair(kc_spv, SpvOp.ExecutionMode, uint(SpvExecutionMode.LocalSize), 64u), "LocalSize x = 64")
    }
}

[test]
def test_kernel_class_twin_equivalence(t : T?) {
    t |> run("class kernel == module-globals twin (per-opcode counts)") <| @(t : T?) {
        t |> equal(length(kc_spv), length(kc_twin_spv))
        var ca <- opcode_counts(kc_spv)
        var cb <- opcode_counts(kc_twin_spv)
        t |> equal(length(ca), length(cb))
        for (op, n in keys(ca), values(ca)) {
            let nm = SPV_OP_NAMES?[op] ?? "Op?({op})"
            t |> success((cb?[op] ?? 0) == n, "opcode {nm} count matches")
        }
        delete ca
        delete cb
    }
}

[test]
def test_kernel_class_methods(t : T?) {
    t |> run("class kernel: method calls devirtualize to self-dropped OpFunctionCalls") <| @(t : T?) {
        t |> equal(count_op(km_spv, SpvOp.Function), 3)       // main + scale + gain
        t |> equal(count_op(km_spv, SpvOp.FunctionCall), 2)   // km -> scale -> gain
    }
}

[test]
def test_kernel_class_validates(t : T?) {
    t |> run("class kernels: spirv-val clean") <| @(t : T?) {
        check_valid(t, kc_spv, "kc")
        check_valid(t, kw_spv, "kw")
        check_valid(t, kc_twin_spv, "kc_twin")
        check_valid(t, km_spv, "km")
    }
}

[test]
def test_kernel_class_reflection(t : T?) {
    t |> run("class kernel: reflection carries bindings + push range") <| @(t : T?) {
        var refl <- decode_reflection(kc_spv_reflect)
        t |> equal(length(refl.bindings), 2)
        t |> equal(refl.bindings[0].set, 0)
        t |> equal(refl.bindings[0].binding, 0)
        t |> equal(refl.bindings[1].binding, 1)
        t |> equal(length(refl.push_constants), 1)
        t |> equal(refl.push_constants[0].size, 8)
        t |> equal(refl.local_size, int3(64, 1, 1))
        finalize(refl)
    }
}
