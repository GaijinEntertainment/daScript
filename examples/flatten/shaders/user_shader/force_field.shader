require engine.render.shader_dsl

var {
    @color fieldColor = float3(0.2, 0.9, 1.0)
    @color pulseColor = float3(1.5, 2.0, 2.5)
    gridScale = 8.0
    gridLineWidth = 0.05
    pulseSpeed = 1.5
    fresnelPower = 2.0
}

[shader(stage="pixel", type="pbr")]
def force_field(inp : PbrInput) : PbrOutput {
    let scrollY = g_Time * pulseSpeed * 0.3
    let gridUV = inp.worldPos * gridScale + float3(0, scrollY, 0)

    let g = abs(frac(gridUV) - float3(0.5))
    let gridMin = min(min(g.x, g.y), g.z)
    let gridMask = smooth_step(0.5 - gridLineWidth, 0.5, gridMin)

    let pulse = sin(g_Time * pulseSpeed - inp.worldPos.y * 2.0) * 0.5 + 0.5

    let f = fresnel(fresnelPower, inp.worldNormal, inp.viewDir)
    let rim = pow(f, 0.8)

    let baseGlow = fieldColor * (rim * 0.4 + gridMask * 0.6)
    let pulseGlow = pulseColor * gridMask * pulse * 0.5

    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        albedo = float3(0),
        emission = baseGlow + pulseGlow,
        emissionStrength = (rim + gridMask) * lerp(0.4, 1.5, daylight),
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
