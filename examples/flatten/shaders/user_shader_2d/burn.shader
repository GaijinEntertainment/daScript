require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    @color burnColor = float3(2.0, 0.4, 0.0)
    burnSpeed = 0.6
    noiseScale = 5.0
}

[shader(stage="pixel", type="pbr")]
def burn(inp : PbrInput) : PbrOutput {
    let c = tex2d(albedo_texture, inp.uv).xyz
    let t = g_Time * burnSpeed
    let n = noise(float2(inp.uv.x * noiseScale, inp.uv.y * noiseScale * 0.5 + t))
    let burnLine = remap(sin(t * 0.8), float2(-1.0, 1.0), float2(0.1, 0.9))
    let burnMask = smooth_step(burnLine + n * 0.3, burnLine + n * 0.3 + 0.05, 1.0 - inp.uv.y)
    let edgeMask = smooth_step(burnLine + n * 0.3 - 0.1, burnLine + n * 0.3, 1.0 - inp.uv.y) - burnMask
    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        albedo = c * (1.0 - burnMask) + burnColor * edgeMask,
        emission = burnColor * edgeMask,
        emissionStrength = edgeMask * lerp(1.0, 3.0, daylight),
        alpha = 1.0 - burnMask + edgeMask,
        alphaCutoff = 0.5,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
