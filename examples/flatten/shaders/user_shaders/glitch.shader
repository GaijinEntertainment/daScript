require engine.render.shader_dsl

var {
    @color baseColor = float3(0.2, 0.3, 0.5)
    glitchSpeed = 8.0
    glitchAmplitude = 0.5
    barFreq = 30.0
    splitStrength = 0.3
}

[pixel_shader]
def glitch(inp : PbrInput) {
    let t = g_Time * glitchSpeed
    let bar = sin(inp.worldPos.y * barFreq + t) * 0.5 + 0.5
    let glitchMask = step(0.85, bar)

    let phase = sin(g_Time * 1.7) * 0.5 + 0.5

    let basePhase = inp.worldPos.y * 12.0 + t * 1.3
    let rgb = sin(float3(basePhase) + float3(0.0, 1.0, 2.0)) * 0.5 + float3(0.5)
    let rgbSplit = rgb * splitStrength + baseColor

    let final_col = lerp(baseColor, rgbSplit, glitchMask)

    let daylight = saturate(g_LightDirection.y)
    let strength = lerp(0.8, 2.0, daylight)

    return PbrOutput(
        albedo = final_col,
        emission = rgbSplit * glitchMask * phase,
        emissionStrength = glitchMask * phase * strength,
        metalness = 0.0,
        roughness = 0.6,
        ao = 1.0
    )
}
