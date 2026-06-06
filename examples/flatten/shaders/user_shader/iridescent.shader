require engine.render.shader_dsl

var {
    @color baseTint = float3(0.05, 0.05, 0.1)
    bandFreq = 8.0
    bandPhaseR = 0.0
    bandPhaseG = 2.1
    bandPhaseB = 4.2
    timeShift = 0.5
}

[shader(stage="pixel", type="pbr")]
def iridescent(inp : PbrInput) : PbrOutput {
    let f = fresnel(1.0, inp.worldNormal, inp.viewDir)

    let phase = f * bandFreq + g_Time * timeShift
    let phaseVec = float3(phase + bandPhaseR, phase + bandPhaseG, phase + bandPhaseB)

    let film = sin(phaseVec) * 0.5 + float3(0.5)
    let intensity = pow(f, 1.5)

    return PbrOutput(
        albedo = baseTint,
        emission = film * intensity,
        emissionStrength = intensity * 0.6,
        metalness = 1.0,
        roughness = 0.15,
        ao = 1.0
    )
}
