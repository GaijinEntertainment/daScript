options gen2
options indenting = 4

// Headless batch that reproduces the hand-coded strudel_synth render_*() drum recipes
// as parametric .sfx files (sounds/<name>_synth.sfx), to A/B against the analysis-matched
// sounds/<name>.sfx. For each drum it: builds a recipe-mirroring layer structure, renders
// the hand-coded render_*() as the optimization TARGET, runs the SAME annealed-ES optimizer
// as the app over the COMBINED scope to dial in the params, loudness-matches master_gain to
// the GM reference (so the A/B plays at equal loudness), and saves. mimic_target points at
// the GM wav so both tool copies load the same reference.
// Run: bin/daslang examples/daStrudel/sfx_lab/batch_synth.das

require strudel/strudel_sfx
require strudel/strudel_modal
require strudel/strudel_sfxr
require strudel/strudel_synth
require sfx_analysis
require audio/audio_wav
require daslib/json
require daslib/fio
require math

let SR = 44100
let SYNTH_SR = 48000   // strudel_synth renders at 48 kHz
let DIR = "/Users/borisbatkin/Work/daScript/examples/daStrudel/sfx_lab/sounds/"
let REFDIR = "/Users/borisbatkin/Work/daScript/examples/daStrudel/sfx_lab/drum_refs/"

let OPT_VOICE = 0
let OPT_POST = 1
let OPT_COMBINED = 2

// ===== state mirrored from main.das / batch_drums.das =====
var g_layers : array<Layer>
var g_post = PostChain()
var g_ref : array<float>
var g_sel = 0
var g_opt_scope = 0
var g_kv : array<float>
var g_klo : array<float>
var g_khi : array<float>

def load_to_mono44k(src : array<float>; src_rate, src_ch : int) : array<float> {
    let ch = max(src_ch, 1)
    let frames = length(src) / ch
    var mono : array<float>
    mono |> resize(frames)
    for (i in range(frames)) {
        var s = 0.0
        for (c in range(ch)) {
            s += src[i * ch + c]
        }
        mono[i] = s / float(ch)
    }
    if (src_rate == SR || frames == 0) {
        return <- mono
    }
    let outn = int(float(frames) * float(SR) / float(src_rate))
    var out : array<float>
    out |> resize(outn)
    for (i in range(outn)) {
        let pos = float(i) * float(src_rate) / float(SR)
        let i0 = int(pos)
        let frac = pos - float(i0)
        let a = mono[min(i0, frames - 1)]
        let b = mono[min(i0 + 1, frames - 1)]
        out[i] = a + (b - a) * frac
    }
    delete mono
    return <- out
}

def load_wav_mono(path : string) : array<float> {
    var samples : array<float>
    var sr = SR
    var ch = 2
    var out : array<float>
    if (read_wav(path, samples, sr, ch)) {
        out <- load_to_mono44k(samples, sr, ch)
    }
    delete samples
    return <- out
}

def rms_of(b : array<float>) : float {
    if (empty(b)) return 0.0
    var s = 0.0
    for (x in b) {
        s += x * x
    }
    return sqrt(s / float(length(b)))
}

// ===== synth target: render the hand-coded recipe (48 kHz) resampled to 44.1 kHz =====
def synth_target(name : string) : array<float> {
    var raw : array<float>
    if (name == "kick") {
        raw <- render_bd(0.6, 0.9)
    } elif (name == "sd") {
        raw <- render_sd(0.5, 0.9)
    } elif (name == "hh") {
        raw <- render_hh(0.2, 0.9)
    } elif (name == "oh") {
        raw <- render_oh(0.6, 0.9)
    } elif (name == "cp") {
        raw <- render_cp(0.5, 0.9)
    } elif (name == "tom_low") {
        raw <- render_tom(0.6, 0.9, 80.0)
    } elif (name == "tom_high") {
        raw <- render_tom(0.5, 0.9, 175.0)
    } elif (name == "ride") {
        raw <- render_ride(1.2, 0.9)
    } elif (name == "crash") {
        raw <- render_crash(1.0, 0.9)
    } elif (name == "rimshot") {
        raw <- render_rimshot(0.25, 0.9)
    } elif (name == "cowbell") {
        raw <- render_cowbell(0.5, 0.9)
    } elif (name == "tambourine") {
        raw <- render_tambourine(0.6, 0.9)
    }
    var out <- load_to_mono44k(raw, SYNTH_SR, 1)
    delete raw
    return <- out
}

// ===== recipe-mirroring initial structure =====
def push_modal(name : string; gain : float; v : ModalVoice) {
    var l = modal_layer(name, v)
    l.gain = gain
    g_layers |> push(l)
}

def push_sfxr(name : string; gain : float; s : SfxrSound) {
    var l = sfxr_layer(name, s)
    l.gain = gain
    g_layers |> push(l)
}

def build_structure(name : string) {
    g_layers |> clear()
    g_post = PostChain()
    if (name == "kick") {
        push_modal("body", 1.0, ModalVoice(pitch = 58.0, spread = 1.6, n_modes = 2, ring = 0.1,
            brightness = 0.3, damp_skew = 0.8, transient = 0.5, transient_rate = 250.0,
            echo_amount = 0.15, echo_delay = 0.02, duration = 0.6))
        push_sfxr("snap", 0.35, SfxrSound(wave = SfxrWave.noise, env_attack = 0.0, env_sustain = 0.01,
            env_punch = 0.3, env_decay = 0.18, base_freq = 0.25, lpf_freq = 0.5, hpf_freq = 0.05, sound_vol = 0.5))
    } elif (name == "sd") {
        push_modal("body", 0.9, ModalVoice(pitch = 220.0, spread = 1.5, n_modes = 2, ring = 0.083,
            brightness = 0.2, damp_skew = 0.5, transient = 0.4, transient_rate = 150.0,
            echo_amount = 0.05, echo_delay = 0.02, duration = 0.5))
        push_sfxr("wires", 0.5, SfxrSound(wave = SfxrWave.noise, env_attack = 0.0, env_sustain = 0.05,
            env_decay = 0.25, base_freq = 0.5, lpf_freq = 1.0, hpf_freq = 0.25, sound_vol = 0.5))
    } elif (name == "hh") {
        push_modal("metal", 0.9, ModalVoice(pitch = 3400.0, spread = 1.45, n_modes = 4, ring = 0.05,
            brightness = 0.85, damp_skew = 0.2, transient = 0.4, transient_rate = 1000.0,
            echo_amount = 0.0, echo_delay = 0.02, duration = 0.2))
        push_modal("bell", 0.25, ModalVoice(pitch = 180.0, spread = 1.5, n_modes = 1, ring = 0.033,
            brightness = 0.5, damp_skew = 0.5, transient = 0.0, transient_rate = 600.0,
            echo_amount = 0.0, echo_delay = 0.02, duration = 0.15))
    } elif (name == "oh") {
        push_modal("metal", 1.0, ModalVoice(pitch = 3400.0, spread = 1.45, n_modes = 4, ring = 0.125,
            brightness = 0.85, damp_skew = 0.2, transient = 0.4, transient_rate = 1000.0,
            echo_amount = 0.1, echo_delay = 0.02, duration = 0.6))
    } elif (name == "cp") {
        push_sfxr("clap", 1.0, SfxrSound(wave = SfxrWave.noise, env_attack = 0.0, env_sustain = 0.02,
            env_punch = 0.4, env_decay = 0.2, base_freq = 0.45, lpf_freq = 1.0, hpf_freq = 0.0, sound_vol = 0.5))
        g_post.bandpass = FxBandpass(enabled = true, freq = 1100.0, q = 2.0)
        g_post.reverb = FxReverb(enabled = true, wet = 0.18, decay = 0.7, lp_start = 6000.0, lp_end = 2000.0)
    } elif (name == "tom_low") {
        push_modal("body", 1.0, ModalVoice(pitch = 80.0, spread = 1.6, n_modes = 3, ring = 0.125,
            brightness = 0.35, damp_skew = 0.5, transient = 0.5, transient_rate = 220.0,
            echo_amount = 0.05, echo_delay = 0.045, duration = 0.6))
        push_sfxr("click", 0.3, SfxrSound(wave = SfxrWave.noise, env_attack = 0.0, env_sustain = 0.0,
            env_punch = 0.4, env_decay = 0.08, base_freq = 0.3, lpf_freq = 0.6, hpf_freq = 0.05, sound_vol = 0.5))
    } elif (name == "tom_high") {
        push_modal("body", 1.0, ModalVoice(pitch = 175.0, spread = 1.6, n_modes = 3, ring = 0.11,
            brightness = 0.35, damp_skew = 0.5, transient = 0.5, transient_rate = 220.0,
            echo_amount = 0.05, echo_delay = 0.04, duration = 0.5))
        push_sfxr("click", 0.3, SfxrSound(wave = SfxrWave.noise, env_attack = 0.0, env_sustain = 0.0,
            env_punch = 0.4, env_decay = 0.08, base_freq = 0.3, lpf_freq = 0.6, hpf_freq = 0.05, sound_vol = 0.5))
    } elif (name == "ride") {
        push_modal("bell", 0.8, ModalVoice(pitch = 340.0, spread = 1.138, n_modes = 2, ring = 0.4,
            brightness = 0.8, damp_skew = 0.1, transient = 0.2, transient_rate = 600.0,
            echo_amount = 0.1, echo_delay = 0.04, duration = 1.0))
        push_modal("shimmer", 0.35, ModalVoice(pitch = 800.0, spread = 1.3, n_modes = 5, ring = 0.4,
            brightness = 0.7, damp_skew = 0.2, transient = 0.2, transient_rate = 800.0,
            echo_amount = 0.0, echo_delay = 0.02, duration = 1.0))
    } elif (name == "crash") {
        push_modal("bell", 0.6, ModalVoice(pitch = 180.0, spread = 1.378, n_modes = 2, ring = 0.125,
            brightness = 0.6, damp_skew = 0.2, transient = 0.3, transient_rate = 600.0,
            echo_amount = 0.05, echo_delay = 0.03, duration = 1.0))
        push_sfxr("wash", 0.7, SfxrSound(wave = SfxrWave.noise, env_attack = 0.0, env_sustain = 0.1,
            env_decay = 0.4, base_freq = 0.6, lpf_freq = 1.0, hpf_freq = 0.5, sound_vol = 0.5))
    } elif (name == "rimshot") {
        push_modal("ping", 0.7, ModalVoice(pitch = 200.0, spread = 1.5, n_modes = 1, ring = 0.0125,
            brightness = 0.5, damp_skew = 0.5, transient = 0.6, transient_rate = 400.0,
            echo_amount = 0.1, echo_delay = 0.02, duration = 0.25))
        push_sfxr("snap", 0.9, SfxrSound(wave = SfxrWave.noise, env_attack = 0.0, env_sustain = 0.0,
            env_punch = 0.5, env_decay = 0.1, base_freq = 0.35, lpf_freq = 0.7, hpf_freq = 0.0, sound_vol = 0.5))
        g_post.bandpass = FxBandpass(enabled = true, freq = 800.0, q = 0.8)
    } elif (name == "cowbell") {
        push_modal("tone", 1.0, ModalVoice(pitch = 375.0, spread = 1.344, n_modes = 2, ring = 0.05,
            brightness = 0.9, damp_skew = 0.2, transient = 0.2, transient_rate = 800.0,
            echo_amount = 0.2, echo_delay = 0.008, duration = 0.5))
        g_post.bandpass = FxBandpass(enabled = true, freq = 850.0, q = 4.0)
    } elif (name == "tambourine") {
        push_sfxr("jingle", 1.0, SfxrSound(wave = SfxrWave.noise, env_attack = 0.0, env_sustain = 0.05,
            env_decay = 0.25, base_freq = 0.7, lpf_freq = 1.0, hpf_freq = 0.65, sound_vol = 0.5))
        g_post.bandpass = FxBandpass(enabled = true, freq = 3800.0, q = 8.0)
        g_post.reverb = FxReverb(enabled = true, wet = 0.35, decay = 0.5, lp_start = 12000.0, lp_end = 3000.0)
    }
}

// ===== optimizer (verbatim from batch_drums.das) =====
def orand(var seed : uint&) : float {
    seed = seed * 1664525u + 1013904223u
    return float(seed >> 8u) * (1.0 / 16777216.0)
}

def knob(writing : bool; var i : int&; var field : float&; lo, hi : float) {
    if (writing) {
        field = clamp(g_kv[i], lo, hi)
    } else {
        if (length(g_kv) < i + 1) {
            g_kv |> resize(i + 1)
            g_klo |> resize(i + 1)
            g_khi |> resize(i + 1)
        }
        g_kv[i] = field
        g_klo[i] = lo
        g_khi[i] = hi
    }
    i++
}

def voice_knobs_io(writing : bool; li : int; var i : int&) {
    if (g_layers[li].voice is sfxr) {
        var s = g_layers[li].voice as sfxr
        knob(writing, i, s.base_freq, 0.0, 1.0)
        knob(writing, i, s.freq_limit, 0.0, 1.0)
        knob(writing, i, s.freq_ramp, -1.0, 1.0)
        knob(writing, i, s.freq_dramp, -1.0, 1.0)
        knob(writing, i, s.duty, 0.0, 1.0)
        knob(writing, i, s.duty_ramp, -1.0, 1.0)
        knob(writing, i, s.vib_strength, 0.0, 1.0)
        knob(writing, i, s.vib_speed, 0.0, 1.0)
        knob(writing, i, s.arp_mod, -1.0, 1.0)
        knob(writing, i, s.arp_speed, 0.0, 1.0)
        knob(writing, i, s.repeat_speed, 0.0, 1.0)
        knob(writing, i, s.pha_offset, -1.0, 1.0)
        knob(writing, i, s.pha_ramp, -1.0, 1.0)
        knob(writing, i, s.lpf_freq, 0.0, 1.0)
        knob(writing, i, s.lpf_ramp, -1.0, 1.0)
        knob(writing, i, s.lpf_resonance, 0.0, 1.0)
        knob(writing, i, s.hpf_freq, 0.0, 1.0)
        knob(writing, i, s.hpf_ramp, -1.0, 1.0)
        knob(writing, i, s.env_attack, 0.0, 1.0)
        knob(writing, i, s.env_sustain, 0.0, 1.0)
        knob(writing, i, s.env_punch, 0.0, 1.0)
        knob(writing, i, s.env_decay, 0.0, 1.0)
        knob(writing, i, s.sound_vol, 0.0, 1.0)
        if (writing) {
            g_layers[li].voice = VoiceParams(sfxr = s)
        }
    } else {
        var v = g_layers[li].voice as modal
        knob(writing, i, v.pitch, 20.0, 4000.0)
        knob(writing, i, v.spread, 1.0, 2.6)
        knob(writing, i, v.ring, 0.005, 0.4)
        knob(writing, i, v.brightness, 0.0, 1.0)
        knob(writing, i, v.damp_skew, 0.0, 1.5)
        knob(writing, i, v.transient, 0.0, 1.0)
        knob(writing, i, v.transient_rate, 100.0, 2000.0)
        knob(writing, i, v.echo_amount, 0.0, 1.0)
        knob(writing, i, v.echo_delay, 0.005, 0.15)
        knob(writing, i, v.duration, 0.05, 1.0)
        if (writing) {
            g_layers[li].voice = VoiceParams(modal = v)
        }
    }
}

def knobs_io(writing : bool) {
    var i = 0
    if (g_opt_scope == OPT_VOICE) {
        if (g_sel >= 0 && g_sel < length(g_layers)) {
            voice_knobs_io(writing, g_sel, i)
        }
    } elif (g_opt_scope == OPT_COMBINED) {
        for (li in range(length(g_layers))) {
            voice_knobs_io(writing, li, i)
        }
    } else {
    }
}

def eval_fit() : float {
    if (empty(g_ref)) return 0.0
    var mix <- render_sound(g_layers)
    apply_post(mix, g_post, float(SR))
    let f = fit(g_ref, mix, SR)
    delete mix
    return f.overall
}

def optimize(scope : int; iterations : int) : float {
    g_opt_scope = scope
    g_kv |> clear()
    g_klo |> clear()
    g_khi |> clear()
    knobs_io(false)
    let n = length(g_kv)
    if (n == 0 || empty(g_ref)) return -1.0
    var best : array<float>
    best := g_kv
    var best_fit = eval_fit()
    var seed = 0x51ED2C7Fu
    var sigma = 0.18
    let LAMBDA = 6
    for (_it in range(iterations)) {
        var improved = false
        for (_c in range(LAMBDA)) {
            for (k in range(n)) {
                let span = g_khi[k] - g_klo[k]
                let delta = (orand(seed) * 2.0 - 1.0) * sigma * span
                g_kv[k] = clamp(best[k] + delta, g_klo[k], g_khi[k])
            }
            knobs_io(true)
            let f = eval_fit()
            if (f > best_fit) {
                best_fit = f
                best := g_kv
                improved = true
            }
        }
        if (!improved) {
            sigma *= 0.7
            if (sigma < 0.01) break
        }
    }
    g_kv := best
    knobs_io(true)
    delete best
    return best_fit
}

def do_synth(name : string; iters : int) {
    build_structure(name)
    if (empty(g_layers)) {
        to_log(LOG_ERROR, "{name}: no structure\n")
        return
    }
    g_ref <- synth_target(name)
    if (empty(g_ref)) {
        to_log(LOG_ERROR, "{name}: synth target render failed\n")
        return
    }
    g_sel = 0
    let before = eval_fit()
    let after = optimize(OPT_COMBINED, iters)
    // loudness-match master_gain to the GM reference (the kick's GM file is bd.wav)
    let refname = name == "kick" ? "bd" : name
    let refpath = "{REFDIR}{refname}.wav"
    let gm <- load_wav_mono(refpath)
    let gm_rms = rms_of(gm)
    var mix <- render_sound(g_layers)
    apply_post(mix, g_post, float(SR))
    let mix_rms = rms_of(mix)
    let mg = clamp(mix_rms > 1.0e-6 && gm_rms > 1.0e-6 ? gm_rms / mix_rms : 1.0, 0.05, 1.12)
    delete mix
    var doc = Sound()
    doc.mimic_target = refpath
    doc.comment = "from render"
    doc.layers := g_layers
    doc.post = g_post
    doc.master_gain = mg
    save_sound("{DIR}{name}_synth.sfx", doc)
    to_log(LOG_INFO, "{name}_synth: {int(before * 100.0)}% -> {int(after * 100.0)}%  ({length(g_layers)} layers, mg={mg})\n")
}

[export]
def main {
    let drums <- ["kick", "sd", "hh", "oh", "cp", "tom_low", "tom_high", "ride", "crash", "rimshot", "cowbell", "tambourine"]
    for (d in drums) {
        do_synth(d, 300)
    }
    to_log(LOG_INFO, "batch_synth done\n")
}
