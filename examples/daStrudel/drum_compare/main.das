options gen2
options indenting = 4
options persistent_heap
options no_global_variables = false
options no_unused_block_arguments = false
options no_unused_function_arguments = false

// Interactive drum pad comparison: synth vs SF2 (Musyng Kite).
// Click pads to hear drums. Press S to toggle synth/SF2 mode.
// Usage: daslang.exe examples/daStrudel/drum_compare/main.das

require glfw/glfw_boost
require opengl/opengl_boost
require opengl/opengl_ttf
require audio/audio_boost
require strudel/strudel_synth
require strudel/strudel_sf2
require strudel/strudel_sf2_voice
require daslib/defer
require daslib/fio
require daslib/safe_addr
require daslib/math_boost
require math

// ─── Shaders ───

var @in @location v_position : float2
var @in @location v_color : float4
var @inout f_color : float4
var @uniform v_projection : float4x4
var @out f_FragColor : float4

[vertex_program]
def vs_drum {
    f_color = v_color
    gl_Position = v_projection * float4(v_position, 0.0, 1.0)
}

[fragment_program]
def fs_drum {
    f_FragColor = f_color
}

// ─── Vertex ───

[vertex_buffer]
struct Vertex {
    xy   : float2
    rgba : float4
}

// ─── Drawing helpers ───

def push_quad(var verts : array<Vertex>; x0, y0, x1, y1 : float; color : float4) {
    verts |> push(Vertex(xy = float2(x0, y0), rgba = color))
    verts |> push(Vertex(xy = float2(x1, y0), rgba = color))
    verts |> push(Vertex(xy = float2(x1, y1), rgba = color))
    verts |> push(Vertex(xy = float2(x0, y0), rgba = color))
    verts |> push(Vertex(xy = float2(x1, y1), rgba = color))
    verts |> push(Vertex(xy = float2(x0, y1), rgba = color))
}

def push_circle(var verts : array<Vertex>; cx, cy, radius : float; color : float4; segments : int = 48) {
    let step = 2.0 * PI / float(segments)
    verts |> reserve(length(verts) + segments * 3)
    for (i in range(segments)) {
        let a0 = float(i) * step
        let a1 = float(i + 1) * step
        verts |> push(Vertex(xy = float2(cx, cy), rgba = color))
        verts |> push(Vertex(xy = float2(cx + cos(a0) * radius, cy + sin(a0) * radius), rgba = color))
        verts |> push(Vertex(xy = float2(cx + cos(a1) * radius, cy + sin(a1) * radius), rgba = color))
    }
}

// ─── Drum pad definition ───

struct DrumPad {
    name    : string
    title   : string   // short title for display
    sf2_key : int      // GM percussion key number (bank 128)
    cx      : float    // screen position (set at layout time)
    cy      : float
    color   : float4
    flash   : float    // flash timer (1.0 on hit, decays to 0)
}

// ─── SF2 helpers ───

var g_sf2 : SF2File?
var g_sf2_cache : table<int; array<float>>  // sf2_key → pre-rendered stereo PCM

def find_sf2() : string {
    let dir = "{get_das_root()}/examples/daStrudel/midi_sf2_demo"
    var st : FStat
    for (name in fixed_array("Musyng Kite.sf2", "GeneralUser GS v1.471.sf2", "FluidR3_GM.sf2")) {
        if (stat("{dir}/{name}", st)) {
            return "{dir}/{name}"
        }
    }
    return ""
}

def render_sf2_drum(sf2 : SF2File; key, velocity : int; duration : float) : array<float> {
    let sample_rate = 48000
    let total_frames = int(duration * float(sample_rate))
    let note_off_frame = int(duration * 0.7 * float(sample_rate))
    let preset_idx = sf2_find_preset(sf2, 128, 0)
    if (preset_idx < 0) {
        var empty : array<float>
        return <- empty
    }
    let zones = sf2_find_zones(sf2, preset_idx, key, velocity)
    if (length(zones) == 0) {
        var empty : array<float>
        return <- empty
    }
    let preset = sf2.presets[preset_idx]
    var voices : array<ma_sf2_voice>
    var excl_classes : array<int>
    var atten_state = SF2VoiceAttenState()
    var cc_defaults : int[128]
    sf2_default_cc_values(cc_defaults)
    for (z in zones) {
        var pzone : SF2Zone const?
        for (pz in preset.zones) {
            let inst_gen = get_generator(*pz, int(SF2GenOper.instrument), -1)
            if (inst_gen == z.x) {
                pzone = pz
                break
            }
        }
        var cv : ma_sf2_voice
        if (sf2_create_c_voice(sf2, z.x, z.y, pzone, preset.global_zone, key, velocity, 0, cc_defaults, cv, atten_state)) {
            let excl = sf2_get_exclusive_class(sf2, z.x, z.y, pzone, preset.global_zone)
            if (excl > 0) {
                for (i in range(length(voices))) {
                    if (excl_classes[i] == excl && voices[i].finished == 0) {
                        unsafe { ma_sf2_voice_end_quick(addr(voices[i])); }
                    }
                }
            }
            voices |> push(cv)
            excl_classes |> push(excl)
        }
    }
    if (length(voices) == 0) {
        var empty : array<float>
        return <- empty
    }
    var output : array<float>
    output |> resize(total_frames * 2)
    let chunk_size = 512
    var frame = 0
    var note_off_sent = false
    while (frame < total_frames) {
        let frames = min(chunk_size, total_frames - frame)
        if (!note_off_sent && frame >= note_off_frame) {
            for (i in range(length(voices))) {
                unsafe { ma_sf2_voice_note_off(addr(voices[i])); }
            }
            note_off_sent = true
        }
        for (i in range(length(voices))) {
            unsafe {
                ma_sf2_voice_render(addr(voices[i]), addr(sf2.sample_data[0]), length(sf2.sample_data), addr(output[0]), frame * 2, frames)
            }
        }
        frame += frames
    }
    return <- output
}

// ─── Frequency analysis ───

def fft_in_place(var re, im : array<float>) {
    let n = length(re)
    var j = 0
    for (i in range(1, n)) {
        var bit = n >> 1
        while ((j & bit) != 0) {
            j ^= bit
            bit >>= 1
        }
        j ^= bit
        if (i < j) {
            let tmp_r = re[i]; re[i] = re[j]; re[j] = tmp_r
            let tmp_i = im[i]; im[i] = im[j]; im[j] = tmp_i
        }
    }
    var len = 2
    while (len <= n) {
        let half = len / 2
        let angle = -PI * 2.0 / float(len)
        let w_r = cos(angle)
        let w_i = sin(angle)
        var i = 0
        while (i < n) {
            var cur_r = 1.0
            var cur_i = 0.0
            for (k in range(half)) {
                let t_r = cur_r * re[i + half + k] - cur_i * im[i + half + k]
                let t_i = cur_r * im[i + half + k] + cur_i * re[i + half + k]
                re[i + half + k] = re[i + k] - t_r
                im[i + half + k] = im[i + k] - t_i
                re[i + k] += t_r
                im[i + k] += t_i
                let next_r = cur_r * w_r - cur_i * w_i
                cur_i = cur_r * w_i + cur_i * w_r
                cur_r = next_r
            }
            i += len
        }
        len <<= 1
    }
}

def analyze_spectrum(name : string; pcm : array<float>) {
    // mix stereo to mono, take first 4096 samples for FFT
    let fft_size = 4096
    var re : array<float>
    var im : array<float>
    re |> resize(fft_size)
    im |> resize(fft_size)
    let nframes = min(length(pcm) / 2, fft_size)
    for (i in range(nframes)) {
        re[i] = (pcm[i * 2] + pcm[i * 2 + 1]) * 0.5
    }
    fft_in_place(re, im)
    // find top 5 frequency peaks
    let bin_hz = float(SAMPLE_RATE) / float(fft_size)
    let half = fft_size / 2
    // compute magnitudes
    var mags : array<float>
    mags |> resize(half)
    for (i in range(half)) {
        mags[i] = sqrt(re[i] * re[i] + im[i] * im[i])
    }
    // find peaks (local maxima above threshold)
    var max_mag = 0.0
    for (m in mags) {
        max_mag = max(max_mag, m)
    }
    to_log(LOG_INFO, "  {name} spectrum (top peaks, {bin_hz}Hz resolution):\n")
    // simple: find top 8 bins by magnitude
    for (_ in range(8)) {
        var best_bin = 0
        var best_val = 0.0
        for (i in range(1, half)) {
            if (mags[i] > best_val) {
                best_val = mags[i]
                best_bin = i
            }
        }
        if (best_val < max_mag * 0.01) {
            break
        }
        let freq = float(best_bin) * bin_hz
        let db = 20.0 * log(best_val / max_mag + 0.0001) / log(10.0)
        to_log(LOG_INFO, "    {freq}Hz  {db}dB\n")
        // zero out this peak and neighbors so we find the next one
        for (k in range(max(0, best_bin - 2), min(half, best_bin + 3))) {
            mags[k] = 0.0
        }
    }
}

def measure_audible_duration(pcm : array<float>) : float {
    // find last sample above threshold (stereo interleaved)
    let threshold = 0.001
    var last_frame = 0
    let nframes = length(pcm) / 2
    for (i in range(nframes)) {
        let l = abs(pcm[i * 2])
        let r = abs(pcm[i * 2 + 1])
        if (l > threshold || r > threshold) {
            last_frame = i
        }
    }
    return float(last_frame) / float(SAMPLE_RATE)
}

def prerender_sf2_drums(sf2 : SF2File) {
    // pre-render all drum sounds from SF2 with generous duration, then measure
    let keys = fixed_array(36, 38, 42, 46, 39, 41, 50, 51, 49, 37, 56, 54)
    let names = fixed_array("BD", "SD", "HH", "OH", "CP", "TomL", "TomH", "Ride", "Crash", "Rim", "CB", "Tamb")
    let render_dur = fixed_array(2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 5.0, 5.0, 2.0, 2.0, 2.0)
    to_log(LOG_INFO, "SF2 drum durations:\n")
    for (i in range(length(keys))) {
        var pcm <- render_sf2_drum(sf2, keys[i], 127, render_dur[i])
        if (length(pcm) > 0) {
            let dur = measure_audible_duration(pcm)
            to_log(LOG_INFO, "  {names[i]} (key {keys[i]}): {dur}s\n")
            analyze_spectrum(names[i], pcm)
            g_sf2_cache[keys[i]] <- pcm
        } else {
            to_log(LOG_INFO, "  {names[i]} (key {keys[i]}): NO SOUND\n")
            delete pcm
        }
    }
}

def analyze_synth_drums() {
    to_log(LOG_INFO, "Synth drum spectra:\n")
    let names = fixed_array("bd", "sd", "hh", "oh", "cp", "tom_low", "tom_high", "ride", "crash", "rimshot", "cowbell", "tambourine")
    for (name in names) {
        var pcm <- render_synth_drum(name)
        analyze_spectrum(name, pcm)
        delete pcm
    }
}

// ─── Synth drum rendering ───

def render_synth_drum(name : string) : array<float> {
    var mono : array<float>
    if (name == "bd") {
        mono <- render_bd(0.5, 1.0)
    } elif (name == "sd") {
        mono <- render_sd(0.5, 1.0)
    } elif (name == "hh") {
        mono <- render_hh(0.5, 1.0)
    } elif (name == "oh") {
        mono <- render_oh(0.6, 1.0)
    } elif (name == "cp") {
        mono <- render_cp(0.5, 1.0)
    } elif (name == "tom_low") {
        mono <- render_tom(0.5, 1.0, 80.0)
    } elif (name == "tom_high") {
        mono <- render_tom(0.4, 1.0, 175.0)
    } elif (name == "ride") {
        mono <- render_ride(1.5, 1.0)
    } elif (name == "crash") {
        mono <- render_crash(1.5, 1.0)
    } elif (name == "rimshot") {
        mono <- render_rimshot(0.15, 1.0)
    } elif (name == "cowbell") {
        mono <- render_cowbell(0.25, 1.0)
    } elif (name == "tambourine") {
        mono <- render_tambourine(0.4, 1.0)
    } else {
        mono |> resize(int(0.05 * float(SAMPLE_RATE)))
    }
    return <- mono_to_stereo(mono)
}

// ─── Globals ───

var g_pads : array<DrumPad>
var g_use_sf2 = false
var g_has_sf2 = false
var g_program : uint
var g_vao : uint
var g_vbo : uint
var g_font : Font
var g_window : GLFWwindow?

let PAD_RADIUS = 50.0
let PAD_COLORS = fixed_array(
    float4(0.9, 0.3, 0.2, 1.0),   // bd - red
    float4(0.9, 0.7, 0.2, 1.0),   // sd - yellow
    float4(0.5, 0.8, 0.9, 1.0),   // hh - cyan
    float4(0.3, 0.7, 0.9, 1.0),   // oh - blue
    float4(0.9, 0.4, 0.6, 1.0),   // cp - pink
    float4(0.6, 0.4, 0.2, 1.0),   // tom_low - brown
    float4(0.8, 0.5, 0.3, 1.0),   // tom_high - orange
    float4(0.7, 0.7, 0.3, 1.0),   // ride - olive
    float4(0.9, 0.9, 0.3, 1.0),   // crash - bright yellow
    float4(0.5, 0.5, 0.5, 1.0),   // rimshot - gray
    float4(0.8, 0.6, 0.0, 1.0),   // cowbell - gold
    float4(0.4, 0.8, 0.4, 1.0)    // tambourine - green
)

def init_pads() {
    let names = fixed_array("bd", "sd", "hh", "oh", "cp", "tom_low", "tom_high", "ride", "crash", "rimshot", "cowbell", "tambourine")
    let titles = fixed_array("BD", "SD", "HH", "OH", "CP", "TomL", "TomH", "Ride", "Crash", "Rim", "CB", "Tamb")
    let keys = fixed_array(36, 38, 42, 46, 39, 41, 50, 51, 49, 37, 56, 54)
    g_pads |> reserve(12)
    for (i in range(12)) {
        let row = i / 5
        let col = i % 5
        let cx = 140.0 + float(col) * 140.0
        let cy = 150.0 + float(row) * 150.0
        g_pads |> push(DrumPad(
            name = names[i],
            title = titles[i],
            sf2_key = keys[i],
            cx = cx,
            cy = cy,
            color = PAD_COLORS[i],
            flash = 0.0
        ))
    }
}

def play_drum(pad_index : int) {
    if (pad_index < 0 || pad_index >= length(g_pads)) {
        return
    }
    g_pads[pad_index].flash = 1.0
    if (g_use_sf2 && g_has_sf2) {
        // play from SF2 cache
        let key = g_pads[pad_index].sf2_key
        if (key_exists(g_sf2_cache, key)) {
            var copy : array<float>
            copy := g_sf2_cache[key]
            play_sound_from_pcm(48000, 2, copy)
        }
    } else {
        // play synth
        var pcm <- render_synth_drum(g_pads[pad_index].name)
        play_sound_from_pcm(48000, 2, pcm)
    }
}

def hit_test(mx, my : float) : int {
    for (i in range(length(g_pads))) {
        let dx = mx - g_pads[i].cx
        let dy = my - g_pads[i].cy
        if (dx * dx + dy * dy < PAD_RADIUS * PAD_RADIUS) {
            return i
        }
    }
    return -1
}

// ─── Main ───

[export]
def main {
    if (glfwInit() == 0) {
        panic("can't init glfw")
    }
    defer() {
        glfwTerminate()
    }
    glfwInitOpenGL(3, 3)
    g_window = glfwCreateWindow(860, 600, "Drum Compare: Synth vs SF2", null, null)
    if (g_window == null) {
        panic("can't create window")
    }
    defer() {
        glfwDestroyWindow(g_window)
    }
    glfwMakeContextCurrent(g_window)

    // shaders
    g_program = create_shader_program(@@vs_drum, @@fs_drum)
    glGenVertexArrays(1, safe_addr(g_vao))
    glBindVertexArray(g_vao)
    glGenBuffers(1, safe_addr(g_vbo))
    glBindBuffer(GL_ARRAY_BUFFER, g_vbo)
    bind_vertex_buffer(null, type<Vertex>)

    // font
    create_ttf_objects()
    g_font <- load_ttf("{get_das_root()}/examples/opengl/droidsansmono.ttf", [pixel_height = 18.0, oversampling = 2])
    if (!!empty(g_font.bitmap.bytes)) {
        g_font <- load_ttf("{get_das_root()}/modules/dasStbImage/fonts/droidsansmono.ttf", [pixel_height = 18.0, oversampling = 2])
    }
    if (!empty(g_font.bitmap.bytes)) {
        upload_font_texture(g_font)
    }

    // pads
    init_pads()

    // mouse callback
    unsafe {
        glfwSetMouseButtonCallback(g_window) <| @(w, button, action, mods) {
            if (button == int(GLFW_MOUSE_BUTTON_1) && action == int(GLFW_PRESS)) {
                let cxy = glfwGetCursorPos(g_window)
                let mx = float(cxy.x)
                let my = float(cxy.y)
                let pad = hit_test(mx, my)
                if (pad >= 0) {
                    play_drum(pad)
                }
                // check mode toggle button area (bottom center)
                let btn_x = 430.0
                let btn_y = 560.0
                if (abs(mx - btn_x) < 100.0 && abs(my - btn_y) < 25.0 && g_has_sf2) {
                    g_use_sf2 = !g_use_sf2
                }
            }
        }
    }

    // audio + SF2
    with_audio_system() {
        let sf2_path = find_sf2()
        if (!empty(sf2_path)) {
            g_sf2 = sf2_load(sf2_path)
            if (g_sf2 != null) {
                to_log(LOG_INFO, "Loaded SF2: {sf2_path}\n")
                prerender_sf2_drums(*g_sf2)
                g_has_sf2 = true
                g_use_sf2 = false  // start in synth mode
                to_log(LOG_INFO, "Pre-rendered {length(g_sf2_cache)} SF2 drum sounds\n")
                analyze_synth_drums()
            }
        }
        if (!g_has_sf2) {
            to_log(LOG_INFO, "No SF2 found - synth mode only\n")
        }

        to_log(LOG_INFO, "Drum Compare — click pads to play, S to toggle synth/SF2, ESC to quit\n")

        var last_time = glfwGetTime()

        eval_main_loop() {
            if (glfwWindowShouldClose(g_window) != 0) {
                return false
            }
            if (glfwGetKey(g_window, int(GLFW_KEY_ESCAPE)) == int(GLFW_PRESS)) {
                return false
            }
            // S key toggles mode
            if (glfwGetKey(g_window, int(GLFW_KEY_S)) == int(GLFW_PRESS) && g_has_sf2) {
                g_use_sf2 = !g_use_sf2
                sleep(150u)  // debounce
            }
            // keyboard shortcuts: 1-9, 0, -, = for pads 0-11
            let key_map = fixed_array(
                int(GLFW_KEY_1), int(GLFW_KEY_2), int(GLFW_KEY_3), int(GLFW_KEY_4), int(GLFW_KEY_5),
                int(GLFW_KEY_6), int(GLFW_KEY_7), int(GLFW_KEY_8), int(GLFW_KEY_9), int(GLFW_KEY_0),
                int(GLFW_KEY_MINUS), int(GLFW_KEY_EQUAL)
            )
            for (i in range(12)) {
                if (glfwGetKey(g_window, key_map[i]) == int(GLFW_PRESS)) {
                    play_drum(i)
                    sleep(100u)  // debounce
                }
            }

            glfwPollEvents()

            // delta time for flash decay
            let now = glfwGetTime()
            let dt = float(now - last_time)
            last_time = now

            // decay flash
            for (pad in g_pads) {
                pad.flash = max(0.0, pad.flash - dt * 4.0)
            }

            // render
            var dw, dh : int
            glfwGetFramebufferSize(g_window, dw, dh)
            glViewport(0, 0, dw, dh)
            glClearColor(0.12, 0.12, 0.14, 1.0)
            glClear(GL_COLOR_BUFFER_BIT)

            glEnable(GL_BLEND)
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)

            // orthographic projection (pixel coordinates, Y down from top)
            let fw = float(dw)
            let fh = float(dh)
            v_projection = ortho_rh(0.0, fw, fh, 0.0, -1.0, 1.0)

            glUseProgram(g_program)
            vs_drum_bind_uniform(g_program)

            var verts : array<Vertex>
            verts |> reserve(12 * 48 * 3 + 100)

            // draw pads
            for (pad in g_pads) {
                // flash brightens the color
                let bright = 1.0 + pad.flash * 0.8
                let c = float4(
                    min(pad.color.x * bright, 1.0),
                    min(pad.color.y * bright, 1.0),
                    min(pad.color.z * bright, 1.0),
                    pad.color.w
                )
                push_circle(verts, pad.cx, pad.cy, PAD_RADIUS, c, 48)
                // dark outline
                let outline = float4(pad.color.x * 0.4, pad.color.y * 0.4, pad.color.z * 0.4, 1.0)
                push_circle(verts, pad.cx, pad.cy, PAD_RADIUS + 3.0, outline, 48)
                // redraw pad on top (simple outline effect)
                push_circle(verts, pad.cx, pad.cy, PAD_RADIUS, c, 48)
            }

            // mode toggle button background
            let btn_cx = fw * 0.5
            let btn_cy = fh - 50.0
            let mode_color = g_use_sf2 ? float4(0.2, 0.5, 0.8, 1.0) : float4(0.3, 0.7, 0.3, 1.0)
            push_quad(verts, btn_cx - 100.0, btn_cy - 20.0, btn_cx + 100.0, btn_cy + 20.0, mode_color)

            // upload and draw
            glBindVertexArray(g_vao)
            glBindBuffer(GL_ARRAY_BUFFER, g_vbo)
            glBufferData(GL_ARRAY_BUFFER, verts, GL_DYNAMIC_DRAW)
            glDrawArrays(GL_TRIANGLES, 0, length(verts))
            delete verts

            // draw text titles
            if (!empty(g_font.bitmap.bytes)) {
                for (pad in g_pads) {
                    var lbl <- g_font |> create_quads(pad.title)
                    let dim = quads_dim(lbl)
                    let text_w = dim.vmax.x - dim.vmin.x
                    let sx = int(pad.cx - text_w * 0.5)
                    let sy = int(pad.cy + PAD_RADIUS + 8.0)
                    g_font |> draw_quads_2d(lbl, dw, dh, sx, sy, float3(0.8, 0.8, 0.85))
                    delete lbl
                }
                // mode title
                let mode_text = g_use_sf2 ? "SF2 (Musyng Kite)" : "SYNTH (808-style)"
                var mode_lbl <- g_font |> create_quads(mode_text)
                let mode_dim = quads_dim(mode_lbl)
                let mode_tw = mode_dim.vmax.x - mode_dim.vmin.x
                let mode_sx = int(btn_cx - mode_tw * 0.5)
                let mode_sy = int(btn_cy - 7.0)
                g_font |> draw_quads_2d(mode_lbl, dw, dh, mode_sx, mode_sy, float3(1.0, 1.0, 1.0))
                delete mode_lbl
                // instructions
                let help_text = g_has_sf2 ? "Click pads | Keys 1-9,0,-,= | S=toggle mode | ESC=quit" : "Click pads | Keys 1-9,0,-,= | ESC=quit"
                var help_lbl <- g_font |> create_quads(help_text)
                let help_dim = quads_dim(help_lbl)
                let help_tw = help_dim.vmax.x - help_dim.vmin.x
                g_font |> draw_quads_2d(help_lbl, dw, dh, int(fw * 0.5 - help_tw * 0.5), int(fh - 10.0), float3(0.5, 0.5, 0.55))
                delete help_lbl
            }

            glfwSwapBuffers(g_window)
            return true
        }
    }

    // cleanup
    if (g_sf2 != null) {
        unsafe { delete g_sf2; }
    }
    glDeleteVertexArrays(1, safe_addr(g_vao))
    glDeleteBuffers(1, safe_addr(g_vbo))
}
