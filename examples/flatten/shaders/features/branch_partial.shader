require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
}

// Feature fixture for PARTIALLY-assigned locals under a branch: `a` is written only on the then
// path, `b` only on the else path — each keeps its zero default on the other path (daslang
// zero-inits locals). flatten lowers the branch to predicated selects over the zero-init temps
// (`a = P ? red : a`), so the merged `a + b` picks exactly one color per pixel with no branch.
// Run `./run.sh --das branch_partial` to see the select pair.
[pixel_shader]
def branch_partial(inp : PbrInput) {
    let c = tex2d(albedo_texture, inp.uv).xyz
    var a : float3
    var b : float3
    if (inp.uv.x > 0.5) {
        a = float3(1.0, 0.0, 0.0)
    } else {
        b = float3(0.0, 1.0, 0.0)
    }
    let result = c * (a + b)
    return PbrOutput(
        albedo = result,
        emission = result,
        emissionStrength = 1.0,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
