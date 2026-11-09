options gen2
require gltf/gltf_skinning
require gltf/gltf_processed
require math
require dastest/testing_boost
[test(description = "skin_pack8 quantizes four influences into weights sorted descending that sum to one and stay within 0.006 of the normalized originals, and the processed stride and format versions hold their expected values.")]
def sorted_quantized_influences(t : T?) {
    for (weights in [float4(.25), float4(.0, .2, 1.4, .4), float4(0), float4(.001, .999, 0, 0)]) {
        let packed = skin_pack8(float4(1, 2, 3, 4), weights); let decoded = skin_unpack8(packed)
        t |> success(abs(dot(decoded.weights, float4(1.0)) - 1.0) < .00001)
        for (i in range(1, 4)) { t |> success(decoded.weights[i - 1] >= decoded.weights[i]) }
        if (dot(weights, float4(1)) > 0.0) { for (i in range(4)) { let original = weights[int(decoded.joints[i]) - 1] / dot(weights, float4(1)); t |> success(abs(original - decoded.weights[i]) < .006) } }
    }
    t |> equal(processed_stride(VERTEX_SKIN | VERTEX_UV), 28)
    t |> equal(DAS_GLTF_VERSION, 3)
    t |> equal(DAS_GLTF_PROCESSOR_VERSION, 4)
}
[test(description = "skin_pack8 panics on a joint index past 255 or a fractional one, and canonicalize_skin_influences panics on a negative or oversized joint, a negative weight or an overflowing weight.")]
def compact_skin_rejects_invalid_influences(t : T?) {
    var rejected = false
    try { skin_pack8(float4(256.0, 0.0, 0.0, 0.0), float4(1.0, 0.0, 0.0, 0.0)) } recover { rejected = true }
    t |> success(rejected)
    rejected = false
    try { skin_pack8(float4(1.5, 0.0, 0.0, 0.0), float4(1.0, 0.0, 0.0, 0.0)) } recover { rejected = true }
    t |> success(rejected)
    for (joints, weights in [
            float4(-1.0, 0.0, 0.0, 0.0), float4(65536.0, 0.0, 0.0, 0.0),
            float4(0.0), float4(0.0)], [
            float4(1.0, 0.0, 0.0, 0.0), float4(1.0, 0.0, 0.0, 0.0),
            float4(-1.0, 0.0, 0.0, 0.0), float4(1.0e11, 0.0, 0.0, 0.0)]) {
        rejected = false
        try { canonicalize_skin_influences(joints, weights) } recover { rejected = true }
        t |> success(rejected)
    }
}
[test(description = "canonicalize_skin_influences orders joints by weight with the zero weights trailing, falls back to the first joint at full weight when every weight is zero, and the full vertex layout has a 40-byte stride.")]
def canonical_float_upload_has_trailing_zero_weights(t : T?) {
    let skin = canonicalize_skin_influences(float4(2.0, 8.0, 5.0, 0.0), float4(.1, 0.0, .9, 0.0))
    t |> equal(skin.joints.x, 5.0); t |> equal(skin.joints.y, 2.0)
    t |> equal(skin.weights.z, 0.0); t |> equal(skin.weights.w, 0.0)
    let fallback = canonicalize_skin_influences(float4(4.0, 3.0, 2.0, 1.0), float4(0.0))
    t |> equal(fallback.joints.x, 4.0); t |> equal(fallback.weights, float4(1.0, 0.0, 0.0, 0.0))
    t |> equal(processed_stride(VERTEX_UV | VERTEX_TANGENT | VERTEX_SKIN | VERTEX_UV1 | VERTEX_COLOR), 40)
}
[test(description = "prepare_processed_catalog refuses an older-version or unversioned catalog unless asked to regenerate, and regeneration empties it and stamps the current version.")]
def catalog_version_rollover_requires_full_regeneration(t : T?) {
    var catalog = ProcessedCatalog(version = DAS_GLTF_VERSION - 1)
    catalog.assets |> push(ProcessedCatalogEntry(path = "old.das_glb", key = 1ul))
    var rejected = false
    try { prepare_processed_catalog(catalog, false) } recover { rejected = true }
    t |> success(rejected); t |> equal(length(catalog.assets), 1)
    prepare_processed_catalog(catalog, true)
    t |> equal(catalog.version, DAS_GLTF_VERSION); t |> equal(length(catalog.assets), 0)
    delete catalog; catalog = ProcessedCatalog(); catalog.assets |> push(ProcessedCatalogEntry(path = "unversioned.das_glb", key = 2ul))
    rejected = false
    try { prepare_processed_catalog(catalog, false) } recover { rejected = true }
    t |> success(rejected)
}
