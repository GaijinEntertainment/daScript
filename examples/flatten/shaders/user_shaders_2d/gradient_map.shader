require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    @color shadowColor = float3(0.1, 0.05, 0.3)
    @color midColor = float3(0.8, 0.2, 0.4)
    @color highColor = float3(1.0, 0.9, 0.5)
}

[pixel_shader]
def gradient_map(inp : PbrInput) {
    let c = tex2d(albedo_texture, inp.uv)
    let luma = dot(c.xyz, float3(0.299, 0.587, 0.114))
    // remap luminance through a 3-stop ramp: shadow -> mid -> high (duotone / heat grading)
    let lowMix = lerp(shadowColor, midColor, saturate(luma * 2.0))
    let highMix = lerp(midColor, highColor, saturate(luma * 2.0 - 1.0))
    let result = lerp(lowMix, highMix, step(0.5, luma))
    return PbrOutput(
        albedo = result,
        alpha = c.w,
        alphaCutoff = 0.5,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
