require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    @color tint = float3(1.0, 1.0, 1.0)
    texScale = 1.0
    blendSharpness = 4.0
}

[shader(stage="pixel", type="pbr")]
def triplanar(inp : PbrInput) : PbrOutput {
    let p = inp.worldPos * texScale

    let uvX = p.zy
    let uvY = p.xz
    let uvZ = p.xy

    let cX = tex2d(albedo_texture, uvX)
    let cY = tex2d(albedo_texture, uvY)
    let cZ = tex2d(albedo_texture, uvZ)

    let an = pow(abs(inp.worldNormal), float3(blendSharpness))
    let sumW = an.x + an.y + an.z
    let w = an / float3(sumW)

    let blended = cX * w.x + cY * w.y + cZ * w.z

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
