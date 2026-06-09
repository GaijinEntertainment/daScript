require engine.render.shader_dsl

// Capability lock: boolean algebraic identities never reach the backend. `true && c`
// / `false || c` carry a const operand the typer does NOT fold (it only folds
// fully-const ops), so the SOURCE keeps the `&&` / `||`; the backend would otherwise
// emit a genuine `and` / `or` node. flatten folds them to the bare comparison, so the
// graph carries the compares + selects and ZERO `and` / `or`. (For this no-early-return
// shape the lowering's mask-collapse folds it; the post-infer FoldVisitor is the
// durable home and additionally covers early-return bodies — see test_flatten_const.das.)

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
}

[pixel_shader]
def cap_fold_bool(inp : PbrInput) : PbrOutput {
    let base = tex2d(albedo_texture, inp.uv).xyz
    let bright = true && (inp.uv.x > 0.5)     // `true &&` folds away -> just the compare
    let dim = false || (inp.uv.y > 0.5)       // `false ||` folds away -> just the compare
    let result = bright ? (dim ? base : base * 0.5) : base * 0.25
    return PbrOutput(
        albedo = result,
        emission = result,
        emissionStrength = 1.0,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
