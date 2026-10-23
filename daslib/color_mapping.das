options gen2
module color_mapping shared public
require math
require math_boost

[arch(at="ARCHITECTURE_SHADER.md#color-output-boundaries")]
def tone_reinhard(value : float3) : float3 {
    //! Compress RGB channels independently with the Reinhard curve;
    //! negative input is clamped to zero.
    let x = max(value, float3(0.0))
    return x / (float3(1.0) + x)
}
// Narkowicz ACES fit, not a complete ACES output transform.
// CC0/MIT source: color_mapping.LICENSE; SDK: COLOR_MAPPING.LICENSE.
[arch(at="ARCHITECTURE_SHADER.md#color-output-boundaries")]
def tone_aces_fit(value : float3) : float3 {
    //! Apply the Narkowicz ACES approximation, clamping negative
    //! input to zero. This is not a full ACES transform.
    let x = max(value, float3(0.0))
    return clamp((x * (2.51 * x + float3(.03))) /
        (x * (2.43 * x + float3(.59)) + float3(.14)), float3(0.0), float3(1.0))
}
// nolint:STYLE014 - retain upstream attribution and license notice.
// Khronos PBR Neutral. Copyright 2024 The Khronos Group Inc.
// Licensed under the Apache License, Version 2.0.
// References: color_mapping.LICENSE; SDK: COLOR_MAPPING.LICENSE.
// Distributed without warranties or conditions.
def tone_pbr_neutral(value : float3) : float3 {
    //! Apply Khronos PBR Neutral to nonnegative RGB, compressing and
    //! desaturating highlights together.
    var color = max(value, float3(0.0))
    let x = min(color.x, min(color.y, color.z))
    let offset = x < .08 ? x - 6.25 * x * x : .04
    color -= float3(offset)
    let peak = max(color.x, max(color.y, color.z))
    if (peak < .76) return color
    let new_peak = 1.0 - .24 * .24 / (peak + .24 - .76)
    color *= new_peak / peak
    let desaturation = 1.0 - 1.0 / (.15 * (peak - new_peak) + 1.0)
    return lerp(color, float3(new_peak), desaturation)
}
def color_srgb_encode_channel(value : float) : float {
    //! Encode one linear channel with the sRGB transfer function,
    //! clamping negative input to zero.
    return linear_to_SRGB(max(value, 0.0))
}
def color_srgb_decode_channel(value : float) : float {
    //! Decode one sRGB channel to linear light, clamping negative
    //! input to zero.
    let x = max(value, 0.0)
    return x <= .04045 ? x / 12.92 : pow((x + .055) / 1.055, 2.4)
}
def color_srgb_encode(value : float3) : float3 {
    //! Encode nonnegative linear RGB with the sRGB transfer function;
    //! values above one remain above one.
    return linear_to_SRGB(max(value, float3(0.0)))
}
def color_srgb_decode(value : float3) : float3 {
    //! Decode nonnegative sRGB channels independently to linear RGB;
    //! values above one remain above one.
    return float3(color_srgb_decode_channel(value.x), color_srgb_decode_channel(value.y),
        color_srgb_decode_channel(value.z))
}
