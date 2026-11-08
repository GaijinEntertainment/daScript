options gen2

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_dis
require spirv/spirv_grammar
require spirv/spirv_shader
require spirv/spirv_builtins public
require math
require daslib/strings_boost

// Phase-10.3 gate: fragment realism. discard() -> OpKill (a block terminator); screen-space
// derivatives dFdx/dFdy/fwidth -> OpDPdx/OpDPdy/OpFwidth; textureSize -> OpImageQuerySizeLod (which
// pulls in the ImageQuery capability); and a gl_FragDepth write -> BuiltIn FragDepth Output + the
// DepthReplacing execution mode. Each opcode/decoration is asserted positionally, and the module is
// spirv-val-clean -- the real check that OpKill terminates its block and the query/derivative results
// are well-typed.
def private validate(t : T?; words : array<uint>; lbl : string) {
    let r = validate_spirv(words)
    if (r.ran) {
        t |> success(r.ok, "{lbl}: spirv-val: {r.msg}")
    } else {
        feint("spirv-val not found locally; skipping (CI enforces)\n")
    }
}

[test(description = "A fragment shader using discard, derivatives, textureSize and gl_FragDepth emits the matching opcodes and validates.")]
def test_fragment_realism(t : T?) {
    t |> run("fragment realism: discard/derivatives/textureSize/gl_FragDepth") <| @(t : T?) {
        var words <- fragx_words()
        // a fragment entry point
        let model = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(model.ok && model.value == uint(SpvExecutionModel.Fragment),
            "fragx: EntryPoint model is Fragment")
        // discard() -> OpKill (the fragment-abandon terminator)
        t |> success(has_op(words, SpvOp.Kill), "fragx: discard() -> OpKill")
        // screen-space derivatives over the 2x2 quad
        t |> success(has_op(words, SpvOp.DPdx), "fragx: dFdx -> OpDPdx")
        t |> success(has_op(words, SpvOp.DPdy), "fragx: dFdy -> OpDPdy")
        t |> success(has_op(words, SpvOp.Fwidth), "fragx: fwidth -> OpFwidth")
        // textureSize -> OpImageQuerySizeLod + the ImageQuery capability it requires
        t |> success(has_op(words, SpvOp.ImageQuerySizeLod), "fragx: textureSize -> OpImageQuerySizeLod")
        t |> success(op_has_operand_at(words, SpvOp.Capability, 0, uint(SpvCapability.ImageQuery)),
            "fragx: textureSize pulls in OpCapability ImageQuery")
        // gl_FragDepth write -> BuiltIn FragDepth decoration + the DepthReplacing execution mode
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.BuiltIn), uint(SpvBuiltIn.FragDepth)),
            "fragx: gl_FragDepth -> BuiltIn FragDepth")
        t |> success(op_has_operand_at(words, SpvOp.ExecutionMode, 1, uint(SpvExecutionMode.DepthReplacing)),
            "fragx: gl_FragDepth write -> DepthReplacing execution mode")
        validate(t, words, "fragx")
        delete words
    }
}

[test(description = "early_fragment_tests emits the EarlyFragmentTests execution mode and validates.")]
def test_early_fragment_tests(t : T?) {
    t |> run("early_fragment_tests -> EarlyFragmentTests execution mode; spirv-val clean") <| @(t : T?) {
        var words <- early_frag_words()
        let model = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(model.ok && model.value == uint(SpvExecutionModel.Fragment),
            "early_frag: EntryPoint model is Fragment")
        // [fragment_shader(early_fragment_tests=true)] -> OpExecutionMode ... EarlyFragmentTests
        t |> success(op_has_operand_at(words, SpvOp.ExecutionMode, 1, uint(SpvExecutionMode.EarlyFragmentTests)),
            "early_frag: early_fragment_tests -> EarlyFragmentTests execution mode")
        // OriginUpperLeft is still emitted (every fragment shader); DepthReplacing is NOT (no gl_FragDepth).
        t |> success(op_has_operand_at(words, SpvOp.ExecutionMode, 1, uint(SpvExecutionMode.OriginUpperLeft)),
            "early_frag: OriginUpperLeft still present")
        t |> success(!op_has_operand_at(words, SpvOp.ExecutionMode, 1, uint(SpvExecutionMode.DepthReplacing)),
            "early_frag: no DepthReplacing (no gl_FragDepth write)")
        validate(t, words, "early_frag")
        delete words
    }
}

// Fragment-shader barycentrics (VK_KHR_fragment_shader_barycentric).  A `@pervertex` input ARRAY carries
// the PerVertexKHR decoration and is indexed [i] (OpAccessChain off the variable) to fetch vertex i's raw
// value -- never interpolated, so never Flat even for an int element.  gl_BaryCoordEXT (BuiltIn
// BaryCoordKHR) gives the perspective-correct weights.  Either one pulls in OpCapability
// FragmentBarycentricKHR + OpExtension "SPV_KHR_fragment_shader_barycentric".  The blob is spirv-val clean.
var @in @location = 0 @pervertex in_vert_i : int[3]
var @out @location = 0            out_color : float4

[fragment_shader(name="bary_spv"), marker(no_coverage)]
def bary_frag {
    let w = gl_BaryCoordEXT
    let v = float3(float(in_vert_i[0]), float(in_vert_i[1]), float(in_vert_i[2]))
    out_color = float4(dot(v, w), w.x, w.y, w.z)
}

def public bary_words : array<uint> => clone_to_move(bary_spv)

// OpExtension operand words encode the extension-name string (LSB-first bytes, NUL-terminated).
// Reassemble across each OpExtension and substring-search the requested name.
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

[test(description = "A @pervertex input read through gl_BaryCoordEXT emits PerVertexKHR, BaryCoordKHR and FragmentBarycentricKHR and validates.")]
def test_fragment_barycentric(t : T?) {
    t |> run("@pervertex input + gl_BaryCoordEXT -> PerVertexKHR / BaryCoordKHR / FragmentBarycentricKHR; spirv-val clean") <| @(t : T?) {
        var words <- clone_to_move(bary_spv)
        t |> success(op_has_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.PerVertexKHR)),
            "the @pervertex input is decorated PerVertexKHR")
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.BuiltIn), uint(SpvBuiltIn.BaryCoordKHR)),
            "gl_BaryCoordEXT decorated BuiltIn BaryCoordKHR")
        // The capability is operand 0 of OpCapability, so an id equal to the enum cannot match.
        t |> success(count_op_operand_at(words, SpvOp.Capability, 0, uint(SpvCapability.FragmentBarycentricKHR)) == 1,
            "FragmentBarycentricKHR declared exactly once")
        t |> success(has_extension(words, "SPV_KHR_fragment_shader_barycentric"),
            "OpExtension SPV_KHR_fragment_shader_barycentric")
        // an integer per-vertex input must NOT be Flat (per-vertex inputs carry no interpolation mode)
        t |> success(count_op_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.Flat)) == 0,
            "the @pervertex int input is not Flat")
        // in_vert_i[0..2] -> three OpAccessChain off the input array
        t |> success(count_op(words, SpvOp.AccessChain) == 3,
            "three in_vert_i[i] reads -> three OpAccessChain (got {count_op(words, SpvOp.AccessChain)})")
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}
