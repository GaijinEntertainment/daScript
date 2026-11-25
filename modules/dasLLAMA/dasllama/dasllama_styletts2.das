options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_styletts2 shared public

require dasllama/dasllama_lint public
require daslib/array_boost   // temp_array: the writer's view over the source's bytes
require math
require strings
require daslib/strings_boost   // split: a recipe pin's pairs
require daslib/enum_trait      // enum_to_table: a recipe pin's format names
require daslib/defer
require daslib/fio
require daslib/archive
require dasllama/dasllama_env
require dasllama/dasllama_gguf
require dasllama/dasllama_common
require dasllama/dasllama_math
require dasllama/dasllama_par
require dasllama/dasllama_plane
require dasllama/dasllama_image
require dasllama/dasllama_load          // kq_fmt_of: a stored tensor's format, tagged from its disk type
require dasllama/dasllama_layout        // q8_region_to_metal_blob: the Q8_0 block writer a stored file's capture shares
require dasllama/dasllama_gemm_schema   // Q8_BLOCK_ELEMS / Q8_BLOCK_BYTES: the Q8_0 block's size
require dasllama/dasllama_tower_flavor   // [tower_flavor]: the family's GPU flavor seat
require dasllama/dasllama_tower_image    // tower_dev_flavor
require dasllama/dasllama_tower   // asr_prof_add: the encode-stage profiling rail; add_inplace_rows: the residual add
require dasllama/dasllama_gpu_math   // sigmoid_f32
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
    dim_in_s : int64   // the served input width: dim_in, or padded to 32 for the q8 lane
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
    voices_q : table<string; array<int8>>    //! a voice pack minted Q8_0: its quants, in place of its `voices` entry
    voices_qs : table<string; array<float>>  //! ... and their scales, one per 32
    voice_q_spans : table<string; TtsSpan>
    voice_qs_spans : table<string; TtsSpan>
    albert : St2Albert = St2Albert()
    text_encoder : St2TextEncoder = St2TextEncoder()
    predictor : St2Predictor = St2Predictor()
    decoder : St2Decoder = St2Decoder()
    source : SineSourceCfg = SineSourceCfg()
    q8 : bool               // the lane this image was minted on: the rows GEMMs serve Q8_0 quants
    kq : bool               //! ... on the recipe lane: each weight group in the format `st2_recipe_default` names
    dev_backend : TowerDev  //! the GPU home this image was staged for: its K-quant planes sit in disk order, which that home's seats stage as stored; `none` on a CPU image
    host_weights : bool     //! a cell's mint (`set_tts_host_weights_`): the weights its device block replaces are held too
    dev_layout : uint64     //! the layout of the device block this image carries (`set_styletts2_block_layout`); 0 on a CPU image
    kitten : KittenFamily
    kokoro : KokoroFamily
    blob : PlaneF = PlaneF()
    qblob : PlaneI8 = PlaneI8()
    dev : PlaneU8 = PlaneU8()         //! a GPU image's device block (`#tower-dev-block`): the pieces its home's seats bind ...
    devplan : PlaneI64 = PlaneI64()   //! ... and the plan naming them; empty on a CPU image
    image_map : void?
    image_bytes : uint64
}

def weights_walk(var io : TtsBlobIo; var m : StyleTts2Model) {
    weights_walk(io, m.albert)
    weights_walk(io, m.text_encoder)
    weights_walk(io, m.predictor)
    weights_walk(io, m.decoder)
    for (name in m.voice_names) {
        table_slot(io, m.voices, m.voice_spans, name)
        table_slot(io, m.voices_q, m.voice_q_spans, name)
        table_slot(io, m.voices_qs, m.voice_qs_spans, name)
    }
}

// serialize_image_meta covers 19 fields; the three voice tables (views bound from their spans), image_map and
// image_bytes are the deliberate skips
let private ST2_META_FIELDS = 21 + 6
let ST2_IMAGE_PREFIX = "tts-"   //!< every lane tag of the family starts with it
let ST2_IMAGE_TAG = "tts-f32"
let ST2_IMAGE_TAG_Q8 = "tts-q8"
let ST2_IMAGE_TAG_KQ = "tts-kq"   //!< the recipe lane, served where a load asks for it

//! A weight group's format on the recipe lane.
enum St2Fmt {
    f32
    q8
    k4
    k5
    k6
}

//! The weight groups a recipe names, each a set of tensors by name (`st2_group`).
let ST2_GROUPS = fixed_array<string>("bert", "predictor", "fc", "lstm", "text_encoder", "decoder", "generator", "ups", "noise", "post", "voice")

//! The recipe lane's format a weight group, each one the rig's pick: the decoder stack, the generator's res blocks and the
//! text encoder's convs take four bits, the upsamplers six; the noise convs and conv_post read and write the audio itself
//! and stay f32; every other group is Q8_0 where its kernel has the form.
[arch(at = "../ARCHITECTURE_TTS_FORMATS.md#tts-recipe")]
def st2_recipe_default(group : string) : St2Fmt {
    return St2Fmt.k4 if (group == "decoder" || group == "generator" || group == "text_encoder")
    return St2Fmt.k6 if (group == "ups")
    return St2Fmt.f32 if (group == "noise" || group == "post")
    return St2Fmt.q8
}

//! the weight group tensor `name` (or its prefix) belongs to
def st2_group(name : string) : string {
    return "fc" if (name |> ends_with(".fc") || name |> ends_with(".fc.weight"))
    return "voice" if (name |> starts_with("voice."))
    return "lstm" if (name == "lstm")
    return "bert" if (name |> starts_with("bert"))
    return "predictor" if (name |> starts_with("predictor."))
    return "text_encoder" if (name |> starts_with("text_encoder."))
    return "ups" if (name |> starts_with("decoder.generator.ups."))
    return "noise" if (name |> starts_with("decoder.generator.noise_convs."))
    return "post" if (name |> starts_with("decoder.generator.conv_post"))
    return "generator" if (name |> starts_with("decoder.generator."))
    return "decoder"
}

//! a recipe group of the decoder, whose convs the GPU seats run on their f16 tiles; the front end's stay exact
def private st2_decoder_group(g : string) : bool => g == "decoder" || g == "generator" || g == "ups" || g == "noise" || g == "post"

def private st2_kq(f : St2Fmt) : KqFmt => f == St2Fmt.k4 ? KqFmt.k4 : (f == St2Fmt.k5 ? KqFmt.k5 : KqFmt.k6)
def private st2_is_kq(f : St2Fmt) : bool => f == St2Fmt.k4 || f == St2Fmt.k5 || f == St2Fmt.k6

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
    arch |> serialize(m.voice_q_spans)
    arch |> serialize(m.voice_qs_spans)
    arch |> serialize(m.albert)
    arch |> serialize(m.text_encoder)
    arch |> serialize(m.predictor)
    arch |> serialize(m.decoder)
    arch |> serialize(m.source)
    arch |> serialize_raw(m.q8)
    arch |> serialize_raw(m.kq)
    arch |> serialize_raw(m.dev_backend)
    arch |> serialize_raw(m.host_weights)
    arch |> serialize_raw(m.dev_layout)
    // the family records are hand-listed too, so each pins its count - an unwritten field reads back zero
    verify(count_meta_fields(m.kitten) == 2)   // grew KittenFamily? extend this list
    verify(count_meta_fields(m.kokoro) == 2)   // grew KokoroFamily? extend this list
    arch |> serialize(m.kitten.speed_priors)
    arch |> serialize(m.kitten.aliases)
    arch |> serialize(m.kokoro.sym)
    arch |> serialize(m.kokoro.voice_langs)
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

var private g_st2_q8_pin = GemmLane.unset
var private g_st2_recipe_asked = false   // set_styletts2_recipe is in force: the next load serves the recipe lane
var private g_st2_recipe_spec = ""       // the group pins it took, as given
var private g_st2_recipe_pin : table<string; St2Fmt>

def private st2_serve_kq() : bool => g_st2_recipe_asked
def private st2_serve_q8() : bool => g_st2_recipe_asked || lane_serves_q8(g_st2_q8_pin, true)   // the rig held q8 at f32 quality, so f32 is the reference lane, not the served one

//! Pin the rows GEMMs' format for subsequent loads: q8 quants vs the file's own f32 planes.
//! Each lane is its own prepared image. ``reset_styletts2_q8`` returns to the policy default, the q8 lane.
//! The facade spells these ``set_tts_q8`` / ``reset_tts_q8`` / ``tts_serves_q8``.
def set_styletts2_q8(on : bool) {
    g_st2_q8_pin = lane_pin(on)
}

def reset_styletts2_q8() {
    g_st2_q8_pin = GemmLane.unset
}

//! Would the next load serve the rows GEMMs as q8 - the pin when set, the policy otherwise.
def styletts2_serves_q8() : bool => st2_serve_q8()

//! Whether ``set_styletts2_q8`` holds a pin ``reset_styletts2_q8`` has not dropped.
def styletts2_q8_pinned() : bool => g_st2_q8_pin != GemmLane.unset

//! Would the next load serve the recipe lane - true while ``set_styletts2_recipe`` is in force.
def styletts2_serves_recipe() : bool => st2_serve_kq()

//! The group pins ``set_styletts2_recipe`` took, as given: "" with no group pinned, and with the recipe lane not asked.
def styletts2_recipe() : string => g_st2_recipe_spec

def private st2_tag_of(q8, kq : bool) : string => kq ? ST2_IMAGE_TAG_KQ : (q8 ? ST2_IMAGE_TAG_Q8 : ST2_IMAGE_TAG)

//! The family's GPU flavor seat (`ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block`): the registered driver's backend, whether
//! its seats serve the recipe lane (`serves`, the lane's kq flag), and its bake
[tower_flavor(family = "styletts2", model = "StyleTts2Model")]
struct St2Flavor {}

//! the flavor a load takes on the lane (`kq`: the recipe lane): the driver's where its seats serve it, else `none`, the CPU image
def styletts2_dev_flavor(kq : bool) : TowerDev => tower_dev_flavor(g_styletts2_flavor, kq)

//! `stage_styletts2` under the GPU flavor this lane's load takes (`styletts2_dev_flavor`): the seats find the K-quant planes as stored
def stage_styletts2_served(path : string; q8 : bool = false; kq : bool = false) : St2Staging {
    return <- tower_dev_staged(styletts2_dev_flavor(kq)) {
        return <- stage_styletts2(path, q8, kq)
    }
}

// the lane changes what a load mints and serves, so the load says which and why - and names a pin it does not apply:
// a prepared image serves the lane it was baked on, a file that stores its weights the lane it names, and the recipe
// ask overrides the q8 pin
def private st2_announce_lane(dlim : bool; stored : string) {
    let pinned = g_st2_q8_pin != GemmLane.unset
    if (dlim || !empty(stored)) {
        if (!empty(stored)) {
            to_log(LOG_INFO, "dasLLAMA styletts2: rows GEMM lane {stored} - the lane the file stores its weights on\n")
        }
        if (pinned || g_st2_recipe_asked) {
            let which = g_st2_recipe_asked ? "set_styletts2_recipe" : "set_tts_q8"
            let what = dlim ? "a prepared image serves the lane it was baked on" : "a file that stores its weights as minted serves the {stored} lane it names"
            to_log(LOG_WARNING, "dasLLAMA styletts2: {what} - the {which} pin is not applied to this load\n")
        }
        return
    }
    if (g_st2_recipe_asked) {
        let groups = empty(g_st2_recipe_spec) ? "" : " with {g_st2_recipe_spec}, minted in process"
        let over = pinned ? ", over the set_tts_q8 pin" : ""
        to_log(LOG_INFO, "dasLLAMA styletts2: rows GEMM lane the recipe{groups} - asked via set_styletts2_recipe{over}\n")
        return
    }
    let why = pinned ? "pinned via set_tts_q8" : "the policy default"
    to_log(LOG_INFO, "dasLLAMA styletts2: rows GEMM lane {st2_serve_q8() ? "q8" : "f32 planes"} - {why}\n")
}

// what the readers mint while a stage runs: the lane's quant choice and whether the box's
// backend wants the quants repacked
var private g_stage_q8 = false
var private g_stage_repack = false
var private g_stage_kq = false
var private g_stage_stored = false   // the file declares the stored layout: a quantized tensor is taken, a float one stays f32
var private g_stage_capture = false  // the writer's stage: every weight a site mints is recorded as its disk blocks

//! The stored layout a file's `styletts2.stored_layout` names: a weight the lane mints is its quant matrix, as minted.
let ST2_STORED_LAYOUT = 1l

// a weight the writer stores as minted: its tensor's GGUF type and dims, its blocks, and a conv's [cout, cin, source cin]
struct private St2Stored {
    name : string
    gtype : int
    dims : array<uint64>
    @exact_size bytes : array<uint8>
    conv : array<int>
}

var private g_captured : array<St2Stored>
var private g_captured_at : table<string; int>

// the capture slot of tensor `name`, made with `gtype` and `dims` at its first call: an LSTM's two directions land in one
def private captured_slot(name : string; gtype : int; dims : array<int64>) : int {
    let had = g_captured_at?[name] ?? -1
    return had if (had >= 0)
    var slot = St2Stored(name = name, gtype = gtype)
    slot.dims <- [for (d in dims); uint64(d)]
    g_captured |> emplace(slot)
    g_captured_at[name] = length(g_captured) - 1
    return length(g_captured) - 1
}

// answers the capture's slot, -1 outside the writer's stage
def private capture_q8(name : string; wq : array<int8>; wqs : array<float>; n : int64; dims : array<int64>) : int {
    return -1 if (!g_stage_capture)
    let at = captured_slot(name, GGML_TYPE_Q8_0, dims)
    var bytes & = unsafe(g_captured[at].bytes)
    let from = long_length(bytes)
    bytes |> reserve(from + n / Q8_BLOCK_ELEMS * Q8_BLOCK_BYTES)
    bytes |> resize_no_init(from + n / Q8_BLOCK_ELEMS * Q8_BLOCK_BYTES)
    unsafe {   // disk order is the unrepacked region's: the block writer the Metal blob takes, its scale clamped into a half
        q8_region_to_metal_blob(addr<int8 const?>(wq[0]), addr<float const?>(wqs[0]), dims[0], n / dims[0], false, 1l, Q8_BLOCK_ELEMS, 0l, addr(bytes[from]))
    }
    return at
}

// a conv's minted quant matrix: [K][rows] in its format, with the channel counts its K-quant width hides and `cin_src`, the
// input width before the load's pad
def private capture_conv(wname : string; c : TtsConv1d; cin_src : int64) {
    return if (!g_stage_capture || (!c.q8 && c.kq_fmt == 0))
    var at = -1
    if (c.q8) {
        at = capture_q8(wname, c.wq, c.wqs, long_length(c.wq), [c.q8_k, long_length(c.wq) / c.q8_k])
    } else {
        let fmt = kq_fmt_of_id(c.kq_fmt, "a tts conv")
        let n = long_length(c.kq) / kq_qsb(fmt) * KQ_SB_ELEMS
        at = captured_slot(wname, kq_ggml_type(fmt), [c.kq_k, n / c.kq_k])
        kq_planes_to_disk(fmt, c.kq, c.ks, n, g_captured[at].bytes)
    }
    g_captured[at].conv <- [int(c.cout), int(c.cin), int(cin_src)]
}

// Whether the file stores tensor `name` as a quant matrix, `fmt` its format off the disk type: false where it holds floats.
// A stored format is Q8_0 or a K-quant the recipe mints, in a file that declares the stored layout; anything else is refused.
def private stored_quant(m : GGUFMeta; name : string; var fmt : KqFmt&) : bool {
    let gt = gguf_tensor_type(m, name)
    return false if (gt < 0 || gt == GGML_TYPE_F32 || gt == GGML_TYPE_F16)
    fmt = kq_fmt_of(gt)
    let named = fmt != KqFmt.q8 || gt == GGML_TYPE_Q8_0   // the mapping tags a type it does not know q8
    if (!named || !(fmt == KqFmt.q8 || fmt == KqFmt.k4 || fmt == KqFmt.k5 || fmt == KqFmt.k6)) {
        let what = named ? "{fmt}" : "ggml type {gt}"
        panic("dasLLAMA styletts2: '{name}' is stored as {what}; a StyleTTS2 file stores floats, q8, or the recipe's k4, k5 and k6")
    }
    if (!g_stage_stored) {
        panic("dasLLAMA styletts2: '{name}' is stored quantized in a file that declares no styletts2.stored_layout - the matrix it holds is unknown")
    }
    return true
}

//! Ask the recipe lane for subsequent loads, in process, over any ``set_styletts2_q8`` pin: `spec` "" is the recipe as it
//! is, `group=format` pairs (comma-separated; `ST2_GROUPS`, `St2Fmt`) pin those groups and mint in memory. A spec that does
//! not parse panics and leaves the pin as it was; ``reset_styletts2_recipe`` returns to the policy lane.
def set_styletts2_recipe(spec : string) {
    var inscope pin : table<string; St2Fmt>
    var inscope formats <- enum_to_table(type<St2Fmt>)
    var inscope pairs <- empty(strip(spec)) ? array<string>() : split(spec, ",")
    for (pair in pairs) {
        var inscope kv <- split(strip(pair), "=")
        let group = length(kv) == 2 ? strip(kv[0]) : ""
        if (!has_value(ST2_GROUPS, group)) {
            panic("dasLLAMA styletts2: recipe pin '{pair}' is not group=format over the groups {ST2_GROUPS}")
        }
        let fmt = strip(kv[1])
        if (!key_exists(formats, fmt)) {
            panic("dasLLAMA styletts2: recipe pin '{pair}' names no format of {join(typeinfo enum_names(type<St2Fmt>), ", ")}")
        }
        pin[group] = formats?[fmt] ?? St2Fmt.f32
    }
    delete g_st2_recipe_pin
    g_st2_recipe_pin <- pin
    g_st2_recipe_spec = clone_string(spec)
    g_st2_recipe_asked = true
}

def reset_styletts2_recipe() {
    delete g_st2_recipe_pin
    g_st2_recipe_spec = ""
    g_st2_recipe_asked = false
}

// the format the staged tensor `name` mints in: its group's on the recipe lane - the pin's, else the recipe's; the q8 lane is the
// recipe with every K-quant group at Q8_0
def private stage_fmt(name : string) : St2Fmt {
    let group = st2_group(name)
    if (!g_stage_kq) {
        let fmt = st2_recipe_default(group)
        return st2_is_kq(fmt) ? St2Fmt.q8 : fmt
    }
    return g_st2_recipe_pin?[group] ?? st2_recipe_default(group)
}

// what a stage minted against what its recipe asked, a group: the mint's own report
struct private St2Minted {
    asked : St2Fmt
    leaves : int
    as_asked : int
}

var private g_stage_minted : table<string; St2Minted>

def private stage_note(name : string; asked : St2Fmt; honored : bool) {
    return if (!g_stage_kq)
    let group = st2_group(name)
    var note = g_stage_minted?[group] ?? St2Minted(asked = asked)
    note.leaves++
    note.as_asked += honored ? 1 : 0
    g_stage_minted[group] = note
}

def private stage_report() {
    for (group in ST2_GROUPS) {
        g_stage_minted |> get(group) $(note) {
            to_log(LOG_INFO, "dasLLAMA styletts2: recipe group {group} asked {note.asked}: {note.as_asked} of {note.leaves} weights minted in it\n")
        }
    }
    delete g_stage_minted
}

def finalize(var m : StyleTts2Model) {
    for (pack in values(m.voices)) {
        release_plane(pack)
    }
    for (pack in values(m.voices_q)) {
        release_plane(pack)
    }
    for (pack in values(m.voices_qs)) {
        release_plane(pack)
    }
    delete m.voices
    delete m.voices_q
    delete m.voices_qs
    delete m.voice_q_spans
    delete m.voice_qs_spans
    delete m.voice_spans
    delete m.voice_names
    delete m.albert
    delete m.text_encoder
    delete m.predictor
    delete m.decoder
    delete m.kitten
    delete m.kokoro
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
    @exact_size dev : array<uint8>
    @exact_size devplan : array<int64>
}

def private serialize_image_meta(var arch : Archive; var st : St2Staging) {
    serialize_image_meta(arch, st.e)
}

[unused_argument(st)]
def private image_post_load(var st : St2Staging) {}

def finalize(var st : St2Staging) {
    delete_staging(st)
}

def private styletts2_layout_fingerprint() : uint64 {
    var proto = StyleTts2Model()
    let layout = layout_fingerprint(proto)
    delete proto
    return layout
}

var private g_st2_block_layout = 0ul

//! the registered driver's block layout: a GPU image records it (`dev_layout`), and a load re-mints an image another layout
//! baked; answers the layout it replaced
def set_styletts2_block_layout(layout : uint64) : uint64 {
    let was = g_st2_block_layout
    g_st2_block_layout = layout
    return was
}

[init]
def private register_styletts2_image_tags {
    register_image_family_tag(ST2_IMAGE_TAG, true, @@styletts2_layout_fingerprint)   // no box property shapes the f32 blob
    register_image_family_tag(ST2_IMAGE_TAG_Q8, false, @@styletts2_layout_fingerprint)   // the q8 quants follow the backend's repack
    register_image_family_tag(ST2_IMAGE_TAG_KQ, false, @@styletts2_layout_fingerprint)   // and so do the K-quant planes
    for (dev in [TowerDev.metal, TowerDev.vulkan, TowerDev.cuda]) {   // a flavor's Q8_0 planes are staged in the CPU backend's layout: config-bound
        register_image_flavor_tag(tower_dev_tag(ST2_IMAGE_TAG, dev), ST2_IMAGE_TAG)
        register_image_flavor_tag(tower_dev_tag(ST2_IMAGE_TAG_Q8, dev), ST2_IMAGE_TAG_Q8, false)
        register_image_flavor_tag(tower_dev_tag(ST2_IMAGE_TAG_KQ, dev), ST2_IMAGE_TAG_KQ, false)
    }
}

let ST2_ADAIN_EPS = 1e-5   //!< the instance norms' eps, the generator driver's twin reads it
let ST2_ALBERT_EPS = 1e-12   //!< the PL-BERT norms' eps
let ST2_STFT_PAD = 10l       //!< the STFT's centre pad in samples
let private LN_EPS = ST2_ADAIN_EPS
let private ALBERT_EPS = ST2_ALBERT_EPS
let private STFT_PAD = ST2_STFT_PAD

// ===== loading =====

def private need_tensor(m : GGUFMeta; name : string) : int => gguf_need_tensor(m, name, "styletts2")

def private read_arr(m : GGUFMeta; bytes : array<uint8> | #; name : string) : array<float> =>
    <- gguf_read_f32_arr(m, bytes, name, "styletts2")

[arch(at = "../ARCHITECTURE_TTS.md#tts-padded-input-width")]
def private read_conv(m : GGUFMeta; bytes : array<uint8> | #; wname, bname : string; rows : bool = false; cin_s : int64 = 0l) : TtsConv1d {
    var c = TtsConv1d()
    var stored_as = KqFmt.q8
    let stored = stored_quant(m, wname, stored_as)
    if (!stored) {
        c.w <- read_arr(m, bytes, wname)
    }
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
    c.served_rows = rows
    if (stored) {   // the quant matrix as the file stores it: its width hides the channel counts, so the file names them
        var inscope chans <- stored_conv_chans(m, bytes, wname)
        if (cin_s > 0l && chans[1] != cin_s) {
            panic("dasLLAMA styletts2: '{wname}' is stored with {chans[1]} input channels where its block reads {cin_s}")
        }
        c.cout = chans[0]
        c.cin = chans[1]
        conv1d_read_quant(c, m, bytes, wname, stored_as, g_stage_repack)
        return <- c
    }
    let ti = need_tensor(m, wname)
    let cc = conv1d_dims_of(m.tensors[ti].dims, c.transposed, c.groups)
    c.cin = cc.cin
    c.cout = cc.cout
    if (cin_s > 0l) {
        conv1d_pad_cin(c, cin_s)
    }
    let fmt = stage_fmt(wname)
    let quant_cm = st2_group(wname) == "text_encoder"   //! the one group whose channel-major convs take a quant matrix: the others' read a curve or the signal
    if (st2_is_kq(fmt) && (rows || quant_cm) && conv1d_kq_eligible(c)) {
        conv1d_prepare_kq(c, st2_kq(fmt), g_stage_repack)
    } else {
        let quant = g_stage_q8 && fmt != St2Fmt.f32
        // a GPU home's image holds an unquantized decoder rows conv as the halves its f16 tiles read
        let halves = rows && !quant && !g_stage_capture && tower_dev_halfword(tower_staging_dev()) && st2_decoder_group(st2_group(wname))
        conv1d_prepare(c, quant, g_stage_repack, quant_cm, halves)
    }
    stage_note(wname, fmt, st2_is_kq(fmt) ? c.kq_fmt != 0 : (c.q8 == (fmt == St2Fmt.q8)))
    capture_conv(wname, c, cc.cin)
    return <- c
}

def private read_conv_p(m : GGUFMeta; bytes : array<uint8> | #; prefix : string; rows : bool = false; cin_s : int64 = 0l) : TtsConv1d =>
    <- read_conv(m, bytes, "{prefix}.weight", "{prefix}.bias", rows, cin_s)

// a stored conv's [cout, cin, source cin]: its quant matrix's width hides the channel counts, so the file names them
def private stored_conv_chans(m : GGUFMeta; bytes : array<uint8> | #; wname : string) : array<int64> {
    let key = "styletts2.stored_conv.{wname}"
    var chans <- gguf_has(m, key) ? gguf_int_array(m, bytes, key) : array<int64>()
    if (length(chans) != 3) {
        panic("dasLLAMA styletts2: '{wname}' is stored quantized and {key} gives {chans} where it names [cout, cin, source cin]")
    }
    return <- chans
}

// the input width a conv will read, from its tensor dims and geometry - known before the read
def private conv_cin_of(m : GGUFMeta; bytes : array<uint8> | #; wname : string) : int64 {
    var stored_as = KqFmt.q8
    if (stored_quant(m, wname, stored_as)) {   // a stored quant matrix's dims are its own: the file names the source width
        var inscope chans <- stored_conv_chans(m, bytes, wname)
        return chans[2]
    }
    var inscope geo <- gguf_int_array(m, bytes, "styletts2.conv.{wname}")
    let ti = need_tensor(m, wname)
    return conv1d_dims_of(m.tensors[ti].dims, geo[6] != 0l, geo[5]).cin
}

def private read_linear(m : GGUFMeta; bytes : array<uint8> | #; prefix : string; rows_only : bool = false;
                        vec_only : bool = false) : TtsLinear {
    var l = TtsLinear()
    let wname = "{prefix}.weight"
    let ti = need_tensor(m, wname)
    l.nin = m.tensors[ti].dims[0]
    l.nout = m.tensors[ti].dims[1]
    if (gguf_find_tensor(m, "{prefix}.bias") >= 0) {
        l.b <- read_arr(m, bytes, "{prefix}.bias")
    }
    var stored_as = KqFmt.q8
    if (stored_quant(m, wname, stored_as)) {
        if (stored_as != KqFmt.q8) {
            panic("dasLLAMA styletts2: '{wname}' is stored {stored_as}; a dense layer's stored form is Q8_0")
        }
        q8_matrix_read(m, bytes, wname, l.nin, l.nout, l.wq, l.wqs, g_stage_repack)
        l.q8 = true
        return <- l
    }
    l.w <- read_arr(m, bytes, wname)
    let fmt = stage_fmt(wname)
    linear_prepare(l, rows_only, g_stage_q8 && fmt != St2Fmt.f32, g_stage_repack, vec_only)
    if (!vec_only) {
        stage_note(wname, fmt, l.q8 == (fmt == St2Fmt.q8))
        if (l.q8) {
            capture_q8(wname, l.wq, l.wqs, l.nin * l.nout, [l.nin, l.nout])
        }
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
    let ih = "{prefix}.weight_ih"
    let hh = "{prefix}.weight_hh"
    let ti = need_tensor(m, ih)
    l.nin = m.tensors[ti].dims[0]
    l.hidden = m.tensors[ti].dims[1] / 4l
    let g4 = 4l * l.hidden
    l.b |> reserve_resize(g4)
    gguf_read_tensor_f32(m, bytes, "{prefix}.bias", l.b, 0l, g4, dir * g4)
    var ih_as = KqFmt.q8
    var hh_as = KqFmt.q8
    let ih_stored = stored_quant(m, ih, ih_as)
    let hh_stored = stored_quant(m, hh, hh_as)
    if (ih_stored || hh_stored) {   // both directions' rows ride one tensor, a direction's at dir * 4 hidden
        if (!(ih_stored && hh_stored) || ih_as != KqFmt.q8 || hh_as != KqFmt.q8) {
            let ih_what = ih_stored ? "{ih_as}" : "floats"
            let hh_what = hh_stored ? "{hh_as}" : "floats"
            panic("dasLLAMA styletts2: '{prefix}' stores {ih_what} and {hh_what}; an LSTM's stored form is both its matrices as Q8_0")
        }
        q8_matrix_read(m, bytes, ih, l.nin, g4, l.wq_ih, l.wqs_ih, g_stage_repack, dir * g4 * l.nin)
        q8_matrix_read(m, bytes, hh, l.hidden, g4, l.wq_hh, l.wqs_hh, g_stage_repack, dir * g4 * l.hidden)
        l.q8 = true
        return <- l
    }
    l.w_ih |> reserve_resize(g4 * l.nin)
    l.w_hh |> reserve_resize(g4 * l.hidden)
    gguf_read_tensor_f32(m, bytes, ih, l.w_ih, 0l, g4 * l.nin, dir * g4 * l.nin)
    gguf_read_tensor_f32(m, bytes, hh, l.w_hh, 0l, g4 * l.hidden, dir * g4 * l.hidden)
    let fmt = stage_fmt("lstm")
    if (g_stage_q8 && fmt == St2Fmt.q8) {
        lstm_prepare_q8(l, g_stage_repack)
    }
    stage_note("lstm", fmt, l.q8 == (fmt == St2Fmt.q8))
    if (l.q8) {
        capture_q8(ih, l.wq_ih, l.wqs_ih, g4 * l.nin, m.tensors[ti].dims)
        capture_q8(hh, l.wq_hh, l.wqs_hh, g4 * l.hidden, m.tensors[need_tensor(m, hh)].dims)
    }
    return <- l
}

def private read_bilstm(m : GGUFMeta; bytes : array<uint8> | #; prefix : string) : TtsBiLstm {
    var l = TtsBiLstm()
    l.fwd <- read_lstm_dir(m, bytes, prefix, 0l)
    l.bwd <- read_lstm_dir(m, bytes, prefix, 1l)
    return <- l
}

// a style projection on a quantized lane: Q8_0 where its widths allow and its group asks no f32, after any re-lay of its rows
def private stage_style_fc(var l : TtsLinear; wname : string) {
    let fmt = stage_fmt(".fc")
    if (g_stage_q8 && fmt != St2Fmt.f32 && linear_vec_prepare_q8(l, g_stage_repack)) {
        capture_q8(wname, l.wq, l.wqs, l.nin * l.nout, [l.nin, l.nout])
    }
    stage_note(".fc", fmt, l.q8 == (fmt != St2Fmt.f32))
}

//! an AdaIN with its style projection; `cs` past `c` re-lays the affine's two halves at the padded width first
def private read_adain(m : GGUFMeta; bytes : array<uint8> | #; prefix : string; c : int64 = 0l; cs : int64 = 0l) : TtsAdaIn {
    var a = TtsAdaIn()
    a.fc <- read_linear(m, bytes, "{prefix}.fc", vec_only = true)
    a.norm <- read_norm(m, bytes, "{prefix}.norm.weight", "{prefix}.norm.bias")
    adain_pad(a, c, cs)
    stage_style_fc(a.fc, "{prefix}.fc.weight")
    return <- a
}

def private read_adaln(m : GGUFMeta; bytes : array<uint8> | #; prefix : string) : TtsAdaLayerNorm {
    var a = TtsAdaLayerNorm()
    a.fc <- read_linear(m, bytes, "{prefix}.fc", vec_only = true)
    stage_style_fc(a.fc, "{prefix}.fc.weight")
    return <- a
}

// The block runs on token-major rows, so its width must sit on the 16-column tile the rows
// convs serve at - the row stride of every buffer in the block is the width itself. `pad_in`
// pads an input width off 32 (the decoder's concat rows) so its convs reach the q8 lane.
def private read_adain_res_blk(m : GGUFMeta; bytes : array<uint8> | #; prefix : string; pad_in : bool = false) : St2AdainResBlk {
    var b = St2AdainResBlk()
    b.dim_in = conv_cin_of(m, bytes, "{prefix}.conv1.weight")
    b.learned_sc = gguf_find_tensor(m, "{prefix}.conv1x1.weight") >= 0
    b.upsample = gguf_find_tensor(m, "{prefix}.pool.weight") >= 0
    let pad = pad_in && b.learned_sc && b.dim_in % 32l != 0l
    b.dim_in_s = pad ? round_up(b.dim_in, 32l) : b.dim_in
    let cin_s = pad ? b.dim_in_s : 0l
    b.norm1 <- read_adain(m, bytes, "{prefix}.norm1", b.dim_in, b.dim_in_s)
    b.norm2 <- read_adain(m, bytes, "{prefix}.norm2")
    b.conv1 <- read_conv_p(m, bytes, "{prefix}.conv1", true, cin_s)
    b.conv2 <- read_conv_p(m, bytes, "{prefix}.conv2", true)
    b.dim_out = b.conv1.cout
    if (b.dim_out % 16l != 0l) {
        panic("dasLLAMA styletts2: {prefix} is {b.dim_out} wide; a residual block's width sits on the 16-column tile")
    }
    if (b.learned_sc) {
        b.conv1x1 <- read_conv_p(m, bytes, "{prefix}.conv1x1", true, cin_s)
    }
    if (b.upsample) {
        b.pool <- read_conv_p(m, bytes, "{prefix}.pool", false, cin_s)
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
    d.encode <- read_adain_res_blk(m, bytes, "decoder.encode", true)
    var i = 0
    while (gguf_find_tensor(m, "decoder.decode.{i}.conv1.weight") >= 0) {
        d.decode |> emplace(read_adain_res_blk(m, bytes, "decoder.decode.{i}", true))
        i++
    }
    d.f0_conv <- read_conv_p(m, bytes, "decoder.F0_conv")
    d.n_conv <- read_conv_p(m, bytes, "decoder.N_conv")
    d.asr_res <- read_conv_p(m, bytes, "decoder.asr_res.0", true)
    i = 0
    while (gguf_find_tensor(m, "decoder.generator.ups.{i}.weight") >= 0) {
        d.ups |> emplace(read_conv_p(m, bytes, "decoder.generator.ups.{i}", true))
        d.noise_convs |> emplace(read_conv_p(m, bytes, "decoder.generator.noise_convs.{i}", true))
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

def private read_weight(m : GGUFMeta; bytes : array<uint8> | #; name : string) : TtsWeight => TtsWeight(a <- read_arr(m, bytes, name))

//! Read a converted StyleTTS2-lineage GGUF (harness/convert_kitten.py, harness/convert_kokoro.py)
//! into the mint-side carrier: served layouts minted, voices, STFT convention and the family's
//! driver keys read, every weight moved into the staging blob.
[arch(at = "../ARCHITECTURE_TTS.md#tts-image-rail")]
def stage_styletts2(path : string; q8 : bool = false; kq : bool = false) : St2Staging =>
    <- stage_styletts2_(path, q8, kq, false)

//! every weight into the staging's planes, a GPU flavor's block baked first so the walk drops the arrays it replaces
def private st2_stage_planes(var st : St2Staging) {
    var io = TtsBlobIo(staging = true, blob = unsafe(addr(st.blob)), qblob = unsafe(addr(st.qblob)))
    var inscope spent : table<uint64>
    if (st.e.dev_backend != TowerDev.none) {
        tower_bake_block(g_styletts2_flavor, st, spent, "styletts2")
        st.e.dev_layout = g_st2_block_layout
        st.e.host_weights = tts_host_weights_()
        if (!st.e.host_weights) {
            io.spent = unsafe(addr(spent))
        }
    }
    weights_walk(io, st.e)
}

// `for_writer` is the writer's stage: the planes stay in disk order and every minted weight is recorded as its blocks
def private stage_styletts2_(path : string; q8, kq, for_writer : bool) : St2Staging {
    if (kq && !q8) {
        panic("dasLLAMA styletts2: the recipe lane mints Q8_0 beside its K-quant groups; it is not an f32 lane")
    }
    var inscope st = St2Staging()
    st.e.q8 = q8
    st.e.kq = kq
    st.e.dev_backend = for_writer ? TowerDev.none : tower_staging_dev()
    delete g_stage_minted   // a stage a panic cut short left its counts behind
    delete g_captured
    delete g_captured_at
    g_stage_q8 = q8
    g_stage_kq = kq
    g_stage_stored = false
    g_stage_capture = for_writer
    g_stage_repack = q8 && !for_writer && select_matmul_backend_for_load_()
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
        if (gguf_has(m, "styletts2.stored_layout") || gguf_has(m, "styletts2.stored_lane")) {
            stage_stored_lane(m, bytes, path, st.e)
        }
        st.e.name = gguf_str(m, bytes, "general.name")
        st.e.sample_rate = int(gguf_int(m, bytes, "styletts2.sample_rate"))
        st.e.n_symbols = gguf_int(m, bytes, "styletts2.n_symbols")
        st.e.style_dim = gguf_int(m, bytes, "styletts2.style_dim")
        st.e.style_rows = int(gguf_int(m, bytes, "styletts2.style_rows"))
        if (st.e.style_rows <= 0 || st.e.style_dim <= 0l) {
            panic("dasLLAMA styletts2: '{path}' declares {st.e.style_rows} style rows of {st.e.style_dim}; a voice pack needs at least one row")
        }
        st.e.source.n_harm = gguf_int(m, bytes, "styletts2.n_harmonics")
        st.e.source.sample_rate = float(st.e.sample_rate)
        st.e.source.torch_math = gguf_has(m, "styletts2.resample") && gguf_str(m, bytes, "styletts2.resample") == "torch"
        st.e.voice_names <- gguf_str_array(m, bytes, "styletts2.voices")
        for (v in st.e.voice_names) {
            let vname = "voice.{v}"
            var stored_as = KqFmt.q8
            if (stored_quant(m, vname, stored_as)) {
                let rows = m.tensors[need_tensor(m, vname)].n_elem / st.e.style_dim
                if (stored_as != KqFmt.q8) {
                    panic("dasLLAMA styletts2: '{vname}' is stored {stored_as}; a voice pack's stored form is Q8_0")
                }
                q8_matrix_read(m, bytes, vname, st.e.style_dim, rows, st.e.voices_q[v], st.e.voices_qs[v], false)
                continue
            }
            var inscope pack <- read_arr(m, bytes, vname)
            let fmt = stage_fmt(vname)
            let quant = g_stage_q8 && fmt == St2Fmt.q8 && st.e.style_dim % 32l == 0l
            if (quant) {   // a style row is whole blocks of 32, so a row's scales are its own
                let n = long_length(pack)
                q8_matrix_mint(pack, st.e.style_dim, n / st.e.style_dim, st.e.voices_q[v], st.e.voices_qs[v], false)
                capture_q8(vname, st.e.voices_q[v], st.e.voices_qs[v], n, [st.e.style_dim, n / st.e.style_dim])
            } else {
                st.e.voices[v] <- pack
            }
            stage_note(vname, fmt, quant == (fmt == St2Fmt.q8))
        }
        stage_family_data(m, bytes, st.e)
        read_albert(m, bytes, st.e.albert)
        read_text_encoder(m, bytes, st.e.text_encoder)
        read_predictor(m, bytes, st.e.predictor)
        read_decoder(m, bytes, st.e.decoder)
    }
    fclose(f)
    stage_report()
    g_stage_kq = false
    g_stage_stored = false
    st2_stage_planes(st)
    return <- st
}

// A file that stores its weights as minted decides the lane: `e` takes the lane the file names whatever the stage was asked,
// the readers take each quantized tensor as it is and mint nothing from a float one. A layout this build does not read,
// and the writer's stage over such a file, are refused.
[arch(at = "../ARCHITECTURE_TTS_FORMATS.md#tts-stored-file")]
def private stage_stored_lane(m : GGUFMeta; bytes : array<uint8>#; path : string; var e : StyleTts2Model) {
    let layout = gguf_has(m, "styletts2.stored_layout") ? gguf_int(m, bytes, "styletts2.stored_layout") : 0l
    if (layout != ST2_STORED_LAYOUT) {
        panic("dasLLAMA styletts2: '{path}' stores its weights in layout {layout}; this build reads layout {ST2_STORED_LAYOUT}")
    }
    if (g_stage_capture) {
        panic("dasLLAMA styletts2: '{path}' already stores its weights as minted; the writer takes a file of floats")
    }
    let lane = gguf_str_or(m, bytes, "styletts2.stored_lane", "")
    if (lane != "q8" && lane != "kq") {
        panic("dasLLAMA styletts2: '{path}' names the stored lane '{lane}'; a file stores the q8 lane or the kq one")
    }
    e.q8 = true
    e.kq = lane == "kq"
    g_stage_stored = true
    g_stage_q8 = false
    g_stage_kq = false
    g_stage_repack = select_matmul_backend_for_load_()
}

//! The lane a StyleTTS2-lineage GGUF stores its weights on - "q8" or "kq" - and "" for a file of floats, which mints at
//! load, or for a path the header read declines.
[arch(at = "../ARCHITECTURE_TTS_FORMATS.md#tts-stored-file")]
def styletts2_stored_lane(path : string) : string => gguf_file_str(path, "styletts2.stored_lane")

def private stage_family_data(m : GGUFMeta; bytes : array<uint8>#; var e : StyleTts2Model) {
    if (e.family == "kitten-tts") {
        for (v in e.voice_names) {
            if (gguf_has(m, "kitten.speed_prior.{v}")) {
                e.kitten.speed_priors[v] = gguf_f32(m, bytes, "kitten.speed_prior.{v}")
            }
        }
        var inscope anames <- gguf_str_array(m, bytes, "kitten.alias_names")
        var inscope avoices <- gguf_str_array(m, bytes, "kitten.alias_voices")
        for (an, av in anames, avoices) {
            e.kitten.aliases[an] = av
        }
    } elif (e.family == "kokoro") {
        var inscope cpts <- gguf_int_array(m, bytes, "kokoro.symbol_cpts")
        var inscope ids <- gguf_int_array(m, bytes, "kokoro.symbol_ids")
        for (c, i in cpts, ids) {
            e.kokoro.sym[int(c)] = int(i)
        }
        if (gguf_has(m, "kokoro.voice_langs")) {
            var inscope langs <- gguf_str_array(m, bytes, "kokoro.voice_langs")
            if (length(langs) != length(e.voice_names)) {
                panic("dasLLAMA tts: kokoro.voice_langs names {length(langs)} languages for {length(e.voice_names)} voices - the converter writes one per styletts2.voices entry")
            }
            for (voice, lang in e.voice_names, langs) {
                if (!empty(lang)) {
                    e.kokoro.voice_langs[voice] = lang
                }
            }
        }
    }
}

//! Mint a served model from a staged read without touching disk - the in-memory image rail,
//! the off-rail control for suites. The staged read's lane picks the tag.
def mint_styletts2(var st : St2Staging; var out : StyleTts2Model) {
    cache_via_image_staged(st, out, "", tower_dev_tag(st2_tag_of(st.e.q8, st.e.kq), st.e.dev_backend))
    st2_bind(out)
}

//! Bake a GGUF's prepared image to `out` on the lane `q8` and `kq` name - the converter's TTS arm. The
//! box's backend pin (the converter's ``--config``) decides the quant planes' repack. false = not written.
def bake_styletts2_image(path, out : string; q8 : bool; kq : bool = false) : bool {
    var inscope st <- stage_styletts2(path, q8, kq)
    var inscope m = StyleTts2Model()
    return cache_via_image_staged(st, m, out, st2_tag_of(st.e.q8, st.e.kq))   // a file that stores its weights names its own lane
}

// The stored file's head (`gguf_rewritten_head`): the source's keys with the stored layout's own after them, and the
// tensor table with every minted weight retyped.
def private stored_file_head(m : GGUFMeta; bytes : array<uint8>#; src : string; kq : bool; var head : array<uint8>) {
    var inscope extra : array<uint8>
    gguf_kv_u32(extra, "styletts2.stored_layout", uint64(ST2_STORED_LAYOUT))
    gguf_kv_str(extra, "styletts2.stored_lane", kq ? "kq" : "q8")
    var convs = 0
    for (cap in g_captured) {
        if (!empty(cap.conv)) {
            gguf_kv_i32_array(extra, "styletts2.stored_conv.{cap.name}", cap.conv)
            convs++
        }
    }
    var taken = 0
    gguf_rewritten_head(m, bytes, 2 + convs, extra, head) $(ti : GGUFTensorInfo; var gtype : int&; var dims : array<uint64>; var nbytes : int64&) {
        let at = g_captured_at?[ti.name] ?? -1
        return if (at < 0)
        taken++
        gtype = g_captured[at].gtype
        dims := g_captured[at].dims
        nbytes = long_length(g_captured[at].bytes)
        var n_elem = 1l
        for (d in dims) {
            n_elem *= int64(d)
        }
        if (nbytes != ggml_type_bytes(gtype, n_elem)) {
            panic("dasLLAMA styletts2: '{ti.name}' minted {nbytes} bytes for {dims} of ggml type {gtype}")
        }
    }
    if (taken != length(g_captured)) {
        panic("dasLLAMA styletts2: {length(g_captured) - taken} minted weights name no tensor of '{src}'")
    }
}

// every tensor's bytes to `w` in the table's order, each padded to the alignment; false where a write came up short
def private stored_file_data(w : file; m : GGUFMeta; bytes : array<uint8>#) : bool {
    var inscope pad : array<uint8>
    for (ti in m.tensors) {
        let at = g_captured_at?[ti.name] ?? -1
        let nbytes = at >= 0 ? long_length(g_captured[at].bytes) : ggml_type_bytes(ti.ggml_type, ti.n_elem)
        var wrote = nbytes == 0l
        if (at >= 0) {
            wrote = long_fwrite(w, g_captured[at].bytes) == nbytes
        } elif (nbytes > 0l) {
            unsafe {
                wrote = long_fwrite(w, temp_array(addr<uint8?>(bytes[m.data_offset + ti.offset]), nbytes, type<uint8>)) == nbytes
            }
        }
        pad |> resize(round_up(nbytes, m.alignment) - nbytes)
        return false if (!wrote || long_fwrite(w, pad) != long_length(pad))
    }
    return true
}

//! Write the GGUF at `src` - a StyleTTS2-lineage file of floats - to `dst` with every weight the q8 lane mints stored as
//! minted, or with `kq` the recipe lane's: each a quant matrix under its tensor's name, the rest and every key as `src`
//! holds them, the stored layout and lane named. The file serves as stored, with no mint at load. false = not written.
[arch(at = "../ARCHITECTURE_TTS_FORMATS.md#tts-stored-file"), cold_path]
def write_styletts2_gguf(src, dst : string; kq : bool) : bool {
    {
        var inscope st <- stage_styletts2_(src, true, kq, true)
    }
    g_stage_capture = false
    var written = false
    let tmp = "{dst}.tmp"
    let f = fopen(src, "rb")
    fmap(f) $(var bytes : array<uint8>#) {
        var inscope m <- parse_gguf_meta(bytes)
        var inscope head : array<uint8>
        stored_file_head(m, bytes, src, kq, head)
        let w = fopen(tmp, "wb")
        if (w != null) {
            written = long_fwrite(w, head) == long_length(head) && stored_file_data(w, m, bytes)
            fclose(w)
        }
    }
    fclose(f)
    delete g_captured
    delete g_captured_at
    if (!written || !rename(tmp, dst)) {
        remove(tmp)
        to_log(LOG_ERROR, "dasLLAMA styletts2: '{dst}' was not written - an unwritable path, or a full disk\n")
        return false
    }
    return true
}

//! Load a converted StyleTTS2-lineage GGUF through its prepared image on the current lane (the sibling `.dlim` maps when it
//! exists, else the GGUF stages, bakes and maps; ``DASLLAMA_IMAGE=0`` mints in memory), or a `.dlim` path directly: the
//! file names its lane, and the gguf is not needed.
[arch(at = "../ARCHITECTURE_TTS.md#tts-image-rail"), arch(at = "../ARCHITECTURE_IMAGE.md#image-identity-backend-order")]
def load_styletts2(path : string) : StyleTts2Model {
    tts_host_weights_refuse_load("dasLLAMA styletts2")
    bump_weights_epoch()   //! the previous model's GPU slabs drop before this one's weights can land at their addresses
    let dlim = path |> ends_with(".dlim")
    let stored = dlim ? "" : styletts2_stored_lane(path)   // a file that stores its weights as minted serves the lane it names
    let q8 = !empty(stored) || st2_serve_q8()
    let kq = empty(stored) ? st2_serve_kq() : stored == "kq"
    st2_announce_lane(dlim, stored)
    if (kq && !empty(g_st2_recipe_pin) && !dlim && empty(stored)) {   // pinned groups: minted in memory, no image read or written
        var inscope pinned = StyleTts2Model()
        var inscope st <- stage_styletts2_served(path, q8, kq)
        mint_styletts2(st, pinned)
        return <- pinned
    }
    let lane = image_path_lane(path, st2_tag_of(q8, kq), ST2_IMAGE_PREFIX)
    let tag = dlim ? named_image_tag(path, lane) : tower_dev_tag(lane, styletts2_dev_flavor(kq))
    var inscope m <- load_staged_image(path, tag, "styletts2", "source gguf", type<StyleTts2Model>, $ {
        return <- stage_styletts2_served(path, q8, kq)
    }, $(t : StyleTts2Model) => tower_block_current(t, g_st2_block_layout), $(t : StyleTts2Model; src : string) {
        if (!tower_block_current(t, g_st2_block_layout)) {
            panic("dasLLAMA styletts2: '{src}' carries a {t.dev_backend} device block another build laid out - regenerate it from the source gguf")
        }
    })
    if (m.dev_backend != TowerDev.none) {
        to_log(LOG_INFO, "dasLLAMA styletts2: the load serves the {m.dev_backend} image - its seats' weights are the device block's\n")
    }
    st2_bind(m)
    return <- m
}

//! True when `name` is one of the model's voices.
def styletts2_has_voice(m : StyleTts2Model; name : string) : bool => key_exists(m.voices, name) || key_exists(m.voices_q, name)

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
    m.voices_q |> get(name) $(quants) {   // a pack minted Q8_0, in disk order: the row read back off its own blocks
        m.voices_qs |> get(name) $(scales) {
            q8_matrix_row(quants, scales, false, m.style_dim, int64(m.style_rows), int64(r), style, 0l)
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
    cpu_only : bool   //! the trace rail: every seat declines for the synthesis that set it
    blk : TtsDevBlock = TtsDevBlock()   //! the served model's device block, a view the synthesis sets: the seats attach its pieces
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
    albert : TtsBertScratch = TtsBertScratch()
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
    @scratch @exact_size dec_asr_tm : array<float>
    @scratch @exact_size dec_a : array<float>
    @scratch @exact_size dec_b : array<float>
    @scratch @exact_size dec_cat : array<float>
    @scratch @exact_size dec_har : array<float>
    @scratch @exact_size dec_spec : array<float>
    @scratch @exact_size dec_spec_tm : array<float>
    @scratch @exact_size so_sines : array<float>   //! one source window's harmonic rows
    @scratch @exact_size so_noise : array<float>   //! the window's noise draw, when none was captured
    @scratch @exact_size so_har : array<float>     //! the window's mixed signal before it joins `dec_har`
    so_carry : SineSourceCarry = SineSourceCarry()   //! the phase state between windows
    @scratch @exact_size hs_padded : array<float>
    @scratch @exact_size hs_re : array<float>
    @scratch @exact_size hs_im : array<float>
    @scratch @exact_size gen_a : array<float>    //! the generator's stage stream, one side of the ping-pong
    @scratch @exact_size gen_b : array<float>    //! the other side
    @scratch @exact_size st_y : array<float>     //! a stage's noise branch output, then its upsampled stream
    @scratch @exact_size st_res : array<float>   //! the noise branch's residual output, then each Snake block's
    @scratch @exact_size grb_t0 : array<float>   //! a conv pair's AdaIN + Snake input, then its second conv output
    @scratch @exact_size grb_t1 : array<float>   //! a conv pair's first conv output
    @scratch @exact_size gen_post : array<float>
    @scratch @exact_size gen_re : array<float>
    @scratch @exact_size gen_im : array<float>
    @scratch @exact_size gen_wr : array<float>
    @scratch @exact_size gen_wi : array<float>
    @scratch @exact_size ids : array<int>
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

//! a GPU image's stage reaching its CPU body (a declined seat, the knob off after load, a trace) refuses: the device block alone holds it
def private st2_cpu_refuse(sc : St2Scratch; stage : string) {
    if (sc.blk.plan_n > 0l && !sc.blk.host) {
        panic("dasLLAMA styletts2: the {stage} did not run on the device - a GPU image never runs on the CPU (the seat's log line names the reason; with the tower knob off or a stage trace asked, load the source model: it maps the CPU image)")
    }
}

//! The PL-BERT encoder: embeddings, then one ALBERT layer applied n_layers times; out [T][hidden].
def styletts2_albert(a : St2Albert; ids : array<int>; @scratch @exact_size var out : array<float>; var sc : St2Scratch) {
    let t = long_length(ids)
    if (t > a.max_pos) {
        panic("dasLLAMA styletts2: {t} tokens exceed the encoder's {a.max_pos} positions; styletts2_synthesize truncates before it gets here")
    }
    if (g_st2_drv.albert != null && !sc.cpu_only) {
        st2_seat_call(ST2_SEAT_ALBERT)
        if (invoke(g_st2_drv.albert, a, ids, out, sc)) {
            st2_seat_served(ST2_SEAT_ALBERT)
            return
        }
    }
    st2_cpu_refuse(sc, "PL-BERT")
    bert_embed_rows(ids, a.word_emb.a, a.pos_emb.a, a.type_emb.a, a.emb_ln, ALBERT_EPS, a.emb_dim, sc.al_e)
    linear_rows(a.map_in, sc.al_e, t, out)
    for (_layer in range64(a.n_layers)) {
        bert_layer_rows(a, a.heads, ALBERT_EPS, TtsGelu.tanh, out, t, sc.albert)
    }
}

//! The text encoder: embedding, conv + channel LayerNorm + LeakyReLU stack, BiLSTM; out [C][T].
def styletts2_text_encoder(te : St2TextEncoder; ids : array<int>; @scratch @exact_size var out : array<float>; var sc : St2Scratch) {
    if (g_st2_drv.text != null && !sc.cpu_only) {
        st2_seat_call(ST2_SEAT_TEXT)
        if (invoke(g_st2_drv.text, te, ids, out, sc)) {
            st2_seat_served(ST2_SEAT_TEXT)
            return
        }
    }
    st2_cpu_refuse(sc, "text encoder")
    let t = long_length(ids)
    let c = te.c
    unsafe(scratch_resize(sc.te_xtm, t * c))
    var xtm & = unsafe(sc.te_xtm)
    gather_rows(xtm, te.emb.a, ids, c)
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
    unsafe(scratch_resize(out, t * (c + sd)))
    for (i in range64(t)) {
        for (d in range64(c)) {
            out[i * (c + sd) + d] = x[i * c + d]
        }
        for (d in range64(sd)) {
            out[i * (c + sd) + c + d] = s[d]
        }
    }
}

//! The duration encoder: the BERT projection, then alternating BiLSTM and AdaLayerNorm passes
//! with the prosody style appended after each; out is token-major [T][C + style]. The projected
//! rows stay in `sc.d_en` for the trace rail.
def styletts2_duration_encoder(p : St2Predictor; bert : array<float>; t : int64; s : array<float>; @scratch @exact_size var out : array<float>; var sc : St2Scratch) {
    if (g_st2_drv.dur_enc != null && !sc.cpu_only) {
        st2_seat_call(ST2_SEAT_DUR_ENC)
        if (invoke(g_st2_drv.dur_enc, p, bert, t, s, out, sc)) {
            st2_seat_served(ST2_SEAT_DUR_ENC)
            return
        }
    }
    st2_cpu_refuse(sc, "duration encoder")
    let c = p.bert_encoder.nout
    var d_en & = unsafe(sc.d_en)
    linear_rows(p.bert_encoder, bert, t, d_en)
    var x & = unsafe(sc.de_x)
    var y & = unsafe(sc.de_y)
    append_style_rows(d_en, t, c, s, x)
    for (i in range(length(p.lstms))) {
        bilstm(p.lstms[i], x, t, y)
        ada_layernorm_rows(p.lns[i], y, t, c, s, LN_EPS)
        append_style_rows(y, t, c, s, x)
    }
    unsafe(scratch_resize(out, t * (c + long_length(s))))
    copy_floats(x, 0l, out, 0l, t * (c + long_length(s)))
}

//! Frame counts per token: sigmoid(duration logits) summed, divided by speed, rounded half
//! to even, at least one. Returns the total frame count.
def styletts2_durations(p : St2Predictor; d : array<float>; t : int64; speed : float; @scratch @exact_size var dur : array<int>; @scratch @exact_size var raw : array<float>; var sc : St2Scratch) : int64 {
    var served = false
    if (g_st2_drv.durations != null && !sc.cpu_only) {
        st2_seat_call(ST2_SEAT_DURATIONS)
        served = invoke(g_st2_drv.durations, p, d, t, speed, raw, sc)
        if (served) {
            st2_seat_served(ST2_SEAT_DURATIONS)
        }
    }
    if (!served) {
        st2_cpu_refuse(sc, "duration head")
        var x & = unsafe(sc.du_x)
        var logits & = unsafe(sc.du_logits)
        bilstm(p.lstm, d, t, x)
        linear_rows(p.duration_proj, x, t, logits)
        let nb = p.duration_proj.nout
        unsafe(scratch_resize(raw, t))
        for (i in range64(t)) {
            var acc = 0.0
            for (j in range64(nb)) {
                acc += sigmoid_f32(logits[i * nb + j])
            }
            raw[i] = acc / speed
        }
    }
    unsafe(scratch_resize(dur, t))
    var frames = 0l
    for (i in range64(t)) {
        let r = max(1.0, round_half_even(raw[i]))
        dur[i] = int(r)
        frames += int64(dur[i])
    }
    return frames
}

// out = (out + shortcut) / sqrt(2): the residual block's join
def private residual_join(var out : array<float>; shortcut : array<float>; n : int64) {
    let inv = 1.0 / sqrt(2.0)
    for (i in range64(n)) {
        out[i] = (out[i] + shortcut[i]) * inv
    }
}

//! the rows an AdaIN residual block returns over `t` rows: its upsampling pool's, or `t`
def st2_blk_rows(b : St2AdainResBlk; t : int64) : int64 => b.upsample ? conv1d_out_len(b.pool, t) : t

// The AdaIN residual block on token-major rows x [t][dim_in] -> out [t1][dim_out]: the first
// norm copies as it normalizes, the identity shortcut reads x in place, the upsampling pool is
// the rows depthwise transposed conv and the nearest copy of the shortcut.
def private adain_res_blk(b : St2AdainResBlk; x : array<float>; t : int64; s : array<float>; @scratch @exact_size var out : array<float>; var sc : St2Scratch) : int64 {
    var r & = unsafe(sc.rb_r)
    var pooled & = unsafe(sc.rb_pooled)
    var c1 & = unsafe(sc.rb_c1)
    var shortcut & = unsafe(sc.rb_sc)
    var shortcut2 & = unsafe(sc.rb_sc2)
    adain_rows_into(b.norm1, x, r, t, b.dim_in_s, s, LN_EPS)
    leaky_relu(r, b.dim_in_s * t, 0.2)
    var t1 = t
    if (b.upsample) {
        conv1d_rows_transposed_depthwise(b.pool, r, t, pooled)
        t1 = st2_blk_rows(b, t)
        conv1d_rows(b.conv1, pooled, t1, c1)
    } else {
        conv1d_rows(b.conv1, r, t1, c1)
    }
    adain_rows(b.norm2, c1, t1, b.dim_out, s, LN_EPS)
    leaky_relu(c1, b.dim_out * t1, 0.2)
    conv1d_rows(b.conv2, c1, t1, out)
    if (b.upsample) {
        resize_nearest_rows(x, t, b.dim_in_s, 2l, shortcut)
        if (b.learned_sc) {
            conv1d_rows(b.conv1x1, shortcut, t1, shortcut2)
            residual_join(out, shortcut2, b.dim_out * t1)
        } else {
            residual_join(out, shortcut, b.dim_out * t1)
        }
    } elif (b.learned_sc) {
        conv1d_rows(b.conv1x1, x, t1, shortcut2)
        residual_join(out, shortcut2, b.dim_out * t1)
    } else {
        residual_join(out, x, b.dim_out * t1)
    }
    return t1
}

// The branch's residual blocks ping-pong the carrier's two branch buffers, the first reading x.
def private prosody_branch(blocks : array<St2AdainResBlk>; proj : TtsConv1d; x : array<float>; frames : int64; s : array<float>;
                           @scratch @exact_size var out : array<float>; var sc : St2Scratch) : int64 {
    var a & = unsafe(sc.pb_a)
    var b & = unsafe(sc.pb_b)
    var ct & = unsafe(sc.pr_x)
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
    let c = nb == 0 ? proj.cin : blocks[nb - 1].dim_out
    if (nb == 0) {   // the single-channel projection stays channel-major: one transpose in
        transpose_tc(x, t, c, ct)
    } elif (nb % 2 == 1) {
        transpose_tc(a, t, c, ct)
    } else {
        transpose_tc(b, t, c, ct)
    }
    conv1d(proj, ct, t, out)
    return conv1d_out_len(proj, t)
}

//! F0 and energy curves from the aligned duration-encoder features: the shared BiLSTM, then
//! each branch's three residual blocks (the middle one doubles the frame rate) and projection.
def styletts2_prosody(p : St2Predictor; en : array<float>; c, frames : int64; s : array<float>;
                      @scratch @exact_size var f0 : array<float>; @scratch @exact_size var n : array<float>; var sc : St2Scratch) : int64 {
    if (g_st2_drv.prosody != null && !sc.cpu_only) {
        st2_seat_call(ST2_SEAT_PROSODY)
        var f2g = 0l
        if (invoke(g_st2_drv.prosody, p, en, c, frames, s, f0, n, f2g, sc)) {
            st2_seat_served(ST2_SEAT_PROSODY)
            return f2g
        }
    }
    st2_cpu_refuse(sc, "prosody stage")
    var en_tm & = unsafe(sc.pr_en_tm)
    var x_tm & = unsafe(sc.pr_x_tm)
    transpose_ct(en, c, frames, en_tm)
    bilstm(p.shared_lstm, en_tm, frames, x_tm)
    let f2 = prosody_branch(p.f0, p.f0_proj, x_tm, frames, s, f0, sc)
    prosody_branch(p.n, p.n_proj, x_tm, frames, s, n, sc)
    return f2
}

// One Snake residual block on token-major rows: `y` = `src` plus the three conv pairs' residuals,
// the first pair reading `src` so no copy of the input is ever made. Every op class lands in
// its tts.gen.* bucket of the encode-stage profiling rail.
[arch(at = "../ARCHITECTURE_TTS_MEMORY.md#tts-generator-buffers")]
def private gen_res_block(b : St2GenResBlock; src : array<float>; @scratch @exact_size var y : array<float>; t : int64; s : array<float>; var sc : St2Scratch) {
    let n = b.c * t
    var xt & = unsafe(sc.grb_t0)
    var y1 & = unsafe(sc.grb_t1)
    var y2 & = xt
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
            add_inplace_rows(y, y2, n)
        }
        asr_prof_add("tts.gen.resadd", tc)
    }
}

let SOURCE_WINDOW_FRAMES = 64l
var private g_source_window = TtsStreamWindow(frames = SOURCE_WINDOW_FRAMES)

//! The frame window the harmonic source runs in (`set_styletts2_source_window`; 0 = the whole run).
def styletts2_source_window() : int64 => g_source_window.frames

[arch(at = "../ARCHITECTURE_TTS_MEMORY.md#tts-source-stream")]
def set_styletts2_source_window(frames : int64) {
    //! Run the harmonic source `frames` phase frames at a time (the default `SOURCE_WINDOW_FRAMES`; 0 = the
    //! whole run in one window; a negative count is refused; every set prints one info line): the same
    //! samples either way, the tables sized to the window.
    stream_window_set(g_source_window, frames, "set_styletts2_source_window")
}

//! The harmonic source: sines plus noise from the F0 curve, mixed to one signal of
//! frames * upsample samples, a window of frames at a time; `seed` draws the noise unless
//! `noise` captured it.
[arch(at = "../ARCHITECTURE_TTS_MEMORY.md#tts-source-stream")]
def styletts2_source(d : St2Decoder; cfg : SineSourceCfg; f0 : array<float>; f0_frames : int64;
                     noise : TtsNoise; seed : uint64; @scratch @exact_size var har : array<float>; var sc : St2Scratch) : int64 {
    let up = cfg.upsample
    let nh = cfg.n_harm
    let geo = sine_source_geometry(cfg, f0_frames)
    let n = geo.samples
    let nlow = geo.nlow
    let win = stream_window_for(g_source_window, nlow)
    var rng = TtsRng()
    if (!noise.captured) {
        rng_seed(rng, seed)
        for (_h in range64(nh)) {
            rng_uniform(rng)
        }
    }
    var sines & = unsafe(sc.so_sines)
    var nwin & = unsafe(sc.so_noise)
    var hwin & = unsafe(sc.so_har)
    sine_carry_reset(sc.so_carry, nh, win)
    unsafe(scratch_resize(har, n))
    var fa = 0l
    while (fa < nlow) {
        let fb = min(fa + win, nlow)
        let sa = fa * up
        var rows = 0l
        if (noise.captured) {
            rows = sine_source(cfg, f0, f0_frames, noise.uniform, fa, fb, sc.so_carry, noise.normal, sa * nh, sines)
        } else {
            let draw = ((fb == nlow ? n : fb * up) - sa) * nh
            unsafe(scratch_resize(nwin, draw))
            for (i in range64(draw)) {
                nwin[i] = rng_normal(rng)
            }
            rows = sine_source(cfg, f0, f0_frames, noise.uniform, fa, fb, sc.so_carry, nwin, 0l, sines)
        }
        linear_rows(d.source_linear, sines, rows, hwin)
        copy_floats(hwin, 0l, har, sa, rows)
        fa = fb
    }
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

//! the rows generator stage `i` reflects onto the left of its upsampled frames: one on the last stage
def st2_stage_pad(d : St2Decoder; i : int64) : int64 => i == long_length(d.ups) - 1l ? 1l : 0l

// One generator stage: x [tt][cc] rows in, `out` the next stage's rows; the upsampled stream
// carries the noise branch and the averaged Snake residual blocks.
[arch(at = "../ARCHITECTURE_TTS_MEMORY.md#tts-generator-buffers")]
def private generator_stage(d : St2Decoder; i : int64; var x : array<float>; @scratch @exact_size var out : array<float>;
                            var cc, tt : int64&; s : array<float>; har_spec : array<float>; har_frames : int64;
                            var tr : StyleTts2Trace?; var sc : St2Scratch) {
    var src & = unsafe(sc.st_y)
    var y & = src
    var src_res & = unsafe(sc.st_res)
    var tc = ref_time_ticks()
    leaky_relu(x, cc * tt, 0.1)
    asr_prof_add("tts.gen.leaky", tc)
    tc = ref_time_ticks()
    conv1d_rows(d.noise_convs[i], har_spec, har_frames, src)
    let src_t = conv1d_out_len(d.noise_convs[i], har_frames)
    let src_c = d.noise_convs[i].cout
    if (tr != null && i == 0l) {
        trace_rows(tr.src0, src, src_t, src_c)
    }
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
    let pad = st2_stage_pad(d, i)
    if (pad > 0l) {
        pad_reflect_left_rows_inplace(y, tt, cc, pad)
        tt += pad
    }
    if (src_t != tt) {
        panic("dasLLAMA styletts2: source branch {src_t} frames against {tt} at stage {i}")
    }
    let n = cc * tt
    tc = ref_time_ticks()
    add_inplace_rows(y, src_res, n)
    asr_prof_add("tts.gen.stagesum", tc)
    stage_resblocks(d, i, y, out, cc, tt, s, tr, sc)
}

// The stage's n_kernels Snake residual blocks, each from the stage stream x, averaged into out.
[arch(at = "../ARCHITECTURE_TTS_MEMORY.md#tts-generator-buffers")]
def private stage_resblocks(d : St2Decoder; i : int64; x : array<float>; @scratch @exact_size var out : array<float>;
                            cc, tt : int64; s : array<float>; var tr : StyleTts2Trace?; var sc : St2Scratch) {
    let n = cc * tt
    var y & = unsafe(sc.st_res)
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
            add_inplace_rows(out, y, n)
        }
        asr_prof_add("tts.gen.stagesum", tc)
    }
    tc = ref_time_ticks()
    scale_rows(out, 1.0 / float(d.n_kernels), n)
    asr_prof_add("tts.gen.stagesum", tc)
}

//! The PL-BERT encoder: the token ids to out [t][hidden].
typedef St2AlbertGpuFn = function<(a : St2Albert; ids : array<int>; @scratch @exact_size var out : array<float>; var sc : St2Scratch) : bool>
//! The text encoder: the token ids to out [c][t] (channel-major, the CPU form's).
typedef St2TextGpuFn = function<(te : St2TextEncoder; ids : array<int>; @scratch @exact_size var out : array<float>; var sc : St2Scratch) : bool>
//! The duration encoder: the BERT rows [t][hidden] through the BERT projection and the prosody style to out [t][c + style].
typedef St2DurEncGpuFn = function<(p : St2Predictor; bert : array<float>; t : int64; s : array<float>; @scratch @exact_size var out : array<float>; var sc : St2Scratch) : bool>
//! The duration head: d [t][c + style] to the raw frame counts [t] (the sigmoid sums over speed).
typedef St2DurationsGpuFn = function<(p : St2Predictor; d : array<float>; t : int64; speed : float; @scratch @exact_size var raw : array<float>; var sc : St2Scratch) : bool>
//! The prosody predictor: en [c][frames] (channel-major) and the style to F0 and N [f2], f2 written.
typedef St2ProsodyGpuFn = function<(p : St2Predictor; en : array<float>; c, frames : int64; s : array<float>; @scratch @exact_size var f0 : array<float>; @scratch @exact_size var n : array<float>; var f2 : int64&; var sc : St2Scratch) : bool>
//! The decoder from the aligned text features, F0 and N through the source and the generator to the samples.
typedef St2DecodeGpuFn = function<(d : St2Decoder; text_c : int64; source : SineSourceCfg; asr : array<float>; frames : int64; f0, nrg : array<float>; f2 : int64;
                                    s_ref : array<float>; noise : TtsNoise; seed : uint64; @scratch @exact_size var wave : array<float>; var sc : St2Scratch) : bool>

//! A driver's seats, one per stage; a seat left empty keeps the CPU form for that stage.
struct St2GpuDriver {
    albert : St2AlbertGpuFn
    text : St2TextGpuFn
    dur_enc : St2DurEncGpuFn
    durations : St2DurationsGpuFn
    prosody : St2ProsodyGpuFn
    decode : St2DecodeGpuFn
}

var private g_st2_drv : St2GpuDriver
var private g_st2_drv_prev : St2GpuDriver   //! the registration a caller's seat displaced

var private g_st2_seats <- tts_seats(["albert", "text", "dur_enc", "durations", "prosody", "decode"])
let private ST2_SEAT_ALBERT = 0   //!< the seats' indices in the record's order
let private ST2_SEAT_TEXT = 1
let private ST2_SEAT_DUR_ENC = 2
let private ST2_SEAT_DURATIONS = 3
let private ST2_SEAT_PROSODY = 4
let private ST2_SEAT_DECODE = 5

[arch(at = "../ARCHITECTURE_MEDIA.md#tower-gpu-hook")]
def register_styletts2_gpu(drv : St2GpuDriver) {
    g_st2_drv_prev = g_st2_drv
    g_st2_drv = drv
}

//! The restore half of the seats a caller takes: the displaced registration serves again.
def unregister_styletts2_gpu {
    g_st2_drv = g_st2_drv_prev
    g_st2_drv_prev = St2GpuDriver()
}

def private st2_seat_call(seat : int) {
    tts_seat_call(g_st2_seats, seat)
}

def private st2_seat_served(seat : int) {
    tts_seat_served(g_st2_seats, seat)
}

//! Hook invocations and the ones that served, since the process started, per stage (a seat name of the record).
[cold_path]
def styletts2_gpu_stats(stage : string) : tuple<calls : int; served : int> => tts_seat_stats(g_st2_seats, stage, "styletts2")

//! The seat names of the hook record, in dispatch order.
[cold_path]
def styletts2_gpu_seats() : array<string> => tts_seat_names(g_st2_seats)

//! Whether the registered driver fills the seat `stage` names (a driver ported seat by seat leaves the rest empty).
[cold_path]
def styletts2_gpu_seat_present(stage : string) : bool {
    if (stage == "albert") {
        return g_st2_drv.albert != null
    } elif (stage == "text") {
        return g_st2_drv.text != null
    } elif (stage == "dur_enc") {
        return g_st2_drv.dur_enc != null
    } elif (stage == "durations") {
        return g_st2_drv.durations != null
    } elif (stage == "prosody") {
        return g_st2_drv.prosody != null
    } elif (stage == "decode") {
        return g_st2_drv.decode != null
    }
    panic("dasLLAMA styletts2: no seat named '{stage}'")
}

//! The iSTFTNet generator on token-major rows x [t][c]: two upsampling stages, each adding the
//! source spectrum through its noise branch and refining through Snake residual blocks, then the
//! inverse STFT into `wave`: the CPU form, which a device image refuses (its generator runs inside the decode seat).
def styletts2_generator_rows(d : St2Decoder; var x : array<float>; c, t : int64; s : array<float>;
                             har_spec : array<float>; har_frames : int64; var tr : StyleTts2Trace?;
                             @scratch @exact_size var wave : array<float>; var sc : St2Scratch) : int64 {
    var a & = unsafe(sc.gen_a)
    var b & = unsafe(sc.gen_b)
    st2_cpu_refuse(sc, "generator")
    var cc = c
    var tt = t
    let nst = long_length(d.ups)
    for (i in range64(nst)) {   // the first stage reads x, then the stages ping-pong b and a
        if (i == 0l) {
            generator_stage(d, i, x, b, cc, tt, s, har_spec, har_frames, tr, sc)
        } elif (i % 2l == 1l) {
            generator_stage(d, i, b, a, cc, tt, s, har_spec, har_frames, tr, sc)
        } else {
            generator_stage(d, i, a, b, cc, tt, s, har_spec, har_frames, tr, sc)
        }
    }
    if (nst == 0l) {
        return generator_tail(d, x, cc, tt, tr, wave, sc)
    }
    if (nst % 2l == 1l) {
        return generator_tail(d, b, cc, tt, tr, wave, sc)
    }
    return generator_tail(d, a, cc, tt, tr, wave, sc)
}

//! The generator from channel-major x_in [c][t] and har_spec [2 * bins][har_frames] - the
//! parity rail's seam form: two transposes into the carrier, then the rows form.
def styletts2_generator(d : St2Decoder; x_in : array<float>; c, t : int64; s : array<float>;
                        har_spec : array<float>; har_frames : int64; var tr : StyleTts2Trace?;
                        @scratch @exact_size var wave : array<float>; var sc : St2Scratch) : int64 {
    var a & = unsafe(sc.gen_a)
    var spec_tm & = unsafe(sc.dec_spec_tm)
    transpose_ct(x_in, c, t, a)
    transpose_ct(har_spec, d.noise_convs[0].cin, har_frames, spec_tm)
    return styletts2_generator_rows(d, a, c, t, s, spec_tm, har_frames, tr, wave, sc)
}

def private generator_tail(d : St2Decoder; var x : array<float>; cc, tt : int64; var tr : StyleTts2Trace?;
                           @scratch @exact_size var wave : array<float>; var sc : St2Scratch) : int64 {
    var y & = unsafe(sc.gen_post)
    let tc = ref_time_ticks()
    leaky_relu(x, cc * tt, 0.01)
    conv1d_rows(d.conv_post, x, tt, y)
    if (tr != null) {
        trace_rows_strided(tr.conv_post, y, tt, d.conv_post.cout, d.conv_post.cout_s)
    }
    asr_prof_add("tts.gen.post", tc)
    return styletts2_istft(d, y, d.conv_post.cout_s, tt, wave, sc)
}

//! the samples the inverse STFT returns over `tt` frames, the centre pad trimmed
def st2_wave_samples(d : St2Decoder; tt : int64) : int64 => conv1d_out_len(d.stft_bwd_re, tt) - 2l * ST2_STFT_PAD

//! The inverse STFT from conv_post's rows y [tt][stride] (a bin's log magnitude, then its phase):
//! the polar-to-rectangular pair, the two transposed convs, the envelope; the sample count.
def styletts2_istft(d : St2Decoder; y : array<float>; stride, tt : int64;
                    @scratch @exact_size var wave : array<float>; var sc : St2Scratch) : int64 {
    var re & = unsafe(sc.gen_re)
    var im & = unsafe(sc.gen_im)
    var wr & = unsafe(sc.gen_wr)
    var wi & = unsafe(sc.gen_wi)
    let tc = ref_time_ticks()
    polar_to_rect_rows(y, stride, d.bins, tt, re, im)
    asr_prof_add("tts.gen.post", tc)
    conv1d(d.stft_bwd_re, re, tt, wr)
    conv1d(d.stft_bwd_im, im, tt, wi)
    let n = st2_wave_samples(d, tt)
    unsafe(scratch_resize(wave, n))
    for (i in range64(n)) {
        wave[i] = wr[STFT_PAD + i] - wi[STFT_PAD + i]
    }
    if (d.istft_envelope) {
        istft_envelope_divide(wave, n, d.window.a, d.stft_bwd_re.k, d.stft_bwd_re.stride, STFT_PAD)
    }
    return n
}

//! Draw the harmonics' initial phases when no noise was captured; the per-sample normals that
//! follow them in the stream are drawn by `styletts2_source`, a window at a time.
def styletts2_draw_noise(cfg : SineSourceCfg; seed : uint64; var noise : TtsNoise) {
    return if (noise.captured)
    var rng = TtsRng()
    rng_seed(rng, seed)
    unsafe(scratch_resize(noise.uniform, cfg.n_harm))
    for (h in range64(cfg.n_harm)) {
        noise.uniform[h] = rng_uniform(rng)
    }
}

//! cat(x [t][c], asr_res [t][rc] at row stride `rs` when `with_res`, F0 [t], N [t]) as one row
//! per frame, zero-padded to `width` columns: out [t][width].
def private concat_rows(x : array<float>; c : int64; res : array<float>; rc, rs : int64; f0d, nd : array<float>; with_res : bool; t, width : int64; @scratch @exact_size var out : array<float>) {
    let r = with_res ? rc : 0l
    let total = c + r + 2l
    unsafe(scratch_resize(out, width * t))
    var op = unsafe(addr(out[0]))
    let xp = unsafe(addr<float const?>(x[0]))
    let rp = with_res ? unsafe(addr<float const?>(res[0])) : null
    unsafe {
        for (i in range64(t)) {
            var row = op + i * width
            memcpy(row, xp + i * c, c * 4l)
            if (with_res) {
                memcpy(row + c, rp + i * rs, rc * 4l)
            }
            row[c + r] = f0d[i]
            row[c + r + 1l] = nd[i]
            for (j in range64(total, width)) {
                row[j] = 0.0
            }
        }
    }
}

[cold_path]
def private trace_dec_in(var tr : StyleTts2Trace?; y : array<float>; cw : int64; f0d, nd, res : array<float>; rc, rs, frames, width : int64; @scratch @exact_size var cat : array<float>) {
    concat_rows(y, cw, res, rc, rs, f0d, nd, true, frames, width, cat)
    trace_rows_strided(tr.dec_in, cat, frames, cw + rc + 2l, width)
}

// One decoder block from `src`: through the concat scratch when the block's width asks for
// the F0, energy and residual channels again, straight through otherwise.
def private decode_step(blk : St2AdainResBlk; cw : int64; src : array<float>; @scratch @exact_size var cat : array<float>;
                        @scratch @exact_size var dst : array<float>; t : int64; f0d, nd, res : array<float>; rc, rs : int64; s_ref : array<float>; var sc : St2Scratch) : int64 {
    if (blk.dim_in != cw) {
        concat_rows(src, cw, res, rc, rs, f0d, nd, true, t, blk.dim_in_s, cat)
        return adain_res_blk(blk, cat, t, s_ref, dst, sc)
    }
    if (blk.dim_in_s != blk.dim_in) {
        panic("dasLLAMA styletts2: a decode block reading the previous stream at width {cw} serves a padded input width {blk.dim_in_s}; the padded width needs the concat")
    }
    return adain_res_blk(blk, src, t, s_ref, dst, sc)
}

//! The decoder: F0, energy and the asr residual as channels, the encode block, the decode blocks
//! ping-ponging the carrier's two stream buffers, then the source and the generator into sc.wave;
//! a registered driver gets the first refusal, except on the trace rail. The sample count.
def styletts2_decode(d : St2Decoder; text_c : int64; source : SineSourceCfg; asr : array<float>; frames : int64;
                     f0, nrg : array<float>; f2 : int64; s_ref : array<float>; var noise : TtsNoise; seed : uint64;
                     var tr : StyleTts2Trace?; var tm : TtsTimings; var sc : St2Scratch) : int64 {
    var clock = ref_time_ticks()
    if (g_st2_drv.decode != null && !sc.cpu_only) {
        //! A served decode is one command buffer: the decoder clock carries it, the source and generator clocks read zero.
        st2_seat_call(ST2_SEAT_DECODE)
        styletts2_draw_noise(source, seed, noise)
        if (invoke(g_st2_drv.decode, d, text_c, source, asr, frames, f0, nrg, f2, s_ref, noise, seed, sc.wave, sc)) {
            st2_seat_served(ST2_SEAT_DECODE)
            tm.decoder_us = int64(get_time_usec(clock))
            tm.source_us = 0l
            tm.generator_us = 0l
            tm.samples = long_length(sc.wave)
            return tm.samples
        }
    }
    st2_cpu_refuse(sc, "decoder")
    var f0d & = unsafe(sc.dec_f0d)
    var nd & = unsafe(sc.dec_nd)
    var res & = unsafe(sc.dec_res)
    var asr_tm & = unsafe(sc.dec_asr_tm)
    var a & = unsafe(sc.dec_a)
    var b & = unsafe(sc.dec_b)
    var cat & = unsafe(sc.dec_cat)
    conv1d(d.f0_conv, f0, f2, f0d)
    conv1d(d.n_conv, nrg, f2, nd)
    transpose_ct(asr, text_c, frames, asr_tm)
    conv1d_rows(d.asr_res, asr_tm, frames, res)
    let rc = d.asr_res.cout
    let rs = d.asr_res.cout_s
    if (tr != null) {
        trace_rows_strided(tr.asr_res, res, frames, rc, rs)
    }
    concat_rows(asr_tm, text_c, res, rc, rs, f0d, nd, false, frames, d.encode.dim_in_s, cat)
    var t = adain_res_blk(d.encode, cat, frames, s_ref, a, sc)
    var cw = d.encode.dim_out
    if (tr != null) {
        let w0 = !empty(d.decode) ? d.decode[0].dim_in_s : cw + rc + 2l
        trace_dec_in(tr, a, cw, f0d, nd, res, rc, rs, frames, w0, cat)
    }
    var in_a = true
    for (i in range(length(d.decode))) {
        if (in_a) {
            t = decode_step(d.decode[i], cw, a, cat, b, t, f0d, nd, res, rc, rs, s_ref, sc)
        } else {
            t = decode_step(d.decode[i], cw, b, cat, a, t, f0d, nd, res, rc, rs, s_ref, sc)
        }
        in_a = !in_a
        cw = d.decode[i].dim_out
    }
    if (tr != null) {
        trace_rows(tr.gen_in, in_a ? a : b, t, cw)
    }
    tm.decoder_us = int64(get_time_usec(clock))
    clock = ref_time_ticks()
    styletts2_draw_noise(source, seed, noise)
    var har & = unsafe(sc.dec_har)
    var har_spec & = unsafe(sc.dec_spec)
    let n = styletts2_source(d, source, f0, f2, noise, seed, har, sc)
    if (tr != null) {
        trace_take(tr.har, har)
    }
    let har_frames = styletts2_har_spectrum(d, har, n, tr, har_spec, sc)
    var spec_tm & = unsafe(sc.dec_spec_tm)
    transpose_ct(har_spec, d.noise_convs[0].cin, har_frames, spec_tm)
    tm.source_us = int64(get_time_usec(clock))
    clock = ref_time_ticks()
    var samples = 0l
    if (in_a) {
        samples = styletts2_generator_rows(d, a, cw, t, s_ref, spec_tm, har_frames, tr, sc.wave, sc)
    } else {
        samples = styletts2_generator_rows(d, b, cw, t, s_ref, spec_tm, har_frames, tr, sc.wave, sc)
    }
    tm.generator_us = int64(get_time_usec(clock))
    tm.samples = samples
    return samples
}

// the reference pipeline truncates an over-long chunk to the encoder's positions and says so
[cold_path]
def private warn_truncated(n, cap : int64) {
    to_log(LOG_WARNING, "dasLLAMA styletts2: {n} tokens exceed the encoder's {cap} positions - the chunk is truncated to {cap}\n")
}

//! the carrier set to serve a pass of `m`: the trace rail (`traced`) keeps every seat off
def private st2_carrier_arm(var sc : St2Scratch; m : StyleTts2Model; traced : bool) {
    sc.cpu_only = traced
    sc.blk = tts_dev_block(m.dev, m.devplan, m.host_weights, image_backing_mapped(m.image_map))
}

def private st2_carrier_disarm(var sc : St2Scratch) {
    sc.cpu_only = false
    sc.blk = TtsDevBlock()
}

//! a synthesis through its durations stage on a served carrier, ids clipped to the encoder's positions, stage rows left in `sc`; the frames
def private st2_duration_pass(m : StyleTts2Model; ids : array<int>; style : array<float>;
                              speed : float; var tr : StyleTts2Trace?; var tm : TtsTimings;
                              var sc : St2Scratch) : int64 {
    var clock = ref_time_ticks()
    let n_in = long_length(ids)
    let t = min(n_in, m.albert.max_pos)
    if (n_in > t) {
        warn_truncated(n_in, t)
    }
    unsafe(scratch_resize(sc.ids, t))
    var tok & = unsafe(sc.ids)
    for (i in range64(t)) {
        tok[i] = ids[i]
    }
    let hs = m.style_dim / 2l
    unsafe(scratch_resize(sc.s_ref, hs))
    unsafe(scratch_resize(sc.s_pro, hs))
    var s_ref & = unsafe(sc.s_ref)
    var s_pro & = unsafe(sc.s_pro)
    for (i in range64(hs)) {
        s_ref[i] = style[i]
        s_pro[i] = style[hs + i]
    }
    styletts2_albert(m.albert, tok, sc.bert, sc)
    tm.bert_us = int64(get_time_usec(clock))
    clock = ref_time_ticks()
    styletts2_text_encoder(m.text_encoder, tok, sc.t_en, sc)
    if (tr != null) {
        trace_take(tr.text_enc, sc.t_en)
    }
    tm.text_enc_us = int64(get_time_usec(clock))
    clock = ref_time_ticks()
    styletts2_duration_encoder(m.predictor, sc.bert, t, s_pro, sc.d, sc)
    if (tr != null) {
        trace_take(tr.bert, sc.d_en)
    }
    let frames = styletts2_durations(m.predictor, sc.d, t, speed, sc.dur, sc.raw, sc)
    if (tr != null) {
        trace_take(tr.dur_raw, sc.raw)
        tr.frames = frames
    }
    tm.duration_us = int64(get_time_usec(clock))
    return frames
}

[cold_path, arch(at = "../ARCHITECTURE_TTS_MEMORY.md#tts-scratch-carrier")]
def styletts2_frames(m : StyleTts2Model; ids : array<int>; style : array<float>; speed : float;
                     var tr : StyleTts2Trace?; var tm : TtsTimings;
                     var sc : St2Scratch) : int64 {
    st2_carrier_arm(sc, m, tr != null)
    defer() {
        st2_carrier_disarm(sc)
    }
    return st2_duration_pass(m, ids, style, speed, tr, tm, sc)
}

//! the samples a synthesis of `frames` frames returns, through the length steps the decoder and the generator take
[cold_path]
def styletts2_samples_of(d : St2Decoder; frames : int64) : int64 {
    var t = st2_blk_rows(d.encode, frames)
    for (b in d.decode) {
        t = st2_blk_rows(b, t)
    }
    for (i in range64(long_length(d.ups))) {
        t = conv1d_out_len(d.ups[i], t) + st2_stage_pad(d, i)
    }
    return st2_wave_samples(d, t)
}

//! Synthesize from token ids: the full StyleTTS2 pass into `sc.wave` (the facade trims the
//! tail), the sample count back. `noise` supplies the source noise (empty = drawn from `seed`),
//! `tr` collects the stage tensors when not null, `tm` the stage walls, `sc` every buffer.
[hot_path, arch(at = "../ARCHITECTURE_TTS_MEMORY.md#tts-scratch-carrier")]
def styletts2_synthesize(m : StyleTts2Model; ids : array<int>; style : array<float>; speed : float;
                         var noise : TtsNoise; seed : uint64; var tr : StyleTts2Trace?; var tm : TtsTimings; var sc : St2Scratch) : int64 {
    let start = ref_time_ticks()
    st2_carrier_arm(sc, m, tr != null)
    defer() {
        st2_carrier_disarm(sc)
    }
    let frames = st2_duration_pass(m, ids, style, speed, tr, tm, sc)
    var clock = ref_time_ticks()
    let t = long_length(sc.ids)
    let hs = m.style_dim / 2l
    let dc = m.predictor.bert_encoder.nout + hs
    align_expand_rows(sc.d, t, dc, sc.dur, sc.en)
    align_expand_channels(sc.t_en, m.text_encoder.c, t, sc.dur, sc.asr)
    tm.duration_us += int64(get_time_usec(clock))
    clock = ref_time_ticks()
    let f2 = styletts2_prosody(m.predictor, sc.en, dc, frames, sc.s_pro, sc.f0, sc.nrg, sc)
    if (tr != null) {
        trace_take(tr.f0, sc.f0)
        trace_take(tr.n, sc.nrg)
    }
    tm.prosody_us = int64(get_time_usec(clock))
    let samples = styletts2_decode(m.decoder, m.text_encoder.c, m.source, sc.asr, frames, sc.f0,
                                   sc.nrg, f2, sc.s_ref, noise, seed, tr, tm, sc)
    tm.total_us = tm.front_end_us + int64(get_time_usec(start))
    return samples
}
