// The byte writer the synthetic GGUF fixtures share: little-endian words, the format's u64-length strings and its v3 header.
// Root-free: a model-free file may require it.

options gen2

module _gguf_writer shared public

require strings   // peek_data: a string's bytes
require daslib/fio   // fopen / fwrite: the synthetic file
require daslib/strings_boost   // repeat: the safetensors header pad

//! a safetensors file of `tensors` (name, dtype, shape, bytes) at `path`; `data_mod4` >= 0 pads the header so the
//! data starts at that residue modulo 4, -1 writes the header unpadded
def write_safetensors(path : string; tensors : array<tuple<name : string; dtype : string; shape : string; data : array<uint8>>>; data_mod4 : int = -1) : bool {
    var data : array<uint8>
    let body = build_string() $(var wr) {
        write(wr, "\{")
        for (t, i in tensors, count()) {
            let b = length(data)
            data |> push_from(t.data)
            write(wr, "{i > 0 ? "," : ""}\"{t.name}\":\{\"dtype\":\"{t.dtype}\",\"shape\":{t.shape},\"data_offsets\":[{b},{length(data)}]\}")
        }
        write(wr, "\}")
    }
    let hdr = data_mod4 < 0 ? body : body + repeat(" ", (data_mod4 - (8 + length(body)) % 4 + 4) % 4)
    var bytes : array<uint8>
    gguf_push_le(bytes, uint64(long_length(hdr)), 8)
    peek_data(hdr) $(d) {
        for (c in d) {
            bytes |> push(c)
        }
    }
    bytes |> push_from(data)
    var ok = false
    fopen(path, "wb") $(f) {
        if (f != null) {
            fwrite(f, bytes)
            ok = true
        }
    }
    return ok
}

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
