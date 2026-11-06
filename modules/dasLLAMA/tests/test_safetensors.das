options gen2
options _dasllama_internal = true
options persistent_heap

require dastest/testing_boost public
require dasllama/dasllama_safetensors
require dasllama/dasllama_gguf
require dasllama/dasllama_kqformat
require daslib/fio
require strings
require math
require _gguf_writer   // write_safetensors, half_bytes: the synthetic files and their F16 planes

// The safetensors snapshot reader on a synthetic two-file snapshot the cells write: the index's files sorted, a header whose
// data section starts off a 4-byte boundary, the checkpoint names renamed to the GGUF ones, an affine triplet read as one
// logical tensor, an unread vision tensor dropped, and the config's geometry as the KVs the GGUF loader reads.

def private ramp(n : int; scale : float) : array<float> => <- [for (i in range(n)); float(i) * scale]

let private CONFIG = "\{\"model_type\":\"llama\",\"hidden_size\":64,\"num_hidden_layers\":1,\"num_attention_heads\":4,\"num_key_value_heads\":2,\"head_dim\":16,\"intermediate_size\":128,\"max_position_embeddings\":1024,\"rms_norm_eps\":1e-05,\"rope_theta\":500000.0,\"rope_scaling\":\{\"factor\":32.0,\"high_freq_factor\":4.0,\"low_freq_factor\":1.0,\"original_max_position_embeddings\":8192,\"rope_type\":\"llama3\"\},\"tie_word_embeddings\":true,\"quantization\":\{\"group_size\":64,\"bits\":8\}\}"

let private INDEX = "\{\"metadata\":\{\},\"weight_map\":\{\"model.norm.weight\":\"model-00002-of-00002.safetensors\",\"model.embed_tokens.weight\":\"model-00001-of-00002.safetensors\"\}\}"

//! the synthetic snapshot written under a fresh temp directory; "" where a write failed
def private write_snapshot() : string {
    var err = ""
    let dir = create_temp_directory("dasllama_st_", err)
    return "" if (!empty(err))
    var codes <- [for (i in range(4 * 64)); uint8(i & 255)]
    let a <- [(name = "model.embed_tokens.weight", dtype = "U32", shape = "[4,16]", data <- codes),
        (name = "model.embed_tokens.scales", dtype = "F16", shape = "[4,1]", data <- half_bytes([0.5, 0.25, 1.0, 2.0], false)),
        (name = "model.embed_tokens.biases", dtype = "F16", shape = "[4,1]", data <- half_bytes([-1.0, 0.0, 1.0, -2.0], false)),
        (name = "vision_tower.patch.weight", dtype = "F16", shape = "[2]", data <- half_bytes([7.0, 8.0], false)),
        (name = "model.layers.0.mlp.down_proj.weight", dtype = "U32", shape = "[2,32]", data <- [for (i in range(2 * 128)); uint8((i * 7) & 255)]),
        (name = "model.layers.0.mlp.down_proj.scales", dtype = "F16", shape = "[2,2]", data <- half_bytes([0.5, -0.25, 2.0, 0.125], false)),
        (name = "model.layers.0.mlp.down_proj.biases", dtype = "F16", shape = "[2,2]", data <- half_bytes([1.0, -3.0, 0.5, 4.0], false))]
    let b <- [(name = "model.norm.weight", dtype = "F16", shape = "[64]", data <- half_bytes(ramp(64, 0.125), false)),
        (name = "model.layers.0.input_layernorm.weight", dtype = "F16", shape = "[64]", data <- half_bytes(ramp(64, -0.5), false))]
    let ok = (write_safetensors(path_join(dir, "model-00001-of-00002.safetensors"), a, 2)
        && write_safetensors(path_join(dir, "model-00002-of-00002.safetensors"), b, 1)
        && fwrite(path_join(dir, "config.json"), CONFIG) && fwrite(path_join(dir, "model.safetensors.index.json"), INDEX))
    return ok ? dir : ""
}

[test]
def test_safetensors_snapshot(t : T?) {
    let dir = write_snapshot()
    t |> success(!empty(dir), "the synthetic snapshot is written")
    return if (empty(dir))
    t |> success(hf_snapshot(dir), "a directory with config.json is a snapshot")
    t |> success(!hf_snapshot(path_join(dir, "config.json")), "a file is not")
    var files <- hf_shard_files(dir)
    t |> equal(length(files), 2, "two weight files off the index")
    t |> equal(base_name(files[0]), "model-00001-of-00002.safetensors", "the index's files sorted")
    delete files
    t |> run("the header's offsets read off an unaligned data start") @(t : T?) {
        fopen(path_join(dir, "model-00001-of-00002.safetensors"), "rb") $(f) {
            fmap(f) $(bytes) {
                var inscope st <- parse_safetensors(bytes)
                t |> equal(st.data_off % 4l, 2l, "the data section starts two bytes off a word")
                t |> equal(length(st.tensors), 7, "seven tensors")
            }
        }
    }
    with_hf_snapshot(dir, false) $(m, bytes) {
        let emb = gguf_find_tensor(m, "token_embd.weight")
        t |> success(emb >= 0, "embed_tokens -> token_embd")
        if (emb >= 0) {
            t |> equal(m.tensors[emb].ggml_type, GGML_TYPE_MLX8, "8-bit codes under F16 scales")
            t |> equal(m.tensors[emb].dims[0], 64l, "the logical row is the unpacked width")
            t |> equal(m.tensors[emb].dims[1], 4l, "four rows")
            t |> equal(m.tensors[emb].n_elem, 256l, "n_elem counts weights, not words")
        }
        t |> equal(gguf_tensor_type(m, "token_embd.scales"), GGML_TYPE_F16, "the scale plane beside it")
        t |> equal(gguf_find_tensor(m, "vision_tower.patch.weight"), -1, "the vision tower is not read")
        t |> success(gguf_find_tensor(m, "blk.0.attn_norm.weight") >= 0, "input_layernorm -> attn_norm")
        var inscope v <- gguf_read_f32_arr(m, bytes, "output_norm.weight", "test")
        t |> equal(length(v), 64, "the second file's F16 tensor: 64 values")
        t |> equal(v[63], 63.0 * 0.125, "... read back exactly")
        var inscope w <- gguf_read_f32_arr(m, bytes, "blk.0.attn_norm.weight", "test")
        t |> equal(w[5], -2.5, "the layer norm's value")
        t |> equal(gguf_str(m, bytes, "general.architecture"), "llama", "arch")
        t |> equal(gguf_int(m, bytes, "llama.embedding_length"), 64l, "hidden_size")
        t |> equal(gguf_int(m, bytes, "llama.attention.head_count_kv"), 2l, "num_key_value_heads")
        t |> equal(gguf_int(m, bytes, "llama.rope.dimension_count"), 16l, "head_dim")
        t |> equal(gguf_f32(m, bytes, "llama.rope.freq_base"), 500000.0, "rope_theta")
        t |> success(m.hf_rope_halves, "the llama checkpoint's q/k rows pair i and i + head/2")
        var inscope rf <- gguf_read_f32_arr(m, bytes, "rope_freqs.weight", "test")
        t |> equal(length(rf), 8, "head_dim / 2 rotary factors")
        t |> equal(rf[0], 1.0, "the high-frequency end is unscaled")
        t |> equal(rf[7], 32.0, "the low-frequency end takes the factor")
        t |> equal(rf[3], 1.0, "the last wavelength under original_ctx / high_freq_factor is unscaled")
        t |> equal(rf[5], 32.0, "the first wavelength over original_ctx / low_freq_factor takes the factor")
        let wave4 = 2.0lf * 3.141592653589793lf * sqrt(500000.0lf)
        let smooth4 = (8192.0lf / wave4 - 1.0lf) / (4.0lf - 1.0lf)
        let blend4 = float(1.0lf / ((1.0lf - smooth4) / 32.0lf + smooth4))
        t |> success(abs(rf[4] - blend4) < 1e-5 * blend4, "the wavelength between the bounds blends: got {rf[4]}, want {blend4}")
        var mp := m
        mlx_transcode_cells(t, mp, bytes)
        delete mp
    }
    rmdir_rec(dir)
}

let private MOE_CONFIG = "\{\"model_type\":\"qwen3_5_moe\",\"quantization\":\{\"group_size\":64,\"bits\":8\},\"text_config\":\{\"hidden_size\":64,\"num_hidden_layers\":1,\"num_attention_heads\":4,\"num_key_value_heads\":2,\"head_dim\":16,\"max_position_embeddings\":1024,\"rms_norm_eps\":1e-06,\"rope_parameters\":\{\"rope_theta\":10000000,\"partial_rotary_factor\":0.25,\"mrope_section\":[1,1,0]\},\"full_attention_interval\":4,\"linear_conv_kernel_dim\":4,\"linear_key_head_dim\":16,\"linear_num_key_heads\":2,\"linear_num_value_heads\":4,\"linear_value_head_dim\":16,\"num_experts\":2,\"num_experts_per_tok\":1,\"moe_intermediate_size\":3,\"shared_expert_intermediate_size\":5\}\}"

//! an 8-bit triplet a spec (base, experts, rows): `rows` rows (`lead` experts of them where it is not 0) of 64 codes, byte k
//! of row r = r * 64 + k, the row's (scale, bias) = (0.5 + r, r - 1)
def private moe_triplets(specs : array<tuple<base : string; lead : int; rows : int>>) : array<tuple<name : string; dtype : string; shape : string; data : array<uint8>>> {
    var out : array<tuple<name : string; dtype : string; shape : string; data : array<uint8>>>
    out |> reserve(3 * length(specs))
    for (sp in specs) {
        let n = sp.lead > 0 ? sp.lead * sp.rows : sp.rows
        let pre = sp.lead > 0 ? "{sp.lead}," : ""
        out |> emplace((name = "{sp.base}.weight", dtype = "U32", shape = "[{pre}{sp.rows},16]", data <- [for (i in range(n * 64)); uint8(i & 255)]))
        out |> emplace((name = "{sp.base}.scales", dtype = "F16", shape = "[{pre}{sp.rows},1]", data <- half_bytes([for (r in range(n)); 0.5 + float(r)], false)))
        out |> emplace((name = "{sp.base}.biases", dtype = "F16", shape = "[{pre}{sp.rows},1]", data <- half_bytes([for (r in range(n)); float(r) - 1.0], false)))
    }
    return <- out
}

//! weight (r, k) of a `moe_triplet` - r counts rows across the experts
def private moe_w(r, k : int) : float => (0.5 + float(r)) * float((r * 64 + k) & 255) + (float(r) - 1.0)

//! a one-layer routed snapshot: the 8-bit router, the shared expert's gate vector and its down projection, a two-expert gate
//! stack; with `stray` one more layer tensor no GGUF name maps
//! `stray` adds a layer tensor no GGUF name maps; `short` halves the first tensor's bytes under its declared shape
def private write_moe_snapshot(stray, short : bool) : string {
    var err = ""
    let dir = create_temp_directory("dasllama_st_moe_", err)
    return "" if (!empty(err))
    let lp = "language_model.model.layers.0.mlp"
    var specs <- [(base = "{lp}.gate", lead = 0, rows = 2), (base = "{lp}.shared_expert_gate", lead = 0, rows = 1),
        (base = "{lp}.switch_mlp.gate_proj", lead = 2, rows = 3), (base = "{lp}.shared_expert.up_proj", lead = 0, rows = 5)]
    if (stray) {
        specs |> push((base = "{lp}.unknown_proj", lead = 0, rows = 1))
    }
    var ts <- moe_triplets(specs)
    if (short) {
        ts[0].data |> resize(length(ts[0].data) / 2)
    }
    let ok = write_safetensors(path_join(dir, "model.safetensors"), ts, 0) && fwrite(path_join(dir, "config.json"), MOE_CONFIG)
    return ok ? dir : ""
}

[test]
def test_safetensors_moe_snapshot(t : T?) {
    let dir = write_moe_snapshot(false, false)
    t |> success(!empty(dir), "the synthetic routed snapshot is written")
    return if (empty(dir))
    with_hf_snapshot(dir, false) $(m, bytes) {
        t |> equal(gguf_str(m, bytes, "general.architecture"), "qwen35moe", "model_type qwen3_5_moe loads as qwen35moe")
        t |> equal(gguf_int(m, bytes, "qwen35moe.expert_count"), 2l, "num_experts")
        t |> equal(gguf_int(m, bytes, "qwen35moe.expert_used_count"), 1l, "num_experts_per_tok")
        t |> equal(gguf_int(m, bytes, "qwen35moe.expert_feed_forward_length"), 3l, "moe_intermediate_size")
        t |> equal(gguf_int(m, bytes, "qwen35moe.expert_shared_feed_forward_length"), 5l, "shared_expert_intermediate_size")
        t |> success(!gguf_has(m, "qwen35moe.feed_forward_length"), "no dense width: the config carries no intermediate_size")
        let st = gguf_find_tensor(m, "blk.0.ffn_gate_exps.weight")
        t |> success(st >= 0, "switch_mlp.gate_proj -> ffn_gate_exps")
        if (st >= 0) {
            t |> equal(m.tensors[st].ggml_type, GGML_TYPE_MLX8, "the stack is an 8-bit triplet")
            t |> equal(m.tensors[st].dims[0], 64l, "an expert row is the unpacked width")
            t |> equal(m.tensors[st].dims[1], 3l, "three rows an expert")
            t |> equal(m.tensors[st].dims[2], 2l, "two experts, the outermost dim")
            t |> equal(m.tensors[st].n_elem, 384l, "n_elem counts every expert's weights")
        }
        for (want in ["blk.0.ffn_gate_inp.weight", "blk.0.ffn_gate_inp_shexp.weight", "blk.0.ffn_up_shexp.weight"]) {
            t |> success(gguf_find_tensor(m, want) >= 0, "the routed block's '{want}'")
        }
        var inscope router <- gguf_read_f32_arr(m, bytes, "blk.0.ffn_gate_inp.weight", "test")
        var exact = 0
        for (i in range(128)) {
            exact += router[i] == moe_w(i / 64, i % 64) ? 1 : 0
        }
        t |> equal(exact, 128, "the 8-bit router reads as f32 off its own codes and pairs, every weight scale x q + bias")
        var q, s : array<uint8>
        q |> resize(384)
        s |> resize(6 * 4)
        gguf_transcode_mlx(m, bytes, "blk.0.ffn_gate_exps.weight", KqFmt.mlx8, q, s, 0l, 384l, 0l)
        var w <- [for (_i in range(384)); 0.0]
        for (b in range64(12l)) {
            dequant_b32_plane_block(KqFmt.mlx8, q, s, b, w, b * 32l)
        }
        var stacked = 0
        for (i in range(384)) {
            stacked += w[i] == moe_w(i / 64, i % 64) ? 1 : 0
        }
        t |> equal(stacked, 384, "the expert-major stack transcodes row after row across its experts")
        delete q
        delete s
        delete w
    }
    rmdir_rec(dir)
    let stray = write_moe_snapshot(true, false)
    t |> success(!empty(stray), "the snapshot with a stray layer tensor is written")
    return if (empty(stray))
    var refused = false
    try {
        with_hf_snapshot(stray, false) $(_m, _bytes) {
            t |> failure("a snapshot with an unmapped layer tensor built a meta")
        }
    } recover {
        refused = true
    }
    t |> success(refused, "a layer tensor no GGUF name maps refuses the snapshot")
    rmdir_rec(stray)
    //! a header entry whose byte span is not its shape's: the meta never builds (a short span would read past the
    //! mapped file at load, a long one would hand the tensor another's bytes)
    let short = write_moe_snapshot(false, true)
    t |> success(!empty(short), "the snapshot with a short tensor span is written")
    return if (empty(short))
    var span_refused = false
    try {
        with_hf_snapshot(short, false) $(_m, _bytes) {
            t |> failure("a snapshot whose tensor spans half its shape's bytes built a meta")
        }
    } recover {
        span_refused = true
    }
    t |> success(span_refused, "a tensor whose data span is not its shape's bytes refuses the snapshot")
    rmdir_rec(short)
}

//! weight (r, k) of the fixture's down projection: its code byte under its 64-group's (scale, bias)
def private down_w(r, k : int) : float {
    let s = fixed_array(0.5, -0.25, 2.0, 0.125)
    let b = fixed_array(1.0, -3.0, 0.5, 4.0)
    let g = r * 2 + k / 64
    return s[g] * float(((r * 128 + k) * 7) & 255) + b[g]
}

//! the down projection's rows dequantized off planes `gguf_transcode_mlx` filled under the meta's permutes
def private down_planes(t : T?; m : GGUFMeta; bytes : array<uint8>) : array<float> {
    var q, s : array<uint8>
    q |> resize(2 * 128)
    s |> resize(2 * 4 * 2)
    gguf_transcode_mlx(m, bytes, "blk.0.ffn_down.weight", KqFmt.mlx8, q, s, 0l, 256l, 0l)
    var w <- [for (_i in range(256)); 0.0]
    for (b in range64(8l)) {
        dequant_b32_plane_block(KqFmt.mlx8, q, s, b, w, b * 32l)
    }
    t |> equal(length(q), 256, "the code plane holds the row's bytes verbatim")
    delete q
    delete s
    return <- w
}

//! the triplet transcode: the plane dequant equals the hand-computed weights, a row permute reads each logical row off its
//! stored row, and a 64-group segment permute swaps whole column groups with their pairs
def private mlx_transcode_cells(t : T?; var m : GGUFMeta; bytes : array<uint8>) {
    t |> equal(gguf_tensor_type(m, "blk.0.ffn_down.weight"), GGML_TYPE_MLX8, "the down projection is an 8-bit triplet")
    var inscope w <- down_planes(t, m, bytes)
    var exact = 0
    for (i in range(256)) {
        exact += w[i] == down_w(i / 128, i % 128) ? 1 : 0
    }
    t |> equal(exact, 256, "every weight is scale x q + bias of its own group")
    m.permutes |> emplace("blk.0.ffn_down.weight", GGUFRowPermute(rows <- [1l, 0l]))
    var inscope wr <- down_planes(t, m, bytes)
    var rowed = 0
    for (i in range(256)) {
        rowed += wr[i] == down_w(1 - i / 128, i % 128) ? 1 : 0
    }
    t |> equal(rowed, 256, "logical row r reads stored row rows[r]")
    m.permutes |> erase("blk.0.ffn_down.weight")
    m.permutes |> emplace("blk.0.ffn_down.weight", GGUFRowPermute(seg_cols = 64l, segs <- [1l, 0l]))
    var inscope ws <- down_planes(t, m, bytes)
    var segged = 0
    for (i in range(256)) {
        let k = i % 128
        segged += ws[i] == down_w(i / 128, k < 64 ? k + 64 : k - 64) ? 1 : 0
    }
    t |> equal(segged, 256, "segment s reads stored segment segs[s], its group pairs with it")
}
