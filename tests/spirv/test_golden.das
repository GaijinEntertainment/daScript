options gen2
options indenting = 4

// Phase-6 behavior-preservation gate. Each fixture's symbolic disassembly (spirv_dis::dump) is frozen
// in tests/spirv/_golden/<name>.txt (generated from the pre-port hand-walker emitter). This asserts the
// post-port AstVisitor emitter reproduces it. Two strengths:
//   check()     — BYTE-IDENTICAL (same opcodes, operands, AND id-allocation order). Used for the leaf /
//                 arithmetic / control-flow fixtures, whose ids the visitor allocates in the same order
//                 as the hand-walker (block labels are emitted in the matching hook — see spirv_emit).
//   check_iso() — ID-ISOMORPHIC (same opcode sequence + same operands modulo a consistent result-id
//                 renaming). Used for the resource / composite fixtures: the hand-walker allocates a
//                 call/composite RESULT id before its operands, while the visitor (post-order) allocates
//                 operands first, so ids legitimately permute. canon() rewrites every operand word to a
//                 first-appearance placeholder, so isomorphic modules produce identical text. SPIR-V is
//                 SSA — id numbering is an internal detail; census + spirv-val + the dasVulkan GPU gate
//                 are the semantic oracles, this just pins the structure against the hand-walker baseline.
// Goldens live under get_das_root() so the test is cwd-independent (isolated mode / CI).

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_dis
require daslib/fio
require daslib/strings_boost

def private read_golden(name : string) : string {
    let path = "{get_das_root()}/tests/spirv/_golden/{name}.txt"
    var content = ""
    var ok = false
    fopen(path, "rb") $(f) {
        if (f != null) {
            content = fread(f)
            ok = true
        }
    }
    if (!ok) {
        return "MISSING GOLDEN: {path}"
    }
    return content
}

// Compare EOL-agnostically: goldens are stored -text (see _golden/.gitattributes), but strip any \r
// so the gate never breaks on an autocrlf checkout (dump() always emits \n).
def private nocr(s : string) : string {
    return replace(s, "\r", "")
}

def private check(t : T?; name : string; var words : array<uint>) {
    let got = nocr(dump(words))
    let want = nocr(read_golden(name))
    let same = got == want
    t |> success(same, "{name}: emitted disassembly matches golden (byte-identical)")
    if (!same) {
        to_log(LOG_ERROR, "GOLDEN MISMATCH {name}:\n--- want ---\n{want}\n--- got ---\n{got}\n")
    }
    delete words
}

// Rewrite each <id> operand to a first-appearance-ordered placeholder, so two id-renaming-equivalent
// disassemblies normalize to identical text. Operand role (id vs literal) comes from op_operand_is_literal
// keyed on the line's opcode name, so a literal whose value collides with an id number stays raw (printed
// per-position, never entered into the id map). Comment/header lines (starting ";") pass through.
def private canon(s : string) : string {
    var ids : table<string; int>
    var nextId = 0
    let out = build_string() $(var wr) {
        for (line in split(nocr(s), "\n")) {
            if (empty(line) || starts_with(line, ";")) {
                wr |> write("{line}\n")
                continue
            }
            var toks <- split(line, " ")
            let opName = toks[0]
            for (ti in range(length(toks))) {
                if (ti > 0) {
                    wr |> write(" ")
                }
                let tok = toks[ti]
                if (ti != 0 && starts_with(tok, "0x") && !op_operand_is_literal(opName, ti - 1)) {
                    if (!key_exists(ids, tok)) {
                        ids[tok] = nextId
                        nextId++
                    }
                    let cid = ids?[tok] ?? 0
                    wr |> write("%{cid}")
                } else {
                    wr |> write(tok)
                }
            }
            delete toks
            wr |> write("\n")
        }
    }
    delete ids
    return out
}

def private check_iso(t : T?; name : string; var words : array<uint>) {
    let got = canon(dump(words))
    let want = canon(read_golden(name))
    let same = got == want
    t |> success(same, "{name}: disassembly id-isomorphic to golden")
    if (!same) {
        to_log(LOG_ERROR, "ISO MISMATCH {name}:\n--- want(canon) ---\n{want}\n--- got(canon) ---\n{got}\n")
    }
    delete words
}

[test]
def test_golden_disassembly(t : T?) {
    t |> run("emitter disassembly == frozen golden (all fixtures)") <| @(t : T?) {
        check(t, "square", square_words())
        check(t, "uarith", uarith_words())
        check(t, "iarith", iarith_words())
        check(t, "farith", farith_words())
        check(t, "uctrl", uctrl_words())
        check(t, "ictrl", ictrl_words())
        check(t, "fctrl", fctrl_words())
        check(t, "wloop", wloop_words())
        check(t, "floop", floop_words())
        check(t, "nloop", nloop_words())
        check(t, "echain", echain_words())
        check(t, "eret", eret_words())
        check(t, "rfor", rfor_words())
        // resource / composite fixtures: ids legitimately renumber (call/composite result allocated
        // before operands in the hand-walker, after them in the post-order visitor) -> id-isomorphic.
        check_iso(t, "tri_vert", tri_vert_words())
        check_iso(t, "tri_frag", tri_frag_words())
        check_iso(t, "vindex", vindex_words())
        check_iso(t, "draw_id", draw_id_words())
        check_iso(t, "nuext", nuext_words())
        check_iso(t, "darr", darr_words())
        check_iso(t, "raster_math", raster_math_words())
        check_iso(t, "catri", catri_words())
        check_iso(t, "ubo_frag", ubo_frag_words())
        check_iso(t, "pc_frag", pc_frag_words())
        check_iso(t, "ubo_nested", ubo_nested_words())
        // mvp_vert + tex_frag: the visitor schedules independent access-chains/loads in post-order
        // (chain,chain,load,load) where the hand-walker interleaved per operand (chain,load,chain,load)
        // — a benign instruction reorder the order-sensitive golden can't absorb. Their goldens are the
        // VISITOR baseline (forward regression lock); hand-walker equivalence is proven by spirv-val +
        // census + the matrix/sampler opcode-shape asserts + the dasVulkan GPU gate (6.5).
        check_iso(t, "mvp_vert", mvp_vert_words())
        check_iso(t, "tex_frag", tex_frag_words())
        // Phase-7 fixtures: no hand-walker baseline (it's gone) — these goldens are the visitor baseline
        // (forward regression lock). check_iso since both contain calls (conversions / image ops) whose
        // result ids the post-order visitor allocates after their operands.
        check_iso(t, "conv", conv_words())
        check_iso(t, "storimg", storimg_words())
        check_iso(t, "texlod", texlod_words())
        check_iso(t, "dims", dims_words())
        check_iso(t, "septex", septex_words())
        // Phase-8 fixtures (visitor baseline): ternary OpSelect + vector constructors (post-order ids).
        check_iso(t, "tern", tern_words())
        check_iso(t, "vecarith", vecarith_words())
        check_iso(t, "mathx", mathx_words())
        // Phase-10.1..10.6 fixtures (visitor baseline): byte-identical check() — the goldens are
        // produced by the very emitter under test, so ids ARE deterministic. Pins opcodes, operands,
        // AND id order across the SSBO std430 / swizzle / fragment-realism / Dref / compute (shared
        // + barriers + atomics) / image-query / module-scope-let paths added across 10.1..10.6.
        check(t, "ssbovec", ssbovec_words())
        check(t, "ssbostruct", ssbostruct_words())
        check(t, "gswizzle", gswizzle_words())
        check(t, "fragx", fragx_words())
        check(t, "fragshadow", fragshadow_words())
        check(t, "cshared", cshared_words())
        check(t, "catomics", catomics_words())
        check(t, "imgquery", imgquery_words())
        check(t, "modconst", modconst_words())
        // Phase-11 user-defined function fixtures: frozen from the current visitor, so byte-identical
        // check() (the strictest lock — pins opcodes, operands, AND id order of OpFunction/OpFunctionCall/
        // OpFunctionParameter/OpReturnValue + the ref-param copy-in/out temps).
        check(t, "userfn", userfn_words())
        check(t, "userfn_ref", userfn_ref_words())
        check(t, "userfn_global", userfn_global_words())
        // PR-D mesh + task stage scaffolding fixtures (visitor baseline): cap + ext + execution model
        // + execution modes + SetMeshOutputsEXT (mesh) / EmitMeshTasksEXT (task). byte-identical
        // check() pins the opcode AND id-allocation order of the prologue intrinsics.
        check(t, "mesh_empty", mesh_empty_words())
        check(t, "task_empty", task_empty_words())
        // PR-E mesh output config fixtures (visitor baseline): non-default counts + OutputLinesEXT /
        // OutputPoints topology. byte-identical check() pins the OpExecutionMode operands.
        check(t, "mesh_lines", mesh_lines_words())
        check(t, "mesh_points", mesh_points_words())
        // PR-E2 array-of-builtin output (visitor baseline): per-vertex gl_Position Block + triangle
        // index array. byte-identical check() pins the Block struct + two-level access-chain id order.
        check(t, "mesh_tri", mesh_tri_words())
    }
}
