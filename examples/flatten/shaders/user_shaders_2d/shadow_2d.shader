require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    @color shadowColor = float3(0.05, 0.05, 0.1)
    offsetX = 0.03
    offsetY = 0.03
    shadowStrength = 0.7
}

[pixel_shader]
def shadow_2d(inp : PbrInput) {
    let c = tex2d(albedo_texture, inp.uv).xyz
    let cs = tex2d(albedo_texture, inp.uv - float2(offsetX, offsetY)).xyz
    let shadowGray = dot(cs, float3(0.299, 0.587, 0.114))
    let isShadowArea = smooth_step(0.3, 0.7, shadowGray) * shadowStrength
    let shifted = dot(c, float3(0.299, 0.587, 0.114))
    let isOriginal = step(0.1, shifted)
    let result = lerp(lerp(c, shadowColor, isShadowArea * one_minus(isOriginal)), c, isOriginal)
    return PbrOutput(
        albedo = result,
        emission = result,
        emissionStrength = 0.15,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
