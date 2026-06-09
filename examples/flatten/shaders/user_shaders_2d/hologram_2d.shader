require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    @color holoColor = float3(0.3, 0.9, 1.0)
    scanFreq = 80.0
    flickerSpeed = 6.0
}

[pixel_shader]
def hologram_2d(inp : PbrInput) {
    let c = tex2d(albedo_texture, inp.uv).xyz
    let gray = dot(c, float3(0.299, 0.587, 0.114))
    let scan = sin(inp.uv.y * scanFreq + g_Time * 2.0) * 0.5 + 0.5
    let flicker = remap(sin(g_Time * flickerSpeed), float2(-1.0, 1.0), float2(0.85, 1.0))
    let result = holoColor * gray * scan * flicker
    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        albedo = float3(0),
        emission = result,
        emissionStrength = lerp(0.3, 1.2, daylight),
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
