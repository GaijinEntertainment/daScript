options gen2
options indenting = 4

// Census gate (gate B): the union of opcodes the emitter produces across EVERY fixture must equal
// the declared supported set, in both directions. Stronger than line coverage — it catches an
// opcode whose emit line ran but was never asserted by any test, and a declared opcode that stopped
// firing. Failure messages resolve the numeric opcode to its OpName for readability.

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_dis
require spirv/spirv_grammar

def private op_name(op : uint) : string {
    return SPV_OP_NAMES?[op] ?? "op#{op}"
}

def private add_set(var dst : table<uint>; var words : array<uint>) {
    var s <- opcode_set(words)
    for (op in keys(s)) {
        dst |> insert(op)
    }
    delete s
    delete words
}

[test]
def test_opcode_census(t : T?) {
    t |> run("emitter opcode census == declared set (all fixtures)") <| @(t : T?) {
        var present : table<uint>
        add_set(present, square_words())
        add_set(present, uarith_words())
        add_set(present, iarith_words())
        add_set(present, farith_words())
        add_set(present, uctrl_words())
        add_set(present, ictrl_words())
        add_set(present, fctrl_words())
        add_set(present, wloop_words())
        add_set(present, floop_words())
        add_set(present, tri_vert_words())
        add_set(present, tri_frag_words())
        add_set(present, vindex_words())
        add_set(present, raster_math_words())
        add_set(present, catri_words())
        add_set(present, ubo_frag_words())
        add_set(present, mvp_vert_words())
        add_set(present, pc_frag_words())
        add_set(present, tex_frag_words())
        var declared <- phase4d_emitter_opcodes()
        for (op in keys(present)) {
            t |> success(key_exists(declared, op), "emitted {op_name(op)} is in the declared set")
        }
        for (op in keys(declared)) {
            t |> success(key_exists(present, op), "declared {op_name(op)} is actually emitted")
        }
        delete present
        delete declared
    }
}
