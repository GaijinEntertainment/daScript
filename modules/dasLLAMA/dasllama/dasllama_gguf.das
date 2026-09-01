options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_gguf shared public

require dasllama/dasllama_lint public
require daslib/array_boost
require strings
require math                        // sqrt/abs/clamp — the Q5_K encoder's per-sub-block fit
require daslib/fio                  // shard-path expansion + mapping (split GGUF)
require daslib/strings_boost        // trim_suffix / pad_left (split shard names)
require daslib/strings_convert     // try_to_int (validating shard-index parse)
require llvm/daslib/f16_cvt public  // f16_to_f32 / f32_to_f16 (re-export: whisper/vad/parakeet/audio callers)
require dasllama/dasllama_convert public   // the pure codec layer (readers/dequants/transcodes/encoder) — split out 2026-07-29
require dasllama/dasllama_par       // maybe_parallel_for (+ its emitted dispatch counters) — the transcode hot loops thread when a jobque is up

// Generic GGUF (v2/v3) reader: parses the header, typed key-value metadata, and the
// tensor-info table out of an mmap'd byte buffer, then exposes typed getters. Knows nothing
// about transformers — the semantic mapping (tensor names -> weight blob) lives in
// dasllama_transformer.
//
// File layout (little-endian): "GGUF" magic, u32 version, i64 n_tensors, i64 n_kv,
// then n_kv {key:str, type:i32, [arr_type:i32, count:i64], value...} pairs,
// then n_tensors {name:str, n_dims:u32, dims:i64[], ggml_type:i32, offset:u64},
// then padding to alignment (only if n_tensors>0), then the tensor data blob.

// GGUF value type tags
let private GT_U8 = 0
let private GT_I8 = 1
let private GT_U16 = 2
let private GT_I16 = 3
let private GT_U32 = 4
let private GT_I32 = 5
let private GT_F32 = 6
let private GT_BOOL = 7
let private GT_STRING = 8
let private GT_ARRAY = 9
let private GT_U64 = 10
let private GT_I64 = 11
let private GT_F64 = 12

// upstream tensor data types (subset we read)
let GGML_TYPE_F32 = 0
let GGML_TYPE_F16 = 1
let GGML_TYPE_Q4_0 = 2
let GGML_TYPE_Q5_0 = 6
let GGML_TYPE_Q5_1 = 7
let GGML_TYPE_Q8_0 = 8
let GGML_TYPE_Q3_K = 11
let GGML_TYPE_Q2_K = 10
let GGML_TYPE_IQ3_S = 21
let GGML_TYPE_IQ2_S = 22
let GGML_TYPE_IQ2_XS = 17
let GGML_TYPE_IQ2_XXS = 16
let GGML_TYPE_IQ3_XXS = 18
let GGML_TYPE_IQ4_NL = 20
let GGML_TYPE_Q4_K = 12
let GGML_TYPE_Q5_K = 13
let GGML_TYPE_Q6_K = 14
let GGML_TYPE_IQ4_XS = 23
let GGML_TYPE_BF16 = 30
let GGML_TYPE_MXFP4 = 39

// MXFP4 e2m1 magnitudes DOUBLED (nibble -> ±{0,.5,1,1.5,2,3,4,6}×2, OCP MX spec); the E8M0
// block scale below is halved to compensate.
let private MXFP4_KVALUES = fixed_array(0.0, 1.0, 2.0, 3.0, 4.0, 6.0, 8.0, 12.0,
                                        0.0, -1.0, -2.0, -3.0, -4.0, -6.0, -8.0, -12.0)


def private rd_u64(b : array<uint8> | #; o : int64) : uint64 {
    return uint64(rd_u32(b, o)) | (uint64(rd_u32(b, o + 4l)) << 32ul)
}

def rd_i32(b : array<uint8> | #; o : int64) : int {
    return int(rd_u32(b, o))
}

def private rd_i64(b : array<uint8> | #; o : int64) : int64 {
    return int64(rd_u64(b, o))
}

// Read a length-prefixed string (u64 length + raw bytes), advancing the cursor.
def private rd_str(b : array<uint8> | #; var o : int64&) : string {
    let n = rd_i64(b, o)
    o += 8l
    var s = ""
    unsafe {
        s = string(temp_array(addr<uint8?>(b[o]), n, type<uint8>))
    }
    o += n
    return s
}

// ===== metadata model =====

//! Location of one KV value within the byte buffer (the value bytes are read lazily by getters).
struct GGUFKVLoc {
    gtype : int
    arr_type : int   // element type when gtype == array
    count : int64    // array element count (1 for scalars)
    data_off : int64 // byte offset of the value payload
}

//! One tensor's descriptor: name, type, element count, and offset within the data blob.
struct GGUFTensorInfo {
    name : string
    ggml_type : int
    offset : int64   // relative to the tensor data blob start (of the shard holding it)
    n_elem : int64
    dims : array<int64>
    shard : int      // which shard's data blob holds this tensor (0 for single-file models)
}

//! Parsed GGUF metadata: version, alignment, KV table, tensor list, tensor-data offset.
//! Multi-part (split) models fill shard_* (one entry per mapped shard); empty for single-file.
struct GGUFMeta {
    version : uint
    alignment : int64
    data_offset : int64
    kv : table<string; GGUFKVLoc>
    tensors : array<GGUFTensorInfo>
    @do_not_delete shard_base : array<void?>   // borrowed mapped base per shard (owned by gguf_map_shards)
    shard_size : array<int64>
    shard_data_off : array<int64>              // per-shard data-blob offset (each shard pads independently)
}

def private gguf_scalar_size(gtype : int) : int64 {
    if (gtype == GT_U8 || gtype == GT_I8 || gtype == GT_BOOL) {
        return 1l
    }
    if (gtype == GT_U16 || gtype == GT_I16) {
        return 2l
    }
    if (gtype == GT_U32 || gtype == GT_I32 || gtype == GT_F32) {
        return 4l
    }
    if (gtype == GT_U64 || gtype == GT_I64 || gtype == GT_F64) {
        return 8l
    }
    return 0l  // string is length-prefixed, handled by the caller
}

// Walk past an array value, returning the offset just after it.
def private skip_array(b : array<uint8> | #; off : int64; arr_type : int; count : int64) : int64 {
    if (arr_type == GT_STRING) {
        var o = off
        for (_i in range64(count)) {
            let slen = rd_i64(b, o)
            o += 8l + slen
        }
        return o
    }
    return off + count * gguf_scalar_size(arr_type)
}

//! Parse the GGUF header, KV metadata, and tensor-info table out of `bytes`.
def parse_gguf_meta(bytes : array<uint8> | #) : GGUFMeta {
    var m : GGUFMeta
    if (!(int(bytes[0]) == 'G' && int(bytes[1]) == 'G' && int(bytes[2]) == 'U' && int(bytes[3]) == 'F')) {
        panic("gguf: bad magic (not a GGUF file)")
    }
    m.version = rd_u32(bytes, 4l)
    let n_tensors = rd_i64(bytes, 8l)
    let n_kv = rd_i64(bytes, 16l)
    m.alignment = 32l
    var off = 24l
    // key-value metadata
    for (_i in range64(n_kv)) {
        let key = rd_str(bytes, off)
        let gtype = rd_i32(bytes, off)
        off += 4l
        var loc : GGUFKVLoc
        loc.gtype = gtype
        loc.count = 1l
        if (gtype == GT_ARRAY) {
            loc.arr_type = rd_i32(bytes, off)
            off += 4l
            loc.count = rd_i64(bytes, off)
            off += 8l
            loc.data_off = off
            off = skip_array(bytes, off, loc.arr_type, loc.count)
        } elif (gtype == GT_STRING) {
            loc.data_off = off
            let slen = rd_i64(bytes, off)
            off += 8l + slen
        } else {
            loc.data_off = off
            off += gguf_scalar_size(gtype)
        }
        m.kv |> insert(key, loc)
    }
    if (key_exists(m.kv, "general.alignment")) {
        let alip = m.kv?["general.alignment"] ?? default<GGUFKVLoc>
        m.alignment = rd_i64(bytes, alip.data_off)
    }
    // tensor info table
    m.tensors |> reserve(int(n_tensors))
    for (_i in range64(n_tensors)) {
        var ti : GGUFTensorInfo
        ti.name = rd_str(bytes, off)
        let nd = rd_u32(bytes, off)
        off += 4l
        ti.n_elem = 1l
        ti.dims |> reserve(int(nd))
        for (_d in range(int(nd))) {
            let dv = rd_i64(bytes, off)
            off += 8l
            ti.dims |> push(dv)
            ti.n_elem *= dv
        }
        ti.ggml_type = rd_i32(bytes, off)
        off += 4l
        ti.offset = rd_i64(bytes, off)
        off += 8l
        m.tensors |> emplace(ti)
    }
    // tensor data starts here, padded to alignment only when the file has tensors
    m.data_offset = off
    if (n_tensors > 0l) {
        m.data_offset = ((off + m.alignment - 1l) / m.alignment) * m.alignment
    }
    return <- m
}

// ===== multi-part (split) GGUF support =====

// Invoke blk with the byte view holding tensor ti plus the tensor's data base offset within that
// view. Single-file metas resolve within the caller's bytes; sharded metas resolve the tensor's
// own mapped shard. The one choke point every tensor-data reader goes through.
def private with_tensor_view(m : GGUFMeta; srcbytes : array<uint8> | #; ti : int; blk : block<(bytes : array<uint8>; tbase : int64) : void>) {
    if (empty(m.shard_base)) {
        unsafe {
            invoke(blk, temp_array(addr<uint8?>(srcbytes[0]), long_length(srcbytes), type<uint8>),
                m.data_offset + m.tensors[ti].offset)
        }
        return
    }
    let sh = m.tensors[ti].shard
    unsafe {
        invoke(blk, temp_array(reinterpret<uint8?>(m.shard_base[sh]), m.shard_size[sh], type<uint8>),
            m.shard_data_off[sh] + m.tensors[ti].offset)
    }
}

//! Expand a model path into its shard list. A GGUF split name ("<prefix>-00001-of-00003.gguf")
//! yields every sibling shard (any shard index normalizes); anything else yields [path]. A missing
//! sibling panics (partial split must not run), or yields [] with `missing_ok` (presence gates).
def gguf_shard_paths(path : string; missing_ok : bool = false) : array<string> {
    var res : array<string>
    let name = base_name(path)
    let stem_part = trim_suffix(name, ".gguf")
    let sl = length(stem_part)
    // tail shape: "-NNNNN-of-NNNNN" = 15 chars, with at least 1 prefix char before it
    if (name == stem_part || sl < 16) {
        res |> push(path)
        return <- res
    }
    let tail = slice(stem_part, sl - 15)
    if (slice(tail, 0, 1) != "-" || slice(tail, 6, 10) != "-of-") {
        res |> push(path)
        return <- res
    }
    let ridx = try_to_int(slice(tail, 1, 6))
    let rcnt = try_to_int(slice(tail, 10, 15))
    if (!ridx._is_ok || !rcnt._is_ok) {
        res |> push(path)
        return <- res
    }
    let cnt = rcnt._value
    if (ridx._value < 1 || cnt < 1 || ridx._value > cnt) {
        res |> push(path)
        return <- res
    }
    let prefix = slice(stem_part, 0, sl - 15)
    let dir = dir_name(path)
    let cnt5 = pad_left("{cnt}", 5, '0')
    res |> reserve(cnt)
    for (i in range(1, cnt + 1)) {
        let i5 = pad_left("{i}", 5, '0')
        let shard = path_join(dir, "{prefix}-{i5}-of-{cnt5}.gguf")
        if (!fexist(shard)) {
            if (missing_ok) {
                res |> clear()
                return <- res
            }
            panic("gguf: split model '{name}' is missing shard '{shard}'")
        }
        res |> push(shard)
    }
    return <- res
}

//! Map every shard and invoke blk once with ALL mappings alive (bases/sizes gain one entry per
//! shard, in order). The mappings die when blk returns — nothing derived from them may escape.
def gguf_map_shards(paths : array<string>; idx : int; var bases : array<void?>; var sizes : array<int64>; blk : block<() : void>) {
    if (idx >= length(paths)) {
        invoke(blk)
        return
    }
    fopen(paths[idx], "rb") $(f) {
        if (f == null) {
            panic("gguf: cannot open shard '{paths[idx]}'")
        }
        fmap(f) $(var bytes : array<uint8>#) {
            unsafe {
                bases |> push(addr<void?>(bytes[0]))
            }
            sizes |> push(long_length(bytes))
            gguf_map_shards(paths, idx + 1, bases, sizes, blk)
        }
    }
}

def private parse_gguf_shard(base : void?; size : int64) : GGUFMeta {
    unsafe {
        return <- parse_gguf_meta(temp_array(reinterpret<uint8?>(base), size, type<uint8>))
    }
}

//! Parse a mapped shard set into one unified meta: shard 0 carries the model KVs (config, vocab,
//! chat template — the GGUF split convention), tensor directories merge across all shards with
//! each tensor stamped with its shard. Tensor reads then resolve through the shard_* arrays.
def parse_gguf_meta_shards(bases : array<void?>; sizes : array<int64>) : GGUFMeta {
    var m <- parse_gguf_shard(bases[0], sizes[0])
    m.shard_base := bases
    m.shard_size := sizes
    m.shard_data_off |> reserve(length(bases))
    m.shard_data_off |> push(m.data_offset)
    for (k in range(1, length(bases))) {
        var mk <- parse_gguf_shard(bases[k], sizes[k])
        m.shard_data_off |> push(mk.data_offset)
        m.tensors |> reserve(length(m.tensors) + length(mk.tensors))
        for (i in range(length(mk.tensors))) {
            mk.tensors[i].shard = k
            m.tensors |> emplace(mk.tensors[i])
        }
        delete mk
    }
    return <- m
}

// ===== typed getters (call inside the mmap block — they read from `bytes`) =====

//! True if `key` is present in the metadata.
def gguf_has(m : GGUFMeta; key : string) : bool {
    return key_exists(m.kv, key)
}

//! Read an integer-typed value (any width / signedness) widened to int64.
def gguf_int(m : GGUFMeta; bytes : array<uint8> | #; key : string) : int64 {
    if (!key_exists(m.kv, key)) {
        panic("gguf: missing key '{key}'")
    }
    let loc = m.kv?[key] ?? default<GGUFKVLoc>
    let o = loc.data_off
    let t = loc.gtype
    if (t == GT_U8 || t == GT_I8 || t == GT_BOOL) {
        return int64(bytes[o])
    }
    if (t == GT_U16) {
        return int64(rd_u16(bytes, o))
    }
    if (t == GT_U32) {
        return int64(rd_u32(bytes, o))
    }
    if (t == GT_I32) {
        return int64(rd_i32(bytes, o))
    }
    if (t == GT_U64 || t == GT_I64) {
        return rd_i64(bytes, o)
    }
    panic("gguf: key '{key}' is not an integer (type {t})")
    return 0l
}

//! Read a float32 value (also accepts the rare f64-typed key, narrowed).
def gguf_f32(m : GGUFMeta; bytes : array<uint8> | #; key : string) : float {
    if (!key_exists(m.kv, key)) {
        panic("gguf: missing key '{key}'")
    }
    let loc = m.kv?[key] ?? default<GGUFKVLoc>
    if (loc.gtype == GT_F32) {
        return rd_f32(bytes, loc.data_off)
    }
    if (loc.gtype == GT_F64) {
        return float(unsafe(reinterpret<double>(rd_u64(bytes, loc.data_off))))
    }
    panic("gguf: key '{key}' is not a float (type {loc.gtype})")
    return 0.0
}

//! Read a string value.
def gguf_str(m : GGUFMeta; bytes : array<uint8> | #; key : string) : string {
    if (!key_exists(m.kv, key)) {
        panic("gguf: missing key '{key}'")
    }
    let loc = m.kv?[key] ?? default<GGUFKVLoc>
    if (loc.gtype != GT_STRING) {
        panic("gguf: key '{key}' is not a string (type {loc.gtype})")
    }
    var o = loc.data_off
    return rd_str(bytes, o)
}

//! Read a string array (e.g. tokenizer.upstream.tokens).
def gguf_str_array(m : GGUFMeta; bytes : array<uint8> | #; key : string) : array<string> {
    if (!key_exists(m.kv, key)) {
        panic("gguf: missing key '{key}'")
    }
    let loc = m.kv?[key] ?? default<GGUFKVLoc>
    if (loc.gtype != GT_ARRAY || loc.arr_type != GT_STRING) {
        panic("gguf: key '{key}' is not a string array")
    }
    var out : array<string>
    out |> reserve(int(loc.count))
    var o = loc.data_off
    for (_i in range64(loc.count)) {
        out |> push(rd_str(bytes, o))
    }
    return <- out
}

//! Read a float32 array (e.g. tokenizer.upstream.scores).
def gguf_f32_array(m : GGUFMeta; bytes : array<uint8> | #; key : string) : array<float> {
    if (!key_exists(m.kv, key)) {
        panic("gguf: missing key '{key}'")
    }
    let loc = m.kv?[key] ?? default<GGUFKVLoc>
    if (loc.gtype != GT_ARRAY || loc.arr_type != GT_F32) {
        panic("gguf: key '{key}' is not a float32 array")
    }
    var out : array<float>
    out |> resize(loc.count)
    var o = loc.data_off
    for (i in range64(loc.count)) {
        out[i] = rd_f32(bytes, o)
        o += 4l
    }
    return <- out
}

//! Read an int32 array (e.g. tokenizer.upstream.token_type), widened to int64.
def gguf_i32_array(m : GGUFMeta; bytes : array<uint8> | #; key : string) : array<int64> {
    if (!key_exists(m.kv, key)) {
        panic("gguf: missing key '{key}'")
    }
    let loc = m.kv?[key] ?? default<GGUFKVLoc>
    if (loc.gtype != GT_ARRAY || loc.arr_type != GT_I32) {
        panic("gguf: key '{key}' is not an int32 array")
    }
    var out : array<int64>
    out |> resize(loc.count)
    var o = loc.data_off
    for (i in range64(loc.count)) {
        out[i] = int64(rd_i32(bytes, o))
        o += 4l
    }
    return <- out
}

//! True if `key` is present AND holds an array-typed value (some keys are scalar-or-array,
//! e.g. gemma4's per-layer attention.head_count_kv / sliding_window_pattern).
def gguf_is_array(m : GGUFMeta; key : string) : bool {
    if (!key_exists(m.kv, key)) {
        return false
    }
    return (m.kv?[key] ?? default<GGUFKVLoc>).gtype == GT_ARRAY
}

//! Read an integer array of any element width (u8/i8/bool/u16/i16/u32/i32/u64/i64), widened to
//! int64. gemma4 stores sliding_window_pattern as bool[] and head_count_kv as i32[].
def gguf_int_array(m : GGUFMeta; bytes : array<uint8> | #; key : string) : array<int64> {
    if (!key_exists(m.kv, key)) {
        panic("gguf: missing key '{key}'")
    }
    let loc = m.kv?[key] ?? default<GGUFKVLoc>
    if (loc.gtype != GT_ARRAY) {
        panic("gguf: key '{key}' is not an array")
    }
    let et = loc.arr_type
    var out : array<int64>
    out |> resize(loc.count)
    var o = loc.data_off
    for (i in range64(loc.count)) {
        if (et == GT_U8 || et == GT_BOOL) {
            out[i] = int64(bytes[o])
            o++
        } elif (et == GT_I8) {
            let v = int(bytes[o])
            out[i] = int64(v < 128 ? v : v - 256)
            o++
        } elif (et == GT_U16) {
            out[i] = int64(rd_u16(bytes, o))
            o += 2l
        } elif (et == GT_I16) {
            let v = int(rd_u16(bytes, o))
            out[i] = int64(v < 32768 ? v : v - 65536)
            o += 2l
        } elif (et == GT_U32) {
            out[i] = int64(rd_u32(bytes, o))
            o += 4l
        } elif (et == GT_I32) {
            out[i] = int64(rd_i32(bytes, o))
            o += 4l
        } elif (et == GT_U64 || et == GT_I64) {
            out[i] = rd_i64(bytes, o)
            o += 8l
        } else {
            panic("gguf: key '{key}' element type {et} is not an integer")
        }
    }
    return <- out
}

//! Panic by name unless tensor ``name`` exists with exactly ``want`` elements — the loaders'
//! shape gate; ``who`` prefixes the message ("dasLLAMA gemma4v").
def gguf_expect_elems(m : GGUFMeta; name : string; want : int64; who : string) {
    let ti = gguf_find_tensor(m, name)
    if (ti < 0) {
        panic("{who}: mmproj is missing tensor '{name}'")
    }
    let n = m.tensors[ti].n_elem
    if (n != want) {
        panic("{who}: tensor '{name}' has {n} elements, expected {want}")
    }
}

//! Take ``size`` elements off one of two layout cursors — ``alt`` when ``use_alt``, ``o``
//! otherwise — and return the offset taken: the per-GEMM plane pick at layout time (a bf16
//! tensor rides the halfword cursor, a q8 one the quant cursor, an f32 one the blob cursor).
def take_cursor(use_alt : bool; var o : int64&; var alt : int64&; size : int64) : int64 {
    if (use_alt) {
        let at = alt
        alt += size
        return at
    }
    let at = o
    o += size
    return at
}

//! Index of tensor `name` in m.tensors, or -1 if absent.
def gguf_find_tensor(m : GGUFMeta; name : string) : int {
    for (i in range(length(m.tensors))) {
        if (m.tensors[i].name == name) {
            return i
        }
    }
    return -1
}

// byte-wise copy in uint64-stride loops (uint8 source and destination regions never overlap)
def private bcopy(var d : uint8?; s : uint8 const?; n : int64) {
    unsafe {
        var d8 = reinterpret<uint64?>(d)
        let s8 = reinterpret<uint64 const?>(s)
        for (i in range64(n / 8l)) {
            d8[i] = s8[i]
        }
        for (i in range64((n / 8l) * 8l, n)) {
            d[i] = s[i]
        }
    }
}

// Job count for a parallel transcode/dequant over `blocks` blocks of `disk_sb` bytes: ~1MB of
// source per job, capped, 1 (inline) without a jobque.
def private transcode_jobs(blocks, disk_sb : int64) : int {
    if (!is_job_que_available()) {
        return 1
    }
    return int(clamp(blocks * disk_sb / 1048576l, 1l, 64l))
}

// the tensor index (the caller resolves its view/base through with_tensor_view).
def private kq_transcode_check(m : GGUFMeta; name : string; want_type : int; type_name : string; src_off, expect_n : int64) : int {
    let ti = gguf_find_tensor(m, name)
    if (ti < 0) {
        panic("gguf: tensor '{name}' not found")
    }
    if (m.tensors[ti].ggml_type != want_type) {
        panic("gguf: tensor '{name}' is not {type_name} (type {m.tensors[ti].ggml_type})")
    }
    if (src_off % 256l != 0l || expect_n % 256l != 0l) {
        panic("gguf: tensor '{name}' {type_name} slice [{src_off}, +{expect_n}) is not superblock-aligned")
    }
    if (src_off + expect_n > m.tensors[ti].n_elem) {
        panic("gguf: tensor '{name}' slice [{src_off}, {src_off + expect_n}) exceeds {m.tensors[ti].n_elem} elems")
    }
    return ti
}

//! Transcode a Q4_K tensor into the k4 planes at plane-local element offset `eloff` (from the
//! layout's k4 cursor): quant bytes at (eloff/256)*128, scale blocks at (eloff/256)*20.
def gguf_transcode_q4k(m : GGUFMeta; srcbytes : array<uint8> | #; name : string; var kq : array<uint8>; var ks : array<uint8>; eloff, expect_n : int64; src_off : int64 = 0l) {
    let ti = kq_transcode_check(m, name, GGML_TYPE_Q4_K, "Q4_K", src_off, expect_n)
    let nb = expect_n / 256l
    if (nb <= 0l) {
        return
    }
    guard_dst(name, "k4 quant plane", (eloff / 256l) * K4_QSB, nb * K4_QSB, long_length(kq))
    guard_dst(name, "k4 scale plane", (eloff / 256l) * K4_SSB, nb * K4_SSB, long_length(ks))
    with_tensor_view(m, srcbytes, ti) $(bytes, tbase) {
        let bo = tbase + (src_off / 256l) * 144l
        unsafe {   // per-superblock byte shuffle (see transcode_q4k_superblock), threaded + pointerized
            let srcp = addr<uint8 const?>(bytes[bo])
            var kqp = addr(kq[(eloff / 256l) * K4_QSB])
            var ksp = addr(ks[(eloff / 256l) * K4_SSB])
            maybe_parallel_for(0, int(nb), transcode_jobs(nb, 144l)) $(rb, re) {
                unsafe {
                    for (blk in range64(int64(rb), int64(re))) {
                        bcopy(ksp + blk * K4_SSB, srcp + blk * 144l, 16l)
                        var pad = reinterpret<uint?>(ksp + blk * K4_SSB + 16l)
                        pad[0] = 0u
                        bcopy(kqp + blk * K4_QSB, srcp + blk * 144l + 16l, 128l)
                    }
                }
            }
        }
    }
}

//! Transcode a Q5_K tensor into the k5 planes (see gguf_transcode_q4k; strides 160/20).
def gguf_transcode_q5k(m : GGUFMeta; srcbytes : array<uint8> | #; name : string; var kq : array<uint8>; var ks : array<uint8>; eloff, expect_n : int64; src_off : int64 = 0l) {
    let ti = kq_transcode_check(m, name, GGML_TYPE_Q5_K, "Q5_K", src_off, expect_n)
    let nb = expect_n / 256l
    if (nb <= 0l) {
        return
    }
    guard_dst(name, "k5 quant plane", (eloff / 256l) * K5_QSB, nb * K5_QSB, long_length(kq))
    guard_dst(name, "k5 scale plane", (eloff / 256l) * K5_SSB, nb * K5_SSB, long_length(ks))
    with_tensor_view(m, srcbytes, ti) $(bytes, tbase) {
        let bo = tbase + (src_off / 256l) * 176l
        unsafe {   // per-superblock byte shuffle (see transcode_q5k_superblock), threaded + pointerized
            let srcp = addr<uint8 const?>(bytes[bo])
            var kqp = addr(kq[(eloff / 256l) * K5_QSB])
            var ksp = addr(ks[(eloff / 256l) * K5_SSB])
            maybe_parallel_for(0, int(nb), transcode_jobs(nb, 176l)) $(rb, re) {
                unsafe {
                    for (blk in range64(int64(rb), int64(re))) {
                        bcopy(ksp + blk * K5_SSB, srcp + blk * 176l, 16l)
                        var pad = reinterpret<uint?>(ksp + blk * K5_SSB + 16l)
                        pad[0] = 0u
                        bcopy(kqp + blk * K5_QSB, srcp + blk * 176l + 48l, 128l)   // qs
                        bcopy(kqp + blk * K5_QSB + 128l, srcp + blk * 176l + 16l, 32l)   // qh
                    }
                }
            }
        }
    }
}

//! Transcode a Q6_K tensor into the k6 planes (see gguf_transcode_q4k; strides 192/18, exact).
def gguf_transcode_q6k(m : GGUFMeta; srcbytes : array<uint8> | #; name : string; var kq : array<uint8>; var ks : array<uint8>; eloff, expect_n : int64; src_off : int64 = 0l) {
    let ti = kq_transcode_check(m, name, GGML_TYPE_Q6_K, "Q6_K", src_off, expect_n)
    let nb = expect_n / 256l
    if (nb <= 0l) {
        return
    }
    guard_dst(name, "k6 quant plane", (eloff / 256l) * K6_QSB, nb * K6_QSB, long_length(kq))
    guard_dst(name, "k6 scale plane", (eloff / 256l) * K6_SSB, nb * K6_SSB, long_length(ks))
    with_tensor_view(m, srcbytes, ti) $(bytes, tbase) {
        let bo = tbase + (src_off / 256l) * 210l
        unsafe {   // per-superblock byte shuffle (see transcode_q6k_superblock), threaded + pointerized
            let srcp = addr<uint8 const?>(bytes[bo])
            var kqp = addr(kq[(eloff / 256l) * K6_QSB])
            var ksp = addr(ks[(eloff / 256l) * K6_SSB])
            maybe_parallel_for(0, int(nb), transcode_jobs(nb, 210l)) $(rb, re) {
                unsafe {
                    for (blk in range64(int64(rb), int64(re))) {
                        bcopy(kqp + blk * K6_QSB, srcp + blk * 210l, 192l)   // ql then qh — disk order
                        bcopy(ksp + blk * K6_SSB, srcp + blk * 210l + 192l, 18l)   // 16 sub-scales + f16 d
                    }
                }
            }
        }
    }
}

//! Transcode a Q4_0 tensor into the q40 planes (see gguf_transcode_q4k; strides 128/16, exact):
//! per 32-block the 18 disk bytes split into 2B f16 d (scale plane) + 16B nibbles (quant plane).
def gguf_transcode_q40(m : GGUFMeta; srcbytes : array<uint8> | #; name : string; var kq : array<uint8>; var ks : array<uint8>; eloff, expect_n : int64; src_off : int64 = 0l) {
    let ti = kq_transcode_check(m, name, GGML_TYPE_Q4_0, "Q4_0", src_off, expect_n)
    let nb = expect_n / 256l
    if (nb <= 0l) {
        return
    }
    guard_dst(name, "q40 quant plane", (eloff / 256l) * Q40_QSB, nb * Q40_QSB, long_length(kq))
    guard_dst(name, "q40 scale plane", (eloff / 256l) * Q40_SSB, nb * Q40_SSB, long_length(ks))
    with_tensor_view(m, srcbytes, ti) $(bytes, tbase) {
        let bo = tbase + (src_off / 256l) * 144l   // 8 x 18B disk blocks per superblock
        unsafe {
            let srcp = addr<uint8 const?>(bytes[bo])
            var kqp = addr(kq[(eloff / 256l) * Q40_QSB])
            var ksp = addr(ks[(eloff / 256l) * Q40_SSB])
            maybe_parallel_for(0, int(nb), transcode_jobs(nb, 144l)) $(rb, re) {
                unsafe {
                    for (sb in range64(int64(rb), int64(re))) {
                        for (blk in range64(8l)) {
                            let src = srcp + sb * 144l + blk * 18l
                            ksp[sb * Q40_SSB + blk * 2l] = src[0]        // f16 d
                            ksp[sb * Q40_SSB + blk * 2l + 1l] = src[1]
                            bcopy(kqp + sb * Q40_QSB + blk * 16l, src + 2l, 16l)
                        }
                    }
                }
            }
        }
    }
}

//! Transcode an IQ4_XS tensor into the iq4xs planes (see gguf_transcode_q4k; strides 128/20,
//! exact): the 136B disk superblock splits into the verbatim 128 nibble bytes and a DECODED
//! 20B scale row (f16 d, 2 pad, 8 x int8 (ls - 32), 8 pad — transcode_iq4xs_superblock).
def gguf_transcode_iq4xs(m : GGUFMeta; srcbytes : array<uint8> | #; name : string; var kq : array<uint8>; var ks : array<uint8>; eloff, expect_n : int64; src_off : int64 = 0l) {
    let ti = kq_transcode_check(m, name, GGML_TYPE_IQ4_XS, "IQ4_XS", src_off, expect_n)
    let nb = expect_n / 256l
    if (nb <= 0l) {
        return
    }
    guard_dst(name, "iq4xs quant plane", (eloff / 256l) * IQ4XS_QSB, nb * IQ4XS_QSB, long_length(kq))
    guard_dst(name, "iq4xs scale plane", (eloff / 256l) * IQ4XS_SSB, nb * IQ4XS_SSB, long_length(ks))
    with_tensor_view(m, srcbytes, ti) $(bytes, tbase) {
        let bo = tbase + (src_off / 256l) * 136l
        unsafe {
            let srcp = addr<uint8 const?>(bytes[bo])
            var kqp = addr(kq[(eloff / 256l) * IQ4XS_QSB])
            var ksp = addr(ks[(eloff / 256l) * IQ4XS_SSB])
            maybe_parallel_for(0, int(nb), transcode_jobs(nb, 136l)) $(rb, re) {
                unsafe {
                    for (sb in range64(int64(rb), int64(re))) {
                        let src = srcp + sb * 136l
                        var row = ksp + sb * IQ4XS_SSB
                        row[0] = src[0]
                        row[1] = src[1]
                        row[2] = uint8(0)
                        row[3] = uint8(0)
                        let sh = uint(src[2]) | (uint(src[3]) << 8u)
                        for (blk in range64(8l)) {
                            let lo = (uint(src[4l + blk / 2l]) >> uint(4l * (blk % 2l))) & 15u
                            let ls = int(lo | (((sh >> uint(2l * blk)) & 3u) << 4u))
                            row[4l + blk] = uint8(ls - 32)
                        }
                        for (i in range64(8l)) {
                            row[12l + i] = uint8(0)
                        }
                        bcopy(kqp + sb * IQ4XS_QSB, src + 8l, 128l)
                    }
                }
            }
        }
    }
}

//! Transcode a Q2_K tensor into the k2 planes (strides 64/20, exact): the 84B disk superblock
//! splits into qs verbatim and the 20B scale row [f16 d][f16 dmin][16 sc/min bytes] - all
//! bytes verbatim, header moved first (transcode_q2k_superblock).
def gguf_transcode_q2k(m : GGUFMeta; srcbytes : array<uint8> | #; name : string; var kq : array<uint8>; var ks : array<uint8>; eloff, expect_n : int64; src_off : int64 = 0l) {
    let ti = kq_transcode_check(m, name, GGML_TYPE_Q2_K, "Q2_K", src_off, expect_n)
    let nb = expect_n / 256l
    if (nb <= 0l) {
        return
    }
    guard_dst(name, "k2 quant plane", (eloff / 256l) * K2_QSB, nb * K2_QSB, long_length(kq))
    guard_dst(name, "k2 scale plane", (eloff / 256l) * K2_SSB, nb * K2_SSB, long_length(ks))
    with_tensor_view(m, srcbytes, ti) $(bytes, tbase) {
        let bo = tbase + (src_off / 256l) * 84l
        unsafe {
            let srcp = addr<uint8 const?>(bytes[bo])
            var kqp = addr(kq[(eloff / 256l) * K2_QSB])
            var ksp = addr(ks[(eloff / 256l) * K2_SSB])
            maybe_parallel_for(0, int(nb), transcode_jobs(nb, 84l)) $(rb, re) {
                unsafe {
                    for (sb in range64(int64(rb), int64(re))) {
                        let src = srcp + sb * 84l
                        var row = ksp + sb * K2_SSB
                        row[0l] = src[80l]
                        row[1l] = src[81l]
                        row[2l] = src[82l]
                        row[3l] = src[83l]
                        bcopy(row + 4l, src, 16l)
                        bcopy(kqp + sb * K2_QSB, src + 16l, 64l)
                    }
                }
            }
        }
    }
}

//! Transcode a Q3_K tensor into the k3 planes (see gguf_transcode_q4k; strides 96/18, exact): the
//! 110B disk superblock splits into [qs][hmask] verbatim and the k6-shaped scale row with the 16
//! packed 6-bit sub-scales DECODED to int8 (scale - 32) — transcode_q3k_superblock.
def gguf_transcode_q3k(m : GGUFMeta; srcbytes : array<uint8> | #; name : string; var kq : array<uint8>; var ks : array<uint8>; eloff, expect_n : int64; src_off : int64 = 0l) {
    let ti = kq_transcode_check(m, name, GGML_TYPE_Q3_K, "Q3_K", src_off, expect_n)
    let nb = expect_n / 256l
    if (nb <= 0l) {
        return
    }
    guard_dst(name, "k3 quant plane", (eloff / 256l) * K3_QSB, nb * K3_QSB, long_length(kq))
    guard_dst(name, "k3 scale plane", (eloff / 256l) * K3_SSB, nb * K3_SSB, long_length(ks))
    with_tensor_view(m, srcbytes, ti) $(bytes, tbase) {
        let bo = tbase + (src_off / 256l) * 110l
        unsafe {
            let srcp = addr<uint8 const?>(bytes[bo])
            var kqp = addr(kq[(eloff / 256l) * K3_QSB])
            var ksp = addr(ks[(eloff / 256l) * K3_SSB])
            maybe_parallel_for(0, int(nb), transcode_jobs(nb, 110l)) $(rb, re) {
                unsafe {
                    for (sb in range64(int64(rb), int64(re))) {
                        let src = srcp + sb * 110l
                        var row = ksp + sb * K3_SSB
                        for (i in range64(16l)) {
                            let lo = i < 8l ? int(src[96l + i]) & 15 : int(src[96l + i - 8l]) >> 4
                            let hi = int(src[104l + i % 4l]) >> int(2l * (i / 4l))
                            row[i] = uint8((lo | ((hi & 3) << 4)) - 32)
                        }
                        row[16l] = src[108l]
                        row[17l] = src[109l]
                        bcopy(kqp + sb * K3_QSB, src + 32l, 64l)
                        bcopy(kqp + sb * K3_QSB + 64l, src, 32l)
                    }
                }
            }
        }
    }
}

//! Transcode an IQ3_S tensor into the iq3s planes (strides 104/20, exact): the 110B disk
//! superblock splits into [qs 64][qh 8][signs 32] verbatim and the k4-shaped scale row with the
//! 8 packed 4-bit sub-scales DECODED to int8 (1 + 2s) — transcode_iq3s_superblock.
def gguf_transcode_iq3s(m : GGUFMeta; srcbytes : array<uint8> | #; name : string; var kq : array<uint8>; var ks : array<uint8>; eloff, expect_n : int64; src_off : int64 = 0l) {
    let ti = kq_transcode_check(m, name, GGML_TYPE_IQ3_S, "IQ3_S", src_off, expect_n)
    let nb = expect_n / 256l
    if (nb <= 0l) {
        return
    }
    guard_dst(name, "iq3s quant plane", (eloff / 256l) * IQ3S_QSB, nb * IQ3S_QSB, long_length(kq))
    guard_dst(name, "iq3s scale plane", (eloff / 256l) * IQ3S_SSB, nb * IQ3S_SSB, long_length(ks))
    with_tensor_view(m, srcbytes, ti) $(bytes, tbase) {
        let bo = tbase + (src_off / 256l) * 110l
        unsafe {
            let srcp = addr<uint8 const?>(bytes[bo])
            var kqp = addr(kq[(eloff / 256l) * IQ3S_QSB])
            var ksp = addr(ks[(eloff / 256l) * IQ3S_SSB])
            maybe_parallel_for(0, int(nb), transcode_jobs(nb, 110l)) $(rb, re) {
                unsafe {
                    for (sb in range64(int64(rb), int64(re))) {
                        let src = srcp + sb * 110l
                        var row = ksp + sb * IQ3S_SSB
                        row[0] = src[0]
                        row[1] = src[1]
                        row[2] = uint8(0)
                        row[3] = uint8(0)
                        for (b in range64(8l)) {
                            let nib = b % 2l == 0l ? int(src[106l + b / 2l]) & 15 : int(src[106l + b / 2l]) >> 4
                            row[4l + b] = uint8(1 + 2 * nib)
                        }
                        for (i in range64(12l, 20l)) {
                            row[i] = uint8(0)
                        }
                        bcopy(kqp + sb * IQ3S_QSB, src + 2l, 104l)
                    }
                }
            }
        }
    }
}


//! Transcode an IQ3_XXS tensor into the iq3xxs planes (strides 96/20, exact): qs+aux copy
//! verbatim off the 98B disk block, the iq3s-shaped scale row carries d HALVED (f16_half_bits)
//! and strips (2*ls + 1) — with the halved grid this absorbs upstream's 0.25 exactly.
def gguf_transcode_iq3xxs(m : GGUFMeta; srcbytes : array<uint8> | #; name : string; var kq : array<uint8>; var ks : array<uint8>; eloff, expect_n : int64; src_off : int64 = 0l) {
    let ti = kq_transcode_check(m, name, GGML_TYPE_IQ3_XXS, "IQ3_XXS", src_off, expect_n)
    let nb = expect_n / 256l
    if (nb <= 0l) {
        return
    }
    guard_dst(name, "iq3xxs quant plane", (eloff / 256l) * IQ3XXS_QSB, nb * IQ3XXS_QSB, long_length(kq))
    guard_dst(name, "iq3xxs scale plane", (eloff / 256l) * IQ3XXS_SSB, nb * IQ3XXS_SSB, long_length(ks))
    with_tensor_view(m, srcbytes, ti) $(bytes, tbase) {
        let bo = tbase + (src_off / 256l) * 98l
        unsafe {
            let srcp = addr<uint8 const?>(bytes[bo])
            var kqp = addr(kq[(eloff / 256l) * IQ3XXS_QSB])
            var ksp = addr(ks[(eloff / 256l) * IQ3XXS_SSB])
            maybe_parallel_for(0, int(nb), transcode_jobs(nb, 98l)) $(rb, re) {
                unsafe {
                    for (sb in range64(int64(rb), int64(re))) {
                        let src = srcp + sb * 98l
                        var row = ksp + sb * IQ3XXS_SSB
                        let dh = f16_half_bits(uint(src[0]) | (uint(src[1]) << 8u))
                        row[0] = uint8(dh & 0xFFu)
                        row[1] = uint8(dh >> 8u)
                        row[2] = uint8(0)
                        row[3] = uint8(0)
                        for (b in range64(8l)) {
                            row[4l + b] = uint8(1 + 2 * (int(src[66l + b * 4l + 3l]) >> 4))
                        }
                        for (i in range64(12l, 20l)) {
                            row[i] = uint8(0)
                        }
                        bcopy(kqp + sb * IQ3XXS_QSB, src + 2l, 96l)
                    }
                }
            }
        }
    }
}


//! Transcode an IQ4_NL tensor into the iq4nl planes (q40's strides 128/16, exact): the disk
//! bytes ARE q40's shape - per 32-block f16 d + 16 nibble bytes, split verbatim; only the
//! nibble semantics differ (IQ4NL_LUT at decode, no -8 offset, no bsum term).
def gguf_transcode_iq4nl(m : GGUFMeta; srcbytes : array<uint8> | #; name : string; var kq : array<uint8>; var ks : array<uint8>; eloff, expect_n : int64; src_off : int64 = 0l) {
    let ti = kq_transcode_check(m, name, GGML_TYPE_IQ4_NL, "IQ4_NL", src_off, expect_n)
    let nb = expect_n / 256l
    if (nb <= 0l) {
        return
    }
    guard_dst(name, "iq4nl quant plane", (eloff / 256l) * Q40_QSB, nb * Q40_QSB, long_length(kq))
    guard_dst(name, "iq4nl scale plane", (eloff / 256l) * Q40_SSB, nb * Q40_SSB, long_length(ks))
    with_tensor_view(m, srcbytes, ti) $(bytes, tbase) {
        let bo = tbase + (src_off / 256l) * 144l   // 8 x 18B disk blocks per superblock
        unsafe {
            let srcp = addr<uint8 const?>(bytes[bo])
            var kqp = addr(kq[(eloff / 256l) * Q40_QSB])
            var ksp = addr(ks[(eloff / 256l) * Q40_SSB])
            maybe_parallel_for(0, int(nb), transcode_jobs(nb, 144l)) $(rb, re) {
                unsafe {
                    for (sb in range64(int64(rb), int64(re))) {
                        for (blk in range64(8l)) {
                            let src = srcp + sb * 144l + blk * 18l
                            ksp[sb * Q40_SSB + blk * 2l] = src[0]        // f16 d
                            ksp[sb * Q40_SSB + blk * 2l + 1l] = src[1]
                            bcopy(kqp + sb * Q40_QSB + blk * 16l, src + 2l, 16l)
                        }
                    }
                }
            }
        }
    }
}

//! Transcode an IQ2_S tensor into the iq2s planes (strides 72/20, exact): the 82B disk
//! superblock splits into [idx][signs][qh] verbatim, d eighth-ed, the 16 strips decoded to
//! (1 + 2s) - transcode_iq2s_superblock.
def gguf_transcode_iq2s(m : GGUFMeta; srcbytes : array<uint8> | #; name : string; var kq : array<uint8>; var ks : array<uint8>; eloff, expect_n : int64; src_off : int64 = 0l) {
    let ti = kq_transcode_check(m, name, GGML_TYPE_IQ2_S, "IQ2_S", src_off, expect_n)
    let nb = expect_n / 256l
    if (nb <= 0l) {
        return
    }
    guard_dst(name, "iq2s quant plane", (eloff / 256l) * IQ2S_QSB, nb * IQ2S_QSB, long_length(kq))
    guard_dst(name, "iq2s scale plane", (eloff / 256l) * IQ2S_SSB, nb * IQ2S_SSB, long_length(ks))
    with_tensor_view(m, srcbytes, ti) $(bytes, tbase) {
        let bo = tbase + (src_off / 256l) * 82l
        unsafe {
            let srcp = addr<uint8 const?>(bytes[bo])
            var kqp = addr(kq[(eloff / 256l) * IQ2S_QSB])
            var ksp = addr(ks[(eloff / 256l) * IQ2S_SSB])
            maybe_parallel_for(0, int(nb), transcode_jobs(nb, 82l)) $(rb, re) {
                unsafe {
                    for (sb in range64(int64(rb), int64(re))) {
                        let src = srcp + sb * 82l
                        var row = ksp + sb * IQ2S_SSB
                        let d8 = f16_half_bits(f16_half_bits(f16_half_bits(uint(src[0l]) | (uint(src[1l]) << 8u))))
                        row[0l] = uint8(d8 & 0xFFu)
                        row[1l] = uint8(d8 >> 8u)
                        row[2l] = uint8(0)
                        row[3l] = uint8(0)
                        for (g in range64(16l)) {
                            let nib = g % 2l == 0l ? int(src[74l + g / 2l]) & 15 : int(src[74l + g / 2l]) >> 4
                            row[4l + g] = uint8(1 + 2 * nib)
                        }
                        bcopy(kqp + sb * IQ2S_QSB, src + 2l, 72l)
                    }
                }
            }
        }
    }
}

//! Transcode an IQ2_XS tensor into the iq2xs planes (strides 64/20, exact): the 74B disk
//! superblock's 32 u16 qs words go verbatim, d eighth-ed, the 16 strips decoded to (1 + 2s) -
//! transcode_iq2xs_superblock.
def gguf_transcode_iq2xs(m : GGUFMeta; srcbytes : array<uint8> | #; name : string; var kq : array<uint8>; var ks : array<uint8>; eloff, expect_n : int64; src_off : int64 = 0l) {
    let ti = kq_transcode_check(m, name, GGML_TYPE_IQ2_XS, "IQ2_XS", src_off, expect_n)
    let nb = expect_n / 256l
    if (nb <= 0l) {
        return
    }
    guard_dst(name, "iq2xs quant plane", (eloff / 256l) * IQ2XS_QSB, nb * IQ2XS_QSB, long_length(kq))
    guard_dst(name, "iq2xs scale plane", (eloff / 256l) * IQ2XS_SSB, nb * IQ2XS_SSB, long_length(ks))
    with_tensor_view(m, srcbytes, ti) $(bytes, tbase) {
        let bo = tbase + (src_off / 256l) * 74l
        unsafe {
            let srcp = addr<uint8 const?>(bytes[bo])
            var kqp = addr(kq[(eloff / 256l) * IQ2XS_QSB])
            var ksp = addr(ks[(eloff / 256l) * IQ2XS_SSB])
            maybe_parallel_for(0, int(nb), transcode_jobs(nb, 74l)) $(rb, re) {
                unsafe {
                    for (sb in range64(int64(rb), int64(re))) {
                        let src = srcp + sb * 74l
                        var row = ksp + sb * IQ2XS_SSB
                        let d8 = f16_half_bits(f16_half_bits(f16_half_bits(uint(src[0l]) | (uint(src[1l]) << 8u))))
                        row[0l] = uint8(d8 & 0xFFu)
                        row[1l] = uint8(d8 >> 8u)
                        row[2l] = uint8(0)
                        row[3l] = uint8(0)
                        for (g in range64(16l)) {
                            let nib = g % 2l == 0l ? int(src[66l + g / 2l]) & 15 : int(src[66l + g / 2l]) >> 4
                            row[4l + g] = uint8(1 + 2 * nib)
                        }
                        bcopy(kqp + sb * IQ2XS_QSB, src + 2l, 64l)
                    }
                }
            }
        }
    }
}


//! Transcode an IQ2_XXS tensor into the iq2xxs planes (strides 64/20, exact): the 66B disk
//! superblock's qs region goes verbatim, d eighth-ed, the 8 per-32 strips decoded to
//! (1 + 2s) - transcode_iq2xxs_superblock.
def gguf_transcode_iq2xxs(m : GGUFMeta; srcbytes : array<uint8> | #; name : string; var kq : array<uint8>; var ks : array<uint8>; eloff, expect_n : int64; src_off : int64 = 0l) {
    let ti = kq_transcode_check(m, name, GGML_TYPE_IQ2_XXS, "IQ2_XXS", src_off, expect_n)
    let nb = expect_n / 256l
    if (nb <= 0l) {
        return
    }
    guard_dst(name, "iq2xxs quant plane", (eloff / 256l) * IQ2XXS_QSB, nb * IQ2XXS_QSB, long_length(kq))
    guard_dst(name, "iq2xxs scale plane", (eloff / 256l) * IQ2XXS_SSB, nb * IQ2XXS_SSB, long_length(ks))
    with_tensor_view(m, srcbytes, ti) $(bytes, tbase) {
        let bo = tbase + (src_off / 256l) * 66l
        unsafe {
            let srcp = addr<uint8 const?>(bytes[bo])
            var kqp = addr(kq[(eloff / 256l) * IQ2XXS_QSB])
            var ksp = addr(ks[(eloff / 256l) * IQ2XXS_SSB])
            maybe_parallel_for(0, int(nb), transcode_jobs(nb, 66l)) $(rb, re) {
                unsafe {
                    for (sb in range64(int64(rb), int64(re))) {
                        let src = srcp + sb * 66l
                        var row = ksp + sb * IQ2XXS_SSB
                        let d8 = f16_half_bits(f16_half_bits(f16_half_bits(uint(src[0l]) | (uint(src[1l]) << 8u))))
                        row[0l] = uint8(d8 & 0xFFu)
                        row[1l] = uint8(d8 >> 8u)
                        row[2l] = uint8(0)
                        row[3l] = uint8(0)
                        for (b in range64(8l)) {
                            row[4l + b] = uint8(1 + 2 * (int(src[2l + b * 8l + 7l]) >> 4))
                        }
                        for (i in range64(12l, 20l)) {
                            row[i] = uint8(0)
                        }
                        bcopy(kqp + sb * IQ2XXS_QSB, src + 2l, 64l)
                    }
                }
            }
        }
    }
}

//! Transcode a Q5_1 tensor into the q51 planes (per 32-BLOCK strides 20/4 — see Q51_QB): each
//! 24B disk block splits into 2B d + 2B m (scale plane) and 16B nibbles + 4B qh (quant plane),
//! all verbatim. Exact; `eloff`/`src_off`/`expect_n` are element offsets, % 32.
def gguf_transcode_q51(m : GGUFMeta; srcbytes : array<uint8> | #; name : string; var kq : array<uint8>; var ks : array<uint8>; eloff, expect_n : int64; src_off : int64 = 0l) {
    let ti = kq_transcode_check(m, name, GGML_TYPE_Q5_1, "Q5_1", src_off, expect_n)
    let nb = expect_n / 32l
    if (nb <= 0l) {
        return
    }
    if (eloff % 32l != 0l || src_off % 32l != 0l || expect_n % 32l != 0l) {
        panic("gguf: tensor '{name}' Q5_1 slice [{src_off}, +{expect_n}) @ {eloff} is not block-aligned")
    }
    guard_dst(name, "q51 quant plane", (eloff / 32l) * Q51_QB, nb * Q51_QB, long_length(kq))
    guard_dst(name, "q51 scale plane", (eloff / 32l) * Q51_SB, nb * Q51_SB, long_length(ks))
    with_tensor_view(m, srcbytes, ti) $(bytes, tbase) {
        let bo = tbase + (src_off / 32l) * 24l
        unsafe {
            let srcp = addr<uint8 const?>(bytes[bo])
            var kqp = addr(kq[(eloff / 32l) * Q51_QB])
            var ksp = addr(ks[(eloff / 32l) * Q51_SB])
            maybe_parallel_for(0, int(nb), transcode_jobs(nb, 24l)) $(rb, re) {
                unsafe {
                    for (blk in range64(int64(rb), int64(re))) {
                        let src = srcp + blk * 24l
                        bcopy(ksp + blk * Q51_SB, src, 4l)               // f16 d, f16 m
                        bcopy(kqp + blk * Q51_QB, src + 8l, 16l)         // nibbles
                        bcopy(kqp + blk * Q51_QB + 16l, src + 4l, 4l)    // qh
                    }
                }
            }
        }
    }
}

//! The kq format tag of a superblock i-quant / Q2_K / Q3_K GGML type (q8 = not one of them).
def gguf_kq_fmt_of_type(gtype : int) : KqFmt {
    if (gtype == GGML_TYPE_IQ4_XS) return KqFmt.iq4xs
    if (gtype == GGML_TYPE_Q3_K) return KqFmt.k3
    if (gtype == GGML_TYPE_IQ3_S) return KqFmt.iq3s
    if (gtype == GGML_TYPE_IQ3_XXS) return KqFmt.iq3xxs
    if (gtype == GGML_TYPE_IQ4_NL) return KqFmt.iq4nl
    if (gtype == GGML_TYPE_Q2_K) return KqFmt.k2
    if (gtype == GGML_TYPE_IQ2_S) return KqFmt.iq2s
    if (gtype == GGML_TYPE_IQ2_XS) return KqFmt.iq2xs
    if (gtype == GGML_TYPE_IQ2_XXS) return KqFmt.iq2xxs
    return KqFmt.q8
}

//! Disk bytes per 256-superblock of the i-quant / Q2_K / Q3_K GGML types the native tier reads
//! (0 = not one of them). IQ4_NL is eight 18B blocks of 32.
def gguf_kq_disk_sbb(gtype : int) : int64 {
    if (gtype == GGML_TYPE_IQ4_XS) return 136l
    if (gtype == GGML_TYPE_Q3_K || gtype == GGML_TYPE_IQ3_S) return 110l
    if (gtype == GGML_TYPE_IQ3_XXS) return 98l
    if (gtype == GGML_TYPE_IQ4_NL) return 144l
    if (gtype == GGML_TYPE_Q2_K) return 84l
    if (gtype == GGML_TYPE_IQ2_S) return 82l
    if (gtype == GGML_TYPE_IQ2_XS) return 74l
    if (gtype == GGML_TYPE_IQ2_XXS) return 66l
    return 0l
}

//! One superblock's disk -> plane transcode, keyed by format - the array form the tests and the
//! f32 fallback drive (the bulk loaders are the per-format `gguf_transcode_*`).
def kq_transcode_superblock(fmt : KqFmt; bytes : array<uint8> | #; bo : int64; var kq : array<uint8>; kqo : int64; var ks : array<uint8>; kso : int64) { // nolint:STYLE037 — the format ladder
    if (fmt == KqFmt.iq4xs) {
        transcode_iq4xs_superblock(bytes, bo, kq, kqo, ks, kso)
    } elif (fmt == KqFmt.k3) {
        transcode_q3k_superblock(bytes, bo, kq, kqo, ks, kso)
    } elif (fmt == KqFmt.iq3s) {
        transcode_iq3s_superblock(bytes, bo, kq, kqo, ks, kso)
    } elif (fmt == KqFmt.iq3xxs) {
        transcode_iq3xxs_superblock(bytes, bo, kq, kqo, ks, kso)
    } elif (fmt == KqFmt.iq4nl) {
        transcode_iq4nl_superblock(bytes, bo, kq, kqo, ks, kso)
    } elif (fmt == KqFmt.k2) {
        transcode_q2k_superblock(bytes, bo, kq, kqo, ks, kso)
    } elif (fmt == KqFmt.iq2s) {
        transcode_iq2s_superblock(bytes, bo, kq, kqo, ks, kso)
    } elif (fmt == KqFmt.iq2xs) {
        transcode_iq2xs_superblock(bytes, bo, kq, kqo, ks, kso)
    } elif (fmt == KqFmt.iq2xxs) {
        transcode_iq2xxs_superblock(bytes, bo, kq, kqo, ks, kso)
    } else {
        panic("kq_transcode_superblock: '{fmt}' has no superblock transcode")
    }
}

//! Decode tensor `name` into dst[dst_off ..] as fp32 from any on-disk type the native tier reads.
//! Panics if the tensor is missing, its element count differs, or its type has no decoder here.
def gguf_read_tensor_f32(m : GGUFMeta; srcbytes : array<uint8> | #; name : string; var dst : array<float>; dst_off, expect_n : int64; src_off : int64 = 0l) {   // nolint:STYLE038 — one arm per on-disk upstream type; each arm is a self-contained decode kernel
    let ti = gguf_find_tensor(m, name)
    if (ti < 0) {
        panic("gguf: tensor '{name}' not found")
    }
    let nelem = m.tensors[ti].n_elem
    // src_off (element offset into the tensor) reads a sub-range — used to split fused qkv / gate_up
    // tensors (Phi3). For quantized types it must be 32-aligned so it lands on a block boundary
    // (256-aligned for the K-quant superblock types — those arms check).
    if (src_off + expect_n > nelem) {
        panic("gguf: tensor '{name}' slice [{src_off}, {src_off + expect_n}) exceeds {nelem} elems")
    }
    let gtype = m.tensors[ti].ggml_type
    guard_dst(name, "f32 dst", dst_off, expect_n, long_length(dst))
    with_tensor_view(m, srcbytes, ti) $(bytes, base) {   // nolint:STYLE037 — the upstream-type ladder; the arms share only the borrowed view
    if (gtype == GGML_TYPE_F32) {
        unsafe {   // raw copy, threaded in u64 words (+ one odd-float tail)
            let sp = addr<uint64 const?>(bytes[base + src_off * 4l])
            var dp = addr<uint64?>(dst[dst_off])
            let nw = expect_n / 2l
            maybe_parallel_for(0, int(nw), transcode_jobs(expect_n, 4l)) $(rb, re) {
                unsafe {
                    for (i in range64(int64(rb), int64(re))) {
                        dp[i] = sp[i]
                    }
                }
            }
            if (expect_n % 2l != 0l) {
                let spf = addr<float const?>(bytes[base + src_off * 4l])
                var dpf = addr(dst[dst_off])
                dpf[expect_n - 1l] = spf[expect_n - 1l]
            }
        }
    } elif (gtype == GGML_TYPE_F16) {
        unsafe {   // threaded 32-aligned chunks through the tuned cvt kernel (+ scalar tail)
            let sp = addr<uint16 const?>(bytes[base + src_off * 2l])
            var dp = addr(dst[dst_off])
            let ng = expect_n / 32l
            maybe_parallel_for(0, int(ng), transcode_jobs(expect_n, 2l)) $(rb, re) {
                unsafe {
                    cvt_f16_to_f32(dp + int64(rb) * 32l, sp + int64(rb) * 32l, (int64(re) - int64(rb)) * 32l)
                }
            }
            for (i in range64(ng * 32l, expect_n)) {
                dp[i] = f16_to_f32(uint(sp[i]))
            }
        }
    } elif (gtype == GGML_TYPE_BF16) {
        // bf16 is the top 16 bits of an f32 — the shift-up conversion is exact
        unsafe {
            let src = temp_array(addr<uint16?>(bytes[base + src_off * 2l]), expect_n, type<uint16>)
            for (i in range64(expect_n)) {
                dst[dst_off + i] = reinterpret<float>(uint(src[i]) << 16u)
            }
        }
    } elif (gtype == GGML_TYPE_Q8_0) {
        // block of 32: fp16 scale d (2 bytes) + 32 int8 quants; x[i] = q[i] * d
        let nb = expect_n / 32l
        var bo = base + (src_off / 32l) * 34l
        for (blk in range64(nb)) {
            let d = f16_to_f32(rd_u16(bytes, bo))
            bo += 2l
            for (k in range64(32l)) {
                let u = int(bytes[bo + k])
                let q = u < 128 ? u : u - 256
                dst[dst_off + blk * 32l + k] = float(q) * d
            }
            bo += 32l
        }
    } elif (gtype == GGML_TYPE_Q4_0) {
        // block of 32: fp16 scale d (2 bytes) + 16 bytes (split-half nibbles); x = (nibble - 8) * d
        let nb = expect_n / 32l
        var bo = base + (src_off / 32l) * 18l
        for (blk in range64(nb)) {
            let d = f16_to_f32(rd_u16(bytes, bo))
            bo += 2l
            for (k in range64(16l)) {
                let b = int(bytes[bo + k])
                dst[dst_off + blk * 32l + k] = float((b & 15) - 8) * d
                dst[dst_off + blk * 32l + k + 16l] = float((b >> 4) - 8) * d
            }
            bo += 16l
        }
    } elif (gtype == GGML_TYPE_Q5_0) {
        // block of 32: fp16 scale d (2 bytes) + 4 bytes of high bits + 16 bytes (split-half
        // nibbles); x = ((nibble | high_bit << 4) - 16) * d
        if (src_off % 32l != 0l || expect_n % 32l != 0l) {
            panic("gguf: tensor '{name}' Q5_0 slice [{src_off}, +{expect_n}) is not block-aligned")
        }
        let nb = expect_n / 32l
        var bo = base + (src_off / 32l) * 22l
        for (blk in range64(nb)) {
            let d = f16_to_f32(rd_u16(bytes, bo))
            let qh = rd_u32(bytes, bo + 2l)
            bo += 6l
            for (k in range64(16l)) {
                let b = int(bytes[bo + k])
                let h0 = (int(qh >> uint(k)) & 1) << 4
                let h1 = (int(qh >> uint(k + 16l)) & 1) << 4
                dst[dst_off + blk * 32l + k] = float(((b & 15) | h0) - 16) * d
                dst[dst_off + blk * 32l + k + 16l] = float(((b >> 4) | h1) - 16) * d
            }
            bo += 16l
        }
    } elif (gtype == GGML_TYPE_Q5_1) {
        // block of 32: fp16 d + fp16 min m (4 bytes) + 4 bytes of high bits + 16 bytes (split-half
        // nibbles); x = (nibble | high_bit << 4) * d + m — the min carries the offset, q is unsigned
        if (src_off % 32l != 0l || expect_n % 32l != 0l) {
            panic("gguf: tensor '{name}' Q5_1 slice [{src_off}, +{expect_n}) is not block-aligned")
        }
        let nb = expect_n / 32l
        var bo = base + (src_off / 32l) * 24l
        for (blk in range64(nb)) {
            let d = f16_to_f32(rd_u16(bytes, bo))
            let mn = f16_to_f32(rd_u16(bytes, bo + 2l))
            let qh = rd_u32(bytes, bo + 4l)
            bo += 8l
            for (k in range64(16l)) {
                let b = int(bytes[bo + k])
                let h0 = (int(qh >> uint(k)) & 1) << 4
                let h1 = (int(qh >> uint(k + 16l)) & 1) << 4
                dst[dst_off + blk * 32l + k] = float((b & 15) | h0) * d + mn
                dst[dst_off + blk * 32l + k + 16l] = float((b >> 4) | h1) * d + mn
            }
            bo += 16l
        }
    } elif (gtype == GGML_TYPE_Q4_K || gtype == GGML_TYPE_Q5_K || gtype == GGML_TYPE_Q6_K) {
        if (src_off % 256l != 0l || expect_n % 256l != 0l) {
            panic("gguf: tensor '{name}' K-quant slice [{src_off}, +{expect_n}) is not superblock-aligned")
        }
        // threaded superblock dequant: each worker gets a temp view over ITS chunk of the disk
        // slice + dst and drives the exact per-superblock helper — bit-identical to the serial loop
        let sbb = gtype == GGML_TYPE_Q4_K ? 144l : (gtype == GGML_TYPE_Q5_K ? 176l : 210l)
        let nb = expect_n / 256l
        if (nb > 0l) {
            unsafe {
                let sp = addr<uint8 const?>(bytes[base + (src_off / 256l) * sbb])
                var dp = addr(dst[dst_off])
                maybe_parallel_for(0, int(nb), transcode_jobs(nb, sbb)) $(rb, re) {
                    unsafe {
                        for (blk in range64(int64(rb), int64(re))) {
                            if (sbb == 144l) {
                                dequant_q4k_superblock_p(sp, blk * sbb, dp, blk * 256l)
                            } elif (sbb == 176l) {
                                dequant_q5k_superblock_p(sp, blk * sbb, dp, blk * 256l)
                            } else {
                                dequant_q6k_superblock_p(sp, blk * sbb, dp, blk * 256l)
                            }
                        }
                    }
                }
            }
        }
    } elif (gguf_kq_disk_sbb(gtype) > 0l) {
        // transcode one superblock into scratch planes + its plane dequant: the native load's own two steps, value for value
        if (src_off % 256l != 0l || expect_n % 256l != 0l) {
            panic("gguf: tensor '{name}' superblock slice [{src_off}, +{expect_n}) is not superblock-aligned")
        }
        let sbb = gguf_kq_disk_sbb(gtype)
        let fmt = gguf_kq_fmt_of_type(gtype)
        var kq : array<uint8>
        var ks : array<uint8>
        kq |> resize(int(kq_qsb(fmt)))
        ks |> resize(int(kq_ssb(fmt)))
        let nb = expect_n / 256l
        for (blk in range64(nb)) {
            let bo = base + (src_off / 256l
+ blk) * sbb
            kq_transcode_superblock(fmt, bytes, bo, kq, 0l, ks, 0l)
            dequant_kq_plane_sb(fmt, kq, ks, 0l, false, dst, dst_off + blk * 256l)
        }
        delete kq
        delete ks
    } elif (gtype == GGML_TYPE_MXFP4) {
        // block of 32: 1 E8M0 scale byte + 16 nibble bytes (low nibbles = elems 0..15, high = 16..31);
        // x = e2m1[nibble] * scale — both sides carry the ×2/÷2 on-disk convention (see MXFP4_KVALUES)
        let nb = expect_n / 32l
        var bo = base + (src_off / 32l) * 17l
        for (blk in range64(nb)) {
            let d = e8m0_to_f32_half(uint(bytes[bo]))
            bo++
            for (k in range64(16l)) {
                let b = int(bytes[bo + k])
                dst[dst_off + blk * 32l + k] = MXFP4_KVALUES[b & 15] * d
                dst[dst_off + blk * 32l + k + 16l] = MXFP4_KVALUES[b >> 4] * d
            }
            bo += 16l
        }
    } else {
        panic("gguf: tensor '{name}' type {gtype} not supported (have F32/F16/BF16/Q8_0/Q4_0/Q5_0/Q5_1/Q4_K/Q5_K/Q6_K/MXFP4)")
    }
    }
}

//! Copy a BF16 tensor's raw halfwords into dst[dst_off..] — no fp32 expand (read kernels widen
//! in-loop by the exact bit shift). Requires the tensor to be BF16 on disk (check gguf_tensor_type).
def gguf_transcode_bf16(m : GGUFMeta; srcbytes : array<uint8> | #; name : string; var dst : array<uint16>; dst_off, expect_n : int64) {
    let ti = gguf_find_tensor(m, name)
    if (ti < 0) {
        panic("gguf: tensor '{name}' not found")
    }
    if (m.tensors[ti].ggml_type != GGML_TYPE_BF16) {
        panic("gguf: tensor '{name}' is not BF16 (type {m.tensors[ti].ggml_type})")
    }
    if (expect_n != m.tensors[ti].n_elem) {
        panic("gguf: tensor '{name}' has {m.tensors[ti].n_elem} elems, expected {expect_n}")
    }
    with_tensor_view(m, srcbytes, ti) $(bytes, base) {
        unsafe {
            let src = temp_array(addr<uint16?>(bytes[base]), expect_n, type<uint16>)
            for (i in range64(expect_n)) {
                dst[dst_off + i] = src[i]
            }
        }
    }
}

//! The on-disk upstream type of tensor `name` (GGML_TYPE_*), or -1 if the tensor is absent. Lets a
//! caller pick the direct-transcode path when the disk type already matches the target precision.
def gguf_tensor_type(m : GGUFMeta; name : string) : int {
    let ti = gguf_find_tensor(m, name)
    return ti < 0 ? -1 : m.tensors[ti].ggml_type
}

//! Transcode a Q8_0 tensor straight from disk into our Q8 layout — no fp32 round-trip. int8 quants
//! copy verbatim to qdst[qoff..]; per-block fp16 scale widens to fp32 in sdst[soff..] (soff = qoff/32).
//! Requires the tensor to be Q8_0 on disk (check gguf_tensor_type first).
def gguf_transcode_q8_0(m : GGUFMeta; srcbytes : array<uint8> | #; name : string; var qdst : array<int8>; var sdst : array<float>; qoff, soff, expect_n : int64; src_off : int64 = 0l) {
    let ti = gguf_find_tensor(m, name)
    if (ti < 0) {
        panic("gguf: tensor '{name}' not found")
    }
    if (m.tensors[ti].ggml_type != GGML_TYPE_Q8_0) {
        panic("gguf: tensor '{name}' is not Q8_0 (type {m.tensors[ti].ggml_type})")
    }
    // src_off (32-aligned element offset) transcodes a sub-range — preserves the original Q8 blocks
    // when splitting a fused tensor (Phi3 attn_qkv / gate_up), so there's no lossy fp32 requantize
    if (src_off + expect_n > m.tensors[ti].n_elem) {
        panic("gguf: tensor '{name}' slice [{src_off}, {src_off + expect_n}) exceeds {m.tensors[ti].n_elem} elems")
    }
    let nb = expect_n / 32l
    if (nb <= 0l) {
        return
    }
    guard_dst(name, "q8 quant", qoff, nb * 32l, long_length(qdst))
    guard_dst(name, "q8 scale", soff, nb, long_length(sdst))
    // pointerized + threaded: the quant copy is a bit-reinterpret (uint8 -> int8, same bits), so a
    // block is 4 unaligned u64 moves + one f16 scale widen. Blocks are disjoint — bit-exact at any
    // lane count; jobs are floored to ~1MB of source each so small tensors stay inline.
    with_tensor_view(m, srcbytes, ti) $(bytes, tbase) {
        let base = tbase + (src_off / 32l) * 34l
        unsafe {
            let srcp = addr<uint8 const?>(bytes[base])
            var qdp = addr(qdst[qoff])
            var sdp = addr(sdst[soff])
            let njobs = is_job_que_available() ? int(clamp(nb / 32768l, 1l, 64l)) : 1
            maybe_parallel_for(0, int(nb), njobs) $(rb, re) {
                unsafe {
                    for (blk in range64(int64(rb), int64(re))) {
                        let bo = blk * 34l
                        sdp[blk] = f16_to_f32(uint(srcp[bo]) | (uint(srcp[bo + 1l]) << 8u))
                        let s64 = reinterpret<uint64 const?>(srcp + bo + 2l)
                        var d64 = reinterpret<uint64?>(qdp + blk * 32l)
                        d64[0] = s64[0]
                        d64[1] = s64[1]
                        d64[2] = s64[2]
                        d64[3] = s64[3]
                    }
                }
            }
        }
    }
}

//! Transcode a Q5_0 tensor straight from disk into our Q8 layout — exact, no fp32 round-trip: a
//! Q5_0 block is 32 5-bit ints q in [0,32) times one fp16 scale, so int8 = q - 16 with the same
//! scale is a bit-exact Q8 representation. qoff is the weight-element offset, soff = qoff / 32.
def gguf_transcode_q5_0_to_q8(m : GGUFMeta; srcbytes : array<uint8> | #; name : string; var qdst : array<int8>; var sdst : array<float>; qoff, soff, expect_n : int64; src_off : int64 = 0l) {
    let ti = gguf_find_tensor(m, name)
    if (ti < 0) {
        panic("gguf: tensor '{name}' not found")
    }
    if (m.tensors[ti].ggml_type != GGML_TYPE_Q5_0) {
        panic("gguf: tensor '{name}' is not Q5_0 (type {m.tensors[ti].ggml_type})")
    }
    if (src_off + expect_n > m.tensors[ti].n_elem) {
        panic("gguf: tensor '{name}' slice [{src_off}, {src_off + expect_n}) exceeds {m.tensors[ti].n_elem} elems")
    }
    if (src_off % 32l != 0l || expect_n % 32l != 0l) {
        panic("gguf: tensor '{name}' Q5_0 slice [{src_off}, +{expect_n}) is not block-aligned")
    }
    let nb = expect_n / 32l
    if (nb <= 0l) {
        return
    }
    guard_dst(name, "q8 quant", qoff, nb * 32l, long_length(qdst))
    guard_dst(name, "q8 scale", soff, nb, long_length(sdst))
    // threaded like the Q8_0 transcode: blocks are disjoint — bit-exact at any lane count
    with_tensor_view(m, srcbytes, ti) $(bytes, tbase) {
        let base = tbase + (src_off / 32l) * 22l
        unsafe {
            let srcp = addr<uint8 const?>(bytes[base])
            var qdp = addr(qdst[qoff])
            var sdp = addr(sdst[soff])
            let njobs = is_job_que_available() ? int(clamp(nb / 32768l, 1l, 64l)) : 1
            maybe_parallel_for(0, int(nb), njobs) $(rb, re) {
                unsafe {
                    for (blk in range64(int64(rb), int64(re))) {
                        let bo = blk * 22l
                        sdp[blk] = f16_to_f32(uint(srcp[bo]) | (uint(srcp[bo + 1l]) << 8u))
                        let qh = uint(srcp[bo + 2l]) | (uint(srcp[bo + 3l]) << 8u) | (uint(srcp[bo + 4l]) << 16u) | (uint(srcp[bo + 5l]) << 24u)
                        for (k in range64(16l)) {
                            let b = int(srcp[bo + 6l + k])
                            let h0 = (int(qh >> uint(k)) & 1) << 4
                            let h1 = (int(qh >> uint(k + 16l)) & 1) << 4
                            qdp[blk * 32l + k] = int8(((b & 15) | h0) - 16)
                            qdp[blk * 32l + k + 16l] = int8(((b >> 4) | h1) - 16)
                        }
                    }
                }
            }
        }
    }
}

//! Transcode a Q4_0 tensor straight from disk into our Q4 layout — no fp32 round-trip. The on-disk
//! split-half nibble packing copies verbatim to qdst[qoff_bytes..] (= weight-offset/2); per-block
//! fp16 scale widens to fp32 in sdst[soff..]. Requires the tensor to be Q4_0 on disk.
def gguf_transcode_q4_0(m : GGUFMeta; srcbytes : array<uint8> | #; name : string; var qdst : array<uint8>; var sdst : array<float>; qoff_bytes, soff, expect_n : int64; src_off : int64 = 0l) {
    let ti = gguf_find_tensor(m, name)
    if (ti < 0) {
        panic("gguf: tensor '{name}' not found")
    }
    if (m.tensors[ti].ggml_type != GGML_TYPE_Q4_0) {
        panic("gguf: tensor '{name}' is not Q4_0 (type {m.tensors[ti].ggml_type})")
    }
    if (src_off + expect_n > m.tensors[ti].n_elem) {
        panic("gguf: tensor '{name}' slice [{src_off}, {src_off + expect_n}) exceeds {m.tensors[ti].n_elem} elems")
    }
    let nb = expect_n / 32l
    with_tensor_view(m, srcbytes, ti) $(bytes, tbase) {
        var bo = tbase + (src_off / 32l) * 18l
        for (blk in range64(nb)) {
            sdst[soff + blk] = f16_to_f32(rd_u16(bytes, bo))
            bo += 2l
            for (j in range64(16l)) {
                qdst[qoff_bytes + blk * 16l + j] = bytes[bo + j]
            }
            bo += 16l
        }
    }
}

//! Transcode an MXFP4 tensor straight from disk into split native planes — RAW bits, no dequant
//! round-trip: 16 packed-nibble bytes/32-block verbatim to ndst[noff_bytes..] (=offset/2), 1 RAW
//! E8M0 scale byte to edst[eoff..] (=offset/32). Exact vs the old dequant->requantize-to-Q8 path.
def gguf_transcode_mx4(m : GGUFMeta; srcbytes : array<uint8> | #; name : string; var ndst : array<uint8>; var edst : array<uint8>; noff_bytes, eoff, expect_n : int64; src_off : int64 = 0l) {
    let ti = gguf_find_tensor(m, name)
    if (ti < 0) {
        panic("gguf: tensor '{name}' not found")
    }
    if (m.tensors[ti].ggml_type != GGML_TYPE_MXFP4) {
        panic("gguf: tensor '{name}' is not MXFP4 (type {m.tensors[ti].ggml_type})")
    }
    if (src_off + expect_n > m.tensors[ti].n_elem) {
        panic("gguf: tensor '{name}' slice [{src_off}, {src_off + expect_n}) exceeds {m.tensors[ti].n_elem} elems")
    }
    let nb = expect_n / 32l
    with_tensor_view(m, srcbytes, ti) $(bytes, tbase) {
        var bo = tbase + (src_off / 32l) * 17l
        for (blk in range64(nb)) {
            edst[eoff + blk] = bytes[bo]
            bo++
            for (j in range64(16l)) {
                ndst[noff_bytes + blk * 16l + j] = bytes[bo + j]
            }
            bo += 16l
        }
    }
}
