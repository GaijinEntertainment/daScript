options gen2
options indenting = 4
options no_unused_block_arguments = false

// Packed integer dot product (SPV_KHR_integer_dot_product): sdot4 lowers to OpSDot with the
// PackedVectorFormat4x8Bit LITERAL at operand index 4 (after result-type/result/a/b ids), pulls
// the DotProduct + DotProductInput4x8BitPacked capabilities + the extension, and spirv-val is clean.

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_dis
require spirv/spirv_grammar
require daslib/strings_boost

// OpExtension's operand words encode the extension-name string (LSB-first bytes, NUL-terminated,
// zero-padded — same form as OpExtInstImport / OpEntryPoint's name slot). Reassemble the bytes across
// each OpExtension and substring-search the requested name.
def private has_extension(words : array<uint>; want : string) : bool {
    var found = false
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (found || opcode != uint(SpvOp.Extension)) return
        let name = build_string() $(var wr) {
            for (k in range(n)) {
                let wd = w[s + k]
                for (i in range(4)) {
                    let b = uint8((wd >> uint(i * 8)) & 0xFFu)
                    if (b == 0u8) return
                    wr |> write_char(int(b))
                }
            }
        }
        if (find(name, want) >= 0) {
            found = true
        }
    }
    return found
}

// OpConstant is [type_id, result_id, value] — return the result id of the (single-word) constant
// with the given value, 0 when absent
def private const_id_of(words : array<uint>; value : uint) : uint {
    var id = 0u
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (id != 0u || opcode != uint(SpvOp.Constant) || n != 3) return
        if (w[s + 2] == value) {
            id = w[s + 1]
        }
    }
    return id
}

// first instruction of the given opcode: return its operand word at index i (0 when absent)
def private op_operand_at(words : array<uint>; op : SpvOp; i : int) : uint {
    var v = 0u
    var seen = false
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (seen || opcode != uint(op) || i >= n) return
        seen = true
        v = w[s + i]
    }
    return v
}

[test]
def test_sdot4(t : T?) {
    t |> run("sdot4 -> OpSDot shape + caps + extension; spirv-val clean") <| @(t : T?) {
        var words <- sdot_words()
        // two call sites, two OpSDot — no folding of the constant-operand form
        t |> equal(count_op(words, SpvOp.SDot), 2)
        // the PackedVectorFormat4x8Bit LITERAL rides at operand index 4
        t |> success(op_has_operand_at(words, SpvOp.SDot, 4, uint(SpvPackedVectorFormat.PackedVectorFormat4x8Bit)),
            "PackedVectorFormat4x8Bit literal at operand 4")
        // usdot4(a unsigned, b signed) -> one OpSUDot with the same trailing literal, and the
        // SIGNED arg (the unique 0x02020202 constant) swapped into V1 (operand index 2)
        t |> equal(count_op(words, SpvOp.SUDot), 1)
        t |> success(op_has_operand_at(words, SpvOp.SUDot, 4, uint(SpvPackedVectorFormat.PackedVectorFormat4x8Bit)),
            "PackedVectorFormat4x8Bit literal at operand 4 (SUDot)")
        let sconst = const_id_of(words, 0x02020202u)
        t |> success(sconst != 0u, "signed-operand constant present")
        t |> success(op_operand_at(words, SpvOp.SUDot, 2) == sconst, "signed arg lands in OpSUDot V1")
        // capability pair, once each
        for (cap in [SpvCapability.DotProduct, SpvCapability.DotProductInput4x8BitPacked]) {
            t |> success(op_has_operand_at(words, SpvOp.Capability, 0, uint(cap)), "capability {uint(cap)} present")
        }
        // the extension is declared exactly once (core only at SPIR-V 1.6, above our target)
        t |> success(has_extension(words, "SPV_KHR_integer_dot_product"), "OpExtension SPV_KHR_integer_dot_product")
        t |> success(count_op(words, SpvOp.Extension) == 1,
            "exactly one OpExtension (got {count_op(words, SpvOp.Extension)})")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}
