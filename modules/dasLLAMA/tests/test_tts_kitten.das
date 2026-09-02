options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_kitten
require dasllama/dasllama_tts_types
require dasllama/dasllama_math   // setup_dasllama_jobque_
require daslib/jobque_boost
require daslib/fio
require math
require _tts_corpus
require _tts_oracle
require _model_tier

// The KittenTTS family. Model-free: the symbol map (the front end's inventory into the
// espeak-style IPA the models consume) over the 200-sentence corpus, and the reference driver's
// token rule. Model-gated (`kitten-<size>.gguf` + `tts_oracle/kitten_<size>/` under models_dir(),
// both minted by performance/build_tts_data.das through harness/convert_kitten.py and
// harness/kitten_oracle.py): the token ids against the reference driver, and the parity rail
// against onnxruntime with the captured noise fed. The rail gates what float32 can prove: every
// stage up to the decoder output within 1e-4 of the oracle's peak, the sine source within 1e-4
// when fed the oracle's F0, the generator within 1e-4 when fed the oracle's source signal and
// decoder output, and identical durations end to end. The end-to-end waveform difference is
// logged, not gated: the source integrates F0 into a phase of 1e5 radians, so the GEMM-order
// rounding the predictor's F0 carries (a few ulps) becomes radians of harmonic phase - the
// reference's own arithmetic would drift the same way against any other GEMM.

let private BAR = 1e-4

[test]
def test_symbol_map(t : T?) {
    t |> run("the corpus rewrites symbol for symbol") @(t : T?) {
        let rows <- load_tts_corpus()
        var mismatches = 0
        for (r in rows) {
            let got = kitten_phonemes_from_misaki(r.ps_misaki)
            if (got != r.ps_espeak) {
                mismatches++
                t |> failure("{r.id}: got  {got}\n       want {r.ps_espeak}")
            }
        }
        t |> equal(mismatches, 0, "sentences rewritten differently")
    }
    t |> run("the calibrated rules") @(t : T?) {
        t |> equal(kitten_phonemes_from_misaki("kəmpjˈuTəɹ"), "kəmpjˈuːɾɚ")   // nolint:STYLE039
        t |> equal(kitten_phonemes_from_misaki("bˈɜɹd"), "bˈɜːd")   // nolint:STYLE039
        t |> equal(kitten_phonemes_from_misaki("ʧˈOk"), "tʃˈoʊk")   // nolint:STYLE039
        t |> equal(kitten_phonemes_from_misaki("ɹˈɪTᵊn"), "ɹˈɪɾən")   // nolint:STYLE039
    }
    t |> run("the token rule") @(t : T?) {
        // pad, "h", "i", space, ",", space, "w", space, ".", ellipsis, pad - punctuation is its own token
        let ids <- kitten_token_ids("hi, w.")
        t |> equal(length(ids), 11)
        t |> equal(ids[0], 0)
        t |> equal(ids[3], 16, "the space symbol")
        t |> equal(ids[4], 3, "the comma")
        t |> equal(ids[length(ids) - 2], 10, "the ellipsis closes")
        t |> equal(ids[length(ids) - 1], 0)
        t |> equal(kitten_style_row("abc", 400), 3)
        t |> equal(kitten_style_row("", 400), 0)
    }
}

def private oracle_dir(size : string) : string {
    return path_join(models_dir(), "tts_oracle/kitten_{size}")
}

def private gguf_path(size : string) : string {
    return path_join(models_dir(), "kitten-{size}.gguf")
}

// the model and its oracle store, or a loud skip
def private kitten_ready(t : T?; size : string; var mf : TtsOracleManifest) : bool {
    if (!stat(gguf_path(size)).is_valid) {
        t |> skip("kitten-{size}.gguf not present")
        return false
    }
    delete mf
    mf <- load_tts_oracle_manifest(oracle_dir(size))
    if (empty(mf.cases)) {
        t |> skip("tts_oracle/kitten_{size} not stocked")
        return false
    }
    return true
}

def private case_style(dump : table<string; TtsDumpTensor>) : array<float> {
    return clone_to_move((dump?["style"] ?? default<TtsDumpTensor>).f)
}

def private case_ids(dump : table<string; TtsDumpTensor>) : array<int> {
    return clone_to_move((dump?["input_ids"] ?? default<TtsDumpTensor>).i)
}

def private case_noise(dump : table<string; TtsDumpTensor>) : TtsNoise {
    var n = TtsNoise()
    n.uniform := (dump?["noise_uniform"] ?? default<TtsDumpTensor>).f
    n.normal := (dump?["noise_normal"] ?? default<TtsDumpTensor>).f
    return <- n
}

def private same_ints(a, b : array<int>) : bool {
    return false if (length(a) != length(b))
    for (x, y in a, b) {
        return false if (x != y)
    }
    return true
}

// one stage against its dump: the max-abs difference relative to the oracle's peak (or absolute
// when `absolute`), logged on green as well as red
def private stage_diff(t : T?; tag, name : string; ours : array<float>; dump : table<string; TtsDumpTensor>; absolute : bool = false) {
    dump |> get(name) $(tensor) {
        if (length(tensor.f) != length(ours)) {
            t |> failure("{tag} {name}: {length(ours)} elements, oracle has {length(tensor.f)}")
            return
        }
        var peak = 0.0
        for (v in tensor.f) {
            peak = max(peak, abs(v))
        }
        let d = max_abs_diff(ours, tensor.f)
        let bar = absolute ? BAR : BAR * peak
        to_log(LOG_INFO, "{tag} {name}: maxdiff {d} over {length(tensor.f)} (oracle peak {peak})\n")
        t |> success(d <= bar, "{tag} {name}: maxdiff {d} <= {bar}")
    }
}

def private dump_f(dump : table<string; TtsDumpTensor>; name : string) : array<float> {
    var out : array<float>
    dump |> get(name) $(tensor) {
        out := tensor.f
    }
    return <- out
}

// the generator fed the oracle's own F0 curve and decoder output: block correctness at the seam,
// with the phase drift the predictor's rounding noise induces taken out of the picture
def private seam_check(t : T?; m : KittenModel; size : string; c : TtsOracleCase; dump : table<string; TtsDumpTensor>) {
    let f0 <- dump_f(dump, "f0")
    let gen_in <- dump_f(dump, "gen_in")
    let style <- case_style(dump)
    let hs = m.style_dim / 2l
    var s_ref : array<float>
    s_ref |> resize(hs)
    for (i in range64(hs)) {
        s_ref[i] = style[i]
    }
    let f2 = long_length(f0)
    let cw = long_length(gen_in) / f2
    let noise <- case_noise(dump)
    var tr = new KittenTrace()
    var har : array<float>
    let n = kitten_source(m.decoder, m.source, f0, f2, noise, har)
    stage_diff(t, "{size} {c.id} {c.voice} SOURCE", "har", har, dump, true)
    // the generator from the oracle's own source signal: the blocks after the sine source
    let har_ref <- dump_f(dump, "har")
    var spec : array<float>
    let frames = kitten_har_spectrum(m.decoder, har_ref, n, tr, spec)
    var wave : array<float>
    kitten_generator(m.decoder, gen_in, cw, f2, s_ref, spec, frames, tr, wave)
    let tag = "{size} {c.id} {c.voice} SEAM"
    stage_diff(t, tag, "stft_re", tr.stft_re, dump)
    stage_diff(t, tag, "ups0", tr.ups0, dump)
    stage_diff(t, tag, "ups1", tr.ups1, dump)
    stage_diff(t, tag, "conv_post", tr.conv_post, dump)
    stage_diff(t, tag, "waveform", wave, dump, true)
    unsafe {
        delete tr
    }
}

def private oracle_size(t : T?; size : string) {
    var mf = TtsOracleManifest()
    return if (!kitten_ready(t, size, mf))
    with_job_que() {
        setup_dasllama_jobque_()
        let m <- load_kitten_model(gguf_path(size))
        t |> equal(m.size, size)
        t |> equal(m.sample_rate, 24000)
        var ids_ok = 0
        var same_length = 0
        var worst = 0.0
        var worst_case = ""
        for (c in mf.cases) {
            let dump <- load_tts_dump(path_join(oracle_dir(size), c.file))
            let ids <- case_ids(dump)
            let ours <- kitten_token_ids(c.ps_espeak)
            if (same_ints(ours, ids)) {
                ids_ok++
            } else {
                to_log(LOG_INFO, "{size} {c.id}: token ids differ - ours {ours}\n   oracle {ids}\n")
            }
            var noise <- case_noise(dump)
            var tr = new KittenTrace()
            let wave <- kitten_synthesize_ids(m, ids, case_style(dump), c.speed, noise, 0ul, tr)
            dump |> get("waveform") $(ref) {
                if (length(wave) != length(ref.f)) {
                    t |> failure("{size} {c.id} {c.voice}: {length(wave)} samples, oracle has {length(ref.f)} (frames {tr.frames} vs {c.duration_sum})")
                } else {
                    same_length++
                    let d = max_abs_diff(wave, ref.f)
                    to_log(LOG_INFO, "{size} {c.id} {c.voice}: waveform maxdiff {d} over {length(ref.f)} samples (informational)\n")
                    if (d > worst) {
                        worst = d
                        worst_case = "{c.id} {c.voice}"
                    }
                }
            }
            if (c.stages) {
                seam_check(t, m, size, c, dump)
                let tag = "{size} {c.id} {c.voice}"
                stage_diff(t, tag, "bert", tr.bert, dump)
                stage_diff(t, tag, "text_enc", tr.text_enc, dump)
                stage_diff(t, tag, "dur_raw", tr.dur_raw, dump)
                stage_diff(t, tag, "f0", tr.f0, dump)
                stage_diff(t, tag, "n", tr.n, dump)
                stage_diff(t, tag, "asr_res", tr.asr_res, dump)
                stage_diff(t, tag, "dec_in", tr.dec_in, dump)
                stage_diff(t, tag, "gen_in", tr.gen_in, dump)
            }
            unsafe {
                delete tr
            }
        }
        t |> equal(ids_ok, length(mf.cases), "cases whose token ids match the reference driver")
        t |> equal(same_length, length(mf.cases), "cases whose durations, hence sample counts, match the oracle")
        to_log(LOG_INFO, "{size}: worst end-to-end waveform maxdiff {worst} at {worst_case} over {length(mf.cases)} cases (informational)\n")
    }
}

[test]
def test_kitten_oracle(t : T?) {
    t |> run("nano against onnxruntime, captured noise") @(t : T?) {
        oracle_size(t, "nano")
    }
    t |> run("mini against onnxruntime, captured noise") @(t : T?) {
        oracle_size(t, "mini")
    }
}
