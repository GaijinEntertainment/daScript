.. _stdlib_lz4:

LZ4 blocks
==========

``require daslib/lz4`` provides portable byte-array compression and bounded
reconstruction. Both operations are implemented in daslang, without a native
compression library dependency.

``lz4_compress(input, output) : bool`` creates an independent LZ4 block from an
``array<uint8>`` or borrowed byte view. Output replaces the previous array only
on success. Inputs exceeding the safe signed-array encoded-size bound are refused.

``lz4_decompress(input, output, max_output_bytes) : bool`` requires a nonnegative
output budget. It checks literal lengths, copy offsets, overlapping matches,
terminal sequences and output growth before access or allocation. Malformed data
or an insufficient budget returns false and preserves the previous output.
Input and output may alias for both operations.

These are `LZ4 blocks <https://github.com/lz4/lz4/blob/dev/doc/lz4_Block_format.md>`_,
not LZ4 frames. Applications supply framing, expected decoded size and integrity
checks where needed. A successful decode does not authenticate the data. Decoding
work and memory are bounded by the supplied input and output budgets; callers must
also bound compressed input before admitting untrusted data.

Example::

    options gen2
    require daslib/lz4

    [export]
    def main() {
        var input <- [uint8(1), uint8(2), uint8(3)]
        var encoded, decoded : array<uint8>
        verify(lz4_compress(input, encoded))
        verify(lz4_decompress(encoded, decoded, length(input)))
        delete input; delete encoded; delete decoded
    }
