require engine.render.shader_dsl

// Capability demo: mad/lerp re-fusion. The fold DISASSEMBLES lerp/mad into raw arithmetic so
// identities/CSE see through them, then the finishing pass re-packs every surviving `a*b + c`
// into ONE mad node (`lum * 0.5 + 0.5` and the scalar-broadcast `c * remap + bias`), and
// re-packs the hand-written lerp shape `(white - c) * t + c` into ONE lerp node — the backend
// gets single instructions where the source spelled two or three.

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
}

[pixel_shader]
def cap_mad(inp : PbrInput) : PbrOutput {
    let c = tex2d(albedo_texture, inp.uv).xyz
    let lum = dot(c, float3(0.299, 0.587, 0.114))
    let remap = lum * 0.5 + 0.5
    let tinted = c * remap + float3(0.04, 0.04, 0.04)
    let edge = (float3(1.0, 1.0, 1.0) - c) * remap + c
    return PbrOutput(
        albedo = tinted,
        emission = edge,
        emissionStrength = 1.0,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
