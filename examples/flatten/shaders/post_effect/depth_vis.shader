require engine.render.shader_dsl

var {
    max_depth = 50.0
}

[pixel_shader]
def depth_vis(inp : PostEffectInput) {
    let depth = sample_scene_depth(inp.screenPos)
    let t = smooth_step(1.0, 0.0, depth / max_depth)
    return PostEffectOutput(out_color = float3(t))
}
