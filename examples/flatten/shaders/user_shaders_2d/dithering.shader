require engine.render.shader_dsl

// Ordered (Bayer) dithering — quantizes the image to `levels` shades using
// a 4x4 Bayer threshold matrix encoded as a literal array.
// The `?:` loop accumulator selects the threshold for the current pixel cell.

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    levels         = 4.0
    pixelSize      = 3.0
    @color tint    = float3(1.0, 1.0, 1.0)
}

def bayer4(px : float2) : float {
    // 4x4 Bayer matrix row-major, values / 16
    let xi  = floor(frac(px.x * 0.25) * 4.0)
    let yi  = floor(frac(px.y * 0.25) * 4.0)
    let row = yi * 4.0 + xi

    var threshold = 0.0
    for (i, v in range(16),
                 [0.0,  8.0,  2.0, 10.0,
                  12.0, 4.0, 14.0,  6.0,
                  3.0, 11.0,  1.0,  9.0,
                  15.0, 7.0, 13.0,  5.0]) {
        threshold = float(i) == row ? v * (1.0 / 16.0) : threshold
    }
    return threshold
}

[pixel_shader]
def dithering(inp : PbrInput) {
    let pixelUV   = floor(inp.uv * pixelSize * 128.0) / (pixelSize * 128.0)
    let c         = tex2d(albedo_texture, pixelUV).xyz * tint
    let gray      = dot(c, float3(0.299, 0.587, 0.114))
    let thresh    = bayer4(inp.uv * pixelSize * 128.0)
    let quantized = floor(gray * levels + thresh) / levels
    let result    = c * (quantized / max(gray, 0.001))

    return PbrOutput(
        albedo           = result,
        emission         = result * quantized * 0.3,
        emissionStrength = 0.4,
        metalness        = 0.0,
        roughness        = 1.0,
        ao               = 1.0
    )
}
