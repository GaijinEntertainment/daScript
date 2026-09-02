options gen2
options _dasllama_internal = true

module _tts_parity shared public

require dasllama/dasllama_lint public
require dastest/testing_boost public
require daslib/fio public
require strings public
require math
require dasllama/dasllama_styletts2
require dasllama/dasllama_tts_types
require dasllama/dasllama_math   // setup_dasllama_jobque_
require daslib/jobque_boost
require _tts_oracle
require _model_tier

// The parity rail both StyleTTS2 families run against their onnxruntime / PyTorch oracle
// dumps (harness/kitten_oracle.py, harness/kokoro_oracle.py): the token ids against the
// reference driver on every case, identical durations on every case, and on the bring-up set
// every stage through the decoder output within BAR of the oracle's peak, the sine source
// within BAR fed the oracle's F0, the generator within BAR fed the oracle's source signal and
// decoder output. The end-to-end waveform difference is logged, not gated: the source
// integrates F0 into a phase of 1e5 radians, so the GEMM-order rounding the predictor's F0
// carries becomes radians of harmonic phase.

let BAR = 1e-4

//! One family's rig: where its model and dumps live, and how it turns the fixture's phoneme
//! string into token ids.
struct TtsParityRig {
    family : string
    gguf : string
    oracle_dir : string
    ids : function<(ps : string) : array<int>>
}

def same_ints(a, b : array<int>) : bool {
    return false if (length(a) != length(b))
    for (x, y in a, b) {
        return false if (x != y)
    }
    return true
}

// one stage against its dump: the max-abs difference relative to the oracle's peak (or absolute
// when `absolute`), logged on green as well as red; a stage the dump lacks is skipped
def stage_diff(t : T?; tag, name : string; ours : array<float>; dump : table<string; TtsDumpTensor>; absolute : bool = false) {
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

let PHASE_BAR = 1e-2   // radians, where the magnitude is above a thousandth of its peak

// the source spectrum against the oracle's: magnitude rows relative to the peak, phase rows
// wrapped and only where the magnitude carries signal
def spectrum_diff(t : T?; tag : string; ours, ref : array<float>; bins, frames : int64) {
    let n = bins * frames
    if (long_length(ours) != 2l * n || long_length(ref) != 2l * n) {
        t |> failure("{tag}: {length(ours)} elements against {length(ref)}, want {2l * n}")
        return
    }
    var peak = 0.0
    for (i in range64(n)) {
        peak = max(peak, abs(ref[i]))
    }
    var mag_d = 0.0
    var ph_d = 0.0
    var gated = 0l
    for (i in range64(n)) {
        mag_d = max(mag_d, abs(ours[i] - ref[i]))
        if (ref[i] > 1e-3 * peak) {
            gated++
            var d = abs(ours[n + i] - ref[n + i])
            if (d > PI) {
                d = 2.0 * PI - d
            }
            ph_d = max(ph_d, d)
        }
    }
    to_log(LOG_INFO, "{tag}: magnitude maxdiff {mag_d} (peak {peak}), phase maxdiff {ph_d} rad over {gated} of {n} bins with signal\n")
    t |> success(mag_d <= BAR * peak, "{tag}: magnitude maxdiff {mag_d} <= {BAR * peak}")
    t |> success(ph_d <= PHASE_BAR, "{tag}: phase maxdiff {ph_d} rad <= {PHASE_BAR} where the magnitude carries signal")
}

def dump_f(dump : table<string; TtsDumpTensor>; name : string) : array<float> {
    var inscope out : array<float>
    dump |> get(name) $(tensor) {
        out := tensor.f
    }
    return <- out
}

def dump_i(dump : table<string; TtsDumpTensor>; name : string) : array<int> {
    var inscope out : array<int>
    dump |> get(name) $(tensor) {
        out := tensor.i
    }
    return <- out
}

def case_noise(dump : table<string; TtsDumpTensor>) : TtsNoise {
    return <- TtsNoise(uniform <- dump_f(dump, "noise_uniform"), normal <- dump_f(dump, "noise_normal"), captured = true)
}

// the generator fed the oracle's own F0 curve, source signal and decoder output: block
// correctness at the seams, with the phase drift the predictor's rounding noise induces taken out
def private seam_check(t : T?; m : StyleTts2Model; tag : string; dump : table<string; TtsDumpTensor>) {
    var inscope f0 <- dump_f(dump, "f0")
    var inscope gen_in <- dump_f(dump, "gen_in")
    var inscope style <- dump_f(dump, "style")
    let hs = m.style_dim / 2l
    var inscope s_ref : array<float>
    s_ref |> resize(hs)
    for (i in range64(hs)) {
        s_ref[i] = style[i]
    }
    let f2 = long_length(f0)
    let cw = long_length(gen_in) / f2
    var inscope noise <- case_noise(dump)
    var tr = new StyleTts2Trace()
    var inscope sc = St2Scratch()
    var inscope har : array<float>
    let n = styletts2_source(m.decoder, m.source, f0, f2, noise, har, sc)
    stage_diff(t, "{tag} SOURCE", "har", har, dump, true)
    var inscope har_ref <- dump_f(dump, "har")
    var inscope spec : array<float>
    let frames = styletts2_har_spectrum(m.decoder, har_ref, n, tr, spec, sc)
    var inscope spec_ref <- dump_f(dump, "har_spec")
    if (!empty(spec_ref)) {
        // the reference's FFT and a direct sum agree on the magnitude everywhere and on the phase
        // wherever the magnitude is meaningful; below that the phase is rounding noise the network
        // nonetheless consumes, so the generator is fed the oracle's spectrum from here on
        spectrum_diff(t, "{tag} SPECTRUM", spec, spec_ref, m.decoder.bins, frames)
        spec := spec_ref
    }
    var inscope wave : array<float>
    styletts2_generator(m.decoder, gen_in, cw, f2, s_ref, spec, frames, tr, wave, sc)
    stage_diff(t, "{tag} SEAM", "stft_re", tr.stft_re, dump)
    stage_diff(t, "{tag} SEAM", "ups0", tr.ups0, dump)
    stage_diff(t, "{tag} SEAM", "src0", tr.src0, dump)
    stage_diff(t, "{tag} SEAM", "nres0", tr.nres0, dump)
    stage_diff(t, "{tag} SEAM", "rb0", tr.rb0, dump)
    stage_diff(t, "{tag} SEAM", "rb1", tr.rb1, dump)
    stage_diff(t, "{tag} SEAM", "rb2", tr.rb2, dump)
    stage_diff(t, "{tag} SEAM", "ups1", tr.ups1, dump)
    stage_diff(t, "{tag} SEAM", "conv_post", tr.conv_post, dump)
    stage_diff(t, "{tag} SEAM", "waveform", wave, dump, true)
    unsafe {
        delete tr
    }
}

def private stage_checks(t : T?; tag : string; tr : StyleTts2Trace?; dump : table<string; TtsDumpTensor>) {
    stage_diff(t, tag, "bert", tr.bert, dump)
    stage_diff(t, tag, "text_enc", tr.text_enc, dump)
    stage_diff(t, tag, "dur_raw", tr.dur_raw, dump)
    stage_diff(t, tag, "f0", tr.f0, dump)
    stage_diff(t, tag, "n", tr.n, dump)
    stage_diff(t, tag, "asr_res", tr.asr_res, dump)
    stage_diff(t, tag, "dec_in", tr.dec_in, dump)
    stage_diff(t, tag, "gen_in", tr.gen_in, dump)
}

//! Run one family's rig, or skip loudly when its model or dumps are not stocked.
def tts_parity(t : T?; rig : TtsParityRig) {
    if (!stat(rig.gguf).is_valid) {
        t |> skip("{base_name(rig.gguf)} not present")
        return
    }
    var inscope mf <- load_tts_oracle_manifest(rig.oracle_dir)
    if (empty(mf.cases)) {
        t |> skip("{rig.oracle_dir} not stocked")
        return
    }
    with_job_que() {
        setup_dasllama_jobque_()
        var inscope m <- load_styletts2(rig.gguf)
        t |> equal(m.family, rig.family)
        t |> equal(m.sample_rate, 24000)
        var ids_ok = 0
        var same_length = 0
        var worst = 0.0
        var worst_case = ""
        for (c in mf.cases) {
            let tag = "{rig.family} {c.id} {c.voice}"
            var inscope dump <- load_tts_dump(path_join(rig.oracle_dir, c.file))
            var inscope ids <- dump_i(dump, "input_ids")
            var inscope ours <- invoke(rig.ids, c.ps_espeak)
            if (same_ints(ours, ids)) {
                ids_ok++
            } else {
                to_log(LOG_INFO, "{tag}: token ids differ - ours {ours}\n   oracle {ids}\n")
            }
            var inscope noise <- case_noise(dump)
            var tr = new StyleTts2Trace()
            var tm = TtsTimings()
            var inscope sc = St2Scratch()
            styletts2_synthesize(m, ids, dump_f(dump, "style"), c.speed, noise, 0ul, tr, tm, sc)
            let wave & = sc.wave
            dump |> get("waveform") $(ref) {
                if (length(wave) != length(ref.f)) {
                    t |> failure("{tag}: {length(wave)} samples, oracle has {length(ref.f)} (frames {tr.frames} vs {c.duration_sum})")
                } else {
                    same_length++
                    let d = max_abs_diff(wave, ref.f)
                    to_log(LOG_INFO, "{tag}: waveform maxdiff {d} over {length(ref.f)} samples (informational); {timings_line(tm, m.sample_rate)}\n")
                    if (d > worst) {
                        worst = d
                        worst_case = "{c.id} {c.voice}"
                    }
                }
            }
            if (c.stages) {
                seam_check(t, m, tag, dump)
                stage_checks(t, tag, tr, dump)
            }
            unsafe {
                delete tr
            }
        }
        t |> equal(ids_ok, length(mf.cases), "cases whose token ids match the reference driver")
        t |> equal(same_length, length(mf.cases), "cases whose durations, hence sample counts, match the oracle")
        to_log(LOG_INFO, "{rig.family}: worst end-to-end waveform maxdiff {worst} at {worst_case} over {length(mf.cases)} cases (informational)\n")
    }
}
