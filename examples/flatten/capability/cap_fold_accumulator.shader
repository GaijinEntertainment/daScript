require engine.render.shader_dsl

// Capability demo: a CONSTANT accumulator folds to a single constant. The loop
// unrolls to `acc += 0.1` four times; the general compiler leaves the accumulator
// (it does no const-propagation across reassignments) and the backend has no
// const-fold, so without flatten this is a 4-node `add` chain. flatten SSA-renames the
// reassigned `acc` to single-def versions, then const-propagates the chain to one
// constant — so the graph carries ZERO `add` nodes and `result` is `base * <const>`.

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
}

[pixel_shader]
def cap_fold_accumulator(inp : PbrInput) : PbrOutput {
    let base = tex2d(albedo_texture, inp.uv).xyz
    var acc = 0.0
    for (i in range(4)) {
        acc += 0.1
    }
    let result = base * acc
    return PbrOutput(
        albedo = result,
        emission = result,
        emissionStrength = 1.0,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
