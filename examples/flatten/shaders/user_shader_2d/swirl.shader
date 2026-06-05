require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    swirlStrength = 3.0
    swirlSpeed = 1.0
}

[shader(stage="pixel", type="pbr")]
def swirl(inp : PbrInput) : PbrOutput {
    let uv = inp.uv - float2(0.5, 0.5)
    let dist = length(uv)
    let angle = atan2(uv.y, uv.x) + swirlStrength * dist * sin(g_Time * swirlSpeed)
    let warped = float2(cos(angle), sin(angle)) * dist + float2(0.5, 0.5)
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
