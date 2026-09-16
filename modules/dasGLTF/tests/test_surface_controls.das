options gen2
require dastest/testing_boost public
require gltf/gltf_pbr
require math

[test]
def test_surface_controls_preserve_standard_defaults(t : T?){
    let renderer = GltfPbrRenderer()
    t |> equal(renderer.materialScale, float2(1.0)); t |> equal(renderer.materialWetness, 0.0); t |> equal(renderer.reflectionAmount, 0.0)
    u_material_scale = float2(1.0); u_material_wetness = 0.0; u_roughness_factor = .5
    t |> equal(gltf_surface_roughness(float4(1.0)), .5)
}
[test]
def test_optional_wetness_respects_red_mask_and_roughness_limits(t : T?){
    u_material_scale = float2(1.0); u_roughness_factor = .8; u_material_wetness = 1.0
    t |> success(abs(gltf_surface_roughness(float4(1.0)) - .16) < .0001)
    t |> equal(gltf_surface_roughness(float4(0.0, 1.0, 0.0, 1.0)), .8)
    u_material_wetness = 0.0; u_material_scale = float2(.01, 1.0)
    t |> equal(gltf_surface_roughness(float4(1.0)), .04)
    u_material_scale = float2(1.0); u_roughness_factor = 1.0
}
