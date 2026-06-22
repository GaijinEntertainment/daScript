require engine.render.shader_dsl

// Demonstrates both sample_scene_color and sample_scene_depth:
// refracts the scene behind a glass-like surface using normal-based UV distortion,
// and uses scene depth to tint deep regions with the glass color.

var {
    @color tintColor    = float3(0.15, 0.55, 0.9)
    @color edgeColor    = float3(0.6, 0.9, 1.0)
    refractStrength     = 0.04
    tintDepth           = 4.0     // depth at which full tintColor is applied
    fresnelPower        = 3.0
    noiseScale          = 3.0
    noiseSpeed          = 0.3
}

def refract_offset(normal : float3; right : float3; up : float3; strength : float; t : float; p : float3; scale : float) : float2 {
    let ripple = noise(p * scale + float3(t, 0.0, t * 0.5)) * 0.5
    let nx = dot(normal, right) + ripple * 0.3
    let ny = dot(normal, up)    + ripple * 0.3
    return float2(nx, ny) * strength
}

[pixel_shader]
def scene_refraction(inp : PbrInput) {
    let t = g_Time * noiseSpeed

    let offset = refract_offset(
        inp.worldNormal, g_CameraRight, g_CameraUp,
        refractStrength, t, inp.worldPos, noiseScale)

    let refractUV  = inp.screenPos + offset
    let sceneColor = sample_scene_color(refractUV)

    let surfaceDepth = length(inp.worldPos - g_CameraPosition)
    let sceneDepth   = sample_scene_depth(inp.screenPos)
    let depthBehind  = clamp((sceneDepth - surfaceDepth) / tintDepth, 0.0, 1.0)

    let tinted = lerp(sceneColor, tintColor, depthBehind * 0.6)

    let f    = fresnel(fresnelPower, inp.worldNormal, inp.viewDir)
    let edge = lerp(tinted, edgeColor, pow(f, 2.0) * 0.5)

    return PbrOutput(
        albedo = edge,
        alpha  = lerp(0.5, 0.95, f),
        emission = edgeColor * pow(f, 4.0),
        emissionStrength = 0.4,
        metalness = 0.0,
        roughness = 0.05,
        ao = 1.0
    )
}
