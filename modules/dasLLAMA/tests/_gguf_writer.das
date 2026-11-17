// The synthetic file writers the fixtures share, over the loader's own GGUF record writers: a retyped copy of a
// carrier, a safetensors file, and the half-precision words a synthetic scale plane or safetensors tensor holds.
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
require dasllama/dasllama_gguf public   // the loader's own writers: gguf_put_le / gguf_put_str, the header, key and tensor-info records
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
        var inscope none : array<uint8>
        zeros = gguf_rewritten_head(m, bytes, 0, none, out) $ [unused_argument(dims)] (ti : GGUFTensorInfo; var gtype : int&; var dims : array<uint64>; var nbytes : int64&) {
            gtype = retype?[ti.name] ?? ti.ggml_type
            nbytes = ggml_type_bytes(gtype, ti.n_elem)
        }
        for (kb in kv_bytes) {   // the head keeps the source's key records at the source's own offsets
            if (!key_exists(m.kv, kb.key)) {
                panic("the carrier holds no key '{kb.key}'")
            }
            out[(m.kv?[kb.key] ?? default<GGUFKVLoc>).data_off + int64(kb.at)] = uint8(kb.v)
        }
    }
    fclose(f)
    let w = fopen(dst, "wb")
    fwrite(w, out)
    var chunk : array<uint8>
    chunk |> resize(1048576)
    while (zeros > 0l) {   // every tensor's zero bytes, each padded to the alignment
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
