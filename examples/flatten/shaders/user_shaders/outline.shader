require engine.render.shader_dsl

var {
    @color baseColor = float3(0.8, 0.7, 0.6)
    outlineThreshold = 0.6
    outlineSharpness = 4.0
}

[pixel_shader]
def outline(inp : PbrInput) {
    let f = fresnel(outlineSharpness, inp.worldNormal, inp.viewDir)
    let outlineMask = step(outlineThreshold, f)

    return PbrOutput(
        albedo = baseColor * (1.0 - outlineMask),
        emission = float3(0),
        emissionStrength = 0.0,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
