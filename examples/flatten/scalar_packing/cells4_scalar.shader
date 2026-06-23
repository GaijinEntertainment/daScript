require engine.render.shader_dsl

// 4 voronoi cells (2x2 block), scalar — four independent distance computations + min.
[pixel_shader(flatten=false)]
def cells4_scalar(inp : PbrInput) {
    let uv  = inp.uv * 5.0
    let t   = g_Time * 0.3
    let cx  = floor(uv.x)
    let cy  = floor(uv.y)
    let cx1 = cx + 1.0
    let cy1 = cy + 1.0
    // cell 0 (cx,cy)
    let hx0 = frac(sin(cx * 127.1 + cy * 311.7) * 43758.5)
    let hy0 = frac(sin(cx * 269.5 + cy * 183.3) * 43758.5)
    let dx0 = mad(sin(mad(hx0, 6.28, t)), 0.3, mad(hx0, 0.5, cx)) - uv.x
    let dy0 = mad(cos(mad(hy0, 6.28, t)), 0.3, mad(hy0, 0.5, cy)) - uv.y
    let d0  = sqrt(mad(dx0, dx0, dy0 * dy0))
    // cell 1 (cx1,cy)
    let hx1 = frac(sin(cx1 * 127.1 + cy * 311.7) * 43758.5)
    let hy1 = frac(sin(cx1 * 269.5 + cy * 183.3) * 43758.5)
    let dx1 = mad(sin(mad(hx1, 6.28, t)), 0.3, mad(hx1, 0.5, cx1)) - uv.x
    let dy1 = mad(cos(mad(hy1, 6.28, t)), 0.3, mad(hy1, 0.5, cy)) - uv.y
    let d1  = sqrt(mad(dx1, dx1, dy1 * dy1))
    // cell 2 (cx,cy1)
    let hx2 = frac(sin(cx * 127.1 + cy1 * 311.7) * 43758.5)
    let hy2 = frac(sin(cx * 269.5 + cy1 * 183.3) * 43758.5)
    let dx2 = mad(sin(mad(hx2, 6.28, t)), 0.3, mad(hx2, 0.5, cx)) - uv.x
    let dy2 = mad(cos(mad(hy2, 6.28, t)), 0.3, mad(hy2, 0.5, cy1)) - uv.y
    let d2  = sqrt(mad(dx2, dx2, dy2 * dy2))
    // cell 3 (cx1,cy1)
    let hx3 = frac(sin(cx1 * 127.1 + cy1 * 311.7) * 43758.5)
    let hy3 = frac(sin(cx1 * 269.5 + cy1 * 183.3) * 43758.5)
    let dx3 = mad(sin(mad(hx3, 6.28, t)), 0.3, mad(hx3, 0.5, cx1)) - uv.x
    let dy3 = mad(cos(mad(hy3, 6.28, t)), 0.3, mad(hy3, 0.5, cy1)) - uv.y
    let d3  = sqrt(mad(dx3, dx3, dy3 * dy3))

    let m = min(min(d0, d1), min(d2, d3))
    return PbrOutput(albedo = float3(m, m, m), emission = float3(0.0, 0.0, 0.0),
        emissionStrength = 0.0, metalness = 0.0, roughness = 1.0, ao = 1.0)
}
