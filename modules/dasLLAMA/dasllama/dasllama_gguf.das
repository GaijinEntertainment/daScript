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
require dasllama/dasllama_env       // g_env_engine.mtp_head — the split NextN head sidecar override
require daslib/f16_cvt public  // f16_to_f32 / f32_to_f16 (re-export: whisper/vad/parakeet/audio callers)
require dasllama/dasllama_convert public   // the pure codec layer (readers/dequants/transcodes/encoder) — split out 2026-07-29
require dasllama/dasllama_par       // maybe_parallel_for (+ its emitted dispatch counters) — the transcode hot loops thread when a jobque is up
require dasllama/dasllama_math      // e8m0_half — the MXFP4 scale decode

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

//! Bytes `n` elements of a ggml type take on disk (block bytes x blocks); 0 for a type the loader
//! does not know, so a census sums what it can and names the rest.
def ggml_type_bytes(gt : int; n : int64) : int64 {
    if (gt == GGML_TYPE_F32) return n * 4l
    if (gt == GGML_TYPE_F16 || gt == GGML_TYPE_BF16) return n * 2l
    if (gt == GGML_TYPE_Q4_0 || gt == GGML_TYPE_IQ4_NL) return n / 32l * 18l
    if (gt == GGML_TYPE_Q5_0) return n / 32l * 22l
    if (gt == GGML_TYPE_Q5_1) return n / 32l * 24l
    if (gt == GGML_TYPE_Q8_0) return n / 32l * 34l
    if (gt == GGML_TYPE_MXFP4) return n / 32l * 17l
    if (gt == GGML_TYPE_Q2_K) return n / 256l * 84l
    if (gt == GGML_TYPE_Q3_K || gt == GGML_TYPE_IQ3_S) return n / 256l * 110l
    if (gt == GGML_TYPE_Q4_K) return n / 256l * 144l
    if (gt == GGML_TYPE_Q5_K) return n / 256l * 176l
    if (gt == GGML_TYPE_Q6_K) return n / 256l * 210l
    if (gt == GGML_TYPE_IQ2_XXS) return n / 256l * 66l
    if (gt == GGML_TYPE_IQ2_XS) return n / 256l * 74l
    if (gt == GGML_TYPE_IQ2_S) return n / 256l * 82l
    if (gt == GGML_TYPE_IQ3_XXS) return n / 256l * 98l
    if (gt == GGML_TYPE_IQ4_XS) return n / 256l * 136l
    return 0l
}

//! Bytes `n` weights of a kq format take on disk (the ggml block sizes, so a 32-block format
//! answers per 32) - what a device plane is measured against; q8 counts its Q8_0 blocks.
def kq_disk_bytes(f : KqFmt; n : int64) : int64 => ggml_type_bytes(kq_ggml_type(f), n)

let private GGML_TYPE_NAMES <- {
    GGML_TYPE_F32 => "F32", GGML_TYPE_F16 => "F16", GGML_TYPE_BF16 => "BF16",
    GGML_TYPE_Q4_0 => "Q4_0", GGML_TYPE_Q5_0 => "Q5_0", GGML_TYPE_Q5_1 => "Q5_1", GGML_TYPE_Q8_0 => "Q8_0", GGML_TYPE_MXFP4 => "MXFP4",
    GGML_TYPE_Q2_K => "Q2_K", GGML_TYPE_Q3_K => "Q3_K", GGML_TYPE_Q4_K => "Q4_K", GGML_TYPE_Q5_K => "Q5_K", GGML_TYPE_Q6_K => "Q6_K",
    GGML_TYPE_IQ2_XXS => "IQ2_XXS", GGML_TYPE_IQ2_XS => "IQ2_XS", GGML_TYPE_IQ2_S => "IQ2_S",
    GGML_TYPE_IQ3_XXS => "IQ3_XXS", GGML_TYPE_IQ3_S => "IQ3_S", GGML_TYPE_IQ4_NL => "IQ4_NL", GGML_TYPE_IQ4_XS => "IQ4_XS"
}

//! The ggml type's upstream spelling for logs ("Q4_K", "IQ3_S"); "type N" for one the loader does not know.
def ggml_type_name(gt : int) : string => GGML_TYPE_NAMES?[gt] ?? "type {gt}"

// MXFP4 e2m1 magnitudes DOUBLED (nibble -> ±{0,.5,1,1.5,2,3,4,6}×2, OCP MX spec); the E8M0
// block scale below is halved to compensate.
let private MXFP4_KVALUES = fixed_array(0.0, 1.0, 2.0, 3.0, 4.0, 6.0, 8.0, 12.0,
                                        0.0, -1.0, -2.0, -3.0, -4.0, -6.0, -8.0, -12.0)


def private rd_u64(b : array<uint8> | #; o : int64) : uint64 {
    return uint64(rd_u32(b, o)) | (uint64(rd_u32(b, o + 4l)) << 32ul)
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
    nextn_sidecar : int64                      // nextn_predict_layers carried by a head shard (0 = the trunk's own KVs rule)
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

let GGUF_FIXED_HEADER_BYTES = 24l   // magic, version, tensor count, kv count: what parse_gguf_meta reads before any table

def gguf_magic_ok(bytes : array<uint8> | #) : bool {
    return long_length(bytes) >= 4l && int(bytes[0]) == 'G' && int(bytes[1]) == 'G' && int(bytes[2]) == 'U' && int(bytes[3]) == 'F'
}

//! Parse the GGUF header, KV metadata, and tensor-info table out of `bytes`.
def parse_gguf_meta(bytes : array<uint8> | #) : GGUFMeta {
    var m : GGUFMeta
    if (!gguf_magic_ok(bytes)) {
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

//! the in-process override of DASLLAMA_MTP_HEAD - for a trunk/head pair the adjacency rule cannot name
var private g_mtp_head_override = ""
var private g_mtp_head_override_set = false

def public set_mtp_head_path(path : string) {
    g_mtp_head_override = path
    g_mtp_head_override_set = true
    if (!empty(path)) {
        to_log(LOG_INFO, "dasLLAMA: MTP head pinned to {path} (set_mtp_head_path)\n")
    }
}

def public clear_mtp_head_path() {
    g_mtp_head_override = ""
    g_mtp_head_override_set = false
}

//! The split NextN/MTP head that rides `path`'s shard walk: the override / `DASLLAMA_MTP_HEAD` when
//! set, else the `mtp-<trunk basename>` sibling beside the trunk (the published split-head layout),
//! else "". An assistant drafter is never a head - its weights ride their own lane.
def mtp_head_sidecar(path : string) : string {
    let name = base_name(path)
    if (name |> starts_with("mtp-")) {   // a head loaded on its own never recurses onto itself
        return ""
    }
    let over = g_mtp_head_override_set ? g_mtp_head_override : g_env_engine.mtp_head
    if (!empty(over)) {
        if (!g_mtp_head_override_set) {
            to_log(LOG_INFO, "dasLLAMA: MTP head pinned to {over} (DASLLAMA_MTP_HEAD)\n")
        }
        return head_file_or_empty(over)
    }
    return head_file_or_empty(path_join(dir_name(path), "mtp-{name}"))
}

def private head_file_or_empty(side : string) : string {
    return fexist(side) && !gguf_is_assistant(side) ? side : ""
}

//! Expand a model path into its shard list: a GGUF split name ("<prefix>-00001-of-00003.gguf") yields
//! every sibling (any index normalizes), anything else [path]; a split NextN head rides LAST. A
//! missing sibling panics (a partial split must not run), or yields [] with `missing_ok` (presence gates).
def gguf_shard_paths(path : string; missing_ok : bool = false) : array<string> {
    var res <- gguf_split_paths(path, missing_ok)
    if (!empty(res)) {
        let side = mtp_head_sidecar(path)
        if (!empty(side)) {
            res |> push(side)   // the head is the LAST shard: first-match lookups keep the trunk's tables
        }
    }
    return <- res
}

//! `general.architecture` of the GGUF at `path` ("" when unreadable - a file that is not a GGUF,
//! or one cut short of its own metadata, the way a download that stopped is) - a header-only read.
def gguf_file_arch(path : string) : string {
    var arch = ""
    fopen(path, "rb") $(f) {
        if (f == null) {
            return
        }
        fmap(f) $(var bytes : array<uint8>#) {
            return if (long_length(bytes) < GGUF_FIXED_HEADER_BYTES || !gguf_magic_ok(bytes))
            try {
                var inscope m <- parse_gguf_meta(bytes)
                if (gguf_has(m, "general.architecture")) {
                    arch = gguf_str(m, bytes, "general.architecture")
                }
            } recover {
                arch = ""
            }
        }
    }
    return arch
}

//! An assistant drafter (arch `<family>-assistant`: gemma4-assistant) is its own tiny model over the
//! target's K/V, never a shard of the trunk - the shard walk leaves it to the drafter loader.
def gguf_is_assistant(path : string) : bool => gguf_file_arch(path) |> ends_with("-assistant")

// the GGUF split-name expansion proper ("<prefix>-NNNNN-of-NNNNN.gguf" -> every sibling)
def private gguf_split_paths(path : string; missing_ok : bool) : array<string> {
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
            bases |> push(unsafe(addr<void?>(bytes[0])))
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
        // a head shard carries the nextn count the trunk file dropped (a split conversion writes the
        // trunk without the head, the head as its own file); shard-0 KVs otherwise rule, so promote it only when absent there
        unsafe {
            let kb <- temp_array(reinterpret<uint8?>(bases[k]), sizes[k], type<uint8>)
            let arch = gguf_has(mk, "general.architecture") ? gguf_str(mk, kb, "general.architecture") : ""
            let key = "{arch}.nextn_predict_layers"
            if (!empty(arch) && gguf_has(mk, key) && !gguf_has(m, key)) {
                m.nextn_sidecar = gguf_int(mk, kb, key)
            }
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

//! The tensor's index, or a panic naming the family that needed it.
def gguf_need_tensor(m : GGUFMeta; name, who : string) : int {
    let ti = gguf_find_tensor(m, name)
    if (ti < 0) {
        panic("dasLLAMA {who}: tensor '{name}' missing from the model")
    }
    return ti
}

//! A whole tensor as f32, whatever its on-disk type.
def gguf_read_f32_arr(m : GGUFMeta; bytes : array<uint8> | #; name, who : string) : array<float> {
    let ti = gguf_need_tensor(m, name, who)
    var inscope a : array<float>
    a |> reserve_resize(m.tensors[ti].n_elem)
    gguf_read_tensor_f32(m, bytes, name, a, 0l, m.tensors[ti].n_elem)
    return <- a
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

// Job count for a parallel transcode/dequant over `blocks` blocks of `disk_sb` bytes: ~1MB of
// source per job, capped, 1 (inline) without a jobque.
def private transcode_jobs(blocks, disk_sb : int64) : int {
    if (!is_job_que_available()) {
        return 1
    }
    return int(clamp(blocks * disk_sb / 1048576l, 1l, 64l))
}

//! `align` is the block the slice must tile - 256 for the superblock formats, 32 for the per-32 ones.
def private kq_transcode_check(m : GGUFMeta; name : string; want_type : int; type_name : string; src_off, expect_n : int64; align : int64 = 256l) : int {
    let ti = gguf_find_tensor(m, name)
    if (ti < 0) {
        panic("gguf: tensor '{name}' not found")
    }
    if (m.tensors[ti].ggml_type != want_type) {
        panic("gguf: tensor '{name}' is not {type_name} (type {m.tensors[ti].ggml_type})")
    }
    if (src_off % align != 0l || expect_n % align != 0l) {
        panic("gguf: tensor '{name}' {type_name} slice [{src_off}, +{expect_n}) is not block-aligned ({align})")
    }
    if (src_off + expect_n > m.tensors[ti].n_elem) {
        panic("gguf: tensor '{name}' slice [{src_off}, {src_off + expect_n}) exceeds {m.tensors[ti].n_elem} elems")
    }
    return ti
}

//! Transcode tensor `name`'s [src_off, +expect_n) slice - a whole number of `fmt`'s stride units
//! (256-weight superblocks; q51's 32-blocks) - into the format's planes at plane-local element
//! offset `eloff`: kq_transcode_p per unit, threaded over units (~1MB of disk bytes per job).
def gguf_transcode_kq(m : GGUFMeta; srcbytes : array<uint8> | #; name : string; fmt : KqFmt; var kq : array<uint8>; var ks : array<uint8>; eloff, expect_n : int64; src_off : int64 = 0l) {
    let d = kq_desc(fmt)
    let ti = kq_transcode_check(m, name, d.ggml_type, ggml_type_name(d.ggml_type), src_off, expect_n, d.elems)
    let nb = expect_n / d.elems
    if (nb <= 0l) {
        return
    }
    if (eloff % d.elems != 0l) {
        panic("gguf: tensor '{name}' {fmt} plane offset {eloff} is not block-aligned ({d.elems})")
    }
    let unit0 = eloff / d.elems
    let disk = d.disk_bytes
    let qsb = d.qsb
    let ssb = d.ssb
    guard_dst(name, "quant plane", unit0 * qsb, nb * qsb, long_length(kq))
    guard_dst(name, "scale plane", unit0 * ssb, nb * ssb, long_length(ks))
    with_tensor_view(m, srcbytes, ti) $(bytes, tbase) {
        unsafe {
            let srcp = addr<uint8 const?>(bytes[tbase + (src_off / d.elems) * disk])
            var kqp = addr(kq[unit0 * qsb])
            var ksp = addr(ks[unit0 * ssb])
            maybe_parallel_for(0, int(nb), transcode_jobs(nb, disk)) $(rb, re) {
                kq_transcode_units(fmt, srcp, kqp, ksp, int64(rb), int64(re), disk, qsb, ssb)
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
    } elif (gguf_kq_fmt_of_type(gtype) != KqFmt.q8) {
        // transcode one superblock into scratch planes + its plane dequant: the native load's own two steps, value for value
        if (src_off % 256l != 0l || expect_n % 256l != 0l) {
            panic("gguf: tensor '{name}' superblock slice [{src_off}, +{expect_n}) is not superblock-aligned")
        }
        let sbb = ggml_type_bytes(gtype, 256l)
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
            let d = e8m0_half(uint(bytes[bo]))
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
    let ti = kq_transcode_check(m, name, GGML_TYPE_BF16, "BF16", 0l, expect_n, 1l)
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
    // src_off (32-aligned element offset) transcodes a sub-range — preserves the original Q8 blocks
    // when splitting a fused tensor (Phi3 attn_qkv / gate_up), so there's no lossy fp32 requantize
    let ti = kq_transcode_check(m, name, GGML_TYPE_Q8_0, "Q8_0", src_off, expect_n, 32l)
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
    let ti = kq_transcode_check(m, name, GGML_TYPE_Q5_0, "Q5_0", src_off, expect_n, 32l)
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
    let ti = kq_transcode_check(m, name, GGML_TYPE_Q4_0, "Q4_0", src_off, expect_n, 32l)
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
    let ti = kq_transcode_check(m, name, GGML_TYPE_MXFP4, "MXFP4", src_off, expect_n, 32l)
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
