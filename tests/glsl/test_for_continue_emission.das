options gen2
options indenting = 4

// Regression gate for `continue` inside a lowered for-loop.
//
// The emitter used to lower a range- or dim-for to `while (cond) { body; i++ }`, with the
// increment as the LAST STATEMENT OF THE BODY. A `continue` in that body jumps over the
// increment, so the loop variable never advances and the shader spins forever -- a GPU hang
// with no diagnostic in any tier, because the das source is correct and the GLSL compiles.
//
// The loops now emit a real `for ( ; cond; i++ )` header, so `continue` reaches the
// increment. The asserts pin the property, not the spelling: the increment appears inside a
// for-header, and no `while` survives. Renaming the emitter's temporaries keeps them green.

require dastest/testing_boost public
require glsl/glsl_opengl
require math
require strings

var @in @location a_uv : float2
var @out o_color : float4

[fragment_program(name="loop_continue_glsl")]
def loop_continue_shader {
    var acc = 0.0
    for (i in range(4)) {
        if (i == 2) {
            continue
        }
        acc += float(i) * a_uv.x
    }
    var d : float[3]
    d[0] = 1.0
    d[1] = 2.0
    d[2] = 3.0
    for (v in d) {
        if (v > 2.5) {
            continue
        }
        acc += v
    }
    o_color = float4(acc)
}

def private has(hay, needle : string) : bool => find(hay, needle) >= 0

[test]
def test_for_continue_emission(t : T?) {
    t |> run("the shader under test really does emit a continue") <| @(t : T?) {
        t |> success(has(loop_continue_glsl, "continue;"), "continue; present in the emitted GLSL")
    }
    t |> run("a range for-loop carries its increment in the for header") <| @(t : T?) {
        t |> success(has(loop_continue_glsl, "; i++ )"), "for ( ; ... ; i++ ) header emitted")
    }
    t |> run("a dim for-loop carries its increment in the for header") <| @(t : T?) {
        t |> success(has(loop_continue_glsl, "; v++ )"), "for ( ; ... ; v++ ) header emitted")
    }
    t |> run("no loop is lowered to a while with a body increment") <| @(t : T?) {
        t |> success(!has(loop_continue_glsl, "while ("), "no while ( in the emitted GLSL")
    }
}
