require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    @color shieldColor = float3(0.3, 0.7, 2.0)
    hitSpeed = 3.0
    rings = 4.0
}

[shader(stage="pixel", type="pbr")]
def shield_hit(inp : PbrInput) : PbrOutput {
    let c = tex2d(albedo_texture, inp.uv).xyz
    let uv = inp.uv - float2(0.5, 0.5)
    let dist = length(uv)
    let pulse = frac(dist * rings - g_Time * hitSpeed)
    let ring = smooth_step(0.0, 0.3, pulse) * smooth_step(1.0, 0.7, pulse)
    let flash = sin(g_Time * 8.0) * 0.5 + 0.5
    let result = lerp(c, shieldColor, ring * flash * 0.7)
    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        albedo = result,
        emission = shieldColor * ring * flash,
        emissionStrength = ring * flash * lerp(0.5, 2.5, daylight),
        metalness = 0.0,
        roughness = 0.2,
        ao = 1.0
    )
}
