require engine.render.shader_dsl

// Demonstrates sample_scene_depth on a 3D object:
// reads the depth behind the object's surface to compute
// edge intersection thickness -- like soft particles / water foam.

var {
    @color shallowColor = float3(0.3, 0.7, 1.0)
    @color deepColor    = float3(0.0, 0.2, 0.5)
    @color edgeColor    = float3(0.8, 1.0, 1.0)
    softDepth           = 1.5    // world-unit depth range for soft edge
    edgeThreshold       = 0.25
    opacity             = 0.7
}

def depth_thickness(screen_uv : float2; surface_depth : float) : float {
    let scene_depth = sample_scene_depth(screen_uv)
    return clamp((scene_depth - surface_depth) / softDepth, 0.0, 1.0)
}

[pixel_shader]
def depth_fog_3d(inp : PbrInput) {
    let surfaceDepth = length(inp.worldPos - g_CameraPosition)
    let thickness    = depth_thickness(inp.screenPos, surfaceDepth)

    let edgeMask = smooth_step(edgeThreshold, edgeThreshold + 0.15, thickness) * smooth_step(1.0, 0.7, thickness)

    let bodyColor = lerp(shallowColor, deepColor, thickness)
    let color     = lerp(bodyColor, edgeColor, edgeMask)

    let f = fresnel(2.0, inp.worldNormal, inp.viewDir)

    return PbrOutput(
        albedo = color,
        alpha  = lerp(0.05, opacity, thickness) + f * 0.2,
        emission = edgeColor * edgeMask * 0.5,
        emissionStrength = edgeMask * 0.8,
        metalness = 0.0,
        roughness = 0.1,
        ao = 1.0
    )
}
