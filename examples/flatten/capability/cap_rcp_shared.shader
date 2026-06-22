require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    segments = 6.0
}

// A uniform divisor that is ALSO used as a multiplier in the same expression. The shared use
// forces the divisor's value to be CSE'd / hoisted into a uniform `_preshader_` let; the
// rcp-rewrite must still turn the per-pixel `x / slice` into `x * (1f / slice)`. Before the fix
// the rewrite went blind to a freshly-hoisted (un-typed) divisor and left the per-pixel division
// in — now both the reciprocal AND the multiplier ride one shared per-draw let, so the per-pixel
// graph carries NO division by the uniform. (This is the kaleidoscope `frac(angle/slice)*slice`
// shape, minimized.)
[pixel_shader]
def cap_rcp_shared(inp : PbrInput) {
    let c = tex2d(albedo_texture, inp.uv).xyz
    let slice = 6.28318 / segments              // uniform
    let folded = frac(c.x / slice) * slice      // divisor AND multiplier -> slice is CSE'd
    let result = c * folded
    return PbrOutput(
        albedo = result,
        emission = result,
        emissionStrength = 1.0,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
