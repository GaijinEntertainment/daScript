require engine.render.shader_dsl

// Frost — dendritic ice crystal pattern growing from screen edges inward.
// Simulates fern-like crystals: layered domain-warped noise at multiple
// scales creates branching ridge structures with bright spines and dark
// clear patches between clusters, matching real window frost.

var {
    @color crystalColor  = float3(0.88, 0.95, 1.0)
    @color deepIceColor  = float3(0.55, 0.75, 0.95)
    freezeAmount         = 0.72    // how far inward frost grows (0=none, 1=full)
    crystalScale         = 6.0     // base frequency of crystal cells
    branchSharpness      = 14.0    // ridge sharpness — higher = finer spines
    frostOpacity         = 0.92    // max opacity of frost over scene
}

// Domain-warped ridge noise — creates branching, anisotropic structures
def ridge(p : float2; sharpness : float) : float {
    let n = noise(p)
    return pow(1.0 - abs(n * 2.0 - 1.0), sharpness)
}

def crystal_density(uv : float2) : float {
    let uvx    = uv.x
    let uvy    = uv.y
    // Layer 1: large cell structure
    let warp1x = noise(uv * crystalScale * 0.5 + float2(1.7, 0.3))
    let warp1y = noise(uv * crystalScale * 0.5 + float2(0.2, 2.1))
    let warp1  = float2(warp1x - 0.5, warp1y - 0.5)

    // Layer 2: medium branches with domain warp
    let p2    = uv * crystalScale + warp1 * 0.6
    let r2    = ridge(p2, branchSharpness)
    let r2b   = ridge(p2 + float2(0.4, 0.7), branchSharpness * 0.7)

    // Layer 3: fine sub-branches at rotated angle (~60 deg)
    let p3x   = uvx * crystalScale * 1.8 + uvy * crystalScale * 1.0 + warp1x * 0.4
    let p3y   = uvy * crystalScale * 1.8 - uvx * crystalScale * 1.0 + warp1y * 0.4
    let r3    = ridge(float2(p3x, p3y), branchSharpness * 1.4)

    // Layer 4: large-scale variation — creates clear patches between clusters
    let cluster = noise(uv * crystalScale * 0.25 + float2(3.1, 1.4))
    let clusterMask = smooth_step(0.35, 0.65, cluster)

    return (r2 * 0.5 + r2b * 0.3 + r3 * 0.2) * clusterMask
}

[pixel_shader]
def frost(inp : PostEffectInput) {
    let uv = inp.screenPos

    // Grow pulse: fast freeze-in from edges, slow melt-out — like entering frost zone
    let pulse    = frac(g_Time * 0.4)                              // 2.5 sec cycle
    let grow     = smooth_step(0.0, 0.2, pulse)                    // fast grow (0.5s)
    let melt     = smooth_step(1.0, 0.25, pulse)                   // slow melt (1.9s)
    let freeze   = freezeAmount * grow * melt                      // 0 -> full -> 0

    // Frost grows from edges inward — the animated freeze drives how far it reaches
    let d        = length(uv - float2(0.5, 0.5)) * 1.415
    let edgeMask = smooth_step(1.0 - freeze, 1.0, d)

    // Extra buildup at top edge, scaled by same pulse
    let topEdge  = smooth_step(0.35 * (1.0 - freeze), 0.0, uv.y) * 0.6 * grow * melt
    let botEdge  = smooth_step(0.65, 1.0, uv.y) * 0.4 * grow * melt
    let iceMask  = saturate(edgeMask + topEdge + botEdge)

    let crystal  = crystal_density(uv)
    let frost    = crystal * iceMask

    // Scene visible through clear patches — slight blue refraction offset
    let uvx    = uv.x
    let uvy    = uv.y
    let wn     = noise(float2(uvx * crystalScale, uvy * crystalScale))
    let offset = float2(wn - 0.5, noise(float2(uvy * crystalScale, uvx * crystalScale)) - 0.5) * 0.004 * iceMask
    let scene  = sample_scene_color(uv + offset)
    let gray   = dot(scene, float3(0.299, 0.587, 0.114))

    // Icy tint on scene under frost
    let tinted = lerp(scene, float3(gray, gray, gray) * deepIceColor, iceMask * 0.7)

    // Bright crystal ridges overlaid
    let bright = crystalColor * frost * 2.2
    let result = lerp(tinted, crystalColor, frost * frostOpacity) + bright * (1.0 - frost * 0.5)

    return PostEffectOutput(out_color = saturate(result))
}
