options gen2
options persistent_heap
options indenting = 4

require rr_globals public
require gameplay

// The instrument layer: fuel gauge, throttle, life icons, powerup timers and
// the section progress bar, all drawn as flat 2D geometry in a screen-space
// orthographic pass on top of the tone-mapped frame.
//
// Every constant below is in DESIGN pixels against a 1280x720 reference and is
// multiplied by hud_scale() at draw time. That is the fix for the old HUD,
// which used raw framebuffer pixels and so came out half size on any HiDPI
// display.

let PANEL_MARGIN = 26.0
let PANEL_PAD = 12.0

let GAUGE_W = 240.0
let GAUGE_H = 16.0
let GAUGE_LABEL_H = 15.0

let THROTTLE_W = 240.0
let THROTTLE_H = 8.0

let LIFE_ICON_R = 13.0
let LIFE_ICON_GAP = 12.0

let PWR_ICON_R = 15.0
let PWR_RING_R = 23.0
let PWR_RING_TICKS = 16
let PWR_ROW_H = 58.0

let PROGRESS_H = 3.0

let COLOR_PANEL = float3(0.03, 0.045, 0.075)
let COLOR_FRAME = float3(0.34, 0.46, 0.58)
let COLOR_DIM = float3(0.16, 0.21, 0.27)
let COLOR_ACCENT = float3(0.35, 0.86, 1.0)

def hud_set_uniforms() {
    v_projection = ortho_rh(0.0, float(display_w), float(display_h), 0.0, -100.0, 100.0)
    v_view = compose(float3(0.0), float4(0.0, 0.0, 0.0, 1.0), float3(1.0))
}

// A screen-space axis-aligned rectangle, given in design pixels.
def hud_rect(x, y, w, h : float; color : float3; alpha : float = 1.0; emissive : float = 0.0) {
    let s = hud_scale()
    draw_unlit(
        float3((x + w * 0.5) * s, (y + h * 0.5) * s, 0.0),
        float3(w * 0.5 * s, h * 0.5 * s, 1.0),
        color, alpha, float4(0.0, 0.0, 0.0, 1.0), emissive
    )
    geo_cube |> draw_geometry_fragment()
}

def hud_disc(cx, cy, r : float; color : float3; alpha : float = 1.0; emissive : float = 0.0) {
    let s = hud_scale()
    draw_unlit(
        float3(cx * s, cy * s, 0.0), float3(r * s, r * s, 1.0),
        color, alpha, float4(0.0, 0.0, 0.0, 1.0), emissive
    )
    geo_disc |> draw_geometry_fragment()
}

// A framed panel: dark plate, a hairline border, and a bright inset on the top
// edge so the whole HUD reads as one instrument cluster rather than loose bars.
def hud_panel(x, y, w, h : float; alpha : float = 0.72) {
    hud_rect(x, y, w, h, COLOR_PANEL, alpha)
    hud_rect(x, y, w, 1.5, COLOR_FRAME, alpha * 0.9)
    hud_rect(x, y + h - 1.5, w, 1.5, COLOR_FRAME, alpha * 0.5)
    hud_rect(x, y, 1.5, h, COLOR_FRAME, alpha * 0.5)
    hud_rect(x + w - 1.5, y, 1.5, h, COLOR_FRAME, alpha * 0.5)
}

def design_width() : float {
    return float(display_w) / hud_scale()
}

def design_height() : float {
    return float(display_h) / hud_scale()
}

// --- Fuel + throttle cluster (bottom left) ---

def private draw_fuel_gauge(x, y : float) {
    let frac = clamp(player_fuel / PLAYER_FUEL_MAX, 0.0, 1.0)
    let color = fuel_bar_color()

    hud_rect(x, y, GAUGE_W, GAUGE_H, COLOR_DIM, 0.9)
    if (frac > 0.001) {
        hud_rect(x + 1.5, y + 1.5, (GAUGE_W - 3.0) * frac, GAUGE_H - 3.0, color, 1.0, 0.55)
        // A brighter cap at the leading edge gives the bar a readable head as
        // it drains, which a flat fill does not.
        let head_x = x + 1.5 + max((GAUGE_W - 3.0) * frac - 3.0, 0.0)
        hud_rect(head_x, y + 1.5, 3.0, GAUGE_H - 3.0, color * 1.6 + float3(0.25), 1.0, 1.4)
    }
    // Quarter ticks, so the pilot can read "half tank" without counting pixels.
    for (i in range(1, 4)) {
        let tx = x + GAUGE_W * float(i) * 0.25
        hud_rect(tx - 0.75, y, 1.5, GAUGE_H, COLOR_PANEL, 0.85)
    }
}

def private draw_throttle(x, y : float) {
    let speed_max = PLAYER_FWD_SPEED_MAX * section_speed_mult()
    let den = max(speed_max - PLAYER_FWD_SPEED_MIN, 0.001)
    let frac = clamp((player_fwd_speed - PLAYER_FWD_SPEED_MIN) / den, 0.0, 1.0)
    hud_rect(x, y, THROTTLE_W, THROTTLE_H, COLOR_DIM, 0.9)
    if (frac > 0.001) {
        hud_rect(x + 1.0, y + 1.0, (THROTTLE_W - 2.0) * frac, THROTTLE_H - 2.0,
            lerp(COLOR_ACCENT, float3(1.0, 0.85, 0.35), frac), 1.0, 0.4)
    }
}

def draw_status_cluster() {
    let base_y = design_height() - PANEL_MARGIN - (GAUGE_LABEL_H + GAUGE_H + THROTTLE_H + PANEL_PAD * 2.0 + 6.0)
    let panel_w = GAUGE_W + PANEL_PAD * 2.0
    let panel_h = GAUGE_LABEL_H + GAUGE_H + THROTTLE_H + PANEL_PAD * 2.0 + 6.0
    hud_panel(PANEL_MARGIN, base_y, panel_w, panel_h)

    let inner_x = PANEL_MARGIN + PANEL_PAD
    draw_fuel_gauge(inner_x, base_y + PANEL_PAD + GAUGE_LABEL_H)
    draw_throttle(inner_x, base_y + PANEL_PAD + GAUGE_LABEL_H + GAUGE_H + 6.0)
}

// --- Lives (top right) ---
//
// The life counter draws the player's own silhouette rather than an abstract
// pip, so the icon and the thing it stands for match.

def draw_lives() {
    if (player_lives <= 0) {
        return
    }
    let s = hud_scale()
    let step = LIFE_ICON_R * 2.0 + LIFE_ICON_GAP
    let top = PANEL_MARGIN + LIFE_ICON_R + 4.0
    let panel_w = float(player_lives) * step + PANEL_PAD * 2.0 - LIFE_ICON_GAP
    hud_panel(design_width() - PANEL_MARGIN - panel_w, PANEL_MARGIN,
        panel_w, LIFE_ICON_R * 2.0 + PANEL_PAD * 1.5)

    for (i in range(player_lives)) {
        let cx = design_width() - PANEL_MARGIN - PANEL_PAD - LIFE_ICON_R - float(i) * step
        hud_disc(cx, top, LIFE_ICON_R + 2.0, COLOR_FRAME, 0.35)
        // Nose-up plan view of the helicopter, scaled to the icon radius.
        draw_prop(
            float3(cx * s, top * s, 0.0),
            float3(LIFE_ICON_R * 0.92 * s),
            float3(1.0),
            quat_mul(quat_z_rot(PI), quat_x_rot(-PI * 0.5)),
            MAT_HULL
        )
        geo_heli |> draw_geometry_fragment()
    }
}

// --- Powerup timers (left, under the status cluster) ---

def private draw_powerup(cx, cy : float; t : BonusType; timer, max_timer : float) {
    let color = bonus_color(t)
    let frac = clamp(timer / max(max_timer, 0.001), 0.0, 1.0)

    hud_disc(cx, cy, PWR_ICON_R, COLOR_PANEL, 0.85)
    hud_disc(cx, cy, PWR_ICON_R - 3.0, color, 0.95, 0.7)

    // A ring of ticks reading counter-clockwise from the top as the timer runs
    // down -- readable at a glance without a number.
    let lit = int(frac * float(PWR_RING_TICKS) + 0.999)
    for (i in range(PWR_RING_TICKS)) {
        let angle = float(i) * (2.0 * PI / float(PWR_RING_TICKS)) - PI * 0.5
        let tx = cx + cos(angle) * PWR_RING_R
        let ty = cy + sin(angle) * PWR_RING_R
        let on = i < lit
        hud_disc(tx, ty, (on ? 2.6 : 1.8), (on ? color : COLOR_DIM), 1.0, (on ? 0.8 : 0.0))
    }

    // The last two seconds flash, so an expiring powerup is impossible to miss.
    if (timer < 2.0 && int(get_uptime() * 8.0) % 2 == 0) {
        hud_disc(cx, cy, PWR_RING_R + 4.0, color, 0.22, 0.5)
    }
}

def draw_powerups() {
    let base_y = design_height() - PANEL_MARGIN - 150.0
    var row = 0
    if (player_multishot_timer > 0.0) {
        draw_powerup(PANEL_MARGIN + PWR_RING_R + 6.0, base_y - float(row) * PWR_ROW_H,
            BonusType.multishot, player_multishot_timer, BONUS_MULTISHOT_TIME)
        row++
    }
    if (player_fastshot_timer > 0.0) {
        draw_powerup(PANEL_MARGIN + PWR_RING_R + 6.0, base_y - float(row) * PWR_ROW_H,
            BonusType.fastshot, player_fastshot_timer, BONUS_FASTSHOT_TIME)
        row++
    }
}

// --- Section progress (a thin rail across the top of the screen) ---

def draw_progress_rail() {
    let w = design_width()
    let frac = clamp(section_dist / SECTION_LENGTH, 0.0, 1.0)
    hud_rect(0.0, 0.0, w, PROGRESS_H, COLOR_PANEL, 0.45)
    hud_rect(0.0, 0.0, w * frac, PROGRESS_H, COLOR_ACCENT, 0.95, 0.6)

    // Section boundaries as notches, so the rail also shows how far into the
    // run you are overall.
    for (i in range(1, MAX_SECTIONS)) {
        let tx = w * float(i) / float(MAX_SECTIONS)
        let passed = i <= current_section
        hud_rect(tx - 1.0, 0.0, 2.0, PROGRESS_H + 2.0,
            (passed ? COLOR_ACCENT : COLOR_DIM), 0.75)
    }
}

// --- Low fuel warning bar ---

def draw_low_fuel_warning() {
    if (player_fuel >= LOW_FUEL_THRESHOLD || game_state != GameState.playing) {
        return
    }
    let pulse = 0.35 + 0.35 * abs(sin(get_uptime() * 6.0))
    let h = design_height()
    let w = design_width()
    // A vignette-like band top and bottom rather than a full-screen wash, so
    // the warning never obscures the water the player is threading.
    hud_rect(0.0, 0.0, w, 26.0, float3(1.0, 0.15, 0.1), pulse * 0.5, 0.6)
    hud_rect(0.0, h - 26.0, w, 26.0, float3(1.0, 0.15, 0.1), pulse * 0.5, 0.6)
}

// The scene shaders fog by view distance, and in the screen-space ortho pass
// "view distance" is a pixel coordinate -- hundreds of units, which fogs every
// panel and scrim to solid haze. The whole overlay therefore runs with fog
// switched off; the caller owns the save/restore because the text layer draws
// its own geometry too.
def begin_hud_overlay() : float3 {
    let saved_fog = f_FogParams
    f_FogParams = float3(0.0, 0.0, 0.0)
    return saved_fog
}

def end_hud_overlay(saved_fog : float3) {
    f_FogParams = saved_fog
}

def draw_hud_3d(_game_time : float) {
    let saved_proj = v_projection
    let saved_view = v_view
    hud_set_uniforms()
    glDisable(GL_DEPTH_TEST)
    glDisable(GL_CULL_FACE)
    glEnable(GL_BLEND)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)

    if (game_state != GameState.menu) {
        use_unlit_program()
        draw_progress_rail()
        draw_status_cluster()
        draw_powerups()
        draw_low_fuel_warning()

        // Life icons are lit geometry under a fixed palette, so the counter
        // stays legible at night and in the ash channel.
        if (player_lives > 0) {
            use_prop_program()
            let saved_light = push_hud_light()
            player_hud_palette()
            draw_lives()
            clear_model_palette()
            pop_hud_light(saved_light)
        }
    }

    // Culling and depth stay off: the text layer draws next, and its quads have
    // no reliable winding.
    glEnable(GL_DEPTH_TEST)
    v_projection = saved_proj
    v_view = saved_view
}

// A fixed, sunlit palette for the HUD helicopters so they stay legible at
// night and in the ash channel.
def private player_hud_palette() {
    set_model_palette(
        float3(0.78, 0.86, 0.94),
        float3(0.20, 0.38, 0.48),
        float3(0.58, 0.62, 0.66),
        float3(0.12, 0.14, 0.16)
    )
}

// The life icons are lit geometry drawn in screen space, where the world sun
// direction means nothing -- at night it left them unlit and unreadable. Swap
// in a fixed three-quarter key for the icon draw and put the scene's lighting
// back afterwards.
struct SavedLight {
    sun_dir : float3
    sun_color : float3
    sky_color : float3
    horizon_color : float3
    bounce_color : float3
}

def private push_hud_light() : SavedLight {
    let saved = SavedLight(
        sun_dir = f_SunDir,
        sun_color = f_SunColor,
        sky_color = f_SkyColor,
        horizon_color = f_HorizonColor,
        bounce_color = f_BounceColor
    )
    f_SunDir = normalize(float3(-0.45, -0.55, 0.70))
    f_SunColor = float3(1.15, 1.18, 1.25)
    f_SkyColor = float3(0.26, 0.30, 0.38)
    f_HorizonColor = float3(0.30, 0.34, 0.40)
    f_BounceColor = float3(0.10, 0.12, 0.16)
    return saved
}

def private pop_hud_light(saved : SavedLight) {
    f_SunDir = saved.sun_dir
    f_SunColor = saved.sun_color
    f_SkyColor = saved.sky_color
    f_HorizonColor = saved.horizon_color
    f_BounceColor = saved.bounce_color
}
