require engine.render.shader_dsl

// One voronoi cell, scalar — matches flatten's per-cell output form (mads already fused).
[pixel_shader(flatten=false)]
def cell_scalar(inp : PbrInput) {
    let uv  = inp.uv * 5.0
    let t   = g_Time * 0.3
    let ncx = floor(uv.x)
    let ncy = floor(uv.y)
    let hx  = frac(sin(ncx * 127.1 + ncy * 311.7) * 43758.5)
    let hy  = frac(sin(ncx * 269.5 + ncy * 183.3) * 43758.5)
    let dx  = mad(sin(mad(hx, 6.28, t)), 0.3, mad(hx, 0.5, ncx)) - uv.x
    let dy  = mad(cos(mad(hy, 6.28, t)), 0.3, mad(hy, 0.5, ncy)) - uv.y
    let d   = sqrt(mad(dx, dx, dy * dy))
    return PbrOutput(albedo = float3(d, d, d), emission = float3(0.0, 0.0, 0.0),
        emissionStrength = 0.0, metalness = 0.0, roughness = 1.0, ao = 1.0)
}
