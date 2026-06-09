require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    blurSize = 0.01
}

// Fixture for `for i in urange(N)` unrolling — iterator is a uint constant
// (0u, 1u, 2u), used in float math via float(i).
[pixel_shader]
def loop_urange(inp : PbrInput) {
    let s = blurSize
    var acc = float3(0.0, 0.0, 0.0)
    for (i in urange(3u)) {
        let off = float2(float(i) * s, 0.0)
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
