require engine.render.shader_dsl

// The real thing: an escape-time Mandelbrot set.
//
// For each pixel we map UV to a complex number c, then iterate the canonical
// recurrence  z <- z^2 + c  starting from z = 0. Points that stay bounded are
// *in* the set and drawn dark; points that escape are colored by how many
// iterations it took them to leave the escape radius (smooth/continuous count).
//
// z = (x + i*y), so  z^2 = (x^2 - y^2) + i*(2*x*y).  The escape test uses the
// squared magnitude x^2 + y^2 to avoid a sqrt.


var {
    // Classic Ultra Fractal palette: deep blue -> white -> gold, black inside.
    @color insideColor = float3(0.0, 0.0, 0.0)
    @color colorA      = float3(0.0, 0.03, 0.25) // low count: deep blue
    @color colorB      = float3(0.95, 0.98, 1.0) // mid: near-white
    @color colorC      = float3(1.0, 0.7, 0.0)   // near the boundary: gold
    center             = float2(-0.745, 0.1)     // zoom target (near the boundary)
    baseZoom           = 1.3                     // half-height at zoom phase 0
    zoomSpeed          = 0.4                     // how fast we dive in / pull out
}

let MAX_ITER  = 8
let ESCAPE_R2 = 4.0

[pixel_shader]
def mandelbrot(inp : PbrInput) {
    let zoom = baseZoom * (0.55 + 0.45 * cos(g_Time * zoomSpeed))

    let c = center + (inp.uv - float2(0.5, 0.5)) * float2(2.0, -2.0) * zoom

    var zx   = 0.0
    var zy   = 0.0
    var iter = 0.0

    for (i in range(MAX_ITER)) {
        if (zx * zx + zy * zy > ESCAPE_R2) {
            break
        }
        let nx = zx * zx - zy * zy + c.x
        let ny = 2.0 * zx * zy + c.y
        zx = nx
        zy = ny
        iter += 1.0
    }

    let inside = iter > float(MAX_ITER) - 0.5

    let r2     = zx * zx + zy * zy
    let smooth = iter - log(max(r2, 1.0001)) * 0.25

    let tNorm = sqrt(saturate(smooth / float(MAX_ITER)))
    let grad  = tNorm < 0.5 ? lerp(colorA, colorB, tNorm * 2.0) : lerp(colorB, colorC, (tNorm - 0.5) * 2.0)

    let col = lerp(grad, insideColor, inside ? 1.0 : 0.0)

    return PbrOutput(
        albedo           = col,
        emission         = col,
        emissionStrength = inside ? 0.0 : 1.5,
        metalness        = 0.0,
        roughness        = 1.0,
        ao               = 1.0
    )
}
