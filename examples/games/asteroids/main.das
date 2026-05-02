options gen2
options persistent_heap

require glfw/glfw_boost
require live/glfw_live
require opengl/opengl_boost
require opengl/opengl_gen
require opengl/opengl_cache
require live/opengl_live
require live/live_commands
require live/live_api
require live/live_watch_boost
require live/live_vars
require live/audio_live
require audio/audio_boost
require strudel/strudel
require strudel/strudel_player
require strudel/strudel_live
require daslib/math_boost
require daslib/json
require daslib/json_boost
require daslib/strings_boost
require daslib/safe_addr
require opengl/opengl_ttf
require live_host

// --- Constants ---

let FIELD_W = 1024.0
let FIELD_H = 768.0
let SHIP_ROT_SPEED = 4.0
let SHIP_THRUST = 250.0
let SHIP_BACK_THRUST = 180.0
let SHIP_FRICTION = 0.98
let SHIP_INVULN_TIME = 1.0
let BULLET_SPEED = 500.0
let BULLET_LIFETIME = 1.5
let FIRE_COOLDOWN = 0.2
let WAVE_BANNER_DURATION = 2.8
let DEATH_HIDE_TIME = 2.0
let DEATH_BLINK_TIME = 1.0
let WAVE_SPEED_STEP = 0.05
let MAX_WAVES = 5
let SHIP_SIZE = 10.0
let BULLET_SIZE = 3.2
let ASTEROID_MIN_SIZE = 15.0
let ASTEROID_MAX_SIZE = 40.0
let ASTEROID_MIN_SPEED = 70.0
let ASTEROID_MAX_SPEED = 150.0
let BULLET_SPIN_MIN = 8.0
let BULLET_SPIN_MAX = 14.0
let POWERUP_DROP_CHANCE = 0.24
let POWERUP_LIFETIME = 9.0
let POWERUP_PICKUP_RADIUS = 18.0
let POWERUP_RAPID_FIRE_TIME = 10.0
let POWERUP_INVULN_TIME = 10.0
let PARTICLE_LIFETIME = 0.8
let AUDIO_RATE = 48000
let AUDIO_CHANNELS = 1

enum GameState { menu, playing, paused, game_over_state, win_state }
enum PowerupType { rapid_fire, extra_life, invulnerability }

struct AsteroidObj {
    pos : float3
    vel : float3
    rotation : float
    rotation_speed : float
    size : float
}

struct BulletObj {
    pos : float3
    prev_pos : float3
    vel : float3
    rotation : float
    rotation_speed : float
    lifetime : float
}

struct ParticleObj {
    pos : float3
    vel : float3
    color : float3
    lifetime : float
    max_life : float
}

struct PowerupObj {
    pos : float3
    vel : float3
    kind : PowerupType
    lifetime : float
    rotation : float
    rotation_speed : float
}

// --- Game State ---

var @live game_state = GameState.menu
var @live score = 0
var @flag lives = 3
var @live ship_rotation = 0.0
var @live ship_pos = float3(512, 384, 0)
var @live ship_vel = float3(0, 0, 0)
var @live last_shot_time : float = 0.0
var @live ship_invuln_timer : float = 0.0
var @live wave_number = 1
var wave_banner_timer = 0.0
var ship_respawn_timer = 0.0
var @live mouse_aim_enabled = true
var @live mouse_fire_enabled = true
var @live show_crosshair = true
var @live wireframe_mode = true
var @live wireframe_line_width = 1.8
var tick_dt = 0.0
var mouse_world = float3(512, 384, 0)

var g_asteroids : array<AsteroidObj>
var g_bullets : array<BulletObj>
var g_particles : array<ParticleObj>
var g_powerups : array<PowerupObj>

var rapid_fire_timer = 0.0
var invuln_powerup_timer = 0.0

// --- Audio ---

var @live snd_shoot : array<float>
var @live snd_asteroid_hit : array<float>
var @live snd_ship_hit : array<float>
var @live snd_wave_start : array<float>
var @live snd_game_over : array<float>
var @live audio_initialized = false
var asch : AudioSystemChannels
var g_music_tracks : table<string; int>
var @live music_initialized = false
var music_state = GameState.menu
var wave_music_active = false
var death_music_active = false

def init_audio_samples() {
    snd_shoot <- gen_sine_sweep(1400.0, 700.0, 0.06, 0.2)

    snd_asteroid_hit <- gen_noise_burst(0.09, 0.1)
    let asteroid_tone <- gen_sine_sweep(520.0, 220.0, 0.09, 0.14)
    mix_into(snd_asteroid_hit, asteroid_tone)

    snd_ship_hit <- gen_noise_burst(0.18, 0.2)
    let ship_tone <- gen_sine_sweep(420.0, 90.0, 0.22, 0.18)
    mix_into(snd_ship_hit, ship_tone)

    let wave_notes = fixed_array(392.0, 523.0, 659.0)
    snd_wave_start <- build_note_sequence(wave_notes, 0.08, 0.14, true)

    let game_over_notes = fixed_array(392.0, 294.0, 220.0, 165.0)
    snd_game_over <- build_note_sequence(game_over_notes, 0.11, 0.2, false)
}

def gen_sine_sweep(f1, f2, dur, vol) : array<float> {
    let n = int(float(AUDIO_RATE) * dur)
    var s : array<float>
    s |> resize(n)
    for (i in range(n)) {
        let t = float(i) / float(AUDIO_RATE)
        let f = f1 + (f2 - f1) * (t / dur)
        s[i] = sin(2.0 * PI * f * t) * vol
    }
    return <- s
}

def gen_noise_burst(dur, vol) : array<float> {
    let n = int(float(AUDIO_RATE) * dur)
    var s : array<float>
    s |> resize(n)
    var seed = 42u
    for (i in range(n)) {
        seed = seed * 1103515245u + 12345u
        let noise = float(int(seed >> 16u) % 1000) / 500.0 - 1.0
        s[i] = noise * vol
    }
    return <- s
}

def mix_into(var dst : array<float>; src : array<float>) {
    let n = min(length(dst), length(src))
    for (i in range(n)) {
        dst[i] += src[i]
    }
}

def build_note_sequence(notes : float[]; note_dur : float; volume : float; rising : bool) : array<float> {
    let total_dur = note_dur * float(length(notes))
    let total_samples = int(float(AUDIO_RATE) * total_dur)
    var seq : array<float>
    seq |> resize(total_samples)
    for (ni in range(length(notes))) {
        let f1 = notes[ni]
        let f2 = (rising ? notes[ni] * 1.05 : notes[ni] * 0.9)
        let note <- gen_sine_sweep(f1, f2, note_dur, volume)
        let offset = ni * int(float(AUDIO_RATE) * note_dur)
        for (i in range(length(note))) {
            if (offset + i < total_samples) {
                seq[offset + i] += note[i]
            }
        }
    }
    return <- seq
}

def play_sfx(samples : array<float>) {
    if (!audio_initialized) {
        return
    }
    var copy <- clone(samples)
    play_sound_from_pcm(AUDIO_RATE, AUDIO_CHANNELS, copy)
}

def music_cmd(cmd : string) {
    let parts <- split(cmd, ":")
    if (length(parts) < 2) {
        return
    }
    let action = parts[0]
    let name = parts[1]
    if (action == "play" && length(parts) >= 3) {
        let gain = (length(parts) >= 4 ? float(to_double(parts[3])) : 1.0)
        if (key_exists(g_music_tracks, name)) {
            strudel_remove_track(g_music_tracks[name])
        }
        g_music_tracks[name] = strudel_add_track(s(parts[2]), gain)
    } elif (action == "note" && length(parts) >= 4) {
        let gain = (length(parts) >= 5 ? float(to_double(parts[4])) : 0.3)
        if (key_exists(g_music_tracks, name)) {
            strudel_remove_track(g_music_tracks[name])
        }
        g_music_tracks[name] = strudel_add_track(note_pattern(parts[2], parts[3]), gain)
    } elif (action == "stop") {
        if (key_exists(g_music_tracks, name)) {
            let fade_time = (length(parts) >= 3 ? float(to_double(parts[2])) : 0.5)
            strudel_fade_track(g_music_tracks[name], 0.0, fade_time)
        }
    }
}

def stop_all_tracks(fade : float) {
    strudel_command("stop:drums:{fade}")
    strudel_command("stop:bass:{fade}")
    strudel_command("stop:lead:{fade}")
    strudel_command("stop:arp:{fade}")
}

def set_play_melody_tracks() {
    strudel_command("note:lead:<e4 g4 b4 e5> <d5 b4 g4 e4> <g4 b4 d5 g5> <a4 g4 e4 d4>:sawtooth:0.2")
    strudel_command("note:arp:<[e5 b4 g4 e4 b3 g3 e3 b2]> <[g5 d5 b4 g4 d4 b3 g3 d3]> <[a5 e5 c5 a4 e4 c4 a3 e3]> <[b5 g5 d5 b4 g4 d4 b3 g3]>:triangle:0.1")
}

def set_wave_banner_melody_tracks() {
    strudel_command("note:lead:<e5 g5 b5 e6> <g5 b5 d6 g6> <a5 c6 e6 a6> <b5 d6 fs6 b6>:square:0.22")
    strudel_command("note:arp:<[e6 b5 g5 e5]> <[g6 d6 b5 g5]> <[a6 e6 c6 a5]> <[b6 fs6 d6 b5]>:triangle:0.14")
}

def set_death_transition_tracks() {
    strudel_set_bpm(92.0lf)
    strudel_command("play:drums:<bd ~ ~ ~> <~ ~ hh ~> <bd ~ ~ ~> <~ hh ~ ~>:0.2")
    strudel_command("note:bass:<e2 ~ d2 ~> <c2 ~ b1 ~> <a1 ~ g1 ~> <e2 ~ d2 ~>:sine:0.2")
    strudel_command("note:lead:<e4 ~ d4 ~> <c4 ~ b3 ~> <a3 ~ g3 ~> <e4 ~ d4 ~>:triangle:0.14")
    strudel_command("note:arp:<[e5 b4 g4 e4]> <[d5 a4 f4 d4]> <[c5 g4 e4 c4]> <[b4 fs4 d4 b3]>:triangle:0.06")
}

def start_play_music() {
    strudel_set_bpm(134.0lf)
    strudel_command("play:drums:<bd [~ hh] ~ [hh cp?0.25]> <bd [~ hh] sd [hh ~]> <bd [~ hh] ~ [hh cp?0.35]> <bd [hh hh] sd [hh hh?0.6]>:0.52")
    strudel_command("note:bass:<[e2 e2] ~ [b1 b1] ~> <[e2 e2] [d2 d2] [c2 c2] [b1 b1]> <[e2 e2] ~ [g1 g1] ~> <[a1 a1] [b1 b1] [c2 c2] [d2 d2]>:square:0.27")
    set_play_melody_tracks()
}

def start_game_over_music() {
    strudel_set_bpm(84.0lf)
    strudel_command("play:drums:<bd ~ ~ ~> <bd ~ [~ hh?0.25] ~> <bd ~ ~ [~ bd?0.2]> <bd ~ ~ ~>:0.22")
    strudel_command("note:bass:<e2 d2 c2 b1> <a1 ~ g1 ~> <e2 d2 c2 b1> <a1 ~ g1 ~>:sine:0.26")
    strudel_command("note:lead:<g4 ~ e4 ~> <d4 ~ c4 ~> <b3 ~ a3 ~> <g3 ~ e3 ~>:triangle:0.17")
    strudel_command("note:arp:<[e4 b3 g3 e3]> <[d4 a3 f3 d3]> <[c4 g3 e3 c3]> <[b3 fs3 d3 b2]>:triangle:0.08")
}

def start_win_music() {
    strudel_set_bpm(148.0lf)
    strudel_command("play:drums:<bd [hh hh] sd [hh cp]> <bd [hh hh] [sd sd] [hh hh]> <bd [hh cp] sd [hh hh]> <bd [hh hh] [sd cp] [hh hh]>:0.58")
    strudel_command("note:bass:<[e2 e2] [g2 g2] [b2 b2] [e3 e3]> <[d3 d3] [b2 b2] [a2 a2] [g2 g2]> <[c3 c3] [a2 a2] [g2 g2] [b2 b2]> <[e2 e2] [g2 g2] [b2 b2] [e3 e3]>:square:0.3")
    strudel_command("note:lead:<e5 g5 b5 e6> <g5 b5 d6 g6> <a5 c6 e6 a6> <b5 d6 fs6 b6>:sawtooth:0.24")
    strudel_command("note:arp:<[e6 b5 g5 e5 b4 g4 e4 b3]> <[g6 d6 b5 g5 d5 b4 g4 d4]> <[a6 e6 c6 a5 e5 c5 a4 e4]> <[b6 fs6 d6 b5 fs5 d5 b4 fs4]>:triangle:0.12")
}

def strudel_music_main() {
    start_play_music()
    strudel_play(@@music_cmd)
}

def update_music_state() {
    if (!music_initialized || game_state == music_state) {
        return
    }
    stop_all_tracks(0.7)
    if (game_state == GameState.game_over_state) {
        start_game_over_music()
    } elif (game_state == GameState.win_state) {
        start_win_music()
    } elif (game_state == GameState.playing) {
        start_play_music()
    }
    wave_music_active = false
    death_music_active = false
    music_state = game_state
}

def update_overlay_music() {
    if (!music_initialized || game_state != GameState.playing) {
        wave_music_active = false
        death_music_active = false
        return
    }

    if (ship_respawn_timer > 0.0) {
        if (!death_music_active) {
            set_death_transition_tracks()
            death_music_active = true
            wave_music_active = false
        }
        return
    }

    if (death_music_active) {
        strudel_set_bpm(134.0lf)
        if (wave_banner_timer > 0.0) {
            set_wave_banner_melody_tracks()
            wave_music_active = true
        } else {
            set_play_melody_tracks()
            wave_music_active = false
        }
        death_music_active = false
        return
    }

    if (wave_banner_timer > 0.0) {
        if (!wave_music_active) {
            set_wave_banner_melody_tracks()
            wave_music_active = true
        }
    } elif (wave_music_active) {
        set_play_melody_tracks()
        wave_music_active = false
    }
}

// --- Helpers ---

var global_rng_seed = 42u

def random_f() : float {
    global_rng_seed = global_rng_seed * 1103515245u + 12345u
    return float(int(global_rng_seed >> 16u) % 1000) / 1000.0
}

def random_range(min, max : float) : float {
    return min + random_f() * (max - min)
}

def wrap_pos(pos : float3) : float3 {
    let x = (pos.x + FIELD_W) % FIELD_W
    let y = (pos.y + FIELD_H) % FIELD_H
    return float3(x, y, pos.z)
}

def wrap_delta(a, b, period : float) : float {
    var d = a - b
    let half = period * 0.5
    if (d > half) {
        d -= period
    } elif (d < -half) {
        d += period
    }
    return d
}

def wrapped_delta(a, b : float3) : float3 {
    return float3(
        wrap_delta(a.x, b.x, FIELD_W),
        wrap_delta(a.y, b.y, FIELD_H),
        a.z - b.z
    )
}

def wrapped_distance_sq(a, b : float3) : float {
    let d = wrapped_delta(a, b)
    return dot(d, d)
}

def segment_hits_wrapped_circle(p0, p1, center : float3; radius : float) : bool {
    let v0 = wrapped_delta(p0, center)
    let v1 = wrapped_delta(p1, center)
    let seg = v1 - v0
    let seg_len2 = dot(seg, seg)
    if (seg_len2 < 0.000001) {
        return dot(v0, v0) <= radius * radius
    }
    let t = clamp(-dot(v0, seg) / seg_len2, 0.0, 1.0)
    let closest = v0 + seg * t
    return dot(closest, closest) <= radius * radius
}

def ship_forward(rotation : float) : float3 {
    return float3(-sin(rotation), cos(rotation), 0.0)
}

def update_mouse_world() {
    var win_w, win_h : int
    glfwGetWindowSize(live_window, safe_addr(win_w), safe_addr(win_h))
    if (win_w <= 0 || win_h <= 0) {
        return
    }
    let mpos = glfwGetCursorPos(live_window)
    let mx = clamp(mpos.x / float(win_w), 0.0, 1.0)
    let my = clamp(mpos.y / float(win_h), 0.0, 1.0)
    mouse_world = float3(mx * FIELD_W, (1.0 - my) * FIELD_H, 0.0)
}

// --- GL Setup ---

var @in @location = 0 v_position : float3
var @in @location = 1 v_normal : float3
var @in @location = 2 v_texture : float2
var @uniform v_model : float4x4
var @uniform v_view : float4x4
var @uniform v_projection : float4x4
var @uniform f_Color : float4
var @out f_FragColor : float4

[vertex_program]
def vs_asteroids {
    gl_Position = v_projection * v_view * v_model * float4(v_position, 1.0)
}

[fragment_program]
def fs_asteroids {
    f_FragColor = f_Color
}

var program : uint
var ship_geo : OpenGLGeometryFragment
var asteroid_geo : OpenGLGeometryFragment
var bullet_geo : OpenGLGeometryFragment
var particle_geometry : OpenGLGeometryFragment
var hud_font : Font?
var display_w = 0
var display_h = 0
var active_program : uint

def create_gl_objects() {
    program = cache_shader_program(vs_asteroids`shader_text, fs_asteroids`shader_text)
    ship_geo <- create_geometry_fragment <| gen_ship_triangle()
    asteroid_geo <- create_geometry_fragment <| gen_sphere(12, 8, false)
    bullet_geo <- create_geometry_fragment <| gen_cube()
    particle_geometry <- create_geometry_fragment <| gen_cube()
    cache_ttf_objects()
    hud_font = cache_font("{get_das_root()}/modules/dasStbImage/fonts/droidsansmono.ttf")
}

def gen_ship_triangle() {
    var frag : GeometryFragment
    frag.vertices |> push(GeometryPreviewVertex(
        xyz = float3(0.0, 1.2, 0.0),
        normal = float3(0.0, 0.0, 1.0),
        uv = float2(0.5, 1.0)
    ))
    frag.vertices |> push(GeometryPreviewVertex(
        xyz = float3(-0.75, -0.8, 0.0),
        normal = float3(0.0, 0.0, 1.0),
        uv = float2(0.0, 0.0)
    ))
    frag.vertices |> push(GeometryPreviewVertex(
        xyz = float3(0.75, -0.8, 0.0),
        normal = float3(0.0, 0.0, 1.0),
        uv = float2(1.0, 0.0)
    ))
    frag.indices |> push(0)
    frag.indices |> push(1)
    frag.indices |> push(2)
    frag.prim = GeometryFragmentType.triangles
    gen_bbox(frag)
    return <- frag
}

def draw_text(text : string; x, y : int; tint : float3 = float3(1.0)) {
    if (hud_font == null) {
        return
    }
    var quads <- (*hud_font) |> create_quads(text)
    (*hud_font) |> draw_quads_2d(quads, display_w, display_h, x, y, tint)
    delete quads
}

def draw_text_centered(text : string; cx, y : int; tint : float3 = float3(1.0)) {
    if (hud_font == null) {
        return
    }
    var quads <- (*hud_font) |> create_quads(text)
    let dim = quads_dim(quads)
    let tw = int(dim.vmax.x - dim.vmin.x)
    let x = cx - tw / 2
    (*hud_font) |> draw_quads_2d(quads, display_w, display_h, x, y, tint)
    delete quads
}

def draw_hud() {
    draw_text("ASTEROIDS", 10, 24, float3(0.8, 0.9, 1.0))
    draw_text("SCORE: {score}", 10, 52, float3(1.0))
    draw_text("LIVES: {lives}", 10, 80, float3(1.0, 0.9, 0.6))
    draw_text("WAVE: {wave_number}", 10, 108, float3(0.7, 1.0, 0.7))
    draw_text("W/UP thrust, A/D rotate, RMB aim, LMB or SPACE shoot", 10, display_h - 30, float3(0.65, 0.65, 0.75))

    let cx = display_w / 2
    let cy = display_h / 2
    if (wave_banner_timer > 0.0 && game_state == GameState.playing) {
        draw_text_centered("WAVE {wave_number}", cx, cy - 8, float3(0.9, 1.0, 0.7))
    }
    if (rapid_fire_timer > 0.0 && game_state == GameState.playing) {
        draw_text("RAPID FIRE {int(rapid_fire_timer + 1.0)}s", 10, 136, float3(0.3, 1.0, 1.0))
    }
    if (invuln_powerup_timer > 0.0 && game_state == GameState.playing) {
        draw_text("INVULN {int(invuln_powerup_timer + 1.0)}s", 10, 164, float3(1.0, 0.95, 0.35))
    }
    if (game_state == GameState.game_over_state) {
        draw_text_centered("GAME OVER", cx, cy - 20, float3(1.0, 0.3, 0.3))
        draw_text_centered("PRESS SPACE TO RESTART", cx, cy + 25, float3(0.8, 0.8, 0.8))
    } elif (game_state == GameState.win_state) {
        draw_text_centered("YOU WIN", cx, cy - 20, float3(0.4, 1.0, 0.5))
        draw_text_centered("PRESS SPACE TO RESTART", cx, cy + 25, float3(0.8, 0.8, 0.8))
    }
}

// --- Gameplay ---

def spawn_asteroid(pos : float3; size, speed_mult : float = 1.0) {
    let vx_abs = random_range(ASTEROID_MIN_SPEED, ASTEROID_MAX_SPEED)
    let vy_abs = random_range(ASTEROID_MIN_SPEED, ASTEROID_MAX_SPEED)
    let vx = (random_f() < 0.5 ? -vx_abs : vx_abs)
    let vy = (random_f() < 0.5 ? -vy_abs : vy_abs)
    g_asteroids |> push(AsteroidObj(
        pos = pos,
        vel = float3(vx, vy, 0.0) * speed_mult,
        rotation = random_f() * 2.0 * PI,
        rotation_speed = random_range(-2.0, 2.0),
        size = size
    ))
}

def spawn_particles(origin, color : float3; count : int) {
    g_particles |> reserve(length(g_particles) + count)
    for (_i in range(count)) {
        let angle = random_f() * 2.0 * PI
        let speed = 50.0 + random_f() * 100.0
        g_particles |> push(ParticleObj(
            pos = origin,
            vel = float3(cos(angle) * speed, sin(angle) * speed, 0.0),
            color = color,
            lifetime = PARTICLE_LIFETIME,
            max_life = PARTICLE_LIFETIME
        ))
    }
}

def random_powerup_type() : PowerupType {
    let r = random_f()
    if (r < 0.42) {
        return PowerupType.rapid_fire
    } elif (r < 0.72) {
        return PowerupType.extra_life
    }
    return PowerupType.invulnerability
}


def powerup_color(kind : PowerupType) : float3 {
    if (kind == PowerupType.rapid_fire) {
        return float3(0.25, 1.0, 1.0)
    } elif (kind == PowerupType.extra_life) {
        return float3(0.45, 1.0, 0.4)
    }
    return float3(1.0, 0.95, 0.3)
}

def maybe_drop_powerup(pos : float3) {
    if (random_f() > POWERUP_DROP_CHANCE) {
        return
    }
    let kind = random_powerup_type()
    let angle = random_f() * 2.0 * PI
    let speed = 26.0 + random_f() * 20.0
    let spin = random_range(2.0, 5.0) * (random_f() < 0.5 ? -1.0 : 1.0)
    g_powerups |> push(PowerupObj(
        pos = pos,
        vel = float3(cos(angle) * speed, sin(angle) * speed, 0.0),
        kind = kind,
        lifetime = POWERUP_LIFETIME,
        rotation = 0.0,
        rotation_speed = spin
    ))
}

def apply_powerup(kind : PowerupType) {
    if (kind == PowerupType.rapid_fire) {
        rapid_fire_timer = max(rapid_fire_timer, POWERUP_RAPID_FIRE_TIME)
        spawn_particles(ship_pos, powerup_color(kind), 8)
    } elif (kind == PowerupType.extra_life) {
        lives += 1
        spawn_particles(ship_pos, powerup_color(kind), 10)
    } else {
        invuln_powerup_timer = max(invuln_powerup_timer, POWERUP_INVULN_TIME)
        ship_invuln_timer = max(ship_invuln_timer, invuln_powerup_timer)
        spawn_particles(ship_pos, powerup_color(kind), 10)
    }
    play_sfx(snd_wave_start)
}

def spawn_wave(wave : int) {
    let asteroid_count = min(8 + (wave - 1) * 2, 28)
    let speed_mult = 1.0 + float(wave - 1) * WAVE_SPEED_STEP
    let max_size = max(ASTEROID_MAX_SIZE - float(wave - 1) * 1.5, ASTEROID_MIN_SIZE + 6.0)
    for (_i in range(asteroid_count)) {
        var pos = float3(random_range(0.0, FIELD_W), random_range(0.0, FIELD_H), 0.0)
        var tries = 0
        while (tries < 8 && wrapped_distance_sq(pos, ship_pos) < 180.0 * 180.0) {
            pos = float3(random_range(0.0, FIELD_W), random_range(0.0, FIELD_H), 0.0)
            tries += 1
        }
        spawn_asteroid(pos, random_range(ASTEROID_MIN_SIZE, max_size), speed_mult)
    }
}

def reset_game() {
    score = 0
    lives = 3
    wave_number = 1
    game_state = GameState.playing
    last_shot_time = 0.0
    ship_invuln_timer = 0.0
    ship_pos = float3(FIELD_W * 0.5, FIELD_H * 0.5, 0.0)
    ship_vel = float3(0.0)
    ship_rotation = 0.0
    wave_banner_timer = WAVE_BANNER_DURATION
    ship_respawn_timer = 0.0
    rapid_fire_timer = 0.0
    invuln_powerup_timer = 0.0
    g_asteroids |> resize(0)
    g_bullets |> resize(0)
    g_particles |> resize(0)
    g_powerups |> resize(0)
    spawn_wave(wave_number)
    play_sfx(snd_wave_start)
}

def update_ship() {
    if (game_state != GameState.playing) {
        return
    }

    if (ship_respawn_timer > DEATH_BLINK_TIME) {
        return
    }

    let up = glfwGetKey(live_window, int(GLFW_KEY_W)) == int(GLFW_PRESS) || glfwGetKey(live_window, int(GLFW_KEY_UP)) == int(GLFW_PRESS)
    let down = glfwGetKey(live_window, int(GLFW_KEY_S)) == int(GLFW_PRESS) || glfwGetKey(live_window, int(GLFW_KEY_DOWN)) == int(GLFW_PRESS)
    let left = glfwGetKey(live_window, int(GLFW_KEY_A)) == int(GLFW_PRESS) || glfwGetKey(live_window, int(GLFW_KEY_LEFT)) == int(GLFW_PRESS)
    let right = glfwGetKey(live_window, int(GLFW_KEY_D)) == int(GLFW_PRESS) || glfwGetKey(live_window, int(GLFW_KEY_RIGHT)) == int(GLFW_PRESS)

    let mouse_aim_active = mouse_aim_enabled && glfwGetMouseButton(live_window, int(GLFW_MOUSE_BUTTON_2)) == int(GLFW_PRESS)
    if (mouse_aim_active) {
        let d = mouse_world - ship_pos
        if (length_sq(d) > 1.0) {
            ship_rotation = atan2(-d.x, d.y)
        }
    } else {
        if (left) {
            ship_rotation += SHIP_ROT_SPEED * tick_dt
        }
        if (right) {
            ship_rotation -= SHIP_ROT_SPEED * tick_dt
        }
    }

    if (up) {
        let force = SHIP_THRUST * tick_dt
        ship_vel += ship_forward(ship_rotation) * force
    }
    if (down) {
        let force = SHIP_BACK_THRUST * tick_dt
        ship_vel -= ship_forward(ship_rotation) * force
    }

    let fire_kb = glfwGetKey(live_window, int(GLFW_KEY_SPACE)) == int(GLFW_PRESS)
    let fire_mouse = mouse_fire_enabled && glfwGetMouseButton(live_window, int(GLFW_MOUSE_BUTTON_1)) == int(GLFW_PRESS)
    let fire_cooldown = (rapid_fire_timer > 0.0 ? FIRE_COOLDOWN * 0.5 : FIRE_COOLDOWN)
    let now = get_uptime()
    if (ship_respawn_timer <= 0.0 && (fire_kb || fire_mouse) && now - last_shot_time > fire_cooldown) {
        let dir = ship_forward(ship_rotation)
        let bullet_spin = random_range(BULLET_SPIN_MIN, BULLET_SPIN_MAX) * (random_f() < 0.5 ? -1.0 : 1.0)
        g_bullets |> push(BulletObj(
            pos = ship_pos + dir * (SHIP_SIZE * 1.1),
            prev_pos = ship_pos + dir * (SHIP_SIZE * 1.1),
            vel = dir * BULLET_SPEED + ship_vel * 0.2,
            rotation = 0.0,
            rotation_speed = bullet_spin,
            lifetime = 0.0
        ))
        spawn_particles(ship_pos + dir * (SHIP_SIZE * 1.2), float3(1.0, 0.8, 0.2), 2)
        play_sfx(snd_shoot)
        last_shot_time = now
    }

    ship_vel *= SHIP_FRICTION
    ship_pos += ship_vel * tick_dt
    ship_pos = wrap_pos(ship_pos)

    ship_invuln_timer = max(ship_invuln_timer - tick_dt, 0.0)
}

def update_asteroids() {
    for (i in range(length(g_asteroids))) {
        g_asteroids[i].pos += g_asteroids[i].vel * tick_dt
        g_asteroids[i].pos = wrap_pos(g_asteroids[i].pos)
        g_asteroids[i].rotation += g_asteroids[i].rotation_speed * tick_dt
    }
}

def update_bullets() {
    var i = 0
    while (i < length(g_bullets)) {
        g_bullets[i].lifetime += tick_dt
        if (g_bullets[i].lifetime > BULLET_LIFETIME) {
            g_bullets |> erase(i)
        } else {
            g_bullets[i].prev_pos = g_bullets[i].pos
            g_bullets[i].pos += g_bullets[i].vel * tick_dt
            g_bullets[i].pos = wrap_pos(g_bullets[i].pos)
            g_bullets[i].rotation += g_bullets[i].rotation_speed * tick_dt
            i += 1
        }
    }
}

def update_particles() {
    var i = 0
    while (i < length(g_particles)) {
        g_particles[i].lifetime -= tick_dt
        if (g_particles[i].lifetime <= 0.0) {
            g_particles |> erase(i)
        } else {
            g_particles[i].pos += g_particles[i].vel * tick_dt
            g_particles[i].pos = wrap_pos(g_particles[i].pos)
            i += 1
        }
    }
}

def update_powerups() {
    var i = 0
    while (i < length(g_powerups)) {
        g_powerups[i].lifetime -= tick_dt
        if (g_powerups[i].lifetime <= 0.0) {
            g_powerups |> erase(i)
        } else {
            g_powerups[i].pos += g_powerups[i].vel * tick_dt
            g_powerups[i].pos = wrap_pos(g_powerups[i].pos)
            g_powerups[i].rotation += g_powerups[i].rotation_speed * tick_dt
            i += 1
        }
    }
}

def process_powerup_pickups() {
    if (game_state != GameState.playing) {
        return
    }
    var i = 0
    while (i < length(g_powerups)) {
        if (wrapped_distance_sq(ship_pos, g_powerups[i].pos) <= POWERUP_PICKUP_RADIUS * POWERUP_PICKUP_RADIUS) {
            let kind = g_powerups[i].kind
            g_powerups |> erase(i)
            apply_powerup(kind)
        } else {
            i += 1
        }
    }
}

def process_collisions() {
    if (game_state != GameState.playing) {
        return
    }

    if (wave_banner_timer <= 0.0) {
        var bi = 0
        while (bi < length(g_bullets)) {
            var hit = false
            var ai_hit = -1
            for (ai in range(length(g_asteroids))) {
                let r = g_asteroids[ai].size + BULLET_SIZE
                let b = g_bullets[bi]
                let ok = wrapped_distance_sq(b.pos, g_asteroids[ai].pos) <= r * r || segment_hits_wrapped_circle(b.prev_pos, b.pos, g_asteroids[ai].pos, r)
                if (ok) {
                    hit = true
                    ai_hit = ai
                    break
                }
            }

            if (hit && ai_hit >= 0) {
                let hit_pos = g_asteroids[ai_hit].pos
                let hit_size = g_asteroids[ai_hit].size
                g_bullets |> erase(bi)
                g_asteroids |> erase(ai_hit)
                spawn_particles(hit_pos, float3(0.8, 0.8, 0.8), 10)
                play_sfx(snd_asteroid_hit)
                score += 100
                maybe_drop_powerup(hit_pos)
                if (hit_size > ASTEROID_MIN_SIZE) {
                    let new_size = hit_size * 0.5
                    spawn_asteroid(hit_pos, new_size, 1.0 + float(wave_number - 1) * WAVE_SPEED_STEP)
                    spawn_asteroid(hit_pos, new_size, 1.0 + float(wave_number - 1) * WAVE_SPEED_STEP)
                }
            } else {
                bi += 1
            }
        }
    }

    if (wave_banner_timer <= 0.0 && ship_invuln_timer <= 0.0 && ship_respawn_timer <= 0.0 && invuln_powerup_timer <= 0.0) {
        for (ai in range(length(g_asteroids))) {
            let ship_r = g_asteroids[ai].size + SHIP_SIZE * 0.7
            if (wrapped_distance_sq(ship_pos, g_asteroids[ai].pos) < ship_r * ship_r) {
                lives -= 1
                spawn_particles(ship_pos, float3(1.0, 0.4, 0.3), 16)
                ship_pos = float3(FIELD_W * 0.5, FIELD_H * 0.5, 0.0)
                ship_vel = float3(0.0)
                ship_respawn_timer = DEATH_HIDE_TIME + DEATH_BLINK_TIME
                ship_invuln_timer = ship_respawn_timer + SHIP_INVULN_TIME
                if (lives <= 0) {
                    play_sfx(snd_game_over)
                    game_state = GameState.game_over_state
                } else {
                    play_sfx(snd_ship_hit)
                }
                break
            }
        }
    }
}

// --- Main Loop ---

[export]
def init() {
    live_create_window("Asteroids", 1024, 768)
    create_gl_objects()
    if (!audio_initialized) {
        asch = audio_system_create()
        audio_initialized = true
        init_audio_samples()
    }
    if (!music_initialized) {
        strudel_init(@@strudel_music_main)
        strudel_set_volume(0.38, 0.0)
        music_initialized = true
        music_state = game_state
    }
    if (!is_reload()) {
        reset_game()
    }
}

[export]
def update() {
    if (!live_begin_frame()) {
        return
    }

    live_get_framebuffer_size(display_w, display_h)
    glViewport(0, 0, display_w, display_h)
    glClearColor(0.0, 0.0, 0.0, 1.0)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glEnable(GL_DEPTH_TEST)

    let aspect = (display_h != 0 ? float(display_w) / float(display_h) : 1.0)
    v_projection = perspective_rh_opengl(45.0 * PI / 180.0, aspect, 0.1, 1000.0)
    v_view = look_at_rh(float3(512, 384, 500), float3(512, 384, 0), float3(0, 1, 0))

    let dt = get_dt()
    tick_dt = min(dt, 1.0 / 30.0)
    wave_banner_timer = max(wave_banner_timer - tick_dt, 0.0)
    ship_respawn_timer = max(ship_respawn_timer - tick_dt, 0.0)
    rapid_fire_timer = max(rapid_fire_timer - tick_dt, 0.0)
    invuln_powerup_timer = max(invuln_powerup_timer - tick_dt, 0.0)
    update_mouse_world()

    if (game_state == GameState.game_over_state || game_state == GameState.win_state) {
        if (glfwGetKey(live_window, int(GLFW_KEY_SPACE)) == int(GLFW_PRESS)) {
            reset_game()
        }
    }

    update_ship()
    update_asteroids()
    update_bullets()
    update_powerups()
    update_particles()
    process_collisions()
    process_powerup_pickups()
    update_music_state()
    update_overlay_music()

    if (game_state == GameState.playing && length(g_asteroids) == 0) {
        if (wave_number >= MAX_WAVES) {
            game_state = GameState.win_state
            wave_banner_timer = 0.0
        } else {
            wave_number += 1
            lives += 1
            wave_banner_timer = WAVE_BANNER_DURATION
            g_bullets |> resize(0)
            ship_invuln_timer = max(ship_invuln_timer, 1.0)
            spawn_wave(wave_number)
            play_sfx(snd_wave_start)
        }
    }

    glUseProgram(program)
    active_program = program

    if (wireframe_mode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)
        glLineWidth(wireframe_line_width)
    }

    let ship_dir = ship_forward(ship_rotation)
    let ship_is_transitioning = game_state == GameState.playing && ship_respawn_timer > 0.0
    let ship_is_hidden = ship_respawn_timer > DEATH_BLINK_TIME
    let ship_is_blinking = ship_respawn_timer > 0.0 && ship_respawn_timer <= DEATH_BLINK_TIME
    let blink_visible = !ship_is_blinking || int(get_uptime() * 18.0) % 2 == 0
    if (!ship_is_transitioning || (!ship_is_hidden && blink_visible)) {
        let ship_rot = float4(0.0, 0.0, sin(ship_rotation * 0.5), cos(ship_rotation * 0.5))
        v_model = compose(ship_pos, ship_rot, float3(SHIP_SIZE, SHIP_SIZE, SHIP_SIZE))
        f_Color = float4(1, 1, 1, 1)
        vs_asteroids_bind_uniform(active_program)
        fs_asteroids_bind_uniform(active_program)
        ship_geo |> draw_geometry_fragment()

        v_model = compose(ship_pos + ship_dir * (SHIP_SIZE * 1.1), float4(0), float3(3.0, 3.0, 3.0))
        f_Color = float4(1.0, 0.2, 0.2, 1.0)
        vs_asteroids_bind_uniform(active_program)
        fs_asteroids_bind_uniform(active_program)
        bullet_geo |> draw_geometry_fragment()
    }

    let asteroids_blink = game_state == GameState.playing && wave_banner_timer > 0.0
    let asteroids_visible = !asteroids_blink || int(get_uptime() * 10.0) % 2 == 0
    if (asteroids_visible) {
        for (i in range(length(g_asteroids))) {
            let a = g_asteroids[i]
            v_model = compose(a.pos, float4(0), float3(a.size, a.size, a.size))
            f_Color = float4(0.7, 0.7, 0.7, 1.0)
            vs_asteroids_bind_uniform(active_program)
            fs_asteroids_bind_uniform(active_program)
            asteroid_geo |> draw_geometry_fragment()
        }
    }

    for (i in range(length(g_powerups))) {
        let p = g_powerups[i]
        let pulse = 0.85 + 0.25 * abs(sin(get_uptime() * 8.0 + p.rotation * 0.7))
        let c = powerup_color(p.kind)
        let rot = float4(0.0, 0.0, sin(p.rotation * 0.5), cos(p.rotation * 0.5))
        v_model = compose(p.pos, rot, float3(5.0 * pulse, 5.0 * pulse, 5.0 * pulse))
        f_Color = float4(c.x, c.y, c.z, 0.95)
        vs_asteroids_bind_uniform(active_program)
        fs_asteroids_bind_uniform(active_program)
        bullet_geo |> draw_geometry_fragment()
    }

    for (i in range(length(g_bullets))) {
        let b = g_bullets[i]
        let bullet_rot = float4(0.0, 0.0, sin(b.rotation * 0.5), cos(b.rotation * 0.5))
        v_model = compose(b.pos, bullet_rot, float3(BULLET_SIZE, BULLET_SIZE, BULLET_SIZE))
        f_Color = float4(0.2, 1.0, 1.0, 1.0)
        vs_asteroids_bind_uniform(active_program)
        fs_asteroids_bind_uniform(active_program)
        bullet_geo |> draw_geometry_fragment()
    }

    for (i in range(length(g_particles))) {
        let p = g_particles[i]
        v_model = compose(p.pos, float4(0), float3(1.5, 1.5, 1.5))
        f_Color = float4(p.color, p.lifetime / p.max_life)
        vs_asteroids_bind_uniform(active_program)
        fs_asteroids_bind_uniform(active_program)
        particle_geometry |> draw_geometry_fragment()
    }

    if (show_crosshair && mouse_aim_enabled) {
        v_model = compose(mouse_world, float4(0), float3(3.5, 3.5, 3.5))
        f_Color = float4(0.3, 1.0, 0.3, 0.8)
        vs_asteroids_bind_uniform(active_program)
        fs_asteroids_bind_uniform(active_program)
        particle_geometry |> draw_geometry_fragment()
    }

    if (wireframe_mode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)
    }

    draw_hud()
    live_end_frame()
}

[export]
def shutdown() {
    if (music_initialized) {
        strudel_shutdown()
        music_initialized = false
    }
    if (audio_initialized) {
        audio_system_finalize(asch.command, asch.next_sid)
        audio_initialized = false
    }
    live_destroy_window()
}

var max_frame_limit = 0

def parse_args() {
    let args <- get_command_line_arguments()
    for (i in range(length(args) - 1)) {
        if (args[i] == "--max-frames") {
            max_frame_limit = to_int(args[i + 1])
        }
    }
}

[export]
def main() {
    parse_args()
    init()
    var frame_count = 0
    while (!exit_requested()) {
        update()
        frame_count += 1
        if (max_frame_limit > 0 && frame_count >= max_frame_limit) {
            break
        }
    }
    shutdown()
}
