require engine.render.shader_dsl

var {
    @color coolColor = float3(0.05, 0.0, 0.0)
    @color hotColor = float3(1.0, 0.4, 0.05)
    @color glowColor = float3(2.5, 1.2, 0.2)
    flowSpeed = 0.15
    noiseScale = 2.0
}

[pixel_shader]
def lava(inp : PbrInput) {
    let flowOffset = g_Time * flowSpeed
    let uv = inp.worldPos * noiseScale + float3(0, flowOffset, 0)
    let n = noise(uv)

    let crackMask = smooth_step(0.55, 0.85, n)
    let warmMask = smooth_step(0.3, 0.7, n)

    let surface = lerp(coolColor, hotColor, warmMask)

    return PbrOutput(
        albedo = surface,
        emission = glowColor * crackMask,
        emissionStrength = crackMask * 1.5,
        metalness = 0.0,
        roughness = 0.85,
        ao = 1.0
    )
}
