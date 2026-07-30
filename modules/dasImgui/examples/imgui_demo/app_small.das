options gen2
options _comment_hygiene = true
options indenting = 4

module app_small

require imgui
require imgui/imgui_widgets_builtin
require imgui/imgui_containers_builtin
require imgui/imgui_id_builtin
require imgui/imgui_scope_builtin
require imgui/imgui_layout_builtin
require imgui/imgui_style_builtin
require imgui/imgui_table_builtin
require imgui/imgui_window_constraints_builtin
require daslib/safe_addr
require daslib/strings_boost
require math
require strings

// =============================================================================
// Bundles eight small example apps from imgui_demo.cpp:7644-8121. Each mirrors
// one `static void ShowExampleApp…(bool* p_open)` in the original.
//
// Boost-surface conventions (same as widgets.das / layout.das):
//   - Window-family containers: `window(IDENT, (text=…, closable=…, flags=…))`.
//     `##`/`###` ID separators in the `text` string are preserved verbatim and
//     reach ImGui's Begin() unchanged (WindowTitles depends on this).
//   - Menu chrome: `menu_bar` + `menu` + `menu_item` containers.
//   - Tables: `data_table(IDENT, (text=…, columns=N, flags=…, outer_size=…,
//     inner_width=0.0f))` + body-internal `table_setup_*`/`table_next_*`/
//     `table_set_column_index` pass-throughs from `imgui_table_builtin`.
//     Named `data_table` (not `table`) because `table` is a daslang reserved
//     keyword (the `table<K;V>` type constructor).
//   - Size constraints: 2-arg `SetNextWindowSizeConstraints(min, max)` is raw
//     allow-listed; the callback overload uses `ImGuiSizeConstraints(@(...))`
//     from `imgui/imgui_window_constraints_builtin`.
//   - HelpMarker pattern: inlined `text_disabled("(?)") +
//     item_tooltip(IDENT) { with_text_wrap_pos { text_unformatted(…) } }`
//     (same as widgets.das / layout.das / inputs.das).
//   - IMGUI_DEMO_MARKER lines dropped (off-limits per CLAUDE.md).
//
// Per-app entry points return `bool` for closable apps: `true` = still open,
// `false` = the user clicked the window X. Dispatcher in `imgui_demo.das`
// observes the return value and clears the corresponding SHOW_APP_* toggle.
// WindowTitles (non-closable, 3 windows) returns `void`.
//
// Skipped vs C++ (cited at the occurrence):
//   - LongText case 0 uses single-string `text_unformatted(string(buf.c_str()))`
//     in place of the two-pointer `TextUnformatted(begin, end)` overload —
//     the two-ptr form isn't on the boost surface; the perf delta at the
//     demo's 100K-line ceiling is invisible vs the actual glyph rasterization.
//   - AutoResize substitutes printf `"%*sThis is line %d"` with
//     `"{strrep(\" \", i*4)}This is line {i}"` (daslang has no printf).
// =============================================================================

// =============================================================================
// Simple Layout — cpp:7644-7704
// =============================================================================

var SMALL_LAYOUT_LEFT_SEL : table<int; ToggleState>
var private SMALL_LAYOUT_SELECTED = 0

def ShowExampleAppLayout() : bool {
    SetNextWindowSize(float2(500.0f, 440.0f), ImGuiCond.FirstUseEver)
    window(SMALL_LAYOUT_WIN, (text = "Example: Simple layout", closable = true,
                              flags = ImGuiWindowFlags.MenuBar)) {
        menu_bar(SMALL_LAYOUT_MENU_BAR) {
            menu(SMALL_LAYOUT_MENU_FILE, (text = "File", enabled = true)) {
                if (menu_item(SMALL_LAYOUT_MI_CLOSE, (text = "Close", shortcut = "Ctrl+W"))) {
                    SMALL_LAYOUT_WIN.open = false
                }
            }
        }
        // Left pane — resizable child with 100 selectable rows.
        let left_flags = int(ImGuiChildFlags.Borders | ImGuiChildFlags.ResizeX)
        child(SMALL_LAYOUT_LEFT_CHILD,
              (text = "left pane", size = float2(150.0f, 0.0f),
               child_flags = unsafe(reinterpret<ImGuiChildFlags>(left_flags)),
               window_flags = ImGuiWindowFlags.None)) {
            for (i in range(100)) {
                if (selectable(SMALL_LAYOUT_LEFT_SEL[i], (text = "MyObject {i}"))) {
                    SMALL_LAYOUT_SELECTED = i
                }
                // ToggleState mirror — selected row stays toggled, others off.
                SMALL_LAYOUT_LEFT_SEL[i].value = (i == SMALL_LAYOUT_SELECTED)
            }
        }
        same_line(SMALL_LAYOUT_SL_PANES)
        // Right pane — group bracketing a header + tab-bar + button row.
        group(SMALL_LAYOUT_RIGHT_GROUP) {
            // Reserve one frame-height-with-spacing for the button row below.
            child(SMALL_LAYOUT_RIGHT_CHILD,
                  (text = "item view",
                   size = float2(0.0f, -GetFrameHeightWithSpacing()),
                   child_flags = ImGuiChildFlags.None,
                   window_flags = ImGuiWindowFlags.None)) {
                text(SMALL_LAYOUT_SEL_TEXT, (text = "MyObject: {SMALL_LAYOUT_SELECTED}"))
                separator(SMALL_LAYOUT_SEP1)
                tab_bar(SMALL_LAYOUT_TAB_BAR, (text = "##Tabs",
                                                flags = ImGuiTabBarFlags.None)) {
                    tab_item(SMALL_LAYOUT_TAB_DESC, (text = "Description",
                                                      closable = false,
                                                      flags = ImGuiTabItemFlags.None)) {
                        text_wrapped(SMALL_LAYOUT_DESC_TEXT,
                            (text = "Lorem ipsum dolor sit amet, consectetur " +
                                    "adipiscing elit, sed do eiusmod tempor " +
                                    "incididunt ut labore et dolore magna aliqua. "))
                    }
                    tab_item(SMALL_LAYOUT_TAB_DETAILS, (text = "Details",
                                                        closable = false,
                                                        flags = ImGuiTabItemFlags.None)) {
                        text(SMALL_LAYOUT_DETAILS_TEXT, (text = "ID: 0123456789"))
                    }
                }
            }
            button(SMALL_LAYOUT_BTN_REVERT, (text = "Revert"))
            same_line(SMALL_LAYOUT_SL_BTNS)
            button(SMALL_LAYOUT_BTN_SAVE, (text = "Save"))
        }
    }
    if (!SMALL_LAYOUT_WIN.open) {
        SMALL_LAYOUT_WIN.open = true
        return false
    }
    return true
}

// =============================================================================
// Property Editor — cpp:7759-7795 + helper cpp:7710-7755
// =============================================================================

// Recursive ShowPlaceholderObject reuses the SAME uid (424242) for child rows;
// ImGui's PushID(uid)+PushID(i) disambiguates. The port mirrors this with `with_id`
// scopes — tree-node state keys by container-path, not state-global identity.

// Static shared float[8] mirroring cpp's `static float placeholder_members[8]`.
// All ShowPlaceholderObject instances write to the same array — matches cpp.
var private SMALL_PROPED_PLACEHOLDER : array<float>
[init]
def proped_init() {
    SMALL_PROPED_PLACEHOLDER <- [0.0f, 0.0f, 1.0f, 3.1416f, 100.0f, 999.0f, 0.0f, 0.0f]
}

var SMALL_PROPED_NODE : table<int; TreeNodeState>
var SMALL_PROPED_FIELD : table<int; TreeNodeState>
var SMALL_PROPED_INPUT : table<int; InputStateFloat>
var SMALL_PROPED_DRAG : table<int; DragStateFloat>

def private ShowPlaceholderObject(prefix : string; uid : int) {
    with_id("uid_{uid}") {
        table_next_row()
        table_set_column_index(0)
        align_text_to_frame_padding(SMALL_PROPED_ATFP_OBJ)
        let node_open = tree_node_ex(SMALL_PROPED_NODE[uid],
            (text = "{prefix}_{uid}", flags = ImGuiTreeNodeFlags.SpanAllColumns))
        table_set_column_index(1)
        text(SMALL_PROPED_KIND_TEXT, (text = "my sailor is rich"))
        if (node_open) {
            for (i in range(8)) {
                with_id("field_{i}") {
                    if (i < 2) {
                        // Recursive child row — same prefix as cpp.
                        ShowPlaceholderObject("Child", 424242)
                    } else {
                        table_next_row()
                        table_set_column_index(0)
                        align_text_to_frame_padding(SMALL_PROPED_ATFP_FIELD)
                        let leaf_flags = int(ImGuiTreeNodeFlags.Leaf |
                                              ImGuiTreeNodeFlags.NoTreePushOnOpen |
                                              ImGuiTreeNodeFlags.Bullet)
                        tree_node_ex(SMALL_PROPED_FIELD[i],
                            (text = "Field_{i}",
                             flags = unsafe(reinterpret<ImGuiTreeNodeFlags>(leaf_flags))))
                        table_set_column_index(1)
                        SetNextItemWidth(-FLT_MIN)
                        if (i >= 5) {
                            input_float(SMALL_PROPED_INPUT[i],
                                (text = "##value", step = 1.0f))
                            SMALL_PROPED_PLACEHOLDER[i] = SMALL_PROPED_INPUT[i].value
                        } else {
                            drag_float(SMALL_PROPED_DRAG[i],
                                (text = "##value", speed = 0.01f))
                            SMALL_PROPED_PLACEHOLDER[i] = SMALL_PROPED_DRAG[i].value
                        }
                    }
                }
            }
            tree_pop()
        }
    }
}

def ShowExampleAppPropertyEditor() : bool {
    SetNextWindowSize(float2(430.0f, 450.0f), ImGuiCond.FirstUseEver)
    window(SMALL_PROPED_WIN, (text = "Example: Property editor", closable = true,
                              flags = ImGuiWindowFlags.None)) {
        // HelpMarker shim — text_disabled + item_tooltip wrap.
        text_disabled(SMALL_PROPED_HM_LABEL, (text = "(?)"))
        item_tooltip(SMALL_PROPED_HM) {
            with_text_wrap_pos(GetFontSize() * 35.0f) {
                text_unformatted(SMALL_PROPED_HM_TEXT,
                    (text = "This example shows how you may implement a " +
                            "property editor using two columns.\n" +
                            "All objects/fields data are dummies here."))
            }
        }
        with_style((ImGuiStyleVar.FramePadding, float2(2.0f, 2.0f))) {
            let tflags = int(ImGuiTableFlags.BordersOuter |
                              ImGuiTableFlags.Resizable |
                              ImGuiTableFlags.ScrollY)
            data_table(SMALL_PROPED_TABLE,
                       (text = "##split", columns = 2,
                        flags = unsafe(reinterpret<ImGuiTableFlags>(tflags)),
                        outer_size = float2(0.0f, 0.0f),
                        inner_width = 0.0f)) {
                table_setup_scroll_freeze(0, 1)
                table_setup_column("Object")
                table_setup_column("Contents")
                table_headers_row()
                for (obj_i in range(4)) {
                    ShowPlaceholderObject("Object", obj_i)
                }
            }
        }
    }
    if (!SMALL_PROPED_WIN.open) {
        SMALL_PROPED_WIN.open = true
        return false
    }
    return true
}

// =============================================================================
// Long Text — cpp:7796-7852
// =============================================================================

// Log buffer — cpp uses ImGuiTextBuffer but the daslang binding doesn't
// expose `appendf`; substitute `array<string>` (one line per entry). "bytes"
// reported in the header is the sum of line lengths.
var private SMALL_LONG_LINES_BUF : array<string>
var private SMALL_LONG_TEST_TYPE = 0

var SMALL_LONG_LINES_TEXT : table<int; NarrativeState>

def private long_byte_count() : int {
    var n = 0
    for (s in SMALL_LONG_LINES_BUF) {
        n += length(s)
    }
    return n
}

def ShowExampleAppLongText() : bool {
    SetNextWindowSize(float2(520.0f, 600.0f), ImGuiCond.FirstUseEver)
    window(SMALL_LONG_WIN, (text = "Example: Long text display", closable = true,
                            flags = ImGuiWindowFlags.None)) {
        text(SMALL_LONG_TEXT_HEAD, (text = "Printing unusually long amount of text."))
        // Combo for test type. items[] alters which strategy renders below.
        combo(SMALL_LONG_COMBO,
              (text = "Test type", init = SMALL_LONG_TEST_TYPE,
               items <- ["Single call to TextUnformatted()",
                         "Multiple calls to Text(), clipped",
                         "Multiple calls to Text(), not clipped (slow)"]))
        SMALL_LONG_TEST_TYPE = SMALL_LONG_COMBO.value
        let lines_n = length(SMALL_LONG_LINES_BUF)
        let bytes_n = long_byte_count()
        text(SMALL_LONG_TEXT_INFO,
            (text = "Buffer contents: {lines_n} lines, {bytes_n} bytes"))
        let do_clear = button(SMALL_LONG_BTN_CLEAR, (text = "Clear"))
        same_line(SMALL_LONG_SL_BUTTONS)
        let do_add = button(SMALL_LONG_BTN_ADD, (text = "Add 1000 lines"))
        if (do_clear) {
            SMALL_LONG_LINES_BUF |> clear()
        }
        if (do_add) {
            let base = lines_n
            for (i in range(1000)) {
                let n = base + i
                SMALL_LONG_LINES_BUF |> push_clone(
                    "{n} The quick brown fox jumps over the lazy dog")
            }
        }
        child(SMALL_LONG_CHILD, (text = "Log",
                                  size = float2(0.0f, 0.0f),
                                  child_flags = ImGuiChildFlags.None,
                                  window_flags = ImGuiWindowFlags.None)) {
            if (SMALL_LONG_TEST_TYPE == 0) {
                // Single TextUnformatted over the whole concatenated buffer.
                // cpp uses ImGuiTextBuffer + the (begin, end) two-ptr overload;
                // both are off the boost surface — substitute join+single-arg form.
                with_style((ImGuiStyleVar.ItemSpacing, float2(0.0f, 0.0f))) {
                    let joined = SMALL_LONG_LINES_BUF |> join("\n")
                    text_unformatted(SMALL_LONG_TEXT_ALL, (text = joined))
                }
            } elif (SMALL_LONG_TEST_TYPE == 1) {
                // Clipper-cull. Per-line content is identical (matches cpp).
                with_style((ImGuiStyleVar.ItemSpacing, float2(0.0f, 0.0f))) {
                    list_clipper(lines_n) $(start, end_) {
                        for (i in range(start, end_)) {
                            text(SMALL_LONG_LINES_TEXT[i],
                                (text = SMALL_LONG_LINES_BUF[i]))
                        }
                    }
                }
            } else {
                // Unclipped — explicit "slow" path matching cpp's case 2.
                with_style((ImGuiStyleVar.ItemSpacing, float2(0.0f, 0.0f))) {
                    for (i in range(lines_n)) {
                        text(SMALL_LONG_LINES_TEXT[i],
                            (text = SMALL_LONG_LINES_BUF[i]))
                    }
                }
            }
        }
    }
    if (!SMALL_LONG_WIN.open) {
        SMALL_LONG_WIN.open = true
        return false
    }
    return true
}

// =============================================================================
// Auto Resize — cpp:7859-7877
// =============================================================================

def ShowExampleAppAutoResize() : bool {
    window(SMALL_AUTO_WIN, (text = "Example: Auto-resizing window", closable = true,
                            flags = ImGuiWindowFlags.AlwaysAutoResize)) {
        text_unformatted(SMALL_AUTO_TEXT_HEAD,
            (text = "Window will resize every-frame to the size of its content.\n" +
                    "Note that you probably don't want to query the window size to\n" +
                    "output your content because that would create a feedback loop."))
        slider_int(SMALL_AUTO_SLIDER,
            (text = "Number of lines", init = 10, bounds = (1, 20)))
        for (i in range(SMALL_AUTO_SLIDER.value)) {
            // cpp: printf("%*sThis is line %d", i*4, "", i) — `%*s` left-pads
            // an empty string to width i*4. Substitute strrep(" ", i*4).
            let pad = repeat(" ", i * 4)
            text(SMALL_AUTO_LINES[i], (text = "{pad}This is line {i}"))
        }
    }
    if (!SMALL_AUTO_WIN.open) {
        SMALL_AUTO_WIN.open = true
        return false
    }
    return true
}

var SMALL_AUTO_LINES : table<int; NarrativeState>

// =============================================================================
// Constrained Resize — cpp:7885-7978
// =============================================================================

var private SMALL_CONSTR_AUTO_RESIZE = false
var private SMALL_CONSTR_WINDOW_PADDING = true
var private SMALL_CONSTR_TYPE = 6  // default = "Custom: Aspect Ratio 16:9"
var private SMALL_CONSTR_DISPLAY_LINES = 10

var SMALL_CONSTR_LINES : table<int; NarrativeState>

// Module-scope holders for the three callback-bearing constraints: ImGui invokes
// the stored callback from the NEXT Begin() (incl. the auto-fit pass), so the
// wrapper's address must outlive SetNextWindowSizeConstraints — no stack temps.
var private SMALL_CONSTR_ASPECT_CN : ImGuiSizeConstraints
var private SMALL_CONSTR_SQUARE_CN : ImGuiSizeConstraints
var private SMALL_CONSTR_STEP_CN : ImGuiSizeConstraints

[init]
def small_constr_init() {
    let aspect_ratio = 16.0f / 9.0f
    SMALL_CONSTR_ASPECT_CN <- ImGuiSizeConstraints(
        @ capture(= aspect_ratio) (var data : ImGuiSizeCallbackData) : void {
            data.DesiredSize = float2(data.DesiredSize.x,
                                      float(int(data.DesiredSize.x / aspect_ratio)))
        })
    SMALL_CONSTR_SQUARE_CN <- ImGuiSizeConstraints(
        @(var data : ImGuiSizeCallbackData) : void {
            let s = max(data.DesiredSize.x, data.DesiredSize.y)
            data.DesiredSize = float2(s, s)
        })
    let fixed_step = 100.0f
    SMALL_CONSTR_STEP_CN <- ImGuiSizeConstraints(
        @ capture(= fixed_step) (var data : ImGuiSizeCallbackData) : void {
            let rx = float(int(data.DesiredSize.x / fixed_step + 0.5f)) * fixed_step
            let ry = float(int(data.DesiredSize.y / fixed_step + 0.5f)) * fixed_step
            data.DesiredSize = float2(rx, ry)
        })
}

def ShowExampleAppConstrainedResize() : bool {
    // Submit constraint — 9 options. Options 0..5 use the no-callback overload;
    // 6..8 use the lambda-callback overload from imgui_window_constraints_builtin,
    // backed by the module-scope wrappers seeded in `small_constr_init`.
    if (SMALL_CONSTR_TYPE == 0) {
        SetNextWindowSizeConstraints(float2(100.0f, 100.0f), float2(500.0f, 500.0f))
    } elif (SMALL_CONSTR_TYPE == 1) {
        SetNextWindowSizeConstraints(float2(100.0f, 100.0f), float2(FLT_MAX, FLT_MAX))
    } elif (SMALL_CONSTR_TYPE == 2) {
        SetNextWindowSizeConstraints(float2(-1.0f, 0.0f), float2(-1.0f, FLT_MAX))
    } elif (SMALL_CONSTR_TYPE == 3) {
        SetNextWindowSizeConstraints(float2(0.0f, -1.0f), float2(FLT_MAX, -1.0f))
    } elif (SMALL_CONSTR_TYPE == 4) {
        SetNextWindowSizeConstraints(float2(400.0f, -1.0f), float2(500.0f, -1.0f))
    } elif (SMALL_CONSTR_TYPE == 5) {
        SetNextWindowSizeConstraints(float2(-1.0f, 400.0f), float2(-1.0f, FLT_MAX))
    } elif (SMALL_CONSTR_TYPE == 6) {
        SetNextWindowSizeConstraints(float2(0.0f, 0.0f), float2(FLT_MAX, FLT_MAX),
                                     SMALL_CONSTR_ASPECT_CN)
    } elif (SMALL_CONSTR_TYPE == 7) {
        SetNextWindowSizeConstraints(float2(0.0f, 0.0f), float2(FLT_MAX, FLT_MAX),
                                     SMALL_CONSTR_SQUARE_CN)
    } elif (SMALL_CONSTR_TYPE == 8) {
        SetNextWindowSizeConstraints(float2(0.0f, 0.0f), float2(FLT_MAX, FLT_MAX),
                                     SMALL_CONSTR_STEP_CN)
    }
    // Submit window — push WindowPadding=0 around Begin if window_padding off.
    let win_flags = (SMALL_CONSTR_AUTO_RESIZE ?
                     ImGuiWindowFlags.AlwaysAutoResize : ImGuiWindowFlags.None)
    if (!SMALL_CONSTR_WINDOW_PADDING) {
        with_style((ImGuiStyleVar.WindowPadding, float2(0.0f, 0.0f))) {
            ConstrainedResizeBody(win_flags)
        }
    } else {
        ConstrainedResizeBody(win_flags)
    }
    if (!SMALL_CONSTR_WIN.open) {
        SMALL_CONSTR_WIN.open = true
        return false
    }
    return true
}

def private ConstrainedResizeBody(win_flags : ImGuiWindowFlags) {
    window(SMALL_CONSTR_WIN, (text = "Example: Constrained Resize",
                              closable = true, flags = win_flags)) {
        if (button(SMALL_CONSTR_BTN_200, (text = "Set 200x200"))) {
            set_window_size(float2(200.0f, 200.0f))
        }
        same_line(SMALL_CONSTR_SL_1)
        if (button(SMALL_CONSTR_BTN_500, (text = "Set 500x500"))) {
            set_window_size(float2(500.0f, 500.0f))
        }
        same_line(SMALL_CONSTR_SL_2)
        if (button(SMALL_CONSTR_BTN_800, (text = "Set 800x200"))) {
            set_window_size(float2(800.0f, 200.0f))
        }
        SetNextItemWidth(GetFontSize() * 20.0f)
        combo(SMALL_CONSTR_COMBO,
              (text = "Constraint", init = SMALL_CONSTR_TYPE,
               items <- ["Between 100x100 and 500x500",
                         "At least 100x100",
                         "Resize vertical + lock current width",
                         "Resize horizontal + lock current height",
                         "Width Between 400 and 500",
                         "Height at least 400",
                         "Custom: Aspect Ratio 16:9",
                         "Custom: Always Square",
                         "Custom: Fixed Steps (100)"]))
        SMALL_CONSTR_TYPE = SMALL_CONSTR_COMBO.value
        SetNextItemWidth(GetFontSize() * 20.0f)
        drag_int(SMALL_CONSTR_DRAG,
            (text = "Lines", init = SMALL_CONSTR_DISPLAY_LINES,
             bounds = (1, 100), speed = 0.2f))
        SMALL_CONSTR_DISPLAY_LINES = SMALL_CONSTR_DRAG.value
        if (edit_checkbox(safe_addr(SMALL_CONSTR_AUTO_RESIZE),
                          (id = "SMALL_CONSTR_AUTO", text = "Auto-resize"))) {}
        if (edit_checkbox(safe_addr(SMALL_CONSTR_WINDOW_PADDING),
                          (id = "SMALL_CONSTR_PAD", text = "Window padding"))) {}
        for (i in range(SMALL_CONSTR_DISPLAY_LINES)) {
            let pad = repeat(" ", i * 4)
            text(SMALL_CONSTR_LINES[i],
                (text = "{pad}Hello, sailor! " +
                        "Making this line long enough for the example."))
        }
    }
}

// =============================================================================
// Simple Overlay — cpp:7986-8035
// =============================================================================

var private SMALL_OVERLAY_LOCATION = 0  // 0..3 = corner, -1 = custom, -2 = center

def ShowExampleAppSimpleOverlay() : bool {
    let PAD = 10.0f
    var window_flags = int(ImGuiWindowFlags.NoDecoration |
                            ImGuiWindowFlags.NoDocking |
                            ImGuiWindowFlags.AlwaysAutoResize |
                            ImGuiWindowFlags.NoSavedSettings |
                            ImGuiWindowFlags.NoFocusOnAppearing |
                            ImGuiWindowFlags.NoNav)
    let viewport = GetMainViewport()
    if (SMALL_OVERLAY_LOCATION >= 0) {
        let work_pos = viewport.WorkPos
        let work_size = viewport.WorkSize
        let pos_x = (((SMALL_OVERLAY_LOCATION & 1) != 0) ?
                     (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD))
        let pos_y = (((SMALL_OVERLAY_LOCATION & 2) != 0) ?
                     (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD))
        let pivot_x = ((SMALL_OVERLAY_LOCATION & 1) != 0) ? 1.0f : 0.0f
        let pivot_y = ((SMALL_OVERLAY_LOCATION & 2) != 0) ? 1.0f : 0.0f
        SetNextWindowPos(float2(pos_x, pos_y), ImGuiCond.Always,
                         float2(pivot_x, pivot_y))
        SetNextWindowViewport(viewport.ID)
        window_flags |= int(ImGuiWindowFlags.NoMove)
    } elif (SMALL_OVERLAY_LOCATION == -2) {
        let center = viewport_center(unsafe(*viewport))   // nolint:STYLE024  ptr-deref needs unsafe (rule misses ExprPtr2Ref)
        SetNextWindowPos(center, ImGuiCond.Always, float2(0.5f, 0.5f))
        SetNextWindowViewport(viewport.ID)
        window_flags |= int(ImGuiWindowFlags.NoMove)
    }
    SetNextWindowBgAlpha(0.35f)
    window(SMALL_OVERLAY_WIN,
           (text = "Example: Simple overlay", closable = true,
            flags = unsafe(reinterpret<ImGuiWindowFlags>(window_flags)))) {
        text(SMALL_OVERLAY_HEADER,
            (text = "Simple overlay\n(right-click to change position)"))
        separator(SMALL_OVERLAY_SEP)
        if (is_mouse_pos_valid()) {
            let mp = GetMousePos()
            text(SMALL_OVERLAY_MP, (text = "Mouse Position: ({mp.x},{mp.y})"))
        } else {
            text(SMALL_OVERLAY_MP, (text = "Mouse Position: <invalid>"))
        }
        popup_context_window(SMALL_OVERLAY_CTX,
                             (str_id = "##overlay_ctx", flags = ImGuiPopupFlags.None)) {
            if (menu_item(SMALL_OVERLAY_MI_CUSTOM, (text = "Custom"))) {
                SMALL_OVERLAY_LOCATION = -1
            }
            SMALL_OVERLAY_MI_CUSTOM.value = (SMALL_OVERLAY_LOCATION == -1)
            if (menu_item(SMALL_OVERLAY_MI_CENTER, (text = "Center"))) {
                SMALL_OVERLAY_LOCATION = -2
            }
            SMALL_OVERLAY_MI_CENTER.value = (SMALL_OVERLAY_LOCATION == -2)
            if (menu_item(SMALL_OVERLAY_MI_TL, (text = "Top-left"))) {
                SMALL_OVERLAY_LOCATION = 0
            }
            SMALL_OVERLAY_MI_TL.value = (SMALL_OVERLAY_LOCATION == 0)
            if (menu_item(SMALL_OVERLAY_MI_TR, (text = "Top-right"))) {
                SMALL_OVERLAY_LOCATION = 1
            }
            SMALL_OVERLAY_MI_TR.value = (SMALL_OVERLAY_LOCATION == 1)
            if (menu_item(SMALL_OVERLAY_MI_BL, (text = "Bottom-left"))) {
                SMALL_OVERLAY_LOCATION = 2
            }
            SMALL_OVERLAY_MI_BL.value = (SMALL_OVERLAY_LOCATION == 2)
            if (menu_item(SMALL_OVERLAY_MI_BR, (text = "Bottom-right"))) {
                SMALL_OVERLAY_LOCATION = 3
            }
            SMALL_OVERLAY_MI_BR.value = (SMALL_OVERLAY_LOCATION == 3)
            if (menu_item(SMALL_OVERLAY_MI_CLOSE, (text = "Close"))) {
                SMALL_OVERLAY_WIN.open = false
            }
        }
    }
    if (!SMALL_OVERLAY_WIN.open) {
        SMALL_OVERLAY_WIN.open = true
        return false
    }
    return true
}

// =============================================================================
// Fullscreen Window — cpp:8042-8071
// =============================================================================

var private SMALL_FULL_USE_WORK_AREA = true
var private SMALL_FULL_FLAGS = int(ImGuiWindowFlags.NoDecoration |
                                    ImGuiWindowFlags.NoMove |
                                    ImGuiWindowFlags.NoSavedSettings)

def ShowExampleAppFullscreen() : bool {
    let viewport = GetMainViewport()
    let pos = SMALL_FULL_USE_WORK_AREA ? viewport.WorkPos : viewport.Pos
    let size = SMALL_FULL_USE_WORK_AREA ? viewport.WorkSize : viewport.Size
    SetNextWindowPos(pos, ImGuiCond.Always, float2(0.0f, 0.0f))
    SetNextWindowSize(size, ImGuiCond.Always)
    window(SMALL_FULL_WIN, (text = "Example: Fullscreen window", closable = true,
                            flags = unsafe(reinterpret<ImGuiWindowFlags>(SMALL_FULL_FLAGS)))) {
        if (edit_checkbox(safe_addr(SMALL_FULL_USE_WORK_AREA),
                          (id = "SMALL_FULL_UWA",
                           text = "Use work area instead of main area"))) {}
        same_line(SMALL_FULL_SL_HM)
        // HelpMarker shim.
        text_disabled(SMALL_FULL_HM_LABEL, (text = "(?)"))
        item_tooltip(SMALL_FULL_HM) {
            with_text_wrap_pos(GetFontSize() * 35.0f) {
                text_unformatted(SMALL_FULL_HM_TEXT,
                    (text = "Main Area = entire viewport,\n" +
                            "Work Area = entire viewport minus sections used by " +
                            "the main menu bars, task bars etc.\n\n" +
                            "Enable the main-menu bar in Examples menu to see the " +
                            "difference."))
            }
        }
        edit_checkbox_flags(safe_addr(SMALL_FULL_FLAGS),
            (id = "SMALL_FULL_NBG", text = "ImGuiWindowFlags_NoBackground",
             flags_value = int(ImGuiWindowFlags.NoBackground)))
        edit_checkbox_flags(safe_addr(SMALL_FULL_FLAGS),
            (id = "SMALL_FULL_NDC", text = "ImGuiWindowFlags_NoDecoration",
             flags_value = int(ImGuiWindowFlags.NoDecoration)))
        with_indent(0.0f) {
            edit_checkbox_flags(safe_addr(SMALL_FULL_FLAGS),
                (id = "SMALL_FULL_NTB", text = "ImGuiWindowFlags_NoTitleBar",
                 flags_value = int(ImGuiWindowFlags.NoTitleBar)))
            edit_checkbox_flags(safe_addr(SMALL_FULL_FLAGS),
                (id = "SMALL_FULL_NCO", text = "ImGuiWindowFlags_NoCollapse",
                 flags_value = int(ImGuiWindowFlags.NoCollapse)))
            edit_checkbox_flags(safe_addr(SMALL_FULL_FLAGS),
                (id = "SMALL_FULL_NSB", text = "ImGuiWindowFlags_NoScrollbar",
                 flags_value = int(ImGuiWindowFlags.NoScrollbar)))
        }
        if (button(SMALL_FULL_BTN_CLOSE, (text = "Close this window"))) {
            SMALL_FULL_WIN.open = false
        }
    }
    if (!SMALL_FULL_WIN.open) {
        SMALL_FULL_WIN.open = true
        return false
    }
    return true
}

// =============================================================================
// Manipulating Window Titles — cpp:8080-8107
// =============================================================================

// All three windows pass `bool* p_open = nullptr` in cpp (closable=false).
// `##` / `###` separators in the `text` argument flow unchanged into Begin()
// per imgui_containers_builtin.das:139 — IDENT controls daslang state only.

def ShowExampleAppWindowTitles() {
    let viewport = GetMainViewport()
    let base_pos = viewport.Pos

    SetNextWindowPos(float2(base_pos.x + 100.0f, base_pos.y + 100.0f),
                     ImGuiCond.FirstUseEver, float2(0.0f, 0.0f))
    window(SMALL_TITLES_W1, (text = "Same title as another window##1",
                              closable = false, flags = ImGuiWindowFlags.None)) {
        text(SMALL_TITLES_W1_TEXT,
            (text = "This is window 1.\nMy title is the same as window 2, " +
                    "but my identifier is unique."))
    }

    SetNextWindowPos(float2(base_pos.x + 100.0f, base_pos.y + 200.0f),
                     ImGuiCond.FirstUseEver, float2(0.0f, 0.0f))
    window(SMALL_TITLES_W2, (text = "Same title as another window##2",
                              closable = false, flags = ImGuiWindowFlags.None)) {
        text(SMALL_TITLES_W2_TEXT,
            (text = "This is window 2.\nMy title is the same as window 1, " +
                    "but my identifier is unique."))
    }

    let frame = GetFrameCount()
    let spinner = "|/-\\"
    let si = (frame / 15) % 4
    let title = "Animated title {character_at(spinner, si)} {frame}###AnimatedTitle"   // nolint:PERF003  4-char literal, once per frame
    SetNextWindowPos(float2(base_pos.x + 100.0f, base_pos.y + 300.0f),
                     ImGuiCond.FirstUseEver, float2(0.0f, 0.0f))
    window(SMALL_TITLES_W3, (text = title, closable = false,
                              flags = ImGuiWindowFlags.None)) {
        text(SMALL_TITLES_W3_TEXT, (text = "This window has a changing title."))
    }
}
