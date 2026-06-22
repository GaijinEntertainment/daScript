require engine.render.shader_dsl

// Capability: a HELPER that reads its by-value vector param through single-component swizzles
// (p.x/p.y/p.z). flatten inlines the helper, and copy-prop substitutes the (value) call argument
// into the swizzle base. Before the typer folded ref2value-of-a-value, this failed to compile with
// error[30921] "can only dereference a reference". Distilled from the reported heal_3d/force_field
// shaders, whose edited helper versions added exactly this shape.

var {
    @color tint = float3(0.2, 0.6, 1.0)
    scale = 2.0
}

def axis_mix(p : float3) : float {
    return p.x * 0.5 + p.y - p.z * 0.25
}

[pixel_shader]
def cap_helper_swizzle(inp : PbrInput) {
    let m = axis_mix(inp.worldPos * scale)
    return PbrOutput(
        albedo = tint * m,
        emission = float3(0),
        emissionStrength = saturate(m),
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
