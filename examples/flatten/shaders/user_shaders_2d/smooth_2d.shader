require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    step_size = 0.003
    amount = 1.0
}

[pixel_shader]
def smooth_2d(inp : PbrInput) {
    let c = tex2d(albedo_texture, inp.uv)
    let l = tex2d(albedo_texture, inp.uv - float2(step_size, 0.0)).xyz
    let r = tex2d(albedo_texture, inp.uv + float2(step_size, 0.0)).xyz
    let u = tex2d(albedo_texture, inp.uv - float2(0.0, step_size)).xyz
    let d = tex2d(albedo_texture, inp.uv + float2(0.0, step_size)).xyz
    // inverse of sharpen_2d: blend the center toward the 4-neighbour average to
    // soften edges (amount 0 = original, 1 = fully smoothed)
    let neighborAvg = (l + r + u + d) * 0.25
    let result = lerp(c.xyz, neighborAvg, amount)
    return PbrOutput(
        albedo = result,
        alpha = c.w,
        alphaCutoff = 0.5,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
