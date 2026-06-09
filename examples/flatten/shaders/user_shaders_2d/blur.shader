require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    blurSize = 0.015
}

[pixel_shader]
def blur(inp : PbrInput) {
    let s = blurSize
    // 3x3 box blur — both for loops are fully unrolled at compile time into
    // 9 texture taps (the shader graph has no runtime loop construct), and `acc`
    // is a compile-time accumulator: each `+=` chains one more add node.
    var acc = float3(0.0, 0.0, 0.0)
    for (y in range(3)) {
        for (x in range(3)) {
            let offset = float2(float(x - 1) * s, float(y - 1) * s)
            acc += tex2d(albedo_texture, inp.uv + offset).xyz
        }
    }
    let result = acc * (1.0 / 9.0)
    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        albedo = result,
        emission = result,
        emissionStrength = lerp(0.5, 2.0, daylight),
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
