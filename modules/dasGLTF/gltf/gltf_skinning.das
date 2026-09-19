options gen2
module gltf_skinning shared public
require math
struct SkinInfluences {
    //! Joint indices and normalized weights, paired in descending weight order.
    joints, weights : float4
}
def canonicalize_skin_influences(joints, weights : float4) : SkinInfluences {
    var result = SkinInfluences(joints = joints, weights = weights)
    var sum = 0.0
    for (i in range(4)) {
        if (!(joints[i] >= 0.0 && joints[i] <= 65535.0 && joints[i] == floor(joints[i]) && weights[i] >= 0.0 && weights[i] <= 1.0e10)) { panic("Invalid skin influence") }
        sum += weights[i]
    }
    result.weights = sum > 0.0 ? weights / sum : float4(1.0, 0.0, 0.0, 0.0)
    for (i in range(1, 4)) { var j = i; while (j > 0 && result.weights[j] > result.weights[j - 1]) { swap(result.weights[j], result.weights[j - 1]); swap(result.joints[j], result.joints[j - 1]); j-- } }
    return result
}
[arch(at="../ARCHITECTURE.md#compact-skin-influences")]
def skin_pack8(joints, weights : float4) : uint2 {
    let sorted = canonicalize_skin_influences(joints, weights)
    var indices = uint4(sorted.joints)
    for (i in range(4)) { if (indices[i] > 255u) { panic("Skin joint index exceeds 8-bit palette range") } }
    var quantized_weights = uint4(round(sorted.weights * 255.0))
    quantized_weights.x = 255u - quantized_weights.y - quantized_weights.z - quantized_weights.w
    for (i in range(1, 4)) { var j = i; while (j > 0 && quantized_weights[j] > quantized_weights[j - 1]) { swap(quantized_weights[j], quantized_weights[j - 1]); swap(indices[j], indices[j - 1]); j-- } }
    return uint2(indices.x | (indices.y << 8u) | (indices.z << 16u) | (indices.w << 24u), quantized_weights.x | (quantized_weights.y << 8u) | (quantized_weights.z << 16u) | (quantized_weights.w << 24u))
}
def skin_unpack8(bits : uint2) : SkinInfluences {
    return SkinInfluences(joints = float4(float(bits.x & 255u), float((bits.x >> 8u) & 255u), float((bits.x >> 16u) & 255u), float(bits.x >> 24u)), weights = float4(float(bits.y & 255u), float((bits.y >> 8u) & 255u), float((bits.y >> 16u) & 255u), float(bits.y >> 24u)) / 255.0)
}
