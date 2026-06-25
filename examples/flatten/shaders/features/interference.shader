require engine.render.shader_dsl

// Demonstrates the flatten producer-vectorizer: three sine waves at different
// frequencies/directions are summed INLINE (no temp vars), so the lanes reach the
// hadd fold as direct `sin(...)` calls. The fold pulls the shared `sin` out over the
// packed args — `sin(a)+sin(b)+sin(c)` becomes `hadd(sin(float3(a,b,c)))` — collapsing
// three `sin` nodes into one width-free `sin(float3)` (a width-3 hadd that would
// otherwise be node-neutral turns into a -2 win).

var {
    freqA = 12.0
    freqB = 18.0
    freqC = 7.0
    speed = 1.5
    tint = float3(0.2, 0.5, 0.9)
}

[pixel_shader]
def interference(inp : PbrInput) {
    let t = g_Time * speed
    let uv = inp.uv
    // parens keep the multi-line sum a single expression (a bare multi-line `let` whose
    // continuation lines start with `+` would parse as separate dropped statements)
    let wave = (sin(uv.x * freqA + t)
              + sin(uv.y * freqB - t)
              + sin((uv.x + uv.y) * freqC + t * 0.5))
    let h = wave * 0.1666667 + 0.5
    let col = tint * h
    return PbrOutput(
        albedo = col,
        emission = col * 0.3,
        emissionStrength = 0.4,
        metalness = 0.0,
        roughness = 0.8,
        ao = 1.0
    )
}
