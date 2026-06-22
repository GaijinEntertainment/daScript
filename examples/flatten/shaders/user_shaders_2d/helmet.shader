require engine.render.shader_dsl

var {
    @color visorColor = float3(0.3, 0.7, 1.0)
    @color hudColor   = float3(0.2, 1.0, 0.5)
    visorThickness    = 0.07   // thickness of the visor frame ring
    lensDistort       = 0.04
}

[pixel_shader]
def helmet(inp : PostEffectInput) {
    let uv = inp.screenPos

    // Oval visor shape: scale Y so the visor is wider than tall
    let centered = uv - float2(0.5)
    let oval     = float2(centered.x * 0.85, centered.y * 1.15)
    let ovalDist = length(oval)

    // Lens barrel distortion (only inside visor)
    let r2     = dot(centered, centered)
    let distUV = centered * (1.0 + lensDistort * r2) + float2(0.5)
    let scene  = sample_scene_color(distUV)

    // inside = 1 inside visor, 0 outside
    let inside    = smooth_step(0.52, 0.50, ovalDist)
    // innerEdge = 1 in visor interior (not the frame border)
    let innerEdge = smooth_step(0.45, 0.43, ovalDist)
    // frame = just the ring border
    let frame     = inside * (1.0 - innerEdge)

    // Outer darkness beyond visor
    let outside = smooth_step(0.50, 0.58, ovalDist)

    // Subtle reflection streak in upper-left of visor
    let reflectUV = float2(centered.x + 0.15, centered.y + 0.2)
    let reflectDist = length(float2(reflectUV.x * 2.5, reflectUV.y * 0.6))
    let reflection = smooth_step(0.18, 0.0, reflectDist) * innerEdge * 0.35

    // HUD: thin horizontal line at bottom third of visor
    let hudLine = smooth_step(0.004, 0.0, abs(centered.y + 0.12)) * smooth_step(0.35, 0.30, abs(centered.x)) * innerEdge

    // HUD: corner bracket marks
    let bx = abs(centered.x) - 0.28
    let by = abs(centered.y) - 0.18
    let bracketX = smooth_step(0.003, 0.0, abs(bx)) * smooth_step(0.0, 0.04, by) * smooth_step(0.07, 0.03, by) * innerEdge
    let bracketY = smooth_step(0.003, 0.0, abs(by)) * smooth_step(0.0, 0.04, bx) * smooth_step(0.07, 0.03, bx) * innerEdge
    let brackets = saturate(bracketX + bracketY)

    let pulse = sin(g_Time * 2.0) * 0.15 + 0.85

    let base    = scene * (1.0 - outside * 0.97)
    let withRim = base + visorColor * frame * 0.9 + float3(reflection)
    let result  = withRim + hudColor * (hudLine + brackets) * pulse

    return PostEffectOutput(out_color = result)
}
