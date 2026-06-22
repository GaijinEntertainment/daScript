require engine.render.shader_dsl

var {
    @color baseTint   = float3(0.05, 0.05, 0.1)
    bandFreq          = 8.0
    bandPhaseR        = 0.0
    bandPhaseG        = 2.1
    bandPhaseB        = 4.2
    timeShift         = 0.5
    microDetailScale  = 12.0
    microDetailAmount = 0.3
}

def film_band(f : float; freq : float; phaseR : float; phaseG : float; phaseB : float; t : float) : float3 {
    let phase = f * freq + t
    return sin(float3(phase + phaseR, phase + phaseG, phase + phaseB)) * 0.5 + float3(0.5)
}

[pixel_shader]
def iridescent(inp : PbrInput) {
    let f = fresnel(1.0, inp.worldNormal, inp.viewDir)

    let macroFilm = film_band(f, bandFreq,        bandPhaseR, bandPhaseG, bandPhaseB, g_Time * timeShift)
    let microFilm = film_band(f, bandFreq * 3.1,  bandPhaseB, bandPhaseR, bandPhaseG, g_Time * timeShift * 0.7)

    // Surface micro-variation via normal noise
    let surfaceVar = noise(inp.worldNormal * microDetailScale) * microDetailAmount
    let film = lerp(macroFilm, microFilm, surfaceVar)

    let intensity = pow(f, 1.5)
    let viewAngle = saturate(dot(inp.worldNormal, inp.viewDir))
    let grazeBoost = pow(1.0 - viewAngle, 3.0) * 0.4

    return PbrOutput(
        albedo = baseTint,
        emission = film * (intensity + grazeBoost),
        emissionStrength = (intensity + grazeBoost) * 0.6,
        metalness = 1.0,
        roughness = lerp(0.05, 0.25, surfaceVar),
        ao = 1.0
    )
}
