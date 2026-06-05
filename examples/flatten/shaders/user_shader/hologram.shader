require engine.render.shader_dsl

var {
    @color hologramColor = float3(0.2, 0.8, 1.0)
    scanlineDensity = 60.0
    scanlineSpeed = 2.0
    flickerSpeed = 8.0
    fresnelPower = 2.0
}

[shader(stage="pixel", type="pbr")]
def hologram(inp : PbrInput) : PbrOutput {
    let scan = frac(inp.worldPos.y * scanlineDensity - g_Time * scanlineSpeed)
    let scanlines = smooth_step(0.3, 0.7, scan)

    let flicker = remap(sin(g_Time * flickerSpeed), float2(-1, 1), float2(0.9, 1))

    let rim = fresnel(fresnelPower, inp.worldNormal, inp.viewDir)

    let bandIntensity = scanlines * flicker * 1.5
    let rimGlow = pow(rim, 2.5) * 0.5

    let color = hologramColor * (bandIntensity + rimGlow)

    let daylight = saturate(g_LightDirection.y)
    let strength = lerp(0.08, 1.5, daylight)

    return PbrOutput(
        emission = color,
        emissionStrength = strength,
        albedo = float3(0),
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
