options gen2
require daslib/lz4
require strings
require dastest/testing_boost
[test]
def literal_and_overlapping_match_vectors(t : T?) {
    var inscope output : array<uint8>
    t |> success(lz4_decompress([0x30u8, 97u8, 98u8, 99u8], output, 3))
    t |> equal(output, [97u8, 98u8, 99u8])
    // One literal, an overlapping eight-byte match, then five final literals.
    t |> success(lz4_decompress([0x14u8, 97u8, 1u8, 0u8, 0x50u8, 98u8, 99u8, 100u8, 101u8, 102u8], output, 14))
    t |> equal(length(output), 14)
    for (i in range(9)) { t |> equal(output[i], 97u8) }
    t |> equal(output[13], 102u8)
}
[test]
def malformed_blocks_and_bombs_leave_output_unchanged(t : T?) {
    var inscope output <- [42u8]
    for (input in [[0xf0u8], [0x20u8, 1u8], [0x10u8, 97u8, 0u8, 0u8], [0x10u8, 97u8, 2u8, 0u8], [0x1fu8, 97u8, 1u8, 0u8, 255u8, 255u8]]) {
        t |> success(!lz4_decompress(input, output, 32)); t |> equal(output, [42u8])
    }
    t |> success(!lz4_decompress([0u8], output, -1)); t |> equal(output, [42u8])
    t |> success(lz4_decompress([0u8], output, 0)); t |> success(empty(output))
}
[test]
def blocks_roundtrip_and_compress_repeated_data(t : T?) {
    for (size in [0, 1, 4, 5, 12, 13, 14, 15, 16, 255, 4096, 70000]) {
        var inscope input : array<uint8>; resize(input, size)
        for (v, i in input, count()) { v = uint8(i % 19) }
        var inscope encoded, decoded : array<uint8>
        t |> success(lz4_compress(input, encoded)); t |> success(lz4_decompress(encoded, decoded, size)); t |> equal(input, decoded); t |> success(long_capacity(decoded) <= int64(size), "decode allocation stays within the supplied budget")
        if (size > 1000) { t |> success(length(encoded) < size / 10) }
        if (size > 0) { t |> success(!lz4_decompress(encoded, decoded, size - 1)); t |> equal(input, decoded) }
        t |> success(lz4_compress(input, input)); t |> success(lz4_decompress(input, input, size)); t |> equal(input, decoded); t |> success(long_capacity(decoded) <= int64(size), "decode allocation stays within the supplied budget")
    }
}
[test]
def decoder_rejects_missing_terminal_literals(t : T?) {
    var inscope output <- [42u8]
    t |> success(!lz4_decompress([0x10u8, 97u8, 1u8, 0u8], output, 100))
    t |> success(!lz4_decompress([0x10u8, 97u8, 1u8, 0u8, 0u8], output, 100))
    t |> equal(output, [42u8])
}
[test]
def deterministic_malformed_inputs_stay_bounded(t : T?) {
    var random = 87131u
    for (trial in range(1000)) {
        var inscope input, output : array<uint8>; resize(input, trial % 64)
        for (byte in input) { random = random * 1664525u + 1013904223u; byte = uint8(random >> 24u) }
        output |> push(42u8)
        if (lz4_decompress(input, output, 128)) { t |> success(length(output) <= 128) }
        else { t |> equal(output, [42u8], "failure never publishes a partial decode") }
    }
}

require daslib/debugger
[test]
def oversized_views_are_rejected_before_narrowing_or_access(t : T?) {
    var input : array<uint8>
    var inscope output <- [42u8]
    var header = unsafe(addr<DapiArray?>(input))
    // A header-only view exercises admission without allocating gigabytes.
    header.size = 2147483648ul
    let compressed = lz4_compress(input, output)
    let decoded = lz4_decompress(input, output, 128)
    header.size = 0ul
    t |> success(!compressed && !decoded)
    t |> equal(output, [42u8])
}

[test]
def terminal_constraints_are_independent(t : T?) {
    var inscope output <- [42u8]
    // Match starts early enough, but only four final literals remain.
    t |> success(!lz4_decompress([0x18u8, 97u8, 1u8, 0u8, 0x40u8, 98u8, 99u8, 100u8, 101u8], output, 17))
    t |> equal(output, [42u8])
    // Five final literals, but the last match starts fewer than twelve bytes from the end.
    t |> success(!lz4_decompress([0x10u8, 97u8, 1u8, 0u8, 0x50u8, 98u8, 99u8, 100u8, 101u8, 102u8], output, 10))
    t |> equal(output, [42u8])
}
[test]
def compression_refuses_encoded_size_overflow_with_signed_input_size(t : T?) {
    var input : array<uint8>; var inscope output <- [42u8]
    var header = unsafe(addr<DapiArray?>(input)); header.size = 2147483647ul
    let result = lz4_compress(input, output); header.size = 0ul
    t |> success(!result); t |> equal(output, [42u8])
}
[test]
def borrowed_byte_views_roundtrip(t : T?) {
    var inscope encoded, decoded : array<uint8>; var compressed = false; var restored = false
    peek_data("abc") $(bytes) { compressed = lz4_compress(bytes, encoded) }
    t |> success(compressed); t |> equal(encoded, [0x30u8, 97u8, 98u8, 99u8])
    peek_data("0abc") $(bytes) { restored = lz4_decompress(bytes, decoded, 3) }
    t |> success(restored); t |> equal(decoded, [97u8, 98u8, 99u8])
}

[test]
def tiny_blocks_do_not_allocate_a_match_table(t : T?) {
    for (size in range(13)) {
        var inscope input, encoded, decoded : array<uint8>; resize(input, size)
        for (byte, i in input, count()) { byte = uint8(i + 97) }
        let before = heap_allocation_stats().x
        let compressed = lz4_compress(input, encoded)
        let allocated = heap_allocation_stats().x - before
        t |> success(compressed)
        t |> success(allocated < 1024ul, "tiny blocks allocate only small output storage, not a match table")
        t |> equal(length(encoded), size + 1); t |> equal(encoded[0], uint8(size << 4))
        t |> success(lz4_decompress(encoded, decoded, size)); t |> equal(decoded, input)
    }
}

[test]
def minimum_matchable_block_still_compresses(t : T?) {
    var inscope input <- [for (_ in range(13)); 97u8]
    var inscope encoded, decoded : array<uint8>
    t |> success(lz4_compress(input, encoded)); t |> success(length(encoded) < length(input))
    t |> success(lz4_decompress(encoded, decoded, 13)); t |> equal(decoded, input)
}
