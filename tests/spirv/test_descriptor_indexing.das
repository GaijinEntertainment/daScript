options gen2
options indenting = 4

// Bindless / VK_EXT_descriptor_indexing surface. PR-B ships the nonuniformEXT marker (OpCopyObject +
// OpDecorate NonUniform on the fresh id, OpCapability ShaderNonUniform + OpExtension
// "SPV_EXT_descriptor_indexing" once per module). PR-C will add descriptor arrays (sampler2D[] etc.)
// and append their own asserts here. Every emitted module passes spirv-val.

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_dis
require spirv/spirv_grammar
require daslib/strings_boost

def private validate(t : T?; words : array<uint>; lbl : string) {
    let r = validate_spirv(words)
    if (r.ran) {
        t |> success(r.ok, "{lbl}: spirv-val: {r.msg}")
    } else {
        feint("spirv-val not found locally; skipping (CI enforces)\n")
    }
}

// OpExtension's operand words encode the extension-name string (LSB-first bytes, NUL-terminated,
// zero-padded — same form as OpExtInstImport / OpEntryPoint's name slot). Reassemble the bytes across
// each OpExtension and substring-search the requested name.
def private has_extension(words : array<uint>; want : string) : bool {
    var found = false
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (found || opcode != uint(SpvOp.Extension)) return
        var name = build_string() $(var wr) {
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

[test]
def test_nonuniform_ext_marker(t : T?) {
    t |> run("nonuniformEXT(x) -> OpCopyObject + OpDecorate NonUniform + cap/extension") <| @(t : T?) {
        var words <- nuext_words()
        // OpCapability ShaderNonUniform (the base bindless cap; specific NonUniformIndexing caps land
        // when the access actually hits a typed descriptor array, which is PR-C scope).
        t |> success(op_has_operand_at(words, SpvOp.Capability, 0, uint(SpvCapability.ShaderNonUniform)),
            "nuext: OpCapability ShaderNonUniform")
        let ncap_nu = count_op_operand_at(words, SpvOp.Capability, 0, uint(SpvCapability.ShaderNonUniform))
        t |> success(ncap_nu == 1, "nuext: exactly one ShaderNonUniform cap (got {ncap_nu})")
        // OpExtension "SPV_EXT_descriptor_indexing" emitted once.
        t |> success(has_extension(words, "SPV_EXT_descriptor_indexing"),
            "nuext: OpExtension SPV_EXT_descriptor_indexing")
        t |> success(count_op(words, SpvOp.Extension) == 1,
            "nuext: exactly one OpExtension (got {count_op(words, SpvOp.Extension)})")
        // OpCopyObject: the fresh id we decorate (one per nonuniformEXT call in this shader).
        t |> success(has_op(words, SpvOp.CopyObject), "nuext: emits OpCopyObject")
        t |> success(count_op(words, SpvOp.CopyObject) == 1,
            "nuext: exactly one OpCopyObject (got {count_op(words, SpvOp.CopyObject)})")
        // OpDecorate <id> NonUniform on the fresh id. Operand at index 1 is the decoration enum.
        t |> success(op_has_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.NonUniform)),
            "nuext: OpDecorate <id> NonUniform")
        let nnu = count_op_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.NonUniform))
        t |> success(nnu == 1, "nuext: exactly one NonUniform decoration (got {nnu})")
        validate(t, words, "nuext")
        delete words
    }
}
