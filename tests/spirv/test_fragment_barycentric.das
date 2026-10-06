options gen2
options indenting = 4

// Fragment-shader barycentrics (VK_KHR_fragment_shader_barycentric).  A `@pervertex` input ARRAY carries
// the PerVertexKHR decoration and is indexed [i] (OpAccessChain off the variable) to fetch vertex i's raw
// value -- never interpolated, so never Flat even for an int element.  gl_BaryCoordEXT (BuiltIn
// BaryCoordKHR) gives the perspective-correct weights.  Either one pulls in OpCapability
// FragmentBarycentricKHR + OpExtension "SPV_KHR_fragment_shader_barycentric".  The blob is spirv-val clean.

require dastest/testing_boost public
require _spirv_common               // validate_spirv
require spirv/spirv_shader          // [fragment_shader]
require spirv/spirv_builtins public // gl_BaryCoordEXT
require spirv/spirv_dis             // count_op / foreach_inst / op_has_operand*
require spirv/spirv_grammar         // SpvOp / SpvDecoration / SpvCapability / SpvBuiltIn
require math                        // dot
require daslib/strings_boost        // build_string / write_char / find

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

[test]
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
