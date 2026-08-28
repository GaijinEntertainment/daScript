options gen2
options persistent_heap
options gc

require rr_globals public
require rr_audio
require river
require gameplay
require hud
require hud3d
require rr_live
require daslib/strings_convert

// --- GL Setup ---

def create_gl_objects() {
    create_scene_programs()
    create_postfx_programs()

    geo_sphere <- create_geometry_fragment <| gen_sphere(14, 10, false)
    geo_cube <- create_geometry_fragment <| gen_cube()
    geo_cylinder <- create_geometry_fragment <| gen_cylinder(GenDirection.xy, 12)
    geo_cone <- create_geometry_fragment <| gen_cone(GenDirection.xy, 12)
    geo_prism <- create_geometry_fragment <| gen_prism(GenDirection.xy)
    geo_disc <- create_geometry_fragment <| gen_disc(24)
    geo_heli <- create_geometry_fragment <| gen_helicopter()
    geo_gunboat <- create_geometry_fragment <| gen_gunboat()
    geo_jet <- create_geometry_fragment <| gen_jet()
    for (frag in geo_shards) {
        finalize(frag)
    }
    geo_shards |> clear()
    geo_shards |> reserve(SHARD_VARIANTS)
    for (i in range(SHARD_VARIANTS)) {
        geo_shards |> emplace(create_geometry_fragment(gen_shard_variant(i)))
    }

    // Several pre-generated instances of each flora kind, so a crowded bank is
    // not the same tree stamped forty times.
    for (frag in geo_flora) {
        finalize(frag)
    }
    geo_flora |> clear()
    geo_flora |> reserve(FLORA_VARIANTS * FLORA_INSTANCES)
    for (kind in range(FLORA_VARIANTS)) {
        for (i in range(FLORA_INSTANCES)) {
            geo_flora |> emplace(create_geometry_fragment(gen_flora(kind, kind * 31 + i * 7 + 1)))
        }
    }

    cache_ttf_objects()
    hud_font = cache_font("{get_das_root()}/modules/dasStbImage/fonts/droidsansmono.ttf")
}

// --- Camera ---

var cam_eye = float3(0.0, 0.0, 10.0)
var cam_right = float3(1.0, 0.0, 0.0)
var cam_up = float3(0.0, 0.0, 1.0)
var cam_fwd = float3(0.0, 1.0, 0.0)
var cam_scale = float2(1.0, 1.0)

let CAM_FOV_DEG = 50.0
let Z_NEAR = 0.1
let Z_FAR = 420.0

def update_camera(aspect : float) {
    // Camera lags behind player X
    cam_x += (player_pos.x - cam_x) * CAM_X_LAG

    // Pitch lerps with current player speed: slow → CAM_PITCH_SLOW_DEG, fast → CAM_PITCH_FAST_DEG.
    let speed_max = PLAYER_FWD_SPEED_MAX * section_speed_mult()
    let speed_den = max(speed_max - PLAYER_FWD_SPEED_MIN, 0.001)
    let speed_t = clamp((player_fwd_speed - PLAYER_FWD_SPEED_MIN) / speed_den, 0.0, 1.0)
    let pitch_deg = CAM_PITCH_SLOW_DEG + (CAM_PITCH_FAST_DEG - CAM_PITCH_SLOW_DEG) * speed_t
    let cam_height = (CAM_BACK + CAM_LOOK_AHEAD) * tan(pitch_deg * PI / 180.0)

    let t = get_uptime()
    let shake = float3(sin(t * 47.0), sin(t * 53.0), sin(t * 61.0)) * screen_shake_amount

    cam_eye = float3(cam_x, player_pos.y - CAM_BACK, cam_height) + shake
    let center = float3(player_pos.x, player_pos.y + CAM_LOOK_AHEAD, 0.0)
    let world_up = float3(0.0, 0.0, 1.0)
    v_view = look_at_rh(cam_eye, center, world_up)

    // The sky pass is attributeless, so it rebuilds view rays from this basis
    // rather than from an inverse view-projection.
    cam_fwd = normalize(center - cam_eye)
    cam_right = normalize(cross(cam_fwd, world_up))
    cam_up = cross(cam_right, cam_fwd)
    let tan_half = tan(CAM_FOV_DEG * 0.5 * PI / 180.0)
    cam_scale = float2(tan_half * aspect, tan_half)
}

// --- Lighting environment ---
//
// One ortho cascade tracking the play area ahead of the player. The box is wide
// enough to hold both banks and long enough to cover everything the camera can
// see, which is all a game this shallow needs.

let SHADOW_HALF_WIDTH = 34.0
let SHADOW_AHEAD = 58.0
let SHADOW_BEHIND = 22.0
let SHADOW_DEPTH = 120.0

def build_light_matrix() {
    let focus = float3(cam_x, player_pos.y + (SHADOW_AHEAD - SHADOW_BEHIND) * 0.5, 1.5)
    let eye = focus + env_now.sun_dir * (SHADOW_DEPTH * 0.5)
    let up = float3(0.0, 0.0, 1.0)
    let half_len = (SHADOW_AHEAD + SHADOW_BEHIND) * 0.5
    let view = look_at_rh(eye, focus, up)
    let proj = ortho_rh(-SHADOW_HALF_WIDTH, SHADOW_HALF_WIDTH,
        -half_len, half_len, 0.1, SHADOW_DEPTH)
    v_light_vp = proj * view
}

def apply_environment(game_time : float) {
    f_SunDir = env_now.sun_dir
    f_SunColor = env_now.sun_color
    f_SkyColor = env_now.sky_color
    f_HorizonColor = env_now.horizon_color
    f_BounceColor = env_now.bounce_color
    f_FogColor = env_now.fog_color
    f_FogParams = float3(env_now.fog_density, 0.018, 38.0)
    f_NightAmount = env_now.night
    f_CameraPos = cam_eye
    f_GameTime = game_time
    f_ZFar = Z_FAR
    f_CamRight = cam_right
    f_CamUp = cam_up
    f_CamFwd = cam_fwd
    f_CamScale = cam_scale
    f_ShadowMap := shadow_tex
    f_ShadowTexel = float2(1.0 / float(SHADOW_SIZE))
    f_PaletteOn = 0.0
}

// Where the sun lands on screen, for the shaft pass. Returns w <= 0 behind the
// camera, in which case the caller skips the whole scattering chain.
def sun_screen_uv() : float3 {
    let sun_world = cam_eye + env_now.sun_dir * 900.0
    let clip = v_projection * (v_view * float4(sun_world, 1.0))
    if (clip.w <= 0.0) {
        return float3(0.5, 0.5, 0.0)
    }
    let ndc = clip.xyz / clip.w
    let uv = ndc.xy * 0.5 + float2(0.5)
    // Let the shafts persist a little past the frame edge so the effect fades
    // out as the sun leaves the view instead of popping off.
    let on_screen = (uv.x > -0.45 && uv.x < 1.45 && uv.y > -0.45 && uv.y < 1.45 ? 1.0 : 0.0)
    return float3(uv, on_screen)
}

def build_post_settings(game_time : float) : PostSettings {
    var cfg = PostSettings()
    let sun_uv = sun_screen_uv()
    cfg.exposure = env_now.exposure
    cfg.bloom_strength = env_now.bloom
    cfg.shaft_uv = sun_uv.xy
    cfg.shaft_on_screen = sun_uv.z
    cfg.shaft_strength = env_now.shafts
    cfg.grade_tint = env_now.grade_tint
    cfg.grade_amount = env_now.grade_amount
    cfg.tan_half_fov = cam_scale
    cfg.z_far = Z_FAR
    cfg.time = game_time
    // A hit briefly hot-grades the frame: more bloom, more grain, harder
    // vignette. Cheap, and it sells the impact better than the shake alone.
    let stress = saturate(screen_shake_amount * 0.9)
    cfg.bloom_strength += stress * 0.5
    cfg.grain += stress * 0.05
    cfg.vignette_strength += stress * 0.25
    return cfg
}

// --- State Transitions ---

var prev_esc_pressed = false
var prev_space_pressed = false

def handle_menu_input() {
    let space = glfwGetKey(live_window, GLFW_KEY_SPACE) == GLFW_PRESS
    let space_just = space && !prev_space_pressed
    prev_space_pressed = space

    if (game_state == GameState.menu && space_just) {
        reset_game()
    } elif ((game_state == GameState.game_over_state || game_state == GameState.win_state)
            && space_just && restart_input_lock <= 0.0) {
        reset_game()
    }
}

def handle_pause_input() {
    let esc = glfwGetKey(live_window, GLFW_KEY_ESCAPE) == GLFW_PRESS
    let esc_just = esc && !prev_esc_pressed
    prev_esc_pressed = esc

    if (!esc_just) {
        return
    }
    if (game_state == GameState.playing) {
        game_state = GameState.paused
    } elif (game_state == GameState.paused) {
        game_state = GameState.playing
    } elif (game_state == GameState.menu) {
        pass
    }
}

// --- Main Init / Update / Shutdown ---

[export]
def init() {
    live_create_window("River Run", 1280, 720)
    create_gl_objects()
    init_audio()

    if (!is_reload()) {
        decs::restart()
        commit()
        game_state = GameState.menu
        player_pos = float3(0.0, 0.0, PLAYER_Z)
        player_lives = 3
        player_fuel = PLAYER_FUEL_MAX
        score = 0
        current_section = 0
        cam_x = 0.0
        init_river()
    } elif (empty(river_segments)) {
        // A FULL reload resets every @live variable, so the river is gone even
        // though is_reload() is true. Rebuild rather than run on an empty world.
        init_river()
    }

    // Neither of these survives a reload: the section environment lives in
    // plain (non-@live) globals, and the geometry handles name GL objects in a
    // context that has just been destroyed. Rebuilding both unconditionally is
    // what keeps a reload from coming back to a black screen.
    set_env_immediate(section_idx())
    rebuild_river_geometry()
    river_dirty = false
}

def simulate() {
    // fx_freeze is the effect-inspection rig: it holds the whole world, not just
    // the particles, because otherwise the player flies out of the blast's
    // postcode between one inspection command and the next.
    if (game_state != GameState.playing || fx_freeze) {
        return
    }
    update_section()
    update_player(false)
    commit()
    if (game_state == GameState.playing) {
        update_enemies()
        process_collisions()
        commit()
    }
    if (game_state == GameState.playing) {
        cull_far_entities()
        advance_river()
        if (river_dirty) {
            rebuild_river_geometry()
            river_dirty = false
        }
    }
}

def render_frame(game_time : float) {
    ensure_render_targets(display_w, display_h)

    build_light_matrix()
    apply_environment(game_time)

    begin_shadow_pass()
    render_shadow_casters(game_time)
    end_shadow_pass()

    begin_scene_pass(env_now.horizon_color)

    // Sky first, depth-writes off: it is a background, and every later surface
    // must be able to draw over it.
    glDepthMask(false)
    glDisable(GL_DEPTH_TEST)
    glDisable(GL_CULL_FACE)
    glUseProgram(sky_program)
    active_program = sky_program
    vs_sky_bind_uniform(sky_program)
    fs_sky_bind_uniform(sky_program)
    fullscreen_pass()
    glEnable(GL_DEPTH_TEST)
    glDepthMask(true)
    glEnable(GL_CULL_FACE)
    glCullFace(GL_BACK)
    glFrontFace(GL_CCW)

    render_river(game_time)
    render_all(game_time)

    begin_transparent_span()
    render_effects(game_time)
    end_transparent_span()

    run_postfx(build_post_settings(game_time))

    // HUD lands on the back buffer after tone mapping, so it keeps its designed
    // colours instead of being graded with the scene. Culling stays off for the
    // whole overlay -- glyph quads have no reliable winding.
    glDisable(GL_CULL_FACE)
    let saved_fog = begin_hud_overlay()
    draw_hud_3d(game_time)
    draw_hud()
    end_hud_overlay(saved_fog)
    glEnable(GL_CULL_FACE)
}

[export]
def update() {
    if (!live_begin_frame()) {
        return
    }

    live_get_framebuffer_size(display_w, display_h)
    let aspect = (display_h != 0 ? float(display_w) / float(display_h) : 1.0)
    v_projection = perspective_rh_minus1_to_1(CAM_FOV_DEG * PI / 180.0, aspect, Z_NEAR, Z_FAR)

    let real_dt = get_dt()
    tick_dt = min(real_dt, 1.0 / 30.0)

    if (slow_mo_timer > 0.0) {
        slow_mo_timer -= real_dt
        tick_dt *= SLOW_MO_SCALE
    }

    screen_shake_amount = max(screen_shake_amount - SHAKE_DECAY * real_dt, 0.0)

    if (restart_input_lock > 0.0) {
        restart_input_lock -= tick_dt
    }

    let game_time = get_uptime()

    handle_menu_input()
    handle_pause_input()
    simulate()

    update_music_state()
    update_engine_sound(game_state == GameState.playing, player_fwd_speed,
        PLAYER_FWD_SPEED_MAX * section_speed_mult())

    update_camera(aspect)
    render_frame(game_time)

    live_end_frame()
}

[export]
def shutdown() {
    shutdown_audio()
    live_destroy_window()
}

var max_frame_limit = 0

def parse_args() {
    let args <- get_command_line_arguments()
    for (i in range(length(args) - 1)) {
        if (args[i] == "--max-frames") {
            max_frame_limit = try_to_int(args[i + 1]) ?? 0   // garbage = no cap
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
        maybe_collect_gc()
        frame_count++
        if (max_frame_limit > 0 && frame_count >= max_frame_limit) {
            break
        }
    }
    shutdown()
}
