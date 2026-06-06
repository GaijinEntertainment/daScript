require engine.render.shader_dsl

// Capability demo (parity): a shader that factors logic into a HELPER function.
//
// The backend bans user function calls. flatten inlines `shade` into the entry,
// so this compiles to exactly the same opcode graph as cap_inlined.shader (the
// same body written by hand) — flatten's inlining is transparent.

var {
    @color baseColor   = float3(0.9, 0.7, 0.2)
    @color shadowColor = float3(0.1, 0.05, 0.2)
    lightDir = float3(0.5, 0.8, 0.3)
}

def shade(n : float3; l : float3) : float3 {
    let nDotL = saturate(dot(normalize(n), normalize(l)))
    return lerp(shadowColor, baseColor, nDotL)
}

[shader(stage="pixel", type="pbr")]
def cap_helper(inp : PbrInput) : PbrOutput {
    let lit = shade(inp.worldNormal, lightDir)
    return PbrOutput(
        albedo = float3(0),
        emission = lit,
        emissionStrength = 1.0,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
