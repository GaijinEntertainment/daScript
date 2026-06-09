require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    @color shineColor = float3(2.0, 2.0, 2.5)
    shineSpeed = 0.8
    shineWidth = 0.12
}

[pixel_shader]
def shiny(inp : PbrInput) {
    let c = tex2d(albedo_texture, inp.uv).xyz
    let pos = remap(sin(g_Time * shineSpeed), float2(-1.0, 1.0), float2(-0.2, 1.2))
    let diag = inp.uv.x + inp.uv.y
    let shine = smooth_step(pos - shineWidth, pos, diag) * smooth_step(pos + shineWidth, pos, diag)
    let result = c + shineColor * shine
    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        albedo = result,
        emission = shineColor * shine,
        emissionStrength = shine * lerp(1.0, 3.0, daylight),
        metalness = 0.3,
        roughness = 0.3,
        ao = 1.0
    )
}
