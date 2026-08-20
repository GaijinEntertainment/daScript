options gen2
options persistent_heap
options gc

require glfw/glfw_boost
require live/glfw_live
require opengl/opengl_boost
require opengl/opengl_gen
require opengl/opengl_cache
require daslib/defer
require daslib/safe_addr
require live/opengl_live
require live/live_commands
require ?dashv live/live_api
require live/live_watch_boost
require live/live_vars
require live/audio_live
require daslib/json_boost
require daslib/math_boost
require daslib/archive
require opengl/opengl_ttf
require audio/audio_boost
require strudel/strudel
require strudel/strudel_player
require strudel/strudel_live
require daslib/strings_boost
require daslib/strings_convert
require live_host
require cave
require cave_gen

let CELL_SIZE = 1.0
let LIGHT_DIR = normalize(float3(-0.28, -0.62, -1.2))
let FILL_DIR = normalize(float3(0.55, 0.28, -0.32))
let CAM_EYE = float3(0.0, -6.0, 26.0)
let START_LIVES = 3
let AUDIO_RATE = 48000
let AUDIO_CHANNELS = 1

let COL_DIRT = float3(0.45, 0.28, 0.12)
let COL_BOULDER = float3(0.55, 0.55, 0.58)
let COL_DIAMOND = float3(0.35, 0.95, 1.0)
let COL_BRICK = float3(0.7, 0.25, 0.18)
let COL_STEEL = float3(0.22, 0.28, 0.38)
let COL_MAGIC = float3(0.85, 0.25, 0.7)
let COL_EXPAND = float3(0.9, 0.45, 0.15)
let COL_SLIME = float3(0.25, 0.75, 0.3)
let COL_AMOEBA = float3(0.45, 0.95, 0.25)
let COL_FIREFLY = float3(0.95, 0.9, 0.2)
let COL_BUTTERFLY = float3(0.95, 0.3, 0.75)
let COL_PLAYER = float3(1.0, 0.75, 0.35)
let COL_EXIT = float3(0.95, 0.8, 0.2)
let COL_FLOOR = float3(0.08, 0.07, 0.1)
let COL_EXPLODE = float3(1.0, 0.7, 0.2)

enum GameState {
    menu
    playing
    paused
    dead
    cave_cleared
    game_over_state
}

var @in @location = 0 v_position : float3
var @in @location = 1 v_normal : float3
var @in @location = 2 v_texcoord : float2
var @uniform v_model : float4x4
var @uniform v_view : float4x4
var @uniform v_projection : float4x4
var @uniform v_Time : float
var @uniform v_Wave : float
var @inout f_normal : float3
var @inout f_position : float3
var @uniform f_Color : float4
var @uniform f_Time : float
var @uniform f_Eye : float3
var @uniform f_Spec : float
var @uniform f_Gloss : float
var @uniform f_Rim : float
var @uniform f_Wave : float
var @uniform f_Hash : float
var @out f_FragColor : float4

[vertex_program]
def vs_bd {
    let world = (v_model * float4(v_position, 1.0)).xyz
    let nrm = normalize(float3x3(v_model) * v_normal)
    let w1 = sin(v_Time * 3.4 + world.x * 2.4 + world.y * 1.7)
    let w2 = sin(v_Time * 5.8 + world.x * 4.1 + world.y * 3.2)
    let wave = v_Wave * (0.13 * w1 + 0.07 * w2)
    f_normal = nrm
    let tpos = float4(world + float3(0.0, 0.0, wave), 1.0)
    f_position = tpos.xyz
    gl_Position = v_projection * v_view * tpos
}

[fragment_program]
def fs_bd {
    let n = normalize(f_normal)
    let key = saturate(-dot(LIGHT_DIR, n))
    let fill = saturate(-dot(FILL_DIR, n))
    let hp = f_position.xy * 3.1
    let hr = sin(dot(floor(hp), float2(12.9898, 78.233))) * 43758.5
    let hash = 1.0 + (hr - floor(hr) - 0.5) * f_Hash
    var col = f_Color.xyz * (key * 0.70 + fill * 0.28 + 0.44) * hash
    let view_dir = normalize(f_Eye - f_position)
    let halfv = normalize(view_dir - LIGHT_DIR)
    let spec = pow(saturate(dot(n, halfv)), max(f_Gloss, 1.0)) * f_Spec
    let spark_ph = sin(f_Time * 6.5 + f_position.x * 14.0 + f_position.y * 9.0)
    let spark = spec * spec * f_Spec * (0.25 + 0.75 * saturate(spark_ph))
    col += lerp(float3(1.0), f_Color.xyz, 0.18) * (spec * 0.95 + spark * 1.6)
    let rim = pow(1.0 - saturate(dot(n, view_dir)), 2.4) * f_Rim
    col += f_Color.xyz * rim * 1.35
    let w1 = sin(f_Time * 3.4 + f_position.x * 2.4 + f_position.y * 1.7)
    let w2 = sin(f_Time * 5.8 + f_position.x * 4.1 + f_position.y * 3.2)
    col += f_Color.xyz * (w1 * 0.42 + w2 * 0.22) * f_Wave
    f_FragColor.xyz = col
    f_FragColor.w = f_Color.w
}

var program : uint
var geo_sphere : OpenGLGeometryFragment
var geo_cube : OpenGLGeometryFragment
var geo_plane : OpenGLGeometryFragment
var hud_font : Font?
var display_w = 0
var display_h = 0
var skip_draw : array<bool>

def gen_sine_sweep(freq_start, freq_end : float; duration : float; volume : float = 0.3) : array<float> {
    let num_samples = int(float(AUDIO_RATE) * duration)
    var samples : array<float>
    samples |> resize(num_samples)
    for (i in range(num_samples)) {
        let t = float(i) / float(AUDIO_RATE)
        let frac = t / duration
        let freq = freq_start + (freq_end - freq_start) * frac
        samples[i] = sin(2.0 * PI * freq * t) * (1.0 - frac) * volume
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
        seed = seed * 1103515245u + 12345u
        let noise = float(int(seed >> 16u) % 1000) / 500.0 - 1.0
        samples[i] = noise * (1.0 - frac) * volume
    }
    return <- samples
}

def gen_tone(freq, duration, volume : float) : array<float> {
    return <- gen_sine_sweep(freq, freq, duration, volume)
}

def overlay_at(var dst : array<float>; src : array<float>; at : int) {
    let need = at + length(src)
    if (length(dst) < need) {
        dst |> resize(need)
    }
    for (i in range(length(src))) {
        dst[at + i] += src[i]
    }
}

def gen_fanfare(base : float; vol : float = 0.28) : array<float> {
    var out : array<float>
    let step = int(float(AUDIO_RATE) * 0.11)
    let n1 <- gen_tone(base, 0.09, vol)
    let n2 <- gen_tone(base * 1.25, 0.09, vol)
    let n3 <- gen_tone(base * 1.5, 0.10, vol * 1.05)
    let n4 <- gen_tone(base * 2.0, 0.28, vol * 1.15)
    overlay_at(out, n1, 0)
    overlay_at(out, n2, step)
    overlay_at(out, n3, step * 2)
    overlay_at(out, n4, step * 3)
    return <- out
}

var @live snd_dig : array<float>
var @live snd_gem : array<float>
var @live snd_land_boulder : array<float>
var @live snd_land_diamond : array<float>
var @live snd_fall_boulder : array<float>
var @live snd_fall_diamond : array<float>
var @live snd_boom : array<float>
var @live snd_die : array<float>
var @live snd_timeout : array<float>
var @live snd_exit_open : array<float>
var @live snd_clear : array<float>
var @live snd_push : array<float>
var @live snd_magic : array<float>
var @live snd_magic_convert : array<float>
var @live snd_amoeba : array<float>
var @live snd_amoeba_gems : array<float>
var @live snd_amoeba_rocks : array<float>
var @live snd_expand : array<float>
var @live snd_time_low : array<float>
var @live audio_initialized = false

def init_audio_samples() {
    delete snd_dig
    delete snd_gem
    delete snd_land_boulder
    delete snd_land_diamond
    delete snd_fall_boulder
    delete snd_fall_diamond
    delete snd_boom
    delete snd_die
    delete snd_timeout
    delete snd_exit_open
    delete snd_clear
    delete snd_push
    delete snd_magic
    delete snd_magic_convert
    delete snd_amoeba
    delete snd_amoeba_gems
    delete snd_amoeba_rocks
    delete snd_expand
    delete snd_time_low
    snd_dig <- gen_sine_sweep(180.0, 90.0, 0.05, 0.2)
    snd_gem <- gen_sine_sweep(980.0, 1560.0, 0.09, 0.28)
    snd_land_boulder <- gen_sine_sweep(140.0, 55.0, 0.07, 0.24)
    snd_land_diamond <- gen_sine_sweep(1400.0, 2100.0, 0.08, 0.26)
    snd_fall_boulder <- gen_sine_sweep(200.0, 110.0, 0.04, 0.18)
    snd_fall_diamond <- gen_sine_sweep(1600.0, 2200.0, 0.035, 0.22)
    snd_boom <- gen_noise_burst(0.25, 0.28)
    snd_die <- gen_sine_sweep(440.0, 80.0, 0.5, 0.3)
    snd_timeout <- gen_sine_sweep(220.0, 40.0, 0.7, 0.32)
    snd_exit_open <- gen_fanfare(392.0, 0.3)
    snd_clear <- gen_fanfare(523.25, 0.32)
    snd_push <- gen_sine_sweep(90.0, 70.0, 0.08, 0.22)
    snd_magic <- gen_sine_sweep(300.0, 900.0, 0.18, 0.22)
    snd_magic_convert <- gen_sine_sweep(700.0, 1400.0, 0.1, 0.24)
    snd_amoeba <- gen_sine_sweep(220.0, 160.0, 0.06, 0.16)
    snd_amoeba_gems <- gen_sine_sweep(600.0, 1600.0, 0.25, 0.26)
    snd_amoeba_rocks <- gen_sine_sweep(160.0, 50.0, 0.3, 0.26)
    snd_expand <- gen_sine_sweep(240.0, 180.0, 0.07, 0.18)
    snd_time_low <- gen_sine_sweep(880.0, 440.0, 0.15, 0.22)
}

def play_sfx(samples : array<float>) {
    return if (!audio_initialized)
    var copy <- clone(samples)
    play_sound_from_pcm(AUDIO_RATE, AUDIO_CHANNELS, copy)
}

var g_music_tracks : table<string; int>
var music_initialized = false
var music_enabled = false

def strudel_music_cmd(cmd : string) {
    let parts <- split(cmd, ":")
    return if (length(parts) < 2)
    let action = parts[0]
    let name = parts[1]
    if (action == "play" && length(parts) >= 3) {
        let gain = length(parts) >= 4 ? float(to_double(parts[3])) : 1.0
        if (key_exists(g_music_tracks, name)) {
            strudel_remove_track(g_music_tracks[name])
        }
        g_music_tracks[name] = strudel_add_track(s(parts[2]), gain)
    } elif (action == "note" && length(parts) >= 4) {
        let gain = length(parts) >= 5 ? float(to_double(parts[4])) : 0.4
        if (key_exists(g_music_tracks, name)) {
            strudel_remove_track(g_music_tracks[name])
        }
        g_music_tracks[name] = strudel_add_track(note_pattern(parts[2], parts[3]), gain)
    } elif (action == "stop") {
        if (key_exists(g_music_tracks, name)) {
            let fade_time = length(parts) >= 3 ? float(to_double(parts[2])) : 0.5
            strudel_fade_track(g_music_tracks[name], 0.0, fade_time)
        }
    }
}

def strudel_music_main() {
    strudel_set_bpm(110.0lf)
    strudel_play(@@strudel_music_cmd)
}

def stop_all_music(fade : float) {
    strudel_command("stop:drums:{fade}")
    strudel_command("stop:hats:{fade}")
    strudel_command("stop:bass:{fade}")
    strudel_command("stop:lead:{fade}")
    strudel_command("stop:arp:{fade}")
}

def start_menu_music() {
    strudel_set_bpm(92.0lf)
    strudel_command("play:drums:<bd ~ ~ hh?0.4> <~ sd ~ ~> <bd ~ [~ hh?0.5] ~> <~ ~ sd?0.6 cp?0.3>:0.32")
    strudel_command("play:hats:hh(3,8):0.18")
    strudel_command("note:bass:<[d2 ~] [a1 ~] [f1 ~] [g1 ~] [d2 ~] [c2 ~] [bb1 ~] [a1 a1?0.5]>:sine:0.22")
    strudel_command("note:lead:<d4 ~ f4 ~ a4 ~ g4 ~> <~ f4 ~ d4 ~ a3 ~ c4> <d4 f4?0.6 ~ a4 ~ g4 f4 ~> <~ d4 ~ ~ a4?0.5 ~ ~ ~>:triangle:0.14")
}

def start_play_music() {
    strudel_set_bpm(118.0lf)
    strudel_command("play:drums:<bd [hh hh] sd [hh hh] bd [hh hh] sd [hh cp] bd [hh?0.75 hh] sd [hh hh] bd [hh hh] [sd?0.45 sd] [hh cp?0.35] bd [hh hh] sd [hh?0.6 hh] bd [hh cp?0.4] sd [hh hh] bd [hh hh] sd [cp?0.5 hh]>:0.48")
    strudel_command("play:hats:hh(5,8,2):0.2")
    strudel_command("note:bass:<[d2 d2] [d2 a1] [f2 f2] [g2 g2] [d2 d2] [c2 c2] [bb1 bb1] [a1 a1] [d2 d2] [d2 f2] [g2 g2] [a1 a1] [d2 d2] [f2?0.65 f2] [g2 g2] [a1 c2]>:square:0.24")
    strudel_command("note:lead:<d4 ~ f4 a4 ~ g4 f4 ~ d4 ~ a4 c5 ~ g4 f4 d4 ~ f4 ~ g4 a4 ~ c5 a4 g4 ~ d4 ~ [f4 g4] a4 ~ g4?0.55 f4 d4 ~ a4 ~ f4 ~ d4 g4 ~ f4 ~>:triangle:0.16")
    strudel_command("note:arp:<[d4 f4 a4 d5] [f4 a4 c5 f5?0.45] [d4 f4 a4 d5] [c4 e4 g4 c5?0.4] [d4 f4 a4 d5] [a3 d4 f4 a4] [d4 f4 a4 d5?0.5] [g3 bb3 d4 g4]>:triangle:0.09")
}

def start_gameover_music() {
    strudel_set_bpm(68.0lf)
    strudel_command("play:drums:<bd ~ ~ ~> <~ ~ bd?0.5 ~> <bd ~ hh?0.3 ~> <~ ~ ~ bd?0.35>:0.2")
    strudel_command("note:bass:<d2 ~ bb1 ~> <a1 ~ g1 ~> <f1 ~ a1 ~> <d2 ~ ~ ~>:sine:0.26")
    strudel_command("note:lead:<f4 ~ e4 ~> <d4 ~ c4 ~> <bb3 ~ a3 ~> <d4?0.6 ~ ~ ~>:triangle:0.16")
}

def start_music_for_state() {
    return if (!music_enabled)
    if (game_state == GameState.game_over_state) {
        start_gameover_music()
    } elif (game_state == GameState.menu) {
        start_menu_music()
    } else {
        start_play_music()
    }
    prev_music_state = game_state
}

var @live g_cave <- Cave()
var @live score = 0
var @live lives = START_LIVES
var @live game_state = GameState.menu
var @live world_seed = 1984
var @live cave_index = 0
var @live tick_acc = 0.0
var @live cave_hz = CAVE_HZ
var @live game_speed = 1.0
var @live god_mode = false
var @live death_timer = 0.0
var @live clear_timer = 0.0
var @live prev_music_state = GameState.menu
var space_was_pressed = false
var esc_was_pressed = false
var asch : AudioSystemChannels
var time_low_armed = false
var @live queued_dir = int2(0, 0)
var @live queued_grab = false
var player_slide = false
var player_from = int2(0, 0)
var player_to = int2(0, 0)
var player_slide_t = 0.0
var player_slide_dur = 0.12
var hold_time = 0.0
var step_this_press = false
let REPEAT_DELAY = 0.32
var key_left = false
var key_right = false
var key_up = false
var key_down = false
var key_grab = false

def half_w() : float {
    return float(g_cave.cols) * CELL_SIZE * 0.5
}

def half_h() : float {
    return float(g_cave.rows) * CELL_SIZE * 0.5
}

def cell_to_world(col, row : int) : float2 {
    return float2(
        float(col) * CELL_SIZE - half_w() + CELL_SIZE * 0.5,
        half_h() - float(row) * CELL_SIZE - CELL_SIZE * 0.5
    )
}

def create_gl_objects() {
    program = cache_shader_program(vs_bd`shader_text, fs_bd`shader_text)
    geo_sphere <- create_geometry_fragment <| gen_sphere(12, 7, false)
    geo_cube <- create_geometry_fragment <| gen_cube()
    geo_plane <- create_geometry_fragment <| gen_plane(GenDirection.xy)
    cache_ttf_objects()
    hud_font = cache_font("{get_das_root()}/modules/dasStbImage/fonts/droidsansmono.ttf")
}

def draw_box(pos : float3; dim : float3; color : float3; alpha : float = 1.0) {
    v_model = compose(pos, float4(0), dim * 0.5)
    f_Color = float4(color, alpha)
    vs_bd_bind_uniform(program)
    fs_bd_bind_uniform(program)
    geo_cube |> draw_geometry_fragment()
}

def draw_sphere_obj(pos : float3; radius : float; color : float3; alpha : float = 1.0) {
    v_model = compose(pos, float4(0), float3(radius))
    f_Color = float4(color, alpha)
    vs_bd_bind_uniform(program)
    fs_bd_bind_uniform(program)
    geo_sphere |> draw_geometry_fragment()
}

def draw_text(txt : string; x, y : int; tint : float3 = float3(1.0)) {
    return if (hud_font == null)
    var quads <- (*hud_font) |> create_quads(txt)
    (*hud_font) |> draw_quads_2d(quads, display_w, display_h, x, y, tint)
    delete quads
}

def draw_text_centered(txt : string; cx, y : int; tint : float3 = float3(1.0)) {
    return if (hud_font == null)
    var quads <- (*hud_font) |> create_quads(txt)
    let dim = quads_dim(quads)
    let tw = int(dim.vmax.x - dim.vmin.x)
    (*hud_font) |> draw_quads_2d(quads, display_w, display_h, cx - tw / 2, y, tint)
    delete quads
}

def pulse_tint(base : float3) : float3 {
    let pulse = 0.5 + 0.5 * sin(get_uptime() * 4.5)
    return lerp(base * 0.86, base * 1.06, pulse)
}

def set_mat(spec, gloss, rim, wave, hash : float) {
    f_Spec = spec
    f_Gloss = gloss
    f_Rim = rim
    f_Wave = wave
    v_Wave = wave
    f_Hash = hash
}

def mat_for(t : Tile) {
    if (t == Tile.dirt) {
        set_mat(0.06, 8.0, 0.05, 0.0, 0.28)
        return
    }
    if (is_gem_tile(t) || t == Tile.dexplode0 || t == Tile.dexplode1 || t == Tile.dexplode2) {
        set_mat(1.45, 72.0, 0.62, 0.0, 0.06)
        return
    }
    if (t == Tile.boulder || t == Tile.boulder_fall) {
        set_mat(0.32, 22.0, 0.14, 0.0, 0.20)
        return
    }
    if (t == Tile.brick) {
        set_mat(0.14, 12.0, 0.10, 0.0, 0.16)
        return
    }
    if (t == Tile.steel || t == Tile.exit_closed) {
        set_mat(0.55, 36.0, 0.22, 0.0, 0.12)
        return
    }
    if (t == Tile.magic || t == Tile.magic_active) {
        let pulse = 0.5 + 0.5 * sin(get_uptime() * 5.0)
        set_mat(0.62, 28.0, 0.35 + pulse * 0.55, 0.35, 0.10)
        return
    }
    if (t == Tile.expand) {
        set_mat(0.38, 18.0, 0.32, 1.0, 0.08)
        return
    }
    if (t == Tile.slime) {
        set_mat(0.48, 42.0, 0.22, 0.40, 0.18)
        return
    }
    if (t == Tile.amoeba) {
        let pulse = 0.5 + 0.5 * sin(get_uptime() * 4.5)
        set_mat(0.36, 16.0, 0.40 + pulse * 0.45, 0.55, 0.14)
        return
    }
    if (is_firefly(t)) {
        set_mat(0.85, 30.0, 0.85, 0.18, 0.0)
        return
    }
    if (is_butterfly(t)) {
        set_mat(0.75, 26.0, 0.80, 0.22, 0.0)
        return
    }
    if (t == Tile.player) {
        set_mat(0.42, 24.0, 0.28, 0.0, 0.0)
        return
    }
    if (t == Tile.exit_open) {
        let pulse = 0.5 + 0.5 * sin(get_uptime() * 4.5)
        set_mat(0.75, 34.0, 0.45 + pulse * 0.5, 0.25, 0.0)
        return
    }
    if (is_explode(t)) {
        set_mat(0.95, 10.0, 0.55, 0.15, 0.0)
        return
    }
    set_mat(0.10, 12.0, 0.08, 0.0, 0.0)
}

def tile_color(t : Tile) : float3 {
    if (t == Tile.dirt) { return COL_DIRT }
    if (is_gem_tile(t) || t == Tile.dexplode0 || t == Tile.dexplode1 || t == Tile.dexplode2) {
        return COL_DIAMOND
    }
    if (t == Tile.boulder || t == Tile.boulder_fall) { return COL_BOULDER }
    if (t == Tile.brick) { return COL_BRICK }
    if (t == Tile.steel || t == Tile.exit_closed) { return COL_STEEL }
    if (t == Tile.magic || t == Tile.magic_active) { return COL_MAGIC }
    if (t == Tile.expand) { return COL_EXPAND }
    if (t == Tile.slime) { return COL_SLIME }
    if (t == Tile.amoeba) { return COL_AMOEBA }
    if (is_firefly(t)) { return COL_FIREFLY }
    if (is_butterfly(t)) { return COL_BUTTERFLY }
    if (t == Tile.player) { return COL_PLAYER }
    if (t == Tile.exit_open) { return COL_EXIT }
    if (is_explode(t)) { return COL_EXPLODE }
    return COL_FLOOR
}

def tile_sphere(t : Tile) : bool {
    return t == Tile.boulder || t == Tile.boulder_fall || t == Tile.amoeba || t == Tile.player || is_insect(t) || is_explode(t)
}

def draw_tile(t : Tile; pos : float3) {
    return if (t == Tile.empty)
    mat_for(t)
    var col = tile_color(t)
    if (t == Tile.magic_active || t == Tile.exit_open || t == Tile.amoeba || t == Tile.expand || is_insect(t)) {
        col = pulse_tint(col)
    }
    if (t == Tile.dirt) {
        draw_box(pos, float3(0.95, 0.95, 0.45), col)
        return
    }
    if (is_gem_tile(t)) {
        let ang = get_uptime() * 3.0
        let q = float4(0.0, 0.0, sin(ang * 0.5), cos(ang * 0.5))
        v_model = compose(pos + float3(0.0, 0.0, 0.22), q, float3(0.28))
        f_Color = float4(col, 1.0)
        vs_bd_bind_uniform(program)
        fs_bd_bind_uniform(program)
        geo_cube |> draw_geometry_fragment()
        return
    }
    if (tile_sphere(t)) {
        var r = 0.38
        if (t == Tile.player) { r = 0.42 }
        if (is_explode(t)) { r = 0.5 }
        if (t == Tile.amoeba) { r = 0.36 + 0.06 * sin(get_uptime() * 5.0) }
        draw_sphere_obj(pos + float3(0.0, 0.0, r), r, col)
        return
    }
    var h = 0.7
    if (t == Tile.steel || t == Tile.exit_closed) { h = 0.85 }
    if (t == Tile.slime) {
        draw_box(pos + float3(0.0, 0.0, 0.15), float3(0.95, 0.95, 0.35), col, 0.55)
        return
    }
    if (t == Tile.exit_open) {
        draw_box(pos + float3(0.0, 0.0, 0.4), float3(0.7, 0.7, 0.85), col)
        return
    }
    draw_box(pos + float3(0.0, 0.0, h * 0.5), float3(1.0, 1.0, h), col)
}

def draw_floor() {
    set_mat(0.05, 8.0, 0.0, 0.0, 0.12)
    v_model = compose(float3(0.0, 0.0, -0.02), float4(0), float3(half_w() + 1.0, half_h() + 1.0, 1.0))
    f_Color = float4(COL_FLOOR, 1.0)
    vs_bd_bind_uniform(program)
    fs_bd_bind_uniform(program)
    geo_plane |> draw_geometry_fragment()
}

def mark_movers() {
    let n = g_cave.cols * g_cave.rows
    if (length(skip_draw) != n) {
        skip_draw |> resize(n)
    }
    for (i in range(n)) {
        skip_draw[i] = false
    }
    for (m in g_cave.movers) {
        if (m.tile == Tile.player) {
            continue
        }
        if (in_bounds(g_cave, m.to_x, m.to_y)) {
            skip_draw[cave_idx(g_cave, m.to_x, m.to_y)] = true
        }
    }
}

def draw_static_tiles() {
    for (y in range(g_cave.rows)) {
        for (x in range(g_cave.cols)) {
            if (skip_draw[cave_idx(g_cave, x, y)]) {
                continue
            }
            let t = cave_at(g_cave, x, y)
            if (t == Tile.player && player_slide) {
                continue
            }
            if (player_slide && x == player_to.x && y == player_to.y) {
                if (t == Tile.dirt || is_gem_tile(t) || t == Tile.empty) {
                    continue
                }
            }
            let w = cell_to_world(x, y)
            draw_tile(t, float3(w.x, w.y, 0.0))
        }
    }
}

def draw_movers(blend : float) {
    let u = saturate(blend)
    let s = u * u * (3.0 - 2.0 * u)
    for (m in g_cave.movers) {
        if (m.tile == Tile.player) {
            continue
        }
        let a = cell_to_world(m.from_x, m.from_y)
        let b = cell_to_world(m.to_x, m.to_y)
        let p = lerp(a, b, s)
        draw_tile(m.tile, float3(p.x, p.y, 0.0))
    }
}

def draw_player_slide() {
    return if (!player_slide)
    let u = saturate(player_slide_t)
    let s = u * u * (3.0 - 2.0 * u)
    let a = cell_to_world(player_from.x, player_from.y)
    let b = cell_to_world(player_to.x, player_to.y)
    let p = lerp(a, b, s)
    draw_tile(Tile.player, float3(p.x, p.y, 0.0))
}

def maybe_start_player_slide(remain : float) {
    return if (player_slide || queued_grab || !g_cave.alive)
    let dest = cave_peek_step(g_cave, g_cave.wish_dir)
    return if (dest.x == g_cave.rockford.x && dest.y == g_cave.rockford.y)
    player_slide = true
    player_from = g_cave.rockford
    player_to = dest
    player_slide_t = 0.0
    player_slide_dur = max(remain, 1.0 / 120.0)
    cave_lock_input(g_cave)
}

def draw_hud() {
    let cx = display_w / 2
    let cy = display_h / 2
    if (game_state == GameState.menu) {
        draw_text_centered("BOULDER DASH", cx, cy - 70, float3(1.0, 0.75, 0.2))
        draw_text_centered("PRESS SPACE TO START", cx, cy + 10, pulse_tint(float3(0.7)))
        draw_text_centered("ARROWS MOVE   Z/CTRL GRAB", cx, cy + 50, float3(0.55))
        return
    }
    draw_text("SCORE {score}", 10, 32, float3(1.0))
    draw_text("LIVES {lives}", 10, 62, float3(1.0, 0.85, 0.3))
    draw_text("GEMS {g_cave.diamonds_got}/{g_cave.diamonds_need}", 10, 92, COL_DIAMOND)
    var tcol = float3(1.0, 1.0, 1.0)
    if (g_cave.time_left < 10.0) {
        tcol = float3(1.0, 0.25, 0.2)
    }
    draw_text("TIME {int(g_cave.time_left)}", 10, 122, tcol)
    draw_text("CAVE {cave_index + 1}", display_w - 160, 32, float3(0.7))
    if (god_mode) {
        draw_text("GOD", display_w - 80, 62, float3(1.0, 0.5, 0.0))
    }
    if (game_state == GameState.paused) {
        draw_text_centered("PAUSED", cx, cy - 10, float3(0.85))
        draw_text_centered("ESC TO RESUME", cx, cy + 30, pulse_tint(float3(0.5)))
    } elif (game_state == GameState.game_over_state) {
        draw_text_centered("GAME OVER", cx, cy - 20, float3(1.0, 0.25, 0.2))
        draw_text_centered("PRESS SPACE", cx, cy + 25, pulse_tint(float3(0.7)))
    } elif (game_state == GameState.cave_cleared) {
        draw_text_centered("CAVE CLEARED", cx, cy - 20, float3(0.3, 1.0, 0.4))
        draw_text_centered("PRESS SPACE", cx, cy + 25, pulse_tint(float3(0.7)))
    } elif (game_state == GameState.dead) {
        draw_text_centered("CRUSHED", cx, cy - 10, float3(1.0, 0.4, 0.2))
    }
}

def play_tick_sfx() {
    if (g_cave.dug) { play_sfx(snd_dig) }
    if (g_cave.collected > 0) { play_sfx(snd_gem) }
    if (g_cave.fell_boulder) { play_sfx(snd_fall_boulder) }
    if (g_cave.fell_diamond) { play_sfx(snd_fall_diamond) }
    if (g_cave.landed_boulder) { play_sfx(snd_land_boulder) }
    if (g_cave.landed_diamond) { play_sfx(snd_land_diamond) }
    if (g_cave.exploded) { play_sfx(snd_boom) }
    if (g_cave.died) {
        play_sfx(g_cave.timed_out ? snd_timeout : snd_die)
    }
    if (g_cave.exit_opened) { play_sfx(snd_exit_open) }
    if (g_cave.cleared) { play_sfx(snd_clear) }
    if (g_cave.pushed) { play_sfx(snd_push) }
    if (g_cave.magic_on) { play_sfx(snd_magic) }
    if (g_cave.magic_convert) { play_sfx(snd_magic_convert) }
    if (g_cave.amoeba_grew) { play_sfx(snd_amoeba) }
    if (g_cave.amoeba_gems) { play_sfx(snd_amoeba_gems) }
    if (g_cave.amoeba_rocks) { play_sfx(snd_amoeba_rocks) }
    if (g_cave.wall_grew) { play_sfx(snd_expand) }
}

def begin_cave() {
    var next <- generate_cave(world_seed, cave_index)
    adopt_cave(g_cave, next)
    g_cave.god = god_mode
    tick_acc = 0.0
    time_low_armed = false
    queued_dir = int2(0, 0)
    queued_grab = false
    hold_time = 0.0
    step_this_press = false
    player_slide = false
}

def reset_game() {
    score = 0
    lives = START_LIVES
    cave_index = 0
    begin_cave()
    game_state = GameState.playing
}

def grabbing() : bool {
    let z = glfwGetKey(live_window, GLFW_KEY_Z) == GLFW_PRESS
    let ctrl = glfwGetKey(live_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS
    let rctrl = glfwGetKey(live_window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS
    return z || ctrl || rctrl
}

def held_dir(left, right, up, down : bool) : int2 {
    var dx = 0
    var dy = 0
    if (left) { dx = -1 }
    if (right) { dx = 1 }
    if (up) { dy = -1 }
    if (down) { dy = 1 }
    if (dx != 0 && dy != 0) {
        if (queued_dir.y != 0 && ((queued_dir.y < 0 && up) || (queued_dir.y > 0 && down))) {
            return int2(0, queued_dir.y)
        }
        return int2(dx, 0)
    }
    return int2(dx, dy)
}

def poll_move_keys(dt : float) {
    let left = glfwGetKey(live_window, GLFW_KEY_LEFT) == GLFW_PRESS
    let right = glfwGetKey(live_window, GLFW_KEY_RIGHT) == GLFW_PRESS
    let up = glfwGetKey(live_window, GLFW_KEY_UP) == GLFW_PRESS
    let down = glfwGetKey(live_window, GLFW_KEY_DOWN) == GLFW_PRESS
    let grab = grabbing()
    var tapped = false
    if (left && !key_left) { queued_dir = int2(-1, 0); tapped = true }
    if (right && !key_right) { queued_dir = int2(1, 0); tapped = true }
    if (up && !key_up) { queued_dir = int2(0, -1); tapped = true }
    if (down && !key_down) { queued_dir = int2(0, 1); tapped = true }
    if (tapped) {
        step_this_press = false
        hold_time = 0.0
    }
    if (grab && !key_grab) {
        queued_grab = true
    }
    let held = held_dir(left, right, up, down)
    if (held.x != 0 || held.y != 0) {
        hold_time += dt
        if (step_this_press && hold_time >= REPEAT_DELAY) {
            queued_dir = held
            if (grab) {
                queued_grab = true
            }
        }
    } else {
        hold_time = 0.0
        step_this_press = false
    }
    key_left = left
    key_right = right
    key_up = up
    key_down = down
    key_grab = grab
}

def consume_move_queue() {
    cave_consume_input(g_cave)
    queued_dir = g_cave.wish_dir
    queued_grab = g_cave.grab
    step_this_press = true
}

def handle_menu_keys(space_just, esc_just : bool) {
    if (game_state == GameState.menu && space_just) {
        reset_game()
        return
    }
    if (game_state == GameState.playing && esc_just) {
        game_state = GameState.paused
        return
    }
    if (game_state == GameState.paused && (esc_just || space_just)) {
        game_state = GameState.playing
        return
    }
    if (game_state == GameState.game_over_state && space_just) {
        reset_game()
        return
    }
    if (game_state == GameState.cave_cleared && space_just) {
        cave_index++
        begin_cave()
        game_state = GameState.playing
    }
}

def after_death() {
    lives--
    if (lives <= 0) {
        game_state = GameState.game_over_state
        return
    }
    begin_cave()
    game_state = GameState.playing
}

def sim_playing(dt : float) {
    g_cave.god = god_mode
    poll_move_keys(dt)
    cave_latch_input(g_cave, queued_dir, queued_grab)
    cave_apply_time(g_cave, dt)
    let step = 1.0 / max(cave_hz * game_speed, 0.5)
    tick_acc += dt
    if (tick_acc >= step) {
        tick_acc -= step
        if (tick_acc >= step) {
            tick_acc = 0.0
        }
        cave_tick(g_cave)
        score += g_cave.collected * 10
        play_tick_sfx()
        consume_move_queue()
        player_slide = false
    } else {
        maybe_start_player_slide(step - tick_acc)
        if (player_slide) {
            player_slide_t += dt / player_slide_dur
            if (player_slide_t > 1.0) {
                player_slide_t = 1.0
            }
        }
    }
    if (g_cave.cleared) {
        score += int(g_cave.time_left)
        game_state = GameState.cave_cleared
        clear_timer = 1.4
        return
    }
    if (!g_cave.alive) {
        player_slide = false
        game_state = GameState.dead
        death_timer = 1.4
    }
    if (g_cave.time_left < 10.0 && !time_low_armed) {
        time_low_armed = true
        play_sfx(snd_time_low)
    }
}

def update_music() {
    return if (!music_enabled || prev_music_state == game_state)
    stop_all_music(0.2)
    start_music_for_state()
}

def setup_camera() {
    let aspect = display_h != 0 ? float(display_w) / float(display_h) : 1.0
    v_projection = perspective_rh_opengl(50.0 * PI / 180.0, aspect, 0.1, 200.0)
    v_view = look_at_rh(CAM_EYE, float3(0.0, 0.0, 0.0), float3(0.0, 1.0, 0.0))
    f_Eye = CAM_EYE
    let t = get_uptime()
    v_Time = t
    f_Time = t
}

[export]
def init() {
    live_create_window("Boulder Dash", 1280, 720)
    create_gl_objects()
    music_enabled = !audio_is_single_threaded()
    if (!audio_initialized) {
        asch = audio_system_create()
        audio_initialized = true
        init_audio_samples()
    }
    if (music_enabled && !music_initialized) {
        strudel_init(@@strudel_music_main)
        strudel_set_volume(0.4, 0.0)
        music_initialized = true
        start_music_for_state()
    }
    if (!is_reload()) {
        var next <- generate_cave(world_seed, cave_index)
        adopt_cave(g_cave, next)
        g_cave.god = god_mode
    }
}

[export]
def update() {
    if (!live_begin_frame()) {
        return
    }
    live_get_framebuffer_size(display_w, display_h)
    glViewport(0, 0, display_w, display_h)
    glClearColor(0.03, 0.03, 0.05, 1.0)
    glClearDepthf(1.0)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glEnable(GL_DEPTH_TEST)
    glEnable(GL_CULL_FACE)
    setup_camera()

    let dt = min(get_dt(), 1.0 / 30.0)
    let space_pressed = glfwGetKey(live_window, GLFW_KEY_SPACE) == GLFW_PRESS
    let space_just = space_pressed && !space_was_pressed
    space_was_pressed = space_pressed
    let esc_pressed = glfwGetKey(live_window, GLFW_KEY_ESCAPE) == GLFW_PRESS
    let esc_just = esc_pressed && !esc_was_pressed
    esc_was_pressed = esc_pressed
    handle_menu_keys(space_just, esc_just)

    if (game_state == GameState.dead) {
        death_timer -= dt
        if (death_timer <= 0.0) {
            after_death()
        }
    } elif (game_state == GameState.cave_cleared) {
        clear_timer -= dt
        if (clear_timer <= 0.0) {
            cave_index++
            begin_cave()
            game_state = GameState.playing
        }
    } elif (game_state == GameState.playing) {
        sim_playing(dt)
    }

    update_music()
    glUseProgram(program)
    draw_floor()
    mark_movers()
    draw_static_tiles()
    let blend = saturate(tick_acc * max(cave_hz * game_speed, 0.5))
    draw_movers(blend)
    draw_player_slide()
    glDisable(GL_CULL_FACE)
    draw_hud()
    live_end_frame()
}

[export]
def shutdown() {
    if (music_enabled) {
        strudel_shutdown()
    }
    if (!is_reload()) {
        audio_system_finalize(asch.command, asch.next_sid)
    }
    live_destroy_window()
}

var max_frame_limit = 0

def parse_args() {
    let args <- get_command_line_arguments()
    for (i in range(length(args) - 1)) {
        if (args[i] == "--max-frames") {
            max_frame_limit = try_to_int(args[i + 1]) ?? 0
        }
    }
}

struct GameStatusResult {
    score : int
    lives : int
    cave_index : int
    seed : int
    diamonds_got : int
    diamonds_need : int
    time_left : float
    rockford_x : int
    rockford_y : int
    state : string
    god_mode : bool
}

[live_command(description="Score, lives, cave, seed, diamonds, time, Rockford")]
def cmd_game_status(_input : JsonValue?) : JsonValue? {
    return JV(GameStatusResult(
        score = score,
        lives = lives,
        cave_index = cave_index,
        seed = world_seed,
        diamonds_got = g_cave.diamonds_got,
        diamonds_need = g_cave.diamonds_need,
        time_left = g_cave.time_left,
        rockford_x = g_cave.rockford.x,
        rockford_y = g_cave.rockford.y,
        state = "{game_state}",
        god_mode = god_mode
    ))
}

[live_command(description="Reset game, keep seed")]
def cmd_reset_game(_input : JsonValue?) : JsonValue? {
    reset_game()
    return JV(true)
}

struct CmdSeedArgs {
    seed : int = 1984
}

[live_command(description="Set world seed and regenerate. Args: seed")]
def cmd_set_seed(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<CmdSeedArgs>)
    world_seed = args.seed
    begin_cave()
    game_state = GameState.playing
    return JV(args)
}

[live_command(description="Advance to next cave")]
def cmd_next_cave(_input : JsonValue?) : JsonValue? {
    cave_index++
    begin_cave()
    game_state = GameState.playing
    return JV(cave_index)
}

[live_command(description="Go to previous cave")]
def cmd_prev_cave(_input : JsonValue?) : JsonValue? {
    cave_index = max(cave_index - 1, 0)
    begin_cave()
    game_state = GameState.playing
    return JV(cave_index)
}

[live_command(description="Toggle god mode (no crush, insects, or timeout)")]
def cmd_god_mode(_input : JsonValue?) : JsonValue? {
    god_mode = !god_mode
    g_cave.god = god_mode
    return JV(god_mode)
}

struct CmdTimeArgs {
    seconds : float = 150.0
}

[live_command(description="Set remaining time. Args: seconds")]
def cmd_set_time(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<CmdTimeArgs>)
    g_cave.time_left = args.seconds
    return JV(args)
}

struct CmdGemsArgs {
    got : int = 0
}

[live_command(description="Set diamonds collected. Args: got")]
def cmd_set_diamonds(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<CmdGemsArgs>)
    g_cave.diamonds_got = args.got
    cave_tick(g_cave)
    return JV(args)
}

[live_command(description="Dump current cave as ASCII")]
def cmd_dump_cave(_input : JsonValue?) : JsonValue? {
    return JV(cave_to_ascii(g_cave))
}

struct CmdAsciiArgs {
    ascii : string = ""
}

[live_command(description="Load cave from ASCII. Args: ascii")]
def cmd_load_ascii(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<CmdAsciiArgs>)
    if (empty(args.ascii)) {
        return JV(false)
    }
    var loaded <- cave_from_ascii(args.ascii)
    adopt_cave(g_cave, loaded)
    g_cave.god = god_mode
    game_state = GameState.playing
    return JV(true)
}

[live_command(description="Force one cave tick")]
def cmd_tick(_input : JsonValue?) : JsonValue? {
    cave_tick(g_cave)
    score += g_cave.collected * 10
    play_tick_sfx()
    return JV(true)
}

struct CmdSpeedArgs {
    speed : float = 1.0
}

[live_command(description="Game speed multiplier. Args: speed")]
def cmd_slow_motion(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<CmdSpeedArgs>)
    game_speed = max(args.speed, 0.05)
    return JV(args)
}

[export]
def main() {
    parse_args()
    init()
    var frame_count = 0
    while (!exit_requested()) {
        update()
        maybe_collect_gc()
        frame_count++
        if (max_frame_limit > 0 && frame_count >= max_frame_limit) {
            break
        }
    }
    shutdown()
}
