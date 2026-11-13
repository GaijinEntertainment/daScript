require engine.render.shader_dsl


var {
    @color insideColor = float3(0.08, 0.0, 0.2)
    @color colorA      = float3(0.1, 0.3, 1.0)
    @color colorB      = float3(0.9, 0.2, 0.8)
    @color colorC      = float3(1.0, 0.85, 0.2)
    zoom               = 1.5
    cRadius            = 0.7885
    morphSpeed         = 0.25
}

let MAX_ITER  = 7
let ESCAPE_R2 = 4.0

[pixel_shader]
def julia(inp : PbrInput) {
    let c = float2(cos(g_Time * morphSpeed), sin(g_Time * morphSpeed)) * cRadius

    let z0 = (inp.uv - float2(0.5, 0.5)) * float2(2.0, -2.0) * zoom

    var zx   = z0.x
    var zy   = z0.y
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
