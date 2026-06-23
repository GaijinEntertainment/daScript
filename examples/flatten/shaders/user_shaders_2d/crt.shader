require engine.render.shader_dsl

// CRT monitor simulation — barrel lens distortion, RGB scanlines,
// phosphor glow (multi-tap), and flicker. Uses helper functions and loops.

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    barrelStrength = 0.05
    scanlineCount  = 120.0
    scanlineDark   = 0.6
    flickerSpeed   = 8.0
    flickerAmt     = 0.04
    glowSize       = 0.004
}

def barrel_uv(uv : float2; strength : float) : float2 {
    let c = uv * 2.0 - float2(1.0, 1.0)
    let r2 = dot(c, c)
    let distort = 1.0 + r2 * strength
    return (c * distort) * 0.5 + float2(0.5, 0.5)
}

def phosphor_glow(uv : float2; size : float) : float3 {
    var acc = float3(0.0, 0.0, 0.0)
    for (off in [float2(-1.0, 0.0), float2(0.0, 0.0), float2(1.0, 0.0),
                 float2(0.0, -1.0), float2(0.0,  1.0)]) {
        acc += tex2d(albedo_texture, uv + off * size).xyz
    }
    return acc * 0.2
}

[pixel_shader]
def crt(inp : PbrInput) {
    let distUV = barrel_uv(inp.uv, barrelStrength)

    // vignette from barrel: kill pixels outside [0,1]
    let inBounds = (step(0.0, distUV.x) * step(distUV.x, 1.0) * step(0.0, distUV.y) * step(distUV.y, 1.0))

    let base = tex2d(albedo_texture, distUV).xyz

    // RGB channel split — tiny lateral offset per channel
    let r = tex2d(albedo_texture, distUV + float2( glowSize, 0.0)).x
    let g = base.y
    let b = tex2d(albedo_texture, distUV - float2( glowSize, 0.0)).z
    let rgb = float3(r, g, b)

    // Horizontal scanlines
    let scanMask = lerp(scanlineDark, 1.0, step(0.5, frac(distUV.y * scanlineCount)))
    let scanned = rgb * scanMask

    // Phosphor glow
    let glow = phosphor_glow(distUV, glowSize * 2.0)

    // Flicker
    let flicker = 1.0 - flickerAmt * (sin(g_Time * flickerSpeed) * 0.5 + 0.5)

    let result = (scanned + glow * 0.3) * flicker * inBounds

    return PbrOutput(
        albedo = result,
        emission = glow * 0.5 * inBounds,
        emissionStrength = 0.8,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
