options gen2
options persistent_heap
options stack = 524288
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_gigaam
require dasllama/dasllama_asr   // the facade's gigaam arms: caps, asr_exec_fmt, create_session, transcribe
require dasllama/dasllama_audio_io
require dasllama/dasllama_vad   // set_vad_model_path_probe_: the missing-weights refusal's seam
require daslib/env_registry   // env_value_of: the corpus harness child's image-rail knob, read the sanctioned way
require dasllama/dasllama_tower
require dasllama/dasllama_audio
require dasllama/dasllama_math
require daslib/jobque_boost
require daslib/fio
require strings
require daslib/strings_boost
require math
require daslib/linq   // sequence_equal
require _model_tier
require _json_file
require _npy   // read_npy: the oracle's per-stage dumps
require _daslang_argv   // daslang_argv: the corpus tool leaves out the modules this run leaves out
require _compares   // nan_wide_diff: a NaN probability reads as outside the bar
require _rnnt_oracle   // rnnt_seat_*: the build's device transducer seat, its pin and counters
require _gpu_knobs   // with_gpu_knobs / set_serving_gpu_tower: the device decode's tower pin
require dasllama/dasllama_load   // load_gguf: the foreign family's image the loader refuses
require dasllama/dasllama_gguf   // parse_gguf_meta: the Q8_0 seats' disk blocks; gguf_put_*: the retyped carriers' bytes
require daslib/f16_cvt           // f16_to_f32: a disk block's scale
require _gguf_writer             // gguf_retyped_copy: the retyped carriers the planner refuses
require dasllama/dasllama_image   // save_model_image
require dasllama/dasllama_common   // QuantMode
require ?das_metal dasllama/dasllama_metal_tower   // set_metal_tower: the Metal CTC cell's pin
require _ctc_oracle   // ctc_seat_heads: the build's device CTC seat's count
require _example_rail   // example_daslang: the corpus harness's child runs on this binary
require _image_memory   // read_page_aligned: a host's own copy of an image
require _model_once   // with_model_once: a carrier's one block in the area; own_load
require _audio_twin   // poison_fc_blocks / poison_kq_seats: a poisoned form's staging
require _audio_gigaam_cells   // the device twins' legs and compares, run under this file's blocks
require _kernel_coverage_cells   // coverage_tower_gigaam_metal: the kernel census's GigaAM encode

// GigaAM v3 against the upstream package's per-stage dumps (benchmarks/asr/gigaam_oracle.py into the model root's
// `asr_oracle/gigaam/<clip>_<variant>/`, minted by the `mint_gigaam.sh` stocked beside them over the corpus clips the
// cells read through `asr_clip`; encoder weights
// rounded through f16 - the carrier's precision) - the external-reference compare: the f16 lane stage by stage, the q8
// lane by its transcript's words, and the long-form spans. Every cell skips without its carrier, its dumps or its clip;
// the facade and `.dlim` cells also skip when interpreted (`jit_model_available`), the carrier's own `.dlim` cell with the
// image rail off (DASLLAMA_IMAGE=0), the device decode twin where the build
// or the box has no device transducer seat (`rnnt_seat_skip`), the kq lane's device leg where no GPU device stands up for
// the serving tower (`serving_tower_device_up`), the CTC-head cell on a build without das_metal and when
// interpreted, the decoder-needs-its-mmproj arm and the foreign-`.dlim`
// refusal without SmolLM2-135M-Instruct-Q8_0.gguf, and the side-file arm without the Qwen3-ASR-0.6B mmproj and decoder.
// A carrier loads once a form, in its one `with_model_once` block (`gigaam_form`, every form minted in memory): the CPU
// forms carry the external-reference cells, the device forms the Metal and the discrete twins' legs and the device seats'
// (`_audio_gigaam_cells`). A form's cells run on one home, so a compare of the CPU chain with a device chain is two forms,
// made after the block on what they kept. The quantized e2e carriers' cells read the f16 carrier's exact lane as their
// reference: their blocks follow its block in one `[test]`.

let private E2E_RNNT = "gigaam-v3-e2e-rnnt-f16.gguf"
let private KQ = "gigaam-v3-e2e-rnnt-q4_k.gguf"
let private KQ_MIX = "gigaam-v3-e2e-rnnt-q4_k-mix-local.gguf"
let private KQ_Q8_0 = "gigaam-v3-e2e-rnnt-q8_0.gguf"
let private E2E_CTC = "gigaam-v3-e2e-ctc-f16.gguf"
let private CTC = "gigaam-v3-ctc-f16.gguf"
let private RNNT = "gigaam-v3-rnnt-f16.gguf"

//! One stage's relative error bar: max |got - want| over max |want|.
let private MEL_BAR = 1e-4
let private FRONT_BAR = 1e-4
let private LAYER_BAR = 1e-4
let private Q8_WORD_AGREE = 0.9
let private LONG_WORD_AGREE = 0.95
let private PLOG_BAR = 1e-4

struct private FleursClip {
    text : string
}

struct private FleursRefs {
    @rename = "long" long_clips : array<FleursClip>
}

struct private OracleTokens {
    text : string
    ids : array<int>
    frames : array<int>
    n_enc_frames : int
}

def private dump_dir(clip, kind : string) : string => path_join(asr_oracle_dir("gigaam"), "{clip}_{kind}")

//! `rel` of the expectation's largest magnitude, the poisoned expected element that magnitude off
def private stage_bar(rel : float) : Bar => Bar(kind = BarKind.rel_of_max, rel = rel, poison = 1.0, poison_of_max = true)

//! One stage's rows against its dump: the shape, then every element within `bar` of the dump's largest magnitude, the
//! measured max difference logged by `check_rows`.
def private stage_ok(t : T?; tag : string; got : array<float>; want : Npy; rows, cols : int64; bar : float) {
    t |> equal(want.rows, rows, "{tag}: the row count")
    t |> equal(want.cols, cols, "{tag}: the column count")
    return if (want.rows != rows || want.cols != cols)
    check_rows(t, "test_gigaam: {tag}", copy_span(got, 0l, rows * cols), want.data, stage_bar(bar), int(cols))
}


//! the published Q4_K carrier of the e2e transducer: its attention and FF GEMMs Q4_K, its conv pointwise GEMMs Q8_0 - the kq lane's carrier
def private kq_carrier() : string => path_join(models_dir(), "gigaam-v3-e2e-rnnt-q4_k.gguf")

def private dumps_present(t : T?; clip, kind : string) : bool {
    let dir = dump_dir(clip, kind)
    if (!stat("{dir}/tokens.json").is_valid || !stat(asr_clip("{clip}.wav")).is_valid) {
        t |> skip("{clip}_{kind}: oracle dumps or clip not present")
        return false
    }
    return true
}

//! a form of the carrier at `path` minted in memory (`gigaam_form`), under the knobs the caller set
def private gigaam_minted(path : string; f : TowerForm) : GigaamModel {
    let q8 = f.lane == "q8" || f.edit == "q8flag"
    let nohead = f.edit == "nohead" || f.edit == "nohead_served"
    let held = f.zero_block >= 0l || f.edit == "held" || nohead
    var dev = tower_form_dev_of(f, TowerFamily.gigaam)
    if (f.edit == "served" || f.edit == "nohead_served") {
        var inscope probe <- stage_gigaam_model(path, q8, false)
        dev = gigaam_dev_flavor(probe.e)
    }
    var inscope stg <- tower_dev_staged(dev) $ { return <- stage_gigaam_model(path, q8, held, f.edit == "pinned") }
    if (f.zero_block >= 0l) {
        if (stg.e.kq_lane) {
            poison_kq_seats(stg.gemm_seats, f.zero_block * FC_BLOCK_GEMMS, long_length(stg.gemm_seats) / TOWER_SEAT_WORDS, stg.kq, stg.qblob)
        } elif (q8) {
            poison_fc_blocks(stg.qblob, stg.e.n_layer, f.zero_block, stg.e.d_model, stg.e.n_ff) $(b) => gigaam_layer_offsets(stg.e, b)
        } else {
            poison_fc_blocks(stg.wblob, stg.e.n_layer, f.zero_block, stg.e.d_model, stg.e.n_ff) $(b) => gigaam_layer_offsets(stg.e, b)
        }
    }
    if (nohead) {
        for (i in range64(stg.e.ctc_w, stg.e.ctc_w + stg.e.n_classes * stg.e.d_model)) {
            stg.wblob[i] = uint16(0)
        }
    }
    var m = GigaamModel()
    mint_gigaam_model(stg, m, q8, dev)
    return <- m
}

//! One form of a GigaAM carrier as the ASR facade holds it, its block's form loader (`gigaam_form_of`), minted in memory from
//! the file's staging - never the family's load, which bakes a `.dlim`. The lane names the staging's q8 flag (`q8`) or its
//! absence (`f16`, a quantized carrier's `kq` and `f32`); `_z<block>` zeroes the GEMM planes of the poison's blocks from that
//! block on (a kq lane's seats); `@dev` is the image flavor of the family's serving home. The edits: `served` the flavor the
//! lane serves on this box (`gigaam_dev_flavor`), `pinned` the lane pinned on a quantized carrier, `q8flag` the q8 flag on a
//! kq carrier (its planes decide), `held` the held staging, `nohead` the CTC head's halfwords zeroed (`nohead_served` in the
//! served flavor), `portable` the mint under the portable kernel backend, put back after it.
def private gigaam_form(path, form : string) : AsrModel {
    let f = gigaam_form_of(form)
    var inscope am = AsrModel(kind = AsrKind.gigaam)
    if (f.edit == "portable") {
        let pinned = with_kernel_backend("portable") {
            am.ga <- gigaam_minted(path, f)
        }
        if (!pinned) {
            panic("test_gigaam: the portable kernel backend did not pin for the {form} form")
        }
        return <- am
    }
    am.ga <- gigaam_minted(path, f)
    return <- am
}

//! The form is the lane, poison and image flavor it names: on `m`, a carrier loaded as the form `form` of `file`, the lane
//! flags, the poison's block the tower's middle one, and the image flavor - a `served` form's flavor is the box's pick and
//! is not read.
def private gigaam_form_is(t : T?; file, form : string; m : GigaamModel) {
    let f = gigaam_form_of(form)
    t |> success(m.kq_lane == (f.lane == "kq") && m.q8 == (f.lane == "q8"), "{file}: the {form} form serves the lane it names")
    if (f.edit != "served" && f.edit != "nohead_served") {
        form_zero_and_flavor_is(t, file, form, f, TowerFamily.gigaam, m.n_layer, m.dev_backend)
    } elif (f.zero_block >= 0l) {
        t |> equal(f.zero_block, m.n_layer / 2l, "{file}: the {form} form zeroes from the middle block on")
    }
}

//! what the box lacks for the device transducer seat's cells, "" where the build has the seat and the box its device
def private ga_seat_why() : string => rnnt_seat_name() != "" && rnnt_seat_ready() ? "" : "no device transducer seat on this build or box"

//! what the box lacks for the CTC head's Metal seat, "" on a das_metal build under the JIT
def private ga_ctc_metal_why() : string {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return jit_enabled() ? "" : "interpreted (dasLLAMA model tests are JIT-only)"
    } else {
        return "not a Metal build - the CTC head's device seat is Metal's"
    }
}

//! what the box lacks for the CTC head oracle's device leg, "" where the build's device seat serves on the box
def private ga_ctc_seat_why() : string => jit_enabled() && rnnt_seat_ready() ? "" : "no device seat serves the CTC head on this build or box, or interpreted"

//! the kq lane's rows bar over the pinned q8 lane's distance from the same carrier widened to f32: both read the carrier's weights,
//! and the kq lane's Q8_K activations carry one scale per 256 elements where the q8 lane's Q8_0 ones carry one per 32
let private KQ_ROWS_SLACK = 1.5lf

//! the encoder's rows over the whole clip through `m`, one span
def private encoder_rows(m : GigaamModel; samples : array<float>) : array<float> {
    var st = GigaamState()
    let n = gigaam_encode_span(samples, 0l, long_length(samples), m, st)
    var r <- copy_span(st.x, 0l, n * m.d_model)
    delete st
    return <- r
}

//! the whole clip's transcript through `m`, one span
[cold_path]
def private transcript_of(m : GigaamModel; samples : array<float>) : string {
    var st = GigaamState()
    var text = ""
    var lang = ""
    gigaam_transcribe(m, st, samples, $(seg) {
        text += seg.text
    }, lang)
    delete st
    return text
}

//! The f16 lane on one clip against the external reference: stage by stage against its dumps, then the decoded ids against
//! the oracle's; each stage bar's control the same stages over the clip at 1.1x, outside it.
def private reference_clip(t : T?; m : GigaamModel; clip, kind : string) {
    let dir = dump_dir(clip, kind)
    let samples <- load_audio_16k_mono(asr_clip("{clip}.wav"))
    var st = GigaamState()
    let tag = "{clip}_{kind}"
    gigaam_log_mel(samples, m, st)
    let mel <- read_npy("{dir}/mel.npy")
    stage_ok(t, "{tag} mel", st.mel, mel, st.n_len, m.n_mel, MEL_BAR)
    let mel_rows = mel.rows
    if (mel_rows == st.n_len) {
        unsafe {
            memcpy(addr(st.mel[0]), addr(mel.data[0]), mel_rows * m.n_mel * 4l)
        }
    }
    let tt = gigaam_encode_front(m, st)
    let front <- read_npy("{dir}/front.npy")
    stage_ok(t, "{tag} front", st.x, front, tt, m.d_model, FRONT_BAR)
    gigaam_rope_tables(m, st, tt)
    for (l in range64(m.n_layer)) {
        let ln = l < 10l ? "0{l}" : "{l}"
        let want <- read_npy("{dir}/layer_{ln}.npy")
        gigaam_encode_block(m, st, l, tt)
        if (l == 0l) {
            let next <- read_npy("{dir}/layer_01.npy")
            check_exceeds(t, "{tag}: block 0 rows against block 1's dump - the compare tells layers apart",
                copy_span(st.x, 0l, tt * m.d_model), next.data, stage_bar(LAYER_BAR))
        }
        stage_ok(t, "{tag} layer {ln}", st.x, want, tt, m.d_model, LAYER_BAR)
    }
    let enc <- read_npy("{dir}/encoded.npy")
    stage_ok(t, "{tag} encoded", st.x, enc, tt, m.d_model, LAYER_BAR)
    var tk = OracleTokens()
    t |> success(load_json_file("{dir}/tokens.json", tk), "{tag}: tokens.json read")
    t |> equal(int64(tk.n_enc_frames), tt, "{tag}: the encoder frame count")
    with_gpu_knobs() {
        set_serving_gpu_tower(false, TowerFamily.gigaam)   // the CPU chain stage by stage: the device head's own oracle cell is test_gigaam_ctc_head_oracle
        gigaam_decode(m, st, tt)
    }
    let ids <- [for (x in st.toks); x.id]
    let frames <- [for (x in st.toks); int(x.frame)]
    let text = gigaam_detokenize(m, st.toks)
    to_log(LOG_INFO, "test_gigaam: {tag} das  : {text}\n")
    to_log(LOG_INFO, "test_gigaam: {tag} torch: {tk.text}\n")
    t |> success(sequence_equal(ids, tk.ids), "{tag}: the token ids match the oracle's")
    t |> success(sequence_equal(frames, tk.frames), "{tag}: the token frames match the oracle's")
    t |> equal(text, tk.text, "{tag}: the transcript")
    let lp <- m.head == GigaamHead.ctc ? read_npy("{dir}/ctc_logprobs.npy") : Npy()
    if (m.head == GigaamHead.ctc) {
        let worst = ctc_plog_worst(st.toks, lp)
        to_log(LOG_INFO, "test_gigaam: {tag} token probabilities off the oracle's softmax by {worst}\n")
        t |> success(!empty(st.toks) && worst <= PLOG_BAR, "{tag}: token probabilities within {PLOG_BAR} of the oracle's softmax ({worst})")
    } else {
        let first <- [for (x in st.toks); x.plog]
        gigaam_decode(m, st, tt)
        var unstable = 0
        for (a, x in first, st.toks) {
            unstable += a == x.plog ? 0 : 1
        }
        t |> equal(length(st.toks), length(first), "{tag}: a second decode's token count")
        t |> equal(unstable, 0, "{tag}: tokens whose plog moved between two decodes of the same rows")
    }
    stage_controls(t, m, samples, tag, mel, front, enc, lp)
}

//! The stage bars' input-side controls: the clip at 1.1x through the same CPU stages - its mel, front rows and encoded rows each
//! outside the stage's bar of the dump, and on a CTC head its token probabilities past `PLOG_BAR` of the oracle's softmax.
def private stage_controls(t : T?; m : GigaamModel; samples : array<float>; tag : string; mel, front, enc, lp : Npy) {
    var inscope loud <- [for (v in samples); v * 1.1]
    var cs = GigaamState()
    gigaam_log_mel(loud, m, cs)
    check_exceeds(t, "test_gigaam: {tag} the control - the clip at 1.1x off the mel dump", copy_span(cs.mel, 0l, mel.rows * mel.cols), mel.data,
        stage_bar(MEL_BAR))
    let tt = gigaam_encode_front(m, cs)
    check_exceeds(t, "test_gigaam: {tag} the control - the clip at 1.1x off the front dump", copy_span(cs.x, 0l, min(tt, front.rows) * m.d_model),
        front.data, stage_bar(FRONT_BAR))
    gigaam_rope_tables(m, cs, tt)
    for (l in range64(m.n_layer)) {
        gigaam_encode_block(m, cs, l, tt)
    }
    check_exceeds(t, "test_gigaam: {tag} the control - the clip at 1.1x off the encoded dump", copy_span(cs.x, 0l, min(tt, enc.rows) * m.d_model),
        enc.data, stage_bar(LAYER_BAR))
    if (m.head == GigaamHead.ctc) {
        gigaam_decode(m, cs, tt)
        let worst = ctc_plog_worst(cs.toks, lp)
        t |> success(!empty(cs.toks) && is_finite(worst) && worst > PLOG_BAR, "{tag}: the control - the clip at 1.1x lands its token probabilities {worst} off the oracle's softmax, past {PLOG_BAR}")
    }
    delete cs
}

//! The largest difference of a CTC token's probability, exp(plog), from the oracle's softmax at the token's frame and id (a
//! frame past the dump reads as infinite).
def private ctc_plog_worst(toks : array<GigaamToken>; lp : Npy) : float {
    var worst = 0.0
    for (x in toks) {
        let at = x.frame * lp.cols + int64(x.id)
        worst = max(worst, at < long_length(lp.data) ? nan_wide_diff(exp(x.plog), exp(lp.data[at])) : INFINITE_DIFF)
    }
    return worst
}

//! a transcript's lower-cased words with the letter yo read as ye - the spelling Russian references leave free
def private ru_words(text : string) : array<string> {
    var w <- transcript_words(text)
    for (x in w) {
        x = replace(x, "ё", "е")   // nolint:STYLE039 (Cyrillic letters - transcript data, not prose)
    }
    return <- w
}

//! `_model_tier`'s word agreement over the two texts' Russian words (`ru_words`)
def private word_agreement(got, want : string) : float => word_agreement(ru_words(got), ru_words(want))

//! a quantized lane's transcript of `clip` at the bar against the words of `want` (named `want_label`), the clip with its second
//! half silenced the control under it - both transcripts logged
[cold_path]
def private lane_clip(t : T?; m : GigaamModel; clip, kind, lane : string; want, want_label : string) {
    let samples <- load_audio_16k_mono(asr_clip("{clip}.wav"))
    var st = GigaamState()
    var text = ""
    var lang = ""
    let nseg = gigaam_transcribe(m, st, samples, $(seg) {
        text += seg.text
    }, lang)
    delete st
    t |> equal(nseg, 1, "{clip}_{kind} {lane}: one segment under the clip cap")
    t |> equal(lang, "ru", "{clip}_{kind} {lane}: the detected language")
    let agree = word_agreement(text, want)
    to_log(LOG_INFO, "test_gigaam: {clip}_{kind} {lane} words {agree}: {text}\n")
    to_log(LOG_INFO, "test_gigaam: {clip}_{kind} {want_label} : {want}\n")
    t |> success(agree >= Q8_WORD_AGREE, "{clip}_{kind} {lane}: {agree} of the {want_label}'s words")
    var inscope half <- [for (v, i in samples, count()); i < length(samples) / 2 ? v : 0.0]
    let half_text = transcript_of(m, half)
    let half_agree = word_agreement(half_text, want)
    to_log(LOG_INFO, "test_gigaam: {clip}_{kind} {lane} control (second half silenced) words {half_agree}: {half_text}\n")
    t |> success(half_agree < Q8_WORD_AGREE, "{clip}_{kind} {lane}: the control - the clip with its second half silenced reads {half_agree} of the {want_label}'s words, under the bar")
}

//! the q8 lane's transcript of `clip` against the oracle's words
def private q8_clip(t : T?; m : GigaamModel; clip, kind : string) {
    var tk = OracleTokens()
    t |> success(load_json_file("{dump_dir(clip, kind)}/tokens.json", tk), "{clip}_{kind}: tokens.json read")
    lane_clip(t, m, clip, kind, "q8", tk.text, "oracle")
}

// A carrier's lane against the external reference, on its `f16` form stage by stage and on its `q8` form by the
// transcript's words; `clips` the comma list of its clips.
[cold_path]
def private variant_cell(t : T?; m : GigaamModel; form, kind, clips : string) {
    t |> equal(gigaam_exec_fmt(m), form, "{kind}: the {form == "f16" ? "exact" : "q8"} lane's name")
    for (clip in split(clips, ",")) {
        continue if (!dumps_present(t, clip, kind))
        if (form == "f16") {
            reference_clip(t, m, clip, kind)
        } else {
            q8_clip(t, m, clip, kind)
        }
    }
}

[cold_path]
def private test_gigaam_ctc(t : T?; m : GigaamModel; k : GaKept) {
    variant_cell(t, m, k.form, "ctc", "ru_short,ru_mid")
}

//! the kq lane's Q8_0 seats of the carrier at `path` against the file's own blocks: staged held under a GPU flavor's staging (no
//! CPU backend lays the planes), every Q8_0 GEMM's quants and scales the disk block's bit for bit
[cold_path]
def private q8_seats_exact(t : T?; file : string) {
    own_load("asserts on the load itself: the held staging's Q8_0 seats against the file's own blocks, a staging no minted form keeps")
    let path = path_join(models_dir(), file)
    var inscope st <- tower_dev_staged(TowerDev.vulkan) $ { return <- stage_gigaam_model(path, false, true) }
    t |> success(st.e.kq_lane, "the carrier stages on the kq lane")
    let f = fopen(path, "rb")
    var seats = 0
    var off_block = 0l
    fmap(f) $(var bytes : array<uint8>#) {
        var inscope m <- parse_gguf_meta(bytes)
        for (j in st.plan.jobs) {
            continue if (j.plane != GigaamPlane.q8 || j.fmt < 0)
            seats++
            let ti = find_index_if(m.tensors) $(x) => x.name == j.name
            let base = m.data_offset + m.tensors[ti].offset
            for (blk in range64(j.n / 32l)) {
                let bo = base + blk * 34l
                off_block += st.qscales[j.off / 32l + blk] != f16_to_f32(uint(bytes[bo]) | (uint(bytes[bo + 1l]) << 8u)) ? 1l : 0l
                for (i in range64(32l)) {
                    off_block += st.qblob[j.off + blk * 32l + i] != int8(bytes[bo + 2l + i]) ? 1l : 0l
                }
            }
        }
    }
    fclose(f)
    t |> success(seats > 0, "the carrier holds Q8_0 seats ({seats})")
    t |> equal(off_block, 0l, "every Q8_0 seat's quants and scales are the file's blocks")
}

//! the f16 e2e carrier's exact lane as the quantized carriers' cells read it: the clips' transcripts, and ru_short's
//! encoder rows
struct private ExactRefs {
    texts : table<string; string>
    rows : array<float>
}

//! What the forms of the e2e transducer's carriers keep for the cells and compares of the quantized ones: `form` the form
//! whose cells run, the f16 carrier's references, the Q4_K carrier's lane name, transcript and rows under the q8 flag and
//! the pins, and the mixed carrier's streamed and held mints' transcripts.
struct private E2eKept {
    form : string
    refs : ExactRefs
    fmt : string
    text : string
    fmt8 : string
    text8 : string
    xw : array<float>
    xk : array<float>
    xq : array<float>
    mix_text : string
    mix_held_text : string
}

// The quantized carriers' reference, on the f16 e2e carrier's `f16` form: the exact lane's transcript of each clip present
// (the lane the oracle holds stage by stage, so their cells run with no oracle dumps) and ru_short's encoder rows.
[cold_path]
def private exact_reference_kept(t : T?; m : GigaamModel; var k : E2eKept) {
    for (clip in ["ru_short", "ru_mid"]) {
        let wav = asr_clip("{clip}.wav")
        if (!stat(wav).is_valid) {
            t |> skip("{clip}.wav not present - the f16 lane's reference transcript")
            continue
        }
        var inscope samples <- load_audio_16k_mono(wav)
        k.refs.texts[clip] = transcript_of(m, samples)
        if (clip == "ru_short") {
            delete k.refs.rows
            k.refs.rows <- encoder_rows(m, samples)
        }
    }
}

//! the kq-lane cells' reference: the f16 carrier's exact-lane transcript of `clip`, as its block kept it
def private exact_reference(t : T?; refs : ExactRefs; clip : string; var want : string&) : bool {
    if (!key_exists(refs.texts, clip)) {
        t |> skip("{clip}: no reference transcript - {E2E_RNNT} or {clip}.wav not present")
        return false
    }
    want = refs.texts?[clip] ?? ""
    return true
}

// The kq lane over the published Q4_K carrier - the hosted file whose attention and FF GEMMs are Q4_K and conv pointwise
// GEMMs Q8_0: on its `kq` form the carrier's own planes serve, named by their one K-quant member; on its `kq@dev` form the
// serving tower's flavor serves it with no decline. Every lane's transcript holds the q8 bar against the f16 carrier's
// exact lane.
[cold_path]
def private test_gigaam_kq_lane(t : T?; m : GigaamModel; k : E2eKept) {
    if (k.form == "kq") {
        t |> success(m.kq_lane, "the un-pinned load takes the kq lane")
        t |> equal(gigaam_exec_fmt(m), "k4", "the lane's name: the one K-quant member")
        t |> equal(gigaam_kq_members(m), "k4+q8", "the carrier's members: Q4_K attention and FF, Q8_0 conv pointwise")
        t |> success(!gigaam_device_gpu(m), "the device-lane report: the CPU image's encode is the CPU's")
        for (clip in split("ru_short,ru_mid", ",")) {
            var want = ""
            if (exact_reference(t, k.refs, clip, want)) {
                lane_clip(t, m, clip, "e2e_rnnt", "kq", want, "f16 lane")
            }
        }
        return
    }
    with_gpu_knobs() {
        set_serving_gpu_tower(true, TowerFamily.gigaam)
        let dev = gigaam_dev_flavor(GigaamModel(kq_lane = true))
        t |> equal(dev, serving_tower_dev(TowerFamily.gigaam), "the serving tower's flavor serves the kq lane where its device stands up")
        return if (dev == TowerDev.none)
        t |> success(m.kq_lane && m.dev_backend == dev && gigaam_device_gpu(m), "the {dev}-flavored image of the kq lane: the device-lane report names the device")
        let declines = gpu_tower_declines_of("kq_lane", TowerFamily.gigaam)
        var want = ""
        if (exact_reference(t, k.refs, "ru_short", want)) {
            let e0 = gpu_tower_encodes(TowerFamily.gigaam)
            lane_clip(t, m, "ru_short", "e2e_rnnt", "kq {dev}", want, "f16 lane")
            t |> success(gpu_tower_encodes(TowerFamily.gigaam) > e0, "the device served the kq lane's encodes ({gpu_tower_encodes(TowerFamily.gigaam) - e0})")
            t |> equal(gpu_tower_declines_of("kq_lane", TowerFamily.gigaam), declines, "no kq_lane decline: every member of the carrier has a tile on the device")
        }
    }
}

// The Q4_K carrier under the lane flag, a leg a form: on `kq` the un-pinned mint's lane name, transcript and encoder rows
// of ru_short; on `kq+q8flag` the same off the mint that took the q8 flag.
[cold_path]
def private kq_flag_leg(t : T?; m : GigaamModel; var k : E2eKept) {
    let clip = asr_clip("ru_short.wav")
    var inscope samples <- load_audio_16k_mono(clip)
    if (k.form == "kq") {
        k.fmt = gigaam_exec_fmt(m)
        if (!empty(samples)) {
            k.text = transcript_of(m, samples)
            delete k.xk
            k.xk <- encoder_rows(m, samples)
        }
    } else {
        t |> success(m.kq_lane && !m.q8, "asked for q8, the kq carrier still takes the kq lane")
        k.fmt8 = gigaam_exec_fmt(m)
        if (!empty(samples)) {
            k.text8 = transcript_of(m, samples)
        }
    }
}

// The q8 flag a kq carrier's staging and mint take changes nothing, after the block: the lane's name and the transcript
// the q8=false mint's.
[cold_path]
def private kq_flag_held(t : T?; k : E2eKept) {
    t |> success(!empty(k.fmt) && !empty(k.fmt8), "both mints of the Q4_K carrier kept their lane's name")
    t |> equal(k.fmt8, k.fmt, "the lane's name the q8=false mint's")
    if (!asr_clip_present("ru_short.wav")) {
        t |> skip("ru_short.wav not present - the transcript compare")
        return
    }
    t |> equal(k.text8, k.text, "the transcript the q8=false mint's")
}

//! a pinned lane's legs on the Q4_K carrier: off the kq lane, the lane named `lane` (`what` the assert's words), ru_short
//! transcribed against the exact reference; its encoder rows of ru_short answered, empty where the corpus lacks the clip
def private kq_pinned_rows(t : T?; m : GigaamModel; k : E2eKept; lane, what : string) : array<float> {
    t |> success(!m.kq_lane, "the pin keeps the carrier off the kq lane")
    t |> equal(gigaam_exec_fmt(m), lane, what)
    var want = ""
    if (exact_reference(t, k.refs, "ru_short", want)) {
        lane_clip(t, m, "ru_short", "e2e_rnnt", lane, want, "f16 lane")
    }
    var inscope samples <- load_audio_16k_mono(asr_clip("ru_short.wav"))
    return <- array<float>() if (empty(samples))
    return <- encoder_rows(m, samples)
}

// A pinned q8 lane dequantizes the Q4_K carrier into Q8_0 planes, on the `q8+pinned` form; its encoder rows of ru_short
// are kept for the rows compare.
[cold_path]
def private kq_pinned_q8_cell(t : T?; m : GigaamModel; var k : E2eKept) {
    var inscope rows <- kq_pinned_rows(t, m, k, "q8", "the pinned lane's name")
    if (!empty(rows)) {
        delete k.xq
        k.xq <- rows
    }
}

// A pinned exact lane widens the Q4_K carrier to f32, on the `f32+pinned` form; its encoder rows of ru_short are kept for
// the rows compare.
[cold_path]
def private kq_pinned_exact_cell(t : T?; m : GigaamModel; var k : E2eKept) {
    var inscope rows <- kq_pinned_rows(t, m, k, "f32", "the pinned lane's name: no f16 block GEMMs to serve")
    if (!empty(rows)) {
        delete k.xw
        k.xw <- rows
    }
}

// The kq lane's encoder rows sit no further from the carrier widened to f32 than the pinned q8 lane's requant of it, after
// the block, on the rows three forms kept; the control a chain on the f16 carrier's weights, outside the bar.
[cold_path]
def private kq_rows_held(t : T?; k : E2eKept) {
    if (!asr_clip_present("ru_short.wav")) {
        t |> skip("ru_short.wav not present - the rows compare")
        return
    }
    var sk, sq = TowerParityStats()
    return if (!tower_parity_stats(t, "kq lane vs widened", k.xw, k.xk, sk) || !tower_parity_stats(t, "q8 lane vs widened", k.xw, k.xq, sq))
    to_log(LOG_INFO, "test_gigaam: q4_k rows rel_l2 from the widened carrier - kq lane {sk.rel_l2}, pinned q8 lane {sq.rel_l2}\n")
    t |> success(is_finite(sk.rel_l2) && sk.rel_l2 > 0.0lf, "the kq lane's activation quants move its rows off the widened chain's - two chains compared ({sk.rel_l2})")
    let bar = KQ_ROWS_SLACK * sq.rel_l2
    t |> success(sk.rel_l2 <= bar, "the kq lane's rows within {KQ_ROWS_SLACK}x the q8 lane's distance from the widened carrier ({sk.rel_l2} vs {bar})")
    if (empty(k.refs.rows)) {
        t |> skip("{E2E_RNNT}'s block kept no rows - the control")
        return
    }
    var sf = TowerParityStats()
    return if (!tower_parity_stats(t, "f16 carrier vs widened q4_k", k.xw, k.refs.rows, sf))
    to_log(LOG_INFO, "test_gigaam: the f16 carrier's exact rows rel_l2 from the widened q4_k carrier {sf.rel_l2}\n")
    t |> success(is_finite(sf.rel_l2) && sf.rel_l2 > bar, "the control - a chain on the f16 carrier's weights lands outside the bar ({sf.rel_l2} vs {bar})")
}

//! The Q4_K carrier under the family's pin: the pin reaches the facade load, and un-pinned the facade load takes the kq lane.
[test(description = "On the Q4_K GigaAM carrier the family's q8 pin reaches the facade load, which serves the q8 lane, and un-pinned the facade load takes the kq lane")]
def test_gigaam_kq_lane_pins(tst : T?) {
    return if (!family_on(tst, "gigaam"))
    with_dasllama_jobque_() {
        let path = path_join(models_dir(), "gigaam-v3-e2e-rnnt-q4_k.gguf")
        return if (!model_available(tst, path))
        tst |> run("e2e_rnnt q4_k: the family's pin reaches the facade load - set_gigaam_q8 serves the q8 lane") @(t : T?) {
            own_load("reloads the model: the facade's load under the family's pin and again un-pinned, each cold mint landing in scratch")
            set_asr_fp32(false)
            set_asr_tower_fp32(false)
            set_gigaam_q8(true)
            with_image_destination(path_join(test_temp_dir(), "ga_pin_q8.dlim")) {   // each cold mint lands in scratch, not beside the carrier
                var inscope am <- load_asr_model(path)
                t |> equal(asr_exec_fmt(am), "q8", "the pinned facade load's lane")
            }
            reset_gigaam_q8()
            with_image_destination(path_join(test_temp_dir(), "ga_pin_kq.dlim")) {
                var inscope au <- load_asr_model(path)
                t |> equal(asr_exec_fmt(au), "k4", "un-pinned, the facade load takes the kq lane (the control)")
            }
            remove(path_join(test_temp_dir(), "ga_pin_q8.dlim"))
            remove(path_join(test_temp_dir(), "ga_pin_kq.dlim"))
        }
    }
}

//! The facade's fp32 pin over the Q4_K carrier: the exact lane widens the carrier to f32, which no GPU chain reads, so the load
//! maps the CPU image whatever the box serves.
[test(description = "The facade fp32 pin on the Q4_K GigaAM carrier loads the widened f32 lane on the CPU image, never a GPU flavor, and still transcribes the clip")]
def test_gigaam_fp32_pin_load(tst : T?) {
    with_dasllama_jobque_() {
        let path = path_join(models_dir(), "gigaam-v3-e2e-rnnt-q4_k.gguf")
        return if (!model_available(tst, path))
        tst |> run("e2e_rnnt q4_k: the facade's fp32 pin loads the widened carrier on the CPU image - no GPU chain reads f32 block GEMMs") @(t : T?) {
            own_load("asserts on the load itself: the lane and the image flavor the facade's fp32 pin loads, minted into scratch")
            let clip = asr_clip("ru_short.wav")
            if (!stat(clip).is_valid) {
                t |> skip("ru_short.wav not present - the transcript leg")
                return
            }
            let dlim = path_join(test_temp_dir(), "ga_fp32_pin.dlim")
            with_image_destination(dlim) {   // the cold mint lands in scratch, not beside the carrier
                var inscope m <- load_gigaam_model(path, false, true)
                t |> equal(gigaam_exec_fmt(m), "f32", "the pin widens the carrier's planes")
                t |> equal(m.dev_backend, TowerDev.none, "and the load maps the CPU image")
                t |> success(!empty(transcript_of(m, load_audio_16k_mono(clip))), "the widened lane transcribes")
            }
            remove(dlim)
        }
    }
}

//! the GPU flavor a load of `path` under the serving tower mints, read into memory: its bytes are refused by name
def private gpu_flavor_refused(t : T?) {
    own_load("asserts on the load itself - its refusal: a GPU flavor's bytes in memory are refused by name")
    let path = path_join(models_dir(), "gigaam-v3-e2e-rnnt-q4_k.gguf")
    if (!serving_tower_device_up(TowerFamily.gigaam)) {
        t |> skip("no GPU device stands up for the serving tower - no GPU flavor to mint")
        return
    }
    with_gpu_knobs() {
        set_serving_gpu_tower(true, TowerFamily.gigaam)
        let dlim = path_join(test_temp_dir(), "ga_memory_dev.dlim")
        var flavored = false
        with_image_destination(dlim) {
            var inscope m <- load_gigaam_model(path)
            flavored = m.dev_backend != TowerDev.none
        }
        t |> success(flavored, "the load under the serving tower mints the GPU flavor")
        var raw : array<uint8>
        var size = 0ul
        let base = read_page_aligned(dlim, raw, size)
        remove(dlim)
        t |> success(base != null, "the flavor's image reads into memory")
        if (flavored && base != null) {
            let refused = panic_text() {
                var inscope bad <- load_gigaam_model(base, size)
            }
            t |> success(find(refused, "serves on the CPU alone") >= 0, "the flavor is refused by name: '{refused}'")
        }
        delete raw
    }
}

[test(description = "A GigaAM CPU image its host holds in memory loads from the bytes on the lane it was baked on and transcribes the clip as the image's file does, `load_asr_model` routes the bytes to gigaam, a base off the 16 KiB boundary is refused by name and so is a GPU flavor's bytes, and with its planes zeroed the load transcribes otherwise")]
def test_gigaam_from_memory(tst : T?) {
    with_dasllama_jobque_() {
        let path = path_join(models_dir(), "gigaam-v3-e2e-rnnt-q4_k.gguf")
        return if (!jit_model_available(tst, path))
        tst |> run("e2e_rnnt q4_k: the CPU image's bytes, borrowed in place, serve the transcript its file serves") @(t : T?) {
            own_load("reloads the model: the cold mint lands in scratch, then loads from the bytes in memory, off the page, routed and poisoned")
            let clip = asr_clip("ru_short.wav")
            if (!stat(clip).is_valid) {
                t |> skip("ru_short.wav not present - the transcript leg")
                return
            }
            with_gpu_knobs() {
                set_serving_gpu_tower(false, TowerFamily.gigaam)   // the CPU image: the one an image in memory serves
                let dlim = path_join(test_temp_dir(), "ga_memory.dlim")
                var from_file = ""
                var lane = ""
                with_image_destination(dlim) {   // the cold mint lands in scratch, not beside the carrier
                    var inscope m <- load_gigaam_model(path)
                    from_file = transcript_of(m, load_audio_16k_mono(clip))
                    lane = gigaam_exec_fmt(m)
                }
                t |> success(!empty(from_file), "the image's file transcribes the clip")
                var raw : array<uint8>
                var size = 0ul
                let base = read_page_aligned(dlim, raw, size)
                remove(dlim)
                t |> success(base != null, "the image reads into memory")
                if (base != null) {
                    {
                        var inscope m <- load_gigaam_model(base, size)
                        t |> equal(gigaam_exec_fmt(m), lane, "the bytes load on the lane the image was baked on")
                        let from_memory = transcript_of(m, load_audio_16k_mono(clip))
                        to_log(LOG_INFO, "gigaam from memory file: {from_file}\n")
                        to_log(LOG_INFO, "gigaam from memory bytes: {from_memory}\n")
                        t |> equal(from_memory, from_file, "and transcribe as the file does, with the file gone")
                    }
                    let off_page = unsafe(reinterpret<void?>(intptr(base) + 64ul))
                    let refused = panic_text() {
                        var inscope bad <- load_gigaam_model(off_page, size)
                    }
                    t |> success(find(refused, "starts on a {IMAGE_PAGE}-byte boundary") >= 0, "a base off the page is refused by name: '{refused}'")
                    {
                        var inscope routed <- load_asr_model(base, size)
                        t |> success(routed.kind == AsrKind.gigaam, "the ASR loader routes the bytes to gigaam")
                    }
                    zero_planes(base, size)
                    var inscope poisoned <- load_gigaam_model(base, size)
                    let other = transcript_of(poisoned, load_audio_16k_mono(clip))
                    to_log(LOG_INFO, "gigaam from memory zeroed: {other}\n")
                    t |> success(other != from_file, "its planes zeroed, the load transcribes otherwise (the control)")
                }
                delete raw
            }
        }
        tst |> run("e2e_rnnt q4_k: a GPU flavor's bytes are refused by name") @(t : T?) {
            gpu_flavor_refused(t)
        }
    }
}

// The kq lane over the local mint mixing two K-quant members (Q4_K attention, Q6_K FF): on its `kq` form the carrier's
// own planes serve named `kq`, the transcript within the word bar; on its `kq+held` form the held staging's mint
// transcribes ru_short.
[cold_path]
def private test_gigaam_kq_other_carriers(t : T?; m : GigaamModel; var k : E2eKept) {
    var inscope samples <- load_audio_16k_mono(asr_clip("ru_short.wav"))
    if (k.form == "kq+held") {
        // the held staging writes every member at the plan's own offsets, each on its format's strides
        if (!empty(samples)) {
            k.mix_held_text = transcript_of(m, samples)
        }
        return
    }
    t |> success(m.kq_lane, "the un-pinned load takes the kq lane")
    t |> equal(gigaam_exec_fmt(m), "kq", "the lane's name where the K-quant members mix")
    t |> equal(gigaam_kq_members(m), "k4+k6+q8", "the members: Q4_K attention, Q6_K FF, Q8_0 conv pointwise")
    var want = ""
    if (exact_reference(t, k.refs, "ru_short", want)) {
        lane_clip(t, m, "ru_short", "e2e_rnnt", "kq mix", want, "f16 lane")
    }
    if (!empty(samples)) {
        k.mix_text = transcript_of(m, samples)
    }
}

// The mixed carrier's held staging mints the streamed mint's model, after the block: the same transcript.
[cold_path]
def private kq_mix_held(t : T?; k : E2eKept) {
    if (!asr_clip_present("ru_short.wav")) {
        t |> skip("ru_short.wav not present - the transcript compare")
        return
    }
    t |> success(!empty(k.mix_text) && !empty(k.mix_held_text), "the streamed and the held mint kept their transcripts")
    t |> equal(k.mix_held_text, k.mix_text, "the held staging mints the streamed mint's model: the same transcript")
}

// The Q8_0 carrier, whose every block GEMM is Q8_0, on its `kq` form: the kq lane serves them, named `q8`.
[cold_path]
def private kq_q8_0_cell(t : T?; m : GigaamModel; k : E2eKept) {
    t |> success(m.kq_lane, "the un-pinned load takes the kq lane")
    t |> equal(gigaam_exec_fmt(m), "q8", "the lane's name where every seat is Q8_0")
    t |> equal(gigaam_kq_members(m), "q8", "the carrier's one member")
    var want = ""
    if (exact_reference(t, k.refs, "ru_short", want)) {
        lane_clip(t, m, "ru_short", "e2e_rnnt", "kq q8", want, "f16 lane")
    }
}

// The Q4_K carrier's load on the box's GPU flavor (`#gigaam-planes`): the kq lane on the device where its chain serves
// that lane, else the kq lane on the CPU image - a Vulkan device off the f16 GEMM feed (`DASLLAMA_COOPMAT=sdot4`) among
// them; the transcript holds the word bar against the f16 carrier's exact lane.
[cold_path]
def private test_gigaam_kq_load_flavor(t : T?; refs : ExactRefs) {
    own_load("asserts on the load itself: the flavor the family's load lands on under the serving tower, minted into scratch")
    let path = path_join(models_dir(), "gigaam-v3-e2e-rnnt-q4_k.gguf")
    reset_gigaam_q8()
    with_gpu_knobs() {
        set_serving_gpu_tower(true, TowerFamily.gigaam)
        let kq_dev = serving_tower_device_up(TowerFamily.gigaam) ? serving_tower_dev(TowerFamily.gigaam) : TowerDev.none   // the box fact: the device the chain rides stands up
        let dlim = path_join(test_temp_dir(), "ga_load_flavor.dlim")
        with_image_destination(dlim) {   // the cold mint lands in scratch, not beside the carrier
            var inscope m <- load_gigaam_model(path)
            to_log(LOG_INFO, "test_gigaam: the box's device {kq_dev}: loaded {gigaam_exec_fmt(m)} on {m.dev_backend}\n")
            if (kq_dev != TowerDev.none) {
                t |> success(m.kq_lane && m.dev_backend == kq_dev, "the chain serves the kq lane: the carrier's own planes on the device")
            } else {
                t |> success(m.kq_lane && m.dev_backend == TowerDev.none, "no device the chain rides: the kq lane on the CPU image")
            }
            var want = ""
            if (exact_reference(t, refs, "ru_short", want)) {
                lane_clip(t, m, "ru_short", "e2e_rnnt", gigaam_exec_fmt(m), want, "f16 lane")
            }
        }
        remove(dlim)
    }
}

//! The kq lane planner's refusals, each over a copy of the Q4_K carrier with tensors retyped (`gguf_retyped_copy`): a float
//! GEMM among the quantized ones, a member the lane has no seat for (a per-32 Q5_1, NVFP4), q, k and v across two classes, and a
//! file that does not read; the lane peek's float disjuncts - BF16 and F32 GEMMs are the exact lane's, not the kq lane's.
[test(description = "Over retyped copies of the Q4_K carrier the kq planner refuses a float GEMM among quantized ones, a Q5_1 or NVFP4 seat, and q in Q8_0 beside K-quant k and v, each by name, stages an unchanged copy, and the lane peek tags BF16 and F32 GEMMs as the exact lane")]
def test_gigaam_kq_refusals(tst : T?) {
    own_load("asserts on the load itself - its refusal: the kq planner over copies of the carrier with tensors retyped")
    let path = path_join(models_dir(), "gigaam-v3-e2e-rnnt-q4_k.gguf")
    return if (!model_available(tst, path))
    let l0 = "encoder.layers.0."
    tst |> run("the kq carrier's planner refuses what its lane cannot seat") @(t : T?) {
        let mixed = gguf_retyped_refusal(kq_carrier(), "ga_mixed", { "{l0}ff1.linear1.weight" => GGML_TYPE_F16 }) $(p) {
            gigaam_lane_tag(p, false, false)
        }
        t |> success(find(mixed, "mix quantized and float tensors") >= 0, "a float GEMM among quantized ones refused at the lane peek: '{mixed}'")
        for (gt in [GGML_TYPE_Q5_1, GGML_TYPE_NVFP4]) {
            let why = gguf_retyped_refusal(kq_carrier(), "ga_seat_{gt}", { "{l0}ff1.linear1.weight" => gt }) $(p) {
                var inscope st <- stage_gigaam_model(p)
            }
            t |> success(find(why, "has no seat for") >= 0 && find(why, "{l0}ff1.linear1.weight") >= 0, "a {ggml_type_name(gt)} GEMM refused at its seat naming the tensor: '{why}'")
        }
        let qkv = gguf_retyped_refusal(kq_carrier(), "ga_qkv", { "{l0}attn.q.weight" => GGML_TYPE_Q8_0 }) $(p) {
            var inscope st <- stage_gigaam_model(p)
        }
        t |> success(find(qkv, "mix Q8_0 with a K-quant format") >= 0, "q in Q8_0 beside a Q4_K k and v refused: '{qkv}'")
        let nothing : table<string; int>
        let none = gguf_retyped_refusal(kq_carrier(), "ga_clean", nothing) $(p) {
            var inscope st <- stage_gigaam_model(p)
        }
        t |> equal(none, "", "the copy with nothing retyped stages (the control)")
        let unread = panic_text() $ {
            gigaam_lane_tag(path_join(test_temp_dir(), "ga_absent.gguf"), false, false)
        }
        t |> success(find(unread, "cannot read model") >= 0, "a file that does not read refused at the lane peek: '{unread}'")
    }
    tst |> run("the lane peek reads BF16 and F32 block GEMMs as the exact lane's") @(t : T?) {
        let gemms <- ["ff1.linear1", "ff1.linear2", "attn.q", "attn.k", "attn.v", "attn.out", "conv.pw1", "conv.pw2", "ff2.linear1", "ff2.linear2"]
        var retype <- {for (i in range(16 * length(gemms))); "encoder.layers.{i / length(gemms)}.{gemms[i % length(gemms)]}.weight" => GGML_TYPE_BF16}
        retype["{l0}attn.q.weight"] = GGML_TYPE_F32
        let dst = path_join(test_temp_dir(), "ga_float.gguf")
        gguf_retyped_copy(path, dst, retype)
        t |> equal(gigaam_lane_tag(dst, false, false), "gigaam-f16", "BF16 and F32 GEMMs: the exact lane's tag")
        t |> equal(gigaam_lane_tag(path, false, false), "gigaam-kq", "the Q4_K carrier: the kq lane's tag (the control)")
        remove(dst)
        delete retype
    }
}

//! `tower_mm`'s form without the Q8_K sums refuses a kq-lane model - its K-quant seats read them - model-free.
[test(description = "The `tower_mm` form without Q8_K sums panics on a kq-lane model, naming the form with the sums it must take")]
def test_gigaam_tower_mm_needs_the_sums(t : T?) {
    t |> run("the tower_mm form with no Q8_K sums refuses a kq-lane model") @(t : T?) {
        let m = GigaamModel(kq_lane = true)
        var y, x : array<float>
        var xq : array<int8>
        var xs : array<float>
        let why = panic_text() $ {
            tower_mm(m, y, 0l, x, xq, xs, 256l, 1l, 1l)
        }
        t |> success(find(why, "takes the tower_mm form with the Q8_K sums") >= 0, "refused naming the form: '{why}'")
    }
}

//! The kq lane's seat index over the chain's virtual offsets: every block GEMM of `gigaam_layer_offsets`'s record maps to its
//! place in layout order, layer by layer - model-free, over the v3 geometry.
[test(description = "Over the v3 geometry `fc_gemm_seat_index` maps each of every layer's ten block GEMM virtual offsets, and the last GEMM's last element, to its seat in layout order")]
def test_gigaam_seat_index(t : T?) {
    t |> run("the ten block GEMMs' virtual offsets map to their seats in layout order") @(t : T?) {
        var m = GigaamModel(d_model = 768l, n_ff = 3072l, n_layer = 16l, conv_kernel = 5l)
        m.gemm_stride = gigaam_gemm_stride(m.d_model, m.n_ff)
        m.aux_stride = 1l
        for (l in range64(m.n_layer)) {
            let lo = gigaam_layer_offsets(m, l)
            let woffs = fixed_array(lo.ff1a_w, lo.ff1b_w, lo.q_w, lo.k_w, lo.v_w, lo.o_w, lo.pw1_w, lo.pw2_w, lo.ff2a_w, lo.ff2b_w)
            for (wo, i in woffs, count()) {
                t |> equal(fc_gemm_seat_index(m.d_model, m.n_ff, m.gemm_off, m.gemm_stride, wo), l * FC_BLOCK_GEMMS + int64(i), "layer {l} GEMM {i}")
            }
            t |> equal(fc_gemm_seat_index(m.d_model, m.n_ff, m.gemm_off, m.gemm_stride, lo.ff2b_w + m.d_model * m.n_ff - 1l), l * FC_BLOCK_GEMMS + FC_BLOCK_GEMMS - 1l, "layer {l}: the last GEMM's last element")
        }
        delete m
    }
}

[cold_path]
def private test_gigaam_e2e_rnnt(t : T?; m : GigaamModel; k : GaKept) {
    variant_cell(t, m, k.form, "e2e_rnnt", "ru_short,ru_mid")
}

[cold_path]
def private test_gigaam_e2e_ctc(t : T?; m : GigaamModel; k : GaKept) {
    variant_cell(t, m, k.form, "e2e_ctc", "ru_short")
}

[cold_path]
def private test_gigaam_charwise_rnnt(t : T?; m : GigaamModel; k : GaKept) {
    variant_cell(t, m, k.form, "rnnt", "ru_short")
}

// A long clip past the cap splits at silences, on the f16 e2e carrier's `f16` form.
[cold_path]
def private test_gigaam_long_form(t : T?; m : GigaamModel) {
    let wav = asr_clip("ru_long.wav")
    let refs_path = asr_clip("ru_fleurs.json")
    if (!stat(wav).is_valid || !stat(refs_path).is_valid) {
        t |> skip("ru_long.wav or ru_fleurs.json not present")
        return
    }
    var refs = FleursRefs()
    t |> success(load_json_file(refs_path, refs), "ru_fleurs.json read")
    let reference = join([for (c in refs.long_clips); c.text], " ")
    let samples <- load_audio_16k_mono(wav)
    var st = GigaamState()
    let spans <- gigaam_chunks(st, samples)
    t |> success(length(spans) > 1, "ru_long: {length(spans)} spans")
    var last_end = 0l
    for (sp in spans) {
        t |> success(sp.end - sp.start <= GIGAAM_MAX_CLIP_S * AUDIO_SAMPLE_RATE, "ru_long: span [{sp.start}, {sp.end}) within the cap")
        t |> success(sp.start >= last_end, "ru_long: spans ascend without overlap")
        last_end = sp.end
    }
    var segs : array<TranscribeSegment>
    var lang = ""
    let n = gigaam_transcribe(m, st, samples, $(seg) {
        segs |> push_clone(seg)
    }, lang)
    t |> equal(n, length(spans), "ru_long: one segment a span")
    for (s, sp in segs, spans) {
        t |> equal(s.t0, sp.start * 100l / AUDIO_SAMPLE_RATE, "ru_long: a segment starts at its span")
        to_log(LOG_INFO, "test_gigaam: ru_long [{s.t0}, {s.t1}] cs: {s.text}\n")
        t |> success(!empty(s.text), "ru_long: segment [{s.t0}, {s.t1}] carries text")
    }
    let text = join([for (s in segs); s.text], " ")
    let agree = word_agreement(text, reference)
    to_log(LOG_INFO, "test_gigaam: ru_long words {agree} of {length(ru_words(reference))} reference words\n")
    t |> success(agree >= LONG_WORD_AGREE, "ru_long: {agree} of the references' words, in order")
    let shuffled = join([for (i in range(length(refs.long_clips))); refs.long_clips[length(refs.long_clips) - 1 - i].text], " ")
    let control = word_agreement(text, shuffled)
    to_log(LOG_INFO, "test_gigaam: ru_long control (references in reverse order) {control}\n")
    t |> success(control < LONG_WORD_AGREE, "ru_long: the references in reverse order miss the bar ({control})")
}

[test(description = "Through the ASR facade a decoder GGUF needs its mmproj")]
def test_gigaam_facade(tst : T?) {
    tst |> run("a decoder GGUF needs its mmproj, where the gigaam GGUF loads alone") @(t : T?) {
        let decoder = path_join(models_dir(), "SmolLM2-135M-Instruct-Q8_0.gguf")
        return if (!model_available(t, decoder))
        t |> success(asr_needs_mmproj(decoder), "a decoder GGUF needs its mmproj")
    }
}

// The facade's gigaam arms, on the f16 e2e carrier's `f16` form: the one-file carrier, its caps, the session's language
// and the family's own transcript.
[cold_path]
def private gigaam_facade_arms(t : T?; m : AsrModel) {
    return if (!jit_only(t))
    let wav = asr_clip("ru_short.wav")
    if (!stat(wav).is_valid) {
        t |> skip("ru_short.wav not present")
        return
    }
    t |> success(!asr_needs_mmproj(path_join(models_dir(), E2E_RNNT)), "the gigaam GGUF loads alone")
    var inscope c <- caps(m)
    t |> equal(join(c.languages, ","), "ru", "the caps' languages")
    t |> success(c.timestamps == AsrTimestamps.segment && !c.translate && !c.streaming, "segment timestamps, no translate, whole-clip")
    t |> equal(asr_exec_fmt(m), "f16", "the facade's exec format is the family's")
    let refused = panic_text() ${
        var inscope en <- create_session(m, "en")
    }
    t |> success(find(refused, "transcribes Russian (ru)") >= 0, "an English session is refused: {refused}")
    let samples <- load_audio_16k_mono(wav)
    var inscope s <- create_session(m, "")
    var segs : array<TranscribeSegment>
    let n = transcribe(m, s, samples) $(seg) {
        segs |> push_clone(seg)
    }
    var st = GigaamState()
    var lang = ""
    var direct = ""
    gigaam_transcribe(m.ga, st, samples, $(seg) {
        direct += seg.text
    }, lang)
    t |> equal(n, 1, "one segment under the clip cap")
    t |> equal(s.detected_lang, "ru", "the detected language")
    t |> success(n == 1 && segs[0].text == direct && !empty(direct), "the facade's transcript is the family's: '{direct}'")
    for (seg in segs) {
        t |> success(is_finite(seg.avg_logprob) && seg.avg_logprob <= 0.0,
            "a finite mean log-probability at or under 0 ({seg.avg_logprob})")
    }
}

//! The v3 CTC carrier's header, every field one the loader takes.
def private good_hparams() : GigaamHparams {
    return GigaamHparams(sample_rate = AUDIO_SAMPLE_RATE, mel_center = false, attention = "rotary", conv_norm = "layer_norm",
        n_fft = 320l, win_length = 320l, d_model = 768l, n_head = 16l, head_dim = 48l, n_ff = 3072l, subsampling = 4l,
        n_sub_ch = 768l, subs_kernel = 5l, conv_kernel = 5l, n_classes = 34l, n_vocab = 33l, blank_id = 33l,
        head = "ctc", tokenizer = "charwise", n_tokens = 33l, pred_layers = 1l)
}

def private hparams_refusal(h : GigaamHparams) : string {
    return panic_text() ${
        gigaam_check_hparams(h)
    }
}

def private refused_with(t : T?; h : GigaamHparams; fragment, what : string) {
    let why = hparams_refusal(h)
    t |> success(find(why, fragment) >= 0, "{what}: refused naming '{fragment}', got '{why}'")
}

//! the v3 header with `edit` applied must be refused naming `fragment`
def private refused_field(t : T?; fragment : string; edit : block<(var h : GigaamHparams) : void>) {
    var h = good_hparams()
    invoke(edit, h)
    refused_with(t, h, fragment, fragment)
}

[test(description = "`gigaam_check_hparams` accepts the v3 CTC and one-layer RNNT headers and refuses, naming the field, a head neither ctc nor rnnt, a two-layer predictor, d_model or ff_dim off the 32 lattice, and an odd head_dim")]
def test_gigaam_hparam_refusals(t : T?) {
    t |> run("the v3 headers are taken: ctc, and rnnt with its one predictor layer") @(t : T?) {
        t |> equal(hparams_refusal(good_hparams()), "", "the ctc header")
        var rnnt = good_hparams()
        rnnt.head = "rnnt"
        t |> equal(hparams_refusal(rnnt), "", "the rnnt header")
        var ctc_layers = good_hparams()
        ctc_layers.pred_layers = 2l
        t |> equal(hparams_refusal(ctc_layers), "", "a ctc head reads no predictor layer count")
    }
    t |> run("a head neither ctc nor rnnt is refused") @(t : T?) {
        var h = good_hparams()
        h.head = "tdt"
        refused_with(t, h, "neither ctc nor rnnt", "head tdt")
    }
    t |> run("an rnnt predictor of two layers is refused") @(t : T?) {
        var h = good_hparams()
        h.head = "rnnt"
        h.pred_layers = 2l
        refused_with(t, h, "the predictor has 2 layers", "pred_layers 2")
    }
    t |> run("a d_model off the 32 lattice is refused") @(t : T?) {
        var h = good_hparams()
        h.d_model = 784l
        h.n_head = 8l
        h.head_dim = 98l
        h.n_sub_ch = 784l
        refused_with(t, h, "d_model 784 is not a multiple of 32", "d_model 784")
    }
    t |> run("an ff_dim off the 32 lattice is refused") @(t : T?) {
        var h = good_hparams()
        h.n_ff = 3080l
        refused_with(t, h, "ff_dim 3080 is not a multiple of 32", "ff_dim 3080")
    }
    t |> run("an odd head_dim is refused") @(t : T?) {
        var h = good_hparams()
        h.n_head = 256l
        h.head_dim = 3l
        refused_with(t, h, "head_dim 3 is odd", "head_dim 3")
    }
}

[test(description = "Every other unsupported GigaAM header field (sample rate, centered STFT, attention, conv norm, window, heads, subsampling, kernels, blank, tokenizer, token count) is refused by its own clause, and `gigaam_check_arch` refuses a llama GGUF")]
def test_gigaam_hparam_clauses(t : T?) {
    t |> run("every other header field the served encoder and decoders do not take is refused by its own clause") @(t : T?) {
        refused_field(t, "the sample rate is not 16 kHz") $(var h) {
            h.sample_rate = 8000
        }
        refused_field(t, "a centered STFT") $(var h) {
            h.mel_center = true
        }
        refused_field(t, "only rotary attention") $(var h) {
            h.attention = "rel_pos"
        }
        refused_field(t, "only the layer_norm conv module") $(var h) {
            h.conv_norm = "batch_norm"
        }
        refused_field(t, "the window length differs from n_fft") $(var h) {
            h.win_length = 400l
        }
        refused_field(t, "n_heads x head_dim is not d_model") $(var h) {
            h.head_dim = 32l
        }
        refused_field(t, "subsampling 4 only") $(var h) {
            h.subsampling = 8l
        }
        refused_field(t, "its channels must be d_model") $(var h) {
            h.n_sub_ch = 512l
        }
        refused_field(t, "even conv kernels") $(var h) {
            h.subs_kernel = 4l
        }
        refused_field(t, "even conv kernels") $(var h) {
            h.conv_kernel = 6l
        }
        refused_field(t, "the blank is not the class past the vocab") $(var h) {
            h.blank_id = 0l
        }
        refused_field(t, "the blank is not the class past the vocab") $(var h) {
            h.n_classes = 33l
        }
        refused_field(t, "tokenizer type 'bpe' is neither spm nor charwise") $(var h) {
            h.tokenizer = "bpe"
        }
        refused_field(t, "the token table has 32 entries, the vocab 33") $(var h) {
            h.n_tokens = 32l
        }
    }
    t |> run("a GGUF of another architecture is refused") @(t : T?) {
        let why = panic_text() ${
            gigaam_check_arch("llama", "x.gguf")
        }
        t |> success(find(why, "'x.gguf' is a 'llama' GGUF, not gigaam") >= 0, "llama refused: {why}")
        let taken = panic_text() ${
            gigaam_check_arch("gigaam", "x.gguf")
        }
        t |> equal(taken, "", "gigaam taken")
    }
}

def private span_list(spans : array<SpeechSegment>) : string => join([for (s in spans); "[{s.start},{s.end})"], " ")

[test(description = "`gigaam_spans` merges short speech regions under the cap, closes a span when the next region would pass it, cuts a region longer than the cap into ascending cap-sized spans, and yields none for no regions")]
def test_gigaam_spans(t : T?) {
    t |> run("two short regions merge into one span under the cap") @(t : T?) {
        let spans <- gigaam_spans([SpeechSegment(start = 0l, end = 30l), SpeechSegment(start = 40l, end = 90l)], 100l)
        t |> equal(span_list(spans), "[0,90)")
    }
    t |> run("regions past the cap together close the span and open the next") @(t : T?) {
        let spans <- gigaam_spans([SpeechSegment(start = 0l, end = 60l), SpeechSegment(start = 70l, end = 130l)], 100l)
        t |> equal(span_list(spans), "[0,60) [70,130)")
    }
    t |> run("a region longer than the cap is cut at the cap into ascending spans") @(t : T?) {
        let spans <- gigaam_spans([SpeechSegment(start = 0l, end = 30l), SpeechSegment(start = 50l, end = 280l)], 100l)
        t |> equal(span_list(spans), "[0,30) [50,150) [150,250) [250,280)")
        var last_end = 0l
        for (s in spans) {
            t |> success(s.end - s.start <= 100l && s.start >= last_end, "[{s.start},{s.end}) within the cap, after the last")
            last_end = s.end
        }
    }
    t |> run("no regions, no spans") @(t : T?) {
        let none : array<SpeechSegment>
        let spans <- gigaam_spans(none, 100l)
        t |> equal(length(spans), 0)
    }
}

[test(description = "`gigaam_chunks` on a silent clip past the cap panics naming a missing silero VAD weights path, and with the shipped weights returns no span and no refusal")]
def test_gigaam_long_clip_needs_the_vad(t : T?) {
    t |> run("a clip past the cap splits at the VAD's silences, and refuses where the VAD weights are missing") @(t : T?) {
        var samples : array<float>
        samples |> resize(int(GIGAAM_MAX_CLIP_S * AUDIO_SAMPLE_RATE) + 1)
        with_dasllama_jobque_() {
            let was = set_vad_model_path_probe_("/nonexistent/silero_vad.bin")
            var st = GigaamState()
            let why = panic_text() ${
                var inscope spans <- gigaam_chunks(st, samples)
            }
            set_vad_model_path_probe_(was)
            delete st
            t |> success(find(why, "the silero VAD weights are not at '/nonexistent/silero_vad.bin'") >= 0, "refused naming the path: '{why}'")
            var st2 = GigaamState()
            let taken = panic_text() ${
                var inscope spans <- gigaam_chunks(st2, samples)
                t |> equal(length(spans), 0, "a silent clip past the cap: no speech, no span")
            }
            delete st2
            t |> equal(taken, "", "the shipped weights: no refusal")
        }
    }
}

[test(description = "The GigaAM facade refuses a translate session at transcribe, `gguf_is_side_file` marks the ASR carrier and an mmproj but not a decoder, and the loader refuses another family's .dlim while mapping the carrier's own image")]
def test_gigaam_facade_refusals(tst : T?) {
    tst |> run("a translate session on a model whose caps declare no translation is refused at transcribe") @(t : T?) {
        with_dasllama_jobque_() {
            var inscope m = AsrModel(kind = AsrKind.gigaam)
            m.ga.n_fft = 320l
            m.ga.hop = 160l
            m.ga.n_mel = 64l
            var samples : array<float>
            samples |> resize(100)
            var inscope s = AsrSession()
            let plain = panic_text() ${
                transcribe(m, s, samples)
            }
            t |> equal(plain, "", "a transcribe session runs")
            s.translate = true
            let why = panic_text() ${
                transcribe(m, s, samples)
            }
            t |> success(find(why, "this model does not translate") >= 0, "refused: '{why}'")
        }
    }
    tst |> run("the side files: a one-file ASR carrier and a projector (arch clip) serve no chat, a decoder does") @(t : T?) {
        let ga = path_join(models_dir(), E2E_RNNT)
        let mmproj = path_join(models_dir(), "mmproj-Qwen3-ASR-0.6B-bf16.gguf")
        let decoder = path_join(models_dir(), "Qwen3-ASR-0.6B-Q8_0.gguf")
        return if (!model_available(t, ga) || !model_available(t, mmproj) || !model_available(t, decoder))
        t |> success(gguf_is_side_file(ga), "the GigaAM carrier is a side file")
        t |> success(gguf_is_side_file(mmproj), "the mmproj is a side file")
        t |> success(!gguf_is_side_file(decoder), "the decoder is no side file")
    }
    tst |> run("a .dlim another family prepared is refused by the GigaAM loader") @(t : T?) {
        own_load("asserts on the load itself - its refusal: another family's image, minted into scratch, is refused")
        let ga = path_join(models_dir(), "gigaam-v3-e2e-rnnt-f16.gguf")
        let smol = path_join(models_dir(), "SmolLM2-135M-Instruct-Q8_0.gguf")
        return if (!jit_model_available(t, ga) || !model_available(t, smol))
        with_dasllama_jobque_() {
            let other = path_join(test_temp_dir(), "smollm2.dlim")
            var inscope lm <- load_gguf(smol, QuantMode.q8)
            t |> success(save_model_image(lm, other), "SmolLM2's image saves under the test's temp directory")
            let why = panic_text() ${
                var inscope m <- load_gigaam_model(other)
            }
            t |> success(find(why, "is not a prepared gigaam image") >= 0, "refused: '{why}'")
        }
    }
    tst |> run("the GigaAM carrier's own .dlim, baked by its cold load, maps through the loader") @(t : T?) {
        own_load("reloads the model: the cold load bakes the carrier's image, which the cell loads back by path")
        let ga = path_join(models_dir(), "gigaam-v3-e2e-rnnt-f16.gguf")
        let smol = path_join(models_dir(), "SmolLM2-135M-Instruct-Q8_0.gguf")
        return if (!jit_model_available(t, ga) || !model_available(t, smol) || !image_rail_on(t))
        with_dasllama_jobque_() {
            let taken = panic_text() ${
                var inscope cold <- load_gigaam_model(ga)
                //! the image the cold load prepared on this box: the exact lane on the device the tower serves, not the CPU lane's name
                var inscope warm <- load_gigaam_model(image_path_for(ga, tower_dev_tag(gigaam_image_tag(false), cold.dev_backend)))
                t |> equal(warm.n_layer, cold.n_layer, "the carrier's own image maps")
            }
            t |> equal(taken, "", "the carrier's own image: no refusal")
        }
    }
}

[test(description = "A clip shorter than n_fft makes `gigaam_transcribe` return zero segments, hand out none, report Russian, and not panic")]
def test_gigaam_short_span(t : T?) {
    t |> run("a clip shorter than n_fft transcribes no segment and does not panic") @(t : T?) {
        var inscope m = GigaamModel(n_fft = 320l, hop = 160l, n_mel = 64l)
        var st = GigaamState()
        var samples : array<float>
        samples |> resize(100)
        var lang = ""
        var segs = 0
        let n = gigaam_transcribe(m, st, samples, $(seg) {
            segs++
        }, lang)
        t |> equal(n, 0, "the segment count")
        t |> equal(segs, 0, "segments handed out")
        t |> equal(lang, "ru")
    }
}

let private DEVICE_PLOG_BAR = 1e-5   //!< the device decode's plog off the CPU decoder's: float log-sums against double, ~1e-6 measured

//! a transducer carrier's device-decode twins: `form` the form whose legs run, `kind` the carrier's kind
struct private DecodeKept {
    form : string
    kind : string
    twins : array<RnntDecodeTwin>
}

// The device legs of the decode twin, on a lane's `+served` form - the flavor the lane serves on this box: each clip's
// encode with the serving tower pinned on, then the transducer decode on the build's device seat, counted once there -
// projected on the device, off the encode's own device rows where the tower encoded (the encode recorded a device
// generation), off uploaded rows where the CPU encoded. The encode's rows are kept for the CPU form's legs.
[cold_path]
def private decode_dev_legs(t : T?; m : GigaamModel; var k : DecodeKept) {
    for (clip in ["ru_short", "ru_mid"]) {
        let tag = "{k.kind} {m.q8 ? "q8" : "f16"} {clip}"
        continue if (rnnt_seat_skip(t, tag))
        if (m.dev_backend == TowerDev.none) {
            t |> skip("{tag}: the box serves the lane on its CPU image (a Vulkan device off the f16 GEMM feed) - the device seat decodes GPU images alone")
            continue
        }
        let path = asr_clip("{clip}.wav")
        if (!stat(path).is_valid) {
            t |> skip("{tag}: {clip}.wav not present")
            continue
        }
        var d = RnntDecodeTwin(tag = tag, q8 = m.q8, cpu_q8 = m.q8)
        with_gpu_knobs() {
            set_serving_gpu_tower(true, TowerFamily.gigaam)
            var inscope samples <- load_audio_16k_mono(path)
            var st = GigaamState()
            d.tt = gigaam_encode_span(samples, 0l, min(long_length(samples), GIGAAM_MAX_CLIP_S * AUDIO_SAMPLE_RATE), m, st)
            let device_encoded = st.enc_gen != 0ul
            d.rows := st.x
            rnnt_decode_dev_leg(t, "test_gigaam: {tag}", 1l, device_encoded ? 1l : 0l) {
                gigaam_decode(m, st, d.tt)
            }
            d.dev := st.toks
            d.dev_text = gigaam_detokenize(m, d.dev)
            delete st
        }
        d.legs = 1
        k.twins |> emplace(d)
    }
}

// The CPU legs of the decode twin, on the lane's CPU form: the device form's kept rows decoded on the CPU loop with the seat
// pinned off (the decode counter must not move, so a knob that pins nothing reds), then at 1.05x - the control.
[cold_path]
def private decode_cpu_legs(t : T?; mc : GigaamModel; var k : DecodeKept) {
    for (d in k.twins) {
        continue if (d.cpu_q8 != mc.q8 || d.legs != 1)
        with_gpu_knobs() {
            set_serving_gpu_tower(true, TowerFamily.gigaam)
            var stc = GigaamState()
            rnnt_decode_cpu_legs(t, "test_gigaam: {d.tag}", d) $(rows; tt; var toks) {
                stc.x := rows
                gigaam_decode(mc, stc, tt)
                toks := stc.toks
            }
            d.cpu_text = gigaam_detokenize(mc, d.cpu)
            delete stc
        }
    }
}

// A form's legs of a transducer carrier's device-decode twin.
[cold_path]
def private decode_legs(t : T?; m : GigaamModel; var k : DecodeKept) {
    with_dasllama_jobque_() {
        if (gigaam_form_of(k.form).edit == "served") {
            decode_dev_legs(t, m, k)
        } else {
            decode_cpu_legs(t, m, k)
        }
    }
}

// The transducer decode on the build's device seat against the CPU decoder's, after the carrier's block, the twins of the
// lane `k.form` names (`f16`, `q8`): the same ids and frames, the plog within `DEVICE_PLOG_BAR`, the 1.05x control's plogs
// outside it; both decodes' transcripts logged.
[cold_path]
def private test_gigaam_device_decode_twin(t : T?; k : DecodeKept) {
    for (d in k.twins) {
        continue if (d.q8 != (k.form == "q8"))
        rnnt_decode_twin_held(t, "test_gigaam: {d.tag}", d, DEVICE_PLOG_BAR)
    }
}

// The CTC head's device legs on the Metal seat, on a CTC carrier's `f16@dev` and `q8@dev` forms (`ga_ctc_dev_leg`).
[cold_path]
def private ctc_head_metal_dev(t : T?; m : GigaamModel; var k : GaKept) {
    return if (!jit_only(t))
    ga_ctc_dev_leg(t, m, k, "test_gigaam", "{k.strs?["kind"] ?? ""} {m.q8 ? "q8" : "f16"} ru_short", false)
}

// The CTC head's device legs on the family's discrete seat, on a CTC carrier's `+served` forms (`ga_ctc_dev_leg`).
[cold_path]
def private ctc_head_discrete_dev(t : T?; m : GigaamModel; var k : GaKept) {
    ga_ctc_dev_leg(t, m, k, "test_audio", "gigaam {replace(k.strs?["kind"] ?? "", "_", "-")} {m.q8 ? "q8" : "f16"} ru_short", true)
}

// The CTC head on the Metal seat against the CPU head, after a CTC carrier's block, the lane `k.form` names (`ga_ctc_held`).
[cold_path]
def private test_gigaam_ctc_head_metal(t : T?; k : GaKept) {
    ga_ctc_held(t, k, "test_gigaam")
}

//! the frames whose pick is not the oracle's argmax where the oracle's top two log-probabilities sit apart by `CTC_TIE_MARGIN` or
//! more: a near-tie frame may pick either
def private oracle_hard_flips(picks : array<int>; lp : Npy) : int {
    var hard = 0
    for (f in range64(min(long_length(picks), lp.rows))) {
        var a = -3.4e38
        var b = -3.4e38
        var best = 0
        for (c in range64(lp.cols)) {
            let v = lp.data[f * lp.cols + c]
            if (v > a) {
                b = a
                a = v
                best = int(c)
            } elif (v > b) {
                b = v
            }
        }
        if (picks[f] != best && a - b >= CTC_TIE_MARGIN) {
            to_log(LOG_WARNING, "test_gigaam: frame {f} picks {picks[f]}, the oracle {best}, margin {a - b}\n")
            hard++
        }
    }
    return hard
}

//! One CTC head decode over the oracle's encoder rows: the picks, the ids, the emitted tokens' worst probability off the oracle's
//! softmax, the transcript, and the build's device seat's served count it moved (0 with no seat)
struct private CtcOracleLeg {
    picks : array<int>
    ids : array<int>
    worst : float
    text : string
    served : int64
}

//! the oracle's encoder rows `enc` through the CTC head of `m`, the build's GPU tower on (`device`) or off
def private ctc_oracle_leg(m : GigaamModel; enc, lp : Npy; device : bool) : CtcOracleLeg {
    var r : CtcOracleLeg
    var st = GigaamState()
    st.x := enc.data
    with_gpu_knobs() {
        set_serving_gpu_tower(device, TowerFamily.gigaam)
        let c0 = ctc_seat_heads()
        gigaam_decode(m, st, enc.rows)
        r.served = ctc_seat_heads() - c0
    }
    r.picks <- copy_span(st.picks, 0l, enc.rows)
    r.ids <- [for (x in st.toks); x.id]
    r.worst = ctc_plog_worst(st.toks, lp)
    r.text = gigaam_detokenize(m, st.toks)
    delete st
    return <- r
}

// The CTC head alone against the oracle, a leg a form: the oracle's encoder rows (`encoded.npy`) through the head on the
// lane's CPU form (the tower off) and, where the build's device seat serves on the box (Metal's, else Vulkan's), on its
// `+served` form (counted once, off the uploaded rows) - each frame's pick the oracle's argmax but at a near-tie frame,
// the emitted tokens' probabilities within `PLOG_BAR` of the oracle's softmax, the ids the oracle's. The head reads the
// carrier's halfwords in ``wblob``, so the control is the carrier staged with the head's halfwords zeroed there, the
// `+nohead` forms: its ids leave the oracle's on every leg.
[cold_path]
def private test_gigaam_ctc_head_oracle(t : T?; m : GigaamModel; k : GaKept) {
    return if (!jit_only(t))
    let kind = k.strs?["kind"] ?? ""
    let clip = "ru_short"
    return if (!dumps_present(t, clip, kind))
    let f = gigaam_form_of(k.form)
    let device = f.edit == "served" || f.edit == "nohead_served"
    let dir = dump_dir(clip, kind)
    let tag = "{clip}_{kind} {f.lane == "q8" ? "q8" : "f16"} ctc head"
    let enc <- read_npy("{dir}/encoded.npy")
    let lp <- read_npy("{dir}/ctc_logprobs.npy")
    var tk = OracleTokens()
    t |> success(load_json_file("{dir}/tokens.json", tk), "{tag}: tokens.json read")
    if (f.edit == "nohead" || f.edit == "nohead_served") {
        var inscope z <- ctc_oracle_leg(m, enc, lp, device)
        t |> success(!sequence_equal(z.ids, tk.ids), "{tag} {device ? rnnt_seat_name() : "cpu"}: the control - the head's halfwords zeroed in wblob leave the oracle's ids ({length(z.ids)} tokens)")
        return
    }
    if (!device) {
        t |> success(m.ctc_f16, "{tag}: the head serves off the carrier's halfwords")
        t |> equal(enc.cols, m.d_model, "{tag}: the oracle's encoder rows are d_model wide")
        t |> equal(lp.cols, m.n_classes, "{tag}: the oracle's log-probabilities are n_classes wide")
    }
    let leg = device ? "{tag} {rnnt_seat_name()}" : "{tag} cpu"
    var inscope r <- ctc_oracle_leg(m, enc, lp, device)
    to_log(LOG_INFO, "test_gigaam: {leg}: token probabilities off the oracle's softmax by {r.worst}: {r.text}\n")
    t |> equal(r.served, device ? 1l : 0l, "{leg}: the head served {device ? "on the device seat" : "on the CPU"}")
    t |> equal(oracle_hard_flips(r.picks, lp), 0, "{leg}: the picks are the oracle's argmax but at a near-tie frame")
    t |> success(!empty(r.ids) && r.worst <= PLOG_BAR, "{leg}: token probabilities within {PLOG_BAR} of the oracle's softmax ({r.worst})")
    t |> success(sequence_equal(r.ids, tk.ids), "{leg}: the token ids are the oracle's")
}

//! the corpus harness spawned on this binary over `args` under DASLLAMA_IMAGE=0 (no sidecar beside the stocked carrier), its
//! output into `out`; 300 s covers a cold JIT of the facade's ASR and translation modules
def private run_corpus_eval(args : array<string>; var out : string&) : int {
    let prev_image = env_value_of("DASLLAMA_IMAGE")
    set_env_variable("DASLLAMA_IMAGE", "0")
    let root = get_das_root()
    var argv <- daslang_argv(example_daslang(), ["-jit", "-dasroot", root,
        "{root}/modules/dasLLAMA/harness/asr_corpus_eval.das", "--"])
    argv |> push_from(args)
    let rc = run_and_capture(argv, out, 300.0)
    set_env_variable("DASLLAMA_IMAGE", prev_image)
    return rc
}

[test(description = "The asr_corpus_eval harness on the Q4_K carrier transcribes a FLEURS-layout split up to --limit into one hypothesis row carrying the clip's words, translating with a Bergamot student where present, and refuses an unknown --lane by name")]
def test_gigaam_corpus_eval(tst : T?) {
    tst |> run("the corpus harness transcribes a FLEURS-layout split up to --limit, translating where --nmt names a student, and refuses a lane it has no name for") @(t : T?) {
        return if (!model_available(t, kq_carrier()))
        let clip = asr_clip("ru_short.wav")
        if (!stat(clip).is_valid) {
            t |> skip("ru_short.wav not present under the ASR corpus")
            return
        }
        let corpus = path_join(test_temp_dir(), "ga_corpus")
        mkdir_rec(path_join(corpus, "test"))
        var why = ""
        t |> success(copy_file(clip, path_join(corpus, "test/a.wav"), true, why), "the clip staged: {why}")
        fopen(path_join(corpus, "test.tsv"), "wb") $(f) {
            fwrite(f, "1\ta.wav\tв среду\n2\ta.wav\tв среду\n")   // nolint:STYLE039 - the corpus's Russian reference
        }
        let nmt = path_join(models_dir(), "bergamot/ru-en-base-memory/model.ruen.intgemm.alphas.bin")
        let has_nmt = stat(nmt).is_valid
        if (!has_nmt) {
            to_log(LOG_WARNING, "test_gigaam: no Bergamot ru-en student under the models - the corpus run's --nmt columns go unchecked\n")
        }
        let hyp = path_join(corpus, "hyp/kq.tsv")
        var argv <- ["--model", kq_carrier(), "--corpus", corpus, "--out", hyp, "--limit", "1", "--threads", "8"]
        if (has_nmt) {
            argv |> push_from(["--nmt", nmt])
        }
        var out = ""
        let rc = run_corpus_eval(argv, out)
        t |> equal(0, rc, "the run exits clean: {slice(out, max(0, length(out) - 600))}")
        t |> success(find(out, "LOAD\tgigaam-v3-e2e-rnnt-q4_k.gguf") >= 0 && find(out, "DONE\t1 clips") >= 0, "LOAD names the carrier and DONE counts the one clip --limit kept:\n{out}")
        var inscope rows : array<string>
        fopen(hyp, "rb") $(f) {
            return if (f == null)
            while (!feof(f)) {
                let line = strip_right(fgets(f))
                rows |> push(line) if (!empty(line))
            }
        }
        t |> equal(1, length(rows), "one row, --limit's count, out of the split's two")
        return if (length(rows) != 1)
        var inscope cols <- split(rows[0], "\t")
        t |> equal(has_nmt ? 7 : 5, length(cols), "id, wav, seconds, ms, hypothesis{has_nmt ? ", both translations" : ""}: {rows[0]}")
        return if (length(cols) < 5)
        t |> equal("1", cols[0], "the split's first id")
        t |> success(find(cols[4], "телевизионные") >= 0, "the hypothesis carries the clip's words ({cols[4]})")   // nolint:STYLE039 - the clip's Russian word
        if (has_nmt && length(cols) == 7) {
            t |> success(find(cols[6], "Wednesday") >= 0, "the reference translated ({cols[6]})")
        }
        var bad = ""
        let brc = run_corpus_eval(["--model", kq_carrier(), "--corpus", corpus, "--out", hyp, "--lane", "f16"], bad)
        t |> equal(1, brc, "a lane that is neither q8 nor exact is a refusal")
        t |> success(find(bad, "--lane is q8 or exact") >= 0, "and says so by name: {slice(bad, max(0, length(bad) - 600))}")
    }
}


//! a kq carrier's forms: its own lane's CPU form, then the Metal twin's (minted and run under the portable backend) and
//! the discrete twin's
def private kq_twin_forms(t : T?; var forms : array<string>; file : string) {
    add_served_forms(t, forms, file, ["kq+portable", "kq@dev+portable", "kq_z8@dev+portable"], metal_build_why())
    add_served_forms(t, forms, file, ["kq+served", "kq_z8@dev"], audio_discrete_form_why(TowerFamily.gigaam))
}

//! a kq carrier's twin cells of the form `form`: the Metal twin's under the portable backend - the CPU kq leg in disk
//! order on the portable batch GEMM, as the Metal image holds its planes - and the discrete twin's
def private kq_twin_cells(t : T?; var m : GigaamModel; form : string; var kept : GaKept) {
    let f = gigaam_form_of(form)
    if (f.edit == "portable") {
        let pinned = with_kernel_backend("portable") {
            tower_cell(t, "{kept.carrier}: the kq lane's Metal twin, the {form} form's legs", m, kept, @@ga_metal_kq_legs)
        }
        t |> success(pinned, "{kept.carrier}: the portable backend pinned for the kq lane's {form} form")
    } elif (empty(audio_discrete_form_why(TowerFamily.gigaam)) && (form == "kq" || form == "kq+served" || form == "kq_z8@dev")) {
        tower_cell(t, "{kept.carrier}: the kq lane's discrete twin, the {form} form's legs", m, kept, @@ga_discrete_kq_legs)
    }
}

//! a kq carrier's twin compares after its block
def private kq_twin_held(t : T?; file : string; var kept : GaKept) {
    if (empty(metal_build_why())) {
        kept_cell(t, "{file}: the kq lane on the Metal chain vs the CPU kq chain", kept, @@ga_metal_kq_twin)
    }
    if (empty(audio_discrete_form_why(TowerFamily.gigaam))) {
        kept_cell(t, "{file}: the kq lane on the discrete home's chain vs the CPU kq chain", kept, @@ga_discrete_kq_twin)
    }
}

// The e2e transducer's f16 carrier's block: the external reference, the quantized carriers' reference, the long form, the
// facade's arms, and the device twins' legs with their compares after it.
[cold_path]
def private e2e_rnnt_f16_block(t : T?; var kept : GaKept; var e2e : E2eKept) {
    var inscope dec = DecodeKept(kind = "e2e_rnnt")
    let metal_on = empty(metal_build_why())
    let discrete_on = empty(audio_discrete_form_why(TowerFamily.gigaam))
    let seat_on = empty(ga_seat_why())
    var inscope forms : array<string>
    add_served_forms(t, forms, E2E_RNNT, ["f16+served"], either_why(ga_seat_why(), audio_discrete_form_why(TowerFamily.gigaam)))
    add_served_forms(t, forms, E2E_RNNT, ["q8+served"], ga_seat_why())
    forms |> push("f16")
    forms |> push("q8")
    add_served_forms(t, forms, E2E_RNNT, ["q8@dev", "q8_z8@dev"], either_why(metal_build_why(), audio_discrete_form_why(TowerFamily.gigaam)))
    add_served_forms(t, forms, E2E_RNNT, ["f16@dev"], metal_build_why())
    add_served_forms(t, forms, E2E_RNNT, ["f16_z8@dev"], audio_discrete_form_why(TowerFamily.gigaam))
    let ran = with_model_once(t, "audio", "gigaam-v3-e2e-rnnt-f16.gguf", forms, @@gigaam_form) $(var m; form) {
        gigaam_form_is(t, E2E_RNNT, form, m.ga)
        kept.form = form
        e2e.form = form
        dec.form = form
        let served = form == "f16+served" || form == "q8+served"
        if (form == "f16") {
            tower_cell(t, "e2e_rnnt: the f16 lane matches the external reference stage by stage", m.ga, kept, @@test_gigaam_e2e_rnnt)
            tower_cell(t, "e2e_rnnt: the exact lane's transcripts and rows, the quantized carriers' reference", m.ga, e2e, @@exact_reference_kept)
            tower_cell(t, "ru_long: past the cap the clip splits at silences into spans of at most 25 s, the words the references'", m.ga, @@test_gigaam_long_form)
            tower_cell(t, "the facade's gigaam arms: caps, exec format, the one-file carrier, session language, transcribe", m, @@gigaam_facade_arms)
        } elif (form == "q8") {
            tower_cell(t, "e2e_rnnt: the q8 lane's transcript agrees with the oracle's words", m.ga, kept, @@test_gigaam_e2e_rnnt)
        }
        if (seat_on && (served || form == "f16" || form == "q8")) {
            tower_cell(t, "e2e_rnnt: the transducer decode twin, the {form} form's legs", m.ga, dec, @@decode_legs)
        }
        if (metal_on && form == "f16@dev") {
            coverage_tower_gigaam_metal(t,
                "GigaAM v3 e2e-rnnt: the kernel census's jfk on the Metal f16 image",
                m.ga)
        }
        if (metal_on && !served) {
            tower_cell(t, "GigaAM v3 e2e-rnnt: the Metal twin, the {form} form's legs", m.ga, kept, @@ga_metal_legs)
        }
        if (discrete_on && form != "q8+served") {
            tower_cell(t, "GigaAM v3 e2e-rnnt: the discrete twin, the {form} form's legs", m.ga, kept, @@ga_discrete_legs)
        }
    }
    if (ran && seat_on) {
        dec.form = "f16"
        kept_cell(t, "e2e_rnnt f16: the transducer decode on the device seat is the CPU decoder's", dec, @@test_gigaam_device_decode_twin)
        dec.form = "q8"
        kept_cell(t, "e2e_rnnt q8: the transducer decode on the device seat is the CPU decoder's", dec, @@test_gigaam_device_decode_twin)
    }
    if (ran && metal_on) {
        kept_cell(t, "GigaAM v3 e2e-rnnt: the Metal rotary FastConformer chain vs the CPU exact chain on the FLEURS ru clips, the q8 lane and the Q4_K carrier's kq lane beside it", kept, @@test_gigaam_metal_twin)
    }
    if (ran && discrete_on) {
        kept_cell(t, "GigaAM v3 e2e-rnnt: the discrete home's rotary FastConformer front and blocks on every lane - the q8 lane vs the CPU q8 chain, the exact lane off the halfwords vs the CPU exact chain, the Q4_K carrier's kq lane vs the CPU kq chain - on the FLEURS ru clips", kept, @@test_gigaam_discrete_twin)
    }
}

// The published Q4_K carrier's block, after the f16 carrier's: the kq lane, the lane flag and the pins, the kq lane's
// device twins, then the cells that own their load - the Q8_0 seats against the file and the family's load on the box's
// flavor.
[cold_path]
def private e2e_rnnt_q4_k_block(t : T?; var kept : GaKept; var e2e : E2eKept) {
    var inscope kq_forms <- ["kq"]
    add_served_forms(t, kq_forms, KQ, ["kq@dev"], device_up_why(TowerFamily.gigaam))
    kq_forms |> push_from(["kq+q8flag", "q8+pinned", "f32+pinned"])
    kq_twin_forms(t, kq_forms, KQ)
    kept.carrier = "k4"
    kept.fmt = "k4"
    let kq_ran = with_model_once(t, "audio", "gigaam-v3-e2e-rnnt-q4_k.gguf", kq_forms, @@gigaam_form) $(var m; form) {
        gigaam_form_is(t, KQ, form, m.ga)
        kept.form = form
        e2e.form = form
        if (form == "kq") {
            tower_cell(t, "e2e_rnnt q4_k: the kq lane serves the carrier's own planes", m.ga, e2e, @@test_gigaam_kq_lane)
            tower_cell(t, "e2e_rnnt q4_k: the un-pinned mint's lane name, transcript and rows", m.ga, e2e, @@kq_flag_leg)
        } elif (form == "kq@dev") {
            tower_cell(t, "e2e_rnnt q4_k: the kq lane serves the carrier's own planes, on the GPU flavor too", m.ga, e2e, @@test_gigaam_kq_lane)
        } elif (form == "kq+q8flag") {
            tower_cell(t, "e2e_rnnt q4_k: the mint that took the q8 flag, its lane name and transcript", m.ga, e2e, @@kq_flag_leg)
        } elif (form == "q8+pinned") {
            tower_cell(t, "e2e_rnnt q4_k: a pinned q8 lane dequantizes the carrier into Q8_0 planes", m.ga, e2e, @@kq_pinned_q8_cell)
        } elif (form == "f32+pinned") {
            tower_cell(t, "e2e_rnnt q4_k: a pinned exact lane widens the carrier to f32", m.ga, e2e, @@kq_pinned_exact_cell)
        }
        kq_twin_cells(t, m.ga, form, kept)
    }
    if (kq_ran) {
        kept_cell(t, "e2e_rnnt q4_k: the q8 flag a kq carrier's staging and mint take changes nothing - the carrier's planes decide", e2e, @@kq_flag_held)
        kept_cell(t, "e2e_rnnt q4_k: the kq lane's encoder rows sit no further from the carrier widened to f32 than the pinned q8 lane's requant of it", e2e, @@kq_rows_held)
        kq_twin_held(t, KQ, kept)
        t |> run("e2e_rnnt q4_k: the kq lane's Q8_0 seats are the carrier's blocks, moved exactly") @(t : T?) {
            q8_seats_exact(t, "gigaam-v3-e2e-rnnt-q4_k.gguf")
        }
        unsafe {
            t |> run("e2e_rnnt q4_k: the load serves the carrier's own planes - on the device where its chain serves the lane, else on the CPU image") @capture(& e2e)(t : T ?) {
                test_gigaam_kq_load_flavor(t, e2e.refs)
            }
        }
    }
}

// The other quantized carriers' blocks, after the f16 carrier's: the local mint mixing two K-quant members, and the
// carrier whose every seat is Q8_0.
[cold_path]
def private e2e_rnnt_other_blocks(t : T?; var kept : GaKept; var e2e : E2eKept) {
    var inscope mix_forms <- ["kq", "kq+held"]
    kq_twin_forms(t, mix_forms, KQ_MIX)
    kept.carrier = "mix"
    kept.fmt = "kq"
    let mix_ran = with_model_once(t, "audio", "gigaam-v3-e2e-rnnt-q4_k-mix-local.gguf", mix_forms, @@gigaam_form) $(var m; form) {
        gigaam_form_is(t, KQ_MIX, form, m.ga)
        kept.form = form
        e2e.form = form
        if (form == "kq" || form == "kq+held") {
            tower_cell(t, "e2e_rnnt q4_k + q6_k: the kq lane serves a carrier mixing two K-quant members, the {form} form", m.ga, e2e, @@test_gigaam_kq_other_carriers)
        }
        kq_twin_cells(t, m.ga, form, kept)
    }
    if (mix_ran) {
        kept_cell(t, "e2e_rnnt q4_k + q6_k: the held staging mints the streamed mint's model", e2e, @@kq_mix_held)
        kq_twin_held(t, KQ_MIX, kept)
    }
    let q8_0_ran = with_model_once(t, "audio", "gigaam-v3-e2e-rnnt-q8_0.gguf", ["kq"], @@gigaam_form) $(var m; form) {
        gigaam_form_is(t, KQ_Q8_0, form, m.ga)
        tower_cell(t, "e2e_rnnt q8_0: a carrier whose every block GEMM is Q8_0 serves them on the kq lane", m.ga, e2e, @@kq_q8_0_cell)
    }
    if (q8_0_ran) {
        t |> run("e2e_rnnt q8_0: a carrier whose every block GEMM is Q8_0 serves them exactly on the kq lane") @(t : T?) {
            q8_seats_exact(t, "gigaam-v3-e2e-rnnt-q8_0.gguf")
        }
    }
}

// The e2e transducer's f16 carrier's block, then the quantized carriers' - each reads the f16 block's exact lane.
[cold_path]
def private e2e_rnnt_blocks(t : T?) {
    var inscope kept <- ga_kept()
    var inscope e2e = E2eKept()
    e2e_rnnt_f16_block(t, kept, e2e)
    e2e_rnnt_q4_k_block(t, kept, e2e)
    e2e_rnnt_other_blocks(t, kept, e2e)
    static_if (typeinfo builtin_module_exists(das_metal)) {
        metal_tower_shutdown()
    }
    if (empty(audio_discrete_form_why(TowerFamily.gigaam))) {
        gpu_tower_shutdown(TowerFamily.gigaam)
    }
}

[test(description = "The GigaAM v3 e2e RNNT carriers load once a form. The f16 carrier matches the upstream dumps stage by stage within 1e-4 through the decode on two clips, its q8 lane's transcript holds 90 percent of the oracle's words with a half-silenced control under the bar; a long clip past the cap splits at silences into ascending non-overlapping spans of at most 25 s whose joined transcript holds 95 percent of the FLEURS references' words in order while the reversed references miss; through the ASR facade the carrier loads alone, its caps are Russian segment-timestamped whole-clip, an English session is refused, and transcribe yields the family's own one-segment transcript; the transducer decode on the build's device seat emits the CPU decoder's ids and frames with plog within 1e-5, counted once on the device; on Metal the conv front and every block run on the device within 1e-2 of the CPU exact rows with the CPU transcript, a scaled mel outside the bar, the q8 lane holding its three-way bar; on the discrete home the q8 lane and the exact f16 lane hold their bars with the CPU chain ids and poisons outside. The Q4_K carrier loads on the kq lane named k4 with members k4+q8, its transcripts hold the q8 word bar against the f16 lane on the CPU and on the serving GPU tower with no decline, the q8 flag leaves its lane and transcript unchanged, its rows sit within 1.5x the pinned q8 lane's distance from the widened carrier, pinned q8 and f32 lanes transcribe, its Q8_0 seats equal the file's blocks bit for bit, its load lands on the GPU flavor where the device stands up, and its kq lane holds the three-way bar on either GPU home with every member tile counted. The mixed Q4_K and Q6_K carrier serves as kq with members k4+k6+q8, its held staging minting the same transcript, and the all-Q8_0 carrier as q8 with seats equal to the file's blocks")]
def test_gigaam_carriers_e2e_rnnt(t : T?) {
    return if (!family_on(t, "gigaam"))
    with_dasllama_jobque_() {
        e2e_rnnt_blocks(t)
    }
}

//! the forms of a CTC carrier: the head's device forms ahead of the CPU forms that read their rows
def private ctc_carrier_forms(t : T?; file : string) : array<string> {
    var forms : array<string>
    add_served_forms(t, forms, file, ["f16@dev", "q8@dev"], ga_ctc_metal_why())
    add_served_forms(t, forms, file, ["f16+served", "q8+served"], either_why(ga_ctc_seat_why(), audio_discrete_form_why(TowerFamily.gigaam)))
    forms |> push_from(["f16", "q8", "f16+nohead", "q8+nohead"])
    add_served_forms(t, forms, file, ["f16+nohead_served", "q8+nohead_served"], ga_ctc_seat_why())
    return <- forms
}

//! a CTC carrier's cells of the form `form`: the external reference, the CTC head's oracle legs, and the head's device
//! twin on the Metal seat or on the discrete seat
def private ctc_carrier_cells(t : T?; var m : GigaamModel; form : string; var kept : GaKept; lane_cell : function<(t : T?; m : GigaamModel; k : GaKept) : void>) {
    let kind = kept.strs?["kind"] ?? ""
    let f = gigaam_form_of(form)
    kept.form = form
    if (form == "f16") {
        tower_cell(t, "{kind}: the f16 lane matches the external reference stage by stage", m, kept, lane_cell)
    } elif (form == "q8") {
        tower_cell(t, "{kind}: the q8 lane's transcript agrees with the oracle's words", m, kept, lane_cell)
    }
    if (f.dev) {
        tower_cell(t, "{kind}: the CTC head on the Metal device seat, the {form} form's legs", m, kept, @@ctc_head_metal_dev)
        return
    }
    if (f.edit == "served" && empty(audio_discrete_form_why(TowerFamily.gigaam))) {
        tower_cell(t, "GigaAM v3 {replace(kind, "_", "-")}: the CTC head on the discrete device seat, the {form} form's legs", m, kept, @@ctc_head_discrete_dev)
    }
    if (empty(f.edit)) {
        tower_cell(t, "{kind}: the CTC head's CPU leg over the device form's rows, the {form} form", m, kept, @@ga_ctc_cpu_leg)
    }
    if (empty(f.edit) || empty(ga_ctc_seat_why())) {
        tower_cell(t, "{kind} {f.lane}: the CTC head over the oracle's encoder rows reads its halfwords and lands the oracle's picks, the {form} form", m, kept, @@test_gigaam_ctc_head_oracle)
    }
}

//! a CTC carrier's compares after its block: the head's device twin of each lane
def private ctc_carrier_held(t : T?; var kept : GaKept) {
    let kind = kept.strs?["kind"] ?? ""
    for (lane in ["f16", "q8"]) {
        kept.form = lane
        if (empty(ga_ctc_metal_why())) {
            kept_cell(t, "{kind} {lane}: the CTC head on the Metal device seat is the CPU head's", kept, @@test_gigaam_ctc_head_metal)
        }
        if (empty(audio_discrete_form_why(TowerFamily.gigaam))) {
            kept_cell(t, "GigaAM v3 {replace(kind, "_", "-")} {lane}: the CTC head on the discrete device seat is the CPU head's, off the encode's held rows and off uploaded rows", kept, @@test_gigaam_ctc_discrete_twin)
        }
    }
    if (empty(audio_discrete_form_why(TowerFamily.gigaam))) {
        gpu_tower_shutdown(TowerFamily.gigaam)
    }
}

[cold_path]
def private e2e_ctc_block(t : T?) {
    var inscope kept <- ga_kept()
    kept.strs["kind"] = "e2e_ctc"
    var inscope forms <- ctc_carrier_forms(t, E2E_CTC)
    let ran = with_model_once(t, "audio", "gigaam-v3-e2e-ctc-f16.gguf", forms, @@gigaam_form) $(var m; form) {
        gigaam_form_is(t, E2E_CTC, form, m.ga)
        ctc_carrier_cells(t, m.ga, form, kept, @@test_gigaam_e2e_ctc)
    }
    if (ran) {
        ctc_carrier_held(t, kept)
    }
}

[test(description = "The GigaAM v3 e2e CTC carrier loads once a form: its f16 lane matches the upstream dumps stage by stage within 1e-4 through the decode, and its q8 lane's transcript holds 90 percent of the oracle's words with a half-silenced control under the bar; over the oracle's encoder rows its CTC head, on the CPU and the build's device seat, picks the oracle's argmax but at near-ties, emits tokens within 1e-4 of the oracle's softmax and its exact ids, while zeroing the head's halfwords leaves the oracle's ids; on the Metal seat and on the discrete seat the head, f16 and q8, picks the CPU head's classes but at near-ties with log-sums within the bar and the same ids and transcript, from device and uploaded rows, while a 1.05x louder clip exceeds the bar")]
def test_gigaam_carrier_e2e_ctc(t : T?) {
    return if (!family_on(t, "gigaam"))
    with_dasllama_jobque_() {
        e2e_ctc_block(t)
    }
}

[cold_path]
def private ctc_block(t : T?) {
    var inscope kept <- ga_kept()
    kept.strs["kind"] = "ctc"
    var inscope forms <- ctc_carrier_forms(t, CTC)
    let ran = with_model_once(t, "audio", "gigaam-v3-ctc-f16.gguf", forms, @@gigaam_form) $(var m; form) {
        gigaam_form_is(t, CTC, form, m.ga)
        ctc_carrier_cells(t, m.ga, form, kept, @@test_gigaam_ctc)
    }
    if (ran) {
        ctc_carrier_held(t, kept)
    }
}

[test(description = "The GigaAM v3 CTC carrier loads once a form: its f16 lane matches the upstream package's dumps stage by stage (mel, front, every layer, encoded within 1e-4, then the decode), and the q8 lane's transcript holds 90 percent of the oracle's words while a half-silenced clip does not; over the oracle's encoder rows its CTC head, on the CPU and the build's device seat, picks the oracle's argmax but at near-ties, emits tokens within 1e-4 of the oracle's softmax and its exact ids, while zeroing the head's halfwords leaves the oracle's ids; on the Metal seat and on the discrete seat the head, f16 and q8, picks the CPU head's classes but at near-ties with log-sums within the bar and the same ids and transcript, from device and uploaded rows, while a 1.05x louder clip exceeds the bar")]
def test_gigaam_carrier_ctc(t : T?) {
    return if (!family_on(t, "gigaam"))
    with_dasllama_jobque_() {
        ctc_block(t)
    }
}

[cold_path]
def private charwise_rnnt_block(t : T?) {
    var inscope kept <- ga_kept()
    var inscope dec = DecodeKept(kind = "rnnt")
    let seat_on = empty(ga_seat_why())
    var inscope forms : array<string>
    add_served_forms(t, forms, RNNT, ["f16+served", "q8+served"], ga_seat_why())
    forms |> push_from(["f16", "q8"])
    let ran = with_model_once(t, "audio", "gigaam-v3-rnnt-f16.gguf", forms, @@gigaam_form) $(var m; form) {
        gigaam_form_is(t, RNNT, form, m.ga)
        kept.form = form
        dec.form = form
        if (form == "f16") {
            tower_cell(t, "rnnt: the f16 lane matches the external reference stage by stage", m.ga, kept, @@test_gigaam_charwise_rnnt)
        } elif (form == "q8") {
            tower_cell(t, "rnnt: the q8 lane's transcript agrees with the oracle's words", m.ga, kept, @@test_gigaam_charwise_rnnt)
        }
        if (seat_on) {
            tower_cell(t, "rnnt: the transducer decode twin, the {form} form's legs", m.ga, dec, @@decode_legs)
        }
    }
    if (ran && seat_on) {
        dec.form = "f16"
        kept_cell(t, "rnnt f16: the transducer decode on the device seat is the CPU decoder's", dec, @@test_gigaam_device_decode_twin)
        dec.form = "q8"
        kept_cell(t, "rnnt q8: the transducer decode on the device seat is the CPU decoder's", dec, @@test_gigaam_device_decode_twin)
    }
}

[test(description = "The GigaAM v3 char-wise RNNT carrier loads once a form: its f16 lane matches the upstream dumps stage by stage within 1e-4 through the decode, its q8 lane's transcript holds 90 percent of the oracle's words with a half-silenced control under the bar, and on f16 and q8 lanes the transducer decode on the build's device seat emits the CPU decoder's ids and frames with plog within 1e-5, counted once on the device")]
def test_gigaam_carrier_rnnt(t : T?) {
    return if (!family_on(t, "gigaam"))
    with_dasllama_jobque_() {
        charwise_rnnt_block(t)
    }
}
