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
