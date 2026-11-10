options gen2
options stack = 524288
options persistent_heap
options _dasllama_internal = true

require dastest/testing_boost public
require _model_tier
require _gguf_writer   // write_safetensors, half_bytes: the synthetic files and their BF16 planes
require dasllama/dasllama_safetensors
require dasllama/dasllama_gguf
require dasllama/dasllama_kqformat
require dasllama/dasllama_load   // hc_planes_unserved, map_ngram_table: the load's decline and its table mapping
require dasllama/dasllama_common   // Model: the table mapped for a model's life
require dasllama/dasllama_arch_qwen4exp   // ngram_row_a4g32: the sharded table's row gather
require dasllama/dasllama_env   // g_env_core.home: the kapitsa model cache
require daslib/fio
require daslib/array_boost   // nolint:STYLE030 - temp_array: a mapped shard as the bytes its getters read (a generic instance the lint does not see)
require strings
require math

// The qwen4_exp snapshot family (Qwen3.8-Flash-Next) through the safetensors meta: model-free cells over a synthetic snapshot
// (the family's KVs, the I64 hash constants, the per-tensor group, the zero-centred gains, the indexer's row split, the draft
// head's column concat, refuse-or-map under the head's prefix), then the pack against its GGUF oracles tensor by tensor through
// small windows - never a model load: the BF16 shards for the trunk, the shared-head Q8_0 sidecar for the draft head, the
// served IQ4_XS file for the KVs. The pack cells skip where the pack or its GGUF oracles are not stocked, and where any of
// them is large-tier (`snapshot_available`, `model_available`) without DASLLAMA_PARITY_FULL=1.

def private i64_bytes(v : array<int64>) : array<uint8> {
    var b : array<uint8>
    for (x in v) {
        gguf_put_le(b, uint64(x), 8)
    }
    return <- b
}

//! an 8-bit BF16 triplet `base` of `rows` rows of 64 codes: byte k of row r = (r * 64 + k + seed) & 255, the row's (scale, bias) =
//! (0.5 + r, r - seed); the codes' shape names 16 words a row
def private triplet8(base : string; rows, seed : int) : array<tuple<name : string; dtype : string; shape : string; data : array<uint8>>> {
    return <- [(name = "{base}.weight", dtype = "U32", shape = "[{rows},16]", data <- [for (i in range(rows * 64)); uint8((i + seed) & 255)]),
        (name = "{base}.scales", dtype = "BF16", shape = "[{rows},1]", data <- half_bytes([for (r in range(rows)); 0.5 + float(r)], true)),
        (name = "{base}.biases", dtype = "BF16", shape = "[{rows},1]", data <- half_bytes([for (r in range(rows)); float(r - seed)], true))]
}

//! weight (r, k) of a `triplet8`
def private w8(r, k, seed : int) : float => (0.5 + float(r)) * float((r * 64 + k + seed) & 255) + float(r - seed)

let private FN_CONFIG = "\{\"model_type\":\"qwen4_exp\",\"image_token_id\":77,\"quantization\":\{\"group_size\":32,\"bits\":4,\"language_model.model.layers.0.self_attn.indexer.index_qk_proj\":\{\"group_size\":64,\"bits\":8\},\"mtp.fc_embedding\":\{\"group_size\":64,\"bits\":8\},\"mtp.fc_hidden\":\{\"group_size\":64,\"bits\":8\}\},\"text_config\":\{\"hidden_size\":64,\"num_hidden_layers\":1,\"num_attention_heads\":4,\"num_key_value_heads\":2,\"head_dim\":16,\"max_position_embeddings\":1024,\"rms_norm_eps\":1e-06,\"rope_parameters\":\{\"rope_theta\":10000000,\"partial_rotary_factor\":0.25,\"mrope_section\":[1,1,0]\},\"full_attention_interval\":1,\"linear_conv_kernel_dim\":4,\"linear_key_head_dim\":16,\"linear_num_key_heads\":2,\"linear_num_value_heads\":4,\"linear_value_head_dim\":16,\"num_experts\":2,\"num_experts_per_tok\":1,\"moe_intermediate_size\":3,\"output_gate_type\":\"sigmoid\",\"hc_count\":4,\"hc_lowrank\":32,\"indexer_n_heads\":2,\"indexer_head_dim\":2,\"indexer_kv_heads\":1,\"indexer_budget\":8,\"indexer_compress_ratio\":4,\"layer_types\":[\"full_attention\"],\"ple_layer_ids\":[1],\"ngram_size\":3,\"heads_per_ngram\":2,\"ple_embed_dim\":64,\"ple_conv_kernel_size\":4,\"eos_token_id\":5,\"mtp_num_hidden_layers\":1\}\}"

//! the hash multipliers past 2^32: an I64 tensor read as the KV's 64-bit values
let private MULTS = fixed_array(23703573157769l, 20109073645365l, 8052911324071l)

//! the synthetic n-gram table: two shards of `NG_ROWS` rows of `NG_COLS` 4-bit codes in 32-groups
let private NG_ROWS = 3
let private NG_COLS = 64

//! code byte j of local row r of shard s
def private ng_code(s, r, j : int) : int => (s * 37 + r * 11 + j * 5 + 3) & 255

//! the (scale, bias) of 32-group g of local row r of shard s
def private ng_pair(s, r, g : int) : float2 => float2(0.25 * float(1 + g) + float(r), float(s) - 0.5 * float(g))

//! element k of global row `row`, computed off the definitions alone: element k is the low nibble of byte k / 2 for even k
def private ng_ref(row, k : int) : float {
    let s = row / NG_ROWS
    let r = row % NG_ROWS
    let b = ng_code(s, r, k / 2)
    let q = k % 2 == 0 ? b & 15 : b >> 4
    let p = ng_pair(s, r, k / 32)
    return p.x * float(q) + p.y
}

//! shard s of the synthetic table: its codes, scales and biases
def private ng_shard(base : string; s : int) : array<tuple<name : string; dtype : string; shape : string; data : array<uint8>>> {
    let words = NG_COLS / 8
    let groups = NG_COLS / 32
    return <- [(name = "{base}.{s}.weight", dtype = "U32", shape = "[{NG_ROWS},{words}]", data <- [for (i in range(NG_ROWS * NG_COLS / 2)); uint8(ng_code(s, i / (NG_COLS / 2), i % (NG_COLS / 2)))]),
        (name = "{base}.{s}.scales", dtype = "BF16", shape = "[{NG_ROWS},{groups}]", data <- half_bytes([for (i in range(NG_ROWS * groups)); ng_pair(s, i / groups, i % groups).x], true)),
        (name = "{base}.{s}.biases", dtype = "BF16", shape = "[{NG_ROWS},{groups}]", data <- half_bytes([for (i in range(NG_ROWS * groups)); ng_pair(s, i / groups, i % groups).y], true))]
}

//! a one-layer qwen4_exp snapshot: a zero-centred q_norm and hc gain, the plain deltanet gain, an inject, the indexer's fused
//! q|k projection (2 x 2 query rows, 2 key rows), the n-gram constants and the draft head's two input projections; with `stray`
//! one draft-head tensor no GGUF name maps
def private write_fn_snapshot(stray : bool) : string {
    var err = ""
    let dir = create_temp_directory("dasllama_st_fn_", err)
    return "" if (!empty(err))
    let lp = "language_model.model.layers.0"
    var ts <- [(name = "{lp}.self_attn.q_norm.weight", dtype = "BF16", shape = "[4]", data <- half_bytes([0.25, -0.5, 0.125, 0.0], true)),
        (name = "{lp}.attn_hyper_connection.hc_norm.weight", dtype = "BF16", shape = "[4]", data <- half_bytes([2.5, -0.75, 0.5, 1.0], true)),
        (name = "{lp}.linear_attn.norm.weight", dtype = "BF16", shape = "[4]", data <- half_bytes([0.875, 1.0, 0.9375, 1.0], true)),
        (name = "{lp}.attn_hyper_connection.block_inject_weight.weight", dtype = "BF16", shape = "[4,2]", data <- half_bytes([1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0], true)),
        (name = "{lp}.ple.ple_embedding.layer_multipliers", dtype = "I64", shape = "[3]", data <- i64_bytes([MULTS[0], MULTS[1], MULTS[2]])),
        (name = "{lp}.ple.ple_embedding.ngram_heads_offsets", dtype = "I64", shape = "[4]", data <- i64_bytes([0l, 7l, 14l, 21l])),
        (name = "{lp}.ple.ple_embedding.ngram_heads_vocab_sizes", dtype = "I64", shape = "[4]", data <- i64_bytes([7l, 7l, 7l, 7l]))]
    for (spec in [(base = "{lp}.self_attn.indexer.index_qk_proj", rows = 6, seed = 0), (base = "mtp.fc_embedding", rows = 3, seed = 1),
                  (base = "mtp.fc_hidden", rows = 3, seed = 9)]) {
        var more <- triplet8(spec.base, spec.rows, spec.seed)
        ts |> reserve(length(ts) + length(more))
        for (x in more) {
            ts |> emplace(x)
        }
        delete more
    }
    for (s in range(2)) {
        var more <- ng_shard("{lp}.ple.ple_embedding.ngram_embedding.shards", s)
        ts |> reserve(length(ts) + length(more))
        for (x in more) {
            ts |> emplace(x)
        }
        delete more
    }
    if (stray) {
        ts |> emplace((name = "mtp.unknown_norm.weight", dtype = "BF16", shape = "[4]", data <- half_bytes([1.0, 1.0, 1.0, 1.0], true)))
    }
    let ok = write_safetensors(path_join(dir, "model.safetensors"), ts) && fwrite(path_join(dir, "config.json"), FN_CONFIG)
    return ok ? dir : ""
}

//! the bytes of tensor `name` in the meta's mapped shard
def private tensor_ptr(m : GGUFMeta; name : string) : uint8 const? {
    let ti = gguf_need_tensor(m, name, "test")
    let sh = m.tensors[ti].shard
    var p : uint8 const?
    unsafe {
        p = reinterpret<uint8 const?>(m.shard_base[sh]) + m.shard_data_off[sh] + m.tensors[ti].offset
    }
    return p
}

//! the synthetic table's two shards as one `per_layer_token_embd`, its rows gathered off the bytes in place against the
//! reference, then mapped for a model's life the way the load does and released by the model's finalizer
def private ngram_table_cells(t : T?; m : GGUFMeta) {
    let ti = gguf_find_tensor(m, "per_layer_token_embd.weight")
    t |> success(ti >= 0, "the row shards present one per_layer_token_embd")
    return if (ti < 0)
    t |> equal(m.tensors[ti].ggml_type, GGML_TYPE_NGRAM_A4G32, "the group-32 affine row shards' loader type, off the default 4-bit group-32 entry")
    t |> equal(m.tensors[ti].dims, [int64(NG_COLS), int64(2 * NG_ROWS)], "[row width, every shard's rows]")
    t |> equal(m.ngram_shard_rows, int64(NG_ROWS), "the rows a shard")
    t |> equal(ggml_type_bytes(GGML_TYPE_NGRAM_A4G32, m.tensors[ti].n_elem), int64(2 * NG_ROWS * (NG_COLS / 2 + NG_COLS / 32 * 4)), "codes plus pairs: 20 bytes a 32-group")
    var row : array<float>
    row |> resize(NG_COLS)
    var exact = 0
    for (r in range(2 * NG_ROWS)) {
        ngram_row_a4g32(tensor_ptr(m, "per_layer_token_embd.weight"), int64(r), m.ngram_shard_rows, int64(NG_COLS), unsafe(addr(row[0])))
        for (k in range(NG_COLS)) {
            exact += row[k] == ng_ref(r, k) ? 1 : 0
        }
    }
    t |> equal(exact, 2 * NG_ROWS * NG_COLS, "every row of both shards gathers to scale x q + bias, the low nibble first")
    var mdl = Model(ngram_type = GGML_TYPE_NGRAM_A4G32, ngram_rows = int64(2 * NG_ROWS))
    mdl.config.ple_head_dim = int64(NG_COLS)
    map_ngram_table(mdl, m, ti)
    t |> success(mdl.ngram_map != null && long_length(mdl.ngram_tab) == ggml_type_bytes(GGML_TYPE_NGRAM_A4G32, m.tensors[ti].n_elem),
        "the model views the table in a mapping it holds")
    ngram_row_a4g32(unsafe(addr<uint8 const?>(mdl.ngram_tab[0])), int64(2 * NG_ROWS - 1), mdl.ngram_shard_rows, int64(NG_COLS), unsafe(addr(row[0])))
    t |> equal(row[NG_COLS - 1], ng_ref(2 * NG_ROWS - 1, NG_COLS - 1), "the model's view gathers the last row's last element")
    delete mdl
    delete row
}

[test(description = "A synthetic one-layer qwen4_exp safetensors snapshot maps to the qwen4exp GGUF KVs, 1 + w gains, split indexer q and k, concatenated draft head and n-gram row gather exactly, and a stray draft-head tensor refuses the snapshot")]
def test_qwen4exp_snapshot_meta(t : T?) {
    let dir = write_fn_snapshot(false)
    t |> success(!empty(dir), "the synthetic qwen4_exp snapshot is written")
    return if (empty(dir))
    with_hf_snapshot(dir, false) $(m, bytes) {
        t |> equal(gguf_str(m, bytes, "general.architecture"), "qwen4exp", "model_type qwen4_exp loads as qwen4exp")
        t |> equal(gguf_int(m, bytes, "qwen4exp.hyper_connection.count"), 4l, "hc_count")
        t |> equal(gguf_int(m, bytes, "qwen4exp.attention.indexer.top_k"), 8l, "indexer_budget")
        var inscope ratios <- gguf_int_array(m, bytes, "qwen4exp.attention.compress_ratios")
        t |> equal(ratios, [4l], "the compress ratio on the full-attention layer")
        var inscope pl <- gguf_int_array(m, bytes, "qwen4exp.ple.layers")
        t |> equal(pl, [0l], "ple_layer_ids counts from 1: config [1] is layer 0")
        t |> equal(gguf_int(m, bytes, "qwen4exp.embedding_length_per_layer_input"), 16l, "ple_embed_dim / ((ngram_size - 1) x heads_per_ngram)")
        t |> equal(gguf_int(m, bytes, "qwen4exp.ple.image_token_id"), 77l, "image_token_id off the top-level config")
        var inscope mult <- gguf_int_array(m, bytes, "qwen4exp.ple.layer_multipliers")
        t |> equal(mult, [MULTS[0], MULTS[1], MULTS[2]], "the I64 multipliers past 2^32 read as the KV's 64-bit values")
        var inscope voc <- gguf_int_array(m, bytes, "qwen4exp.ple.head_vocab_sizes")
        t |> equal(voc, [7l, 7l, 7l, 7l], "ngram_heads_vocab_sizes -> head_vocab_sizes")
        t |> equal(m.nextn_sidecar, 1l, "mtp_num_hidden_layers is the split head's count")
        var inscope qn <- gguf_read_f32_arr(m, bytes, "blk.0.attn_q_norm.weight", "test")
        t |> equal(qn, [1.25, 0.5, 1.125, 1.0], "a zero-centred q_norm (the probe's mean below the bar) reads 1 + w")
        var inscope hn <- gguf_read_f32_arr(m, bytes, "blk.0.hc_attn_norm.weight", "test")
        t |> equal(hn, [3.5, 0.25, 1.5, 2.0], "the hc gain takes the same 1 + w")
        var inscope sn <- gguf_read_f32_arr(m, bytes, "blk.0.ssm_norm.weight", "test")
        t |> equal(sn, [0.875, 1.0, 0.9375, 1.0], "the deltanet's gated norm reads plain")
        t |> equal(gguf_tensor_type(m, "blk.0.hc_attn_inject.weight"), GGML_TYPE_BF16, "the inject passes its BF16 bytes through")
        var inscope inj <- gguf_read_f32_arr(m, bytes, "blk.0.hc_attn_inject.weight", "test")
        t |> equal(inj[7], 8.0, "... in their stored order")
        let qi = gguf_find_tensor(m, "blk.0.indexer.q_proj.weight")
        let ki = gguf_find_tensor(m, "blk.0.indexer.k_proj.weight")
        t |> success(qi >= 0 && ki >= 0, "index_qk_proj splits into indexer.q_proj and indexer.k_proj")
        if (qi >= 0 && ki >= 0) {
            t |> equal(m.tensors[qi].ggml_type, GGML_TYPE_MLX8B, "its own quantization entry (8-bit, group 64) wins over the 4-bit group-32 default")
            t |> equal(m.tensors[qi].dims, [64l, 4l], "the query rows: heads x head_dim")
            t |> equal(m.tensors[ki].dims, [64l, 2l], "the key rows: head_dim")
            var inscope q <- gguf_read_f32_arr(m, bytes, "blk.0.indexer.q_proj.weight", "test")
            var inscope k <- gguf_read_f32_arr(m, bytes, "blk.0.indexer.k_proj.weight", "test")
            var exact = 0
            for (i in range(4 * 64)) {
                exact += q[i] == w8(i / 64, i % 64, 0) ? 1 : 0
            }
            for (i in range(2 * 64)) {
                exact += k[i] == w8(4 + i / 64, i % 64, 0) ? 1 : 0
            }
            t |> equal(exact, 384, "the key half starts four rows into the codes, scales and biases alike")
        }
        let ei = gguf_find_tensor(m, "blk.1.nextn.eh_proj.weight")
        t |> success(ei >= 0, "fc_embedding | fc_hidden -> nextn.eh_proj at the trunk's count")
        if (ei >= 0) {
            t |> equal(m.tensors[ei].dims, [128l, 3l], "the two halves' columns end to end")
            var inscope eh <- gguf_read_f32_arr(m, bytes, "blk.1.nextn.eh_proj.weight", "test")
            var exact = 0
            for (i in range(3 * 128)) {
                let r = i / 128
                let k = i % 128
                exact += eh[i] == (k < 64 ? w8(r, k, 1) : w8(r, k - 64, 9)) ? 1 : 0
            }
            t |> equal(exact, 384, "every row the embedding half's codes and pairs, then the hidden half's")
        }
        ngram_table_cells(t, m)
    }
    rmdir_rec(dir)
    let stray = write_fn_snapshot(true)
    return if (empty(stray))
    var refused = false
    try {
        with_hf_snapshot(stray, false) $(_m, _bytes) {
            t |> failure("a draft-head tensor no GGUF name maps built a meta")
        }
    } recover {
        refused = true
    }
    t |> success(refused, "a draft-head tensor no GGUF name maps refuses the snapshot, never a silent skip")
    rmdir_rec(stray)
}

//! the pack under the kapitsa model cache
def private fn_pack() : string => "{g_env_core.home}/.cache/kapitsa/models/qwen3.8-flash-next-4bit"
let private FN_BF16 = "Qwen3.8-Flash-Next-GGUF/BF16"
let private FN_SERVED = "Qwen3.8-Flash-Next-GGUF/IQ4_XS-pure/Qwen3.8-Flash-Next-IQ4_XS-pure.gguf"
let private FN_HEAD = "Qwen3.8-Flash-Next-GGUF/MTP/mtp-Qwen3.8-Flash-Next-shared-Q8_0.gguf"

//! the GGUF at `paths` (a split's shards in order) mapped, its meta parsed, and `blk` invoked with it and shard 0's bytes
def private with_gguf(paths : array<string>; blk : block<(m : GGUFMeta; bytes : array<uint8>) : void>) {
    var bases : array<void?>
    var sizes : array<int64>
    gguf_map_shards(paths, 0, bases, sizes) {
        var m <- parse_gguf_meta_shards(bases, sizes)
        unsafe {
            invoke(blk, m, temp_array(reinterpret<uint8?>(bases[0]), sizes[0], type<uint8>))
        }
        m.shard_base |> clear()
        delete m
    }
    bases |> clear()
    delete bases
    delete sizes
}

//! `nrows` rows from row `row0` of tensor `name` as f32
def private rows_of(m : GGUFMeta; bytes : array<uint8>; name : string; row0, nrows : int64) : array<float> {
    let ti = gguf_need_tensor(m, name, "test")
    let cols = m.tensors[ti].dims[0]
    var out : array<float>
    out |> resize(cols * nrows)
    gguf_read_tensor_f32(m, bytes, name, out, 0l, cols * nrows, row0 * cols)
    return <- out
}

//! max |a - b| over max |b|
def private rel_gap(a, b : array<float>) : float {
    var d = 0.0
    var s = 0.0
    for (x, y in a, b) {
        d = max(d, nan_wide_diff(x, y))
        s = max(s, abs(y))
    }
    return d / max(s, 1.0e-30)
}

//! the KVs the served file carries and the meta must equal: integer scalars, integer arrays, f32 scalars
let private FN_INT_KVS <- ["block_count", "context_length", "embedding_length", "attention.head_count", "attention.head_count_kv",
    "attention.key_length", "attention.value_length", "rope.dimension_count", "expert_count", "expert_used_count",
    "expert_feed_forward_length", "expert_shared_feed_forward_length", "ssm.conv_kernel", "ssm.state_size", "ssm.group_count",
    "ssm.time_step_rank", "ssm.inner_size", "full_attention_interval", "hyper_connection.count", "hyper_connection.low_rank",
    "attention.indexer.head_count", "attention.indexer.key_length", "attention.indexer.top_k", "ple.ngram_size",
    "ple.heads_per_ngram", "ple.conv_kernel", "ple.eos_token_id", "ple.image_token_id", "embedding_length_per_layer_input"]
let private FN_ARR_KVS <- ["rope.dimension_sections", "attention.compress_ratios", "ple.layers", "ple.layer_multipliers",
    "ple.head_offsets", "ple.head_vocab_sizes"]
let private FN_F32_KVS <- ["rope.freq_base", "attention.layer_norm_rms_epsilon"]

//! the trunk tensors the meta synthesizes or passes through that equal the BF16 oracle's bit for bit: the 1 + w gains, the
//! plain deltanet gain, the inject, the n-gram conv taps
let private FN_EXACT <- ["output_hc_norm.weight", "blk.0.hc_attn_norm.weight", "blk.0.hc_ffn_norm.weight", "blk.47.hc_ffn_norm.weight",
    "blk.3.attn_q_norm.weight", "blk.3.attn_k_norm.weight", "blk.3.indexer.q_norm.weight", "blk.3.indexer.k_norm.weight",
    "blk.1.ple_norm_conv.weight", "blk.1.ple_norm_key.weight", "blk.1.ple_norm_query.weight", "blk.0.ssm_norm.weight",
    "blk.0.hc_attn_inject.weight", "blk.47.hc_ffn_inject.weight", "blk.1.ple_conv1d.weight"]

//! the draft head's f32 gains the shared-head sidecar carries
let private FN_HEAD_EXACT <- ["blk.48.nextn.enorm.weight", "blk.48.nextn.hnorm.weight", "blk.48.nextn.hc_head_norm.weight",
    "blk.48.hc_attn_norm.weight", "blk.48.hc_ffn_norm.weight", "blk.48.attn_q_norm.weight", "blk.48.attn_k_norm.weight",
    "blk.48.indexer.q_norm.weight", "blk.48.indexer.k_norm.weight"]

//! an 8-bit window against a BF16 row: half a 64-group step is under 1/255 of the group's span
let private BAR_8BIT = 0.01
//! two 8-bit quantizers of one weight (the pack's affine, the sidecar's Q8_0)
let private BAR_8V8 = 0.02
//! a 4-bit window against BF16 rows
let private BAR_4BIT = 0.12
//! a 6-bit window against BF16 rows
let private BAR_6BIT = 0.04

//! the pack, the BF16 oracle's shards, the served file and the sidecar; "" first, the loud skip registered on `t`, where any is
//! absent or large-tier without DASLLAMA_PARITY_FULL=1
def private fn_paths(t : T?) : tuple<pack : string; bf16 : array<string>; served : string; head : string> {
    var bf16 <- [for (i in range(1, 9)); path_join(models_dir(), "{FN_BF16}/Qwen3.8-Flash-Next-BF16-0000{i}-of-00008.gguf")]
    let served = path_join(models_dir(), FN_SERVED)
    let head = path_join(models_dir(), FN_HEAD)
    let ok = (snapshot_available(t, fn_pack()) && model_available(t, bf16[0]) && model_available(t, served)
        && model_available(t, head))
    return <- (pack = ok ? fn_pack() : "", bf16 <- bf16, served = served, head = head)
}

[test(description = "The Flash-Next MLX pack's integer, array and f32 KVs equal the served IQ4_XS GGUF's exactly, its draft-head count equals the sidecar's, and `hc_planes_unserved` declines the affine mixers only off the native rail")]
def test_qwen4exp_pack_kvs(t : T?) {
    t |> run("the Flash-Next pack's KVs == the served GGUF's, the draft head's count == the sidecar's") @(t : T?) {
        if (!family_on(t, "qwen4exp")) return
        var inscope fp <- fn_paths(t)
        return if (empty(fp.pack))
        with_hf_snapshot(fp.pack, false) $(m, bytes) {
            with_gguf([fp.served]) $(g, gb) {
                for (k in FN_INT_KVS) {
                    t |> equal(gguf_int(m, bytes, "qwen4exp.{k}"), gguf_int(g, gb, "qwen4exp.{k}"), k)
                }
                for (k in FN_ARR_KVS) {
                    var inscope a <- gguf_int_array(m, bytes, "qwen4exp.{k}")
                    var inscope b <- gguf_int_array(g, gb, "qwen4exp.{k}")
                    t |> equal(a, b, k)
                }
                for (k in FN_F32_KVS) {
                    t |> equal(gguf_f32(m, bytes, "qwen4exp.{k}"), gguf_f32(g, gb, "qwen4exp.{k}"), k)
                }
                t |> success(!gguf_has(m, "qwen4exp.feed_forward_length") && !gguf_has(g, "qwen4exp.feed_forward_length"), "no dense width on either side")
            }
            with_gguf([fp.head]) $(g, gb) {
                t |> equal(m.nextn_sidecar, gguf_int(g, gb, "qwen4exp.nextn_predict_layers"), "the draft head's count")
            }
            let why = hc_planes_unserved(m, false)
            to_log(LOG_INFO, "the pack's load decline off the native rail: {why}\n")
            t |> success(find(why, "hc_attn_down") >= 0, "off the native q8 rail the pack's affine mixers decline the load before a weight is read ({why})")
            t |> equal(hc_planes_unserved(m, true), "", "on the native rail every plane rides its tag and the table is present")
            with_gguf([fp.served]) $(g, _gb) {
                t |> equal(hc_planes_unserved(g, false), "", "the served file's q8 mixers and its table pass")
            }
        }
    }
}

//! n-gram rows at the first, a shard's last, the next shard's first, a middle shard's and the last global row, gathered off the
//! pack's row shards in place against the BF16 oracle's `per_layer_token_embd` row; each row's neighbour lands outside the bar
def private pack_ngram_rows(t : T?; m : GGUFMeta; g : GGUFMeta; gb : array<uint8>) {
    let cols = m.tensors[gguf_need_tensor(m, "per_layer_token_embd.weight", "test")].dims[0]
    var ng : array<float>
    ng |> resize(cols)
    for (row in [0l, 2500011l, 2500012l, 160000007l, 320001535l]) {
        ngram_row_a4g32(tensor_ptr(m, "per_layer_token_embd.weight"), row, m.ngram_shard_rows, cols, unsafe(addr(ng[0])))
        var inscope b <- rows_of(g, gb, "per_layer_token_embd.weight", row, 1l)
        var inscope nb <- rows_of(g, gb, "per_layer_token_embd.weight", row == 0l ? 1l : row - 1l, 1l)
        let d = rel_gap(ng, b)
        let ctl = rel_gap(ng, nb)
        to_log(LOG_INFO, "n-gram row {row} (shard {row / m.ngram_shard_rows}): max |d| / max |w| {d}, the neighbour row {ctl}\n")
        t |> success(d < BAR_4BIT && ctl > 2.0 * BAR_4BIT, "n-gram row {row} within {BAR_4BIT} of the BF16 oracle's ({d}), its neighbour outside ({ctl})")
    }
    delete ng
}

[test(description = "Flash-Next pack tensors match the BF16 shards and shared-head sidecar: gains bit for bit, quantized row windows within per-bit-width relative bars, with swapped-half and wrong-row controls landing outside the bar")]
def test_qwen4exp_pack_tensors(t : T?) {
    t |> run("the Flash-Next pack tensor by tensor against the BF16 shards and the shared-head sidecar") @(t : T?) {
        if (!family_on(t, "qwen4exp")) return
        var inscope fp <- fn_paths(t)
        return if (empty(fp.pack))
        with_hf_snapshot(fp.pack, false) $(m, bytes) {
            with_gguf(fp.bf16) $(g, gb) {
                for (n in FN_EXACT) {
                    var inscope a <- gguf_read_f32_arr(m, bytes, n, "test")
                    var inscope b <- gguf_read_f32_arr(g, gb, n, "test")
                    let d = rel_gap(a, b)
                    to_log(LOG_INFO, "{n}: max |d| / max |w| {d}\n")
                    t |> success(length(a) == length(b) && d == 0.0, "{n} equals the BF16 oracle's bit for bit ({d})")
                }
                for (spec in [(n = "blk.3.indexer.q_proj.weight", r0 = 0l, nr = 64l, bar = BAR_8BIT),
                              (n = "blk.3.indexer.k_proj.weight", r0 = 64l, nr = 64l, bar = BAR_8BIT),
                              (n = "blk.47.indexer.k_proj.weight", r0 = 0l, nr = 32l, bar = BAR_8BIT),
                              (n = "blk.1.ple_key.weight", r0 = 8192l, nr = 16l, bar = BAR_8BIT),
                              (n = "blk.1.ple_value.weight", r0 = 0l, nr = 16l, bar = BAR_8BIT),
                              (n = "blk.0.ffn_gate_inp.weight", r0 = 100l, nr = 16l, bar = BAR_8BIT),
                              (n = "token_embd.weight", r0 = 248000l, nr = 4l, bar = BAR_8BIT),
                              (n = "output.weight", r0 = 1000l, nr = 4l, bar = BAR_4BIT),
                              (n = "blk.5.ffn_down_exps.weight", r0 = 2560l * 7l, nr = 8l, bar = BAR_4BIT)]) {
                    var inscope a <- rows_of(m, bytes, spec.n, spec.r0, spec.nr)
                    var inscope b <- rows_of(g, gb, spec.n, spec.r0, spec.nr)
                    let d = rel_gap(a, b)
                    to_log(LOG_INFO, "{spec.n} rows [{spec.r0}, +{spec.nr}): max |d| / max |w| {d} ({ggml_type_name(gguf_tensor_type(m, spec.n))})\n")
                    t |> success(d < spec.bar, "{spec.n} rows [{spec.r0}, +{spec.nr}) within {spec.bar} of the BF16 oracle ({d})")
                }
                pack_ngram_rows(t, m, g, gb)
                var inscope kq <- rows_of(m, bytes, "blk.3.indexer.k_proj.weight", 0l, 64l)
                var inscope qg <- rows_of(g, gb, "blk.3.indexer.q_proj.weight", 0l, 64l)
                let ctl = rel_gap(kq, qg)
                t |> success(ctl > 10.0 * BAR_8BIT, "the control - the key half against the oracle's query rows - lands outside the bar ({ctl})")
                for (n in ["blk.0.hc_attn_down.weight", "blk.0.hc_ffn_up.weight", "output_hc_down.weight", "blk.3.attn_q.weight"]) {
                    var inscope a <- rows_of(m, bytes, n, 0l, 16l)
                    var inscope b <- rows_of(g, gb, n, 0l, 16l)
                    let d = rel_gap(a, b)
                    to_log(LOG_INFO, "{n}: max |d| / max |w| {d} ({ggml_type_name(gguf_tensor_type(m, n))})\n")
                    t |> success(d < BAR_6BIT, "{n} rows [0, +16) within {BAR_6BIT} of the BF16 oracle ({d})")
                }
            }
            with_gguf([fp.head]) $(g, gb) {
                for (n in FN_HEAD_EXACT) {
                    var inscope a <- gguf_read_f32_arr(m, bytes, n, "test")
                    var inscope b <- gguf_read_f32_arr(g, gb, n, "test")
                    let d = rel_gap(a, b)
                    t |> success(length(a) == length(b) && d == 0.0, "{n} equals the sidecar's bit for bit ({d})")
                }
                for (spec in [(n = "blk.48.nextn.eh_proj.weight", r0 = 0l, nr = 16l), (n = "blk.48.nextn.eh_proj.weight", r0 = 2000l, nr = 8l),
                              (n = "blk.48.hc_attn_down.weight", r0 = 0l, nr = 8l), (n = "blk.48.nextn.hc_head_up.weight", r0 = 4096l, nr = 8l),
                              (n = "blk.48.attn_q.weight", r0 = 0l, nr = 8l), (n = "blk.48.indexer.k_proj.weight", r0 = 0l, nr = 32l),
                              (n = "blk.48.ffn_gate_inp.weight", r0 = 0l, nr = 8l)]) {
                    var inscope a <- rows_of(m, bytes, spec.n, spec.r0, spec.nr)
                    var inscope b <- rows_of(g, gb, spec.n, spec.r0, spec.nr)
                    let d = rel_gap(a, b)
                    to_log(LOG_INFO, "{spec.n} rows [{spec.r0}, +{spec.nr}): max |d| / max |w| {d} vs the sidecar\n")
                    t |> success(d < BAR_8V8, "{spec.n} rows [{spec.r0}, +{spec.nr}) within {BAR_8V8} of the sidecar's ({d})")
                }
                var inscope eh <- rows_of(m, bytes, "blk.48.nextn.eh_proj.weight", 0l, 4l)
                var inscope ehg <- rows_of(g, gb, "blk.48.nextn.eh_proj.weight", 0l, 4l)
                var inscope swapped <- [for (i in range(length(eh))); eh[(i / 5120) * 5120 + (i % 5120 + 2560) % 5120]]
                let ctl = rel_gap(swapped, ehg)
                t |> success(ctl > 10.0 * BAR_8V8, "the control - the halves swapped - lands outside the bar ({ctl})")
            }
        }
    }
}
