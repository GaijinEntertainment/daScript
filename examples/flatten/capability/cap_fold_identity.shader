require engine.render.shader_dsl

// Capability demo: scalar algebraic identities on a runtime material prop. The
// general compiler leaves `gain * 1.0` / `+ 0.0` (gain is a runtime global), and
// the backend has no const-fold pass — flatten's fold removes them, so the emitted
// graph carries no identity multiply/add nodes and matches the hand-minimal
// cap_fold_identity_ref.shader (a parity check, like cap_helper == cap_inlined).

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    gain = 1.0
}

[pixel_shader]
def cap_fold_identity(inp : PbrInput) : PbrOutput {
    let base = tex2d(albedo_texture, inp.uv).xyz
    let k = gain * 1.0 + 0.0
    let result = base * k
    return PbrOutput(
        albedo = result,
        emission = result,
        emissionStrength = 1.0,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
