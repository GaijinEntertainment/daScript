require engine.render.shader_dsl

// Capability demo: a shader written WITH control flow (an `if`).
//
// The backend bans `ExprIfThenElse` outright — this shader does not compile
// against it directly. flatten lowers the `if` to a predicated `?:`, which the
// backend consumes as a `Select` node. So flatten is what makes this shader
// expressible at all.

var {
    @color baseColor   = float3(0.9, 0.7, 0.2)
    @color shadowColor = float3(0.1, 0.05, 0.2)
    lightDir = float3(0.5, 0.8, 0.3)
    threshold = 0.5
}

[pixel_shader]
def cap_control(inp : PbrInput) : PbrOutput {
    let nDotL = saturate(dot(normalize(inp.worldNormal), normalize(lightDir)))
    var lit = float3(0)
    if (nDotL > threshold) {
        lit = baseColor
    } else {
        lit = shadowColor
    }
    return PbrOutput(
        albedo = float3(0),
        emission = lit,
        emissionStrength = 1.0,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
