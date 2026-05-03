options gen2
options persistent_heap

require glfw/glfw_boost public
require live/glfw_live public
require opengl/opengl_boost public
require opengl/opengl_gen public
require opengl/opengl_cache public
require live/opengl_live public
require live/live_commands public
require live/live_api public
require live/live_watch_boost public
require live/live_vars public
require live/audio_live public
require audio/audio_boost public
require strudel/strudel public
require strudel/strudel_player public
require strudel/strudel_live public
require daslib/math_boost public
require daslib/json public
require daslib/json_boost public
require daslib/strings_boost public
require daslib/safe_addr public
require opengl/opengl_ttf public
require live_host public
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
let FIRE_COOLDOWN = 0.18

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
let CAM_HEIGHT = 19.0
let CAM_LOOK_AHEAD = 9.0
let CAM_X_LAG = 0.08

let WAVE_BANNER_DURATION = 2.5

let AUDIO_RATE = 48000
let AUDIO_CHANNELS = 1

// --- Enums ---

enum GameState { menu, playing, paused, game_over_state, win_state }

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
var @live cam_x = 0.0

var @live current_section = 0
var @live section_dist = 0.0
var @live section_banner_timer = 0.0
var @live section_score_bonus_pending = false

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
var global_rng_seed = 12345u
var display_w = 0
var display_h = 0

var @live low_fuel_beep_timer = 0.0
var @live refuel_sound_timer = 0.0

var @live section_color_blend_t = 1.0
var @live river_color_from = float3(0.1, 0.4, 0.7)
var @live river_color_to   = float3(0.1, 0.4, 0.7)
var @live bank_color_from  = float3(0.25, 0.55, 0.2)
var @live bank_color_to    = float3(0.25, 0.55, 0.2)

var section_colors = fixed_array(
    float3(0.1, 0.4, 0.7),
    float3(0.7, 0.55, 0.2),
    float3(0.1, 0.35, 0.1),
    float3(0.2, 0.2, 0.3),
    float3(0.6, 0.15, 0.05),
    float3(0.1, 0.4, 0.7),
    float3(0.7, 0.55, 0.2),
    float3(0.1, 0.35, 0.1),
    float3(0.2, 0.2, 0.3),
    float3(0.6, 0.15, 0.05)
)

var bank_colors = fixed_array(
    float3(0.25, 0.55, 0.2),
    float3(0.65, 0.5, 0.15),
    float3(0.15, 0.4, 0.1),
    float3(0.3, 0.3, 0.4),
    float3(0.4, 0.1, 0.05),
    float3(0.25, 0.55, 0.2),
    float3(0.65, 0.5, 0.15),
    float3(0.15, 0.4, 0.1),
    float3(0.3, 0.3, 0.4),
    float3(0.4, 0.1, 0.05)
)

var fog_colors = fixed_array(
    float3(0.52, 0.68, 0.88),  // blue water: pale blue haze
    float3(0.82, 0.72, 0.48),  // desert: warm sandy haze
    float3(0.48, 0.68, 0.48),  // forest: pale green mist
    float3(0.16, 0.20, 0.36),  // night: dark blue mist
    float3(0.68, 0.40, 0.24),  // volcanic: reddish haze
    float3(0.52, 0.68, 0.88),
    float3(0.82, 0.72, 0.48),
    float3(0.48, 0.68, 0.48),
    float3(0.16, 0.20, 0.36),
    float3(0.68, 0.40, 0.24)
)

var @live fog_color_from = float3(0.52, 0.68, 0.88)
var @live fog_color_to   = float3(0.52, 0.68, 0.88)

// --- Shader Interface ---

var @in @location = 0 v_position : float3
var @in @location = 1 v_normal : float3
var @in @location = 2 v_texture : float2
var @uniform v_model : float4x4
var @uniform v_view : float4x4
var @uniform v_projection : float4x4
var @inout f_normal : float3
var @inout f_tex_pos : float3
var @inout f_fog_t : float
var @uniform f_Color : float4
var @uniform f_RiverColor : float4
var @uniform f_GameTime : float
var @uniform f_FogColor : float3
var @uniform f_FogParams : float2
var @out f_FragColor : float4

// flat shader — bullets, particles, terrain quads

[vertex_program]
def vs_flat {
    let world_pos = v_model * float4(v_position, 1.0)
    f_fog_t = world_pos.y
    gl_Position = v_projection * v_view * world_pos
}

[fragment_program]
def fs_flat {
    let fog_t = saturate((f_fog_t - f_FogParams.x) / (f_FogParams.y - f_FogParams.x))
    let fog_alpha = fog_t * fog_t * 0.5
    let col = f_Color.xyz * (1.0 - fog_alpha) + f_FogColor * fog_alpha
    f_FragColor = float4(col, f_Color.w)
}

// phong shader — player, enemies, bridges, islands

let LIGHT_DIR = normalize(float3(0.4, -0.6, -1.0))

[vertex_program]
def vs_phong {
    let world_pos = v_model * float4(v_position, 1.0)
    f_normal = normalize(float3x3(v_model) * v_normal)
    f_fog_t = world_pos.y
    gl_Position = v_projection * v_view * world_pos
}

[fragment_program]
def fs_phong {
    let n = normalize(f_normal)
    let k = saturate(-dot(LIGHT_DIR, n))
    let lit = f_Color.xyz * (k * 0.75 + 0.25)
    let fog_t = saturate((f_fog_t - f_FogParams.x) / (f_FogParams.y - f_FogParams.x))
    let fog_alpha = fog_t * fog_t * 0.5
    let col = lit * (1.0 - fog_alpha) + f_FogColor * fog_alpha
    f_FragColor = float4(col, f_Color.w)
}

// river surface shader — animated water

[vertex_program]
def vs_river {
    let world_pos = v_model * float4(v_position, 1.0)
    f_tex_pos = world_pos.xyz
    f_fog_t = world_pos.y
    gl_Position = v_projection * v_view * world_pos
}

[fragment_program]
def fs_river {
    let shimmer = 0.07 * sin(f_GameTime * 2.3 + f_tex_pos.x * 0.8 + f_tex_pos.y * 0.5)
    let c = f_RiverColor.xyz + float3(shimmer)
    let fog_t = saturate((f_fog_t - f_FogParams.x) / (f_FogParams.y - f_FogParams.x))
    let fog_alpha = fog_t * fog_t * 0.5
    let col = c * (1.0 - fog_alpha) + f_FogColor * fog_alpha
    f_FragColor = float4(col, 1.0)
}

// rotor shader — alpha-blended propeller cross

[vertex_program]
def vs_rotor {
    let world_pos = v_model * float4(v_position, 1.0)
    f_tex_pos = float3(v_texture.x, 0.0, v_texture.y)
    f_fog_t = world_pos.y
    gl_Position = v_projection * v_view * world_pos
}

[fragment_program]
def fs_rotor {
    let u = f_tex_pos.x
    let v = f_tex_pos.z
    let d1 = abs(u - v)
    let d2 = abs(u + v - 1.0)
    let min_dist = min(d1, d2)
    let t = saturate(1.0 - min_dist * 3.5)
    let alpha = f_Color.w * (t * t * t * t)
    let fog_t = saturate((f_fog_t - f_FogParams.x) / (f_FogParams.y - f_FogParams.x))
    let fog_alpha = fog_t * fog_t * 0.5
    let col = f_Color.xyz * (1.0 - fog_alpha) + f_FogColor * fog_alpha
    f_FragColor = float4(col, alpha)
}

// --- GL Object Handles ---

var flat_program : uint
var phong_program : uint
var river_program : uint
var rotor_program : uint
var active_program : uint

var geo_sphere : OpenGLGeometryFragment
var geo_cube : OpenGLGeometryFragment
var geo_plane_xz : OpenGLGeometryFragment
var geo_cylinder : OpenGLGeometryFragment
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
    return min(current_section, 9)
}

def get_river_color() : float3 {
    // River transitions are intentionally slower than shore transitions.
    let t = saturate(section_color_blend_t * 0.42)
    let c = river_color_from * (1.0 - t) + river_color_to * t
    // Keep a persistent blue bias regardless of section palette.
    let blue_base = float3(0.08, 0.36, 0.74)
    let blended = c * 0.78 + blue_base * 0.22
    return float3(blended.x * 0.92, blended.y * 0.95, min(blended.z + 0.08, 1.0))
}

def get_bank_color() : float3 {
    let t = saturate(section_color_blend_t)
    return bank_color_from * (1.0 - t) + bank_color_to * t
}

def get_fog_color() : float3 {
    let t = saturate(section_color_blend_t)
    return fog_color_from * (1.0 - t) + fog_color_to * t
}

def section_speed_mult() : float {
    return 1.0 + float(current_section) * 0.15
}

def draw_with_flat(pos, scale, color : float3; rot_quat : float4 = float4(0.0, 0.0, 0.0, 1.0)) {
    v_model = compose(pos, rot_quat, scale)
    f_Color = float4(color, 1.0)
    vs_flat_bind_uniform(active_program)
    fs_flat_bind_uniform(active_program)
}

def draw_with_phong(pos, scale, color : float3; rot_quat : float4 = float4(0.0, 0.0, 0.0, 1.0)) {
    v_model = compose(pos, rot_quat, scale)
    f_Color = float4(color, 1.0)
    vs_phong_bind_uniform(active_program)
    fs_phong_bind_uniform(active_program)
}
