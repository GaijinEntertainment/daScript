require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    step_size = 0.004
    strength = 2.0
}

[pixel_shader]
def emboss(inp : PbrInput) {
    // difference between an up-left and a down-right tap -> directional relief (engraved look)
    let tl = tex2d(albedo_texture, inp.uv - float2(step_size, step_size)).xyz
    let br = tex2d(albedo_texture, inp.uv + float2(step_size, step_size)).xyz
    let diff = dot(br - tl, float3(0.299, 0.587, 0.114)) * strength
    let result = float3(0.5) + float3(diff)
    return PbrOutput(
        albedo = saturate(result),
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
