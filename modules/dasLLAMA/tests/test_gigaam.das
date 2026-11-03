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
require _compares   // nan_wide_diff: a NaN probability reads as outside the bar
require _rnnt_oracle   // rnnt_seat_*: the build's device transducer seat, its pin and counters
require _gpu_knobs   // with_gpu_knobs / set_serving_gpu_tower: the device decode's tower pin
require dasllama/dasllama_load   // load_gguf: the foreign family's image the loader refuses
require dasllama/dasllama_gguf   // parse_gguf_meta: the Q8_0 seats' disk blocks
require daslib/f16_cvt           // f16_to_f32: a disk block's scale
require _gguf_writer             // gguf_header / gguf_push_*: the retyped carriers the planner refuses
require dasllama/dasllama_image   // save_model_image
require dasllama/dasllama_common   // QuantMode
require ?das_metal dasllama/dasllama_metal_tower   // set_metal_tower: the Metal CTC cell's pin
require _ctc_oracle   // ctc_seat_heads: the build's device CTC seat's count
require _example_rail   // example_daslang: the corpus harness's child runs on this binary

// GigaAM v3 against the upstream package's per-stage dumps (benchmarks/asr/gigaam_oracle.py into the model root's
// `asr_oracle/gigaam/<clip>_<variant>/`, minted by the `mint_gigaam.sh` stocked beside them over the corpus clips the
// cells read through `asr_clip`; encoder weights
// rounded through f16 - the carrier's precision) - the external-reference compare: the f16 lane stage by stage, the q8
// lane by its transcript's words, and the long-form spans. Every cell skips without its carrier, its dumps or its clip;
// the facade and `.dlim` cells also skip when interpreted (`jit_model_available`), the device decode twin where the build
// or the box has no device transducer seat (`rnnt_seat_skip`), the kq lane's device leg where no GPU device stands up for
// the serving tower (`serving_tower_device_up`), the CTC-head cell on a build without das_metal and when
// interpreted, the decoder-needs-its-mmproj arm and the foreign-`.dlim`
// refusal without SmolLM2-135M-Instruct-Q8_0.gguf, and the side-file arm without the Qwen3-ASR-0.6B mmproj and decoder.

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

def private carrier(kind : string) : string => path_join(models_dir(), "gigaam-v3-{replace(kind, "_", "-")}-f16.gguf")

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

def private mint(path : string; q8 : bool; pinned : bool = false; dev : TowerDev = TowerDev.none) : GigaamModel {
    var inscope st <- stage_gigaam_model(path, q8, false, pinned)
    var m = GigaamModel()
    mint_gigaam_model(st, m, q8, dev)
    return <- m
}

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

//! `mint` in the flavor the lane serves on this build (`#tower-dev-block`): the GPU driver's where its chain serves the lane, else
//! the CPU image
def private mint_served(path : string; q8 : bool; pinned : bool = false) : GigaamModel {
    var inscope st <- stage_gigaam_model(path, q8, false, pinned)
    var m = GigaamModel()
    mint_gigaam_model(st, m, q8, gigaam_dev_flavor(st.e))
    return <- m
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
        set_serving_gpu_tower(false)   //! the CPU chain stage by stage: the device head's own oracle cell is test_gigaam_ctc_head_oracle
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

//! The words the transcript and the reference share in order: their longest common subsequence over the longer one's word count, so a dropped word and an inserted one each cost one word.
def private word_agreement(got, want : string) : float {
    let g <- ru_words(got)
    let w <- ru_words(want)
    let ng = length(g)
    let nw = length(w)
    var row, prev : array<int>
    row |> resize(nw + 1)
    prev |> resize(nw + 1)
    for (i in range(ng)) {
        for (j in range(nw)) {
            row[j + 1] = g[i] == w[j] ? prev[j] + 1 : max(prev[j + 1], row[j])
        }
        swap(row, prev)
    }
    return float(prev[nw]) / float(max(max(nw, ng), 1))
}

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

[cold_path]
def private variant_cells(tst : T?; kind, clips : string) {
    let path = carrier(kind)
    return if (!model_available(tst, path))
    tst |> run("{kind}: the f16 lane matches the external reference stage by stage") @(t : T?) {
        var inscope m <- mint(path, false)
        t |> equal(gigaam_exec_fmt(m), "f16", "{kind}: the exact lane's name")
        for (clip in split(clips, ",")) {
            if (dumps_present(t, clip, kind)) {
                reference_clip(t, m, clip, kind)
            }
        }
    }
    tst |> run("{kind}: the q8 lane's transcript agrees with the oracle's words") @(t : T?) {
        var inscope m <- mint(path, true)
        t |> equal(gigaam_exec_fmt(m), "q8", "{kind}: the q8 lane's name")
        for (clip in split(clips, ",")) {
            if (dumps_present(t, clip, kind)) {
                q8_clip(t, m, clip, kind)
            }
        }
    }
}

[test]
def test_gigaam_ctc(tst : T?) {
    with_dasllama_jobque_() {
        variant_cells(tst, "ctc", "ru_short,ru_mid")
    }
}

//! the kq lane's Q8_0 seats of the carrier at `path` against the file's own blocks: staged held under a GPU flavor's staging (no
//! CPU backend lays the planes), every Q8_0 GEMM's quants and scales the disk block's bit for bit
[cold_path]
def private q8_seats_exact(t : T?; path : string) {
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

//! the kq-lane cells' reference: the f16 carrier's exact-lane transcript of `clip` (the lane the oracle holds stage by stage), so they run with no oracle dumps
[cold_path]
def private exact_reference(t : T?; clip : string; var want : string&) : bool {
    let f16 = carrier("e2e_rnnt")
    return false if (!model_available(t, f16))
    if (!stat(asr_clip("{clip}.wav")).is_valid) {
        t |> skip("{clip}.wav not present - the f16 lane's reference transcript")
        return false
    }
    var inscope m <- mint(f16, false)
    want = transcript_of(m, load_audio_16k_mono(asr_clip("{clip}.wav")))
    return true
}

//! The kq lane over the published Q4_K carrier - the hosted file whose attention and FF GEMMs are Q4_K and conv pointwise GEMMs
//! Q8_0: the carrier's own planes serve, named by their one K-quant member; the GPU block seats serve it (the parity cells
//! `test_gigaam_vulkan_twin` and `test_gigaam_metal_twin`); its Q8_0 seats are the carrier's blocks. Every lane's transcript
//! holds the q8 bar against the f16 carrier's exact lane.
[test]
def test_gigaam_kq_lane(tst : T?) {
    with_dasllama_jobque_() {
        let path = kq_carrier()
        return if (!model_available(tst, path))
        tst |> run("e2e_rnnt q4_k: the kq lane serves the carrier's own planes, on the GPU flavor too") @(t : T?) {
            {
                var inscope m <- mint(path, false)
                t |> success(m.kq_lane, "the un-pinned load takes the kq lane")
                t |> equal(gigaam_exec_fmt(m), "k4", "the lane's name: the one K-quant member")
                t |> equal(gigaam_kq_members(m), "k4+q8", "the carrier's members: Q4_K attention and FF, Q8_0 conv pointwise")
                t |> success(!gigaam_device_gpu(m), "the device-lane report: the CPU image's encode is the CPU's")
                for (clip in split("ru_short,ru_mid", ",")) {
                    var want = ""
                    if (exact_reference(t, clip, want)) {
                        lane_clip(t, m, clip, "e2e_rnnt", "kq", want, "f16 lane")
                    }
                }
                if (!serving_tower_device_up()) {
                    t |> skip("no GPU device stands up for the serving tower on this build or box - the device leg")
                    return
                }
                with_gpu_knobs() {
                    set_serving_gpu_tower(true)
                    let dev = gigaam_dev_flavor(GigaamModel(kq_lane = true))
                    t |> equal(dev, serving_tower_dev(), "the serving tower's flavor serves the kq lane where its device stands up")
                    return if (dev == TowerDev.none)
                    var inscope md <- mint(path, false, false, dev)
                    t |> success(md.kq_lane && md.dev_backend == dev && gigaam_device_gpu(md), "the {dev}-flavored image of the kq lane: the device-lane report names the device")
                    let declines = gpu_tower_declines_of("kq_lane")
                    var want = ""
                    if (exact_reference(t, "ru_short", want)) {
                        let e0 = gpu_tower_encodes()
                        lane_clip(t, md, "ru_short", "e2e_rnnt", "kq {dev}", want, "f16 lane")
                        t |> success(gpu_tower_encodes() > e0, "the device served the kq lane's encodes ({gpu_tower_encodes() - e0})")
                        t |> equal(gpu_tower_declines_of("kq_lane"), declines, "no kq_lane decline: every member of the carrier has a tile on the device")
                    }
                }
            }
        }
        tst |> run("e2e_rnnt q4_k: the kq lane's Q8_0 seats are the carrier's blocks, moved exactly") @(t : T?) {
            q8_seats_exact(t, path)
        }
    }
}

//! The Q4_K carrier under the lane flag and pins: the q8 flag a kq carrier's mint takes changes nothing, the family's pin reaches
//! the facade load, and a pinned q8 or exact lane dequantizes the carrier into that lane's planes.
[test]
def test_gigaam_kq_lane_pins(tst : T?) {   // nolint:STYLE038 - a flat list of cells over one carrier
    with_dasllama_jobque_() {
        let path = kq_carrier()
        return if (!model_available(tst, path))
        tst |> run("e2e_rnnt q4_k: the q8 flag a kq carrier's staging and mint take changes nothing - the carrier's planes decide") @(t : T?) {
            var inscope m <- mint(path, false)
            var inscope st8 <- stage_gigaam_model(path, true)
            var inscope m8 = GigaamModel()
            mint_gigaam_model(st8, m8, true)
            t |> success(m8.kq_lane && !m8.q8, "asked for q8, the kq carrier still takes the kq lane")
            t |> equal(gigaam_exec_fmt(m8), gigaam_exec_fmt(m), "the lane's name the q8=false mint's")
            let clip = asr_clip("ru_short.wav")
            if (!stat(clip).is_valid) {
                t |> skip("ru_short.wav not present - the transcript compare")
                return
            }
            let samples <- load_audio_16k_mono(clip)
            t |> equal(transcript_of(m8, samples), transcript_of(m, samples), "the transcript the q8=false mint's")
        }
        tst |> run("e2e_rnnt q4_k: the family's pin reaches the facade load - set_gigaam_q8 serves the q8 lane") @(t : T?) {
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
        tst |> run("e2e_rnnt q4_k: the kq lane's encoder rows sit no further from the carrier widened to f32 than the pinned q8 lane's requant of it") @(t : T?) {
            let clip = asr_clip("ru_short.wav")
            if (!stat(clip).is_valid) {
                t |> skip("ru_short.wav not present - the rows compare")
                return
            }
            let samples <- load_audio_16k_mono(clip)
            var inscope mw <- mint(path, false, true)
            var inscope mk <- mint(path, false)
            var inscope mq <- mint(path, true, true)
            var inscope xw <- encoder_rows(mw, samples)
            var inscope xk <- encoder_rows(mk, samples)
            var inscope xq <- encoder_rows(mq, samples)
            var sk, sq = TowerParityStats()
            return if (!tower_parity_stats(t, "kq lane vs widened", xw, xk, sk) || !tower_parity_stats(t, "q8 lane vs widened", xw, xq, sq))
            to_log(LOG_INFO, "test_gigaam: q4_k rows rel_l2 from the widened carrier - kq lane {sk.rel_l2}, pinned q8 lane {sq.rel_l2}\n")
            t |> success(is_finite(sk.rel_l2) && sk.rel_l2 > 0.0lf, "the kq lane's activation quants move its rows off the widened chain's - two chains compared ({sk.rel_l2})")
            let bar = KQ_ROWS_SLACK * sq.rel_l2
            t |> success(sk.rel_l2 <= bar, "the kq lane's rows within {KQ_ROWS_SLACK}x the q8 lane's distance from the widened carrier ({sk.rel_l2} vs {bar})")
            let f16 = carrier("e2e_rnnt")
            return if (!model_available(t, f16))
            var inscope mf <- mint(f16, false)
            var inscope xf <- encoder_rows(mf, samples)
            var sf = TowerParityStats()
            return if (!tower_parity_stats(t, "f16 carrier vs widened q4_k", xw, xf, sf))
            to_log(LOG_INFO, "test_gigaam: the f16 carrier's exact rows rel_l2 from the widened q4_k carrier {sf.rel_l2}\n")
            t |> success(is_finite(sf.rel_l2) && sf.rel_l2 > bar, "the control - a chain on the f16 carrier's weights lands outside the bar ({sf.rel_l2} vs {bar})")
        }
        tst |> run("e2e_rnnt q4_k: a pinned q8 lane dequantizes the carrier into Q8_0 planes") @(t : T?) {
            var inscope m <- mint(path, true, true)
            t |> success(!m.kq_lane, "the pin keeps the carrier off the kq lane")
            t |> equal(gigaam_exec_fmt(m), "q8", "the pinned lane's name")
            var want = ""
            if (exact_reference(t, "ru_short", want)) {
                lane_clip(t, m, "ru_short", "e2e_rnnt", "q8", want, "f16 lane")
            }
        }
        tst |> run("e2e_rnnt q4_k: a pinned exact lane widens the carrier to f32") @(t : T?) {
            var inscope m <- mint(path, false, true)
            t |> success(!m.kq_lane, "the pin keeps the carrier off the kq lane")
            t |> equal(gigaam_exec_fmt(m), "f32", "the pinned lane's name: no f16 block GEMMs to serve")
            var want = ""
            if (exact_reference(t, "ru_short", want)) {
                lane_clip(t, m, "ru_short", "e2e_rnnt", "f32", want, "f16 lane")
            }
        }
    }
}

//! The facade's fp32 pin over the Q4_K carrier: the exact lane widens the carrier to f32, which no GPU chain reads, so the load
//! maps the CPU image whatever the box serves.
[test]
def test_gigaam_fp32_pin_load(tst : T?) {
    with_dasllama_jobque_() {
        let path = kq_carrier()
        return if (!model_available(tst, path))
        tst |> run("e2e_rnnt q4_k: the facade's fp32 pin loads the widened carrier on the CPU image - no GPU chain reads f32 block GEMMs") @(t : T?) {
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

//! The kq lane over the other carriers: a mint mixing two K-quant members (Q4_K attention, Q6_K FF), whose held staging mints the
//! streamed mint's model, and the Q8_0 carrier, whose every seat is Q8_0.
[test]
def test_gigaam_kq_other_carriers(tst : T?) {
    with_dasllama_jobque_() {
        tst |> run("e2e_rnnt q4_k + q6_k: the kq lane serves a carrier mixing two K-quant members") @(t : T?) {
            let mix_path = path_join(models_dir(), "gigaam-v3-e2e-rnnt-q4_k-mix-local.gguf")
            return if (!model_available(t, mix_path))
            var inscope m <- mint(mix_path, false)
            t |> success(m.kq_lane, "the un-pinned load takes the kq lane")
            t |> equal(gigaam_exec_fmt(m), "kq", "the lane's name where the K-quant members mix")
            t |> equal(gigaam_kq_members(m), "k4+k6+q8", "the members: Q4_K attention, Q6_K FF, Q8_0 conv pointwise")
            var want = ""
            if (exact_reference(t, "ru_short", want)) {
                lane_clip(t, m, "ru_short", "e2e_rnnt", "kq mix", want, "f16 lane")
                // the held staging writes every member at the plan's own offsets, each on its format's strides
                var inscope sth <- stage_gigaam_model(mix_path, false, true)
                var inscope mh = GigaamModel()
                mint_gigaam_model(sth, mh, false)
                let samples <- load_audio_16k_mono(asr_clip("ru_short.wav"))
                t |> equal(transcript_of(mh, samples), transcript_of(m, samples), "the held staging mints the streamed mint's model: the same transcript")
            }
        }
        tst |> run("e2e_rnnt q8_0: a carrier whose every block GEMM is Q8_0 serves them exactly on the kq lane") @(t : T?) {
            let q8_path = path_join(models_dir(), "gigaam-v3-e2e-rnnt-q8_0.gguf")
            return if (!model_available(t, q8_path))
            {
                var inscope m <- mint(q8_path, false)
                t |> success(m.kq_lane, "the un-pinned load takes the kq lane")
                t |> equal(gigaam_exec_fmt(m), "q8", "the lane's name where every seat is Q8_0")
                t |> equal(gigaam_kq_members(m), "q8", "the carrier's one member")
                var want = ""
                if (exact_reference(t, "ru_short", want)) {
                    lane_clip(t, m, "ru_short", "e2e_rnnt", "kq q8", want, "f16 lane")
                }
            }
            q8_seats_exact(t, q8_path)
        }
    }
}

//! The Q4_K carrier's load on the box's GPU flavor (`#gigaam-planes`): the kq lane on the device where its chain serves that lane,
//! else the kq lane on the CPU image - a Vulkan device off the f16 GEMM feed (`DASLLAMA_COOPMAT=sdot4`) among them.
[test]
def test_gigaam_kq_load_flavor(tst : T?) {
    with_dasllama_jobque_() {
        let path = kq_carrier()
        return if (!model_available(tst, path))
        tst |> run("e2e_rnnt q4_k: the load serves the carrier's own planes - on the device where its chain serves the lane, else on the CPU image") @(t : T?) {
            reset_gigaam_q8()
            with_gpu_knobs() {
                set_serving_gpu_tower(true)
                let kq_dev = serving_tower_device_up() ? serving_tower_dev() : TowerDev.none   // the box fact: the device the chain rides stands up
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
                    if (exact_reference(t, "ru_short", want)) {
                        lane_clip(t, m, "ru_short", "e2e_rnnt", gigaam_exec_fmt(m), want, "f16 lane")
                    }
                }
                remove(dlim)
            }
        }
    }
}

//! a copy of the carrier at `src` written to `dst` with the tensors `retype` names retyped, every weight byte zero and every
//! offset recomputed - the planner's refusals read the tensors' types alone, so a carrier no quantize tool mints stands in for one
[cold_path]
def private ga_retyped_carrier(src, dst : string; retype : table<string; int>) {
    var out : array<uint8>
    var zeros = 0l
    let f = fopen(src, "rb")
    fmap(f) $(var bytes : array<uint8>#) {
        var inscope m <- parse_gguf_meta(bytes)
        var kv_last = 24l   // the tensor table starts at the first tensor's name record, past the last KV's value
        for (loc in values(m.kv)) {
            kv_last = max(kv_last, loc.data_off)
        }
        var rec : array<uint8>
        gguf_push_str(rec, m.tensors[0].name)
        var table_at = -1l
        for (o in range64(kv_last, long_length(bytes) - long_length(rec))) {
            var hit = true
            for (i in range64(long_length(rec))) {
                if (bytes[o + i] != rec[i]) {
                    hit = false
                    break
                }
            }
            if (hit) {
                table_at = o
                break
            }
        }
        verify(table_at > 0l, "the carrier's tensor table not found")
        gguf_header(out, uint64(long_length(m.tensors)), uint64(long_length(m.kv)))
        let head = long_length(out)
        out |> resize(head + table_at - 24l)
        for (i in range64(24l, table_at)) {
            out[head + i - 24l] = bytes[i]
        }
        var off = 0l
        for (ti in m.tensors) {
            let gt = retype?[ti.name] ?? ti.ggml_type
            gguf_push_str(out, ti.name)
            gguf_push_le(out, uint64(long_length(ti.dims)), 4)
            for (dim in ti.dims) {
                gguf_push_le(out, uint64(dim), 8)
            }
            gguf_push_le(out, uint64(gt), 4)
            gguf_push_le(out, uint64(off), 8)
            off = (off + ggml_type_bytes(gt, ti.n_elem) + m.alignment - 1l) / m.alignment * m.alignment
        }
        let data_at = (long_length(out) + m.alignment - 1l) / m.alignment * m.alignment
        zeros = data_at - long_length(out) + off
    }
    fclose(f)
    let w = fopen(dst, "wb")
    fwrite(w, out)
    var chunk : array<uint8>
    chunk |> resize(1048576)
    while (zeros > 0l) {   // the pad to the data's alignment, then every tensor's zero bytes
        let n = min(zeros, long_length(chunk))
        if (n < long_length(chunk)) {
            chunk |> resize(n)
        }
        fwrite(w, chunk)
        zeros -= n
    }
    fclose(w)
    delete chunk
    delete out
}

//! the planner's refusal of the Q4_K carrier with `retype` applied, run through `stage` - "" where it staged
[cold_path]
def private ga_retyped_refusal(name : string; retype : table<string; int>; stage : block<(path : string) : void>) : string {
    let dst = path_join(test_temp_dir(), "{name}.gguf")
    ga_retyped_carrier(kq_carrier(), dst, retype)
    let why = panic_text() $ {
        invoke(stage, dst)
    }
    remove(dst)
    return why
}

//! The kq lane planner's refusals, each over a copy of the Q4_K carrier with tensors retyped (`ga_retyped_carrier`): a float
//! GEMM among the quantized ones, a member the lane has no seat for (a per-32 Q5_1, NVFP4), q, k and v across two classes, and a
//! file that does not read; the lane peek's float disjuncts - BF16 and F32 GEMMs are the exact lane's, not the kq lane's.
[test]
def test_gigaam_kq_refusals(tst : T?) {
    let path = kq_carrier()
    return if (!model_available(tst, path))
    let l0 = "encoder.layers.0."
    tst |> run("the kq carrier's planner refuses what its lane cannot seat") @(t : T?) {
        let mixed = ga_retyped_refusal("ga_mixed", { "{l0}ff1.linear1.weight" => GGML_TYPE_F16 }) $(p) {
            gigaam_lane_tag(p, false, false)
        }
        t |> success(find(mixed, "mix quantized and float tensors") >= 0, "a float GEMM among quantized ones refused at the lane peek: '{mixed}'")
        for (gt in [GGML_TYPE_Q5_1, GGML_TYPE_NVFP4]) {
            let why = ga_retyped_refusal("ga_seat_{gt}", { "{l0}ff1.linear1.weight" => gt }) $(p) {
                var inscope st <- stage_gigaam_model(p)
            }
            t |> success(find(why, "has no seat for") >= 0 && find(why, "{l0}ff1.linear1.weight") >= 0, "a {ggml_type_name(gt)} GEMM refused at its seat naming the tensor: '{why}'")
        }
        let qkv = ga_retyped_refusal("ga_qkv", { "{l0}attn.q.weight" => GGML_TYPE_Q8_0 }) $(p) {
            var inscope st <- stage_gigaam_model(p)
        }
        t |> success(find(qkv, "mix Q8_0 with a K-quant format") >= 0, "q in Q8_0 beside a Q4_K k and v refused: '{qkv}'")
        let nothing : table<string; int>
        let none = ga_retyped_refusal("ga_clean", nothing) $(p) {
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
        ga_retyped_carrier(path, dst, retype)
        t |> equal(gigaam_lane_tag(dst, false, false), "gigaam-f16", "BF16 and F32 GEMMs: the exact lane's tag")
        t |> equal(gigaam_lane_tag(path, false, false), "gigaam-kq", "the Q4_K carrier: the kq lane's tag (the control)")
        remove(dst)
        delete retype
    }
}

//! A CTC carrier's device block carries no transducer half: the bake every GPU flavor runs places none, model-free.
[test]
def test_gigaam_ctc_bakes_no_transducer(t : T?) {
    t |> run("the transducer half of a GPU flavor's bake is the RNNT head's alone") @(t : T?) {
        var inscope b = TowerDevBuild()
        let why = panic_text() $ {
            gigaam_rnnt_dev_bake(GigaamModel(head = GigaamHead.ctc, d_model = 768l), b, "probe.rnnt")
        }
        t |> equal(why, "", "a CTC carrier's bake does not refuse")
        t |> equal(long_length(b.plan), 0l, "and places no transducer piece")
    }
}

//! `tower_mm`'s form without the Q8_K sums refuses a kq-lane model - its K-quant seats read them - model-free.
[test]
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
[test]
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

[test]
def test_gigaam_e2e_rnnt(tst : T?) {
    with_dasllama_jobque_() {
        variant_cells(tst, "e2e_rnnt", "ru_short,ru_mid")
    }
}

[test]
def test_gigaam_e2e_ctc(tst : T?) {
    with_dasllama_jobque_() {
        variant_cells(tst, "e2e_ctc", "ru_short")
    }
}

[test]
def test_gigaam_charwise_rnnt(tst : T?) {
    with_dasllama_jobque_() {
        variant_cells(tst, "rnnt", "ru_short")
    }
}

[test]
def test_gigaam_long_form(tst : T?) {
    tst |> run("ru_long: past the cap the clip splits at silences into spans of at most 25 s, the words the references'") @(t : T?) {
        let path = carrier("e2e_rnnt")
        let wav = asr_clip("ru_long.wav")
        let refs_path = asr_clip("ru_fleurs.json")
        return if (!model_available(t, path))
        if (!stat(wav).is_valid || !stat(refs_path).is_valid) {
            t |> skip("ru_long.wav or ru_fleurs.json not present")
            return
        }
        var refs = FleursRefs()
        t |> success(load_json_file(refs_path, refs), "ru_fleurs.json read")
        let reference = join([for (c in refs.long_clips); c.text], " ")
        with_dasllama_jobque_() {
            var inscope m <- mint(path, false)
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
    }
}

[test]
def test_gigaam_facade(tst : T?) {
    tst |> run("a decoder GGUF needs its mmproj, where the gigaam GGUF loads alone") @(t : T?) {
        let decoder = path_join(models_dir(), "SmolLM2-135M-Instruct-Q8_0.gguf")
        return if (!model_available(t, decoder))
        t |> success(asr_needs_mmproj(decoder), "a decoder GGUF needs its mmproj")
    }
    tst |> run("the facade's gigaam arms: caps, exec format, the one-file carrier, session language, transcribe") @(t : T?) {
        let path = carrier("e2e_rnnt")
        let wav = asr_clip("ru_short.wav")
        return if (!jit_model_available(t, path))
        if (!stat(wav).is_valid) {
            t |> skip("ru_short.wav not present")
            return
        }
        t |> success(!asr_needs_mmproj(path), "the gigaam GGUF loads alone")
        with_dasllama_jobque_() {
            var inscope m = AsrModel(kind = AsrKind.gigaam)
            m.ga <- mint(path, false)
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

[test]
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

[test]
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

[test]
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

[test]
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

[test]
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
        let ga = carrier("e2e_rnnt")
        let mmproj = path_join(models_dir(), "mmproj-Qwen3-ASR-0.6B-bf16.gguf")
        let decoder = path_join(models_dir(), "Qwen3-ASR-0.6B-Q8_0.gguf")
        return if (!model_available(t, ga) || !model_available(t, mmproj) || !model_available(t, decoder))
        t |> success(gguf_is_side_file(ga), "the GigaAM carrier is a side file")
        t |> success(gguf_is_side_file(mmproj), "the mmproj is a side file")
        t |> success(!gguf_is_side_file(decoder), "the decoder is no side file")
    }
    tst |> run("a .dlim another family prepared is refused by the GigaAM loader") @(t : T?) {
        let ga = carrier("e2e_rnnt")
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
            let taken = panic_text() ${
                var inscope cold <- load_gigaam_model(ga)
                var inscope warm <- load_gigaam_model("{ga}.gigaam-f16.dlim")
                t |> equal(warm.n_layer, cold.n_layer, "the carrier's own image maps")
            }
            t |> equal(taken, "", "the carrier's own image: no refusal")
        }
    }
}

[test]
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

//! one clip's encoder rows decoded on the CPU loop and on the build's device seat, the serving tower pinned on: the same ids and
//! frames, the plog within `DEVICE_PLOG_BAR`, the decode counted on the device - projected there, off the encode's own device rows
//! where the tower encoded (the encode recorded a device generation), off uploaded rows where the CPU encoded; both decodes'
//! transcripts logged
[cold_path]
//! `m` the lane's GPU-flavored model, `mc` its CPU image: the device encode's rows decoded on both (`rnnt_device_twin`)
def private device_decode_clip(t : T?; m, mc : GigaamModel; clip, tag : string) {
    return if (rnnt_seat_skip(t, tag))
    if (m.dev_backend == TowerDev.none) {
        t |> skip("{tag}: the box serves the lane on its CPU image (a Vulkan device off the f16 GEMM feed) - the device seat decodes GPU images alone")
        return
    }
    let path = asr_clip("{clip}.wav")
    if (!stat(path).is_valid) {
        t |> skip("{tag}: {clip}.wav not present")
        return
    }
    with_gpu_knobs() {
        set_serving_gpu_tower(true)
        let samples <- load_audio_16k_mono(path)
        var st = GigaamState()
        var stc = GigaamState()
        let tt = gigaam_encode_span(samples, 0l, min(long_length(samples), GIGAAM_MAX_CLIP_S * AUDIO_SAMPLE_RATE), m, st)
        let device_encoded = st.enc_gen != 0ul
        var inscope tw <- rnnt_device_twin(t, "test_gigaam: {tag}", DEVICE_PLOG_BAR, 1l, device_encoded ? 1l : 0l, st.x, $(var toks) {
            stc.x := st.x
            gigaam_decode(mc, stc, tt)
            toks := stc.toks
        }) $(var toks) {
            gigaam_decode(m, st, tt)
            toks := st.toks
        }
        to_log(LOG_INFO, "test_gigaam: {tag} cpu   : {gigaam_detokenize(m, tw.cpu)}\n")
        to_log(LOG_INFO, "test_gigaam: {tag} device: {gigaam_detokenize(m, tw.dev)}\n")
        delete stc
        delete st
    }
}

[test]
def test_gigaam_device_decode_twin(tst : T?) {
    with_dasllama_jobque_() {
        for (kind in ["e2e_rnnt", "rnnt"]) {
            let path = carrier(kind)
            continue if (!model_available(tst, path))
            for (q8 in [false, true]) {
                tst |> run("{kind} {q8 ? "q8" : "f16"}: the transducer decode on the device seat is the CPU decoder's") @(t : T?) {
                    var inscope m <- mint_served(path, q8)
                    var inscope mc <- mint(path, q8)
                    for (clip in ["ru_short", "ru_mid"]) {
                        device_decode_clip(t, m, mc, clip, "{kind} {q8 ? "q8" : "f16"} {clip}")
                    }
                }
            }
        }
    }
}

//! the CTC head on the Metal seat against the CPU head over one encode (an f16 head's CPU leg over half-rounded rows): each pick the CPU's but at a near-tie, -log sum exp within `CTC_PLOG_BAR`, the ids and transcript the CPU's, the device leg counted once and the CPU leg never, the 1.05x control outside the bar; then the head off the first clip's uploaded rows; `m` Metal-flavored, `mc` its CPU image
[cold_path, unused_argument(t, m, mc, samples, tag)]
def private ctc_head_metal_cell(t : T?; m, mc : GigaamModel; samples : array<float>; tag : string) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        with_gpu_knobs() {
            set_metal_tower(true)
            let bar = Bar(rel = CTC_PLOG_BAR, abs_floor = CTC_PLOG_BAR, poison_scaled = true)
            var st = GigaamState()
            let n = gigaam_encode_span(samples, 0l, min(long_length(samples), GIGAAM_MAX_CLIP_S * AUDIO_SAMPLE_RATE), m, st)
            t |> success(st.enc_gen != 0ul, "{tag}: the encode served on the device")
            set_metal_tower(false)
            var sc = GigaamState()
            sc.x := st.x
            var inscope cpu <- ctc_cpu_leg(mc, sc, n)
            delete sc
            set_metal_tower(true)
            var inscope dev <- ctc_leg(m, st, n)
            to_log(LOG_INFO, "test_gigaam: {tag} cpu   : {cpu.text}\n")
            to_log(LOG_INFO, "test_gigaam: {tag} device: {dev.text}\n")
            t |> equal(cpu.served, 0l, "{tag}: the tower knob off - the head stays on the CPU")
            t |> equal(dev.served, 1l, "{tag}: the head served on the device")
            t |> equal(ctc_hard_flips(cpu, dev, m.n_classes), 0, "{tag}: the picks are the CPU's but at a near-tie frame")
            check(t, "test_gigaam: {tag} the device head's per-frame log-sums", dev.plogs, cpu.plogs, bar, FLAT_SHAPE)
            t |> success(sequence_equal(dev.ids, cpu.ids), "{tag}: the token ids are the CPU head's")
            t |> equal(dev.text, cpu.text, "{tag}: the transcript")
            var inscope loud <- [for (v in samples); v * 1.05]
            var sl = GigaamState()
            let nl = gigaam_encode_span(loud, 0l, min(long_length(loud), GIGAAM_MAX_CLIP_S * AUDIO_SAMPLE_RATE), m, sl)
            var inscope moved <- ctc_leg(m, sl, nl)
            t |> equal(moved.served, 1l, "{tag}: the control's head served on the device")
            check_exceeds(t, "test_gigaam: {tag} the control - the clip at 1.05x through the device chain", moved.plogs, cpu.plogs, bar)
            t |> success(sl.enc_gen != st.enc_gen, "{tag}: the control's encode holds the device rows now")
            delete sl
            var inscope up <- ctc_leg(m, st, n)
            t |> equal(up.served, 1l, "{tag}: off rows the device no longer holds the head serves off the uploaded rows")
            t |> equal(ctc_hard_flips(cpu, up, m.n_classes), 0, "{tag}: the uploaded rows' picks are the CPU head's but at a near-tie frame")
            check(t, "test_gigaam: {tag} the head off the uploaded rows' log-sums", up.plogs, cpu.plogs, bar, FLAT_SHAPE)
            t |> equal(up.text, cpu.text, "{tag}: the uploaded rows' transcript")
            delete st
        }
    }
}
[test]
def test_gigaam_ctc_head_metal(tst : T?) {
    with_dasllama_jobque_() {
        for (kind, q8 in ["e2e_ctc", "ctc", "e2e_ctc", "ctc"], [false, false, true, true]) {
            tst |> run("{kind} {q8 ? "q8" : "f16"}: the CTC head on the Metal device seat is the CPU head's") @(t : T?) {
                static_if (!typeinfo builtin_module_exists(das_metal)) {
                    t |> skip("not a Metal build - the CTC head's device seat is Metal's")
                } else {
                    let path = carrier(kind)
                    return if (!jit_model_available(t, path))
                    let wav = asr_clip("ru_short.wav")
                    if (!stat(wav).is_valid) {
                        t |> skip("ru_short.wav not present")
                        return
                    }
                    var inscope m <- mint(path, q8, false, TowerDev.metal)
                    var inscope mc <- mint(path, q8)
                    let samples <- load_audio_16k_mono(wav)
                    ctc_head_metal_cell(t, m, mc, samples, "{kind} {q8 ? "q8" : "f16"} ru_short")
                }
            }
        }
    }
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
        set_serving_gpu_tower(device)
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

//! The CTC head alone against the oracle on one lane: the oracle's encoder rows (`encoded.npy`) through the head on the CPU (the
//! tower off) and, where the build's device seat serves on the box (Metal's, else Vulkan's), on that seat (counted once, off the
//! uploaded rows) - each frame's pick the oracle's argmax but at a near-tie
//! frame, the emitted tokens' probabilities within `PLOG_BAR` of the oracle's softmax, the ids the oracle's. The head reads the
//! carrier's halfwords in ``wblob``, so the control is the same carrier staged with the head's halfwords zeroed there: its ids
//! leave the oracle's on every leg.
[cold_path]
def private ctc_head_oracle_cell(t : T?; path, kind : string; q8 : bool) {
    let clip = "ru_short"
    return if (!dumps_present(t, clip, kind))
    let dir = dump_dir(clip, kind)
    let tag = "{clip}_{kind} {q8 ? "q8" : "f16"} ctc head"
    let enc <- read_npy("{dir}/encoded.npy")
    let lp <- read_npy("{dir}/ctc_logprobs.npy")
    var tk = OracleTokens()
    t |> success(load_json_file("{dir}/tokens.json", tk), "{tag}: tokens.json read")
    var inscope m <- mint(path, q8)
    var inscope md <- mint_served(path, q8)
    t |> success(m.ctc_f16, "{tag}: the head serves off the carrier's halfwords")
    t |> equal(enc.cols, m.d_model, "{tag}: the oracle's encoder rows are d_model wide")
    t |> equal(lp.cols, m.n_classes, "{tag}: the oracle's log-probabilities are n_classes wide")
    var legs = [false]
    if (rnnt_seat_ready()) {
        legs |> push(true)
    }
    for (device in legs) {
        let leg = device ? "{tag} {rnnt_seat_name()}" : "{tag} cpu"
        var inscope r <- ctc_oracle_leg(device ? md : m, enc, lp, device)
        to_log(LOG_INFO, "test_gigaam: {leg}: token probabilities off the oracle's softmax by {r.worst}: {r.text}\n")
        t |> equal(r.served, device ? 1l : 0l, "{leg}: the head served {device ? "on the device seat" : "on the CPU"}")
        t |> equal(oracle_hard_flips(r.picks, lp), 0, "{leg}: the picks are the oracle's argmax but at a near-tie frame")
        t |> success(!empty(r.ids) && r.worst <= PLOG_BAR, "{leg}: token probabilities within {PLOG_BAR} of the oracle's softmax ({r.worst})")
        t |> success(sequence_equal(r.ids, tk.ids), "{leg}: the token ids are the oracle's")
    }
    for (device in legs) {
        let dev = device ? md.dev_backend : TowerDev.none
        var inscope stg <- tower_dev_staged(dev) $ { return <- stage_gigaam_model(path, q8, true) }   // held planes staged in the form the flavor bakes
        for (i in range64(stg.e.ctc_w, stg.e.ctc_w + stg.e.n_classes * stg.e.d_model)) {
            stg.wblob[i] = uint16(0)
        }
        var inscope mz = GigaamModel()
        mint_gigaam_model(stg, mz, q8, dev)
        var inscope z <- ctc_oracle_leg(mz, enc, lp, device)
        t |> success(!sequence_equal(z.ids, tk.ids), "{tag} {device ? rnnt_seat_name() : "cpu"}: the control - the head's halfwords zeroed in wblob leave the oracle's ids ({length(z.ids)} tokens)")
    }
}

[test]
def test_gigaam_ctc_head_oracle(tst : T?) {
    with_dasllama_jobque_() {
        for (kind, q8 in ["ctc", "e2e_ctc", "ctc", "e2e_ctc"], [false, false, true, true]) {
            tst |> run("{kind} {q8 ? "q8" : "f16"}: the CTC head over the oracle's encoder rows reads its halfwords and lands the oracle's picks, on the CPU and the build's device seat") @(t : T?) {
                let path = carrier(kind)
                return if (!jit_model_available(t, path))
                ctc_head_oracle_cell(t, path, kind, q8)
            }
        }
    }
}

//! the corpus harness spawned on this binary over `args` under DASLLAMA_IMAGE=0 (no sidecar beside the stocked carrier), its
//! output into `out`; 300 s covers a cold JIT of the facade's ASR and translation modules
def private run_corpus_eval(args : array<string>; var out : string&) : int {
    let prev_image = env_value_of("DASLLAMA_IMAGE")
    set_env_variable("DASLLAMA_IMAGE", "0")
    let root = get_das_root()
    var argv <- [example_daslang(), "-jit", "-dasroot", root, "{root}/modules/dasLLAMA/harness/asr_corpus_eval.das", "--"]
    argv |> push_from(args)
    let rc = run_and_capture(argv, out, 300.0)
    set_env_variable("DASLLAMA_IMAGE", prev_image)
    return rc
}

[test]
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
        t |> success(find(cols[4], "сред") >= 0, "the hypothesis carries the clip's words ({cols[4]})")   // nolint:STYLE039 - the clip's Russian word
        if (has_nmt && length(cols) == 7) {
            t |> success(find(cols[6], "Wednesday") >= 0, "the reference translated ({cols[6]})")
        }
        var bad = ""
        let brc = run_corpus_eval(["--model", kq_carrier(), "--corpus", corpus, "--out", hyp, "--lane", "f16"], bad)
        t |> equal(1, brc, "a lane that is neither q8 nor exact is a refusal")
        t |> success(find(bad, "--lane is q8 or exact") >= 0, "and says so by name: {slice(bad, max(0, length(bad) - 600))}")
    }
}
