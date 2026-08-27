options gen2
options persistent_heap
options indenting = 4

require rr_globals public

// Text layer. Everything is positioned in DESIGN pixels against a 1280x720
// reference and scaled by hud_scale(), so the type keeps its proportions on a
// HiDPI framebuffer instead of shrinking to half size.
//
// Every string is drawn with a dark outline first. Over bright water or a lit
// sky, plain glyphs lose their edges entirely; the outline costs four extra
// quad draws and makes the HUD readable on any background.

let TEXT_TITLE = 1.65
let TEXT_HEAD = 0.90
let TEXT_BODY = 0.62
let TEXT_SMALL = 0.50

let OUTLINE_COLOR = float3(0.02, 0.03, 0.05)

def private text_mvp(x, y, scale : float) : float4x4 {
    let projection = ortho_rh(0.0, float(display_w), float(display_h), 0.0, -1.0, 1.0)
    let model = compose(float3(x, y, 0.0), float4(0.0, 0.0, 0.0, 1.0), float3(scale, scale, 1.0))
    return projection * model
}

def private text_width(quads : array<FontVertex>; scale : float) : float {
    let dim = quads_dim(quads)
    return (dim.vmax.x - dim.vmin.x) * scale
}

// Draw one string at a design-space position with an outline. `scale` is a
// multiplier on the design text size; hud_scale() is applied on top.
def draw_text(text : string; x, y : float; size : float = TEXT_BODY;
              tint : float3 = float3(1.0); outline : bool = true) {
    if (hud_font == null) {
        return
    }
    var quads <- (*hud_font) |> create_quads(text)
    let s = hud_scale() * size
    let px = x * hud_scale()
    let py = y * hud_scale()
    if (outline) {
        let o = max(hud_scale() * size * 1.1, 1.0)
        for (d in fixed_array(float2(-1.0, 0.0), float2(1.0, 0.0), float2(0.0, -1.0),
                float2(0.0, 1.0), float2(-0.7, -0.7), float2(0.7, 0.7))) {
            (*hud_font) |> draw_quads(quads, text_mvp(px + d.x * o, py + d.y * o, s), OUTLINE_COLOR)
        }
    }
    (*hud_font) |> draw_quads(quads, text_mvp(px, py, s), tint)
    delete quads
}

def draw_text_centered(text : string; cx, y : float; size : float = TEXT_BODY;
                       tint : float3 = float3(1.0)) {
    if (hud_font == null) {
        return
    }
    var quads <- (*hud_font) |> create_quads(text)
    let w = text_width(quads, size)
    delete quads
    draw_text(text, cx - w * 0.5, y, size, tint)
}

def private design_w() : float {
    return float(display_w) / hud_scale()
}

def private design_h() : float {
    return float(display_h) / hud_scale()
}

// A dimming plate behind a full-screen message, so titles never fight the
// scene behind them.
def private draw_scrim(alpha : float) {
    use_unlit_program()
    v_projection = ortho_rh(0.0, float(display_w), float(display_h), 0.0, -100.0, 100.0)
    v_view = compose(float3(0.0), float4(0.0, 0.0, 0.0, 1.0), float3(1.0))
    glEnable(GL_BLEND)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
    glDisable(GL_DEPTH_TEST)
    draw_unlit(
        float3(float(display_w) * 0.5, float(display_h) * 0.5, 0.0),
        float3(float(display_w) * 0.5, float(display_h) * 0.5, 1.0),
        float3(0.01, 0.015, 0.03), alpha
    )
    geo_cube |> draw_geometry_fragment()
}

def private draw_menu() {
    let cx = design_w() * 0.5
    let cy = design_h() * 0.5
    draw_scrim(0.55)
    draw_text_centered("RIVER RUN", cx, cy - 150.0, TEXT_TITLE, float3(0.55, 0.92, 1.0))
    draw_text_centered("fly the canyon · shoot what shoots back · do not run dry",
        cx, cy - 92.0, TEXT_SMALL, float3(0.62, 0.70, 0.78))

    draw_text_centered("ARROWS / A D      steer", cx, cy - 30.0, TEXT_BODY, float3(0.86, 0.90, 0.94))
    draw_text_centered("W S               throttle", cx, cy + 2.0, TEXT_BODY, float3(0.86, 0.90, 0.94))
    draw_text_centered("SPACE             fire", cx, cy + 34.0, TEXT_BODY, float3(0.86, 0.90, 0.94))
    draw_text_centered("ESC               pause", cx, cy + 66.0, TEXT_BODY, float3(0.86, 0.90, 0.94))

    let blink = 0.55 + 0.45 * abs(sin(get_uptime() * 2.6))
    draw_text_centered("PRESS SPACE TO START", cx, cy + 128.0, TEXT_HEAD,
        float3(1.0, 0.92, 0.45) * blink)
}

def private draw_playing_readout() {
    let w = design_w()
    // Score sits inboard of the top-right life panel and clear of the progress
    // rail, which is what the old centred readout kept colliding with.
    draw_text("SCORE", 28.0, 32.0, TEXT_SMALL, float3(0.52, 0.62, 0.72))
    draw_text("{score}", 28.0, 62.0, TEXT_HEAD, float3(1.0, 1.0, 1.0))

    let display_section = min(current_section + 1, MAX_SECTIONS)
    draw_text_centered("SECTION {display_section} / {MAX_SECTIONS}", w * 0.5, 30.0,
        TEXT_SMALL, float3(0.60, 0.72, 0.80))
    draw_text_centered("{env_now.name}", w * 0.5, 54.0, TEXT_BODY, float3(0.62, 0.86, 0.78))
}

def private draw_section_banner() {
    if (section_banner_timer <= 0.0) {
        return
    }
    let cx = design_w() * 0.5
    let cy = design_h() * 0.42
    // Fade in over the first quarter second, hold, then fade out.
    let t = section_banner_timer / WAVE_BANNER_DURATION
    let alpha = min(min((1.0 - t) * 6.0, 1.0), min(t * 2.2, 1.0))
    let display_section = min(current_section + 1, MAX_SECTIONS)
    draw_text_centered("SECTION {display_section}", cx, cy, TEXT_TITLE * 0.75,
        float3(1.0, 0.94, 0.55) * alpha)
    draw_text_centered("{env_now.name}", cx, cy + 46.0, TEXT_HEAD,
        float3(0.70, 0.88, 0.95) * alpha)
}

def private draw_overlay_card(title : string; title_color : float3; sub, hint : string) {
    let cx = design_w() * 0.5
    let cy = design_h() * 0.5
    draw_scrim(0.62)
    draw_text_centered(title, cx, cy - 70.0, TEXT_TITLE, title_color)
    if (sub != "") {
        draw_text_centered(sub, cx, cy + 6.0, TEXT_HEAD, float3(0.92, 0.95, 1.0))
    }
    let blink = 0.55 + 0.45 * abs(sin(get_uptime() * 2.6))
    draw_text_centered(hint, cx, cy + 62.0, TEXT_BODY, float3(0.80, 0.86, 0.92) * blink)
}

// Pause deliberately does NOT dim the scene. Pausing is how you stop and look
// at something -- a dimming scrim over the whole frame hides the very thing you
// paused to inspect. Game over and the win screen keep their scrim, because
// there the message IS what you are meant to be reading.
def private draw_pause_banner() {
    let cx = design_w() * 0.5
    let blink = 0.6 + 0.4 * abs(sin(get_uptime() * 2.6))
    draw_text_centered("PAUSED", cx, design_h() * 0.5 - 18.0, TEXT_HEAD,
        float3(1.0, 0.88, 0.35) * blink)
    draw_text_centered("ESC to resume", cx, design_h() * 0.5 + 14.0, TEXT_SMALL,
        float3(0.72, 0.78, 0.86))
}

def draw_hud() {
    if (hud_font == null) {
        return
    }
    glEnable(GL_BLEND)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
    glDisable(GL_DEPTH_TEST)

    if (game_state == GameState.menu) {
        draw_menu()
        glEnable(GL_DEPTH_TEST)
        return
    }

    draw_playing_readout()
    draw_section_banner()

    if (game_state == GameState.paused) {
        draw_pause_banner()
    } elif (game_state == GameState.game_over_state) {
        draw_overlay_card("GAME OVER", float3(1.0, 0.32, 0.28), "SCORE  {score}",
            "PRESS SPACE TO RESTART")
    } elif (game_state == GameState.win_state) {
        draw_overlay_card("RIVER RUN COMPLETE", float3(0.45, 1.0, 0.58), "SCORE  {score}",
            "PRESS SPACE TO RESTART")
    }

    glEnable(GL_DEPTH_TEST)
}
