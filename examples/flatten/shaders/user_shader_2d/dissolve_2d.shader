require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    noiseScale = 6.0
    edgeWidth = 0.06
    @color edgeColor = float3(2.0, 0.5, 0.1)
}

[shader(stage="pixel", type="pbr")]
def dissolve_2d(inp : PbrInput) : PbrOutput {
    let c = tex2d(albedo_texture, inp.uv).xyz
    let amount = remap(sin(g_Time * 0.8), float2(-1.0, 1.0), float2(0.1, 0.9))
    let n = noise(inp.uv * noiseScale)
    let cutoff = step(amount, n)
    let edge = smooth_step(amount, amount + edgeWidth, n) - cutoff
    let result = c * cutoff + edgeColor * edge
    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        albedo = result,
        emission = edgeColor * edge,
        emissionStrength = lerp(0.5, 2.0, daylight),
        alpha = cutoff + edge,
        alphaCutoff = 0.5,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
