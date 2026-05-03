options gen2
options persistent_heap

require rr_globals public

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

def draw_fuel_bar() {
    // Draw fuel label
    let low = player_fuel < LOW_FUEL_THRESHOLD
    let blink = low && int(get_uptime() * 6.0) % 2 == 0
    let bar_color = (
        low ? (blink ? float3(1.0, 0.2, 0.2) : float3(1.0, 0.5, 0.1)) : float3(0.2, 0.9, 0.3)
    )
    draw_text("FUEL", 10, display_h - 28, bar_color)
    // Represent fuel as bar of characters
    let bar_chars = 20
    let filled = int(player_fuel / PLAYER_FUEL_MAX * float(bar_chars))
    let bar_str = build_string() $(var w) {
        w |> write("[")
        for (i in range(bar_chars)) {
            w |> write(i < filled ? "#" : "-")
        }
        w |> write("]")
    }
    draw_text(bar_str, 52, display_h - 28, bar_color)
}

def draw_hud() {
    let cx = display_w / 2
    let cy = display_h / 2

    if (game_state == GameState.menu) {
        draw_text_centered("RIVER RUN", cx, cy - 80, float3(0.3, 0.9, 1.0))
        draw_text_centered("Use mouse or arrows to steer", cx, cy - 30, float3(0.8, 0.8, 0.8))
        draw_text_centered("SPACE or LMB to fire", cx, cy, float3(0.8, 0.8, 0.8))
        draw_text_centered("ESC to pause", cx, cy + 28, float3(0.8, 0.8, 0.8))
        draw_text_centered("PRESS SPACE TO START", cx, cy + 74, float3(0.9, 1.0, 0.5))
        return
    }

    // In-game HUD
    let display_section = min(current_section + 1, MAX_SECTIONS)
    draw_text("SCORE: {score}", 10, 24, float3(1.0))
    draw_text("LIVES: {player_lives}", 10, 52, float3(1.0, 0.9, 0.5))
    draw_text("SECTION: {display_section}", 10, 80, float3(0.6, 1.0, 0.7))

    let speed_max = PLAYER_FWD_SPEED_MAX * section_speed_mult()
    let speed_den = max(speed_max - PLAYER_FWD_SPEED_MIN, 0.001)
    let speed_t = clamp((player_fwd_speed - PLAYER_FWD_SPEED_MIN) / speed_den, 0.0, 1.0)
    let speed_pct = int(speed_t * 100.0)
    draw_text("SPD: {speed_pct}%", 10, 108, float3(0.7, 0.8, 1.0))

    draw_fuel_bar()

    // Controls hint
    draw_text("Arrows/mouse: steer  W/S: speed  SPC: fire  ESC: pause",
        10, display_h - 54, float3(0.5, 0.5, 0.6))

    // Section banner
    if (section_banner_timer > 0.0) {
        let alpha = min(section_banner_timer, 1.0)
        draw_text_centered("SECTION {display_section}", cx, cy - 12,
            float3(0.9, 1.0, 0.4) * alpha)
    }

    if (game_state == GameState.paused) {
        draw_text_centered("PAUSED", cx, cy - 20, float3(1.0, 0.9, 0.3))
        draw_text_centered("PRESS ESC TO RESUME", cx, cy + 16, float3(0.8, 0.8, 0.8))
    } elif (game_state == GameState.game_over_state) {
        draw_text_centered("GAME OVER", cx, cy - 24, float3(1.0, 0.25, 0.25))
        draw_text_centered("SCORE: {score}", cx, cy + 8, float3(1.0))
        draw_text_centered("PRESS SPACE TO RESTART", cx, cy + 40, float3(0.8, 0.8, 0.8))
    } elif (game_state == GameState.win_state) {
        draw_text_centered("YOU WIN!", cx, cy - 28, float3(0.4, 1.0, 0.5))
        draw_text_centered("SCORE: {score}", cx, cy + 8, float3(1.0))
        draw_text_centered("PRESS SPACE TO RESTART", cx, cy + 40, float3(0.8, 0.8, 0.8))
    }
}
