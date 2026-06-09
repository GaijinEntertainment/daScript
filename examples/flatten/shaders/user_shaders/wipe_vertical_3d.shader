require engine.render.shader_dsl

var {
    @color baseColor = float3(0.6, 0.8, 1.0)
    @color edgeColor = float3(1.5, 1.8, 2.5)
    edgeWidth = 0.05
}

[pixel_shader]
def wipe_vertical_3d(inp : PbrInput) {
    let threshold = remap(sin(g_Time * 0.8), float2(-1.0, 1.0), float2(-0.2, 1.2))
    let h = inp.localPos.y * 0.5 + 0.5
    let mask = step(threshold, h)
    let edge = smooth_step(threshold - edgeWidth, threshold, h) - mask
    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        albedo = baseColor * mask,
        emission = edgeColor * edge,
        emissionStrength = edge * lerp(1.0, 3.0, daylight),
        alpha = mask + edge,
        alphaCutoff = 0.5,
        metalness = 0.0,
        roughness = 0.8,
        ao = 1.0
    )
}
