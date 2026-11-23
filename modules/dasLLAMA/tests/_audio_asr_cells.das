// The ASR families' facade and oracle cells that run under test_audio.das's blocks - the Parakeet oracles on the v2 and v3
// bins, the q8_0 bin's read against the f32 bin's, the qwen3a tower's oracle - and the session helpers they share with
// test_whisper.das's own cells: a transcription's segments, the reused-session and flat-heap legs, a pair's decoder gate.
// No load: every model is a form of its file's block.

options gen2
options _dasllama_internal = true

module _audio_asr_cells shared public

require dastest/testing_boost public
require dasllama/dasllama_asr public
require dasllama/dasllama_par    // set_single_thread_ / get_disp_count - the parakeet decode step's one-lane leg
require dasllama/dasllama_math   // with_dasllama_jobque_
require daslib/jobque_boost
require dasllama/dasllama_audio_io
require strings
require _model_tier   // asr_clip, jit_only, eyeball_text, jfk_words_floor
require dasllama/dasllama_common public   // Model, KVDtype: the oracle cells pin the f32 parity rail
require dasllama/dasllama   // eval_: the forced walk's step
require dasllama/dasllama_canary public
require dasllama/dasllama_gemma4a public
require dasllama/dasllama_qwen3a public
require daslib/fio   // base_name
require math
require _compares   // argmax_of, nan_wide_diff
require _tower_twin   // knob_off_unrouted, the tower knobs and counters
require _f32_tile_pin   // with_put_back: a pair's pieces handed back however its cell leaves

//! a fresh session over `m` (an `AsrModel` or a `WhisperModel`) transcribing `samples`, its segments collected; `lang` names the
//! session's language where the cell sets one
def transcribe_segs(m; samples : array<float>; lang : string = "") : array<TranscribeSegment> {
    if (empty(lang)) {
        var inscope s <- create_session(m)
        return <- session_segs(m, s, samples)
    }
    var inscope s <- create_session(m, lang)
    return <- session_segs(m, s, samples)
}

//! `s` transcribing `samples` - a cell reuses one session across transcriptions the way a serving worker does
def session_segs(m; var s; samples : array<float>) : array<TranscribeSegment> {
    var segs : array<TranscribeSegment>
    transcribe(m, s, samples) $(seg) {
        segs |> push_clone(seg)
    }
    return <- segs
}

//! A serving worker transcribes clip after clip on one session: the heap after the fourth transcription of a clip is the
//! heap after the second (within 256 KB), so a transcription frees what it allocates.
def heap_flat_leg(t : T?; m; var s; samples : array<float>; tag : string) {
    for (_i in range(2)) {
        var inscope warm <- session_segs(m, s, samples)
    }
    let h0 = heap_bytes_allocated()
    for (_i in range(2)) {
        var inscope again <- session_segs(m, s, samples)
    }
    let h1 = heap_bytes_allocated()
    t |> success(h1 <= h0 + 262144ul, "{tag}: two more transcriptions leave the heap where it stood ({h0} -> {h1} bytes)")
}

//! a serving worker's session sees clips of every length: a session that read the whole clip reads its first half as a fresh one does
def reused_session_leg(t : T?; m; var s; samples : array<float>) {
    var inscope half <- [for (i in range(length(samples) / 2)); samples[i]]
    var inscope rsegs <- session_segs(m, s, half)
    var inscope sh <- create_session(m)
    var inscope hsegs <- session_segs(m, sh, half)
    t |> success(length(rsegs) == 1 && length(hsegs) == 1, "one segment a leg")
    to_log(LOG_INFO, "half clip, reused session: '{rsegs[0].text}'\nhalf clip, fresh session: '{hsegs[0].text}'\n")
    t |> success(length(hsegs[0].tokens) > 8, "the half clip is a transcript of its own ({length(hsegs[0].tokens)} tokens)")
    t |> equal(length(rsegs[0].tokens), length(hsegs[0].tokens), "reused-session token count")
    for (a, b in rsegs[0].tokens, hsegs[0].tokens) {
        t |> equal(a, b, "reused-session token")
    }
}

def segs_text(segs : array<TranscribeSegment>) : string {
    return build_string() $(var w : StringBuilderWriter) {
        for (seg in segs) {
            w |> write(seg.text)
        }
    }
}

//! what the v2 oracle's two forms keep: the facade load's lane flag and jfk's tokens, the other lane's flag and tokens
struct PkOracleKept {
    default_q8 : bool
    other_q8 : bool
    tokens : array<int>
    other : array<int>
    legs : int
}

// Parakeet-TDT v2 greedy on jfk against parakeet-cli, on the facade's `image` form: the tokens kept for the other lane's
// compare, then the reused session, the flat heap and the decode step's one-lane form.
[cold_path]
def test_parakeet_oracle(t : T?; m : AsrModel; var k : PkOracleKept) {
    return if (!jit_only(t))
    let samples <- load_audio_16k_mono(asr_clip("jfk.wav"))
    if (empty(samples)) {
        t |> skip("jfk.wav not present")
        return
    }
    // through the uniform ASR surface: the sniff routes a small-vocab ggml bin to parakeet
    let c <- caps(m)
    t |> equal(length(c.languages), 1, "English-only")
    t |> success(c.timestamps == AsrTimestamps.segment, "segment timestamps")
    t |> equal(m.pk.n_vocab, 1024l, "vocab")
    t |> equal(m.pk.n_layer, 24l, "layers")
    t |> equal(m.pk.durations[4], 4, "tdt durations")
    with_dasllama_jobque_() {
        var inscope s <- create_session(m)
        var inscope segs <- session_segs(m, s, samples)
        // parakeet-cli -ng -ps (CPU, f32 self-convert): 33 tokens, these exact ends
        t |> equal(length(segs), 1)
        to_log(LOG_INFO, "parakeet-v2 jfk eyeball: '{segs[0].text}'\n")
        t |> equal(length(segs[0].tokens), 33, "token count")
        t |> equal(segs[0].tokens[0], 123, "tok[0] '_And' (leading SentencePiece space marker)")
        t |> equal(segs[0].tokens[1], 96, "tok[1] '_so' (leading SentencePiece space marker)")
        t |> equal(segs[0].tokens[31], 353, "tok[31] 'ry'")
        t |> equal(segs[0].tokens[32], 841, "tok[32] '.'")
        t |> success(segs[0].text == "And so, my fellow Americans, ask not what your country can do for you, ask what you can do for your country.", "text")
        t |> equal(s.detected_lang, "en")

        reused_session_leg(t, m, s, samples)
        heap_flat_leg(t, m, s, samples, "parakeet")

        t |> equal(m.pk.q8, parakeet_serves_q8(), "the default load takes the lane parakeet_serves_q8 picks unpinned")
        k.default_q8 = m.pk.q8
        k.tokens := segs[0].tokens
        k.legs++
        // the decode step's one-lane form against its team publish: the same rows, columns and order either way
        let was_single = get_single_thread_()
        set_single_thread_(false)
        let d0 = get_disp_count()
        var inscope s0 <- create_session(m)
        var inscope tsegs <- session_segs(m, s0, samples)
        let d1 = get_disp_count()
        set_single_thread_(true)
        var inscope s1 <- create_session(m)
        var inscope isegs <- session_segs(m, s1, samples)
        let d2 = get_disp_count()
        set_single_thread_(was_single)
        t |> success(d1 > d0, "the team transcription dispatched ({d1 - d0} times)")
        t |> equal(d2, d1, "the one-lane transcription never dispatched")
        t |> success(length(tsegs) == 1 && length(isegs) == 1, "one segment a leg")
        to_log(LOG_INFO, "parakeet-v2 jfk team eyeball: '{tsegs[0].text}'\nparakeet-v2 jfk one-lane eyeball: '{isegs[0].text}'\n")
        t |> equal(length(isegs[0].tokens), length(tsegs[0].tokens), "one-lane token count")
        for (a, b in isegs[0].tokens, tsegs[0].tokens) {
            t |> equal(a, b, "one-lane token")
        }
    }
}

// The other weight lane of the v2 oracle, on the bin's `f32` form: jfk's tokens off the lane the facade load did not take.
[cold_path]
def parakeet_other_lane_leg(t : T?; mf : AsrModel; var k : PkOracleKept) {
    return if (!jit_only(t))
    let samples <- load_audio_16k_mono(asr_clip("jfk.wav"))
    if (empty(samples)) {
        t |> skip("jfk.wav not present")
        return
    }
    with_dasllama_jobque_() {
        k.other_q8 = mf.pk.q8
        var inscope sf <- create_session(mf)
        var inscope fsegs : array<TranscribeSegment>
        transcribe(mf, sf, samples) $(seg) {
            fsegs |> push_clone(seg)
        }
        t |> equal(length(fsegs), 1)
        to_log(LOG_INFO, "parakeet-v2 jfk other-lane eyeball: '{fsegs[0].text}'\n")
        k.other := fsegs[0].tokens
        k.legs++
    }
}

// The v2 oracle's two lanes, after the bin's block: the mint took the lane the facade load did not, and reads jfk to the
// same token ids.
[cold_path]
def parakeet_lanes_held(t : T?; k : PkOracleKept) {
    return if (!jit_only(t))
    if (!stat(asr_clip("jfk.wav")).is_valid) {
        t |> skip("jfk.wav not present")
        return
    }
    t |> equal(k.legs, 2, "the v2 oracle kept the facade load's tokens and the other lane's")
    return if (k.legs != 2)
    t |> equal(k.other_q8, !k.default_q8, "the mint takes the other lane")
    t |> equal(length(k.other), length(k.tokens), "q8/fp32 token count")
    for (a, b in k.other, k.tokens) {
        t |> equal(a, b, "q8/fp32 token")
    }
}

// Parakeet-TDT v3 greedy on jfk against parakeet-cli, on the facade's `image` form.
[cold_path]
def test_parakeet_v3_oracle(t : T?; m : AsrModel) {
    return if (!jit_only(t))
    let samples <- load_audio_16k_mono(asr_clip("jfk.wav"))
    if (empty(samples)) {
        t |> skip("jfk.wav not present")
        return
    }
    // the 8192-piece SPM vocab routes to parakeet AND flips the multilingual caps
    let c <- caps(m)
    t |> equal(length(c.languages), 25, "25 languages")
    t |> equal(m.pk.n_vocab, 8192l, "vocab")
    t |> equal(m.pk.q8, parakeet_serves_q8(), "the default load takes the lane parakeet_serves_q8 picks unpinned")
    with_dasllama_jobque_() {
        var inscope s <- create_session(m, "de")   // any of the 25; parakeet has no conditioning
        var inscope segs <- session_segs(m, s, samples)
        //! parakeet-cli -ng -ps (CPU, f32 self-convert) gives these 38 tokens; either lane stays id-exact on jfk
        t |> equal(length(segs), 1)
        to_log(LOG_INFO, "parakeet-v3 jfk eyeball: '{segs[0].text}'\n")
        t |> equal(length(segs[0].tokens), 38, "token count")
        t |> equal(segs[0].tokens[0], 1976, "tok[0] '_And' (leading SentencePiece space marker)")
        t |> equal(segs[0].tokens[1], 547, "tok[1] '_so' (leading SentencePiece space marker)")
        t |> equal(segs[0].tokens[36], 7893, "tok[36] 'y'")
        t |> equal(segs[0].tokens[37], 7883, "tok[37] '.'")
        t |> success(segs[0].text == "And so, my fellow Americans, ask not what your country can do for you, ask what you can do for your country.", "text")
        t |> equal(s.detected_lang, "", "v3 does not report a language")
    }
}

//! what the q8_0 file's compare keeps off the f32 bin: jfk's text on its exact lane
struct PkBinText {
    f32 : string
}

//! jfk read through a parakeet bin's form `m` on an English session
def private parakeet_text(m : AsrModel; samples : array<float>) : string {
    var text = ""
    with_dasllama_jobque_() {
        var inscope s <- create_session(m, "en")
        text = transcribe(m, s, samples)
    }
    return text
}

// The f32 bin's side of the q8_0 file's compare, on the v3 f32 bin's `f32` form.
[cold_path]
def parakeet_f32_text_leg(tt : T?; m : AsrModel; var k : PkBinText) {
    return if (!jit_only(tt))
    let samples <- load_audio_16k_mono(asr_clip("jfk.wav"))
    if (empty(samples)) {
        tt |> skip("jfk.wav not present")
        return
    }
    k.f32 = parakeet_text(m, samples)
}

// A parakeet q8_0 bin loads through the reader the whisper bins share, and reads what the f32 bin of the same model does -
// on the q8_0 bin's `f32` form, against the text the f32 bin's `f32` form kept.
[cold_path]
def test_parakeet_q8_0_file(tt : T?; m8 : AsrModel; k : PkBinText) {
    return if (!jit_only(tt))
    let samples <- load_audio_16k_mono(asr_clip("jfk.wav"))
    if (empty(samples)) {
        tt |> skip("jfk.wav not present")
        return
    }
    if (empty(k.f32)) {
        tt |> skip("the f32 bin's f32 form kept no text - its block did not run")
        return
    }
    let from_f32 = k.f32
    let from_q8 = parakeet_text(m8, samples)
    to_log(LOG_INFO, "parakeet v3 f32 bin: '{from_f32}'\n")
    to_log(LOG_INFO, "parakeet v3 q8_0 bin: '{from_q8}'\n")
    tt |> success(!empty(from_q8), "the q8_0 bin transcribes")
    tt |> equal(from_q8, from_f32, "and reads what the f32 bin reads")
}

//! the context the ASR facade serves an LLM decoder at: a decoder's block in the audio area cuts its file's context to it
let ASR_DEC_CTX = 8192l

//! A cell of the Canary pair: `cell` on the ASR model `canary_asr_model` builds from the decoder `dec` its block loaded
//! and the encoder `enc` a form of the encoder's block holds, each handed back before its block drops it, a panic
//! included; as the subtest `name`.
def canary_pair_cell(t : T?; name : string; var dec : Model; var enc : CanaryEncoder; cell : function<(t : T?; m : AsrModel) : void>) {
    unsafe {
        t |> run(name) @capture(& dec, & enc)(t : T ?) {
            var inscope m <- canary_asr_model(dec, enc, "the Canary-Qwen decoder")
            with_put_back($ {
                dec <- m.dec
                enc <- m.cenc
            }) {
                invoke(cell, t, m)
            }
        }
    }
}

// Transcribe one corpus clip through the uniform ASR surface (fresh session) and gate its ids
// against the frozen NeMo SALM greedy reference. Skips if the clip is absent.
//! the widest gap a forced walk may read between its argmax logit and the oracle token's: a greedy near-tie the two
//! implementations' rounding flips (`PERF_LEDGER.md`, the Canary-Qwen oracle row)
let CN_TIE_BAR = 0.25

//! the oracle's ids fed to the decoder after the prompt, `lead` ids ahead of the step that reads them: at each step our argmax
//! and the gap from its logit down to the oracle token's (0 where they are one id)
struct private CnForced {
    picks : array<int>
    gap : array<float>
}

def private canary_forced(m : AsrModel; samples : array<float>; expected : array<int>; lead : int) : CnForced {
    var r : CnForced
    r.picks |> reserve(length(expected))
    r.gap |> reserve(length(expected))
    var inscope s <- create_session(m, "en")
    canary_prefill(m.dec, m.cenc, s.cn, samples)
    var inscope one <- [0l]
    for (i in range(length(expected))) {
        let best = int(argmax_of(s.cn.qctx.logits))
        r.picks |> push(best)
        r.gap |> push(s.cn.qctx.logits[best] - s.cn.qctx.logits[expected[i]])
        if (i + lead + 1 < length(expected)) {
            one[0] = int64(expected[i + lead])
            eval_(m.dec, s.cn.qctx, one)
        }
    }
    return <- r
}

//! One clip against the frozen oracle on one encoder route, an external-reference compare in the forced-feed form: every
//! oracle token is our argmax or within `CN_TIE_BAR` of it; the control feeds the oracle's ids one step early, every step
//! whose input moved reading the oracle token outside the bar; then the greedy ids are the oracle's up to the step where
//! the forced walk's pick leaves the oracle's - all of them where it never does
def private check_canary_route(t : T?; m : AsrModel; wav, route : string; samples : array<float>; expected : array<int>) {
    let name = "{base_name(wav)} {route}"
    var inscope fw <- canary_forced(m, samples, expected, 0)
    var worst = 0.0
    var worst_at = -1
    var agree = length(expected)
    for (i in range(length(expected))) {
        if (!(fw.gap[i] <= worst)) {   //! a NaN gap is the widest, so a NaN logit reds the bar
            worst = fw.gap[i]
            worst_at = i
        }
        if (fw.picks[i] != expected[i] && agree == length(expected)) {
            agree = i
        }
    }
    var wide_text = ""
    if (worst_at >= 0) {
        var inscope ours_at <- [int64(fw.picks[worst_at])]
        var inscope oracle_at <- [int64(expected[worst_at])]
        wide_text = ", our '{eyeball_text(m.dec, ours_at)}' against the oracle's '{eyeball_text(m.dec, oracle_at)}'"
    }
    t |> success(worst <= CN_TIE_BAR, "{name}: every oracle token is our argmax or within {CN_TIE_BAR} of it (widest {worst} at step {worst_at}{wide_text})")
    var inscope ctl <- canary_forced(m, samples, expected, 1)
    var ctl_in = 0
    var ctl_least = 1e30
    for (i in range(1, length(expected) - 1)) {
        ctl_in += !(ctl.gap[i] > CN_TIE_BAR) ? 1 : 0   //! a NaN gap counts inside, so a NaN logit reds the control too
        ctl_least = min(ctl_least, ctl.gap[i])
    }
    let ctl_steps = length(expected) - 2
    t |> success(ctl_steps > 0 && ctl_in == 0, "{name} control: fed one id early, the oracle token sits outside {CN_TIE_BAR} at every one of {ctl_steps} steps ({ctl_in} inside, nearest {ctl_least})")
    var inscope s <- create_session(m, "en")
    var inscope segs <- session_segs(m, s, samples)
    t |> equal(length(segs), 1, "{name}: one segment")
    var inscope ours <- [for (id in segs[0].tokens); int64(id)]
    var inscope oracle <- [for (id in expected); int64(id)]
    to_log(LOG_INFO, "canary {name}: the forced walk leaves the oracle's pick at step {agree} of {length(expected)} (widest gap {worst} at step {worst_at}{wide_text}), the early-fed control's nearest gap {ctl_least} over {ctl_steps} steps\n")
    to_log(LOG_INFO, "canary {name} oracle text: {eyeball_text(m.dec, oracle)}\n")
    to_log(LOG_INFO, "canary {name} greedy text: {eyeball_text(m.dec, ours)}\n")
    //! up to `agree` the oracle's ids are the forced walk's own picks, and greedy decode computes the walk's logits bit for bit:
    //! `canary_transcribe` runs the same `canary_prefill` and then one-token `eval_` steps on the same session kind
    t |> success(length(segs[0].tokens) >= agree, "{name}: the greedy ids reach step {agree}, where the forced walk's pick leaves the oracle's")
    for (i in range(min(agree, length(segs[0].tokens)))) {
        t |> equal(expected[i], segs[0].tokens[i], "{name}: token id {i}")
    }
    if (agree == length(expected)) {
        t |> equal(length(expected), length(segs[0].tokens), "{name}: token count")
    }
    t |> equal("en", s.detected_lang, "{name}: english")
}

//! the clip's frozen ids on one route of `m`'s encoder: on the device (`device`, canary's tower knob on, every forced walk and the
//! greedy decode counted as a device encode) or on the CPU chain (every tower off, no device encode - the count's control)
def private check_canary_clip(t : T?; m : AsrModel; wav, route : string; device : bool; expected : array<int>) {
    let samples <- load_audio_16k_mono(wav)
    if (empty(samples)) {
        t |> skip("{base_name(wav)} not present")
        return
    }
    with_gpu_knobs() {
        if (device) {
            set_serving_gpu_tower(true, TowerFamily.canary)
        } else {
            set_every_gpu_tower(false)
        }
        let e0 = gpu_tower_encodes(TowerFamily.canary)
        check_canary_route(t, m, wav, route, samples, expected)
        let encodes = gpu_tower_encodes(TowerFamily.canary) - e0
        if (device) {
            t |> success(encodes >= 3l, "{base_name(wav)} {route}: the tower encoded both forced walks and the greedy decode on the device ({encodes} encodes)")
        } else {
            t |> equal(encodes, 0l, "{base_name(wav)} {route}: the tower encoded nothing with every tower off (the encode count's control)")
        }
    }
}

//! the oracle's three clips and their frozen NeMo SALM greedy ids (`harness/canary_qwen_expected.tsv`; trailing 151645 = text EOS)
def private canary_oracle_clips() : array<tuple<wav : string; ids : array<int>>> {
    return <- [
        (wav = asr_clip("jfk.wav"), ids = [3036, 773, 847, 12357, 8877, 2548, 537, 1128, 697, 3146, 646, 653, 369, 498, 2548,
                                           1128, 498, 646, 653, 369, 697, 3146, 151645]),
        (wav = asr_clip("librispeech/09_1089-134686-0008.wav"), ids = [785, 26915, 304, 892, 806, 83909, 413, 55707, 3304, 5086,
                                                                         572, 264, 9255, 11, 78908, 6540, 315, 5561, 151645]),
        (wav = asr_clip("librispeech/05_1089-134686-0004.wav"), ids = [2833, 5779, 29160, 451, 11896, 374, 8580, 389, 498, 13,
                                                                         7684, 3729, 9972, 151645])
    ]
}

// Canary-Qwen 2.5B against the frozen NeMo SALM ids on the CPU chain: the pair of the decoder GGUF (a LoRA-merged
// Qwen3-1.7B) its block loaded and the "CNRY" FastConformer bin's `f32` form, the encoder's CPU image on its exact lane.
[cold_path]
def test_canary_qwen_oracle(t : T?; m : AsrModel) {
    let prior_kv = get_asr_kv()
    set_asr_kv(KVDtype.f32)   // the frozen NeMo ids are the f32 parity rail
    let c <- caps(m)
    t |> equal(length(c.languages), 1, "English-only")
    t |> success(!c.translate && c.timestamps == AsrTimestamps.none && !c.streaming, "no translate / timestamps / streaming")
    t |> equal(m.cenc.n_layer, 32l, "FastConformer-32")
    t |> equal(m.cenc.proj_dim, 2048l, "projects to the Qwen3 hidden width")
    var inscope clips <- canary_oracle_clips()
    with_dasllama_jobque_() {
        for (clip in clips) {
            check_canary_clip(t, m, clip.wav, "cpu chain", false, clip.ids)
        }
        var inscope jfk <- load_audio_16k_mono(asr_clip("jfk.wav"))
        if (!empty(jfk)) {
            var inscope hs <- create_session(m, "en")
            heap_flat_leg(t, m, hs, jfk, "canary")
        }
    }
    set_asr_kv(prior_kv)
}

// The oracle's device walk: the pair whose encoder is the form minted on the lane the home's tower serves canary by
// default, in that tower's flavor.
[cold_path]
def canary_oracle_device_walk(t : T?; m : AsrModel) {
    let prior_kv = get_asr_kv()
    set_asr_kv(KVDtype.f32)
    let q8 = m.cenc.q8
    let dev = m.cenc.dev_backend
    var inscope clips <- canary_oracle_clips()
    with_dasllama_jobque_() {
        for (clip in clips) {
            check_canary_clip(t, m, clip.wav, "{dev} {q8 ? "q8" : "exact"} lane", true, clip.ids)
        }
    }
    set_asr_kv(prior_kv)
}

// Gemma-4 E2B audio: greedy no-think transcription is token-for-token with llama-mtmd-cli through
// the confident prefix. The gemma4a Conformer encoder soft tokens match mtmd's audio embeddings at
// rel ~0.0027 (float-reduction-order noise); the full transcription diverges only at the Gemma-4
// q8 decoder's OWN ~0.6-logit near-tie (the upstream raw logits at that step: three tokens within
// 0.6), the qwen3moe/gemma-26B situation. So the fixture asserts the confident head, not the tail.
// Oracle: llama-mtmd-cli -m E2B-Q8 --mmproj bf16 --audio clip --temp 0 --jinja (no-think turn).
// On the E2B mmproj's `image_asr` form: the ASR facade's two-file load of the mmproj with its Q8 decoder.
[cold_path]
def test_gemma4a_audio_oracle(t : T?; m : AsrModel) {
    return if (!jit_only(t))
    t |> equal(m.g4a.n_layer, 12l, "gemma4a Conformer, 12 blocks")
    t |> equal(m.g4a.d_model, 1024l, "audio d_model 1024")
    t |> equal(m.g4a.n_head, 8l, "8 heads")
    t |> equal(m.g4a.proj_dim, m.dec.config.dim, "projects into the Gemma-4 embedding width")
    let wav = asr_clip("gemma4a_test2.wav")
    let samples <- load_audio_16k_mono(wav)
    if (empty(samples)) {
        t |> skip("gemma4a_test2.wav not present")
        return
    }
    let prior_kv = get_asr_kv()
    set_asr_kv(KVDtype.f32)   // the frozen mtmd-prefix ids are the f32 parity rail; below every skip
    with_dasllama_jobque_() {
        var inscope s <- create_session(m, "en")
        var inscope segs <- session_segs(m, s, samples)
        heap_flat_leg(t, m, s, samples, "gemma4a")
        t |> equal(length(segs), 1, "one segment")
        to_log(LOG_INFO, "gemma4a eyeball: '{segs[0].text}'\n")
        // frozen llama-mtmd-cli no-think greedy ids (first 40 tokens — the confident prefix
        // before the model's own ~0.6-logit near-tie at step 48). "The New York Times from
        // July 21st, 1969. This isn't just newsprint and ink. This is the moment when..."
        let expected = [818, 1799, 3773, 11922, 699, 5505, 236743, 236778, 236770, 540,
                        236764, 236743, 236770, 236819, 236825, 236819, 236761, 1174, 5889, 236789,
                        236745, 1164, 4668, 1995, 532, 19764, 236761, 1174, 563, 506,
                        3479, 1056, 27069, 236789, 236751, 24625, 9156, 5452, 3645, 236772]
        t |> success(long_length(segs[0].tokens) >= long_length(expected),
            "emitted at least {long_length(expected)} tokens ({long_length(segs[0].tokens)})")
        var mismatches = 0
        for (i in range(min(length(expected), length(segs[0].tokens)))) {
            if (segs[0].tokens[i] != expected[i]) {
                mismatches++
            }
        }
        t |> success(mismatches == 0, "confident prefix token-for-token ({mismatches} mismatches)")
        t |> equal(s.detected_lang, "en", "english")
    }
    set_asr_kv(prior_kv)
}

// Gemma-4 E4B audio through the facade, on the E4B mmproj's `image_asrdev` form - the facade's two-file load with the
// serving tower's knob on: the embedder widens 1536 to 2560, soft tokens off the serving GPU tower, the transcript jfk's
// words.
[cold_path]
def test_gemma4a_e4b_audio_facade(t : T?; m : AsrModel) {
    return if (!jit_only(t))
    let samples <- load_audio_16k_mono(asr_clip("jfk.wav"))
    if (empty(samples)) {
        t |> skip("jfk.wav not present")
        return
    }
    with_gpu_knobs() {
        set_serving_gpu_tower(true, TowerFamily.gemma4a)   // the tower knob the load's flavor pick and the encode read
        t |> equal(gemma4a_mid_dim(m.g4a), 1536l, "the encoder's own width")
        t |> equal(m.g4a.proj_dim, 2560l, "the embedder widens to the decoder's width")
        let device_up = serving_tower_device_up(TowerFamily.gemma4a)
        let e0 = gpu_tower_encodes(TowerFamily.gemma4a)
        var text = ""
        with_dasllama_jobque_() {
            var inscope s <- create_session(m, "en")
            var inscope segs <- session_segs(m, s, samples)
            for (sg in segs) {
                text += sg.text
            }
            if (device_up) {
                t |> success(gpu_tower_encodes(TowerFamily.gemma4a) > e0, "the soft tokens came off the {serving_tower_dev(TowerFamily.gemma4a)} tower")
                knob_off_unrouted(t, "gemma4a E4B jfk", TowerFamily.gemma4a) {
                    var inscope so <- create_session(m, "en")
                    var inscope off_segs <- session_segs(m, so, samples)
                    to_log(LOG_INFO, "gemma4a E4B jfk with the tower knob off: {length(off_segs)} segments\n")
                }
            }
        }
        jfk_words_floor(t, "gemma4a E4B jfk", text)
    }
}

// The gemma4a log-mel of jfk computed twice on one reused state is bit-identical, on a form of the E2B mmproj minted in
// memory.
[cold_path]
def test_mel_state_reuse_deterministic(t : T?; enc : Gemma4aEncoder) {
    return if (!jit_only(t))
    let wav = asr_clip("jfk.wav")
    let samples <- load_audio_16k_mono(wav)
    if (empty(samples)) {
        t |> skip("jfk.wav not present in the asr corpus")
        return
    }
    var st = Gemma4aState()
    gemma4a_log_mel(enc, samples, 0l, long_length(samples), st)
    var mel1 := st.mel
    gemma4a_log_mel(enc, samples, 0l, long_length(samples), st)
    var md = 0.0
    for (a, b in mel1, st.mel) {
        md = max(md, nan_wide_diff(a, b))
    }
    t |> success(md == 0.0, "gemma4a mel maxdiff across state reuse is 0 (got {md})")
    delete mel1
    delete st
}

// The canary half of the mel state-reuse cell, on the encoder bin's `f32` form.
[cold_path]
def canary_mel_state_reuse(t : T?; cenc : CanaryEncoder) {
    return if (!jit_only(t))
    let wav = asr_clip("jfk.wav")
    let samples <- load_audio_16k_mono(wav)
    if (empty(samples)) {
        t |> skip("jfk.wav not present in the asr corpus")
        return
    }
    var cst = CanaryState()
    canary_log_mel(cenc, samples, cst)
    var cmel1 := cst.mel
    canary_log_mel(cenc, samples, cst)
    var cmd = 0.0
    for (a, b in cmel1, cst.mel) {
        cmd = max(cmd, nan_wide_diff(a, b))
    }
    t |> success(cmd == 0.0, "canary mel maxdiff across state reuse is 0 (got {cmd})")
    delete cmel1
    delete cst
}

//! Whether the ASR facade's decoder `file` may load, as a subtest of its own: the JIT, and the file's gate
//! (`model_available`: present, its category named, its tier open) - a miss its loud skip.
def asr_decoder_available(t : T?; file : string) : bool {
    var ok = false
    unsafe {
        t |> run("{file}: the decoder of the ASR facade's pair") @capture(& ok)(t : T ?) {
            ok = jit_model_available(t, path_join(models_dir(), file))
        }
    }
    return ok
}

// The qwen3a tower on the CPU f32 rail against llama-mtmd's embeddings, on the Qwen3-ASR mmproj's `image_f32` form: its
// exact lane's CPU image, every GPU tower off for the encode - the documented ~1e-4 reorder bars are CPU claims.
[cold_path]
def test_qwen3a_tower(t : T?; tw : Qwen3aTower) {
    return if (!jit_only(t))
    let samples <- load_audio_16k_mono(asr_clip("jfk.wav"))
    if (empty(samples)) {
        t |> skip("jfk.wav not present")
        return
    }
    with_gpu_knobs() {
        set_every_gpu_tower(false)
        t |> equal(tw.core.d_model, 896l, "d_model")
        t |> equal(tw.core.n_layer, 18l, "layers")
        t |> equal(tw.core.n_head, 14l, "heads")
        t |> equal(tw.n_ch, 480l, "conv channels")
        t |> equal(tw.proj_dim, 1024l, "proj_dim")
        t |> success(tw.core.has_k_bias, "k bias")
        let mel <- log_mel_qwen3a(samples, tw.core.n_mel)
        t |> equal(mel.n_len, 1101l, "effective mel frames (11 s)")
        var enc <- make_encoder_state(tw.core)
        var cs = Qwen3aConvState()
        with_dasllama_jobque_() {
            let win <- qwen3a_mel_window(mel, 0l)
            t |> equal(win.n_len, 800l, "window 0 padded frames")
            qwen3a_encode(tw, enc, cs, win)
        }
        // llama-mtmd-cli MTMD_DEBUG_EMBEDDINGS (CPU, bf16 mmproj) on jfk window 0: token 0
        // opens 0.043467 -0.015482 -0.032934 0.023813; our fp32 re-orders sit ~1e-4 off.
        t |> success(abs(enc.out[0] - 0.043467) < 5e-3, "tok0[0]")
        t |> success(abs(enc.out[1] + 0.015482) < 5e-3, "tok0[1]")
        t |> success(abs(enc.out[2] + 0.032934) < 5e-3, "tok0[2]")
        t |> success(abs(enc.out[3] - 0.023813) < 5e-3, "tok0[3]")
        delete enc
    }
}
