options gen2
options indenting = 4

// local struct value member access (Hi-Z cull prereq): `let e = ssbo[i]; e.field` copies an SSBO struct
// element into an immutable local, then reads members off it. The let holds the loaded composite (OpLoad
// of the whole struct), so each member access lowers to OpCompositeExtract -- the value-path twin of the
// indexed access-chain form (`ssbo[i].field`). The fixture reads two float4 members and adds them (no
// swizzles), so OpCompositeExtract appears EXACTLY twice: one per member access. spirv-val'd.

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_dis
require spirv/spirv_grammar

def private validate(t : T?; words : array<uint>; lbl : string) {
    let r = validate_spirv(words)
    if (r.ran) {
        t |> success(r.ok, "{lbl}: spirv-val: {r.msg}")
    } else {
        feint("spirv-val not found locally; skipping (CI enforces)\n")
    }
}

[test]
def test_local_struct_member(t : T?) {
    t |> run("local struct value member access: let e = ssbo[i]; e.field -> OpCompositeExtract") <| @(t : T?) {
        var words <- lsvmember_words()
        // The discriminating proof of the value path: members are read with OpCompositeExtract off the
        // loaded struct composite, NOT a per-member OpAccessChain (the indexed `ssbo[i].field` form). The
        // fixture has no swizzles, so OpCompositeExtract appears EXACTLY twice -- one per member read.
        t |> success(count_op(words, SpvOp.CompositeExtract) == 2,
            "lsvmember: two float4 member reads -> exactly two OpCompositeExtract (value path, not per-member access chain)")
        t |> success(has_op(words, SpvOp.FAdd), "lsvmember: e.a + e.b -> OpFAdd on the extracted members")
        validate(t, words, "lsvmember")
        delete words
    }
}
