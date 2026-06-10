require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    exposure = 2.5
}

// Two per-pixel divisions by the same uniform prop. flatten hoists ONE shared per-draw
// reciprocal (`_preshader_N = 1f / exposure`) and turns both divisions into muls — the
// per-pixel graph carries no division by `exposure` at all. Fast-math (the reciprocal
// rounds); `options _flatten_no_fast_math` keeps the divisions.
[pixel_shader]
def cap_rcp(inp : PbrInput) {
    let c = tex2d(albedo_texture, inp.uv).xyz
    let toned = c / exposure
    let glow = (c.x + c.y) / exposure
    let result = toned + float3(glow * 0.1, 0.0, 0.0)
    return PbrOutput(
        albedo = result,
        emission = result,
        emissionStrength = 1.0,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
