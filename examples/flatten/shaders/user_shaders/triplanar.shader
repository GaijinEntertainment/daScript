require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    @color tint = float3(1.0, 1.0, 1.0)
    texScale = 1.0
    blendSharpness = 4.0
}

[pixel_shader]
def triplanar(inp : PbrInput) {
    let p = inp.worldPos * texScale

    let an = pow(abs(inp.worldNormal), float3(blendSharpness))
    let sumW = an.x + an.y + an.z
    let w = an / float3(sumW)

    var blended = float4(0.0, 0.0, 0.0, 0.0)
    for (uv, weight in [p.zy, p.xz, p.xy], [w.x, w.y, w.z]) {
        blended += tex2d(albedo_texture, uv) * weight
    }

    return PbrOutput(
        albedo = blended.xyz * tint,
        alpha = blended.w,
        alphaCutoff = 0.1,
        metalness = 0.0,
        roughness = 0.85,
        emission = float3(0),
        emissionStrength = 0.0,
        ao = 1.0
    )
}
