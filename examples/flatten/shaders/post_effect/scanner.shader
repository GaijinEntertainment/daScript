require engine.render.shader_dsl

var {
    @color scan_color = float3(0.0, 1.0, 0.4)
    speed     = 4.0    // wave travel speed in world units/sec
    frequency = 0.4    // rings per world unit (lower = wider spacing)
    sharpness = 10.0   // ring thinness (higher = narrower rings)
    intensity = 3.0    // peak ring brightness
    dim_scene = 0.15   // scene multiplier between rings (0 = pitch black, 1 = unaffected)
}

[pixel_shader]
def scanner(inp : PostEffectInput) {
    let uv    = inp.screenPos
    let col   = sample_scene_color(uv)
    let raw_depth = sample_scene_depth(uv)
    let sky_mask  = step(100.0, raw_depth)   // 1 for sky / far background, 0 for scene
    let depth     = min(raw_depth, 50.0)
    let s = smooth_step(1.0, 0.0, depth / 50.0) // fade in effect from near to far

    // Rings travel outward from the camera.
    // sin(phase*pi) goes 0->1->0 each period; pow sharpens it to a thin spike.
    let phase = depth * frequency - g_Time * speed
    // Zero out the wave on sky pixels so they are never affected.
    let wave  = pow(saturate(sin(phase * 3.14159265)), sharpness) * intensity * (1.0 - sky_mask) * s

    // Scene pixels are dimmed; sky pixels keep their original brightness (sky_mask -> lerp to 1).
    let scene_dim = lerp(dim_scene, 1.0, sky_mask)
    return PostEffectOutput(out_color = col * scene_dim + scan_color * wave)
}
