require engine.render.shader_dsl

// Minimal posteffect contract fixture: passthrough with a slight desaturation.
// Verifies that PostEffectInput / PostEffectOutput compile and screenPos + sample_scene_color work.
[pixel_shader]
def posteffect_basic(inp : PostEffectInput) {
    let col = sample_scene_color(inp.screenPos)
    let lum = dot(col, float3(0.299, 0.587, 0.114))
    return PostEffectOutput(out_color = lerp(col, float3(lum), 0.5))
}
