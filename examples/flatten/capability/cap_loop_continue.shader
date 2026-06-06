require engine.render.shader_dsl

// Capability demo: a fixed-count loop with a RUNTIME `continue`. Where `break`
// lowers to ONE loop-scoped mask that persists across the unrolled copies, `continue`
// lowers to a PER-COPY mask: each unrolled iteration gets its own `boolConst` init,
// narrowed by that copy's runtime condition, gating only that copy's accumulation.
// The backend consumes the per-copy bool masks on the same Phase-C selector rail.

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    blurSize = 0.01
}

[shader(stage="pixel", type="pbr")]
def cap_loop_continue(inp : PbrInput) : PbrOutput {
    var acc = float3(0)
    for (i in range(4)) {
        let o = blurSize * float(i)
        let s = tex2d(albedo_texture, inp.uv + float2(o, o)).xyz
        if (s.y > 0.9) {        // runtime skip — continue past THIS copy only
            continue
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
