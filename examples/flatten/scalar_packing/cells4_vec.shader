require engine.render.shader_dsl

// Same 4 cells, packed across cells into ONE float4 lane-set. Each lane is a full independent
// cell, so the body has no within-vector lane-crossing (sin is sin in all 4 lanes, etc.).
// mad() needs matching widths, so scalar args are broadcast (operators broadcast on their own).
[pixel_shader(flatten=false)]
def cells4_vec(inp : PbrInput) {
    let uv  = inp.uv * 5.0
    let t   = g_Time * 0.3
    let cx  = floor(uv.x)
    let cy  = floor(uv.y)
    let cx1 = cx + 1.0
    let cy1 = cy + 1.0
    let ncx = float4(cx, cx1, cx, cx1)
    let ncy = float4(cy, cy, cy1, cy1)
    let t4  = float4(t)
    let uvx4 = float4(uv.x)
    let uvy4 = float4(uv.y)
    let hx  = frac(sin(ncx * 127.1 + ncy * 311.7) * 43758.5)
    let hy  = frac(sin(ncx * 269.5 + ncy * 183.3) * 43758.5)
    let dx  = mad(sin(mad(hx, float4(6.28), t4)), float4(0.3), mad(hx, float4(0.5), ncx)) - uvx4
    let dy  = mad(cos(mad(hy, float4(6.28), t4)), float4(0.3), mad(hy, float4(0.5), ncy)) - uvy4
    let d   = sqrt(mad(dx, dx, dy * dy))
    let m   = min(min(d.x, d.y), min(d.z, d.w))
    return PbrOutput(albedo = float3(m, m, m), emission = float3(0.0, 0.0, 0.0),
        emissionStrength = 0.0, metalness = 0.0, roughness = 1.0, ao = 1.0)
}
