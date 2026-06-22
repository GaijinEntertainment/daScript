require engine.render.shader_dsl

// Fractal Brownian Motion (fBm) — 6 octaves of noise, each at double frequency
// and half amplitude. Written as explicit octave additions (no loop) so the
// shader compiler can resolve every expression statically.
// Domain-warping: UV is displaced by fBm itself before the final sample,
// creating the characteristic "folded" fractal look.

var {
    @color colorA  = float3(0.05, 0.0, 0.15)
    @color colorB  = float3(0.4, 0.1, 0.8)
    @color colorC  = float3(1.0, 0.5, 0.1)
    scale          = 3.0
    speed          = 0.12
    warpStrength   = 0.6
}

def fbm(p : float2) : float {
    return noise(p) * 0.5 + noise(p * 2.1) * 0.25 + noise(p * 4.41) * 0.125 + noise(p * 9.261) * 0.0625 + noise(p * 19.448) * 0.03125 + noise(p * 40.841) * 0.015625
}

[pixel_shader]
def mandelbrot(inp : PbrInput) {
    let t  = g_Time * speed
    let uv = inp.uv * scale + float2(t, t * 0.3)

    let wx   = fbm(uv + float2(1.7, 9.2))
    let wy   = fbm(uv + float2(8.3, 2.8))
    let n    = fbm(uv + float2(wx, wy) * warpStrength)

    let c = n < 0.5 ? lerp(colorA, colorB, n * 2.0) : lerp(colorB, colorC, (n - 0.5) * 2.0)

    return PbrOutput(
        albedo           = c,
        emission         = c * n,
        emissionStrength = n * 1.5,
        metalness        = 0.0,
        roughness        = 1.0,
        ao               = 1.0
    )
}
