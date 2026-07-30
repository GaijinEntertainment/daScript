options gen2
options _comment_hygiene = true
options indenting = 4

module style_editor

require imgui
require imgui/imgui_widgets_builtin
require imgui/imgui_containers_builtin
require imgui/imgui_scope_builtin
require imgui/imgui_id_builtin
require imgui/imgui_layout_builtin       // group + same_line for Circle Tess samples
require imgui/imgui_drawlist_builtin     // add_circle / with_window_drawlist for samples
require daslib/enum_trait
require math                              // max / floor for Circle Tess preview

// Indexed widget tables for the per-ImGuiCol loop in the Colors tab.
// `[widget]` macros don't auto-emit table globals — indexed forms require
// the table declared at module scope.
var private STYLE_COLOR_SAMELINE : table<int; EmptyMarkerState>
var private STYLE_COLOR_NAME : table<int; NarrativeState>

// Circle Tessellation tooltip samples — 8 BeginGroup'd preview blocks shown
// while the user is dragging Circle Tessellation Max Error (cpp:6919-6945).
var private STYLE_CIRCLE_GROUP : table<int; GroupState>
var private STYLE_CIRCLE_SL : table<int; EmptyMarkerState>
var private STYLE_CIRCLE_TEXT : table<int; NarrativeState>

// Mirrors imgui_demo.cpp:6680-6962 — ShowStyleEditor(ImGuiStyle* ref).
// Caller wraps this in Begin/End; sliders edit ImGui's live style via edit_* (safe_addr rejects struct fields, so unsafe(addr(style.X))).
// Skipped vs C++: HoverFlags CheckboxFlags, Save/Revert Ref, Colors export/filter/alpha radios, Fonts ShowFontAtlas — all need bounce-buffer/binding work.
def ShowStyleEditor() {
    var style & = unsafe(GetStyle())

    with_item_width(GetWindowWidth() * 0.50f) {
        // Built-in style/font pickers.
        if (ShowStyleSelector("Colors##Selector")) {}
        ShowFontSelector("Fonts##Selector")

        // "Simplified Settings": one float slider editing style.FrameRounding.
        if (edit_slider_float(unsafe(addr(style.FrameRounding)),
                              (id = "STYLE_FRAME_ROUNDING",
                               text = "FrameRounding",
                               v_min = 0.0f, v_max = 12.0f,
                               format = "%.0f"))) {
            // C++ keeps GrabRounding == FrameRounding for visual consistency.
            style.GrabRounding = style.FrameRounding
        }

        separator()

        tab_bar(STYLE_TABS, (text = "##tabs", flags = ImGuiTabBarFlags.None)) {
            tab_item(STYLE_TAB_SIZES, (text = "Sizes", closable = false,
                                       flags = ImGuiTabItemFlags.None)) {
                separator_text("Main")
                edit_slider_float2(unsafe(addr(style.WindowPadding)),
                                   (id = "STYLE_WINDOW_PADDING",
                                    text = "WindowPadding",
                                    v_min = 0.0f, v_max = 20.0f, format = "%.0f"))
                edit_slider_float2(unsafe(addr(style.FramePadding)),
                                   (id = "STYLE_FRAME_PADDING",
                                    text = "FramePadding",
                                    v_min = 0.0f, v_max = 20.0f, format = "%.0f"))
                edit_slider_float2(unsafe(addr(style.ItemSpacing)),
                                   (id = "STYLE_ITEM_SPACING",
                                    text = "ItemSpacing",
                                    v_min = 0.0f, v_max = 20.0f, format = "%.0f"))
                edit_slider_float2(unsafe(addr(style.ItemInnerSpacing)),
                                   (id = "STYLE_ITEM_INNER_SPACING",
                                    text = "ItemInnerSpacing",
                                    v_min = 0.0f, v_max = 20.0f, format = "%.0f"))
                edit_slider_float2(unsafe(addr(style.TouchExtraPadding)),
                                   (id = "STYLE_TOUCH_EXTRA_PADDING",
                                    text = "TouchExtraPadding",
                                    v_min = 0.0f, v_max = 10.0f, format = "%.0f"))
                edit_slider_float(unsafe(addr(style.IndentSpacing)),
                                  (id = "STYLE_INDENT_SPACING",
                                   text = "IndentSpacing",
                                   v_min = 0.0f, v_max = 30.0f, format = "%.0f"))
                edit_slider_float(unsafe(addr(style.ScrollbarSize)),
                                  (id = "STYLE_SCROLLBAR_SIZE",
                                   text = "ScrollbarSize",
                                   v_min = 1.0f, v_max = 20.0f, format = "%.0f"))
                edit_slider_float(unsafe(addr(style.GrabMinSize)),
                                  (id = "STYLE_GRAB_MIN_SIZE",
                                   text = "GrabMinSize",
                                   v_min = 1.0f, v_max = 20.0f, format = "%.0f"))

                separator_text("Borders")
                edit_slider_float(unsafe(addr(style.WindowBorderSize)),
                                  (id = "STYLE_WINDOW_BORDER_SIZE",
                                   text = "WindowBorderSize",
                                   v_min = 0.0f, v_max = 1.0f, format = "%.0f"))
                edit_slider_float(unsafe(addr(style.ChildBorderSize)),
                                  (id = "STYLE_CHILD_BORDER_SIZE",
                                   text = "ChildBorderSize",
                                   v_min = 0.0f, v_max = 1.0f, format = "%.0f"))
                edit_slider_float(unsafe(addr(style.PopupBorderSize)),
                                  (id = "STYLE_POPUP_BORDER_SIZE",
                                   text = "PopupBorderSize",
                                   v_min = 0.0f, v_max = 1.0f, format = "%.0f"))
                edit_slider_float(unsafe(addr(style.FrameBorderSize)),
                                  (id = "STYLE_FRAME_BORDER_SIZE",
                                   text = "FrameBorderSize",
                                   v_min = 0.0f, v_max = 1.0f, format = "%.0f"))
                edit_slider_float(unsafe(addr(style.TabBorderSize)),
                                  (id = "STYLE_TAB_BORDER_SIZE",
                                   text = "TabBorderSize",
                                   v_min = 0.0f, v_max = 1.0f, format = "%.0f"))
                edit_slider_float(unsafe(addr(style.TabBarBorderSize)),
                                  (id = "STYLE_TAB_BAR_BORDER_SIZE",
                                   text = "TabBarBorderSize",
                                   v_min = 0.0f, v_max = 2.0f, format = "%.0f"))

                separator_text("Rounding")
                edit_slider_float(unsafe(addr(style.WindowRounding)),
                                  (id = "STYLE_WINDOW_ROUNDING",
                                   text = "WindowRounding",
                                   v_min = 0.0f, v_max = 12.0f, format = "%.0f"))
                edit_slider_float(unsafe(addr(style.ChildRounding)),
                                  (id = "STYLE_CHILD_ROUNDING",
                                   text = "ChildRounding",
                                   v_min = 0.0f, v_max = 12.0f, format = "%.0f"))
                edit_slider_float(unsafe(addr(style.PopupRounding)),
                                  (id = "STYLE_POPUP_ROUNDING",
                                   text = "PopupRounding",
                                   v_min = 0.0f, v_max = 12.0f, format = "%.0f"))
                edit_slider_float(unsafe(addr(style.ScrollbarRounding)),
                                  (id = "STYLE_SCROLLBAR_ROUNDING",
                                   text = "ScrollbarRounding",
                                   v_min = 0.0f, v_max = 12.0f, format = "%.0f"))
                edit_slider_float(unsafe(addr(style.GrabRounding)),
                                  (id = "STYLE_GRAB_ROUNDING",
                                   text = "GrabRounding",
                                   v_min = 0.0f, v_max = 12.0f, format = "%.0f"))
                edit_slider_float(unsafe(addr(style.TabRounding)),
                                  (id = "STYLE_TAB_ROUNDING",
                                   text = "TabRounding",
                                   v_min = 0.0f, v_max = 12.0f, format = "%.0f"))

                separator_text("Misc")
                edit_slider_float(unsafe(addr(style.SeparatorTextBorderSize)),
                                  (id = "STYLE_SEPARATOR_TEXT_BORDER_SIZE",
                                   text = "SeparatorTextBorderSize",
                                   v_min = 0.0f, v_max = 10.0f, format = "%.0f"))
                edit_slider_float(unsafe(addr(style.LogSliderDeadzone)),
                                  (id = "STYLE_LOG_SLIDER_DEADZONE",
                                   text = "LogSliderDeadzone",
                                   v_min = 0.0f, v_max = 12.0f, format = "%.0f"))

                separator_text("Docking")
                edit_slider_float(unsafe(addr(style.DockingSeparatorSize)),
                                  (id = "STYLE_DOCKING_SPLITTER_SIZE",
                                   text = "DockingSplitterSize",
                                   v_min = 0.0f, v_max = 12.0f, format = "%.0f"))
            }

            tab_item(STYLE_TAB_COLORS, (text = "Colors", closable = false,
                                        flags = ImGuiTabItemFlags.None)) {
                // Stripped-down color editor: one ColorEdit4 per ImGuiCol entry.
                // Skipped vs C++: filter, export-to-clipboard, alpha-mode radios, per-color Save/Revert (see file-level doc).
                child(STYLE_COLORS_CHILD,
                      (text = "##colors", size = float2(0.0f, 0.0f),
                       child_flags = ImGuiChildFlags.Borders,
                       window_flags = ImGuiWindowFlags.AlwaysVerticalScrollbar)) {
                    with_item_width(GetFontSize() * -12.0f) {
                        for (col in type<ImGuiCol>) {
                            if (col == ImGuiCol.COUNT) continue
                            let name = GetStyleColorName(col)
                            with_id("row_{int(col)}") {
                                edit_color_edit4(
                                    unsafe(addr(style.Colors[int(col)])),
                                    (id = "COLOR", text = "##color",
                                     flags = ImGuiColorEditFlags.AlphaBar))
                                same_line(STYLE_COLOR_SAMELINE[int(col)],
                                          (spacing = style.ItemInnerSpacing.x))
                                text(STYLE_COLOR_NAME[int(col)], (text = name))
                            }
                        }
                    }
                }
            }

            tab_item(STYLE_TAB_FONTS, (text = "Fonts", closable = false,
                                       flags = ImGuiTabItemFlags.None)) {
                // ShowFontAtlas (cpp:6872) skipped — debug helper.
                // style.FontScaleMain is the global font scale.
                edit_drag_float(unsafe(addr(style.FontScaleMain)),
                                (id = "STYLE_FONT_GLOBAL_SCALE",
                                 text = "global scale",
                                 speed = 0.005f, v_min = 0.3f, v_max = 2.0f,
                                 format = "%.2f",
                                 flags = ImGuiSliderFlags.AlwaysClamp))
            }

            tab_item(STYLE_TAB_RENDERING, (text = "Rendering", closable = false,
                                           flags = ImGuiTabItemFlags.None)) {
                edit_checkbox(unsafe(addr(style.AntiAliasedLines)),
                              (id = "STYLE_AA_LINES",
                               text = "Anti-aliased lines"))
                edit_checkbox(unsafe(addr(style.AntiAliasedLinesUseTex)),
                              (id = "STYLE_AA_LINES_USE_TEX",
                               text = "Anti-aliased lines use texture"))
                edit_checkbox(unsafe(addr(style.AntiAliasedFill)),
                              (id = "STYLE_AA_FILL",
                               text = "Anti-aliased fill"))
                edit_drag_float(unsafe(addr(style.CurveTessellationTol)),
                                (id = "STYLE_CURVE_TESS_TOL",
                                 text = "Curve Tessellation Tolerance",
                                 speed = 0.02f, v_min = 0.10f, v_max = 10.0f,
                                 format = "%.2f"))
                if (style.CurveTessellationTol < 0.10f) {
                    style.CurveTessellationTol = 0.10f
                }
                edit_drag_float(unsafe(addr(style.CircleTessellationMaxError)),
                                (id = "STYLE_CIRCLE_TESS_MAX_ERROR",
                                 text = "Circle Tessellation Max Error",
                                 speed = 0.005f, v_min = 0.10f, v_max = 5.0f,
                                 format = "%.2f",
                                 flags = ImGuiSliderFlags.AlwaysClamp))
                // While the user is dragging Max Error, preview the
                // auto-segment count at 8 radii from 5..70 (cpp:6911-6947).
                let show_samples = IsItemActive()
                if (show_samples) {
                    SetNextWindowPos(GetCursorScreenPos())
                    tooltip(STYLE_CIRCLE_TESS_TOOLTIP) {
                        text_unformatted("(R = radius, N = number of segments)")
                        spacing()
                        let min_widget_width = CalcTextSize("N: MMM\nR: MMM").x
                        let RAD_MIN = 5.0f
                        let RAD_MAX = 70.0f
                        with_window_drawlist() $(var dl) {
                            for (n in range(8)) {
                                let rad = (RAD_MIN +
                                           (RAD_MAX - RAD_MIN) * float(n) / 7.0f)
                                let seg_count = *dl |> _CalcCircleAutoSegmentCount(rad)
                                group(STYLE_CIRCLE_GROUP[n]) {
                                    text(STYLE_CIRCLE_TEXT[n],
                                         (text = "R: {fmt(":.0f", rad)}\nN: {seg_count}"))
                                    let canvas_width = max(min_widget_width,
                                                           rad * 2.0f)
                                    let offset_x = floor(canvas_width * 0.5f)
                                    let offset_y = floor(RAD_MAX)
                                    let p1 = GetCursorScreenPos()
                                    dl |> add_circle(
                                        float2(p1.x + offset_x, p1.y + offset_y),
                                        rad, GetColorU32(ImGuiCol.Text))
                                    dummy((size = float2(canvas_width,
                                                         RAD_MAX * 2.0f)))
                                }
                                same_line(STYLE_CIRCLE_SL[n])
                            }
                        }
                    }
                }
                edit_drag_float(unsafe(addr(style.Alpha)),
                                (id = "STYLE_ALPHA",
                                 text = "Global Alpha",
                                 speed = 0.005f, v_min = 0.20f, v_max = 1.0f,
                                 format = "%.2f"))
                edit_drag_float(unsafe(addr(style.DisabledAlpha)),
                                (id = "STYLE_DISABLED_ALPHA",
                                 text = "Disabled Alpha",
                                 speed = 0.005f, v_min = 0.0f, v_max = 1.0f,
                                 format = "%.2f"))
            }
        }
    }
}
