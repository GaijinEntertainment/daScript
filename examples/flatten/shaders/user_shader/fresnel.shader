require engine.render.shader_dsl

var {
    @color fresnelColor = float3(0.0, 0.5, 1.0)
}

[shader(stage="pixel", type="pbr")]
def fresnel(inp : PbrInput) : PbrOutput {
    let rim = fresnel(3., inp.worldNormal, inp.viewDir) * fresnelColor
    let scale = remap(sin(g_Time * 4.0), float2(-1, 1), float2(0, 1))
    let color = rim * scale
    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        emission = color.xyz,
        emissionStrength = length(color.xyz) * lerp(0.2, 10, daylight),
        metalness = 0.,
        roughness = 0.
    )
}
