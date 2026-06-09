require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    blurSize = 0.01
}

// Fixture for the multi-iterator form `for (a, b in [...], [...])` — two parallel
// arrays unrolled in lockstep: each tap has its own offset AND its own weight,
// neither derivable from the other.
[pixel_shader]
def loop_multi(inp : PbrInput) {
    let s = blurSize
    var acc = float3(0.0, 0.0, 0.0)
    for (off, weight in [float2(-s, 0.0), float2(0.0, 0.0), float2(s, 0.0)], [0.25, 0.5, 0.25]) {
        acc += tex2d(albedo_texture, inp.uv + off).xyz * weight
    }
    return PbrOutput(
        albedo = acc,
        emission = acc,
        emissionStrength = 1.0,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
