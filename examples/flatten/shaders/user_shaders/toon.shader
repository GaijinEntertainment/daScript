require engine.render.shader_dsl

var {
    @color baseColor = float3(0.6, 0.2, 0.8)
    @color shadowColor = float3(0.15, 0.05, 0.25)
    @color rimColor = float3(1.0, 0.7, 1.0)
    lightDir = float3(0.5, 0.8, 0.3)
    bands = 3.0
    rimStart = 0.85
    rimEnd = 0.98
}

[pixel_shader]
def toon(inp : UnlitInput) : UnlitOutput {
    let nDotL = saturate(dot(normalize(inp.worldNormal), normalize(lightDir)))

    let stepped = floor(nDotL * bands) / bands

    let lit = lerp(shadowColor, baseColor, stepped)

    let f = fresnel(1.0, inp.worldNormal, inp.viewDir)
    let rimMask = smooth_step(rimStart, rimEnd, f)

    let outColor = lerp(lit, rimColor, rimMask)

    let daylight = saturate(g_LightDirection.y)
    return UnlitOutput(
        color = outColor
    )
}
