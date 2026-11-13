require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    @color glowTint = float3(1.0, 0.85, 0.5)
    threshold = 0.5
    radius = 0.04
    intensity = 3.0
}

def bright_pass(uv : float2) : float3 {
    let s = tex2d(albedo_texture, uv).xyz
    let luma = dot(s, float3(0.299, 0.587, 0.114))
    // keep only the part of the color above the threshold (bright-pass)
    return s * smooth_step(threshold, threshold + 0.15, luma)
}

[pixel_shader]
def bloom(inp : PbrInput) {
    let c = tex2d(albedo_texture, inp.uv)
    // gather the bright-pass over a wide 8-tap ring -> bright areas bleed a soft halo
    // into the dark letter and out past the sprite edge
    let d = radius
    let dc = radius * 0.7
    var acc = float3(0.0)
    for (off in [float2(-d, 0.0), float2(d, 0.0), float2(0.0, -d), float2(0.0, d),
                 float2(-dc, -dc), float2(dc, -dc), float2(-dc, dc), float2(dc, dc)]) {
        acc += bright_pass(inp.uv + off)
    }
    let bloomColor = acc * 0.125 * intensity * glowTint
    let result = c.xyz + bloomColor
    return PbrOutput(
        albedo = result,
        emission = bloomColor,
        emissionStrength = 2.0,
        alpha = max(c.w, saturate(dot(bloomColor, float3(0.333, 0.333, 0.333)))),
        alphaCutoff = 0.5,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
