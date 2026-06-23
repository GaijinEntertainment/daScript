require engine.render.shader_dsl

var {
    @color frostColor   = float3(0.75, 0.90, 1.0)
    @color iceEdgeColor = float3(0.95, 0.98, 1.0)
    @color deepIce      = float3(0.4,  0.65, 1.0)
    freezeAmount        = 0.65
    crystalScale        = 9.0
}

[pixel_shader]
def frost(inp : PostEffectInput) {
    let uv = inp.screenPos

    // Edge-based mask: stronger at screen edges, zero in center
    let dist    = length(uv - float2(0.5)) * 1.41421
    let iceMask = smooth_step(1.0 - freezeAmount, 1.0, dist)

    // Crystal structure: three overlapping noise layers at different scales/angles
    // Using abs(noise - 0.5) * 2 sharpens each into a ridge pattern
    let n1 = abs(noise(uv * crystalScale) - 0.5) * 2.0
    let n2 = abs(noise(uv * crystalScale * 1.7 + float2(0.4, 0.9)) - 0.5) * 2.0
    let n3 = abs(noise(uv * crystalScale * 0.6 + float2(1.3, 0.2)) - 0.5) * 2.0

    // Crystal edges = where ridges from different layers intersect (minimum = tightest crossing)
    let cellEdge = min(min(n1, n2), n3)
    let edge     = smooth_step(0.15, 0.0, cellEdge) * iceMask  // sharp bright lines

    // Fill: deeper ice color in cell interiors
    let fill = smooth_step(0.5, 0.8, n1 * 0.5 + n2 * 0.5) * iceMask

    // Scene: desaturate and blue-tint under ice, blur slightly
    let offset = float2(n1 - 0.5, n2 - 0.5) * 0.003 * iceMask
    let scene  = sample_scene_color(uv + offset)
    let gray   = dot(scene, float3(0.299, 0.587, 0.114))
    let frozen = lerp(scene, lerp(float3(gray) * frostColor, deepIce, fill), iceMask * 0.8)

    // Overlay bright crystal edges on top
    let result = frozen + iceEdgeColor * edge * 1.5

    return PostEffectOutput(out_color = result)
}
