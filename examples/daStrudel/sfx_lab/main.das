// SFX Lab — an interactive sfxr-style sound-effect workbench.
// Tweak the full sfxr parameter set with sliders, fire the classic presets, randomize/mutate,
// and see the rendered waveform + spectrum (ImPlot) live. Play with the button (or autoplay on preset).
//
// Run (external imgui/implot modules loaded explicitly):
//   daslang -load_module <dasImgui> -load_module <dasImguiImplot> examples/daStrudel/sfx_lab/main.das
options gen2

require imgui/imgui_harness
require imgui/imgui_containers_builtin
require imgui/imgui_widgets_builtin
require imgui/imgui_implot_boost_v2
require implot
require audio/audio_boost
require strudel/strudel_sfxr
require math

let TWO_PI = 6.28318530717958647692
let INV_LN10 = 0.43429448190325176

var g_buf : array<float>           // last rendered mono buffer (kept for plots)
var g_wave_plot : array<float>     // downsampled waveform for display
var g_spec_plot : array<float>     // spectrum magnitudes (dB) for display
var g_seed = 0x1234567u            // RNG seed for presets / randomize / mutate
var g_autoplay = true
var g_changed = false
var g_started = false
var g_ctx : ImPlotContext?
var g_audio : AudioSystemChannels

// ===== Audio =====

def play_buf() {
    if (empty(g_buf)) return
    var tmp : array<float>
    tmp := g_buf  // clone — play_sound_from_pcm moves its buffer, we keep g_buf for the plots
    play_sound_from_pcm(SFXR_SAMPLE_RATE, 1, tmp)
}

// ===== Plot data =====

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
            let w = 0.5 - 0.5 * cos(TWO_PI * float(i) / float(N - 1))  // Hann window
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
        for (i in range(cnt)) {
            out[i] = buf[i]
        }
    } else {
        for (i in range(cnt)) {
            out[i] = buf[i * n / maxpts]
        }
    }
}

def rebuild_plots() {
    downsample(g_buf, g_wave_plot, 2000)
    compute_spectrum(g_buf, g_spec_plot)
}

// ===== Parameter <-> widget sync =====

def write_params(s : SfxrSound) {
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

def read_params() : SfxrSound {
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

// Apply a whole sound (preset / randomize / mutate): push to widgets, render, audition.
def apply_sound(s : SfxrSound) {
    write_params(s)
    g_buf <- render_sfxr(s)
    rebuild_plots()
    if (g_autoplay) {
        play_buf()
    }
}

def start_app() {
    g_started = true
    CB_AUTO.value = true
    g_autoplay = true
    let s = sfxr_pickup_coin(g_seed)
    write_params(s)
    g_buf <- render_sfxr(s)
    rebuild_plots()
}

[export]
def init() {
    harness_init("SFX Lab — sfxr", 1180, 820)
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
    g_changed = false

    SetNextWindowPos(float2(20.0, 20.0), ImGuiCond.Always)
    SetNextWindowSize(float2(1140.0, 780.0), ImGuiCond.Always)
    window(SFX_WIN, (text = "SFX Lab", closable = false, flags = ImGuiWindowFlags.None)) {
        // --- preset row ---
        if (button(BTN_COIN, (text = "Coin"))) { apply_sound(sfxr_pickup_coin(g_seed)); }
        same_line(SL_A)
        if (button(BTN_LASER, (text = "Laser"))) { apply_sound(sfxr_laser_shoot(g_seed)); }
        same_line(SL_B)
        if (button(BTN_EXPL, (text = "Explosion"))) { apply_sound(sfxr_explosion(g_seed)); }
        same_line(SL_C)
        if (button(BTN_POW, (text = "Powerup"))) { apply_sound(sfxr_powerup(g_seed)); }
        same_line(SL_D)
        if (button(BTN_HIT, (text = "Hit"))) { apply_sound(sfxr_hit_hurt(g_seed)); }
        same_line(SL_E)
        if (button(BTN_JUMP, (text = "Jump"))) { apply_sound(sfxr_jump(g_seed)); }
        same_line(SL_F)
        if (button(BTN_BLIP, (text = "Blip"))) { apply_sound(sfxr_blip_select(g_seed)); }

        // --- action row ---
        if (button(BTN_RND, (text = "Randomize"))) { apply_sound(sfxr_randomize(g_seed)); }
        same_line(SL_G)
        if (button(BTN_MUT, (text = "Mutate"))) { apply_sound(sfxr_mutate(read_params(), g_seed, 0.1)); }
        same_line(SL_H)
        if (button(BTN_PLAY, (text = "Play"))) { play_buf(); }
        same_line(SL_I)
        checkbox(CB_AUTO, (text = "autoplay"))
        g_autoplay = CB_AUTO.value

        separator(SEP1)

        if (combo(WAVE_COMBO, (text = "wave", items <- ["square", "sawtooth", "sine", "noise", "triangle"]))) {
            g_changed = true
        }

        collapsing_header(H_ENV, (text = "Envelope", closable = false, flags = ImGuiTreeNodeFlags.DefaultOpen)) {
            SL_ATTACK.bounds = (0.0f, 1.0f)
            if (slider_float(SL_ATTACK, (text = "attack"))) { g_changed = true; }
            SL_SUSTAIN.bounds = (0.0f, 1.0f)
            if (slider_float(SL_SUSTAIN, (text = "sustain"))) { g_changed = true; }
            SL_PUNCH.bounds = (0.0f, 1.0f)
            if (slider_float(SL_PUNCH, (text = "punch"))) { g_changed = true; }
            SL_DECAY.bounds = (0.0f, 1.0f)
            if (slider_float(SL_DECAY, (text = "decay"))) { g_changed = true; }
        }
        collapsing_header(H_FREQ, (text = "Frequency", closable = false, flags = ImGuiTreeNodeFlags.DefaultOpen)) {
            SL_BASEFREQ.bounds = (0.0f, 1.0f)
            if (slider_float(SL_BASEFREQ, (text = "base freq"))) { g_changed = true; }
            SL_FREQLIMIT.bounds = (0.0f, 1.0f)
            if (slider_float(SL_FREQLIMIT, (text = "freq limit"))) { g_changed = true; }
            SL_FREQRAMP.bounds = (-1.0f, 1.0f)
            if (slider_float(SL_FREQRAMP, (text = "freq ramp"))) { g_changed = true; }
            SL_FREQDRAMP.bounds = (-1.0f, 1.0f)
            if (slider_float(SL_FREQDRAMP, (text = "freq dramp"))) { g_changed = true; }
        }
        collapsing_header(H_TONE, (text = "Tone / modulation", closable = false, flags = ImGuiTreeNodeFlags.None)) {
            SL_DUTY.bounds = (0.0f, 1.0f)
            if (slider_float(SL_DUTY, (text = "duty"))) { g_changed = true; }
            SL_DUTYRAMP.bounds = (-1.0f, 1.0f)
            if (slider_float(SL_DUTYRAMP, (text = "duty ramp"))) { g_changed = true; }
            SL_VIBSTR.bounds = (0.0f, 1.0f)
            if (slider_float(SL_VIBSTR, (text = "vibrato depth"))) { g_changed = true; }
            SL_VIBSPD.bounds = (0.0f, 1.0f)
            if (slider_float(SL_VIBSPD, (text = "vibrato speed"))) { g_changed = true; }
            SL_ARPMOD.bounds = (-1.0f, 1.0f)
            if (slider_float(SL_ARPMOD, (text = "arp mod"))) { g_changed = true; }
            SL_ARPSPD.bounds = (0.0f, 1.0f)
            if (slider_float(SL_ARPSPD, (text = "arp speed"))) { g_changed = true; }
            SL_REPEAT.bounds = (0.0f, 1.0f)
            if (slider_float(SL_REPEAT, (text = "repeat speed"))) { g_changed = true; }
            SL_PHAOFF.bounds = (-1.0f, 1.0f)
            if (slider_float(SL_PHAOFF, (text = "phaser offset"))) { g_changed = true; }
            SL_PHARAMP.bounds = (-1.0f, 1.0f)
            if (slider_float(SL_PHARAMP, (text = "phaser ramp"))) { g_changed = true; }
        }
        collapsing_header(H_FILTER, (text = "Filter", closable = false, flags = ImGuiTreeNodeFlags.None)) {
            SL_LPFFREQ.bounds = (0.0f, 1.0f)
            if (slider_float(SL_LPFFREQ, (text = "lpf cutoff"))) { g_changed = true; }
            SL_LPFRAMP.bounds = (-1.0f, 1.0f)
            if (slider_float(SL_LPFRAMP, (text = "lpf ramp"))) { g_changed = true; }
            SL_LPFRES.bounds = (-1.0f, 1.0f)
            if (slider_float(SL_LPFRES, (text = "lpf resonance"))) { g_changed = true; }
            SL_HPFFREQ.bounds = (0.0f, 1.0f)
            if (slider_float(SL_HPFFREQ, (text = "hpf cutoff"))) { g_changed = true; }
            SL_HPFRAMP.bounds = (-1.0f, 1.0f)
            if (slider_float(SL_HPFRAMP, (text = "hpf ramp"))) { g_changed = true; }
        }
        SL_VOL.bounds = (0.0f, 1.0f)
        if (slider_float(SL_VOL, (text = "master volume"))) { g_changed = true; }

        separator(SEP2)

        plot(PLOT_WAVE, (title = "waveform", size = float2(-1.0f, 170.0f), flags = ImPlotFlags.None)) {
            setup_axes_limits(0.0lf, double(max(length(g_wave_plot), 1)), -1.0lf, 1.0lf)
            plot_line("wave", g_wave_plot)
        }
        plot(PLOT_SPEC, (title = "spectrum (dB)", size = float2(-1.0f, 170.0f), flags = ImPlotFlags.None)) {
            setup_axes_limits(0.0lf, double(max(length(g_spec_plot), 1)), -80.0lf, 0.0lf)
            plot_line("mag", g_spec_plot)
        }
    }

    // Slider edits re-render + refresh the plots (visual); audition stays on Play / preset autoplay
    // so dragging a slider doesn't machine-gun the speaker.
    if (g_changed) {
        let cur = read_params()
        g_buf <- render_sfxr(cur)
        rebuild_plots()
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
