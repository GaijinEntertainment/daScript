options gen2
options persistent_heap

// river run -- web playground port (live/* + live_host replaced by live_stub)

// Playground shim: replaces the whole live/* + live_host stack (window, clock,
// no-op [live_command]). Static requires cannot be switched at runtime, which
// is the reason this port exists as a copy at all.
require live_stub public
require glfw/glfw_boost public
require opengl/opengl_boost public
require opengl/opengl_gen public
require opengl/opengl_cache public
require rr_shaders public
require rr_models public
require rr_postfx public
require audio/audio_boost public
require strudel/strudel public
require strudel/strudel_player public
require daslib/math_boost public
require daslib/json public
require daslib/json_boost public
require daslib/strings_boost public
require daslib/safe_addr public
require opengl/opengl_ttf public
require daslib/decs_boost public

// --- Constants ---

let RIVER_HALF_WIDTH_MAX = 7.0
let RIVER_HALF_WIDTH_MIN = 3.0
let RIVER_CENTER_DRIFT_MAX = 3.5
let SEGMENT_LENGTH = 6.0
let RIVER_SEGMENTS_COUNT = 52
let SECTION_LENGTH = 240.0
let MAX_SECTIONS = 10

let PLAYER_Z = 2.5
let BOAT_Z = 0.3
let PLANE_Z = 2.5
let ENEMY_HELI_Z = 2.5
let BRIDGE_Z = 0.0
let ISLAND_Z = 0.0
let FUEL_DEPOT_Z = 0.2

let PLAYER_STRAFE_SPEED = 8.0
let PLAYER_FWD_SPEED_MIN = 4.0
let PLAYER_FWD_SPEED_MAX = 14.0
let PLAYER_FRICTION = 0.88
let PLAYER_FUEL_MAX = 100.0
let PLAYER_FUEL_DRAIN = 1.8
let PLAYER_INVULN_TIME = 2.5
let PLAYER_RESPAWN_HIDE = 1.5
let PLAYER_RESPAWN_BLINK = 1.0
let PLAYER_SIZE = 0.55
let PLAYER_ROTOR_SIZE = 1.0

let BULLET_SPEED = 38.0
let BULLET_LIFETIME = 1.2
let BULLET_SIZE = 0.12
let FIRE_COOLDOWN = 0.312

let BONUS_DROP_CHANCE = 0.33
let BONUS_PICKUP_RADIUS = 0.9
let BONUS_FUEL_AMOUNT = 30.0
let BONUS_MULTISHOT_TIME = 12.0
let BONUS_FASTSHOT_TIME = 7.0
let BONUS_BOB_AMPLITUDE = 0.22
let BONUS_BOB_SPEED = 2.6
let BONUS_SPIN_SPEED = 2.4

let ENEMY_BULLET_SPEED = 10.0
let ENEMY_HELI_BULLET_SPEED = 18.0
let ENEMY_BULLET_LIFETIME = 2.2
let ENEMY_BULLET_SIZE = 0.14
let ENEMY_HELI_RANGE = 28.0
let ENEMY_HELI_SHOOT_INTERVAL_FAST = 1.2

let BOAT_SIZE = 0.7
let PLANE_SIZE = 0.6
let ENEMY_HELI_SIZE = 0.55

let BOAT_PATROL_SPEED = 2.5
let BOAT_SHOOT_INTERVAL = 2.8
let PLANE_SPEED = 6.0
let PLANE_SHOOT_INTERVAL = 1.8
let ENEMY_HELI_SPEED = 3.5
let ENEMY_HELI_SHOOT_INTERVAL = 2.2

let FUEL_DEPOT_SIZE = 1.2
let REFUEL_RATE = 35.0
let REFUEL_RADIUS = 1.5
let LOW_FUEL_THRESHOLD = 25.0

let BRIDGE_SPAN_Z = 3.2
let BRIDGE_GAP_WIDTH = 2.5
let BRIDGE_HEALTH_MIN = 2
let BRIDGE_HEALTH_MAX = 4
let BRIDGE_HEALTH_STEP_SECTIONS = 3
let ISLAND_SIZE_MIN = 1.0
let ISLAND_SIZE_MAX = 2.5

let PARTICLE_LIFETIME = 0.7
let TRAIL_LIFETIME = 0.12

let CAM_BACK = 14.0
let CAM_LOOK_AHEAD = 11.0
let CAM_X_LAG = 0.08
// Pitch is the camera's angle below horizontal. It has to stay under half the
// vertical FOV or the horizon never enters the frame -- which is why the old
// 42-degree framing showed nothing but ground.
let CAM_PITCH_SLOW_DEG = 19.0   // pitch at PLAYER_FWD_SPEED_MIN
let CAM_PITCH_FAST_DEG = 15.0   // pitch at top speed (PLAYER_FWD_SPEED_MAX * section_speed_mult)

let SHAKE_SMALL = 0.20
let SHAKE_MED   = 0.55
let SHAKE_BIG   = 1.05
let SHAKE_DECAY = 5.0

let SLOW_MO_DURATION = 0.6
let SLOW_MO_SCALE = 0.4

let WAVE_BANNER_DURATION = 2.5

let AUDIO_RATE = 48000
let AUDIO_CHANNELS = 1

// --- Enums ---

enum GameState { menu, playing, paused, game_over_state, win_state }

enum BonusType {
    fuel
    multishot
    life
    fastshot
}

enum EnemyKind {
    boat
    plane
    helicopter
    bridge
    depot
}

// --- DECS Templates ---

[decs_template]
struct EnemyBoat {
    pos : float3
    vel : float3
    health : int
    shoot_timer : float
    patrol_dir : float
    lane_prefer_x : float
}

[decs_template]
struct EnemyPlane {
    pos : float3
    vel : float3
    health : int
    shoot_timer : float
}

[decs_template]
struct EnemyHelicopter {
    pos : float3
    vel : float3
    health : int
    shoot_timer : float
    hover_phase : float
    target_x : float
}

[decs_template]
struct PlayerBullet {
    pos : float3
    vel : float3
    age : float
}

[decs_template]
struct EnemyBullet {
    pos : float3
    vel : float3
    age : float
    bullet_type : int  // 0=boat(sphere), 1=heli(red cyl), 2=plane(orange cyl)
}

[decs_template]
struct FuelDepot {
    pos : float3
}

[decs_template]
struct Bridge {
    pos : float3
    gap_center_x : float
    left_x : float
    right_x : float
    health : int
}

[decs_template]
struct Island {
    pos : float3
    size : float
}

// One template covers everything growing on the bank -- conifers, broadleaf,
// dead trees, bushes, boulders and reeds -- because they differ only in which
// mesh they draw and how they are tinted. `kind` selects the mesh; `seed`
// selects which pre-generated instance of that mesh, so no two neighbours are
// the same object.
[decs_template]
struct RiverTree {
    pos : float3
    size : float
    tiers : int
    green_tint : float
    green_shift : float
    trunk_ratio : float
    kind : int
    seed : int
    yaw : float
}

[decs_template]
struct RiverHouse {
    pos : float3
    size : float
    body_tint : float
    roof_tint : float
}

[decs_template]
struct BonusPickup {
    pos : float3
    bonus_type : BonusType
    bob_phase : float
}

// Explosions are layered, and each layer moves and draws differently, so the
// particle carries its kind rather than having the renderer infer one from a
// lifetime threshold.
enum ParticleKind {
    trail       // bullet tracer dots
    debris      // spinning solid chunks, gravity
    ember       // small bright sparks, gravity, additive
    smoke       // dark puff, rises and expands, outlives everything else
    fire        // expanding fireball, cools white -> orange -> red
    flash       // one very short white blowout at the instant of the hit
    shock       // flat ring racing outward across the water
}

[decs_template]
struct Particle {
    pos : float3
    vel : float3
    color : float3
    lifetime : float
    max_life : float
    size : float
    spin_speed : float
    spin_phase : float
    kind : ParticleKind
    drag : float
    gravity : float
}

// --- River Segment ---

struct RiverSegment {
    left_bank_x : float
    right_bank_x : float
    split : bool
    split_left_x : float
    split_right_x : float
    world_y : float
}

// --- Hit collection ---

struct EnemyHitInfo {
    bullet_eid : EntityId
    enemy_eid : EntityId
    pos : float3
    enemy_kind : EnemyKind
    score_value : int
    blast_radius : float
}

// --- Global Game State ---

var @live game_state = GameState.menu
var @live score = 0
var @live player_lives = 3
var @live player_pos = float3(0.0, 0.0, PLAYER_Z)
var @live player_vel = float3(0.0, 0.0, 0.0)
var @live player_fwd_speed = PLAYER_FWD_SPEED_MIN
var @live player_fuel = PLAYER_FUEL_MAX
var @live player_invuln_timer = 0.0
var @live player_respawn_timer = 0.0
var @live player_shoot_cooldown = 0.0
var @live player_multishot_timer = 0.0
var @live player_fastshot_timer = 0.0
var @live cam_x = 0.0

var @live current_section = 0
var @live section_dist = 0.0
var @live section_banner_timer = 0.0
var @live section_score_bonus_pending = false
var @live restart_input_lock = 0.0
var @live god_mode = false
var @live screen_shake_amount = 0.0
var @live slow_mo_timer = 0.0

var @live river_segments : array<RiverSegment>
var @live river_dirty = true
var @live river_gen_y = 0.0
var @live river_center_x = 0.0
var @live river_half_width = 6.0
var @live river_center_target = 0.0
var @live river_width_target = 6.0
var @live river_split_target = false
var @live river_split_blend = 0.0
var @live river_split_half = 0.0
var @live river_split_hold = 0.0
var @live river_first_split_pending = true
var @live river_first_split_start_y = 0.0

var tick_dt = 0.0
// Effect-inspection rig: with fx_freeze set, the world and its particles both
// stop, so a blast can be held still and stepped by hand (cmd_fx_freeze /
// cmd_fx_step). A screenshot round-trip is far longer than a 100ms flash, so
// there is no other way to actually look at one.
var fx_freeze = false
var global_rng_seed = 12345u
var display_w = 0
var display_h = 0

var @live low_fuel_beep_timer = 0.0
var @live refuel_sound_timer = 0.0

// --- Section environments ---
//
// Each section is a whole lighting setup, not just a river tint: sun colour and
// elevation, sky and horizon, fog, exposure and grade. The look of the game
// therefore changes with progress without a single authored texture, and the
// post chain reads the same values, so bloom and shafts stay in key with the
// biome.

struct SectionEnv {
    sun_dir : float3
    sun_color : float3
    sky_color : float3
    horizon_color : float3
    bounce_color : float3
    fog_color : float3
    fog_density : float
    river_color : float3
    bank_color : float3
    shore_color : float3
    rock_color : float3
    foliage_tint : float3
    night : float
    exposure : float
    bloom : float
    shafts : float
    grade_tint : float3
    grade_amount : float
    name : string
}

def private env_dawn() : SectionEnv {
    return <- SectionEnv(
        sun_dir = normalize(float3(0.26, 0.95, 0.125)),
        sun_color = float3(1.45, 1.16, 0.82),
        sky_color = float3(0.22, 0.40, 0.78),
        horizon_color = float3(0.92, 0.68, 0.46),
        bounce_color = float3(0.10, 0.17, 0.26),
        fog_color = float3(0.78, 0.72, 0.70),
        fog_density = 0.0062,
        river_color = float3(0.06, 0.24, 0.52),
        bank_color = float3(0.26, 0.44, 0.20),
        shore_color = float3(0.62, 0.55, 0.42),
        rock_color = float3(0.34, 0.33, 0.32),
        foliage_tint = float3(1.0, 0.97, 0.86),
        night = 0.0,
        exposure = 1.05,
        bloom = 0.40,
        shafts = 0.80,
        grade_tint = float3(1.06, 0.98, 0.92),
        grade_amount = 0.10,
        name = "DELTA DAWN"
    )
}

def private env_desert() : SectionEnv {
    return <- SectionEnv(
        sun_dir = normalize(float3(0.20, 0.93, 0.30)),
        sun_color = float3(1.72, 1.52, 1.18),
        sky_color = float3(0.30, 0.50, 0.85),
        horizon_color = float3(0.95, 0.83, 0.60),
        bounce_color = float3(0.30, 0.24, 0.15),
        fog_color = float3(0.88, 0.80, 0.62),
        fog_density = 0.0080,
        river_color = float3(0.10, 0.30, 0.44),
        bank_color = float3(0.62, 0.50, 0.26),
        shore_color = float3(0.80, 0.70, 0.46),
        rock_color = float3(0.52, 0.42, 0.30),
        foliage_tint = float3(1.0, 0.94, 0.72),
        night = 0.0,
        exposure = 0.92,
        bloom = 0.34,
        shafts = 0.55,
        grade_tint = float3(1.10, 1.02, 0.84),
        grade_amount = 0.16,
        name = "SUN FLATS"
    )
}

def private env_forest() : SectionEnv {
    return <- SectionEnv(
        sun_dir = normalize(float3(-0.30, 0.92, 0.25)),
        sun_color = float3(1.10, 1.16, 0.96),
        sky_color = float3(0.42, 0.52, 0.58),
        horizon_color = float3(0.66, 0.72, 0.64),
        bounce_color = float3(0.12, 0.20, 0.13),
        fog_color = float3(0.62, 0.70, 0.63),
        fog_density = 0.0105,
        river_color = float3(0.05, 0.22, 0.26),
        bank_color = float3(0.16, 0.36, 0.14),
        shore_color = float3(0.44, 0.42, 0.34),
        rock_color = float3(0.28, 0.30, 0.28),
        foliage_tint = float3(0.90, 1.0, 0.88),
        night = 0.0,
        exposure = 1.18,
        bloom = 0.30,
        shafts = 1.10,
        grade_tint = float3(0.92, 1.02, 0.94),
        grade_amount = 0.18,
        name = "GREEN NARROWS"
    )
}

def private env_night() : SectionEnv {
    return <- SectionEnv(
        sun_dir = normalize(float3(-0.26, 0.94, 0.21)),
        sun_color = float3(0.52, 0.64, 1.00),
        sky_color = float3(0.026, 0.040, 0.090),
        horizon_color = float3(0.12, 0.16, 0.30),
        bounce_color = float3(0.055, 0.075, 0.130),
        fog_color = float3(0.10, 0.13, 0.23),
        fog_density = 0.0095,
        river_color = float3(0.020, 0.055, 0.135),
        // Moonlit ground still has to read as ground: at the first pass's values
        // the banks went black and the player could not see the channel edge.
        bank_color = float3(0.19, 0.24, 0.28),
        shore_color = float3(0.30, 0.33, 0.38),
        rock_color = float3(0.24, 0.26, 0.30),
        foliage_tint = float3(0.60, 0.72, 0.92),
        night = 1.0,
        exposure = 1.40,
        bloom = 0.72,
        shafts = 0.35,
        grade_tint = float3(0.80, 0.90, 1.18),
        grade_amount = 0.26,
        name = "MIDNIGHT RUN"
    )
}

def private env_volcanic() : SectionEnv {
    return <- SectionEnv(
        sun_dir = normalize(float3(0.32, 0.94, 0.115)),
        sun_color = float3(1.95, 0.92, 0.44),
        sky_color = float3(0.16, 0.09, 0.13),
        horizon_color = float3(0.88, 0.34, 0.14),
        bounce_color = float3(0.26, 0.09, 0.04),
        fog_color = float3(0.55, 0.28, 0.19),
        fog_density = 0.0120,
        river_color = float3(0.16, 0.13, 0.16),
        bank_color = float3(0.30, 0.15, 0.11),
        shore_color = float3(0.24, 0.17, 0.15),
        rock_color = float3(0.18, 0.14, 0.14),
        foliage_tint = float3(1.0, 0.78, 0.62),
        night = 0.35,
        exposure = 1.10,
        bloom = 0.66,
        shafts = 1.35,
        grade_tint = float3(1.18, 0.88, 0.76),
        grade_amount = 0.28,
        name = "ASH CHANNEL"
    )
}

def section_env(idx : int) : SectionEnv {
    let biome = clamp(idx, 0, MAX_SECTIONS - 1) % 5
    if (biome == 0) {
        return <- env_dawn()
    } elif (biome == 1) {
        return <- env_desert()
    } elif (biome == 2) {
        return <- env_forest()
    } elif (biome == 3) {
        return <- env_night()
    }
    return <- env_volcanic()
}

def lerp_env(a, b : SectionEnv; t : float) : SectionEnv {
    return <- SectionEnv(
        sun_dir = normalize(lerp(a.sun_dir, b.sun_dir, t)),
        sun_color = lerp(a.sun_color, b.sun_color, t),
        sky_color = lerp(a.sky_color, b.sky_color, t),
        horizon_color = lerp(a.horizon_color, b.horizon_color, t),
        bounce_color = lerp(a.bounce_color, b.bounce_color, t),
        fog_color = lerp(a.fog_color, b.fog_color, t),
        fog_density = lerp(a.fog_density, b.fog_density, t),
        river_color = lerp(a.river_color, b.river_color, t),
        bank_color = lerp(a.bank_color, b.bank_color, t),
        shore_color = lerp(a.shore_color, b.shore_color, t),
        rock_color = lerp(a.rock_color, b.rock_color, t),
        foliage_tint = lerp(a.foliage_tint, b.foliage_tint, t),
        night = lerp(a.night, b.night, t),
        exposure = lerp(a.exposure, b.exposure, t),
        bloom = lerp(a.bloom, b.bloom, t),
        shafts = lerp(a.shafts, b.shafts, t),
        grade_tint = lerp(a.grade_tint, b.grade_tint, t),
        grade_amount = lerp(a.grade_amount, b.grade_amount, t),
        name = (t < 0.5 ? a.name : b.name)
    )
}

var @live section_color_blend_t = 1.0
var env_from : SectionEnv
var env_to : SectionEnv
var env_now : SectionEnv

def refresh_env() {
    env_now <- lerp_env(env_from, env_to, saturate(section_color_blend_t))
}

def set_env_immediate(idx : int) {
    env_from <- section_env(idx)
    env_to <- section_env(idx)
    section_color_blend_t = 1.0
    refresh_env()
}

def begin_env_transition(idx : int) {
    env_from <- env_now
    env_to <- section_env(idx)
    section_color_blend_t = 0.0
}

// --- GL Object Handles ---

var geo_sphere : OpenGLGeometryFragment
var geo_cube : OpenGLGeometryFragment
var geo_cylinder : OpenGLGeometryFragment
var geo_cone : OpenGLGeometryFragment
var geo_prism : OpenGLGeometryFragment
var geo_disc : OpenGLGeometryFragment
var geo_heli : OpenGLGeometryFragment
var geo_heli_tail : OpenGLGeometryFragment
var geo_gunboat : OpenGLGeometryFragment
var geo_jet : OpenGLGeometryFragment
var geo_shards : array<OpenGLGeometryFragment>
var geo_flora : array<OpenGLGeometryFragment>
var hud_font : Font?
var river_bank_geo : OpenGLGeometryFragment
var river_surface_geo : OpenGLGeometryFragment

// --- Helpers ---

def random_f() : float {
    global_rng_seed = global_rng_seed * 1103515245u + 12345u
    return float(int(global_rng_seed >> 16u) % 1000) / 1000.0
}

def random_range(lo, hi : float) : float {
    return lo + random_f() * (hi - lo)
}

def random_sign() : float {
    return (random_f() < 0.5 ? -1.0 : 1.0)
}

def section_idx() : int {
    return min(current_section, MAX_SECTIONS - 1)
}

def get_river_color() : float3 {
    return env_now.river_color
}

def get_bank_color() : float3 {
    return env_now.bank_color
}

def get_fog_color() : float3 {
    return env_now.fog_color
}

def section_speed_mult() : float {
    return 1.0 + float(current_section) * 0.15
}

def add_screen_shake(amount : float) {
    screen_shake_amount = max(screen_shake_amount, amount)
}

def fuel_bar_color() : float3 {
    let low = player_fuel < LOW_FUEL_THRESHOLD
    let blink = low && int(get_uptime() * 6.0) % 2 == 0
    return (
        low ? (blink ? float3(1.0, 0.2, 0.2) : float3(1.0, 0.5, 0.1)) : float3(0.25, 0.95, 0.42)
    )
}

// Screen-space scale so the HUD keeps its designed proportions on a HiDPI
// framebuffer. display_w/display_h are framebuffer pixels, not window points --
// on a retina display that is 2x, which is why the old fixed-pixel HUD came out
// half size.
def hud_scale() : float {
    return max(float(display_h) / 720.0, 0.5)
}
