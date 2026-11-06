// The byte writer the synthetic GGUF fixtures share, over the loader's own word and string writers: the format's v3
// header and tensor infos, and the half-precision words a synthetic scale plane or safetensors tensor holds.
// Root-free: a model-free file may require it.

options gen2
options _dasllama_internal = true

module _gguf_writer shared public

require strings   // peek_data: a string's bytes
require daslib/fio   // fopen / fwrite: the synthetic file
require daslib/strings_boost   // repeat: the safetensors header pad
require daslib/f16_cvt   // f32_to_f16: the F16 half
require dasllama/dasllama_gguf   // gguf_put_le / gguf_put_str: the loader's own writers
require dasllama/dasllama_math_gen   // bf16_bits: the BF16 half, round-to-nearest-even

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
    gguf_put_le(bytes, uint64(long_length(hdr)), 8)
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

//! the GGUF v3 header - the magic, the version, `n_tensors` and `n_kv` - appended to `b`
def gguf_header(var b : array<uint8>; n_tensors, n_kv : uint64) {
    gguf_put_le(b, 0x46554747ul, 4)
    gguf_put_le(b, 3ul, 4)
    gguf_put_le(b, n_tensors, 8)
    gguf_put_le(b, n_kv, 8)
}

//! one GGUF tensor info appended to `b`: its name, its dims, its type and its offset in the data
def gguf_tensor_info(var b : array<uint8>; name : string; dims : array<uint64>; gtype : int; off : uint64) {
    gguf_put_str(b, name)
    gguf_put_le(b, uint64(long_length(dims)), 4)
    for (dv in dims) {
        gguf_put_le(b, dv, 8)
    }
    gguf_put_le(b, uint64(gtype), 4)
    gguf_put_le(b, off, 8)
}

//! `b` zero-padded to the GGUF data alignment, 32 bytes
def gguf_pad_data(var b : array<uint8>) {
    while (long_length(b) % 32l != 0l) {
        b |> push(uint8(0))
    }
}

//! `v` as a half word: BF16 (`bf16`) or F16, each rounded to nearest even
def half_bits(v : float; bf16 : bool) : uint => bf16 ? uint(bf16_bits(v)) : f32_to_f16(v)

//! `v`'s half words (`half_bits`), two little-endian bytes each
def half_bytes(v : array<float>; bf16 : bool) : array<uint8> {
    var b : array<uint8>
    for (x in v) {
        gguf_put_le(b, uint64(half_bits(x, bf16)), 2)
    }
    return <- b
}
