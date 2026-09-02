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
    blob : PlaneF = PlaneF()
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

// serialize_image_meta covers 13 fields; voices (views bound from voice_spans), image_map and
// image_bytes are the deliberate skips
let private ST2_META_FIELDS = 13 + 3
let ST2_IMAGE_TAG = "tts-f32"

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
}

[unused_argument(m)]
def private image_post_load(var m : StyleTts2Model) {}

//! Point every weight view into the mapped blob - after the parse bound the plane.
def private st2_bind(var m : StyleTts2Model) {
    var io = TtsBlobIo(staging = false, plane = m.blob)
    weights_walk(io, m)
}

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
}

def private serialize_image_meta(var arch : Archive; var st : St2Staging) {
    serialize_image_meta(arch, st.e)
}

[unused_argument(st)]
def private image_post_load(var st : St2Staging) {}

def finalize(var st : St2Staging) {
    delete st.blob
    delete st.e
}

[init]
def private register_styletts2_image_tags {
    register_image_family_tag(ST2_IMAGE_TAG, true)   // no CPU knob shapes the blob: one image per box
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
    conv1d_prepare(c)
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
    linear_prepare(l, rows_only)
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
def stage_styletts2(path : string) : St2Staging {
    var inscope st = St2Staging()
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
    var io = TtsBlobIo(staging = true, blob = unsafe(addr(st.blob)))
    weights_walk(io, st.e)
    return <- st
}

//! Mint a served model from a staged read without touching disk - the in-memory image rail,
//! the off-rail control for suites.
def mint_styletts2(var st : St2Staging; var out : StyleTts2Model) {
    cache_via_image_staged(st, out, "", ST2_IMAGE_TAG)
    st2_bind(out)
}

//! Load a converted StyleTTS2-lineage GGUF through its prepared image: the sibling `.dlim` maps
//! when it exists, else the GGUF stages, bakes and maps (``DASLLAMA_IMAGE=0`` mints in memory).
def load_styletts2(path : string) : StyleTts2Model {
    var inscope m = StyleTts2Model()
    if (g_env_engine.image) {
        apply_box_profile_runtime()   // a backend pin must precede the identity (it names the backend)
        let img = image_path_for(path, ST2_IMAGE_TAG)
        let ts = ref_time_ticks()
        if (load_image(img, m, ST2_IMAGE_TAG)) {
            st2_bind(m)
            to_log(LOG_INFO, "dasLLAMA styletts2: prepared image mapped in {get_time_usec(ts) / 1000} ms - {img}\n")
            return <- m
        }
        var inscope st <- stage_styletts2(path)
        if (cache_via_image_staged(st, m, img, ST2_IMAGE_TAG)) {
            dlim_gc_stale(path, ST2_IMAGE_TAG)   // the fresh bake ON DISK proves its lane's siblings dead
        }
        st2_bind(m)
        return <- m
    }
    var inscope st <- stage_styletts2(path)
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

// Self-attention on token-major h [t][hidden]: the projections on the tiled GEMM, then one
// head per lane - Q.K^T and P.V on the tiles over a 16-padded key axis (the pad keys score
// -1e30 and carry zero values), softmax per row between them.
def private albert_attention(a : St2Albert; h : array<float>; t : int64; var out : array<float>) {
    var inscope @scratch q : array<float>
    var inscope @scratch k : array<float>
    var inscope @scratch v : array<float>
    var inscope @scratch ctx : array<float>
    var inscope @scratch head_scratch : array<float>
    linear_rows(a.q, h, t, q)
    linear_rows(a.k, h, t, k)
    linear_rows(a.v, h, t, v)
    let hidden = a.hidden
    let heads = a.heads
    let dh = hidden / heads
    let tp = (t + 15l) / 16l * 16l
    let per_head = t * dh + dh * tp + tp * dh + t * tp + t * dh   // qh, kT, vh, s, oh
    ctx |> reserve_resize(t * hidden)
    head_scratch |> reserve_resize(heads * per_head)
    let scale = 1.0 / sqrt(float(dh))
    let qp = unsafe(addr<float const?>(q[0]))
    let kp = unsafe(addr<float const?>(k[0]))
    let vp = unsafe(addr<float const?>(v[0]))
    var cp = unsafe(addr(ctx[0]))
    var sp = unsafe(addr(head_scratch[0]))
    maybe_parallel_for(0, int(heads), int(heads)) $(hb, he) {
        unsafe {
            for (hd in range64(int64(hb), int64(he))) {
                let off = hd * dh
                var qh = sp + hd * per_head
                var kT = qh + t * dh
                var vh = kT + dh * tp
                var s = vh + tp * dh
                var oh = s + t * tp
                for (i in range64(t)) {
                    for (e in range64(dh)) {
                        qh[i * dh + e] = qp[i * hidden + off + e]
                        vh[i * dh + e] = vp[i * hidden + off + e]
                        kT[e * tp + i] = kp[i * hidden + off + e]
                    }
                }
                for (i in range64(t * dh, tp * dh)) {
                    vh[i] = 0.0
                }
                for (e in range64(dh)) {
                    for (i in range64(t, tp)) {
                        kT[e * tp + i] = 0.0
                    }
                }
                for (i in range64(t * tp)) {
                    s[i] = 0.0
                }
                gemm_f32_jo(s, qh, kT, t, dh, tp)
                for (i in range64(t)) {
                    var row = s + i * tp
                    for (j in range64(t)) {
                        row[j] *= scale
                    }
                    for (j in range64(t, tp)) {
                        row[j] = -1e30
                    }
                    softmax(row, tp)
                }
                for (i in range64(t * dh)) {
                    oh[i] = 0.0
                }
                gemm_f32_jo(oh, s, vh, t, tp, dh)
                for (i in range64(t)) {
                    for (e in range64(dh)) {
                        cp[i * hidden + off + e] = oh[i * dh + e]
                    }
                }
            }
        }
    }
    linear_rows(a.dense, ctx, t, out)
}

//! The PL-BERT encoder: embeddings, then one ALBERT layer applied n_layers times; out [T][hidden].
def styletts2_albert(a : St2Albert; ids : array<int>; @exact_size var out : array<float>) {
    let t = long_length(ids)
    if (t > a.max_pos) {
        panic("dasLLAMA styletts2: {t} tokens exceed the encoder's {a.max_pos} positions")
    }
    var inscope @exact_size e : array<float>
    e |> reserve_resize(t * a.emb_dim)
    for (i in range64(t)) {
        let id = int64(ids[i])
        for (d in range64(a.emb_dim)) {
            e[i * a.emb_dim + d] = a.word_emb.a[id * a.emb_dim + d] + a.pos_emb.a[i * a.emb_dim + d] + a.type_emb.a[d]
        }
    }
    layernorm_rows(e, t, a.emb_dim, a.emb_ln, ALBERT_EPS)
    var inscope @exact_size h : array<float>
    linear_rows(a.map_in, e, t, h)
    var inscope @scratch attn : array<float>
    var inscope @scratch f : array<float>
    var inscope @scratch f2 : array<float>
    let n = t * a.hidden
    for (_layer in range64(a.n_layers)) {
        albert_attention(a, h, t, attn)
        for (i in range64(n)) {
            attn[i] += h[i]
        }
        layernorm_rows(attn, t, a.hidden, a.attn_ln, ALBERT_EPS)
        linear_rows(a.ffn, attn, t, f)
        gelu(f, t * a.ffn.nout)
        linear_rows(a.ffn_out, f, t, f2)
        for (i in range64(n)) {
            h[i] = f2[i] + attn[i]
        }
        layernorm_rows(h, t, a.hidden, a.full_ln, ALBERT_EPS)
    }
    delete out
    out <- h
}

//! The text encoder: embedding, conv + channel LayerNorm + LeakyReLU stack, BiLSTM; out [C][T].
def styletts2_text_encoder(te : St2TextEncoder; ids : array<int>; @exact_size var out : array<float>) {
    let t = long_length(ids)
    let c = te.c
    var inscope @exact_size xtm : array<float>
    xtm |> reserve_resize(t * c)
    for (i in range64(t)) {
        let id = int64(ids[i])
        for (d in range64(c)) {
            xtm[i * c + d] = te.emb.a[id * c + d]
        }
    }
    var inscope @exact_size x : array<float>
    transpose_tc(xtm, t, c, x)
    var inscope @exact_size y : array<float>
    for (i in range(length(te.cnn))) {
        conv1d(te.cnn[i], x, t, y)
        layernorm_channels(y, c, t, te.cnn_ln[i], LN_EPS)
        leaky_relu(y, c * t, 0.2)
        delete x
        x <- y
    }
    transpose_ct(x, c, t, xtm)
    bilstm(te.lstm, xtm, t, y)
    transpose_tc(y, t, c, out)
}

def private append_style_rows(x : array<float>; t, c : int64; s : array<float>; @exact_size var out : array<float>) {
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
def styletts2_duration_encoder(p : St2Predictor; d_en : array<float>; t : int64; s : array<float>; @exact_size var out : array<float>) {
    let c = p.bert_encoder.nout
    var inscope @exact_size x : array<float>
    var inscope @exact_size y : array<float>
    append_style_rows(d_en, t, c, s, x)
    for (i in range(length(p.lstms))) {
        bilstm(p.lstms[i], x, t, y)
        ada_layernorm_rows(p.lns[i], y, t, c, s, LN_EPS)
        append_style_rows(y, t, c, s, x)
    }
    delete out
    out <- x
}

//! Frame counts per token: sigmoid(duration logits) summed, divided by speed, rounded half
//! to even, at least one. Returns the total frame count.
def styletts2_durations(p : St2Predictor; d : array<float>; t : int64; speed : float; @exact_size var dur : array<int>; @exact_size var raw : array<float>) : int64 {
    var inscope @exact_size x : array<float>
    var inscope @exact_size logits : array<float>
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

def private copy_floats_into(src : array<float>; n : int64; @exact_size var dst : array<float>) {
    dst |> reserve_resize(n)
    for (i in range64(n)) {
        dst[i] = src[i]
    }
}

def private adain_res_blk(b : St2AdainResBlk; x : array<float>; t : int64; s : array<float>; @exact_size var out : array<float>) : int64 {
    var inscope @exact_size r : array<float>
    copy_floats_into(x, b.dim_in * t, r)
    adain(b.norm1, r, b.dim_in, t, s, LN_EPS)
    leaky_relu(r, b.dim_in * t, 0.2)
    var t1 = t
    if (b.upsample) {
        var inscope @exact_size pooled : array<float>
        conv1d(b.pool, r, t, pooled)
        t1 = conv1d_out_len(b.pool, t)
        delete r
        r <- pooled
    }
    var inscope @exact_size c1 : array<float>
    conv1d(b.conv1, r, t1, c1)
    adain(b.norm2, c1, b.dim_out, t1, s, LN_EPS)
    leaky_relu(c1, b.dim_out * t1, 0.2)
    conv1d(b.conv2, c1, t1, out)
    var inscope @exact_size sc : array<float>
    if (b.upsample) {
        resize_nearest(x, b.dim_in, t, 2l, sc)
    } else {
        copy_floats_into(x, b.dim_in * t, sc)
    }
    if (b.learned_sc) {
        var inscope @exact_size sc2 : array<float>
        conv1d(b.conv1x1, sc, t1, sc2)
        delete sc
        sc <- sc2
    }
    let inv = 1.0 / sqrt(2.0)
    for (i in range64(b.dim_out * t1)) {
        out[i] = (out[i] + sc[i]) * inv
    }
    return t1
}

def private prosody_branch(blocks : array<St2AdainResBlk>; proj : TtsConv1d; x : array<float>; frames : int64; s : array<float>; @exact_size var out : array<float>) : int64 {
    var inscope @exact_size a : array<float>
    var inscope @exact_size b : array<float>
    copy_floats_into(x, long_length(x), a)
    var t = frames
    for (blk in blocks) {
        t = adain_res_blk(blk, a, t, s, b)
        delete a
        a <- b
    }
    conv1d(proj, a, t, out)
    return conv1d_out_len(proj, t)
}

//! F0 and energy curves from the aligned duration-encoder features: the shared BiLSTM, then
//! each branch's three residual blocks (the middle one doubles the frame rate) and projection.
def styletts2_prosody(p : St2Predictor; en : array<float>; c, frames : int64; s : array<float>;
                      @exact_size var f0 : array<float>; @exact_size var n : array<float>) : int64 {
    var inscope @exact_size en_tm : array<float>
    var inscope @exact_size x_tm : array<float>
    var inscope @exact_size x : array<float>
    transpose_ct(en, c, frames, en_tm)
    bilstm(p.shared_lstm, en_tm, frames, x_tm)
    let width = 2l * p.shared_lstm.fwd.hidden
    transpose_tc(x_tm, frames, width, x)
    let f2 = prosody_branch(p.f0, p.f0_proj, x, frames, s, f0)
    prosody_branch(p.n, p.n_proj, x, frames, s, n)
    return f2
}

// One Snake residual block on token-major x [t][c].
def private gen_res_block(b : St2GenResBlock; var x : array<float>; t : int64; s : array<float>) {
    let n = b.c * t
    var inscope @exact_size xt : array<float>
    var inscope @exact_size y1 : array<float>
    var inscope @exact_size y2 : array<float>
    for (j in range(length(b.convs1))) {
        copy_floats_into(x, n, xt)
        adain_rows(b.adain1[j], xt, t, b.c, s, LN_EPS)
        snake_rows(xt, t, b.c, b.alpha1[j].a)
        conv1d_rows(b.convs1[j], xt, t, y1)
        adain_rows(b.adain2[j], y1, t, b.c, s, LN_EPS)
        snake_rows(y1, t, b.c, b.alpha2[j].a)
        conv1d_rows(b.convs2[j], y1, t, y2)
        for (i in range64(n)) {
            x[i] += y2[i]
        }
    }
}

//! The harmonic source: sines plus noise from the F0 curve, mixed to one signal of
//! frames * upsample samples.
def styletts2_source(d : St2Decoder; cfg : SineSourceCfg; f0 : array<float>; f0_frames : int64;
                     noise : TtsNoise; @exact_size var har : array<float>) : int64 {
    var inscope @exact_size sines : array<float>
    let n = sine_source(cfg, f0, f0_frames, noise.uniform, noise.normal, sines)
    linear_rows(d.source_linear, sines, n, har)
    tanh_inplace(har, n)
    return n
}

//! The source spectrum: the mixed signal edge-padded and analysed by the STFT convs into
//! [2 * bins][frames] (magnitude rows, then phase rows).
def styletts2_har_spectrum(d : St2Decoder; har : array<float>; n : int64; var tr : StyleTts2Trace?;
                           @exact_size var har_spec : array<float>) : int64 {
    var inscope @exact_size padded : array<float>
    if (d.stft_pad_reflect) {
        pad_reflect(har, n, STFT_PAD, padded)
    } else {
        pad_edge(har, n, STFT_PAD, padded)
    }
    var inscope @exact_size re : array<float>
    var inscope @exact_size im : array<float>
    conv1d(d.stft_fwd_re, padded, n + 2l * STFT_PAD, re)
    conv1d(d.stft_fwd_im, padded, n + 2l * STFT_PAD, im)
    let frames = conv1d_out_len(d.stft_fwd_re, n + 2l * STFT_PAD)
    if (tr != null) {
        tr.stft_re := re
    }
    magnitude_phase(re, im, d.bins, frames, d.stft_eps, har_spec)
    if (tr != null) {
        tr.har_spec := har_spec
    }
    return frames
}

def private generator_stage(d : St2Decoder; i : int64; var x : array<float>; var cc, tt : int64&; s : array<float>;
                            har_spec : array<float>; har_frames : int64; var tr : StyleTts2Trace?) {
    var inscope @exact_size y : array<float>
    var inscope @exact_size xs : array<float>
    var inscope @exact_size src_ct : array<float>
    var inscope @exact_size src : array<float>
    leaky_relu(x, cc * tt, 0.1)
    conv1d(d.noise_convs[i], har_spec, har_frames, src_ct)
    let src_t = conv1d_out_len(d.noise_convs[i], har_frames)
    let src_c = d.noise_convs[i].cout
    if (tr != null && i == 0l) {
        tr.src0 := src_ct
    }
    transpose_ct(src_ct, src_c, src_t, src)
    gen_res_block(d.noise_res[i], src, src_t, s)
    if (tr != null && i == 0l) {
        transpose_tc(src, src_t, src_c, tr.nres0)
    }
    conv1d_rows(d.ups[i], x, tt, y)
    tt = conv1d_out_len(d.ups[i], tt)
    cc = d.ups[i].cout
    if (tr != null) {
        if (i == 0l) {
            transpose_tc(y, tt, cc, tr.ups0)
        } else {
            transpose_tc(y, tt, cc, tr.ups1)
        }
    }
    if (i == long_length(d.ups) - 1l) {
        pad_reflect_left_rows(y, tt, cc, 1l, x)
        tt++
    } else {
        delete x
        x <- y
    }
    if (src_t != tt) {
        panic("dasLLAMA styletts2: source branch {src_t} frames against {tt} at stage {i}")
    }
    let n = cc * tt
    for (e in range64(n)) {
        x[e] += src[e]
    }
    xs |> reserve_resize(n)
    for (e in range64(n)) {
        xs[e] = 0.0
    }
    for (j in range64(d.n_kernels)) {
        copy_floats_into(x, n, y)
        gen_res_block(d.resblocks[i * d.n_kernels + j], y, tt, s)
        if (tr != null && i == 0l) {
            if (j == 0l) {
                transpose_tc(y, tt, cc, tr.rb0)
            } elif (j == 1l) {
                transpose_tc(y, tt, cc, tr.rb1)
            } elif (j == 2l) {
                transpose_tc(y, tt, cc, tr.rb2)
            }
        }
        for (e in range64(n)) {
            xs[e] += y[e]
        }
    }
    let inv = 1.0 / float(d.n_kernels)
    for (e in range64(n)) {
        x[e] = xs[e] * inv
    }
}

//! The iSTFTNet generator: two upsampling stages, each adding the source spectrum through
//! its noise branch and refining through Snake residual blocks, then the inverse STFT.
def styletts2_generator(d : St2Decoder; x_in : array<float>; c, t : int64; s : array<float>;
                        har_spec : array<float>; har_frames : int64; var tr : StyleTts2Trace?; @exact_size var wave : array<float>) : int64 {
    var inscope @exact_size x : array<float>
    transpose_ct(x_in, c, t, x)
    var cc = c
    var tt = t
    for (i in range64(long_length(d.ups))) {
        generator_stage(d, i, x, cc, tt, s, har_spec, har_frames, tr)
    }
    leaky_relu(x, cc * tt, 0.01)
    var inscope @exact_size y : array<float>
    conv1d_rows(d.conv_post, x, tt, y)
    if (tr != null) {
        transpose_tc_strided(y, tt, d.conv_post.cout, d.conv_post.cout_s, tr.conv_post)
    }
    var inscope @exact_size re : array<float>
    var inscope @exact_size im : array<float>
    polar_to_rect_rows(y, d.conv_post.cout_s, d.bins, tt, re, im)
    var inscope @exact_size wr : array<float>
    var inscope @exact_size wi : array<float>
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
    return if (!empty(noise.normal))
    var rng = TtsRng()
    rng_seed(rng, seed)
    let n = frames * cfg.upsample
    noise.uniform |> reserve_resize(cfg.n_harm)
    for (h in range64(cfg.n_harm)) {
        noise.uniform[h] = rng_uniform(rng)
    }
    noise.normal |> reserve_resize(n * cfg.n_harm)
    for (i in range64(n * cfg.n_harm)) {
        noise.normal[i] = rng_normal(rng)
    }
}

//! cat(x [c][t], F0 [1][t], N [1][t]) with asr_res between when `with_res`.
def private concat_channels(x : array<float>; c : int64; f0d, nd, res : array<float>; with_res : bool; t : int64; @exact_size var out : array<float>) {
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

def private styletts2_decode(d : St2Decoder; text_c : int64; source : SineSourceCfg; asr : array<float>; frames : int64;
                             f0, nrg : array<float>; f2 : int64; s_ref : array<float>; var noise : TtsNoise; seed : uint64;
                             var tr : StyleTts2Trace?; var tm : TtsTimings) : array<float> {
    var clock = ref_time_ticks()
    var inscope @exact_size f0d : array<float>
    var inscope @exact_size nd : array<float>
    var inscope @exact_size res : array<float>
    conv1d(d.f0_conv, f0, f2, f0d)
    conv1d(d.n_conv, nrg, f2, nd)
    conv1d(d.asr_res, asr, frames, res)
    if (tr != null) {
        tr.asr_res := res
    }
    var inscope @exact_size x : array<float>
    var inscope @exact_size y : array<float>
    concat_channels(asr, text_c, f0d, nd, res, false, frames, x)
    var t = adain_res_blk(d.encode, x, frames, s_ref, y)
    var cw = d.encode.dim_out
    if (tr != null) {
        concat_channels(y, cw, f0d, nd, res, true, frames, tr.dec_in)
    }
    for (i in range(length(d.decode))) {
        if (d.decode[i].dim_in != cw) {
            concat_channels(y, cw, f0d, nd, res, true, t, x)
        } else {
            delete x
            x <- y
        }
        t = adain_res_blk(d.decode[i], x, t, s_ref, y)
        cw = d.decode[i].dim_out
    }
    if (tr != null) {
        tr.gen_in := y
    }
    tm.decoder_us = int64(get_time_usec(clock))
    clock = ref_time_ticks()
    styletts2_draw_noise(source, f2, seed, noise)
    var inscope @exact_size har : array<float>
    let n = styletts2_source(d, source, f0, f2, noise, har)
    if (tr != null) {
        tr.har := har
    }
    var inscope @exact_size har_spec : array<float>
    let har_frames = styletts2_har_spectrum(d, har, n, tr, har_spec)
    tm.source_us = int64(get_time_usec(clock))
    clock = ref_time_ticks()
    var inscope @exact_size wave : array<float>
    styletts2_generator(d, y, cw, t, s_ref, har_spec, har_frames, tr, wave)
    tm.generator_us = int64(get_time_usec(clock))
    tm.samples = long_length(wave)
    return <- wave
}

//! Synthesize from token ids: the full StyleTTS2 pass, raw waveform (the reference driver's
//! tail trim is the facade's). `noise` supplies the source noise (empty = drawn from `seed`);
//! `tr` collects the stage tensors when not null; `tm` receives each stage's wall time.
def styletts2_synthesize(m : StyleTts2Model; ids : array<int>; style : array<float>; speed : float;
                         var noise : TtsNoise; seed : uint64; var tr : StyleTts2Trace?; var tm : TtsTimings) : array<float> {
    let start = ref_time_ticks()
    var clock = start
    let t = long_length(ids)
    let hs = m.style_dim / 2l
    var inscope @exact_size s_ref : array<float>
    var inscope @exact_size s_pro : array<float>
    s_ref |> reserve_resize(hs)
    s_pro |> reserve_resize(hs)
    for (i in range64(hs)) {
        s_ref[i] = style[i]
        s_pro[i] = style[hs + i]
    }
    var inscope @exact_size bert : array<float>
    styletts2_albert(m.albert, ids, bert)
    var inscope @exact_size d_en : array<float>
    linear_rows(m.predictor.bert_encoder, bert, t, d_en)
    if (tr != null) {
        tr.bert := d_en
    }
    tm.bert_us = int64(get_time_usec(clock))
    clock = ref_time_ticks()
    var inscope @exact_size t_en : array<float>
    styletts2_text_encoder(m.text_encoder, ids, t_en)
    if (tr != null) {
        tr.text_enc := t_en
    }
    tm.text_enc_us = int64(get_time_usec(clock))
    clock = ref_time_ticks()
    var inscope @exact_size d : array<float>
    styletts2_duration_encoder(m.predictor, d_en, t, s_pro, d)
    let dc = m.predictor.bert_encoder.nout + hs
    var inscope @exact_size dur : array<int>
    var inscope @exact_size raw : array<float>
    let frames = styletts2_durations(m.predictor, d, t, speed, dur, raw)
    if (tr != null) {
        tr.dur_raw := raw
        tr.frames = frames
    }
    var inscope @exact_size en : array<float>
    var inscope @exact_size asr : array<float>
    align_expand_rows(d, t, dc, dur, en)
    align_expand_channels(t_en, m.text_encoder.c, t, dur, asr)
    tm.duration_us = int64(get_time_usec(clock))
    clock = ref_time_ticks()
    var inscope @exact_size f0 : array<float>
    var inscope @exact_size nrg : array<float>
    let f2 = styletts2_prosody(m.predictor, en, dc, frames, s_pro, f0, nrg)
    if (tr != null) {
        tr.f0 := f0
        tr.n := nrg
    }
    tm.prosody_us = int64(get_time_usec(clock))
    var inscope wave <- styletts2_decode(m.decoder, m.text_encoder.c, m.source, asr, frames, f0, nrg, f2, s_ref, noise, seed, tr, tm)
    tm.total_us = tm.front_end_us + int64(get_time_usec(start))
    return <- wave
}
