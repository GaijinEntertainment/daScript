options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_styletts2 shared public

require dasllama/dasllama_lint public
require math
require strings
require daslib/fio
require daslib/archive
require dasllama/dasllama_env
require dasllama/dasllama_gguf
require dasllama/dasllama_common
require dasllama/dasllama_math
require dasllama/dasllama_par
require dasllama/dasllama_plane
require dasllama/dasllama_image
require dasllama/dasllama_tower   // asr_prof_add: the encode-stage profiling rail
require dasllama/dasllama_tts_types
require dasllama/dasllama_tts_blocks

//! The StyleTTS2-lineage model both TTS families share (KittenTTS nano and mini, Kokoro):
//! the weight map of the converted GGUF (conv geometry rides as `styletts2.conv.<weight>`
//! metadata, so the assembly hardcodes the wiring and reads the shapes), the voices, the STFT
//! convention the checkpoint was trained with, and the assembly: PL-BERT -> text encoder ->
//! duration encoder -> durations -> alignment -> prosody -> decoder -> iSTFTNet generator.
//! Symbol tables, style-row rules and driver conventions are the family files'.

// ===== weights =====

struct St2Albert {
    word_emb : TtsWeight = TtsWeight()
    pos_emb : TtsWeight = TtsWeight()
    type_emb : TtsWeight = TtsWeight()
    emb_ln : TtsNorm = TtsNorm()
    map_in : TtsLinear = TtsLinear()
    q : TtsLinear = TtsLinear()
    k : TtsLinear = TtsLinear()
    v : TtsLinear = TtsLinear()
    dense : TtsLinear = TtsLinear()
    attn_ln : TtsNorm = TtsNorm()
    ffn : TtsLinear = TtsLinear()
    ffn_out : TtsLinear = TtsLinear()
    full_ln : TtsNorm = TtsNorm()
    emb_dim : int64
    hidden : int64
    heads : int64
    n_layers : int64
    max_pos : int64
}

def weights_walk(var io : TtsBlobIo; var a : St2Albert) {
    weights_walk(io, a.word_emb)
    weights_walk(io, a.pos_emb)
    weights_walk(io, a.type_emb)
    weights_walk(io, a.emb_ln)
    weights_walk(io, a.map_in)
    weights_walk(io, a.q)
    weights_walk(io, a.k)
    weights_walk(io, a.v)
    weights_walk(io, a.dense)
    weights_walk(io, a.attn_ln)
    weights_walk(io, a.ffn)
    weights_walk(io, a.ffn_out)
    weights_walk(io, a.full_ln)
}

struct St2TextEncoder {
    emb : TtsWeight = TtsWeight()
    cnn : array<TtsConv1d>
    cnn_ln : array<TtsNorm>
    lstm : TtsBiLstm = TtsBiLstm()
    c : int64
}

def weights_walk(var io : TtsBlobIo; var te : St2TextEncoder) {
    weights_walk(io, te.emb)
    for (c in te.cnn) {
        weights_walk(io, c)
    }
    for (n in te.cnn_ln) {
        weights_walk(io, n)
    }
    weights_walk(io, te.lstm)
}

//! StyleTTS2's AdainResBlk1d: two AdaIN + LeakyReLU + conv passes, an optional 2x upsample
//! (depthwise transposed conv on the residual, nearest on the shortcut), a learned 1x1
//! shortcut when the width changes, the sum scaled by 1/sqrt(2).
struct St2AdainResBlk {
    norm1 : TtsAdaIn = TtsAdaIn()
    norm2 : TtsAdaIn = TtsAdaIn()
    conv1 : TtsConv1d = TtsConv1d()
    conv2 : TtsConv1d = TtsConv1d()
    conv1x1 : TtsConv1d = TtsConv1d()
    pool : TtsConv1d = TtsConv1d()
    upsample : bool
    learned_sc : bool
    dim_in : int64
    dim_out : int64
}

def weights_walk(var io : TtsBlobIo; var b : St2AdainResBlk) {
    weights_walk(io, b.norm1)
    weights_walk(io, b.norm2)
    weights_walk(io, b.conv1)
    weights_walk(io, b.conv2)
    weights_walk(io, b.conv1x1)
    weights_walk(io, b.pool)
}

struct St2Predictor {
    bert_encoder : TtsLinear = TtsLinear()
    lstms : array<TtsBiLstm>
    lns : array<TtsAdaLayerNorm>
    lstm : TtsBiLstm = TtsBiLstm()
    duration_proj : TtsLinear = TtsLinear()
    shared_lstm : TtsBiLstm = TtsBiLstm()
    f0 : array<St2AdainResBlk>
    n : array<St2AdainResBlk>
    f0_proj : TtsConv1d = TtsConv1d()
    n_proj : TtsConv1d = TtsConv1d()
}

def weights_walk(var io : TtsBlobIo; var p : St2Predictor) {
    weights_walk(io, p.bert_encoder)
    for (l in p.lstms) {
        weights_walk(io, l)
    }
    for (l in p.lns) {
        weights_walk(io, l)
    }
    weights_walk(io, p.lstm)
    weights_walk(io, p.duration_proj)
    weights_walk(io, p.shared_lstm)
    for (b in p.f0) {
        weights_walk(io, b)
    }
    for (b in p.n) {
        weights_walk(io, b)
    }
    weights_walk(io, p.f0_proj)
    weights_walk(io, p.n_proj)
}

//! The generator's AdaINResBlock1: three dilated conv pairs with AdaIN and Snake before each.
struct St2GenResBlock {
    convs1 : array<TtsConv1d>
    convs2 : array<TtsConv1d>
    adain1 : array<TtsAdaIn>
    adain2 : array<TtsAdaIn>
    alpha1 : array<TtsWeight>
    alpha2 : array<TtsWeight>
    c : int64
}

def weights_walk(var io : TtsBlobIo; var b : St2GenResBlock) {
    for (c in b.convs1) {
        weights_walk(io, c)
    }
    for (c in b.convs2) {
        weights_walk(io, c)
    }
    for (a in b.adain1) {
        weights_walk(io, a)
    }
    for (a in b.adain2) {
        weights_walk(io, a)
    }
    for (w in b.alpha1) {
        weights_walk(io, w)
    }
    for (w in b.alpha2) {
        weights_walk(io, w)
    }
}

struct St2Decoder {
    encode : St2AdainResBlk = St2AdainResBlk()
    decode : array<St2AdainResBlk>
    f0_conv : TtsConv1d = TtsConv1d()
    n_conv : TtsConv1d = TtsConv1d()
    asr_res : TtsConv1d = TtsConv1d()
    ups : array<TtsConv1d>
    noise_convs : array<TtsConv1d>
    noise_res : array<St2GenResBlock>
    resblocks : array<St2GenResBlock>
    conv_post : TtsConv1d = TtsConv1d()
    stft_fwd_re : TtsConv1d = TtsConv1d()
    stft_fwd_im : TtsConv1d = TtsConv1d()
    stft_bwd_re : TtsConv1d = TtsConv1d()
    stft_bwd_im : TtsConv1d = TtsConv1d()
    source_linear : TtsLinear = TtsLinear()
    n_kernels : int64
    bins : int64
    stft_pad_reflect : bool
    istft_envelope : bool
    stft_eps : float
    window : TtsWeight = TtsWeight()
}

def weights_walk(var io : TtsBlobIo; var d : St2Decoder) {
    weights_walk(io, d.encode)
    for (b in d.decode) {
        weights_walk(io, b)
    }
    weights_walk(io, d.f0_conv)
    weights_walk(io, d.n_conv)
    weights_walk(io, d.asr_res)
    for (c in d.ups) {
        weights_walk(io, c)
    }
    for (c in d.noise_convs) {
        weights_walk(io, c)
    }
    for (b in d.noise_res) {
        weights_walk(io, b)
    }
    for (b in d.resblocks) {
        weights_walk(io, b)
    }
    weights_walk(io, d.conv_post)
    weights_walk(io, d.stft_fwd_re)
    weights_walk(io, d.stft_fwd_im)
    weights_walk(io, d.stft_bwd_re)
    weights_walk(io, d.stft_bwd_im)
    weights_walk(io, d.source_linear)
    weights_walk(io, d.window)
}

//! The served StyleTTS2 carrier: every weight a borrowed view over `blob`, the `.dlim` plane
//! the served layouts were minted into; scalars, spans and the voice roster ride the meta.
struct StyleTts2Model {
    family : string
    name : string
    sample_rate : int
    n_symbols : int64
    style_dim : int64
    style_rows : int
    voice_names : array<string>
    voices : table<string; array<float>>
    voice_spans : table<string; TtsSpan>
    albert : St2Albert = St2Albert()
    text_encoder : St2TextEncoder = St2TextEncoder()
    predictor : St2Predictor = St2Predictor()
    decoder : St2Decoder = St2Decoder()
    source : SineSourceCfg = SineSourceCfg()
    q8 : bool               // the lane this image was minted on: the rows GEMMs serve Q8_0 quants
    blob : PlaneF = PlaneF()
    qblob : PlaneI8 = PlaneI8()
    image_map : void?
    image_bytes : uint64
}

def weights_walk(var io : TtsBlobIo; var m : StyleTts2Model) {
    weights_walk(io, m.albert)
    weights_walk(io, m.text_encoder)
    weights_walk(io, m.predictor)
    weights_walk(io, m.decoder)
    for (name in m.voice_names) {
        var span = m.voice_spans?[name] ?? TtsSpan()
        weight_slot(io, m.voices[name], span)
        m.voice_spans[name] = span
    }
}

// serialize_image_meta covers 14 fields; voices (views bound from voice_spans), image_map and
// image_bytes are the deliberate skips
let private ST2_META_FIELDS = 14 + 3
let ST2_IMAGE_TAG = "tts-f32"
let ST2_IMAGE_TAG_Q8 = "tts-q8"

def private serialize_image_meta(var arch : Archive; var m : StyleTts2Model) {
    verify(count_meta_fields(m) == ST2_META_FIELDS)   // grew StyleTts2Model? extend this list
    arch |> serialize(m.family)
    arch |> serialize(m.name)
    arch |> serialize_raw(m.sample_rate)
    arch |> serialize_raw(m.n_symbols)
    arch |> serialize_raw(m.style_dim)
    arch |> serialize_raw(m.style_rows)
    serialize_strings(arch, m.voice_names)
    arch |> serialize(m.voice_spans)
    arch |> serialize(m.albert)
    arch |> serialize(m.text_encoder)
    arch |> serialize(m.predictor)
    arch |> serialize(m.decoder)
    arch |> serialize(m.source)
    arch |> serialize_raw(m.q8)
}

[unused_argument(m)]
def private image_post_load(var m : StyleTts2Model) {}

//! Point every weight view into the mapped blobs - after the parse bound the planes.
def private st2_bind(var m : StyleTts2Model) {
    var io = TtsBlobIo(staging = false, plane = m.blob, qplane = m.qblob)
    weights_walk(io, m)
}

// ===== the serving lane: f32 planes (the parity rail's form) or Q8_0 quants on the rows GEMMs.
// Each lane is its own prepared image; the q8 image's bytes follow the backend's repack, so its
// tag is config-bound while the f32 tag is config-free =====

enum private St2Lane {
    unset    // the policy decides
    exact    // f32 planes
    q8
}
var private g_st2_q8_pin = St2Lane.unset

def private st2_serve_q8() : bool {
    if (g_st2_q8_pin != St2Lane.unset) {
        return g_st2_q8_pin == St2Lane.q8
    }
    return false   // the policy default stays f32 until the rig rules on the q8 lane
}

//! Pin the rows GEMMs' format for subsequent loads: q8 quants vs the file's own f32 planes.
//! Each lane is its own prepared image. ``reset_styletts2_q8`` returns to the policy default.
//! The facade spells these ``set_tts_q8`` / ``reset_tts_q8`` / ``tts_serves_q8``.
def set_styletts2_q8(on : bool) {
    g_st2_q8_pin = on ? St2Lane.q8 : St2Lane.exact
}

def reset_styletts2_q8() {
    g_st2_q8_pin = St2Lane.unset
}

//! Would the next load serve the rows GEMMs as q8 - the pin when set, the policy otherwise.
def styletts2_serves_q8() : bool => st2_serve_q8()

def private st2_tag() : string => st2_serve_q8() ? ST2_IMAGE_TAG_Q8 : ST2_IMAGE_TAG

// the lane changes what a load mints and serves, so the load says which and why
def private st2_announce_lane() {
    let why = g_st2_q8_pin != St2Lane.unset ? "pinned via set_tts_q8" : "the policy default"
    to_log(LOG_INFO, "dasLLAMA styletts2: rows GEMM lane {st2_serve_q8() ? "q8" : "f32 planes"} - {why}\n")
}

// what the readers mint while a stage runs: the lane's quant choice and whether the box's
// backend wants the quants repacked
var private g_stage_q8 = false
var private g_stage_repack = false

def finalize(var m : StyleTts2Model) {
    for (name in m.voice_names) {
        if (key_exists(m.voices, name)) {
            release_weight(m.voices[name])
        }
    }
    delete m.voices
    delete m.voice_spans
    delete m.voice_names
    delete m.albert
    delete m.text_encoder
    delete m.predictor
    delete m.decoder
    delete m.blob
    delete m.qblob
    if (m.image_map != null) {
        image_backing_release(m.image_map, m.image_bytes)
        m.image_map = null
        m.image_bytes = 0ul
    }
}

//! The mint-side carrier: `e` holds the scalars and spans, `blob` the plane bytes until the
//! writer has them. Public because the off-rail suites compare a staged read to the served map.
struct St2Staging {
    e : StyleTts2Model = StyleTts2Model()
    blob : array<float>
    qblob : array<int8>
}

def private serialize_image_meta(var arch : Archive; var st : St2Staging) {
    serialize_image_meta(arch, st.e)
}

[unused_argument(st)]
def private image_post_load(var st : St2Staging) {}

def finalize(var st : St2Staging) {
    delete st.blob
    delete st.qblob
    delete st.e
}

[init]
def private register_styletts2_image_tags {
    register_image_family_tag(ST2_IMAGE_TAG, true)   // no CPU knob shapes the f32 blob: one image per box
    register_image_family_tag(ST2_IMAGE_TAG_Q8)      // the q8 quants follow the backend's repack
}

let private LN_EPS = 1e-5
let private ALBERT_EPS = 1e-12
let private STFT_PAD = 10l

// ===== loading =====

def private need_tensor(m : GGUFMeta; name : string) : int {
    let ti = gguf_find_tensor(m, name)
    if (ti < 0) {
        panic("dasLLAMA styletts2: tensor '{name}' missing from the model")
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

def private read_conv(m : GGUFMeta; bytes : array<uint8> | #; wname, bname : string; rows : bool = false) : TtsConv1d {
    var c = TtsConv1d()
    c.w <- read_arr(m, bytes, wname)
    if (!empty(bname) && gguf_find_tensor(m, bname) >= 0) {
        c.b <- read_arr(m, bytes, bname)
    }
    var inscope geo <- gguf_int_array(m, bytes, "styletts2.conv.{wname}")
    c.k = geo[0]
    c.stride = geo[1]
    c.pad_l = geo[2]
    c.pad_r = geo[3]
    c.dilation = geo[4]
    c.groups = geo[5]
    c.transposed = geo[6] != 0l
    c.output_padding = geo[7]
    let ti = need_tensor(m, wname)
    let d1 = m.tensors[ti].dims[1]
    let d2 = m.tensors[ti].dims[2]
    if (c.transposed) {
        c.cin = d2
        c.cout = d1 * c.groups
    } else {
        c.cout = d2
        c.cin = d1 * c.groups
    }
    c.served_rows = rows
    conv1d_prepare(c, g_stage_q8, g_stage_repack)
    return <- c
}

def private read_conv_p(m : GGUFMeta; bytes : array<uint8> | #; prefix : string; rows : bool = false) : TtsConv1d {
    return <- read_conv(m, bytes, "{prefix}.weight", "{prefix}.bias", rows)
}

def private read_linear(m : GGUFMeta; bytes : array<uint8> | #; prefix : string; rows_only : bool = false) : TtsLinear {
    var l = TtsLinear()
    l.w <- read_arr(m, bytes, "{prefix}.weight")
    let ti = need_tensor(m, "{prefix}.weight")
    l.nin = m.tensors[ti].dims[0]
    l.nout = m.tensors[ti].dims[1]
    if (gguf_find_tensor(m, "{prefix}.bias") >= 0) {
        l.b <- read_arr(m, bytes, "{prefix}.bias")
    }
    linear_prepare(l, rows_only, g_stage_q8, g_stage_repack)
    return <- l
}

def private read_norm(m : GGUFMeta; bytes : array<uint8> | #; wname, bname : string) : TtsNorm {
    var n = TtsNorm()
    if (gguf_find_tensor(m, wname) >= 0) {
        n.w <- read_arr(m, bytes, wname)
        n.b <- read_arr(m, bytes, bname)
    }
    return <- n
}

def private read_lstm_dir(m : GGUFMeta; bytes : array<uint8> | #; prefix : string; dir : int64) : TtsLstm {
    var l = TtsLstm()
    let ti = need_tensor(m, "{prefix}.weight_ih")
    l.nin = m.tensors[ti].dims[0]
    l.hidden = m.tensors[ti].dims[1] / 4l
    let g4 = 4l * l.hidden
    l.w_ih |> reserve_resize(g4 * l.nin)
    l.w_hh |> reserve_resize(g4 * l.hidden)
    l.b |> reserve_resize(g4)
    gguf_read_tensor_f32(m, bytes, "{prefix}.weight_ih", l.w_ih, 0l, g4 * l.nin, dir * g4 * l.nin)
    gguf_read_tensor_f32(m, bytes, "{prefix}.weight_hh", l.w_hh, 0l, g4 * l.hidden, dir * g4 * l.hidden)
    gguf_read_tensor_f32(m, bytes, "{prefix}.bias", l.b, 0l, g4, dir * g4)
    return <- l
}

def private read_bilstm(m : GGUFMeta; bytes : array<uint8> | #; prefix : string) : TtsBiLstm {
    var l = TtsBiLstm()
    l.fwd <- read_lstm_dir(m, bytes, prefix, 0l)
    l.bwd <- read_lstm_dir(m, bytes, prefix, 1l)
    return <- l
}

def private read_adain(m : GGUFMeta; bytes : array<uint8> | #; prefix : string) : TtsAdaIn {
    var a = TtsAdaIn()
    a.fc <- read_linear(m, bytes, "{prefix}.fc")
    a.norm <- read_norm(m, bytes, "{prefix}.norm.weight", "{prefix}.norm.bias")
    return <- a
}

def private read_adaln(m : GGUFMeta; bytes : array<uint8> | #; prefix : string) : TtsAdaLayerNorm {
    var a = TtsAdaLayerNorm()
    a.fc <- read_linear(m, bytes, "{prefix}.fc")
    return <- a
}

def private read_adain_res_blk(m : GGUFMeta; bytes : array<uint8> | #; prefix : string) : St2AdainResBlk {
    var b = St2AdainResBlk()
    b.norm1 <- read_adain(m, bytes, "{prefix}.norm1")
    b.norm2 <- read_adain(m, bytes, "{prefix}.norm2")
    b.conv1 <- read_conv_p(m, bytes, "{prefix}.conv1")
    b.conv2 <- read_conv_p(m, bytes, "{prefix}.conv2")
    b.dim_in = b.conv1.cin
    b.dim_out = b.conv1.cout
    b.learned_sc = gguf_find_tensor(m, "{prefix}.conv1x1.weight") >= 0
    if (b.learned_sc) {
        b.conv1x1 <- read_conv_p(m, bytes, "{prefix}.conv1x1")
    }
    b.upsample = gguf_find_tensor(m, "{prefix}.pool.weight") >= 0
    if (b.upsample) {
        b.pool <- read_conv_p(m, bytes, "{prefix}.pool")
    }
    return <- b
}

def private read_gen_res_block(m : GGUFMeta; bytes : array<uint8> | #; prefix : string) : St2GenResBlock {
    var b = St2GenResBlock()
    b.convs1 |> reserve(3)
    b.convs2 |> reserve(3)
    b.adain1 |> reserve(3)
    b.adain2 |> reserve(3)
    b.alpha1 |> reserve(3)
    b.alpha2 |> reserve(3)
    for (j in range(3)) {
        b.convs1 |> emplace(read_conv_p(m, bytes, "{prefix}.convs1.{j}", true))
        b.convs2 |> emplace(read_conv_p(m, bytes, "{prefix}.convs2.{j}", true))
        b.adain1 |> emplace(read_adain(m, bytes, "{prefix}.adain1.{j}"))
        b.adain2 |> emplace(read_adain(m, bytes, "{prefix}.adain2.{j}"))
        b.alpha1 |> emplace(read_weight(m, bytes, "{prefix}.alpha1.{j}"))
        b.alpha2 |> emplace(read_weight(m, bytes, "{prefix}.alpha2.{j}"))
    }
    b.c = b.convs1[0].cout
    return <- b
}

def private read_albert(m : GGUFMeta; bytes : array<uint8> | #; var a : St2Albert) {
    a.word_emb <- read_weight(m, bytes, "bert.embeddings.word_embeddings.weight")
    a.pos_emb <- read_weight(m, bytes, "bert.embeddings.position_embeddings.weight")
    a.type_emb <- read_weight(m, bytes, "bert.embeddings.token_type_embeddings.weight")
    a.emb_ln <- read_norm(m, bytes, "bert.embeddings.LayerNorm.weight", "bert.embeddings.LayerNorm.bias")
    a.map_in <- read_linear(m, bytes, "bert.encoder.embedding_hidden_mapping_in", true)
    a.q <- read_linear(m, bytes, "bert.layer.attention.query", true)
    a.k <- read_linear(m, bytes, "bert.layer.attention.key", true)
    a.v <- read_linear(m, bytes, "bert.layer.attention.value", true)
    a.dense <- read_linear(m, bytes, "bert.layer.attention.dense", true)
    a.attn_ln <- read_norm(m, bytes, "bert.layer.attention.LayerNorm.weight", "bert.layer.attention.LayerNorm.bias")
    a.ffn <- read_linear(m, bytes, "bert.layer.ffn", true)
    a.ffn_out <- read_linear(m, bytes, "bert.layer.ffn_output", true)
    a.full_ln <- read_norm(m, bytes, "bert.layer.full_layer_layer_norm.weight", "bert.layer.full_layer_layer_norm.bias")
    a.emb_dim = a.map_in.nin
    a.hidden = a.map_in.nout
    a.heads = 12l
    a.n_layers = 12l
    a.max_pos = m.tensors[need_tensor(m, "bert.embeddings.position_embeddings.weight")].dims[1]
}

def private read_text_encoder(m : GGUFMeta; bytes : array<uint8> | #; var te : St2TextEncoder) {
    te.emb <- read_weight(m, bytes, "text_encoder.embedding.weight")
    te.c = m.tensors[need_tensor(m, "text_encoder.embedding.weight")].dims[0]
    var i = 0
    while (gguf_find_tensor(m, "text_encoder.cnn.{i}.0.weight") >= 0) {
        te.cnn |> emplace(read_conv_p(m, bytes, "text_encoder.cnn.{i}.0"))
        te.cnn_ln |> emplace(read_norm(m, bytes, "text_encoder.cnn.{i}.1.gamma", "text_encoder.cnn.{i}.1.beta"))
        i++
    }
    te.lstm <- read_bilstm(m, bytes, "text_encoder.lstm")
}

def private read_predictor(m : GGUFMeta; bytes : array<uint8> | #; var p : St2Predictor) {
    p.bert_encoder <- read_linear(m, bytes, "bert_encoder", true)
    var pairs = 0
    while (gguf_find_tensor(m, "predictor.text_encoder.lstms.{2 * pairs}.weight_ih") >= 0) {
        pairs++
    }
    p.lstms |> reserve(pairs)
    p.lns |> reserve(pairs)
    for (i in range(pairs)) {
        p.lstms |> emplace(read_bilstm(m, bytes, "predictor.text_encoder.lstms.{2 * i}"))
        p.lns |> emplace(read_adaln(m, bytes, "predictor.text_encoder.lstms.{2 * i + 1}"))
    }
    p.lstm <- read_bilstm(m, bytes, "predictor.lstm")
    p.duration_proj <- read_linear(m, bytes, "predictor.duration_proj.linear_layer")
    p.shared_lstm <- read_bilstm(m, bytes, "predictor.shared")
    p.f0 |> reserve(3)
    p.n |> reserve(3)
    for (j in range(3)) {
        p.f0 |> emplace(read_adain_res_blk(m, bytes, "predictor.F0.{j}"))
        p.n |> emplace(read_adain_res_blk(m, bytes, "predictor.N.{j}"))
    }
    p.f0_proj <- read_conv_p(m, bytes, "predictor.F0_proj")
    p.n_proj <- read_conv_p(m, bytes, "predictor.N_proj")
}

def private read_decoder(m : GGUFMeta; bytes : array<uint8> | #; var d : St2Decoder) {
    d.encode <- read_adain_res_blk(m, bytes, "decoder.encode")
    var i = 0
    while (gguf_find_tensor(m, "decoder.decode.{i}.conv1.weight") >= 0) {
        d.decode |> emplace(read_adain_res_blk(m, bytes, "decoder.decode.{i}"))
        i++
    }
    d.f0_conv <- read_conv_p(m, bytes, "decoder.F0_conv")
    d.n_conv <- read_conv_p(m, bytes, "decoder.N_conv")
    d.asr_res <- read_conv_p(m, bytes, "decoder.asr_res.0")
    i = 0
    while (gguf_find_tensor(m, "decoder.generator.ups.{i}.weight") >= 0) {
        d.ups |> emplace(read_conv_p(m, bytes, "decoder.generator.ups.{i}", true))
        d.noise_convs |> emplace(read_conv_p(m, bytes, "decoder.generator.noise_convs.{i}"))
        d.noise_res |> emplace(read_gen_res_block(m, bytes, "decoder.generator.noise_res.{i}"))
        i++
    }
    i = 0
    while (gguf_find_tensor(m, "decoder.generator.resblocks.{i}.convs1.0.weight") >= 0) {
        d.resblocks |> emplace(read_gen_res_block(m, bytes, "decoder.generator.resblocks.{i}"))
        i++
    }
    d.n_kernels = long_length(d.resblocks) / long_length(d.ups)
    d.conv_post <- read_conv_p(m, bytes, "decoder.generator.conv_post", true)
    d.stft_fwd_re <- read_conv(m, bytes, "decoder.generator.stft.weight_forward_real", "")
    d.stft_fwd_im <- read_conv(m, bytes, "decoder.generator.stft.weight_forward_imag", "")
    d.stft_bwd_re <- read_conv(m, bytes, "decoder.generator.stft.weight_backward_real", "")
    d.stft_bwd_im <- read_conv(m, bytes, "decoder.generator.stft.weight_backward_imag", "")
    d.bins = d.stft_fwd_re.cout
    d.source_linear <- read_linear(m, bytes, "decoder.generator.m_source.l_linear")
    d.stft_pad_reflect = gguf_str(m, bytes, "styletts2.stft_pad") == "reflect"
    d.istft_envelope = gguf_str(m, bytes, "styletts2.istft_norm") == "envelope"
    d.stft_eps = gguf_f32(m, bytes, "styletts2.stft_eps")
    if (gguf_find_tensor(m, "decoder.generator.stft.window") >= 0) {
        d.window <- read_weight(m, bytes, "decoder.generator.stft.window")
    }
}

def private read_weight(m : GGUFMeta; bytes : array<uint8> | #; name : string) : TtsWeight {
    var w = TtsWeight()
    w.a <- read_arr(m, bytes, name)
    return <- w
}

//! Read a converted StyleTTS2-lineage GGUF (harness/convert_kitten.py, harness/convert_kokoro.py)
//! into the mint-side carrier: served layouts minted, voices and STFT convention read, every
//! weight moved into the staging blob. Family keys are the family file's to read.
def stage_styletts2(path : string; q8 : bool = false) : St2Staging {
    var inscope st = St2Staging()
    st.e.q8 = q8
    g_stage_q8 = q8
    g_stage_repack = q8 && select_matmul_backend_for_load_()
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA styletts2: cannot open model '{path}'")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        var inscope m <- parse_gguf_meta(bytes)
        st.e.family = gguf_str(m, bytes, "general.architecture")
        if (!gguf_has(m, "styletts2.sample_rate")) {
            panic("dasLLAMA styletts2: '{path}' ({st.e.family}) carries no styletts2 metadata")
        }
        st.e.name = gguf_str(m, bytes, "general.name")
        st.e.sample_rate = int(gguf_int(m, bytes, "styletts2.sample_rate"))
        st.e.n_symbols = gguf_int(m, bytes, "styletts2.n_symbols")
        st.e.style_dim = gguf_int(m, bytes, "styletts2.style_dim")
        st.e.style_rows = int(gguf_int(m, bytes, "styletts2.style_rows"))
        st.e.source.n_harm = gguf_int(m, bytes, "styletts2.n_harmonics")
        st.e.source.sample_rate = float(st.e.sample_rate)
        st.e.source.torch_math = gguf_has(m, "styletts2.resample") && gguf_str(m, bytes, "styletts2.resample") == "torch"
        st.e.voice_names <- gguf_str_array(m, bytes, "styletts2.voices")
        for (v in st.e.voice_names) {
            st.e.voices[v] <- read_arr(m, bytes, "voice.{v}")
        }
        read_albert(m, bytes, st.e.albert)
        read_text_encoder(m, bytes, st.e.text_encoder)
        read_predictor(m, bytes, st.e.predictor)
        read_decoder(m, bytes, st.e.decoder)
    }
    fclose(f)
    var io = TtsBlobIo(staging = true, blob = unsafe(addr(st.blob)), qblob = unsafe(addr(st.qblob)))
    weights_walk(io, st.e)
    return <- st
}

//! Mint a served model from a staged read without touching disk - the in-memory image rail,
//! the off-rail control for suites. The staged read's lane picks the tag.
def mint_styletts2(var st : St2Staging; var out : StyleTts2Model) {
    let tag = st.e.q8 ? ST2_IMAGE_TAG_Q8 : ST2_IMAGE_TAG
    cache_via_image_staged(st, out, "", tag)
    st2_bind(out)
}

//! Load a converted StyleTTS2-lineage GGUF through its prepared image on the current lane: the
//! sibling `.dlim` maps when it exists, else the GGUF stages, bakes and maps
//! (``DASLLAMA_IMAGE=0`` mints in memory).
def load_styletts2(path : string) : StyleTts2Model {
    var inscope m = StyleTts2Model()
    let q8 = st2_serve_q8()
    let tag = st2_tag()
    st2_announce_lane()
    if (g_env_engine.image) {
        apply_box_profile_runtime()   // a backend pin must precede the identity (it names the backend)
        let img = image_path_for(path, tag)
        let ts = ref_time_ticks()
        if (load_image(img, m, tag)) {
            st2_bind(m)
            to_log(LOG_INFO, "dasLLAMA styletts2: prepared image mapped in {get_time_usec(ts) / 1000} ms - {img}\n")
            return <- m
        }
        var inscope st <- stage_styletts2(path, q8)
        if (cache_via_image_staged(st, m, img, tag)) {
            dlim_gc_stale(path, tag)   // the fresh bake ON DISK proves its lane's siblings dead
        }
        st2_bind(m)
        return <- m
    }
    var inscope st <- stage_styletts2(path, q8)
    mint_styletts2(st, m)
    return <- m
}

//! True when `name` is one of the model's voices.
def styletts2_has_voice(m : StyleTts2Model; name : string) : bool {
    return key_exists(m.voices, name)
}

//! The style row `row` of voice `name` (a canonical name), `style_dim` floats.
def styletts2_style(m : StyleTts2Model; name : string; row : int; @exact_size var style : array<float>) {
    style |> reserve_resize(m.style_dim)
    let sd = int(m.style_dim)
    let r = clamp(row, 0, m.style_rows - 1)
    m.voices |> get(name) $(pack) {
        for (i in range(sd)) {
            style[i] = pack[r * sd + i]
        }
    }
}

// ===== assembly =====

//! Per-stage activations, filled when a synthesis is asked to trace (the oracle rail).
struct StyleTts2Trace {
    bert : array<float>
    text_enc : array<float>
    dur_raw : array<float>
    f0 : array<float>
    n : array<float>
    asr_res : array<float>
    dec_in : array<float>
    gen_in : array<float>
    har : array<float>
    stft_re : array<float>
    ups0 : array<float>
    ups1 : array<float>
    conv_post : array<float>
    har_spec : array<float>
    src0 : array<float>
    nres0 : array<float>
    rb0 : array<float>
    rb1 : array<float>
    rb2 : array<float>
    frames : int64
}

//! The synthesis carrier: every activation buffer the pass sizes, reused across syntheses (a
//! context runs one at a time), so a synthesis allocates nothing - the `[hot_path]` contract.
//! `wave` is the last synthesis's waveform; `noise` the source noise it drew.
struct St2Scratch {
    @scratch @exact_size s_ref : array<float>
    @scratch @exact_size s_pro : array<float>
    @scratch @exact_size bert : array<float>
    @scratch @exact_size d_en : array<float>
    @scratch @exact_size t_en : array<float>
    @scratch @exact_size d : array<float>
    @scratch @exact_size dur : array<int>
    @scratch @exact_size raw : array<float>
    @scratch @exact_size en : array<float>
    @scratch @exact_size asr : array<float>
    @scratch @exact_size f0 : array<float>
    @scratch @exact_size nrg : array<float>
    @scratch @exact_size wave : array<float>
    noise : TtsNoise = TtsNoise()
    @scratch @exact_size al_e : array<float>
    @scratch @exact_size al_attn : array<float>
    @scratch @exact_size al_f : array<float>
    @scratch @exact_size al_f2 : array<float>
    @scratch @exact_size att_q : array<float>
    @scratch @exact_size att_k : array<float>
    @scratch @exact_size att_v : array<float>
    @scratch @exact_size att_ctx : array<float>
    @scratch @exact_size te_xtm : array<float>
    @scratch @exact_size te_a : array<float>
    @scratch @exact_size te_b : array<float>
    @scratch @exact_size de_x : array<float>
    @scratch @exact_size de_y : array<float>
    @scratch @exact_size du_x : array<float>
    @scratch @exact_size du_logits : array<float>
    @scratch @exact_size pr_en_tm : array<float>
    @scratch @exact_size pr_x_tm : array<float>
    @scratch @exact_size pr_x : array<float>
    @scratch @exact_size pb_a : array<float>
    @scratch @exact_size pb_b : array<float>
    @scratch @exact_size rb_r : array<float>
    @scratch @exact_size rb_pooled : array<float>
    @scratch @exact_size rb_c1 : array<float>
    @scratch @exact_size rb_sc : array<float>
    @scratch @exact_size rb_sc2 : array<float>
    @scratch @exact_size dec_f0d : array<float>
    @scratch @exact_size dec_nd : array<float>
    @scratch @exact_size dec_res : array<float>
    @scratch @exact_size dec_a : array<float>
    @scratch @exact_size dec_b : array<float>
    @scratch @exact_size dec_cat : array<float>
    @scratch @exact_size dec_har : array<float>
    @scratch @exact_size dec_spec : array<float>
    @scratch @exact_size so_sines : array<float>
    @scratch @exact_size hs_padded : array<float>
    @scratch @exact_size hs_re : array<float>
    @scratch @exact_size hs_im : array<float>
    @scratch @exact_size gen_a : array<float>
    @scratch @exact_size gen_b : array<float>
    @scratch @exact_size st_y : array<float>
    @scratch @exact_size st_rb : array<float>
    @scratch @exact_size st_src_ct : array<float>
    @scratch @exact_size st_src : array<float>
    @scratch @exact_size st_src_res : array<float>
    @scratch @exact_size grb_xt : array<float>
    @scratch @exact_size grb_y1 : array<float>
    @scratch @exact_size grb_y2 : array<float>
    @scratch @exact_size gen_post : array<float>
    @scratch @exact_size gen_re : array<float>
    @scratch @exact_size gen_im : array<float>
    @scratch @exact_size gen_wr : array<float>
    @scratch @exact_size gen_wi : array<float>
}

// ===== the trace rail's captures: the oracle harness's opt-in leg, cold by contract =====

[cold_path]
def private trace_take(var dst : array<float>; src : array<float>) {
    dst := src
}

[cold_path]
def private trace_rows(var dst : array<float>; rows : array<float>; t, c : int64) {
    transpose_tc(rows, t, c, dst)
}

[cold_path]
def private trace_rows_strided(var dst : array<float>; rows : array<float>; t, c, stride : int64) {
    transpose_tc_strided(rows, t, c, stride, dst)
}

// Self-attention on token-major h [t][hidden]: the projections on the tiled GEMM, the heads
// on the attention kernel, the output projection back on the tile.
def private albert_attention(a : St2Albert; h : array<float>; t : int64; var out : array<float>; var sc : St2Scratch) {
    var q & = unsafe(sc.att_q)
    var k & = unsafe(sc.att_k)
    var v & = unsafe(sc.att_v)
    var ctx & = unsafe(sc.att_ctx)
    linear_rows(a.q, h, t, q)
    linear_rows(a.k, h, t, k)
    linear_rows(a.v, h, t, v)
    attention_rows(q, k, v, t, a.hidden, a.heads, ctx)
    linear_rows(a.dense, ctx, t, out)
}

//! The PL-BERT encoder: embeddings, then one ALBERT layer applied n_layers times; out [T][hidden].
def styletts2_albert(a : St2Albert; ids : array<int>; @scratch @exact_size var out : array<float>; var sc : St2Scratch) {
    let t = long_length(ids)
    if (t > a.max_pos) {
        panic("dasLLAMA styletts2: {t} tokens exceed the encoder's {a.max_pos} positions")
    }
    unsafe(scratch_resize(sc.al_e, t * a.emb_dim))
    var e & = unsafe(sc.al_e)
    for (i in range64(t)) {
        let id = int64(ids[i])
        for (d in range64(a.emb_dim)) {
            e[i * a.emb_dim + d] = a.word_emb.a[id * a.emb_dim + d] + a.pos_emb.a[i * a.emb_dim + d] + a.type_emb.a[d]
        }
    }
    layernorm_rows(e, t, a.emb_dim, a.emb_ln, ALBERT_EPS)
    linear_rows(a.map_in, e, t, out)
    var attn & = unsafe(sc.al_attn)
    var f & = unsafe(sc.al_f)
    var f2 & = unsafe(sc.al_f2)
    let n = t * a.hidden
    for (_layer in range64(a.n_layers)) {
        albert_attention(a, out, t, attn, sc)
        for (i in range64(n)) {
            attn[i] += out[i]
        }
        layernorm_rows(attn, t, a.hidden, a.attn_ln, ALBERT_EPS)
        linear_rows(a.ffn, attn, t, f)
        gelu(f, t * a.ffn.nout)
        linear_rows(a.ffn_out, f, t, f2)
        for (i in range64(n)) {
            out[i] = f2[i] + attn[i]
        }
        layernorm_rows(out, t, a.hidden, a.full_ln, ALBERT_EPS)
    }
}

//! The text encoder: embedding, conv + channel LayerNorm + LeakyReLU stack, BiLSTM; out [C][T].
def styletts2_text_encoder(te : St2TextEncoder; ids : array<int>; @scratch @exact_size var out : array<float>; var sc : St2Scratch) {
    let t = long_length(ids)
    let c = te.c
    unsafe(scratch_resize(sc.te_xtm, t * c))
    var xtm & = unsafe(sc.te_xtm)
    for (i in range64(t)) {
        let id = int64(ids[i])
        for (d in range64(c)) {
            xtm[i * c + d] = te.emb.a[id * c + d]
        }
    }
    var a & = unsafe(sc.te_a)
    var b & = unsafe(sc.te_b)
    transpose_tc(xtm, t, c, a)
    for (i in range(length(te.cnn))) {   // the conv stack ping-pongs a and b
        if (i % 2 == 0) {
            conv1d(te.cnn[i], a, t, b)
            layernorm_channels(b, c, t, te.cnn_ln[i], LN_EPS)
            leaky_relu(b, c * t, 0.2)
        } else {
            conv1d(te.cnn[i], b, t, a)
            layernorm_channels(a, c, t, te.cnn_ln[i], LN_EPS)
            leaky_relu(a, c * t, 0.2)
        }
    }
    if (length(te.cnn) % 2 == 1) {
        transpose_ct(b, c, t, xtm)
    } else {
        transpose_ct(a, c, t, xtm)
    }
    bilstm(te.lstm, xtm, t, a)
    transpose_tc(a, t, c, out)
}

def private append_style_rows(x : array<float>; t, c : int64; s : array<float>; @scratch @exact_size var out : array<float>) {
    let sd = long_length(s)
    out |> reserve_resize(t * (c + sd))
    for (i in range64(t)) {
        for (d in range64(c)) {
            out[i * (c + sd) + d] = x[i * c + d]
        }
        for (d in range64(sd)) {
            out[i * (c + sd) + c + d] = s[d]
        }
    }
}

//! The duration encoder: alternating BiLSTM and AdaLayerNorm passes over the BERT features
//! with the prosody style appended after each; out is token-major [T][C + style].
def styletts2_duration_encoder(p : St2Predictor; d_en : array<float>; t : int64; s : array<float>; @scratch @exact_size var out : array<float>; var sc : St2Scratch) {
    let c = p.bert_encoder.nout
    var x & = unsafe(sc.de_x)
    var y & = unsafe(sc.de_y)
    append_style_rows(d_en, t, c, s, x)
    for (i in range(length(p.lstms))) {
        bilstm(p.lstms[i], x, t, y)
        ada_layernorm_rows(p.lns[i], y, t, c, s, LN_EPS)
        append_style_rows(y, t, c, s, x)
    }
    copy_floats_into(x, t * (c + long_length(s)), out)
}

//! Frame counts per token: sigmoid(duration logits) summed, divided by speed, rounded half
//! to even, at least one. Returns the total frame count.
def styletts2_durations(p : St2Predictor; d : array<float>; t : int64; speed : float; @scratch @exact_size var dur : array<int>; @scratch @exact_size var raw : array<float>; var sc : St2Scratch) : int64 {
    var x & = unsafe(sc.du_x)
    var logits & = unsafe(sc.du_logits)
    bilstm(p.lstm, d, t, x)
    linear_rows(p.duration_proj, x, t, logits)
    let nb = p.duration_proj.nout
    dur |> reserve_resize(t)
    raw |> reserve_resize(t)
    var frames = 0l
    for (i in range64(t)) {
        var acc = 0.0
        for (j in range64(nb)) {
            acc += 1.0 / (1.0 + exp(-logits[i * nb + j]))
        }
        raw[i] = acc / speed
        let r = max(1.0, round_half_even(raw[i]))
        dur[i] = int(r)
        frames += int64(dur[i])
    }
    return frames
}

def private copy_floats_into(src : array<float>; n : int64; @scratch @exact_size var dst : array<float>) {
    dst |> reserve_resize(n)
    for (i in range64(n)) {
        dst[i] = src[i]
    }
}

// out = (out + shortcut) / sqrt(2): the residual block's join
def private residual_join(var out : array<float>; shortcut : array<float>; n : int64) {
    let inv = 1.0 / sqrt(2.0)
    for (i in range64(n)) {
        out[i] = (out[i] + shortcut[i]) * inv
    }
}

def private adain_res_blk(b : St2AdainResBlk; x : array<float>; t : int64; s : array<float>; @scratch @exact_size var out : array<float>; var sc : St2Scratch) : int64 {
    var r & = unsafe(sc.rb_r)
    var pooled & = unsafe(sc.rb_pooled)
    var c1 & = unsafe(sc.rb_c1)
    var shortcut & = unsafe(sc.rb_sc)
    var shortcut2 & = unsafe(sc.rb_sc2)
    copy_floats_into(x, b.dim_in * t, r)
    adain(b.norm1, r, b.dim_in, t, s, LN_EPS)
    leaky_relu(r, b.dim_in * t, 0.2)
    var t1 = t
    if (b.upsample) {
        conv1d(b.pool, r, t, pooled)
        t1 = conv1d_out_len(b.pool, t)
        conv1d(b.conv1, pooled, t1, c1)
    } else {
        conv1d(b.conv1, r, t1, c1)
    }
    adain(b.norm2, c1, b.dim_out, t1, s, LN_EPS)
    leaky_relu(c1, b.dim_out * t1, 0.2)
    conv1d(b.conv2, c1, t1, out)
    if (b.upsample) {
        resize_nearest(x, b.dim_in, t, 2l, shortcut)
    } else {
        copy_floats_into(x, b.dim_in * t, shortcut)
    }
    if (b.learned_sc) {
        conv1d(b.conv1x1, shortcut, t1, shortcut2)
        residual_join(out, shortcut2, b.dim_out * t1)
    } else {
        residual_join(out, shortcut, b.dim_out * t1)
    }
    return t1
}

// The branch's residual blocks ping-pong the carrier's two branch buffers, the first reading x.
def private prosody_branch(blocks : array<St2AdainResBlk>; proj : TtsConv1d; x : array<float>; frames : int64; s : array<float>;
                           @scratch @exact_size var out : array<float>; var sc : St2Scratch) : int64 {
    var a & = unsafe(sc.pb_a)
    var b & = unsafe(sc.pb_b)
    var t = frames
    let nb = length(blocks)
    for (i in range(nb)) {
        if (i == 0) {
            t = adain_res_blk(blocks[i], x, t, s, a, sc)
        } elif (i % 2 == 1) {
            t = adain_res_blk(blocks[i], a, t, s, b, sc)
        } else {
            t = adain_res_blk(blocks[i], b, t, s, a, sc)
        }
    }
    if (nb == 0) {
        conv1d(proj, x, t, out)
    } elif (nb % 2 == 1) {
        conv1d(proj, a, t, out)
    } else {
        conv1d(proj, b, t, out)
    }
    return conv1d_out_len(proj, t)
}

//! F0 and energy curves from the aligned duration-encoder features: the shared BiLSTM, then
//! each branch's three residual blocks (the middle one doubles the frame rate) and projection.
def styletts2_prosody(p : St2Predictor; en : array<float>; c, frames : int64; s : array<float>;
                      @scratch @exact_size var f0 : array<float>; @scratch @exact_size var n : array<float>; var sc : St2Scratch) : int64 {
    var en_tm & = unsafe(sc.pr_en_tm)
    var x_tm & = unsafe(sc.pr_x_tm)
    var x & = unsafe(sc.pr_x)
    transpose_ct(en, c, frames, en_tm)
    bilstm(p.shared_lstm, en_tm, frames, x_tm)
    let width = 2l * p.shared_lstm.fwd.hidden
    transpose_tc(x_tm, frames, width, x)
    let f2 = prosody_branch(p.f0, p.f0_proj, x, frames, s, f0, sc)
    prosody_branch(p.n, p.n_proj, x, frames, s, n, sc)
    return f2
}

// One Snake residual block on token-major rows: `y` = `src` plus the three conv pairs' residuals,
// the first pair reading `src` so no copy of the input is ever made. Every op class lands in
// its tts.gen.* bucket of the encode-stage profiling rail.
def private gen_res_block(b : St2GenResBlock; src : array<float>; @scratch @exact_size var y : array<float>; t : int64; s : array<float>; var sc : St2Scratch) {
    let n = b.c * t
    var xt & = unsafe(sc.grb_xt)
    var y1 & = unsafe(sc.grb_y1)
    var y2 & = unsafe(sc.grb_y2)
    for (j in range(length(b.convs1))) {
        var tc = ref_time_ticks()
        if (j == 0) {
            adain_rows_into(b.adain1[j], src, xt, t, b.c, s, LN_EPS)
        } else {
            adain_rows_into(b.adain1[j], y, xt, t, b.c, s, LN_EPS)
        }
        asr_prof_add("tts.gen.adain", tc)
        tc = ref_time_ticks()
        snake_rows(xt, t, b.c, b.alpha1[j].a)
        asr_prof_add("tts.gen.snake", tc)
        tc = ref_time_ticks()
        conv1d_rows(b.convs1[j], xt, t, y1)
        asr_prof_add("tts.gen.conv", tc)
        tc = ref_time_ticks()
        adain_rows(b.adain2[j], y1, t, b.c, s, LN_EPS)
        asr_prof_add("tts.gen.adain", tc)
        tc = ref_time_ticks()
        snake_rows(y1, t, b.c, b.alpha2[j].a)
        asr_prof_add("tts.gen.snake", tc)
        tc = ref_time_ticks()
        conv1d_rows(b.convs2[j], y1, t, y2)
        asr_prof_add("tts.gen.conv", tc)
        tc = ref_time_ticks()
        if (j == 0) {
            sum_rows_into(y, src, y2, n)
        } else {
            add_rows(y, y2, n)
        }
        asr_prof_add("tts.gen.resadd", tc)
    }
}

//! The harmonic source: sines plus noise from the F0 curve, mixed to one signal of
//! frames * upsample samples.
def styletts2_source(d : St2Decoder; cfg : SineSourceCfg; f0 : array<float>; f0_frames : int64;
                     noise : TtsNoise; @scratch @exact_size var har : array<float>; var sc : St2Scratch) : int64 {
    var sines & = unsafe(sc.so_sines)
    let n = sine_source(cfg, f0, f0_frames, noise.uniform, noise.normal, sines)
    linear_rows(d.source_linear, sines, n, har)
    tanh_inplace(har, n)
    return n
}

//! The source spectrum: the mixed signal edge-padded and analysed by the STFT convs into
//! [2 * bins][frames] (magnitude rows, then phase rows).
def styletts2_har_spectrum(d : St2Decoder; har : array<float>; n : int64; var tr : StyleTts2Trace?;
                           @scratch @exact_size var har_spec : array<float>; var sc : St2Scratch) : int64 {
    var padded & = unsafe(sc.hs_padded)
    var re & = unsafe(sc.hs_re)
    var im & = unsafe(sc.hs_im)
    if (d.stft_pad_reflect) {
        pad_reflect(har, n, STFT_PAD, padded)
    } else {
        pad_edge(har, n, STFT_PAD, padded)
    }
    conv1d(d.stft_fwd_re, padded, n + 2l * STFT_PAD, re)
    conv1d(d.stft_fwd_im, padded, n + 2l * STFT_PAD, im)
    let frames = conv1d_out_len(d.stft_fwd_re, n + 2l * STFT_PAD)
    if (tr != null) {
        trace_take(tr.stft_re, re)
    }
    magnitude_phase(re, im, d.bins, frames, d.stft_eps, har_spec)
    if (tr != null) {
        trace_take(tr.har_spec, har_spec)
    }
    return frames
}

// One generator stage: x [tt][cc] rows in, `out` the next stage's rows; the upsampled stream
// carries the noise branch and the averaged Snake residual blocks.
def private generator_stage(d : St2Decoder; i : int64; var x : array<float>; @scratch @exact_size var out : array<float>;
                            var cc, tt : int64&; s : array<float>; har_spec : array<float>; har_frames : int64;
                            var tr : StyleTts2Trace?; var sc : St2Scratch) {
    var y & = unsafe(sc.st_y)
    var src_ct & = unsafe(sc.st_src_ct)
    var src & = unsafe(sc.st_src)
    var src_res & = unsafe(sc.st_src_res)
    var tc = ref_time_ticks()
    leaky_relu(x, cc * tt, 0.1)
    asr_prof_add("tts.gen.leaky", tc)
    tc = ref_time_ticks()
    conv1d(d.noise_convs[i], har_spec, har_frames, src_ct)
    let src_t = conv1d_out_len(d.noise_convs[i], har_frames)
    let src_c = d.noise_convs[i].cout
    if (tr != null && i == 0l) {
        trace_take(tr.src0, src_ct)
    }
    transpose_ct(src_ct, src_c, src_t, src)
    asr_prof_add("tts.gen.noise", tc)
    gen_res_block(d.noise_res[i], src, src_res, src_t, s, sc)
    if (tr != null && i == 0l) {
        trace_rows(tr.nres0, src_res, src_t, src_c)
    }
    tc = ref_time_ticks()
    conv1d_rows(d.ups[i], x, tt, y)
    asr_prof_add("tts.gen.ups", tc)
    tt = conv1d_out_len(d.ups[i], tt)
    cc = d.ups[i].cout
    if (tr != null) {
        if (i == 0l) {
            trace_rows(tr.ups0, y, tt, cc)
        } else {
            trace_rows(tr.ups1, y, tt, cc)
        }
    }
    if (i == long_length(d.ups) - 1l) {
        pad_reflect_left_rows_inplace(y, tt, cc, 1l)
        tt++
    }
    if (src_t != tt) {
        panic("dasLLAMA styletts2: source branch {src_t} frames against {tt} at stage {i}")
    }
    let n = cc * tt
    tc = ref_time_ticks()
    add_rows(y, src_res, n)
    asr_prof_add("tts.gen.stagesum", tc)
    stage_resblocks(d, i, y, out, cc, tt, s, tr, sc)
}

// The stage's n_kernels Snake residual blocks, each from the stage stream x, averaged into out.
def private stage_resblocks(d : St2Decoder; i : int64; x : array<float>; @scratch @exact_size var out : array<float>;
                            cc, tt : int64; s : array<float>; var tr : StyleTts2Trace?; var sc : St2Scratch) {
    let n = cc * tt
    var y & = unsafe(sc.st_rb)
    var tc = 0l
    for (j in range64(d.n_kernels)) {
        gen_res_block(d.resblocks[i * d.n_kernels + j], x, y, tt, s, sc)
        if (tr != null && i == 0l) {
            if (j == 0l) {
                trace_rows(tr.rb0, y, tt, cc)
            } elif (j == 1l) {
                trace_rows(tr.rb1, y, tt, cc)
            } elif (j == 2l) {
                trace_rows(tr.rb2, y, tt, cc)
            }
        }
        tc = ref_time_ticks()
        if (j == 0l) {
            copy_rows(out, y, n)
        } else {
            add_rows(out, y, n)
        }
        asr_prof_add("tts.gen.stagesum", tc)
    }
    tc = ref_time_ticks()
    scale_rows(out, 1.0 / float(d.n_kernels), n)
    asr_prof_add("tts.gen.stagesum", tc)
}

// ===== the generator GPU hook (a driver registers at [init] and requires this module for its
// types, so nothing here can require the driver back; the CPU chain below is the
// specification and the fallback) =====

//! Serves the whole generator - the rows form [t][c] of its input, the style, the source
//! spectrum - writing exactly the waveform's samples into `wave`, or declines with false.
typedef St2GeneratorGpuFn = function<(d : St2Decoder; x_rows : array<float>; c, t : int64; s : array<float>;
                                       har_spec : array<float>; har_frames : int64; var wave : array<float>) : bool>
var private g_st2_gen_gpu : St2GeneratorGpuFn
var private g_st2_gen_gpu_set = false
var private g_st2_gen_gpu_calls = 0
var private g_st2_gen_gpu_served = 0

[arch(at = "../ARCHITECTURE_MEDIA.md#tower-gpu-hook")]
def register_styletts2_generator_gpu(fn : St2GeneratorGpuFn) {
    g_st2_gen_gpu = fn
    g_st2_gen_gpu_set = true
}

//! Hook invocations and the ones that served, since the process started: engage is read from
//! these counters, never inferred from a synthesis having run.
def styletts2_generator_gpu_stats() : tuple<calls : int; served : int> {
    return (calls = g_st2_gen_gpu_calls, served = g_st2_gen_gpu_served)
}

//! The iSTFTNet generator: two upsampling stages, each adding the source spectrum through its
//! noise branch and refining through Snake residual blocks, then the inverse STFT. A registered
//! driver gets the first refusal, except on the trace rail (its stage tensors are the point).
def styletts2_generator(d : St2Decoder; x_in : array<float>; c, t : int64; s : array<float>;
                        har_spec : array<float>; har_frames : int64; var tr : StyleTts2Trace?;
                        @scratch @exact_size var wave : array<float>; var sc : St2Scratch) : int64 {
    var a & = unsafe(sc.gen_a)
    var b & = unsafe(sc.gen_b)
    transpose_ct(x_in, c, t, a)
    if (g_st2_gen_gpu_set && tr == null) {
        g_st2_gen_gpu_calls++
        if (invoke(g_st2_gen_gpu, d, a, c, t, s, har_spec, har_frames, wave)) {
            g_st2_gen_gpu_served++
            return long_length(wave)
        }
    }
    var cc = c
    var tt = t
    let nst = long_length(d.ups)
    for (i in range64(nst)) {   // the stages ping-pong a and b
        if (i % 2l == 0l) {
            generator_stage(d, i, a, b, cc, tt, s, har_spec, har_frames, tr, sc)
        } else {
            generator_stage(d, i, b, a, cc, tt, s, har_spec, har_frames, tr, sc)
        }
    }
    if (nst % 2l == 1l) {
        return generator_tail(d, b, cc, tt, tr, wave, sc)
    }
    return generator_tail(d, a, cc, tt, tr, wave, sc)
}

// conv_post, the polar-to-rectangular pair and the inverse STFT: the waveform into `wave`.
def private generator_tail(d : St2Decoder; var x : array<float>; cc, tt : int64; var tr : StyleTts2Trace?;
                           @scratch @exact_size var wave : array<float>; var sc : St2Scratch) : int64 {
    var y & = unsafe(sc.gen_post)
    var re & = unsafe(sc.gen_re)
    var im & = unsafe(sc.gen_im)
    var wr & = unsafe(sc.gen_wr)
    var wi & = unsafe(sc.gen_wi)
    let tc = ref_time_ticks()
    leaky_relu(x, cc * tt, 0.01)
    conv1d_rows(d.conv_post, x, tt, y)
    if (tr != null) {
        trace_rows_strided(tr.conv_post, y, tt, d.conv_post.cout, d.conv_post.cout_s)
    }
    polar_to_rect_rows(y, d.conv_post.cout_s, d.bins, tt, re, im)
    asr_prof_add("tts.gen.post", tc)
    conv1d(d.stft_bwd_re, re, tt, wr)
    conv1d(d.stft_bwd_im, im, tt, wi)
    let full = conv1d_out_len(d.stft_bwd_re, tt)
    let n = full - 2l * STFT_PAD
    wave |> reserve_resize(n)
    for (i in range64(n)) {
        wave[i] = wr[STFT_PAD + i] - wi[STFT_PAD + i]
    }
    if (d.istft_envelope) {
        istft_envelope_divide(wave, n, d.window.a, d.stft_bwd_re.k, d.stft_bwd_re.stride, STFT_PAD)
    }
    return n
}

//! Draw the source noise the synthesis needs when none was captured.
def styletts2_draw_noise(cfg : SineSourceCfg; frames : int64; seed : uint64; var noise : TtsNoise) {
    return if (noise.captured)
    var rng = TtsRng()
    rng_seed(rng, seed)
    let n = frames * cfg.upsample
    unsafe(scratch_resize(noise.uniform, cfg.n_harm))
    for (h in range64(cfg.n_harm)) {
        noise.uniform[h] = rng_uniform(rng)
    }
    unsafe(scratch_resize(noise.normal, n * cfg.n_harm))
    for (i in range64(n * cfg.n_harm)) {
        noise.normal[i] = rng_normal(rng)
    }
}

//! cat(x [c][t], F0 [1][t], N [1][t]) with asr_res between when `with_res`.
def private concat_channels(x : array<float>; c : int64; f0d, nd, res : array<float>; with_res : bool; t : int64; @scratch @exact_size var out : array<float>) {
    let rc = with_res ? long_length(res) / t : 0l
    let total = c + rc + 2l
    out |> reserve_resize(total * t)
    for (i in range64(c * t)) {
        out[i] = x[i]
    }
    if (with_res) {
        for (i in range64(rc * t)) {
            out[c * t + i] = res[i]
        }
    }
    for (i in range64(t)) {
        out[(c + rc) * t + i] = f0d[i]
        out[(c + rc + 1l) * t + i] = nd[i]
    }
}

[cold_path]
def private trace_dec_in(var tr : StyleTts2Trace?; y : array<float>; cw : int64; f0d, nd, res : array<float>; frames : int64) {
    concat_channels(y, cw, f0d, nd, res, true, frames, tr.dec_in)
}

// One decoder block from `src`: through the concat scratch when the block's width asks for
// the F0, energy and residual channels again, straight through otherwise.
def private decode_step(blk : St2AdainResBlk; cw : int64; src : array<float>; @scratch @exact_size var cat : array<float>;
                        @scratch @exact_size var dst : array<float>; t : int64; f0d, nd, res, s_ref : array<float>; var sc : St2Scratch) : int64 {
    if (blk.dim_in != cw) {
        concat_channels(src, cw, f0d, nd, res, true, t, cat)
        return adain_res_blk(blk, cat, t, s_ref, dst, sc)
    }
    return adain_res_blk(blk, src, t, s_ref, dst, sc)
}

// The decoder: F0, energy and the asr residual as channels, the encode block, the decode blocks
// ping-ponging the carrier's two stream buffers, then the source and the generator into sc.wave.
def private styletts2_decode(d : St2Decoder; text_c : int64; source : SineSourceCfg; asr : array<float>; frames : int64;
                             f0, nrg : array<float>; f2 : int64; s_ref : array<float>; var noise : TtsNoise; seed : uint64;
                             var tr : StyleTts2Trace?; var tm : TtsTimings; var sc : St2Scratch) : int64 {
    var clock = ref_time_ticks()
    var f0d & = unsafe(sc.dec_f0d)
    var nd & = unsafe(sc.dec_nd)
    var res & = unsafe(sc.dec_res)
    var a & = unsafe(sc.dec_a)
    var b & = unsafe(sc.dec_b)
    var cat & = unsafe(sc.dec_cat)
    conv1d(d.f0_conv, f0, f2, f0d)
    conv1d(d.n_conv, nrg, f2, nd)
    conv1d(d.asr_res, asr, frames, res)
    if (tr != null) {
        trace_take(tr.asr_res, res)
    }
    concat_channels(asr, text_c, f0d, nd, res, false, frames, cat)
    var t = adain_res_blk(d.encode, cat, frames, s_ref, a, sc)
    var cw = d.encode.dim_out
    if (tr != null) {
        trace_dec_in(tr, a, cw, f0d, nd, res, frames)
    }
    var in_a = true
    for (i in range(length(d.decode))) {
        if (in_a) {
            t = decode_step(d.decode[i], cw, a, cat, b, t, f0d, nd, res, s_ref, sc)
        } else {
            t = decode_step(d.decode[i], cw, b, cat, a, t, f0d, nd, res, s_ref, sc)
        }
        in_a = !in_a
        cw = d.decode[i].dim_out
    }
    if (tr != null) {
        if (in_a) {
            trace_take(tr.gen_in, a)
        } else {
            trace_take(tr.gen_in, b)
        }
    }
    tm.decoder_us = int64(get_time_usec(clock))
    clock = ref_time_ticks()
    styletts2_draw_noise(source, f2, seed, noise)
    var har & = unsafe(sc.dec_har)
    var har_spec & = unsafe(sc.dec_spec)
    let n = styletts2_source(d, source, f0, f2, noise, har, sc)
    if (tr != null) {
        trace_take(tr.har, har)
    }
    let har_frames = styletts2_har_spectrum(d, har, n, tr, har_spec, sc)
    tm.source_us = int64(get_time_usec(clock))
    clock = ref_time_ticks()
    var samples = 0l
    if (in_a) {
        samples = styletts2_generator(d, a, cw, t, s_ref, har_spec, har_frames, tr, sc.wave, sc)
    } else {
        samples = styletts2_generator(d, b, cw, t, s_ref, har_spec, har_frames, tr, sc.wave, sc)
    }
    tm.generator_us = int64(get_time_usec(clock))
    tm.samples = samples
    return samples
}

//! Synthesize from token ids: the full StyleTTS2 pass into `sc.wave` (the facade trims the
//! tail), the sample count back. `noise` supplies the source noise (empty = drawn from `seed`),
//! `tr` collects the stage tensors when not null, `tm` the stage walls, `sc` every buffer.
[hot_path]
def styletts2_synthesize(m : StyleTts2Model; ids : array<int>; style : array<float>; speed : float;
                         var noise : TtsNoise; seed : uint64; var tr : StyleTts2Trace?; var tm : TtsTimings; var sc : St2Scratch) : int64 {
    let start = ref_time_ticks()
    var clock = start
    let t = long_length(ids)
    let hs = m.style_dim / 2l
    unsafe(scratch_resize(sc.s_ref, hs))
    unsafe(scratch_resize(sc.s_pro, hs))
    var s_ref & = unsafe(sc.s_ref)
    var s_pro & = unsafe(sc.s_pro)
    for (i in range64(hs)) {
        s_ref[i] = style[i]
        s_pro[i] = style[hs + i]
    }
    styletts2_albert(m.albert, ids, sc.bert, sc)
    linear_rows(m.predictor.bert_encoder, sc.bert, t, sc.d_en)
    if (tr != null) {
        trace_take(tr.bert, sc.d_en)
    }
    tm.bert_us = int64(get_time_usec(clock))
    clock = ref_time_ticks()
    styletts2_text_encoder(m.text_encoder, ids, sc.t_en, sc)
    if (tr != null) {
        trace_take(tr.text_enc, sc.t_en)
    }
    tm.text_enc_us = int64(get_time_usec(clock))
    clock = ref_time_ticks()
    styletts2_duration_encoder(m.predictor, sc.d_en, t, s_pro, sc.d, sc)
    let dc = m.predictor.bert_encoder.nout + hs
    let frames = styletts2_durations(m.predictor, sc.d, t, speed, sc.dur, sc.raw, sc)
    if (tr != null) {
        trace_take(tr.dur_raw, sc.raw)
        tr.frames = frames
    }
    align_expand_rows(sc.d, t, dc, sc.dur, sc.en)
    align_expand_channels(sc.t_en, m.text_encoder.c, t, sc.dur, sc.asr)
    tm.duration_us = int64(get_time_usec(clock))
    clock = ref_time_ticks()
    let f2 = styletts2_prosody(m.predictor, sc.en, dc, frames, s_pro, sc.f0, sc.nrg, sc)
    if (tr != null) {
        trace_take(tr.f0, sc.f0)
        trace_take(tr.n, sc.nrg)
    }
    tm.prosody_us = int64(get_time_usec(clock))
    let samples = styletts2_decode(m.decoder, m.text_encoder.c, m.source, sc.asr, frames, sc.f0, sc.nrg, f2, s_ref, noise, seed, tr, tm, sc)
    tm.total_us = tm.front_end_us + int64(get_time_usec(start))
    return samples
}
