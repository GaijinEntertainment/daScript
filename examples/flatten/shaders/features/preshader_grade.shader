require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    exposure = 1.2
    contrast = 1.1
    whiteBalance = float3(1.0, 0.95, 0.9)
}

// Feature fixture for preshader extraction. A small colour grade whose tuning is computed entirely
// from material props (exposure / contrast / whiteBalance) — UNIFORM, identical for every pixel —
// but written inline in a per-pixel chain:
//   • `gain  = pow(2.0, exposure - 1.0)`            (exposure stop -> linear gain)
//   • `pivot = 0.5 * contrast + 0.25 * (1.0 - contrast)`
// The general compiler evaluates both per-pixel (the props are runtime); flatten colours them
// uniform and hoists each to a top-of-body `_preshader_` let the backend routes to the per-draw
// preshader, leaving only the `c`-dependent arithmetic per-pixel. Run `./run.sh --das
// preshader_grade` to see the `_preshader_` group gathered on top.
[pixel_shader]
def preshader_grade(inp : PbrInput) {
    let c = tex2d(albedo_texture, inp.uv).xyz
    let gain = pow(2.0, exposure - 1.0)
    let pivot = 0.5 * contrast + 0.25 * (1.0 - contrast)
    let graded = (c * gain - float3(pivot)) * contrast + float3(pivot)
    let result = graded * whiteBalance
    return PbrOutput(
        albedo = result,
        emission = result,
        emissionStrength = 1.0,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
