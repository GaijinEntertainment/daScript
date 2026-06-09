require engine.render.shader_dsl

var {
    @color colorA = float3(0.8, 0.0, 1.0)
    @color colorB = float3(0.0, 0.8, 1.0)
    @color colorC = float3(1.0, 0.5, 0.0)
    speed = 1.5
    scale = 4.0
}

[pixel_shader]
def plasma(inp : PbrInput) {
    let t = g_Time * speed
    let uv = inp.uv * scale
    let v1 = sin(uv.x + t)
    let v2 = sin(uv.y + t * 0.7)
    let v3 = sin(uv.x + uv.y + t * 0.5)
    let v4 = sin(length(uv - float2(2.0, 2.0)) - t)
    let v = (v1 + v2 + v3 + v4) * 0.25 * 0.5 + 0.5
    let c = lerp(colorA, colorB, v)
    let result = lerp(c, colorC, sin(v * 3.14159) * 0.5 + 0.5)
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
