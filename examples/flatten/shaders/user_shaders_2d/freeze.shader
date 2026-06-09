require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    @color iceColor = float3(0.5, 0.85, 1.0)
    @color frostColor = float3(0.9, 0.97, 1.0)
    freezeAmount = 0.6
    noiseScale = 6.0
}

[pixel_shader]
def freeze(inp : PbrInput) {
    let c = tex2d(albedo_texture, inp.uv).xyz
    let n = noise(inp.uv * noiseScale)
    let iceMask = smooth_step(1.0 - freezeAmount - 0.1, 1.0 - freezeAmount + 0.1, n)
    let crystal = step(0.75, n) * iceMask
    let result = lerp(c, iceColor, iceMask * 0.7) + frostColor * crystal * 0.5
    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        albedo = result,
        emission = frostColor * crystal * 0.5 + iceColor * iceMask * 0.2,
        emissionStrength = lerp(0.3, 1.5, daylight),
        metalness = 0.0,
        roughness = 0.1,
        ao = 1.0
    )
}
