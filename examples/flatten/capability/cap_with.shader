require engine.render.shader_dsl

// Capability demo: a `with` scope over the input struct.
//
// `with (inp) { ... }` lets the body read `worldNormal` unqualified. The backend
// bans ExprWith, but at flatten's patch stage `with` is pure infer-time name
// resolution: the body's accesses are already bound to inp's fields and the node
// simulates to its body alone, so flatten unwraps it (like a bare block) and the
// shader compiles.

var {
    @color baseColor = float3(0.9, 0.7, 0.2)
    lightDir = float3(0.5, 0.8, 0.3)
}

[shader(stage="pixel", type="pbr")]
def cap_with(inp : PbrInput) : PbrOutput {
    var lit = float3(0)
    with (inp) {
        let d = saturate(dot(normalize(worldNormal), normalize(lightDir)))
        lit = baseColor * d
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
