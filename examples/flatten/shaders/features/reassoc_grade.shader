require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    exposure = 1.0
    tint = 0.0
}

// Feature fixture for constant reassociation. A small colour grade whose constants the author
// wrote in natural reading order, scattered around the runtime `exposure` / `tint` props:
//   • additive: `float3(0.05) + c*exposure + float3(0.05)` — the two black-lift consts gather
//     into `c*exposure + float3(0.1)` (one vector add, not two).
//   • multiplicative: `3.0 * lifted * 0.5` — the const factors gather into `lifted * 1.5`
//     (one multiply by a single folded factor, not two).
// The typer folds neither (the consts are non-adjacent and the props are runtime), and the
// backend has no const-fold; flatten reassociates under fast-math. Run
// `./run.sh --das reassoc_grade` to see the gathered form.
[pixel_shader]
def reassoc_grade(inp : PbrInput) {
    let c = tex2d(albedo_texture, inp.uv).xyz
    let lifted = float3(0.05) + c * exposure + float3(0.05)
    let graded = 3.0 * lifted * 0.5
    let result = graded + float3(tint)
    return PbrOutput(
        albedo = result,
        emission = result,
        emissionStrength = 1.0,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
