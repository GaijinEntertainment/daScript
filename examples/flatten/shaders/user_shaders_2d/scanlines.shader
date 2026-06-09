require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    lineFreq = 120.0
    lineStrength = 0.7
    scrollSpeed = 0.8
}

[pixel_shader]
def scanlines(inp : PbrInput) {
    let c = tex2d(albedo_texture, inp.uv).xyz
    let scan = sin((inp.uv.y + g_Time * scrollSpeed) * lineFreq) * 0.5 + 0.5
    let mask = step(0.5, scan)
    let result = c * lerp(1.0 - lineStrength, 1.0, mask)
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
