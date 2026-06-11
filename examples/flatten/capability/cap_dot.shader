require engine.render.shader_dsl

// Capability demo: horizontal-add → dot re-fusion. Each lane read is a full graph node (a
// splat), so `c.x + c.y + c.z` costs 3 splats + 2 adds; the finishing pass collapses it — and
// the weighted `c.x*0.299 + c.y*0.587 + c.z*0.114` luma spelling — into ONE dot node each,
// with the lane weights riding as a free const mask.

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
}

[pixel_shader]
def cap_dot(inp : PbrInput) : PbrOutput {
    let c = tex2d(albedo_texture, inp.uv).xyz
    let lum = c.x * 0.299 + c.y * 0.587 + c.z * 0.114
    let sum = c.x + c.y + c.z
    return PbrOutput(
        albedo = float3(lum, sum * 0.333, lum),
        emission = float3(0.0, 0.0, 0.0),
        emissionStrength = 1.0,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
