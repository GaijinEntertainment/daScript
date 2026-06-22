require engine.render.shader_dsl

var {
    @color electricColor = float3(0.5, 0.8, 2.5)
    @color coreColor     = float3(1.5, 1.8, 2.5)
    @color arcColor      = float3(2.0, 2.5, 3.0)
    speed                = 8.0
    noiseScale           = 6.0
    branchThreshold      = 0.72
}

def bolt_layer(p : float3; scale : float; t : float; dir : float3) : float {
    let n1 = noise(p * scale      + dir * t)
    let n2 = noise(p * scale * 2.0 - dir * t * 0.7)
    return n1 * n2 * 2.0
}

[pixel_shader]
def electric_3d(inp : PbrInput) {
    let t = g_Time * speed

    let b0 = bolt_layer(inp.worldPos, noiseScale,        t, float3(1.0, 0.0, 0.0))
    let b1 = bolt_layer(inp.worldPos, noiseScale * 1.5,  t, float3(0.0, 1.0, 0.3))
    let b2 = bolt_layer(inp.worldPos, noiseScale * 0.7,  t, float3(-0.5, 0.0, 1.0))

    let bolt = step(branchThreshold, max(b0, b1))
    let arc  = step(branchThreshold + 0.08, b2) * (1.0 - bolt)
    let glow = smooth_step(0.45, branchThreshold, b0 + b1 * 0.6)

    let flicker  = sin(t * 3.0) * 0.5 + 0.5
    let flicker2 = sin(t * 7.3 + 1.1) * 0.5 + 0.5

    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        albedo = float3(0),
        emission = coreColor * bolt * flicker + arcColor * arc * flicker2 + electricColor * glow,
        emissionStrength = (bolt + arc * 0.7 + glow * 0.5) * flicker * lerp(0.5, 2.5, daylight),
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
