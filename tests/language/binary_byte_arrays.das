options gen2
require dastest/testing_boost
require daslib/sha_256

struct BytePayload {
    before : uint = 0x12345678u
    bytes : array<uint8>
    after : uint = 0x87654321u
}

[test(description = "binary byte arrays preserve the scalar serializer wire format and all byte values")]
def test_byte_wire(t : T?) {
    var source = BytePayload()
    resize(source.bytes, 256)
    for (value, i in source.bytes, count()) { value = uint8(i) }
    binary_save(source) $(data) {
        t |> equal(sha256_hex(data),
            "94ac9ad7d8e044c6e4bf4cf95700669bba1fcd9223ece6a70ef74f6a88bd7121",
            "scalar serializer wire format")
        var decoded = BytePayload()
        binary_load(decoded, data)
        t |> equal(hash(decoded), hash(source))
    }
}

[test(description = "empty and large binary byte arrays replace existing data and preserve following fields")]
def test_byte_sizes(t : T?) {
    for (size in fixed_array(0, 1, 65537, 1048576)) {
        var source = BytePayload()
        reserve(source.bytes, size); resize(source.bytes, size)
        for (value, i in source.bytes, count()) { value = uint8(i * 37) }
        var decoded = BytePayload(bytes <- [99u8, 100u8])
        binary_save(source) $(data) { binary_load(decoded, data) }
        t |> equal(length(decoded.bytes), size)
        t |> equal(hash(decoded), hash(source))
    }
}

struct ByteContainers {
    rows : array<array<uint8>>
    fixed_rows : array<uint8[3]>
    signed_bytes : array<int8>
    after : string = "after arrays"
}

[test(description = "nested byte arrays, fixed byte rows and signed bytes preserve container boundaries")]
def test_byte_containers(t : T?) {
    var source = ByteContainers()
    resize(source.rows, 3)
    source.rows[0] <- [0u8, 255u8]
    source.rows[2] <- [128u8]
    source.fixed_rows <- [
        fixed_array(1u8, 2u8, 3u8),
        fixed_array(254u8, 0u8, 255u8)
    ]
    source.signed_bytes <- [int8(-128), int8(-1), int8(0), int8(127)]
    binary_save(source) $(data) {
        var decoded = ByteContainers()
        binary_load(decoded, data)
        t |> equal(hash(decoded), hash(source))
    }
}

[test(description = "binary byte arrays reject truncated payloads and mismatched type headers")]
def test_byte_invalid(t : T?) {
    let source = BytePayload(bytes <- [1u8, 2u8, 3u8])
    binary_save(source) $(data) {
        var damaged <- clone(data)
        resize(damaged, length(damaged) - 5)
        var decoded = BytePayload()
        var failed = false
        try { binary_load(decoded, damaged) } recover { failed = true }
        t |> success(failed, "truncated byte payload must be rejected")
        delete damaged; damaged <- clone(data)
        damaged[12] = uint8(uint(damaged[12]) ^ 1u)
        failed = false
        try { binary_load(decoded, damaged) } recover { failed = true }
        t |> success(failed, "byte-array type mismatch must be rejected")
    }
}

[test(description = "an invalid byte-array length is rejected before replacing destination bytes")]
def test_byte_invalid_length(t : T?) {
    let source = BytePayload(bytes <- [1u8, 2u8, 3u8])
    binary_save(source) $(data) {
        var damaged <- clone(data)
        for (i in range(20, 28)) { damaged[i] = 0u8 }
        damaged[22] = 16u8
        var decoded = BytePayload(bytes <- [99u8, 100u8])
        var failed = false
        try { binary_load(decoded, damaged) } recover { failed = true }
        t |> success(failed, "declared payload exceeds available bytes")
        t |> equal(length(decoded.bytes), 2, "invalid length must not resize the destination")
        t |> equal(decoded.bytes[0], 99u8)
        t |> equal(decoded.bytes[1], 100u8)
    }
}
