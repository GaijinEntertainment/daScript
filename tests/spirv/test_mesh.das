options gen2
options indenting = 4

// PR-D mesh + task stage scaffolding (VK_EXT_mesh_shader). Each fixture asserts:
//   * the right execution model (MeshEXT 5365 / TaskEXT 5364) in the entry point's first operand
//   * OpCapability MeshShadingEXT (5283) emitted exactly once
//   * OpExtension "SPV_EXT_mesh_shader" emitted exactly once
//   * the stage-required execution modes (mesh: LocalSize + OutputVertices + OutputPrimitivesEXT +
//     OutputTrianglesEXT; task: LocalSize)
//   * the prologue intrinsic (SetMeshOutputsEXT in mesh; EmitMeshTasksEXT in task)
//   * spirv-val passes against vulkan1.3 (the first env that supports SPV_EXT_mesh_shader natively).
// Per-vertex / per-primitive output arrays (gl_MeshVerticesEXT, gl_PrimitiveTriangleIndicesEXT, ...)
// and the lines / points topology selectors arrive in PR-E together with the array-of-builtin
// classifier and the topology-via-annotation-arg surface. PR-D hardcodes 64-vertex / 64-primitive /
// triangles.

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_dis
require spirv/spirv_grammar
require daslib/strings_boost

def private validate(t : T?; words : array<uint>; lbl : string) {
    let r = validate_spirv(words, "vulkan1.3")
    if (r.ran) {
        t |> success(r.ok, "{lbl}: spirv-val: {r.msg}")
    } else {
        feint("spirv-val not found locally; skipping (CI enforces)\n")
    }
}

// OpExtension's operand words encode the extension-name string (LSB-first bytes, NUL-terminated,
// zero-padded). Reassemble bytes across each OpExtension and substring-search the requested name.
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

// Walk every OpExecutionMode and check whether one of them lists `mode` as the second operand
// (operand 1). The first operand (operand 0) is the entry-point id; the mode literal sits at index 1.
def private has_execution_mode(words : array<uint>; mode : SpvExecutionMode) : bool {
    return op_has_operand_at(words, SpvOp.ExecutionMode, 1, uint(mode))
}

// Read the operand at `opIdx` (0-based operand index, after word0 — same convention as
// op_has_operand_at) of the OpExecutionMode whose mode literal (operand 1) equals `mode`. The count
// for OutputVertices / OutputPrimitivesEXT sits at operand index 2 (operand 0 is the entry-point id,
// operand 1 the mode literal). Returns the first matching mode's operand.
def private exec_mode_operand(words : array<uint>; mode : SpvExecutionMode; opIdx : int) : tuple<found : bool; value : uint> {
    var res = (found = false, value = 0u)
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (res.found || opcode != uint(SpvOp.ExecutionMode)) return
        if (n > opIdx && w[s + 1] == uint(mode)) {
            res = (found = true, value = w[s + opIdx])
        }
    }
    return res
}

// Count the EntryPoint's interface ids (the trailing operands after the null-terminated entry name).
// OpEntryPoint operands: 0 = execution model, 1 = entry-point id, 2.. = the name string (packed
// LSB-first, NUL-terminated, so the first operand word carrying a zero byte is the terminator), then
// one operand per interface variable. Used to assert the mesh outputs are listed (SPIR-V 1.4 requires
// every referenced global in the interface).
def private entry_point_interface_count(words : array<uint>) : int {
    var count = -1
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (count >= 0 || opcode != uint(SpvOp.EntryPoint)) return
        var i = 2
        var name_done = false
        while (i < n && !name_done) {
            let wd = w[s + i]
            for (b in range(4)) {
                if (((wd >> uint(b * 8)) & 0xFFu) == 0u) {
                    name_done = true
                }
            }
            i++
        }
        count = n - i
    }
    return count
}

[test]
def test_mesh_stage_scaffolding(t : T?) {
    t |> run("[mesh_shader] empty: cap + ext + execution model + execution modes + SetMeshOutputsEXT") <| @(t : T?) {
        var words <- mesh_empty_words()
        // SPIR-V header version word (index 1) must be 1.4 — SPV_EXT_mesh_shader requires >= 1.4.
        // Asserted directly (not only via spirv-val, which soft-skips when the SDK is absent) so a
        // regression back to 1.3 is caught in every tier.
        t |> success(words[1] == SPV_VERSION_1_4, "mesh_empty: SPIR-V header version is 1.4")
        // OpCapability MeshShadingEXT — emitted once.
        t |> success(op_has_operand_at(words, SpvOp.Capability, 0, uint(SpvCapability.MeshShadingEXT)),
            "mesh_empty: OpCapability MeshShadingEXT")
        let ncap = count_op_operand_at(words, SpvOp.Capability, 0, uint(SpvCapability.MeshShadingEXT))
        t |> success(ncap == 1, "mesh_empty: exactly one MeshShadingEXT cap (got {ncap})")
        // OpExtension "SPV_EXT_mesh_shader" — emitted once.
        t |> success(has_extension(words, "SPV_EXT_mesh_shader"),
            "mesh_empty: OpExtension SPV_EXT_mesh_shader")
        t |> success(count_op(words, SpvOp.Extension) == 1,
            "mesh_empty: exactly one OpExtension (got {count_op(words, SpvOp.Extension)})")
        // ExecutionModel MeshEXT in the OpEntryPoint header (operand 0 is the execution model).
        t |> success(op_has_operand_at(words, SpvOp.EntryPoint, 0, uint(SpvExecutionModel.MeshEXT)),
            "mesh_empty: OpEntryPoint MeshEXT")
        // Mesh execution-mode trio (all MANDATORY per SPV_EXT_mesh_shader for a mesh entry point):
        t |> success(has_execution_mode(words, SpvExecutionMode.LocalSize),
            "mesh_empty: ExecutionMode LocalSize")
        t |> success(has_execution_mode(words, SpvExecutionMode.OutputVertices),
            "mesh_empty: ExecutionMode OutputVertices")
        t |> success(has_execution_mode(words, SpvExecutionMode.OutputPrimitivesEXT),
            "mesh_empty: ExecutionMode OutputPrimitivesEXT")
        t |> success(has_execution_mode(words, SpvExecutionMode.OutputTrianglesEXT),
            "mesh_empty: ExecutionMode OutputTrianglesEXT")
        // The prologue intrinsic itself.
        t |> success(has_op(words, SpvOp.SetMeshOutputsEXT), "mesh_empty: emits OpSetMeshOutputsEXT")
        t |> success(count_op(words, SpvOp.SetMeshOutputsEXT) == 1,
            "mesh_empty: exactly one OpSetMeshOutputsEXT (got {count_op(words, SpvOp.SetMeshOutputsEXT)})")
        validate(t, words, "mesh_empty")
        delete words
    }
}

[test]
def test_task_stage_scaffolding(t : T?) {
    t |> run("[task_shader] empty: cap + ext + execution model + LocalSize + EmitMeshTasksEXT terminator") <| @(tt : T?) {
        var words <- task_empty_words()
        // Header version word must be 1.4 (SPV_EXT_mesh_shader); asserted directly so a 1.3 regression
        // is caught even when spirv-val soft-skips.
        tt |> success(words[1] == SPV_VERSION_1_4, "task_empty: SPIR-V header version is 1.4")
        // Same MeshShadingEXT cap + extension as mesh (both stages share VK_EXT_mesh_shader).
        tt |> success(op_has_operand_at(words, SpvOp.Capability, 0, uint(SpvCapability.MeshShadingEXT)),
            "task_empty: OpCapability MeshShadingEXT")
        tt |> success(has_extension(words, "SPV_EXT_mesh_shader"),
            "task_empty: OpExtension SPV_EXT_mesh_shader")
        // ExecutionModel TaskEXT.
        tt |> success(op_has_operand_at(words, SpvOp.EntryPoint, 0, uint(SpvExecutionModel.TaskEXT)),
            "task_empty: OpEntryPoint TaskEXT")
        // Task needs only LocalSize (no OutputVertices / OutputPrimitivesEXT / output topology).
        tt |> success(has_execution_mode(words, SpvExecutionMode.LocalSize),
            "task_empty: ExecutionMode LocalSize")
        tt |> success(!has_execution_mode(words, SpvExecutionMode.OutputVertices),
            "task_empty: NO ExecutionMode OutputVertices (task is not a mesh)")
        tt |> success(!has_execution_mode(words, SpvExecutionMode.OutputPrimitivesEXT),
            "task_empty: NO ExecutionMode OutputPrimitivesEXT")
        tt |> success(!has_execution_mode(words, SpvExecutionMode.OutputTrianglesEXT),
            "task_empty: NO ExecutionMode OutputTrianglesEXT")
        // The dispatch terminator.
        tt |> success(has_op(words, SpvOp.EmitMeshTasksEXT), "task_empty: emits OpEmitMeshTasksEXT")
        tt |> success(count_op(words, SpvOp.EmitMeshTasksEXT) == 1,
            "task_empty: exactly one OpEmitMeshTasksEXT (got {count_op(words, SpvOp.EmitMeshTasksEXT)})")
        validate(tt, words, "task_empty")
        delete words
    }
}

[test]
def test_mesh_output_config(t : T?) {
    t |> run("[mesh_shader] output config: max_vertices / max_primitives / output_topology -> execution modes") <| @(t : T?) {
        // mesh_lines: max_vertices=96, max_primitives=32, output_topology="lines".
        var lwords <- mesh_lines_words()
        let lv = exec_mode_operand(lwords, SpvExecutionMode.OutputVertices, 2)
        t |> success(lv.found && lv.value == 96u, "mesh_lines: OutputVertices count == 96 (found={lv.found} value={lv.value})")
        let lp = exec_mode_operand(lwords, SpvExecutionMode.OutputPrimitivesEXT, 2)
        t |> success(lp.found && lp.value == 32u, "mesh_lines: OutputPrimitivesEXT count == 32 (found={lp.found} value={lp.value})")
        t |> success(has_execution_mode(lwords, SpvExecutionMode.OutputLinesEXT), "mesh_lines: ExecutionMode OutputLinesEXT")
        t |> success(!has_execution_mode(lwords, SpvExecutionMode.OutputTrianglesEXT), "mesh_lines: NO OutputTrianglesEXT (topology is lines)")
        t |> success(!has_execution_mode(lwords, SpvExecutionMode.OutputPoints), "mesh_lines: NO OutputPoints")
        validate(t, lwords, "mesh_lines")
        delete lwords
        // mesh_points: max_vertices=48, max_primitives=16, output_topology="points".
        var pwords <- mesh_points_words()
        let pv = exec_mode_operand(pwords, SpvExecutionMode.OutputVertices, 2)
        t |> success(pv.found && pv.value == 48u, "mesh_points: OutputVertices count == 48 (found={pv.found} value={pv.value})")
        let pp = exec_mode_operand(pwords, SpvExecutionMode.OutputPrimitivesEXT, 2)
        t |> success(pp.found && pp.value == 16u, "mesh_points: OutputPrimitivesEXT count == 16 (found={pp.found} value={pp.value})")
        t |> success(has_execution_mode(pwords, SpvExecutionMode.OutputPoints), "mesh_points: ExecutionMode OutputPoints")
        t |> success(!has_execution_mode(pwords, SpvExecutionMode.OutputTrianglesEXT), "mesh_points: NO OutputTrianglesEXT (topology is points)")
        t |> success(!has_execution_mode(pwords, SpvExecutionMode.OutputLinesEXT), "mesh_points: NO OutputLinesEXT")
        validate(t, pwords, "mesh_points")
        delete pwords
    }
}

[test]
def test_mesh_output_arrays(t : T?) {
    t |> run("[mesh_shader] per-vertex/per-primitive output arrays: gl_MeshVerticesEXT + gl_PrimitiveTriangleIndicesEXT") <| @(t : T?) {
        var words <- mesh_tri_words()
        // per-vertex output block: an OpTypeStruct decorated Block whose member 0 is BuiltIn Position.
        t |> success(op_has_operand_pair(words, SpvOp.MemberDecorate, uint(SpvDecoration.BuiltIn), uint(SpvBuiltIn.Position)),
            "mesh_tri: per-vertex block member 0 is BuiltIn Position")
        t |> success(op_has_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.Block)),
            "mesh_tri: per-vertex block is Block-decorated")
        // per-primitive index array: the variable itself is BuiltIn PrimitiveTriangleIndicesEXT.
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.BuiltIn), uint(SpvBuiltIn.PrimitiveTriangleIndicesEXT)),
            "mesh_tri: index array is BuiltIn PrimitiveTriangleIndicesEXT")
        // output sizing matches the [mesh_shader] args (3 vertices, 1 primitive, triangles).
        let mv = exec_mode_operand(words, SpvExecutionMode.OutputVertices, 2)
        t |> success(mv.found && mv.value == 3u, "mesh_tri: OutputVertices == 3 (found={mv.found} value={mv.value})")
        let mp = exec_mode_operand(words, SpvExecutionMode.OutputPrimitivesEXT, 2)
        t |> success(mp.found && mp.value == 1u, "mesh_tri: OutputPrimitivesEXT == 1 (found={mp.found} value={mp.value})")
        t |> success(has_execution_mode(words, SpvExecutionMode.OutputTrianglesEXT), "mesh_tri: ExecutionMode OutputTrianglesEXT")
        // prologue + the four stores (3 per-vertex positions + 1 per-primitive index triple).
        t |> success(has_op(words, SpvOp.SetMeshOutputsEXT), "mesh_tri: emits OpSetMeshOutputsEXT")
        t |> success(count_op(words, SpvOp.Store) >= 4,
            "mesh_tri: >= 4 OpStore (3 positions + 1 index, got {count_op(words, SpvOp.Store)})")
        // mesh entry point + both mesh outputs listed in its interface (SPIR-V 1.4 requires every
        // referenced global in the interface; the shader references exactly the two mesh-output arrays).
        t |> success(op_has_operand_at(words, SpvOp.EntryPoint, 0, uint(SpvExecutionModel.MeshEXT)),
            "mesh_tri: OpEntryPoint MeshEXT")
        let niface = entry_point_interface_count(words)
        t |> success(niface == 2, "mesh_tri: OpEntryPoint interface lists both mesh outputs (got {niface})")
        validate(t, words, "mesh_tri")
        delete words
    }
}

[test]
def test_mesh_user_outputs(t : T?) {
    t |> run("[mesh_shader] user + per-primitive-builtin outputs: per-vertex varying + @per_primitive + gl_MeshPrimitivesEXT") <| @(t : T?) {
        var words <- mesh_outs_words()
        // per-vertex @out varying mo_vcolor: a Location-decorated Output array (Location 0).
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.Location), 0u),
            "mesh_outs: per-vertex varying has Location 0")
        // per-primitive @out mo_pcolor: Location 1 + PerPrimitiveEXT on the variable.
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.Location), 1u),
            "mesh_outs: per-primitive output has Location 1")
        t |> success(op_has_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.PerPrimitiveEXT)),
            "mesh_outs: per-primitive output variable is PerPrimitiveEXT")
        // gl_MeshPrimitivesEXT block: member 0 = BuiltIn PrimitiveId + PerPrimitiveEXT (both member decos).
        t |> success(op_has_operand_pair(words, SpvOp.MemberDecorate, uint(SpvDecoration.BuiltIn), uint(SpvBuiltIn.PrimitiveId)),
            "mesh_outs: gl_MeshPrimitivesEXT member 0 is BuiltIn PrimitiveId")
        t |> success(op_has_operand_at(words, SpvOp.MemberDecorate, 2, uint(SpvDecoration.PerPrimitiveEXT)),
            "mesh_outs: gl_MeshPrimitivesEXT member 0 is PerPrimitiveEXT")
        // a per-vertex int varying (mo_id) must be Flat — matches the auto-Flat fragment-side int input.
        t |> success(op_has_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.Flat)),
            "mesh_outs: per-vertex int varying is Flat")
        // six outputs in the interface: gl_MeshVerticesEXT, gl_PrimitiveTriangleIndicesEXT,
        // gl_MeshPrimitivesEXT, mo_vcolor, mo_pcolor, mo_id.
        let niface = entry_point_interface_count(words)
        t |> success(niface == 6, "mesh_outs: OpEntryPoint interface lists all 6 outputs (got {niface})")
        validate(t, words, "mesh_outs")
        delete words
        // pp_frag: a fragment reading a @per_primitive @in WITHOUT gl_PrimitiveID — the PerPrimitiveEXT
        // decoration alone must still pull in the MeshShadingEXT cap (-> 1.4), else the blob is invalid.
        var ppwords <- pp_frag_words()
        t |> success(ppwords[1] == SPV_VERSION_1_4, "pp_frag: SPIR-V header version is 1.4 (PerPrimitiveEXT needs MeshShadingEXT)")
        t |> success(op_has_operand_at(ppwords, SpvOp.Capability, 0, uint(SpvCapability.MeshShadingEXT)),
            "pp_frag: OpCapability MeshShadingEXT")
        t |> success(op_has_operand_at(ppwords, SpvOp.Decorate, 1, uint(SpvDecoration.PerPrimitiveEXT)),
            "pp_frag: @per_primitive @in is PerPrimitiveEXT-decorated")
        validate(t, ppwords, "pp_frag")
        delete ppwords
        // prim_frag: the fragment side reads gl_PrimitiveID — BuiltIn PrimitiveId + Flat (int input) +
        // the MeshShadingEXT capability (-> SPIR-V 1.4).
        var fwords <- prim_frag_words()
        t |> success(fwords[1] == SPV_VERSION_1_4, "prim_frag: SPIR-V header version is 1.4 (MeshShadingEXT)")
        t |> success(op_has_operand_at(fwords, SpvOp.Capability, 0, uint(SpvCapability.MeshShadingEXT)),
            "prim_frag: OpCapability MeshShadingEXT")
        t |> success(op_has_operand_pair(fwords, SpvOp.Decorate, uint(SpvDecoration.BuiltIn), uint(SpvBuiltIn.PrimitiveId)),
            "prim_frag: gl_PrimitiveID is BuiltIn PrimitiveId")
        t |> success(op_has_operand_at(fwords, SpvOp.Decorate, 1, uint(SpvDecoration.Flat)),
            "prim_frag: gl_PrimitiveID is Flat (mandatory for an int fragment input)")
        validate(t, fwords, "prim_frag")
        delete fwords
    }
}
