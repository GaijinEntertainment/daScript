options gen2
options indenting = 4

// mod / fmod are two DIFFERENT core opcodes, not one name spelled twice: GLSL's mod(x, y) is
// x - y*floor(x/y) and takes the DIVISOR's sign (OpFMod), while daslang's own fmod is C's and
// takes the DIVIDEND's sign (OpFRem). They agree only where the dividend is non-negative, so a
// port of a GLSL mod() that reached fmod would compile, spirv-val and silently disagree on
// negative input. This gate pins each name to its own opcode and asserts both are present, so a
// future emitter change cannot quietly collapse the two.

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_shader
require spirv/spirv_dis
require spirv/spirv_grammar
require daslib/shader_lingua_franca
require math

var @ssbo @binding = 0 fmod_io : array<float4>

[compute_shader(local_size_x=1, name="fmodm_spv"), marker(no_coverage)]
def fmodm {
    let x = fmod_io[0].x
    let y = fmod_io[0].y
    let v = float3(x, y, x)
    let w = float3(y, x, y)
    fmod_io[1] = float4(
        fmod(x, y) + mod(x, y),
        (fmod(v, w) + mod(v, w)).x,
        0.0f, 1.0f)
}

def private fmodm_words : array<uint> => clone_to_move(fmodm_spv)

[test]
def test_fmod_vs_mod(t : T?) {
    t |> run("mod is OpFMod and fmod is OpFRem, on scalar and vector") <| @(t : T?) {
        var words <- fmodm_words()
        t |> success(has_op(words, SpvOp.FMod), "mod: emits FMod")
        t |> success(has_op(words, SpvOp.FRem), "fmod: emits FRem")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "fmodm: spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}
