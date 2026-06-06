require engine.render.shader_dsl

var {
    @color burnColor = float3(2.0, 0.4, 0.0)
    @color ashColor = float3(0.15, 0.1, 0.1)
    burnSpeed = 0.5
    noiseScale = 5.0
}

[shader(stage="pixel", type="pbr")]
def burn_3d(inp : PbrInput) : PbrOutput {
    let t = g_Time * burnSpeed
    let n = noise(float3(inp.worldPos.x * noiseScale, inp.worldPos.y * noiseScale * 0.5 + t, inp.worldPos.z * noiseScale))
    let burnLine = remap(sin(t * 0.8), float2(-1.0, 1.0), float2(0.1, 0.9))
    let h = inp.localPos.y * 0.5 + 0.5
    let burnMask = step(burnLine + n * 0.25, h)
    let edgeMask = smooth_step(burnLine + n * 0.25 - 0.08, burnLine + n * 0.25, h) - burnMask
    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        albedo = ashColor * burnMask,
        emission = burnColor * edgeMask,
        emissionStrength = edgeMask * lerp(1.5, 3.0, daylight),
        alpha = burnMask + edgeMask,
        alphaCutoff = 0.5,
        metalness = 0.0,
        roughness = 0.9,
        ao = 1.0
    )
}
