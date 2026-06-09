require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    @color outlineColor = float3(0.3, 1.0, 0.5)
    thickness = 0.003
}

[pixel_shader]
def outline_2d(inp : PbrInput) {
    let c = tex2d(albedo_texture, inp.uv).xyz
    let r = tex2d(albedo_texture, inp.uv + float2(thickness, 0.0)).x
    let l = tex2d(albedo_texture, inp.uv - float2(thickness, 0.0)).x
    let u = tex2d(albedo_texture, inp.uv + float2(0.0, thickness)).x
    let d = tex2d(albedo_texture, inp.uv - float2(0.0, thickness)).x
    let gray = dot(c, float3(0.299, 0.587, 0.114))
    let edge = max(abs(r - l), abs(u - d))
    let outline = step(0.05, edge) * step(gray, 0.8)
    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        albedo = float3(0),
        emission = outlineColor * outline,
        emissionStrength = outline * lerp(1.0, 3.0, daylight),
        alpha = outline,
        alphaCutoff = 0.5,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
