// SFX Lab — multi-layer procedural sound-effect workbench.
// A sound is a stack of layers (sfxr or modal voices), each with its own gain + onset delay,
// mixed. Docked panels: Layers / Editor / Mix. Build, audition per-layer or the mix, and watch
// the waveform + spectrum live. (Reference target + save/load land in later slices.)
//
// Run: daslang -load_module <dasImgui> -load_module <dasImguiImplot> examples/daStrudel/sfx_lab/main.das
options gen2
options _allow_imgui_legacy = true   // raw GetActiveID (host-agnostic) for the undo coalesce key

require imgui/imgui_harness
require imgui/imgui_containers_builtin
require imgui/imgui_widgets_builtin
require imgui/imgui_file_dialog
require imgui/imgui_implot_boost_v2
require implot
require audio/audio_boost
require audio/audio_wav
require strudel/strudel_sfx
require imgui/imgui_icons
require minfft
require sfx_analysis
require live/live_commands
require daslib/json_boost
require daslib/fio
require math
require strings

let SR = 44100
let TWO_PI = 6.28318530717958647692
let INV_LN10 = 0.43429448190325176

// ===== app state =====
var g_layers : array<Layer>
var g_sel = 0
var g_sel_loaded = -1            // which layer's voice is currently in the editor sliders
var g_mix : array<float>
var g_mix_wave : array<float>
var g_mix_spec : array<float>
var g_lyr_wave : array<float>    // selected layer's own preview
var g_lyr_spec : array<float>
var g_ref : array<float>         // loaded reference target (mono @ SR)
var g_ref_wave : array<float>
var g_ref_spec : array<float>
var g_ref_name = "(no reference loaded)"
var g_ref_path = ""              // full path of the loaded reference (saved as mimic_target)
var g_show_ref = true            // overlay the reference on the mix plots
var g_fit : FitScore             // current reference-vs-mix fit (valid when g_have_fit)
var g_ref_feat : SfxFeatures
var g_mix_feat : SfxFeatures
var g_have_fit = false

// spectrogram (spectrum-over-time) — STFT dB heatmap, SPEC_BINS x SPEC_FRAMES row-major
let SPEC_FRAMES = 64
let SPEC_BINS = 48
let SPEC_FMAX = 8000.0
var g_mix_spectro : array<float>
var g_ref_spectro : array<float>

// postprocess effect chain (applied to the mix before analysis), persisted in the .sfx
var g_post = PostChain()
var g_post_reload = true          // push g_post -> the postprocess sliders on next frame
var g_sound_path = ""             // last opened/saved .sfx path (for sfx_save with no arg)

// undo/redo: each entry is a serialized sound state + the widget id that produced it
// (consecutive edits with the same nonzero id coalesce, so a slider drag = one step)
struct EditSnap {
    json : string
    key  : uint
}
let UNDO_CAP = 60
var g_undo : array<EditSnap>
var g_undo_cursor = -1
var g_edit_dirty = false          // a user edit happened this frame; record at frame end
var g_applying_history = false    // guard: don't record while restoring a snapshot

// which file-dialog operation is in flight (the dialog is shared)
let DLG_NONE = 0
let DLG_REF = 1
let DLG_SAVE = 2
let DLG_OPEN = 3
let DLG_EXP_VOICE = 4
let DLG_IMP_VOICE = 5
let DLG_EXP_DAS = 6
var g_dialog_mode = DLG_NONE
var g_seed = 0x1234567u
var g_autoplay = true
var g_match_loudness = true       // match the mix's level to the reference's RMS (audition + baked into export)
var g_master_gain = 1.0           // export gain (post-render, clip-safe), baked into the saved .sfx as Sound.master_gain
var g_match_ratio = 1.0           // audition gain: the TRUE mix->reference RMS ratio (uncapped; peak-limited at play time)
var g_mix_dirty = true
var g_lyr_dirty = true
var g_play_pending = false       // autoplay the mix after the next rebuild (set by discrete actions)
var g_started = false
var g_ctx : ImPlotContext?
var g_audio : AudioSystemChannels

// per-row widget state (indexed by layer position) — must be declared at module scope
var private LAYER_ROW : table<int; ClickState>
var private SOLO_ROW : table<int; ClickState>
var private MUTE_ROW : table<int; ClickState>
var private PLAY_ROW : table<int; ClickState>
var private ROW_SL1 : table<int; EmptyMarkerState>
var private ROW_SL2 : table<int; EmptyMarkerState>
var private ROW_SL3 : table<int; EmptyMarkerState>

// ===== DSP / plot helpers =====

def private compute_spectrum(buf : array<float>; var out : array<float>) {
    let N = 2048
    var win : array<float>
    win |> resize(N)
    let m = min(length(buf), N)
    for (i in range(N)) {
        if (i < m) {
            let w = 0.5 - 0.5 * cos(TWO_PI * float(i) / float(N - 1))  // Hann window
            win[i] = buf[i] * w
        } else {
            win[i] = 0.0
        }
    }
    // fast FFT from the minfft module (real -> complex), then magnitudes -> dB
    var cplx : array<float2>
    fft_real_forward(win, cplx)
    var mags : array<float>
    fft_calculate_magnitudes(cplx, mags)
    let nrm = float(N) * 0.5
    let n = length(mags)
    out |> resize(n)
    for (i in range(n)) {
        out[i] = 20.0 * log(mags[i] / nrm + 1.0e-9) * INV_LN10
    }
    delete win
    delete cplx
    delete mags
}

def private downsample(buf : array<float>; var out : array<float>; maxpts : int) {
    let n = length(buf)
    let cnt = min(n, maxpts)
    out |> resize(cnt)
    if (n <= maxpts) {
        for (i in range(cnt)) { out[i] = buf[i]; }
    } else {
        for (i in range(cnt)) { out[i] = buf[i * n / maxpts]; }
    }
}

// ===== audio =====

def private play_buf_copy(buf : array<float>) {
    if (empty(buf)) return
    var tmp : array<float>
    tmp := buf
    play_sound_from_pcm(SR, 1, tmp)
}

// RMS over the ACTIVE span only (onset .. last sample above -34 dB of peak), ignoring trailing
// silence/decay-tail. Full-buffer RMS is distorted by however much dead tail a sound carries, so
// two sounds of different length can't be loudness-matched by it — this measures energy density of
// the part you actually hear, consistently for both reference and mix.
def private active_rms(buf : array<float>) : float {
    if (empty(buf)) return 0.0
    var pk = 0.0
    for (s in buf) {
        pk = max(pk, abs(s))
    }
    if (pk < 1.0e-6) return 0.0
    let thr = pk * 0.02   // -34 dB
    var first = -1
    var last = 0
    for (i in range(length(buf))) {
        if (abs(buf[i]) > thr) {
            if (first < 0) {
                first = i
            }
            last = i
        }
    }
    if (first < 0) return 0.0
    var acc = 0.0
    for (i in range(first, last + 1)) {
        acc += buf[i] * buf[i]
    }
    return sqrt(acc / float(last - first + 1))
}

// Recompute the two loudness gains from the current (post-applied) mix vs the reference:
//   g_match_ratio  = the TRUE mix->reference RMS ratio (uncapped) — used for AUDITION; play_mix
//                    applies it then peak-limits, so what you hear always sits at the reference's
//                    level no matter how the mix is tweaked (both windows stay level-locked).
//   g_master_gain  = the EXPORT gain baked into the .sfx; capped so the raw render (no limiter in
//                    render_sound_file) can't clip on playback.
// Both reset to 1.0 when matching is off / no reference — so "off" is genuinely raw, audition AND
// export alike.
def update_levels() {
    g_match_ratio = 1.0
    g_master_gain = 1.0
    if (!g_match_loudness || empty(g_ref) || empty(g_mix)) return
    let mr = active_rms(g_mix)
    if (mr <= 1.0e-6) return
    g_match_ratio = active_rms(g_ref) / mr
    var pk = 0.0
    for (s in g_mix) {
        pk = max(pk, abs(s))
    }
    let ceil = pk > 1.0e-6 ? 0.99 / pk : 1.12   // clip-safe ceiling for the baked export gain
    g_master_gain = min(g_match_ratio, ceil)
}

def play_mix() {
    if (g_mix_dirty) {
        rebuild_mix()
    }
    if (g_match_ratio == 1.0) {
        play_buf_copy(g_mix)
        return
    }
    var tmp : array<float>
    tmp := g_mix
    for (s in tmp) {
        s *= g_match_ratio
    }
    var pk = 0.0
    for (s in tmp) {
        pk = max(pk, abs(s))
    }
    if (pk > 0.99) {            // matched to reference RMS, but never let the audition clip
        let lim = 0.99 / pk
        for (s in tmp) {
            s *= lim
        }
    }
    play_sound_from_pcm(SR, 1, tmp)
}

def play_layer(idx : int) {
    if (idx < 0 || idx >= length(g_layers)) return
    let g = g_layers[idx].gain
    var b <- render_voice(g_layers[idx].voice)
    for (s in b) { s *= g }   // the per-layer audition must honor the layer's gain (gain 0 = silent)
    play_buf_copy(b)
    delete b
}

def rebuild_mix() {
    propagate_links(g_layers)
    // solo: if any layer is soloed, only soloed (and enabled) layers play
    var any_solo = false
    for (lyr in g_layers) {
        if (lyr.solo) {
            any_solo = true
            break
        }
    }
    var eff : array<Layer>
    eff |> reserve(length(g_layers))
    for (lyr in g_layers) {
        var c = lyr
        if (any_solo) {
            c.enabled = lyr.enabled && lyr.solo
        }
        eff |> push(c)
    }
    g_mix <- render_sound(eff)
    delete eff
    apply_post(g_mix, g_post, float(SR))   // postprocess before analysis, so the metrics see it
    downsample(g_mix, g_mix_wave, 2000)
    compute_spectrum(g_mix, g_mix_spec)
    g_mix_spectro <- spectrogram(g_mix, SR, SPEC_FRAMES, SPEC_BINS, SPEC_FMAX)
    recompute_fit()
    update_levels()   // re-match the audition + export levels to the reference as the mix changes
    g_mix_dirty = false
}

// reference-vs-mix features + fit; valid only when both a reference and a mix exist
def recompute_fit() {
    if (empty(g_ref) || empty(g_mix)) {
        g_have_fit = false
        return
    }
    g_ref_feat = analyze(g_ref, SR)
    g_mix_feat = analyze(g_mix, SR)
    g_fit = fit(g_ref, g_mix, SR)
    g_have_fit = true
}

// ===== reference target =====

def private load_to_mono44k(src : array<float>; src_rate, src_ch : int) : array<float> {
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

def load_reference(path : string) {
    var samples : array<float>
    var sr = SR
    var ch = 2
    if (!read_wav(path, samples, sr, ch)) {
        g_ref_name = "load failed: {base_name(path)}"
        delete samples
        return
    }
    g_ref <- load_to_mono44k(samples, sr, ch)
    delete samples
    downsample(g_ref, g_ref_wave, 2000)
    compute_spectrum(g_ref, g_ref_spec)
    g_ref_spectro <- spectrogram(g_ref, SR, SPEC_FRAMES, SPEC_BINS, SPEC_FMAX)
    g_ref_name = "{base_name(path)}  ({sr} Hz, {ch} ch)"
    g_ref_path = path
    g_show_ref = true
    REF_SHOW.value = true
    recompute_fit()
    update_levels()   // match the audition + export levels to the freshly-loaded reference
}

def do_save_sound(path : string) {
    var doc = Sound()
    doc.mimic_target = g_ref_path
    doc.comment = SOUND_COMMENT.value   // freeform label (which lineage / variant this is)
    doc.layers := g_layers
    doc.post = g_post
    doc.master_gain = g_master_gain   // bake the audited level so the exported .sfx plays the same
    save_sound(path, doc)
    g_sound_path = path
}

def do_export_voice(path : string) {
    if (g_sel < 0 || g_sel >= length(g_layers)) return
    save_layer(path, g_layers[g_sel])
}

def do_import_voice(path : string) {
    var lyr = Layer()
    if (!load_layer(path, lyr)) return
    add_layer(lyr)            // selects it at the end
    g_sel_loaded = -1
    g_mix_dirty = true
    g_lyr_dirty = true
    g_edit_dirty = true       // one undoable step
    recompute_fit()
}

// ===== export the current sound as a drop-in .das render function =====
// Emits real daslang (constructors, only non-default fields) that rebuilds this sound's layers +
// post and calls the shared engine, conformed to the standard drum signature (duration, gain) and
// resampled 44.1k->48k. Drop it into strudel_synth.das (which renders at 48k) and it "just works".

def private das_join(a : array<string>; sep : string) : string {
    var s = ""
    for (i in range(length(a))) {
        if (i > 0) { s += sep }
        s += a[i]
    }
    return s
}

def private wave_name(w : SfxrWave) : string {
    if (w == SfxrWave.sawtooth) return "sawtooth"
    if (w == SfxrWave.sine) return "sine"
    if (w == SfxrWave.noise) return "noise"
    if (w == SfxrWave.triangle) return "triangle"
    return "square"
}

def private emit_modal(v : ModalVoice; pitch_param : bool; fund : float) : string {
    let d = ModalVoice()
    var a : array<string>
    if (pitch_param) {
        let r = fund > 0.0 ? v.pitch / fund : 1.0
        a |> push("pitch = {r} * base_freq")
    } elif (v.pitch != d.pitch) {
        a |> push("pitch = {v.pitch}")
    }
    if (v.spread != d.spread) { a |> push("spread = {v.spread}") }
    if (v.n_modes != d.n_modes) { a |> push("n_modes = {v.n_modes}") }
    if (v.ring != d.ring) { a |> push("ring = {v.ring}") }
    if (v.brightness != d.brightness) { a |> push("brightness = {v.brightness}") }
    if (v.damp_skew != d.damp_skew) { a |> push("damp_skew = {v.damp_skew}") }
    if (v.transient != d.transient) { a |> push("transient = {v.transient}") }
    if (v.transient_rate != d.transient_rate) { a |> push("transient_rate = {v.transient_rate}") }
    if (v.echo_amount != d.echo_amount) { a |> push("echo_amount = {v.echo_amount}") }
    if (v.echo_delay != d.echo_delay) { a |> push("echo_delay = {v.echo_delay}") }
    if (v.duration != d.duration) { a |> push("duration = {v.duration}") }
    let inner = das_join(a, ", ")
    return "ModalVoice({inner})"
}

def private emit_sfxr(s : SfxrSound) : string {
    let d = SfxrSound()
    var a : array<string>
    if (s.wave != d.wave) { a |> push("wave = SfxrWave.{wave_name(s.wave)}") }
    if (s.sound_vol != d.sound_vol) { a |> push("sound_vol = {s.sound_vol}") }
    if (s.env_attack != d.env_attack) { a |> push("env_attack = {s.env_attack}") }
    if (s.env_sustain != d.env_sustain) { a |> push("env_sustain = {s.env_sustain}") }
    if (s.env_punch != d.env_punch) { a |> push("env_punch = {s.env_punch}") }
    if (s.env_decay != d.env_decay) { a |> push("env_decay = {s.env_decay}") }
    if (s.base_freq != d.base_freq) { a |> push("base_freq = {s.base_freq}") }
    if (s.freq_limit != d.freq_limit) { a |> push("freq_limit = {s.freq_limit}") }
    if (s.freq_ramp != d.freq_ramp) { a |> push("freq_ramp = {s.freq_ramp}") }
    if (s.freq_dramp != d.freq_dramp) { a |> push("freq_dramp = {s.freq_dramp}") }
    if (s.vib_strength != d.vib_strength) { a |> push("vib_strength = {s.vib_strength}") }
    if (s.vib_speed != d.vib_speed) { a |> push("vib_speed = {s.vib_speed}") }
    if (s.arp_mod != d.arp_mod) { a |> push("arp_mod = {s.arp_mod}") }
    if (s.arp_speed != d.arp_speed) { a |> push("arp_speed = {s.arp_speed}") }
    if (s.duty != d.duty) { a |> push("duty = {s.duty}") }
    if (s.duty_ramp != d.duty_ramp) { a |> push("duty_ramp = {s.duty_ramp}") }
    if (s.repeat_speed != d.repeat_speed) { a |> push("repeat_speed = {s.repeat_speed}") }
    if (s.pha_offset != d.pha_offset) { a |> push("pha_offset = {s.pha_offset}") }
    if (s.pha_ramp != d.pha_ramp) { a |> push("pha_ramp = {s.pha_ramp}") }
    if (s.lpf_freq != d.lpf_freq) { a |> push("lpf_freq = {s.lpf_freq}") }
    if (s.lpf_ramp != d.lpf_ramp) { a |> push("lpf_ramp = {s.lpf_ramp}") }
    if (s.lpf_resonance != d.lpf_resonance) { a |> push("lpf_resonance = {s.lpf_resonance}") }
    if (s.hpf_freq != d.hpf_freq) { a |> push("hpf_freq = {s.hpf_freq}") }
    if (s.hpf_ramp != d.hpf_ramp) { a |> push("hpf_ramp = {s.hpf_ramp}") }
    let inner = das_join(a, ", ")
    return "SfxrSound({inner})"
}

def private emit_post(p : PostChain) : string {
    var a : array<string>
    if (p.filter.enabled) { a |> push("filter = FxFilter(enabled = true, position = {p.filter.position})") }
    if (p.bandpass.enabled) { a |> push("bandpass = FxBandpass(enabled = true, freq = {p.bandpass.freq}, q = {p.bandpass.q})") }
    if (p.drive.enabled) { a |> push("drive = FxDrive(enabled = true, drive = {p.drive.drive})") }
    if (p.crush.enabled) { a |> push("crush = FxCrush(enabled = true, bits = {p.crush.bits}, coarse = {p.crush.coarse})") }
    if (p.comp.enabled) { a |> push("comp = FxComp(enabled = true, threshold = {p.comp.threshold}, ratio = {p.comp.ratio}, knee = {p.comp.knee}, attack = {p.comp.attack}, release = {p.comp.release})") }
    if (p.chorus.enabled) { a |> push("chorus = FxChorus(enabled = true, rate = {p.chorus.rate}, depth = {p.chorus.depth}, feedback = {p.chorus.feedback}, delay_ms = {p.chorus.delay_ms}, wet = {p.chorus.wet})") }
    if (p.phaser.enabled) { a |> push("phaser = FxPhaser(enabled = true, rate = {p.phaser.rate}, depth = {p.phaser.depth}, center = {p.phaser.center}, sweep = {p.phaser.sweep})") }
    if (p.tremolo.enabled) { a |> push("tremolo = FxTremolo(enabled = true, rate = {p.tremolo.rate}, depth = {p.tremolo.depth})") }
    if (p.delay.enabled) { a |> push("delay = FxDelay(enabled = true, time = {p.delay.time}, feedback = {p.delay.feedback})") }
    if (p.reverb.enabled) { a |> push("reverb = FxReverb(enabled = true, wet = {p.reverb.wet}, decay = {p.reverb.decay}, lp_start = {p.reverb.lp_start}, lp_end = {p.reverb.lp_end})") }
    return das_join(a, ", ")
}

def emit_das(func_name : string; pitch_param : bool) : string {
    let LB = "\x7b"   // literal { (avoids daslang string interpolation)
    let RB = "\x7d"   // literal }
    var fund = 0.0    // fundamental = lowest modal pitch, for the pitch-scaled (tom) form
    for (lyr in g_layers) {
        if (lyr.voice is modal) {
            let mp = (lyr.voice as modal).pitch
            if (fund <= 0.0 || mp < fund) { fund = mp }
        }
    }
    let sig = pitch_param ? "duration, gain, base_freq : float" : "duration, gain : float"
    var lines : array<string>
    lines |> push("// Generated by the SFX Lab. Drop-in render function; requires:")
    lines |> push("//   require strudel/strudel_sfx   // re-exports strudel_modal + strudel_sfxr")
    lines |> push("def {func_name}({sig}) : array<float> {LB}")
    lines |> push("    var layers : array<Layer>")
    var idx = 0
    for (lyr in g_layers) {
        if (!lyr.enabled) { continue }
        let lv = "l{idx}"
        var ctor = ""
        if (lyr.voice is modal) {
            ctor = "modal_layer(\"{lyr.name}\", {emit_modal(lyr.voice as modal, pitch_param, fund)})"
        } else {
            ctor = "sfxr_layer(\"{lyr.name}\", {emit_sfxr(lyr.voice as sfxr)})"
        }
        lines |> push("    var {lv} = {ctor}")
        if (lyr.gain != 1.0) { lines |> push("    {lv}.gain = {lyr.gain}") }
        if (lyr.delay != 0.0) { lines |> push("    {lv}.delay = {lyr.delay}") }
        lines |> push("    layers |> emplace({lv})")
        idx++
    }
    lines |> push("    var out <- render_sound(layers)")
    let post = emit_post(g_post)
    if (!empty(post)) {
        lines |> push("    apply_post(out, PostChain({post}), 44100.0)")
    }
    lines |> push("    return <- sfx_conform(out, 44100, 48000, duration, gain * {g_master_gain})")
    lines |> push("{RB}")
    return das_join(lines, "\n") + "\n"
}

def do_export_das(path : string) {
    var leaf = base_name(path)
    if (length(leaf) > 4 && leaf |> ends_with(".das")) {
        leaf = slice(leaf, 0, length(leaf) - 4)
    }
    fwrite(path, emit_das(leaf, false))
}

def do_open_sound(path : string) {
    var doc = Sound()
    if (!load_sound(path, doc)) return
    g_layers := doc.layers
    g_post = doc.post
    g_post_reload = true
    g_master_gain = doc.master_gain   // restore the baked level (re-matched live below if a ref loads)
    g_sound_path = path
    SOUND_COMMENT.pending_value = doc.comment   // push the label into the top-of-panel editor
    SOUND_COMMENT.has_pending = true
    g_sel = 0
    g_sel_loaded = -1
    g_mix_dirty = true
    g_lyr_dirty = true
    if (!empty(doc.mimic_target)) {
        load_reference(doc.mimic_target)
    }
    reset_history()
}

// Analysis-driven seeding: read the reference's fundamental / decay / noisiness / brightness and
// build a modal body (+ a noise layer for noisy drums) starting CLOSE to the target, so the local
// optimizer only has to polish instead of climbing from an arbitrary seed. Undoable (one step).
def seed_from_ref() {
    if (empty(g_ref)) {
        g_opt_result = "load a reference first"
        return
    }
    let sf = analyze_seed(g_ref, SR)
    var mv = ModalVoice()
    mv.pitch = sf.fundamental > 20.0 ? clamp(sf.fundamental, 20.0, 4000.0) : 200.0
    mv.spread = clamp(sf.spread, 1.0, 2.6)
    mv.n_modes = clamp(sf.n_modes, 1, 8)
    mv.ring = clamp(sf.decay_ms / 1000.0 * 0.6, 0.005, 2.0)
    mv.brightness = clamp(sf.flatness + 0.3, 0.1, 0.95)
    mv.damp_skew = 0.7
    mv.transient = clamp((sf.crest - 2.0) / 12.0, 0.1, 0.9)
    mv.transient_rate = 800.0
    mv.echo_delay = 0.04
    mv.duration = clamp(sf.decay_ms / 1000.0 * 1.8, 0.05, 3.0)
    var layers : array<Layer>
    layers |> emplace(modal_layer("body", mv))
    if (sf.flatness > 0.12) {                       // noisy drum -> add a noise layer (wires / wash)
        var s = SfxrSound()
        s.wave = SfxrWave.noise
        s.base_freq = clamp(sf.centroid / 12000.0, 0.1, 0.9)
        s.env_attack = 0.0
        s.env_sustain = clamp(sf.decay_ms / 1000.0 * 0.1, 0.0, 0.3)
        s.env_punch = 0.2
        s.env_decay = clamp(sf.decay_ms / 1000.0 * 1.2, 0.02, 0.9)
        s.hpf_freq = clamp(sf.centroid / 12000.0 * 0.6, 0.0, 0.7)
        s.lpf_freq = 1.0
        s.sound_vol = 0.6
        var nl = sfxr_layer("noise", s)
        nl.gain = clamp(sf.flatness * 1.3, 0.1, 0.9)
        layers |> emplace(nl)
    }
    g_layers <- layers
    g_post = PostChain()      // fresh estimate: start with a clean post chain (don't inherit the previous drum's effects)
    g_post_reload = true
    g_sel = 0
    g_sel_loaded = -1
    g_mix_dirty = true
    g_lyr_dirty = true
    g_edit_dirty = true       // one undoable step (Cmd+Z restores the prior layers + post)
    recompute_fit()
    g_opt_result = "seeded: {int(mv.pitch)}Hz, {sf.flatness > 0.12 ? "body+noise" : "body"} (flat {int(sf.flatness * 100.0)}%)"
}

// ===== undo / redo =====

def snapshot_json() : string {
    var doc = Sound()
    doc.mimic_target = g_ref_path
    doc.layers := g_layers
    doc.post = g_post
    return sound_to_json(doc)
}

def restore_snapshot(j : string) {
    var doc = Sound()
    if (!sound_from_json(j, doc)) return
    g_layers := doc.layers
    g_post = doc.post
    g_post_reload = true
    g_sel = clamp(g_sel, 0, max(length(g_layers) - 1, 0))
    g_sel_loaded = -1
    g_mix_dirty = true
    g_lyr_dirty = true
}

def reset_history() {
    g_undo |> clear()
    g_undo_cursor = -1
    g_edit_dirty = false
    g_undo |> push(EditSnap(json = snapshot_json(), key = 0u))
    g_undo_cursor = 0
}

// record the current state; coalesce when the producing widget id matches the top entry
def record_edit(key : uint) {
    if (g_applying_history) return
    if (length(g_undo) > g_undo_cursor + 1) {   // drop the redo branch
        g_undo |> resize(g_undo_cursor + 1)
    }
    if (key != 0u && g_undo_cursor >= 0 && g_undo[g_undo_cursor].key == key) {
        g_undo[g_undo_cursor] = EditSnap(json = snapshot_json(), key = key)
    } else {
        g_undo |> push(EditSnap(json = snapshot_json(), key = key))
        if (length(g_undo) > UNDO_CAP) {
            g_undo |> erase(0)
        }
        g_undo_cursor = length(g_undo) - 1
    }
}

def do_undo() {
    if (g_undo_cursor <= 0) return
    g_undo_cursor--
    g_applying_history = true
    restore_snapshot(g_undo[g_undo_cursor].json)
    g_applying_history = false
}

def do_redo() {
    if (g_undo_cursor >= length(g_undo) - 1) return
    g_undo_cursor++
    g_applying_history = true
    restore_snapshot(g_undo[g_undo_cursor].json)
    g_applying_history = false
}

// ===== auto-tuner (annealed hill-climbing / (1+lambda) ES toward the fit metric) =====

let OPT_VOICE = 0      // selected layer's voice
let OPT_POST = 1       // enabled postprocess params
let OPT_COMBINED = 2   // all layers + enabled postprocess
var g_opt_scope = 0
var g_kv : array<float>    // working param vector
var g_klo : array<float>   // per-param lower bound
var g_khi : array<float>   // per-param upper bound
var g_opt_result = ""

def private orand(var seed : uint&) : float {
    seed = seed * 1664525u + 1013904223u
    return float(seed >> 8u) * (1.0 / 16777216.0)   // [0,1)
}

def private knob(writing : bool; var i : int&; var field : float&; lo, hi : float) {
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

// walk one layer's continuous voice params (wave / n_modes excluded — structural)
def private voice_knobs_io(writing : bool; li : int; var i : int&) {
    // layer gain is the user's mix decision — the optimizer matches timbre, never touches gain
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
        knob(writing, i, v.ring, 0.005, 2.0)
        knob(writing, i, v.brightness, 0.0, 1.0)
        knob(writing, i, v.damp_skew, 0.0, 1.5)
        knob(writing, i, v.transient, 0.0, 1.0)
        knob(writing, i, v.transient_rate, 100.0, 2000.0)
        knob(writing, i, v.echo_amount, 0.0, 1.0)
        knob(writing, i, v.echo_delay, 0.005, 0.15)
        knob(writing, i, v.duration, 0.05, 3.0)
        if (writing) {
            g_layers[li].voice = VoiceParams(modal = v)
        }
    }
}

// walk the enabled postprocess effects' continuous params (never toggles enabled)
def private post_knobs_io(writing : bool; var i : int&) {
    if (g_post.filter.enabled) {
        knob(writing, i, g_post.filter.position, 0.0, 1.0)
    }
    if (g_post.bandpass.enabled) {
        knob(writing, i, g_post.bandpass.freq, 20.0, 12000.0)
        knob(writing, i, g_post.bandpass.q, 0.1, 10.0)
    }
    if (g_post.drive.enabled) {
        knob(writing, i, g_post.drive.drive, 0.0, 8.0)
    }
    if (g_post.crush.enabled) {
        knob(writing, i, g_post.crush.bits, 1.0, 16.0)
    }
    if (g_post.comp.enabled) {
        knob(writing, i, g_post.comp.threshold, -60.0, 0.0)
        knob(writing, i, g_post.comp.ratio, 1.0, 20.0)
        knob(writing, i, g_post.comp.knee, 0.0, 24.0)
        knob(writing, i, g_post.comp.attack, 0.001, 0.1)
        knob(writing, i, g_post.comp.release, 0.01, 0.5)
    }
    if (g_post.chorus.enabled) {
        knob(writing, i, g_post.chorus.rate, 0.05, 8.0)
        knob(writing, i, g_post.chorus.depth, 0.0, 12.0)
        knob(writing, i, g_post.chorus.feedback, 0.0, 0.9)
        knob(writing, i, g_post.chorus.delay_ms, 1.0, 50.0)
        knob(writing, i, g_post.chorus.wet, 0.0, 1.0)
    }
    if (g_post.phaser.enabled) {
        knob(writing, i, g_post.phaser.rate, 0.05, 8.0)
        knob(writing, i, g_post.phaser.depth, 0.0, 1.0)
        knob(writing, i, g_post.phaser.center, 100.0, 5000.0)
        knob(writing, i, g_post.phaser.sweep, 0.0, 2400.0)
    }
    if (g_post.tremolo.enabled) {
        knob(writing, i, g_post.tremolo.rate, 0.1, 16.0)
        knob(writing, i, g_post.tremolo.depth, 0.0, 1.0)
    }
    if (g_post.delay.enabled) {
        knob(writing, i, g_post.delay.time, 0.01, 1.0)
        knob(writing, i, g_post.delay.feedback, 0.0, 0.95)
    }
    if (g_post.reverb.enabled) {
        knob(writing, i, g_post.reverb.wet, 0.0, 1.0)
        knob(writing, i, g_post.reverb.decay, 0.1, 3.0)
        knob(writing, i, g_post.reverb.lp_start, 2000.0, 18000.0)
        knob(writing, i, g_post.reverb.lp_end, 500.0, 8000.0)
    }
}

def private knobs_io(writing : bool) {
    var i = 0
    if (g_opt_scope == OPT_VOICE) {
        if (g_sel >= 0 && g_sel < length(g_layers)) {
            voice_knobs_io(writing, g_sel, i)
        }
    } elif (g_opt_scope == OPT_COMBINED) {
        for (li in range(length(g_layers))) {
            voice_knobs_io(writing, li, i)
        }
        post_knobs_io(writing, i)
    } else {
        post_knobs_io(writing, i)
    }
}

// render the current model and return the overall fit (the optimizer objective)
def private eval_fit() : float {
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
    knobs_io(false)   // gather current params + bounds
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
    knobs_io(true)   // apply the winner
    delete best
    g_mix_dirty = true
    g_edit_dirty = true   // one undoable entry for the whole optimize
    return best_fit
}

// fill the in-scope params with random values within their bounds (reuses the knob bounds)
def randomize_scope(scope : int) {
    g_opt_scope = scope
    g_kv |> clear()
    g_klo |> clear()
    g_khi |> clear()
    knobs_io(false)
    for (k in range(length(g_kv))) {
        g_kv[k] = g_klo[k] + orand(g_seed) * (g_khi[k] - g_klo[k])
    }
    knobs_io(true)
    g_mix_dirty = true
    g_edit_dirty = true
}

// add a fresh layer, random-init it, tune it alone, then refine the whole sound
def discover(modal : bool; iters : int) : float {
    if (modal) {
        var mv = ModalVoice()
        mv.n_modes = 3 + int(orand(g_seed) * 4.0)   // 3..6 partials so an inharmonic ladder can form
        add_layer(modal_layer("discover", mv))
    } else {
        add_layer(sfxr_layer("discover", SfxrSound()))
    }
    randomize_scope(OPT_VOICE)                        // random-init the new layer's timbre
    g_layers[g_sel].gain = 0.7                        // hold it audible (the optimizer won't touch gain)
    optimize(OPT_VOICE, max(iters, 1))               // fit its timbre to the residual on its own
    return optimize(OPT_COMBINED, max(iters / 2, 1))    // then refine all timbres together
}

def run_discover() {
    if (empty(g_ref)) {
        g_opt_result = "load a reference first"
        return
    }
    let before = eval_fit()
    let after = discover(true, OPT_ITERS.value)
    g_post_reload = true
    g_sel_loaded = -1
    g_opt_result = "discover: {int(before * 100.0)}% -> {int(after * 100.0)}%"
}

def run_optimize(scope : int) {
    if (empty(g_ref)) {
        g_opt_result = "load a reference first"
        return
    }
    let before = eval_fit()
    let after = optimize(scope, OPT_ITERS.value)
    if (after < 0.0) {
        g_opt_result = "nothing to tune (enable effects / select a layer)"
        return
    }
    g_post_reload = true
    g_sel_loaded = -1
    g_opt_result = "fit {int(before * 100.0)}% -> {int(after * 100.0)}%"
}

def rebuild_layer_preview() {
    if (g_sel < 0 || g_sel >= length(g_layers)) {
        g_lyr_wave |> clear()
        g_lyr_spec |> clear()
    } else {
        var b <- render_voice(g_layers[g_sel].voice)
        downsample(b, g_lyr_wave, 1200)
        compute_spectrum(b, g_lyr_spec)
        delete b
    }
    g_lyr_dirty = false
}

// ===== sfxr <-> sliders =====

def sfxr_to_sliders(s : SfxrSound) {
    WAVE_COMBO.value = int(s.wave)
    SL_ATTACK.value = s.env_attack
    SL_SUSTAIN.value = s.env_sustain
    SL_PUNCH.value = s.env_punch
    SL_DECAY.value = s.env_decay
    SL_BASEFREQ.value = s.base_freq
    SL_FREQLIMIT.value = s.freq_limit
    SL_FREQRAMP.value = s.freq_ramp
    SL_FREQDRAMP.value = s.freq_dramp
    SL_DUTY.value = s.duty
    SL_DUTYRAMP.value = s.duty_ramp
    SL_VIBSTR.value = s.vib_strength
    SL_VIBSPD.value = s.vib_speed
    SL_ARPMOD.value = s.arp_mod
    SL_ARPSPD.value = s.arp_speed
    SL_REPEAT.value = s.repeat_speed
    SL_PHAOFF.value = s.pha_offset
    SL_PHARAMP.value = s.pha_ramp
    SL_LPFFREQ.value = s.lpf_freq
    SL_LPFRAMP.value = s.lpf_ramp
    SL_LPFRES.value = s.lpf_resonance
    SL_HPFFREQ.value = s.hpf_freq
    SL_HPFRAMP.value = s.hpf_ramp
    SL_VOL.value = s.sound_vol
}

def sfxr_from_sliders() : SfxrSound {
    var s = SfxrSound()
    s.wave = sfxr_wave_of(WAVE_COMBO.value)
    s.env_attack = SL_ATTACK.value
    s.env_sustain = SL_SUSTAIN.value
    s.env_punch = SL_PUNCH.value
    s.env_decay = SL_DECAY.value
    s.base_freq = SL_BASEFREQ.value
    s.freq_limit = SL_FREQLIMIT.value
    s.freq_ramp = SL_FREQRAMP.value
    s.freq_dramp = SL_FREQDRAMP.value
    s.duty = SL_DUTY.value
    s.duty_ramp = SL_DUTYRAMP.value
    s.vib_strength = SL_VIBSTR.value
    s.vib_speed = SL_VIBSPD.value
    s.arp_mod = SL_ARPMOD.value
    s.arp_speed = SL_ARPSPD.value
    s.repeat_speed = SL_REPEAT.value
    s.pha_offset = SL_PHAOFF.value
    s.pha_ramp = SL_PHARAMP.value
    s.lpf_freq = SL_LPFFREQ.value
    s.lpf_ramp = SL_LPFRAMP.value
    s.lpf_resonance = SL_LPFRES.value
    s.hpf_freq = SL_HPFFREQ.value
    s.hpf_ramp = SL_HPFRAMP.value
    s.sound_vol = SL_VOL.value
    return s
}

// ===== modal <-> sliders =====

def modal_to_sliders(v : ModalVoice) {
    M_PITCH.value = v.pitch
    M_SPREAD.value = v.spread
    M_NMODES.value = v.n_modes
    M_RING.value = v.ring
    M_BRIGHT.value = v.brightness
    M_DAMP.value = v.damp_skew
    M_TRANS.value = v.transient
    M_TRATE.value = v.transient_rate
    M_ECHOAMT.value = v.echo_amount
    M_ECHODELAY.value = v.echo_delay
    M_DUR.value = v.duration
}

def modal_from_sliders() : ModalVoice {
    var v = ModalVoice()
    v.pitch = M_PITCH.value
    v.spread = M_SPREAD.value
    v.n_modes = M_NMODES.value
    v.ring = M_RING.value
    v.brightness = M_BRIGHT.value
    v.damp_skew = M_DAMP.value
    v.transient = M_TRANS.value
    v.transient_rate = M_TRATE.value
    v.echo_amount = M_ECHOAMT.value
    v.echo_delay = M_ECHODELAY.value
    v.duration = M_DUR.value
    return v
}

def load_editor(idx : int) {
    if (idx < 0 || idx >= length(g_layers)) return
    let lyr = g_layers[idx]
    GAIN.value = lyr.gain
    DELAY.value = lyr.delay
    LOCK.value = lyr.locked
    if (lyr.voice is sfxr) {
        sfxr_to_sliders(lyr.voice as sfxr)
    } else {
        modal_to_sliders(lyr.voice as modal)
    }
}

// ===== layer ops =====

def add_layer(lyr : Layer) {
    g_layers |> push(lyr)
    g_sel = length(g_layers) - 1
    g_sel_loaded = -1
    g_mix_dirty = true
    g_lyr_dirty = true
    g_edit_dirty = true
    g_play_pending = g_autoplay
}

def duplicate_layer(idx : int; linked : bool) {
    if (idx < 0 || idx >= length(g_layers)) return
    var copy = g_layers[idx]
    copy.name = "{g_layers[idx].name} copy"
    copy.delay = g_layers[idx].delay + 0.06
    copy.gain = g_layers[idx].gain * 0.8
    copy.locked = linked
    copy.link_source = linked ? idx : -1
    add_layer(copy)
}

def delete_layer(idx : int) {
    if (idx < 0 || idx >= length(g_layers)) return
    g_layers |> erase(idx)
    // any link pointing at/after the removed index is now stale -> unlink for safety
    for (lyr in g_layers) {
        if (lyr.link_source == idx) {
            lyr.link_source = -1
        } elif (lyr.link_source > idx) {
            lyr.link_source = lyr.link_source - 1
        }
    }
    g_sel = clamp(g_sel, 0, max(length(g_layers) - 1, 0))
    g_sel_loaded = -1
    g_mix_dirty = true
    g_lyr_dirty = true
    g_edit_dirty = true
}

def clear_all() {
    g_layers |> clear()
    g_sel = 0
    g_sel_loaded = -1
    g_mix_dirty = true
    g_lyr_dirty = true
    g_edit_dirty = true
}

// ===== panels =====

def layers_panel() {
    if (button(ADD_SFXR, (text = "+ sfxr"))) {
        var sd = g_seed
        add_layer(sfxr_layer("sfxr {length(g_layers) + 1}", sfxr_pickup_coin(sd)))
        g_seed = sd
    }
    same_line(TB1)
    if (button(ADD_MODAL, (text = "+ modal"))) {
        add_layer(modal_layer("modal {length(g_layers) + 1}", ModalVoice()))
    }
    same_line(TB2)
    if (icon_button(TB_DUP, (glyph = "duplicate", tip = "Duplicate selected layer"))) {
        duplicate_layer(g_sel, false)
    }
    same_line(TB3)
    if (icon_button(TB_DUPLINK, (glyph = "link", tip = "Duplicate linked (mirrors the source voice)"))) {
        duplicate_layer(g_sel, true)
    }
    same_line(TB4)
    if (icon_button(TB_DEL, (glyph = "delete", tip = "Delete selected layer"))) {
        delete_layer(g_sel)
    }
    separator(LSEP)
    for (i in range(length(g_layers))) {
        let kind = g_layers[i].voice is sfxr ? "sfxr" : "modal"
        var marks = ""
        if (!g_layers[i].enabled) { marks += " [mute]"; }
        if (g_layers[i].locked) { marks += " [lock]"; }
        if (g_layers[i].link_source >= 0) { marks += " [link]"; }
        if (g_layers[i].solo) { marks += " [S]"; }
        let lbl = "{g_layers[i].name}  ({kind}){marks}###L{i}"
        // limit the selectable to the name column so its hit-box doesn't sit under the row buttons
        let name_w = max(GetContentRegionAvail().x - 180.0, 80.0)
        if (selectable_label(LAYER_ROW[i], lbl, i == g_sel, ImGuiSelectableFlags.None, float2(name_w, 0.0))) {
            g_sel = i
        }
        same_line(ROW_SL1[i])
        if (icon_button(SOLO_ROW[i], (glyph = "solo", tip = "Solo this layer", size = 16.0f))) {
            g_layers[i].solo = !g_layers[i].solo
            g_mix_dirty = true
            g_edit_dirty = true
        }
        same_line(ROW_SL2[i])
        if (icon_button(MUTE_ROW[i], (glyph = g_layers[i].enabled ? "volume" : "mute", tip = g_layers[i].enabled ? "Mute this layer" : "Unmute this layer", size = 16.0f))) {
            g_layers[i].enabled = !g_layers[i].enabled
            g_mix_dirty = true
            g_edit_dirty = true
        }
        same_line(ROW_SL3[i])
        if (icon_button(PLAY_ROW[i], (glyph = "play", tip = "Play this layer", size = 16.0f))) {
            play_layer(i)
        }
    }
}

def editor_panel() {
    if (empty(g_layers)) {
        text(NOLAYERS, (text = "No layers. Add an sfxr or modal voice from the Layers panel."))
        return
    }
    if (g_sel != g_sel_loaded) {
        load_editor(g_sel)
        g_sel_loaded = g_sel
    }
    let locked = g_layers[g_sel].locked

    text(EDIT_NAME, (text = "editing: {g_layers[g_sel].name}"))
    // gain + delay are ALWAYS editable (even when locked)
    GAIN.bounds = (0.0f, 2.0f)
    if (slider_float(GAIN, (text = "gain"))) {
        g_layers[g_sel].gain = GAIN.value
        g_mix_dirty = true
        g_edit_dirty = true
    }
    DELAY.bounds = (0.0f, 1.0f)
    if (slider_float(DELAY, (text = "onset delay (s)"))) {
        g_layers[g_sel].delay = DELAY.value
        g_mix_dirty = true
        g_edit_dirty = true
    }
    if (checkbox(LOCK, (text = "locked (voice read-only)"))) {
        g_layers[g_sel].locked = LOCK.value
        g_edit_dirty = true
    }
    same_line(EDIT_SL)
    if (icon_button(EDITPLAY, (glyph = "play", tip = "Play this layer"))) {
        play_layer(g_sel)
    }
    separator(ESEP)

    if (locked) {
        let src = g_layers[g_sel].link_source
        text(LOCK_NOTE, (text = src >= 0 ? "locked — mirrors layer {src}; unlock to edit the voice" : "locked — unlock to edit the voice"))
    } else {
        var changed = false
        if (g_layers[g_sel].voice is sfxr) {
            changed = sfxr_editor() || changed
            if (changed) {
                g_layers[g_sel].voice = VoiceParams(sfxr = sfxr_from_sliders())
            }
        } else {
            changed = modal_editor() || changed
            if (changed) {
                g_layers[g_sel].voice = VoiceParams(modal = modal_from_sliders())
            }
        }
        if (changed) {
            g_mix_dirty = true
            g_lyr_dirty = true
            g_edit_dirty = true
        }
    }

    collapsing_header(PREVIEW_H, (text = "layer preview", closable = false, flags = ImGuiTreeNodeFlags.None)) {
        plot(PV_WAVE, (title = "layer waveform", size = float2(-1.0f, 120.0f), flags = ImPlotFlags.None)) {
            setup_axes("sample", "amp", ImPlotAxisFlags.AutoFit, ImPlotAxisFlags.AutoFit)
            setup_legend(ImPlotLocation.NorthEast)
            plot_line("layer", g_lyr_wave)
        }
        plot(PV_SPEC, (title = "layer spectrum (dB)", size = float2(-1.0f, 120.0f), flags = ImPlotFlags.None)) {
            setup_axes("bin", "dB", ImPlotAxisFlags.AutoFit, ImPlotAxisFlags.AutoFit)
            setup_legend(ImPlotLocation.NorthEast)
            plot_line("layer", g_lyr_spec)
        }
    }
}

def private wave_icon(idx : int) : string {
    // map the wave combo index to its oscillator glyph
    if (idx == 1) { return "wave-saw" }
    elif (idx == 2) { return "wave-sine" }
    elif (idx == 3) { return "wave-noise" }
    elif (idx == 4) { return "wave-triangle" }
    return "wave-square"
}

def sfxr_editor() : bool {
    var ch = false
    icon(wave_icon(WAVE_COMBO.value), 18.0f)
    same_line()
    if (combo(WAVE_COMBO, (text = "wave", items <- ["square", "sawtooth", "sine", "noise", "triangle"]))) { ch = true; }
    collapsing_header(H_ENV, (text = "Envelope", closable = false, flags = ImGuiTreeNodeFlags.DefaultOpen)) {
        SL_ATTACK.bounds = (0.0f, 1.0f)
        if (slider_float(SL_ATTACK, (text = "attack"))) { ch = true; }
        SL_SUSTAIN.bounds = (0.0f, 1.0f)
        if (slider_float(SL_SUSTAIN, (text = "sustain"))) { ch = true; }
        SL_PUNCH.bounds = (0.0f, 1.0f)
        if (slider_float(SL_PUNCH, (text = "punch"))) { ch = true; }
        SL_DECAY.bounds = (0.0f, 1.0f)
        if (slider_float(SL_DECAY, (text = "decay"))) { ch = true; }
    }
    collapsing_header(H_FREQ, (text = "Frequency", closable = false, flags = ImGuiTreeNodeFlags.DefaultOpen)) {
        SL_BASEFREQ.bounds = (0.0f, 1.0f)
        if (slider_float(SL_BASEFREQ, (text = "base freq"))) { ch = true; }
        SL_FREQLIMIT.bounds = (0.0f, 1.0f)
        if (slider_float(SL_FREQLIMIT, (text = "freq limit"))) { ch = true; }
        SL_FREQRAMP.bounds = (-1.0f, 1.0f)
        if (slider_float(SL_FREQRAMP, (text = "freq ramp"))) { ch = true; }
        SL_FREQDRAMP.bounds = (-1.0f, 1.0f)
        if (slider_float(SL_FREQDRAMP, (text = "freq dramp"))) { ch = true; }
    }
    collapsing_header(H_TONE, (text = "Tone / modulation", closable = false, flags = ImGuiTreeNodeFlags.None)) {
        SL_DUTY.bounds = (0.0f, 1.0f)
        if (slider_float(SL_DUTY, (text = "duty"))) { ch = true; }
        SL_DUTYRAMP.bounds = (-1.0f, 1.0f)
        if (slider_float(SL_DUTYRAMP, (text = "duty ramp"))) { ch = true; }
        SL_VIBSTR.bounds = (0.0f, 1.0f)
        if (slider_float(SL_VIBSTR, (text = "vibrato depth"))) { ch = true; }
        SL_VIBSPD.bounds = (0.0f, 1.0f)
        if (slider_float(SL_VIBSPD, (text = "vibrato speed"))) { ch = true; }
        SL_ARPMOD.bounds = (-1.0f, 1.0f)
        if (slider_float(SL_ARPMOD, (text = "arp mod"))) { ch = true; }
        SL_ARPSPD.bounds = (0.0f, 1.0f)
        if (slider_float(SL_ARPSPD, (text = "arp speed"))) { ch = true; }
        SL_REPEAT.bounds = (0.0f, 1.0f)
        if (slider_float(SL_REPEAT, (text = "repeat speed"))) { ch = true; }
        SL_PHAOFF.bounds = (-1.0f, 1.0f)
        if (slider_float(SL_PHAOFF, (text = "phaser offset"))) { ch = true; }
        SL_PHARAMP.bounds = (-1.0f, 1.0f)
        if (slider_float(SL_PHARAMP, (text = "phaser ramp"))) { ch = true; }
    }
    collapsing_header(H_FILTER, (text = "Filter", closable = false, flags = ImGuiTreeNodeFlags.None)) {
        SL_LPFFREQ.bounds = (0.0f, 1.0f)
        if (slider_float(SL_LPFFREQ, (text = "lpf cutoff"))) { ch = true; }
        SL_LPFRAMP.bounds = (-1.0f, 1.0f)
        if (slider_float(SL_LPFRAMP, (text = "lpf ramp"))) { ch = true; }
        SL_LPFRES.bounds = (-1.0f, 1.0f)
        if (slider_float(SL_LPFRES, (text = "lpf resonance"))) { ch = true; }
        SL_HPFFREQ.bounds = (0.0f, 1.0f)
        if (slider_float(SL_HPFFREQ, (text = "hpf cutoff"))) { ch = true; }
        SL_HPFRAMP.bounds = (-1.0f, 1.0f)
        if (slider_float(SL_HPFRAMP, (text = "hpf ramp"))) { ch = true; }
    }
    SL_VOL.bounds = (0.0f, 1.0f)
    if (slider_float(SL_VOL, (text = "voice volume"))) { ch = true; }
    return ch
}

def modal_editor() : bool {
    var ch = false
    M_PITCH.bounds = (60.0f, 4000.0f)
    if (slider_float(M_PITCH, (text = "pitch (Hz)"))) { ch = true; }
    M_SPREAD.bounds = (1.0f, 2.6f)
    if (slider_float(M_SPREAD, (text = "spread"))) { ch = true; }
    M_NMODES.bounds = (1, 8)
    if (slider_int(M_NMODES, (text = "modes"))) { ch = true; }
    M_RING.bounds = (0.005f, 2.0f)
    if (slider_float(M_RING, (text = "ring (s)"))) { ch = true; }
    M_BRIGHT.bounds = (0.0f, 1.0f)
    if (slider_float(M_BRIGHT, (text = "brightness"))) { ch = true; }
    M_DAMP.bounds = (0.0f, 1.5f)
    if (slider_float(M_DAMP, (text = "damp skew"))) { ch = true; }
    M_TRANS.bounds = (0.0f, 1.0f)
    if (slider_float(M_TRANS, (text = "transient"))) { ch = true; }
    M_TRATE.bounds = (100.0f, 2000.0f)
    if (slider_float(M_TRATE, (text = "transient rate"))) { ch = true; }
    M_ECHOAMT.bounds = (0.0f, 1.0f)
    if (slider_float(M_ECHOAMT, (text = "echo amount"))) { ch = true; }
    M_ECHODELAY.bounds = (0.005f, 0.15f)
    if (slider_float(M_ECHODELAY, (text = "echo delay (s)"))) { ch = true; }
    M_DUR.bounds = (0.05f, 3.0f)
    if (slider_float(M_DUR, (text = "duration (s)"))) { ch = true; }
    return ch
}

// shared play button + fit readout — rendered at the top of both the Mix and Spectrogram tabs
def mix_header() {
    if (icon_button(MIX_PLAY, (glyph = "play", tip = "Play the mix"))) {
        play_mix()
    }
    same_line(MIX_SL)
    if (g_have_fit) {
        text(FIT_TXT, (text = "fit {int(g_fit.overall * 100.0)}%   (timbre {int(g_fit.spectral * 100.0)}%  pitch {int(g_fit.pitch * 100.0)}%  texture {int(g_fit.texture * 100.0)}%  shape {int(g_fit.shape * 100.0)}%  attack {int(g_fit.attack * 100.0)}%)"))
        let mdb = 20.0 * log(max(g_match_ratio, 1.0e-6)) * 0.43429448190325176
        let mtxt = g_match_loudness ? "matched {int(mdb)} dB to reference (audition + export)" : "raw level (audition + export)"
        text(FIT_TXT2, (text = "ref: {int(g_ref_feat.centroid)}Hz / {int(g_ref_feat.decay_ms)}ms    mix: {int(g_mix_feat.centroid)}Hz / {int(g_mix_feat.decay_ms)}ms   (centroid / decay)    [{mtxt}]"))
    } else {
        text(FIT_TXT, (text = "fit: load a reference to compare"))
    }
}

// shared opt-button row (only meaningful once a reference is loaded)
def opt_row() {
    if (button(OPT_SEED_BTN, (text = "seed"))) {
        seed_from_ref()       // analysis-seed the layers from the reference (start close)
    }
    same_line(OPT_SL0)
    if (button(OPT_VOICE_BTN, (text = "opt voice"))) {
        run_optimize(OPT_VOICE)
    }
    same_line(OPT_SL1)
    if (button(OPT_FX_BTN, (text = "opt fx"))) {
        run_optimize(OPT_POST)
    }
    same_line(OPT_SL2)
    if (button(OPT_ALL_BTN, (text = "opt all"))) {
        run_optimize(OPT_COMBINED)
    }
    same_line(OPT_SL3)
    if (button(OPT_DISC_BTN, (text = "+ discover modal"))) {
        run_discover()
    }
    same_line(OPT_SL4)
    OPT_ITERS.bounds = (4, 80)
    SetNextItemWidth(120.0f)   // compact, so the "iters" label isn't pushed off the panel edge
    slider_int(OPT_ITERS, (text = "iters"))
    if (!empty(g_opt_result)) {
        text(OPT_RESULT, (text = g_opt_result))
    }
}

// Unified preview: shared play + fit + opt-row on top, the different views in tabs underneath
// (waveform/spectrum vs spectrum-over-time spectrogram). Replaces the separate Mix + Spectrogram
// docks, which duplicated the play button and fit readout.
def preview_panel() {
    input_text(SOUND_COMMENT, (text = "label"))   // freeform caption at the very top (which variant this is)
    mix_header()
    if (g_have_fit) {
        opt_row()
    }
    tab_bar(PV_TABS, (text = "preview", flags = ImGuiTabBarFlags.None)) {
        tab_item(PV_TAB_WAVE, (text = "Wave + Spectrum", closable = false, flags = ImGuiTabItemFlags.None)) {
            plot(MIX_WAVE, (title = "mix waveform", size = float2(-1.0f, 150.0f), flags = ImPlotFlags.None)) {
                setup_axes("sample", "amp", ImPlotAxisFlags.AutoFit, ImPlotAxisFlags.AutoFit)
                setup_legend(ImPlotLocation.NorthEast)   // top-right: keep the legend off the onset (left)
                plot_line("mix", g_mix_wave)
                if (g_show_ref && !empty(g_ref_wave)) {
                    plot_line("reference", g_ref_wave)
                }
            }
            plot(MIX_SPEC, (title = "mix spectrum (dB)", size = float2(-1.0f, 150.0f), flags = ImPlotFlags.None)) {
                setup_axes("bin", "dB", ImPlotAxisFlags.AutoFit, ImPlotAxisFlags.AutoFit)
                setup_legend(ImPlotLocation.NorthEast)
                plot_line("mix", g_mix_spec)
                if (g_show_ref && !empty(g_ref_spec)) {
                    plot_line("reference", g_ref_spec)
                }
            }
        }
        tab_item(PV_TAB_SPECTRO, (text = "Spectrogram", closable = false, flags = ImGuiTabItemFlags.None)) {
            plot(SPEC_MIX, (title = "mix - spectrum over time", size = float2(-1.0f, 150.0f), flags = ImPlotFlags.None)) {
                setup_axes("time", "freq (log)", ImPlotAxisFlags.AutoFit, ImPlotAxisFlags.AutoFit)
                setup_legend(ImPlotLocation.NorthEast)
                if (!empty(g_mix_spectro)) {
                    plot_heatmap("mix", g_mix_spectro, SPEC_BINS, SPEC_FRAMES, -90.0lf, -20.0lf, "")
                }
            }
            plot(SPEC_REF, (title = "reference - spectrum over time", size = float2(-1.0f, 150.0f), flags = ImPlotFlags.None)) {
                setup_axes("time", "freq (log)", ImPlotAxisFlags.AutoFit, ImPlotAxisFlags.AutoFit)
                setup_legend(ImPlotLocation.NorthEast)
                if (!empty(g_ref_spectro)) {
                    plot_heatmap("ref", g_ref_spectro, SPEC_BINS, SPEC_FRAMES, -90.0lf, -20.0lf, "")
                }
            }
        }
    }
}

// push g_post -> the postprocess widget states (called when the chain changes externally)
def sync_post_to_sliders() {
    PFX_FILT_EN.value = g_post.filter.enabled
    PFX_FILT_POS.value = g_post.filter.position
    PFX_BP_EN.value = g_post.bandpass.enabled
    PFX_BP_FREQ.value = g_post.bandpass.freq
    PFX_BP_Q.value = g_post.bandpass.q
    PFX_DR_EN.value = g_post.drive.enabled
    PFX_DR_AMT.value = g_post.drive.drive
    PFX_CR_EN.value = g_post.crush.enabled
    PFX_CR_BITS.value = g_post.crush.bits
    PFX_CR_COARSE.value = g_post.crush.coarse
    PFX_CMP_EN.value = g_post.comp.enabled
    PFX_CMP_THR.value = g_post.comp.threshold
    PFX_CMP_RATIO.value = g_post.comp.ratio
    PFX_CH_EN.value = g_post.chorus.enabled
    PFX_CH_RATE.value = g_post.chorus.rate
    PFX_CH_DEPTH.value = g_post.chorus.depth
    PFX_CH_WET.value = g_post.chorus.wet
    PFX_PH_EN.value = g_post.phaser.enabled
    PFX_PH_RATE.value = g_post.phaser.rate
    PFX_PH_DEPTH.value = g_post.phaser.depth
    PFX_TR_EN.value = g_post.tremolo.enabled
    PFX_TR_RATE.value = g_post.tremolo.rate
    PFX_TR_DEPTH.value = g_post.tremolo.depth
    PFX_DL_EN.value = g_post.delay.enabled
    PFX_DL_TIME.value = g_post.delay.time
    PFX_DL_FB.value = g_post.delay.feedback
    PFX_RV_EN.value = g_post.reverb.enabled
    PFX_RV_WET.value = g_post.reverb.wet
    PFX_RV_DECAY.value = g_post.reverb.decay
}

def postprocess_panel() {
    if (g_post_reload) {
        sync_post_to_sliders()
        g_post_reload = false
    }
    var ch = false
    text(PFX_HINT, (text = "effects run on the mix before analysis; all off by default"))
    collapsing_header(PFX_FILT_H, (text = "Filter (DJ)", closable = false, flags = ImGuiTreeNodeFlags.None)) {
        if (checkbox(PFX_FILT_EN, (text = "enable###pfilt"))) { g_post.filter.enabled = PFX_FILT_EN.value; ch = true; }
        PFX_FILT_POS.bounds = (0.0f, 1.0f)
        if (slider_float(PFX_FILT_POS, (text = "position (LPF<->HPF)"))) { g_post.filter.position = PFX_FILT_POS.value; ch = true; }
    }
    collapsing_header(PFX_BP_H, (text = "Bandpass", closable = false, flags = ImGuiTreeNodeFlags.None)) {
        if (checkbox(PFX_BP_EN, (text = "enable###pbp"))) { g_post.bandpass.enabled = PFX_BP_EN.value; ch = true; }
        PFX_BP_FREQ.bounds = (20.0f, 12000.0f)
        if (slider_float(PFX_BP_FREQ, (text = "freq (Hz)"))) { g_post.bandpass.freq = PFX_BP_FREQ.value; ch = true; }
        PFX_BP_Q.bounds = (0.1f, 10.0f)
        if (slider_float(PFX_BP_Q, (text = "Q"))) { g_post.bandpass.q = PFX_BP_Q.value; ch = true; }
    }
    collapsing_header(PFX_DR_H, (text = "Drive (waveshaper)", closable = false, flags = ImGuiTreeNodeFlags.None)) {
        if (checkbox(PFX_DR_EN, (text = "enable###pdr"))) { g_post.drive.enabled = PFX_DR_EN.value; ch = true; }
        PFX_DR_AMT.bounds = (0.0f, 8.0f)
        if (slider_float(PFX_DR_AMT, (text = "drive"))) { g_post.drive.drive = PFX_DR_AMT.value; ch = true; }
    }
    collapsing_header(PFX_CR_H, (text = "Bitcrush", closable = false, flags = ImGuiTreeNodeFlags.None)) {
        if (checkbox(PFX_CR_EN, (text = "enable###pcr"))) { g_post.crush.enabled = PFX_CR_EN.value; ch = true; }
        PFX_CR_BITS.bounds = (1.0f, 16.0f)
        if (slider_float(PFX_CR_BITS, (text = "bits"))) { g_post.crush.bits = PFX_CR_BITS.value; ch = true; }
        PFX_CR_COARSE.bounds = (0, 16)
        if (slider_int(PFX_CR_COARSE, (text = "downsample"))) { g_post.crush.coarse = PFX_CR_COARSE.value; ch = true; }
    }
    collapsing_header(PFX_CMP_H, (text = "Compressor", closable = false, flags = ImGuiTreeNodeFlags.None)) {
        if (checkbox(PFX_CMP_EN, (text = "enable###pcmp"))) { g_post.comp.enabled = PFX_CMP_EN.value; ch = true; }
        PFX_CMP_THR.bounds = (-60.0f, 0.0f)
        if (slider_float(PFX_CMP_THR, (text = "threshold (dB)"))) { g_post.comp.threshold = PFX_CMP_THR.value; ch = true; }
        PFX_CMP_RATIO.bounds = (1.0f, 20.0f)
        if (slider_float(PFX_CMP_RATIO, (text = "ratio"))) { g_post.comp.ratio = PFX_CMP_RATIO.value; ch = true; }
    }
    collapsing_header(PFX_CH_H, (text = "Chorus", closable = false, flags = ImGuiTreeNodeFlags.None)) {
        if (checkbox(PFX_CH_EN, (text = "enable###pch"))) { g_post.chorus.enabled = PFX_CH_EN.value; ch = true; }
        PFX_CH_RATE.bounds = (0.05f, 8.0f)
        if (slider_float(PFX_CH_RATE, (text = "rate (Hz)"))) { g_post.chorus.rate = PFX_CH_RATE.value; ch = true; }
        PFX_CH_DEPTH.bounds = (0.0f, 12.0f)
        if (slider_float(PFX_CH_DEPTH, (text = "depth (ms)"))) { g_post.chorus.depth = PFX_CH_DEPTH.value; ch = true; }
        PFX_CH_WET.bounds = (0.0f, 1.0f)
        if (slider_float(PFX_CH_WET, (text = "wet"))) { g_post.chorus.wet = PFX_CH_WET.value; ch = true; }
    }
    collapsing_header(PFX_PH_H, (text = "Phaser", closable = false, flags = ImGuiTreeNodeFlags.None)) {
        if (checkbox(PFX_PH_EN, (text = "enable###pph"))) { g_post.phaser.enabled = PFX_PH_EN.value; ch = true; }
        PFX_PH_RATE.bounds = (0.05f, 8.0f)
        if (slider_float(PFX_PH_RATE, (text = "rate (Hz)"))) { g_post.phaser.rate = PFX_PH_RATE.value; ch = true; }
        PFX_PH_DEPTH.bounds = (0.0f, 1.0f)
        if (slider_float(PFX_PH_DEPTH, (text = "depth"))) { g_post.phaser.depth = PFX_PH_DEPTH.value; ch = true; }
    }
    collapsing_header(PFX_TR_H, (text = "Tremolo", closable = false, flags = ImGuiTreeNodeFlags.None)) {
        if (checkbox(PFX_TR_EN, (text = "enable###ptr"))) { g_post.tremolo.enabled = PFX_TR_EN.value; ch = true; }
        PFX_TR_RATE.bounds = (0.1f, 16.0f)
        if (slider_float(PFX_TR_RATE, (text = "rate (Hz)"))) { g_post.tremolo.rate = PFX_TR_RATE.value; ch = true; }
        PFX_TR_DEPTH.bounds = (0.0f, 1.0f)
        if (slider_float(PFX_TR_DEPTH, (text = "depth"))) { g_post.tremolo.depth = PFX_TR_DEPTH.value; ch = true; }
    }
    collapsing_header(PFX_DL_H, (text = "Delay", closable = false, flags = ImGuiTreeNodeFlags.None)) {
        if (checkbox(PFX_DL_EN, (text = "enable###pdl"))) { g_post.delay.enabled = PFX_DL_EN.value; ch = true; }
        PFX_DL_TIME.bounds = (0.01f, 1.0f)
        if (slider_float(PFX_DL_TIME, (text = "time (s)"))) { g_post.delay.time = PFX_DL_TIME.value; ch = true; }
        PFX_DL_FB.bounds = (0.0f, 0.95f)
        if (slider_float(PFX_DL_FB, (text = "feedback"))) { g_post.delay.feedback = PFX_DL_FB.value; ch = true; }
    }
    collapsing_header(PFX_RV_H, (text = "Reverb (room)", closable = false, flags = ImGuiTreeNodeFlags.None)) {
        if (checkbox(PFX_RV_EN, (text = "enable###prv"))) { g_post.reverb.enabled = PFX_RV_EN.value; ch = true; }
        PFX_RV_WET.bounds = (0.0f, 1.0f)
        if (slider_float(PFX_RV_WET, (text = "wet"))) { g_post.reverb.wet = PFX_RV_WET.value; ch = true; }
        PFX_RV_DECAY.bounds = (0.1f, 3.0f)
        if (slider_float(PFX_RV_DECAY, (text = "decay (s)"))) { g_post.reverb.decay = PFX_RV_DECAY.value; ch = true; }
    }
    if (ch) {
        g_mix_dirty = true
        g_edit_dirty = true
    }
}

def reference_panel() {
    if (icon_button(REFLOAD, (glyph = "import", tip = "Load a reference wav to mimic"))) {
        g_dialog_mode = DLG_REF
        file_dialog_open(FileDialogConfig(title = "Load reference audio", start_dir = "",
            filters <- file_filters("Audio (*.wav)|wav|All files|"), mode = FileDialogMode.open, max_selection = 0))
    }
    same_line(REF_SL1)
    if (icon_button(REFPLAY, (glyph = "play", tip = "Play the reference"))) {
        play_buf_copy(g_ref)
    }
    same_line(REF_SL2)
    checkbox(REF_SHOW, (text = "overlay on mix"))
    g_show_ref = REF_SHOW.value
    text(REF_NAME, (text = g_ref_name))
    plot(REF_WAVE, (title = "reference waveform", size = float2(-1.0f, 120.0f), flags = ImPlotFlags.None)) {
        setup_axes("sample", "amp", ImPlotAxisFlags.AutoFit, ImPlotAxisFlags.AutoFit)
        setup_legend(ImPlotLocation.NorthEast)
        plot_line("ref", g_ref_wave)
    }
    plot(REF_SPEC, (title = "reference spectrum (dB)", size = float2(-1.0f, 120.0f), flags = ImPlotFlags.None)) {
        setup_axes("bin", "dB", ImPlotAxisFlags.AutoFit, ImPlotAxisFlags.AutoFit)
        setup_legend(ImPlotLocation.NorthEast)
        plot_line("ref", g_ref_spec)
    }
}

def setup_default_layout(dock_id : uint) {
    DockBuilderRemoveNode(dock_id)
    DockBuilderAddDockSpaceNode(dock_id, ImGuiDockNodeFlags.None)
    let vp = GetMainViewport()
    DockBuilderSetNodeSize(dock_id, ImVec2(vp.Size.x, vp.Size.y))
    var layers_id : uint = 0u
    var rest_id : uint = 0u
    DockBuilderSplitNode(dock_id, ImGuiDir.Up, 0.16f, layers_id, rest_id)
    var mix_id : uint = 0u
    var mid_id : uint = 0u
    DockBuilderSplitNode(rest_id, ImGuiDir.Down, 0.38f, mix_id, mid_id)
    var editor_id : uint = 0u
    var ref_id : uint = 0u
    DockBuilderSplitNode(mid_id, ImGuiDir.Left, 0.60f, editor_id, ref_id)
    DockBuilderDockWindow("Layers", layers_id)
    DockBuilderDockWindow("Editor", editor_id)
    DockBuilderDockWindow("Reference", ref_id)
    DockBuilderDockWindow("Preview", mix_id)
    DockBuilderDockWindow("Postprocess", editor_id)
    DockBuilderFinish(dock_id)
}

def start_app() {
    g_started = true
    MB_AUTO.value = true
    g_autoplay = true
    MB_MATCH.value = true
    REF_SHOW.value = true
    // seed a starter sound: the ball-on-table (thud + click), so the lab opens on a real multi-layer sound
    add_layer(modal_layer("table thud", ModalVoice(pitch = 170.0, spread = 2.4, n_modes = 2, ring = 0.05, brightness = 0.35, transient = 0.2, duration = 0.5)))
    add_layer(modal_layer("ball click", ModalVoice(pitch = 1500.0, spread = 1.6, n_modes = 4, ring = 0.05, brightness = 0.55, damp_skew = 0.9, transient = 0.6, echo_amount = 0.32, echo_delay = 0.055, duration = 0.5)))
    g_sel = 0
    g_play_pending = false
    OPT_ITERS.value = 16
    reset_history()
}

[export]
def init() {
    harness_init("SFX Lab — multi-layer", 1180, 860)
    var io & = unsafe(GetIO())
    io.ConfigFlags |= ImGuiConfigFlags.DockingEnable
    g_ctx = implot::CreateContext()
    g_audio = audio_system_create()
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    if (!g_started) {
        start_app()
    }

    main_menu_bar(MENUBAR) {
        menu(FILE_MENU, "File", true) {
            if (menu_item(MI_NEW, (text = "New (clear layers)"))) {
                clear_all()
                MI_NEW.value = false
            }
            if (menu_item(MI_OPEN, (text = "Open sound..."))) {
                MI_OPEN.value = false
                g_dialog_mode = DLG_OPEN
                file_dialog_open(FileDialogConfig(title = "Open sound", start_dir = "",
                    filters <- file_filters("Sound (*.sfx)|sfx|All files|"), mode = FileDialogMode.open, max_selection = 0))
            }
            if (menu_item(MI_SAVE, (text = "Save sound..."))) {
                MI_SAVE.value = false
                g_dialog_mode = DLG_SAVE
                file_dialog_open(FileDialogConfig(title = "Save sound", start_dir = "",
                    filters <- file_filters("Sound (*.sfx)|sfx|All files|"), mode = FileDialogMode.save, max_selection = 1, default_name = "untitled.sfx"))
            }
            if (menu_item(MI_IMP_VOICE, (text = "Import voice..."))) {
                MI_IMP_VOICE.value = false
                g_dialog_mode = DLG_IMP_VOICE
                file_dialog_open(FileDialogConfig(title = "Import voice", start_dir = "",
                    filters <- file_filters("Voice (*.voice)|voice|All files|"), mode = FileDialogMode.open, max_selection = 0))
            }
            if (menu_item(MI_EXP_VOICE, (text = "Export selected voice..."))) {
                MI_EXP_VOICE.value = false
                g_dialog_mode = DLG_EXP_VOICE
                file_dialog_open(FileDialogConfig(title = "Export voice", start_dir = "",
                    filters <- file_filters("Voice (*.voice)|voice|All files|"), mode = FileDialogMode.save, max_selection = 1, default_name = "untitled.voice"))
            }
            if (menu_item(MI_EXP_DAS, (text = "Export .das render function..."))) {
                MI_EXP_DAS.value = false
                g_dialog_mode = DLG_EXP_DAS
                file_dialog_open(FileDialogConfig(title = "Export .das (filename = function name)", start_dir = "",
                    filters <- file_filters("daslang (*.das)|das|All files|"), mode = FileDialogMode.save, max_selection = 1, default_name = "render_sound.das"))
            }
        }
        menu(EDIT_MENU, "Edit", true) {
            if (menu_item(MI_UNDO, (text = "Undo  (Cmd+Z)"))) {
                do_undo()
                MI_UNDO.value = false
            }
            if (menu_item(MI_REDO, (text = "Redo  (Cmd+Shift+Z)"))) {
                do_redo()
                MI_REDO.value = false
            }
        }
        menu(PLAY_MENU, "Play", true) {
            if (menu_item(MI_PLAY, (text = "Play mix"))) {
                play_mix()
                MI_PLAY.value = false
            }
            checkbox(MB_AUTO, (text = "autoplay"))
            checkbox(MB_MATCH, (text = "match reference loudness"))
        }
    }
    g_autoplay = MB_AUTO.value
    if (MB_MATCH.value != g_match_loudness) {   // toggling match must re-level immediately
        g_match_loudness = MB_MATCH.value
        g_mix_dirty = true
    }

    // keyboard: Cmd+Z undo, Cmd+Shift+Z redo (skip while a text field has focus)
    let io & = unsafe(GetIO())
    if (!io.WantTextInput && io.KeySuper && IsKeyPressed(ImGuiKey.Z, false)) {
        if (io.KeyShift) {
            do_redo()
        } else {
            do_undo()
        }
    }

    dockspace(DOCK_ROOT, (flags = ImGuiDockNodeFlags.PassthruCentralNode)) {
        if (!DOCK_ROOT.has_initial_layout && DOCK_ROOT.dock_id != 0u) {
            setup_default_layout(DOCK_ROOT.dock_id)
            DOCK_ROOT.has_initial_layout = true
        }
        dock_window(LAYERS, (text = "Layers", closable = false, flags = ImGuiWindowFlags.None)) {
            layers_panel()
        }
        dock_window(EDITOR, (text = "Editor", closable = false, flags = ImGuiWindowFlags.None)) {
            editor_panel()
        }
        dock_window(REFERENCE, (text = "Reference", closable = false, flags = ImGuiWindowFlags.None)) {
            reference_panel()
        }
        dock_window(PREVIEW, (text = "Preview", closable = false, flags = ImGuiWindowFlags.None)) {
            preview_panel()
        }
        dock_window(POSTPROC, (text = "Postprocess", closable = false, flags = ImGuiWindowFlags.None)) {
            postprocess_panel()
        }
    }

    // record one undo entry per frame that had a user edit; GetActiveID() coalesces slider drags
    if (g_edit_dirty) {
        record_edit(GetActiveID())
        g_edit_dirty = false
    }

    // the file dialog draws at top level (outside any window) so its path stays FILEDLG/...
    let fdr = file_dialog_draw()
    if (fdr == FileDialogResult.confirmed) {
        let picked = file_dialog_selected_path()
        if (g_dialog_mode == DLG_REF) {
            load_reference(picked)
        } elif (g_dialog_mode == DLG_OPEN) {
            do_open_sound(picked)
        } elif (g_dialog_mode == DLG_SAVE) {
            do_save_sound(picked)
        } elif (g_dialog_mode == DLG_EXP_VOICE) {
            do_export_voice(picked)
        } elif (g_dialog_mode == DLG_IMP_VOICE) {
            do_import_voice(picked)
        } elif (g_dialog_mode == DLG_EXP_DAS) {
            do_export_das(picked)
        }
        g_dialog_mode = DLG_NONE
    }

    if (g_mix_dirty) {
        rebuild_mix()
        if (g_play_pending) {
            play_buf_copy(g_mix)
            g_play_pending = false
        }
    }
    if (g_lyr_dirty) {
        rebuild_layer_preview()
    }

    harness_end_frame()
}

[export]
def shutdown() {
    audio_system_finalize(g_audio.command, g_audio.next_sid)
    if (g_ctx != null) {
        DestroyContext(g_ctx)
    }
    harness_shutdown()
}

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}

// ===== live commands (so the signal is observable headlessly, not just on screen) =====

struct SpecPeak {
    hz : float
    db : float
}

struct SfxReport {
    ref_loaded : bool
    ref_name   : string
    ref        : SfxFeatures
    mix        : SfxFeatures
    fit        : FitScore
    ref_spec   : array<float>      // ~48-pt downsampled dB spectrum (broad shape)
    mix_spec   : array<float>
    ref_peaks  : array<SpecPeak>   // top dB peaks from the full-res FFT (precise)
    mix_peaks  : array<SpecPeak>
    ref_energy : array<float>      // peak-normalized RMS over time (the falloff shape)
    mix_energy : array<float>
    ref_centroid_t : array<float>  // spectral centroid (Hz) per time frame (pitch sweep)
    mix_centroid_t : array<float>
    ref_ms     : float             // buffer duration, ms (time scale for the curves)
    mix_ms     : float
}

def private spec48(src : array<float>) : array<float> {
    var d : array<float>
    downsample(src, d, 48)
    return <- d
}

// top `count` local-maximum peaks (freq + dB) from a full-resolution dB spectrum.
def private top_peaks(spec : array<float>; count : int) : array<SpecPeak> {
    var peaks : array<SpecPeak>
    let n = length(spec)
    if (n < 3) return <- peaks
    let hz_per_bin = float(SR) / 2048.0   // compute_spectrum uses N = 2048
    for (i in range(1, n - 1)) {
        if (spec[i] > spec[i - 1] && spec[i] >= spec[i + 1]) {
            peaks |> push(SpecPeak(hz = float(i) * hz_per_bin, db = spec[i]))
        }
    }
    peaks |> sort() $(a, b : SpecPeak) => a.db > b.db
    if (length(peaks) > count) {
        peaks |> resize(count)
    }
    return <- peaks
}

def private make_report() : JsonValue? {
    return JV(SfxReport(ref_loaded = g_have_fit, ref_name = g_ref_name,
        ref = g_ref_feat, mix = g_mix_feat, fit = g_fit,
        ref_spec <- spec48(g_ref_spec), mix_spec <- spec48(g_mix_spec),
        ref_peaks <- top_peaks(g_ref_spec, 6), mix_peaks <- top_peaks(g_mix_spec, 6),
        ref_energy <- energy_curve(g_ref, 40), mix_energy <- energy_curve(g_mix, 40),
        ref_centroid_t <- centroid_curve(g_ref, SR, 24), mix_centroid_t <- centroid_curve(g_mix, SR, 24),
        ref_ms = float(length(g_ref)) / float(SR) * 1000.0,
        mix_ms = float(length(g_mix)) / float(SR) * 1000.0))
}

[live_command(description = "SFX lab: reference-vs-mix features + fit score + downsampled spectra")]
def sfx_report(_input : JsonValue?) : JsonValue? {
    if (g_mix_dirty) {
        rebuild_mix()
    }
    return make_report()
}

struct LoadRefArgs {
    path : string
}

[live_command(description = "SFX lab: load a reference wav by path, then report fit. Args: path")]
def sfx_load_ref(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<LoadRefArgs>)
    if (empty(args.path)) return JV("missing path")
    load_reference(args.path)
    return make_report()
}

[live_command(description = "SFX lab: open a .sfx sound by path (replaces layers, loads its mimic reference), then report fit. Args: path")]
def sfx_open(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<LoadRefArgs>)
    if (empty(args.path)) return JV("missing path")
    do_open_sound(args.path)
    if (g_mix_dirty) {
        rebuild_mix()
    }
    return make_report()
}

// Edit a layer's voice params live — pass ANY subset of keys, only those are applied
// (the rest keep their current value). Covers one slider or a batch in one call.
// Keys: index (default = selected), gain, delay; modal: pitch/spread/n_modes/ring/
// brightness/damp_skew/transient/transient_rate/echo_amount/echo_delay/duration;
// sfxr: wave + env_attack/env_sustain/env_punch/env_decay/base_freq/freq_limit/
// freq_ramp/freq_dramp/duty/duty_ramp/vib_strength/vib_speed/arp_mod/arp_speed/
// repeat_speed/pha_offset/pha_ramp/lpf_freq/lpf_ramp/lpf_resonance/hpf_freq/hpf_ramp/sound_vol.
[live_command(description = "SFX lab: set a layer's voice params (partial — only the keys you pass). Args: index? + any of the gain/delay/modal/sfxr fields. Returns fit.")]
def sfx_set_layer(input : JsonValue?) : JsonValue? {
    let idx = (input ?[ "index"]) ?? g_sel
    if (idx < 0 || idx >= length(g_layers)) return JV("bad layer index")
    var lyr = g_layers[idx]
    lyr.gain = (input ?[ "gain"]) ?? lyr.gain
    lyr.delay = (input ?[ "delay"]) ?? lyr.delay
    if (lyr.voice is modal) {
        var v = lyr.voice as modal
        v.pitch = (input ?[ "pitch"]) ?? v.pitch
        v.spread = (input ?[ "spread"]) ?? v.spread
        v.n_modes = (input ?[ "n_modes"]) ?? v.n_modes
        v.ring = (input ?[ "ring"]) ?? v.ring
        v.brightness = (input ?[ "brightness"]) ?? v.brightness
        v.damp_skew = (input ?[ "damp_skew"]) ?? v.damp_skew
        v.transient = (input ?[ "transient"]) ?? v.transient
        v.transient_rate = (input ?[ "transient_rate"]) ?? v.transient_rate
        v.echo_amount = (input ?[ "echo_amount"]) ?? v.echo_amount
        v.echo_delay = (input ?[ "echo_delay"]) ?? v.echo_delay
        v.duration = (input ?[ "duration"]) ?? v.duration
        lyr.voice = VoiceParams(modal = v)
    } elif (lyr.voice is sfxr) {
        var s = lyr.voice as sfxr
        s.wave = sfxr_wave_of((input ?[ "wave"]) ?? int(s.wave))
        s.env_attack = (input ?[ "env_attack"]) ?? s.env_attack
        s.env_sustain = (input ?[ "env_sustain"]) ?? s.env_sustain
        s.env_punch = (input ?[ "env_punch"]) ?? s.env_punch
        s.env_decay = (input ?[ "env_decay"]) ?? s.env_decay
        s.base_freq = (input ?[ "base_freq"]) ?? s.base_freq
        s.freq_limit = (input ?[ "freq_limit"]) ?? s.freq_limit
        s.freq_ramp = (input ?[ "freq_ramp"]) ?? s.freq_ramp
        s.freq_dramp = (input ?[ "freq_dramp"]) ?? s.freq_dramp
        s.duty = (input ?[ "duty"]) ?? s.duty
        s.duty_ramp = (input ?[ "duty_ramp"]) ?? s.duty_ramp
        s.vib_strength = (input ?[ "vib_strength"]) ?? s.vib_strength
        s.vib_speed = (input ?[ "vib_speed"]) ?? s.vib_speed
        s.arp_mod = (input ?[ "arp_mod"]) ?? s.arp_mod
        s.arp_speed = (input ?[ "arp_speed"]) ?? s.arp_speed
        s.repeat_speed = (input ?[ "repeat_speed"]) ?? s.repeat_speed
        s.pha_offset = (input ?[ "pha_offset"]) ?? s.pha_offset
        s.pha_ramp = (input ?[ "pha_ramp"]) ?? s.pha_ramp
        s.lpf_freq = (input ?[ "lpf_freq"]) ?? s.lpf_freq
        s.lpf_ramp = (input ?[ "lpf_ramp"]) ?? s.lpf_ramp
        s.lpf_resonance = (input ?[ "lpf_resonance"]) ?? s.lpf_resonance
        s.hpf_freq = (input ?[ "hpf_freq"]) ?? s.hpf_freq
        s.hpf_ramp = (input ?[ "hpf_ramp"]) ?? s.hpf_ramp
        s.sound_vol = (input ?[ "sound_vol"]) ?? s.sound_vol
        lyr.voice = VoiceParams(sfxr = s)
    }
    g_layers[idx] = lyr
    g_sel = idx
    g_sel_loaded = -1   // force the editor sliders to reload from the edited model
    g_mix_dirty = true
    g_edit_dirty = true
    rebuild_mix()
    return make_report()
}

// overlay only the postprocess keys present in `input` (nested per-effect objects) onto g_post
def private overlay_post(input : JsonValue?) {
    let f = input ?[ "filter"]
    g_post.filter.enabled = (f ?[ "enabled"]) ?? g_post.filter.enabled
    g_post.filter.position = (f ?[ "position"]) ?? g_post.filter.position
    let bp = input ?[ "bandpass"]
    g_post.bandpass.enabled = (bp ?[ "enabled"]) ?? g_post.bandpass.enabled
    g_post.bandpass.freq = (bp ?[ "freq"]) ?? g_post.bandpass.freq
    g_post.bandpass.q = (bp ?[ "q"]) ?? g_post.bandpass.q
    let dr = input ?[ "drive"]
    g_post.drive.enabled = (dr ?[ "enabled"]) ?? g_post.drive.enabled
    g_post.drive.drive = (dr ?[ "drive"]) ?? g_post.drive.drive
    let cr = input ?[ "crush"]
    g_post.crush.enabled = (cr ?[ "enabled"]) ?? g_post.crush.enabled
    g_post.crush.bits = (cr ?[ "bits"]) ?? g_post.crush.bits
    g_post.crush.coarse = (cr ?[ "coarse"]) ?? g_post.crush.coarse
    let cm = input ?[ "comp"]
    g_post.comp.enabled = (cm ?[ "enabled"]) ?? g_post.comp.enabled
    g_post.comp.threshold = (cm ?[ "threshold"]) ?? g_post.comp.threshold
    g_post.comp.ratio = (cm ?[ "ratio"]) ?? g_post.comp.ratio
    g_post.comp.knee = (cm ?[ "knee"]) ?? g_post.comp.knee
    g_post.comp.attack = (cm ?[ "attack"]) ?? g_post.comp.attack
    g_post.comp.release = (cm ?[ "release"]) ?? g_post.comp.release
    let chx = input ?[ "chorus"]
    g_post.chorus.enabled = (chx ?[ "enabled"]) ?? g_post.chorus.enabled
    g_post.chorus.rate = (chx ?[ "rate"]) ?? g_post.chorus.rate
    g_post.chorus.depth = (chx ?[ "depth"]) ?? g_post.chorus.depth
    g_post.chorus.feedback = (chx ?[ "feedback"]) ?? g_post.chorus.feedback
    g_post.chorus.delay_ms = (chx ?[ "delay_ms"]) ?? g_post.chorus.delay_ms
    g_post.chorus.wet = (chx ?[ "wet"]) ?? g_post.chorus.wet
    let ph = input ?[ "phaser"]
    g_post.phaser.enabled = (ph ?[ "enabled"]) ?? g_post.phaser.enabled
    g_post.phaser.rate = (ph ?[ "rate"]) ?? g_post.phaser.rate
    g_post.phaser.depth = (ph ?[ "depth"]) ?? g_post.phaser.depth
    g_post.phaser.center = (ph ?[ "center"]) ?? g_post.phaser.center
    g_post.phaser.sweep = (ph ?[ "sweep"]) ?? g_post.phaser.sweep
    let tr = input ?[ "tremolo"]
    g_post.tremolo.enabled = (tr ?[ "enabled"]) ?? g_post.tremolo.enabled
    g_post.tremolo.rate = (tr ?[ "rate"]) ?? g_post.tremolo.rate
    g_post.tremolo.depth = (tr ?[ "depth"]) ?? g_post.tremolo.depth
    let dl = input ?[ "delay"]
    g_post.delay.enabled = (dl ?[ "enabled"]) ?? g_post.delay.enabled
    g_post.delay.time = (dl ?[ "time"]) ?? g_post.delay.time
    g_post.delay.feedback = (dl ?[ "feedback"]) ?? g_post.delay.feedback
    let rv = input ?[ "reverb"]
    g_post.reverb.enabled = (rv ?[ "enabled"]) ?? g_post.reverb.enabled
    g_post.reverb.wet = (rv ?[ "wet"]) ?? g_post.reverb.wet
    g_post.reverb.decay = (rv ?[ "decay"]) ?? g_post.reverb.decay
    g_post.reverb.lp_start = (rv ?[ "lp_start"]) ?? g_post.reverb.lp_start
    g_post.reverb.lp_end = (rv ?[ "lp_end"]) ?? g_post.reverb.lp_end
}

[live_command(description = "SFX lab: set postprocess chain params (partial nested JSON per effect, e.g. reverb.enabled / reverb.wet / comp.threshold). Returns fit.")]
def sfx_set_post(input : JsonValue?) : JsonValue? {
    overlay_post(input)
    g_post_reload = true
    g_mix_dirty = true
    g_edit_dirty = true
    rebuild_mix()
    return make_report()
}

[live_command(description = "SFX lab: save the current sound (layers + reference + postprocess) to a .sfx. Args: path (optional; defaults to the last opened/saved path).")]
def sfx_save(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<LoadRefArgs>)
    let path = empty(args.path) ? g_sound_path : args.path
    if (empty(path)) return JV("no path: open or save a sound first, or pass a path arg")
    do_save_sound(path)
    return JV("saved {path}")
}

[live_command(description = "SFX lab: analysis-seed the layers from the loaded reference (modal body + noise layer), then report fit.")]
def sfx_seed(_input : JsonValue?) : JsonValue? {
    seed_from_ref()
    if (g_mix_dirty) {
        rebuild_mix()
    }
    return make_report()
}

[live_command(description = "SFX lab: export the selected voice layer to a .voice file. Args: path")]
def sfx_export_voice(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<LoadRefArgs>)
    if (empty(args.path)) return JV("pass a path arg")
    if (g_sel < 0 || g_sel >= length(g_layers)) return JV("no layer selected")
    do_export_voice(args.path)
    return JV("exported voice to {args.path}")
}

struct ExportDasArgs {
    path : string = ""
    name : string = "render_sound"
    pitched : bool = false       //!< add a base_freq param that scales the modal pitch (e.g. render_tom)
}

[live_command(description = "SFX lab: export the current sound as a drop-in .das render function. Args: path, name, pitched (bool — adds a base_freq pitch-scale param for the tom)")]
def sfx_export_das(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<ExportDasArgs>)
    if (empty(args.path)) return JV("pass a path arg")
    let src = emit_das(args.name, args.pitched)
    if (!fwrite(args.path, src)) return JV("write failed: {args.path}")
    return JV("exported {args.name} ({length(g_layers)} layers) to {args.path}")
}

[live_command(description = "SFX lab: import a .voice file as a new layer, then report fit. Args: path")]
def sfx_import_voice(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<LoadRefArgs>)
    if (empty(args.path)) return JV("pass a path arg")
    do_import_voice(args.path)
    if (g_mix_dirty) {
        rebuild_mix()
    }
    return make_report()
}

struct AddLayerArgs {
    kind : string = "sfxr"
    name : string = "layer"
}

[live_command(description = "SFX lab: add a new voice layer (kind = sfxr | modal), selected at the end; tune it with sfx_set_layer using its index. Args: kind, name.")]
def sfx_add_layer(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<AddLayerArgs>)
    if (args.kind == "modal") {
        add_layer(modal_layer(args.name, ModalVoice()))
    } else {
        add_layer(sfxr_layer(args.name, SfxrSound()))
    }
    rebuild_mix()
    return make_report()
}

[live_command(description = "SFX lab: undo the last edit. Returns fit.")]
def sfx_undo(_input : JsonValue?) : JsonValue? {
    do_undo()
    if (g_mix_dirty) {
        rebuild_mix()
    }
    return make_report()
}

[live_command(description = "SFX lab: redo the last undone edit. Returns fit.")]
def sfx_redo(_input : JsonValue?) : JsonValue? {
    do_redo()
    if (g_mix_dirty) {
        rebuild_mix()
    }
    return make_report()
}

struct OptArgs {
    scope : string = "all"
    iters : int = 16
}

[live_command(description = "SFX lab: auto-tune toward the reference fit. Args: scope (voice|post|all), iters. Returns fit.")]
def sfx_optimize(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<OptArgs>)
    var sc = OPT_COMBINED
    if (args.scope == "voice") {
        sc = OPT_VOICE
    } elif (args.scope == "post") {
        sc = OPT_POST
    }
    optimize(sc, max(args.iters, 1))
    g_post_reload = true
    g_sel_loaded = -1
    if (g_mix_dirty) {
        rebuild_mix()
    }
    return make_report()
}

[live_command(description = "SFX lab: randomize the selected layer's voice params (within bounds). Returns fit.")]
def sfx_randomize_layer(_input : JsonValue?) : JsonValue? {
    randomize_scope(OPT_VOICE)
    g_post_reload = true
    g_sel_loaded = -1
    rebuild_mix()
    return make_report()
}

struct DiscoverArgs {
    kind : string = "modal"
    iters : int = 24
}

[live_command(description = "SFX lab: add a fresh layer (kind modal|sfxr), random-init it, optimize it alone, then optimize the whole sound. Args: kind, iters. Returns fit.")]
def sfx_discover(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<DiscoverArgs>)
    discover(args.kind != "sfxr", max(args.iters, 1))
    g_post_reload = true
    g_sel_loaded = -1
    rebuild_mix()
    return make_report()
}
