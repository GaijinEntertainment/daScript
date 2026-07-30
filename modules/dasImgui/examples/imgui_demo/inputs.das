options gen2
options _comment_hygiene = true
options indenting = 4

module inputs

require imgui
require imgui/imgui_widgets_builtin
require imgui/imgui_containers_builtin
require imgui/imgui_scope_builtin
require imgui/imgui_drawlist_builtin     // with_foreground_drawlist + add_line
require daslib/safe_addr
require daslib/enum_trait
require strings    // format / to_char for InputQueueCharacters per-char render

// Mirrors imgui_demo.cpp:6249-6452 — static void ShowDemoWindowInputs().

// ----- Outputs/WantCapture override — external-pointer plain vars -----
// `edit_slider_int` ties these to the SliderInt widgets below; the
// `format` arg renders the name ("None"/"Set to false"/"Set to true")
// instead of the int.
var INPUTS_CAPTURE_OVERRIDE_MOUSE = -1
var INPUTS_CAPTURE_OVERRIDE_KEYBOARD = -1

// ----- Tabbing / Focus-from-code — shared text-input buffer state -----
// `input_text` owns its buffer via [widget] state; init= seeds the first
// frame's contents.

// ----- Indexed loop tables -----
// Per-row state for the Mouse Cursors list (9 rows for the 9 standard
// cursors). Bullet + same_line + selectable_hover per row.
var private INPUTS_LOOP_CURSOR_BULLET : table<int; EmptyMarkerState>
var private INPUTS_LOOP_CURSOR_SL : table<int; EmptyMarkerState>
var private INPUTS_LOOP_CURSOR_SL_HOVER : table<int; EmptyMarkerState>

def ShowDemoWindowInputs() {
    collapsing_header(INPUTS_SECTION,
        (text = "Inputs & Focus", closable = false,
         flags = ImGuiTreeNodeFlags.None)) {

        InputsSubSection()
        OutputsSubSection()
        MouseCursorsSubSection()
        TabbingSubSection()
        FocusFromCodeSubSection()
        DraggingSubSection()
    }
}


// =============================================================================
// Inputs — cpp:6258-6290
// =============================================================================

def private InputsSubSection() {
    SetNextItemOpen(true, ImGuiCond.Once)
    tree_node(INPUTS_TN_INPUTS,
        (text = "Inputs", flags = ImGuiTreeNodeFlags.None)) {

        let io & = unsafe(GetIO())

        // HelpMarker shim.
        text_disabled("(?)")
        item_tooltip(INPUTS_HM_INPUTS) {
            with_text_wrap_pos(GetFontSize() * 35.0f) {
                text_unformatted((text = "This is a simplified view. See more detailed input state:\n" +
                    "- in 'Tools->Metrics/Debugger->Inputs'.\n" +
                    "- in 'Tools->Debug Log->IO'."))
            }
        }

        if (IsMousePosValid(null)) {
            text("Mouse pos: ({io.MousePos.x}, {io.MousePos.y})")
        } else {
            text("Mouse pos: <INVALID>")
        }
        text("Mouse delta: ({io.MouseDelta.x}, {io.MouseDelta.y})")

        // C++ splices per-button with SameLine; we build one string, one widget.
        var down_line = "Mouse down:"
        for (i in range(5)) {
            if (IsMouseDown(unsafe(reinterpret<ImGuiMouseButton>(i)))) {
                down_line += " b{i} ({io.MouseDownDuration[i]} secs)"
            }
        }
        text((text = down_line))

        text("Mouse wheel: {io.MouseWheel}")

        // Keys down: iterate every named ImGuiKey value, skip ones not pressed.
        var keys_line = "Keys down:"
        for (k in type<ImGuiKey>) {
            if (IsKeyDown(k)) {
                keys_line += " \"{GetKeyName(k)}\" {int(k)}"
            }
        }
        text((text = keys_line))

        var mods_line = "Keys mods:"
        if (io.KeyCtrl) {
            mods_line += " CTRL"
        }
        if (io.KeyShift) {
            mods_line += " SHIFT"
        }
        if (io.KeyAlt) {
            mods_line += " ALT"
        }
        if (io.KeySuper) {
            mods_line += " SUPER"
        }
        text((text = mods_line))

        // InputQueueCharacters is an ImVector<ImWchar>; its binding wires das_index/das_default_vector_size so length()+[i] work directly.
        var chars_line = "Chars queue:"
        for (c16 in io.InputQueueCharacters) {
            let c = int(c16)
            let printable = (c > 32 && c <= 255) ? to_char(c) : "?"
            chars_line += " '{printable}' (0x{fmt(":04X", c)})"
        }
        text((text = chars_line))
    }
}


// =============================================================================
// Outputs — cpp:6293-6335
// =============================================================================

def private OutputsSubSection() {
    SetNextItemOpen(true, ImGuiCond.Once)
    tree_node(INPUTS_TN_OUTPUTS,
        (text = "Outputs", flags = ImGuiTreeNodeFlags.None)) {

        let io & = unsafe(GetIO())

        text_disabled("(?)")
        item_tooltip(INPUTS_HM_OUTPUTS) {
            with_text_wrap_pos(GetFontSize() * 35.0f) {
                text_unformatted((text = "The value of io.WantCaptureMouse and io.WantCaptureKeyboard " +
                    "are normally set by Dear ImGui to instruct your application " +
                    "of how to route inputs. Typically, when a value is true, it " +
                    "means Dear ImGui wants the corresponding inputs and we " +
                    "expect the underlying application to ignore them.\n\n" +
                    "The most typical case is: when hovering a window, Dear ImGui " +
                    "set io.WantCaptureMouse to true, and underlying application " +
                    "should ignore mouse inputs (in practice there are many and " +
                    "more subtle rules leading to how those flags are set)."))
            }
        }

        text("io.WantCaptureMouse: {io.WantCaptureMouse}")
        text("io.WantCaptureMouseUnlessPopupClose: {io.WantCaptureMouseUnlessPopupClose}")
        text("io.WantCaptureKeyboard: {io.WantCaptureKeyboard}")
        text("io.WantTextInput: {io.WantTextInput}")
        text("io.WantSetMousePos: {io.WantSetMousePos}")
        text("io.NavActive: {io.NavActive}, io.NavVisible: {io.NavVisible}")

        WantCaptureOverrideSubSection()
    }
}

def private WantCaptureOverrideSubSection() {
    tree_node(INPUTS_TN_CAPTURE_OVERRIDE,
        (text = "WantCapture override", flags = ImGuiTreeNodeFlags.None)) {

        text_disabled("(?)")
        item_tooltip(INPUTS_HM_CAPTURE) {
            with_text_wrap_pos(GetFontSize() * 35.0f) {
                text_unformatted((text = "Hovering the colored canvas will override io.WantCaptureXXX " +
                    "fields.\nNotice how normally (when set to none), the value " +
                    "of io.WantCaptureKeyboard would be false when hovering and " +
                    "true when clicking."))
            }
        }

        let desc = fixed_array("None", "Set to false", "Set to true")
        let mouse_v = INPUTS_CAPTURE_OVERRIDE_MOUSE
        let kbd_v = INPUTS_CAPTURE_OVERRIDE_KEYBOARD
        let mouse_label = (mouse_v >= -1 && mouse_v <= 1) ? desc[mouse_v + 1] : "?"
        let kbd_label = (kbd_v >= -1 && kbd_v <= 1) ? desc[kbd_v + 1] : "?"

        SetNextItemWidth(GetFontSize() * 15.0f)
        edit_slider_int(safe_addr(INPUTS_CAPTURE_OVERRIDE_MOUSE),
            (id = "INPUTS_CAP_MOUSE_SL",
             text = "SetNextFrameWantCaptureMouse() on hover",
             v_min = -1, v_max = 1, format = mouse_label,
             flags = ImGuiSliderFlags.AlwaysClamp))

        SetNextItemWidth(GetFontSize() * 15.0f)
        edit_slider_int(safe_addr(INPUTS_CAPTURE_OVERRIDE_KEYBOARD),
            (id = "INPUTS_CAP_KBD_SL",
             text = "SetNextFrameWantCaptureKeyboard() on hover",
             v_min = -1, v_max = 1, format = kbd_label,
             flags = ImGuiSliderFlags.AlwaysClamp));

        let swatch_flags = (ImGuiColorEditFlags.NoTooltip | ImGuiColorEditFlags.NoDragDrop)
        let hovered = color_button_hover(INPUTS_CAP_SWATCH,
            (desc_id = "##panel", col = float4(0.7f, 0.1f, 0.7f, 1.0f),
             size = float2(128.0f, 96.0f), flags = swatch_flags))
        if (hovered && INPUTS_CAPTURE_OVERRIDE_MOUSE != -1) {
            SetNextFrameWantCaptureMouse(INPUTS_CAPTURE_OVERRIDE_MOUSE == 1)
        }
        if (hovered && INPUTS_CAPTURE_OVERRIDE_KEYBOARD != -1) {
            SetNextFrameWantCaptureKeyboard(INPUTS_CAPTURE_OVERRIDE_KEYBOARD == 1)
        }
    }
}


// =============================================================================
// Mouse Cursors — cpp:6338-6364
// =============================================================================

def private MouseCursorsSubSection() {
    tree_node(INPUTS_TN_MOUSE_CURSORS,
        (text = "Mouse Cursors", flags = ImGuiTreeNodeFlags.None)) {

        let io & = unsafe(GetIO())
        let cursors = fixed_array("Arrow", "TextInput", "ResizeAll", "ResizeNS",
                                  "ResizeEW", "ResizeNESW", "ResizeNWSE", "Hand",
                                  "NotAllowed")
        let current = GetMouseCursor()
        let cur_idx = int(current)
        let cur_name = ((cur_idx >= 0 && cur_idx < length(cursors))
                        ? cursors[cur_idx] : "?")
        text("Current mouse cursor = {cur_idx}: {cur_name}")

        // C++ shows a disabled CheckboxFlags; editing an ImGuiBackendFlags field via the int? rail needs a bounce-buffer, so we render the bit as plain text.
        let has_mouse_cursors = (uint(io.BackendFlags & ImGuiBackendFlags.HasMouseCursors) != 0u)
        text("io.BackendFlags: HasMouseCursors = {has_mouse_cursors}")

        text("Hover to see mouse cursors:")
        same_line(INPUTS_SL_HOVER)
        text_disabled("(?)")
        item_tooltip(INPUTS_HM_HOVER_CURSORS) {
            with_text_wrap_pos(GetFontSize() * 35.0f) {
                text_unformatted((text = "Your application can render a different mouse cursor based " +
                    "on what ImGui::GetMouseCursor() returns. If software cursor " +
                    "rendering (io.MouseDrawCursor) is set ImGui will draw the " +
                    "right cursor for you, otherwise your backend needs to " +
                    "handle it."))
            }
        }

        for (i in range(int(ImGuiMouseCursor.COUNT))) {
            bullet(INPUTS_LOOP_CURSOR_BULLET[i])
            same_line(INPUTS_LOOP_CURSOR_SL[i])
            let hovered = selectable_hover(INPUTS_LOOP_CURSOR_SL_HOVER[i],
                (text = "Mouse cursor {i}: {cursors[i]}"))
            if (hovered) {
                SetMouseCursor(unsafe(reinterpret<ImGuiMouseCursor>(i)))
            }
        }
    }
}


// =============================================================================
// Tabbing — cpp:6367-6380
// =============================================================================

def private TabbingSubSection() {
    tree_node(INPUTS_TN_TABBING,
        (text = "Tabbing", flags = ImGuiTreeNodeFlags.None)) {

        text("Use TAB/SHIFT+TAB to cycle through keyboard editable fields.")
        input_text(INPUTS_TAB_1, (text = "1", init = "hello"))
        input_text(INPUTS_TAB_2, (text = "2", init = "hello"))
        input_text(INPUTS_TAB_3, (text = "3", init = "hello"))
        with_tab_stop(false) {
            input_text(INPUTS_TAB_4, (text = "4 (tab skip)", init = "hello"))
            same_line(INPUTS_SL_TAB_4)
            text_disabled("(?)")
            item_tooltip(INPUTS_HM_TAB) {
                with_text_wrap_pos(GetFontSize() * 35.0f) {
                    text_unformatted("Item won't be cycled through when using TAB or Shift+Tab.")
                }
            }
        }
        input_text(INPUTS_TAB_5, (text = "5", init = "hello"))
    }
}


// =============================================================================
// Focus from code — cpp:6383-6422
// =============================================================================

def private FocusFromCodeSubSection() {
    tree_node(INPUTS_TN_FOCUS,
        (text = "Focus from code", flags = ImGuiTreeNodeFlags.None)) {

        let focus_1 = button(INPUTS_FOCUS_BTN_1, (text = "Focus on 1"))
        same_line(INPUTS_SL_FOCUS_1)
        let focus_2 = button(INPUTS_FOCUS_BTN_2, (text = "Focus on 2"))
        same_line(INPUTS_SL_FOCUS_2)
        let focus_3 = button(INPUTS_FOCUS_BTN_3, (text = "Focus on 3"))

        var has_focus = 0

        if (focus_1) {
            SetKeyboardFocusHere(0)
        }
        input_text(INPUTS_FOCUS_TXT_1,
            (text = "1", init = "click on a button to set focus"))
        if (IsItemActive()) {
            has_focus = 1
        }

        if (focus_2) {
            SetKeyboardFocusHere(0)
        }
        input_text(INPUTS_FOCUS_TXT_2,
            (text = "2", init = "click on a button to set focus"))
        if (IsItemActive()) {
            has_focus = 2
        }

        with_tab_stop(false) {
            if (focus_3) {
                SetKeyboardFocusHere(0)
            }
            input_text(INPUTS_FOCUS_TXT_3,
                (text = "3 (tab skip)", init = "click on a button to set focus"))
            if (IsItemActive()) {
                has_focus = 3
            }
            same_line(INPUTS_SL_FOCUS_3)
            text_disabled("(?)")
            item_tooltip(INPUTS_HM_FOCUS_TAB) {
                with_text_wrap_pos(GetFontSize() * 35.0f) {
                    text_unformatted("Item won't be cycled through when using TAB or Shift+Tab.")
                }
            }
        }

        if (has_focus != 0) {
            text("Item with focus: {has_focus}")
        } else {
            text("Item with focus: <none>")
        }

        // Focus-ahead: SetKeyboardFocusHere(N) focuses the Nth upcoming item.
        let focus_x = button(INPUTS_FOCUS_BTN_X, (text = "Focus on X"))
        same_line(INPUTS_SL_FOCUS_X)
        let focus_y = button(INPUTS_FOCUS_BTN_Y, (text = "Focus on Y"))
        same_line(INPUTS_SL_FOCUS_Y)
        let focus_z = button(INPUTS_FOCUS_BTN_Z, (text = "Focus on Z"))

        var focus_ahead = -1
        if (focus_x) {
            focus_ahead = 0
        }
        if (focus_y) {
            focus_ahead = 1
        }
        if (focus_z) {
            focus_ahead = 2
        }
        if (focus_ahead != -1) {
            SetKeyboardFocusHere(focus_ahead)
        }
        slider_float3(INPUTS_FOCUS_F3,
            (text = "Float3", init = float3(0.0f, 0.0f, 0.0f),
             bounds = (0.0f, 1.0f)))

        text_wrapped("NB: Cursor & selection are preserved when refocusing last used item in code.")
    }
}


// =============================================================================
// Dragging — cpp:6425-6450
// =============================================================================

def private DraggingSubSection() {
    tree_node(INPUTS_TN_DRAGGING,
        (text = "Dragging", flags = ImGuiTreeNodeFlags.None)) {

        let io & = unsafe(GetIO())

        text_wrapped("You can use ImGui::GetMouseDragDelta(0) to query for the dragged amount on any widget.")
        for (b in range(3)) {
            let btn = unsafe(reinterpret<ImGuiMouseButton>(b))
            let dr_def : bool = IsMouseDragging(btn, -1.0f)
            let dr_zero : bool = IsMouseDragging(btn, 0.0f)
            let dr_big : bool = IsMouseDragging(btn, 20.0f)
            text("IsMouseDragging({b}):")
            text("  w/ default threshold: {dr_def},")
            text("  w/ zero threshold: {dr_zero},")
            text("  w/ large threshold: {dr_big},")
        }

        button(INPUTS_DRAG_ME, (text = "Drag Me"))
        if (IsItemActive()) {
            with_foreground_drawlist() $(var dl) {
                let col = GetColorU32(ImGuiCol.Button, 1.0f)
                dl |> add_line(io.MouseClickedPos[0], io.MousePos, col, 4.0f)
            }
        }

        let value_raw = GetMouseDragDelta(ImGuiMouseButton.Left, 0.0f)
        let value_with_lock = GetMouseDragDelta(ImGuiMouseButton.Left, -1.0f)
        let mouse_delta = io.MouseDelta
        text("GetMouseDragDelta(0):")
        text("  w/ default threshold: ({value_with_lock.x}, {value_with_lock.y})")
        text("  w/ zero threshold: ({value_raw.x}, {value_raw.y})")
        text("io.MouseDelta: ({mouse_delta.x}, {mouse_delta.y})")
    }
}
