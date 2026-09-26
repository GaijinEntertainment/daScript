options gen2
options _dasllama_internal = true

module _tts_parity shared public

require dasllama/dasllama_lint public
require dastest/testing_boost public
require daslib/fio public
require strings public
require math
require dasllama/dasllama_styletts2
require dasllama/dasllama_pocket   // pocket_gpu_stats / pocket_gpu_seats: the served-lane counters read either family's seats
require dasllama/dasllama_tts_types
require dasllama/dasllama_tts_blocks
require dasllama/dasllama_tts   // nolint:STYLE029,LINT019 — the facade synthesis, read only inside the Apple half
require dasllama/dasllama_g2p   // nolint:STYLE029,LINT019 — the facade's packs, read only inside the Apple half
require dasllama/dasllama_postag   // nolint:STYLE029,LINT019 — the facade's packs, read only inside the Apple half
require dasllama/dasllama_math   // setup_dasllama_jobque_
require ?das_metal dasllama/dasllama_metal_tower
require daslib/jobque_boost
require daslib/defer
require daslib/linq   // sequence_equal; reverse inside the Apple half
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

//! The Metal driver's bars: rel-rms against the CPU chain fed the same inputs on the f32 lane (the readings: tests/CLAUDE.md).
let GPU_SEAM_BAR = 5e-3
let GPU_ALBERT_BAR = 5e-5   //! the f32-exact front end reads 3e-6; the f16-staged route read 2e-3, so the bars sit between
let GPU_TEXT_BAR = 1e-5
let GPU_PRED_BAR = 2e-5
let GPU_DECODE_BAR = 5e-2
let GPU_DECODE_Q8_BAR = 1e-1   //! the served lane: the CPU q8 chain quantizes its activations, the tower reads the dequantized weights at f32

//! sqrt(sum (a - b)^2 / sum b^2) over the shorter length
def rel_rms(a, b : array<float>) : float {
    var num = 0.0lf
    var den = 0.0lf
    for (i in range(min(length(a), length(b)))) {
        let d = double(a[i]) - double(b[i])
        num += d * d
        den += double(b[i]) * double(b[i])
    }
    return float(sqrt(num / max(den, 1e-30lf)))
}

def private within_twentieth(n, m : int64) : bool => abs(n - m) * 20l < m

//! The verdict where the tower served nothing: a skip where no Metal device serves, a red where a present device declined. Always false.
def tower_served(t : T?; msg, declines : string) : bool {
    if (find(declines, "device") < 0) {
        t |> failure("{msg}: the tower declined (declines {declines})")
    } else {
        t |> skip("{msg}: no Metal device serves (declines {declines})")
    }
    return false
}

//! The facade minted in memory from the GGUF's staging on the lane `q8` names - no `.dlim` baked, no pin taken.
def private mint_tts_model(path : string; q8 : bool) : TtsModel {
    var inscope m = TtsModel()
    var inscope stg <- stage_styletts2(path, q8)
    mint_styletts2(stg, m.model)
    m.kind = m.model.family == "kokoro" ? TtsKind.kokoro : TtsKind.kitten
    let dir = dir_name(path)
    m.g2p <- load_g2p(g2p_pack_path(dir))
    m.tagger <- load_pos_tagger(path_join(dir, "tts_postag.bin"))
    return <- m
}

//! One family's rig: where its model and dumps live, and how it turns the fixture's phoneme
//! string into token ids.
struct TtsParityRig {
    family : string
    gguf : string
    oracle_dir : string
    ids : function<(ps : string) : array<int>>
}

//! `ours` against `ref` within `bar` - absolute, or relative to the reference's peak - logged on green and red
//! alike, with the bar's control: one expected element moved by twice the bar must land outside it.
def bar_diff(t : T?; tag : string; ours, ref : array<float>; bar : float; absolute : bool) {
    if (length(ours) != length(ref)) {
        t |> failure("{tag}: {length(ours)} elements against the reference's {length(ref)}")
        return
    }
    let peak = logits_maxabs(ref)
    let lim = absolute ? bar : bar * peak
    let d = max_abs_diff(ours, ref)
    to_log(LOG_INFO, "{tag}: maxdiff {d} over {length(ref)} (reference peak {peak})\n")
    t |> success(d <= lim, "{tag}: maxdiff {d} <= {lim}")
    if (!empty(ref)) {
        var inscope poisoned := ref
        poisoned[0] += 2.0 * lim
        t |> success(max_abs_diff(ours, poisoned) > lim, "{tag}: the bar discriminates a poisoned expectation")
    }
}

//! One stage against its dump within BAR (of the oracle's peak, or absolute); a stage the dump lacks is skipped.
def stage_diff(t : T?; tag, name : string; ours : array<float>; dump : table<string; TtsDumpTensor>; absolute : bool = false) {
    dump |> get(name) $(tensor) {
        bar_diff(t, "{tag} {name}", ours, tensor.f, BAR, absolute)
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
def private seam_check(t : T?; m, m8 : StyleTts2Model; tag : string; dump : table<string; TtsDumpTensor>) {
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
    let n = styletts2_source(m.decoder, m.source, f0, f2, noise, 0ul, har, sc)
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
    gpu_seam_check(t, m, tag, gen_in, cw, f2, s_ref, spec, frames, wave, sc)
    gpu_stage_checks(t, m, m8, tag, dump, noise, sc)
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

[unused_argument(t, m, tag, gen_in, cw, f2, s_ref, spec, frames, wave_cpu, sc)]   // off-Apple the gated body is empty
def private gpu_seam_check(t : T?; m : StyleTts2Model; tag : string; gen_in : array<float>; cw, f2 : int64; s_ref, spec : array<float>;
                           frames : int64; wave_cpu : array<float>; var sc : St2Scratch) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let knob_found = metal_tower_knob()
        defer() {
            set_metal_tower(knob_found)
        }
        var inscope wave : array<float>
        let h0 = styletts2_generator_gpu_stats()
        let s0 = metal_tower_stats()
        set_metal_tower(true)
        styletts2_generator(m.decoder, gen_in, cw, f2, s_ref, spec, frames, null, wave, sc)
        let h1 = styletts2_generator_gpu_stats()
        let s1 = metal_tower_stats()
        var inscope gen_x <- scaled(gen_in, 3.0)
        var inscope wave_p : array<float>
        styletts2_generator(m.decoder, gen_x, cw, f2, s_ref, spec, frames, null, wave_p, sc)
        return if (!gpu_stage_verdict(t, tag, "SEAM", wave, wave_cpu, wave_p, GPU_SEAM_BAR, h1.calls - h0.calls, h1.served - h0.served,
                                      s1.encodes - s0.encodes, "{metal_tower_declines()}"))
        t |> success(s1.convs - s0.convs > 0l, "{tag} GPU SEAM: the convs rode the tower")
        knob_off_leg(t, "{tag} GPU SEAM", 1l, $ => styletts2_generator_gpu_stats()) $ {
            var inscope wave_off : array<float>
            styletts2_generator(m.decoder, gen_in, cw, f2, s_ref, spec, frames, null, wave_off, sc)
            return rel_rms(wave_off, wave_cpu) == 0.0
        }
    }
}

//! The knob-off leg: `run` with the tower knob off still reaches the seat `stats` reads - one call, none served -
//! records `declines` knob declines, and is the CPU chain (`run` answers whether its output equals the CPU chain's).
//! The knob stays off after.
[unused_argument(t, tag, declines, stats, run)]   // off-Apple the gated body is empty
def knob_off_leg(t : T?; tag : string; declines : int64; stats : block<() : tuple<calls : int; served : int>>; run : block<() : bool>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_tower(false)
        let h0 = invoke(stats)
        let d0 = metal_tower_declines()
        let same = invoke(run)
        let h1 = invoke(stats)
        let d1 = metal_tower_declines()
        t |> equal(h1.calls - h0.calls, 1, "{tag}: the knob-off leg still reaches the hook")
        t |> equal(h1.served - h0.served, 0, "{tag}: the knob-off leg serves nothing")
        t |> equal((d1?["knob"] ?? 0l) - (d0?["knob"] ?? 0l), declines, "{tag}: the knob-off leg records its decline at every seat it reaches")
        t |> success(same, "{tag}: the knob-off leg is the CPU chain")
    }
}

//! `g` with one element moved by twice what the rel-rms bar tolerates over the whole row: the smallest wrong output the bar must catch.
def one_wrong(g, cpu : array<float>; bar : float) : array<float> {
    var w := g
    let n = long_length(cpu)
    return <- w if (n < 1l)
    var den = 0.0lf
    for (v in cpu) {
        den += double(v) * double(v)
    }
    w[n / 2l] += 2.0 * bar * float(sqrt(den))
    return <- w
}

//! Asserts one hook call, one tower encode, the served leg's length and its rel-rms within `bar`, the poisoned leg
//! and the one-wrong-element leg past it; false where the tower did not serve once.
def gpu_stage_verdict(t : T?; tag, stage : string; g, cpu, gp : array<float>; bar : float; calls, served : int; encodes : int64; declines : string) : bool {
    return tower_served(t, "{tag} GPU {stage}", declines) if (served != 1)
    let d = rel_rms(g, cpu)
    let dp = rel_rms(gp, cpu)
    to_log(LOG_INFO, "{tag} GPU {stage}: rel-rms {d} against the CPU stage over {length(cpu)} (bar {bar}); poisoned input {dp}\n")
    t |> equal(calls, 1, "{tag} GPU {stage}: one hook call")
    t |> equal(encodes, 1l, "{tag} GPU {stage}: one tower encode served the stage")
    t |> equal(length(g), length(cpu), "{tag} GPU {stage}: the output length")
    t |> success(d <= bar, "{tag} GPU {stage}: rel-rms {d} within {bar}")
    t |> success(dp > bar, "{tag} GPU {stage}: the poisoned input reds the bar (the compare's control)")
    var inscope w <- one_wrong(g, cpu, bar)
    t |> success(rel_rms(w, cpu) > bar, "{tag} GPU {stage}: one output element moved by twice the bar reds it (the bar's control)")
    return true
}

def scaled(a : array<float>; k : float) : array<float> {
    var r := a
    for (v in r) {
        v *= k
    }
    return <- r
}

//! The three runs a seat leg compares.
enum TowerLeg {
    cpu        //! the knob off: the CPU stage, the reference
    served     //! the knob on: the stage on its seat
    poisoned   //! the knob on over a scaled or reversed input: the compare's control
}

//! One StyleTTS2 stage on its seat: `run` writes the stage's output for each leg - the CPU leg's into `cpu`, which the
//! caller keeps - the served leg counted at the `stage` seat and in the tower's encodes, then the verdict. The knob stays on.
[unused_argument(t, tag, stage, bar, cpu, run)]   // off-Apple the gated body is empty
def private seat_leg(t : T?; tag, stage : string; bar : float; var cpu : array<float>; run : block<(leg : TowerLeg; var out : array<float>) : void>) : bool {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_tower(false)
        invoke(run, TowerLeg.cpu, cpu)
        set_metal_tower(true)
        let h0 = styletts2_gpu_stats(stage)
        let e0 = metal_tower_stats().encodes
        var inscope g : array<float>
        invoke(run, TowerLeg.served, g)
        let h1 = styletts2_gpu_stats(stage)
        let e1 = metal_tower_stats().encodes
        var inscope p : array<float>
        invoke(run, TowerLeg.poisoned, p)
        return gpu_stage_verdict(t, tag, stage, g, cpu, p, bar, h1.calls - h0.calls, h1.served - h0.served, e1 - e0, "{metal_tower_declines()}")
    } else {
        return false
    }
}

//! Every front, predictor and decoder stage on the tower fed the CPU stage's own inputs (the
//! oracle's ids, style and captured noise): each within its bar, each served once, a scaled or
//! reversed input as each compare's control; the durations token for token.
[unused_argument(t, m, m8, tag, dump, noise, sc)]   // off-Apple the gated body is empty
def private gpu_stage_checks(t : T?; m, m8 : StyleTts2Model; tag : string; dump : table<string; TtsDumpTensor>; var noise : TtsNoise; var sc : St2Scratch) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let knob_found = metal_tower_knob()
        defer() {
            set_metal_tower(knob_found)
        }
        var inscope ids <- dump_i(dump, "input_ids")
        var inscope style <- dump_f(dump, "style")
        let hs = m.style_dim / 2l
        var inscope s_ref : array<float>
        var inscope s_pro : array<float>
        s_ref |> resize(hs)
        s_pro |> resize(hs)
        for (i in range64(hs)) {
            s_ref[i] = style[i]
            s_pro[i] = style[hs + i]
        }
        let n_tok = long_length(ids)
        var inscope bert_c : array<float>
        var inscope ten_c : array<float>
        return if (!gpu_front_checks(t, m, tag, ids, bert_c, ten_c, sc))
        var inscope d_c : array<float>
        let dur_served = seat_leg(t, tag, "dur_enc", GPU_PRED_BAR, d_c) $(leg : TowerLeg; var out : array<float>) {
            if (leg == TowerLeg.poisoned) {
                var inscope bert_x <- scaled(bert_c, 3.0)
                styletts2_duration_encoder(m.predictor, bert_x, n_tok, s_pro, out, sc)
            } else {
                styletts2_duration_encoder(m.predictor, bert_c, n_tok, s_pro, out, sc)
            }
        }
        return if (!dur_served)
        var inscope dur_c : array<int>
        var inscope raw_c : array<float>
        var frames = 0l
        var frames_g = 0l
        let durations_served = seat_leg(t, tag, "durations", GPU_PRED_BAR, raw_c) $(leg : TowerLeg; var out : array<float>) {
            var inscope dur : array<int>
            if (leg == TowerLeg.poisoned) {
                var inscope d_x <- scaled(d_c, 3.0)
                styletts2_durations(m.predictor, d_x, n_tok, 1.0, dur, out, sc)
            } elif (leg == TowerLeg.served) {
                frames_g = styletts2_durations(m.predictor, d_c, n_tok, 1.0, dur, out, sc)
            } else {
                frames = styletts2_durations(m.predictor, d_c, n_tok, 1.0, dur_c, out, sc)
            }
        }
        return if (!durations_served)
        t |> equal(frames_g, frames, "{tag} GPU durations: the rounded frame count")
        var inscope f0_c : array<float>
        var inscope n_c : array<float>
        let f2 = gpu_prosody_check(t, m, tag, d_c, n_tok, dur_c, frames, s_pro, f0_c, n_c, sc)
        return if (f2 < 0l)
        gpu_decode_check(t, m, tag, ten_c, n_tok, dur_c, frames, f0_c, n_c, f2, s_ref, noise, sc, GPU_DECODE_BAR)
        // the served q8 lane: the same inputs through the q8 planes, the CPU chain quantizing its
        // activations against the tower's dequantized weights - the q8 slab writes' only numeric gate
        var inscope sc8 = St2Scratch()
        gpu_decode_check(t, m8, "{tag} q8 lane", ten_c, n_tok, dur_c, frames, f0_c, n_c, f2, s_ref, noise, sc8, GPU_DECODE_Q8_BAR)
    }
}

//! PL-BERT and the text encoder on the tower against the CPU stages, the reversed ids as each compare's control; the CPU rows out.
[unused_argument(t, m, tag, ids, bert_c, ten_c, sc)]   // off-Apple the gated body is empty
def private gpu_front_checks(t : T?; m : StyleTts2Model; tag : string; ids : array<int>; var bert_c, ten_c : array<float>; var sc : St2Scratch) : bool {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let knob_found = metal_tower_knob()
        defer() {
            set_metal_tower(knob_found)
        }
        var inscope rids <- [for (id in reverse(ids)); id]
        let albert_served = seat_leg(t, tag, "albert", GPU_ALBERT_BAR, bert_c) $(leg : TowerLeg; var out : array<float>) {
            styletts2_albert(m.albert, leg == TowerLeg.poisoned ? rids : ids, out, sc)
        }
        return false if (!albert_served)
        return seat_leg(t, tag, "text", GPU_TEXT_BAR, ten_c) $(leg : TowerLeg; var out : array<float>) {
            styletts2_text_encoder(m.text_encoder, leg == TowerLeg.poisoned ? rids : ids, out, sc)
        }
    } else {
        return false
    }
}

//! The CPU F0 and N rows out and f2, or -1 where the tower did not serve or missed its bar.
[unused_argument(t, m, tag, d_c, n_tok, dur_c, frames, s_pro, f0_c, n_c, sc)]   // off-Apple the gated body is empty
def private gpu_prosody_check(t : T?; m : StyleTts2Model; tag : string; d_c : array<float>; n_tok : int64; dur_c : array<int>; frames : int64;
                              s_pro : array<float>; var f0_c, n_c : array<float>; var sc : St2Scratch) : int64 {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let knob_found = metal_tower_knob()
        defer() {
            set_metal_tower(knob_found)
        }
        let dc = m.predictor.bert_encoder.nout + m.style_dim / 2l
        var inscope en : array<float>
        align_expand_rows(d_c, n_tok, dc, dur_c, en)
        var inscope n_g : array<float>
        var f2 = 0l
        var f2g = 0l
        let served = seat_leg(t, tag, "prosody", GPU_PRED_BAR, f0_c) $(leg : TowerLeg; var out : array<float>) {
            if (leg == TowerLeg.poisoned) {
                var inscope en_x <- scaled(en, 3.0)
                var inscope n_p : array<float>
                styletts2_prosody(m.predictor, en_x, dc, frames, s_pro, out, n_p, sc)
            } elif (leg == TowerLeg.served) {
                f2g = styletts2_prosody(m.predictor, en, dc, frames, s_pro, out, n_g, sc)
            } else {
                f2 = styletts2_prosody(m.predictor, en, dc, frames, s_pro, out, n_c, sc)
            }
        }
        return -1l if (!served)
        t |> success(rel_rms(n_g, n_c) <= GPU_PRED_BAR, "{tag} GPU prosody: the N rows within {GPU_PRED_BAR} (rel-rms {rel_rms(n_g, n_c)})")
        t |> equal(f2g, f2, "{tag} GPU prosody: the frame count")
        return f2
    } else {
        return -1l
    }
}

[unused_argument(t, m, tag, ten_c, n_tok, dur_c, frames, f0_c, n_c, f2, s_ref, noise, sc, bar)]   // off-Apple the gated body is empty
def private gpu_decode_check(t : T?; m : StyleTts2Model; tag : string; ten_c : array<float>; n_tok : int64; dur_c : array<int>; frames : int64;
                             f0_c, n_c : array<float>; f2 : int64; s_ref : array<float>; var noise : TtsNoise; var sc : St2Scratch; bar : float) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let knob_found = metal_tower_knob()
        defer() {
            set_metal_tower(knob_found)
        }
        var inscope asr : array<float>
        align_expand_channels(ten_c, m.text_encoder.c, n_tok, dur_c, asr)
        var tm = TtsTimings()
        var inscope wave_c : array<float>
        seat_leg(t, tag, "decode", bar, wave_c) $(leg : TowerLeg; var out : array<float>) {
            if (leg == TowerLeg.poisoned) {
                var inscope asr_x <- scaled(asr, 3.0)
                styletts2_decode(m.decoder, m.text_encoder.c, m.source, asr_x, frames, f0_c, n_c, f2, s_ref, noise, 0ul, null, tm, sc)
            } else {
                styletts2_decode(m.decoder, m.text_encoder.c, m.source, asr, frames, f0_c, n_c, f2, s_ref, noise, 0ul, null, tm, sc)
            }
            out := sc.wave
        }
    }
}

//! The seats one served chunk encodes; the generator seat, reached only after a declined decode, is not one.
//! The seats one served chunk encodes: every seat of the engine's record but the generator, which only a declined decode reaches.
def private served_seat_count() : int64 => long_length(styletts2_gpu_seats()) - 1l

//! A captured noise stream from `seed` - the initial phases, then `samples` rows of normals - so two legs read one noise.
def private capture_noise(var noise : TtsNoise; nh, samples : int64; seed : uint64) {
    var rng = TtsRng()
    rng_seed(rng, seed)
    noise.captured = true
    unsafe(scratch_resize(noise.uniform, nh))
    for (h in range64(nh)) {
        noise.uniform[h] = rng_uniform(rng)
    }
    unsafe(scratch_resize(noise.normal, samples * nh))
    for (i in range64(samples * nh)) {
        noise.normal[i] = rng_normal(rng)
    }
}

//! The served synthesis across the tower knob on both lanes; skips without the model, the front-end packs, or a serving tower.
[unused_argument(path, text, voice)]   // off-Apple the gated body is a skip
def tts_gpu_synthesis(t : T?; path, text, voice : string) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return if (tts_data_missing(t, path))
        with_job_que() {
            setup_dasllama_jobque_()
            return if (!gpu_served_lane(t, path, text, voice))
            gpu_reference_lane(t, path, text, voice)
        }
    } else {
        t |> skip("no Metal module in this build")
    }
}

//! The served lane's leg of `tts_gpu_synthesis`: false where the tower did not serve.
def private gpu_served_lane(t : T?; path, text, voice : string) : bool {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        reset_tts_q8()   //! the served lane is the unpinned default, whatever an earlier cell left
        var inscope m <- mint_tts_model(path, tts_serves_q8())
        let msg = "{base_name(path)} served lane"
        return served_lane_counters(t, m, text, voice, msg, "generator") $(a, b : TtsAudio; chunks : int; encodes : int64) {
            t |> equal(encodes, int64(chunks) * served_seat_count(), "{msg}: one tower encode per seat per chunk")
            let peak = logits_maxabs(b.pcm)
            t |> success(peak > 0.05, "peak {peak}: the tower leg spoke")
            var inscope silent <- scaled(b.pcm, 0.0)
            t |> success(logits_maxabs(silent) <= 0.05, "silence reads under the floor (the floor's control)")
            to_log(LOG_INFO, "{base_name(path)} {voice}: served lane, tower vs CPU: {length(b.pcm)} / {length(a.pcm)} samples, sample-wise rel-rms {rel_rms(b.pcm, a.pcm)} (the q8 chain's own rounding, informational); CPU {timings_line(a.timings, a.sample_rate)}; tower {timings_line(b.timings, b.sample_rate)}\n")
            t |> success(within_twentieth(long_length(b.pcm), long_length(a.pcm)), "the sample counts {length(b.pcm)} against {length(a.pcm)} within a twentieth")
            t |> success(!within_twentieth(long_length(b.pcm), long_length(a.pcm) / 2l), "half the count reds the twentieth (the bound's control)")
        }
    } else {
        return false
    }
}

//! A seat's counters on the family `kind` names.
def private seat_stats(kind : TtsKind; seat : string) : tuple<calls : int; served : int> {
    return kind == TtsKind.pocket ? pocket_gpu_stats(seat) : styletts2_gpu_stats(seat)
}

//! Every seat's counters of the family `kind` names, in the order `seats` lists them.
def private seat_counters(kind : TtsKind; seats : array<string>) : array<tuple<calls : int; served : int>> {
    return <- [for (s in seats); seat_stats(kind, s)]
}

//! The served synthesis across the tower knob, read at every seat of the model family's record: the knob-off leg reaches
//! each seat once a chunk, serves nothing and records one knob decline per seat per chunk; the knob-on leg serves every
//! seat once a chunk but `unserved`, which it never reaches. `extra` holds the family's own asserts over both legs' audio,
//! the chunk count and the knob-on leg's tower encodes. False where the tower served nothing.
[unused_argument(t, m, text, voice, msg, unserved, extra)]   // off-Apple the gated body is empty
def served_lane_counters(t : T?; var m : TtsModel; text, voice, msg, unserved : string;
                         extra : block<(a, b : TtsAudio; chunks : int; encodes : int64) : void>) : bool {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let knob_found = metal_tower_knob()
        defer() {
            set_metal_tower(knob_found)
        }
        var inscope seats <- m.kind == TtsKind.pocket ? pocket_gpu_seats() : styletts2_gpu_seats()
        set_metal_tower(false)
        var inscope h0 <- seat_counters(m.kind, seats)
        let d0 = metal_tower_declines()
        var inscope a <- synthesize(m, text, voice)
        var inscope h1 <- seat_counters(m.kind, seats)
        let d1 = metal_tower_declines()
        set_metal_tower(true)
        let e1 = metal_tower_stats().encodes
        var inscope b <- synthesize(m, text, voice)
        let e2 = metal_tower_stats().encodes
        var inscope h2 <- seat_counters(m.kind, seats)
        var served = 0
        for (x, y in h1, h2) {
            served += y.served - x.served
        }
        return tower_served(t, msg, "{metal_tower_declines()}") if (served == 0)
        let chunks = h1[0].calls - h0[0].calls
        t |> success(chunks >= 1, "{msg}: {chunks} chunk(s) reached the seats with the tower off")
        for (s, x, y, z in seats, h0, h1, h2) {
            t |> equal(y.calls - x.calls, chunks, "{msg}: every tower-off chunk reached the {s} seat")
            t |> equal(y.served - x.served, 0, "{msg}: the tower-off chunks served nothing at the {s} seat")
            if (s == unserved) {
                t |> equal(z.calls - y.calls, 0, "{msg}: the {s} seat is never reached while the others serve")
            } else {
                t |> equal(z.served - y.served, chunks, "{msg}: every chunk served on the tower at the {s} seat")
            }
        }
        t |> equal((d1?["knob"] ?? 0l) - (d0?["knob"] ?? 0l), int64(chunks) * long_length(seats), "{msg}: every tower-off chunk recorded a knob decline per seat")
        invoke(extra, a, b, chunks, e2 - e1)
        return true
    } else {
        return false
    }
}

def private gpu_reference_lane(t : T?; path, text, voice : string) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let knob_found = metal_tower_knob()
        defer() {
            set_metal_tower(knob_found)
        }
        var inscope m <- mint_tts_model(path, false)   //! the f32 lane: the durations round as the CPU's only there
        set_metal_tower(false)
        var inscope sized <- synthesize(m, text, voice)   //! the untrimmed count the captured stream covers
        let nh = m.model.source.n_harm
        let rows = 2l * sized.timings.samples + 65536l   //! the source's sample count runs past the generator's output; twice covers it
        capture_noise(m.scratch.noise, nh, rows, 0x5eedul)
        var inscope a <- synthesize(m, text, voice)
        set_metal_tower(true)
        let h1 = styletts2_gpu_stats("decode")
        let e1 = metal_tower_stats().encodes
        var inscope b <- synthesize(m, text, voice)
        let h2 = styletts2_gpu_stats("decode")
        let e2 = metal_tower_stats().encodes
        set_metal_tower(false)
        return if (h2.served == h1.served && !tower_served(t, "{base_name(path)} reference lane", "{metal_tower_declines()}"))
        t |> success(e2 - e1 >= int64(h2.served - h1.served), "reference lane: the tower encoded every served decode")
        t |> equal(length(b.pcm), length(a.pcm), "reference lane: the sample counts (the durations token for token)")
        to_log(LOG_INFO, "{base_name(path)} {voice}: reference lane, tower vs CPU on one captured noise: {length(b.pcm)} / {length(a.pcm)} samples, sample-wise rel-rms {rel_rms(b.pcm, a.pcm)} (the f32 phase's ulps, informational); CPU {timings_line(a.timings, a.sample_rate)}; tower {timings_line(b.timings, b.sample_rate)}\n")
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
    return if (!model_available(t, rig.gguf))
    var inscope mf <- load_tts_oracle_manifest(rig.oracle_dir)
    if (empty(mf.cases)) {
        t |> skip("{rig.oracle_dir} not stocked")
        return
    }
    with_job_que() {
        setup_dasllama_jobque_()
        var inscope stg <- stage_styletts2(rig.gguf, false)   //! the bars are the f32 lane's; minted in memory, no .dlim baked
        var inscope m = StyleTts2Model()
        mint_styletts2(stg, m)
        var inscope stg8 <- stage_styletts2(rig.gguf, true)
        var inscope m8 = StyleTts2Model()   //! the served lane's planes, for the q8 decode cell
        mint_styletts2(stg8, m8)
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
            if (sequence_equal(ours, ids)) {
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
                seam_check(t, m, m8, tag, dump)
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
