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
        add_set(present, nloop_words())
        add_set(present, echain_words())
        add_set(present, eret_words())
        add_set(present, rfor_words())
        add_set(present, tri_vert_words())
        add_set(present, tri_frag_words())
        add_set(present, point_size_words())
        add_set(present, point_coord_words())
        add_set(present, draw_id_words())
        add_set(present, nuext_words())
        add_set(present, darr_words())
        add_set(present, vindex_words())
        add_set(present, raster_math_words())
        add_set(present, catri_words())
        add_set(present, ubo_frag_words())
        add_set(present, mvp_vert_words())
        add_set(present, pc_frag_words())
        add_set(present, ubo_nested_words())
        add_set(present, tex_frag_words())
        add_set(present, conv_words())
        add_set(present, storimg_words())
        add_set(present, texlod_words())
        add_set(present, dims_words())
        add_set(present, septex_words())
        add_set(present, tern_words())
        add_set(present, vecarith_words())
        add_set(present, mathx_words())
        add_set(present, ssbovec_words())
        add_set(present, ssbostruct_words())
        add_set(present, gswizzle_words())
        add_set(present, fragx_words())
        add_set(present, fragshadow_words())
        add_set(present, cshared_words())
        add_set(present, catomics_words())
        add_set(present, imgquery_words())
        add_set(present, modconst_words())
        add_set(present, userfn_words())
        add_set(present, userfn_ref_words())
        add_set(present, userfn_global_words())
        add_set(present, mesh_empty_words())
        add_set(present, task_empty_words())
        add_set(present, mesh_lines_words())
        add_set(present, mesh_points_words())
        add_set(present, mesh_tri_words())
        add_set(present, mesh_outs_words())
        add_set(present, prim_frag_words())
        add_set(present, pp_frag_words())
        add_set(present, task_payload_words())
        add_set(present, mesh_payload_words())
        var declared <- mesh_stage_emitter_opcodes()
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
