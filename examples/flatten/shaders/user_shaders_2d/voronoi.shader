require engine.render.shader_dsl

// Voronoi / cellular noise — 2x2 block centered on the pixel via round().
// The original used floor() + a forward-only 2x2 block, so the nearest feature
// point in the back/left cells was never tested -> hard diagonal seams. Rounding
// to the nearest lattice point and testing the 4 cells touching it keeps the
// search centered on the pixel (no directional bias) while staying within the
// 16-register limit (a full 3x3 needs 24 registers and overflows).

var {
    @color edgeColor  = float3(0.9, 1.0, 1.0)
    @color fillA      = float3(0.05, 0.2, 0.5)
    @color fillB      = float3(0.1, 0.05, 0.3)
    @color glowColor  = float3(0.3, 0.7, 1.0)
    scale             = 4.0
    speed             = 0.2
    edgeWidth         = 0.08
}

def cell_d2(uvx : float; uvy : float; ncx : float; ncy : float) : float {
    let h  = frac(sin(ncx * 127.1 + ncy * 311.7) * 43758.5)
    let h2 = frac(sin(ncx * 269.5 + ncy * 183.3) * 43758.5)
    let dx = ncx + h  - uvx
    let dy = ncy + h2 - uvy
    return dx * dx + dy * dy
}

[pixel_shader]
def voronoi(inp : PbrInput) {
    let t    = g_Time * speed
    let uvs  = inp.uv * scale + float2(sin(t) * 0.3, cos(t * 0.7) * 0.3)
    let uvx  = uvs.x
    let uvy  = uvs.y
    // nearest lattice point -> the 4 cells touching it form a pixel-centered 2x2
    let corner = floor(uvs + float2(0.5, 0.5))
    let cx   = corner.x
    let cy   = corner.y

    let d0 = cell_d2(uvx, uvy, cx - 1.0, cy - 1.0)
    let d1 = cell_d2(uvx, uvy, cx,       cy - 1.0)
    let d2 = cell_d2(uvx, uvy, cx - 1.0, cy      )
    let d3 = cell_d2(uvx, uvy, cx,       cy      )
    let md = min(min(d0, d1), min(d2, d3))

    let dist  = sqrt(md)
    let edge  = smooth_step(edgeWidth, edgeWidth * 3.0, dist)
    let glow  = smooth_step(edgeWidth * 2.0, 0.0, dist)
    let fill  = lerp(fillA, fillB, noise(float2(cx + t * 0.1, cy + t * 0.07)))
    let col   = lerp(edgeColor, fill, edge)

    return PbrOutput(
        albedo           = col,
        emission         = col + glowColor * glow,
        emissionStrength = 1.0,
        metalness        = 0.0,
        roughness        = 1.0,
        ao               = 1.0
    )
}
