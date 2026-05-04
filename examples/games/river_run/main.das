options gen2
options persistent_heap

require rr_globals public
require rr_audio
require river
require gameplay
require hud
require hud3d

// --- GL Setup ---

def create_gl_objects() {
    flat_program = cache_shader_program(vs_flat`shader_text, fs_flat`shader_text)
    phong_program = cache_shader_program(vs_phong`shader_text, fs_phong`shader_text)
    river_program = cache_shader_program(vs_river`shader_text, fs_river`shader_text)
    rotor_program = cache_shader_program(vs_rotor`shader_text, fs_rotor`shader_text)
    grass_program = cache_shader_program(vs_grass`shader_text, fs_grass`shader_text)

    geo_sphere <- create_geometry_fragment <| gen_sphere(10, 7, false)
    geo_cube <- create_geometry_fragment <| gen_cube()
    geo_plane_xz <- create_geometry_fragment <| gen_plane(GenDirection.xz)
    geo_cylinder <- create_geometry_fragment <| gen_cylinder(GenDirection.xy, 8)
    geo_cone <- create_geometry_fragment <| gen_cone(GenDirection.xy, 10)
    geo_prism <- create_geometry_fragment <| gen_prism(GenDirection.xy)

    cache_ttf_objects()
    hud_font = cache_font("{get_das_root()}/modules/dasStbImage/fonts/droidsansmono.ttf")
}

// --- Camera ---

def update_camera() {
    // Camera lags behind player X
    cam_x += (player_pos.x - cam_x) * CAM_X_LAG

    // Pitch lerps with current player speed: slow → CAM_PITCH_SLOW_DEG, fast → CAM_PITCH_FAST_DEG.
    let speed_max = PLAYER_FWD_SPEED_MAX * section_speed_mult()
    let speed_den = max(speed_max - PLAYER_FWD_SPEED_MIN, 0.001)
    let speed_t = clamp((player_fwd_speed - PLAYER_FWD_SPEED_MIN) / speed_den, 0.0, 1.0)
    let pitch_deg = CAM_PITCH_SLOW_DEG + (CAM_PITCH_FAST_DEG - CAM_PITCH_SLOW_DEG) * speed_t
    let cam_height = (CAM_BACK + CAM_LOOK_AHEAD) * tan(pitch_deg * PI / 180.0)

    let t = float(get_uptime())
    let shake = float3(sin(t * 47.0), sin(t * 53.0), sin(t * 61.0)) * screen_shake_amount

    let eye = float3(cam_x, player_pos.y - CAM_BACK, cam_height) + shake
    let center = float3(player_pos.x, player_pos.y + CAM_LOOK_AHEAD, 0.0)
    let up = float3(0.0, 0.0, 1.0)
    v_view = look_at_rh(eye, center, up)
}

// --- State Transitions ---

var prev_esc_pressed = false
var prev_space_pressed = false

def handle_menu_input() {
    let space = glfwGetKey(live_window, int(GLFW_KEY_SPACE)) == int(GLFW_PRESS)
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
    let esc = glfwGetKey(live_window, int(GLFW_KEY_ESCAPE)) == int(GLFW_PRESS)
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
        rebuild_river_geometry()
        river_dirty = false
    }
}

[export]
def update() {
    if (!live_begin_frame()) {
        return
    }

    live_get_framebuffer_size(display_w, display_h)
    glViewport(0, 0, display_w, display_h)

    // Sky color: blend from section sky color to dark at horizon
    let sky = get_river_color() * 0.15 + float3(0.03, 0.05, 0.12)
    glClearColor(sky.x, sky.y, sky.z, 1.0)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glEnable(GL_DEPTH_TEST)
    glEnable(GL_CULL_FACE)
    glCullFace(GL_BACK)
    glFrontFace(GL_CCW)

    let aspect = (display_h != 0 ? float(display_w) / float(display_h) : 1.0)
    v_projection = perspective_rh_opengl(50.0 * PI / 180.0, aspect, 0.1, 500.0)
    update_camera()

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

    let game_time = float(get_uptime())

    handle_menu_input()
    handle_pause_input()

    if (game_state == GameState.playing) {
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

    update_music_state()
    update_engine_sound(game_state == GameState.playing, player_fwd_speed, PLAYER_FWD_SPEED_MAX * section_speed_mult())

    // Fog uniforms — set once per frame before any render
    f_FogColor = get_fog_color()
    f_FogParams = float2(player_pos.y + 3.0, player_pos.y + 30.0)

    // Render
    render_river(game_time)
    render_all(game_time)

    glDisable(GL_CULL_FACE)
    draw_hud_3d(game_time)
    draw_hud()
    glEnable(GL_CULL_FACE)

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
