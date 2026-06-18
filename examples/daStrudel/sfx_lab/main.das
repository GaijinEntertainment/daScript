// SFX Lab — multi-layer procedural sound-effect workbench.
// A sound is a stack of layers (sfxr or modal voices), each with its own gain + onset delay,
// mixed. Docked panels: Layers / Editor / Mix. Build, audition per-layer or the mix, and watch
// the waveform + spectrum live. (Reference target + save/load land in later slices.)
//
// Run: daslang -load_module <dasImgui> -load_module <dasImguiImplot> examples/daStrudel/sfx_lab/main.das
options gen2

require imgui/imgui_harness
require imgui/imgui_containers_builtin
require imgui/imgui_widgets_builtin
require imgui/imgui_implot_boost_v2
require implot
require audio/audio_boost
require strudel/strudel_sfx
require math

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
var g_seed = 0x1234567u
var g_autoplay = true
var g_mix_dirty = true
var g_lyr_dirty = true
var g_play_pending = false       // autoplay the mix after the next rebuild (set by discrete actions)
var g_started = false
var g_ctx : ImPlotContext?
var g_audio : AudioSystemChannels

// per-row widget state (indexed by layer position) — must be declared at module scope
var private LAYER_ROW : table<int; ClickState>
var private MUTE_ROW : table<int; ClickState>
var private PLAY_ROW : table<int; ClickState>
var private ROW_SL1 : table<int; EmptyMarkerState>
var private ROW_SL2 : table<int; EmptyMarkerState>

// ===== DSP / plot helpers =====

def private fft(var re, im : array<float>) {
    let n = length(re)
    var j = 0
    for (i in range(1, n)) {
        var bit = n >> 1
        while ((j & bit) != 0) {
            j = j ^ bit
            bit = bit >> 1
        }
        j = j | bit
        if (i < j) {
            let tr = re[i]; re[i] = re[j]; re[j] = tr
            let ti = im[i]; im[i] = im[j]; im[j] = ti
        }
    }
    var len = 2
    while (len <= n) {
        let half = len / 2
        let ang = -TWO_PI / float(len)
        let wr = cos(ang)
        let wi = sin(ang)
        var i = 0
        while (i < n) {
            var cr = 1.0
            var ci = 0.0
            for (k in range(half)) {
                let a = i + k
                let b = a + half
                let xr = re[b] * cr - im[b] * ci
                let xi = re[b] * ci + im[b] * cr
                re[b] = re[a] - xr
                im[b] = im[a] - xi
                re[a] = re[a] + xr
                im[a] = im[a] + xi
                let ncr = cr * wr - ci * wi
                ci = cr * wi + ci * wr
                cr = ncr
            }
            i = i + len
        }
        len = len << 1
    }
}

def private compute_spectrum(buf : array<float>; var out : array<float>) {
    let N = 2048
    var re : array<float>
    var im : array<float>
    re |> resize(N)
    im |> resize(N)
    let m = min(length(buf), N)
    for (i in range(N)) {
        im[i] = 0.0
        if (i < m) {
            let w = 0.5 - 0.5 * cos(TWO_PI * float(i) / float(N - 1))
            re[i] = buf[i] * w
        } else {
            re[i] = 0.0
        }
    }
    fft(re, im)
    let half = N / 2
    out |> resize(half)
    for (i in range(half)) {
        let mag = sqrt(re[i] * re[i] + im[i] * im[i]) / float(half)
        out[i] = 20.0 * log(mag + 1.0e-9) * INV_LN10
    }
    delete re
    delete im
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

def play_mix() {
    if (g_mix_dirty) {
        rebuild_mix()
    }
    play_buf_copy(g_mix)
}

def play_layer(idx : int) {
    if (idx < 0 || idx >= length(g_layers)) return
    var b <- render_voice(g_layers[idx].voice)
    play_buf_copy(b)
    delete b
}

def rebuild_mix() {
    propagate_links(g_layers)
    g_mix <- render_sound(g_layers)
    downsample(g_mix, g_mix_wave, 2000)
    compute_spectrum(g_mix, g_mix_spec)
    g_mix_dirty = false
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
}

def clear_all() {
    g_layers |> clear()
    g_sel = 0
    g_sel_loaded = -1
    g_mix_dirty = true
    g_lyr_dirty = true
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
    if (button(DUP_BTN, (text = "duplicate"))) {
        duplicate_layer(g_sel, false)
    }
    same_line(TB3)
    if (button(DUPLINK_BTN, (text = "dup linked"))) {
        duplicate_layer(g_sel, true)
    }
    same_line(TB4)
    if (button(DEL_BTN, (text = "delete"))) {
        delete_layer(g_sel)
    }
    separator(LSEP)
    for (i in range(length(g_layers))) {
        let kind = g_layers[i].voice is sfxr ? "sfxr" : "modal"
        var marks = ""
        if (!g_layers[i].enabled) { marks += " [mute]"; }
        if (g_layers[i].locked) { marks += " [lock]"; }
        if (g_layers[i].link_source >= 0) { marks += " [link]"; }
        let lbl = "{g_layers[i].name}  ({kind}){marks}###L{i}"
        // limit the selectable to the name column so its hit-box doesn't sit under the row buttons
        let name_w = max(GetContentRegionAvail().x - 130.0, 80.0)
        if (selectable_label(LAYER_ROW[i], lbl, i == g_sel, ImGuiSelectableFlags.None, float2(name_w, 0.0))) {
            g_sel = i
        }
        same_line(ROW_SL1[i])
        let on_lbl = (g_layers[i].enabled ? "on" : "off") + "###mute{i}"
        if (button(MUTE_ROW[i], (text = on_lbl))) {
            g_layers[i].enabled = !g_layers[i].enabled
            g_mix_dirty = true
        }
        same_line(ROW_SL2[i])
        if (button(PLAY_ROW[i], (text = "play###pl{i}"))) {
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
    }
    DELAY.bounds = (0.0f, 1.0f)
    if (slider_float(DELAY, (text = "onset delay (s)"))) {
        g_layers[g_sel].delay = DELAY.value
        g_mix_dirty = true
    }
    if (checkbox(LOCK, (text = "locked (voice read-only)"))) {
        g_layers[g_sel].locked = LOCK.value
    }
    same_line(EDIT_SL)
    if (button(EDIT_PLAY, (text = "play layer"))) {
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
        }
    }

    collapsing_header(PREVIEW_H, (text = "layer preview", closable = false, flags = ImGuiTreeNodeFlags.None)) {
        plot(PV_WAVE, (title = "layer waveform", size = float2(-1.0f, 120.0f), flags = ImPlotFlags.None)) {
            setup_axes_limits(0.0lf, double(max(length(g_lyr_wave), 1)), -1.0lf, 1.0lf)
            plot_line("layer", g_lyr_wave)
        }
        plot(PV_SPEC, (title = "layer spectrum (dB)", size = float2(-1.0f, 120.0f), flags = ImPlotFlags.None)) {
            setup_axes_limits(0.0lf, double(max(length(g_lyr_spec), 1)), -80.0lf, 0.0lf)
            plot_line("layer", g_lyr_spec)
        }
    }
}

def sfxr_editor() : bool {
    var ch = false
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
    M_RING.bounds = (0.005f, 0.4f)
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
    M_DUR.bounds = (0.05f, 1.0f)
    if (slider_float(M_DUR, (text = "duration (s)"))) { ch = true; }
    return ch
}

def mix_panel() {
    plot(MIX_WAVE, (title = "mix waveform", size = float2(-1.0f, 150.0f), flags = ImPlotFlags.None)) {
        setup_axes_limits(0.0lf, double(max(length(g_mix_wave), 1)), -1.0lf, 1.0lf)
        plot_line("mix", g_mix_wave)
    }
    plot(MIX_SPEC, (title = "mix spectrum (dB)", size = float2(-1.0f, 150.0f), flags = ImPlotFlags.None)) {
        setup_axes_limits(0.0lf, double(max(length(g_mix_spec), 1)), -80.0lf, 0.0lf)
        plot_line("mix", g_mix_spec)
    }
}

def setup_default_layout(dock_id : uint) {
    DockBuilderRemoveNode(dock_id)
    DockBuilderAddDockSpaceNode(dock_id, ImGuiDockNodeFlags.None)
    let vp = GetMainViewport()
    DockBuilderSetNodeSize(dock_id, ImVec2(vp.Size.x, vp.Size.y))
    var layers_id : uint = 0u
    var rest_id : uint = 0u
    DockBuilderSplitNode(dock_id, ImGuiDir.Up, 0.18f, layers_id, rest_id)
    var mix_id : uint = 0u
    var editor_id : uint = 0u
    DockBuilderSplitNode(rest_id, ImGuiDir.Down, 0.40f, mix_id, editor_id)
    DockBuilderDockWindow("Layers", layers_id)
    DockBuilderDockWindow("Editor", editor_id)
    DockBuilderDockWindow("Mix", mix_id)
    DockBuilderFinish(dock_id)
}

def start_app() {
    g_started = true
    MB_AUTO.value = true
    g_autoplay = true
    // seed a starter sound: the ball-on-table (thud + click), so the lab opens on a real multi-layer sound
    add_layer(modal_layer("table thud", ModalVoice(pitch = 170.0, spread = 2.4, n_modes = 2, ring = 0.05, brightness = 0.35, transient = 0.2, duration = 0.5)))
    add_layer(modal_layer("ball click", ModalVoice(pitch = 1500.0, spread = 1.6, n_modes = 4, ring = 0.05, brightness = 0.55, damp_skew = 0.9, transient = 0.6, echo_amount = 0.32, echo_delay = 0.055, duration = 0.5)))
    g_sel = 0
    g_play_pending = false
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
        }
        if (button(MB_PLAY, (text = "Play mix"))) {
            play_mix()
        }
        same_line(MB_SL1)
        if (checkbox(MB_AUTO, (text = "autoplay"))) {
        }
        g_autoplay = MB_AUTO.value
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
        dock_window(MIX, (text = "Mix", closable = false, flags = ImGuiWindowFlags.None)) {
            mix_panel()
        }
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
