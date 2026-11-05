options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_safetensors shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_gguf
require daslib/fio
require daslib/array_boost
require daslib/json_boost
require strings
require math
require daslib/f16_cvt   // f16_to_f32 - the deltanet smalls read off the checkpoint

//! one tensor of a safetensors file: its dtype spelling, its shape, and its byte range off the file's data start
struct SafetensorsTensor {
    name : string
    dtype : string
    shape : array<int64>
    begin : int64
    end : int64
}

//! a safetensors file's directory: where its data starts (the 8-byte length word plus the JSON header) and its tensors
struct SafetensorsFile {
    data_off : int64
    tensors : array<SafetensorsTensor>
    index : table<string; int>   //! a tensor's row in `tensors` by name
}

//! the directory of the safetensors file whose bytes are `bytes`; a header that is not the format's panics
def parse_safetensors(bytes : array<uint8> | #) : SafetensorsFile {
    var f : SafetensorsFile
    if (long_length(bytes) < 8l) {
        panic("dasLLAMA safetensors: {long_length(bytes)} bytes hold no header length")
    }
    let n = int64(rd_u64(bytes, 0l))
    if (n <= 0l || 8l + n > long_length(bytes)) {
        panic("dasLLAMA safetensors: header length {n} runs past the file's {long_length(bytes)} bytes")
    }
    f.data_off = 8l + n
    var text = ""
    unsafe {
        text = string(temp_array(addr<uint8?>(bytes[8]), n, type<uint8>))
    }
    f.tensors <- safetensors_entries(text, long_length(bytes) - f.data_off)
    f.index <- { for (ti, t in count(), f.tensors); t.name => ti }
    return <- f
}

//! the header JSON's tensors, each held inside the `data_bytes`-byte data section (a plain function: the JSON variant
//! tests resolve here, not in the caller a generic instantiates in)
def private safetensors_entries(text : string; data_bytes : int64) : array<SafetensorsTensor> {
    var err = ""
    var js = read_json(text, err)
    if (js == null || !(js is _object)) {
        panic("dasLLAMA safetensors: the header is no JSON object ({err})")
    }
    var out : array<SafetensorsTensor>
    for (k, v in keys(js as _object), values(js as _object)) {
        continue if (k == "__metadata__")
        out |> emplace(safetensors_entry(k, v, data_bytes))
    }
    return <- out
}

//! one header entry's tensor, its byte range held inside the `data_bytes`-byte data section
def private safetensors_entry(name : string; v : JsonValue?; data_bytes : int64) : SafetensorsTensor {
    var t = SafetensorsTensor(name = name, dtype = v?["dtype"] ?? "")
    let sh = v?["shape"]
    if (sh is _array) {
        t.shape <- [for (e in sh as _array); e ?? -1l]
    }
    let offs = v?["data_offsets"]
    if (!(offs is _array) || length(offs as _array) != 2) {
        panic("dasLLAMA safetensors: tensor '{name}' carries no [begin, end] data_offsets")
    }
    t.begin = (offs as _array)[0] ?? -1l
    t.end = (offs as _array)[1] ?? -1l
    if (empty(t.dtype) || t.begin < 0l || t.end < t.begin || t.end > data_bytes) {
        panic("dasLLAMA safetensors: tensor '{name}' ({t.dtype}) spans [{t.begin}, {t.end}) of a {data_bytes}-byte data section")
    }
    let esz = safetensors_dtype_bytes(t.dtype)
    if (esz > 0l) {   //! a dtype no reader takes refuses by name later; a known one must span exactly its shape's bytes
        var n = 1l
        for (d in t.shape) {
            n = d < 0l ? -1l : (n < 0l ? -1l : n * d)
        }
        if (n < 0l || n * esz != t.end - t.begin) {
            panic("dasLLAMA safetensors: tensor '{name}' ({t.dtype}, shape {t.shape}) spans {t.end - t.begin} bytes, its shape {n * esz}")
        }
    }
    return <- t
}

//! the bytes one element of a safetensors `dtype` takes; 0 where the loader reads no such dtype
def private safetensors_dtype_bytes(dtype : string) : int64 {
    return 4l if (dtype == "F32" || dtype == "I32" || dtype == "U32")
    return 2l if (dtype == "F16" || dtype == "BF16" || dtype == "I16" || dtype == "U16")
    return 1l if (dtype == "U8" || dtype == "I8" || dtype == "BOOL")
    return 8l if (dtype == "F64" || dtype == "I64" || dtype == "U64")
    return 0l
}

//! a HuggingFace snapshot directory - a `config.json` beside the weights - which the loader reads in place of a GGUF
def hf_snapshot(path : string) : bool {
    let st = stat(path)
    return st.is_valid && st.is_dir && fexist(path_join(path, "config.json"))
}

def private json_file(path : string) : JsonValue? {
    var err = ""
    var js = read_json(fread(path), err)
    if (js == null) {
        panic("dasLLAMA: '{path}' is not JSON ({err})")
    }
    return js
}

//! the snapshot's weight files: the distinct files `model.safetensors.index.json` maps, sorted, else `model.safetensors`
def hf_shard_files(dir : string) : array<string> {
    var files : array<string>
    let index = path_join(dir, "model.safetensors.index.json")
    if (fexist(index)) {
        let wm = json_file(index)?["weight_map"]
        if (wm is _object) {
            for (v in values(wm as _object)) {
                let f : string = v ?? ""
                if (!empty(f) && find_index(files, f) < 0) {
                    files |> push(f)
                }
            }
        }
        sort(files)
    }
    if (empty(files)) {
        files |> push("model.safetensors")
    }
    var paths <- [for (f in files); path_join(dir, f)]
    for (p in paths) {
        if (!fexist(p)) {
            panic("dasLLAMA: snapshot '{dir}' is missing weight file '{p}'")
        }
    }
    return <- paths
}

def private cfg_num(c : JsonValue?; key : string) : double {
    let v = c?[key]
    if (v is _longint) {
        return double(v as _longint)
    }
    if (v is _number) {
        return v as _number
    }
    panic("dasLLAMA: config.json carries no number '{key}'")
    return 0.0lf
}

def private cfg_int(c : JsonValue?; key : string) : int64 => int64(cfg_num(c, key))

def private cfg_has(c : JsonValue?; key : string) : bool {
    let v = c?[key]
    return v != null && !(v is _null)
}

def private cfg_int_or(c : JsonValue?; key : string; dflt : int64) : int64 => cfg_has(c, key) ? cfg_int(c, key) : dflt

//! a token id the config spells as one integer or as a list of them (Llama-3 writes `eos_token_id` as a list): the first
def private cfg_token_id_or(c : JsonValue?; key : string; dflt : int64) : int64 {
    let v = c?[key]
    if (v != null && v.value is _array) {
        for (e in v.value as _array) {
            return e ?? dflt
        }
        return dflt
    }
    return cfg_int_or(c, key, dflt)
}

//! the GGUF arch a checkpoint's `model_type` loads as, and the config object that carries its text model's geometry
struct HfFamily {
    arch : string
    prefix : string       //! the tensor-name prefix the text model's tensors carry
    text_cfg : string     //! the config key nesting the text model's geometry, "" where it is top-level
    mtp_prefix : string   //! the prefix of the draft head's tensors, read as the NextN block; "" where the family reads none
    trunk : int64         //! the trunk's layer count: the NextN block's index
}

def private hf_family(model_type : string) : HfFamily {
    if (model_type == "llama") {
        return HfFamily(arch = "llama", prefix = "", text_cfg = "")
    }
    if (model_type == "qwen3_5") {
        return HfFamily(arch = "qwen35", prefix = "language_model.", text_cfg = "text_config")
    }
    if (model_type == "qwen3_5_moe") {
        return HfFamily(arch = "qwen35moe", prefix = "language_model.", text_cfg = "text_config")
    }
    if (model_type == "qwen4_exp") {
        return HfFamily(arch = "qwen4exp", prefix = "language_model.", text_cfg = "text_config", mtp_prefix = "mtp.")
    }
    panic("dasLLAMA: config.json model_type '{model_type}' has no safetensors loader family (llama, qwen3_5, qwen3_5_moe, qwen4_exp)")
    return HfFamily()
}

//! a layer parameter's GGUF stem, by the checkpoint's module path after `layers.<n>.`
let private HF_LAYER_NAMES <- {
    "input_layernorm" => "attn_norm",
    "self_attn.q_proj" => "attn_q", "self_attn.k_proj" => "attn_k", "self_attn.v_proj" => "attn_v",
    "self_attn.o_proj" => "attn_output", "self_attn.q_norm" => "attn_q_norm", "self_attn.k_norm" => "attn_k_norm",
    "mlp.gate_proj" => "ffn_gate", "mlp.up_proj" => "ffn_up", "mlp.down_proj" => "ffn_down",
    "linear_attn.in_proj_qkv" => "attn_qkv", "linear_attn.in_proj_z" => "attn_gate",
    "linear_attn.in_proj_a" => "ssm_alpha", "linear_attn.in_proj_b" => "ssm_beta",
    "linear_attn.out_proj" => "ssm_out", "linear_attn.norm" => "ssm_norm",
    "mlp.gate" => "ffn_gate_inp", "mlp.shared_expert_gate" => "ffn_gate_inp_shexp",
    "mlp.shared_expert.gate_proj" => "ffn_gate_shexp", "mlp.shared_expert.up_proj" => "ffn_up_shexp",
    "mlp.shared_expert.down_proj" => "ffn_down_shexp",
    "mlp.switch_mlp.gate_proj" => "ffn_gate_exps", "mlp.switch_mlp.up_proj" => "ffn_up_exps",
    "mlp.switch_mlp.down_proj" => "ffn_down_exps",
    "attn_hyper_connection.input_mix_weight_down" => "hc_attn_down", "attn_hyper_connection.input_mix_weight_up" => "hc_attn_up",
    "attn_hyper_connection.hc_norm" => "hc_attn_norm", "attn_hyper_connection.block_inject_weight" => "hc_attn_inject",
    "mlp_hyper_connection.input_mix_weight_down" => "hc_ffn_down", "mlp_hyper_connection.input_mix_weight_up" => "hc_ffn_up",
    "mlp_hyper_connection.hc_norm" => "hc_ffn_norm", "mlp_hyper_connection.block_inject_weight" => "hc_ffn_inject",
    "self_attn.indexer.q_layernorm" => "indexer.q_norm", "self_attn.indexer.k_layernorm" => "indexer.k_norm",
    "ple.key_proj" => "ple_key", "ple.value_proj" => "ple_value", "ple.conv1d" => "ple_conv1d",
    "ple.norm_conv" => "ple_norm_conv", "ple.norm_key" => "ple_norm_key", "ple.norm_query" => "ple_norm_query"
}

//! a head mixer's part (the trunk's `output_hc_*`, the draft head's `nextn.hc_head_*`), by its module name in the mixer
let private HF_MIXER_PART <- { "hc_norm" => "norm", "input_mix_weight_down" => "down", "input_mix_weight_up" => "up" }

//! the checkpoint tensors the meta does not rename because a synthesis reads them: the deltanet smalls (`hf_qwen35_deltanet`),
//! the indexer's fused projection (`hf_indexer_split`), the n-gram hash constants (`put_qwen4exp_kvs`) and the draft head's two
//! input projections (`hf_mtp_eh_proj`)
let private HF_SIDE_READS <- ["linear_attn.A_log", "linear_attn.dt_bias", "linear_attn.conv1d", "self_attn.indexer.index_qk_proj",
    "ple.ple_embedding.layer_multipliers", "ple.ple_embedding.ngram_heads_offsets", "ple.ple_embedding.ngram_heads_vocab_sizes",
    "fc_embedding", "fc_hidden"]

//! the module path of one of the n-gram table's row shards (`...ngram_embedding.shards.<i>`)
let private HF_NGRAM_SHARD = ".ple.ple_embedding.ngram_embedding.shards."

//! a checkpoint module a synthesis reads in place of a rename
def private hf_side_read(base : string) : bool {
    for (side in HF_SIDE_READS) {
        return true if (ends_with(base, ".{side}"))
    }
    return find(base, HF_NGRAM_SHARD) >= 0
}

//! the norm after attention: the llama family's ffn_norm, the qwen35 family's post_attention_norm
def private hf_post_attn_norm(fam : HfFamily) : string => fam.arch == "llama" ? "ffn_norm" : "post_attention_norm"

//! a checkpoint module path's GGUF stem (`model.layers.3.mlp.up_proj` -> `blk.3.ffn_up`, the draft head's `mtp.layers.0.*` ->
//! `blk.<trunk>.*`); "" for a module the text model does not read (a vision tower)
def hf_gguf_stem(fam : HfFamily; module_path : string) : string {
    if (!empty(fam.mtp_prefix) && starts_with(module_path, fam.mtp_prefix)) {
        return hf_mtp_stem(fam, slice(module_path, length(fam.mtp_prefix)))
    }
    return "" if (!starts_with(module_path, fam.prefix))
    let p = slice(module_path, length(fam.prefix))
    return "token_embd" if (p == "model.embed_tokens")
    return "output_norm" if (p == "model.norm")
    return "output" if (p == "lm_head")
    if (starts_with(p, "model.hyper_connection_mixer.")) {
        let part = HF_MIXER_PART?[slice(p, length("model.hyper_connection_mixer."))] ?? ""
        return empty(part) ? "" : "output_hc_{part}"
    }
    return "" if (!starts_with(p, "model.layers."))
    return hf_layer_stem(fam, slice(p, length("model.layers.")), "")
}

//! `rest` = `<n>.<module>`: the layer module's GGUF stem under block `<n>`, or under block `at` where it is not ""
def private hf_layer_stem(fam : HfFamily; rest, at : string) : string {
    let dot = find(rest, ".")
    return "" if (dot <= 0)
    let layer = empty(at) ? slice(rest, 0, dot) : at
    let what = slice(rest, dot + 1)
    let stem = what == "post_attention_layernorm" ? hf_post_attn_norm(fam) : (HF_LAYER_NAMES?[what] ?? "")
    return empty(stem) ? "" : "blk.{layer}.{stem}"
}

//! a draft-head module's GGUF stem (`q` after the head's prefix): its one layer as block `trunk`, its two input norms and its
//! head mixer as that block's `nextn.*`; "" for a module it does not read
def private hf_mtp_stem(fam : HfFamily; q : string) : string {
    let blk = "blk.{fam.trunk}"
    return "{blk}.nextn.enorm" if (q == "pre_fc_norm_embedding")
    return "{blk}.nextn.hnorm" if (q == "pre_fc_norm_hidden")
    if (starts_with(q, "hyper_connection_mixer.")) {
        let part = HF_MIXER_PART?[slice(q, length("hyper_connection_mixer."))] ?? ""
        return empty(part) ? "" : "{blk}.nextn.hc_head_{part}"
    }
    return "" if (!starts_with(q, "layers.0."))
    return hf_layer_stem(fam, slice(q, length("layers.")), "{fam.trunk}")
}

def private ggml_type_of_dtype(dtype, name : string) : int {
    return GGML_TYPE_F16 if (dtype == "F16")
    return GGML_TYPE_BF16 if (dtype == "BF16")
    return GGML_TYPE_F32 if (dtype == "F32")
    panic("dasLLAMA safetensors: tensor '{name}' is {dtype} - the loader reads F16, BF16, F32 and the U32 affine triplets")
    return -1
}

def private reversed_dims(shape : array<int64>) : array<int64> => <- [for (i in range(length(shape))); shape[length(shape) - 1 - i]]

def private dims_elems(dims : array<int64>) : int64 {
    var n = 1l
    for (d in dims) {
        n *= d
    }
    return n
}

//! where a checkpoint tensor's bytes live: its file and its directory row
struct private HfLoc {
    shard : int
    t : int
}

//! the meta's row for the checkpoint tensor at `at` (from byte `skip` of it), under its GGUF `name`, loader type and logical dims
def private hf_push_info(var m : GGUFMeta; files : array<SafetensorsFile>; at : HfLoc; name : string; ggml_type : int; dims : array<int64>; skip : int64 = 0l) {
    var ti = GGUFTensorInfo(name = name, ggml_type = ggml_type, offset = files[at.shard].tensors[at.t].begin + skip,
        n_elem = dims_elems(dims), shard = at.shard)
    ti.dims := dims
    m.tensors |> emplace(ti)
}

//! the file and row of checkpoint tensor `name`; shard -1 where no file carries it
def private hf_locate(files : array<SafetensorsFile>; name : string) : HfLoc {
    for (sh, f in count(), files) {
        let ti = f.index?[name] ?? -1
        return HfLoc(shard = sh, t = ti) if (ti >= 0)
    }
    return HfLoc(shard = -1)
}

//! the triplet's three shapes agree: one rank (2, or 3 for an expert stack), the codes' leading dims the scales', the
//! biases the scales' shape
def private affine_shapes_ok(ws, ss, bs : array<int64>) : bool {
    let r = length(ws)
    return false if ((r != 2 && r != 3) || length(ss) != r || length(bs) != r)
    for (i in range(r)) {
        return false if (bs[i] != ss[i] || (i < r - 1 && ws[i] != ss[i]))
    }
    return true
}

//! the (bits, group) the checkpoint's `quantization` block `q` names for module `hf_base`: its own entry where it carries one
//! (`own`), else the block's defaults
def private hf_quant_of(q : JsonValue?; hf_base : string) : tuple<bits : int64; group : int64; own : bool> {
    let bits = cfg_int_or(q, "bits", 4l)
    let group = cfg_int_or(q, "group_size", 64l)
    let e = q?[hf_base]
    return e is _object ? (bits = cfg_int_or(e, "bits", bits), group = cfg_int_or(e, "group_size", group), own = true) : (bits = bits, group = group, own = false)
}

//! the triplet `<stem>.weight` (U32 codes) + `.scales` + `.biases` as one logical `[in, out]` tensor (`[in, out, experts]` for
//! an expert-major stack) of its member's loader type, its planes beside it as `<stem>.scales` / `.biases`, its group off
//! `quant`; with `nrows` > 0 only rows [row0, +nrows) of a 2-D triplet - that row offset into each of its three planes
def private hf_push_affine(var m : GGUFMeta; files : array<SafetensorsFile>; hf_base, stem : string; quant : JsonValue?; row0 : int64 = 0l; nrows : int64 = 0l) {
    let qa = hf_quant_of(quant, hf_base)
    let group = qa.group
    let wl = hf_locate(files, "{hf_base}.weight")
    let sl = hf_locate(files, "{hf_base}.scales")
    let bl = hf_locate(files, "{hf_base}.biases")
    if (sl.shard < 0 || bl.shard < 0) {
        panic("dasLLAMA safetensors: '{hf_base}.weight' is U32 codes without its .scales and .biases planes")
    }
    let ws := files[wl.shard].tensors[wl.t].shape
    let ss := files[sl.shard].tensors[sl.t].shape
    let bs := files[bl.shard].tensors[bl.t].shape
    let sdt = files[sl.shard].tensors[sl.t].dtype
    let bdt = files[bl.shard].tensors[bl.t].dtype
    if (!affine_shapes_ok(ws, ss, bs) || sdt != bdt) {
        panic("dasLLAMA safetensors: affine triplet '{hf_base}' has codes {ws}, scales {ss} {sdt}, biases {bs} {bdt}")
    }
    let last = length(ws) - 1
    let cols = ss[last] * group
    let bits = ws[last] * 32l / cols
    if (ws[last] * 32l != bits * cols || (qa.own && bits != qa.bits)) {
        panic("dasLLAMA safetensors: affine triplet '{hf_base}' packs {ws[last]} words a row over {cols} weights (group {group}; its quantization entry says {qa.bits}-bit)")
    }
    var dims <- reversed_dims(ws)
    dims[0] = cols
    var sdims <- reversed_dims(ss)
    var qskip = 0l
    var sskip = 0l
    if (nrows > 0l) {
        if (last != 1 || row0 < 0l || row0 + nrows > ws[0]) {
            panic("dasLLAMA safetensors: rows [{row0}, +{nrows}) of affine triplet '{hf_base}' ({ws}) are no 2-D row window")
        }
        dims[1] = nrows
        sdims[1] = nrows
        qskip = row0 * ws[1] * 4l
        sskip = row0 * ss[1] * 2l
    }
    let gt = mlx_affine_ggml_type(int(bits), int(group), sdt == "BF16")
    if (gt < 0 || (sdt != "F16" && sdt != "BF16")) {
        panic("dasLLAMA safetensors: affine triplet '{hf_base}' is {bits}-bit, group {group}, {sdt} scales - the loader serves 4- and 8-bit codes in 64-groups under F16 or BF16 scales, 6-bit codes under BF16")
    }
    let half = ggml_type_of_dtype(sdt, "{hf_base}.scales")
    hf_push_info(m, files, wl, "{stem}.weight", gt, dims, qskip)
    hf_push_info(m, files, sl, "{stem}.scales", half, sdims, sskip)
    hf_push_info(m, files, bl, "{stem}.biases", half, sdims, sskip)
}

//! a text-model tensor (`model.*` under the family's prefix) or a draft-head tensor (under its prefix) that maps to no GGUF
//! name and no synthesis read panics by name: the loader never leaves a parameter of the forward unread
def private hf_refuse_unmapped(fam : HfFamily; name : string) {
    var base = name
    for (sfx in [".weight", ".scales", ".biases"]) {
        if (ends_with(name, sfx)) {
            base = slice(name, 0, length(name) - length(sfx))
            break
        }
    }
    let read = starts_with(base, "{fam.prefix}model.") || (!empty(fam.mtp_prefix) && starts_with(base, fam.mtp_prefix))
    return if (!read || !empty(hf_gguf_stem(fam, base)) || hf_side_read(base))
    panic("dasLLAMA safetensors: the checkpoint's '{name}' has no GGUF name the {fam.arch} loader reads")
}

//! the RMSNorm gains a converter writes as 1 + w where a zero-centred checkpoint stores w - every gain the forward multiplies
//! by, the deltanet's gated `ssm_norm` aside (both store it plain)
let private HF_UNIT_NORMS <- ["attn_norm", "post_attention_norm", "output_norm", "attn_q_norm", "attn_k_norm", "hc_attn_norm",
    "hc_ffn_norm", "output_hc_norm", "indexer.q_norm", "indexer.k_norm", "ple_norm_conv", "ple_norm_key", "ple_norm_query",
    "nextn.enorm", "nextn.hnorm", "nextn.hc_head_norm"]

//! GGUF stem `stem` (`blk.<n>.` dropped) is one of the unit-offset gains
def private hf_unit_norm(stem : string) : bool {
    var tail = stem
    if (starts_with(stem, "blk.")) {
        let dot = find(stem, ".", 4)
        tail = dot > 0 ? slice(stem, dot + 1) : stem
    }
    return find_index(HF_UNIT_NORMS, tail) >= 0
}

//! the mean of a q_norm gain below which the checkpoint stores its gains zero-centred (near 0 there, near 1 or above where
//! the 1 is baked in)
let private HF_ZERO_CENTRED_MEAN = 0.75lf

//! the checkpoint stores its RMSNorm gains zero-centred: the first trunk layer's q_norm, probed by its mean; a checkpoint with
//! no q_norm stores plain gains
def private hf_zero_centred(files : array<SafetensorsFile>; bases : array<void?>; fam : HfFamily) : bool {
    for (l in range64(fam.trunk)) {
        let name = "{fam.prefix}model.layers.{l}.self_attn.q_norm.weight"
        continue if (hf_locate(files, name).shard < 0)
        var inscope w <- hf_read_f32(files, bases, name)
        var sum = 0.0lf
        for (x in w) {
            sum += double(x)
        }
        return sum / double(max(1, length(w))) < HF_ZERO_CENTRED_MEAN
    }
    return false
}

//! a plain tensor's logical dims: the checkpoint shape reversed, a trailing unit dim of a 3-D one (a depthwise conv's
//! `[channels, taps, 1]`) dropped
def private hf_plain_dims(shape : array<int64>) : array<int64> {
    var dims <- reversed_dims(shape)
    if (length(dims) == 3 && dims[0] == 1l) {
        dims |> erase(0)
    }
    return <- dims
}

//! every checkpoint tensor the text model reads, renamed to its GGUF name: affine triplets as one logical tensor (their group
//! off `quant`), a unit-offset gain of a `zero_centred` checkpoint as the f32 1 + w the meta synthesizes into `synth`, the
//! rest as their dtype
def private hf_push_tensors(var m : GGUFMeta; var synth : array<uint8>; synth_shard : int; files : array<SafetensorsFile>; bases : array<void?>;
                            fam : HfFamily; quant : JsonValue?; zero_centred : bool) {
    for (sh, f in count(), files) {
        for (ti, t in count(), f.tensors) {
            hf_refuse_unmapped(fam, t.name)
            let suffix = ends_with(t.name, ".weight") ? ".weight" : ""
            continue if (empty(suffix))
            let hf_base = slice(t.name, 0, length(t.name) - length(suffix))
            let stem = hf_gguf_stem(fam, hf_base)
            continue if (empty(stem))
            if (t.dtype == "U32") {
                hf_push_affine(m, files, hf_base, stem, quant)
            } elif (zero_centred && hf_unit_norm(stem)) {
                var inscope w <- hf_read_f32(files, bases, t.name)
                for (x in w) {
                    x++
                }
                gguf_synth_tensor_f32(m, synth, synth_shard, "{stem}.weight", hf_plain_dims(t.shape), w)
            } else {
                hf_push_info(m, files, HfLoc(shard = sh, t = ti), "{stem}.weight", ggml_type_of_dtype(t.dtype, t.name), hf_plain_dims(t.shape))
            }
        }
    }
}

//! the llama3 rotary factors the GGUF converter writes as `rope_freqs.weight`: 1 above the high-frequency wavelength, the
//! scaling factor below the low one, a smooth blend between
def hf_llama3_rope_factors(rs : JsonValue?; theta : double; head_dim : int64) : array<float> {
    let factor = cfg_num(rs, "factor")
    let lo = cfg_num(rs, "low_freq_factor")
    let hi = cfg_num(rs, "high_freq_factor")
    let old_ctx = cfg_num(rs, "original_max_position_embeddings")
    let lo_wave = old_ctx / lo
    let hi_wave = old_ctx / hi
    var out : array<float>
    out |> reserve(head_dim / 2l)
    for (i in range64(head_dim / 2l)) {
        let freq = 1.0lf / pow(theta, double(2l * i) / double(head_dim))
        let wave = 2.0lf * 3.141592653589793lf / freq
        if (wave < hi_wave) {
            out |> push(1.0)
        } elif (wave > lo_wave) {
            out |> push(float(factor))
        } else {
            let smooth = (old_ctx / wave - lo) / (hi - lo)
            out |> push(float(1.0lf / ((1.0lf - smooth) / factor + smooth)))
        }
    }
    return <- out
}

def private put_attention_kvs(var m : GGUFMeta; var buf : array<uint8>; arch : string; c : JsonValue?) {
    let heads = cfg_int(c, "num_attention_heads")
    let dim = cfg_int(c, "hidden_size")
    let head_dim = cfg_int_or(c, "head_dim", dim / heads)
    gguf_kv_put_int(m, buf, "{arch}.context_length", cfg_int(c, "max_position_embeddings"))
    gguf_kv_put_int(m, buf, "{arch}.embedding_length", dim)
    gguf_kv_put_int(m, buf, "{arch}.block_count", cfg_int(c, "num_hidden_layers"))
    if (cfg_has(c, "intermediate_size")) {
        gguf_kv_put_int(m, buf, "{arch}.feed_forward_length", cfg_int(c, "intermediate_size"))
    }
    gguf_kv_put_int(m, buf, "{arch}.attention.head_count", heads)
    gguf_kv_put_int(m, buf, "{arch}.attention.head_count_kv", cfg_int_or(c, "num_key_value_heads", heads))
    gguf_kv_put_int(m, buf, "{arch}.attention.key_length", head_dim)
    gguf_kv_put_int(m, buf, "{arch}.attention.value_length", head_dim)
    gguf_kv_put_f32(m, buf, "{arch}.attention.layer_norm_rms_epsilon", float(cfg_num(c, "rms_norm_eps")))
}

//! the llama family's KVs and its llama3 rotary factor tensor
def private put_llama_kvs(var m : GGUFMeta; var buf : array<uint8>; synth_shard : int; c : JsonValue?) {
    put_attention_kvs(m, buf, "llama", c)
    let dim = cfg_int(c, "hidden_size")
    let head_dim = cfg_int_or(c, "head_dim", dim / cfg_int(c, "num_attention_heads"))
    let theta = cfg_num(c, "rope_theta")
    gguf_kv_put_int(m, buf, "llama.rope.dimension_count", head_dim)
    gguf_kv_put_f32(m, buf, "llama.rope.freq_base", float(theta))
    let rs = c?["rope_scaling"]
    if (rs is _object) {
        let kind : string = rs?["rope_type"] ?? (rs?["type"] ?? "")
        if (kind != "llama3") {
            panic("dasLLAMA: config.json rope_scaling type '{kind}' has no safetensors loader arm (llama3)")
        }
        let factors <- hf_llama3_rope_factors(rs, theta, head_dim)
        gguf_synth_tensor_f32(m, buf, synth_shard, "rope_freqs.weight", [head_dim / 2l], factors)
    }
    m.hf_rope_halves = true
}

//! the qwen35 hybrid family's KVs under `arch` (qwen35, qwen35moe): the attention geometry, the partial rotary span and its
//! sections, the deltanet geometry, and a routed FFN's expert geometry
def private put_qwen35_kvs(var m : GGUFMeta; var buf : array<uint8>; arch : string; c : JsonValue?) {
    put_attention_kvs(m, buf, arch, c)
    let rp = c?["rope_parameters"]
    let head_dim = cfg_int(c, "head_dim")
    let partial = cfg_has(rp, "partial_rotary_factor") ? cfg_num(rp, "partial_rotary_factor") : 1.0lf
    gguf_kv_put_int(m, buf, "{arch}.rope.dimension_count", int64(double(head_dim) * partial))
    gguf_kv_put_f32(m, buf, "{arch}.rope.freq_base", float(cfg_num(rp, "rope_theta")))
    let sect = rp?["mrope_section"]
    if (sect is _array) {
        var s <- [for (e in sect as _array); e ?? 0l]
        while (length(s) < 4) {
            s |> push(0l)
        }
        gguf_kv_put_i32_array(m, buf, "{arch}.rope.dimension_sections", s)
    }
    gguf_kv_put_int(m, buf, "{arch}.full_attention_interval", cfg_int_or(c, "full_attention_interval", 4l))
    gguf_kv_put_int(m, buf, "{arch}.ssm.conv_kernel", cfg_int(c, "linear_conv_kernel_dim"))
    gguf_kv_put_int(m, buf, "{arch}.ssm.state_size", cfg_int(c, "linear_key_head_dim"))
    gguf_kv_put_int(m, buf, "{arch}.ssm.group_count", cfg_int(c, "linear_num_key_heads"))
    gguf_kv_put_int(m, buf, "{arch}.ssm.time_step_rank", cfg_int(c, "linear_num_value_heads"))
    gguf_kv_put_int(m, buf, "{arch}.ssm.inner_size", cfg_int(c, "linear_value_head_dim") * cfg_int(c, "linear_num_value_heads"))
    if (cfg_int_or(c, "num_experts", 0l) > 0l) {
        put_moe_kvs(m, buf, arch, c)
    }
}

//! a routed FFN's geometry as the converter's expert KVs: the expert count, top-k, the expert and shared-expert widths and the
//! top-k renorm; a layout the family's graph does not run (sparse steps, dense-only layers) panics by name
[arch(at="../ARCHITECTURE_ENGINE_FORMATS_MLX.md#mlx-moe-snapshot")]
def private put_moe_kvs(var m : GGUFMeta; var buf : array<uint8>; arch : string; c : JsonValue?) {
    let mlp_only = c?["mlp_only_layers"]
    if (cfg_int_or(c, "decoder_sparse_step", 1l) != 1l || (mlp_only is _array && !empty(mlp_only as _array))) {
        panic("dasLLAMA: config.json routes a sparse layer subset (decoder_sparse_step / mlp_only_layers) - the {arch} graph routes every layer")
    }
    gguf_kv_put_int(m, buf, "{arch}.expert_count", cfg_int(c, "num_experts"))
    gguf_kv_put_int(m, buf, "{arch}.expert_used_count", cfg_int(c, "num_experts_per_tok"))
    gguf_kv_put_int(m, buf, "{arch}.expert_feed_forward_length", cfg_int(c, "moe_intermediate_size"))
    let shexp = cfg_int_or(c, "shared_expert_intermediate_size", 0l)
    if (shexp > 0l) {
        gguf_kv_put_int(m, buf, "{arch}.expert_shared_feed_forward_length", shexp)
    }
    let norm = c?["norm_topk_prob"]
    if (norm is _bool) {
        gguf_kv_put_int(m, buf, "{arch}.expert_weights_norm", (norm as _bool) ? 1l : 0l)
    }
}

//! qwen4exp's KVs off text config `c` and top-level `top`: the qwen35moe hybrid's, the hyper-connections', the indexer's (its
//! ratio on full-attention layers), the n-gram input's (layer zero-indexed, hash constants off the I64 tensors), the draft head's
//! count as a split head's; a σ(z)-less out-gate or a multi-head indexer key panics by name
[arch(at="../ARCHITECTURE_ENGINE_FORMATS_MLX.md#mlx-qwen4exp-snapshot")]
def private put_qwen4exp_kvs(var m : GGUFMeta; var buf : array<uint8>; files : array<SafetensorsFile>; bases : array<void?>; fam : HfFamily;
                             c, top : JsonValue?) {
    let arch = fam.arch
    put_qwen35_kvs(m, buf, arch, c)
    let gate : string = c?["output_gate_type"] ?? ""
    if (gate != "sigmoid") {
        panic("dasLLAMA: config.json output_gate_type '{gate}' - the {arch} graph gates the deltanet output with sigmoid(z)")
    }
    gguf_kv_put_int(m, buf, "{arch}.hyper_connection.count", cfg_int(c, "hc_count"))
    gguf_kv_put_int(m, buf, "{arch}.hyper_connection.low_rank", cfg_int(c, "hc_lowrank"))
    if (cfg_int_or(c, "indexer_kv_heads", 1l) != 1l) {
        panic("dasLLAMA: config.json indexer_kv_heads {cfg_int(c, "indexer_kv_heads")} - the {arch} indexer reads one key head")
    }
    gguf_kv_put_int(m, buf, "{arch}.attention.indexer.head_count", cfg_int(c, "indexer_n_heads"))
    gguf_kv_put_int(m, buf, "{arch}.attention.indexer.key_length", cfg_int(c, "indexer_head_dim"))
    gguf_kv_put_int(m, buf, "{arch}.attention.indexer.top_k", cfg_int(c, "indexer_budget"))
    let ratio = cfg_int(c, "indexer_compress_ratio")
    let lt = c?["layer_types"]
    if (!(lt is _array) || long_length(lt as _array) != fam.trunk) {
        panic("dasLLAMA: config.json layer_types does not list the {fam.trunk} trunk layers")
    }
    let ratios <- [for (e in lt as _array); (e ?? "") == "full_attention" ? ratio : 0l]
    gguf_kv_put_i32_array(m, buf, "{arch}.attention.compress_ratios", ratios)
    let ids = c?["ple_layer_ids"]
    if (!(ids is _array) || length(ids as _array) != 1) {
        panic("dasLLAMA: config.json ple_layer_ids is not one layer - the {arch} graph reads one n-gram layer")
    }
    let pl = ((ids as _array)[0] ?? 0l) - 1l   //! the config counts the n-gram layer from 1
    gguf_kv_put_i32_array(m, buf, "{arch}.ple.layers", [pl])
    let ngram = cfg_int(c, "ngram_size")
    let per = cfg_int(c, "heads_per_ngram")
    let embed = cfg_int(c, "ple_embed_dim")
    if (ngram < 2l || per <= 0l || embed % ((ngram - 1l) * per) != 0l) {
        panic("dasLLAMA: config.json ple_embed_dim {embed} does not split over {ngram - 1l} n-gram orders x {per} heads")
    }
    gguf_kv_put_int(m, buf, "{arch}.ple.ngram_size", ngram)
    gguf_kv_put_int(m, buf, "{arch}.ple.heads_per_ngram", per)
    gguf_kv_put_int(m, buf, "{arch}.ple.conv_kernel", cfg_int(c, "ple_conv_kernel_size"))
    let eos = cfg_token_id_or(c, "eos_token_id", -1l)
    if (eos < 0l) {
        panic("dasLLAMA: config.json carries no eos_token_id for the n-gram side input")
    }
    gguf_kv_put_int(m, buf, "{arch}.ple.eos_token_id", eos)
    gguf_kv_put_int(m, buf, "{arch}.ple.image_token_id", cfg_int(top, "image_token_id"))
    gguf_kv_put_int(m, buf, "{arch}.embedding_length_per_layer_input", embed / ((ngram - 1l) * per))
    let hc = "{fam.prefix}model.layers.{pl}.ple.ple_embedding"
    for (key, src in ["layer_multipliers", "head_offsets", "head_vocab_sizes"], ["layer_multipliers", "ngram_heads_offsets", "ngram_heads_vocab_sizes"]) {
        var inscope v <- hf_read_i64(files, bases, "{hc}.{src}")
        gguf_kv_put_i64_array(m, buf, "{arch}.ple.{key}", v)
    }
    if (hf_locate(files, "{fam.mtp_prefix}fc_embedding.weight").shard >= 0) {
        m.nextn_sidecar = cfg_int_or(c, "mtp_num_hidden_layers", 0l)
    }
}

//! the BPE pre-tokenizer a split regex names: llama3's three-digit runs, qwen35's marks inside letter runs, qwen2's single
//! digits; "" for a regex the encoder has no split for
def hf_bpe_pre(regex : string) : string {
    return "llama-bpe" if (find(regex, "\\p\{N\}\{1,3\}") >= 0)
    return "qwen35" if (find(regex, "\\p\{M\}") >= 0)
    return "qwen2" if (find(regex, "\\p\{N\}") >= 0)
    return ""
}

//! the split regex of a tokenizer.json pre-tokenizer (a `Split`, or the first inside a `Sequence`); "" where it has none
def private split_regex(pt : JsonValue?) : string {
    let kind : string = pt?["type"] ?? ""
    if (kind == "Split") {
        return pt?["pattern"]?["Regex"] ?? ""
    }
    let seq = pt?["pretokenizers"]
    if (kind == "Sequence" && seq is _array) {
        for (p in seq as _array) {
            let r = split_regex(p)
            return r if (!empty(r))
        }
    }
    return ""
}

//! the id of the added token whose content is `content`; -1 where none is
def private added_id(added : JsonValue?; content : string) : int64 {
    if (added is _array) {
        for (a in added as _array) {
            let c : string = a?["content"] ?? ""
            return a?["id"] ?? -1l if (c == content)
        }
    }
    return -1l
}

//! a special token's id off tokenizer_config.json's spelling `key` (a string, or an object with `content`), else the
//! config's `<key>_id` integer, else -1
def private special_token_id(tc, cfg, added : JsonValue?; key : string) : int64 {
    let v = tc?[key]
    let spelled : string = v is _object ? (v?["content"] ?? "") : (v ?? "")
    let id = empty(spelled) ? -1l : added_id(added, spelled)
    return id >= 0l ? id : cfg_token_id_or(cfg, "{key}_id", -1l)
}

//! a GGUF token type of an added token: control where the checkpoint marks it special or its spelling is `<|...|>` (the
//! converter's rule), user-defined otherwise
def private added_token_type(a : JsonValue?) : int64 {
    let c : string = a?["content"] ?? ""
    let special : bool = a?["special"] ?? false
    return special || (starts_with(c, "<|") && ends_with(c, "|>")) ? 3l : 4l
}

//! a tokenizer.json merge list as the GGUF's `left right` strings (the file spells each a string or a pair)
def private hf_merges(mj : JsonValue?) : array<string> {
    var merges : array<string>
    if (mj is _array) {
        merges |> reserve(length(mj as _array))
        for (e in mj as _array) {
            let l : string = e?[0] ?? ""
            let r : string = e?[1] ?? ""
            merges |> push(e is _array ? "{l} {r}" : (e ?? ""))
        }
    }
    return <- merges
}

//! tokenizer.json, tokenizer_config.json and chat_template.jinja as the GGUF tokenizer KVs: the vocab padded to the model's
//! `vocab_size` with UNUSED `[PAD<i>]` rows, the merges, the token types, the pre-tokenizer the split regex names, the
//! special ids and the chat template
def private put_tokenizer_kvs(var m : GGUFMeta; var buf : array<uint8>; dir : string; cfg : JsonValue?) {
    let tj = json_file(path_join(dir, "tokenizer.json"))
    let tc_path = path_join(dir, "tokenizer_config.json")
    let tc = fexist(tc_path) ? json_file(tc_path) : null
    let model = tj?["model"]
    let kind : string = model?["type"] ?? ""
    if (kind != "BPE") {
        panic("dasLLAMA: '{dir}' tokenizer.json is a {kind} model - the snapshot loader reads byte-level BPE")
    }
    let vocab = model?["vocab"]
    let added = tj?["added_tokens"]
    var n = cfg_int_or(cfg, "vocab_size", 0l)
    if (vocab is _object) {
        for (id in values(vocab as _object)) {
            n = max(n, (id ?? -1l) + 1l)
        }
    }
    if (added is _array) {
        for (a in added as _array) {
            n = max(n, (a?["id"] ?? -1l) + 1l)
        }
    }
    var tokens <- [for (i in range64(n)); "[PAD{i}]"]
    var types <- [for (_i in range64(n)); 5l]
    if (vocab is _object) {
        for (piece, id in keys(vocab as _object), values(vocab as _object)) {
            let i : int64 = id ?? -1l
            tokens[i] = piece
            types[i] = 1l
        }
    }
    if (added is _array) {
        for (a in added as _array) {
            let i : int64 = a?["id"] ?? -1l
            tokens[i] = a?["content"] ?? ""
            types[i] = added_token_type(a)
        }
    }
    var merges <- hf_merges(model?["merges"])
    let config_regex : string = tc?["pretokenize_regex"] ?? ""
    let regex = empty(config_regex) ? split_regex(tj?["pre_tokenizer"]) : config_regex
    let pre = hf_bpe_pre(regex)
    if (empty(pre)) {
        panic("dasLLAMA: '{dir}' tokenizer's split regex '{regex}' names no BPE pre-tokenizer the encoder has (llama-bpe, qwen2, qwen35)")
    }
    gguf_kv_put_str(m, buf, "tokenizer.ggml.model", "gpt2")
    gguf_kv_put_str(m, buf, "tokenizer.ggml.pre", pre)
    gguf_kv_put_str_array(m, buf, "tokenizer.ggml.tokens", tokens)
    gguf_kv_put_i32_array(m, buf, "tokenizer.ggml.token_type", types)
    gguf_kv_put_str_array(m, buf, "tokenizer.ggml.merges", merges)
    let bos = special_token_id(tc, cfg, added, "bos_token")
    if (bos >= 0l) {
        gguf_kv_put_int(m, buf, "tokenizer.ggml.bos_token_id", bos)
    }
    gguf_kv_put_int(m, buf, "tokenizer.ggml.eos_token_id", special_token_id(tc, cfg, added, "eos_token"))
    for (key in ["add_bos_token", "add_eos_token"]) {
        let v = tc?[key]
        if (v is _bool) {
            gguf_kv_put_bool(m, buf, "tokenizer.ggml.{key}", v as _bool)
        }
    }
    delete tokens
    delete types
    delete merges
}

//! the chat template the snapshot ships: chat_template.jinja, else tokenizer_config.json's `chat_template` string
def private put_chat_template(var m : GGUFMeta; var buf : array<uint8>; dir : string) {
    let jinja = path_join(dir, "chat_template.jinja")
    var tmpl = fexist(jinja) ? fread(jinja) : ""
    let tc_path = path_join(dir, "tokenizer_config.json")
    if (empty(tmpl) && fexist(tc_path)) {
        tmpl = json_file(tc_path)?["chat_template"] ?? ""
    }
    if (!empty(tmpl)) {
        gguf_kv_put_str(m, buf, "tokenizer.chat_template", tmpl)
    }
}

//! checkpoint tensor `name` (F32, F16 or BF16) as f32 values off its mapped file
def private hf_read_f32(files : array<SafetensorsFile>; bases : array<void?>; name : string) : array<float> {
    let at = hf_locate(files, name)
    if (at.shard < 0) {
        panic("dasLLAMA safetensors: the checkpoint carries no '{name}'")
    }
    let dtype = files[at.shard].tensors[at.t].dtype
    let begin = files[at.shard].data_off + files[at.shard].tensors[at.t].begin
    let nbytes = files[at.shard].tensors[at.t].end - files[at.shard].tensors[at.t].begin
    let width = dtype == "F32" ? 4l : 2l
    var out : array<float>
    out |> resize(nbytes / width)
    unsafe {
        let p = reinterpret<uint8 const?>(bases[at.shard]) + begin
        for (i in range64(nbytes / width)) {
            let h = uint(p[i * width]) | (uint(p[i * width + 1l]) << 8u)
            if (dtype == "F32") {
                out[i] = reinterpret<float>(h | (uint(p[i * 4l + 2l]) << 16u) | (uint(p[i * 4l + 3l]) << 24u))
            } elif (dtype == "BF16") {
                out[i] = reinterpret<float>(h << 16u)
            } else {
                out[i] = f16_to_f32(h)
            }
        }
    }
    return <- out
}

//! checkpoint tensor `name` (I64: a constant a KV carries, never a plane) as its values off its mapped file
def private hf_read_i64(files : array<SafetensorsFile>; bases : array<void?>; name : string) : array<int64> {
    let at = hf_locate(files, name)
    if (at.shard < 0 || files[at.shard].tensors[at.t].dtype != "I64") {
        panic("dasLLAMA safetensors: the checkpoint carries no I64 '{name}'")
    }
    let tt & = unsafe(files[at.shard].tensors[at.t])
    let n = (tt.end - tt.begin) / 8l
    var out : array<int64>
    out |> resize(n)
    unsafe {
        let p = reinterpret<uint8 const?>(bases[at.shard]) + files[at.shard].data_off + tt.begin
        for (i in range64(n)) {
            var v = 0ul
            for (b in range64(8l)) {
                v |= uint64(p[i * 8l + b]) << uint64(b * 8l)
            }
            out[i] = int64(v)
        }
    }
    return <- out
}

//! the indexer's fused projection `index_qk_proj` of every attention layer and of the draft head's layer as the two planes the
//! loader reads: its first `heads x head_dim` rows `indexer.q_proj`, the next `head_dim` rows `indexer.k_proj` - a row offset into
//! the codes, scales and biases alike
def private hf_indexer_split(var m : GGUFMeta; files : array<SafetensorsFile>; fam : HfFamily; c, quant : JsonValue?) {
    let hd = cfg_int(c, "indexer_head_dim")
    let qr = cfg_int(c, "indexer_n_heads") * hd
    var mods <- [for (l in range64(fam.trunk)); (hf = "{fam.prefix}model.layers.{l}", blk = l)]
    mods |> push((hf = "{fam.mtp_prefix}layers.0", blk = fam.trunk))
    for (md in mods) {
        let hf = "{md.hf}.self_attn.indexer.index_qk_proj"
        let at = hf_locate(files, "{hf}.weight")
        continue if (at.shard < 0)
        let rows = files[at.shard].tensors[at.t].shape[0]
        if (rows != qr + hd) {
            panic("dasLLAMA safetensors: '{hf}' holds {rows} rows, not the {qr} query rows and {hd} key rows of the indexer")
        }
        hf_push_affine(m, files, hf, "blk.{md.blk}.indexer.q_proj", quant, 0l, qr)
        hf_push_affine(m, files, hf, "blk.{md.blk}.indexer.k_proj", quant, qr, hd)
    }
}

//! a checkpoint tensor's bytes in its mapped file, its shape and its dtype
struct private HfBytes {
    @do_not_delete p : uint8 const?
    shape : array<int64>
    dtype : string
}

def private hf_tensor_bytes(files : array<SafetensorsFile>; bases : array<void?>; name : string) : HfBytes {
    let at = hf_locate(files, name)
    if (at.shard < 0) {
        panic("dasLLAMA safetensors: the checkpoint carries no '{name}'")
    }
    let tt & = unsafe(files[at.shard].tensors[at.t])
    var out = HfBytes(dtype = tt.dtype)
    out.shape := tt.shape
    unsafe {
        out.p = reinterpret<uint8 const?>(bases[at.shard]) + files[at.shard].data_off + tt.begin
    }
    return <- out
}

//! the draft head's input projection `nextn.eh_proj` over the embedding row then the hidden row: each output row the embedding
//! triplet's row then the hidden triplet's, end to end - code bytes and 64-group pairs verbatim, both halves whole groups of one
//! member - copied into the meta's own buffer `synth`
def private hf_mtp_eh_proj(var m : GGUFMeta; var synth : array<uint8>; synth_shard : int; files : array<SafetensorsFile>; bases : array<void?>;
                           fam : HfFamily; quant : JsonValue?) {
    let halves = ["{fam.mtp_prefix}fc_embedding", "{fam.mtp_prefix}fc_hidden"]
    return if (hf_locate(files, "{halves[0]}.weight").shard < 0)
    var parts : array<HfBytes>
    parts |> reserve(6)
    for (h in halves) {
        for (plane in [".weight", ".scales", ".biases"]) {
            parts |> emplace(hf_tensor_bytes(files, bases, "{h}{plane}"))
        }
    }
    let rows = parts[0].shape[0]
    let qa = hf_quant_of(quant, halves[0])
    let qb = hf_quant_of(quant, halves[1])
    var same = true
    for (pt in parts) {
        same = same && length(pt.shape) == 2 && pt.shape[0] == rows
    }
    let bits = parts[0].shape[1] * 32l / (parts[1].shape[1] * qa.group)
    let gt = mlx_affine_ggml_type(int(bits), int(qa.group), parts[1].dtype == "BF16")
    if (!same || qa.group != qb.group || parts[0].dtype != "U32" || parts[3].dtype != "U32" || gt < 0
            || parts[1].dtype != parts[2].dtype || parts[1].dtype != parts[4].dtype || parts[4].dtype != parts[5].dtype
            || bits * parts[1].shape[1] * qa.group != parts[0].shape[1] * 32l || bits * parts[4].shape[1] * qa.group != parts[3].shape[1] * 32l) {
        panic("dasLLAMA safetensors: the draft head's {halves[0]} and {halves[1]} are no two triplets of one served member over the same rows")
    }
    let cols = (parts[1].shape[1] + parts[4].shape[1]) * qa.group
    let half = ggml_type_of_dtype(parts[1].dtype, "{halves[0]}.scales")
    let stem = "blk.{fam.trunk}.nextn.eh_proj"
    m.tensors |> reserve(length(m.tensors) + 3)
    for (plane, kind in [0, 1, 2], [".weight", ".scales", ".biases"]) {
        synth |> resize((long_length(synth) + 3l) & ~3l)
        let wa = parts[plane].shape[1] * (plane == 0 ? 4l : 2l)
        let wb = parts[plane + 3].shape[1] * (plane == 0 ? 4l : 2l)
        var ti = GGUFTensorInfo(name = "{stem}{kind}", ggml_type = plane == 0 ? gt : half, offset = long_length(synth), shard = synth_shard)
        ti.dims <- plane == 0 ? [cols, rows] : [cols / qa.group, rows]
        ti.n_elem = ti.dims[0] * rows
        let at = long_length(synth)
        synth |> resize(at + rows * (wa + wb))
        unsafe {
            var d = addr(synth[at])
            for (r in range64(rows)) {
                memcpy(d + r * (wa + wb), parts[plane].p + r * wa, wa)
                memcpy(d + r * (wa + wb) + wa, parts[plane + 3].p + r * wb, wb)
            }
        }
        m.tensors |> emplace(ti)
    }
    delete parts
}

//! the n-gram table's row shards (`<layer>.ple.ple_embedding.ngram_embedding.shards.<i>`, 4-bit codes in 32-groups) as one
//! `per_layer_token_embd` tensor over their bytes in place: every shard's rows equal, its codes, scales and biases back to
//! back, the shards back to back in one file (`paths[shard]`, which the model maps for its life); any other layout panics
def private hf_ngram_table(var m : GGUFMeta; files : array<SafetensorsFile>; paths : array<string>; fam : HfFamily; c, quant : JsonValue?) {
    let ids = c?["ple_layer_ids"]
    return if (!(ids is _array) || length(ids as _array) != 1)
    let base = "{fam.prefix}model.layers.{((ids as _array)[0] ?? 0l) - 1l}.ple.ple_embedding.ngram_embedding.shards"
    let first = hf_locate(files, "{base}.0.weight")
    return if (first.shard < 0)
    let sh = first.shard
    let rows = files[sh].tensors[first.t].shape[0]
    let words = files[sh].tensors[first.t].shape[1]
    let qa = hf_quant_of(quant, "{base}.0")
    if (qa.bits <= 0l || qa.group <= 0l) {
        panic("dasLLAMA safetensors: the n-gram table '{base}' carries no quantization width (bits {qa.bits}, group {qa.group})")
    }
    let cols = words * 32l / qa.bits
    let code_bytes = rows * words * 4l
    let pair_bytes = rows * (cols / qa.group) * 2l
    let shard_bytes = code_bytes + 2l * pair_bytes
    let w0 = files[sh].tensors[first.t].begin
    var nshards = 0l
    while (hf_locate(files, "{base}.{nshards}.weight").shard >= 0) {
        let s = nshards
        var inscope at <- [for (plane in [".weight", ".scales", ".biases"]); hf_locate(files, "{base}.{s}{plane}")]
        var ok = qa.bits == 4l && qa.group == 32l && cols * qa.bits == words * 32l
        for (a, i in at, count()) {
            ok = ok && a.shard == sh
            if (ok) {
                let tt & = unsafe(files[sh].tensors[a.t])
                let want = w0 + s * shard_bytes + (i == 0 ? 0l : code_bytes + int64(i - 1) * pair_bytes)
                ok = (tt.begin == want && length(tt.shape) == 2 && tt.shape[0] == rows && tt.dtype == (i == 0 ? "U32" : "BF16")
                    && tt.shape[1] == (i == 0 ? words : cols / qa.group))
            }
        }
        if (!ok) {
            panic("dasLLAMA safetensors: n-gram table shard {s} of '{base}' is not the 4-bit group-32 layout the gather reads ({rows} rows, codes then scales then biases, shards back to back in one file)")
        }
        nshards++
    }
    var ti = GGUFTensorInfo(name = "per_layer_token_embd.weight", ggml_type = GGML_TYPE_NGRAM_A4G32, offset = w0, shard = sh)
    ti.dims <- [cols, nshards * rows]
    ti.n_elem = cols * nshards * rows
    m.tensors |> emplace(ti)
    m.ngram_path = paths[sh]
    m.ngram_shard_rows = rows
}

//! plane `name` read through `order`: its rows, or with `seg_cols` its `seg_cols`-wide column segments
def private put_permute(var m : GGUFMeta; name : string; order : array<int64>; seg_cols : int64) {
    var p = GGUFRowPermute(seg_cols = seg_cols)
    if (seg_cols > 0l) {
        p.segs := order
    } else {
        p.rows := order
    }
    m.permutes |> emplace(name, p)
}

//! the stored (grouped) head of logical value head `vt`: the checkpoint orders value heads by key head (vg = g x nvpk + j), the
//! deltanet kernels tiled (vt = j x nk + g)
def private hf_grouped_head(vt, nk, nvpk : int64) : int64 => (vt % nk) * nvpk + vt / nk

//! the qwen35 deltanet planes in the kernels' tiled value-head order: whole-row permutes of the qkv V rows, z rows and
//! alpha / beta rows, whole 64-group column segments of the out projection, and the per-head smalls - ssm_a (= -exp(A_log)),
//! the dt bias and the conv channels - synthesized as f32 in tiled order
def private hf_qwen35_deltanet(var m : GGUFMeta; var synth : array<uint8>; synth_shard : int; files : array<SafetensorsFile>; bases : array<void?>;
                               fam : HfFamily; c : JsonValue?) {
    let nk = cfg_int(c, "linear_num_key_heads")
    let nvh = cfg_int(c, "linear_num_value_heads")
    let dk = cfg_int(c, "linear_key_head_dim")
    let dv = cfg_int(c, "linear_value_head_dim")
    let nvpk = nvh / nk
    let qkr = 2l * nk * dk
    var vrows <- [for (r in range64(nvh * dv)); hf_grouped_head(r / dv, nk, nvpk) * dv + r % dv]
    var qkv_rows <- [for (r in range64(qkr + nvh * dv)); r < qkr ? r : qkr + vrows[r - qkr]]
    var heads <- [for (h in range64(nvh)); hf_grouped_head(h, nk, nvpk)]
    let layers = cfg_int(c, "num_hidden_layers")
    for (l in range64(layers)) {
        let hf = "{fam.prefix}model.layers.{l}.linear_attn"
        continue if (hf_locate(files, "{hf}.A_log").shard < 0)
        put_permute(m, "blk.{l}.attn_qkv.weight", qkv_rows, 0l)
        put_permute(m, "blk.{l}.attn_gate.weight", vrows, 0l)
        put_permute(m, "blk.{l}.ssm_alpha.weight", heads, 0l)
        put_permute(m, "blk.{l}.ssm_beta.weight", heads, 0l)
        put_permute(m, "blk.{l}.ssm_out.weight", heads, dv)
        var alog <- hf_read_f32(files, bases, "{hf}.A_log")
        var ssm_a <- [for (h in heads); -exp(alog[h])]
        gguf_synth_tensor_f32(m, synth, synth_shard, "blk.{l}.ssm_a", [nvh], ssm_a)
        var dtb <- hf_read_f32(files, bases, "{hf}.dt_bias")
        var dt <- [for (h in heads); dtb[h]]
        gguf_synth_tensor_f32(m, synth, synth_shard, "blk.{l}.ssm_dt.bias", [nvh], dt)
        var conv <- hf_read_f32(files, bases, "{hf}.conv1d.weight")
        let channels = long_length(qkv_rows)
        let kern = long_length(conv) / channels
        var tiled <- [for (i in range64(channels * kern)); conv[qkv_rows[i / kern] * kern + i % kern]]
        gguf_synth_tensor_f32(m, synth, synth_shard, "blk.{l}.ssm_conv1d.weight", [kern, channels], tiled)
        delete alog
        delete ssm_a
        delete dtb
        delete dt
        delete conv
        delete tiled
    }
    delete vrows
    delete qkv_rows
    delete heads
}

//! the meta of snapshot `dir` over its weight files mapped at `bases` / `sizes` (the tokenizer KVs with `tokenizer`); `synth`
//! holds the getters' bytes, kept alive and unmoved by the caller while the meta is read
[arch(at="../ARCHITECTURE_ENGINE_FORMATS_MLX.md#mlx-snapshot-source")]
def hf_snapshot_meta(dir : string; bases : array<void?>; sizes : array<int64>; var synth : array<uint8>; tokenizer : bool) : GGUFMeta {
    var m = GGUFMeta(hf_snapshot = true, alignment = 32l)
    var files : array<SafetensorsFile>
    files |> reserve(length(bases))
    for (base, size in bases, sizes) {
        unsafe {
            files |> emplace(parse_safetensors(temp_array(reinterpret<uint8?>(base), size, type<uint8>)))
        }
    }
    let synth_shard = length(bases)
    let top = json_file(path_join(dir, "config.json"))
    let model_type : string = top?["model_type"] ?? ""
    var fam = hf_family(model_type)
    let cfg = empty(fam.text_cfg) ? top : top?[fam.text_cfg]
    fam.trunk = cfg_int(cfg, "num_hidden_layers")
    gguf_kv_put_str(m, synth, "general.architecture", fam.arch)
    if (fam.arch == "llama") {
        put_llama_kvs(m, synth, synth_shard, cfg)
    } elif (fam.arch == "qwen4exp") {
        put_qwen4exp_kvs(m, synth, files, bases, fam, cfg, top)
    } else {
        put_qwen35_kvs(m, synth, fam.arch, cfg)
    }
    put_chat_template(m, synth, dir)
    if (tokenizer) {
        put_tokenizer_kvs(m, synth, dir, cfg)
    }
    let quant = top?["quantization"]
    hf_push_tensors(m, synth, synth_shard, files, bases, fam, quant, hf_zero_centred(files, bases, fam))
    if (fam.arch != "llama") {
        hf_qwen35_deltanet(m, synth, synth_shard, files, bases, fam, cfg)
    }
    if (fam.arch == "qwen4exp") {
        hf_indexer_split(m, files, fam, cfg, quant)
        hf_mtp_eh_proj(m, synth, synth_shard, files, bases, fam, quant)
        var inscope paths <- hf_shard_files(dir)
        hf_ngram_table(m, files, paths, fam, cfg, quant)
    }
    synth |> push(0u8)
    m.shard_base := bases
    m.shard_base |> push(unsafe(addr<void?>(synth[0])))
    m.shard_size := sizes
    m.shard_size |> push(long_length(synth))
    m.shard_data_off <- [for (f in files); f.data_off]
    m.shard_data_off |> push(0l)
    delete files
    return <- m
}

//! the snapshot at `dir` mapped, its meta built (the tokenizer's KVs with `tokenizer`), and `blk` invoked with the meta and
//! the bytes its getters read; every mapping dies when `blk` returns
def with_hf_snapshot(dir : string; tokenizer : bool; blk : block<(m : GGUFMeta; bytes : array<uint8>) : void>) {
    let files <- hf_shard_files(dir)
    var bases : array<void?>
    var sizes : array<int64>
    gguf_map_shards(files, 0, bases, sizes) {
        var synth : array<uint8>
        var m <- hf_snapshot_meta(dir, bases, sizes, synth, tokenizer)
        invoke(blk, m, synth)
        m.shard_base |> clear()
        delete m
        delete synth
    }
    bases |> clear()
    delete bases
    delete sizes
}
