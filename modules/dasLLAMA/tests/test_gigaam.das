options gen2
options persistent_heap
options stack = 524288
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_gigaam
require dasllama/dasllama_asr   // the facade's gigaam arms: caps, asr_exec_fmt, create_session, transcribe
require dasllama/dasllama_audio_io
require dasllama/dasllama_vad   // set_vad_model_path_probe_: the missing-weights refusal's seam
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
require dasllama/dasllama_image   // save_model_image
require dasllama/dasllama_common   // QuantMode
require ?das_metal dasllama/dasllama_metal_tower   // set_metal_tower: the Metal CTC cell's pin
require _ctc_oracle   // ctc_seat_heads: the build's device CTC seat's count

// GigaAM v3 against the upstream package's per-stage dumps (benchmarks/asr/gigaam_oracle.py into the model root's
// `asr_oracle/gigaam/<clip>_<variant>/`, minted by the `mint_gigaam.sh` stocked beside them over the corpus clips the
// cells read through `asr_clip`; encoder weights
// rounded through f16 - the carrier's precision) - the external-reference compare: the f16 lane stage by stage, the q8
// lane by its transcript's words, and the long-form spans. Every cell skips without its carrier, its dumps or its clip;
// the facade and `.dlim` cells also skip when interpreted (`jit_model_available`), the device decode twin where the build
// or the box has no device transducer seat (`rnnt_seat_skip`), the CTC-head cell on a build without das_metal and when
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

def private mint(path : string; q8 : bool; pinned : bool = false) : GigaamModel {
    var inscope st <- stage_gigaam_model(path, q8, false, pinned)
    var m = GigaamModel()
    mint_gigaam_model(st, m, q8)
    return <- m
}

//! `blk` as the GPU seat reads the kq carrier: on a Metal build under the row-major pin for the whole of it - the mint and
//! every load and encode after, since a model's planes match the backend at its mint and the first un-pinned load selects
//! the repacking one (the Metal mode pins every load the same way); elsewhere as any run
def private under_seat_backend(blk : block<() : void>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        if (!with_kernel_backend("portable", blk)) {
            panic("test_gigaam: the portable backend did not pin for the kq carrier")
        }
    } else {
        invoke(blk)
    }
}

//! the whole clip's transcript through `m`, one span
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

//! A quantized lane's transcript of `clip` against the words of `want` (named `ref`): one segment, Russian detected, the
//! agreement at the bar, and the control - the clip with its second half silenced reads under it.
def private lane_clip(t : T?; m : GigaamModel; clip, kind, lane : string; want, ref : string) {
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
    to_log(LOG_INFO, "test_gigaam: {clip}_{kind} {ref} : {want}\n")
    t |> success(agree >= Q8_WORD_AGREE, "{clip}_{kind} {lane}: {agree} of the {ref}'s words")
    var inscope half <- [for (v, i in samples, count()); i < length(samples) / 2 ? v : 0.0]
    let half_agree = word_agreement(transcript_of(m, half), want)
    to_log(LOG_INFO, "test_gigaam: {clip}_{kind} {lane} control (second half silenced) words {half_agree}\n")
    t |> success(half_agree < Q8_WORD_AGREE, "{clip}_{kind} {lane}: the control - the clip with its second half silenced reads {half_agree} of the {ref}'s words, under the bar")
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

//! The kq-lane cells' reference: the f16 carrier's exact-lane transcript of `clip` - the lane the oracle holds stage by stage
//! (`reference_clip`) - so a box with the two carriers and the clips runs them with no oracle dumps; skips naming what is missing.
def private exact_reference(t : T?; clip : string; var want : string&) : bool {
    let f16 = carrier("e2e_rnnt")
    if (!stat(f16).is_valid || !stat(asr_clip("{clip}.wav")).is_valid) {
        t |> skip("{clip}: the f16 carrier or the clip not present")
        return false
    }
    var inscope m <- mint(f16, false)
    want = transcript_of(m, load_audio_16k_mono(asr_clip("{clip}.wav")))
    return true
}

//! The kq lane over the published Q4_K carrier - the hosted file whose attention and FF GEMMs are Q4_K and conv pointwise GEMMs
//! Q8_0: the carrier's own planes serve, named by their one K-quant member; the GPU block seats serve it (the parity cells
//! `test_gigaam_vulkan_twin` and `test_gigaam_metal_twin`); a pinned lane dequantizes it into that lane's planes. Every lane's
//! transcript holds the q8 bar against the f16 carrier's exact lane.
[test]
def test_gigaam_kq_lane(tst : T?) {
    with_dasllama_jobque_() {
        let path = kq_carrier()
        return if (!model_available(tst, path))
        tst |> run("e2e_rnnt q4_k: the kq lane serves the carrier's own planes, on the GPU block seat too") @(t : T?) {
            under_seat_backend() {
                let declines = gpu_tower_declines_of("kq_lane")
                var inscope m <- mint(path, false)
                t |> success(m.kq_lane, "the un-pinned load takes the kq lane")
                t |> equal(gigaam_exec_fmt(m), "k4", "the lane's name: the one K-quant member")
                t |> equal(gigaam_kq_members(m), "k4+q8", "the carrier's members: Q4_K attention and FF, Q8_0 conv pointwise")
                t |> equal(gigaam_device_gpu(m), gigaam_gpu_serves(), "the GPU seat serves the kq lane's encode wherever it serves the q8 lane's")
                for (clip in split("ru_short,ru_mid", ",")) {
                    var want = ""
                    if (exact_reference(t, clip, want)) {
                        lane_clip(t, m, clip, "e2e_rnnt", "kq", want, "f16 lane")
                    }
                }
                t |> equal(gpu_tower_declines_of("kq_lane"), declines, "no kq_lane decline: every member of the carrier has a tile on the seat")
            }
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
def private device_decode_clip(t : T?; m : GigaamModel; clip, tag : string) {
    return if (rnnt_seat_skip(t, tag))
    let path = asr_clip("{clip}.wav")
    if (!stat(path).is_valid) {
        t |> skip("{tag}: {clip}.wav not present")
        return
    }
    with_gpu_knobs() {
        set_serving_gpu_tower(true)
        let samples <- load_audio_16k_mono(path)
        var st = GigaamState()
        let tt = gigaam_encode_span(samples, 0l, min(long_length(samples), GIGAAM_MAX_CLIP_S * AUDIO_SAMPLE_RATE), m, st)
        let device_encoded = st.enc_gen != 0ul
        var inscope tw <- rnnt_device_twin(t, "test_gigaam: {tag}", DEVICE_PLOG_BAR, 1l, device_encoded ? 1l : 0l, st.x) $(var toks) {
            gigaam_decode(m, st, tt)
            toks := st.toks
        }
        to_log(LOG_INFO, "test_gigaam: {tag} cpu   : {gigaam_detokenize(m, tw.cpu)}\n")
        to_log(LOG_INFO, "test_gigaam: {tag} device: {gigaam_detokenize(m, tw.dev)}\n")
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
                    var inscope m <- mint(path, q8)
                    for (clip in ["ru_short", "ru_mid"]) {
                        device_decode_clip(t, m, clip, "{kind} {q8 ? "q8" : "f16"} {clip}")
                    }
                }
            }
        }
    }
}

//! The CTC head on the Metal device seat against the CPU head over one encode (an f16 head's CPU leg over the rows rounded to
//! half, as the device stages them): each frame's pick the CPU's but at a near-tie
//! frame, its -log sum exp within `CTC_PLOG_BAR`, the ids and the transcript the CPU's; the device leg counted once, the CPU leg
//! (the tower knob off around the decode alone) not at all; the control the device chain over the clip at 1.05x, its frames'
//! log-sums outside the bar. Then a CPU-encoded clip (no held rows, generation 0) serves the head off the uploaded rows, held
//! to the CPU head over those same rows.
[cold_path, unused_argument(t, m, samples, tag)]
def private ctc_head_metal_cell(t : T?; m : GigaamModel; samples : array<float>; tag : string) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        with_gpu_knobs() {
            set_metal_tower(true)
            let bar = Bar(rel = CTC_PLOG_BAR, abs_floor = CTC_PLOG_BAR, poison_scaled = true)
            var st = GigaamState()
            let n = gigaam_encode_span(samples, 0l, min(long_length(samples), GIGAAM_MAX_CLIP_S * AUDIO_SAMPLE_RATE), m, st)
            t |> success(st.enc_gen != 0ul, "{tag}: the encode served on the device")
            set_metal_tower(false)
            var inscope cpu <- ctc_cpu_leg(m, st, n)
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
            delete sl
            t |> equal(moved.served, 1l, "{tag}: the control's head served on the device")
            check_exceeds(t, "test_gigaam: {tag} the control - the clip at 1.05x through the device chain", moved.plogs, cpu.plogs, bar)
            delete st
            set_metal_tower(false)
            var sc = GigaamState()
            let nc = gigaam_encode_span(samples, 0l, min(long_length(samples), GIGAAM_MAX_CLIP_S * AUDIO_SAMPLE_RATE), m, sc)
            t |> equal(sc.enc_gen, 0ul, "{tag}: a CPU encode records no device generation")
            var inscope cpu_up <- ctc_cpu_leg(m, sc, nc)
            set_metal_tower(true)
            var inscope up <- ctc_leg(m, sc, nc)
            t |> equal(up.served, 1l, "{tag}: off a CPU encode the head serves off the uploaded rows")
            t |> equal(ctc_hard_flips(cpu_up, up, m.n_classes), 0, "{tag}: the uploaded rows' picks are the CPU head's but at a near-tie frame")
            check(t, "test_gigaam: {tag} the head off the uploaded rows' log-sums", up.plogs, cpu_up.plogs, bar, FLAT_SHAPE)
            t |> equal(up.text, cpu_up.text, "{tag}: the uploaded rows' transcript")
            delete sc
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
                    var inscope m <- mint(path, q8)
                    let samples <- load_audio_16k_mono(wav)
                    ctc_head_metal_cell(t, m, samples, "{kind} {q8 ? "q8" : "f16"} ru_short")
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
    t |> success(m.ctc_f16, "{tag}: the head serves off the carrier's halfwords")
    t |> equal(enc.cols, m.d_model, "{tag}: the oracle's encoder rows are d_model wide")
    t |> equal(lp.cols, m.n_classes, "{tag}: the oracle's log-probabilities are n_classes wide")
    var legs = [false]
    if (rnnt_seat_ready()) {
        legs |> push(true)
    }
    for (device in legs) {
        let leg = device ? "{tag} {rnnt_seat_name()}" : "{tag} cpu"
        var inscope r <- ctc_oracle_leg(m, enc, lp, device)
        to_log(LOG_INFO, "test_gigaam: {leg}: token probabilities off the oracle's softmax by {r.worst}: {r.text}\n")
        t |> equal(r.served, device ? 1l : 0l, "{leg}: the head served {device ? "on the device seat" : "on the CPU"}")
        t |> equal(oracle_hard_flips(r.picks, lp), 0, "{leg}: the picks are the oracle's argmax but at a near-tie frame")
        t |> success(!empty(r.ids) && r.worst <= PLOG_BAR, "{leg}: token probabilities within {PLOG_BAR} of the oracle's softmax ({r.worst})")
        t |> success(sequence_equal(r.ids, tk.ids), "{leg}: the token ids are the oracle's")
    }
    var inscope stg <- stage_gigaam_model(path, q8, true)
    for (i in range64(stg.e.ctc_w, stg.e.ctc_w + stg.e.n_classes * stg.e.d_model)) {
        stg.wblob[i] = uint16(0)
    }
    var inscope mz = GigaamModel()
    mint_gigaam_model(stg, mz, q8)
    for (device in legs) {
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
