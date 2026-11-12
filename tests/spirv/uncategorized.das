options gen2
options no_unused_block_arguments = false

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_dis
require daslib/fio
require daslib/strings_boost
require _color_mapping_shader
require spirv/spirv_grammar
require spirv/spirv_shader public
require spirv/spirv_builtins public
require spirv/spirv_builder
require math
require _texture_grad_shaders

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
def private nocr(s : string) : string => replace(s, "\r", "")

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
            var inscope toks <- split(line, " ")
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

[test(description = "The disassembly of every emitter fixture matches its frozen golden text.")]
def test_golden_disassembly(t : T?) { // nolint:STYLE038 - flat list of golden fixtures
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
        check(t, "ufor", ufor_words())
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
        check_iso(t, "imgfmt", imgfmt_words())
        check_iso(t, "lsvmember", lsvmember_words())
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
        check(t, "cbufbar", cbufbar_words())
        check(t, "catomics", catomics_words())
        check(t, "imgquery", imgquery_words())
        check(t, "modconst", modconst_words())
        // Phase-11 user-defined function fixtures: frozen from the current visitor, so byte-identical
        // check() (the strictest lock — pins opcodes, operands, AND id order of OpFunction/OpFunctionCall/
        // OpFunctionParameter/OpReturnValue + the ref-param copy-in/out temps).
        check(t, "userfn", userfn_words())
        check(t, "userfn_ref", userfn_ref_words())
        check(t, "userfn_global", userfn_global_words())
        check(t, "spdirect", spdirect_words())
        check(t, "spelem", spelem_words())
        check(t, "spelemi", spelemi_words())
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
        // PR-F mesh user + per-primitive-builtin outputs (visitor baseline): per-vertex varying +
        // per-primitive output + gl_MeshPrimitivesEXT block; prim_frag = the fragment gl_PrimitiveID read.
        check(t, "mesh_outs", mesh_outs_words())
        check(t, "prim_frag", prim_frag_words())
        check(t, "pp_frag", pp_frag_words())
        // PR-G task -> mesh payload (visitor baseline): the @task_payload TaskPayloadWorkgroupEXT variable,
        // the task writes + EmitMeshTasksEXT 4-operand form, and the mesh survivor-list read. byte-identical
        // check() pins the plain-struct member ids + the payload access-chain order.
        check(t, "task_payload", task_payload_words())
        check(t, "mesh_payload", mesh_payload_words())
        check(t, "lattice", lattice_words())
        // subgroup ops + spec constants (visitor baseline): byte-identical check() pins the
        // GroupNonUniform operand shapes (scope id + Reduce literal) and the OpSpecConstant /
        // WorkgroupSize-composite emission order.
        check(t, "subgroup", subgroup_words())
        check(t, "specc", specc_words())
        // 8/16-bit SSBO storage (visitor baseline): pins the std430 tight strides (34/1/8), the
        // sub-32 member Offsets, and the unpack8/pack32 Bitcast + SConvert emission order.
        check(t, "q8blk", q8blk_words())
        // packed integer dot product: OpSDot + the PackedVectorFormat4x8Bit literal
        check(t, "sdot", sdot_words())
        // cooperative matrix: the four KHR coopmat opcodes + the Vulkan memory model + 1.6 header.
        // byte-identical check() — visitor baseline, deterministic ids.
        check(t, "coopmat", coopmat_words())
        // coopmat int8: s8xs8->s32 tile + the signed-components MulAdd operands mask
        check(t, "coopmat_i8", coopmat_i8_words())
        // coopmat load from @workgroup shared memory (Workgroup-storage access chain)
        check(t, "coopmat_sm", coopmat_sm_words())
    }
}

[test(description = "The color mapping helpers compile as a SPIR-V fragment shader.")]
def test_color_mapping_spirv(t : T?) {
    t |> run("all color mapping helpers compile as a SPIR-V fragment shader") <| @(t : T?) {
        var words <- color_mapping_words()
        t |> equal(words[1], 0x00010300u, "SPIR-V 1.3")
        let model = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(model.ok && model.value == uint(SpvExecutionModel.Fragment),
            "fragment entry point")
        t |> equal(count_op_operand_at(words, SpvOp.Variable, 2, uint(SpvStorageClass.Input)), 1,
            "runtime color input")
        t |> equal(count_op_operand_at(words, SpvOp.Variable, 2, uint(SpvStorageClass.Output)), 4,
            "four color outputs")
        foreach_inst(words) $(opcode : uint; w : array<uint>; start, count : int) {
            if (opcode == uint(SpvOp.Variable) && count >= 3 &&
                    w[start + 2] == uint(SpvStorageClass.Output)) {
                t |> success(op_has_operand_at(words, SpvOp.Store, 0, w[start + 1]),
                    "color output is written")
            }
        }
        let validation = validate_spirv(words, "vulkan1.1")
        if (validation.ran) {
            t |> success(validation.ok, "spirv-val: {validation.msg}")
        } else {
            feint("spirv-val not found locally; skipping external validation\n")
        }
        delete words
    }
}

// Stage-I/O @enabled gate.
//
// A @in/@out global carrying @enabled = <compile-time int/bool const> is EXCLUDED from the SPIR-V
// interface when the const is 0/false -- the das mirror of GLSL wrapping a `layout(location = N)
// in/out ...` in `#if COND`.  The matching write is gated by `static_if (COND)` in the body (dropped
// before name resolution), so no reference to the excluded global survives.  Here v_off (@enabled =
// false, location 3) is excluded while v_main (location 0) and v_on (@enabled = true, location 4)
// remain; the blob is spirv-val clean either way.
let ENABLED_ON  = true
let ENABLED_OFF = false

var @out @location = 0 v_main : float3
var @out @location = 3 @enabled = ENABLED_OFF v_off : float3
var @out @location = 4 @enabled = ENABLED_ON  v_on  : float3

[vertex_shader(name="enabled_spv"), marker(no_coverage)]
def enabled_io {
    gl_Position = float4(1.0f, 2.0f, 3.0f, 1.0f)
    v_main = float3(1.0f, 1.0f, 1.0f)
    static_if (ENABLED_OFF) { v_off = float3(0.5f, 0.0f, 0.0f) }
    static_if (ENABLED_ON)  { v_on  = float3(0.0f, 0.5f, 0.0f) }
}

def private has_location(words : array<uint>; loc : uint) : bool {
    return op_has_operand_pair(words, SpvOp.Decorate,
        uint(SpvDecoration.Location), loc)
}

[test(description = "@enabled = false excludes a stage output from the interface and the module validates.")]
def test_stage_io_enabled(t : T?) {
    t |> run("@enabled = false excludes a stage output from the interface; spirv-val clean") <| @(t : T?) {
        var words <- clone_to_move(enabled_spv)
        t |> success(has_location(words, 0u), "location 0 (default-enabled) present")
        t |> success(has_location(words, 4u), "location 4 (@enabled = true) present")
        t |> success(!has_location(words, 3u), "location 3 (@enabled = false) EXCLUDED")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}

// Spec constants: @constant_id globals -> OpSpecConstant (SpecConstantTrue/False for bools)
// decorated SpecId, never pooled with plain constants; local_size_x_id -> a per-axis OpSpecConstant
// folded into an OpSpecConstantComposite decorated BuiltIn WorkgroupSize, with the LocalSize
// execution mode kept alongside (glslang parity; the builtin takes precedence per the spec).
[test(description = "Spec constants emit OpSpecConstant shapes with distinct SpecIds and a WorkgroupSize composite that validates.")]
def test_spec_constants(t : T?) {
    t |> run("OpSpecConstant shapes + SpecIds + WorkgroupSize composite; spirv-val clean") <| @(t : T?) {
        var words <- specc_words()
        // 4 OpSpecConstant: uint(8) + int(-3) + float(1.5) + the local_size_x axis (64)
        t |> equal(count_op(words, SpvOp.SpecConstant), 4)
        t |> equal(count_op(words, SpvOp.SpecConstantTrue), 1)
        t |> equal(count_op(words, SpvOp.SpecConstantFalse), 1)
        t |> equal(count_op(words, SpvOp.SpecConstantComposite), 1)
        // SpecIds 0..5 all decorated (0 = the workgroup-x axis, 1..5 = the @constant_id globals)
        for (cid in range(6)) {
            t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.SpecId), uint(cid)), "SpecId {cid} present")
        }
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.BuiltIn), uint(SpvBuiltIn.WorkgroupSize)), "WorkgroupSize builtin on the composite")
        t |> success(op_has_operand_at(words, SpvOp.ExecutionMode, 1, uint(SpvExecutionMode.LocalSize)), "LocalSize execution mode kept")
        // default values ride the OpSpecConstant literal at operand index 2
        t |> success(op_has_operand_at(words, SpvOp.SpecConstant, 2, 8u), "uint default 8")
        t |> success(op_has_operand_at(words, SpvOp.SpecConstant, 2, uint(-3)), "int default -3 (raw bits)")
        t |> success(op_has_operand_at(words, SpvOp.SpecConstant, 2, 0x3FC00000), "float default 1.5 (IEEE bits)")
        t |> success(op_has_operand_at(words, SpvOp.SpecConstant, 2, 64u), "workgroup-x axis default 64")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}

// Multiple Render Targets: lock in that the dasSpirv emitter accepts multiple `@out @location = N`
// globals in a fragment shader and emits each as its own Output OpVariable with a distinct Location
// decoration. This is the G-buffer shape tutorial 10 (deferred shading) will use; the per-global
// Location model is already what the emitter does, so this test exists to *guard* the behavior
// against a future regression -- not to add new functionality.
def private validate(t : T?; words : array<uint>; lbl : string) {
    let r = validate_spirv(words)
    if (r.ran) {
        t |> success(r.ok, "{lbl}: spirv-val: {r.msg}")
    } else {
        feint("spirv-val not found locally; skipping (CI enforces)\n")
    }
}

[test(description = "A fragment shader with three @location outputs emits all three output variables.")]
def test_mrt_three_outputs(t : T?) {
    t |> run("MRT: fragment shader with 3 @location outputs") <| @(t : T?) {
        var words <- mrt_words()
        // Entry point is a fragment shader.
        let model = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(model.ok && model.value == uint(SpvExecutionModel.Fragment),
            "mrt: EntryPoint model is Fragment")
        // Exactly 4 Location decorations: 1 @in (mrt_uv at 0) + 3 @out (position/normal/albedo at 0/1/2).
        let n_loc = count_op_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.Location))
        t |> success(n_loc == 4, "mrt: 4 Location decorations (1 input + 3 outputs), got {n_loc}")
        // Each of locations 0, 1, 2 is emitted. (Location 0 is emitted twice -- input + output -- and
        // op_has_operand_pair only checks for presence, not count.)
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.Location), 0u),
            "mrt: Location 0 decoration present")
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.Location), 1u),
            "mrt: Location 1 decoration present")
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.Location), 2u),
            "mrt: Location 2 decoration present")
        // At least 4 OpVariable (1 in + 3 out -- there may be more for builtins / internals).
        let n_var = count_op(words, SpvOp.Variable)
        t |> success(n_var >= 4, "mrt: at least 4 OpVariable (1 input + 3 outputs), got {n_var}")
        // At least 3 OpStore -- one write per output global. The shader body has exactly 3 stores.
        let n_store = count_op(words, SpvOp.Store)
        t |> success(n_store >= 3, "mrt: at least 3 OpStore writes, got {n_store}")
        // spirv-val (CI gate) catches structural CFG / type errors the opcode-counting asserts can't.
        validate(t, words, "mrt")
        delete words
    }
}

// Named integer annotation args: `@set = NAME` / `@binding = NAME` / `local_size_x = NAME`, where NAME
// is a module-scope `let` int constant, must resolve to that constant's value. The annotation stores
// the identifier as a string arg, so a `?as tInt` read silently yields the fallback -- 0 for a binding,
// 1 for a workgroup size -- and the blob still compiles and still passes spirv-val. Only the emitted
// decoration / execution mode reveals it. This guards the const-name resolution in spirv_emit's
// annotation_int, for both the layout indices and the [compute_shader] local_size.
[test(description = "Named @binding, @set and local_size_x constants resolve to the constants' values.")]
def test_binding_name_resolves(t : T?) {
    t |> run("named @binding / @set resolve to the constant's value") <| @(t : T?) {
        var words <- bindconst_words()
        // @binding = BIND_CONST_SLOT (7) -> the Binding decoration must carry 7, not the 0 fallback.
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.Binding), 7u),
            "Binding decoration == 7 (from BIND_CONST_SLOT)")
        // @set = BIND_CONST_SET (2) -> the DescriptorSet decoration must carry 2.
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.DescriptorSet), 2u),
            "DescriptorSet decoration == 2 (from BIND_CONST_SET)")
        // The single descriptor is at binding 7 -- a stray Binding 0 means the name resolved to the fallback.
        t |> success(!op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.Binding), 0u),
            "no fallback Binding 0")
        delete words
    }
    t |> run("named local_size_x resolves to the constant's value") <| @(t : T?) {
        var words <- bindconst_words()
        // OpExecutionMode operands: entry-point id(0), mode(1), then the mode's literals -- LocalSize
        // x(2), y(3), z(4). x must be 32 (from BIND_CONST_LOCAL_SIZE), never the silent 1 fallback.
        t |> success(op_has_operand_at(words, SpvOp.ExecutionMode, 1, uint(SpvExecutionMode.LocalSize)),
            "an ExecutionMode LocalSize is emitted")
        t |> success(op_has_operand_at(words, SpvOp.ExecutionMode, 2, 32u),
            "LocalSize x == 32 (from BIND_CONST_LOCAL_SIZE), not the 1 fallback")
        delete words
    }
}

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

[test(description = "nonuniformEXT(x) emits OpCopyObject, a NonUniform decoration and the capability and extension.")]
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

// Sampler descriptor array: sampler2D[8] -> OpTypeArray of OpTypeSampledImage in UniformConstant
// storage with DescriptorSet/Binding decorations; texture(arr[i], uv) lowers to OpAccessChain ->
// OpLoad -> OpImageSampleImplicitLod. nonuniformEXT on the index cascades NonUniform onto the
// access-chain pointer + the loaded sampler and pulls in the per-resource-type
// SampledImageArrayNonUniformIndexing capability.
[test(description = "A sampler2D array sampled through nonuniformEXT(idx) emits an array type, an access chain and the non-uniform cascade.")]
def test_sampler_descriptor_array(t : T?) {
    t |> run("sampler2D[N] + texture(arr[nonuniformEXT(idx)], uv) -> array + access-chain + cascade") <| @(t : T?) {
        var words <- darr_words()
        // OpTypeArray of OpTypeSampledImage built once.
        t |> success(has_op(words, SpvOp.TypeArray), "darr: emits OpTypeArray")
        t |> success(count_op(words, SpvOp.TypeArray) == 1,
            "darr: exactly one OpTypeArray (got {count_op(words, SpvOp.TypeArray)})")
        // DescriptorSet/Binding on the array variable (count=8 lives on the host side; spirv-val
        // doesn't care about it).
        t |> success(op_has_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.DescriptorSet)),
            "darr: OpDecorate DescriptorSet")
        t |> success(op_has_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.Binding)),
            "darr: OpDecorate Binding")
        // OpAccessChain: the array-element pointer derived from the array variable + idx.
        t |> success(has_op(words, SpvOp.AccessChain), "darr: emits OpAccessChain")
        // SampledImageArrayNonUniformIndexing capability: required because the index carries
        // NonUniform (via nonuniformEXT). The base ShaderNonUniform cap rides along from PR-B.
        t |> success(op_has_operand_at(words, SpvOp.Capability, 0, uint(SpvCapability.SampledImageArrayNonUniformIndexing)),
            "darr: OpCapability SampledImageArrayNonUniformIndexing")
        t |> success(op_has_operand_at(words, SpvOp.Capability, 0, uint(SpvCapability.ShaderNonUniform)),
            "darr: OpCapability ShaderNonUniform")
        // Cascade rule: three NonUniform decorations -- on the nonuniformEXT OpCopyObject result, on
        // the OpAccessChain pointer it indexes through, and on the OpLoad'd sampled image.
        let nnu = count_op_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.NonUniform))
        t |> success(nnu == 3, "darr: 3 NonUniform decorations (marker + access-chain + load) (got {nnu})")
        // OpImageSampleImplicitLod still lands as the actual sample (fragment-stage texture()).
        t |> success(has_op(words, SpvOp.ImageSampleImplicitLod),
            "darr: emits OpImageSampleImplicitLod on the loaded sampler")
        validate(t, words, "darr")
        delete words
    }
}

// Builder gate: hand-build the square module via the cached/deduplicated type+constant helpers,
// assert dedup (re-requests return the same id and don't grow the id-bound), assert the section
// shape, and validate the blob with spirv-val. This is the AST-independent foundation the emitter
// is built on (risk #1 type/const dedup + risk #2 SSBO decoration completeness).
[test(description = "The module builder deduplicates types and constants and a hand-built square kernel validates.")]
def test_builder_dedup_and_validate(t : T?) { // nolint:STYLE038 - linear hand-built SPIR-V module
    t |> run("builder: dedup + hand-built square validates") <| @(t : T?) {
        var m = make_module()

        // ----- types + globals via cached helpers (dependency order) -----
        let t_void = type_void(m)
        let t_fn   = type_function_void(m, t_void)
        let t_uint = type_uint(m)
        let t_v3u  = type_vector(m, t_uint, 3)
        let t_p_in_v3 = type_pointer(m, SpvStorageClass.Input, t_v3u)
        let id_gid = alloc_id(m)
        emit(m, SEC_TYPES, SpvOp.Variable, t_p_in_v3, id_gid, uint(SpvStorageClass.Input))
        emit(m, SEC_DECOR, SpvOp.Decorate, id_gid, uint(SpvDecoration.BuiltIn), uint(SpvBuiltIn.GlobalInvocationId))

        let c0 = const_uint(m, 0u)
        let t_p_in_u = type_pointer(m, SpvStorageClass.Input, t_uint)
        let t_rtarr = type_runtime_array(m, t_uint, 4)
        var members <- [t_rtarr]
        var offsets <- [0]
        let t_outbuf = type_struct_block(m, members, offsets)
        delete members
        delete offsets
        let t_p_sb_ob = type_pointer(m, SpvStorageClass.StorageBuffer, t_outbuf)
        let id_data = alloc_id(m)
        emit(m, SEC_TYPES, SpvOp.Variable, t_p_sb_ob, id_data, uint(SpvStorageClass.StorageBuffer))
        emit(m, SEC_DECOR, SpvOp.Decorate, id_data, uint(SpvDecoration.DescriptorSet), 0u)
        emit(m, SEC_DECOR, SpvOp.Decorate, id_data, uint(SpvDecoration.Binding), 0u)
        let t_p_sb_u = type_pointer(m, SpvStorageClass.StorageBuffer, t_uint)

        // ----- dedup proof: re-request returns the SAME id, id-bound does not grow -----
        let bound_before = m.next_id
        t |> equal(type_uint(m), t_uint)
        t |> equal(type_void(m), t_void)
        t |> equal(type_pointer(m, SpvStorageClass.Input, t_uint), t_p_in_u)
        t |> equal(type_pointer(m, SpvStorageClass.StorageBuffer, t_uint), t_p_sb_u)
        t |> equal(const_uint(m, 0u), c0)
        t |> equal(m.next_id, bound_before)

        // ----- preamble + entry + exec mode -----
        let id_main = alloc_id(m)
        emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.Shader))
        emit(m, SEC_MEMMODEL, SpvOp.MemoryModel, uint(SpvAddressingModel.Logical), uint(SpvMemoryModel.GLSL450))
        var ep <- [uint(SpvExecutionModel.GLCompute), id_main]
        var sw <- string_words("main")
        ep |> push_from(sw)
        delete sw
        ep |> push(id_gid)
        emit_n(m, SEC_ENTRY, SpvOp.EntryPoint, ep)
        delete ep
        var em <- [id_main, uint(SpvExecutionMode.LocalSize), 64u, 1u, 1u]
        emit_n(m, SEC_EXECMODE, SpvOp.ExecutionMode, em)
        delete em

        // ----- function body: data[gid.x] = gid.x * gid.x -----
        let id_label = alloc_id(m)
        let id_gp    = alloc_id(m)
        let id_idx   = alloc_id(m)
        let id_sq    = alloc_id(m)
        let id_ep    = alloc_id(m)
        emit(m, SEC_FUNCS, SpvOp.Function, t_void, id_main, 0u, t_fn)
        emit(m, SEC_FUNCS, SpvOp.Label, id_label)
        emit(m, SEC_FUNCS, SpvOp.AccessChain, t_p_in_u, id_gp, id_gid, c0)
        emit(m, SEC_FUNCS, SpvOp.Load, t_uint, id_idx, id_gp)
        emit(m, SEC_FUNCS, SpvOp.IMul, t_uint, id_sq, id_idx, id_idx)
        var ac <- [t_p_sb_u, id_ep, id_data, c0, id_idx]
        emit_n(m, SEC_FUNCS, SpvOp.AccessChain, ac)
        delete ac
        emit(m, SEC_FUNCS, SpvOp.Store, id_ep, id_sq)
        emit(m, SEC_FUNCS, SpvOp.Return)
        emit(m, SEC_FUNCS, SpvOp.FunctionEnd)

        var words <- module_words(m)
        delete m

        // ----- shape (dedup visible in the instruction counts) -----
        t |> equal(words[0], SPV_MAGIC)
        t |> equal(count_op(words, SpvOp.TypeInt), 1)         // uint emitted once
        t |> equal(count_op(words, SpvOp.TypePointer), 4)     // in_v3, in_uint, sb_outbuf, sb_uint
        t |> equal(count_op(words, SpvOp.Constant), 1)        // const 0 once
        t |> equal(count_op(words, SpvOp.Variable), 2)

        // ----- the real oracle -----
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}

// Regression: const_float must key by raw IEEE-754 bits, not the stringified value. -0.0 and 0.0
// stringify identically but are distinct bit patterns (0x80000000 vs 0x0) and distinct SPIR-V
// constants — a stringified key would alias them to one id.
[test(description = "The builder keys float constants by bits, so 0.0 and -0.0 stay distinct while deduplication still works.")]
def test_const_float_bitkey(t : T?) {
    if (HOST_FAST_MATH) {
        t |> skip("host runtime is built -ffast-math: -fno-signed-zeros folds -0.0 to 0.0, so the two keys are one value")
        return
    }
    t |> run("builder: const_float keys by bits (0.0 != -0.0), dedup intact") <| @(t : T?) {
        var m = make_module()
        let zero = 0.0f
        let negzero = -zero                 // -0.0 (sign-preserving negate, distinct bit pattern)
        let id_pos = const_float(m, zero)
        let id_neg = const_float(m, negzero)
        t |> success(id_pos != id_neg, "0.0 and -0.0 get distinct constant ids")
        t |> equal(const_float(m, zero), id_pos)     // dedup still holds per bit pattern
        t |> equal(const_float(m, negzero), id_neg)
        var words <- module_words(m)
        delete m
        t |> equal(count_op(words, SpvOp.Constant), 2)   // two distinct OpConstant, not collapsed
        delete words
    }
}

// @private_global gate: a module-scope mutable global with no descriptor annotation -> Private storage
// (SPIR-V's per-invocation "function-persistent" class, GLSL's plain global). Unlike @workgroup it is
// valid in ANY stage; here a fragment shader keeps a per-invocation scratch array that a helper writes
// by a dynamic index and main reads back -- the shape a fixed-array cannot express as a function
// parameter (fixed arrays don't cross a shader function boundary), so the array lives at module scope.
// Asserts a Private OpVariable, NO descriptor set/binding on it, and that it stays out of the <= 1.3
// entry-point interface; spirv-val is the structural oracle for the Private access chains.
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

[test(description = "A @private_global becomes a Private-storage variable used as per-invocation scratch.")]
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

// packHalf2x16/unpackHalf2x16 on the SPIR-V rail: GLSL.std.450 PackHalf2x16/UnpackHalf2x16
// ext-insts (numbers 58/62). Ext-inst NUMBERS don't enter the opcode census (only ExtInst
// itself, already declared); spirv-val is the correctness oracle.
var @ssbo @binding = 0 ph_data : array<uint>

[compute_shader(local_size_x=64, name="pack_half_spv"), marker(no_coverage)]
def pack_half_kernel {
    let gi = gl_GlobalInvocationID.x
    let back = unpackHalf2x16(ph_data[gi])
    ph_data[gi] = packHalf2x16(float2(back.y, back.x))
}

[test(description = "Half pack and unpack lower to GLSL.std.450 ext-insts and validate.")]
def test_pack_half_spirv(t : T?) {
    t |> run("pack/unpack lower to GLSL.std.450 ext-insts and validate") <| @(t : T?) {
        var words <- clone_to_move(pack_half_spv)
        t |> equal(words[0], SPV_MAGIC)
        t |> equal(count_op(words, SpvOp.ExtInst), 2)       // one pack + one unpack
        t |> success(has_op(words, SpvOp.ExtInstImport), "GLSL.std.450 imported")
        // OpExtInst operand index 3 is the GLSLstd450 instruction literal, so the census above is
        // pinned to the two specific ext-insts rather than merely counting ExtInst opcodes.
        t |> success(op_has_operand_at(words, SpvOp.ExtInst, 3, uint(GLSLstd450.PackHalf2x16)),
            "the pack ext-inst is GLSLstd450 PackHalf2x16 (58)")
        t |> success(op_has_operand_at(words, SpvOp.ExtInst, 3, uint(GLSLstd450.UnpackHalf2x16)),
            "the unpack ext-inst is GLSLstd450 UnpackHalf2x16 (62)")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}

// Const arrays + dynamic indexing gate: the gl_VertexIndex hardcoded-triangle vertex shader lowers
// fixed-array `let`s to OpTypeArray + OpConstantComposite (vec + array constants), a Function-storage
// array OpVariable initialized via OpStore, and OpAccessChain indexing by gl_VertexIndex. The emitted
// module passes spirv-val (the real define-before-use / type oracle).
def private check_ops(t : T?; words : array<uint>; lbl : string; ops : array<SpvOp>) {
    for (op in ops) {
        t |> success(has_op(words, op), "{lbl}: emits {op}")
    }
}

[test(description = "A constant array indexed by gl_VertexIndex emits an array type, a constant composite and an access chain.")]
def test_const_array_triangle(t : T?) {
    t |> run("const arrays: positions[gl_VertexIndex] -> TypeArray + ConstantComposite + AccessChain") <| @(t : T?) {
        var words <- catri_words()
        // array type + composite constants + dynamic index + the vec4 build
        check_ops(t, words, "catri", [SpvOp.TypeArray, SpvOp.ConstantComposite,
            SpvOp.AccessChain, SpvOp.Load, SpvOp.Store, SpvOp.CompositeConstruct])
        // the length-3 array constant: an OpConstant uint with value 3 (operand index 2)
        t |> success(op_has_operand_at(words, SpvOp.Constant, 2, 3u), "catri: has the uint 3 length constant")
        // a Function-storage OpVariable for the array (storage class at operand index 2)
        t |> success(op_has_operand_at(words, SpvOp.Variable, 2, uint(SpvStorageClass.Function)),
            "catri: array lives in a Function-storage OpVariable")
        // rasterizer builtins driving it
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.BuiltIn), uint(SpvBuiltIn.VertexIndex)),
            "catri: reads gl_VertexIndex")
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.BuiltIn), uint(SpvBuiltIn.Position)),
            "catri: writes gl_Position")
        validate(t, words, "catri")
        delete words
    }
}

[test(description = "A fixed-array literal of runtime elements emits one OpCompositeConstruct and an OpStore, not a constant.")]
def test_runtime_array_literal(t : T?) {
    t |> run("fixed-array literal of runtime elements -> one OpCompositeConstruct + OpStore, not a constant") <| @(t : T?) {
        var words <- rtarr_words()
        t |> success(count_op(words, SpvOp.TypeArray) == 1, "rtarr: one OpTypeArray (the int[5])")
        // the array from its five runtime elements, plus the float3 acc zero-init is a CONSTANT
        // (all-zero) -- so the only run-time composite builds are the array and the stored float4
        t |> success(count_op(words, SpvOp.CompositeConstruct) == 2, "rtarr: the int[5] and the stored float4")
        t |> success(op_has_operand_at(words, SpvOp.Variable, 2, uint(SpvStorageClass.Function)),
            "rtarr: the array lives in a Function-storage OpVariable")
        t |> success(has_op(words, SpvOp.AccessChain), "rtarr: ticks[i] indexes through OpAccessChain")
        validate(t, words, "rtarr")
        delete words
    }
}

[test(description = "A float atomicAdd on an SSBO vector component emits the float atomic add with its capability.")]
def test_float_atomic_add(t : T?) {
    t |> run("compute: float atomicAdd on an SSBO vector component") <| @(t : T?) {
        var words <- cfatomic_words()
        t |> success(has_op(words, SpvOp.AtomicFAddEXT),
            "cfatomic: atomicAdd(float) -> OpAtomicFAddEXT")
        t |> success(!has_op(words, SpvOp.AtomicIAdd),
            "cfatomic: a float target never emits the integer OpAtomicIAdd")
        t |> success(op_has_operand_at(words, SpvOp.Capability, 0,
            uint(SpvCapability.AtomicFloat32AddEXT)),
            "cfatomic: declares OpCapability AtomicFloat32AddEXT")
        t |> success(has_op(words, SpvOp.Extension),
            "cfatomic: declares the SPV_EXT_shader_atomic_float_add OpExtension")
        // The component lvalue: the atomic target and the component store both chain through an
        // OpAccessChain, so a StorageBuffer float pointer type has to exist in the module.
        t |> success(has_op(words, SpvOp.AccessChain),
            "cfatomic: the vector component is an OpAccessChain, not a loaded vector")
        // A component READ takes the value path, so the extract survives alongside the chains.
        t |> success(has_op(words, SpvOp.CompositeExtract),
            "cfatomic: a component read stays an OpCompositeExtract off the loaded vector")
        validate(t, words, "cfatomic")
        delete words
    }
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

[test(description = "sdot4 emits the OpSDot shape with its capabilities and extension and validates.")]
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

// Subgroup (wave) operations -- the GL_KHR_shader_subgroup_* family lowered to OpGroupNonUniform*.
//
// One compute fixture reaches every op family and every subgroup builtin, so the blob carries all five
// per-family capabilities at once.  The asserts pin the three things a surface gate later depends on:
// the OPCODE each daslang name selects (including the S/U/F pick that the operand's scalar class
// drives), the GroupOperation LITERAL that separates a plain reduction (Reduce) from a clustered one
// (ClusteredReduce), and the CAPABILITY set.  The Subgroup scope is a constant <id> rather than a
// literal, so its value is left to spirv-val -- the structural oracle for scope and cluster-size rules.
// The GroupOperation is operand 3 of an arithmetic reduction: result-type(0), result-id(1),
// scope(2), GroupOperation(3), value(4).
let GROUP_OP_OPERAND = 3

// The capability is operand 0 of OpCapability; positional, so an id equal to the enum can't match.
let CAP_OPERAND = 0

var @ssbo @binding = 0 sg_in : array<float>
var @ssbo @binding = 1 sg_out : array<float>
var @ssbo @binding = 2 sg_iout : array<int>

[compute_shader(local_size_x=64, name="sg_all_ops_spv"), marker(no_coverage)]
def subgroup_all_ops {
    let i = gl_GlobalInvocationID.x
    let v = sg_in[i]
    let iv = int(v)
    let uv = uint(iv)

    // vote -> OpGroupNonUniformAll / OpGroupNonUniformAny (GroupNonUniformVote)
    let uniform_sign = subgroupAll(v > 0.0f)
    let any_zero = subgroupAny(v == 0.0f)

    // ballot -> OpGroupNonUniformBroadcastFirst / OpGroupNonUniformBroadcast (GroupNonUniformBallot)
    let first = subgroupBroadcastFirst(float3(v, v, v))
    let lane0 = subgroupBroadcast(v, 0u)

    // shuffle -> OpGroupNonUniformShuffle / OpGroupNonUniformShuffleXor (GroupNonUniformShuffle)
    let shuffled = subgroupShuffle(float3(v, v, v), gl_SubgroupInvocationID)
    let butterfly = subgroupShuffleXor(v, 1u)

    // elect -> OpGroupNonUniformElect, whose family IS the base capability (single-lane writeback guard)
    let leader = subgroupElect()

    // arithmetic reductions -> Reduce, with the S/U/F opcode picked by the operand's scalar class
    let fsum = subgroupAdd(float3(v, v, v))
    let isum = subgroupAdd(iv)
    let fmin = subgroupMin(v)
    let fmax = subgroupMax(v)
    let imin = subgroupMin(iv)
    let imax = subgroupMax(iv)
    let umin = subgroupMin(uv)
    let umax = subgroupMax(uv)

    // clustered -> ClusteredReduce + a constant power-of-two ClusterSize (GroupNonUniformClustered)
    let quad_sum = subgroupClusteredAdd(float4(v, v, v, v), 4u)

    // the RHS is wrapped: a continuation line that begins with `+` would otherwise parse as a
    // separate unary-plus statement and be silently dropped
    sg_out[i] = ((uniform_sign ? 1.0f : 0.0f) + (any_zero ? 2.0f : 0.0f)
        + first.x + lane0 + shuffled.y + butterfly + (leader ? 8.0f : 0.0f)
        + fsum.z + fmin + fmax + quad_sum.w
        + float(gl_SubgroupSize) + float(gl_NumSubgroups) + float(gl_SubgroupID))
    sg_iout[i] = isum + imin + imax + int(umin) + int(umax)
}

// A subgroup op carries no execution-model restriction, and gl_SubgroupInvocationID / gl_SubgroupSize
// are legal in every stage -- unlike gl_SubgroupID / gl_NumSubgroups, which name a place in a workgroup
// (that pair is gated, and _fc_subgroup_id_stage holds the rejection).  A fragment shader is the cheapest
// witness for both halves.
var @out @location = 0 sg_frag_color : float4

[fragment_shader(name="subgroup_frag_spv"), marker(no_coverage)]
def subgroup_in_fragment {
    let lane = float(gl_SubgroupInvocationID) / float(gl_SubgroupSize)
    let all_lit = subgroupAll(lane > 0.0f)
    sg_frag_color = float4(lane, all_lit ? 1.0f : 0.0f, 0.0f, 1.0f)
}

// OpGroupNonUniformBroadcast's Id operand had to be a CONSTANT before SPIR-V 1.5, which relaxed it to
// dynamically uniform.  A dynamic lane index is the natural spelling (a loop counter, an invocation id),
// so the emitter raises the module version for it rather than rejecting it -- and leaves a constant Id
// on the default version.  The bump has to settle before the entry-point interface is built, since the
// 1.4+ rule lists every global; that ordering is exactly what spirv-val checks here.
var @ssbo @binding = 0 dyn_in : array<float>
var @ssbo @binding = 1 dyn_out : array<float>

[compute_shader(local_size_x=64, name="subgroup_dyn_spv"), marker(no_coverage)]
def subgroup_dynamic_broadcast {
    let i = gl_GlobalInvocationID.x
    dyn_out[i] = subgroupBroadcast(dyn_in[i], gl_SubgroupInvocationID)
}

// words[1] is the SPIR-V version word of the module header (magic, version, generator, bound, schema).
let VERSION_WORD = 1

def private has_group_op(words : array<uint>; op : SpvOp; gop : SpvGroupOperation) : bool =>
    op_has_operand_at(words, op, GROUP_OP_OPERAND, uint(gop))

def private has_cap(words : array<uint>; cap : SpvCapability) : bool =>
    op_has_operand_at(words, SpvOp.Capability, CAP_OPERAND, uint(cap))

def private has_builtin(words : array<uint>; bi : SpvBuiltIn) : bool =>
    op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.BuiltIn), uint(bi))

[test(description = "Subgroup ops lower to OpGroupNonUniform opcodes with the right group operation and capabilities, a dynamic broadcast Id raises the module to SPIR-V 1.5, and the ops are legal outside a workgroup stage.")]
def test_subgroup_ops(t : T?) { // nolint:STYLE038 - flat list of opcode/capability assertions
    t |> run("subgroup ops lower to OpGroupNonUniform* with the right opcode, group operation and caps") <| @(t : T?) {
        var words <- clone_to_move(sg_all_ops_spv)

        // ----- vote -----
        t |> success(has_op(words, SpvOp.GroupNonUniformAll), "subgroupAll -> OpGroupNonUniformAll")
        t |> success(has_op(words, SpvOp.GroupNonUniformAny), "subgroupAny -> OpGroupNonUniformAny")

        // ----- ballot -----
        t |> success(has_op(words, SpvOp.GroupNonUniformBroadcastFirst),
            "subgroupBroadcastFirst -> OpGroupNonUniformBroadcastFirst")
        t |> success(has_op(words, SpvOp.GroupNonUniformBroadcast),
            "subgroupBroadcast -> OpGroupNonUniformBroadcast")

        // ----- shuffle -----
        t |> success(has_op(words, SpvOp.GroupNonUniformShuffle), "subgroupShuffle -> OpGroupNonUniformShuffle")
        t |> success(has_op(words, SpvOp.GroupNonUniformShuffleXor),
            "subgroupShuffleXor -> OpGroupNonUniformShuffleXor")

        // ----- elect -----
        t |> success(has_op(words, SpvOp.GroupNonUniformElect), "subgroupElect -> OpGroupNonUniformElect")

        // ----- arithmetic: the scalar class picks F / S / U -----
        t |> success(has_op(words, SpvOp.GroupNonUniformFAdd), "subgroupAdd(float3) -> OpGroupNonUniformFAdd")
        t |> success(has_op(words, SpvOp.GroupNonUniformIAdd), "subgroupAdd(int) -> OpGroupNonUniformIAdd")
        t |> success(has_op(words, SpvOp.GroupNonUniformFMin), "subgroupMin(float) -> OpGroupNonUniformFMin")
        t |> success(has_op(words, SpvOp.GroupNonUniformFMax), "subgroupMax(float) -> OpGroupNonUniformFMax")
        t |> success(has_op(words, SpvOp.GroupNonUniformSMin), "subgroupMin(int) -> OpGroupNonUniformSMin")
        t |> success(has_op(words, SpvOp.GroupNonUniformSMax), "subgroupMax(int) -> OpGroupNonUniformSMax")
        t |> success(has_op(words, SpvOp.GroupNonUniformUMin), "subgroupMin(uint) -> OpGroupNonUniformUMin")
        t |> success(has_op(words, SpvOp.GroupNonUniformUMax), "subgroupMax(uint) -> OpGroupNonUniformUMax")

        // ----- the GroupOperation literal separates a plain reduction from a clustered one -----
        t |> success(has_group_op(words, SpvOp.GroupNonUniformIAdd, SpvGroupOperation.Reduce),
            "subgroupAdd(int) carries GroupOperation Reduce")
        t |> success(has_group_op(words, SpvOp.GroupNonUniformFMin, SpvGroupOperation.Reduce),
            "subgroupMin(float) carries GroupOperation Reduce")
        t |> success(has_group_op(words, SpvOp.GroupNonUniformFAdd, SpvGroupOperation.ClusteredReduce),
            "subgroupClusteredAdd carries GroupOperation ClusteredReduce")
        // both FAdd flavours are present: the plain reduction AND the clustered one
        t |> success(has_group_op(words, SpvOp.GroupNonUniformFAdd, SpvGroupOperation.Reduce),
            "subgroupAdd(float3) carries GroupOperation Reduce")

        // ----- capabilities: the base one plus every family the fixture reaches -----
        t |> success(has_cap(words, SpvCapability.GroupNonUniform), "OpCapability GroupNonUniform")
        t |> success(has_cap(words, SpvCapability.GroupNonUniformVote), "OpCapability GroupNonUniformVote")
        t |> success(has_cap(words, SpvCapability.GroupNonUniformBallot), "OpCapability GroupNonUniformBallot")
        t |> success(has_cap(words, SpvCapability.GroupNonUniformArithmetic), "OpCapability GroupNonUniformArithmetic")
        t |> success(has_cap(words, SpvCapability.GroupNonUniformShuffle), "OpCapability GroupNonUniformShuffle")
        t |> success(has_cap(words, SpvCapability.GroupNonUniformClustered), "OpCapability GroupNonUniformClustered")
        // the base capability is emitted once, not once per op family
        t |> success(count_op_operand_at(words, SpvOp.Capability, 0, uint(SpvCapability.GroupNonUniform)) == 1,
            "GroupNonUniform capability is emitted exactly once")

        // ----- the four subgroup builtins -----
        t |> success(has_builtin(words, SpvBuiltIn.SubgroupSize), "gl_SubgroupSize -> BuiltIn SubgroupSize")
        t |> success(has_builtin(words, SpvBuiltIn.SubgroupLocalInvocationId),
            "gl_SubgroupInvocationID -> BuiltIn SubgroupLocalInvocationId")
        t |> success(has_builtin(words, SpvBuiltIn.SubgroupId), "gl_SubgroupID -> BuiltIn SubgroupId")
        t |> success(has_builtin(words, SpvBuiltIn.NumSubgroups), "gl_NumSubgroups -> BuiltIn NumSubgroups")

        // a CONSTANT broadcast Id needs no version bump
        t |> success(words[VERSION_WORD] == SPV_VERSION_1_3, "constant broadcast Id stays on SPIR-V 1.3")

        // spirv-val is the oracle for the Subgroup scope <id> and the constant power-of-two ClusterSize
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
    t |> run("a dynamic broadcast Id raises the module to SPIR-V 1.5, interface rule included") <| @(t : T?) {
        var words <- clone_to_move(subgroup_dyn_spv)
        t |> success(words[VERSION_WORD] == SPV_VERSION_1_5, "dynamic broadcast Id raises the module to SPIR-V 1.5")
        t |> success(has_op(words, SpvOp.GroupNonUniformBroadcast), "dynamic: OpGroupNonUniformBroadcast emitted")
        // the 1.4+ interface rule: every referenced global is listed, the two SSBOs included. The
        // version settled before the entry point was built, so spirv-val finds no unlisted interface.
        let r = validate_spirv(words, "vulkan1.2")
        if (r.ran) {
            t |> success(r.ok, "dynamic: spirv-val (vulkan1.2): {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
    t |> run("subgroup ops and the invocation builtins are legal outside a workgroup stage") <| @(t : T?) {
        var words <- clone_to_move(subgroup_frag_spv)
        let model = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(model.ok && model.value == uint(SpvExecutionModel.Fragment),
            "fragment: EntryPoint model is Fragment")
        t |> success(has_op(words, SpvOp.GroupNonUniformAll), "fragment: subgroupAll lowers (no stage gate)")
        t |> success(has_builtin(words, SpvBuiltIn.SubgroupLocalInvocationId),
            "fragment: gl_SubgroupInvocationID is legal outside a workgroup stage")
        t |> success(has_builtin(words, SpvBuiltIn.SubgroupSize),
            "fragment: gl_SubgroupSize is legal outside a workgroup stage")
        t |> success(has_cap(words, SpvCapability.GroupNonUniform), "fragment: OpCapability GroupNonUniform")
        // an integer fragment Input cannot be interpolated: both uint builtins must be Flat
        t |> success(count_op_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.Flat)) == 2,
            "fragment: both uint subgroup builtins are Flat-decorated")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "fragment: spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}

// Ext-op scalar-broadcast gate (regression for the FMix-scalar-t crash).
//
// `lerp(vecN, vecN, scalar)` lowers to OpExtInst FMix, but SPIR-V FMix requires ALL three operands to
// match the result type — there is no implicit scalar->vector broadcast. So the emitter MUST splat the
// scalar t to the vector width (OpCompositeConstruct) before the OpExtInst. Before that fix, the scalar
// went straight into FMix: the blob still EMITTED (so a bytes-only check passed) but was INVALID SPIR-V
// and blew up as a driver access-violation at vkCreateGraphicsPipelines. The real oracle is spirv-val.
//
// Each lerp result is written straight to its own @out (NO float4(...) constructors), so the only
// OpCompositeConstruct instructions in the module are the three scalar->vector splats — the counts are
// then exact + attributable even when spirv-val soft-skips locally (a missing splat would drop the
// CompositeConstruct count to 0). `lerp` is the only GLSL.std.450 op daslang exposes with a vec+scalar
// overload, so it is the only reachable broadcast case; the emitter's ext_op_broadcasts_scalar also
// covers FClamp/FMin/FMax/Step/SmoothStep defensively (no daslang overload reaches them yet).
var @in @location = 0 eb_a : float4
var @in @location = 1 eb_b : float4
var @in @location = 2 eb_s : float
var @out @location = 0 eb_out2 : float2
var @out @location = 1 eb_out3 : float3
var @out @location = 2 eb_out4 : float4
[fragment_shader(name="extbcast_spv"), marker(no_coverage)]
def extbcast {
    eb_out2 = lerp(eb_a.xy, eb_b.xy, eb_s)      // FMix float2,float2,scalar -> splat t to float2
    eb_out3 = lerp(eb_a.xyz, eb_b.xyz, eb_s)    // FMix float3,float3,scalar -> splat t to float3
    eb_out4 = lerp(eb_a, eb_b, eb_s)            // FMix float4,float4,scalar -> splat t to float4
}

[test(description = "lerp(vec, vec, scalar) lowers to FMix with the scalar t splatted to the vector width.")]
def test_ext_broadcast(t : T?) {
    t |> run("lerp(vec,vec,scalar) -> FMix splats the scalar t to vec width; spirv-val clean") <| @(t : T?) {
        var words <- clone_to_move(extbcast_spv)
        // Exactly the three lerps lower to OpExtInst FMix. GLSLstd450.FMix is operand #3 of OpExtInst
        // (result-type, result-id, set, instruction, args...), so this rejects a non-FMix ext-inst.
        t |> success(count_op_operand_at(words, SpvOp.ExtInst, 3, uint(GLSLstd450.FMix)) == 3,
            "three OpExtInst FMix (lerp lowered to FMix at widths 2/3/4)")
        // The only OpCompositeConstruct in the module are the three scalar->vector splats; if the
        // broadcast were missing, the scalar would go straight into FMix and this count would be 0.
        t |> success(count_op(words, SpvOp.CompositeConstruct) == 3,
            "three OpCompositeConstruct == the scalar->vec splats, and nothing else")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}

// Census gate (gate B): the union of opcodes the emitter produces across EVERY fixture must equal
// the declared supported set, in both directions. Stronger than line coverage — it catches an
// opcode whose emit line ran but was never asserted by any test, and a declared opcode that stopped
// firing. Failure messages resolve the numeric opcode to its OpName for readability.
def private op_name(op : uint) : string => SPV_OP_NAMES?[op] ?? "op#{op}"

def private add_set(var dst : table<uint>; var words : array<uint>) {
    var s <- opcode_set(words)
    for (op in keys(s)) {
        dst |> insert(op)
    }
    delete s
    delete words
}

[test(description = "The set of opcodes the emitter produces across all fixtures equals the declared opcode set.")]
def test_opcode_census(t : T?) { // nolint:STYLE038 — a flat one-add_set-per-fixture list; splitting hides the census roster
    t |> run("emitter opcode census == declared set (all fixtures)") <| @(t : T?) {
        var present : table<uint>
        add_set(present, square_words())
        add_set(present, color_mapping_words())
        add_set(present, uarith_words())
        add_set(present, iarith_words())
        add_set(present, farith_words())
        add_set(present, precise_words())
        add_set(present, plain_sum_words())
        add_set(present, precise_kern_words())
        add_set(present, uctrl_words())
        add_set(present, ictrl_words())
        add_set(present, fctrl_words())
        add_set(present, wloop_words())
        add_set(present, floop_words())
        add_set(present, nloop_words())
        add_set(present, echain_words())
        add_set(present, eret_words())
        add_set(present, rfor_words())
        add_set(present, ufor_words())
        add_set(present, ufold_words())
        add_set(present, coopmat_arr_words())
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
        add_set(present, imgfmt_words())
        add_set(present, lsvmember_words())
        add_set(present, texlod_words())
        add_set(present, clone_to_move(texture_grad_spv))
        add_set(present, dims_words())
        add_set(present, septex_words())
        add_set(present, tern_words())
        add_set(present, tern_lazy_words())
        add_set(present, vecarith_words())
        add_set(present, vts_words())
        add_set(present, mathx_words())
        add_set(present, ssbovec_words())
        add_set(present, ssbostruct_words())
        add_set(present, gswizzle_words())
        add_set(present, fragx_words())
        add_set(present, fragshadow_words())
        add_set(present, cshared_words())
        add_set(present, cossbo_words())
        add_set(present, catomics_words())
        add_set(present, imgquery_words())
        add_set(present, modconst_words())
        add_set(present, userfn_words())
        add_set(present, userfn_ref_words())
        add_set(present, userfn_inline_words())
        add_set(present, userfn_inline_method_words())
        add_set(present, userfn_private_method_words())
        add_set(present, userfn_public_method_words())
        add_set(present, userfn_inline_lazy_words())
        add_set(present, userfn_global_words())
        add_set(present, mesh_empty_words())
        add_set(present, task_empty_words())
        add_set(present, mesh_lines_words())
        add_set(present, mesh_points_words())
        add_set(present, mesh_tri_words())
        add_set(present, mesh_ubo_words())
        add_set(present, mesh_outs_words())
        add_set(present, prim_frag_words())
        add_set(present, pp_frag_words())
        add_set(present, task_payload_words())
        add_set(present, mesh_payload_words())
        add_set(present, rt_raygen_words())
        add_set(present, rt_miss_words())
        add_set(present, rt_chit_words())
        add_set(present, lattice_words())
        add_set(present, subgroup_words())
        add_set(present, specc_words())
        add_set(present, q8blk_words())
        add_set(present, sdot_words())
        add_set(present, coopmat_words())
        add_set(present, coopmat_f16acc_words())
        add_set(present, coopmat_i8_words())
        add_set(present, coopmat_sm_words())
        add_set(present, coopmat2_words())
        add_set(present, coopmat2s_words())
        add_set(present, coopmat2s64_words())
        add_set(present, coopmat2i8_words())
        add_set(present, coopmat2e_words())
        add_set(present, coopmat2fa_words())
        add_set(present, coopmat2fa256_words())
        add_set(present, coopmat2fa512_words())
        add_set(present, q8store_words())
        add_set(present, unpack16_words())
        add_set(present, spdirect_words())
        add_set(present, spelem_words())
        add_set(present, spelemi_words())
        var declared <- coopmat2fa_emitter_opcodes()
        declared |> insert(uint(SpvOp.Unreachable))
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
