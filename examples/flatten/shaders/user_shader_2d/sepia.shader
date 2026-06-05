require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    strength = 1.0
}

[shader(stage="pixel", type="pbr")]
def sepia(inp : PbrInput) : PbrOutput {
    let c = tex2d(albedo_texture, inp.uv).xyz
    let gray = dot(c, float3(0.299, 0.587, 0.114))
    let result = lerp(c, float3(gray * 1.08, gray * 0.86, gray * 0.66), strength)
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
