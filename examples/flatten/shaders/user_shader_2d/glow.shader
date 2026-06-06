require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    @color glowColor = float3(0.8, 0.3, 2.0)
    glowSize = 0.015
    pulse = 0.3
}

[shader(stage="pixel", type="pbr")]
def glow(inp : PbrInput) : PbrOutput {
    let c = tex2d(albedo_texture, inp.uv).xyz
    let n = tex2d(albedo_texture, inp.uv + float2(0, glowSize)).xyz
    let e = tex2d(albedo_texture, inp.uv + float2(glowSize, 0)).xyz
    let edgeN = length(c - n)
    let edgeE = length(c - e)
    let rim = smooth_step(0.1, 0.5, max(edgeN, edgeE))
    let pulseMask = sin(g_Time * 3.0) * pulse + (1.0 - pulse)
    let daylight = g_LightDirection.y
    return PbrOutput(
        albedo = c,
        emission = glowColor * rim * pulseMask,
        emissionStrength = rim * lerp(0.5, 2.5, daylight),
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
