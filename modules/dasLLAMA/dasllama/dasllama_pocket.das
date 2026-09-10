options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_pocket shared public

require dasllama/dasllama_lint public
require math
require strings
require daslib/fio
require daslib/strings_boost
require dasllama/dasllama_gguf
require dasllama/dasllama_common   // reserve_resize
require dasllama/dasllama_math
require dasllama/dasllama_spm
require dasllama/dasllama_unicode
require dasllama/dasllama_tts_types
require dasllama/dasllama_tts_blocks

//! The Pocket TTS family (Kyutai): a continuous-audio language model - a causal transformer
//! backbone over 32-float latent frames at 12.5 Hz, conditioned on a voice prompt (a clip
//! through the codec's encoder) and the text's unigram SentencePiece ids, a one-step flow head
//! that samples each frame, and the Mimi-derived codec decoder that turns frames into 24 kHz
//! PCM. The weight map of the converted GGUF (`harness/convert_pocket.py`), the assembly, the
//! reference chunker and the voice roster live here; every operator is the block home's.

// ===== weights =====

struct PocketLayer {
    norm1 : TtsNorm = TtsNorm()
    in_proj : TtsLinear = TtsLinear()    // [3d][d]: q, k, v stacked, each [heads][dh]
    out_proj : TtsLinear = TtsLinear()
    norm2 : TtsNorm = TtsNorm()
    ffn1 : TtsLinear = TtsLinear()
    ffn2 : TtsLinear = TtsLinear()
    scale1 : TtsWeight = TtsWeight()     // per-channel layer scale; empty = none
    scale2 : TtsWeight = TtsWeight()
}

struct PocketTransformer {
    layers : array<PocketLayer>
    d : int64
    heads : int64
    ffn : int64
    context : int64      // the key window; 0 = every key
    period : float       // the rotary embedding's max period
    eps : float
}

struct PocketResBlock {
    ln : TtsNorm = TtsNorm()
    adaln : TtsLinear = TtsLinear()      // silu(y) -> shift, scale, gate
    mlp1 : TtsLinear = TtsLinear()
    mlp2 : TtsLinear = TtsLinear()
}

struct PocketHead {
    input_proj : TtsLinear = TtsLinear()
    cond_embed : TtsLinear = TtsLinear()
    time_const : TtsWeight = TtsWeight()   // the two timestep embeddings of the one-step decode, averaged, baked at load
    blocks : array<PocketResBlock>
    final_ln : TtsNorm = TtsNorm()         // the final layer's norm has no affine: unit weights minted at load
    final_adaln : TtsLinear = TtsLinear()
    final_linear : TtsLinear = TtsLinear()
    dim : int64
}

struct PocketResConv {
    conv1 : TtsConv1d = TtsConv1d()      // ELU, k3
    conv2 : TtsConv1d = TtsConv1d()      // ELU, k1; then the residual
}

struct PocketMimi {
    enc_in : TtsConv1d = TtsConv1d()
    enc_res : array<PocketResConv>
    enc_down : array<TtsConv1d>
    enc_out : TtsConv1d = TtsConv1d()
    enc_tf : PocketTransformer = PocketTransformer()
    downsample : TtsConv1d = TtsConv1d()   // 200 Hz -> 12.5 Hz, replicate-padded
    quant_proj : TtsConv1d = TtsConv1d()   // the 1x1 latent projection
    upsample : TtsConv1d = TtsConv1d()     // depthwise transposed, 12.5 -> 200 Hz
    dec_tf : PocketTransformer = PocketTransformer()
    dec_in : TtsConv1d = TtsConv1d()
    dec_up : array<TtsConv1d>
    dec_res : array<PocketResConv>
    dec_out : TtsConv1d = TtsConv1d()
    ratios : array<int64>
    hop : int64            // samples per 200 Hz step
    frame_steps : int64    // 200 Hz steps per latent frame
}

//! A voice as the backbone remembers it: every layer's key-value rows for the prompt (the BOS
//! row and the clip's projected latents), `len` positions long. A chunk's text and frames are
//! appended after `len` and forgotten by resetting the fill to it.
struct PocketVoiceState {
    caches : array<TtsKvCache>
    len : int64
}

struct PocketModel {
    family : string
    name : string
    language : string        // the package's config name, e.g. english_2026-04
    lang : string            // the code `caps` reports: en, fr, de, es, pt, it
    sample_rate : int
    frame_samples : int64    // PCM samples per latent frame
    latent_dim : int64
    default_temperature : float
    eos_threshold : float
    frames_after_eos : int   // -1: the text-length guess
    pad_short_inputs : bool
    remove_semicolons : bool
    append_terminal_punctuation : bool
    bos_before_voice : bool
    default_voice : string
    voice_names : array<string>
    voices : table<string; array<float>>          // a voice as a clip, 24 kHz mono: a cloned one, or a roster of the older file form
    voice_latents : table<string; array<float>>   // a voice as its clip's latent frames [frames][latent_dim]: the roster's stored form
    voice_states : table<string; PocketVoiceState>
    cloning : bool           // the file carries the codec encoder, so a clip can become a voice
    text_emb : TtsWeight = TtsWeight()     // [n_bins + 1][d]
    bos_emb : TtsWeight = TtsWeight()      // [latent_dim]: the first frame's input
    bos_voice : TtsWeight = TtsWeight()    // [d]: the row before the voice prompt
    input_linear : TtsLinear = TtsLinear()
    backbone : PocketTransformer = PocketTransformer()
    out_norm : TtsNorm = TtsNorm()
    out_eos : TtsLinear = TtsLinear()
    speaker_proj : TtsLinear = TtsLinear()
    emb_std : TtsWeight = TtsWeight()
    emb_mean : TtsWeight = TtsWeight()
    head : PocketHead = PocketHead()
    mimi : PocketMimi = PocketMimi()
    tok : SpmTokenizer = SpmTokenizer()
    unk_id : int64
    reserved_ids : array<int64>   // the control ids (unknown, BOS, EOS, padding): text never encodes to them
    byte_fallback : bool
    q8 : bool                // the lane this model serves: the GEMM weights as Q8_0 quants, or f32
}

// ===== the serving lane: f32 planes (the parity rail's form) or Q8_0 quants on the GEMMs, the
// same pin and policy as the StyleTTS2 families'; unpinned, a K-quant tensor of the file serves
// as its own planes (the kq lane), so a file's formats are the lanes it can take =====

enum private PocketLane {
    unset
    exact
    q8
}
var private g_pocket_q8_pin = PocketLane.unset

def private pocket_serve_q8_() : bool {
    if (g_pocket_q8_pin != PocketLane.unset) {
        return g_pocket_q8_pin == PocketLane.q8
    }
    return true
}

//! Pin the GEMM weights' format for subsequent loads: Q8_0 quants or the file's own f32 (a
//! K-quant tensor dequantizes into either); ``reset_pocket_q8`` returns to the policy default,
//! which serves a K-quant tensor as its own planes. The facade: ``set_tts_q8`` / ``reset_tts_q8``.
def set_pocket_q8(on : bool) {
    g_pocket_q8_pin = on ? PocketLane.q8 : PocketLane.exact
}

def reset_pocket_q8() {
    g_pocket_q8_pin = PocketLane.unset
}

//! Would the next load serve its GEMMs as q8 - the pin when set, the policy otherwise.
def pocket_serves_q8() : bool => pocket_serve_q8_()

//! Would the next load serve a K-quant tensor as its own planes - true unless a lane is pinned.
def pocket_serves_native() : bool => g_pocket_q8_pin == PocketLane.unset

// what the readers mint while a load runs: the lane's quant choice and the backend's repack
var private g_stage_q8 = false
var private g_stage_repack = false
var private g_stage_native = false

def finalize(var m : PocketModel) {
    delete m.voice_names
    delete m.voices
    delete m.voice_latents
    delete m.voice_states
    delete m.text_emb
    delete m.bos_emb
    delete m.bos_voice
    delete m.input_linear
    delete m.backbone
    delete m.out_norm
    delete m.out_eos
    delete m.speaker_proj
    delete m.emb_std
    delete m.emb_mean
    delete m.head
    delete m.mimi
    delete m.tok
}

//! The activation carrier one synthesis reuses (sec.2.31's discipline): every stage buffer a
//! `@scratch` field grown once, the waveform the output.
struct PocketScratch {
    @scratch @exact_size rows : array<float>     // the transformer's residual rows
    @scratch @exact_size xb : array<float>
    @scratch @exact_size qkv : array<float>
    @scratch @exact_size q : array<float>
    @scratch @exact_size k : array<float>
    @scratch @exact_size v : array<float>
    @scratch @exact_size ctx : array<float>
    @scratch @exact_size h : array<float>
    @scratch @exact_size x_in : array<float>     // the frame loop's latent input
    @scratch @exact_size cond : array<float>     // the backbone's last row after out_norm
    @scratch @exact_size eos : array<float>
    @scratch @exact_size hy : array<float>       // the head's conditioning
    @scratch @exact_size hx : array<float>       // the head's running state
    @scratch @exact_size hn : array<float>
    @scratch @exact_size hm : array<float>
    @scratch @exact_size hmod : array<float>
    @scratch @exact_size x0 : array<float>
    @scratch @exact_size x1 : array<float>
    @scratch @exact_size latents : array<float>  // [frames][latent_dim], denormalized
    @scratch @exact_size conds : array<float>    // [frames][d]: every frame's conditioning row, the parity rail's witness
    @scratch @exact_size eos_logits : array<float>
    @scratch @exact_size ca : array<float>       // the codec chain's ping-pong rows
    @scratch @exact_size cb : array<float>
    @scratch @exact_size cc : array<float>
    @scratch @exact_size cpad : array<float>
    @scratch @exact_size wave : array<float>
    codec_caches : array<TtsKvCache>
    rng : TtsRng
}

def finalize(var sc : PocketScratch) {
    delete sc.rows
    delete sc.xb
    delete sc.qkv
    delete sc.q
    delete sc.k
    delete sc.v
    delete sc.ctx
    delete sc.h
    delete sc.x_in
    delete sc.cond
    delete sc.eos
    delete sc.hy
    delete sc.hx
    delete sc.hn
    delete sc.hm
    delete sc.hmod
    delete sc.x0
    delete sc.x1
    delete sc.latents
    delete sc.conds
    delete sc.eos_logits
    delete sc.ca
    delete sc.cb
    delete sc.cc
    delete sc.cpad
    delete sc.wave
    delete sc.codec_caches
}

// ===== the GGUF =====

let private LN_EPS = 1e-5
let private HEAD_LN_EPS = 1e-6
let private TIME_RMS_EPS = 1e-5   // the timestep embedder's norm: its activations' variance is 1e-4-ish, so the epsilon is load-bearing
let private MAX_GEN_TOKENS_PER_SECOND = 3.0   // the reference's estimate of speech rate, for the frame cap
let private GEN_SECONDS_PADDING = 2.0
let private FRAME_RATE = 12.5

def private need_tensor(m : GGUFMeta; name : string) : int {
    let ti = gguf_find_tensor(m, name)
    if (ti < 0) {
        panic("dasLLAMA pocket: tensor '{name}' missing from the model")
    }
    return ti
}

def private read_arr(m : GGUFMeta; bytes : array<uint8> | #; name : string) : array<float> {
    let ti = need_tensor(m, name)
    var inscope a : array<float>
    a |> reserve_resize(m.tensors[ti].n_elem)
    gguf_read_tensor_f32(m, bytes, name, a, 0l, m.tensors[ti].n_elem)
    return <- a
}

def private read_weight(m : GGUFMeta; bytes : array<uint8> | #; name : string) : TtsWeight {
    var w = TtsWeight()
    w.a <- read_arr(m, bytes, name)
    return <- w
}

// a Q8_0 tensor of the published file: its blocks go straight into the int8 plane on the q8 lane
var private g_stage_file_q8 = false   // the file's `pocket.weights` says q8: its Q8_0 tensors are in the kernels' layout

// a Q8_0 tensor is read as the kernels' own blocks only where the converter wrote it that way:
// `pocket.weights = "q8"` names the layout, and the block-home rule (32-wide on both dims, a
// dense stride-1 conv) is what the converter's q8_linear / q8_conv wrote a second time
def private is_q8_tensor(m : GGUFMeta; name : string) : bool {
    if (gguf_tensor_type(m, name) != GGML_TYPE_Q8_0) {
        return false
    }
    if (!g_stage_file_q8) {
        panic("dasLLAMA pocket: '{name}' is Q8_0 in a file whose pocket.weights is not q8 - the reader takes the blocks' layout from the converter's q8 form only")
    }
    return true
}

def private read_linear(m : GGUFMeta; bytes : array<uint8> | #; prefix : string; vec_only : bool = false) : TtsLinear {
    var l = TtsLinear()
    let wname = "{prefix}.weight"
    let ti = need_tensor(m, wname)
    l.nin = m.tensors[ti].dims[0]
    l.nout = m.tensors[ti].dims[1]
    if (gguf_find_tensor(m, "{prefix}.bias") >= 0) {
        l.b <- read_arr(m, bytes, "{prefix}.bias")
    }
    if (is_q8_tensor(m, wname) && (l.nin % 32l != 0l || l.nout % 32l != 0l)) {
        panic("dasLLAMA pocket: '{wname}' [{l.nout}][{l.nin}] is Q8_0, a shape the q8 lane does not serve (not 32-wide on both dims)")
    }
    if (g_stage_native && gguf_tensor_type(m, wname) == GGML_TYPE_Q4_K) {
        // the kq lane: the file's own K-quant planes, straight into the layer
        let n = l.nout * l.nin
        if (l.nin % 256l != 0l) {
            panic("dasLLAMA pocket: '{wname}' [{l.nout}][{l.nin}] is Q4_K on a width that is not a whole number of 256-superblocks")
        }
        var inscope kq : array<uint8>
        var inscope ks : array<uint8>
        kq |> reserve_resize(n / 256l * kq_qsb(KqFmt.k4))
        ks |> reserve_resize(n / 256l * kq_ssb(KqFmt.k4))
        gguf_transcode_q4k(m, bytes, wname, kq, ks, 0l, n)
        linear_take_kq(l, 4, kq, ks, g_stage_repack)
        return <- l
    }
    if (g_stage_q8 && is_q8_tensor(m, wname)) {
        // a vector-only layer the file stores Q8_0 (the flow head) takes the blocks too: its GEMV runs on the q8 lane
        let n = l.nout * l.nin
        l.wq |> reserve_resize(n)
        l.wqs |> reserve_resize(n / 32l)
        gguf_transcode_q8_0(m, bytes, wname, l.wq, l.wqs, 0l, 0l, n)
        if (g_stage_repack) {
            repack_q8q8_weight(l.wq, l.wqs, 0l, l.nin, l.nout)
        }
        l.q8 = true
        return <- l
    }
    l.w <- read_arr(m, bytes, wname)   // a Q8_0 or K-quant tensor dequantizes here: the f32 lane of a published file, or the q8 lane pinned over a K-quant one
    linear_prepare(l, true, g_stage_q8 && !vec_only, g_stage_repack, vec_only)
    return <- l
}

def private read_norm(m : GGUFMeta; bytes : array<uint8> | #; prefix : string) : TtsNorm {
    var n = TtsNorm()
    n.w <- read_arr(m, bytes, "{prefix}.weight")
    n.b <- read_arr(m, bytes, "{prefix}.bias")
    return <- n
}

// the published file's conv: a Q8_0 slab [cout][k][cin] (the tap-stacked rows layout, innermost
// cin) - taken as the q8 lane's blocks, or dequantized and re-laid into PyTorch's [cout][cin][k];
// the slab is the dense stride-1 conv's shape alone, any other Q8_0 conv is refused, never guessed
[arch(at="../ARCHITECTURE_POCKET.md#pocket-q8-file")]
def private read_conv_q8(m : GGUFMeta; bytes : array<uint8> | #; wname : string; var c : TtsConv1d) {
    let ti = need_tensor(m, wname)
    c.cin = m.tensors[ti].dims[0]
    c.k = m.tensors[ti].dims[1]
    c.cout = m.tensors[ti].dims[2]
    let kc = c.k * c.cin
    if (c.stride != 1l || c.transposed || c.groups != 1l || c.cin % 32l != 0l || c.cout % 32l != 0l) {
        panic("dasLLAMA pocket: '{wname}' is Q8_0 with stride {c.stride}, groups {c.groups}{c.transposed ? ", transposed" : ""}, {c.cin} -> {c.cout} channels: not the dense stride-1 32-wide conv the slab layout is for")
    }
    if (g_stage_q8) {
        c.wq |> reserve_resize(c.cout * kc)
        c.wqs |> reserve_resize(c.cout * kc / 32l)
        gguf_transcode_q8_0(m, bytes, wname, c.wq, c.wqs, 0l, 0l, c.cout * kc)
        if (g_stage_repack) {
            repack_q8q8_weight(c.wq, c.wqs, 0l, kc, c.cout)
        }
        c.q8 = true
        c.q8_k = kc
        c.cout_s = c.cout
        return
    }
    var inscope slab <- read_arr(m, bytes, wname)
    c.w |> reserve_resize(c.cout * kc)
    for (co in range64(c.cout)) {
        for (kk in range64(c.k)) {
            for (ci in range64(c.cin)) {
                c.w[(co * c.cin + ci) * c.k + kk] = slab[co * kc + kk * c.cin + ci]
            }
        }
    }
    conv1d_prepare(c, g_stage_q8, g_stage_repack)
}

// a causal conv's context is its geometry: a forward conv's left zero pad of `k - stride`, or
// the `k - stride` tail of a transposed conv the stream never emits
[arch(at="../ARCHITECTURE_POCKET.md#pocket-one-shot-codec")]
def private read_conv(m : GGUFMeta; bytes : array<uint8> | #; prefix : string; stride, groups : int64; transposed : bool) : TtsConv1d {
    var c = TtsConv1d()
    let wname = "{prefix}.weight"
    if (gguf_find_tensor(m, "{prefix}.bias") >= 0) {
        c.b <- read_arr(m, bytes, "{prefix}.bias")
    }
    c.stride = stride
    c.groups = groups
    c.transposed = transposed
    c.served_rows = true
    if (is_q8_tensor(m, wname)) {
        read_conv_q8(m, bytes, wname, c)
        c.pad_l = c.k - stride
        c.pad_r = 0l
        return <- c
    }
    c.w <- read_arr(m, bytes, wname)
    let ti = need_tensor(m, wname)
    c.k = m.tensors[ti].dims[0]
    if (transposed) {
        c.cin = m.tensors[ti].dims[2]
        c.cout = m.tensors[ti].dims[1] * groups
        c.pad_l = 0l
        c.pad_r = c.k - stride
    } else {
        c.cout = m.tensors[ti].dims[2]
        c.cin = m.tensors[ti].dims[1] * groups
        c.pad_l = c.k - stride
        c.pad_r = 0l
    }
    conv1d_prepare(c, g_stage_q8, g_stage_repack)
    return <- c
}

def private read_res_conv(m : GGUFMeta; bytes : array<uint8> | #; prefix : string) : PocketResConv {
    var b = PocketResConv()
    b.conv1 <- read_conv(m, bytes, "{prefix}.block.1.conv", 1l, 1l, false)
    b.conv2 <- read_conv(m, bytes, "{prefix}.block.3.conv", 1l, 1l, false)
    return <- b
}

def private read_transformer(m : GGUFMeta; bytes : array<uint8> | #; prefix : string; n_layers, d, heads, ffn, context : int64;
                             period : float; with_scale : bool) : PocketTransformer {
    var tf = PocketTransformer()
    tf.d = d
    tf.heads = heads
    tf.ffn = ffn
    tf.context = context
    tf.period = period
    tf.eps = LN_EPS
    tf.layers |> resize(int(n_layers))
    for (i in range64(n_layers)) {
        let p = "{prefix}.{i}"
        tf.layers[i].norm1 <- read_norm(m, bytes, "{p}.norm1")
        tf.layers[i].in_proj <- read_linear(m, bytes, "{p}.self_attn.in_proj")
        tf.layers[i].out_proj <- read_linear(m, bytes, "{p}.self_attn.out_proj")
        tf.layers[i].norm2 <- read_norm(m, bytes, "{p}.norm2")
        tf.layers[i].ffn1 <- read_linear(m, bytes, "{p}.linear1")
        tf.layers[i].ffn2 <- read_linear(m, bytes, "{p}.linear2")
        if (with_scale) {
            tf.layers[i].scale1 <- read_weight(m, bytes, "{p}.layer_scale_1.scale")
            tf.layers[i].scale2 <- read_weight(m, bytes, "{p}.layer_scale_2.scale")
        }
    }
    return <- tf
}

// the sinusoidal timestep embedder at one time value: cos and sin of t * freqs, the two-layer
// MLP, then the variance-scaled norm - the reference's `_rms_norm`, a variance with N - 1
def private time_embed_at(m : GGUFMeta; bytes : array<uint8> | #; prefix : string; t : double) : array<float> {
    var inscope freqs <- read_arr(m, bytes, "{prefix}.freqs")
    var inscope w1 <- read_linear(m, bytes, "{prefix}.mlp.0", true)
    var inscope w2 <- read_linear(m, bytes, "{prefix}.mlp.2", true)
    var inscope alpha <- read_arr(m, bytes, "{prefix}.mlp.3.alpha")
    let half = long_length(freqs)
    var inscope emb : array<float>
    emb |> reserve_resize(2l * half)
    for (j in range64(half)) {
        let a = float(t * double(freqs[j]))
        emb[j] = cos(a)
        emb[half + j] = sin(a)
    }
    var inscope h1 : array<float>
    linear_vec(w1, emb, h1)
    silu(h1, long_length(h1))
    var inscope h2 : array<float>
    linear_vec(w2, h1, h2)
    var_scale_norm(h2, alpha, long_length(h2), TIME_RMS_EPS)
    return <- h2
}

[arch(at="../ARCHITECTURE_POCKET.md#pocket-one-step-head")]
def private read_head(m : GGUFMeta; bytes : array<uint8> | #; var hd : PocketHead; depth : int64) {
    hd.input_proj <- read_linear(m, bytes, "head.input_proj", true)
    hd.cond_embed <- read_linear(m, bytes, "head.cond_embed", true)
    hd.dim = hd.cond_embed.nout
    // one decode step integrates from s = 0 to t = 1: the two embeddings are constants
    var inscope t0 <- time_embed_at(m, bytes, "head.time_embed.0", 0.0lf)
    var inscope t1 <- time_embed_at(m, bytes, "head.time_embed.1", 1.0lf)
    hd.time_const.a |> reserve_resize(hd.dim)
    for (j in range64(hd.dim)) {
        hd.time_const.a[j] = (t0[j] + t1[j]) * 0.5
    }
    hd.blocks |> reserve_resize(depth)
    for (i in range64(depth)) {
        let p = "head.res_blocks.{i}"
        hd.blocks[i].ln <- read_norm(m, bytes, "{p}.in_ln")
        hd.blocks[i].adaln <- read_linear(m, bytes, "{p}.adaLN_modulation.1", true)
        hd.blocks[i].mlp1 <- read_linear(m, bytes, "{p}.mlp.0", true)
        hd.blocks[i].mlp2 <- read_linear(m, bytes, "{p}.mlp.2", true)
    }
    hd.final_adaln <- read_linear(m, bytes, "head.final_layer.adaLN_modulation.1", true)
    hd.final_linear <- read_linear(m, bytes, "head.final_layer.linear", true)
    hd.final_ln.w |> resize(hd.dim)
    hd.final_ln.b |> resize(hd.dim)
    for (j in range64(hd.dim)) {
        hd.final_ln.w[j] = 1.0
        hd.final_ln.b[j] = 0.0
    }
}

def private read_mimi(m : GGUFMeta; bytes : array<uint8> | #; var mm : PocketMimi; n_layers, d, heads, ffn, context : int64; period : float; encoder : bool) {
    mm.ratios <- gguf_int_array(m, bytes, "pocket.mimi.ratios")
    let nr = long_length(mm.ratios)
    mm.hop = 1l
    for (r in mm.ratios) {
        mm.hop *= r
    }
    mm.quant_proj <- read_conv(m, bytes, "mimi.quantizer.output_proj", 1l, 1l, false)
    // the two resamplers are kernel 2 x stride and mirror each other; the upsampler is always in the file
    mm.frame_steps = m.tensors[need_tensor(m, "mimi.upsample.convtr.convtr.weight")].dims[0] / 2l
    mm.upsample <- read_conv(m, bytes, "mimi.upsample.convtr.convtr", mm.frame_steps, d, true)
    if (encoder) {
        // the encoder's stages read the ratios reversed: 1 -> 64 channels first, doubling per stage
        mm.enc_in <- read_conv(m, bytes, "mimi.encoder.model.0.conv", 1l, 1l, false)
        mm.enc_res |> resize(int(nr))
        mm.enc_down |> resize(int(nr))
        for (i in range64(nr)) {
            let ratio = mm.ratios[nr - 1l - i]
            mm.enc_res[i] <- read_res_conv(m, bytes, "mimi.encoder.model.{1l + 3l * i}")
            mm.enc_down[i] <- read_conv(m, bytes, "mimi.encoder.model.{3l + 3l * i}.conv", ratio, 1l, false)
        }
        mm.enc_out <- read_conv(m, bytes, "mimi.encoder.model.{2l + 3l * nr}.conv", 1l, 1l, false)
        mm.enc_tf <- read_transformer(m, bytes, "mimi.enc_tf", n_layers, d, heads, ffn, context, period, true)
        mm.downsample <- read_conv(m, bytes, "mimi.downsample.conv.conv", 0l, 1l, false)
        verify(mm.downsample.k / 2l == mm.frame_steps)
        mm.downsample.stride = mm.frame_steps
        mm.downsample.pad_l = 0l   // replicate-padded by the caller, `frame_steps` rows
    }
    mm.dec_tf <- read_transformer(m, bytes, "mimi.dec_tf", n_layers, d, heads, ffn, context, period, true)
    mm.dec_in <- read_conv(m, bytes, "mimi.decoder.model.0.conv", 1l, 1l, false)
    mm.dec_up |> resize(int(nr))
    mm.dec_res |> resize(int(nr))
    for (i in range64(nr)) {
        mm.dec_up[i] <- read_conv(m, bytes, "mimi.decoder.model.{2l + 3l * i}.convtr", mm.ratios[i], 1l, true)
        mm.dec_res[i] <- read_res_conv(m, bytes, "mimi.decoder.model.{3l + 3l * i}")
    }
    mm.dec_out <- read_conv(m, bytes, "mimi.decoder.model.{2l + 3l * nr}.conv", 1l, 1l, false)
}

def private lang_code_of(language : string) : string {
    for (pair in [[ "english", "en" ], [ "french", "fr" ], [ "german", "de" ], [ "spanish", "es" ], [ "portuguese", "pt" ], [ "italian", "it"]]) {
        return pair[1] if (language |> starts_with(pair[0]))
    }
    return language
}

// the roster: each voice as its stored latent frames, or as a clip in the older file form (which
// needs the encoder to hear it)
def private read_roster(g : GGUFMeta; bytes : array<uint8> | #; var m : PocketModel; path : string) {
    m.voice_names <- gguf_str_array(g, bytes, "pocket.voices")
    for (v in m.voice_names) {
        if (gguf_find_tensor(g, "voice_latents.{v}") >= 0) {
            m.voice_latents[v] <- read_arr(g, bytes, "voice_latents.{v}")
        } elif (m.cloning) {
            m.voices[v] <- read_arr(g, bytes, "voice.{v}")
        } else {
            panic("dasLLAMA pocket: '{path}' stores voice '{v}' as a clip but carries no codec encoder to hear it with")
        }
    }
}

//! Whether `name` is a voice this model speaks: a roster voice (stored as latent frames, or as
//! a clip in the older file form) or a cloned one.
def pocket_has_voice(m : PocketModel; name : string) : bool {
    return key_exists(m.voice_latents, name) || key_exists(m.voices, name)
}

//! Load a converted Pocket TTS GGUF: the scalars from its `pocket.*` metadata, the weights
//! prepared for the rows kernels, the tokenizer, and the roster (its states built on first use).
def load_pocket(path : string) : PocketModel {
    var inscope m = PocketModel()
    m.q8 = pocket_serve_q8_()
    g_stage_q8 = m.q8
    g_stage_native = pocket_serves_native()
    g_stage_repack = m.q8 && select_matmul_backend_for_load_()
    to_log(LOG_INFO, "dasLLAMA pocket: GEMM lane {m.q8 ? "q8" : "f32"}{g_stage_native ? ", a K-quant tensor as its own planes" : ""} - {g_pocket_q8_pin != PocketLane.unset ? "pinned via set_tts_q8" : "the policy default"}\n")
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA pocket: cannot open model '{path}'")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        var inscope g <- parse_gguf_meta(bytes)
        m.family = gguf_str(g, bytes, "general.architecture")
        if (m.family != "pocket-tts") {
            panic("dasLLAMA pocket: '{path}' has architecture '{m.family}', not pocket-tts")
        }
        m.name = gguf_str(g, bytes, "general.name")
        m.language = gguf_str(g, bytes, "pocket.language")
        m.lang = lang_code_of(m.language)
        m.sample_rate = int(gguf_int(g, bytes, "pocket.sample_rate"))
        m.latent_dim = gguf_int(g, bytes, "pocket.latent_dim")
        m.default_temperature = gguf_f32(g, bytes, "pocket.default_temperature")
        m.eos_threshold = -4.0
        m.frames_after_eos = int(gguf_int(g, bytes, "pocket.frames_after_eos"))
        m.pad_short_inputs = gguf_int(g, bytes, "pocket.pad_short_inputs") != 0l
        m.remove_semicolons = gguf_int(g, bytes, "pocket.remove_semicolons") != 0l
        m.append_terminal_punctuation = gguf_int(g, bytes, "pocket.append_terminal_punctuation") != 0l
        m.bos_before_voice = gguf_int(g, bytes, "pocket.bos_before_voice") != 0l
        m.default_voice = gguf_str(g, bytes, "pocket.default_voice")
        g_stage_file_q8 = gguf_has(g, "pocket.weights") && gguf_str(g, bytes, "pocket.weights") == "q8"
        if (g_stage_file_q8 && !m.q8) {
            to_log(LOG_INFO, "dasLLAMA pocket: '{path}' carries Q8_0 GEMM weights; the f32 lane serves them dequantized\n")
        }
        m.cloning = !gguf_has(g, "pocket.cloning") || gguf_int(g, bytes, "pocket.cloning") != 0l
        read_roster(g, bytes, m, path)
        let d = gguf_int(g, bytes, "pocket.backbone.d_model")
        let heads = gguf_int(g, bytes, "pocket.backbone.heads")
        let layers = gguf_int(g, bytes, "pocket.backbone.layers")
        let ffn = d * gguf_int(g, bytes, "pocket.backbone.hidden_scale")
        let period = gguf_f32(g, bytes, "pocket.backbone.rope_max_period")
        m.text_emb <- read_weight(g, bytes, "flow_lm.conditioner.embed.weight")
        m.bos_emb <- read_weight(g, bytes, "flow_lm.bos_emb")
        m.bos_voice <- read_weight(g, bytes, "flow_lm.bos_before_voice")
        m.input_linear <- read_linear(g, bytes, "flow_lm.input_linear")
        m.backbone <- read_transformer(g, bytes, "backbone", layers, d, heads, ffn, 0l, period, false)
        m.out_norm <- read_norm(g, bytes, "flow_lm.out_norm")
        m.out_eos <- read_linear(g, bytes, "flow_lm.out_eos", true)
        m.speaker_proj.w <- read_arr(g, bytes, "flow_lm.speaker_proj_weight")
        m.speaker_proj.nin = m.latent_dim
        m.speaker_proj.nout = d
        linear_prepare(m.speaker_proj, true)
        m.emb_std <- read_weight(g, bytes, "flow_lm.emb_std")
        m.emb_mean <- read_weight(g, bytes, "flow_lm.emb_mean")
        read_head(g, bytes, m.head, gguf_int(g, bytes, "pocket.head.depth"))
        read_mimi(g, bytes, m.mimi, gguf_int(g, bytes, "pocket.mimi.layers"), gguf_int(g, bytes, "pocket.mimi.d_model"),
            gguf_int(g, bytes, "pocket.mimi.heads"), gguf_int(g, bytes, "pocket.mimi.dim_feedforward"),
            gguf_int(g, bytes, "pocket.mimi.context"), gguf_f32(g, bytes, "pocket.mimi.rope_max_period"), m.cloning)
        m.frame_samples = m.mimi.hop * m.mimi.frame_steps
        m.unk_id = gguf_int(g, bytes, "tokenizer.ggml.unknown_token_id")
        m.byte_fallback = gguf_int(g, bytes, "tokenizer.ggml.byte_fallback") != 0l
        m.reserved_ids |> push(m.unk_id)
        for (key in ["tokenizer.ggml.bos_token_id", "tokenizer.ggml.eos_token_id", "tokenizer.ggml.padding_token_id"]) {
            if (gguf_has(g, key)) {
                m.reserved_ids |> push(gguf_int(g, bytes, key))
            }
        }
    }
    fclose(f)
    g_stage_file_q8 = false
    g_stage_native = false
    m.tok <- load_tokenizer_gguf(path)
    return <- m
}

// ===== the transformer, rows form =====

// q, k, v out of the stacked projection rows [t][3d]
def private split_qkv(qkv : array<float>; t, d : int64; @scratch @exact_size var q : array<float>;
                      @scratch @exact_size var k : array<float>; @scratch @exact_size var v : array<float>) {
    q |> reserve_resize(t * d)
    k |> reserve_resize(t * d)
    v |> reserve_resize(t * d)
    for (r in range64(t)) {
        let row = r * 3l * d
        for (j in range64(d)) {
            q[r * d + j] = qkv[row + j]
            k[r * d + j] = qkv[row + d + j]
            v[r * d + j] = qkv[row + 2l * d + j]
        }
    }
}

// a projection over t rows: one row - a frame step - takes the decode entry (the q8 GEMV), the
// prompt's rows the rows GEMM
def private proj(l : TtsLinear; x : array<float>; t : int64; @scratch @exact_size var y : array<float>) {
    if (t == 1l) {
        linear_rows_decode(l, x, y)
    } else {
        linear_rows(l, x, t, y)
    }
}

def private layer_attention(l : PocketLayer; tf : PocketTransformer; var x : array<float>; t, pos0 : int64;
                            var kc : TtsKvCache; var sc : PocketScratch) {
    let d = tf.d
    let dh = d / tf.heads
    copy_rows(sc.xb, x, t * d)
    layernorm_rows(sc.xb, t, d, l.norm1, tf.eps)
    proj(l.in_proj, sc.xb, t, sc.qkv)
    split_qkv(sc.qkv, t, d, sc.q, sc.k, sc.v)
    rope_rows(sc.q, t, d, dh, pos0, tf.period)
    rope_rows(sc.k, t, d, dh, pos0, tf.period)
    kv_cache_append(kc, sc.k, sc.v, t, d)
    attention_causal_rows(sc.q, t, d, tf.context, kc, sc.ctx)
    proj(l.out_proj, sc.ctx, t, sc.xb)
    if (!empty(l.scale1.a)) {
        layer_scale_rows(sc.xb, t, d, l.scale1.a)
    }
    add_rows(x, sc.xb, t * d)
}

def private layer_ffn(l : PocketLayer; tf : PocketTransformer; var x : array<float>; t : int64; var sc : PocketScratch) {
    let d = tf.d
    copy_rows(sc.xb, x, t * d)
    layernorm_rows(sc.xb, t, d, l.norm2, tf.eps)
    proj(l.ffn1, sc.xb, t, sc.h)
    gelu(sc.h, t * tf.ffn)
    proj(l.ffn2, sc.h, t, sc.xb)
    if (!empty(l.scale2.a)) {
        layer_scale_rows(sc.xb, t, d, l.scale2.a)
    }
    add_rows(x, sc.xb, t * d)
}

//! `t` rows of x [t][d] through the transformer at positions pos0.., every layer's keys and
//! values appended to its cache; x carries the residual out.
def transformer_rows(tf : PocketTransformer; var x : array<float>; t, pos0 : int64; var caches : array<TtsKvCache>; var sc : PocketScratch) {
    for (i, l in iter_range(tf.layers), tf.layers) {
        layer_attention(l, tf, x, t, pos0, caches[i], sc)
        layer_ffn(l, tf, x, t, sc)
    }
}

def private caches_init(var caches : array<TtsKvCache>; tf : PocketTransformer; cap : int64) {
    caches |> resize(length(tf.layers))
    for (kc in caches) {
        kv_cache_init(kc, tf.heads, tf.d / tf.heads, cap)
    }
}

// ===== the codec =====

def private res_conv_rows(b : PocketResConv; x : array<float>; t : int64; var sc : PocketScratch; @scratch @exact_size var y : array<float>) {
    let w = b.conv1.cin
    copy_rows(sc.cc, x, t * w)
    elu_rows(sc.cc, t * w)
    conv1d_rows(b.conv1, sc.cc, t, sc.cpad)
    elu_rows(sc.cpad, t * b.conv1.cout_s)
    conv1d_rows(b.conv2, sc.cpad, t, y)
    add_rows(y, x, t * w)
}

//! The clip's latent frames [frames][latent_dim] through the codec encoder - the voice prompt's
//! source. `pcm` is 24 kHz mono; its tail pads to a whole frame with zeros as the reference does.
def pocket_encode_latents(m : PocketModel; pcm : array<float>; var sc : PocketScratch; @scratch @exact_size var latents : array<float>) : int64 {
    let mm & = unsafe(m.mimi)
    let n = long_length(pcm)
    let frames = (n + m.frame_samples - 1l) / m.frame_samples
    let padded = frames * m.frame_samples
    sc.ca |> reserve_resize(padded)
    for (i in range64(padded)) {
        sc.ca[i] = i < n ? pcm[i] : 0.0
    }
    var t = padded
    conv1d_rows(mm.enc_in, sc.ca, t, sc.cb)
    for (i in range64(long_length(mm.ratios))) {
        res_conv_rows(mm.enc_res[i], sc.cb, t, sc, sc.ca)
        elu_rows(sc.ca, t * mm.enc_down[i].cin)
        conv1d_rows(mm.enc_down[i], sc.ca, t, sc.cb)
        t = conv1d_out_len(mm.enc_down[i], t)
    }
    elu_rows(sc.cb, t * mm.enc_out.cin)
    conv1d_rows(mm.enc_out, sc.cb, t, sc.ca)
    caches_init(sc.codec_caches, mm.enc_tf, t)
    transformer_rows(mm.enc_tf, sc.ca, t, 0l, sc.codec_caches, sc)
    pad_edge_left_rows(sc.ca, t, mm.enc_tf.d, mm.frame_steps, sc.cb)
    conv1d_rows(mm.downsample, sc.cb, t + mm.frame_steps, latents)
    return conv1d_out_len(mm.downsample, t + mm.frame_steps)
}

//! Latent frames [frames][latent_dim] (denormalized) through the codec decoder into
//! `frames * frame_samples` PCM samples - the whole chunk in one pass, which the causal padding
//! makes sample-identical to the reference's frame-by-frame stream.
[arch(at="../ARCHITECTURE_POCKET.md#pocket-one-shot-codec")]
def pocket_decode_latents(m : PocketModel; latents : array<float>; frames : int64; var sc : PocketScratch; @scratch @exact_size var wave : array<float>) {
    let mm & = unsafe(m.mimi)
    conv1d_rows(mm.quant_proj, latents, frames, sc.ca)
    conv1d_rows_transposed_depthwise(mm.upsample, sc.ca, frames, sc.cb)
    var t = conv1d_out_len(mm.upsample, frames)
    caches_init(sc.codec_caches, mm.dec_tf, t)
    transformer_rows(mm.dec_tf, sc.cb, t, 0l, sc.codec_caches, sc)
    conv1d_rows(mm.dec_in, sc.cb, t, sc.ca)
    for (i in range64(long_length(mm.ratios))) {
        elu_rows(sc.ca, t * mm.dec_up[i].cin)
        conv1d_rows(mm.dec_up[i], sc.ca, t, sc.cb)
        t = conv1d_out_len(mm.dec_up[i], t)
        res_conv_rows(mm.dec_res[i], sc.cb, t, sc, sc.ca)
    }
    elu_rows(sc.ca, t * mm.dec_out.cin)
    conv1d_rows(mm.dec_out, sc.ca, t, sc.cb)
    wave |> reserve_resize(t)
    let stride = mm.dec_out.cout_s
    for (i in range64(t)) {
        wave[i] = sc.cb[i * stride]
    }
}

// ===== the voice prompt =====

let private VOICE_CACHE_SLACK = 1024l   // positions past the prompt a chunk's text and frames may take before the caches grow
let POCKET_MAX_VOICE_SECONDS = 60l   // the longest clip a voice is cloned from: the state is the clip's frames per layer

//! The backbone's memory of a clip: its latents projected into the model's width, the BOS row in
//! front, run through the backbone at positions 0..; the state a synthesis continues from.
[arch(at="../ARCHITECTURE_POCKET.md#pocket-voice-state")]
def pocket_voice_state(m : PocketModel; pcm : array<float>; var sc : PocketScratch) : PocketVoiceState {
    var inscope lat : array<float>
    let frames = pocket_encode_latents(m, pcm, sc, lat)
    return <- voice_state_from_latents(m, lat, frames, sc)
}

//! The voice state from a clip's latent frames [frames][latent_dim] - the stored roster form,
//! and the second half of the clip path: the speaker projection, the BOS row, the backbone.
def voice_state_from_latents(m : PocketModel; lat : array<float>; frames : int64; var sc : PocketScratch) : PocketVoiceState {
    var inscope vs = PocketVoiceState()
    let d = m.backbone.d
    let bos = m.bos_before_voice ? 1l : 0l
    var inscope rows : array<float>
    rows |> reserve_resize((frames + bos) * d)
    var inscope proj : array<float>
    linear_rows(m.speaker_proj, lat, frames, proj)
    for (j in range64(bos * d)) {
        rows[j] = m.bos_voice.a[j]
    }
    for (i in range64(frames * d)) {
        rows[bos * d + i] = proj[i]
    }
    vs.len = frames + bos
    caches_init(vs.caches, m.backbone, vs.len + VOICE_CACHE_SLACK)
    transformer_rows(m.backbone, rows, vs.len, 0l, vs.caches, sc)
    return <- vs
}

//! Add (or replace) a voice: `pcm` at the model's sample rate, mono. It joins the roster under
//! `name` once its state is built (a clip the encoder refuses leaves no half voice behind), and a
//! replaced voice's state is freed rather than left in the table's slot.
def pocket_register_voice(var m : PocketModel; name : string; pcm : array<float>; var sc : PocketScratch) {
    if (!m.cloning) {
        panic("dasLLAMA pocket: voice '{name}': this file carries no codec encoder, so it cannot clone a voice")
    }
    if (empty(pcm)) {
        panic("dasLLAMA pocket: voice '{name}' has no samples")
    }
    if (long_length(pcm) > POCKET_MAX_VOICE_SECONDS * int64(m.sample_rate)) {
        panic("dasLLAMA pocket: voice '{name}' is a {long_length(pcm) / int64(m.sample_rate)} s clip; a voice is cloned from at most {POCKET_MAX_VOICE_SECONDS} s")
    }
    var inscope vs <- pocket_voice_state(m, pcm, sc)
    if (key_exists(m.voice_states, name)) {
        m.voice_states |> get(name) $(var old) {
            delete old
        }
        m.voice_states |> erase(name)
    }
    if (!key_exists(m.voices, name) && !key_exists(m.voice_latents, name)) {
        m.voice_names |> push(name)
    }
    m.voice_latents |> erase(name)
    m.voices[name] := pcm
    m.voice_states[name] <- vs
}

// a roster voice's state, built on first use: from its stored latent frames, or from its clip
def private ensure_voice_state(var m : PocketModel; name : string; var sc : PocketScratch) {
    return if (key_exists(m.voice_states, name))
    if (key_exists(m.voice_latents, name)) {
        var inscope lat : array<float>
        m.voice_latents |> get(name) $(stored) {
            lat := stored
        }
        m.voice_states[name] <- voice_state_from_latents(m, lat, long_length(lat) / m.latent_dim, sc)
        return
    }
    if (!key_exists(m.voices, name)) {
        panic("dasLLAMA pocket: unknown voice '{name}'")
    }
    var inscope clip : array<float>
    m.voices |> get(name) $(pcm) {
        clip := pcm
    }
    m.voice_states[name] <- pocket_voice_state(m, clip, sc)
}

// ===== the text =====

// the words of a text the way the reference counts them: runs between whitespace of any kind
def private word_count(cps : array<int>) : int {
    var words = 0
    var in_word = false
    for (c in cps) {
        let space = cpt_is_whitespace(c)
        if (!space && !in_word) {
            words++
        }
        in_word = !space
    }
    return words
}

//! The reference driver's preparation of one chunk: line breaks to spaces and ONE pass of two
//! spaces to one (its own rule, not a whitespace fold), an initial capital, a terminal mark, the
//! short-input padding; the frames it keeps generating after EOS. An empty text is refused, as there.
def prepare_pocket_text(m : PocketModel; text : string) : tuple<prepared : string; frames_after_eos : int> {
    var s = strip(text) |> replace("\n", " ") |> replace("\r", " ") |> replace("  ", " ")
    if (empty(s)) {
        panic("dasLLAMA pocket: the text is empty - nothing to say")
    }
    if (m.remove_semicolons) {
        s = replace(s, ";", ",")
    }
    var inscope cps <- utf8_to_cpts(s)
    let words = word_count(cps)
    let guess = words <= 4 ? 3 : 1
    // the reference upper-cases the first character; an ASCII one turns here, an accented one
    // stays as it is (no case table for it in the engine)
    let first = cpts_to_utf8(cps, 0, 1)
    let up = to_upper(first)
    if (up != first) {
        s = "{up}{cpts_to_utf8(cps, 1, length(cps))}"
    }
    // the released driver's rule (pocket-tts 3.1.0, the oracle): a prompt ending in a letter or
    // digit - accented letters included, the reference asks isalnum - takes a period; any mark,
    // a comma included, is left as it is
    if (m.append_terminal_punctuation) {
        let last = cps[length(cps) - 1]
        if (cpt_is_letter(last) || cpt_is_number(last)) {
            s = "{s}."
        }
    }
    if (m.pad_short_inputs && words < 5) {
        s = "        {s}"
    }
    return (prepared = s, frames_after_eos = guess)
}

//! Token ids of a prepared chunk.
def pocket_text_ids(m : PocketModel; prepared : string) : array<int64> {
    return <- unigram_encode(m.tok, prepared, m.unk_id, m.byte_fallback, m.reserved_ids)
}

let POCKET_MAX_TOKENS = 50   // the reference driver's chunk budget; past it the model skips words

// the ids of a marker string less its leading dummy-space piece
def private marker_ids(m : PocketModel; s : string) : array<int64> {
    var inscope ids <- pocket_text_ids(m, s)
    ids |> erase(0)
    return <- ids
}

// a boundary that sits right after a decimal point's period is no sentence end
def private decimal_boundary(m : PocketModel; ids : array<int64>; at : int) : bool {
    var inscope head : array<int64>
    var inscope tail : array<int64>
    head |> reserve(at)
    tail |> reserve(length(ids) - at)
    for (i, id in count(), ids) {
        if (i < at) {
            head |> push(id)
        } else {
            tail |> push(id)
        }
    }
    var inscope pre <- utf8_to_cpts(spm_decode_ids(m.tok, head))
    var inscope suf <- utf8_to_cpts(spm_decode_ids(m.tok, tail))
    let n = length(pre)
    return n >= 2 && pre[n - 1] == '.' && cpt_is_number(pre[n - 2]) && !empty(suf) && cpt_is_number(suf[0])
}

// the reference's boundary walk: a segment ends after a run of marker ids
def private boundary_indices(m : PocketModel; ids : array<int64>; markers : array<int64>; skip_decimals : bool) : array<int> {
    var inscope out : array<int>
    out |> reserve(length(ids) + 2)
    out |> push(0)
    var prev_marker = false
    for (i, id in count(), ids) {
        if (find_index(markers, id) >= 0) {
            prev_marker = true
        } else {
            if (prev_marker && !(skip_decimals && decimal_boundary(m, ids, i))) {
                out |> push(i)
            }
            prev_marker = false
        }
    }
    out |> push(length(ids))
    return <- out
}

struct private PocketSegment {
    n : int
    text : string
}

def private segments_of(m : PocketModel; ids : array<int64>; bounds : array<int>) : array<PocketSegment> {
    var inscope out : array<PocketSegment>
    out |> reserve(length(bounds))
    for (i in range(length(bounds) - 1)) {
        var inscope part : array<int64>
        part |> reserve(bounds[i + 1] - bounds[i])
        for (k in range(bounds[i], bounds[i + 1])) {
            part |> push(ids[k])
        }
        out |> push(PocketSegment(n = bounds[i + 1] - bounds[i], text = spm_decode_ids(m.tok, part)))
    }
    return <- out
}

[cold_path]
def private warn_over_budget(n, max_tokens : int; text : string) {
    to_log(LOG_WARNING, "dasLLAMA pocket: a chunk of {n} tokens over the {max_tokens} budget, the model may skip words: '{text}'\n")
}

//! The chunks one text takes, the reference driver's rule: the whole text prepared, split on
//! its sentence-mark ids (a decimal point excepted), an over-long sentence re-split on its
//! comma, semicolon and colon ids, then regrouped under the token budget; a piece nothing splits stays whole, warned.
[arch(at="../ARCHITECTURE_POCKET.md#pocket-chunker")]
def pocket_chunks(m : PocketModel; text : string; max_tokens : int = POCKET_MAX_TOKENS) : array<string> {
    var inscope chunks : array<string>
    return <- chunks if (empty(strip(text)))
    let prepared = strip(prepare_pocket_text(m, text).prepared)
    var inscope ids <- pocket_text_ids(m, prepared)
    var inscope ends <- marker_ids(m, ".!...?")
    var inscope weak <- marker_ids(m, ",;:")
    var inscope sentences <- segments_of(m, ids, boundary_indices(m, ids, ends, true))
    var inscope refined : array<PocketSegment>
    for (s in sentences) {
        if (s.n <= max_tokens) {
            refined |> push(s)
            continue
        }
        var inscope sub_ids <- pocket_text_ids(m, strip(s.text))
        var inscope subs <- segments_of(m, sub_ids, boundary_indices(m, sub_ids, weak, false))
        if (length(subs) > 1) {
            refined |> push_from(subs)
        } else {
            warn_over_budget(s.n, max_tokens, s.text)
            refined |> push(s)
        }
    }
    var inscope cur : array<string>
    cur |> reserve(length(refined))
    var cur_n = 0
    for (s in refined) {
        if (!empty(cur) && cur_n + s.n > max_tokens) {
            chunks |> push(strip(join(cur, " ")))
            cur |> clear()
            cur_n = 0
        }
        cur |> push(s.text)
        cur_n += s.n
    }
    if (!empty(cur)) {
        chunks |> push(strip(join(cur, " ")))
    }
    return <- chunks
}

// ===== the frame loop =====

//! One generated frame: the backbone over one input row at `pos`, the EOS logit and the flow
//! head's sample from the noise `x0` (32 floats, drawn or supplied); the normalized frame lands
//! in `x1`, the decoder's denormalized copy in `latent`.
def private frame_step(m : PocketModel; var vs : PocketVoiceState; pos : int64; var sc : PocketScratch) : float {
    let d = m.backbone.d
    linear_rows_decode(m.input_linear, sc.x_in, sc.rows)
    transformer_rows(m.backbone, sc.rows, 1l, pos, vs.caches, sc)
    copy_rows(sc.cond, sc.rows, d)
    layernorm_rows(sc.cond, 1l, d, m.out_norm, m.backbone.eps)
    linear_vec(m.out_eos, sc.cond, sc.eos)
    return sc.eos[0]
}

def private modulate(var x : array<float>; mod : array<float>; n : int64) {
    for (j in range64(n)) {
        x[j] = x[j] * (1.0 + mod[n + j]) + mod[j]
    }
}

//! The flow head at one step: x1 = x0 + v(0, 1, x0 | cond).
def private head_step(hd : PocketHead; var sc : PocketScratch) {
    let n = hd.dim
    linear_vec(hd.cond_embed, sc.cond, sc.hy)
    for (j in range64(n)) {
        sc.hy[j] += hd.time_const.a[j]
    }
    silu(sc.hy, n)
    linear_vec(hd.input_proj, sc.x0, sc.hx)
    for (b in hd.blocks) {
        linear_vec(b.adaln, sc.hy, sc.hmod)
        copy_rows(sc.hn, sc.hx, n)
        layernorm_rows(sc.hn, 1l, n, b.ln, HEAD_LN_EPS)
        modulate(sc.hn, sc.hmod, n)
        linear_vec(b.mlp1, sc.hn, sc.hm)
        silu(sc.hm, n)
        linear_vec(b.mlp2, sc.hm, sc.hn)
        for (j in range64(n)) {
            sc.hx[j] += sc.hmod[2l * n + j] * sc.hn[j]
        }
    }
    linear_vec(hd.final_adaln, sc.hy, sc.hmod)
    copy_rows(sc.hn, sc.hx, n)
    layernorm_rows(sc.hn, 1l, n, hd.final_ln, HEAD_LN_EPS)
    modulate(sc.hn, sc.hmod, n)
    linear_vec(hd.final_linear, sc.hn, sc.x1)
    for (j in range64(long_length(sc.x1))) {
        sc.x1[j] += sc.x0[j]
    }
}

//! The flow head alone, the parity rail's seam: the normalized frame `x1` the head samples from a
//! conditioning row and a noise draw.
def pocket_head(m : PocketModel; cond, x0 : array<float>; var sc : PocketScratch; @scratch @exact_size var x1 : array<float>) {
    copy_rows(sc.cond, cond, m.backbone.d)
    sc.x0 |> reserve_resize(m.latent_dim)
    for (j in range64(m.latent_dim)) {
        sc.x0[j] = x0[j]
    }
    head_step(m.head, sc)
    x1 |> reserve_resize(m.latent_dim)
    for (j in range64(m.latent_dim)) {
        x1[j] = sc.x1[j]
    }
}

// a chunk past the voice's cache: an event of one long unpunctuated input, off the frame loop
[cold_path]
def private grow_cache(var kc : TtsKvCache; cap : int64) {
    kv_cache_grow(kc, cap)
}

// the voice's rows stay; the previous chunk's text and frames are forgotten, and a chunk the
// caches cannot hold - `n_txt` text rows plus every frame the cap allows - grows them first
def private caches_ready(var vs : PocketVoiceState; n_txt : int64) {
    let need = vs.len + n_txt + pocket_max_frames(n_txt) + 1l
    for (kc in vs.caches) {
        kc.n = vs.len
        if (kc.cap < need) {
            grow_cache(kc, need + VOICE_CACHE_SLACK)
        }
    }
}

[cold_path]
def private warn_no_eos(frames, n_txt : int64) {
    to_log(LOG_WARNING, "dasLLAMA pocket: {frames} frames without an end of speech - the cap for {n_txt} tokens\n")
}

//! The frames one chunk needs at most: the reference's speech-rate estimate plus a margin.
def pocket_max_frames(n_tokens : int64) : int64 {
    return int64(ceil((float(n_tokens) / MAX_GEN_TOKENS_PER_SECOND + GEN_SECONDS_PADDING) * FRAME_RATE))
}

//! One prepared chunk in one voice: the text prompt after the voice's rows, frames until EOS plus
//! `frames_after_eos` (noise from `sc.rng` at `temp`, or the `captured` draws; `forced` rows fed
//! back in place of ours = teacher forcing), the latents decoded into `sc.wave`; the frame count.
[hot_path]
def pocket_synthesize(m : PocketModel; ids : array<int64>; var vs : PocketVoiceState; temp : float; frames_after_eos : int;
                      captured, forced : array<float>; var tm : TtsTimings; var sc : PocketScratch) : int64 {
    let d = m.backbone.d
    let ld = m.latent_dim
    let n_txt = long_length(ids)
    let clock = ref_time_ticks()
    caches_ready(vs, n_txt)
    unsafe {
        scratch_resize(sc.rows, n_txt * d)
    }
    for (r in range64(n_txt)) {
        let src = ids[r] * d
        for (j in range64(d)) {
            sc.rows[r * d + j] = m.text_emb.a[src + j]
        }
    }
    transformer_rows(m.backbone, sc.rows, n_txt, vs.len, vs.caches, sc)
    tm.prompt_us = int64(get_time_usec(clock))
    let max_frames = pocket_max_frames(n_txt)
    unsafe {
        scratch_resize(sc.latents, max_frames * ld)
        scratch_resize(sc.conds, max_frames * d)
        scratch_resize(sc.eos_logits, max_frames)
        scratch_resize(sc.x_in, ld)
        scratch_resize(sc.x0, ld)
    }
    for (j in range64(ld)) {
        sc.x_in[j] = m.bos_emb.a[j]
    }
    if (sc.rng.inc == 0ul) {
        rng_seed(sc.rng, 0x5eedul)   // an unseeded generator is a stuck stream, and the normal draw would spin on it
    }
    let std = sqrt(temp)
    var eos_at = -1l
    var frames = 0l
    var backbone_us = 0l
    var head_us = 0l
    while (frames < max_frames) {
        let tb = ref_time_ticks()
        let eos = frame_step(m, vs, vs.len + n_txt + frames, sc)
        backbone_us += int64(get_time_usec(tb))
        for (j in range64(d)) {
            sc.conds[frames * d + j] = sc.cond[j]
        }
        sc.eos_logits[frames] = eos
        if (eos > m.eos_threshold && eos_at < 0l) {
            eos_at = frames
        }
        break if (eos_at >= 0l && frames >= eos_at + int64(frames_after_eos))
        let th = ref_time_ticks()
        for (j in range64(ld)) {
            sc.x0[j] = frames * ld + j < long_length(captured) ? captured[frames * ld + j] : rng_normal(sc.rng) * std
        }
        head_step(m.head, sc)
        let force = (frames + 1l) * ld <= long_length(forced)
        for (j in range64(ld)) {
            sc.latents[frames * ld + j] = sc.x1[j] * m.emb_std.a[j] + m.emb_mean.a[j]
            sc.x_in[j] = force ? forced[frames * ld + j] : sc.x1[j]
        }
        head_us += int64(get_time_usec(th))
        frames++
    }
    if (eos_at < 0l) {
        warn_no_eos(frames, n_txt)
    }
    tm.backbone_us = backbone_us
    tm.head_us = head_us
    let tc = ref_time_ticks()
    pocket_decode_latents(m, sc.latents, frames, sc, sc.wave)
    tm.codec_us = int64(get_time_usec(tc))
    tm.samples = long_length(sc.wave)
    tm.total_us = int64(get_time_usec(clock))
    return frames
}

//! The full chunk path in one call: a roster voice (encoded on first use) speaks one chunk,
//! prepared here as the reference prepares each of its chunks; the facade's entry.
def pocket_speak(var m : PocketModel; chunk : string; voice : string; temp : float; seed : uint64; var tm : TtsTimings; var sc : PocketScratch) : int64 {
    ensure_voice_state(m, voice, sc)
    let p = prepare_pocket_text(m, chunk)
    var inscope ids <- pocket_text_ids(m, p.prepared)
    let fae = m.frames_after_eos >= 0 ? m.frames_after_eos : p.frames_after_eos + 2
    rng_seed(sc.rng, seed)
    var inscope none : array<float>
    var frames = 0l
    m.voice_states |> get(voice) $(var vs) {
        frames = pocket_synthesize(m, ids, vs, temp, fae, none, none, tm, sc)
    }
    return frames
}
