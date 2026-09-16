options gen2
module gltf_atmosphere shared public
require math
// Optical depth integrated along the ray. The lower layer is constant below
// its top and falls off exponentially above it; no screen-space depth approximation.
struct Atmosphere {
    color : float3
    density, start, height_density, height_top, height_falloff : float
}
[arch(at="../ARCHITECTURE.md#atmosphere-transmittance")]
def atmosphere_transmittance(eye, point : float3; params : float4; falloff : float) : float {
    let distance = length(point - eye)
    let a = clamp((params.w - eye.y) * falloff, -40.0, 0.0)
    let b = clamp((params.w - point.y) * falloff, -40.0, 0.0)
    var average = exp(a)
    if (abs(eye.y - point.y) > 0.001){
        let pa = eye.y < params.w ? eye.y - params.w - 1.0 / falloff : -exp(a) / falloff
        let pb = point.y < params.w ? point.y - params.w - 1.0 / falloff : -exp(b) / falloff
        average = (pb - pa) / (point.y - eye.y)
    }
    return exp(-max(distance - params.y, 0.0) * params.x - distance * max(average, 0.0) * params.z)
}
