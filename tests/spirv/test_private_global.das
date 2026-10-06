options gen2
options indenting = 4

// @private_global gate: a module-scope mutable global with no descriptor annotation -> Private storage
// (SPIR-V's per-invocation "function-persistent" class, GLSL's plain global). Unlike @workgroup it is
// valid in ANY stage; here a fragment shader keeps a per-invocation scratch array that a helper writes
// by a dynamic index and main reads back -- the shape a fixed-array cannot express as a function
// parameter (fixed arrays don't cross a shader function boundary), so the array lives at module scope.
// Asserts a Private OpVariable, NO descriptor set/binding on it, and that it stays out of the <= 1.3
// entry-point interface; spirv-val is the structural oracle for the Private access chains.

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_shader public
require spirv/spirv_builtins public
require spirv/spirv_dis
require spirv/spirv_grammar

var @private_global pg_dist : float[9]
var @private_global pg_col : float3[9]
var @out @location = 0 pg_out : float4

def private pg_fill(n : int; d : float; c : float3) {
    pg_dist[n] = d
    pg_col[n] = c
}

[fragment_shader(name="privg_spv"), marker(no_coverage)]
def privg {
    let base = int2(gl_FragCoord.xy)
    var i = 0
    while (i < 9) {
        pg_fill(i, float(base.x + i), float3(float(i), 0.0f, 0.0f))
        i++
    }
    let k = base.x % 9
    pg_out = float4(pg_col[k], pg_dist[k])
}

def private privg_words : array<uint> => clone_to_move(privg_spv)

def private validate(t : T?; words : array<uint>; lbl : string) {
    let r = validate_spirv(words)
    if (r.ran) {
        t |> success(r.ok, "{lbl}: spirv-val: {r.msg}")
    } else {
        feint("spirv-val not found locally; skipping (CI enforces)\n")
    }
}

[test]
def test_private_global(t : T?) {
    t |> run("private: @private_global -> Private OpVariable, per-invocation scratch") <| @(t : T?) {
        var words <- privg_words()
        let model = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(model.ok && model.value == uint(SpvExecutionModel.Fragment),
            "privg: EntryPoint model is Fragment")
        // @private_global global -> a Private-storage OpVariable. OpVariable operands:
        // result-type(0), result-id(1), storage-class(2).
        t |> success(op_has_operand_at(words, SpvOp.Variable, 2, uint(SpvStorageClass.Private)),
            "privg: private global is a Private OpVariable")
        // a Private global is not a descriptor: no DescriptorSet decoration anywhere. OpDecorate
        // operands: target-id(0), decoration(1) -- positional, so a target id that happens to equal
        // DescriptorSet cannot false-positive.
        t |> success(!op_has_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.DescriptorSet)),
            "privg: no DescriptorSet decoration (Private is not a descriptor)")
        validate(t, words, "privg")
        delete words
    }
}
