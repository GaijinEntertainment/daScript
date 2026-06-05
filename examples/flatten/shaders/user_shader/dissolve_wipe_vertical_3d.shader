require engine.render.shader_dsl

var {
    @color baseColor = float3(0.7, 0.7, 0.8)
    @color edgeColor = float3(0.3, 1.5, 0.5)
    noiseScale = 4.0
    edgeWidth = 0.08
}

[shader(stage="pixel", type="pbr")]
def dissolve_wipe_vertical_3d(inp : PbrInput) : PbrOutput {
    let threshold = remap(sin(g_Time * 0.8), float2(-1.0, 1.0), float2(-0.1, 1.1))
    let n = (noise(inp.worldPos * noiseScale) * 2.0 - 1.0) * 0.3
    let dissolve = inp.localPos.y * 0.5 + 0.5 + n
    let mask = step(threshold, dissolve)
    let edge = smooth_step(threshold - edgeWidth, threshold, dissolve) - mask
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
