require engine.render.shader_dsl

var {
    @color sphereColor = float3(0.95, 0.45, 0.2)
    @color glowColor = float3(0.1, 0.35, 0.9)
    smoothK = 0.22
    sphereRadius = 0.2
    marchDist = 2.0        // world units the ray travels through the cube over all steps
}


[pixel_shader]
def raymarch_spheres(inp : PbrInput) {
    let ro = inp.localPos
    let rd = -inp.viewDir

    let t = g_Time * 0.7
    // Centres swing widely enough that the smooth-union fills the cube to its faces.
    let c0 = float3(sin(t) * 0.32, cos(t * 1.1) * 0.28, sin(t * 0.6) * 0.22)
    let c1 = float3(cos(t * 1.3) * 0.3, sin(t * 0.7) * 0.3, sin(t) * 0.26)
    let c2 = float3(sin(t * 0.9) * 0.26, cos(t) * 0.32, cos(t * 1.2) * 0.3)

    var depth = 0.0
    var notHit = 1.0
    var hitDist = 1.0
    for (step in range(2)) {
        let p = ro + rd * depth
        var d = 2.0
        for (c in [c0, c1, c2]) {
            let ds = length(p - c) - sphereRadius
            let h = saturate(0.5 + 0.5 * ((d - ds) / smoothK))
            d = lerp(d, ds, h) + smoothK * h * (h - 1.0)
        }
        hitDist = lerp(hitDist, d, notHit)
        notHit *= step(0.003, d)
        depth += d * notHit
    }
    depth = depth / marchDist
    let hit = 1.0 - step(0.02, hitDist)
    // Cosine palette (iq-style) over march depth + time -> iridescent banding.
    let phase = depth * 5.0 + t * 1.5
    let rainbow = float3(0.5, 0.5, 0.5) + float3(0.5, 0.5, 0.5) * cos(float3(phase) + float3(0.0, 2.0, 4.0))
    let col = lerp(glowColor, rainbow * sphereColor, 1.0 - saturate(depth * 1.3))

    return PbrOutput(
        albedo = float3(0.0, 0.0, 0.0),
        emission = col * hit,
        emissionStrength = hit * 1.3,
        alpha = hit,
        alphaCutoff = 0.5,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
