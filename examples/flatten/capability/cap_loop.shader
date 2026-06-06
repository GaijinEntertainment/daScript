require engine.render.shader_dsl

// Capability demo: a shader with a fixed-count `for` loop.
//
// The backend bans `ExprFor` outright. flatten unrolls the loop into
// straight-line accumulation (the loop var substituted by each iteration's
// constant), which the backend then accepts as a chain of `add` nodes feeding
// the accumulator. So flatten is what makes a loop expressible here.

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    blurSize = 0.01
}

[shader(stage="pixel", type="pbr")]
def cap_loop(inp : PbrInput) : PbrOutput {
    var acc = float3(0)
    for (i in range(3)) {
        let o = blurSize * float(i)
        acc += tex2d(albedo_texture, inp.uv + float2(o, o)).xyz
    }
    let result = acc * (1.0 / 3.0)
    return PbrOutput(
        albedo = result,
        emission = result,
        emissionStrength = 1.0,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
