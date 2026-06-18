require engine.render.shader_dsl

// Feature fixture: native radians/degrees on the shader-graph backend. Neither has a
// graph opcode — flatten_opt lowers each to a single `mul` (`radians(x) → x * K`), and an
// adjacent const factor merges (`degrees(x) * C → x * (K*C)`). So angle math authored in
// degrees costs the backend only the multiplies it would have hand-written.
var {
    spinDegPerSec = 90.0
}

[pixel_shader]
def angle_spin(inp : PbrInput) {
    let ang = radians(g_Time * spinDegPerSec)
    let c = cos(ang)
    let s = sin(ang)
    let p = inp.uv - float2(0.5)
    let rp = float2(p.x * c - p.y * s, p.x * s + p.y * c)
    // degrees(worldNormal.y) * 0.01 → worldNormal.y * (57.29578 * 0.01) = one multiply
    let shade = degrees(inp.worldNormal.y) * 0.01
    return PbrOutput(
        albedo = float3(rp.x + 0.5, rp.y + 0.5, shade),
        roughness = 0.5
    )
}
