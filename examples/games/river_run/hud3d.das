options gen2
options persistent_heap

require rr_globals public
require gameplay

// Layout constants

let LIFE_ICON_SIZE = 18.0
let LIFE_ICON_GAP = 14.0
let LIFE_ICON_RIGHT_PAD = 30.0
let LIFE_ICON_TOP = 38.0

let FUEL_BAR_X = 24.0
let FUEL_BAR_Y = 28.0
let FUEL_BAR_W = 220.0
let FUEL_BAR_H = 18.0
let FUEL_BAR_PAD = 2.0

let PWR_ICON_X = 56.0
let PWR_ICON_Y0 = 78.0
let PWR_ROW_H = 56.0
let PWR_ICON_RADIUS = 16.0
let PWR_RING_RADIUS = 28.0
let PWR_RING_TICKS = 8
let PWR_TICK_SIZE = 3.5

let LIFE_BODY_COLOR = float3(0.7, 0.85, 1.0)

def hud_set_uniforms() {
    let near = -100.0
    let far  = 100.0
    v_projection = ortho_rh(0.0, float(display_w), float(display_h), 0.0, near, far)
    v_view = compose(float3(0.0), float4(0.0, 0.0, 0.0, 1.0), float3(1.0))
}

def draw_lives_3d(game_time : float) {
    if (player_lives <= 0) {
        return
    }
    let cy = LIFE_ICON_TOP
    let step = LIFE_ICON_SIZE * 2.0 + LIFE_ICON_GAP

    // Bodies (phong-shaded spheres)
    glUseProgram(phong_program)
    active_program = phong_program
    for (i in range(player_lives)) {
        let cx = float(display_w) - LIFE_ICON_RIGHT_PAD - float(i) * step - LIFE_ICON_SIZE
        draw_with_phong(float3(cx, cy, 0.0), float3(LIFE_ICON_SIZE), LIFE_BODY_COLOR)
        geo_sphere |> draw_geometry_fragment()
    }

    // Rotors (alpha-blended cross, spinning in screen XY plane)
    glEnable(GL_BLEND)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
    glDepthMask(false)
    glUseProgram(rotor_program)
    active_program = rotor_program
    let face_camera = quat_x_rot(PI * 0.5)
    let rotor_radius = LIFE_ICON_SIZE * 1.55
    let rotor_z = LIFE_ICON_SIZE * 0.6
    for (i in range(player_lives)) {
        let cx = float(display_w) - LIFE_ICON_RIGHT_PAD - float(i) * step - LIFE_ICON_SIZE
        let angle = game_time * 12.0 + float(i) * 0.7
        let spin = quat_z_rot(angle)
        let rot = quat_mul(spin, face_camera)
        v_model = compose(float3(cx, cy, rotor_z), rot, float3(rotor_radius, 0.5, rotor_radius))
        f_Color = float4(0.9, 0.9, 0.95, 0.9)
        vs_rotor_bind_uniform(active_program)
        fs_rotor_bind_uniform(active_program)
        geo_plane_xz |> draw_geometry_fragment()
    }
    glDepthMask(true)
    glDisable(GL_BLEND)
}

def draw_fuel_bar_3d() {
    glUseProgram(flat_program)
    active_program = flat_program

    // Frame
    let frame_cx = FUEL_BAR_X + FUEL_BAR_W * 0.5
    let frame_cy = FUEL_BAR_Y + FUEL_BAR_H * 0.5
    let frame_color = float3(0.05, 0.06, 0.10)
    draw_with_flat(float3(frame_cx, frame_cy, -0.5), float3(FUEL_BAR_W * 0.5, FUEL_BAR_H * 0.5, 0.5), frame_color)
    geo_cube |> draw_geometry_fragment()

    // Fill
    let fill_max_w = FUEL_BAR_W - 2.0 * FUEL_BAR_PAD
    let fill_w = fill_max_w * clamp(player_fuel / PLAYER_FUEL_MAX, 0.0, 1.0)
    if (fill_w > 0.001) {
        let fill_x = FUEL_BAR_X + FUEL_BAR_PAD + fill_w * 0.5
        let fill_h = FUEL_BAR_H - 2.0 * FUEL_BAR_PAD
        draw_with_flat(float3(fill_x, frame_cy, 0.5), float3(fill_w * 0.5, fill_h * 0.5, 0.5), fuel_bar_color())
        geo_cube |> draw_geometry_fragment()
    }
}

def draw_powerup_icon_3d(game_time : float; cx, cy : float; t : BonusType; timer, max_timer : float) {
    let color = bonus_color(t)
    let q_tilt = quat_y_rot(PI * 0.5)
    let spin = quat_z_rot(game_time * BONUS_SPIN_SPEED)
    let rot = quat_mul(spin, q_tilt)

    // Spinning cylinder (phong)
    glUseProgram(phong_program)
    active_program = phong_program
    draw_with_phong(
        float3(cx, cy, 0.5),
        float3(PWR_ICON_RADIUS * 0.42, PWR_ICON_RADIUS * 0.42, PWR_ICON_RADIUS * 1.05),
        color,
        rot
    )
    geo_cylinder |> draw_geometry_fragment()

    // Tick ring (flat)
    glUseProgram(flat_program)
    active_program = flat_program
    let frac = clamp(timer / max(max_timer, 0.001), 0.0, 1.0)
    let lit_count = int(frac * float(PWR_RING_TICKS) + 0.99)
    for (i in range(PWR_RING_TICKS)) {
        let angle = float(i) * (2.0 * PI / float(PWR_RING_TICKS)) - PI * 0.5
        let tx = cx + cos(angle) * PWR_RING_RADIUS
        let ty = cy + sin(angle) * PWR_RING_RADIUS
        let lit = i < lit_count
        let tick_col = (lit ? color : color * 0.22)
        draw_with_flat(
            float3(tx, ty, -0.25),
            float3(PWR_TICK_SIZE),
            tick_col
        )
        geo_cube |> draw_geometry_fragment()
    }
}

def draw_powerups_3d(game_time : float) {
    var row = 0
    if (player_multishot_timer > 0.0) {
        let cy = PWR_ICON_Y0 + float(row) * PWR_ROW_H
        draw_powerup_icon_3d(game_time, PWR_ICON_X, cy, BonusType.multishot,
            player_multishot_timer, BONUS_MULTISHOT_TIME)
        row += 1
    }
    if (player_fastshot_timer > 0.0) {
        let cy = PWR_ICON_Y0 + float(row) * PWR_ROW_H
        draw_powerup_icon_3d(game_time, PWR_ICON_X, cy, BonusType.fastshot,
            player_fastshot_timer, BONUS_FASTSHOT_TIME)
        row += 1
    }
}

def draw_hud_3d(game_time : float) {
    if (game_state == GameState.menu) {
        return
    }

    // Save 3D state
    let saved_proj = v_projection
    let saved_view = v_view
    let saved_fog = f_FogParams

    hud_set_uniforms()
    f_FogParams = float2(1.0e9, 1.0e9 + 1.0)

    // Fresh depth space for HUD geometry — sorts correctly among itself.
    glClear(GL_DEPTH_BUFFER_BIT)
    glEnable(GL_DEPTH_TEST)

    draw_fuel_bar_3d()
    draw_powerups_3d(game_time)
    draw_lives_3d(game_time)

    v_projection = saved_proj
    v_view = saved_view
    f_FogParams = saved_fog
}
