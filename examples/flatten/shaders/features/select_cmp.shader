require engine.render.shader_dsl

// Fixture for comparison operators, logical operators, and component-wise lerp4.
// Covers: select (ternary), cmp_gt/lt/ge/le/eq/ne, and/or, lerp4.
[pixel_shader]
def select_cmp(inp : PbrInput) {
    let x = inp.uv.x
    let y = inp.uv.y

    // cmp_gt -> oneMinus(step(a,b))
    let r = x > 0.5 ? 1.0 : 0.0
    // cmp_lt -> oneMinus(step(b,a))
    let g = x < 0.3 ? 1.0 : 0.0
    // cmp_ge -> step(b,a), cmp_le -> step(a,b); and -> mul
    let inBand = (x >= 0.25 && x <= 0.75) ? 1.0 : 0.0
    // or -> max
    let onEdge = (x < 0.1 || x > 0.9) ? 1.0 : 0.0
    // cmp_eq -> SG_OP_CMP_EQ
    let eq = (x == y) ? 1.0 : 0.0
    // cmp_ne -> oneMinus(cmp_eq)
    let ne = (x != y) ? 1.0 : 0.0
    // lerp4: component-wise t -> SG_OP_LERP (reuses scalar lerp opcode; Float t is stored broadcast)
    let blended = lerp(float4(0.0, 0.0, 0.0, 0.0), float4(1.0, 1.0, 1.0, 1.0), float4(x, y, x, y))

    return PbrOutput(
        albedo = float3(r + inBand, g + onEdge, eq),
        metalness = ne,
        roughness = blended.x
    )
}
