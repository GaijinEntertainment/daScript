require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    @color edgeColor = float3(0.3, 1.5, 0.5)
    noiseScale = 8.0
    edgeWidth = 0.08
}

[shader(stage="pixel", type="pbr")]
def dissolve_wipe_v(inp : PbrInput) : PbrOutput {
    let c = tex2d(albedo_texture, inp.uv).xyz
    let threshold = remap(sin(g_Time * 0.8), float2(-1.0, 1.0), float2(-0.1, 1.1))
    let n = (noise(inp.uv * noiseScale) * 2.0 - 1.0) * 0.3
    let dissolve = one_minus(inp.uv.y) + n
    let mask = step(threshold, dissolve)
    let edge = smooth_step(threshold - edgeWidth, threshold, dissolve) - mask
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

