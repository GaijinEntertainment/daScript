options gen2
options indenting = 4

// Headless batch tuner for the SFX lab drum estimates. For each sounds/<name>.sfx it
// loads the reference (mimic_target), runs the SAME annealed-ES optimizer as the live
// app (main.das, verbatim) over the COMBINED scope, then saves the tuned params back.
// Deterministic (fixed seed) so the result matches what the in-app fitter would produce.
// Run: bin/daslang examples/daStrudel/sfx_lab/batch_drums.das

require strudel/strudel_sfx
require strudel/strudel_modal
require strudel/strudel_sfxr
require sfx_analysis
require audio/audio_wav
require strings
require math

let SR = 44100
let DIR = "{get_das_root()}/examples/daStrudel/sfx_lab/sounds/"

let OPT_VOICE = 0
let OPT_POST = 1
let OPT_COMBINED = 2

// ===== state mirrored from main.das =====
var g_layers : array<Layer>
var g_post = PostChain()
var g_ref : array<float>
var g_sel = 0
var g_seed = 0x1234567u
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

def load_ref(path : string) : bool {
    var samples : array<float>
    var sr = SR
    var ch = 2
    if (!read_wav(path, samples, sr, ch)) {
        delete samples
        return false
    }
    g_ref <- load_to_mono44k(samples, sr, ch)
    delete samples
    return true
}

// ===== optimizer (verbatim from main.das) =====
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

def layer_peak(li : int) : float {
    var one : array<Layer>
    one |> push_clone(g_layers[li])
    let lb <- render_sound(one)
    var lp = 0.0
    for (s in lb) { lp = max(lp, abs(s)) }
    return lp
}

def do_drum(name : string; iters : int) {
    let path = "{DIR}{name}.sfx"
    var doc = Sound()
    if (!load_sound(path, doc)) {
        to_log(LOG_ERROR, "{name}: load failed\n")
        return
    }
    if (!load_ref(doc.mimic_target)) {
        to_log(LOG_ERROR, "{name}: ref load failed ({doc.mimic_target})\n")
        return
    }
    g_layers := doc.layers
    g_post = doc.post
    g_sel = 0
    g_opt_scope = OPT_COMBINED
    g_kv |> clear(); g_klo |> clear(); g_khi |> clear()
    knobs_io(false)
    let before = eval_fit()
    let after = optimize(OPT_COMBINED, iters)
    // write tuned params back
    doc.layers := g_layers
    doc.post = g_post
    save_sound(path, doc)
    // report + mute-guard
    let muted = build_string() $(var writer) {
        for (li in range(length(g_layers))) {
            if (layer_peak(li) < 1.0e-4) {
                writer |> write(" [MUTED:{g_layers[li].name}]")
            }
        }
    }
    to_log(LOG_INFO, "{name}: {int(before*100.0)}% -> {int(after*100.0)}%  ({length(g_layers)} layers){muted}\n")
}

[export]
def main {
    let drums <- ["sd", "hh", "oh", "cp", "tom_low", "tom_high", "ride", "crash", "rimshot", "cowbell", "tambourine"]
    for (d in drums) {
        do_drum(d, 400)
    }
    to_log(LOG_INFO, "batch done\n")
}
