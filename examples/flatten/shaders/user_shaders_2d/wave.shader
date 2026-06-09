require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    waveAmp = 0.025
    waveFreq = 10.0
    waveSpeed = 2.0
}

[pixel_shader]
def wave(inp : PbrInput) {
    let t = g_Time * waveSpeed
    let dx = sin(inp.uv.y * waveFreq + t) * waveAmp
    let dy = sin(inp.uv.x * waveFreq + t * 1.3) * waveAmp
    let result = tex2d(albedo_texture, inp.uv + float2(dx, dy)).xyz
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
