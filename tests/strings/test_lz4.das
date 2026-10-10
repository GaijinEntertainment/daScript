options gen2
require ../../daslib/lz4.das
require strings
require dastest/testing_boost
[test]
def literal_and_overlapping_match_vectors(t : T?) {
    var inscope output : array<uint8>
    t |> success(lz4_decompress([uint8(0x30), uint8(97), uint8(98), uint8(99)], output, 3))
    t |> equal(output, [uint8(97), uint8(98), uint8(99)])
    // One literal, an overlapping eight-byte match, then five final literals.
    t |> success(lz4_decompress([uint8(0x14), uint8(97), uint8(1), uint8(0), uint8(0x50), uint8(98), uint8(99), uint8(100), uint8(101), uint8(102)], output, 14))
    t |> equal(length(output), 14)
    for (i in range(9)) { t |> equal(output[i], uint8(97)) }
    t |> equal(output[13], uint8(102))
}
[test]
def malformed_blocks_and_bombs_leave_output_unchanged(t : T?) {
    var inscope output <- [uint8(42)]
    for (input in [[uint8(0xf0)], [uint8(0x20), uint8(1)], [uint8(0x10), uint8(97), uint8(0), uint8(0)], [uint8(0x10), uint8(97), uint8(2), uint8(0)], [uint8(0x1f), uint8(97), uint8(1), uint8(0), uint8(255), uint8(255)]]) {
        t |> success(!lz4_decompress(input, output, 32)); t |> equal(output, [uint8(42)])
    }
    t |> success(!lz4_decompress([uint8(0)], output, -1)); t |> equal(output, [uint8(42)])
    t |> success(lz4_decompress([uint8(0)], output, 0)); t |> success(empty(output))
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
    var inscope output <- [uint8(42)]
    t |> success(!lz4_decompress([uint8(0x10), uint8(97), uint8(1), uint8(0)], output, 100))
    t |> success(!lz4_decompress([uint8(0x10), uint8(97), uint8(1), uint8(0), uint8(0)], output, 100))
    t |> equal(output, [uint8(42)])
}
[test]
def deterministic_malformed_inputs_stay_bounded(t : T?) {
    var random = 87131u
    for (trial in range(1000)) {
        var inscope input, output : array<uint8>; resize(input, trial % 64)
        for (byte in input) { random = random * 1664525u + 1013904223u; byte = uint8(random >> 24u) }
        output |> push(uint8(42))
        if (lz4_decompress(input, output, 128)) { t |> success(length(output) <= 128) }
        else { t |> equal(output, [uint8(42)], "failure never publishes a partial decode") }
    }
}

require daslib/debugger
[test]
def oversized_views_are_rejected_before_narrowing_or_access(t : T?) {
    var input : array<uint8>
    var inscope output <- [uint8(42)]
    var header = unsafe(addr<DapiArray?>(input))
    // A header-only view exercises admission without allocating gigabytes.
    header.size = 2147483648ul
    let compressed = lz4_compress(input, output)
    let decoded = lz4_decompress(input, output, 128)
    header.size = 0ul
    t |> success(!compressed && !decoded)
    t |> equal(output, [uint8(42)])
}

[test]
def terminal_constraints_are_independent(t : T?) {
    var inscope output <- [uint8(42)]
    // Match starts early enough, but only four final literals remain.
    t |> success(!lz4_decompress([uint8(0x18), uint8(97), uint8(1), uint8(0), uint8(0x40), uint8(98), uint8(99), uint8(100), uint8(101)], output, 17))
    t |> equal(output, [uint8(42)])
    // Five final literals, but the last match starts fewer than twelve bytes from the end.
    t |> success(!lz4_decompress([uint8(0x10), uint8(97), uint8(1), uint8(0), uint8(0x50), uint8(98), uint8(99), uint8(100), uint8(101), uint8(102)], output, 10))
    t |> equal(output, [uint8(42)])
}
[test]
def compression_refuses_encoded_size_overflow_with_signed_input_size(t : T?) {
    var input : array<uint8>; var inscope output <- [uint8(42)]
    var header = unsafe(addr<DapiArray?>(input)); header.size = 2147483647ul
    let result = lz4_compress(input, output); header.size = 0ul
    t |> success(!result); t |> equal(output, [uint8(42)])
}
[test]
def borrowed_byte_views_roundtrip(t : T?) {
    var inscope encoded, decoded : array<uint8>; var compressed = false; var restored = false
    peek_data("abc") $(bytes) { compressed = lz4_compress(bytes, encoded) }
    t |> success(compressed); t |> equal(encoded, [uint8(0x30), uint8(97), uint8(98), uint8(99)])
    peek_data("0abc") $(bytes) { restored = lz4_decompress(bytes, decoded, 3) }
    t |> success(restored); t |> equal(decoded, [uint8(97), uint8(98), uint8(99)])
}
