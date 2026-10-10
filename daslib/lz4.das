options gen2
options indenting = 4
module lz4 shared private
require math

//! Portable LZ4 block compression with bounded, transactional decoding.
//! Blocks carry no size or checksum; applications must supply those separately.
//! This module implements the LZ4 block format, not the LZ4 frame format.

def private lz4_length(var output : array<uint8>; value : int) {
    var remaining = value
    while (remaining >= 255) { output |> push(255u8); remaining -= 255 }
    output |> push(uint8(remaining))
}
def private lz4_word(input : array<uint8> | #; at : int) : uint =>
    uint(input[at]) | (uint(input[at + 1]) << 8u) | (uint(input[at + 2]) << 16u) | (uint(input[at + 3]) << 24u)
def public lz4_compress(input : array<uint8> | #; var output : array<uint8>) : bool {
    //! Compress a byte sequence into an independent LZ4 block.
    //! Returns false if the encoded-size bound cannot fit in a signed array length.
    //! Output is replaced only on success; input and output may alias.
    if (long_length(input) > 2130706432l) { return false }
    let size = length(input)
    var inscope encoded : array<uint8>
    reserve(encoded, size + size / 255 + 16)
    var anchor = 0; var cursor = 0
    // A match needs a preceding byte and twelve bytes remaining in the block.
    if (size >= 13) {
        var inscope positions : array<int>; resize(positions, 65536)
        while (cursor <= size - 12) {
            let word = lz4_word(input, cursor)
            let bucket = (word * 2654435761u) >> 16u
            let previous = positions[bucket] - 1; positions[bucket] = cursor + 1
            if (previous < 0 || cursor - previous > 65535 || lz4_word(input, previous) != word) { cursor++; continue }
            var matched = 4
            while (cursor + matched < size - 5 && input[previous + matched] == input[cursor + matched]) { matched++ }
            let literals = cursor - anchor
            encoded |> push(uint8((min(literals, 15) << 4) | min(matched - 4, 15)))
            if (literals >= 15) { lz4_length(encoded, literals - 15) }
            for (i in range(anchor, cursor)) { encoded |> push(input[i]) }
            let offset = cursor - previous
            encoded |> push(uint8(offset & 255)); encoded |> push(uint8(offset >> 8))
            if (matched >= 19) { lz4_length(encoded, matched - 19) }
            cursor += matched; anchor = cursor
            if (cursor <= size - 12) {
                let at = cursor - 2
                positions[(lz4_word(input, at) * 2654435761u) >> 16u] = at + 1
            }
        }
    }
    let literals = size - anchor
    encoded |> push(uint8(min(literals, 15) << 4))
    if (literals >= 15) { lz4_length(encoded, literals - 15) }
    for (i in range(anchor, size)) { encoded |> push(input[i]) }
    delete output; output <- encoded; return true
}
def private lz4_read_length(input : array<uint8> | #; var cursor : int&; initial, maximum : int; var result : int&) : bool {
    result = initial
    if (result > maximum) { return false }
    if (initial != 15) { return true }
    while (cursor < length(input)) {
        let extra = int(input[cursor++])
        if (extra > maximum - result) { return false }
        result += extra
        if (extra != 255) { return true }
    }
    return false
}
def private lz4_reserve(var output : array<uint8>; needed, maximum : int) {
    if (int64(needed) <= long_capacity(output)) { return }
    let grown = clamp(max(64l, long_capacity(output) * 2l), int64(needed), int64(maximum))
    reserve(output, grown)
}
def public lz4_decompress(input : array<uint8> | #; var output : array<uint8>; max_output_bytes : int) : bool {
    //! Decode a block within a caller-supplied nonnegative output budget.
    //! Malformed or over-budget data leaves output unchanged.
    //! Overlapping matches and aliased input/output are supported.
    if (max_output_bytes < 0 || long_length(input) > 2147483647l || empty(input)) { return false }
    var inscope decoded : array<uint8>
    var cursor = 0; var last_match = -1
    while (cursor < length(input)) {
        let token = int(input[cursor++]); var literals = 0
        if (!lz4_read_length(input, cursor, token >> 4, max_output_bytes - length(decoded), literals) || literals > length(input) - cursor) { return false }
        lz4_reserve(decoded, length(decoded) + literals, max_output_bytes)
        for (i in range(literals)) { decoded |> push(input[cursor + i]) }; cursor += literals // nolint:PERF006 - lz4_reserve above reserves the bounded capacity.
        if (cursor == length(input)) {
            if (last_match >= 0 && (literals < 5 || last_match > length(decoded) - 12)) { return false }
            delete output; output <- decoded; return true
        }
        if (length(input) - cursor < 2) { return false }
        let offset = int(input[cursor]) | (int(input[cursor + 1]) << 8); cursor += 2
        if (offset == 0 || offset > length(decoded)) { return false }
        var matched = 0
        let room = max_output_bytes - length(decoded)
        if (room < 4 || !lz4_read_length(input, cursor, token & 15, room - 4, matched)) { return false }
        last_match = length(decoded)
        matched += 4; lz4_reserve(decoded, length(decoded) + matched, max_output_bytes)
        for (_ in range(matched)) { let value = decoded[length(decoded) - offset]; decoded |> push(value) } // nolint:PERF006 - lz4_reserve above reserves the bounded capacity.
    }
    return false
}
