options gen2
require dastest/testing_boost public
require meshoptimizer
[test]
def test_optimize_and_codec_roundtrip(t : T?){
    var vertices : array<uint8>; vertices |> resize(64)
    for (v in range(4)){for (b in range(16)){vertices[v * 16 + b] = uint8((v % 3) * 20 + b)}}
    var indices <- [0u, 1u, 2u, 3u, 1u, 2u]
    var remap : array<uint>; let count = meshopt_generate_remap(remap, indices, vertices, 16u)
    t |> equal(count, 3u, "duplicate vertex removed")
    var compact : array<uint8>; var remapped : array<uint>
    meshopt_remap_vertices(compact, vertices, remap, 16u, count)
    meshopt_remap_indices(remapped, indices, remap)
    meshopt_optimize_cache(remapped, count)
    t |> equal(meshopt_optimize_fetch(remapped, compact, 16u), 3u)
    var encoded, decoded : array<uint8>
    meshopt_encode_vertices(encoded, compact, 16u)
    t |> success(meshopt_decode_vertices(decoded, encoded, count, 16u), "vertex decode")
    t |> equal(hash(decoded), hash(compact), "codec restores exact packed bytes")
    var index_data : array<uint8>; var index_decoded : array<uint>
    meshopt_encode_indices(index_data, remapped, count)
    t |> success(meshopt_decode_indices(index_decoded, index_data, uint(length(remapped))), "index decode")
    for (triangle in range(length(index_decoded) / 3)){
        let i = triangle * 3
        var first = 0
        if (index_decoded[i + 1] < index_decoded[i]){first = 1}
        if (index_decoded[i + 2] < index_decoded[i + first]){first = 2}
        for (j in range(3)){t |> equal(index_decoded[i + (first + j) % 3], uint(j), "oriented triangle preserved")}
    }
    var bad <- [0u8, 1u8, 2u8]
    t |> success(!meshopt_decode_vertices(decoded, bad, count, 16u), "invalid encoded input is rejected")
    t |> equal(length(decoded), 0)
    var bad_indices <- clone(index_data); bad_indices |> resize(length(bad_indices) - 1)
    delete index_decoded; index_decoded <- [99u]
    t |> success(!meshopt_decode_indices(index_decoded, bad_indices, uint(length(remapped))), "truncated index input is rejected")
    t |> equal(length(index_decoded), 0, "failed index decode clears output")
    var caught_stride = false
    try {
        meshopt_decode_vertices(decoded, encoded, count, 3u)
    } recover {
        caught_stride = true
    }
    t |> success(caught_stride, "invalid codec stride panics")
    var out_of_bounds <- [0u, 1u, 4u]
    var bad_remap : array<uint>; var caught_bounds = false
    try {
        meshopt_generate_remap(bad_remap, out_of_bounds, vertices, 16u)
    } recover {
        caught_bounds = true
    }
    t |> success(caught_bounds, "out-of-range source index panics")
    delete vertices; delete indices; delete remap; delete compact; delete remapped
    delete encoded; delete decoded; delete index_data; delete index_decoded; delete bad; delete bad_indices
    delete out_of_bounds; delete bad_remap
}
