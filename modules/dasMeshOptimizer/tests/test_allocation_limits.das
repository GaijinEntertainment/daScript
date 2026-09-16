options gen2
options max_unreserved_size = 65536
require dastest/testing_boost public
require meshoptimizer

[test]
def test_meshoptimizer_reserves_large_outputs(t : T?){
    let vertex_count = 20000
    var vertices : array<uint8>; vertices |> reserve(vertex_count * 4); vertices |> resize(vertex_count * 4)
    for (i in range(vertex_count)){
        vertices[i * 4] = uint8(i); vertices[i * 4 + 1] = uint8(i >> 8)
        vertices[i * 4 + 2] = uint8(i >> 16); vertices[i * 4 + 3] = uint8(i >> 24)
    }
    var indices : array<uint>; indices |> reserve(18000); indices |> resize(18000)
    for (index, i in indices, count()){index = uint(i % vertex_count)}
    var remap : array<uint>
    let compact_count = meshopt_generate_remap(remap, indices, vertices, 4u)
    t |> equal(compact_count, uint(length(indices)), "large remap output")
    var compact : array<uint8>; meshopt_remap_vertices(compact, vertices, remap, 4u, compact_count)
    var remapped : array<uint>; meshopt_remap_indices(remapped, indices, remap)
    var encoded_vertices : array<uint8>; meshopt_encode_vertices(encoded_vertices, compact, 4u)
    var decoded_vertices : array<uint8>
    t |> success(meshopt_decode_vertices(decoded_vertices, encoded_vertices, compact_count, 4u), "large vertex decode")
    t |> equal(hash(decoded_vertices), hash(compact), "large vertex codec roundtrip")
    var encoded_indices : array<uint8>; meshopt_encode_indices(encoded_indices, remapped, compact_count)
    var decoded_indices : array<uint>
    t |> success(meshopt_decode_indices(decoded_indices, encoded_indices, uint(length(remapped))), "large index decode")
    t |> equal(hash(decoded_indices), hash(remapped), "large index codec roundtrip")
    delete vertices; delete indices; delete remap; delete compact; delete remapped
    delete encoded_vertices; delete decoded_vertices; delete encoded_indices; delete decoded_indices
}
