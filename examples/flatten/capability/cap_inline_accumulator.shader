require engine.render.shader_dsl

// Capability: a `var` accumulator declared inside a HELPER and reassigned in a parallel loop.
// flatten inlines the helper, uniquifying the accumulator to `__flat_loc_N`, and must SSA-rename
// it like a user accumulator — else it stays a self-referential `var` whose live init a later DSE
// strips, and the backend rejects the null-init self-ref (error[50503] undefined __flat_loc_N).
// Distilled from the reported `dithering` shader (its 4x4 Bayer threshold accumulator).

var { levels = 4.0 }

def pick_threshold(cell : float) : float {
    var threshold = 0.0
    for (i, v in range(4), [0.0, 8.0, 4.0, 12.0]) {
        threshold = float(i) == cell ? v * (1.0 / 16.0) : threshold
    }
    return threshold
}

[pixel_shader]
def cap_inline_accumulator(inp : PbrInput) {
    let cell = floor(frac(inp.uv.x) * 4.0)
    let t    = pick_threshold(cell)
    let q    = floor(inp.uv.y * levels + t) / levels
    return PbrOutput(
        albedo = float3(q, q, q), emission = float3(0), emissionStrength = 0.0,
        metalness = 0.0, roughness = 1.0, ao = 1.0
    )
}
