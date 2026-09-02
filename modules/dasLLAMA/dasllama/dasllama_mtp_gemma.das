options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_mtp_gemma shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_common   // Model/Session, embed_row, the KV-cache row accessors
require dasllama/dasllama_gguf     // the container reader (+ f16_to_f32 through its public re-export)
require dasllama/dasllama_math     // rmsnorm / geglu / softmax / rope_scaled_neox_tab
require dasllama/dasllama_rope     // build_rope_tabs — the anchor's two RoPE class rows
require daslib/fio
require daslib/strings_boost   // trim_suffix — the sidecar name stem
require strings
require math

// The gemma-4 assistant drafter ("gemma4-assistant"): a 4-layer Q-only head that borrows the
// TARGET trunk's K/V at two capture layers and drafts k tokens at ONE frozen anchor position.
// This file owns the sidecar loader and the CPU reference forward; the GPU encoder consumes
// the same `GemmaDrafter` struct.

//! The drafter's weights and geometry, loaded from an `mtp-<model>` sidecar GGUF.
//! `blob` is raw GGUF Q8_0 — 34 bytes per 32 elements (f16 scale then 32 int8), rows of `n`
//! input elements per output row, each tensor 32-byte aligned. That is byte-identical to the
//! engine's Metal 34B blob form (`dequant_q8_row`'s mblob arm), so it uploads with no repack.
//! `*_off` name BYTE offsets into `blob` and ELEMENT offsets into `fblob`.
//! `kv_layer[l]` is NEGATIVE until bound: -2/-1 are the offsets from the target's layer count
//! (sliding reads the last sliding layer, full the last global). `gemma_drafter_bind_target`
//! resolves them to absolute target layer indices and sets `kv_bound`.
struct GemmaDrafter {
    ok : bool
    why : string
    dim : int64
    tdim : int64
    hidden : int64
    vocab : int64
    n_layers : int64
    n_heads : int64
    eps : float
    sliding_window : int64
    rope_theta : float
    rope_theta_swa : float
    hs : array<int64>
    kvh : array<int64>
    sliding : array<bool>
    kv_layer : array<int64>
    kv_bound : bool
    layer_scale : array<float>
    blob : array<uint8>
    pre_proj_off : int64
    post_proj_off : int64
    tok_emb_off : int64
    wq_offs : array<int64>
    wo_offs : array<int64>
    w1_offs : array<int64>
    w3_offs : array<int64>
    w2_offs : array<int64>
    fblob : array<float>
    out_norm_off : int64
    rope_freqs_off : int64
    rope_freqs_n : int64
    attn_norm_offs : array<int64>
    post_attn_norm_offs : array<int64>
    ffn_norm_offs : array<int64>
    post_ffn_norm_offs : array<int64>
    q_norm_offs : array<int64>
}

//! One drafter step's output: the greedy token, the 2816-wide hidden the NEXT step consumes in
//! the target hidden's slot, and the full-vocab logits the token came from.
struct GemmaDraftOut {
    token : int64
    h_next : array<float>
    logits : array<float>
}

let private GEMMA_DRAFTER_ARCH = "gemma4-assistant"
let private Q8_BLK_BYTES = 34l
let private Q8_BLK_ELEMS = 32l

// one planned tensor copy: where it lives on disk and where it lands in blob/fblob
struct private DrPlan {
    ti : int
    dst : int64
    n : int64
    q8 : bool
}

// ===== sidecar discovery =====

// the stem before the trailing quant token ("...-it-Q4_K_M" -> "...-it"); the whole stem when
// the name carries no "-Q<digit>" marker
def private quant_stem(stem : string) : string {
    var cut = -1
    peek_data(stem) $(d : array<uint8> const#) {
        let n = length(d)
        for (i in range(max(0, n - 2))) {
            if (int(d[i]) == '-' && int(d[i + 1]) == 'Q' && int(d[i + 2]) >= '0' && int(d[i + 2]) <= '9') {
                cut = i
            }
        }
    }
    return cut < 0 ? stem : slice(stem, 0, cut)
}

//! The gemma-4 drafter sidecar beside `model_path`: `mtp-<stem>-Q8_0.gguf` when it exists (the
//! head's quant is chosen independently of the trunk's), else the exact-name `mtp-<basename>`
//! sibling, else "". A path already naming a head never recurses onto itself.
def gemma_drafter_sidecar(model_path : string) : string {
    let name = base_name(model_path)
    if (name |> starts_with("mtp-")) {
        return ""
    }
    let dir = dir_name(model_path)
    let stem = trim_suffix(name, ".gguf")
    let pref = path_join(dir, "mtp-{quant_stem(stem)}-Q8_0.gguf")
    if (fexist(pref)) {
        return pref
    }
    let exact = path_join(dir, "mtp-{name}")
    return fexist(exact) ? exact : ""
}

// ===== loader =====

def private refuse(var dr : GemmaDrafter; why : string) : bool {
    dr.ok = false
    dr.why = why
    return false
}

// refuse()'s offset-returning twin: -1 is never a valid blob/fblob offset
def private refuse_off(var dr : GemmaDrafter; why : string) : int64 {
    dr.ok = false
    dr.why = why
    return -1l
}

// a tensor's byte base inside the mapped single-shard file
def private tbase(m : GGUFMeta; ti : int) : int64 => m.data_offset + m.tensors[ti].offset

// plan one tensor: presence, on-disk type and element count all have to match, or the load refuses
def private plan_tensor(var dr : GemmaDrafter; m : GGUFMeta; var plans : array<DrPlan>;
                        var bcur : int64&; var fcur : int64&;
                        name : string; want_n : int64; q8 : bool) : int64 {
    let ti = gguf_find_tensor(m, name)
    if (ti < 0) {
        return refuse_off(dr, "missing tensor '{name}'")
    }
    let want_type = q8 ? GGML_TYPE_Q8_0 : GGML_TYPE_F32
    if (m.tensors[ti].ggml_type != want_type) {
        let tn = q8 ? "Q8_0" : "F32"
        return refuse_off(dr, "tensor '{name}' is type {m.tensors[ti].ggml_type}, expected {tn}")
    }
    if (m.tensors[ti].n_elem != want_n) {
        return refuse_off(dr, "tensor '{name}' has {m.tensors[ti].n_elem} elements, expected {want_n}")
    }
    if (q8 && want_n % Q8_BLK_ELEMS != 0l) {
        return refuse_off(dr, "tensor '{name}' element count {want_n} is not a whole Q8_0 block")
    }
    var off = 0l
    if (q8) {
        off = bcur
        bcur += ((want_n / Q8_BLK_ELEMS) * Q8_BLK_BYTES + 31l) / 32l * 32l
    } else {
        off = fcur
        fcur += want_n
    }
    plans |> emplace(DrPlan(ti = ti, dst = off, n = want_n, q8 = q8))
    return off
}

// the drafter owns no K/V projection: a stray k/v weight means the checkpoint and the config
// disagree, which is a hard refusal rather than a silent drop
def private has_stray_kv(m : GGUFMeta) : string {
    for (ti in range(length(m.tensors))) {
        let n = m.tensors[ti].name
        if (find(n, "attn_k") >= 0 || find(n, "attn_v") >= 0) {
            return n
        }
    }
    return ""
}

def private read_drafter_meta(var dr : GemmaDrafter; m : GGUFMeta; bytes : array<uint8> | #) : bool {
    if (!gguf_has(m, "general.architecture")) {
        return refuse(dr, "no general.architecture (not a GGUF model file)")
    }
    let arch = gguf_str(m, bytes, "general.architecture")
    if (arch != GEMMA_DRAFTER_ARCH) {
        return refuse(dr, "architecture is '{arch}', expected '{GEMMA_DRAFTER_ARCH}'")
    }
    let stray = has_stray_kv(m)
    if (!empty(stray)) {
        return refuse(dr, "checkpoint carries '{stray}': every drafter layer is kv-shared by construction")
    }
    let p = "{GEMMA_DRAFTER_ARCH}."
    var need <- ["block_count", "embedding_length", "embedding_length_out", "feed_forward_length",
                 "attention.head_count", "attention.head_count_kv", "attention.sliding_window",
                 "attention.sliding_window_pattern", "attention.key_length", "attention.key_length_swa",
                 "attention.layer_norm_rms_epsilon", "rope.freq_base", "rope.freq_base_swa"]
    for (k in need) {
        if (!gguf_has(m, "{p}{k}")) {
            return refuse(dr, "missing metadata key '{p}{k}'")
        }
    }
    dr.n_layers = gguf_int(m, bytes, "{p}block_count")
    dr.dim = gguf_int(m, bytes, "{p}embedding_length")
    dr.tdim = gguf_int(m, bytes, "{p}embedding_length_out")
    dr.hidden = gguf_int(m, bytes, "{p}feed_forward_length")
    dr.n_heads = gguf_int(m, bytes, "{p}attention.head_count")
    dr.sliding_window = gguf_int(m, bytes, "{p}attention.sliding_window")
    dr.eps = gguf_f32(m, bytes, "{p}attention.layer_norm_rms_epsilon")
    dr.rope_theta = gguf_f32(m, bytes, "{p}rope.freq_base")
    dr.rope_theta_swa = gguf_f32(m, bytes, "{p}rope.freq_base_swa")
    if (dr.n_layers <= 0l || dr.dim <= 0l || dr.tdim <= 0l || dr.hidden <= 0l || dr.n_heads <= 0l) {
        return refuse(dr, "degenerate geometry (layers {dr.n_layers} dim {dr.dim} out {dr.tdim})")
    }
    if (dr.tdim == dr.dim) {
        return refuse(dr, "embedding_length_out equals embedding_length: this is not a drafter head")
    }
    let hs_full = gguf_int(m, bytes, "{p}attention.key_length")
    let hs_swa = gguf_int(m, bytes, "{p}attention.key_length_swa")
    if (!gguf_is_array(m, "{p}attention.head_count_kv")
        || !gguf_is_array(m, "{p}attention.sliding_window_pattern")) {
        return refuse(dr, "head_count_kv / sliding_window_pattern must be per-layer arrays")
    }
    var kvh <- gguf_int_array(m, bytes, "{p}attention.head_count_kv")
    let pat <- gguf_int_array(m, bytes, "{p}attention.sliding_window_pattern")
    if (long_length(kvh) != dr.n_layers || long_length(pat) != dr.n_layers) {
        return refuse(dr, "per-layer arrays are {long_length(kvh)}/{long_length(pat)} long, expected {dr.n_layers}")
    }
    dr.kvh <- kvh
    dr.hs |> reserve(dr.n_layers)
    dr.sliding |> reserve(dr.n_layers)
    dr.kv_layer |> reserve(dr.n_layers)
    for (l in range64(dr.n_layers)) {
        let sl = pat[l] != 0l
        dr.sliding |> push(sl)
        dr.hs |> push(sl ? hs_swa : hs_full)
        dr.kv_layer |> push(sl ? -2l : -1l)   // relative to the target's layer count until bound
        if (dr.kvh[l] <= 0l || dr.n_heads % dr.kvh[l] != 0l) {
            return refuse(dr, "layer {l}: {dr.n_heads} q heads do not divide over {dr.kvh[l]} kv heads")
        }
    }
    let temb = gguf_find_tensor(m, "token_embd.weight")
    if (temb < 0 || length(m.tensors[temb].dims) < 2) {
        return refuse(dr, "missing or malformed token_embd.weight")
    }
    dr.vocab = m.tensors[temb].dims[1]
    return true
}

def private plan_drafter(var dr : GemmaDrafter; m : GGUFMeta; var plans : array<DrPlan>;
                         var nblob : int64&; var nfblob : int64&) : bool {
    var b = 0l
    var f = 0l
    dr.pre_proj_off = plan_tensor(dr, m, plans, b, f, "nextn.pre_projection.weight", 2l * dr.tdim * dr.dim, true)
    dr.post_proj_off = plan_tensor(dr, m, plans, b, f, "nextn.post_projection.weight", dr.dim * dr.tdim, true)
    dr.tok_emb_off = plan_tensor(dr, m, plans, b, f, "token_embd.weight", dr.dim * dr.vocab, true)
    dr.out_norm_off = plan_tensor(dr, m, plans, b, f, "output_norm.weight", dr.dim, false)
    dr.wq_offs |> reserve(dr.n_layers)
    dr.wo_offs |> reserve(dr.n_layers)
    dr.w1_offs |> reserve(dr.n_layers)
    dr.w3_offs |> reserve(dr.n_layers)
    dr.w2_offs |> reserve(dr.n_layers)
    dr.attn_norm_offs |> reserve(dr.n_layers)
    dr.post_attn_norm_offs |> reserve(dr.n_layers)
    dr.ffn_norm_offs |> reserve(dr.n_layers)
    dr.post_ffn_norm_offs |> reserve(dr.n_layers)
    dr.q_norm_offs |> reserve(dr.n_layers)
    for (l in range64(dr.n_layers)) {
        let hs = dr.hs[l]
        let qd = dr.n_heads * hs
        dr.wq_offs |> push(plan_tensor(dr, m, plans, b, f, "blk.{l}.attn_q.weight", dr.dim * qd, true))
        dr.wo_offs |> push(plan_tensor(dr, m, plans, b, f, "blk.{l}.attn_output.weight", qd * dr.dim, true))
        dr.w1_offs |> push(plan_tensor(dr, m, plans, b, f, "blk.{l}.ffn_gate.weight", dr.dim * dr.hidden, true))
        dr.w3_offs |> push(plan_tensor(dr, m, plans, b, f, "blk.{l}.ffn_up.weight", dr.dim * dr.hidden, true))
        dr.w2_offs |> push(plan_tensor(dr, m, plans, b, f, "blk.{l}.ffn_down.weight", dr.hidden * dr.dim, true))
        dr.attn_norm_offs |> push(plan_tensor(dr, m, plans, b, f, "blk.{l}.attn_norm.weight", dr.dim, false))
        dr.post_attn_norm_offs |> push(plan_tensor(dr, m, plans, b, f, "blk.{l}.post_attention_norm.weight", dr.dim, false))
        dr.ffn_norm_offs |> push(plan_tensor(dr, m, plans, b, f, "blk.{l}.ffn_norm.weight", dr.dim, false))
        dr.post_ffn_norm_offs |> push(plan_tensor(dr, m, plans, b, f, "blk.{l}.post_ffw_norm.weight", dr.dim, false))
        dr.q_norm_offs |> push(plan_tensor(dr, m, plans, b, f, "blk.{l}.attn_q_norm.weight", hs, false))
        if (!empty(dr.why)) {
            return false
        }
    }
    // rope_freqs rides the full-attention class only; a drafter with no full layer never reads it
    let rfi = gguf_find_tensor(m, "rope_freqs.weight")
    if (rfi >= 0) {
        dr.rope_freqs_n = m.tensors[rfi].n_elem
        dr.rope_freqs_off = plan_tensor(dr, m, plans, b, f, "rope_freqs.weight", dr.rope_freqs_n, false)
    } else {
        dr.rope_freqs_off = -1l
    }
    if (!empty(dr.why)) {
        return false
    }
    dr.layer_scale |> reserve(dr.n_layers)
    nblob = b
    nfblob = f
    return true
}

def private copy_planned(var dr : GemmaDrafter; m : GGUFMeta; bytes : array<uint8> | #;
                         plans : array<DrPlan>; nblob, nfblob : int64) {
    dr.blob |> reserve(nblob)
    dr.blob |> resize(nblob)
    dr.fblob |> reserve(nfblob)
    dr.fblob |> resize(nfblob)
    for (pl in plans) {
        if (pl.q8) {
            let nbytes = (pl.n / Q8_BLK_ELEMS) * Q8_BLK_BYTES
            unsafe {
                memcpy(addr<void?>(dr.blob[pl.dst]), addr<void?>(bytes[tbase(m, pl.ti)]), nbytes)
            }
        } else {
            gguf_read_tensor_f32(m, bytes, m.tensors[pl.ti].name, dr.fblob, pl.dst, pl.n)
        }
    }
}

//! Resolve `kv_layer` against the target's layer count: a sliding drafter layer reads the target's
//! LAST sliding layer (n_layers-2), a full one its last global layer (n_layers-1). Idempotent.
def gemma_drafter_bind_target(var dr : GemmaDrafter; target_n_layers : int64) {
    if (dr.kv_bound || target_n_layers <= 2l) {
        return
    }
    for (l in range64(dr.n_layers)) {
        dr.kv_layer[l] = target_n_layers + dr.kv_layer[l]
    }
    dr.kv_bound = true
}

//! Load the gemma-4 assistant drafter sidecar at `path`. Never panics: any refusal - a wrong
//! architecture, a missing or non-Q8_0 weight, a stray K/V tensor - lands as `ok = false` plus a
//! `why`. `target_n_layers > 0` binds `kv_layer` in the same call.
def load_gemma_drafter(path : string; target_n_layers : int64 = 0l) : GemmaDrafter {
    var dr : GemmaDrafter
    if (!fexist(path)) {
        dr.why = "no such file: '{path}'"
        return <- dr
    }
    fopen(path, "rb") $(f) {
        if (f == null) {
            dr.why = "cannot open '{path}'"
            return
        }
        fmap(f) $(var bytes : array<uint8>#) {
            if (long_length(bytes) < 24l) {
                dr.why = "'{path}' is too short to be a GGUF file"
                return
            }
            if (!(int(bytes[0]) == 'G' && int(bytes[1]) == 'G' && int(bytes[2]) == 'U' && int(bytes[3]) == 'F')) {
                dr.why = "'{path}' has no GGUF magic"
                return
            }
            var m <- parse_gguf_meta(bytes)
            if (read_drafter_meta(dr, m, bytes)) {
                var plans : array<DrPlan>
                var nblob = 0l
                var nfblob = 0l
                if (plan_drafter(dr, m, plans, nblob, nfblob)) {
                    copy_planned(dr, m, bytes, plans, nblob, nfblob)
                    for (l in range64(dr.n_layers)) {
                        var sc : array<float>
                        sc |> resize(1)
                        gguf_read_tensor_f32(m, bytes, "blk.{l}.layer_output_scale.weight", sc, 0l, 1l)
                        dr.layer_scale |> push(sc[0])
                    }
                    dr.ok = true
                }
            }
        }
    }
    if (dr.ok && target_n_layers > 0l) {
        gemma_drafter_bind_target(dr, target_n_layers)
    }
    return <- dr
}

// ===== CPU reference forward =====

//! One Q8_0 row dot over a 34-byte-block blob: row `row` of a `[rows x n]` weight at BYTE offset
//! `boff`, against the first `n` floats of `x`.
def dot_q8_blob(blob : array<uint8>; boff, row, n : int64; x : array<float>) : float {
    let nb = n / Q8_BLK_ELEMS
    var acc = 0.0
    unsafe {
        let bp = addr<uint8 const?>(blob[boff + row * nb * Q8_BLK_BYTES])
        let xp = addr<float const?>(x[0])
        for (b in range64(nb)) {
            let bo = b * Q8_BLK_BYTES
            let sc = f16_to_f32(uint(bp[bo]) | (uint(bp[bo + 1l]) << 8u))
            let qp = reinterpret<int8 const?>(bp + bo + 2l)
            let xo = b * Q8_BLK_ELEMS
            var sub = 0.0
            for (k in range64(Q8_BLK_ELEMS)) {
                sub += float(qp[k]) * xp[xo + k]
            }
            acc += sub * sc
        }
    }
    return acc
}

// y[0..d) = W . x, W stored as d rows of n inputs in the 34-byte-block blob
def private mv_q8_blob(var y : array<float>; blob : array<uint8>; boff : int64; x : array<float>; n, d : int64) {
    for (r in range64(d)) {
        y[r] = dot_q8_blob(blob, boff, r, n, x)
    }
}

def private rms_f(var o : array<float>; x : array<float>; w : array<float>; woff, n : int64; eps : float) {
    unsafe {
        rmsnorm(addr(o[0]), addr<float const?>(x[0]), addr<float const?>(w[woff]), n, eps)
    }
}

//! The target trunk's hidden that the drafter's `pre_projection` consumes, read after a decode
//! step at the last position. `post_norm` picks the reading: true applies the trunk's final RMS
//! norm, false returns the last decoder layer's output unnormed. The two are different tensors.
def gemma_target_hidden(t : Model; var s : Session; post_norm : bool; var dst : array<float>) {
    let dim = t.config.dim
    dst |> resize(dim)
    if (post_norm) {
        rms_at(dst, t, t.rms_final_off, s.x, dim)
    } else {
        copy_floats(s.x, 0l, dst, 0l, dim)
    }
}

// this step's cos/sin row for one attention class: the sliding class ropes over its own head size
// at theta_swa with no frequency factors, the full class over hs at theta with the p-RoPE table
def private drafter_rope_row(dr : GemmaDrafter; var cosr : array<float>; var sinr : array<float>;
                             var ff : array<float>; anchor, hs : int64; slid : bool) {
    let theta = slid ? dr.rope_theta_swa : dr.rope_theta
    let use_ff = !slid && dr.rope_freqs_off >= 0l
    if (use_ff && long_length(ff) != dr.rope_freqs_n) {
        ff |> resize(dr.rope_freqs_n)
        copy_floats(dr.fblob, dr.rope_freqs_off, ff, 0l, dr.rope_freqs_n)
    }
    build_rope_tabs(cosr, sinr, theta, 1.0, 1.0, ff, use_ff, anchor, 1l, hs)
}

// one drafter layer's attention over the target's frozen K/V at kv_layer[l], scale 1.0, GQA
// broadcast, sliding layers masked to keys within the window of the anchor
def private drafter_attention(t : Model; var s : Session; dr : GemmaDrafter; l, anchor : int64;
                              q : array<float>; var attn : array<float>; var scores : array<float>;
                              var kvrow : array<float>) {
    let c = t.config
    let kvl = dr.kv_layer[l]
    let hs = dr.hs[l]
    if (layer_head_size(c, kvl) != hs || layer_n_kv_heads(c, kvl) != dr.kvh[l]) {
        panic("gemma drafter layer {l}: target layer {kvl} is {layer_n_kv_heads(c, kvl)}x{layer_head_size(c, kvl)}, drafter wants {dr.kvh[l]}x{hs}")
    }
    let kv_dim = layer_kv_dim(c, kvl)
    let kv_mul = dr.n_heads / dr.kvh[l]
    // visible keys [lo, anchor): the seed at `anchor` is unfed; sliding masks anchor - key >= window
    let lo = dr.sliding[l] ? max(0l, anchor - dr.sliding_window + 1l) : 0l
    let cnt = anchor - lo
    var kcl = kv_layer_kbase(s, kvl, c.seq_len)
    var vcl = kv_layer_vbase(s, kvl, c.seq_len)
    for (hq in range64(dr.n_heads)) {
        let qoff = hq * hs
        let sub = (hq / kv_mul) * hs
        for (j in range64(lo, anchor)) {
            let prow = kv_phys_row(s, j)
            unsafe {
                kv_load_row(kcl, s.kv_dtype_k, kv_dim, prow, sub, hs, addr(kvrow[0]))
            }
            var acc = 0.0
            for (i in range64(hs)) {
                acc += q[qoff + i] * kvrow[i]
            }
            scores[j - lo] = acc
        }
        softmax(scores, cnt)
        for (i in range64(hs)) {
            attn[qoff + i] = 0.0
        }
        for (j in range64(lo, anchor)) {
            let prow = kv_phys_row(s, j)
            unsafe {
                kv_load_row(vcl, s.kv_dtype_v, kv_dim, prow, sub, hs, addr(kvrow[0]))
            }
            let w = scores[j - lo]
            for (i in range64(hs)) {
                attn[qoff + i] += w * kvrow[i]
            }
        }
    }
}

//! The drafter's CPU oracle: one step at `anchor` (the seed's position = the target's n_past: the
//! query ropes there and sees the target keys [0, anchor)) from token `tok` and target hidden `h`.
//! Fills `out` with the greedy token, the full-vocab logits and the 2816 hidden the next step feeds.
def gemma_draft_step_cpu(t : Model; var s : Session; dr : GemmaDrafter; tok : int64;   // nolint:STYLE038 - one forward pass; the phases share the scratch and must stay in order
                         h : array<float>; anchor : int64; var out : GemmaDraftOut) {
    if (!dr.ok) {
        panic("gemma_draft_step_cpu: drafter did not load ({dr.why})")
    }
    if (!dr.kv_bound) {
        panic("gemma_draft_step_cpu: kv_layer is unbound - call gemma_drafter_bind_target first")
    }
    if (long_length(h) < dr.tdim) {
        panic("gemma_draft_step_cpu: hidden is {long_length(h)} wide, expected {dr.tdim}")
    }
    let dim = dr.dim
    let eps = dr.eps
    var xcat : array<float>
    xcat |> resize(2l * dr.tdim)
    embed_row(t, tok, unsafe(addr(xcat[0])))
    let escale = sqrt(float(dr.tdim))
    for (i in range64(dr.tdim)) {
        xcat[i] *= escale
    }
    copy_floats(h, 0l, xcat, dr.tdim, dr.tdim)
    var xin : array<float>
    xin |> resize(dim)
    mv_q8_blob(xin, dr.blob, dr.pre_proj_off, xcat, 2l * dr.tdim, dim)

    var nrm : array<float>
    nrm |> resize(dim)
    var o : array<float>
    o |> resize(dim)
    var x1 : array<float>
    x1 |> resize(dim)
    var g : array<float>
    g |> resize(dr.hidden)
    var u : array<float>
    u |> resize(dr.hidden)
    var maxhs = 0l
    for (v in dr.hs) {
        maxhs = max(maxhs, v)
    }
    var q : array<float>
    q |> resize(dr.n_heads * maxhs)
    var attn : array<float>
    attn |> resize(dr.n_heads * maxhs)
    var kvrow : array<float>
    kvrow |> resize(maxhs)
    var scores : array<float>
    scores |> resize(anchor + 1l)
    var cosr : array<float>
    var sinr : array<float>
    var ff : array<float>

    for (l in range64(dr.n_layers)) {
        let hs = dr.hs[l]
        let qd = dr.n_heads * hs
        rms_f(nrm, xin, dr.fblob, dr.attn_norm_offs[l], dim, eps)
        mv_q8_blob(q, dr.blob, dr.wq_offs[l], nrm, dim, qd)
        unsafe {
            let qnp = addr<float const?>(dr.fblob[dr.q_norm_offs[l]])
            for (hq in range64(dr.n_heads)) {
                var qp = addr(q[hq * hs])
                rmsnorm(qp, qp, qnp, hs, eps)
            }
        }
        drafter_rope_row(dr, cosr, sinr, ff, anchor, hs, dr.sliding[l])
        unsafe {
            rope_scaled_neox_tab(addr(q[0]), hs, qd, addr<float const?>(cosr[0]), addr<float const?>(sinr[0]))
        }
        drafter_attention(t, s, dr, l, anchor, q, attn, scores, kvrow)
        mv_q8_blob(o, dr.blob, dr.wo_offs[l], attn, qd, dim)
        rms_f(o, o, dr.fblob, dr.post_attn_norm_offs[l], dim, eps)
        for (i in range64(dim)) {
            x1[i] = xin[i] + o[i]
        }
        rms_f(nrm, x1, dr.fblob, dr.ffn_norm_offs[l], dim, eps)
        mv_q8_blob(g, dr.blob, dr.w1_offs[l], nrm, dim, dr.hidden)
        mv_q8_blob(u, dr.blob, dr.w3_offs[l], nrm, dim, dr.hidden)
        geglu(g, u, dr.hidden)
        mv_q8_blob(o, dr.blob, dr.w2_offs[l], g, dr.hidden, dim)
        rms_f(o, o, dr.fblob, dr.post_ffn_norm_offs[l], dim, eps)
        let ls = dr.layer_scale[l]
        for (i in range64(dim)) {
            xin[i] = (x1[i] + o[i]) * ls
        }
    }
    rms_f(nrm, xin, dr.fblob, dr.out_norm_off, dim, eps)
    out.logits |> resize(dr.vocab)
    mv_q8_blob(out.logits, dr.blob, dr.tok_emb_off, nrm, dim, dr.vocab)
    var best = 0l
    var bv = out.logits[0]
    for (v in range64(dr.vocab)) {
        if (out.logits[v] > bv) {
            bv = out.logits[v]
            best = v
        }
    }
    out.token = best
    out.h_next |> resize(dr.tdim)
    mv_q8_blob(out.h_next, dr.blob, dr.post_proj_off, nrm, dim, dr.tdim)
}
