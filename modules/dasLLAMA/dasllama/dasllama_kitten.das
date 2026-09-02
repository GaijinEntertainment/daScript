options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_kitten shared public

require dasllama/dasllama_lint public
require math
require strings
require daslib/fio
require dasllama/dasllama_gguf
require dasllama/dasllama_common
require dasllama/dasllama_math
require dasllama/dasllama_unicode
require dasllama/dasllama_tts_types
require dasllama/dasllama_tts_blocks

//! The KittenTTS family (nano, mini): the weight map of the converted GGUF
//! (harness/convert_kitten.py), the StyleTTS2-lineage assembly over the block home, the
//! voices with their speed priors, the reference driver's token table and style-row rule, and
//! the rewrite of the front end's inventory into the espeak-style IPA these models consume.

// ===== the reference driver's symbol table and token rule =====

let private KITTEN_SYMBOL_CPTS = fixed_array<int>(36, 59, 58, 44, 46, 33, 63, 161, 191, 8212, 8230, 171, 187, 34, 32, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 593, 592, 594, 230, 595, 665, 946, 596, 597, 231, 599, 598, 240, 676, 601, 600, 602, 603, 604, 605, 606, 607, 644, 609, 608, 610, 667, 614, 615, 295, 613, 668, 616, 618, 669, 621, 620, 619, 622, 671, 625, 623, 624, 331, 627, 626, 628, 248, 629, 632, 952, 339, 630, 664, 633, 634, 638, 635, 640, 641, 637, 642, 643, 648, 679, 649, 650, 651, 11377, 652, 611, 612, 653, 967, 654, 655, 657, 656, 658, 660, 673, 661, 674, 448, 449, 450, 451, 712, 716, 720, 721, 700, 692, 688, 689, 690, 695, 736, 740, 734, 8595, 8593, 8594, 8599, 8600, 809, 39, 7547)
let private KITTEN_SYMBOL_IDS = fixed_array<int>(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 13, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 175, 176, 177)

//! Rewrite phonemes from the front end's inventory into the espeak-style IPA these models
//! consume. Sequential rewrites, in this order: the diphthong and consonant letters, then
//! the calibrated length and rhotic rules.
def kitten_phonemes_from_misaki(ps : string) : string {
    var s = ps
    s = replace(s, "ʤ", "dʒ")   // nolint:STYLE039
    s = replace(s, "ʧ", "tʃ")   // nolint:STYLE039
    s = replace(s, "A", "eɪ")   // nolint:STYLE039
    s = replace(s, "I", "aɪ")   // nolint:STYLE039
    s = replace(s, "Y", "ɔɪ")   // nolint:STYLE039
    s = replace(s, "O", "oʊ")   // nolint:STYLE039
    s = replace(s, "Q", "əʊ")   // nolint:STYLE039
    s = replace(s, "W", "aʊ")   // nolint:STYLE039
    s = replace(s, "T", "ɾ")   // nolint:STYLE039
    s = replace(s, "ᵊ", "ə")   // nolint:STYLE039
    // the calibrated rules: length marks espeak keeps, the rhotic vowel spellings
    s = replace(s, "ɜɹ", "ɜːɹ")   // nolint:STYLE039
    s = replace(s, "ɑ", "ɑː")   // nolint:STYLE039
    s = replace(s, "ɔ", "ɔː")   // nolint:STYLE039
    s = replace(s, "u", "uː")   // nolint:STYLE039
    s = replace(s, "əɹ", "ɚ")   // nolint:STYLE039
    s = replace(s, "ɜːɹ", "ɜː")   // nolint:STYLE039
    return s
}

let private ELLIPSIS_CPT = 8230   // the driver closes every sequence with an ellipsis

def private kitten_word_cpt(c : int) : bool {
    return cpt_is_letter(c) || cpt_is_number(c) || c == '_'
}

//! The token ids the model reads for an espeak-inventory phoneme string: the reference
//! driver's re-spacing (runs of word characters and single punctuation marks, one space
//! between), unknown symbols dropped, wrapped in pad / ellipsis / pad.
def kitten_token_ids(ps : string) : array<int> {
    let sym <- {for (c, i in KITTEN_SYMBOL_CPTS, KITTEN_SYMBOL_IDS); c => i}
    var inscope cps <- utf8_to_cpts(ps)
    var ids : array<int>
    ids |> push(0)
    let space_id = sym?[' '] ?? 0
    var in_word = false
    var first = true
    for (c in cps) {
        if (c == ' ' || c == '\t' || c == '\n') {
            in_word = false
            continue
        }
        let word = kitten_word_cpt(c)
        if (!first && !(word && in_word)) {
            ids |> push(space_id)
        }
        first = false
        in_word = word
        let id = sym?[c] ?? -1
        if (id >= 0) {
            ids |> push(id)
        }
    }
    ids |> push(sym?[ELLIPSIS_CPT] ?? 0)
    ids |> push(0)
    return <- ids
}

//! The style row the reference driver picks: the chunk's character count, capped at the last
//! row.
def kitten_style_row(norm_text : string; rows : int) : int {
    var inscope cps <- utf8_to_cpts(norm_text)
    return min(length(cps), rows - 1)
}

// ===== weights =====

struct KittenAlbert {
    word_emb : array<float>
    pos_emb : array<float>
    type_emb : array<float>
    emb_ln : TtsNorm
    map_in : TtsLinear
    q : TtsLinear
    k : TtsLinear
    v : TtsLinear
    dense : TtsLinear
    attn_ln : TtsNorm
    ffn : TtsLinear
    ffn_out : TtsLinear
    full_ln : TtsNorm
    emb_dim : int64
    hidden : int64
    heads : int64
    n_layers : int64
    max_pos : int64
}

struct KittenTextEncoder {
    emb : array<float>
    cnn : array<TtsConv1d>
    cnn_ln : array<TtsNorm>
    lstm : TtsBiLstm
    c : int64
}

//! StyleTTS2's AdainResBlk1d: two AdaIN + LeakyReLU + conv passes, an optional 2x upsample
//! (depthwise transposed conv on the residual, nearest on the shortcut), a learned 1x1
//! shortcut when the width changes, the sum scaled by 1/sqrt(2).
struct KittenAdainResBlk {
    norm1 : TtsAdaIn
    norm2 : TtsAdaIn
    conv1 : TtsConv1d = TtsConv1d()
    conv2 : TtsConv1d = TtsConv1d()
    conv1x1 : TtsConv1d = TtsConv1d()
    pool : TtsConv1d = TtsConv1d()
    upsample : bool
    learned_sc : bool
    dim_in : int64
    dim_out : int64
}

struct KittenPredictor {
    bert_encoder : TtsLinear
    lstms : array<TtsBiLstm>
    lns : array<TtsAdaLayerNorm>
    lstm : TtsBiLstm
    duration_proj : TtsLinear
    shared_lstm : TtsBiLstm
    f0 : array<KittenAdainResBlk>
    n : array<KittenAdainResBlk>
    f0_proj : TtsConv1d = TtsConv1d()
    n_proj : TtsConv1d = TtsConv1d()
}

//! The generator's AdaINResBlock1: three dilated conv pairs with AdaIN and Snake before each.
struct KittenGenResBlock {
    convs1 : array<TtsConv1d>
    convs2 : array<TtsConv1d>
    adain1 : array<TtsAdaIn>
    adain2 : array<TtsAdaIn>
    alpha1 : array<array<float>>
    alpha2 : array<array<float>>
    c : int64
}

struct KittenDecoder {
    encode : KittenAdainResBlk = KittenAdainResBlk()
    decode : array<KittenAdainResBlk>
    f0_conv : TtsConv1d = TtsConv1d()
    n_conv : TtsConv1d = TtsConv1d()
    asr_res : TtsConv1d = TtsConv1d()
    ups : array<TtsConv1d>
    noise_convs : array<TtsConv1d>
    noise_res : array<KittenGenResBlock>
    resblocks : array<KittenGenResBlock>
    conv_post : TtsConv1d = TtsConv1d()
    stft_fwd_re : TtsConv1d = TtsConv1d()
    stft_fwd_im : TtsConv1d = TtsConv1d()
    stft_bwd_re : TtsConv1d = TtsConv1d()
    stft_bwd_im : TtsConv1d = TtsConv1d()
    source_linear : TtsLinear
    n_kernels : int64
    bins : int64
}

struct KittenModel {
    size : string
    sample_rate : int
    n_symbols : int64
    style_dim : int64
    style_rows : int
    voice_names : array<string>
    voices : table<string; array<float>>
    speed_priors : table<string; float>
    aliases : table<string; string>
    albert : KittenAlbert
    text_encoder : KittenTextEncoder
    predictor : KittenPredictor = KittenPredictor()
    decoder : KittenDecoder = KittenDecoder()
    source : SineSourceCfg = SineSourceCfg()
}

let private LN_EPS = 1e-5
let private ALBERT_EPS = 1e-12
let private STFT_EPS = 1e-14
let private STFT_PAD = 10l
let private TRIM_TAIL = 5000l

// ===== loading =====

def private need_tensor(m : GGUFMeta; name : string) : int {
    let ti = gguf_find_tensor(m, name)
    if (ti < 0) {
        panic("dasLLAMA kitten: tensor '{name}' missing from the model")
    }
    return ti
}

def private read_arr(m : GGUFMeta; bytes : array<uint8> | #; name : string) : array<float> {
    let ti = need_tensor(m, name)
    var a : array<float>
    a |> reserve_resize(m.tensors[ti].n_elem)
    gguf_read_tensor_f32(m, bytes, name, a, 0l, m.tensors[ti].n_elem)
    return <- a
}

def private read_conv(m : GGUFMeta; bytes : array<uint8> | #; wname, bname : string) : TtsConv1d {
    var c = TtsConv1d()
    c.w <- read_arr(m, bytes, wname)
    if (!empty(bname) && gguf_find_tensor(m, bname) >= 0) {
        c.b <- read_arr(m, bytes, bname)
    }
    let geo <- gguf_int_array(m, bytes, "kitten.conv.{wname}")
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
    conv1d_prepare(c)
    return <- c
}

def private read_conv_p(m : GGUFMeta; bytes : array<uint8> | #; prefix : string) : TtsConv1d {
    return <- read_conv(m, bytes, "{prefix}.weight", "{prefix}.bias")
}

def private read_linear(m : GGUFMeta; bytes : array<uint8> | #; prefix : string) : TtsLinear {
    var l = TtsLinear()
    l.w <- read_arr(m, bytes, "{prefix}.weight")
    let ti = need_tensor(m, "{prefix}.weight")
    l.nin = m.tensors[ti].dims[0]
    l.nout = m.tensors[ti].dims[1]
    if (gguf_find_tensor(m, "{prefix}.bias") >= 0) {
        l.b <- read_arr(m, bytes, "{prefix}.bias")
    }
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

def private read_adain_res_blk(m : GGUFMeta; bytes : array<uint8> | #; prefix : string) : KittenAdainResBlk {
    var b = KittenAdainResBlk()
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

def private read_gen_res_block(m : GGUFMeta; bytes : array<uint8> | #; prefix : string) : KittenGenResBlock {
    var b = KittenGenResBlock()
    b.convs1 |> reserve(3)
    b.convs2 |> reserve(3)
    b.adain1 |> reserve(3)
    b.adain2 |> reserve(3)
    b.alpha1 |> reserve(3)
    b.alpha2 |> reserve(3)
    for (j in range(3)) {
        b.convs1 |> emplace(read_conv_p(m, bytes, "{prefix}.convs1.{j}"))
        b.convs2 |> emplace(read_conv_p(m, bytes, "{prefix}.convs2.{j}"))
        b.adain1 |> emplace(read_adain(m, bytes, "{prefix}.adain1.{j}"))
        b.adain2 |> emplace(read_adain(m, bytes, "{prefix}.adain2.{j}"))
        b.alpha1 |> emplace(read_arr(m, bytes, "{prefix}.alpha1.{j}"))
        b.alpha2 |> emplace(read_arr(m, bytes, "{prefix}.alpha2.{j}"))
    }
    b.c = b.convs1[0].cout
    return <- b
}

def private read_albert(m : GGUFMeta; bytes : array<uint8> | #; var a : KittenAlbert) {
    a.word_emb <- read_arr(m, bytes, "bert.embeddings.word_embeddings.weight")
    a.pos_emb <- read_arr(m, bytes, "bert.embeddings.position_embeddings.weight")
    a.type_emb <- read_arr(m, bytes, "bert.embeddings.token_type_embeddings.weight")
    a.emb_ln <- read_norm(m, bytes, "bert.embeddings.LayerNorm.weight", "bert.embeddings.LayerNorm.bias")
    a.map_in <- read_linear(m, bytes, "bert.encoder.embedding_hidden_mapping_in")
    a.q <- read_linear(m, bytes, "bert.layer.attention.query")
    a.k <- read_linear(m, bytes, "bert.layer.attention.key")
    a.v <- read_linear(m, bytes, "bert.layer.attention.value")
    a.dense <- read_linear(m, bytes, "bert.layer.attention.dense")
    a.attn_ln <- read_norm(m, bytes, "bert.layer.attention.LayerNorm.weight", "bert.layer.attention.LayerNorm.bias")
    a.ffn <- read_linear(m, bytes, "bert.layer.ffn")
    a.ffn_out <- read_linear(m, bytes, "bert.layer.ffn_output")
    a.full_ln <- read_norm(m, bytes, "bert.layer.full_layer_layer_norm.weight", "bert.layer.full_layer_layer_norm.bias")
    a.emb_dim = a.map_in.nin
    a.hidden = a.map_in.nout
    a.heads = 12l
    a.n_layers = 12l
    a.max_pos = m.tensors[need_tensor(m, "bert.embeddings.position_embeddings.weight")].dims[1]
}

def private read_text_encoder(m : GGUFMeta; bytes : array<uint8> | #; var te : KittenTextEncoder) {
    te.emb <- read_arr(m, bytes, "text_encoder.embedding.weight")
    te.c = m.tensors[need_tensor(m, "text_encoder.embedding.weight")].dims[0]
    var i = 0
    while (gguf_find_tensor(m, "text_encoder.cnn.{i}.0.weight") >= 0) {
        te.cnn |> emplace(read_conv_p(m, bytes, "text_encoder.cnn.{i}.0"))
        te.cnn_ln |> emplace(read_norm(m, bytes, "text_encoder.cnn.{i}.1.gamma", "text_encoder.cnn.{i}.1.beta"))
        i++
    }
    te.lstm <- read_bilstm(m, bytes, "text_encoder.lstm")
}

def private read_predictor(m : GGUFMeta; bytes : array<uint8> | #; var p : KittenPredictor) {
    p.bert_encoder <- read_linear(m, bytes, "bert_encoder")
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

def private read_decoder(m : GGUFMeta; bytes : array<uint8> | #; var d : KittenDecoder) {
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
        d.ups |> emplace(read_conv_p(m, bytes, "decoder.generator.ups.{i}"))
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
    d.conv_post <- read_conv_p(m, bytes, "decoder.generator.conv_post")
    d.stft_fwd_re <- read_conv(m, bytes, "decoder.generator.stft.weight_forward_real", "")
    d.stft_fwd_im <- read_conv(m, bytes, "decoder.generator.stft.weight_forward_imag", "")
    d.stft_bwd_re <- read_conv(m, bytes, "decoder.generator.stft.weight_backward_real", "")
    d.stft_bwd_im <- read_conv(m, bytes, "decoder.generator.stft.weight_backward_imag", "")
    d.bins = d.stft_fwd_re.cout
    d.source_linear <- read_linear(m, bytes, "decoder.generator.m_source.l_linear")
}

//! Load a converted KittenTTS GGUF (harness/convert_kitten.py).
def load_kitten_model(path : string) : KittenModel {
    var km = KittenModel()
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA kitten: cannot open model '{path}'")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        var inscope m <- parse_gguf_meta(bytes)
        if (gguf_str(m, bytes, "general.architecture") != "kitten-tts") {
            panic("dasLLAMA kitten: '{path}' is not a kitten-tts GGUF")
        }
        km.size = gguf_str(m, bytes, "kitten.size")
        km.sample_rate = int(gguf_int(m, bytes, "kitten.sample_rate"))
        km.n_symbols = gguf_int(m, bytes, "kitten.n_symbols")
        km.style_dim = gguf_int(m, bytes, "kitten.style_dim")
        km.style_rows = int(gguf_int(m, bytes, "kitten.style_rows"))
        km.source.n_harm = gguf_int(m, bytes, "kitten.n_harmonics")
        km.source.sample_rate = float(km.sample_rate)
        km.voice_names <- gguf_str_array(m, bytes, "kitten.voices")
        for (v in km.voice_names) {
            km.voices[v] <- read_arr(m, bytes, "voice.{v}")
            if (gguf_has(m, "kitten.speed_prior.{v}")) {
                km.speed_priors[v] = gguf_f32(m, bytes, "kitten.speed_prior.{v}")
            }
        }
        let anames <- gguf_str_array(m, bytes, "kitten.alias_names")
        let avoices <- gguf_str_array(m, bytes, "kitten.alias_voices")
        for (an, av in anames, avoices) {
            km.aliases[an] = av
        }
        read_albert(m, bytes, km.albert)
        read_text_encoder(m, bytes, km.text_encoder)
        read_predictor(m, bytes, km.predictor)
        read_decoder(m, bytes, km.decoder)
    }
    fclose(f)
    return <- km
}

//! Resolve a voice name or alias to a canonical voice; "" when unknown.
def kitten_voice(m : KittenModel; name : string) : string {
    if (key_exists(m.voices, name)) {
        return name
    }
    return m.aliases?[name] ?? ""
}

// ===== assembly =====

//! Per-stage activations, filled when a synthesis is asked to trace (the oracle rail).
struct KittenTrace {
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
    frames : int64
}

def private albert_attention(a : KittenAlbert; h : array<float>; t : int64; var out : array<float>) {
    var @scratch q : array<float>
    var @scratch k : array<float>
    var @scratch v : array<float>
    var @scratch ctx : array<float>
    var @scratch p : array<float>
    linear_rows(a.q, h, t, q)
    linear_rows(a.k, h, t, k)
    linear_rows(a.v, h, t, v)
    ctx |> reserve_resize(t * a.hidden)
    p |> reserve_resize(t)
    let dh = a.hidden / a.heads
    let scale = 1.0 / sqrt(float(dh))
    for (hd in range64(a.heads)) {
        let off = hd * dh
        for (i in range64(t)) {
            for (j in range64(t)) {
                var s = 0.0
                for (e in range64(dh)) {
                    s += q[i * a.hidden + off + e] * k[j * a.hidden + off + e]
                }
                p[j] = s * scale
            }
            softmax(p, t)
            for (e in range64(dh)) {
                var acc = 0.0
                for (j in range64(t)) {
                    acc += p[j] * v[j * a.hidden + off + e]
                }
                ctx[i * a.hidden + off + e] = acc
            }
        }
    }
    linear_rows(a.dense, ctx, t, out)
}

//! The PL-BERT encoder: embeddings, then one ALBERT layer applied n_layers times; out [T][hidden].
def kitten_albert(a : KittenAlbert; ids : array<int>; @exact_size var out : array<float>) {
    let t = long_length(ids)
    if (t > a.max_pos) {
        panic("dasLLAMA kitten: {t} tokens exceed the encoder's {a.max_pos} positions")
    }
    var @exact_size e : array<float>
    e |> reserve_resize(t * a.emb_dim)
    for (i in range64(t)) {
        let id = int64(ids[i])
        for (d in range64(a.emb_dim)) {
            e[i * a.emb_dim + d] = a.word_emb[id * a.emb_dim + d] + a.pos_emb[i * a.emb_dim + d] + a.type_emb[d]
        }
    }
    layernorm_rows(e, t, a.emb_dim, a.emb_ln, ALBERT_EPS)
    var @exact_size h : array<float>
    linear_rows(a.map_in, e, t, h)
    var @scratch attn : array<float>
    var @scratch f : array<float>
    var @scratch f2 : array<float>
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
def kitten_text_encoder(te : KittenTextEncoder; ids : array<int>; @exact_size var out : array<float>) {
    let t = long_length(ids)
    let c = te.c
    var @exact_size xtm : array<float>
    xtm |> reserve_resize(t * c)
    for (i in range64(t)) {
        let id = int64(ids[i])
        for (d in range64(c)) {
            xtm[i * c + d] = te.emb[id * c + d]
        }
    }
    var @exact_size x : array<float>
    transpose_tc(xtm, t, c, x)
    var @exact_size y : array<float>
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
def kitten_duration_encoder(p : KittenPredictor; d_en : array<float>; t : int64; s : array<float>; @exact_size var out : array<float>) {
    let c = p.bert_encoder.nout
    var @exact_size x : array<float>
    var @exact_size y : array<float>
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
def kitten_durations(p : KittenPredictor; d : array<float>; t : int64; speed : float; @exact_size var dur : array<int>; @exact_size var raw : array<float>) : int64 {
    var @exact_size x : array<float>
    var @exact_size logits : array<float>
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

def private adain_res_blk(b : KittenAdainResBlk; x : array<float>; t : int64; s : array<float>; @exact_size var out : array<float>) : int64 {
    var @exact_size r : array<float>
    copy_floats_into(x, b.dim_in * t, r)
    adain(b.norm1, r, b.dim_in, t, s, LN_EPS)
    leaky_relu(r, b.dim_in * t, 0.2)
    var t1 = t
    if (b.upsample) {
        var @exact_size pooled : array<float>
        conv1d(b.pool, r, t, pooled)
        t1 = conv1d_out_len(b.pool, t)
        delete r
        r <- pooled
    }
    var @exact_size c1 : array<float>
    conv1d(b.conv1, r, t1, c1)
    adain(b.norm2, c1, b.dim_out, t1, s, LN_EPS)
    leaky_relu(c1, b.dim_out * t1, 0.2)
    conv1d(b.conv2, c1, t1, out)
    var @exact_size sc : array<float>
    if (b.upsample) {
        resize_nearest(x, b.dim_in, t, 2l, sc)
    } else {
        copy_floats_into(x, b.dim_in * t, sc)
    }
    if (b.learned_sc) {
        var @exact_size sc2 : array<float>
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

def private prosody_branch(blocks : array<KittenAdainResBlk>; proj : TtsConv1d; x : array<float>; frames : int64; s : array<float>; @exact_size var out : array<float>) : int64 {
    var @exact_size a : array<float>
    var @exact_size b : array<float>
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
def kitten_prosody(p : KittenPredictor; en : array<float>; c, frames : int64; s : array<float>;
                   @exact_size var f0 : array<float>; @exact_size var n : array<float>) : int64 {
    var @exact_size en_tm : array<float>
    var @exact_size x_tm : array<float>
    var @exact_size x : array<float>
    transpose_ct(en, c, frames, en_tm)
    bilstm(p.shared_lstm, en_tm, frames, x_tm)
    let width = 2l * p.shared_lstm.fwd.hidden
    transpose_tc(x_tm, frames, width, x)
    let f2 = prosody_branch(p.f0, p.f0_proj, x, frames, s, f0)
    prosody_branch(p.n, p.n_proj, x, frames, s, n)
    return f2
}

def private gen_res_block(b : KittenGenResBlock; var x : array<float>; t : int64; s : array<float>) {
    let n = b.c * t
    var @exact_size xt : array<float>
    var @exact_size y1 : array<float>
    var @exact_size y2 : array<float>
    for (j in range(length(b.convs1))) {
        copy_floats_into(x, n, xt)
        adain(b.adain1[j], xt, b.c, t, s, LN_EPS)
        snake(xt, b.c, t, b.alpha1[j])
        conv1d(b.convs1[j], xt, t, y1)
        adain(b.adain2[j], y1, b.c, t, s, LN_EPS)
        snake(y1, b.c, t, b.alpha2[j])
        conv1d(b.convs2[j], y1, t, y2)
        for (i in range64(n)) {
            x[i] += y2[i]
        }
    }
}

//! The harmonic source: sines plus noise from the F0 curve, mixed to one signal of
//! frames * upsample samples.
def kitten_source(d : KittenDecoder; cfg : SineSourceCfg; f0 : array<float>; f0_frames : int64;
                  noise : TtsNoise; @exact_size var har : array<float>) : int64 {
    var @exact_size sines : array<float>
    let n = sine_source(cfg, f0, f0_frames, noise.uniform, noise.normal, sines)
    linear_rows(d.source_linear, sines, n, har)
    tanh_inplace(har, n)
    return n
}

//! The source spectrum: the mixed signal edge-padded and analysed by the STFT convs into
//! [2 * bins][frames] (magnitude rows, then phase rows).
def kitten_har_spectrum(d : KittenDecoder; har : array<float>; n : int64; var tr : KittenTrace?;
                        @exact_size var har_spec : array<float>) : int64 {
    var @exact_size padded : array<float>
    pad_edge(har, n, STFT_PAD, padded)
    var @exact_size re : array<float>
    var @exact_size im : array<float>
    conv1d(d.stft_fwd_re, padded, n + 2l * STFT_PAD, re)
    conv1d(d.stft_fwd_im, padded, n + 2l * STFT_PAD, im)
    let frames = conv1d_out_len(d.stft_fwd_re, n + 2l * STFT_PAD)
    if (tr != null) {
        tr.stft_re := re
    }
    magnitude_phase(re, im, d.bins, frames, STFT_EPS, har_spec)
    return frames
}

def private generator_stage(d : KittenDecoder; i : int64; var x : array<float>; var cc, tt : int64&; s : array<float>;
                            har_spec : array<float>; har_frames : int64; var tr : KittenTrace?) {
    var @exact_size y : array<float>
    var @exact_size xs : array<float>
    var @exact_size src : array<float>
    leaky_relu(x, cc * tt, 0.1)
    conv1d(d.noise_convs[i], har_spec, har_frames, src)
    let src_t = conv1d_out_len(d.noise_convs[i], har_frames)
    gen_res_block(d.noise_res[i], src, src_t, s)
    conv1d(d.ups[i], x, tt, y)
    tt = conv1d_out_len(d.ups[i], tt)
    cc = d.ups[i].cout
    if (tr != null) {
        if (i == 0l) {
            tr.ups0 := y
        } else {
            tr.ups1 := y
        }
    }
    if (i == long_length(d.ups) - 1l) {
        pad_reflect_left(y, cc, tt, 1l, x)
        tt++
    } else {
        delete x
        x <- y
    }
    if (src_t != tt) {
        panic("dasLLAMA kitten: source branch {src_t} frames against {tt} at stage {i}")
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
def kitten_generator(d : KittenDecoder; x_in : array<float>; c, t : int64; s : array<float>;
                     har_spec : array<float>; har_frames : int64; var tr : KittenTrace?; @exact_size var wave : array<float>) : int64 {
    var @exact_size x : array<float>
    copy_floats_into(x_in, c * t, x)
    var cc = c
    var tt = t
    for (i in range64(long_length(d.ups))) {
        generator_stage(d, i, x, cc, tt, s, har_spec, har_frames, tr)
    }
    leaky_relu(x, cc * tt, 0.01)
    var @exact_size y : array<float>
    conv1d(d.conv_post, x, tt, y)
    if (tr != null) {
        tr.conv_post := y
    }
    var @exact_size re : array<float>
    var @exact_size im : array<float>
    polar_to_rect(y, d.bins, tt, re, im)
    var @exact_size wr : array<float>
    var @exact_size wi : array<float>
    conv1d(d.stft_bwd_re, re, tt, wr)
    conv1d(d.stft_bwd_im, im, tt, wi)
    let full = conv1d_out_len(d.stft_bwd_re, tt)
    let n = full - 2l * STFT_PAD
    wave |> reserve_resize(n)
    for (i in range64(n)) {
        wave[i] = wr[STFT_PAD + i] - wi[STFT_PAD + i]
    }
    return n
}

//! Draw the source noise the synthesis needs when none was captured.
def kitten_draw_noise(cfg : SineSourceCfg; frames : int64; seed : uint64; var noise : TtsNoise) {
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

def private kitten_decode(d : KittenDecoder; text_c : int64; source : SineSourceCfg; asr : array<float>; frames : int64;
                          f0, nrg : array<float>; f2 : int64; s_ref : array<float>; var noise : TtsNoise; seed : uint64;
                          var tr : KittenTrace?) : array<float> {
    var @exact_size f0d : array<float>
    var @exact_size nd : array<float>
    var @exact_size res : array<float>
    conv1d(d.f0_conv, f0, f2, f0d)
    conv1d(d.n_conv, nrg, f2, nd)
    conv1d(d.asr_res, asr, frames, res)
    if (tr != null) {
        tr.asr_res := res
    }
    var @exact_size x : array<float>
    var @exact_size y : array<float>
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
    kitten_draw_noise(source, f2, seed, noise)
    var @exact_size har : array<float>
    let n = kitten_source(d, source, f0, f2, noise, har)
    if (tr != null) {
        tr.har := har
    }
    var @exact_size har_spec : array<float>
    let har_frames = kitten_har_spectrum(d, har, n, tr, har_spec)
    var @exact_size wave : array<float>
    kitten_generator(d, y, cw, t, s_ref, har_spec, har_frames, tr, wave)
    return <- wave
}

//! Synthesize from token ids: the full StyleTTS2 pass, raw waveform (the reference driver's
//! tail trim is the facade's). `noise` supplies the source noise (empty = drawn from `seed`);
//! `tr` collects the stage tensors when not null.
def kitten_synthesize_ids(m : KittenModel; ids : array<int>; style : array<float>; speed : float;
                          var noise : TtsNoise; seed : uint64; var tr : KittenTrace?) : array<float> {
    let t = long_length(ids)
    let hs = m.style_dim / 2l
    var @exact_size s_ref : array<float>
    var @exact_size s_pro : array<float>
    s_ref |> reserve_resize(hs)
    s_pro |> reserve_resize(hs)
    for (i in range64(hs)) {
        s_ref[i] = style[i]
        s_pro[i] = style[hs + i]
    }
    var @exact_size bert : array<float>
    kitten_albert(m.albert, ids, bert)
    var @exact_size d_en : array<float>
    linear_rows(m.predictor.bert_encoder, bert, t, d_en)
    if (tr != null) {
        tr.bert := d_en
    }
    var @exact_size t_en : array<float>
    kitten_text_encoder(m.text_encoder, ids, t_en)
    if (tr != null) {
        tr.text_enc := t_en
    }
    var @exact_size d : array<float>
    kitten_duration_encoder(m.predictor, d_en, t, s_pro, d)
    let dc = m.predictor.bert_encoder.nout + hs
    var @exact_size dur : array<int>
    var @exact_size raw : array<float>
    let frames = kitten_durations(m.predictor, d, t, speed, dur, raw)
    if (tr != null) {
        tr.dur_raw := raw
        tr.frames = frames
    }
    var @exact_size en : array<float>
    var @exact_size asr : array<float>
    align_expand_rows(d, t, dc, dur, en)
    align_expand_channels(t_en, m.text_encoder.c, t, dur, asr)
    var @exact_size f0 : array<float>
    var @exact_size nrg : array<float>
    let f2 = kitten_prosody(m.predictor, en, dc, frames, s_pro, f0, nrg)
    if (tr != null) {
        tr.f0 := f0
        tr.n := nrg
    }
    return <- kitten_decode(m.decoder, m.text_encoder.c, m.source, asr, frames, f0, nrg, f2, s_ref, noise, seed, tr)
}

//! The reference driver's tail trim: a synthesis ends with 5000 samples of silence.
def kitten_trim(var wave : array<float>) {
    let n = long_length(wave)
    if (n > TRIM_TAIL) {
        wave |> resize(n - TRIM_TAIL)
    }
}
