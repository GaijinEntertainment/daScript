options gen2
require dastest/testing_boost public
require lp_render
require strings

[test]
def test_camera_and_normals(suite : T?) {
    suite |> run("HDR diagnostic rejects out-of-range half-float pixels") @(t : T?) {
        t |> success(hdr_pixel_valid(float4(1.0, 6.0, 0.0, 1.0)))
        t |> success(!hdr_pixel_valid(float4(65505.0, 0.0, 0.0, 1.0)))
    }
    suite |> run("sky interpolation preserves the center viewing direction") @(t : T?) {
        let forward = float3(0.0, 1.0, 0.0)
        let right = float3(1.0, 0.0, 0.0)
        let up = float3(0.0, 0.0, 1.0)
        let scale = float2(0.9, 0.6)
        let a = camera_ray(float2(-1.0, -1.0), scale, forward, right, up)
        let b = camera_ray(float2(3.0, -1.0), scale, forward, right, up)
        let c = camera_ray(float2(-1.0, 3.0), scale, forward, right, up)
        let center = normalize(a * 0.5 + b * 0.25 + c * 0.25)
        t |> success(length(center - forward) < 0.00001)
    }
    suite |> run("stretched surfaces use inverse-transpose normals") @(t : T?) {
        let scale = float3(2.0, 1.0, 1.0)
        let model = compose(float3(0.0), float4(0.0, 0.0, 0.0, 1.0), scale)
        let normal = transformed_normal(normalize(float3(1.0, 1.0, 0.0)), scale, model)
        t |> success(length(normal - normalize(float3(0.5, 1.0, 0.0))) < 0.00001)
    }
}

[test]
def test_twilight(suite : T?) {
    suite |> run("cubemap explicit LOD reaches the generated shader") @(t : T?) {
        t |> success(find(fs_latch_surface`shader_text, "textureLod") >= 0)
        t |> success(find(fs_latch_surface`shader_text, "samplerCube") >= 0)
    }
    suite |> run("storm flashes are bounded, intermittent and disabled by accessibility setting") @(t : T?) {
        var active = 0
        for (i in range(2000)) {
            let time = float(i) * 0.1
            let flash = storm_flash(time, false)
            t |> success(flash >= 0.0 && flash <= 0.66)
            t |> equal(storm_flash(time, true), 0.0)
            if (flash > 0.01) { active++ }
        }
        t |> success(active > 0 && active < 100)
    }
    suite |> run("hanging lamp stays tethered and changes position") @(t : T?) {
        t |> success(length(hanging_lamp(0.0) - hanging_lamp(2.0)) > 0.05)
        for (i in range(100)) {
            let position = hanging_lamp(float(i))
            t |> success(length(position.xy - float2(-7.7, 2.6)) < 0.5)
            t |> success(position.z >= 3.25 && position.z < 3.35)
        }
    }
    suite |> run("robot inspection light follows Patch heading and generator light follows power") @(t : T?) {
        patch_heading = PI * 0.5
        world.power = false
        update_yard_lights(0.0)
        t |> success(lp_light_direction[1].x > 0.9)
        let offline = lp_light_color[7]
        world.power = true
        update_yard_lights(0.0)
        t |> success(lp_light_color[7].y > offline.y)
    }
}

[test]
def test_beveled_geometry(suite : T?) {
    suite |> run("rounded box has finite unit normals, outward triangles and exact bounds") @(t : T?) {
        var mesh <- gen_beveled_box(float3(2.0, 1.0, 0.5), 0.12, 3)
        t |> equal(length(mesh.vertices), 384)
        t |> equal(length(mesh.indices), 1764)
        for (vertex in mesh.vertices) {
            t |> success(abs(length(vertex.normal) - 1.0) < 0.0001)
            t |> success(abs(vertex.xyz.x) <= 2.00001 && abs(vertex.xyz.y) <= 1.00001 && abs(vertex.xyz.z) <= 0.50001)
        }
        for (i in range(length(mesh.indices) / 3)) {
            let a = mesh.vertices[mesh.indices[i * 3]]
            let b = mesh.vertices[mesh.indices[i * 3 + 1]]
            let c = mesh.vertices[mesh.indices[i * 3 + 2]]
            t |> success(dot(cross(b.xyz - a.xyz, c.xyz - a.xyz), a.normal + b.normal + c.normal) > 0.0)
        }
        t |> equal(mesh.bmin, float3(-2.0, -1.0, -0.5))
        t |> equal(mesh.bmax, float3(2.0, 1.0, 0.5))
    }
    suite |> run("zero bevel returns a scaled box and oversized radii stay valid") @(t : T?) {
        let flat <- gen_beveled_box(float3(2.0, 1.0, 0.5), 0.0)
        t |> equal(length(flat.vertices), 24)
        t |> equal(flat.bmax, float3(2.0, 1.0, 0.5))
        var rounded <- gen_beveled_box(float3(0.0, -1.0, 2.0), 100.0, 0)
        for (vertex in rounded.vertices) { t |> success(abs(length(vertex.normal) - 1.0) < 0.001) }
    }
}
