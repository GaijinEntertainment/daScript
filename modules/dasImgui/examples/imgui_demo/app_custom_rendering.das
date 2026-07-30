options gen2
options _comment_hygiene = true
options indenting = 4

module app_custom_rendering

require imgui public
require imgui/imgui_widgets_builtin public
require imgui/imgui_containers_builtin public
require imgui/imgui_scope_builtin public
require imgui/imgui_drawlist_builtin public
require imgui/imgui_colors public
require daslib/safe_addr
require math

// Mirrors imgui_demo.cpp:8122-8423 — static void ShowExampleAppCustomRendering(bool* p_open).
//
// Four tabs:
//   - Primitives — gradient strips + a 2-row "All primitives" drawer of
//                  stroked / filled shapes (ngon / circle / ellipse / square
//                  / triangle / concave / lines / arc / bezier curves).
//   - Canvas     — scrollable infinite-grid canvas; left-drag adds lines,
//                  right-drag pans, right-click opens Remove One / Remove All
//                  context menu. Lines stored module-scope so they survive
//                  live-reload.
//   - BG/FG draw lists — two checkboxes; draws a circle on the background or
//                  foreground drawlist (escapes ImGui window stacking).
//   - Draw Channels — channels_split / channels_set_current / channels_merge
//                  side-by-side with the naive draw order.

// ===== Primitives tab state =====

var private CR_SZ = 36.0f
var private CR_THICKNESS = 3.0f
var private CR_NGON_SIDES = 6
var private CR_CIRCLE_SEG_OVERRIDE = false
var private CR_CIRCLE_SEG_OVERRIDE_V = 12
var private CR_CURVE_SEG_OVERRIDE = false
var private CR_CURVE_SEG_OVERRIDE_V = 8
var private CR_COLOR = float4(1.0f, 1.0f, 0.4f, 1.0f)

// ===== Canvas tab state =====

var private CR_CANVAS_POINTS : array<float2>
var private CR_CANVAS_SCROLLING = float2(0.0f, 0.0f)
var private CR_CANVAS_OPT_GRID = true
var private CR_CANVAS_OPT_CTX = true
var private CR_CANVAS_ADDING_LINE = false

// ===== BG/FG tab state =====

var private CR_DRAW_BG = true
var private CR_DRAW_FG = true

// ===== Helpers =====

// Same shape as imgui_demo.cpp's PathConcaveShape — 9-point spiral that
// produces a non-convex polygon both for PathStroke and PathFillConcave.
def private cr_path_concave_shape(var dl : ImDrawList?; x : float; y : float; sz : float) {
    var pts <- [
        float2(0.0f, 0.0f), float2(0.5f, 0.4f), float2(0.5f, 0.0f),
        float2(1.0f, 0.0f), float2(0.6f, 0.6f), float2(1.0f, 1.0f),
        float2(0.5f, 1.0f), float2(0.5f, 0.6f), float2(0.0f, 1.0f)
    ]
    for (p in pts) {
        dl |> path_line_to(float2(x + p.x * sz, y + p.y * sz))
    }
    delete pts
}

// Pack ImVec4 (sRGB float 0..1) into the packed ABGR uint ImGui draw API
// expects. Matches ImColor(ImVec4) on the C++ side (no style-alpha multiply).
def private cr_pack_color(c : float4) : uint {
    let r = uint(clamp(c.x * 255.0f, 0.0f, 255.0f))
    let g = uint(clamp(c.y * 255.0f, 0.0f, 255.0f))
    let b = uint(clamp(c.z * 255.0f, 0.0f, 255.0f))
    let a = uint(clamp(c.w * 255.0f, 0.0f, 255.0f))
    return rgba(r, g, b, a)
}

// ===== Primitives tab =====

def private show_primitives_tab() {
    with_item_width(-GetFontSize() * 15.0f) {
        // Gradient strips. GetColorU32(rgba(...)) matches C++ GetColorU32(IM_COL32(...)) — both fold in style alpha.
        text(CR_GRAD_LABEL, (text = "Gradients"))
        let gradient_sz = float2(CalcItemWidth(), GetFrameHeight())
        {
            let p0 = GetCursorScreenPos()
            let p1 = float2(p0.x + gradient_sz.x, p0.y + gradient_sz.y)
            let col_a = GetColorU32(rgba(0u, 0u, 0u, 255u))
            let col_b = GetColorU32(rgba(255u, 255u, 255u, 255u))
            with_window_drawlist() $(var dl) {
                dl |> add_rect_filled_multi_color(p0, p1, col_a, col_b, col_b, col_a)
            }
            invisible_button(CR_GRAD1, (text = "##gradient1", size = gradient_sz))
        }
        {
            let p0 = GetCursorScreenPos()
            let p1 = float2(p0.x + gradient_sz.x, p0.y + gradient_sz.y)
            let col_a = GetColorU32(rgba(0u, 255u, 0u, 255u))
            let col_b = GetColorU32(rgba(255u, 0u, 0u, 255u))
            with_window_drawlist() $(var dl) {
                dl |> add_rect_filled_multi_color(p0, p1, col_a, col_b, col_b, col_a)
            }
            invisible_button(CR_GRAD2, (text = "##gradient2", size = gradient_sz))
        }

        // "All primitives" controls.
        text(CR_ALL_PRIMS_LABEL, (text = "All primitives"))
        edit_drag_float(safe_addr(CR_SZ),
            (id = "CR_DRAG_SZ", text = "Size",
             v_speed = 0.2f, v_min = 2.0f, v_max = 100.0f, format = "%.0f"))
        edit_drag_float(safe_addr(CR_THICKNESS),
            (id = "CR_DRAG_TH", text = "Thickness",
             v_speed = 0.05f, v_min = 1.0f, v_max = 8.0f, format = "%.02f"))
        edit_slider_int(safe_addr(CR_NGON_SIDES),
            (id = "CR_SLIDER_NGON", text = "N-gon sides", v_min = 3, v_max = 12))
        edit_checkbox(safe_addr(CR_CIRCLE_SEG_OVERRIDE),
            (id = "CR_CB_CIRCLE_OVR", text = "##circlesegmentoverride"))
        same_line()
        edit_slider_int(safe_addr(CR_CIRCLE_SEG_OVERRIDE_V),
            (id = "CR_SLIDER_CIRCLE_OVR", text = "Circle segments override",
             v_min = 3, v_max = 40))
        edit_checkbox(safe_addr(CR_CURVE_SEG_OVERRIDE),
            (id = "CR_CB_CURVE_OVR", text = "##curvessegmentoverride"))
        same_line()
        edit_slider_int(safe_addr(CR_CURVE_SEG_OVERRIDE_V),
            (id = "CR_SLIDER_CURVE_OVR", text = "Curve segments override",
             v_min = 3, v_max = 40))
        edit_color_edit4(safe_addr(CR_COLOR), (id = "CR_COLOR_EDIT", text = "Color"))

        // Draw primitives row by row: two stroked rows (thickness=1, then user) + one filled.
        let p = GetCursorScreenPos()
        let col = cr_pack_color(CR_COLOR)
        let spacing = 10.0f
        let corners_tl_br = (ImDrawFlags.RoundCornersTopLeft |
                             ImDrawFlags.RoundCornersBottomRight)
        let rounding = CR_SZ / 5.0f
        let circle_segments = CR_CIRCLE_SEG_OVERRIDE ? CR_CIRCLE_SEG_OVERRIDE_V : 0
        let curve_segments = CR_CURVE_SEG_OVERRIDE ? CR_CURVE_SEG_OVERRIDE_V : 0
        let sz = CR_SZ
        var cp3 <- [
            float2(0.0f, sz * 0.6f),
            float2(sz * 0.5f, -sz * 0.4f),
            float2(sz, sz)
        ]
        var cp4 <- [
            float2(0.0f, 0.0f),
            float2(sz * 1.3f, sz * 0.3f),
            float2(sz - sz * 1.3f, sz - sz * 0.3f),
            float2(sz, sz)
        ]

        with_window_drawlist() $(var dl) {
            var x = p.x + 4.0f
            var y = p.y + 4.0f
            for (n in range(2)) {
                let th = (n == 0) ? 1.0f : CR_THICKNESS
                dl |> add_ngon(float2(x + sz * 0.5f, y + sz * 0.5f),
                               sz * 0.5f, col, CR_NGON_SIDES, th); x += sz + spacing
                dl |> add_circle(float2(x + sz * 0.5f, y + sz * 0.5f),
                                 sz * 0.5f, col, circle_segments, th); x += sz + spacing
                dl |> add_ellipse(float2(x + sz * 0.5f, y + sz * 0.5f),
                                  float2(sz * 0.5f, sz * 0.3f),
                                  col, -0.3f, circle_segments, th); x += sz + spacing
                dl |> add_rect(float2(x, y), float2(x + sz, y + sz),
                               col, 0.0f, ImDrawFlags.None, th); x += sz + spacing
                dl |> add_rect(float2(x, y), float2(x + sz, y + sz),
                               col, rounding, ImDrawFlags.None, th); x += sz + spacing
                dl |> add_rect(float2(x, y), float2(x + sz, y + sz),
                               col, rounding, corners_tl_br, th); x += sz + spacing
                dl |> add_triangle(float2(x + sz * 0.5f, y),
                                   float2(x + sz, y + sz - 0.5f),
                                   float2(x, y + sz - 0.5f),
                                   col, th); x += sz + spacing
                cr_path_concave_shape(dl, x, y, sz)
                dl |> path_stroke(col, ImDrawFlags.Closed, th); x += sz + spacing
                dl |> add_line(float2(x, y), float2(x + sz, y), col, th); x += sz + spacing
                dl |> add_line(float2(x, y), float2(x, y + sz), col, th); x += spacing
                dl |> add_line(float2(x, y), float2(x + sz, y + sz), col, th); x += sz + spacing
                dl |> path_arc_to(float2(x + sz * 0.5f, y + sz * 0.5f),
                                  sz * 0.5f, PI, PI * -0.5f, 0)
                dl |> path_stroke(col, ImDrawFlags.None, th); x += sz + spacing
                dl |> add_bezier_quadratic(
                    float2(x + cp3[0].x, y + cp3[0].y),
                    float2(x + cp3[1].x, y + cp3[1].y),
                    float2(x + cp3[2].x, y + cp3[2].y),
                    col, th, curve_segments); x += sz + spacing
                dl |> add_bezier_cubic(
                    float2(x + cp4[0].x, y + cp4[0].y),
                    float2(x + cp4[1].x, y + cp4[1].y),
                    float2(x + cp4[2].x, y + cp4[2].y),
                    float2(x + cp4[3].x, y + cp4[3].y),
                    col, th, curve_segments)
                x = p.x + 4.0f
                y += sz + spacing
            }
            // Filled row.
            dl |> add_ngon_filled(float2(x + sz * 0.5f, y + sz * 0.5f),
                                  sz * 0.5f, col, CR_NGON_SIDES); x += sz + spacing
            dl |> add_circle_filled(float2(x + sz * 0.5f, y + sz * 0.5f),
                                    sz * 0.5f, col, circle_segments); x += sz + spacing
            dl |> add_ellipse_filled(float2(x + sz * 0.5f, y + sz * 0.5f),
                                     float2(sz * 0.5f, sz * 0.3f),
                                     col, -0.3f, circle_segments); x += sz + spacing
            dl |> add_rect_filled(float2(x, y), float2(x + sz, y + sz), col); x += sz + spacing
            dl |> add_rect_filled(float2(x, y), float2(x + sz, y + sz),
                                  col, 10.0f, ImDrawFlags.None); x += sz + spacing
            dl |> add_rect_filled(float2(x, y), float2(x + sz, y + sz),
                                  col, 10.0f, corners_tl_br); x += sz + spacing
            dl |> add_triangle_filled(float2(x + sz * 0.5f, y),
                                      float2(x + sz, y + sz - 0.5f),
                                      float2(x, y + sz - 0.5f), col); x += sz + spacing
            cr_path_concave_shape(dl, x, y, sz)
            dl |> path_fill_concave(col); x += sz + spacing
            dl |> add_rect_filled(float2(x, y), float2(x + sz, y + CR_THICKNESS), col); x += sz + spacing
            dl |> add_rect_filled(float2(x, y), float2(x + CR_THICKNESS, y + sz), col); x += spacing * 2.0f
            dl |> add_rect_filled(float2(x, y), float2(x + 1.0f, y + 1.0f), col); x += sz
            dl |> path_arc_to(float2(x + sz * 0.5f, y + sz * 0.5f),
                              sz * 0.5f, PI * -0.5f, PI, 0)
            dl |> path_fill_convex(col); x += sz + spacing
            dl |> path_line_to(float2(x + cp3[0].x, y + cp3[0].y))
            dl |> path_bezier_quadratic_curve_to(
                float2(x + cp3[1].x, y + cp3[1].y),
                float2(x + cp3[2].x, y + cp3[2].y),
                curve_segments)
            dl |> path_fill_convex(col); x += sz + spacing
            dl |> add_rect_filled_multi_color(float2(x, y), float2(x + sz, y + sz),
                rgba(0u, 0u, 0u, 255u),
                rgba(255u, 0u, 0u, 255u),
                rgba(255u, 255u, 0u, 255u),
                rgba(0u, 255u, 0u, 255u))
            x += sz + spacing
        }
        dummy(CR_PRIM_PAD, (size = float2((sz + spacing) * 13.2f,
                                           (sz + spacing) * 3.0f)))
        delete cp3
        delete cp4
    }
}

// ===== Canvas tab =====

def private show_canvas_tab() {
    edit_checkbox(safe_addr(CR_CANVAS_OPT_GRID),
        (id = "CR_CV_GRID", text = "Enable grid"))
    edit_checkbox(safe_addr(CR_CANVAS_OPT_CTX),
        (id = "CR_CV_CTX", text = "Enable context menu"))
    text(CR_CV_HINT, (text = "Mouse Left: drag to add lines, Mouse Right: drag to scroll, click for context menu."))

    let canvas_p0 = GetCursorScreenPos()
    var canvas_sz = GetContentRegionAvail()
    canvas_sz.x = max(canvas_sz.x, 50.0f)
    canvas_sz.y = max(canvas_sz.y, 50.0f)
    let canvas_p1 = float2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y)

    with_window_drawlist() $(var dl) {
        dl |> add_rect_filled(canvas_p0, canvas_p1, rgba(50u, 50u, 50u, 255u))
        dl |> add_rect(canvas_p0, canvas_p1, rgba(255u, 255u, 255u, 255u))
    }

    invisible_button(CR_CV_CANVAS,
        (text = "canvas", size = canvas_sz,
         flags = ImGuiButtonFlags.MouseButtonLeft |
                 ImGuiButtonFlags.MouseButtonRight))
    let is_hovered = IsItemHovered(ImGuiHoveredFlags.None)
    let is_active = IsItemActive()
    let origin = float2(canvas_p0.x + CR_CANVAS_SCROLLING.x,
                        canvas_p0.y + CR_CANVAS_SCROLLING.y)
    let mouse_pos = GetMousePos()
    let mouse_in_canvas = float2(mouse_pos.x - origin.x, mouse_pos.y - origin.y)

    if (is_hovered && !CR_CANVAS_ADDING_LINE && IsMouseClicked(ImGuiMouseButton.Left, false)) {
        CR_CANVAS_POINTS |> push(mouse_in_canvas)
        CR_CANVAS_POINTS |> push(mouse_in_canvas)
        CR_CANVAS_ADDING_LINE = true
    }
    if (CR_CANVAS_ADDING_LINE) {
        let n = length(CR_CANVAS_POINTS)
        if (n > 0) {
            CR_CANVAS_POINTS[n - 1] = mouse_in_canvas
        }
        if (!IsMouseDown(ImGuiMouseButton.Left)) {
            CR_CANVAS_ADDING_LINE = false
        }
    }

    let pan_threshold = CR_CANVAS_OPT_CTX ? -1.0f : 0.0f
    let io & = unsafe(GetIO())
    if (is_active && IsMouseDragging(ImGuiMouseButton.Right, pan_threshold)) {
        CR_CANVAS_SCROLLING.x += io.MouseDelta.x
        CR_CANVAS_SCROLLING.y += io.MouseDelta.y
    }

    let drag_delta = GetMouseDragDelta(ImGuiMouseButton.Right, -1.0f)
    // Use the STATEFUL state-arg overload — the string-id form's GetID("context") won't match the container's PushID(IDENT) popup ID, so the popup would never open.
    if (CR_CANVAS_OPT_CTX && drag_delta.x == 0.0f && drag_delta.y == 0.0f) {
        open_popup_on_item_click(CR_CV_CTX_MENU, ImGuiPopupFlags.MouseButtonRight)
    }
    popup(CR_CV_CTX_MENU,
          (text = "context", flags = ImGuiWindowFlags.None)) {
        if (CR_CANVAS_ADDING_LINE) {
            let n = length(CR_CANVAS_POINTS)
            if (n >= 2) {
                CR_CANVAS_POINTS |> resize(n - 2)
            }
            CR_CANVAS_ADDING_LINE = false
        }
        let n = length(CR_CANVAS_POINTS)
        if (menu_item(CR_CV_REMOVE_ONE,
                      (text = "Remove one", shortcut = "", enabled = n > 0))) {
            CR_CANVAS_POINTS |> resize(n - 2)
        }
        if (menu_item(CR_CV_REMOVE_ALL,
                      (text = "Remove all", shortcut = "", enabled = n > 0))) {
            CR_CANVAS_POINTS |> clear()
        }
    }

    with_clip_rect(canvas_p0, canvas_p1, true) {
        with_window_drawlist() $(var dl) {
            if (CR_CANVAS_OPT_GRID) {
                let GRID_STEP = 64.0f
                let grid_col = rgba(200u, 200u, 200u, 40u)
                var gx = fmod(CR_CANVAS_SCROLLING.x, GRID_STEP)
                while (gx < canvas_sz.x) {
                    dl |> add_line(float2(canvas_p0.x + gx, canvas_p0.y),
                                   float2(canvas_p0.x + gx, canvas_p1.y),
                                   grid_col, 1.0f)
                    gx += GRID_STEP
                }
                var gy = fmod(CR_CANVAS_SCROLLING.y, GRID_STEP)
                while (gy < canvas_sz.y) {
                    dl |> add_line(float2(canvas_p0.x, canvas_p0.y + gy),
                                   float2(canvas_p1.x, canvas_p0.y + gy),
                                   grid_col, 1.0f)
                    gy += GRID_STEP
                }
            }
            let line_col = rgba(255u, 255u, 0u, 255u)
            let n = length(CR_CANVAS_POINTS)
            var i = 0
            while (i + 1 < n) {
                let a = CR_CANVAS_POINTS[i]
                let b = CR_CANVAS_POINTS[i + 1]
                dl |> add_line(float2(origin.x + a.x, origin.y + a.y),
                               float2(origin.x + b.x, origin.y + b.y),
                               line_col, 2.0f)
                i += 2
            }
        }
    }
}

// ===== BG/FG draw lists tab =====

def private show_bgfg_tab() {
    edit_checkbox(safe_addr(CR_DRAW_BG),
        (id = "CR_BG_TOGGLE", text = "Draw in Background draw list"))
    edit_checkbox(safe_addr(CR_DRAW_FG),
        (id = "CR_FG_TOGGLE", text = "Draw in Foreground draw list"))
    let win_pos = GetWindowPos()
    let win_sz = GetWindowSize()
    let center = float2(win_pos.x + win_sz.x * 0.5f, win_pos.y + win_sz.y * 0.5f)
    if (CR_DRAW_BG) {
        with_background_drawlist() $(var dl) {
            dl |> add_circle(center, win_sz.x * 0.6f,
                             rgba(255u, 0u, 0u, 200u), 0, 14.0f)
        }
    }
    if (CR_DRAW_FG) {
        with_foreground_drawlist() $(var dl) {
            dl |> add_circle(center, win_sz.y * 0.6f,
                             rgba(0u, 255u, 0u, 200u), 0, 10.0f)
        }
    }
}

// ===== Draw Channels tab =====

def private show_channels_tab() {
    text(CR_CH_NAIVE_1, (text = "Blue shape is drawn first: appears in back"))
    text(CR_CH_NAIVE_2, (text = "Red shape is drawn after: appears in front"))
    let p0 = GetCursorScreenPos()
    with_window_drawlist() $(var dl) {
        dl |> add_rect_filled(float2(p0.x, p0.y), float2(p0.x + 50.0f, p0.y + 50.0f),
                              rgba(0u, 0u, 255u, 255u))
        dl |> add_rect_filled(float2(p0.x + 25.0f, p0.y + 25.0f),
                              float2(p0.x + 75.0f, p0.y + 75.0f),
                              rgba(255u, 0u, 0u, 255u))
    }
    dummy(CR_CH_NAIVE_PAD, (size = float2(75.0f, 75.0f)))
    separator(CR_CH_SEP)
    text(CR_CH_SPLIT_1, (text = "Blue shape is drawn first, into channel 1: appears in front"))
    text(CR_CH_SPLIT_2, (text = "Red shape is drawn after, into channel 0: appears in back"))
    let p1 = GetCursorScreenPos()
    with_window_drawlist() $(var dl) {
        dl |> channels_split(2)
        dl |> channels_set_current(1)
        dl |> add_rect_filled(float2(p1.x, p1.y), float2(p1.x + 50.0f, p1.y + 50.0f),
                              rgba(0u, 0u, 255u, 255u))
        dl |> channels_set_current(0)
        dl |> add_rect_filled(float2(p1.x + 25.0f, p1.y + 25.0f),
                              float2(p1.x + 75.0f, p1.y + 75.0f),
                              rgba(255u, 0u, 0u, 255u))
        dl |> channels_merge()
    }
    dummy(CR_CH_SPLIT_PAD, (size = float2(75.0f, 75.0f)))
    text(CR_CH_SPLIT_NOTE, (text = "After reordering, contents of channel 0 appear below channel 1."))
}

// ===== Entry point =====

def public ShowExampleAppCustomRendering() : bool {
    // X-button closes by setting CR_WIN.open=false; the aggregator then
    // clears its SHOW flag. When the user re-checks the demo checkbox,
    // re-enter with stale open=false — reset so the window renders again.
    CR_WIN.open = true
    window(CR_WIN, (text = "Example: Custom rendering", closable = true,
                    flags = ImGuiWindowFlags.None)) {
        tab_bar(CR_TABS, (text = "##TabBar", flags = ImGuiTabBarFlags.None)) {
            tab_item(CR_TAB_PRIM, (text = "Primitives", closable = false,
                                     flags = ImGuiTabItemFlags.None)) {
                show_primitives_tab()
            }
            tab_item(CR_TAB_CANVAS, (text = "Canvas", closable = false,
                                     flags = ImGuiTabItemFlags.None)) {
                show_canvas_tab()
            }
            tab_item(CR_TAB_BGFG, (text = "BG/FG draw lists", closable = false,
                                     flags = ImGuiTabItemFlags.None)) {
                show_bgfg_tab()
            }
            tab_item(CR_TAB_CHANNELS, (text = "Draw Channels", closable = false,
                                       flags = ImGuiTabItemFlags.None)) {
                show_channels_tab()
            }
        }
    }
    return CR_WIN.open
}
