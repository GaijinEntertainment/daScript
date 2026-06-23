require engine.render.shader_dsl

var {
    @color cellColor  = float3(0.1, 0.4, 0.2)
    @color edgeColor  = float3(0.9, 1.0, 0.6)
    @color deepColor  = float3(0.0, 0.05, 0.0)
    @color innerColor = float3(0.05, 0.25, 0.08)
    cellScale         = 4.0
    pulseSpeed        = 0.5
}

def cell_noise(p : float3; t : float; scale : float; drift : float3) : float {
    return noise(p * scale + drift * t)
}

[pixel_shader]
def cells(inp : PbrInput) {
    let t = g_Time * pulseSpeed

    let n0 = cell_noise(inp.worldPos, t, cellScale,       float3(1.0, 0.0, 0.5))
    let n1 = cell_noise(inp.worldPos, t, cellScale * 2.3, float3(-0.7, 0.0, 0.3))
    let n2 = cell_noise(inp.worldPos, t, cellScale * 0.5, float3(0.2, 0.0, 0.8))

    let n = n0 * 0.55 + n1 * 0.25 + n2 * 0.2

    let edgeMask  = smooth_step(0.45, 0.55, n) - smooth_step(0.55, 0.65, n)
    let depthMask = smooth_step(0.6, 0.9, n)
    let innerMask = smooth_step(0.35, 0.5, n) * (1.0 - depthMask)

    let surface = lerp(lerp(cellColor, innerColor, innerMask), deepColor, depthMask)

    let pulse = sin(t * 4.0) * 0.15 + 0.85

    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        albedo = surface,
        emission = edgeColor * edgeMask * pulse,
        emissionStrength = edgeMask * pulse * lerp(0.1, 0.6, daylight),
        metalness = 0.0,
        roughness = lerp(0.4, 0.8, depthMask),
        ao = 1.0
    )
}
