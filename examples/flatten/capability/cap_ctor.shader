require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
}

// Ctor lane algebra, on the two motivating shapes. The zero-lane product collapses to an
// embedded constant (lane 0 dies under the zero const lane — the sin never executes — and
// lane 1 const-folds), and the per-component adds re-vectorize into ONE vector add
// (`sin(c + float3(...))`), so the graph carries a single sin and no per-lane add chain.
[pixel_shader]
def cap_ctor(inp : PbrInput) {
    let c = tex2d(albedo_texture, inp.uv).xyz
    let zk = float2(sin(inp.uv.x) * 12.0, 13.0) * float2(0.0, 123.0)
    let rv = sin(float3(c.x + 0.1, c.y + 0.2, c.z + 0.3))
    let result = rv * 0.25 + float3(zk.x, zk.y * 0.0001, 0.0)
    return PbrOutput(
        albedo = result,
        emission = result,
        emissionStrength = 1.0,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
