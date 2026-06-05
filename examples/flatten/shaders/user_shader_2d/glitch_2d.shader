require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    @color glitchColorA = float3(2.0, 0.0, 0.5)
    @color glitchColorB = float3(0.0, 0.3, 2.0)
    speed = 6.0
    intensity = 0.04
    barFreq = 15.0
}

[shader(stage="pixel", type="pbr")]
def glitch_2d(inp : PbrInput) : PbrOutput {
    let t = g_Time * speed
    let bar = floor(inp.uv.y * barFreq + t) / barFreq
    let noise = frac(sin(bar * 127.1 + t) * 43758.5)
    let shift = (noise - 0.5) * intensity * step(0.7, noise)
    let uvR = inp.uv + float2(shift, 0)
    let uvB = inp.uv - float2(shift, 0)
    let r = tex2d(albedo_texture, uvR)
    let g = tex2d(albedo_texture, inp.uv)
    let b = tex2d(albedo_texture, uvB)
    let col = float3(r.x, g.y, b.z)
    let a = g.a
    let scanline = step(0.85, noise) * step(0.0, shift)
    let tint = lerp(col, glitchColorA * r.x + glitchColorB * b.z, scanline * 0.6)
    let daylight = g_LightDirection.y
    return PbrOutput(
        albedo = tint * a,
        emission = (glitchColorA * r.x + glitchColorB * b.z) * scanline * a,
        emissionStrength = scanline * lerp(0.3, 1.5, daylight),
        alpha = a,
        alphaCutoff = 0.1,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
