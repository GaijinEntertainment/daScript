require engine.render.shader_dsl

var {
    @color deepColor = float3(0.0, 0.15, 0.4)
    @color shallowColor = float3(0.2, 0.6, 0.9)
    @color foamColor = float3(0.7, 0.9, 1.0)
    waveScale = 4.0
    waveSpeed = 0.5
    foamThreshold = 0.6
}

[shader(stage="pixel", type="pbr")]
def water_2d(inp : PbrInput) : PbrOutput {
    let t = g_Time * waveSpeed
    let n1 = noise(inp.uv * waveScale + float2(t, t * 0.7))
    let n2 = noise(inp.uv * waveScale * 2.1 + float2(t * 1.6, 0.0))
    let n = n1 * 0.55 + n2 * 0.45
    let depthMix = lerp(deepColor, shallowColor, n)
    let foamMask = smooth_step(foamThreshold, foamThreshold + 0.15, n)
    let result = lerp(depthMix, foamColor, foamMask)
    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        albedo = result,
        emission = result + foamColor * foamMask * 0.3,
        emissionStrength = lerp(0.3, 1.5, daylight),
        metalness = 0.0,
        roughness = 0.2,
        ao = 1.0
    )
}
