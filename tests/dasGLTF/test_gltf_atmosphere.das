options gen2
require dastest/testing_boost public
require gltf/gltf_atmosphere
require math
[test]
def test_atmosphere_is_shared_and_direction_independent(t : T?){
    let params = float4(.01, 2.0, .15, -1.0)
    let a = float3(0.0, -4.0, 0.0); let b = float3(10.0, 2.0, 0.0)
    let forward = atmosphere_transmittance(a, b, params, 1.0)
    let reverse = atmosphere_transmittance(b, a, params, 1.0)
    t |> success(abs(forward - reverse) < .00001, "optical depth is independent of ray direction")
    t |> success(forward > 0.0 && forward < 1.0, "fog attenuates without leaving physical bounds")
    t |> equal(atmosphere_transmittance(a, b, float4(0.0), 1.0), 1.0, "zero-density atmosphere is neutral")
    let below = atmosphere_transmittance(float3(0.0, -4.0, 0.0), float3(10.0, -4.0, 0.0), params, 1.0)
    let above = atmosphere_transmittance(float3(0.0, 4.0, 0.0), float3(10.0, 4.0, 0.0), params, 1.0)
    t |> success(below < above, "lower layer attenuates more strongly")
}
