require engine.render.shader_dsl
[pixel_shader(flatten=false)]
def plasma_scalar(inp : PbrInput) {
    let t  = g_Time * 1.5
    let uv = inp.uv * 4.0
    let v1 = sin(uv.x + t)
    let v2 = sin(uv.y + t * 0.7)
    let v3 = sin(uv.x + uv.y + t * 0.5)
    let v4 = sin(length(uv - float2(2.0, 2.0)) - t)
    let v  = (v1 + v2 + v3 + v4) * 0.125 + 0.5
    return PbrOutput(albedo = float3(v, v, v), emission = float3(0.0, 0.0, 0.0),
        emissionStrength = 0.0, metalness = 0.0, roughness = 1.0, ao = 1.0)
}
