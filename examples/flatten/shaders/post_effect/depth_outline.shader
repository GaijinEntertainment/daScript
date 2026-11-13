require engine.render.shader_dsl

var {
    @color outlineColor = float3(0.1, 0.8, 1.0)
    outlineThickness    = 0.001
    depthThreshold      = 0.5
    maxDepth            = 80.0
    darken              = 0.5
}

[pixel_shader]
def depth_outline(inp : PostEffectInput) {
    let uv    = inp.screenPos
    let scene = sample_scene_color(uv)

    let c = sample_scene_depth(uv)
    let n = sample_scene_depth(uv + float2(0.0,  outlineThickness))
    let s = sample_scene_depth(uv + float2(0.0, -outlineThickness))
    let e = sample_scene_depth(uv + float2( outlineThickness, 0.0))
    let w = sample_scene_depth(uv + float2(-outlineThickness, 0.0))
    let maxDiff = max(max(abs(c - n), abs(c - s)), max(abs(c - e), abs(c - w)))
    let edge    = smooth_step(depthThreshold, depthThreshold * 3.0, maxDiff)

    let skyMask = step(maxDepth * 0.98, c)
    let outline = outlineColor * edge * (1.0 - skyMask)
    let dimmed  = lerp(scene * darken, scene, 1.0 - edge)

    return PostEffectOutput(out_color = dimmed + outline)
}
