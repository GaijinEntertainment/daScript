require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    step_size = 0.003
    amount = 1.0
}

[pixel_shader]
def sharpen_2d(inp : PbrInput) {
    let c = tex2d(albedo_texture, inp.uv)
    let l = tex2d(albedo_texture, inp.uv - float2(step_size, 0.0)).xyz
    let r = tex2d(albedo_texture, inp.uv + float2(step_size, 0.0)).xyz
    let u = tex2d(albedo_texture, inp.uv - float2(0.0, step_size)).xyz
    let d = tex2d(albedo_texture, inp.uv + float2(0.0, step_size)).xyz
    // unsharp-mask kernel: center*(1+4a) - neighbors*a
    let result = c.xyz * (1.0 + 4.0 * amount) - (l + r + u + d) * amount
    return PbrOutput(
        albedo = saturate(result),
        alpha = c.w,
        alphaCutoff = 0.5,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
