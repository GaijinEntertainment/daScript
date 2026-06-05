require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    blurSize = 0.015
}

[shader(stage="pixel", type="pbr")]
def blur(inp : PbrInput) : PbrOutput {
    let s = blurSize
    let a = tex2d(albedo_texture, inp.uv + float2(-s, -s)).xyz
    let b = tex2d(albedo_texture, inp.uv).xyz
    let c = tex2d(albedo_texture, inp.uv + float2(s, s)).xyz
    let result = a * 0.25 + b * 0.5 + c * 0.25
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
