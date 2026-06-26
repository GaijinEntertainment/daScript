options gen2
options indenting = 4

// Regression gate for the runtime matrix-/vector-/array-column index emitter gap. Indexing a matrix
// (or vector / fixed array) by a NON-constant value lowers, under the JIT tier, to a
// `.[](base, index, __context__, __lineinfo__)` operator call -- the bounds-check carries the context +
// line for an out-of-range panic. GLSL has its own (non-trapping) out-of-bounds behavior, so the emitter
// drops the check and writes `base[index]`; before the fix this errored with `__context__ is not
// supported` (error[50501]) under JIT, even though an optimized compile folded it away. A const index
// stays a plain ExprAt and was never affected.
//
// The asserts check the emitted GLSL has plain `[...]` indexing and that NONE of the bounds-check
// machinery (`.[]`, `__context__`, `__lineinfo__`) leaks through. Uniform-free, so no opengl_boost.

require dastest/testing_boost public
require glsl/glsl_opengl
require math
require strings

var @in @location a_uv : float2
var @out o_color : float4

[fragment_program(name="mcol_glsl")]
def mcol_shader {
    var m : float4x4
    let i = int(a_uv.x)
    let j = int(a_uv.y)
    m[i] = float4(a_uv, 0.0, 1.0)           // matrix-column WRITE, runtime index
    let col = m[j]                           // matrix-column READ
    let comp = m[i][j]                       // 2D index
    let v = float4(1.0, 2.0, 3.0, 4.0)
    let vc = v[i]                            // vector runtime index
    o_color = col + float4(comp + vc)
}

def private has(hay, needle : string) : bool => find(hay, needle) >= 0

[test]
def test_matrix_column_index(t : T?) {
    t |> run("runtime matrix-column index emits plain base[index]") <| @(t : T?) {
        t |> success(has(mcol_glsl, "m[") && has(mcol_glsl, "v["), "emits m[...] and v[...] indexing")
    }
    t |> run("the .[] bounds-check operator call does not leak into the GLSL") <| @(t : T?) {
        t |> success(!has(mcol_glsl, ".[]"), "no .[] call in the emitted GLSL")
    }
    t |> run("no __context__ / __lineinfo__ bounds-check args leak into the GLSL") <| @(t : T?) {
        t |> success(!has(mcol_glsl, "__context__") && !has(mcol_glsl, "__lineinfo__"),
            "no __context__ / __lineinfo__ in the emitted GLSL")
    }
}
