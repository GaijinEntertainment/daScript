options gen2
options indenting = 4

module strudel_sfx shared public
//! Multi-layer sound model — a sound is a stack of voices (sfxr or modal), each with its own
//! gain + onset delay, summed. This is the "several waves with delays" model: e.g. a bouncing
//! ball = one click layer duplicated with increasing delay + falling gain. Voices come from
//! strudel_sfxr / strudel_modal; render_sound mixes a layer stack into one mono buffer.

require strudel/strudel_sfxr public
require strudel/strudel_modal public
require audio
require daslib/fio
require math

let SFX_SAMPLE_RATE = 44100

variant VoiceParams {
    sfxr : SfxrSound
    modal : ModalVoice
}

struct Layer {
    //! One voice in a sound. gain/delay/enabled drive the mix; locked/link_source are authoring
    //! metadata (a linked layer mirrors its source's voice params, keeping its own gain/delay).
    name : string = "layer"
    voice : VoiceParams = VoiceParams(sfxr = SfxrSound())
    gain : float = 1.0
    delay : float = 0.0          //!< onset delay in seconds
    enabled : bool = true        //!< mute when false
    solo : bool = false          //!< UI: when any layer is soloed, only soloed layers play
    locked : bool = false        //!< UI: collapsed + read-only
    link_source : int = -1       //!< index of the layer this mirrors; -1 = independent
}

def sfxr_layer(name : string; s : SfxrSound) : Layer {
    //! Make an sfxr voice layer.
    return Layer(name = name, voice = VoiceParams(sfxr = s))
}

def modal_layer(name : string; v : ModalVoice) : Layer {
    //! Make a modal voice layer.
    return Layer(name = name, voice = VoiceParams(modal = v))
}

def render_voice(v : VoiceParams) : array<float> {
    //! Render a single voice to a mono buffer.
    if (v is sfxr) {
        return <- render_sfxr(v as sfxr)
    }
    return <- render_modal_voice(v as modal)
}

// Copy each linked layer's voice params from its source (keeps own gain/delay/name/enabled).
def propagate_links(var layers : array<Layer>) {
    for (i in range(length(layers))) {
        let src = layers[i].link_source
        if (src >= 0 && src < length(layers) && src != i) {
            layers[i].voice = layers[src].voice
        }
    }
}

def render_sound(layers : array<Layer>) : array<float> {
    //! Mix a layer stack into one mono buffer: each enabled voice, offset by its delay and scaled
    //! by its gain, summed; the result is peak-normalized to -1 dBFS.
    var out : array<float>
    for (lyr in layers) {
        if (!lyr.enabled) {
            continue
        }
        var buf <- render_voice(lyr.voice)
        let off = max(int(lyr.delay * float(SFX_SAMPLE_RATE)), 0)
        let need = off + length(buf)
        if (length(out) < need) {
            out |> resize(need)
        }
        for (i in range(length(buf))) {
            out[off + i] += buf[i] * lyr.gain
        }
        delete buf
    }
    var pk = 0.0
    for (x in out) {
        pk = max(pk, abs(x))
    }
    if (pk > 1.0e-4) {   // don't amplify near-silence into garbage
        let g = 0.89 / pk
        for (x in out) {
            x *= g
        }
    }
    for (x in out) {     // guard any NaN/Inf that slipped through
        if (!(x > -1.0e30 && x < 1.0e30)) {
            x = 0.0
        }
    }
    return <- out
}

// ===== postprocess chain (a fixed chain of toggleable engine effects on the mixed buffer) =====
// Mirrors strudel_synth's VoiceFX init/param/process patterns + apply_room; all off by default.

struct FxFilter {
    enabled  : bool = false
    position : float = 0.5      //!< DJ filter: 0 = full LPF .. 0.5 = bypass .. 1 = full HPF
}

struct FxBandpass {
    enabled : bool = false
    freq    : float = 1000.0
    q       : float = 1.0
}

struct FxDrive {
    enabled : bool = false
    drive   : float = 1.0       //!< waveshaper tanh drive (0 = clean .. 5+ = harsh)
}

struct FxCrush {
    enabled : bool = false
    bits    : float = 8.0       //!< target bit depth
    coarse  : int = 0           //!< sample-rate reduction factor (0 = off)
}

struct FxComp {
    enabled   : bool = false
    threshold : float = -18.0   //!< dB (negative enables)
    ratio     : float = 4.0
    knee      : float = 10.0
    attack    : float = 0.005
    release   : float = 0.05
}

struct FxChorus {
    enabled  : bool = false
    rate     : float = 1.0
    depth    : float = 3.0
    feedback : float = 0.3
    delay_ms : float = 7.0
    wet      : float = 0.5
}

struct FxPhaser {
    enabled : bool = false
    rate    : float = 1.0
    depth   : float = 0.5
    center  : float = 1000.0
    sweep   : float = 2000.0
}

struct FxTremolo {
    enabled : bool = false
    rate    : float = 5.0
    depth   : float = 0.5
}

struct FxDelay {
    enabled  : bool = false
    time     : float = 0.25
    feedback : float = 0.3
}

struct FxReverb {
    enabled  : bool = false
    wet      : float = 0.3
    decay    : float = 0.4
    lp_start : float = 12000.0
    lp_end   : float = 2000.0
}

struct PostChain {
    filter   : FxFilter = FxFilter()
    bandpass : FxBandpass = FxBandpass()
    drive    : FxDrive = FxDrive()
    crush    : FxCrush = FxCrush()
    comp     : FxComp = FxComp()
    chorus   : FxChorus = FxChorus()
    phaser   : FxPhaser = FxPhaser()
    tremolo  : FxTremolo = FxTremolo()
    delay    : FxDelay = FxDelay()
    reverb   : FxReverb = FxReverb()
}

def any_enabled(p : PostChain) : bool {
    return (p.filter.enabled || p.bandpass.enabled || p.drive.enabled || p.crush.enabled ||
        p.comp.enabled || p.chorus.enabled || p.phaser.enabled || p.tremolo.enabled ||
        p.delay.enabled || p.reverb.enabled)
}

// value-field effect states (mirrors strudel_synth's VoiceFX), initialized per call
[safe_when_uninitialized]
struct PostState {
    @safe_when_uninitialized dj   : ma_djfilter
    @safe_when_uninitialized bp   : ma_bandpass
    @safe_when_uninitialized ws   : ma_waveshaper
    @safe_when_uninitialized bc   : ma_bitcrush
    @safe_when_uninitialized comp : ma_compressor
    @safe_when_uninitialized ph   : ma_phaser
    @safe_when_uninitialized trem : ma_tremolo
}

// Apply the enabled effects, in chain order, to a mono buffer in place (rate = sample rate, Hz).
// Effects run on an interleaved stereo working copy; the result is folded back to mono.
def apply_post(var mono : array<float>; p : PostChain; rate : float) {
    let n0 = length(mono)
    if (n0 == 0 || !any_enabled(p)) return
    // extend the buffer with silence so the reverb / delay tail isn't cut off
    var tail = 0
    if (p.reverb.enabled) {
        tail = max(tail, int(p.reverb.decay * rate * 1.5))
    }
    if (p.delay.enabled) {
        let echoes = min(log(0.001) / log(max(p.delay.feedback, 0.02)), 30.0)
        tail = max(tail, int(p.delay.time * rate * echoes))
    }
    if (tail > 0) {
        mono |> resize(n0 + tail)
    }
    let n = length(mono)
    var st : array<float>
    st |> resize(n * 2)
    for (i in range(n)) {
        st[i * 2] = mono[i]
        st[i * 2 + 1] = mono[i]
    }
    var s : PostState

    if (p.filter.enabled) {
        unsafe { djfilter_init(addr(s.dj), rate); }
        s.dj.position = p.filter.position
        unsafe { djfilter_process(addr(s.dj), addr(st[0]), n); }
    }
    if (p.bandpass.enabled) {
        unsafe { bandpass_init(addr(s.bp), rate); }
        unsafe { bandpass_setup(addr(s.bp), p.bandpass.freq, max(p.bandpass.q, 0.01)); }
        unsafe { bandpass_process(addr(s.bp), addr(st[0]), n); }
    }
    if (p.drive.enabled) {
        unsafe { waveshaper_init(addr(s.ws)); }
        s.ws.drive = p.drive.drive
        unsafe { waveshaper_process(addr(s.ws), addr(st[0]), n); }
    }
    if (p.crush.enabled) {
        unsafe { bitcrush_init(addr(s.bc)); }
        s.bc.bits = p.crush.bits
        s.bc.coarse = p.crush.coarse
        unsafe { bitcrush_process(addr(s.bc), addr(st[0]), n); }
    }
    if (p.comp.enabled) {
        unsafe { compressor_init(addr(s.comp), rate); }
        s.comp.threshold_db = p.comp.threshold
        s.comp.ratio = max(p.comp.ratio, 1.0)
        s.comp.knee_db = max(p.comp.knee, 0.0)
        s.comp.attack = max(p.comp.attack, 0.0)
        s.comp.release = max(p.comp.release, 0.0)
        unsafe { compressor_process(addr(s.comp), addr(st[0]), n); }
    }
    if (p.chorus.enabled) {
        var ch = new ma_chorus
        chorus_init(ch, rate)
        var cfg = chorus_config_default()
        cfg.rate = p.chorus.rate
        cfg.depth = p.chorus.depth
        cfg.feedback = p.chorus.feedback
        cfg.delay_ms = p.chorus.delay_ms
        cfg.wet = p.chorus.wet
        chorus_set_config(ch, cfg)
        var out : array<float>
        out |> resize(n * 2)
        unsafe { chorus_process(ch, addr(st[0]), addr(out[0]), n); }
        for (i in range(n * 2)) {
            st[i] = out[i]
        }
        delete out
        unsafe { delete ch; }
    }
    if (p.phaser.enabled) {
        unsafe { phaser_init(addr(s.ph), rate); }
        s.ph.rate = p.phaser.rate
        s.ph.depth = clamp(p.phaser.depth, 0.0, 1.0)
        s.ph.center = p.phaser.center
        s.ph.sweep = p.phaser.sweep
        unsafe { phaser_process(addr(s.ph), addr(st[0]), n); }
    }
    if (p.tremolo.enabled) {
        unsafe { tremolo_init(addr(s.trem), rate); }
        s.trem.rate = p.tremolo.rate
        s.trem.depth = clamp(p.tremolo.depth, 0.0, 1.0)
        unsafe { tremolo_process(addr(s.trem), addr(st[0]), n); }
    }
    if (p.delay.enabled) {
        var d = new ma_delay
        delay_init(d, int(rate), p.delay.time, p.delay.feedback)
        var out : array<float>
        out |> resize(n * 2)
        unsafe { delay_process(d, addr(st[0]), addr(out[0]), n); }
        for (i in range(n * 2)) {
            st[i] = out[i]
        }
        delete out
        delay_uninit(d)
        unsafe { delete d; }
    }
    if (p.reverb.enabled) {
        var rev = new ConvolutionReverb
        conv_reverb_init(rev, int(rate), p.reverb.decay, p.reverb.lp_start, p.reverb.lp_end, 0.005, ConvReverbQuality.high, 0)
        var wet : array<float>
        wet |> resize(n * 2)
        unsafe { conv_reverb_process(rev, addr(st[0]), addr(wet[0]), n); }
        conv_reverb_uninit(rev)
        unsafe { delete rev; }
        for (i in range(n * 2)) {
            st[i] += wet[i] * p.reverb.wet
        }
        delete wet
    }

    for (i in range(n)) {
        var m = (st[i * 2] + st[i * 2 + 1]) * 0.5
        if (!(m > -1.0e30 && m < 1.0e30)) {   // guard NaN/Inf from any effect
            m = 0.0
        }
        mono[i] = m
    }
    delete st
}

// ===== persistence (.sfx = JSON) =====

struct Sound {
    //! A saved sound: the layer stack + an optional reference-target path to mimic.
    version : int = 1
    mimic_target : string         //!< path of an external wav to compare against (never embedded)
    comment : string = ""         //!< freeform label shown at the top of the editor (e.g. "analysis" / "from render")
    layers : array<Layer>
    post : PostChain = PostChain()  //!< postprocess effect chain applied to the mix
    master_gain : float = 1.0       //!< output gain applied AFTER render (so the file plays at the audited / reference level)
}

def save_sound(path : string; doc : Sound) : bool {
    //! Write a sound to a .sfx (JSON) file. The voice variant encodes the per-layer kind.
    return fwrite(path, sprint_json(doc, true))
}

def load_sound(path : string; var doc : Sound) : bool {
    //! Read a .sfx (JSON) file into `doc`. Returns false on a missing/invalid file.
    let text = fread(path)
    if (empty(text)) {
        return false
    }
    return sscan_json(text, doc)
}

def save_layer(path : string; lyr : Layer) : bool {
    //! Write a single voice layer to a .voice (JSON) file — share/reuse one voice across sounds.
    return fwrite(path, sprint_json(lyr, true))
}

def load_layer(path : string; var lyr : Layer) : bool {
    //! Read a single voice layer from a .voice (JSON) file. Returns false on a missing/invalid file.
    let text = fread(path)
    if (empty(text)) {
        return false
    }
    return sscan_json(text, lyr)
}

def sound_to_json(doc : Sound) : string {
    //! Serialize a sound to a JSON string (for in-memory snapshots, e.g. an undo stack).
    return sprint_json(doc, false)
}

def sound_from_json(text : string; var doc : Sound) : bool {
    //! Parse a sound from a JSON string. Returns false on empty/invalid input.
    if (empty(text)) {
        return false
    }
    return sscan_json(text, doc)
}

def render_sound_file(path : string) : array<float> {
    //! Game-facing one-call: load a .sfx and render it to a mono buffer (empty on failure).
    var doc = Sound()
    var out : array<float>
    if (load_sound(path, doc)) {
        out <- render_sound(doc.layers)
        apply_post(out, doc.post, float(SFX_SAMPLE_RATE))
        if (doc.master_gain != 1.0) {
            for (s in out) {
                s *= doc.master_gain
            }
        }
    }
    return <- out
}

def sfx_conform(buf : array<float>; src_rate, dst_rate : int; duration, gain : float) : array<float> {
    //! Resample `buf` from src_rate to dst_rate (linear), conform it to `duration` seconds
    //! (truncate if longer, zero-pad if shorter) and scale by `gain`. This is what lets an
    //! exported `.sfx` render function — which builds its sound at the engine's native rate
    //! (44100) — drop into a host running at a different rate with the standard drum signature
    //! `(duration, gain) : array<float>`.
    let n = length(buf)
    let want = max(int(duration * float(dst_rate)), 0)
    var out : array<float>
    out |> resize(want)
    if (n == 0 || want == 0) {
        return <- out
    }
    let ratio = float(src_rate) / float(dst_rate)
    for (i in range(want)) {
        let pos = float(i) * ratio
        let i0 = int(pos)
        if (i0 >= n) {
            break   // past the source end — remaining samples stay zero (pad)
        }
        let frac = pos - float(i0)
        let a = buf[i0]
        let b = buf[min(i0 + 1, n - 1)]
        out[i] = (a + (b - a) * frac) * gain
    }
    return <- out
}

// ===== code generation: emit a sound as a drop-in .das render function =====
// Emits real daslang (constructors, only non-default fields) that rebuilds the sound's layers +
// post and calls the engine, conformed to the standard one-shot signature (duration, gain) and
// resampled to 48000 (the strudel_synth host rate). `pitch_param` adds a `base_freq` arg that scales
// the modal pitch — for the parametric render_tom. Drop the result into strudel_synth.das (with
// `require strudel/strudel_sfx`) and it "just works".

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

def emit_das(doc : Sound; func_name : string; pitch_param : bool) : string {
    //! Emit a drop-in `.das` render function reproducing `doc`. See section header above.
    let LB = "\x7b"   // literal { (avoids daslang string interpolation)
    let RB = "\x7d"   // literal }
    var fund = 0.0    // fundamental = lowest modal pitch, for the pitch-scaled (tom) form
    for (lyr in doc.layers) {
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
    for (lyr in doc.layers) {
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
    let post = emit_post(doc.post)
    if (!empty(post)) {
        lines |> push("    apply_post(out, PostChain({post}), 44100.0)")
    }
    lines |> push("    return <- sfx_conform(out, 44100, 48000, duration, gain * {doc.master_gain})")
    lines |> push("{RB}")
    return das_join(lines, "\n") + "\n"
}
