require engine.render.shader_dsl

var {
    @color albedo = float3(1.0, 1.0, 1.0)
    alpha = 1.0
    albedo_texture = Sampler2D("%builtin_package/logo.png")
}

[pixel_shader]
def new_shader_pure(inp : PbrInput) {
    let textureColor = tex2d(albedo_texture, inp.uv)
    return PbrOutput(
        albedo = albedo * textureColor.xyz,
        alpha = alpha * textureColor.a,
        alphaCutoff = 0.5,
        metalness = 0.0,
        roughness = 1.0,
        emission = float3(0),
        emissionStrength = 1.0,
        ao = 1.0
    )
}
