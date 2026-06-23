require engine.render.shader_dsl

// Voronoi / cellular noise — nearest-point distance over a 3x3 grid neighbourhood.
// Uses a nested loop with a ?:-based min accumulator.

var {
    @color edgeColor  = float3(1.0, 1.0, 1.0)
    @color fillColor  = float3(0.05, 0.15, 0.4)
    @color glowColor  = float3(0.2, 0.6, 1.0)
    scale             = 5.0
    speed             = 0.3
    edgeWidth         = 0.08
}

def hash_x(cx : float; cy : float) : float {
    return frac(sin(cx * 127.1 + cy * 311.7) * 43758.5)
}
def hash_y(cx : float; cy : float) : float {
    return frac(sin(cx * 269.5 + cy * 183.3) * 43758.5)
}

def cell_dist(uvx : float; uvy : float; ncx : float; ncy : float; t : float) : float {
    let hx  = hash_x(ncx, ncy)
    let hy  = hash_y(ncx, ncy)
    let ptx = ncx + hx * 0.5 + sin(t + hx * 6.28) * 0.3
    let pty = ncy + hy * 0.5 + cos(t + hy * 6.28) * 0.3
    let dx  = ptx - uvx
    let dy  = pty - uvy
    return sqrt(dx * dx + dy * dy)
}

[pixel_shader]
def voronoi(inp : PbrInput) {
    let t    = g_Time * speed
    let uv   = inp.uv * scale
    let uvx  = uv.x
    let uvy  = uv.y
    let cell = floor(uv)
    let cx   = cell.x
    let cy   = cell.y

    var minDist = 10.0
    for (dy in range(3)) {
        for (dx in range(3)) {
            let ncx = cx + float(dx - 1)
            let ncy = cy + float(dy - 1)
            let d   = cell_dist(uvx, uvy, ncx, ncy, t)
            minDist = d < minDist ? d : minDist
        }
    }

    let edge  = smooth_step(0.0, edgeWidth, minDist)
    let glow  = smooth_step(edgeWidth * 3.0, edgeWidth, minDist)
    let col   = lerp(edgeColor, fillColor, edge) + glowColor * glow * 0.5

    return PbrOutput(
        albedo           = col,
        emission         = glowColor * glow,
        emissionStrength = glow * 1.5,
        metalness        = 0.0,
        roughness        = 1.0,
        ao               = 1.0
    )
}
