options gen2

require dastest/testing_boost public
require _glsl_common
require glsl/glsl_opengl
require daslib/color_mapping
require geometry/geom_gen
require math

var @inout color_mapping_input : float3
var @out @location = 0 color_mapping_reinhard : float4
var @out @location = 1 color_mapping_aces : float4
var @out @location = 2 color_mapping_neutral : float4
var @out @location = 3 color_mapping_channels : float4

def private color_mapping_outputs {
    color_mapping_reinhard = float4(color_srgb_encode(tone_reinhard(color_mapping_input)), 1.0)
    color_mapping_aces = float4(tone_aces_fit(color_srgb_decode(color_mapping_input)), 1.0)
    color_mapping_neutral = float4(tone_pbr_neutral(color_mapping_input), 1.0)
    color_mapping_channels = float4(color_srgb_encode_channel(color_mapping_input.x),
        color_srgb_decode_channel(color_mapping_input.y), 0.0, 1.0)
}

[fragment_program(name="color_mapping_glsl")]
def color_mapping_fragment {
    color_mapping_outputs()
}

[fragment_program(name="color_mapping_es300", es300=true)]
def color_mapping_fragment_es300 {
    color_mapping_outputs()
}

def private validate_color_mapping(t : T?; text, version : string) {
    t |> success(has(text, version), "shader declares {version}")
    for (output in ["color_mapping_reinhard", "color_mapping_aces", "color_mapping_neutral",
            "color_mapping_channels"]) {
        t |> success(has(text, "{output} ="), "shader writes {output}")
    }
    let validation = validate_glsl(text, "frag")
    if (validation.ran) {
        t |> success(validation.ok, "glslangValidator: {validation.msg}")
    } else {
        feint("glslangValidator not found locally; skipping external validation\n")
    }
}

[test(description = "The color mapping helpers compile as desktop GLSL and as GLSL ES 3.00.")]
def test_color_mapping_glsl(t : T?) {
    t |> run("all color mapping helpers compile as desktop GLSL") <| @(t : T?) {
        validate_color_mapping(t, color_mapping_glsl, "#version 330 core")
    }
    t |> run("all color mapping helpers compile as GLSL ES 3.00") <| @(t : T?) {
        validate_color_mapping(t, color_mapping_es300, "#version 300 es")
    }
}

// Regression gate for triangle winding in the geometry generators.
//
// Every generator sets an outward per-vertex normal, and callers draw with
// GL_CULL_FACE / GL_BACK under the default CCW front face. So each triangle must be
// wound counter-clockwise seen from outside -- its edge cross product has to point the
// same way as its own vertex normals. A triangle that disagrees is drawn inside out:
// the lit outer surface is culled and the unlit interior shows through.
//
// Nothing checked this, and two separate defects had accumulated. gen_cylinder and
// gen_cone pushed indices in the wrong order. Independently, GenDirection.xz maps the
// generated xy shape onto xz by swapping two axes -- a mirror, determinant negative --
// which reverses the orientation of every triangle it moves, so no single index order
// could be right for all three directions at once.
//
// The audit is geometric, not a golden index list: it recomputes the winding from the
// vertices, so it keeps holding if a generator changes its tessellation.
// Triangles whose winding disagrees with their own vertex normals.
def private inside_out_count(frag : GeometryFragment) : int {
    var bad = 0
    for (t in range(length(frag.indices) / 3)) {
        let a = frag.vertices[frag.indices[t * 3]]
        let b = frag.vertices[frag.indices[t * 3 + 1]]
        let c = frag.vertices[frag.indices[t * 3 + 2]]
        let geo = cross(b.xyz - a.xyz, c.xyz - a.xyz)
        let outward = a.normal + b.normal + c.normal
        // A degenerate triangle or a cancelling normal sum carries no orientation.
        continue if (length(geo) < 0.000001 || length(outward) < 0.000001)
        bad++ if (dot(normalize(geo), normalize(outward)) <= 0.0)
    }
    return bad
}

[test(description = "Every generated geometry primitive is wound counter-clockwise when seen from outside.")]
def test_geom_gen_winding(t : T?) {
    for (d in [GenDirection.xy, GenDirection.xz, GenDirection.yz]) {
        t |> run("gen_cylinder {d} is wound counter-clockwise from outside") <| @(t : T?) {
            let frag <- gen_cylinder(d, 12)
            t |> success(inside_out_count(frag) == 0, "no inside-out triangles")
        }
        t |> run("gen_cone {d} is wound counter-clockwise from outside") <| @(t : T?) {
            let frag <- gen_cone(d, 12)
            t |> success(inside_out_count(frag) == 0, "no inside-out triangles")
        }
        t |> run("gen_plane {d} is wound counter-clockwise from outside") <| @(t : T?) {
            let frag <- gen_plane(d)
            t |> success(inside_out_count(frag) == 0, "no inside-out triangles")
        }
        t |> run("gen_prism {d} is wound counter-clockwise from outside") <| @(t : T?) {
            let frag <- gen_prism(d)
            t |> success(inside_out_count(frag) == 0, "no inside-out triangles")
        }
    }
    t |> run("gen_sphere is wound counter-clockwise from outside") <| @(t : T?) {
        let frag <- gen_sphere(16, 8, false)
        t |> success(inside_out_count(frag) == 0, "no inside-out triangles")
    }
    t |> run("gen_cube is wound counter-clockwise from outside") <| @(t : T?) {
        let frag <- gen_cube()
        t |> success(inside_out_count(frag) == 0, "no inside-out triangles")
    }
}

// packHalf2x16/unpackHalf2x16 bridge (shader_lingua_franca): CPU bodies are exact bit packs
// (das half2 is layout-identical to the packed uint), the GLSL rail passes the names through
// and adds GL_ARB_shading_language_packing on demand below 4.20. This is the ONE fp16 story
// the ES/WebGL2 rail keeps (core in ES 3.00 — no extension line there).
var @inout ph_uv : float2
var @out ph_color : float4

[fragment_program(name="pack_half_glsl")]
def pack_half_fragment {
    let bits = packHalf2x16(ph_uv)
    let back = unpackHalf2x16(bits)
    ph_color = float4(back.x, back.y, 0., 1.)
}

[test(description = "The CPU pack and unpack of half floats yield exact bits and round-trip.")]
def test_pack_half_cpu(t : T?) {
    t |> run("CPU pack/unpack: exact bits and round trip") <| @(t : T?) {
        t |> equal(packHalf2x16(float2(1.5, -2.25)), 0xc0803e00)
        t |> equal(packHalf2x16(float2(0., 0.)), 0x0)
        let rt = unpackHalf2x16(packHalf2x16(float2(0.125, 65504.0)))
        t |> success(rt == float2(0.125, 65504.0), "f16-exact values round trip")
    }
}

[test(description = "Half pack and unpack pass through by name and pull in the ARB packing extension only on demand.")]
def test_pack_half_glsl_emission(t : T?) {
    t |> run("GLSL: pass-through names + demand-driven ARB packing extension") <| @(t : T?) {
        t |> success(has(pack_half_glsl, "#extension GL_ARB_shading_language_packing : require"), "extension line")
        t |> success(has(pack_half_glsl, "packHalf2x16("), "pack call")
        t |> success(has(pack_half_glsl, "unpackHalf2x16("), "unpack call")
        let r = validate_glsl(pack_half_glsl, "frag")
        if (r.ran) {
            t |> success(r.ok, "glslang: {r.msg}")
        } else {
            feint("glslangValidator not found locally; skipping (CI enforces)\n")
        }
    }
}
