require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    rippleSpeed = 2.0
    rippleFreq = 20.0
    rippleAmp = 0.025
}

[pixel_shader]
def ripple(inp : PbrInput) {
    let uv = inp.uv - float2(0.5, 0.5)
    let dist = length(uv)
    let offset = sin(dist * rippleFreq - g_Time * rippleSpeed) * rippleAmp
    let warped = inp.uv + normalize(uv) * offset
    let result = tex2d(albedo_texture, warped).xyz
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
