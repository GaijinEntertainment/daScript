require engine.render.shader_dsl

var {
    @color shieldColor = float3(0.3, 0.6, 2.0)
    @color hitColor = float3(1.5, 1.8, 2.5)
    hitSpeed = 4.0
    rings = 5.0
    fresnel_power = 1.5
}

[pixel_shader]
def shield_hit_3d(inp : PbrInput) {
    let n = length(inp.localPos)
    let pulse = frac(n * rings - g_Time * hitSpeed)
    let ring = smooth_step(0.0, 0.5, pulse) * smooth_step(1.0, 0.5, pulse)
    let flash = 0.8
    let f = fresnel(fresnel_power, inp.worldNormal, inp.viewDir)
    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        albedo = float3(0),
        emission = shieldColor * f + hitColor * ring * flash,
        emissionStrength = (f * 0.3 + ring * flash * 0.5) * lerp(0.08, 1.0, daylight),
        metalness = 0.0,
        roughness = 0.1,
        ao = 1.0
    )
}
