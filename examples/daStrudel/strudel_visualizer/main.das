options gen2
options indenting = 4

require live/glfw_live
require opengl/opengl_boost
require opengl/opengl_ttf
require live/opengl_live
require live/live_commands
require live/live_api
require live/live_watch_boost
require live_host
require daslib/math_boost
require daslib/safe_addr
require audio/audio_boost
require live/audio_live
require strudel/strudel
require strudel/strudel_player
require strudel/strudel_midi_player
require daslib/fio
require daslib/jobque_boost
require daslib/json
require daslib/json_boost
require audio_analysis
require vis_renderer

// USE_SF2 — toggle between two instrument backends:
//   true  = SF2 soundfont (GM percussion bank 128, piano, acoustic bass, string ensemble)
//           Requires a .sf2 file in examples/daStrudel/midi_sf2_demo/
//           Falls back to false at runtime if no SF2 is found
//   false = built-in synth only (bd/sd/hh/cp drums, sawtooth lead, sine bass)
//           No external files needed — works out of the box

var USE_SF2 = true

// USE_MIDI — toggle between two visualization modes:
//   true  = MIDI file playback with vectorscope (Fur Elise via SF2)
//           No strudel patterns — just MIDI player + single vectorscope panel
//   false = strudel pattern engine with 5 tracks (drums/piano/bass/strings/violin)
//           Each track gets its own visualization panel type
let USE_MIDI = false

// --- Shaders ---

var @in @location v_position : float2
var @in @location v_color : float4
var @inout f_color : float4
var @uniform v_projection : float4x4
var @out f_FragColor : float4

[vertex_program]
def vs_pianoroll {
    f_color = v_color
    gl_Position = v_projection * float4(v_position, 0.0, 1.0)
}

[fragment_program]
def fs_pianoroll {
    f_FragColor = f_color
}

// --- GL objects ---

var program : uint
var vao : uint
var vbo : uint

// --- Panel system ---

enum PanelKind {
    pianoroll_panel
    scope_panel
    spectrum_panel
    drums_panel
    vectorscope_panel
}

struct Panel {
    kind           : PanelKind
    name           : string
    color          : uint = 0xFFCA28u
    tap_index      : int = -1   // index into render_states (pianoroll)
    analysis_index : int = -1   // index into audio_analyses (scope/spectrum)
}

// --- Globals ---

var render_states : array<PianorollRenderState>
var audio_analyses : array<AudioAnalysis>
var panels : array<Panel>
var font : Font
var g_window_cycles : float = 4.0
var g_cps : double = 0.5lf

// --- Panel registration ---

def add_pianoroll_panel(tap_idx : int; name : string; color : uint) {
    panels |> emplace(Panel(kind = PanelKind.pianoroll_panel, name = name, color = color, tap_index = tap_idx))
}

def add_scope_panel(name : string; color : uint; analysis_idx : int) {
    panels |> emplace(Panel(kind = PanelKind.scope_panel, name = name, color = color, analysis_index = analysis_idx))
}

def add_spectrum_panel(name : string; color : uint; analysis_idx : int) {
    panels |> emplace(Panel(kind = PanelKind.spectrum_panel, name = name, color = color, analysis_index = analysis_idx))
}

def add_vectorscope_panel(name : string; color : uint; analysis_idx : int) {
    panels |> emplace(Panel(kind = PanelKind.vectorscope_panel, name = name, color = color, analysis_index = analysis_idx))
}

var drum_panel_states : array<DrumPanelState>

def add_drums_panel(tap_idx : int; name : string; color : uint) {
    let dps_idx = length(drum_panel_states)
    drum_panel_states |> emplace(DrumPanelState())
    panels |> emplace(Panel(kind = PanelKind.drums_panel, name = name, color = color, tap_index = tap_idx, analysis_index = dps_idx))
}

// Create per-track AudioAnalysis, returns its index
def create_audio_analysis(var ch : Channel?) : int {
    let idx = length(audio_analyses)
    var aa = AudioAnalysis()
    aa.channel = ch
    audio_analysis_init(aa)
    audio_analyses |> emplace(aa)
    return idx
}

bitfield VisPanels {
    pianoroll,
    scope,
    spectrum,
    drums,
    vectorscope,
    VIS_ALL = VisPanels.pianoroll | VisPanels.scope | VisPanels.spectrum | VisPanels.drums | VisPanels.vectorscope
}

// Register a strudel track with selected visualization panels.
def register_track(name : string; color : uint = 0xFFCA28u; panels : VisPanels = VisPanels.VIS_ALL) {
    let vis = create_track_vis(name, color)
    var audio_ch = track_vis_audio_channel(vis)
    let aa = create_audio_analysis(audio_ch)
    if (panels.pianoroll) {
        add_pianoroll_panel(track_vis_tap_index(vis), name, color)
    }
    if (panels.scope) {
        add_scope_panel("{name} scope", color, aa)
    }
    if (panels.spectrum) {
        add_spectrum_panel("{name} spectrum", color, aa)
    }
    if (panels.drums) {
        add_drums_panel(track_vis_tap_index(vis), name, color)
    }
    if (panels.vectorscope) {
        add_vectorscope_panel("{name} vectorscope", color, aa)
    }
}

var g_extra_channels : array<Channel?>

// Register a MIDI track with selected panels (no pianoroll).
def register_midi_track(name : string; color : uint = 0xE040FFu; panels : VisPanels = VisPanels.scope | VisPanels.spectrum) {
    var ch = unsafe(channel_create())
    ch |> add_ref()
    let aa = create_audio_analysis(ch)
    if (panels.scope) {
        add_scope_panel("{name} scope", color, aa)
    }
    if (panels.spectrum) {
        add_spectrum_panel("{name} spectrum", color, aa)
    }
    if (panels.vectorscope) {
        add_vectorscope_panel("{name} vectorscope", color, aa)
    }
    midi_set_audio_vis_channel(ch)
    g_extra_channels |> push(ch)
}

// --- GL setup ---

def create_gl_objects() {
    program = create_shader_program(@@vs_pianoroll, @@fs_pianoroll)
    glGenVertexArrays(1, safe_addr(vao))
    glBindVertexArray(vao)
    glGenBuffers(1, safe_addr(vbo))
    glBindBuffer(GL_ARRAY_BUFFER, vbo)
    bind_vertex_buffer(null, type<VisVertex>)
}

def delete_gl_objects() {
    if (vao != 0u) {
        glDeleteVertexArrays(1, safe_addr(vao))
        vao = 0u
    }
    if (vbo != 0u) {
        glDeleteBuffers(1, safe_addr(vbo))
        vbo = 0u
    }
}

// --- Strudel thread ---

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

def strudel_main(var state : StrudelState?) {
    if (USE_SF2) {
        // drums — SF2 GM percussion (bank 128)
        var drums <- note_pattern("c2 ~ [~ c2] ~, ~ eb2 ~ eb2, [~ fs2] [~ fs2] [~ fs2] [~ fs2]")
        drums <- drums |> set_sf2(0) |> set_sf2_bank(128) |> set_gain(0.7) |> pianoroll(state, "drums")
        strudel_add_track(state, "drums", drums)
        // piano arpeggios
        var piano <- note_pattern("<[a3 c4 e4 a4 e4 c4 a3 c4] [f3 a3 c4 f4 c4 a3 f3 a3] [c3 e3 g3 c4 g3 e3 c3 e3] [g3 b3 d4 g4 d4 b3 g3 b3]>")
        piano <- piano |> set_sf2("piano") |> set_gain(0.45) |> pianoroll(state, "piano")
        strudel_add_track(state, "piano", piano)
        // bass
        var bass <- note_pattern("<[a2 ~ ~ a2 ~ a2 ~ ~] [f2 ~ ~ f2 ~ f2 ~ ~] [c2 ~ ~ c2 ~ c2 ~ ~] [g2 ~ ~ g2 ~ g2 ~ ~]>")
        bass <- bass |> set_sf2("acoustic_bass") |> set_gain(0.6) |> pianoroll(state, "bass")
        strudel_add_track(state, "bass", bass)
        // string pad
        var strings <- note_pattern("<[a2,c3,e3] [f2,a2,c3] [c2,e2,g2] [g2,b2,d3]>")
        strings <- strings |> set_sf2("string_ensemble") |> set_gain(0.2) |> pianoroll(state, "strings")
        strudel_add_track(state, "strings", strings)
        // violin — sparse lyrical melody
        var violin <- note_pattern("<e4 ~ ~ a4> <~ d5 ~ ~> <g4 ~ f4 ~> <~ ~ b4 ~>")
        violin <- violin |> set_sf2("violin") |> set_gain(0.9) |> pianoroll(state, "violin")
        strudel_add_track(state, "violin", violin)
    } else {
        // drums — built-in synth drums
        let drums <- s("bd [hh hh] sd [hh cp]") |> pianoroll(state, "drums")
        strudel_add_track(state, "drums", drums)
        // piano arpeggios — sawtooth
        var piano <- note_pattern("<[a3 c4 e4 a4 e4 c4 a3 c4] [f3 a3 c4 f4 c4 a3 f3 a3] [c3 e3 g3 c4 g3 e3 c3 e3] [g3 b3 d4 g4 d4 b3 g3 b3]>", "sawtooth")
        piano <- piano |> set_gain(0.3) |> pianoroll(state, "piano")
        strudel_add_track(state, "piano", piano)
        // bass — sine
        var bass <- note_pattern("<[a2 ~ ~ a2 ~ a2 ~ ~] [f2 ~ ~ f2 ~ f2 ~ ~] [c2 ~ ~ c2 ~ c2 ~ ~] [g2 ~ ~ g2 ~ g2 ~ ~]>", "sine")
        bass <- bass |> set_gain(0.4) |> pianoroll(state, "bass")
        strudel_add_track(state, "bass", bass)
        // string pad — sawtooth (detuned chords)
        var strings <- note_pattern("<[a2,c3,e3] [f2,a2,c3] [c2,e2,g2] [g2,b2,d3]>", "sawtooth")
        strings <- strings |> set_gain(0.15) |> pianoroll(state, "strings")
        strudel_add_track(state, "strings", strings)
        // violin — sparse lyrical melody (sawtooth)
        var violin <- note_pattern("<e4 ~ ~ a4> <~ d5 ~ ~> <g4 ~ f4 ~> <~ ~ b4 ~>", "sawtooth")
        violin <- violin |> set_gain(0.1) |> pianoroll(state, "violin")
        strudel_add_track(state, "violin", violin)
    }
    strudel_set_bpm(state, 120.0lf)
    strudel_play(state, @@strudel_on_command)
}

def strudel_on_command(var state : StrudelState?; cmd : string) {
    pass
}

// --- Live commands ---

[live_command(description="Dump debug info")]
def cmd_debug(input : JsonValue?) : JsonValue? {
    var info = "tracks: {num_track_vis()}, panels: {length(panels)}\n"
    for (i in range(length(render_states))) {
        var rs & = unsafe(render_states[i])
        info += "  pianoroll[{i}] \"{rs.name}\": {length(rs.haps)} haps, drum={rs.is_drum_mode}\n"
    }
    for (i in range(length(audio_analyses))) {
        var aa & = unsafe(audio_analyses[i])
        info += "  audio[{i}]: ring_write={aa.ring_write}\n"
    }
    let pt = strudel_get_playback_time()
    info += "playback: wall_time={pt._0}, cps={pt._1}, cycle={pt._0 * pt._1}\n"
    return JV(info)
}

// --- Lifecycle ---

[export]
def init() {
    live_create_window("daStrudel Visualizer", 1280, 720)
    create_gl_objects()
    create_ttf_objects()
    font <- load_ttf("{get_das_root()}/examples/opengl/droidsansmono.ttf", [pixel_height = 16.0, oversampling = 2])
    if (!!empty(font.bitmap.bytes)) {
        font <- load_ttf("{get_das_root()}/examples/opengl/media/droidsansmono.ttf", [oversampling = 2])
    }
    if (!!empty(font.bitmap.bytes)) {
        panic("can't load font")
    }
    upload_font_texture(font)
    // audio
    if (get_audio_command_channel() == null) {
        audio_system_create()
    }
    // load SF2 soundfont
    if (USE_SF2) {
        let sf2_path = find_sf2()
        if (!empty(sf2_path) && strudel_load_sf2(sf2_path)) {
            print("Loaded SF2: {sf2_path}\n")
        } else {
            print("No SF2 found, using regular strudel patterns\n")
            USE_SF2 = false
        }
    }
    visualization_enable()
    strudel_set_look_ahead(4.0) // 4 seconds ahead for pianoroll future display

    if (USE_MIDI) {
        // MIDI track — vectorscope
        register_midi_track("midi", 0x80CBC4u, VisPanels.vectorscope)
        midi_init()
        let sf2_for_midi = find_sf2()
        if (!empty(sf2_for_midi)) {
            midi_load_sf2(sf2_for_midi)
        }
        let media = "{get_das_root()}/examples/media"
        midi_play("music", "{media}/midi/fur_elise.mid", [gain = 0.5, looping = true])
    } else {
        // register tracks — creates channels, audio analysis, and panels
        register_track("drums", 0xFF5722u, VisPanels.drums)
        register_track("piano", 0x42A5F5u, VisPanels.pianoroll)
        register_track("bass", 0x8BC34Au, VisPanels.scope)
        register_track("strings", 0xE040FFu, VisPanels.vectorscope)
        register_track("violin", 0xF06292u, VisPanels.spectrum)
        strudel_init(@@strudel_main, @@strudel_on_command)
    }
}

[export]
def update() {
    if (!live_begin_frame()) {
        return
    }
    var display_w, display_h : int
    live_get_framebuffer_size(display_w, display_h)
    let pt = strudel_get_playback_time()
    g_cps = pt._1
    let current_cycle = float(pt._0 * pt._1)
    // update data
    let dt = get_dt()
    pianoroll_update(render_states, current_cycle, g_window_cycles)
    for (i in range(length(audio_analyses))) {
        var aa & = unsafe(audio_analyses[i])
        audio_analysis_update(aa)
    }
    for (pi in range(length(panels))) {
        let p & = unsafe(panels[pi])
        if (p.kind == PanelKind.drums_panel && p.analysis_index >= 0 && p.analysis_index < length(drum_panel_states)) {
            drum_panel_update(drum_panel_states[p.analysis_index], render_states, p.tap_index, current_cycle, dt)
        }
    }
    // render
    glViewport(0, 0, display_w, display_h)
    glClearColor(0.12, 0.12, 0.14, 1.0)
    glClear(GL_COLOR_BUFFER_BIT)
    glEnable(GL_BLEND)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
    let num_panels = length(panels)
    if (num_panels > 0) {
        let fw = float(display_w)
        let fh = float(display_h)
        let panel_h = fh / float(num_panels)
        let m = 4.0
        var verts : array<VisVertex>
        for (pi in range(num_panels)) {
            let p & = unsafe(panels[pi])
            let py1 = fh - panel_h * float(pi)
            let py0 = fh - panel_h * float(pi + 1)
            let iy0 = py0 + m
            let iy1 = py1 - m
            let col = uint_to_rgba(p.color)
            if (p.kind == PanelKind.pianoroll_panel) {
                pianoroll_render_panel(verts, render_states, p.tap_index, current_cycle, g_window_cycles, display_w, iy0, iy1)
            } elif (p.kind == PanelKind.scope_panel && p.analysis_index >= 0 && p.analysis_index < length(audio_analyses)) {
                let aa & = unsafe(audio_analyses[p.analysis_index])
                scope_render(verts, aa, m, iy0, fw - m, iy1, col)
            } elif (p.kind == PanelKind.spectrum_panel && p.analysis_index >= 0 && p.analysis_index < length(audio_analyses)) {
                let aa & = unsafe(audio_analyses[p.analysis_index])
                spectrum_render(verts, aa, m, iy0, fw - m, iy1, col)
            } elif (p.kind == PanelKind.drums_panel && p.analysis_index >= 0 && p.analysis_index < length(drum_panel_states)) {
                drum_panel_render(verts, drum_panel_states[p.analysis_index], m, iy0, fw - m, iy1, col)
            } elif (p.kind == PanelKind.vectorscope_panel && p.analysis_index >= 0 && p.analysis_index < length(audio_analyses)) {
                let aa & = unsafe(audio_analyses[p.analysis_index])
                vectorscope_render(verts, aa, m, iy0, fw - m, iy1, col)
            }
            // separator
            if (pi > 0) {
                push_quad(verts, 0.0, py1 - 1.0, fw, py1 + 1.0, float4(0.5, 0.5, 0.5, 0.5))
            }
        }
        if (length(verts) > 0) {
            let projection = ortho_rh(0.0, fw, 0.0, fh, -1.0, 1.0)
            glUseProgram(program)
            v_projection = projection
            vs_pianoroll_bind_uniform(program)
            fs_pianoroll_bind_uniform(program)
            glBindVertexArray(vao)
            glBindBuffer(GL_ARRAY_BUFFER, vbo)
            glBufferData(GL_ARRAY_BUFFER, verts, GL_DYNAMIC_DRAW)
            glDrawArrays(GL_TRIANGLES, 0, length(verts))
        }
        // panel name labels (centered)
        for (pi in range(num_panels)) {
            let p & = unsafe(panels[pi])
            let py1 = fh - panel_h * float(pi)
            let py0 = fh - panel_h * float(pi + 1)
            render_panel_name(font, p.name, display_w, display_h, py0 + m, py1 - m)
        }
    }
    live_end_frame()
}

[export]
def shutdown() {
    midi_shutdown()
    strudel_shutdown()
    destroy_all_track_vis()
    for (ch in g_extra_channels) {
        if (ch != null) {
            ch |> release()
        }
    }
    delete g_extra_channels
    delete render_states
    delete audio_analyses
    delete drum_panel_states
    delete panels
    delete_gl_objects()
    live_destroy_window()
}

// --- Standalone mode ---

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
