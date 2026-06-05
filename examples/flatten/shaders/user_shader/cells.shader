require engine.render.shader_dsl

var {
    @color cellColor = float3(0.1, 0.4, 0.2)
    @color edgeColor = float3(0.9, 1.0, 0.6)
    @color deepColor = float3(0.0, 0.05, 0.0)
    cellScale = 4.0
    pulseSpeed = 0.5
}

[shader(stage="pixel", type="pbr")]
def cells(inp : PbrInput) : PbrOutput {
    let pulseT = g_Time * pulseSpeed
    let uv = inp.worldPos * cellScale + float3(pulseT, 0, pulseT * 0.5)
    let n = noise(uv)

    let edgeMask = smooth_step(0.45, 0.55, n) - smooth_step(0.55, 0.65, n)
    let depthMask = smooth_step(0.6, 0.9, n)

    let surface = lerp(cellColor, deepColor, depthMask)

    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        albedo = surface,
        emission = edgeColor * edgeMask,
        emissionStrength = edgeMask * lerp(0.1, 0.6, daylight),
        metalness = 0.0,
        roughness = 0.7,
        ao = 1.0
    )
}
