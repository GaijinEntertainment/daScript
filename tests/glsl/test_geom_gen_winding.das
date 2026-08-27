options gen2
options indenting = 4

// Regression gate for triangle winding in the geometry generators.
//
// Every generator sets an outward per-vertex normal, and callers draw with
// GL_CULL_FACE / GL_BACK under the default CCW front face. So each triangle must be
// wound counter-clockwise seen from outside -- its edge cross product has to point the
// same way as its own vertex normals. A triangle that disagrees is drawn inside out:
// the lit outer surface is culled and the unlit interior shows through.
//
// Nothing checked this, and two separate defects had accumulated. gen_cylinder and
// gen_cone pushed indices in the wrong order. Independently, GenDirection.xz maps the
// generated xy shape onto xz by swapping two axes -- a mirror, determinant negative --
// which reverses the orientation of every triangle it moves, so no single index order
// could be right for all three directions at once.
//
// The audit is geometric, not a golden index list: it recomputes the winding from the
// vertices, so it keeps holding if a generator changes its tessellation.

require dastest/testing_boost public
require geometry/geom_gen
require math

// Triangles whose winding disagrees with their own vertex normals.
def private inside_out_count(frag : GeometryFragment) : int {
    var bad = 0
    for (t in range(length(frag.indices) / 3)) {
        let a = frag.vertices[frag.indices[t * 3]]
        let b = frag.vertices[frag.indices[t * 3 + 1]]
        let c = frag.vertices[frag.indices[t * 3 + 2]]
        let geo = cross(b.xyz - a.xyz, c.xyz - a.xyz)
        let outward = a.normal + b.normal + c.normal
        // A degenerate triangle or a cancelling normal sum carries no orientation.
        continue if (length(geo) < 0.000001 || length(outward) < 0.000001)
        bad++ if (dot(normalize(geo), normalize(outward)) <= 0.0)
    }
    return bad
}

[test]
def test_geom_gen_winding(t : T?) {
    for (d in [GenDirection.xy, GenDirection.xz, GenDirection.yz]) {
        t |> run("gen_cylinder {d} is wound counter-clockwise from outside") <| @(t : T?) {
            let frag <- gen_cylinder(d, 12)
            t |> success(inside_out_count(frag) == 0, "no inside-out triangles")
        }
        t |> run("gen_cone {d} is wound counter-clockwise from outside") <| @(t : T?) {
            let frag <- gen_cone(d, 12)
            t |> success(inside_out_count(frag) == 0, "no inside-out triangles")
        }
        t |> run("gen_plane {d} is wound counter-clockwise from outside") <| @(t : T?) {
            let frag <- gen_plane(d)
            t |> success(inside_out_count(frag) == 0, "no inside-out triangles")
        }
        t |> run("gen_prism {d} is wound counter-clockwise from outside") <| @(t : T?) {
            let frag <- gen_prism(d)
            t |> success(inside_out_count(frag) == 0, "no inside-out triangles")
        }
    }
    t |> run("gen_sphere is wound counter-clockwise from outside") <| @(t : T?) {
        let frag <- gen_sphere(16, 8, false)
        t |> success(inside_out_count(frag) == 0, "no inside-out triangles")
    }
    t |> run("gen_cube is wound counter-clockwise from outside") <| @(t : T?) {
        let frag <- gen_cube()
        t |> success(inside_out_count(frag) == 0, "no inside-out triangles")
    }
}
