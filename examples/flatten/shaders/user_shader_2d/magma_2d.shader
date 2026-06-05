require engine.render.shader_dsl

var {
    @color coolColor = float3(0.1, 0.0, 0.0)
    @color hotColor = float3(1.0, 0.3, 0.0)
    @color glowColor = float3(2.5, 1.2, 0.1)
    flowSpeed = 0.2
    noiseScale = 3.0
}

[shader(stage="pixel", type="pbr")]
def magma_2d(inp : PbrInput) : PbrOutput {
    let t = g_Time * flowSpeed
    let uv = inp.uv * noiseScale + float2(t, t * 0.7)
    let n = noise(uv)
    let crackMask = smooth_step(0.55, 0.85, n)
    let warmMask = smooth_step(0.3, 0.7, n)
    let surface = lerp(coolColor, hotColor, warmMask)
    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        albedo = surface,
        emission = glowColor * crackMask,
        emissionStrength = crackMask * lerp(0.5, 2.0, daylight),
        metalness = 0.0,
        roughness = 0.9,
        ao = 1.0
    )
}
