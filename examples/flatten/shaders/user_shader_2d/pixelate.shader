require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    pixelSize = 32.0
}

[shader(stage="pixel", type="pbr")]
def pixelate(inp : PbrInput) : PbrOutput {
    let uv = floor(inp.uv * pixelSize) / pixelSize
    let result = tex2d(albedo_texture, uv).xyz
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
