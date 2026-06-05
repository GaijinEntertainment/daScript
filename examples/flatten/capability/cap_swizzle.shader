require engine.render.shader_dsl

// Capability demo: a user helper call whose result is SWIZZLED.
//
// The backend bans user calls. Here the call `lighting(...)` is buried under a
// `.xyz` swizzle — flatten must recurse through the ExprSwizzle operand to reach
// and inline the call. Without that, the call would survive lowering. So this is
// the "function in the middle of swizzle stuff" case made to compile.

var {
    @color baseColor = float3(0.9, 0.7, 0.2)
    lightDir = float3(0.5, 0.8, 0.3)
}

def lighting(n : float3; l : float3) : float4 {
    let d = saturate(dot(normalize(n), normalize(l)))
    return float4(baseColor * d, 1.0)
}

[shader(stage="pixel", type="pbr")]
def cap_swizzle(inp : PbrInput) : PbrOutput {
    let rgb = lighting(inp.worldNormal, lightDir).xyz
    return PbrOutput(
        albedo = float3(0),
        emission = rgb,
        emissionStrength = 1.0,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
