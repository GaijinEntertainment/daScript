options gen2
require opengl/opengl_gen public
require math

def fracture_vertex(point : float3; seed : float) : float3 {
    let variation = sin(dot(point, float3(12.3, 8.7, 13.7)) + seed * 1.9)
    var shaped = point * (0.83 + variation * 0.15)
    shaped.x += shaped.z * 0.20
    shaped.y += sin(shaped.z * 7.0 + seed) * 0.11
    shaped.z = floor((shaped.z + 1.0) * 5.0) / 5.0 - 1.0
    return clamp(shaped, float3(-1.0), float3(1.0))
}

def fractured_outcrop(seed : float) : GeometryFragment {
    var inscope source <- gen_sphere(10, 6, false)
    var result = GeometryFragment(prim = GeometryFragmentType.triangles, bmin = float3(-1.0), bmax = float3(1.0))
    result.vertices |> reserve(length(source.indices))
    result.indices |> reserve(length(source.indices))
    for (i in range(length(source.indices) / 3)) {
        let a = fracture_vertex(source.vertices[source.indices[i * 3]].xyz, seed)
        let b = fracture_vertex(source.vertices[source.indices[i * 3 + 1]].xyz, seed)
        let c = fracture_vertex(source.vertices[source.indices[i * 3 + 2]].xyz, seed)
        let cross_normal = cross(b - a, c - a)
        continue if (length(cross_normal) < 0.00001)
        let normal = normalize(cross_normal)
        let first = length(result.vertices)
        for (point in fixed_array(a, b, c)) {
            result.vertices |> push(GeometryPreviewVertex(xyz = point, normal = normal, uv = point.xy))
        }
        result.indices |> push_from(fixed_array(first, first + 1, first + 2))
    }
    return <- result
}
