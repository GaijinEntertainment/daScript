require engine.render.shader_dsl

// Capability demo (parity): the hand-inlined twin of cap_helper.shader.
//
// Same property block, same math, but with `shade` written out inline. flatten
// reduces cap_helper to exactly this — the two emit the same opcode multiset.

var {
    @color baseColor   = float3(0.9, 0.7, 0.2)
    @color shadowColor = float3(0.1, 0.05, 0.2)
    lightDir = float3(0.5, 0.8, 0.3)
}

[pixel_shader]
def cap_inlined(inp : PbrInput) : PbrOutput {
    let nDotL = saturate(dot(normalize(inp.worldNormal), normalize(lightDir)))
    let lit = lerp(shadowColor, baseColor, nDotL)
    return PbrOutput(
        albedo = float3(0),
        emission = lit,
        emissionStrength = 1.0,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
