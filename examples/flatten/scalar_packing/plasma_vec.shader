require engine.render.shader_dsl
[pixel_shader(flatten=false)]
def plasma_vec(inp : PbrInput) {
    let t    = g_Time * 1.5
    let uv   = inp.uv * 4.0
    let args = float4(uv.x + t, uv.y + t * 0.7, uv.x + uv.y + t * 0.5, length(uv - float2(2.0, 2.0)) - t)
    let s    = sin(args)
    let v    = (s.x + s.y + s.z + s.w) * 0.125 + 0.5
    return PbrOutput(albedo = float3(v, v, v), emission = float3(0.0, 0.0, 0.0),
        emissionStrength = 0.0, metalness = 0.0, roughness = 1.0, ao = 1.0)
}
