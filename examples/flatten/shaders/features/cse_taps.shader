require engine.render.shader_dsl

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
}

// Feature fixture for CSE. A fresnel-like rim term `pow(1.0 - dot(n, v), 5.0)` is spelled THREE
// times — once per output channel. It is varying (it reads the surface normal + view dir), so it
// is not a preshader candidate, but neither the general compiler nor the backend dedups it: without
// flatten the graph carries three identical pow/dot/sub chains. flatten value-numbers the body and
// shares the repeat into one `_cse_` let feeding all three channels. Run `./run.sh --das cse_taps`
// to see the single shared let.
[pixel_shader]
def cse_taps(inp : PbrInput) {
    let c = tex2d(albedo_texture, inp.uv).xyz
    let n = inp.worldNormal
    let v = inp.viewDir
    let result = float3(
        pow(1.0 - dot(n, v), 5.0) * c.x,
        pow(1.0 - dot(n, v), 5.0) * c.y,
        pow(1.0 - dot(n, v), 5.0) + c.z
    )
    return PbrOutput(
        albedo = result,
        emission = result,
        emissionStrength = 1.0,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
