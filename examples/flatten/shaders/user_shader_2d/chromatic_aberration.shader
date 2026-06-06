require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    aberration = 0.025
    pulse = 0.5
}

[shader(stage="pixel", type="pbr")]
def chromatic_aberration(inp : PbrInput) : PbrOutput {
    let t = 1.0 + sin(g_Time * 2.0) * pulse
    let offset = aberration * t
    let r = tex2d(albedo_texture, inp.uv + float2(offset, 0)).x
    let g = tex2d(albedo_texture, inp.uv).y
    let b = tex2d(albedo_texture, inp.uv - float2(offset, 0)).z
    let result = float3(r, g, b)
    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        albedo = result,
        emission = result,
        emissionStrength = lerp(0.5, 2.0, daylight),
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
