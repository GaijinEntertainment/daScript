require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    @color edgeColor = float3(1.0, 0.5, 0.1)
    noiseScale = 6.0
    edgeWidth = 0.12
    speed = 0.25
}

[pixel_shader]
def dissolve_edge(inp : PbrInput) {
    let c = tex2d(albedo_texture, inp.uv)
    let n = noise(inp.uv * noiseScale)
    // animated threshold sweeps the noise field -> sprite dissolves, with a glowing border at the edge
    let threshold = frac(g_Time * speed)
    let visible = step(threshold, n)
    let edge = smooth_step(threshold, threshold + edgeWidth, n) - visible
    let glow = saturate(edge)
    let result = c.xyz + edgeColor * glow
    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        albedo = result,
        emission = edgeColor * glow,
        emissionStrength = glow * lerp(1.0, 3.0, daylight),
        alpha = c.w * visible,
        alphaCutoff = 0.5,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
