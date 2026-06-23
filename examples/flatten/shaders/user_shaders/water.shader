require engine.render.shader_dsl

var {
    @color deepColor      = float3(0.0, 0.15, 0.35)
    @color shallowColor   = float3(0.2, 0.6, 0.8)
    @color foamColor      = float3(1.5, 1.6, 1.8)
    waveSpeed             = 0.6
    waveScale             = 5.0
    foamThreshold         = 0.55
    normalStrength        = 0.6
}

def wave_octave(p : float3; speed : float; scale : float; t : float) : float {
    return noise(p * scale + float3(t * speed, t * speed * 0.3, t * speed * 0.7))
}

def water_normal(p : float3; t : float; scale : float) : float3 {
    let eps = 0.05
    let c  = wave_octave(p, 0.6, scale, t)
    let dx = wave_octave(p + float3(eps, 0.0, 0.0), 0.6, scale, t) - c
    let dz = wave_octave(p + float3(0.0, 0.0, eps), 0.6, scale, t) - c
    return normalize(float3(-dx * normalStrength, 1.0, -dz * normalStrength))
}

[pixel_shader]
def water(inp : PbrInput) {
    let t = g_Time * waveSpeed

    let n1 = wave_octave(inp.worldPos, 1.0,  waveScale,       t)
    let n2 = wave_octave(inp.worldPos, 1.6,  waveScale * 2.1, t)
    let n3 = wave_octave(inp.worldPos, 0.7,  waveScale * 0.5, t * 0.4)
    let n = n1 * 0.5 + n2 * 0.3 + n3 * 0.2

    let depthMix = lerp(deepColor, shallowColor, n)
    let foamMask = smooth_step(foamThreshold, foamThreshold + 0.15, n)
    let surface  = lerp(depthMix, foamColor, foamMask)

    let nrm = water_normal(inp.worldPos, t, waveScale)

    return PbrOutput(
        albedo = surface,
        emission = foamColor * foamMask,
        emissionStrength = foamMask * 0.8,
        normalMap = nrm,
        metalness = 0.0,
        roughness = lerp(0.15, 0.4, foamMask),
        ao = 1.0
    )
}
