require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    strength = 0.5
    radius = 0.5
}

[pixel_shader]
def bulge(inp : PbrInput) {
    let center = inp.uv - float2(0.5, 0.5)
    let dist = length(center)
    // push UVs outward (bulge) or inward (pinch via negative strength) within a radius -> lens / fisheye
    let factor = saturate(one_minus(dist / radius))
    let warp = 1.0 - factor * factor * strength
    let uv = center * warp + float2(0.5, 0.5)
    let c = tex2d(albedo_texture, uv)
    return PbrOutput(
        albedo = c.xyz,
        alpha = c.w,
        alphaCutoff = 0.5,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
