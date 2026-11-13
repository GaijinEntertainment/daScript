// The byte writer the synthetic GGUF fixtures share, over the loader's own word and string writers: the format's v3
// header and tensor infos, and the half-precision words a synthetic scale plane or safetensors tensor holds.
// Root-free: a model-free file may require it.

options gen2
options _dasllama_internal = true

module _gguf_writer shared public

require strings   // peek_data: a string's bytes
require math   // min / max: the retyped copy's offsets
require dastest/testing_boost   // panic_text, test_temp_dir: the retyped copy's refusal
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

let GGUF_VALUE_U32 = 4ul   //! the GGUF value types the synthesized headers write
let GGUF_VALUE_F32 = 6ul
let GGUF_VALUE_STRING = 8ul

//! one GGUF key with a u32 value, appended to `b`
def gguf_kv_u32(var b : array<uint8>; key : string; v : uint64) {
    gguf_put_str(b, key)
    gguf_put_le(b, GGUF_VALUE_U32, 4)
    gguf_put_le(b, v, 4)
}

//! one GGUF key with an f32 value given as its bits, appended to `b`
def gguf_kv_f32_bits(var b : array<uint8>; key : string; bits : uint64) {
    gguf_put_str(b, key)
    gguf_put_le(b, GGUF_VALUE_F32, 4)
    gguf_put_le(b, bits, 4)
}

//! one GGUF key with a string value, appended to `b`
def gguf_kv_str(var b : array<uint8>; key, v : string) {
    gguf_put_str(b, key)
    gguf_put_le(b, GGUF_VALUE_STRING, 4)
    gguf_put_str(b, v)
}

let GGUF_VALUE_I32 = 5ul
let GGUF_VALUE_ARRAY = 9ul

//! one GGUF key with a string-array value, appended to `b`
def gguf_kv_str_array(var b : array<uint8>; key : string; v : array<string>) {
    gguf_put_str(b, key)
    gguf_put_le(b, GGUF_VALUE_ARRAY, 4)
    gguf_put_le(b, GGUF_VALUE_STRING, 4)
    gguf_put_le(b, uint64(long_length(v)), 8)
    for (s in v) {
        gguf_put_str(b, s)
    }
}

//! one GGUF key with an i32-array value, appended to `b`
def gguf_kv_i32_array(var b : array<uint8>; key : string; v : array<int>) {
    gguf_put_str(b, key)
    gguf_put_le(b, GGUF_VALUE_ARRAY, 4)
    gguf_put_le(b, GGUF_VALUE_I32, 4)
    gguf_put_le(b, uint64(long_length(v)), 8)
    for (x in v) {
        gguf_put_le(b, uint64(uint(x)), 4)
    }
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

//! a copy of the GGUF at `src` written to `dst` with the tensors `retype` names retyped, every weight byte zero and every
//! offset recomputed - a loader's refusals read the tensors' types alone, so a carrier no quantize tool mints stands in for one
[cold_path]
def gguf_retyped_copy(src, dst : string; retype : table<string; int>) {
    var inscope no_bytes : array<tuple<key : string; at : int; v : int>>
    gguf_retyped_copy(src, dst, retype, no_bytes)
}

//! ... with `kv_bytes` written over the header: byte `v` at offset `at` of key `key`'s value
[cold_path]
def gguf_retyped_copy(src, dst : string; retype : table<string; int>; kv_bytes : array<tuple<key : string; at : int; v : int>>) {
    var out : array<uint8>
    var zeros = 0l
    let f = fopen(src, "rb")
    fmap(f) $(var bytes : array<uint8>#) {
        var inscope m <- parse_gguf_meta(bytes)
        var kv_last = 24l   // the tensor table starts at the first tensor's name record, past the last KV's value
        for (loc in values(m.kv)) {
            kv_last = max(kv_last, loc.data_off)
        }
        var rec : array<uint8>
        gguf_put_str(rec, m.tensors[0].name)
        var table_at = -1l
        for (o in range64(kv_last, long_length(bytes) - long_length(rec))) {
            var hit = true
            for (i in range64(long_length(rec))) {
                if (bytes[o + i] != rec[i]) {
                    hit = false
                    break
                }
            }
            if (hit) {
                table_at = o
                break
            }
        }
        verify(table_at > 0l, "the carrier's tensor table not found")
        gguf_header(out, uint64(long_length(m.tensors)), uint64(long_length(m.kv)))
        let head = long_length(out)
        out |> resize(head + table_at - 24l)
        for (i in range64(24l, table_at)) {
            out[head + i - 24l] = bytes[i]
        }
        for (kb in kv_bytes) {
            if (!key_exists(m.kv, kb.key)) {
                panic("the carrier holds no key '{kb.key}'")
            }
            out[head + (m.kv?[kb.key] ?? default<GGUFKVLoc>).data_off + int64(kb.at) - 24l] = uint8(kb.v)
        }
        var off = 0l
        for (ti in m.tensors) {
            let gt = retype?[ti.name] ?? ti.ggml_type
            gguf_put_str(out, ti.name)
            gguf_put_le(out, uint64(long_length(ti.dims)), 4)
            for (dim in ti.dims) {
                gguf_put_le(out, uint64(dim), 8)
            }
            gguf_put_le(out, uint64(gt), 4)
            gguf_put_le(out, uint64(off), 8)
            off = (off + ggml_type_bytes(gt, ti.n_elem) + m.alignment - 1l) / m.alignment * m.alignment
        }
        let data_at = (long_length(out) + m.alignment - 1l) / m.alignment * m.alignment
        zeros = data_at - long_length(out) + off
    }
    fclose(f)
    let w = fopen(dst, "wb")
    fwrite(w, out)
    var chunk : array<uint8>
    chunk |> resize(1048576)
    while (zeros > 0l) {   // the pad to the data's alignment, then every tensor's zero bytes
        let n = min(zeros, long_length(chunk))
        if (n < long_length(chunk)) {
            chunk |> resize(n)
        }
        fwrite(w, chunk)
        zeros -= n
    }
    fclose(w)
    delete chunk
    delete out
}

//! the refusal `stage` raises over the carrier at `src` with `retype` applied (`gguf_retyped_copy`, the copy named `name`) - "" where it staged
[cold_path]
def gguf_retyped_refusal(src, name : string; retype : table<string; int>; stage : block<(path : string) : void>) : string {
    let dst = path_join(test_temp_dir(), "{name}.gguf")
    gguf_retyped_copy(src, dst, retype)
    let why = panic_text() $ {
        invoke(stage, dst)
    }
    remove(dst)
    return why
}

//! `v`'s half words (`half_bits`), two little-endian bytes each
def half_bytes(v : array<float>; bf16 : bool) : array<uint8> {
    var b : array<uint8>
    for (x in v) {
        gguf_put_le(b, uint64(half_bits(x, bf16)), 2)
    }
    return <- b
}
