require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    @color flashColor = float3(1.0, 1.0, 1.0)
    flashSpeed = 6.0
    flashAmount = 1.0
}

[pixel_shader]
def flash(inp : PbrInput) {
    let c = tex2d(albedo_texture, inp.uv)
    // pulsing solid-color tint over the whole sprite -- the classic hit/damage flash
    let pulse = (sin(g_Time * flashSpeed) * 0.5 + 0.5) * flashAmount
    let result = lerp(c.xyz, flashColor, pulse)
    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        albedo = result,
        emission = flashColor * pulse,
        emissionStrength = pulse * lerp(0.3, 1.5, daylight),
        alpha = c.w,
        alphaCutoff = 0.5,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
