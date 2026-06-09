require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    @color edgeColor = float3(1.5, 1.5, 2.0)
    edgeWidth = 0.04
}

[pixel_shader]
def wipe_horizontal(inp : PbrInput) {
    let c = tex2d(albedo_texture, inp.uv).xyz
    let threshold = remap(sin(g_Time * 0.8), float2(-1.0, 1.0), float2(0.0, 1.0))
    let mask = step(threshold, inp.uv.x)
    let edge = smooth_step(threshold - edgeWidth, threshold, inp.uv.x) * one_minus(mask)
    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        albedo = c * mask + edgeColor * edge,
        emission = edgeColor * edge,
        emissionStrength = edge * lerp(1.0, 3.0, daylight),
        alpha = mask + edge,
        alphaCutoff = 0.5,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
