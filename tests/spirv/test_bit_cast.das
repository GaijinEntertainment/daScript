options gen2
options indenting = 4

// intBitsToFloat / uintBitsToFloat / floatBitsToInt / floatBitsToUint (daslib/math_bits'
// int_bits_to_float / uint_bits_to_float / float_bits_to_int / float_bits_to_uint) each lower to a
// single core OpBitcast, result type from the overload's return.  math_bits is a builtin module, so
// its CPU-reinterpret body is never emitted as an OpFunction.  The blob is spirv-val clean.

require dastest/testing_boost public
require _spirv_common               // validate_spirv
require spirv/spirv_shader          // [fragment_shader]
require spirv/spirv_builtins public // gl_FragCoord
require spirv/spirv_dis             // count_op
require spirv/spirv_grammar         // SpvOp
require daslib/math_bits            // int_bits_to_float / ...

var @out @location = 0 out_color : float4

[fragment_shader(name="bitcast_spv"), marker(no_coverage)]
def bitcast_frag {
    let i = int(gl_FragCoord.x)
    let u = uint(gl_FragCoord.y)
    let j = float_bits_to_int(gl_FragCoord.z)
    let k = float_bits_to_uint(gl_FragCoord.w)
    out_color = float4(
        int_bits_to_float(i),
        uint_bits_to_float(u),
        int_bits_to_float(j),
        uint_bits_to_float(k))
}

def public bitcast_words : array<uint> => clone_to_move(bitcast_spv)

[test]
def test_bit_cast(t : T?) {
    t |> run("int/uint/float bit-casts -> OpBitcast; body never emitted; spirv-val clean") <| @(t : T?) {
        var words <- clone_to_move(bitcast_spv)
        // six bit-cast calls in the body -> six OpBitcast
        t |> success(count_op(words, SpvOp.Bitcast) == 6,
            "six bit-casts -> six OpBitcast (got {count_op(words, SpvOp.Bitcast)})")
        // the math_bits reinterpret body must never become an OpFunction / OpFunctionCall
        t |> success(count_op(words, SpvOp.FunctionCall) == 0,
            "no OpFunctionCall: math_bits is a builtin, not a user function")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}
