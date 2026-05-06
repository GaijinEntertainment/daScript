options gen2
options persistent_heap

require glfw/glfw_boost
require live/glfw_live
require opengl/opengl_boost
require opengl/opengl_gen
require opengl/opengl_cache
require daslib/defer
require daslib/safe_addr
require daslib/decs_boost
require live/decs_live
require live/opengl_live
require live/live_commands
require live/live_api
require live/live_watch_boost
require live/live_vars
require live/audio_live
require daslib/json
require daslib/json_boost
require daslib/math_boost
require daslib/archive
require opengl/opengl_ttf
require audio/audio_boost
require strudel/strudel
require strudel/strudel_player
require strudel/strudel_live
require daslib/strings_boost
require live_host

// ===== Constants =====

let CELL_SIZE   = 1.0          // world units per maze cell
let MAZE_COLS   = 21
let MAZE_ROWS   = 23
let HALF_W      = float(MAZE_COLS) * CELL_SIZE * 0.5
let HALF_H      = float(MAZE_ROWS) * CELL_SIZE * 0.5

let PAC_SPEED       = 4.5      // cells/sec (level 1 base)
let GHOST_SPEED     = 3.2
let GHOST_FLEE_SPEED = 2.0
let POWER_DURATION  = 8.0

// Level-scaled difficulty helpers
def eff_pac_speed()        { return min(PAC_SPEED       + float(level - 1) * 0.15, 7.5); }
def eff_ghost_speed()      { return min(GHOST_SPEED     + float(level - 1) * 0.12, 5.5); }
def eff_ghost_flee_speed() { return min(GHOST_FLEE_SPEED + float(level - 1) * 0.08, 4.0); }
def eff_power_duration()   { return max(POWER_DURATION  - float(level - 1) * 0.7, 2.0); }

let PELLET_SCORE      = 10
let POWER_SCORE       = 50
let GHOST_EAT_SCORE   = 200
let START_LIVES       = 3

let WALL_COLOR    = float3(0.1, 0.2, 0.9)
let FLOOR_COLOR   = float3(0.05, 0.05, 0.12)
let PAC_COLOR     = float3(1.0, 0.9, 0.1)
let PELLET_COLOR  = float3(0.9, 0.9, 0.9)
let POWER_COLOR   = float3(1.0, 0.5, 0.0)

let GHOST_COLORS = fixed_array(
    float3(1.0, 0.2, 0.2),   // Blinky — red
    float3(1.0, 0.6, 0.8),   // Pinky
    float3(0.2, 0.8, 1.0),   // Inky
    float3(1.0, 0.6, 0.0)    // Clyde — orange
)

let FLEE_COLOR  = float3(0.1, 0.1, 0.8)
let LIGHT_DIR   = normalize(float3(-1.0, -2.0, 2.0))

// ===== Maze layout =====
// 1 = wall, 0 = open, 2 = pellet, 3 = power pellet, 4 = ghost house, 5 = ghost door

let MAZE_DATA = fixed_array(
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
    1, 3, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 3, 1,
    1, 2, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 2, 1,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
    1, 2, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 2, 1,
    1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1,
    1, 1, 1, 1, 2, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 2, 1, 1, 1, 1,
    1, 1, 1, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 1, 1, 1,
    1, 1, 1, 1, 2, 1, 0, 1, 1, 4, 4, 4, 1, 1, 0, 1, 2, 1, 1, 1, 1,
    0, 0, 0, 0, 2, 0, 0, 1, 4, 4, 4, 4, 4, 1, 0, 0, 2, 0, 0, 0, 0,
    1, 1, 1, 1, 2, 1, 0, 1, 1, 1, 5, 1, 1, 1, 0, 1, 2, 1, 1, 1, 1,
    1, 1, 1, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 1, 1, 1,
    1, 1, 1, 1, 2, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 2, 1, 1, 1, 1,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
    1, 2, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 2, 1,
    1, 3, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 3, 1,
    1, 1, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 2, 1, 1,
    1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1,
    1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
    1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
)

def cell_at(col, row : int) : int {
    if (col < 0 || col >= MAZE_COLS || row < 0 || row >= MAZE_ROWS) {
        return 1
    }
    return MAZE_DATA[row * MAZE_COLS + col]
}

def is_wall(col, row : int) : bool {
    let c = cell_at(col, row)
    return c == 1
}

def world_to_cell(wx, wy : float) : int2 {
    return int2(
        int(floor((wx + HALF_W) / CELL_SIZE)),
        int(floor((wy + HALF_H) / CELL_SIZE))
    )
}

def cell_to_world(col, row : int) : float2 {
    return float2(
        float(col) * CELL_SIZE - HALF_W + CELL_SIZE * 0.5,
        float(row) * CELL_SIZE - HALF_H + CELL_SIZE * 0.5
    )
}

// ===== Audio =====

let AUDIO_RATE = 48000
let AUDIO_CHANNELS = 1

def gen_sine_sweep(freq_start, freq_end : float; duration : float; volume : float = 0.3) : array<float> {
    let num_samples = int(float(AUDIO_RATE) * duration)
    var samples : array<float>
    samples |> resize(num_samples)
    for (i in range(num_samples)) {
        let t = float(i) / float(AUDIO_RATE)
        let frac = t / duration
        let freq = freq_start + (freq_end - freq_start) * frac
        let envelope = 1.0 - frac
        samples[i] = sin(2.0 * PI * freq * t) * envelope * volume
    }
    return <- samples
}

def gen_noise_burst(duration : float; volume : float = 0.2) : array<float> {
    let num_samples = int(float(AUDIO_RATE) * duration)
    var samples : array<float>
    samples |> resize(num_samples)
    var seed = 12345u
    for (i in range(num_samples)) {
        let frac = float(i) / float(num_samples)
        let envelope = 1.0 - frac
        seed = seed * 1103515245u + 12345u
        let noise = float(int(seed >> 16u) % 1000) / 500.0 - 1.0
        samples[i] = noise * envelope * volume
    }
    return <- samples
}

def mix_into(var dst : array<float>; src : array<float>) {
    let n = min(length(dst), length(src))
    for (i in range(n)) {
        dst[i] += src[i]
    }
}

var @live snd_eat_pellet  : array<float>
var @live snd_eat_power   : array<float>
var @live snd_eat_ghost   : array<float>
var @live snd_pac_die     : array<float>
var @live snd_level_win   : array<float>
var @live audio_initialized = false

def init_audio_samples() {
    snd_eat_pellet <- gen_sine_sweep(600.0, 400.0, 0.04, 0.15)
    snd_eat_power <- gen_sine_sweep(300.0, 900.0, 0.12, 0.3)
    snd_eat_ghost <- gen_sine_sweep(800.0, 1400.0, 0.1, 0.3)
    snd_pac_die <- gen_sine_sweep(440.0, 110.0, 0.6, 0.3)
    let extra <- gen_noise_burst(0.3, 0.1)
    mix_into(snd_pac_die, extra)
    snd_level_win <- gen_sine_sweep(523.0, 1047.0, 0.5, 0.25)
}

def play_sfx(samples : array<float>) {
    if (!audio_initialized) { return ; }
    var copy <- clone(samples)
    play_sound_from_pcm(AUDIO_RATE, AUDIO_CHANNELS, copy)
}

// ===== Music =====

var g_music_tracks : table<string; int>

def strudel_music_cmd(cmd : string) {
    let parts <- split(cmd, ":")
    if (length(parts) < 2) { return ; }
    let action = parts[0]
    let name   = parts[1]
    if (action == "play" && length(parts) >= 3) {
        let gain = length(parts) >= 4 ? float(to_double(parts[3])) : 1.0
        if (key_exists(g_music_tracks, name)) {
            strudel_remove_track(g_music_tracks[name])
        }
        g_music_tracks[name] = strudel_add_track(s(parts[2]), gain)
    } elif (action == "note" && length(parts) >= 4) {
        let synth = parts[3]
        let gain  = length(parts) >= 5 ? float(to_double(parts[4])) : 0.4
        if (key_exists(g_music_tracks, name)) {
            strudel_remove_track(g_music_tracks[name])
        }
        g_music_tracks[name] = strudel_add_track(note_pattern(parts[2], synth), gain)
    } elif (action == "stop") {
        if (key_exists(g_music_tracks, name)) {
            let fade_time = length(parts) >= 3 ? float(to_double(parts[2])) : 0.5
            strudel_fade_track(g_music_tracks[name], 0.0, fade_time)
        }
    } elif (action == "fade" && length(parts) >= 4) {
        if (key_exists(g_music_tracks, name)) {
            let gain = float(to_double(parts[2]))
            let time = float(to_double(parts[3]))
            strudel_fade_track(g_music_tracks[name], gain, time)
        }
    }
}

def strudel_music_main() {
    strudel_set_bpm(130.0lf)
    strudel_play(@@strudel_music_cmd)
}

def stop_all_music(fade : float) {
    strudel_command("stop:drums:{fade}")
    strudel_command("stop:bass:{fade}")
    strudel_command("stop:lead:{fade}")
}

def start_menu_music() {
    strudel_set_bpm(120.0lf)
    strudel_command("play:drums:<bd ~ sd ~> <bd [hh hh] sd hh>:0.4")
    strudel_command("note:bass:<[c2 c2] ~ [g2 g2] ~> <[c2 c2] ~ [f2 f2] [g2 g2]>:square:0.25")
    strudel_command("note:lead:<c5 e5 g5 e5> <a4 c5 e5 g5> <f5 e5 d5 c5> <g5 ~ e5 ~>:triangle:0.15")
}

def start_play_music() {
    strudel_set_bpm(140.0lf)
    strudel_command("play:drums:<bd [hh hh] sd [hh hh]> <bd [hh hh] sd [hh cp]>:0.5")
    strudel_command("note:bass:<[c2 c2] [c2 c2] [f2 f2] [g2 g2]> <[c2 c2] [e2 e2] [f2 f2] [e2 e2]>:square:0.28")
    strudel_command("note:lead:<c5 ~ e5 ~> <~ g5 ~ e5> <f5 e5 d5 ~> <~ c5 ~ ~>:sawtooth:0.18")
}

def start_flee_music() {
    strudel_set_bpm(180.0lf)
    strudel_command("play:drums:<bd [hh hh hh hh] sd [hh hh]>:0.6")
    strudel_command("note:bass:<[c3 c3 c3 c3] [f3 f3 f3 f3]> <[g3 g3 g3 g3] [e3 e3 f3 g3]>:square:0.3")
    strudel_command("note:lead:<c6 e6 g6 e6> <f6 e6 d6 c6>:triangle:0.2")
}

def start_gameover_music() {
    strudel_set_bpm(80.0lf)
    strudel_command("play:drums:<bd ~ ~ ~> <~ ~ bd ~>:0.2")
    strudel_command("note:bass:<c2 ~ bb1 ~> <ab1 ~ g1 ~>:sine:0.3")
    strudel_command("note:lead:<eb4 ~ d4 ~> <c4 ~ ~ ~>:triangle:0.2")
}

// ===== Shaders =====

var @in @location = 0 v_position : float3
var @in @location = 1 v_normal   : float3
var @in @location = 2 v_texcoord : float2
var @uniform v_model      : float4x4
var @uniform v_view       : float4x4
var @uniform v_projection : float4x4
var @inout f_normal   : float3
var @inout f_position : float3
var @uniform f_Color  : float4
var @out f_FragColor  : float4

[vertex_program]
def vs_pac {
    f_normal   = normalize(float3x3(v_model) * v_normal)
    let tpos   = v_model * float4(v_position, 1.0)
    f_position = tpos.xyz
    gl_Position = v_projection * v_view * tpos
}

[fragment_program]
def fs_pac {
    let normal = normalize(f_normal)
    let k = saturate(-dot(LIGHT_DIR, normal))
    f_FragColor.xyz = f_Color.xyz * (k * 0.75 + 0.25)
    f_FragColor.w   = f_Color.w
}

// ===== GL objects =====

var program      : uint
var geo_sphere    : OpenGLGeometryFragment
var geo_cube      : OpenGLGeometryFragment
var geo_plane     : OpenGLGeometryFragment
var geo_pac_closed : OpenGLGeometryFragment
var geo_pac_open   : OpenGLGeometryFragment
var hud_font      : Font?

def gen_pac_sphere(sectorCount, stackCount : int; mouth_half_angle : float) {
    var frag : GeometryFragment
    frag.vertices |> reserve((stackCount + 1) * (sectorCount + 1))
    let sectorStep = 2.0 * PI / float(sectorCount)
    let stackStep  = PI / float(stackCount)
    for (i in range(stackCount + 1)) {
        let stackAngle = PI / 2.0 - float(i) * stackStep
        let xy = cos(stackAngle)
        let z  = sin(stackAngle)
        for (j in range(sectorCount + 1)) {
            let sectorAngle = float(j) * sectorStep
            let x = xy * cos(sectorAngle)
            let y = xy * sin(sectorAngle)
            frag.vertices |> push <| GeometryPreviewVertex(
                xyz    = float3(x, y, z),
                normal = normalize(float3(x, y, z)),
                uv     = float2(float(j) / float(sectorCount), float(i) / float(stackCount))
            )
        }
    }
    var k1, k2 : int
    for (i in range(stackCount)) {
        k1 = i * (sectorCount + 1)
        k2 = k1 + sectorCount + 1
        for (j in range(sectorCount)) {
            let mid = (float(j) + 0.5) * sectorStep
            let a   = mid > PI ? mid - 2.0 * PI : mid
            if (abs(a) >= mouth_half_angle) {
                if (i != 0) {
                    frag.indices |> push(k1)
                    frag.indices |> push(k2)
                    frag.indices |> push(k1 + 1)
                }
                if (i != (stackCount - 1)) {
                    frag.indices |> push(k1 + 1)
                    frag.indices |> push(k2)
                    frag.indices |> push(k2 + 1)
                }
            }
            ++k1
            ++k2
        }
    }
    frag.prim = GeometryFragmentType.triangles
    gen_bbox(frag)
    return <- frag
}

def create_gl_objects() {
    program      = cache_shader_program(vs_pac`shader_text, fs_pac`shader_text)
    geo_sphere    <- create_geometry_fragment <| gen_sphere(12, 7, false)
    geo_cube      <- create_geometry_fragment <| gen_cube()
    geo_plane     <- create_geometry_fragment <| gen_plane(GenDirection.xy)
    geo_pac_closed <- create_geometry_fragment <| gen_pac_sphere(16, 10, 0.08)
    geo_pac_open   <- create_geometry_fragment <| gen_pac_sphere(16, 10, 0.55)
    cache_ttf_objects()
    hud_font = cache_font("{get_das_root()}/modules/dasStbImage/fonts/droidsansmono.ttf")
}

// ===== Draw helpers =====

var display_w = 0
var display_h = 0

def draw_box(pos : float3; dim : float3; color : float3; alpha : float = 1.0; prog : uint) {
    v_model   = compose(pos, float4(0), dim * 0.5)
    f_Color   = float4(color, alpha)
    vs_pac_bind_uniform(prog)
    fs_pac_bind_uniform(prog)
    geo_cube |> draw_geometry_fragment()
}

def draw_sphere_obj(pos : float3; radius : float; color : float3; prog : uint) {
    v_model   = compose(pos, float4(0), float3(radius))
    f_Color   = float4(color, 1.0)
    vs_pac_bind_uniform(prog)
    fs_pac_bind_uniform(prog)
    geo_sphere |> draw_geometry_fragment()
}

def draw_text(txt : string; x, y : int; tint : float3 = float3(1.0)) {
    if (hud_font == null) { return ; }
    var quads <- (*hud_font) |> create_quads(txt)
    (*hud_font) |> draw_quads_2d(quads, display_w, display_h, x, y, tint)
    delete quads
}

def draw_text_centered(txt : string; cx, y : int; tint : float3 = float3(1.0)) {
    if (hud_font == null) { return ; }
    var quads <- (*hud_font) |> create_quads(txt)
    let dim = quads_dim(quads)
    let tw = int(dim.vmax.x - dim.vmin.x)
    (*hud_font) |> draw_quads_2d(quads, display_w, display_h, cx - tw / 2, y, tint)
    delete quads
}

// ===== Score popups =====

struct ScorePopup {
    world_pos : float2
    text      : string
    timer     : float
}

var @live score_popups : array<ScorePopup>

// ===== Bonus fruit =====

let FRUIT_SCORE = fixed_array(100, 300)  // cherry, strawberry
let FRUIT_COLOR = fixed_array(
    float3(1.0, 0.1, 0.1),
    float3(1.0, 0.1, 0.5)
)
// Fruit appears after eating these many total pellets
let FRUIT_TRIGGER = fixed_array(70, 130)

struct Fruit {
    world_pos  : float2
    kind       : int     // 0=cherry, 1=strawberry
    timer      : float   // despawn countdown
    active     : bool
}

var @live fruit : Fruit
var @live next_fruit_idx = 0
var @live pellets_eaten_total = 0

// ===== Pellet state =====
// Stored as flat arrays indexed by cell — simpler than DECS for a static maze

var pellets     : array<bool>   // true = pellet present
var power_pills : array<bool>   // true = power pill present

def init_pellets() {
    pellets     |> resize(MAZE_COLS * MAZE_ROWS)
    power_pills |> resize(MAZE_COLS * MAZE_ROWS)
    for (i in range(MAZE_COLS * MAZE_ROWS)) {
        pellets[i]     = MAZE_DATA[i] == 2
        power_pills[i] = MAZE_DATA[i] == 3
    }
}

def pellet_idx(col, row : int) : int {
    return row * MAZE_COLS + col
}

def has_pellets() : bool {
    for (p in pellets) {
        if (p) { return true; }
    }
    return false
}


// ===== Game state =====

enum GameState {
    menu
    ready
    playing
    dying
    win_state
    game_over_state
}

var @live score      = 0
var @live lives      = START_LIVES
var @live game_state = GameState.menu
var @live power_timer = 0.0     // fleeing time remaining
var @live ghosts_eaten_combo = 0
var @live level = 1

var space_was_pressed = false
var esc_was_pressed   = false
var @live prev_music_state = GameState.menu
var music_initialized = false
var @live die_timer = 0.0       // death animation timer
var @live end_screen_timer = 0.0  // blocks restart input briefly after game over / win
var @live shake_timer = 0.0     // screen shake remaining time
var @live shake_mag   = 0.0     // initial shake magnitude
var asch : AudioSystemChannels

// Dot counter for ghost release stagger
var @live dot_counter = 0
// Scatter/chase cycle: positive = scatter time left, negative = chase time left
var @live scatter_chase_timer = 7.0
var @live scatter_phase = true
// READY! intro
var @live ready_timer = 0.0

// ===== Pac-Man =====

var @live pac_pos       : float2     // logical world position (cell centres)
var @live pac_render_pos : float2    // smoothed visual position
var @live pac_dir  : int2       // current move direction (grid-aligned)
var @live pac_next : int2       // queued next direction from input
var @live pac_anim = 0.0        // mouth open/close phase
var @live pac_dead = false

def pac_start_pos() : float2 {
    return cell_to_world(10, 17)
}

def reset_pac() {
    pac_pos        = pac_start_pos()
    pac_render_pos = pac_start_pos()
    pac_dir  = int2(1, 0)
    pac_next = int2(1, 0)
    pac_dead = false
    pac_anim = 0.0
}

// Can Pac-Man move from cell (col,row) in direction (dc,dr)?
def can_move(col, row, dc, dr : int) : bool {
    let nc = col + dc
    let nr = row + dr
    let c = cell_at(nc, nr)
    return c != 1 && c != 4 && c != 5   // walls, ghost house interior, and ghost door block Pac
}

// ===== Ghosts (DECS) =====

[decs_template]
struct Ghost {
    pos        : float2
    dir        : int2
    ghost_idx  : int      // 0-3 determines color and personality
    flee       : bool
    eaten      : bool     // in house, returning after being eaten
    in_house   : bool     // waiting for dot-counter release
    move_timer : float    // time until next cell step
    scatter_timer : float
    render_pos : float2   // smoothed visual position
    scatter_mode : bool   // true = scatter corner, false = chase pac
}

let GHOST_HOUSE_COL = 10
let GHOST_HOUSE_ROW = 10

def ghost_start_pos(idx : int) : float2 {
    // Unique positions: Blinky above house, Pinky/Inky/Clyde inside
    let cols = fixed_array(10, 9, 10, 11)
    let rows = fixed_array(8,  10, 10, 10)
    return cell_to_world(cols[idx], rows[idx])
}

// Simple ghost AI: target a cell, pick direction toward it that is not a wall and not reversing
def ghost_choose_dir(col, row : int; cur_dir : int2; target_col, target_row : int; is_flee : bool) : int2 {
    let dirs = fixed_array(int2(1, 0), int2(-1, 0), int2(0, 1), int2(0, -1))
    var best_dir = cur_dir
    var best_dist = is_flee ? -1.0 : 1000000.0
    var found = false

    for (d in dirs) {
        // Don't reverse
        if (d.x == -cur_dir.x && d.y == -cur_dir.y) { continue; }
        let nc = col + d.x
        let nr = row + d.y
        let c = cell_at(nc, nr)
        if (c == 1) { continue; }   // wall
        // cell 5 is the ghost door — passable for ghosts

        let dx = float(nc - target_col)
        let dy = float(nr - target_row)
        let dist = dx * dx + dy * dy
        if (is_flee) {
            if (dist > best_dist) {
                best_dist = dist
                best_dir  = d
                found = true
            }
        } else {
            if (dist < best_dist) {
                best_dist = dist
                best_dir  = d
                found = true
            }
        }
    }
    // Dead-end: no valid non-reverse direction found — fall back to reversing
    if (!found) { return int2(-cur_dir.x, -cur_dir.y); }
    return best_dir
}

def ghost_target(ghost_idx : int; pac_col, pac_row : int; pac_dir : int2) : int2 {
    // Blinky: directly after Pac
    if (ghost_idx == 0) { return int2(pac_col, pac_row); }
    // Pinky: 4 cells ahead of Pac
    if (ghost_idx == 1) { return int2(pac_col + pac_dir.x * 4, pac_row + pac_dir.y * 4); }
    // Inky: scatter bottom-right
    if (ghost_idx == 2) { return int2(MAZE_COLS - 2, MAZE_ROWS - 2); }
    // Clyde: scatter bottom-left
    return int2(1, MAZE_ROWS - 2)
}

// Dot thresholds to release each ghost from the house
def ghost_release_dots(idx : int) : int {
    if (idx == 0) { return 0; }   // Blinky exits immediately
    if (idx == 1) { return 30; }
    if (idx == 2) { return 60; }
    return 90;
}

def spawn_ghosts() {
    for (i in range(4)) {
        let pos = ghost_start_pos(i)
        let starts_in_house = i != 0
        create_entity() @(eid, cmp) {
            apply_decs_template(cmp, Ghost(
                pos           = pos,
                dir           = int2(0, -1),
                ghost_idx     = i,
                flee          = false,
                eaten         = false,
                in_house      = starts_in_house,
                move_timer    = float(i) * 0.3 + 0.1,
                scatter_timer = 0.0,
                render_pos    = pos,
                scatter_mode  = true
            ))
        }
    }
    commit()
}

// ===== Maze rendering =====

def draw_maze(prog : uint) {
    let wall_h = 0.6
    for (row in range(MAZE_ROWS)) {
        for (col in range(MAZE_COLS)) {
            let c = cell_at(col, row)
            if (c == 1) {
                let wpos = cell_to_world(col, row)
                draw_box(
                    float3(wpos.x, wpos.y, wall_h * 0.5),
                    float3(CELL_SIZE, CELL_SIZE, wall_h),
                    WALL_COLOR, 1.0, prog
                )
            }
        }
    }
}

def draw_floor(prog : uint) {
    v_model = compose(float3(0.0, 0.0, -0.01), float4(0), float3(HALF_W + 1.0, HALF_H + 1.0, 1.0))
    f_Color = float4(FLOOR_COLOR, 1.0)
    vs_pac_bind_uniform(prog)
    fs_pac_bind_uniform(prog)
    geo_plane |> draw_geometry_fragment()
}

def draw_pellets(prog : uint) {
    for (row in range(MAZE_ROWS)) {
        for (col in range(MAZE_COLS)) {
            let idx = pellet_idx(col, row)
            if (pellets[idx]) {
                let wpos = cell_to_world(col, row)
                draw_sphere_obj(float3(wpos.x, wpos.y, 0.08), 0.1, PELLET_COLOR, prog)
            } elif (power_pills[idx]) {
                let t = float(get_uptime())
                let pulse = 0.15 + 0.05 * sin(t * 4.0)
                let wpos = cell_to_world(col, row)
                draw_sphere_obj(float3(wpos.x, wpos.y, 0.12), pulse, POWER_COLOR, prog)
            }
        }
    }
}

def draw_pac(prog : uint) {
    if (pac_dead) { return ; }
    let angle = atan2(float(pac_dir.y), float(pac_dir.x))
    let quat  = float4(0.0, 0.0, sin(angle * 0.5), cos(angle * 0.5))
    v_model = compose(float3(pac_render_pos.x, pac_render_pos.y, 0.45), quat, float3(0.45, 0.45, 0.45))
    f_Color = float4(PAC_COLOR, 1.0)
    vs_pac_bind_uniform(prog)
    fs_pac_bind_uniform(prog)
    if (sin(pac_anim * 8.0) > 0.0) {
        geo_pac_open |> draw_geometry_fragment()
    } else {
        geo_pac_closed |> draw_geometry_fragment()
    }
}

// ===== HUD =====

def draw_hud() {
    let cx = display_w / 2
    let cy = display_h / 2
    if (game_state == GameState.menu) {
        draw_text_centered("PAC-MAN", cx, cy - 60, float3(1.0, 0.9, 0.1))
        let pulse = 0.5 + 0.5 * sin(float(get_uptime()) * 3.0)
        draw_text_centered("PRESS SPACE TO START", cx, cy + 10, float3(pulse, pulse, pulse))
        draw_text_centered("ARROW KEYS TO MOVE", cx, cy + 45, float3(0.6, 0.6, 0.6))
        return
    }
    draw_text("SCORE: {score}", 10, 35, float3(1.0, 1.0, 1.0))
    draw_text("LIVES: {lives}", 10, 65, float3(1.0, 0.9, 0.1))
    draw_text("LEVEL: {level}", 10, 95, float3(0.6, 0.8, 1.0))
    if (power_timer > 0.0) {
        draw_text("POWER!", display_w - 130, 35, float3(1.0, 0.5, 0.0))
    }
    if (game_state == GameState.ready) {
        let pulse = 0.5 + 0.5 * sin(float(get_uptime()) * 6.0)
        draw_text_centered("READY!", cx, cy, float3(1.0, 1.0, 0.2) * pulse)
    } elif (game_state == GameState.game_over_state) {
        draw_text_centered("GAME OVER", cx, cy - 20, float3(1.0, 0.2, 0.2))
        draw_text_centered("SCORE: {score}", cx, cy + 20, float3(1.0, 1.0, 0.5))
        draw_text_centered("PRESS SPACE TO RESTART", cx, cy + 55, float3(0.7, 0.7, 0.7))
    } elif (game_state == GameState.win_state) {
        draw_text_centered("LEVEL CLEAR!", cx, cy - 20, float3(0.2, 1.0, 0.3))
        draw_text_centered("SCORE: {score}", cx, cy + 20, float3(1.0, 1.0, 0.5))
        draw_text_centered("PRESS SPACE TO CONTINUE", cx, cy + 55, float3(0.7, 0.7, 0.7))
    }

    // Draw floating score popups (project world pos → screen)
    let mvp = v_projection * v_view
    for (popup in score_popups) {
        let wpos4 = float4(popup.world_pos.x, popup.world_pos.y, 0.5, 1.0)
        let clip  = mvp * wpos4
        if (clip.w <= 0.0) { continue; }
        let ndc = clip.xyz / clip.w
        let sx  = int((ndc.x * 0.5 + 0.5) * float(display_w))
        let sy  = int((1.0 - (ndc.y * 0.5 + 0.5)) * float(display_h))
        let t   = popup.timer / 1.5
        draw_text_centered(popup.text, sx, sy, float3(1.0, t, 0.2))
    }
}

// ===== Update: Music transitions =====

def update_music() {
    if (game_state == prev_music_state) { return ; }
    let FADE = 0.5
    stop_all_music(FADE)
    if (game_state == GameState.menu) {
        start_menu_music()
    } elif (game_state == GameState.ready || game_state == GameState.playing) {
        if (power_timer > 0.0) {
            start_flee_music()
        } else {
            start_play_music()
        }
    } elif (game_state == GameState.game_over_state) {
        start_gameover_music()
    }
    prev_music_state = game_state
}

// ===== Update: scatter/chase cycle =====

def update_scatter_chase(dt : float) {
    if (game_state != GameState.playing || power_timer > 0.0) { return ; }
    scatter_chase_timer -= dt
    if (scatter_chase_timer <= 0.0) {
        scatter_phase = !scatter_phase
        scatter_chase_timer = scatter_phase ? 7.0 : 20.0
        query() $(var g : Ghost) {
            if (!g.flee && !g.eaten && !g.in_house) {
                g.scatter_mode = scatter_phase
                // reverse direction on phase switch
                g.dir = int2(-g.dir.x, -g.dir.y)
            }
        }
        commit()
    }
}

// ===== Update: Pac-Man movement =====

var @live pac_move_acc = 0.0  // sub-cell accumulator

def update_pac(dt : float) {
    if (pac_dead || game_state != GameState.playing) { return ; }

    // Read input → queue next direction
    if (glfwGetKey(live_window, int(GLFW_KEY_RIGHT)) == int(GLFW_PRESS)) { pac_next = int2(1, 0); }
    if (glfwGetKey(live_window, int(GLFW_KEY_LEFT))  == int(GLFW_PRESS)) { pac_next = int2(-1, 0); }
    if (glfwGetKey(live_window, int(GLFW_KEY_UP))    == int(GLFW_PRESS)) { pac_next = int2(0, 1); }
    if (glfwGetKey(live_window, int(GLFW_KEY_DOWN))  == int(GLFW_PRESS)) { pac_next = int2(0, -1); }

    pac_anim += dt
    pac_move_acc += eff_pac_speed() * dt

    // Move by whole cells
    while (pac_move_acc >= 1.0) {
        pac_move_acc -= 1.0
        let cell = world_to_cell(pac_pos.x, pac_pos.y)
        let col  = cell.x
        let row  = cell.y

        // Try to switch to queued direction
        if (can_move(col, row, pac_next.x, pac_next.y)) {
            pac_dir = pac_next
        }
        // Move in current direction if possible
        if (can_move(col, row, pac_dir.x, pac_dir.y)) {
            let nc   = col + pac_dir.x
            let nr   = row + pac_dir.y
            // Wrap tunnel (row 10, columns 0 and 20)
            let wc   = (nc + MAZE_COLS) % MAZE_COLS
            let wr   = nr
            pac_pos  = cell_to_world(wc, wr)
        } else {
            // Snap to cell center
            pac_pos = cell_to_world(col, row)
        }

        // Eat pellet
        let cc = world_to_cell(pac_pos.x, pac_pos.y)
        let pi = pellet_idx(cc.x, cc.y)
        if (pellets[pi]) {
            pellets[pi] = false
            score += PELLET_SCORE
            pellets_eaten_total += 1
            dot_counter += 1
            play_sfx(snd_eat_pellet)
            // Bonus fruit trigger
            if (next_fruit_idx < 2 && pellets_eaten_total >= FRUIT_TRIGGER[next_fruit_idx] && !fruit.active) {
                let fw = cell_to_world(10, 12)
                fruit = Fruit(active = true, kind = next_fruit_idx, timer = 9.0, world_pos = fw)
                next_fruit_idx += 1
            }
            if (!has_pellets() && !power_pills_any()) {
                game_state = GameState.win_state
                end_screen_timer = 2.0
                play_sfx(snd_level_win)
            }
        } elif (power_pills[pi]) {
            power_pills[pi] = false
            score += POWER_SCORE
            power_timer = eff_power_duration()
            ghosts_eaten_combo = 0
            play_sfx(snd_eat_power)
            shake_timer = 0.25; shake_mag = 0.08
            // Set all ghosts to flee
            query() $(var g : Ghost) {
                if (!g.eaten && !g.in_house) {
                    g.flee = true
                }
            }
            commit()
            // Switch music to flee
            stop_all_music(0.2)
            start_flee_music()
            if (!has_pellets() && !power_pills_any()) {
                game_state = GameState.win_state
                end_screen_timer = 2.0
                play_sfx(snd_level_win)
            }
        }
    }
}

def power_pills_any() : bool {
    for (p in power_pills) {
        if (p) { return true; }
    }
    return false
}

// ===== Update: Bonus fruit =====

def update_fruit(dt : float) {
    if (!fruit.active) { return ; }
    fruit.timer -= dt
    if (fruit.timer <= 0.0) {
        fruit.active = false
        return
    }
    // Check Pac collision
    let fc = world_to_cell(fruit.world_pos.x, fruit.world_pos.y)
    let pc = world_to_cell(pac_pos.x, pac_pos.y)
    if (fc.x == pc.x && fc.y == pc.y) {
        let pts = FRUIT_SCORE[fruit.kind]
        score += pts
        score_popups |> push <| ScorePopup(world_pos = fruit.world_pos, text = "{pts}", timer = 1.5)
        shake_timer = 0.2; shake_mag = 0.07
        fruit.active = false
    }
}

def draw_fruit(prog : uint) {
    if (!fruit.active) { return ; }
    let pulse = 0.18 + 0.04 * sin(float(get_uptime()) * 5.0)
    draw_sphere_obj(float3(fruit.world_pos.x, fruit.world_pos.y, 0.25), pulse, FRUIT_COLOR[fruit.kind], prog)
}

// ===== Update: Ghost movement =====

def update_ghosts(dt : float) {
    if (game_state != GameState.playing) { return ; }

    // Tick down power timer
    if (power_timer > 0.0) {
        power_timer -= dt
        if (power_timer <= 0.0) {
            power_timer = 0.0
            query() $(var g : Ghost) {
                g.flee = false
            }
            commit()
            stop_all_music(0.3)
            start_play_music()
        }
    }

    let pac_cell = world_to_cell(pac_pos.x, pac_pos.y)

    query() $(var g : Ghost) {
        // Release from house based on dot counter
        if (g.in_house) {
            if (dot_counter >= ghost_release_dots(g.ghost_idx)) {
                g.in_house = false
                g.dir = int2(0, 1)   // exit toward door (row 11 is higher row = door direction)
            } else {
                return
            }
        }

        // Smooth visual position
        let gdiff = g.pos - g.render_pos
        let gdist = length(gdiff)
        let gspeed = g.flee ? eff_ghost_flee_speed() : eff_ghost_speed()
        let gmove = gspeed * CELL_SIZE * dt
        if (gdist > 2.0) {
            g.render_pos = g.pos
        } elif (gdist > gmove) {
            g.render_pos += normalize(gdiff) * gmove
        } else {
            g.render_pos = g.pos
        }

        g.move_timer -= dt
        if (g.move_timer > 0.0) { return ; }

        let speed = g.flee ? eff_ghost_flee_speed() : eff_ghost_speed()
        g.move_timer += 1.0 / speed

        let gc = world_to_cell(g.pos.x, g.pos.y)

        var tgt : int2
        if (g.eaten) {
            // Eyes travel home
            tgt = int2(GHOST_HOUSE_COL, GHOST_HOUSE_ROW)
            if (gc.x == GHOST_HOUSE_COL && gc.y == GHOST_HOUSE_ROW) {
                g.eaten = false
                g.flee  = power_timer > 0.0
                g.scatter_mode = scatter_phase
            }
        } elif (g.scatter_mode) {
            // Scatter to corner
            tgt = ghost_target(g.ghost_idx, pac_cell.x, pac_cell.y, pac_dir)
            if (g.ghost_idx == 0) { tgt = int2(MAZE_COLS - 2, 1); }
            elif (g.ghost_idx == 1) { tgt = int2(1, 1); }
            elif (g.ghost_idx == 2) { tgt = int2(MAZE_COLS - 2, MAZE_ROWS - 2); }
            else { tgt = int2(1, MAZE_ROWS - 2); }
        } else {
            tgt = ghost_target(g.ghost_idx, pac_cell.x, pac_cell.y, pac_dir)
        }

        g.dir = ghost_choose_dir(gc.x, gc.y, g.dir, tgt.x, tgt.y, g.flee && !g.eaten)
        let nc = gc.x + g.dir.x
        let nr = gc.y + g.dir.y
        let wc = (nc + MAZE_COLS) % MAZE_COLS
        let wr = clamp(nr, 0, MAZE_ROWS - 1)
        g.pos = cell_to_world(wc, wr)
    }
    commit()
}

// ===== Update: Collisions Pac vs Ghost =====

def update_pac_ghost_collision() {
    if (pac_dead || game_state != GameState.playing) { return ; }

    let pc = world_to_cell(pac_pos.x, pac_pos.y)
    query() $(var g : Ghost) {
        if (g.eaten) { return ; }
        let gc = world_to_cell(g.pos.x, g.pos.y)
        if (gc.x != pc.x || gc.y != pc.y) { return ; }

        if (g.flee) {
            // Pac eats ghost
            g.eaten = true
            g.flee  = false
            ghosts_eaten_combo++
            let pts = GHOST_EAT_SCORE * (1 << (ghosts_eaten_combo - 1))
            score += pts
            score_popups |> push <| ScorePopup(world_pos = g.render_pos, text = "{pts}", timer = 1.5)
            shake_timer = 0.18; shake_mag = 0.06
            play_sfx(snd_eat_ghost)
        } else {
            // Ghost kills Pac — guard against multiple ghosts hitting in the same frame
            if (pac_dead) { return ; }
            pac_dead  = true
            die_timer = 1.5
            lives--
            play_sfx(snd_pac_die)
            if (lives <= 0) {
                game_state = GameState.game_over_state
                end_screen_timer = 2.0
                stop_all_music(0.3)
                start_gameover_music()
                prev_music_state = GameState.game_over_state
            }
        }
    }
    commit()
}

// ===== Update: Ghosts draw (DECS stage) =====

[decs(stage=draw)]
def draw_ghost_ent(g : Ghost) {
    if (g.in_house) { return ; }
    let rp = g.render_pos
    let fd = float2(float(g.dir.x), float(g.dir.y))
    let rt = float2(fd.y, -fd.x)

    // Flee blink in last 2 seconds
    let blink_active = g.flee && power_timer < 2.0 && (sin(float(get_uptime()) * 14.0) > 0.0)
    let flee_visible = g.flee && !blink_active

    if (!g.eaten) {
        // Body — large sphere, ghost colour
        let body_color = flee_visible ? FLEE_COLOR : (g.flee ? float3(1.0, 1.0, 1.0) : GHOST_COLORS[g.ghost_idx % 4])
        v_model = compose(float3(rp.x, rp.y, 0.5), float4(0, 0, 0, 1), float3(0.46, 0.46, 0.56))
        f_Color = float4(body_color, 1.0)
        vs_pac_bind_uniform(program)
        fs_pac_bind_uniform(program)
        geo_sphere |> draw_geometry_fragment()
    }

    // Eyes: white normally, red when fleeing (classic scared look)
    let eye_color = float4(g.flee ? float3(1.0, 0.2, 0.2) : float3(1.0, 1.0, 1.0), 1.0)
    let leye = float3(rp.x + fd.x * 0.32 - rt.x * 0.17, rp.y + fd.y * 0.32 - rt.y * 0.17, 0.67)
    v_model = compose(leye, float4(0, 0, 0, 1), float3(0.12, 0.12, 0.12))
    f_Color = eye_color
    vs_pac_bind_uniform(program)
    fs_pac_bind_uniform(program)
    geo_sphere |> draw_geometry_fragment()
    let reye = float3(rp.x + fd.x * 0.32 + rt.x * 0.17, rp.y + fd.y * 0.32 + rt.y * 0.17, 0.67)
    v_model = compose(reye, float4(0, 0, 0, 1), float3(0.12, 0.12, 0.12))
    f_Color = eye_color
    vs_pac_bind_uniform(program)
    fs_pac_bind_uniform(program)
    geo_sphere |> draw_geometry_fragment()
}

// ===== Reset helpers =====

def reset_level() {
    init_pellets()
    reset_pac()
    power_timer = 0.0
    ghosts_eaten_combo = 0
    die_timer = 0.0
    dot_counter = 0
    scatter_chase_timer = 7.0
    scatter_phase = true
    next_fruit_idx = 0
    pellets_eaten_total = 0
    fruit = Fruit(active = false, kind = 0, timer = 0.0, world_pos = float2(0.0))
    decs::restart()
    spawn_ghosts()
}

def reset_game() {
    score      = 0
    lives      = START_LIVES
    reset_level()
    game_state = GameState.ready
    ready_timer = 2.0
}

// ===== Lifecycle =====

[export]
def init() {
    live_create_window("Pac-Man", 900, 760)
    create_gl_objects()
    if (!audio_initialized) {
        asch = audio_system_create()
        audio_initialized = true
        init_audio_samples()
    }
    if (!music_initialized) {
        strudel_init(@@strudel_music_main)
        strudel_set_volume(0.4, 0.0)
        music_initialized = true
        start_menu_music()
    }
    if (!is_reload()) {
        init_pellets()
        reset_pac()
        dot_counter = 0
        scatter_chase_timer = 7.0
        scatter_phase = true
        next_fruit_idx = 0
        pellets_eaten_total = 0
        fruit = Fruit(active = false, kind = 0, timer = 0.0, world_pos = float2(0.0))
        decs::restart()
        commit()
    }
}

[export]
def update() {
    if (!live_begin_frame()) { return ; }
    live_get_framebuffer_size(display_w, display_h)
    glViewport(0, 0, display_w, display_h)
    glClearColor(0.02, 0.02, 0.06, 1.0)
    glClearDepth(1.0lf)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glEnable(GL_DEPTH_TEST)
    glEnable(GL_CULL_FACE)

    let aspect = display_h != 0 ? float(display_w) / float(display_h) : 1.0
    v_projection = perspective_rh_opengl(50.0 * PI / 180.0, aspect, 0.1, 200.0)
    // Screen shake
    var shake_off = float2(0.0)
    if (shake_timer > 0.0) {
        shake_timer -= min(float(get_dt()), 1.0 / 30.0)
        let t = float(get_uptime())
        let decay = saturate(shake_timer / max(shake_mag * 3.0, 0.01))
        shake_off = float2(sin(t * 97.0), cos(t * 71.0)) * shake_mag * decay
    }
    // Tilted arcade-cab camera: slightly above and behind, looking down toward the board
    v_view = look_at_rh(
        float3(shake_off.x, -4.0 + shake_off.y, 30.0),
        float3(shake_off.x, shake_off.y, 0.0),
        float3(0.0, 1.0, 0.0)
    )

    let raw_dt = min(float(get_dt()), 1.0 / 30.0)
    // Slow motion during death animation only (not on game over screen)
    let dt = (pac_dead && lives > 0) ? raw_dt * 0.2 : raw_dt

    // Input
    let space_pressed = glfwGetKey(live_window, int(GLFW_KEY_SPACE)) == int(GLFW_PRESS)
    let space_just = space_pressed && !space_was_pressed
    space_was_pressed = space_pressed

    if (game_state == GameState.menu && space_just) {
        level = 1
        reset_game()
        prev_music_state = GameState.menu
    } elif ((game_state == GameState.game_over_state || game_state == GameState.win_state) && space_just && end_screen_timer <= 0.0) {
        if (game_state == GameState.win_state) {
            level++
        } else {
            level = 1
        }
        reset_game()
        prev_music_state = GameState.win_state
    }

    // Tick end-screen input block
    if (end_screen_timer > 0.0) {
        end_screen_timer -= raw_dt
    }

    // READY! countdown before play begins
    if (game_state == GameState.ready) {
        ready_timer -= raw_dt
        if (ready_timer <= 0.0) {
            game_state = GameState.playing
        }
    }

    // Die animation — timer runs at real time even while game is slowed
    if (pac_dead) {
        die_timer -= raw_dt
        if (die_timer <= 0.0 && lives > 0) {
            reset_pac()
            pac_dead = false
            // Reset ghosts
            decs::restart()
            spawn_ghosts()
            power_timer = 0.0
        }
    }

    update_music()
    update_scatter_chase(dt)
    update_fruit(dt)
    update_pac(dt)
    if (!pac_dead) {
        // Smooth visual position — chase logical pos at game speed; snap on tunnel warps
        let pdiff = pac_pos - pac_render_pos
        let pdist = length(pdiff)
        let pmove = eff_pac_speed() * CELL_SIZE * dt
        if (pdist > 2.0) {
            pac_render_pos = pac_pos
        } elif (pdist > pmove) {
            pac_render_pos += normalize(pdiff) * pmove
        } else {
            pac_render_pos = pac_pos
        }
    }
    update_ghosts(dt)
    update_pac_ghost_collision()

    // Tick down score popups; remove expired
    var spi = length(score_popups) - 1
    while (spi >= 0) {
        score_popups[spi].timer -= dt
        if (score_popups[spi].timer <= 0.0) {
            erase(score_popups, spi)
        }
        spi -= 1
    }

    // Render
    glUseProgram(program)
    draw_floor(program)
    draw_maze(program)
    draw_pellets(program)
    draw_fruit(program)
    draw_pac(program)

    glEnable(GL_BLEND)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
    decs_stage("draw")
    glDisable(GL_BLEND)

    glDisable(GL_CULL_FACE)
    draw_hud()
    live_end_frame()
}

[export]
def shutdown() {
    strudel_shutdown()
    audio_system_finalize(asch.command, asch.next_sid)
    live_destroy_window()
}

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}

