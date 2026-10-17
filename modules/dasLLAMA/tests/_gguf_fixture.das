// The byte writer the synthetic GGUF fixtures share: little-endian words, the format's u64-length strings and its v3 header.
// Root-free: a model-free file may require it.

options gen2

module _gguf_fixture shared public

require strings   // peek_data: a string's bytes

//! the low `nbytes` bytes of `v` appended to `b`, little-endian
def gguf_push_le(var b : array<uint8>; v : uint64; nbytes : int) {
    b |> reserve(length(b) + nbytes)
    for (i in range(nbytes)) {
        b |> push(uint8(uint(v >> uint64(i * 8)) & 0xFFu))
    }
}

//! `s` appended to `b` as GGUF writes a string: its u64 byte length, then its bytes
def gguf_push_str(var b : array<uint8>; s : string) {
    gguf_push_le(b, uint64(long_length(s)), 8)
    peek_data(s) $(d) {
        for (c in d) {
            b |> push(c)
        }
    }
}

//! the GGUF v3 header - the magic, the version, `n_tensors` and `n_kv` - appended to `b`
def gguf_header(var b : array<uint8>; n_tensors, n_kv : uint64) {
    gguf_push_le(b, 0x46554747ul, 4)
    gguf_push_le(b, 3ul, 4)
    gguf_push_le(b, n_tensors, 8)
    gguf_push_le(b, n_kv, 8)
}
