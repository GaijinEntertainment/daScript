options gen2
options indenting = 4

require dastest/testing_boost public
require _glsl_common
require glsl/glsl_opengl
require daslib/color_mapping

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

[test]
def test_color_mapping_glsl(t : T?) {
    t |> run("all color mapping helpers compile as desktop GLSL") <| @(t : T?) {
        validate_color_mapping(t, color_mapping_glsl, "#version 330 core")
    }
    t |> run("all color mapping helpers compile as GLSL ES 3.00") <| @(t : T?) {
        validate_color_mapping(t, color_mapping_es300, "#version 300 es")
    }
}
