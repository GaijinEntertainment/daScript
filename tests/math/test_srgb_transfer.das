options gen2
require dastest/testing_boost public
require daslib/math_boost

[test]
def srgb_linear_segment_and_rgb_alpha_contract(t : T?) {
    t |> success(abs(linear_to_SRGB(.001) - .01292) < .000001)
    t |> success(abs(linear_to_SRGB(.003) - .03876) < .000001)
    t |> success(abs(linear_to_SRGB(.0031308) - .040449936) < .000001)
    t |> success(abs(linear_to_SRGB(.18) - .46135613) < .000001)
    t |> success(length(linear_to_SRGB(float3(.001, .18, 1.0)) -
        float3(.01292, .46135613, 1.0)) < .000002)
    let rgba = linear_to_SRGB(float4(.001, .18, 1.0, .37))
    t |> success(length(rgba.xyz - float3(.01292, .46135613, 1.0)) < .000002)
    t |> equal(rgba.w, .37)
    t |> success(abs(linear_to_SRGB(-.01) + .1292) < .000001)
}
