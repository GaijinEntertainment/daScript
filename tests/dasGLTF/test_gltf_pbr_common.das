options gen2
options indenting = 4

require dastest/testing_boost public
require gltf/gltf_pbr_common
require math

def private near(t : T?; got, want, eps : float) {
    t |> success(abs(got - want) < eps, "expected {want}, got {got}")
}

def private near3(t : T?; got, want : float3; eps : float) {
    t |> success(length(got - want) < eps, "expected {want}, got {got}")
}

[test]
def test_skin_point(t : T?) {
    t |> run("identity palette returns the point") @(t : T?) {
        let m = identity4x4()
        let p = float4(1., 2., 3., 1.)
        t |> equal(gltf_skin_point(m, m, m, m, float4(0.25, 0.25, 0.25, 0.25), p), p)
    }
    t |> run("weights blend translated points") @(t : T?) {
        var ta = identity4x4()
        ta[3] = float4(2., 0., 0., 1.)
        var tb = identity4x4()
        tb[3] = float4(0., 4., 0., 1.)
        let p = float4(0., 0., 0., 1.)
        // 0.5/0.5 blend of the two translations = their average
        let blended = gltf_skin_point(ta, tb, identity4x4(), identity4x4(), float4(0.5, 0.5, 0., 0.), p)
        t |> equal(blended, float4(1., 2., 0., 1.))
    }
    t |> run("w=0 directions ignore translation") @(t : T?) {
        var ta = identity4x4()
        ta[3] = float4(5., 6., 7., 1.)
        let n = float4(0., 0., 1., 0.)
        t |> equal(gltf_skin_point(ta, ta, ta, ta, float4(1., 0., 0., 0.), n), n)
    }
}

[test]
def test_brdf_terms(t : T?) {
    let n = float3(0., 0., 1.)
    t |> run("fresnel at grazing/normal incidence") @(t : T?) {
        let f0 = float3(0.04, 0.04, 0.04)
        near3(t, gltf_fresnel_schlick(1.0, f0), f0, 1e-6)                     // head-on -> f0
        near3(t, gltf_fresnel_schlick(0.0, f0), float3(1., 1., 1.), 1e-6)     // grazing -> 1
    }
    t |> run("ggx distribution peaks at 1/pi for roughness 1") @(t : T?) {
        near(t, gltf_ggx_distribution(n, n, 1.0), 1.0 / 3.14159265, 1e-5)
    }
    t |> run("smith geometry is in (0, 1]") @(t : T?) {
        let v = normalize(float3(0.3, 0.2, 1.))
        let l = normalize(float3(-0.4, 0.1, 1.))
        let g = gltf_ggx_geometry_smith(n, v, l, 0.5)
        t |> success(g > 0.0 && g <= 1.0, "geometry term out of range")
    }
    t |> run("direct term is black when light is behind") @(t : T?) {
        let v = float3(0., 0., 1.)
        let l = float3(0., 0., -1.)   // below the surface -> ndotl = 0
        near3(t, gltf_pbr_direct(float3(1., 1., 1.), 0.0, 0.5, n, v, l, float3(3., 3., 3.)), float3(0., 0., 0.), 1e-6)
    }
}

[test]
def test_shading_helpers(t : T?) {
    t |> run("ambient occlusion strength blend") @(t : T?) {
        let albedo = float3(1., 1., 1.)
        let ambient = float3(0.2, 0.2, 0.2)
        near3(t, gltf_pbr_ambient(albedo, ambient, 1.0, 1.0), ambient, 1e-6)          // unoccluded
        near3(t, gltf_pbr_ambient(albedo, ambient, 0.0, 1.0), float3(0., 0., 0.), 1e-6) // fully occluded, full strength
        near3(t, gltf_pbr_ambient(albedo, ambient, 0.0, 0.0), ambient, 1e-6)          // strength 0 disables occlusion
    }
    t |> run("perturb_normal falls back without a tangent frame") @(t : T?) {
        let n = float3(0., 0., 1.)
        let bent = gltf_perturb_normal(n, float3(0., 0., 0.), 1.0, float3(1., 0., 0.), 1.0)
        t |> equal(bent, n)
    }
    t |> run("perturb_normal with a flat sample keeps the normal") @(t : T?) {
        let n = float3(0., 0., 1.)
        let tangent = float3(1., 0., 0.)
        near3(t, gltf_perturb_normal(n, tangent, 1.0, float3(0.5, 0.5, 1.0), 1.0), n, 1e-4)
    }
    t |> run("srgb encode endpoints") @(t : T?) {
        near3(t, gltf_srgb_encode(float3(0., 0., 0.)), float3(0., 0., 0.), 1e-6)
        near3(t, gltf_srgb_encode(float3(1., 1., 1.)), float3(1., 1., 1.), 1e-6)
    }
}

[test]
def test_environment_helpers(t : T?) {
    t |> run("equirect uv poles and seam") @(t : T?) {
        near(t, gltf_equirect_uv(float3(0., 1., 0.)).y, 0.0, 1e-5)     // straight up -> top row
        near(t, gltf_equirect_uv(float3(0., -1., 0.)).y, 1.0, 1e-5)    // straight down -> bottom row
        near(t, gltf_equirect_uv(float3(1., 0., 0.)).x, 0.5, 1e-5)     // +x -> mid-longitude
        near(t, gltf_equirect_uv(float3(0., 0., 1.)).x, 0.75, 1e-5)    // +z -> quarter turn
    }
    t |> run("reflect_dir mirrors the view about the normal") @(t : T?) {
        let n = float3(0., 0., 1.)
        near3(t, gltf_reflect_dir(n, n), n, 1e-6)                              // head-on stays put
        let v = normalize(float3(1., 0., 1.))
        near3(t, gltf_reflect_dir(n, v), normalize(float3(-1., 0., 1.)), 1e-5) // 45 degrees flips x
    }
    t |> run("fresnel_roughness bounds") @(t : T?) {
        let f0 = float3(0.04, 0.04, 0.04)
        near3(t, gltf_fresnel_roughness(1.0, f0, 0.0), f0, 1e-6)                       // head-on -> f0
        near3(t, gltf_fresnel_roughness(0.0, f0, 0.0), float3(1., 1., 1.), 1e-6)       // smooth grazing -> 1
        near3(t, gltf_fresnel_roughness(0.0, f0, 1.0), f0, 1e-6)                       // fully rough can't exceed f0
    }
    t |> run("occlusion factor strength blend") @(t : T?) {
        near(t, gltf_occlusion_factor(1.0, 1.0), 1.0, 1e-6)
        near(t, gltf_occlusion_factor(0.0, 1.0), 0.0, 1e-6)
        near(t, gltf_occlusion_factor(0.0, 0.0), 1.0, 1e-6)
    }
}

[test]
def test_shadow_pcf_subtexel_coverage(t : T?) {
    let left = gltf_shadow_pcf_weights(0.0)
    let middle = gltf_shadow_pcf_weights(0.5)
    let right = gltf_shadow_pcf_weights(1.0)
    near(t, left.x + left.y + left.z + left.w, 1.0, 1e-6)
    near(t, middle.x + middle.y + middle.z + middle.w, 1.0, 1e-6)
    near(t, left.z + left.w, 1.0 / 3.0, 1e-6)
    near(t, middle.z + middle.w, 0.5, 1e-6)
    near(t, right.z + right.w, 2.0 / 3.0, 1e-6)
    near(t, right.z + right.w, left.y + left.z + left.w, 1e-6)
    let coverage = float4(0.0, 1.0, 0.0, 1.0)
    for (i in range(11)) {
        let f = float(i) / 10.0
        let w = gltf_shadow_pcf_weights(f)
        let explicit_pcf = (lerp(coverage.x, coverage.y, f) +
            lerp(coverage.y, coverage.z, f) + lerp(coverage.z, coverage.w, f)) / 3.0
        near(t, dot(w, coverage), explicit_pcf, 1e-6)
    }
}

[test]
def test_shadow_receiver_plane_pcf(t : T?) {
    let slope = float2(0.31, -0.24)
    let dx = float3(0.001, 0.0003, dot(slope, float2(0.001, 0.0003)))
    let dy = float3(-0.0002, 0.001, dot(slope, float2(-0.0002, 0.001)))
    let gradient = gltf_shadow_receiver_gradient(dx, dy)
    t |> success(length(gradient - slope) < 0.00001, "recover the geometric depth slope")
    let rotated = gltf_shadow_receiver_gradient(dy * 2.0, -dx * 0.5)
    t |> success(length(rotated - slope) < 0.00001, "camera-screen basis does not change the receiver plane")
    let p = float3(0.4123, 0.571, 0.4)
    var old_shadowed = 0
    for (y in range(-2, 3)){for (x in range(-2, 3)){
        let uv = p.xy + float2(float(x), float(y)) / 2048.0
        let plane_depth = p.z + dot(slope, uv - p.xy)
        let reference = gltf_shadow_receiver_depth(p, uv, gradient, 0.00001)
        t |> success(reference <= plane_depth, "plane must not shadow itself across the filter footprint")
        t |> success(reference > plane_depth - 0.01, "a separate occluder still casts a shadow")
        if (p.z - 0.00001 > plane_depth){old_shadowed++}
    }}
    t |> success(old_shadowed > 0, "fixture exposes the old constant-reference acne")
    t |> equal(gltf_shadow_receiver_gradient(float3(0.0), float3(0.0)), float2(0.0))
}
