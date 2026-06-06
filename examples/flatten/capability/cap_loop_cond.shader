require engine.render.shader_dsl

// Capability demo: a fixed-count loop whose body is gated on the LOOP VAR only
// (`i >= 2`). After unrolling, each copy's condition is a compile-time constant.
// The typer folds the comparison and collapses the select, but that leaves a
// self-referential `acc = acc` for the false iterations — which the dataflow IR
// builder cannot resolve. flatten's post-infer fold (flatten_fold) collapses the
// const selects and drops those pure self-assigns, so the loop-var-gated
// accumulate compiles down to just the surviving (true-iteration) primitive adds.

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    blurSize = 0.01
}

[shader(stage="pixel", type="pbr")]
def cap_loop_cond(inp : PbrInput) : PbrOutput {
    var acc = float3(0)
    for (i in range(4)) {
        if (i >= 2) {
            let o = blurSize * float(i)
            acc += tex2d(albedo_texture, inp.uv + float2(o, o)).xyz
        }
    }
    return PbrOutput(
        albedo = acc,
        emission = acc,
        emissionStrength = 1.0,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
