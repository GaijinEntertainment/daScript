require engine.render.shader_dsl

var {
    @color baseColor = float3(0.6, 0.2, 0.8)
    @color shadowColor = float3(0.15, 0.05, 0.2)
    @color specularColor = float3(1, 1, 1)
    lightDir = float3(0.5, 0.8, 0.3)
    bands = 3.0
    outlineThreshold = 0.17
    outlineSharpness = 4.0
    specularThreshold = 0.85
    specularPower = 12.0
}

[pixel_shader]
def cel_shading(inp : PbrInput) {
    let nDotL = saturate(dot(normalize(inp.worldNormal), normalize(lightDir)))
    let stepped = floor(nDotL * bands) / bands
    let cel = lerp(shadowColor, baseColor, stepped)

    let h = normalize(normalize(lightDir) + normalize(inp.viewDir))
    let nDotH = saturate(dot(normalize(inp.worldNormal), h))
    let spec = step(specularThreshold, pow(nDotH, specularPower))

    let f = fresnel(outlineSharpness, inp.worldNormal, inp.viewDir)
    let outline = step(outlineThreshold, f)
    let noOutline = 1.0 - outline

    let daylight = saturate(g_LightDirection.y)
    return PbrOutput(
        albedo = float3(0),
        emission = lerp(cel, specularColor, spec) * noOutline,
        emissionStrength = noOutline * lerp(0.1, 0.8, daylight),
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
