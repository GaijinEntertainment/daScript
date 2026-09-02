options gen2
options _dasllama_internal = true

module _tts_oracle shared public

require dasllama/dasllama_lint public
require daslib/fio public
require strings public
require dasllama/dasllama_gguf   // rd_u32 / rd_i32 over the mapped bytes
require daslib/array_boost
require math

// Reader for the KittenTTS oracle dumps (harness/kitten_oracle.py): one `TTSO` container per
// case - named f32 / i32 tensors - and the per-size manifest.json listing the cases.

struct TtsDumpTensor {
    dims : array<int>
    f : array<float>
    i : array<int>
}

struct TtsOracleCase {
    file : string
    id : string
    text : string
    norm : string
    ps_espeak : string
    voice : string
    speed : float
    style_row : int
    seed : int
    n_tokens : int
    n_samples : int
    duration_sum : int
    stages : bool
}

struct TtsOracleManifest {
    size : string
    revision : string
    seed_base : int
    cases : array<TtsOracleCase>
}

def private dump_str(b : array<uint8> | #; o, n : int64) : string {
    var s = ""
    unsafe {
        s = string(temp_array(addr<uint8?>(b[o]), n, type<uint8>))
    }
    return s
}

//! Read one dump into name -> tensor.
def load_tts_dump(path : string) : table<string; TtsDumpTensor> {
    var inscope out : table<string; TtsDumpTensor>
    let f = fopen(path, "rb")
    if (f == null) {
        panic("tts oracle: cannot open {path}")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        if (rd_u32(bytes, 0l) != 0x4f535454u) {
            panic("tts oracle: {path} is not a TTSO dump")
        }
        let nrec = int64(rd_u32(bytes, 8l))
        var o = 12l
        for (_r in range64(nrec)) {
            let name_len = int64(rd_u32(bytes, o))
            o += 4l
            let name = dump_str(bytes, o, name_len)
            o += name_len
            let dtype = rd_u32(bytes, o)
            let ndims = int64(rd_u32(bytes, o + 4l))
            o += 8l
            var t = TtsDumpTensor()
            var n = 1l
            for (_d in range64(ndims)) {
                let d = rd_i32(bytes, o)
                t.dims |> push(d)
                n *= int64(d)
                o += 4l
            }
            if (dtype == 0u) {
                t.f |> resize(n)
                unsafe {
                    memcpy(addr(t.f[0]), addr(bytes[o]), n * 4l)
                }
            } else {
                t.i |> resize(n)
                unsafe {
                    memcpy(addr(t.i[0]), addr(bytes[o]), n * 4l)
                }
            }
            o += n * 4l
            out[name] <- t
        }
    }
    fclose(f)
    return <- out
}

//! The manifest of one size's dumps; `cases` empty when the directory is not stocked.
def load_tts_oracle_manifest(dir_path : string) : TtsOracleManifest {
    var m = TtsOracleManifest()
    let text = fread(path_join(dir_path, "manifest.json"))
    return <- m if (empty(text))
    if (!sscan_json(text, m)) {
        panic("tts oracle: {dir_path}/manifest.json does not parse")
    }
    return <- m
}

//! Largest |a - b| over two equal-length tensors (panics on a length mismatch).
def max_abs_diff(a, b : array<float>) : float {
    if (length(a) != length(b)) {
        panic("tts oracle: comparing {length(a)} against {length(b)} elements")
    }
    var m = 0.0
    for (x, y in a, b) {
        m = max(m, abs(x - y))
    }
    return m
}
