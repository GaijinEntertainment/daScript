options gen2
options indenting = 4

// Maintained golden generator: writes a full symbolic disassembly (spirv_dis::dump) of every
// captured fixture blob to tests/spirv/_golden/<name>.txt.  Re-run whenever a new fixture is
// added or the emitter is intentionally changed to update the frozen baseline.  Not a [test] — run via
//   daslang tests/spirv/_gen_golden.das
// test_golden.das is the permanent byte-identical gate; this script keeps the _golden/ dir in sync.

require _spirv_common
require spirv/spirv_dis
require daslib/fio

def emit_golden(dir, name : string; words : array<uint>) {
    let path = "{dir}/{name}.txt"
    fopen(path, "wb") $(f) {
        if (f != null) {
            fwrite(f, dump(words))
            to_log(LOG_INFO, "wrote {path} ({length(words)} words)\n")
        } else {
            to_log(LOG_ERROR, "FAILED to open {path}\n")
        }
    }
}

[export]
def main {
    let dir = "{get_das_root()}/tests/spirv/_golden"
    mkdir(dir)
    emit_golden(dir, "square", square_words())
    emit_golden(dir, "uarith", uarith_words())
    emit_golden(dir, "iarith", iarith_words())
    emit_golden(dir, "farith", farith_words())
    emit_golden(dir, "uctrl", uctrl_words())
    emit_golden(dir, "ictrl", ictrl_words())
    emit_golden(dir, "fctrl", fctrl_words())
    emit_golden(dir, "wloop", wloop_words())
    emit_golden(dir, "floop", floop_words())
    emit_golden(dir, "nloop", nloop_words())
    emit_golden(dir, "echain", echain_words())
    emit_golden(dir, "eret", eret_words())
    emit_golden(dir, "rfor", rfor_words())
    emit_golden(dir, "tri_vert", tri_vert_words())
    emit_golden(dir, "tri_frag", tri_frag_words())
    emit_golden(dir, "point_size", point_size_words())
    emit_golden(dir, "point_coord", point_coord_words())
    emit_golden(dir, "draw_id", draw_id_words())
    emit_golden(dir, "nuext", nuext_words())
    emit_golden(dir, "darr", darr_words())
    emit_golden(dir, "vindex", vindex_words())
    emit_golden(dir, "raster_math", raster_math_words())
    emit_golden(dir, "catri", catri_words())
    emit_golden(dir, "ubo_frag", ubo_frag_words())
    emit_golden(dir, "mvp_vert", mvp_vert_words())
    emit_golden(dir, "pc_frag", pc_frag_words())
    emit_golden(dir, "ubo_nested", ubo_nested_words())
    emit_golden(dir, "tex_frag", tex_frag_words())
    emit_golden(dir, "conv", conv_words())
    emit_golden(dir, "storimg", storimg_words())
    emit_golden(dir, "imgfmt", imgfmt_words())              // storage-image @format qualifier (r32f + default rgba8)
    emit_golden(dir, "texlod", texlod_words())
    emit_golden(dir, "dims", dims_words())
    emit_golden(dir, "septex", septex_words())
    emit_golden(dir, "tern", tern_words())
    emit_golden(dir, "vecarith", vecarith_words())
    emit_golden(dir, "mathx", mathx_words())
    // Phase-10.1..10.6 fixtures: forward-regression freezes (no hand-walker baseline; locks the
    // current emitter's output so future drift is caught on top of census + spirv-val).
    emit_golden(dir, "ssbovec", ssbovec_words())            // 10.1 SSBO std430 vec elements
    emit_golden(dir, "ssbostruct", ssbostruct_words())      // 10.1 SSBO std430 struct elements
    emit_golden(dir, "gswizzle", gswizzle_words())          // 10.2 composite/global swizzle
    emit_golden(dir, "fragx", fragx_words())                // 10.3 fragment realism rails
    emit_golden(dir, "fragshadow", fragshadow_words())      // 10.4 depth-compare sampling (Dref)
    emit_golden(dir, "cshared", cshared_words())            // 10.5 shared memory + barriers
    emit_golden(dir, "catomics", catomics_words())          // 10.5 atomics
    emit_golden(dir, "imgquery", imgquery_words())          // 10.6 imageSize
    emit_golden(dir, "modconst", modconst_words())          // 10.6 module-scope let constants
    // Phase-11 user-defined function fixtures (forward-regression freeze — byte-identical locks on
    // the current emitter, asserted via check()).
    emit_golden(dir, "userfn", userfn_words())
    emit_golden(dir, "userfn_ref", userfn_ref_words())
    emit_golden(dir, "userfn_global", userfn_global_words())
    // PR-D mesh + task stage scaffolding (VK_EXT_mesh_shader): empty mesh + empty task.
    // Forward-regression freeze.
    emit_golden(dir, "mesh_empty", mesh_empty_words())
    emit_golden(dir, "task_empty", task_empty_words())
    // PR-E mesh output config: non-default counts + lines / points topology.
    emit_golden(dir, "mesh_lines", mesh_lines_words())
    emit_golden(dir, "mesh_points", mesh_points_words())
    // PR-E2 array-of-builtin output: one-triangle per-vertex Position + triangle index array.
    emit_golden(dir, "mesh_tri", mesh_tri_words())
    // PR-F mesh user + per-primitive-builtin outputs + the fragment gl_PrimitiveID read.
    emit_golden(dir, "mesh_outs", mesh_outs_words())
    emit_golden(dir, "prim_frag", prim_frag_words())
    emit_golden(dir, "pp_frag", pp_frag_words())
    // PR-G task -> mesh payload (TaskPayloadWorkgroupEXT): the task writes + EmitMeshTasksEXT 4-operand
    // form, the mesh reads its survivor-list slice.
    emit_golden(dir, "task_payload", task_payload_words())
    emit_golden(dir, "mesh_payload", mesh_payload_words())
}
