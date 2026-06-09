require engine.render.shader_dsl

// Capability demo: a fixed-count loop with a RUNTIME `break` (the raymarch early-out
// idiom). The backend bans `ExprFor` and has no jump primitive, so `break` cannot be
// expressed directly. flatten unrolls the loop and lowers `break` to a loop-scoped
// live mask that persists across the unrolled copies: once a copy's runtime condition
// fires, the mask goes false and every later copy's accumulation is predicated off via
// a `?:` select. The backend consumes those selects (ExprOp3 -> Select, Phase B) plus
// the stored bool mask (a mutable accumulator on the 0/1-selector rail, Phase C), so a
// runtime break becomes expressible here.

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    blurSize = 0.01
}

[pixel_shader]
def cap_loop_break(inp : PbrInput) : PbrOutput {
    var acc = float3(0)
    for (i in range(4)) {
        let o = blurSize * float(i)
        let s = tex2d(albedo_texture, inp.uv + float2(o, o)).xyz
        if (s.x > 0.9) {        // runtime early-out — break across the unrolled copies
            break
        }
        acc += s
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
