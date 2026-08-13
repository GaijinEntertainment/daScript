options gen2
options indenting = 4
options no_unused_function_arguments = false

// Baseline emission gate for the GLSL generator's general surface (untested except by use before
// this arc). Pins the version line, the @in/@out/@inout declaration lowering, and the entry-point
// body shape for a minimal vertex+fragment pair. Compile-time only (no GPU). A forward regression
// lock so an emitter change that reshapes the prelude/declarations/body is caught in CI.

require dastest/testing_boost public
require glsl/glsl_opengl
require strings

var @in @location b_pos : float2
var @in @location b_color : float3
var @inout b_vcolor : float3
var @out b_frag_color : float4

[vertex_program(name="basics_vs_glsl")]
def basics_vertex {
    b_vcolor = b_color
    gl_Position = float4(b_pos, 0.0, 1.0)
}

[fragment_program(name="basics_fs_glsl")]
def basics_fragment {
    b_frag_color = float4(b_vcolor, 1.0)
}

// Scalar float literals in non-constructor contexts (init, comparison,
// assignment, division). The `:g` formatter drops the decimal point on
// integer-valued floats (4.0 -> "4"), which GLSL ES 3.00 (WebGL2) rejects as an
// int in these positions; the emitter must re-attach ".0". See glsl_float_literal.
[fragment_program(name="floatlit_fs_glsl")]
def floatlit_fragment {
    var acc = 0.0
    if (b_vcolor.x > 4.0) {
        acc = 2.0
    }
    b_frag_color = float4(acc / 256.0, 0.0, 0.0, 1.0)
}

// `discard()` can't be a bare daslang call to the GLSL keyword, so the emitter renames it
// `_discard()` and emits a `void _discard ( ) { discard; }` wrapper. The wrapper must be
// emitted for fragment shaders in EVERY version (the keyword exists since GLSL 110 / ES 100;
// gating it on version >= 400 left ES 3.00 / 330 shaders with `_discard()` undefined) and
// ONLY for fragment shaders (`discard` is illegal in a vertex stage).
[fragment_program(name="discard_fs_glsl")]
def discard_fragment {
    if (b_vcolor.x > 0.5) {
        discard()
    }
    b_frag_color = float4(1.0, 1.0, 1.0, 1.0)
}

// MRT (deferred G-buffer): GLSL ES 3.00 / WebGL2 REQUIRES an explicit layout(location=N) on every
// fragment output when there is more than one. The emitter's vertex branch already carried the
// @location; the fragment branch did not, so a multi-output fragment shader was rejected in the
// browser. Each @out @location=N fragment output must now emit its location qualifier.
var @out @location = 0 mrt_g0 : float4
var @out @location = 1 mrt_g1 : float4
var @out @location = 2 mrt_g2 : float4

[fragment_program(name="mrt_fs_glsl")]
def mrt_fragment {
    mrt_g0 = float4(b_vcolor, 1.0)
    mrt_g1 = float4(0.0, 1.0, 0.0, 1.0)
    mrt_g2 = float4(1.0, 0.0, 0.0, 1.0)
}

// Nested inlining must never mint a `__` identifier: GLSL ES reserves any
// identifier containing two consecutive underscores and WebGL enforces it,
// while desktop drivers do not — so a bad name compiles on the desktop and dies
// only in the browser. The shape below is the mandelbrot tutorial's: a mutable
// local passed twice into const parameters makes the inliner manufacture
// _inl<N>_arg_* temps, and when that host function is itself inlined, those
// temps are renamed into the outer _inl<N>_l_ namespace — a join that used to
// spell `_inl1_l__inl0_arg_a`.
def private cmul_nested(a, b : float2) : float2 {
    return float2(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x)
}

def private iterate_nested(c : float2) : float2 {
    var z = float2(0.0, 0.0)
    var n = 0
    while (n < 8) {
        z = cmul_nested(z, z) + c
        n++
    }
    return float2(float(n), z.x)
}

def private sample_nested(c : float2) : float3 {
    let nd = iterate_nested(c)
    if (nd.y > 4.0) {
        return float3(nd.x * 0.1, 0.0, 0.0)
    }
    return float3(0.0, 0.0, 0.0)
}

[fragment_program(name="nested_inline_fs_glsl")]
def nested_inline_fragment {
    var col = float3(0.0, 0.0, 0.0)
    for (s in range(16)) {
        col += sample_nested(float2(b_vcolor.x + float(s), b_vcolor.y))
    }
    b_frag_color = float4(col, 1.0)
}

// The other seam: a PARAMETER spelled with a leading underscore reaches the
// manufactured `_arg_` temp name directly (`_inl<N>_arg_` + `_a`), before any
// nesting — and then rides the rename seam too. Both joins must re-encode.
def private mul_underscore(_a, _b : float2) : float2 {
    return float2(_a.x * _b.x - _a.y * _b.y, _a.x * _b.y + _a.y * _b.x)
}

def private iterate_underscore(c : float2) : float2 {
    var z = float2(0.1, 0.1)
    var n = 0
    while (n < 8) {
        z = mul_underscore(z, z) + c
        n++
    }
    return float2(float(n), z.x)
}

def private sample_underscore(c : float2) : float3 {
    let nd = iterate_underscore(c)
    if (nd.y > 4.0) {
        return float3(nd.x * 0.1, 0.0, 0.0)
    }
    return float3(0.0, 0.0, 0.0)
}

[fragment_program(name="nested_underscore_fs_glsl")]
def nested_underscore_fragment {
    var col = float3(0.0, 0.0, 0.0)
    for (s in range(16)) {
        col += sample_underscore(float2(b_vcolor.x + float(s), b_vcolor.y))
    }
    b_frag_color = float4(col, 1.0)
}

// Injectivity at the joins: `_z` vs `u_z` locals and `_a` vs `u_a` parameters
// must stay FOUR distinct GLSL names — an escape that merely strips or
// collapses leading underscores would silently merge them into one variable,
// which is wrong code, worse than a reserved name.
def private mixmul(_a, u_a : float2) : float2 {
    return float2(_a.x * u_a.x - _a.y * u_a.y, _a.x * u_a.y + _a.y * u_a.x)
}

def private iterate_mixed(c : float2) : float2 {
    var _z = float2(0.1, 0.1) // nolint:LINT004 the underscore spelling is the subject under test
    var u_z = c // nolint:LINT003 must stay mutable — a const arg substitutes instead of minting the _arg_ temp
    var n = 0
    while (n < 8) {
        _z = mixmul(_z, u_z) + c
        n++
    }
    return float2(float(n), _z.x + u_z.x)
}

def private sample_mixed(c : float2) : float3 {
    let nd = iterate_mixed(c)
    if (nd.y > 4.0) {
        return float3(nd.x * 0.1, 0.0, 0.0)
    }
    return float3(0.0, 0.0, 0.0)
}

[fragment_program(name="nested_mixed_fs_glsl")]
def nested_mixed_fragment {
    var col = float3(0.0, 0.0, 0.0)
    for (s in range(16)) {
        col += sample_mixed(float2(b_vcolor.x + float(s), b_vcolor.y))
    }
    b_frag_color = float4(col, 1.0)
}

// The emitter's own function-name uniquifier: a function colliding with a
// struct name gets a prefix, which must never spell `__` for a `_`-spelled
// name (it prepends `u` there instead).
struct private _Coll {
    v : float2
}

def private _Coll(a : float2) : float2 {
    return a * 2.0
}

[fragment_program(name="uniquify_fs_glsl")]
def uniquify_fragment {
    let s = _Coll(v = float2(b_vcolor.x, b_vcolor.y))
    b_frag_color = float4(_Coll(s.v), 0.0, 1.0)
}

def private has(hay, needle : string) : bool => find(hay, needle) >= 0

[test]
def test_glsl_emission_basics(t : T?) {
    t |> run("vertex shader emission shape") <| @(t : T?) {
        t |> success(has(basics_vs_glsl, "#version 330 core"), "version line")
        t |> success(has(basics_vs_glsl, "in vec2 b_pos"), "vec2 attribute declaration")
        t |> success(has(basics_vs_glsl, "in vec3 b_color"), "vec3 attribute declaration")
        t |> success(has(basics_vs_glsl, "out vec3 b_vcolor"), "@inout varying lowers to out on the vertex side")
        t |> success(has(basics_vs_glsl, "void main()"), "entry point")
        t |> success(has(basics_vs_glsl, "gl_Position"), "writes gl_Position")
    }
    t |> run("fragment shader emission shape") <| @(t : T?) {
        t |> success(has(basics_fs_glsl, "#version 330 core"), "version line")
        t |> success(has(basics_fs_glsl, "in vec3 b_vcolor"), "@inout varying lowers to in on the fragment side")
        t |> success(has(basics_fs_glsl, "out vec4 b_frag_color"), "fragment output declaration")
        t |> success(has(basics_fs_glsl, "void main()"), "entry point")
    }
    t |> run("integer-valued float literals keep a decimal point (GLSL ES strictness)") <| @(t : T?) {
        t |> success(has(floatlit_fs_glsl, "= 0.0"), "scalar float init keeps the decimal point")
        t |> success(has(floatlit_fs_glsl, "> 4.0"), "comparison literal keeps the decimal point")
        t |> success(has(floatlit_fs_glsl, "= 2.0"), "assignment literal keeps the decimal point")
        t |> success(has(floatlit_fs_glsl, "256.0"), "divisor literal keeps the decimal point")
        // negative control: the bare-int form `acc = 0;` / `> 4)` must not appear
        t |> success(!has(floatlit_fs_glsl, "= 0;"), "no bare-int float initializer")
        t |> success(!has(floatlit_fs_glsl, "> 4)"), "no bare-int comparison literal")
    }
    t |> run("discard() lowers to the GLSL discard keyword (any version, fragment only)") <| @(t : T?) {
        t |> success(has(discard_fs_glsl, "void _discard ( )"), "the _discard wrapper is emitted in the fragment stage")
        t |> success(has(discard_fs_glsl, "discard;"), "the wrapper bridges to the GLSL discard keyword")
        t |> success(has(discard_fs_glsl, "_discard();"), "discard() lowers to a _discard() call")
        // negative control: discard is fragment-only, so the vertex stage must NOT carry the helper
        t |> success(!has(basics_vs_glsl, "void _discard ( )"), "no _discard helper in the vertex stage")
    }
    t |> run("multiple fragment outputs carry explicit layout(location=N) (MRT / WebGL2)") <| @(t : T?) {
        t |> success(has(mrt_fs_glsl, "layout(location=0) out vec4 mrt_g0"), "MRT output 0 carries its location")
        t |> success(has(mrt_fs_glsl, "layout(location=1) out vec4 mrt_g1"), "MRT output 1 carries its location")
        t |> success(has(mrt_fs_glsl, "layout(location=2) out vec4 mrt_g2"), "MRT output 2 carries its location")
        // negative control: a single bare @out (no @location) stays unqualified
        t |> success(!has(basics_fs_glsl, "layout(location"), "single fragment output has no location qualifier")
    }
    t |> run("inline-manufactured temp names never contain the GLSL-reserved __") <| @(t : T?) {
        // positive control first: the shape really did nest — the outer rename
        // namespace and an escaped inner temp are both present in the emission.
        t |> success(has(nested_inline_fs_glsl, "_arg_"), "the inliner manufactured argument temps")
        t |> success(has(nested_inline_fs_glsl, "_lu_"), "a leading-underscore local re-encoded at the join")
        t |> success(!has(nested_inline_fs_glsl, "__"), "no reserved __ anywhere in the emitted GLSL")
        // the _arg_ seam: a leading-underscore PARAMETER re-encodes in the
        // manufactured temp itself (`_argu_a`), and the nested rename stays clean
        t |> success(has(nested_underscore_fs_glsl, "_argu_"), "a leading-underscore parameter re-encoded in its arg temp")
        t |> success(!has(nested_underscore_fs_glsl, "__"), "no reserved __ with underscore-named parameters")
    }
    t |> run("the underscore re-encoding is injective, not just __-free") <| @(t : T?) {
        // `_z`/`u_z` locals and `_a`/`u_a` parameters must emit as four DISTINCT
        // names — collapsing any pair silently merges two variables into one.
        t |> success(has(nested_mixed_fs_glsl, "_lu_z"), "local _z re-encodes with the u mark")
        t |> success(has(nested_mixed_fs_glsl, "_l_u_z"), "local u_z keeps the plain join")
        t |> success(has(nested_mixed_fs_glsl, "_argu_a"), "parameter _a re-encodes in its arg temp")
        t |> success(has(nested_mixed_fs_glsl, "_arg_u_a"), "parameter u_a keeps the plain join")
        t |> success(!has(nested_mixed_fs_glsl, "__"), "and nothing spells the reserved __")
    }
    t |> run("the function-name uniquifier never spells __ on a struct collision") <| @(t : T?) {
        t |> success(has(uniquify_fs_glsl, "u_Coll"), "the colliding _-spelled function got the u prefix")
        t |> success(!has(uniquify_fs_glsl, "__"), "no reserved __ from the uniquifier")
    }
}
