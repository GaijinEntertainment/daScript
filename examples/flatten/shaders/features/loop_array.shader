require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    blurSize = 0.01
}

// Fixture for `for v in [literal array]` unrolling + accumulator.
// A horizontal 3-tap blur whose tap offsets come straight from an array literal
// in the loop header (no index — the iterator IS the offset value).
[pixel_shader]
def loop_array(inp : PbrInput) {
    let s = blurSize
    var acc = float3(0.0, 0.0, 0.0)
    for (off in [float2(-s, 0.0), float2(0.0, 0.0), float2(s, 0.0)]) {
        acc += tex2d(albedo_texture, inp.uv + off).xyz
    }
    let result = acc * (1.0 / 3.0)
    return PbrOutput(
        albedo = result,
        emission = result,
        emissionStrength = 1.0,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
