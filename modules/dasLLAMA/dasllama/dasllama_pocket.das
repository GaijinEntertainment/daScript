options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_pocket shared public

require dasllama/dasllama_lint public
require math
require strings
require daslib/fio
require daslib/strings_boost
require daslib/math_bits   // float_bits_to_uint
require dasllama/dasllama_gguf
require dasllama/dasllama_common   // reserve_resize
require dasllama/dasllama_math
require dasllama/dasllama_spm
require dasllama/dasllama_unicode
require dasllama/dasllama_tower   // add_inplace_rows: the residual add the block home shares with the towers
require dasllama/dasllama_gpu_math   // tts_modulate
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
    in_proj : TtsLinear = TtsLinear()    //! [attn_dim + 2 kv_heads dh][d]: q, k, v stacked, each [heads][dh]
    out_proj : TtsLinear = TtsLinear()   //! [d][attn_dim]
    norm2 : TtsNorm = TtsNorm()
    ffn1 : TtsLinear = TtsLinear()
    ffn2 : TtsLinear = TtsLinear()
    scale1 : TtsWeight = TtsWeight()     // per-channel layer scale; empty = none
    scale2 : TtsWeight = TtsWeight()
}

struct PocketTransformer {
    layers : array<PocketLayer>
    d : int64
    heads : int64        // query heads
    kv_heads : int64     //! key-value heads, each shared by heads / kv_heads query heads
    attn_dim : int64     //! the queries' and the attention output's width; d unless the config says otherwise
    dh : int64           //! attn_dim / heads
    ffn : int64
    context : int64      // the key window; 0 = every key
    period : float       // the rotary embedding's max period
    eps : float
}

//! The one-step head's sampling rule: `lsd` takes two timestep embeddings and adds the noise back, `pointwise` adds
//! the noise back with no timestep, `drifting` is the head's output alone.
enum PocketFlow {
    lsd
    drifting
    pointwise
}

struct PocketResBlock {
    ln : TtsNorm = TtsNorm()
    adaln : TtsLinear = TtsLinear()      // silu(y) -> shift, scale, gate
    mlp1 : TtsLinear = TtsLinear()
    mlp2 : TtsLinear = TtsLinear()
}

struct PocketHead {
    input_proj : TtsLinear = TtsLinear()
    cond_embed : TtsLinear = TtsLinear()   //! [dim][cond_dim]
    time_const : TtsWeight = TtsWeight()   //! the two timestep embeddings of the one-step decode, averaged, baked at load; empty but for `lsd`
    blocks : array<PocketResBlock>
    final_ln : TtsNorm = TtsNorm()         // the final layer's norm has no affine: unit weights minted at load
    final_adaln : TtsLinear = TtsLinear()
    final_linear : TtsLinear = TtsLinear()
    dim : int64
    flow : PocketFlow = PocketFlow.lsd
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

struct PocketConvCarry {
    //! a codec conv's carry between two windows of a stream: the last rows it saw, which lead
    //! the next window
    @scratch @exact_size rows : array<float>   // [n][cin]
    n : int64
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
    min_frames_before_eos : int64   //! the chunk's first frames, on which the EOS logit is ignored
    fade_in_samples : int64         //! a chunk's onset ramps in over this many samples
    prompt_end_on_pause : bool      //! a voice clip is trimmed to end on a pause before the encoder hears it
    frames_after_eos : int   // -1: the text-length guess
    pad_short_inputs : bool
    remove_semicolons : bool
    append_terminal_punctuation : bool
    capitalize_first_letter : bool
    replace_chars : table<int; string>   //! per-codepoint rewrites before tokenization ("" deletes)
    bos_before_voice : bool
    default_voice : string
    voice_names : array<string>
    voices : table<string; array<float>>          // a voice as a clip, 24 kHz mono: a cloned one, or a roster of the older file form
    voice_latents : table<string; array<float>>   // a voice as its clip's latent frames [frames][latent_dim]: the roster's stored form
    voice_states : table<string; PocketVoiceState>
    cloning : bool           // the file carries the codec encoder, so a clip can become a voice
    text_emb : TtsWeight = TtsWeight()     //! [n_bins + 1][d]; a low-rank table is multiplied out at load
    bos_emb : TtsWeight = TtsWeight()      // [latent_dim]: the first frame's input
    bos_voice : TtsWeight = TtsWeight()    // [d]: the row before the voice prompt
    input_linear : TtsLinear = TtsLinear()
    backbone : PocketTransformer = PocketTransformer()
    out_norm : TtsNorm = TtsNorm()
    z_proj : TtsLinear = TtsLinear()       //! [cond_dim][d] after out_norm, the head's and the EOS logit's input; nout 0 = none
    cond_dim : int64                       //! the conditioning row's width: z_proj's, else d
    out_eos : TtsLinear = TtsLinear()
    speaker_proj : TtsLinear = TtsLinear()
    emb_std : TtsWeight = TtsWeight()
    emb_mean : TtsWeight = TtsWeight()
    latent_space_id : uint64
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

var private g_pocket_q8_pin = GemmLane.unset

def private pocket_serve_q8_() : bool => lane_serves_q8(g_pocket_q8_pin, true)

//! Pin the GEMM weights' format for subsequent loads: Q8_0 quants or the file's own f32 (a
//! K-quant tensor dequantizes into either); ``reset_pocket_q8`` returns to the policy default,
//! which serves a K-quant tensor as its own planes. The facade: ``set_tts_q8`` / ``reset_tts_q8``.
def set_pocket_q8(on : bool) {
    g_pocket_q8_pin = lane_pin(on)
}

def reset_pocket_q8() {
    g_pocket_q8_pin = GemmLane.unset
}

// ===== the codec's window: the latent frames a stream step covers =====

let POCKET_CODEC_WINDOW_DEFAULT = 16l
var private g_pocket_codec_window = TtsStreamWindow(frames = POCKET_CODEC_WINDOW_DEFAULT)

//! Set how many latent frames the codec's encoder and decoder run per stream step; 0 runs a
//! whole chunk or clip in one step, a negative count is refused, and every set prints one info
//! line. The activations scale with the window, the output does not.
def set_pocket_codec_window(frames : int64) {
    stream_window_set(g_pocket_codec_window, frames, "set_pocket_codec_window")
}

def pocket_codec_window() : int64 => g_pocket_codec_window.frames

//! Would the next load serve its GEMMs as q8 - the pin when set, the policy otherwise.
def pocket_serves_q8() : bool => pocket_serve_q8_()

//! Would the next load serve a K-quant tensor as its own planes - true unless a lane is pinned.
def pocket_serves_native() : bool => g_pocket_q8_pin == GemmLane.unset

// what the readers mint while a load runs: the lane's quant choice and the backend's repack
var private g_stage_q8 = false
var private g_stage_repack = false
var private g_stage_native = false

def finalize(var m : PocketModel) {
    delete m.voice_names
    delete m.voices
    delete m.voice_latents
    delete m.voice_states
    delete m.replace_chars
    delete m.text_emb
    delete m.bos_emb
    delete m.bos_voice
    delete m.input_linear
    delete m.backbone
    delete m.out_norm
    delete m.z_proj
    delete m.out_eos
    delete m.speaker_proj
    delete m.emb_std
    delete m.emb_mean
    delete m.head
    delete m.mimi
    delete m.tok
}

//! The activation carrier one synthesis reuses (`ARCHITECTURE_TTS_MEMORY.md#tts-scratch-carrier`'s discipline): every stage buffer a
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
    @scratch @exact_size zn : array<float>       //! the backbone's last row after out_norm, where z_proj reads it
    @scratch @exact_size cond : array<float>     //! the conditioning row: out_norm's row, or z_proj's
    @scratch @exact_size eos : array<float>
    @scratch @exact_size hy : array<float>       // the head's conditioning
    @scratch @exact_size hx : array<float>       // the head's running state
    @scratch @exact_size hn : array<float>
    @scratch @exact_size hm : array<float>
    @scratch @exact_size hmod : array<float>
    @scratch @exact_size x0 : array<float>
    @scratch @exact_size x1 : array<float>
    @scratch @exact_size latents : array<float>  // [frames][latent_dim], denormalized
    @scratch @exact_size conds : array<float>    //! [frames][cond_dim]: every frame's conditioning row, the parity rail's witness
    @scratch @exact_size eos_logits : array<float>
    @scratch @exact_size ca : array<float>       // the codec chain's ping-pong rows, one window wide
    @scratch @exact_size cb : array<float>
    @scratch @exact_size cc : array<float>
    @scratch @exact_size cpad : array<float>
    @scratch @exact_size cs : array<float>       // a stream conv's input: the carried rows, then the window
    @scratch @exact_size ct : array<float>       // a transposed stream conv's whole output, the window's rows cut from it
    @scratch @exact_size lw : array<float>       // the window's latent frames
    @scratch @exact_size wave : array<float>
    codec_caches : array<TtsKvCache>
    dec_carry : array<PocketConvCarry>           // the decoder's convs, in chain order
    enc_carry : array<PocketConvCarry>
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
    delete sc.zn
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
    delete sc.cs
    delete sc.ct
    delete sc.lw
    delete sc.wave
    delete sc.codec_caches
    delete sc.dec_carry
    delete sc.enc_carry
}

def pocket_release_scratch(var sc : PocketScratch) {
    //! Free every buffer the carrier holds and the block home's scratch globals its kernels grew;
    //! the next synthesis or clone grows them back. For a platform where idle memory matters more
    //! than a few milliseconds of allocation per run.
    finalize(sc)
    tts_blocks_release_scratch()
}

// ===== the GGUF =====

let private LN_EPS = 1e-5
let POCKET_HEAD_LN_EPS = 1e-6   //! the flow head's LayerNorm epsilon
let private TIME_RMS_EPS = 1e-5   // the timestep embedder's norm: its activations' variance is 1e-4-ish, so the epsilon is load-bearing
let private MAX_GEN_TOKENS_PER_SECOND = 3.0   // the reference's estimate of speech rate, for the frame cap
let private GEN_SECONDS_PADDING = 2.0
let private FRAME_RATE = 12.5

def private need_tensor(m : GGUFMeta; name : string) : int => gguf_need_tensor(m, name, "pocket")

def private read_arr(m : GGUFMeta; bytes : array<uint8> | #; name : string) : array<float> =>
    <- gguf_read_f32_arr(m, bytes, name, "pocket")

def private read_weight(m : GGUFMeta; bytes : array<uint8> | #; name : string) : TtsWeight => TtsWeight(a <- read_arr(m, bytes, name))

// a Q8_0 tensor of the published file: its blocks go straight into the int8 plane on the q8 lane
var private g_stage_file_q8 = false   // the file's `pocket.weights` says q8: its Q8_0 tensors are in the kernels' layout

// a Q8_0 tensor is read as the kernels' own blocks only where the converter wrote it that way:
// `pocket.weights = "q8"` names the layout, and the block-home rule (32-wide on both dims) is what
// q8_linear / head_q8_linear wrote a second time; a Q8_0 conv is an older file's and dequantizes; Q4_K's disk form is ggml's own and needs no gate
def private is_q8_tensor(m : GGUFMeta; name : string) : bool {
    if (gguf_tensor_type(m, name) != GGML_TYPE_Q8_0) {
        return false
    }
    if (!g_stage_file_q8) {
        panic("dasLLAMA pocket: '{name}' is Q8_0 in a file whose pocket.weights is not q8 - the reader takes the blocks' layout from the converter's q8 form only")
    }
    return true
}

[arch(at = "../ARCHITECTURE_POCKET.md#pocket-q8-file")]
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
        // the kq lane: the file's own K-quant planes, straight into the layer (the width rule is the layer's own)
        let n = l.nout * l.nin
        var inscope kq, ks : array<uint8>
        kq |> reserve_resize(n / 256l * kq_qsb(KqFmt.k4))
        ks |> reserve_resize(n / 256l * kq_ssb(KqFmt.k4))
        gguf_transcode_kq(m, bytes, wname, KqFmt.k4, kq, ks, 0l, n)
        linear_take_kq(l, 4, kq, ks, true)   // repacked wherever the backend the load selected carries kq kernels
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
    linear_prepare(l, true, g_stage_q8, g_stage_repack, vec_only)   // a vector layer mints nothing and keeps w
    return <- l
}

def private read_norm(m : GGUFMeta; bytes : array<uint8> | #; prefix : string) : TtsNorm {
    var n = TtsNorm()
    n.w <- read_arr(m, bytes, "{prefix}.weight")
    n.b <- read_arr(m, bytes, "{prefix}.bias")
    return <- n
}

// an older file's conv: a Q8_0 slab [cout][k][cin] dequantized and re-laid into PyTorch's [cout][cin][k];
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
    var inscope slab <- read_arr(m, bytes, wname)
    c.w |> reserve_resize(c.cout * kc)
    for (co in range64(c.cout)) {
        for (kk in range64(c.k)) {
            for (ci in range64(c.cin)) {
                c.w[(co * c.cin + ci) * c.k + kk] = slab[co * kc + kk * c.cin + ci]
            }
        }
    }
    conv1d_prepare(c, false, g_stage_repack)
}

// a causal conv's context is its geometry: a forward conv's left zero pad of `k - stride`, or
// the `k - stride` tail of a transposed conv the stream never emits
//! a codec conv carries no pad: its causal context is the stream's carry
[arch(at="../ARCHITECTURE_POCKET.md#pocket-codec-stream")]
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
    c.pad_l = 0l
    c.pad_r = 0l
    if (is_q8_tensor(m, wname)) {
        read_conv_q8(m, bytes, wname, c)
        return <- c
    }
    c.w <- read_arr(m, bytes, wname)
    let ti = need_tensor(m, wname)
    c.k = m.tensors[ti].dims[0]
    let cc = conv1d_dims_of(m.tensors[ti].dims, transposed, groups)
    c.cin = cc.cin
    c.cout = cc.cout
    conv1d_prepare(c, false, g_stage_repack)
    return <- c
}

def private read_res_conv(m : GGUFMeta; bytes : array<uint8> | #; prefix : string) : PocketResConv {
    var b = PocketResConv()
    b.conv1 <- read_conv(m, bytes, "{prefix}.block.1.conv", 1l, 1l, false)
    b.conv2 <- read_conv(m, bytes, "{prefix}.block.3.conv", 1l, 1l, false)
    return <- b
}

//! the shape of one transformer: its width, query heads, key-value heads (the query heads when the config names none),
//! the queries' width (d when none), feed-forward width and key window
struct PocketTfShape {
    layers : int64
    d : int64
    heads : int64
    kv_heads : int64
    attn_dim : int64
    ffn : int64
    context : int64
    period : float
}

def private read_transformer(m : GGUFMeta; bytes : array<uint8> | #; prefix : string; s : PocketTfShape; with_scale : bool) : PocketTransformer {
    var tf = PocketTransformer()
    tf.d = s.d
    tf.heads = s.heads
    tf.kv_heads = s.kv_heads
    tf.attn_dim = s.attn_dim
    tf.ffn = s.ffn
    tf.context = s.context
    tf.period = s.period
    tf.eps = LN_EPS
    if (s.heads < 1l || s.attn_dim % s.heads != 0l || s.kv_heads < 1l || s.heads % s.kv_heads != 0l) {
        panic("dasLLAMA pocket: '{prefix}' has {s.heads} heads over {s.attn_dim} query lanes and {s.kv_heads} key-value heads: the heads must tile the width and the key-value heads divide the heads")
    }
    tf.dh = s.attn_dim / s.heads
    let qkv = s.attn_dim + 2l * s.kv_heads * tf.dh
    tf.layers |> resize(int(s.layers))
    for (i in range64(s.layers)) {
        let p = "{prefix}.{i}"
        tf.layers[i].norm1 <- read_norm(m, bytes, "{p}.norm1")
        tf.layers[i].in_proj <- read_linear(m, bytes, "{p}.self_attn.in_proj")
        tf.layers[i].out_proj <- read_linear(m, bytes, "{p}.self_attn.out_proj")
        if (tf.layers[i].in_proj.nout != qkv || tf.layers[i].in_proj.nin != s.d || tf.layers[i].out_proj.nin != s.attn_dim || tf.layers[i].out_proj.nout != s.d) {
            panic("dasLLAMA pocket: '{p}' projects [{tf.layers[i].in_proj.nout}][{tf.layers[i].in_proj.nin}] in and [{tf.layers[i].out_proj.nout}][{tf.layers[i].out_proj.nin}] out; the config says {qkv} query-key-value lanes over {s.d} and {s.attn_dim} back")
        }
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

def private flow_of(name : string) : PocketFlow {
    return PocketFlow.lsd if (name == "lsd")
    return PocketFlow.drifting if (name == "drifting")
    return PocketFlow.pointwise if (name == "pointwise")
    panic("dasLLAMA pocket: the flow head's type '{name}' is not one the engine samples (lsd, drifting, pointwise)")
    return PocketFlow.lsd
}

[arch(at="../ARCHITECTURE_POCKET.md#pocket-one-step-head")]
def private read_head(m : GGUFMeta; bytes : array<uint8> | #; var hd : PocketHead; depth : int64; flow : PocketFlow) {
    hd.flow = flow
    hd.input_proj <- read_linear(m, bytes, "head.input_proj", true)
    hd.cond_embed <- read_linear(m, bytes, "head.cond_embed", true)
    hd.dim = hd.cond_embed.nout
    if (flow == PocketFlow.lsd) {
        var inscope t0 <- time_embed_at(m, bytes, "head.time_embed.0", 0.0lf)
        var inscope t1 <- time_embed_at(m, bytes, "head.time_embed.1", 1.0lf)
        hd.time_const.a |> reserve_resize(hd.dim)
        for (j in range64(hd.dim)) {
            hd.time_const.a[j] = (t0[j] + t1[j]) * 0.5
        }
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

//! the codec's two transformers, the encoder's and the decoder's; a file whose config names no decoder side carries the encoder's shape under both
def private read_mimi(m : GGUFMeta; bytes : array<uint8> | #; var mm : PocketMimi; enc, dec : PocketTfShape; encoder : bool) {
    mm.ratios <- gguf_int_array(m, bytes, "pocket.mimi.ratios")
    let nr = long_length(mm.ratios)
    mm.hop = 1l
    for (r in mm.ratios) {
        mm.hop *= r
    }
    mm.quant_proj <- read_conv(m, bytes, "mimi.quantizer.output_proj", 1l, 1l, false)
    // both resamplers are kernel 2 x stride, mirroring each other; the upsampler is always in the file, depthwise over the decoder side's width
    let up_dims & = unsafe(m.tensors[need_tensor(m, "mimi.upsample.convtr.convtr.weight")].dims)
    mm.frame_steps = up_dims[0] / 2l
    mm.upsample <- read_conv(m, bytes, "mimi.upsample.convtr.convtr", mm.frame_steps, up_dims[2], true)
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
        mm.enc_tf <- read_transformer(m, bytes, "mimi.enc_tf", enc, true)
        mm.downsample <- read_conv(m, bytes, "mimi.downsample.conv.conv", 0l, 1l, false)
        if (mm.downsample.k / 2l != mm.frame_steps) {
            panic("dasLLAMA pocket: the downsampler's kernel {mm.downsample.k} and the upsampler's {2l * mm.frame_steps} disagree on the frame stride")
        }
        mm.downsample.stride = mm.frame_steps
    }
    mm.dec_tf <- read_transformer(m, bytes, "mimi.dec_tf", dec, true)
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
        let ti = gguf_find_tensor(g, "voice_latents.{v}")
        if (ti >= 0) {
            if (g.tensors[ti].dims[0] != m.latent_dim) {
                panic("dasLLAMA pocket: '{path}' stores voice '{v}' as frames of {g.tensors[ti].dims[0]}, the codec's latent width is {m.latent_dim}")
            }
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
def pocket_has_voice(m : PocketModel; name : string) : bool =>
    key_exists(m.voice_latents, name) || key_exists(m.voices, name)

def private read_text_table(g : GGUFMeta; bytes : array<uint8> | #; var m : PocketModel; d : int64) {
    if (gguf_find_tensor(g, "flow_lm.conditioner.proj.weight") < 0) {
        m.text_emb <- read_weight(g, bytes, "flow_lm.conditioner.embed.weight")
        return
    }
    var inscope low <- read_arr(g, bytes, "flow_lm.conditioner.embed.weight")
    var inscope proj = TtsLinear()
    proj.w <- read_arr(g, bytes, "flow_lm.conditioner.proj.weight")
    let ti = need_tensor(g, "flow_lm.conditioner.proj.weight")
    proj.nin = g.tensors[ti].dims[0]
    proj.nout = g.tensors[ti].dims[1]
    if (proj.nout != d || long_length(low) % proj.nin != 0l) {
        panic("dasLLAMA pocket: the text table's projection is [{proj.nout}][{proj.nin}] over a table of {long_length(low)} values; the backbone is {d} wide")
    }
    linear_prepare(proj, true)
    linear_rows(proj, low, long_length(low) / proj.nin, m.text_emb.a)
}

[arch(at="../ARCHITECTURE_POCKET.md#pocket-one-step-head"), arch(at="../ARCHITECTURE_POCKET.md#pocket-chunker")]
def private read_driver_rules(g : GGUFMeta; bytes : array<uint8> | #; var m : PocketModel; path : string) {
    m.eos_threshold = gguf_f32_or(g, bytes, "pocket.eos_threshold", -4.0)
    m.min_frames_before_eos = gguf_int_or(g, bytes, "pocket.min_frames_before_eos", 0l)
    m.fade_in_samples = gguf_int_or(g, bytes, "pocket.fade_in_samples", 0l)
    m.prompt_end_on_pause = gguf_bool_or(g, bytes, "pocket.prompt_end_on_pause", false)
    m.frames_after_eos = int(gguf_int(g, bytes, "pocket.frames_after_eos"))
    m.pad_short_inputs = gguf_int(g, bytes, "pocket.pad_short_inputs") != 0l
    m.remove_semicolons = gguf_int(g, bytes, "pocket.remove_semicolons") != 0l
    m.append_terminal_punctuation = gguf_int(g, bytes, "pocket.append_terminal_punctuation") != 0l
    m.capitalize_first_letter = gguf_bool_or(g, bytes, "pocket.capitalize_first_letter", true)
    return if (!gguf_has(g, "pocket.replace_from"))
    var inscope from <- gguf_str_array(g, bytes, "pocket.replace_from")
    var inscope to <- gguf_str_array(g, bytes, "pocket.replace_to")
    if (length(from) != length(to)) {
        panic("dasLLAMA pocket: '{path}' names {length(from)} characters to replace and {length(to)} replacements")
    }
    for (src, dst in from, to) {
        var inscope cps <- utf8_to_cpts(src)
        if (length(cps) != 1) {
            panic("dasLLAMA pocket: '{path}' replaces '{src}', which is not one character")
        }
        m.replace_chars[cps[0]] = clone_string(dst)
    }
}

def private read_flow_lm(g : GGUFMeta; bytes : array<uint8> | #; var m : PocketModel; path : string) {
    let d = gguf_int(g, bytes, "pocket.backbone.d_model")
    let heads = gguf_int(g, bytes, "pocket.backbone.heads")
    let bb = PocketTfShape(layers = gguf_int(g, bytes, "pocket.backbone.layers"), d = d, heads = heads,
        kv_heads = gguf_int_or(g, bytes, "pocket.backbone.kv_heads", heads), attn_dim = gguf_int_or(g, bytes, "pocket.backbone.attn_dim", d),
        ffn = gguf_int_or(g, bytes, "pocket.backbone.ffn", d * gguf_int(g, bytes, "pocket.backbone.hidden_scale")), context = 0l,
        period = gguf_f32(g, bytes, "pocket.backbone.rope_max_period"))
    read_text_table(g, bytes, m, d)
    m.bos_emb <- read_weight(g, bytes, "flow_lm.bos_emb")
    m.bos_voice <- read_weight(g, bytes, "flow_lm.bos_before_voice")
    m.input_linear <- read_linear(g, bytes, "flow_lm.input_linear")
    m.backbone <- read_transformer(g, bytes, "backbone", bb, false)
    m.out_norm <- read_norm(g, bytes, "flow_lm.out_norm")
    m.cond_dim = d
    if (gguf_find_tensor(g, "flow_lm.z_proj.weight") >= 0) {
        m.z_proj <- read_linear(g, bytes, "flow_lm.z_proj", true)
        m.cond_dim = m.z_proj.nout
    }
    m.out_eos <- read_linear(g, bytes, "flow_lm.out_eos", true)
    if (m.out_eos.nin != m.cond_dim) {
        panic("dasLLAMA pocket: '{path}' reads its EOS logit over {m.out_eos.nin} lanes, the conditioning row is {m.cond_dim} wide")
    }
    m.speaker_proj.w <- read_arr(g, bytes, "flow_lm.speaker_proj_weight")
    m.speaker_proj.nin = m.latent_dim
    m.speaker_proj.nout = d
    linear_prepare(m.speaker_proj, true)
    m.emb_std <- read_weight(g, bytes, "flow_lm.emb_std")
    m.emb_mean <- read_weight(g, bytes, "flow_lm.emb_mean")
    read_head(g, bytes, m.head, gguf_int(g, bytes, "pocket.head.depth"), flow_of(gguf_str_or(g, bytes, "pocket.head.flow_type", "lsd")))
    if (m.head.cond_embed.nin != m.cond_dim) {
        panic("dasLLAMA pocket: '{path}' conditions its head on {m.head.cond_embed.nin} lanes, the conditioning row is {m.cond_dim} wide")
    }
}

//! a backbone with the `z_proj` bottleneck: its conditioning row is `cond_dim` wide, else the residual's `d`
def pocket_has_z(m : PocketModel) : bool => m.z_proj.nout > 0l

//! the key-value rows' width: the key-value heads side by side
def tts_pk_kv_width(tf : PocketTransformer) : int64 => tf.kv_heads * tf.dh

//! the fused projection's width: the q span (`attn_dim`) and the k and v spans at the key-value width
def tts_pk_qkv_width(tf : PocketTransformer) : int64 => tf.attn_dim + 2l * tts_pk_kv_width(tf)

//! The latent space a voice prompt is read in: the codec geometry and the backbone's latent
//! statistics, which every file carries whether or not it ships the encoder. Two files agree
//! here exactly when a prompt prepared on one conditions the other.
def pocket_latent_space_id(m : PocketModel) : uint64 {
    var h = hash_combine64(hash_combine64(uint64(m.sample_rate), uint64(m.latent_dim)), uint64(m.frame_samples))
    for (v in m.emb_mean.a) {
        h = hash_combine64(h, uint64(float_bits_to_uint(v)))
    }
    for (v in m.emb_std.a) {
        h = hash_combine64(h, uint64(float_bits_to_uint(v)))
    }
    return h
}

//! Load a converted Pocket TTS GGUF: the scalars from its `pocket.*` metadata, the weights
//! prepared for the rows kernels, the tokenizer, and the roster (its states built on first use).
def load_pocket(path : string) : PocketModel {
    bump_weights_epoch()   //! the previous model's GPU slabs drop before this one's weights can land at their addresses
    var inscope m = PocketModel()
    m.q8 = pocket_serve_q8_()
    g_stage_q8 = m.q8
    g_stage_native = pocket_serves_native()
    // the backend is selected for any quantized lane; its q8 layout may want a repack (g_stage_repack), and the
    // K-quant planes repack wherever it carries kq kernels - linear_take_kq asks that itself
    let selected = (m.q8 || g_stage_native) ? select_matmul_backend_for_load_() : false
    g_stage_repack = m.q8 && selected
    let kq_arm = !g_stage_native ? "" : (kernel_backend_has_kq() ? ", its K-quant planes repacked for {active_kernel_backend()}" : ", its K-quant planes in disk order (the portable GEMV)")
    to_log(LOG_INFO, "dasLLAMA pocket: GEMM lane {m.q8 ? "q8" : "f32"}{g_stage_native ? ", a K-quant tensor as its own planes" : ""}{kq_arm} - {g_pocket_q8_pin != GemmLane.unset ? "pinned via set_tts_q8" : "the policy default"}\n")
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
        read_driver_rules(g, bytes, m, path)
        m.bos_before_voice = gguf_int(g, bytes, "pocket.bos_before_voice") != 0l
        m.default_voice = gguf_str(g, bytes, "pocket.default_voice")
        g_stage_file_q8 = gguf_has(g, "pocket.weights") && gguf_str(g, bytes, "pocket.weights") == "q8"
        if (g_stage_file_q8 && !m.q8) {
            to_log(LOG_INFO, "dasLLAMA pocket: '{path}' carries Q8_0 GEMM weights; the f32 lane serves them dequantized\n")
        }
        m.cloning = !gguf_has(g, "pocket.cloning") || gguf_int(g, bytes, "pocket.cloning") != 0l
        read_roster(g, bytes, m, path)
        read_flow_lm(g, bytes, m, path)
        let enc = PocketTfShape(layers = gguf_int(g, bytes, "pocket.mimi.layers"), d = gguf_int(g, bytes, "pocket.mimi.d_model"),
            heads = gguf_int(g, bytes, "pocket.mimi.heads"), kv_heads = gguf_int(g, bytes, "pocket.mimi.heads"), attn_dim = gguf_int(g, bytes, "pocket.mimi.d_model"),
            ffn = gguf_int(g, bytes, "pocket.mimi.dim_feedforward"), context = gguf_int(g, bytes, "pocket.mimi.context"),
            period = gguf_f32(g, bytes, "pocket.mimi.rope_max_period"))
        let dec = PocketTfShape(layers = gguf_int_or(g, bytes, "pocket.mimi.dec.layers", enc.layers), d = gguf_int_or(g, bytes, "pocket.mimi.dec.d_model", enc.d),
            heads = gguf_int_or(g, bytes, "pocket.mimi.dec.heads", enc.heads), kv_heads = gguf_int_or(g, bytes, "pocket.mimi.dec.heads", enc.heads),
            attn_dim = gguf_int_or(g, bytes, "pocket.mimi.dec.d_model", enc.d), ffn = gguf_int_or(g, bytes, "pocket.mimi.dec.dim_feedforward", enc.ffn),
            context = gguf_int_or(g, bytes, "pocket.mimi.dec.context", enc.context), period = gguf_f32_or(g, bytes, "pocket.mimi.dec.rope_max_period", enc.period))
        read_mimi(g, bytes, m.mimi, enc, dec, m.cloning)
        m.frame_samples = m.mimi.hop * m.mimi.frame_steps
        m.latent_space_id = pocket_latent_space_id(m)   //! after the codec geometry it hashes is read
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

//! q [t][qd], k and v [t][kvd] out of the stacked projection rows [t][qd + 2 kvd]
def private split_qkv(qkv : array<float>; t, qd, kvd : int64; @scratch @exact_size var q : array<float>;
                      @scratch @exact_size var k : array<float>; @scratch @exact_size var v : array<float>) {
    q |> reserve_resize(t * qd)
    k |> reserve_resize(t * kvd)
    v |> reserve_resize(t * kvd)
    let w = qd + 2l * kvd
    for (r in range64(t)) {
        let row = r * w
        for (j in range64(qd)) {
            q[r * qd + j] = qkv[row + j]
        }
        for (j in range64(kvd)) {
            k[r * kvd + j] = qkv[row + qd + j]
            v[r * kvd + j] = qkv[row + qd + kvd + j]
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

//! `kv_only` ends the layer at its cache append - the last layer of a prompt, whose residual nothing reads
def private layer_attention(l : PocketLayer; tf : PocketTransformer; var x : array<float>; t, pos0 : int64;
                            var kc : TtsKvCache; var sc : PocketScratch; kv_only : bool = false) {
    let d = tf.d
    let dh = tf.dh
    let qd = tf.attn_dim
    let kvd = tts_pk_kv_width(tf)
    layernorm_rows_into(x, sc.xb, t, d, l.norm1, tf.eps)
    proj(l.in_proj, sc.xb, t, sc.qkv)
    split_qkv(sc.qkv, t, qd, kvd, sc.q, sc.k, sc.v)
    if (!kv_only) {
        rope_rows(sc.q, t, qd, dh, pos0, tf.period)
    }
    rope_rows(sc.k, t, kvd, dh, pos0, tf.period)
    kv_cache_append(kc, sc.k, sc.v, t, kvd)
    return if (kv_only)
    attention_causal_rows(sc.q, t, qd, tf.context, kc, sc.ctx)
    proj(l.out_proj, sc.ctx, t, sc.xb)
    if (!empty(l.scale1.a)) {
        layer_scale_rows(sc.xb, t, d, l.scale1.a)
    }
    add_inplace_rows(x, sc.xb, t * d)
}

def private layer_ffn(l : PocketLayer; tf : PocketTransformer; var x : array<float>; t : int64; var sc : PocketScratch) {
    let d = tf.d
    layernorm_rows_into(x, sc.xb, t, d, l.norm2, tf.eps)
    proj(l.ffn1, sc.xb, t, sc.h)
    gelu(sc.h, t * tf.ffn)
    proj(l.ffn2, sc.h, t, sc.xb)
    if (!empty(l.scale2.a)) {
        layer_scale_rows(sc.xb, t, d, l.scale2.a)
    }
    add_inplace_rows(x, sc.xb, t * d)
}

//! `t` rows of x [t][d] through the transformer at positions pos0.., every layer's keys and
//! values appended to its cache; x carries the residual out - unless `kv_only_last`, which ends
//! the last layer at its cache append (a prompt, whose residual nothing reads).
def transformer_rows(tf : PocketTransformer; var x : array<float>; t, pos0 : int64; var caches : array<TtsKvCache>; var sc : PocketScratch;
                     kv_only_last : bool = false) {
    let last = length(tf.layers) - 1
    for (i, l in iter_range(tf.layers), tf.layers) {
        let kv_only = kv_only_last && i == last
        layer_attention(l, tf, x, t, pos0, caches[i], sc, kv_only)
        continue if (kv_only)
        layer_ffn(l, tf, x, t, sc)
    }
}

def private caches_init(var caches : array<TtsKvCache>; tf : PocketTransformer; cap : int64) {
    caches |> resize(length(tf.layers))
    for (kc in caches) {
        kv_cache_init(kc, tf.kv_heads, tf.dh, cap)
    }
}

// ===== the codec =====

// ===== the codec stream: a chunk or a clip in windows, each conv carrying its last rows =====

//! the rows a conv sees again ahead of a window: a forward conv its last `k - stride` inputs
//! (the causal pad's width, zeros before the first window), a transposed one the inputs whose
//! taps reach past the rows it emitted
def private carry_rows(c : TtsConv1d) : int64 =>
    c.transposed ? (c.k + c.stride - 1l) / c.stride - 1l : c.k - c.stride

def private carry_init(var cr : PocketConvCarry; c : TtsConv1d) {
    cr.n = c.transposed ? 0l : carry_rows(c)
    cr.rows |> reserve_resize(cr.n * c.cin)
    for (i in range64(cr.n * c.cin)) {
        cr.rows[i] = 0.0
    }
}

//! the first window's carry for a conv whose pad replicates the edge: `n` copies of the window's first row
def private carry_replicate(var cr : PocketConvCarry; x : array<float>; n, w : int64) {
    cr.n = n
    pad_edge_left_rows(x, 0l, w, n, cr.rows)
}

//! x [n][cin], one window of a stream, through a codec conv: the carried rows lead the window,
//! the conv runs over both unpadded, y takes the window's own output rows and the carry the
//! window's tail. Returns the rows of y.
[arch(at="../ARCHITECTURE_POCKET.md#pocket-codec-stream")]
def private stream_conv(c : TtsConv1d; var cr : PocketConvCarry; x : array<float>; n : int64; var sc : PocketScratch; @scratch @exact_size var y : array<float>) : int64 {
    let w = c.cin
    let need = carry_rows(c)
    let tin = cr.n + n
    if (cr.n > 0l) {
        sc.cs |> reserve_resize(tin * w)
        copy_floats(cr.rows, 0l, sc.cs, 0l, cr.n * w)
        copy_floats(x, 0l, sc.cs, cr.n * w, n * w)
    }
    var t_out = 0l
    if (c.transposed) {
        if (c.groups != 1l) {
            conv1d_rows_transposed_depthwise(c, cr.n > 0l ? sc.cs : x, tin, sc.ct)
        } else {
            conv1d_rows(c, cr.n > 0l ? sc.cs : x, tin, sc.ct)
        }
        t_out = n * c.stride
        y |> reserve_resize(t_out * c.cout_s)
        copy_floats(sc.ct, cr.n * c.stride * c.cout_s, y, 0l, t_out * c.cout_s)
    } else {
        conv1d_rows(c, cr.n > 0l ? sc.cs : x, tin, y)
        t_out = conv1d_out_len(c, tin)
        compact_rows(y, t_out, c.cout_s, c.cout)   // a stage off the 16 lattice (the 2m decoder's 24- and 12-channel ones) reads at its channel count
    }
    if (need > 0l) {
        if (cr.n > 0l) {
            copy_floats(sc.cs, (tin - need) * w, cr.rows, 0l, need * w)
        } else {
            cr.rows |> reserve_resize(need * w)
            copy_floats(x, (n - need) * w, cr.rows, 0l, need * w)
        }
        cr.n = need
    }
    return t_out
}

def private res_conv_stream(b : PocketResConv; var cr : PocketConvCarry; x : array<float>; t : int64; var sc : PocketScratch; @scratch @exact_size var y : array<float>) {
    let w = b.conv1.cin
    copy_rows(sc.cc, x, t * w)
    elu_rows(sc.cc, t * w)
    stream_conv(b.conv1, cr, sc.cc, t, sc, sc.cpad)
    elu_rows(sc.cpad, t * b.conv1.cout)
    conv1d_rows(b.conv2, sc.cpad, t, y)
    compact_rows(y, t, b.conv2.cout_s, b.conv2.cout)
    add_inplace_rows(y, x, t * w)
}

//! the window in frames a stream runs: the knob, else the whole run
def private codec_window(frames : int64) : int64 => stream_window_for(g_pocket_codec_window, frames)

//! The codec decoder as a whole: latents [frames][latent_dim] (denormalized) to `frames * frame_samples` samples in `wave`.
typedef PocketCodecGpuFn = function<(m : PocketModel; latents : array<float>; frames : int64; var sc : PocketScratch; @scratch @exact_size var wave : array<float>) : bool>

//! The frame loop as a whole once the prompt's rows sit in the voice's caches: `pocket_synthesize`'s
//! loop contract (noise from `sc.rng` at `std` or `captured`, `forced` fed back, at most `max_frames`),
//! the latents, conditioning rows and EOS logits into the scratch; frames below zero = declined, the caches untouched.
typedef PocketFramesGpuFn = function<(m : PocketModel; var vs : PocketVoiceState; n_txt : int64; std : float; frames_after_eos : int;
                                      captured, forced : array<float>; max_frames : int64; var sc : PocketScratch) : tuple<frames : int64; eos_at : int64>>

//! The text prompt's rows through the backbone after the voice's: `prompt_rows`' contract - the
//! embedding rows of `sc.rows` (`pocket_prompt_embed`) at positions vs.len.., every layer's cache
//! appended on the host; false = declined, the caches untouched.
typedef PocketPromptGpuFn = function<(m : PocketModel; var vs : PocketVoiceState; n_txt : int64; var sc : PocketScratch) : bool>

//! A driver's seats, one per stage; a seat left empty keeps the CPU form for that stage.
struct PocketGpuDriver {
    codec : PocketCodecGpuFn
    frames : PocketFramesGpuFn
    prompt : PocketPromptGpuFn
}

var private g_pk_drv : PocketGpuDriver
var private g_pk_drv_prev : PocketGpuDriver   //! the registration a caller's seat displaced

let private PK_SEAT_CODEC = 0
let private PK_SEAT_FRAMES = 1
let private PK_SEAT_PROMPT = 2
var private g_pk_seats <- tts_seats(["codec", "frames", "prompt"])

[arch(at = "../ARCHITECTURE_MEDIA.md#tower-gpu-hook")]
def register_pocket_gpu(drv : PocketGpuDriver) {
    g_pk_drv_prev = g_pk_drv
    g_pk_drv = drv
}

//! The restore half of the seats a caller takes: the displaced registration serves again.
[cold_path]
def unregister_pocket_gpu {
    g_pk_drv = g_pk_drv_prev
    g_pk_drv_prev = PocketGpuDriver()
}

//! Hook invocations and the ones that served, since the process started, per stage (a seat name of the record).
[cold_path]
def pocket_gpu_stats(stage : string) : tuple<calls : int; served : int> => tts_seat_stats(g_pk_seats, stage, "pocket")

//! The seat names of the hook record, in dispatch order.
[cold_path]
def pocket_gpu_seats() : array<string> => tts_seat_names(g_pk_seats)

//! The clip's latent frames [frames][latent_dim] through the codec encoder - the voice prompt's
//! source. `pcm` is 24 kHz mono; its tail pads to a whole frame with zeros as the reference does.
def pocket_encode_latents(m : PocketModel; pcm : array<float>; var sc : PocketScratch; @scratch @exact_size var latents : array<float>) : int64 {
    if (!m.cloning) {
        panic("dasLLAMA pocket: this file carries no codec encoder, so it cannot encode a clip")
    }
    let mm & = unsafe(m.mimi)
    let n = long_length(pcm)
    let frames = (n + m.frame_samples - 1l) / m.frame_samples
    let padded = frames * m.frame_samples
    let nr = long_length(mm.ratios)
    let fs = mm.frame_steps
    let ld = mm.downsample.cout_s
    var ec & = unsafe(sc.enc_carry)
    ec |> resize(3l + 2l * nr)
    carry_init(ec[0], mm.enc_in)
    for (i in range64(nr)) {
        carry_init(ec[1l + 2l * i], mm.enc_res[i].conv1)
        carry_init(ec[2l + 2l * i], mm.enc_down[i])
    }
    carry_init(ec[1l + 2l * nr], mm.enc_out)
    ec[2l + 2l * nr].n = 0l   // the downsampler's first carry replicates the edge, set below
    caches_init(sc.codec_caches, mm.enc_tf, frames * fs)
    latents |> reserve_resize(frames * ld)
    let window = codec_window(frames) * m.frame_samples
    var s0 = 0l
    var pos = 0l
    var fout = 0l
    while (s0 < padded) {
        let ns = min(window, padded - s0)
        sc.ca |> reserve_resize(ns)
        for (i in range64(ns)) {
            let s = s0 + i
            sc.ca[i] = s < n ? pcm[s] : 0.0
        }
        var t = ns
        stream_conv(mm.enc_in, ec[0], sc.ca, t, sc, sc.cb)
        for (i in range64(nr)) {
            res_conv_stream(mm.enc_res[i], ec[1l + 2l * i], sc.cb, t, sc, sc.ca)
            elu_rows(sc.ca, t * mm.enc_down[i].cin)
            t = stream_conv(mm.enc_down[i], ec[2l + 2l * i], sc.ca, t, sc, sc.cb)
        }
        elu_rows(sc.cb, t * mm.enc_out.cin)
        stream_conv(mm.enc_out, ec[1l + 2l * nr], sc.cb, t, sc, sc.ca)
        transformer_rows(mm.enc_tf, sc.ca, t, pos, sc.codec_caches, sc)
        pos += t
        if (s0 == 0l) {
            carry_replicate(ec[2l + 2l * nr], sc.ca, fs, mm.enc_tf.d)
        }
        let fw = stream_conv(mm.downsample, ec[2l + 2l * nr], sc.ca, t, sc, sc.cb)
        copy_floats(sc.cb, 0l, latents, fout * ld, fw * ld)
        fout += fw
        s0 += ns
    }
    return fout
}

//! Latent frames [frames][latent_dim] (denormalized) through the codec decoder into
//! `frames * frame_samples` PCM samples, a window of frames per stream step, every conv
//! carrying its rows between steps - the output the whole chunk in one step gives.
[arch(at="../ARCHITECTURE_POCKET.md#pocket-codec-stream")]
def pocket_decode_latents(m : PocketModel; latents : array<float>; frames : int64; var sc : PocketScratch; @scratch @exact_size var wave : array<float>) {
    if (g_pk_drv.codec != null) {
        tts_seat_call(g_pk_seats, PK_SEAT_CODEC)
        if (invoke(g_pk_drv.codec, m, latents, frames, sc, wave)) {
            tts_seat_served(g_pk_seats, PK_SEAT_CODEC)
            return
        }
    }
    let mm & = unsafe(m.mimi)
    let nr = long_length(mm.ratios)
    let ld = mm.quant_proj.cin
    var dc & = unsafe(sc.dec_carry)
    dc |> resize(3l + 2l * nr)
    carry_init(dc[0], mm.upsample)
    carry_init(dc[1], mm.dec_in)
    for (i in range64(nr)) {
        carry_init(dc[2l + 2l * i], mm.dec_up[i])
        carry_init(dc[3l + 2l * i], mm.dec_res[i].conv1)
    }
    carry_init(dc[2l + 2l * nr], mm.dec_out)
    caches_init(sc.codec_caches, mm.dec_tf, frames * mm.frame_steps)
    wave |> reserve_resize(frames * m.frame_samples)
    let window = codec_window(frames)
    let stride = mm.dec_out.cout
    var f0 = 0l
    var pos = 0l
    var out = 0l
    while (f0 < frames) {
        let n = min(window, frames - f0)
        sc.lw |> reserve_resize(n * ld)
        copy_floats(latents, f0 * ld, sc.lw, 0l, n * ld)
        conv1d_rows(mm.quant_proj, sc.lw, n, sc.ca)
        compact_rows(sc.ca, n, mm.quant_proj.cout_s, mm.quant_proj.cout)
        var t = stream_conv(mm.upsample, dc[0], sc.ca, n, sc, sc.cb)
        transformer_rows(mm.dec_tf, sc.cb, t, pos, sc.codec_caches, sc)
        pos += t
        stream_conv(mm.dec_in, dc[1], sc.cb, t, sc, sc.ca)
        for (i in range64(nr)) {
            elu_rows(sc.ca, t * mm.dec_up[i].cin)
            t = stream_conv(mm.dec_up[i], dc[2l + 2l * i], sc.ca, t, sc, sc.cb)
            res_conv_stream(mm.dec_res[i], dc[3l + 2l * i], sc.cb, t, sc, sc.ca)
        }
        elu_rows(sc.ca, t * mm.dec_out.cin)
        stream_conv(mm.dec_out, dc[2l + 2l * nr], sc.ca, t, sc, sc.cb)
        for (i in range64(t)) {
            wave[out + i] = sc.cb[i * stride]
        }
        out += t
        f0 += n
    }
}

// ===== the voice prompt =====

let private VOICE_CACHE_GROW = 64l   // rows past a chunk's need the caches grow by, so a run of chunks of one length grows once
let POCKET_MAX_VOICE_SECONDS = 60l   // the longest clip a voice is cloned from: the state is the clip's frames per layer

//! The clip as the encoder hears it: trimmed to end on a pause where the file's driver did
//! (`prompt_end_on_pause`), else as it came - `pocket_voice_state`'s and the prepared prompt's first step.
def pocket_prompt_pcm(m : PocketModel; pcm : array<float>; @scratch @exact_size var out : array<float>) {
    if (m.prompt_end_on_pause) {
        end_on_pause(pcm, m.sample_rate, out)
        return
    }
    out |> reserve_resize(long_length(pcm))
    copy_floats(pcm, 0l, out, 0l, long_length(pcm))
}

//! The backbone's memory of a clip: its latents projected into the model's width, the BOS row in
//! front, run through the backbone at positions 0..; the state a synthesis continues from.
[arch(at="../ARCHITECTURE_POCKET.md#pocket-voice-state")]
def pocket_voice_state(m : PocketModel; pcm : array<float>; var sc : PocketScratch) : PocketVoiceState {
    var inscope clip : array<float>
    pocket_prompt_pcm(m, pcm, clip)
    var inscope lat : array<float>
    let frames = pocket_encode_latents(m, clip, sc, lat)
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
    caches_init(vs.caches, m.backbone, vs.len + chunk_rows(int64(POCKET_MAX_TOKENS)))
    transformer_rows(m.backbone, rows, vs.len, 0l, vs.caches, sc, kv_only_last = true)
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
    pocket_voice_clear(m, name)
    m.voices[name] := pcm
    m.voice_states[name] <- vs
}

//! A voice's slots freed and erased from every table, its name kept on the roster (joined when new) - the
//! replace half of an add-or-replace; erase frees a slot, never the frames, so each is deleted first.
def pocket_voice_clear(var m : PocketModel; name : string) {
    if (!pocket_has_voice(m, name)) {
        m.voice_names |> push(clone_string(name))
    }
    if (key_exists(m.voice_states, name)) {
        m.voice_states |> get(name) $(var old) {
            delete old
        }
        m.voice_states |> erase(name)
    }
    if (key_exists(m.voices, name)) {
        m.voices |> get(name) $(var old) {
            delete old
        }
        m.voices |> erase(name)
    }
    if (key_exists(m.voice_latents, name)) {
        m.voice_latents |> get(name) $(var old) {
            delete old
        }
        m.voice_latents |> erase(name)
    }
}

// a roster voice's state, built on first use: from its stored latent frames, or from its clip
def private ensure_voice_state(var m : PocketModel; name : string; var sc : PocketScratch) {
    return if (key_exists(m.voice_states, name))
    if (key_exists(m.voice_latents, name)) {
        var inscope lat : array<float>
        m.voice_latents |> get(name) $(stored) {
            lat := stored
        }
        if (long_length(lat) % m.latent_dim != 0l) {
            panic("dasLLAMA pocket: voice '{name}' stores {long_length(lat)} latent values, not whole frames of {m.latent_dim}")
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

let private TERMINAL_MARKS <- ['.', '!', '?', 0x2026]       //! . ! ? and the ellipsis
let private WEAK_MARKS <- [',', ';', ':', '-', 0x2013, 0x2014]   //! , ; : - and the two dashes
let private CLOSERS <- ['"', '\'', 0x201d, 0x2019, ')', ']', 0xbb]   //! the quotes and brackets a sentence may end inside

def private is_end_mark(c : int) : bool => find_index(TERMINAL_MARKS, c) >= 0

def private is_weak(c : int) : bool => find_index(WEAK_MARKS, c) >= 0

def private is_closer(c : int) : bool => find_index(CLOSERS, c) >= 0

//! the length of cps[0..n) once the trailing codepoints `drop` names are removed
def private rstrip_cps(cps : array<int>; n0 : int; drop : block<(c : int) : bool>) : int {
    var n = n0
    while (n > 0 && invoke(drop, cps[n - 1])) {
        n--
    }
    return n
}

//! one codepoint onto the folded text: whitespace becomes one pending space, written before the next mark
def private push_folded(var out : array<int>; var pending_space : bool&; c : int) {
    if (cpt_is_whitespace(c)) {
        pending_space = !empty(out)
        return
    }
    if (pending_space) {
        out |> push(' ')
        pending_space = false
    }
    out |> push(c)
}

//! the reference's per-character rewrites, then its whitespace fold (runs of any whitespace to one space, the ends
//! stripped), then a sentence mark followed by a comma, semicolon or colon loses that mark - what a deleted quote leaves behind
def private replace_and_fold(m : PocketModel; text : string) : string {
    var inscope cps <- utf8_to_cpts(text)
    var inscope out : array<int>
    out |> reserve(length(cps))
    var pending_space = false
    for (c in cps) {
        if (!key_exists(m.replace_chars, c)) {
            push_folded(out, pending_space, c)
            continue
        }
        var rep = ""
        m.replace_chars |> get(c) $(v) {
            rep = v
        }
        var inscope rc <- utf8_to_cpts(rep)
        for (c2 in rc) {
            push_folded(out, pending_space, c2)
        }
    }
    var inscope kept : array<int>
    kept |> reserve(length(out))
    var i = 0
    while (i < length(out)) {
        kept |> push(out[i])
        if (is_end_mark(out[i])) {
            var j = i + 1
            while (j < length(out) && cpt_is_whitespace(out[j])) {
                j++
            }
            if (j < length(out) && (out[j] == ',' || out[j] == ';' || out[j] == ':')) {
                i = j + 1
                continue
            }
        }
        i++
    }
    return cpts_to_utf8(kept)
}

//! the reference's terminal-punctuation rule: a prompt already ending in a sentence mark - closing quotes or brackets
//! after it allowed - is left alone, one ending in a weak mark has that mark (and the spaces about it) replaced by a
//! period before the closers, and anything else takes a period after its closers
def private ensure_terminal_punctuation(cps : array<int>) : string {
    let core_n = rstrip_cps(cps, length(cps)) $(c) {
        return is_closer(c) || c == ' '
    }
    return cpts_to_utf8(cps) if (core_n == 0 || is_end_mark(cps[core_n - 1]))
    if (is_weak(cps[core_n - 1])) {
        let kept = rstrip_cps(cps, core_n) $(c) {
            return is_weak(c) || c == ' '
        }
        var closers_lo = core_n
        while (closers_lo < length(cps) && cpt_is_whitespace(cps[closers_lo])) {
            closers_lo++
        }
        var closers_hi = length(cps)
        while (closers_hi > closers_lo && cpt_is_whitespace(cps[closers_hi - 1])) {
            closers_hi--
        }
        return "{cpts_to_utf8(cps, 0, kept)}.{cpts_to_utf8(cps, closers_lo, closers_hi)}"
    }
    return "{cpts_to_utf8(cps)}."
}

//! The reference driver's preparation of one chunk: the per-character rewrites and the whitespace fold where the file
//! names rewrites, line breaks to spaces and ONE pass of two spaces to one, an initial capital, a terminal mark, the
//! short-input padding; the frames it keeps generating after EOS. An empty text is refused, as there.
def prepare_pocket_text(m : PocketModel; text : string) : tuple<prepared : string; frames_after_eos : int> {
    var s = strip(text)
    if (!empty(m.replace_chars)) {
        s = replace_and_fold(m, s)
    }
    if (empty(s)) {
        panic("dasLLAMA pocket: the text is empty - nothing to say")
    }
    s = s |> replace("\n", " ") |> replace("\r", " ") |> replace("  ", " ")
    if (m.remove_semicolons) {
        s = replace(s, ";", ",")
    }
    var inscope cps <- utf8_to_cpts(s)
    let words = word_count(cps)
    let guess = words <= 4 ? 3 : 1
    // the reference upper-cases the first character; an ASCII one turns here, an accented one
    // stays as it is (no case table for it in the engine)
    if (m.capitalize_first_letter) {
        let first = cpts_to_utf8(cps, 0, 1)
        let up = to_upper(first)
        if (up != first) {
            s = "{up}{cpts_to_utf8(cps, 1, length(cps))}"
            cps[0] = utf8_to_cpts(up)[0]
        }
    }
    if (m.append_terminal_punctuation) {
        s = ensure_terminal_punctuation(cps)
    }
    if (m.pad_short_inputs && words < 5) {
        s = "        {s}"
    }
    return (prepared = s, frames_after_eos = guess)
}

//! Token ids of a prepared chunk.
def pocket_text_ids(m : PocketModel; prepared : string) : array<int64> =>
    <- unigram_encode(m.tok, prepared, m.unk_id, m.byte_fallback, m.reserved_ids)

let POCKET_MAX_TOKENS = 50   // the reference driver's chunk budget; past it the model skips words

// the ids of a marker string less its leading dummy-space piece
def private marker_ids(m : PocketModel; s : string) : array<int64> {
    var inscope ids <- pocket_text_ids(m, s)
    ids |> erase(0)
    return <- ids
}

[arch(at="../ARCHITECTURE_POCKET.md#pocket-chunker")]
def private decimal_boundary(m : PocketModel; ids : array<int64>; at : int) : bool {
    var inscope head, tail : array<int64>
    head |> reserve(2)
    tail |> reserve(2)
    for (i in range(max(0, at - 2), at)) {
        head |> push(ids[i])
    }
    for (i in range(at - 1, min(at + 1, length(ids)))) {
        tail |> push(ids[i])
    }
    var inscope pre <- utf8_to_cpts(spm_decode_ids(m.tok, head))
    var inscope suf <- utf8_to_cpts(spm_decode_ids(m.tok, tail))
    let n = length(pre)
    return n >= 2 && pre[n - 1] == '.' && cpt_is_number(pre[n - 2]) && length(suf) >= 2 && cpt_is_number(suf[1])
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
    if (pocket_has_z(m)) {
        layernorm_rows_into(sc.rows, sc.zn, 1l, d, m.out_norm, m.backbone.eps)
        linear_vec(m.z_proj, sc.zn, sc.cond)
    } else {
        layernorm_rows_into(sc.rows, sc.cond, 1l, d, m.out_norm, m.backbone.eps)
    }
    linear_vec(m.out_eos, sc.cond, sc.eos)
    return sc.eos[0]
}

//! The flow head's adaLN modulation of one row in place: x * (1 + scale) + shift, `mod` holding the shift half then the scale half.
def modulate(var x : array<float>; mod : array<float>; n : int64) {
    for (j in range64(n)) {
        x[j] = tts_modulate(x[j], mod[j], mod[n + j])
    }
}

//! The flow head at one step: x1 = x0 + v(0, 1, x0 | cond) for `lsd`, x0 + v(x0 | cond) for `pointwise`, v(x0 | cond) for `drifting`.
def private head_step(hd : PocketHead; var sc : PocketScratch) {
    let n = hd.dim
    linear_vec(hd.cond_embed, sc.cond, sc.hy)
    if (hd.flow == PocketFlow.lsd) {
        for (j in range64(n)) {
            sc.hy[j] += hd.time_const.a[j]
        }
    }
    silu(sc.hy, n)
    linear_vec(hd.input_proj, sc.x0, sc.hx)
    for (b in hd.blocks) {
        linear_vec(b.adaln, sc.hy, sc.hmod)
        layernorm_rows_into(sc.hx, sc.hn, 1l, n, b.ln, POCKET_HEAD_LN_EPS)
        modulate(sc.hn, sc.hmod, n)
        linear_vec(b.mlp1, sc.hn, sc.hm)
        silu(sc.hm, n)
        linear_vec(b.mlp2, sc.hm, sc.hn)
        for (j in range64(n)) {
            sc.hx[j] += sc.hmod[2l * n + j] * sc.hn[j]
        }
    }
    linear_vec(hd.final_adaln, sc.hy, sc.hmod)
    layernorm_rows_into(sc.hx, sc.hn, 1l, n, hd.final_ln, POCKET_HEAD_LN_EPS)
    modulate(sc.hn, sc.hmod, n)
    linear_vec(hd.final_linear, sc.hn, sc.x1)
    return if (hd.flow == PocketFlow.drifting)
    for (j in range64(long_length(sc.x1))) {
        sc.x1[j] += sc.x0[j]
    }
}

//! The flow head alone, the parity rail's seam: the normalized frame `x1` the head samples from a
//! conditioning row (`cond_dim` wide) and a noise draw.
def pocket_head(m : PocketModel; cond, x0 : array<float>; var sc : PocketScratch; @scratch @exact_size var x1 : array<float>) {
    sc.cond |> reserve_resize(m.cond_dim)
    copy_floats(cond, 0l, sc.cond, 0l, m.cond_dim)
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

//! the rows a chunk of `n_txt` tokens can take after the voice: its text and every frame the cap allows
def private chunk_rows(n_txt : int64) : int64 => n_txt + pocket_max_frames(n_txt) + 1l

// the voice's rows stay; the previous chunk's text and frames are forgotten, and a chunk the
// caches cannot hold - `n_txt` text rows plus every frame the cap allows - grows them first
def private caches_ready(var vs : PocketVoiceState; n_txt : int64) {
    let need = vs.len + chunk_rows(n_txt)
    for (kc in vs.caches) {
        kc.n = vs.len
        if (kc.cap < need) {
            grow_cache(kc, need + VOICE_CACHE_GROW)
        }
    }
}

[cold_path]
//! The loop's noise for latent element `at` of the chunk: the captured stream's value where it
//! reaches, the generator's normal at `std` past it - both loops draw in this order.
def pocket_noise_at(var sc : PocketScratch; captured : array<float>; at : int64; std : float) : float =>
    at < long_length(captured) ? captured[at] : rng_normal(sc.rng) * std

//! The EOS rule on frame `frame`'s logit: the first crossing of the threshold from frame `min_frames_before_eos` on
//! pins `eos_at`, and the loop stops `frames_after_eos` frames past it - both loops stop by this one rule.
def pocket_eos_stops(m : PocketModel; eos : float; frame : int64; frames_after_eos : int; var eos_at : int64&) : bool {
    if (eos > m.eos_threshold && eos_at < 0l && frame >= m.min_frames_before_eos) {
        eos_at = frame
    }
    return eos_at >= 0l && frame >= eos_at + int64(frames_after_eos)
}

var private g_pocket_frame_batch = 8l   //! the frames a GPU seat's submit carries: the host walks the EOS rule between submits

//! the frames one GPU submit of the frame loop carries before the host reads their EOS logits, clamped to one or more
[cold_path]
def set_pocket_frame_batch(n : int64) {
    g_pocket_frame_batch = max(n, 1l)
}

def pocket_frame_batch() : int64 => g_pocket_frame_batch

//! a GPU seat's frame loop: `noise(k)` takes a batch's draws, `run(first, k)` submits the batch at its first frame (false fails: frames -1), `eos(first, i)` reads a logit
def pocket_frames_batched(m : PocketModel; batch, max_frames : int64; std : float; frames_after_eos : int; captured : array<float>;
                          var sc : PocketScratch; var f0 : int64&; noise : block<(k : int64) : float?>; run : block<(first, k : int64) : bool>;
                          eos : block<(first, i : int64) : float>) : tuple<frames : int64; eos_at : int64> {
    let ld = m.latent_dim
    var frames = 0l
    var eos_at = -1l
    var done = false
    while (!done && f0 < max_frames) {
        let k = min(batch, max_frames - f0)
        let rb = sc.rng
        let frames_before = frames
        var np = invoke(noise, k)
        for (i in range64(k * ld)) {
            unsafe(np[i]) = pocket_noise_at(sc, captured, f0 * ld + i, std)
        }
        return (frames = -1l, eos_at = -1l) if (!invoke(run, f0, k))
        for (i in range64(k)) {
            let f = f0 + i
            let e = invoke(eos, f0, i)
            sc.eos_logits[f] = e
            if (pocket_eos_stops(m, e, f, frames_after_eos, eos_at)) {
                done = true
                break
            }
            frames++
        }
        if (frames - frames_before < k) {
            sc.rng = rb
            for (i in range64((frames - frames_before) * ld)) {
                if (f0 * ld + i >= long_length(captured)) {
                    rng_normal(sc.rng)
                }
            }
        }
        f0 += k
    }
    return (frames = frames, eos_at = eos_at)
}

[cold_path]
def private warn_no_eos(frames, n_txt : int64) {
    to_log(LOG_WARNING, "dasLLAMA pocket: {frames} frames without an end of speech - the cap for {n_txt} tokens\n")
}

//! The frames one chunk needs at most: the reference's speech-rate estimate plus a margin.
def pocket_max_frames(n_tokens : int64) : int64 =>
    int64(ceil((float(n_tokens) / MAX_GEN_TOKENS_PER_SECOND + GEN_SECONDS_PADDING) * FRAME_RATE))

//! The text prompt's embedding rows [n_txt][d] into `sc.rows` - the backbone's input on either rail.
def pocket_prompt_embed(m : PocketModel; ids : array<int64>; var sc : PocketScratch) {
    let d = m.backbone.d
    unsafe(scratch_resize(sc.rows, long_length(ids) * d))
    gather_rows(sc.rows, m.text_emb.a, ids, d)
}

//! The text prompt's rows through the backbone after the voice's, every layer's cache filled.
def private prompt_rows(m : PocketModel; ids : array<int64>; var vs : PocketVoiceState; var sc : PocketScratch) {
    let n_txt = long_length(ids)
    pocket_prompt_embed(m, ids, sc)
    if (g_pk_drv.prompt != null) {
        tts_seat_call(g_pk_seats, PK_SEAT_PROMPT)
        if (invoke(g_pk_drv.prompt, m, vs, n_txt, sc)) {
            tts_seat_served(g_pk_seats, PK_SEAT_PROMPT)
            return
        }
    }
    transformer_rows(m.backbone, sc.rows, n_txt, vs.len, vs.caches, sc, kv_only_last = true)
}

//! The prompt's rows into the voice's caches alone, on whichever rail serves it - `pocket_synthesize`'s opening, the
//! caches reset to the voice's rows first.
[cold_path]
def pocket_prompt_rows(m : PocketModel; ids : array<int64>; var vs : PocketVoiceState; var sc : PocketScratch) {
    caches_ready(vs, long_length(ids))
    prompt_rows(m, ids, vs, sc)
}

//! One prepared chunk in one voice: the text prompt after the voice's rows, frames until EOS plus
//! `frames_after_eos` (noise from `sc.rng` at `temp`, or the `captured` draws; `forced` rows fed
//! back in place of ours = teacher forcing), the latents decoded into `sc.wave`; the frame count.
[hot_path]
def pocket_synthesize(m : PocketModel; ids : array<int64>; var vs : PocketVoiceState; temp : float; frames_after_eos : int;
                      captured, forced : array<float>; var tm : TtsTimings; var sc : PocketScratch) : int64 {
    let cd = m.cond_dim
    let ld = m.latent_dim
    let n_txt = long_length(ids)
    let clock = ref_time_ticks()
    caches_ready(vs, n_txt)
    prompt_rows(m, ids, vs, sc)
    tm.prompt_us = int64(get_time_usec(clock))
    let max_frames = pocket_max_frames(n_txt)
    unsafe {
        scratch_resize(sc.latents, max_frames * ld)
        scratch_resize(sc.conds, max_frames * cd)
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
    var served = false
    if (g_pk_drv.frames != null) {
        tts_seat_call(g_pk_seats, PK_SEAT_FRAMES)
        let tb = ref_time_ticks()
        let r = invoke(g_pk_drv.frames, m, vs, n_txt, std, frames_after_eos, captured, forced, max_frames, sc)
        if (r.frames >= 0l) {
            served = true
            frames = r.frames
            eos_at = r.eos_at
            backbone_us = int64(get_time_usec(tb))
            tts_seat_served(g_pk_seats, PK_SEAT_FRAMES)
        }
    }
    while (!served && frames < max_frames) {
        let tb = ref_time_ticks()
        let eos = frame_step(m, vs, vs.len + n_txt + frames, sc)
        backbone_us += int64(get_time_usec(tb))
        for (j in range64(cd)) {
            sc.conds[frames * cd + j] = sc.cond[j]
        }
        sc.eos_logits[frames] = eos
        break if (pocket_eos_stops(m, eos, frames, frames_after_eos, eos_at))
        let th = ref_time_ticks()
        for (j in range64(ld)) {
            sc.x0[j] = pocket_noise_at(sc, captured, frames * ld + j, std)
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
    fade_in(sc.wave, m.fade_in_samples)
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
