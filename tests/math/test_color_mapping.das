options gen2
require math
require dastest/testing_boost
require daslib/color_mapping

//! Allow Win32 pow estimates for these regression inputs only.
let private estimated_pow = (get_platform_name() == "windows" &&
    get_architecture_name() == "x86")
let private roundtrip_epsilon = estimated_pow ? .000025 : .000002
let private hdr_decode_epsilon = estimated_pow ? .0001 : .000003
[test]
def color_mapping_preserves_black_and_highlight_order(t : T?){
    t |> equal(tone_reinhard(float3(0.0)), float3(0.0))
    t |> equal(tone_aces_fit(float3(0.0)), float3(0.0))
    t |> equal(tone_pbr_neutral(float3(0.0)), float3(0.0))
    for (op in range(3)){
        var previous = -1.0
        for (v in [0.0, .001, .01, .18, .5, 1.0, 2.0, 4.0, 16.0]){
            let c = (op == 0 ? tone_reinhard(float3(v)) :
                op == 1 ? tone_aces_fit(float3(v)) : tone_pbr_neutral(float3(v)))
            t |> success(c.x >= previous && c.x >= 0.0 && c.x <= 1.0)
            t |> equal(c.x, c.y); t |> equal(c.y, c.z); previous = c.x
        }
    }
    t |> equal(tone_reinhard(float3(1.0)), float3(.5))
    t |> success(abs(tone_aces_fit(float3(1.0)).x - .80379747) < .000001)
    t |> success(tone_pbr_neutral(float3(4.0)).x > tone_pbr_neutral(float3(2.0)).x)
}
[test]
def color_mapping_srgb_roundtrip(t : T?){
    for (v in [0.0, .001, .0031308, .018, .18, .5, 1.0]){
        let decoded = color_srgb_decode(color_srgb_encode(float3(v))).x
        t |> success(abs(decoded - v) < roundtrip_epsilon,
            "sRGB round trip {v} produced {decoded}")
    }
    t |> success(abs(color_srgb_encode(float3(.0031308)).x - .040449936) < .000001)
}

[test]
def color_mapping_preserves_independent_rgb_channels_and_clamps_negatives(t : T?) {
    t |> equal(tone_reinhard(float3(-1.0)), float3(0.0))
    t |> equal(tone_aces_fit(float3(-1.0)), float3(0.0))
    t |> equal(tone_pbr_neutral(float3(-1.0)), float3(0.0))
    t |> success(length(tone_reinhard(float3(-1.0, 1.0, 4.0)) -
        float3(0.0, .5, .8)) < .000001)
    t |> success(length(color_srgb_encode(float3(-1.0, .18, 2.0)) -
        float3(0.0, .46135613, 1.35325605)) < .000002)
    let decoded = color_srgb_decode(float3(-1.0, .5, 2.0))
    t |> success(length(decoded - float3(0.0, .21404114, 4.95384575)) < hdr_decode_epsilon,
        "sRGB HDR decode produced {decoded}")
    t |> equal(color_srgb_encode_channel(-1.0), 0.0)
    t |> equal(color_srgb_decode_channel(-1.0), 0.0)
    t |> success(abs(color_srgb_decode_channel(.01) - .0007739938) < .0000001)
}
[test]
def pbr_neutral_preserves_color_below_shoulder_and_desaturates_highlights(t : T?) {
    t |> success(length(tone_pbr_neutral(float3(.04, .05, .08)) -
        float3(.01, .02, .05)) < .000001)
    t |> success(length(tone_pbr_neutral(float3(.2, .4, .6)) -
        float3(.16, .36, .56)) < .000001)
    t |> success(length(tone_pbr_neutral(float3(1.0, 2.0, 4.0)) -
        float3(0.468299174, 0.639951387, 0.983255814)) < .000002)
}
