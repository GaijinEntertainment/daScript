options gen2
options indenting = 4

// Phase-3 vertex/fragment gate: stage I/O (@in/@out -> Input/Output + Location), rasterizer builtins
// (gl_Position/gl_VertexIndex/gl_InstanceIndex/gl_FragCoord), vector constructors (OpCompositeConstruct),
// dot (OpDot), and GLSL.std.450 math (OpExtInstImport + OpExtInst). The entry-point execution model
// + interface + decorations are asserted by shape; every emitted module passes spirv-val.

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_dis
require spirv/spirv_grammar

def private check_ops(t : T?; words : array<uint>; lbl : string; ops : array<SpvOp>) {
    for (op in ops) {
        t |> success(has_op(words, op), "{lbl}: emits {op}")
    }
}

def private validate(t : T?; words : array<uint>; lbl : string) {
    let r = validate_spirv(words)
    if (r.ran) {
        t |> success(r.ok, "{lbl}: spirv-val: {r.msg}")
    } else {
        feint("spirv-val not found locally; skipping (CI enforces)\n")
    }
}

def private has_builtin(words : array<uint>; bi : SpvBuiltIn) : bool {
    return op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.BuiltIn), uint(bi))
}

def private has_location(words : array<uint>; loc : uint) : bool {
    return op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.Location), loc)
}

[test]
def test_vertex_passthrough(t : T?) {
    t |> run("vertex: @in/@out + Location, gl_Position, float4(...) -> CompositeConstruct") <| @(t : T?) {
        var words <- tri_vert_words()
        // entry point is a Vertex shader with no execution mode (assert .ok so an empty/truncated
        // module can't pass via the default value 0, which collides with SpvExecutionModel.Vertex)
        let vmodel = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(vmodel.ok && vmodel.value == uint(SpvExecutionModel.Vertex),
            "tri_vert: EntryPoint model is Vertex")
        t |> success(count_op(words, SpvOp.ExecutionMode) == 0, "tri_vert: vertex has no ExecutionMode")
        // stage I/O Locations (a_pos=0, a_color=1, v_color=0) + the Position builtin
        t |> success(has_location(words, 0u), "tri_vert: has a Location 0 decoration")
        t |> success(has_location(words, 1u), "tri_vert: has a Location 1 decoration")
        t |> success(has_builtin(words, SpvBuiltIn.Position), "tri_vert: gl_Position -> BuiltIn Position")
        // an Output-storage variable (gl_Position + v_color). OpVariable operands are
        // [result_type, result_id, storage_class, init?] — check the storage class positionally
        // (index 2), since a bare value match would also hit the small result-type/id operands.
        t |> success(op_has_operand_at(words, SpvOp.Variable, 2, uint(SpvStorageClass.Output)),
            "tri_vert: has an Output-storage OpVariable")
        check_ops(t, words, "tri_vert", [SpvOp.CompositeConstruct, SpvOp.Load, SpvOp.Store])
        validate(t, words, "tri_vert")
        delete words
    }
}

[test]
def test_fragment_passthrough(t : T?) {
    t |> run("fragment: OriginUpperLeft, float4(vec3, 1.0) -> CompositeConstruct") <| @(t : T?) {
        var words <- tri_frag_words()
        let fmodel = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(fmodel.ok && fmodel.value == uint(SpvExecutionModel.Fragment),
            "tri_frag: EntryPoint model is Fragment")
        // ExecutionMode operands are [entry_id, mode, ...]; check the mode positionally (index 1)
        // so a small mode value can't false-match the entry-id operand.
        t |> success(op_has_operand_at(words, SpvOp.ExecutionMode, 1, uint(SpvExecutionMode.OriginUpperLeft)),
            "tri_frag: ExecutionMode OriginUpperLeft")
        t |> success(has_location(words, 0u), "tri_frag: has a Location 0 decoration")
        check_ops(t, words, "tri_frag", [SpvOp.CompositeConstruct, SpvOp.Load, SpvOp.Store])
        validate(t, words, "tri_frag")
        delete words
    }
}

[test]
def test_vertex_index_builtins(t : T?) {
    t |> run("vertex: gl_VertexIndex / gl_InstanceIndex builtins drive a procedural position") <| @(t : T?) {
        var words <- vindex_words()
        t |> success(has_builtin(words, SpvBuiltIn.VertexIndex), "vindex: BuiltIn VertexIndex")
        t |> success(has_builtin(words, SpvBuiltIn.InstanceIndex), "vindex: BuiltIn InstanceIndex")
        t |> success(has_builtin(words, SpvBuiltIn.Position), "vindex: BuiltIn Position")
        check_ops(t, words, "vindex", [SpvOp.IEqual, SpvOp.SGreaterThan, SpvOp.CompositeConstruct])
        validate(t, words, "vindex")
        delete words
    }
}

[test]
def test_vertex_point_size(t : T?) {
    t |> run("vertex: gl_PointSize (BuiltIn PointSize Output) for POINT_LIST rasterisation") <| @(t : T?) {
        var words <- point_size_words()
        let vmodel = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(vmodel.ok && vmodel.value == uint(SpvExecutionModel.Vertex),
            "point_size: EntryPoint model is Vertex")
        // gl_PointSize -> BuiltIn PointSize decoration on a vertex-stage Output variable
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.BuiltIn), uint(SpvBuiltIn.PointSize)),
            "point_size: gl_PointSize -> BuiltIn PointSize")
        t |> success(has_builtin(words, SpvBuiltIn.Position),
            "point_size: gl_Position still present (Vertex shaders must write it)")
        // an Output-storage variable (gl_Position + gl_PointSize)
        t |> success(op_has_operand_at(words, SpvOp.Variable, 2, uint(SpvStorageClass.Output)),
            "point_size: has an Output-storage OpVariable")
        check_ops(t, words, "point_size", [SpvOp.Store])
        validate(t, words, "point_size")
        delete words
    }
}

[test]
def test_fragment_point_coord(t : T?) {
    t |> run("fragment: gl_PointCoord (BuiltIn PointCoord Input) for point-sprite UV") <| @(t : T?) {
        var words <- point_coord_words()
        let fmodel = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(fmodel.ok && fmodel.value == uint(SpvExecutionModel.Fragment),
            "point_coord: EntryPoint model is Fragment")
        // gl_PointCoord -> BuiltIn PointCoord decoration on a fragment-stage Input variable
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.BuiltIn), uint(SpvBuiltIn.PointCoord)),
            "point_coord: gl_PointCoord -> BuiltIn PointCoord")
        t |> success(op_has_operand_at(words, SpvOp.Variable, 2, uint(SpvStorageClass.Input)),
            "point_coord: has an Input-storage OpVariable")
        check_ops(t, words, "point_coord", [SpvOp.CompositeConstruct, SpvOp.Load, SpvOp.Store])
        validate(t, words, "point_coord")
        delete words
    }
}

[test]
def test_fragment_math(t : T?) {
    t |> run("fragment: gl_FragCoord + GLSL.std.450 math (dot/sqrt/clamp)") <| @(t : T?) {
        var words <- raster_math_words()
        t |> success(has_builtin(words, SpvBuiltIn.FragCoord), "raster_math: BuiltIn FragCoord")
        check_ops(t, words, "raster_math", [SpvOp.Dot, SpvOp.ExtInstImport, SpvOp.ExtInst,
            SpvOp.CompositeConstruct])
        validate(t, words, "raster_math")
        delete words
    }
}
