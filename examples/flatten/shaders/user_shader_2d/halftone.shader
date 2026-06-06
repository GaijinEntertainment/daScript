require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    dotSize = 22.0
}

[shader(stage="pixel", type="pbr")]
def halftone(inp : PbrInput) : PbrOutput {
    let c = tex2d(albedo_texture, inp.uv).xyz
    let gray = dot(c, float3(0.299, 0.587, 0.114))
    let cell = frac(inp.uv * dotSize) - float2(0.5, 0.5)
    let dist = length(cell)
    let radius = gray * 0.5
    let mask = step(dist, radius)
    let result = c * mask
    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        albedo = result,
        emission = result,
        emissionStrength = lerp(0.5, 2.5, daylight),
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
