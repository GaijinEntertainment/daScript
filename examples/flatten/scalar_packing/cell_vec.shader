require engine.render.shader_dsl

// Same cell, hand-vectorized: pack the x/y scalars into float2 vars. The trig stays per-lane
// (sin vs cos differ), everything else goes float2.
[pixel_shader(flatten=false)]
def cell_vec(inp : PbrInput) {
    let uv   = inp.uv * 5.0
    let t    = g_Time * 0.3
    let nc   = floor(uv)
    let h    = frac(sin(float2(nc.x * 127.1 + nc.y * 311.7, nc.x * 269.5 + nc.y * 183.3)) * 43758.5)
    let trig = float2(sin(mad(h.x, 6.28, t)), cos(mad(h.y, 6.28, t)))
    let dvec = mad(trig, 0.3, mad(h, 0.5, nc)) - uv
    let d    = length(dvec)
    return PbrOutput(albedo = float3(d, d, d), emission = float3(0.0, 0.0, 0.0),
        emissionStrength = 0.0, metalness = 0.0, roughness = 1.0, ao = 1.0)
}
