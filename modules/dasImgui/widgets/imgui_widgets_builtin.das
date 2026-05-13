options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module imgui_widgets_builtin shared

require imgui public
require imgui/imgui_boost_runtime public
require imgui/imgui_boost_v2 public
require daslib/json public
require daslib/json_boost
require strings
require math

//! Built-in widgets defined via `[widget]` — the same mechanism users get.
//! Each def constructs two short closures (serializer + dispatcher) and
//! hands them to widget_finalize. Common per-frame fields (hex_id/bbox/
//! hover/active/focus) are captured inside widget_finalize; per-kind state
//! lives in the state struct alone.
//!
//! Four private *_finalize helpers below collapse the repetitive
//! lambda-construction + widget_finalize call shared across click /
//! toggle / radio_int / pending-value widget families. Each widget def
//! stays focused on the imgui call + per-frame state mutation. The
//! `pending_value_finalize` helper is generic on the state struct and
//! drives `from_JV` off `typedecl(state.pending_value)` — one body
//! covers every Drag* / Slider* / Input* family without per-type
//! duplication.

// ===== Common finalize helpers =====

def private click_finalize(widget_ident : string; kind : string; var state : ClickState) {
    //! Lambda + dispatcher install for click-style widgets (Button family).
    //! Dispatcher action: "click" → state.pending_click = true (consumed next frame).
    var entry : WidgetEntry
    unsafe {
        let ser <- @ capture(& state) () : JsonValue ? {
            return JV(state)
        }
        let disp <- @ capture(& state) (action : string; payload : JsonValue ?) {
            if (action == "click") {
                state.pending_click = true
            }
        }
        widget_finalize(widget_ident, kind, entry, ser, disp)
    }
    register_focusable(widget_ident)
}

def private toggle_finalize(widget_ident : string; kind : string; var state : ToggleState) {
    //! Lambda + dispatcher install for bool-toggle widgets (Checkbox, RadioButton-bool,
    //! Selectable, MenuItem). Dispatcher action: "set" with bool payload.
    var entry : WidgetEntry
    unsafe {
        let ser <- @ capture(& state) () : JsonValue ? {
            return JV(state)
        }
        let disp <- @ capture(& state) (action : string; payload : JsonValue ?) {
            if (action == "set") {
                state.pending_value = payload?["value"] ?? false
                state.pending_toggle = true
            }
        }
        widget_finalize(widget_ident, kind, entry, ser, disp)
    }
    register_focusable(widget_ident)
}

def private radio_int_finalize(widget_ident : string; kind : string; var state : RadioIntState) {
    //! Lambda + dispatcher install for the int-form RadioButton.
    //! Dispatcher action: "set" with int payload; renderer applies before the imgui call.
    var entry : WidgetEntry
    unsafe {
        let ser <- @ capture(& state) () : JsonValue ? {
            return JV(state)
        }
        let disp <- @ capture(& state) (action : string; payload : JsonValue ?) {
            if (action == "set") {
                state.pending_value = payload?["value"] ?? 0
                state.pending_set = true
            }
        }
        widget_finalize(widget_ident, kind, entry, ser, disp)
    }
    register_focusable(widget_ident)
}

def private text_value_finalize(widget_ident : string; kind : string; var state : InputTextState) {
    // String-typed cousin of pending_value_finalize. The JsonValue payload's
    // underlying bytes drop when the dispatcher returns; we `:=` clone into
    // the widget context heap so `state.pending_value` is still valid when
    // the next widget body frame consumes it.
    var entry : WidgetEntry
    unsafe {
        let ser <- @ capture(& state) () : JsonValue ? {
            return JV(state)
        }
        let disp <- @ capture(& state) (action : string; payload : JsonValue ?) {
            if (action == "set") {
                state.pending_value := payload?["value"] ?? ""
                state.has_pending = true
            }
        }
        widget_finalize(widget_ident, kind, entry, ser, disp)
    }
    register_focusable(widget_ident)
}

// ===== Triggers (click family) =====

[widget]
def button(var state : ClickState; text : string) : bool {
    let im_clicked = Button(text, ImVec2(0.0f, 0.0f))
    var clicked = im_clicked
    if (state.pending_click) {
        state.pending_click = false
        clicked = true
    }
    state.clicked = clicked
    if (clicked) {
        state.click_count++
    }
    click_finalize(widget_ident, "button", state)
    return clicked
}

[widget]
def small_button(var state : ClickState; text : string) : bool {
    let im_clicked = SmallButton(text)
    var clicked = im_clicked
    if (state.pending_click) {
        state.pending_click = false
        clicked = true
    }
    state.clicked = clicked
    if (clicked) {
        state.click_count++
    }
    click_finalize(widget_ident, "small_button", state)
    return clicked
}

[widget]
def arrow_button(var state : ClickState; text : string; dir : ImGuiDir) : bool {
    let im_clicked = ArrowButton(text, dir)
    var clicked = im_clicked
    if (state.pending_click) {
        state.pending_click = false
        clicked = true
    }
    state.clicked = clicked
    if (clicked) {
        state.click_count++
    }
    click_finalize(widget_ident, "arrow_button", state)
    return clicked
}

[widget]
def invisible_button(var state : ClickState; text : string; size : float2;
                     flags : ImGuiButtonFlags = ImGuiButtonFlags.None) : bool {
    let im_clicked = InvisibleButton(text, size, flags)
    var clicked = im_clicked
    if (state.pending_click) {
        state.pending_click = false
        clicked = true
    }
    state.clicked = clicked
    if (clicked) {
        state.click_count++
    }
    click_finalize(widget_ident, "invisible_button", state)
    return clicked
}

[widget]
def image_button(var state : ClickState; text : string; user_texture_id : void?; size : float2;
                 uv0 : float2 = float2(0.0f, 0.0f);
                 uv1 : float2 = float2(1.0f, 1.0f);
                 bg_col : float4 = float4(0.0f, 0.0f, 0.0f, 0.0f);
                 tint_col : float4 = float4(1.0f, 1.0f, 1.0f, 1.0f)) : bool {
    let im_clicked = ImageButton(text, user_texture_id, size, uv0, uv1, bg_col, tint_col)
    var clicked = im_clicked
    if (state.pending_click) {
        state.pending_click = false
        clicked = true
    }
    state.clicked = clicked
    if (clicked) {
        state.click_count++
    }
    click_finalize(widget_ident, "image_button", state)
    return clicked
}

// ===== Toggles (bool family) =====

[widget]
def checkbox(var state : ToggleState; text : string) : bool {
    if (state.pending_toggle) {
        state.value = state.pending_value
        state.pending_toggle = false
    }
    let changed = Checkbox(text, unsafe(addr(state.value)))
    state.changed = changed
    toggle_finalize(widget_ident, "checkbox", state)
    return changed
}

[widget]
def radio_button(var state : ToggleState; text : string) : bool {
    //! Bool-form RadioButton — single toggle. ImGui's RadioButton(label,bool)
    //! does NOT mutate the bool, only returns true on click; daslang side
    //! flips state.value on click to give "single radio = checkbox shaped
    //! like a radio" semantics. Grouped radios use radio_button_int.
    if (state.pending_toggle) {
        state.value = state.pending_value
        state.pending_toggle = false
    }
    let im_clicked = RadioButton(text, state.value)
    if (im_clicked) {
        state.value = !state.value
    }
    state.changed = im_clicked
    toggle_finalize(widget_ident, "radio_button", state)
    return im_clicked
}

[widget]
def selectable(var state : ToggleState; text : string;
               flags : ImGuiSelectableFlags = ImGuiSelectableFlags.None;
               size : float2 = float2(0.0f, 0.0f)) : bool {
    if (state.pending_toggle) {
        state.value = state.pending_value
        state.pending_toggle = false
    }
    let changed = Selectable(text, unsafe(addr(state.value)), flags, size)
    state.changed = changed
    toggle_finalize(widget_ident, "selectable", state)
    return changed
}

[widget]
def menu_item(var state : ToggleState; text : string; shortcut : string = "") : bool {
    //! Non-block MenuItem with toggle semantics. The block-arg form (menu
    //! container) lands in 0b.3. `enabled = true` always; the disabled
    //! variant is a future per-call knob if real users hit it.
    if (state.pending_toggle) {
        state.value = state.pending_value
        state.pending_toggle = false
    }
    let changed = MenuItem(text, shortcut, unsafe(addr(state.value)), true)
    state.changed = changed
    toggle_finalize(widget_ident, "menu_item", state)
    return changed
}

// ===== Radio (int family) =====

[widget]
def radio_button_int(var state : RadioIntState; text : string; v_button : int) : bool {
    //! Grouped-radio form. Each call site picks `v_button`; click writes
    //! state.value = v_button. Multiple radio_button_int calls sharing the
    //! same RadioIntState ident form one logical group.
    if (state.pending_set) {
        state.value = state.pending_value
        state.pending_set = false
    }
    let changed = RadioButton(text, unsafe(addr(state.value)), v_button)
    state.changed = changed
    radio_int_finalize(widget_ident, "radio_button_int", state)
    return changed
}

// ===== Stateful inputs (drag family) =====

[widget]
def drag_float(var state : DragStateFloat; text : string;
               speed : float = 1.0f;
               format : string = "%.3f";
               flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let (mn, mx) = state.bounds
    let changed = DragFloat(text, unsafe(addr(state.value)), speed, mn, mx, format, flags)
    state.changed = changed
    pending_value_finalize(widget_ident, "drag_float", state)
    return changed
}

[widget]
def drag_float2(var state : DragStateFloat2; text : string;
                speed : float = 1.0f;
                format : string = "%.3f";
                flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let (mn, mx) = state.bounds
    let changed = DragFloat2(text, unsafe(addr(state.value.x)), speed, mn, mx, format, flags)
    state.changed = changed
    pending_value_finalize(widget_ident, "drag_float2", state)
    return changed
}

[widget]
def drag_float3(var state : DragStateFloat3; text : string;
                speed : float = 1.0f;
                format : string = "%.3f";
                flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let (mn, mx) = state.bounds
    let changed = DragFloat3(text, unsafe(addr(state.value.x)), speed, mn, mx, format, flags)
    state.changed = changed
    pending_value_finalize(widget_ident, "drag_float3", state)
    return changed
}

[widget]
def drag_float4(var state : DragStateFloat4; text : string;
                speed : float = 1.0f;
                format : string = "%.3f";
                flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let (mn, mx) = state.bounds
    let changed = DragFloat4(text, unsafe(addr(state.value.x)), speed, mn, mx, format, flags)
    state.changed = changed
    pending_value_finalize(widget_ident, "drag_float4", state)
    return changed
}

[widget]
def drag_int(var state : DragStateInt; text : string;
             speed : float = 1.0f;
             format : string = "%d";
             flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let (mn, mx) = state.bounds
    let changed = DragInt(text, unsafe(addr(state.value)), speed, mn, mx, format, flags)
    state.changed = changed
    pending_value_finalize(widget_ident, "drag_int", state)
    return changed
}

[widget]
def drag_int2(var state : DragStateInt2; text : string;
              speed : float = 1.0f;
              format : string = "%d";
              flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let (mn, mx) = state.bounds
    let changed = DragInt2(text, unsafe(addr(state.value.x)), speed, mn, mx, format, flags)
    state.changed = changed
    pending_value_finalize(widget_ident, "drag_int2", state)
    return changed
}

[widget]
def drag_int3(var state : DragStateInt3; text : string;
              speed : float = 1.0f;
              format : string = "%d";
              flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let (mn, mx) = state.bounds
    let changed = DragInt3(text, unsafe(addr(state.value.x)), speed, mn, mx, format, flags)
    state.changed = changed
    pending_value_finalize(widget_ident, "drag_int3", state)
    return changed
}

[widget]
def drag_int4(var state : DragStateInt4; text : string;
              speed : float = 1.0f;
              format : string = "%d";
              flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let (mn, mx) = state.bounds
    let changed = DragInt4(text, unsafe(addr(state.value.x)), speed, mn, mx, format, flags)
    state.changed = changed
    pending_value_finalize(widget_ident, "drag_int4", state)
    return changed
}

[widget]
def drag_float_range2(var state : DragStateRangeFloat; text : string;
                      speed : float = 1.0f;
                      format : string = "%.3f";
                      flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    //! Two-handle drag. `state.value.x` = current_min, `.y` = current_max;
    //! `state.bounds` clamps both. ImGui internally enforces lo <= hi each frame.
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let (mn, mx) = state.bounds
    let changed = DragFloatRange2(text, unsafe(addr(state.value.x)), unsafe(addr(state.value.y)),
                                  speed, mn, mx, format, format, flags)
    state.changed = changed
    pending_value_finalize(widget_ident, "drag_float_range2", state)
    return changed
}

[widget]
def drag_int_range2(var state : DragStateRangeInt; text : string;
                    speed : float = 1.0f;
                    format : string = "%d";
                    flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let (mn, mx) = state.bounds
    let changed = DragIntRange2(text, unsafe(addr(state.value.x)), unsafe(addr(state.value.y)),
                                speed, mn, mx, format, format, flags)
    state.changed = changed
    pending_value_finalize(widget_ident, "drag_int_range2", state)
    return changed
}

// ===== Stateful inputs (slider family) =====

[widget]
def slider_float(var state : SliderStateFloat; text : string;
                 format : string = "%.3f";
                 flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let (mn, mx) = state.bounds
    let changed = SliderFloat(text, unsafe(addr(state.value)), mn, mx, format, flags)
    state.changed = changed
    pending_value_finalize(widget_ident, "slider_float", state)
    return changed
}

[widget]
def slider_float2(var state : SliderStateFloat2; text : string;
                  format : string = "%.3f";
                  flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let (mn, mx) = state.bounds
    let changed = SliderFloat2(text, unsafe(addr(state.value.x)), mn, mx, format, flags)
    state.changed = changed
    pending_value_finalize(widget_ident, "slider_float2", state)
    return changed
}

[widget]
def slider_float3(var state : SliderStateFloat3; text : string;
                  format : string = "%.3f";
                  flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let (mn, mx) = state.bounds
    let changed = SliderFloat3(text, unsafe(addr(state.value.x)), mn, mx, format, flags)
    state.changed = changed
    pending_value_finalize(widget_ident, "slider_float3", state)
    return changed
}

[widget]
def slider_float4(var state : SliderStateFloat4; text : string;
                  format : string = "%.3f";
                  flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let (mn, mx) = state.bounds
    let changed = SliderFloat4(text, unsafe(addr(state.value.x)), mn, mx, format, flags)
    state.changed = changed
    pending_value_finalize(widget_ident, "slider_float4", state)
    return changed
}

[widget]
def slider_int(var state : SliderStateInt; text : string;
               format : string = "%d";
               flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let (mn, mx) = state.bounds
    let changed = SliderInt(text, unsafe(addr(state.value)), mn, mx, format, flags)
    state.changed = changed
    pending_value_finalize(widget_ident, "slider_int", state)
    return changed
}

[widget]
def slider_int2(var state : SliderStateInt2; text : string;
                format : string = "%d";
                flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let (mn, mx) = state.bounds
    let changed = SliderInt2(text, unsafe(addr(state.value.x)), mn, mx, format, flags)
    state.changed = changed
    pending_value_finalize(widget_ident, "slider_int2", state)
    return changed
}

[widget]
def slider_int3(var state : SliderStateInt3; text : string;
                format : string = "%d";
                flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let (mn, mx) = state.bounds
    let changed = SliderInt3(text, unsafe(addr(state.value.x)), mn, mx, format, flags)
    state.changed = changed
    pending_value_finalize(widget_ident, "slider_int3", state)
    return changed
}

[widget]
def slider_int4(var state : SliderStateInt4; text : string;
                format : string = "%d";
                flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let (mn, mx) = state.bounds
    let changed = SliderInt4(text, unsafe(addr(state.value.x)), mn, mx, format, flags)
    state.changed = changed
    pending_value_finalize(widget_ident, "slider_int4", state)
    return changed
}

[widget]
def vslider_float(var state : SliderStateFloat; text : string;
                  size : float2 = float2(40.0f, 200.0f);
                  format : string = "%.3f";
                  flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    //! Vertical slider; reuses SliderStateFloat. `size` is per-call (typical
    //! ~40×200 px); zero-size produces a 0×0 widget so caller-default beats
    //! struct-field-default which never fires on auto-emitted globals.
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let (mn, mx) = state.bounds
    let changed = VSliderFloat(text, size, unsafe(addr(state.value)), mn, mx, format, flags)
    state.changed = changed
    pending_value_finalize(widget_ident, "vslider_float", state)
    return changed
}

[widget]
def vslider_int(var state : SliderStateInt; text : string;
                size : float2 = float2(40.0f, 200.0f);
                format : string = "%d";
                flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    //! Vertical slider; reuses SliderStateInt. See vslider_float for `size` rationale.
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let (mn, mx) = state.bounds
    let changed = VSliderInt(text, size, unsafe(addr(state.value)), mn, mx, format, flags)
    state.changed = changed
    pending_value_finalize(widget_ident, "vslider_int", state)
    return changed
}

// ===== Stateful inputs (input numeric family) =====
//
// InputFloat/Int/Double don't clamp — no bounds field. Scalar forms
// expose step / step_fast (imgui's keyboard +/- buttons); vector forms
// don't (imgui doesn't support them on 2/3/4-component inputs). Int
// vectors don't take format either (imgui hardcodes "%d"). All flags
// are ImGuiInputTextFlags (NOT ImGuiSliderFlags) — Input* shares the
// text-edit widget machinery with InputText.

[widget]
def input_float(var state : InputStateFloat; text : string;
                step : float = 0.0f;
                step_fast : float = 0.0f;
                format : string = "%.3f";
                flags : ImGuiInputTextFlags = ImGuiInputTextFlags.None) : bool {
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let changed = InputFloat(text, unsafe(addr(state.value)), step, step_fast, format, flags)
    state.changed = changed
    pending_value_finalize(widget_ident, "input_float", state)
    return changed
}

[widget]
def input_float2(var state : InputStateFloat2; text : string;
                 format : string = "%.3f";
                 flags : ImGuiInputTextFlags = ImGuiInputTextFlags.None) : bool {
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let changed = InputFloat2(text, unsafe(addr(state.value.x)), format, flags)
    state.changed = changed
    pending_value_finalize(widget_ident, "input_float2", state)
    return changed
}

[widget]
def input_float3(var state : InputStateFloat3; text : string;
                 format : string = "%.3f";
                 flags : ImGuiInputTextFlags = ImGuiInputTextFlags.None) : bool {
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let changed = InputFloat3(text, unsafe(addr(state.value.x)), format, flags)
    state.changed = changed
    pending_value_finalize(widget_ident, "input_float3", state)
    return changed
}

[widget]
def input_float4(var state : InputStateFloat4; text : string;
                 format : string = "%.3f";
                 flags : ImGuiInputTextFlags = ImGuiInputTextFlags.None) : bool {
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let changed = InputFloat4(text, unsafe(addr(state.value.x)), format, flags)
    state.changed = changed
    pending_value_finalize(widget_ident, "input_float4", state)
    return changed
}

[widget]
def input_int(var state : InputStateInt; text : string;
              step : int = 1;
              step_fast : int = 100;
              flags : ImGuiInputTextFlags = ImGuiInputTextFlags.None) : bool {
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let changed = InputInt(text, unsafe(addr(state.value)), step, step_fast, flags)
    state.changed = changed
    pending_value_finalize(widget_ident, "input_int", state)
    return changed
}

[widget]
def input_int2(var state : InputStateInt2; text : string;
               flags : ImGuiInputTextFlags = ImGuiInputTextFlags.None) : bool {
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let changed = InputInt2(text, unsafe(addr(state.value.x)), flags)
    state.changed = changed
    pending_value_finalize(widget_ident, "input_int2", state)
    return changed
}

[widget]
def input_int3(var state : InputStateInt3; text : string;
               flags : ImGuiInputTextFlags = ImGuiInputTextFlags.None) : bool {
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let changed = InputInt3(text, unsafe(addr(state.value.x)), flags)
    state.changed = changed
    pending_value_finalize(widget_ident, "input_int3", state)
    return changed
}

[widget]
def input_int4(var state : InputStateInt4; text : string;
               flags : ImGuiInputTextFlags = ImGuiInputTextFlags.None) : bool {
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let changed = InputInt4(text, unsafe(addr(state.value.x)), flags)
    state.changed = changed
    pending_value_finalize(widget_ident, "input_int4", state)
    return changed
}

[widget]
def input_double(var state : InputStateDouble; text : string;
                 step : double = 0.0lf;
                 step_fast : double = 0.0lf;
                 format : string = "%.6f";
                 flags : ImGuiInputTextFlags = ImGuiInputTextFlags.None) : bool {
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let changed = InputDouble(text, unsafe(addr(state.value)), step, step_fast, format, flags)
    state.changed = changed
    pending_value_finalize(widget_ident, "input_double", state)
    return changed
}

// ===== Stateful inputs (color family) =====
//
// ColorEdit3/ColorPicker3 share ColorState3 (RGB float3); ColorEdit4 /
// ColorPicker4 share ColorState4 (RGBA float4). ColorButton is a click
// trigger — caller passes the swatch color per-frame, state holds only
// click bookkeeping (reuses ClickState).

[widget]
def color_edit3(var state : ColorState3; text : string;
                flags : ImGuiColorEditFlags = ImGuiColorEditFlags.None) : bool {
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let changed = ColorEdit3(text, unsafe(addr(state.value.x)), flags)
    state.changed = changed
    pending_value_finalize(widget_ident, "color_edit3", state)
    return changed
}

[widget]
def color_edit4(var state : ColorState4; text : string;
                flags : ImGuiColorEditFlags = ImGuiColorEditFlags.None) : bool {
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let changed = ColorEdit4(text, unsafe(addr(state.value.x)), flags)
    state.changed = changed
    pending_value_finalize(widget_ident, "color_edit4", state)
    return changed
}

[widget]
def color_picker3(var state : ColorState3; text : string;
                  flags : ImGuiColorEditFlags = ImGuiColorEditFlags.None) : bool {
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let changed = ColorPicker3(text, unsafe(addr(state.value.x)), flags)
    state.changed = changed
    pending_value_finalize(widget_ident, "color_picker3", state)
    return changed
}

[widget]
def color_picker4(var state : ColorState4; text : string;
                  flags : ImGuiColorEditFlags = ImGuiColorEditFlags.None;
                  ref_col : float4 = float4(0.0f, 0.0f, 0.0f, 0.0f);
                  has_ref_col : bool = false) : bool {
    //! Optional reference color. Pass `has_ref_col=true` plus `ref_col`
    //! to surface ImGui's "[ref]" swatch alongside the picker; default is
    //! null (no reference). Local copy gives the addr a frame-stable
    //! address; ImGui reads ref_col synchronously inside ColorPicker4.
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    var ref_local = ref_col
    let ref_ptr = has_ref_col ? unsafe(addr(ref_local.x)) : null
    let changed = ColorPicker4(text, unsafe(addr(state.value.x)), flags, ref_ptr)
    state.changed = changed
    pending_value_finalize(widget_ident, "color_picker4", state)
    return changed
}

// ===== Stateful inputs (choice family) =====
//
// Both widgets take `items : array<string>` and marshal it C-side without
// a shim. `combo` packs a zero-separated buffer on `state.items_buf`
// (capacity reuses across frames) and reinterprets the byte addr as
// `string#` for ImGui's null-separated Combo overload — daslang `string`
// can't itself carry embedded nulls (heap is sized via strlen), but a raw
// pointer into an `array<uint8>` bypasses that entirely. `list_box` passes
// the daslang array storage directly: each `array<string>` element IS a
// `char *`, so `addr(items[0])` already matches `const char* const items[]`.
// Block-arg surfaces (BeginCombo + manual Selectable loop, BeginListBox)
// are container-shaped and land in 0b.3.

[widget]
def combo(var state : ComboState; text : string; items : array<string>;
          popup_max_height_in_items : int = -1) : bool {
    //! `imgui_set {"value": N}` selects the N-th item next frame.
    //! `popup_max_height_in_items = -1` lets ImGui pick a default.
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    state.items_buf |> clear()
    for (s in items) {
        peek_data(s) $(arr) {
            for (b in arr) {
                state.items_buf |> push(b)
            }
        }
        state.items_buf |> push(0u8)
    }
    state.items_buf |> push(0u8)            // ImGui scans for double-null = end of list
    let zsv = unsafe(reinterpret<string#>(unsafe(addr(state.items_buf[0]))))
    let changed = Combo(text, unsafe(addr(state.value)),
                        zsv, popup_max_height_in_items)
    state.changed = changed
    pending_value_finalize(widget_ident, "combo", state)
    return changed
}

[widget]
def list_box(var state : ListBoxState; text : string; items : array<string>;
             height_in_items : int = -1) : bool {
    //! `imgui_set {"value": N}` selects the N-th item next frame.
    //! `height_in_items = -1` lets ImGui pick a default; positive caps the visible rows.
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let changed = ListBox(text, unsafe(addr(state.value)),
                          unsafe(addr(items[0])), length(items), height_in_items)
    state.changed = changed
    pending_value_finalize(widget_ident, "list_box", state)
    return changed
}

[widget]
def color_button(var state : ClickState; desc_id : string;
                 col : float4;
                 size : float2 = float2(0.0f, 0.0f);
                 flags : ImGuiColorEditFlags = ImGuiColorEditFlags.None) : bool {
    //! Click-trigger swatch. The displayed color is per-call (caller
    //! controls it externally — typical use is a row of style swatches);
    //! state holds only click bookkeeping. `size = (0, 0)` lets ImGui
    //! pick a default square. Param order puts size before flags so
    //! callers can pass `(desc_id, col, size = …)` without an explicit
    //! `flags = None` middle arg (named-tuple positional dispatch).
    let im_clicked = ColorButton(desc_id, col, flags, size)
    var clicked = im_clicked
    if (state.pending_click) {
        state.pending_click = false
        clicked = true
    }
    state.clicked = clicked
    if (clicked) {
        state.click_count++
    }
    click_finalize(widget_ident, "color_button", state)
    return clicked
}

// ===== Text-input family (Phase 0b.4) =====
//
// The buffer-as-pointer path: state owns `array<uint8> buffer`, the widget
// passes `addr(buffer[0])` to the C extern, and ImGui mutates it in place.
// `value` mirrors the buffer; we re-clone after every frame ImGui reports
// `changed`. `state.buffer` is `@optional`, so it's dropped on hot-reload —
// the (re)allocate-and-sync below covers both first-frame init and post-
// reload restore.
//
// `input_text_growable` is the one callback variant we ship — uses
// `_builtin_InputText_cb` with a CallbackResize lambda that grows
// `state.buffer` past the initial capacity. All other ImGui callback
// flavors (history / completion / always / char-filter) stay in the v1
// `daslib/imgui_boost` surface.

def private input_text_buffer_init(var state : InputTextState) {
    if (length(state.buffer) != state.capacity) {
        state.buffer |> resize(state.capacity)
        sync_buffer_from_value(state)
    }
}

def private sync_buffer_from_value(var state : InputTextState) {
    // Zero the buffer then copy bytes-of-`value` (clamped to capacity-1
    // so the trailing null is preserved). `length(string)` is byte count.
    for (i in range(state.capacity)) {
        state.buffer[i] = 0u8
    }
    let want = length(state.value)
    if (want > 0) {
        let n = min(want, state.capacity - 1)
        unsafe {
            memcpy(addr(state.buffer[0]), reinterpret<void?>(state.value), n)
        }
    }
}

def private sync_value_from_buffer(var state : InputTextState) {
    unsafe {
        let pbuf = addr(state.buffer[0])
        state.value := clone_string(reinterpret<string>(pbuf))
    }
}

def private apply_pending_text(var state : InputTextState) {
    if (state.has_pending) {
        state.value := state.pending_value
        state.has_pending = false
        sync_buffer_from_value(state)
    }
}

[widget]
def input_text(var state : InputTextState; text : string;
               flags : ImGuiInputTextFlags = ImGuiInputTextFlags.None) : bool {
    //! Single-line text input. `imgui_set {"value":"…"}` overwrites the
    //! buffer next frame. Buffer is fixed-size (`state.capacity`, default
    //! 256). For grow-on-overflow use `input_text_growable`.
    input_text_buffer_init(state)
    apply_pending_text(state)
    let changed = _builtin_InputText_basic(unsafe(addr(state.buffer[0])), state.capacity, text, flags)
    state.changed = changed
    if (changed) {
        sync_value_from_buffer(state)
    }
    text_value_finalize(widget_ident, "input_text", state)
    return changed
}

[widget]
def input_text_with_hint(var state : InputTextState; text : string; hint : string;
                         flags : ImGuiInputTextFlags = ImGuiInputTextFlags.None) : bool {
    //! Same as `input_text` but renders `hint` as a placeholder when the
    //! buffer is empty.
    input_text_buffer_init(state)
    apply_pending_text(state)
    let changed = _builtin_InputTextWithHint_basic(unsafe(addr(state.buffer[0])), state.capacity, text, hint, flags)
    state.changed = changed
    if (changed) {
        sync_value_from_buffer(state)
    }
    text_value_finalize(widget_ident, "input_text_with_hint", state)
    return changed
}

[widget]
def input_text_multiline(var state : InputTextState; text : string;
                         size : float2 = float2(0.0f, 0.0f);
                         flags : ImGuiInputTextFlags = ImGuiInputTextFlags.None) : bool {
    //! Multi-line text input (CR/LF-tolerant). `size = (0, 0)` lets ImGui
    //! pick a default rect — typically wide-but-short. Pass an explicit
    //! size for editor-style panels.
    input_text_buffer_init(state)
    apply_pending_text(state)
    let changed = _builtin_InputTextMultiline_basic(unsafe(addr(state.buffer[0])), state.capacity, text, size, flags)
    state.changed = changed
    if (changed) {
        sync_value_from_buffer(state)
    }
    text_value_finalize(widget_ident, "input_text_multiline", state)
    return changed
}

[widget]
def input_text_growable(var state : InputTextState; text : string;
                        flags : ImGuiInputTextFlags = ImGuiInputTextFlags.None) : bool {
    //! Single-line input that grows `state.buffer` whenever the user types
    //! past `state.capacity`. Plumbs ImGui's CallbackResize through a daslang
    //! lambda that resizes the array and re-points `data.Buf`. The thunk
    //! holding (Context*, LineInfo*, lambda) lives on the C call stack —
    //! ImGui invokes the callback synchronously inside InputText, so no
    //! ABI pin needed. `imgui_set` with a value larger than `state.capacity`
    //! also grows — without that the user-visible "growable" promise leaks.
    input_text_buffer_init(state)
    if (state.has_pending) {
        state.value := state.pending_value
        state.has_pending = false
        let want = length(state.value) + 1
        if (want > state.capacity) {
            state.capacity = want
            state.buffer |> resize(state.capacity)
        }
        sync_buffer_from_value(state)
    }
    let cb_flags = flags | ImGuiInputTextFlags.CallbackResize
    var changed : bool
    unsafe {
        let resize_cb <- @ capture(& state) (var data : ImGuiInputTextCallbackData) : int {
            if (data.EventFlag == ImGuiInputTextFlags.CallbackResize) {
                let new_size = data.BufSize
                state.buffer |> resize(new_size)
                state.capacity = new_size
                unsafe {
                    data.Buf = reinterpret<string>(addr(state.buffer[0]))
                }
            }
            return 0
        }
        changed = _builtin_InputText_cb(addr(state.buffer[0]), state.capacity, text, cb_flags, resize_cb)
    }
    state.changed = changed
    if (changed) {
        sync_value_from_buffer(state)
    }
    text_value_finalize(widget_ident, "input_text_growable", state)
    return changed
}

// ===== Combo (callback-getter) =====

[widget]
def combo_getter(var state : ComboState; text : string;
                 items_count : int;
                 getter : lambda<(idx : int; var result : string&) : bool>;
                 popup_max_height_in_items : int = -1) : bool {
    //! Procedural combo — the dropdown items are produced by a per-frame
    //! lambda instead of an `array<string>`. Useful when the list is
    //! large, virtualized, or computed (filter results, RTTI enums).
    //! `getter(idx, result)` should set `result` to the idx-th item label
    //! and return true; return false to stop. The lambda fires
    //! synchronously inside the ImGui call.
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let changed = _builtin_Combo_cb(unsafe(addr(state.value)), text, items_count,
                                    popup_max_height_in_items, getter)
    state.changed = changed
    pending_value_finalize(widget_ident, "combo_getter", state)
    return changed
}

// ===== Display: text_show =====
//
// Read-only display widget (master plan §4.3a). The standard serializer
// surfaces `state.value`, so playwright tests can `expect_value(STATUS,
// "saved")` without any extra plumbing. The dispatcher accepts "set" so
// tests can also drive the value externally — handy for setting up
// fixtures programmatically.

def private text_show_finalize(widget_ident : string; kind : string; var state : TextShowState) {
    var entry : WidgetEntry
    unsafe {
        let ser <- @ capture(& state) () : JsonValue ? {
            return JV(state)
        }
        let disp <- @ capture(& state) (action : string; payload : JsonValue ?) {
            if (action == "set") {
                state.value := payload?["value"] ?? ""
            }
        }
        widget_finalize(widget_ident, kind, entry, ser, disp)
    }
}

[widget]
def text_show(var state : TextShowState; value : string = "") {
    //! Display-only widget surfaced through telemetry. Two forms:
    //!   1. `text_show(STATUS, value="saved")` — sets state.value to the
    //!      argument every frame (call-site is the source of truth).
    //!   2. `text_show(STATUS)` — displays whatever's in state.value
    //!      (set programmatically by `STATUS.value = compute()` or
    //!      remotely via `imgui_set`).
    //! Either form serializes `state.value` for `expect_value` assertions.
    if (!empty(value)) {
        state.value := value
    }
    Text(state.value)
    text_show_finalize(widget_ident, "text_show", state)
}

// ===== Plot lines / histogram (array form) =====
//
// Display widgets backed by `PlotState`. Plotted values come per-frame as
// an `array<float>` parameter (not stored on state) — same shape as
// daslib v1 and imgui_test_engine. State tracks last-frame's call args
// (title, bounds, sample count) for telemetry. No dispatcher.
//
// Lambda-driven getter form (plot_lines_getter / plot_histogram_getter):
// deferred to Phase 2. Audit outcome (1.6): the existing C++
// ImGuiPlotGetter struct in src/dasIMGUI.main.cpp is still wired into
// the v1 daslib/imgui_boost.das + example/imgui_demo.das via the
// older `vec4f igpg` reinterpret pattern, and per master plan §9.4 we
// keep v1 installable until users migrate. So the right move is NOT to
// rewrite the existing struct but to add a parallel lambda-thunk
// binding (mirroring 0b.4 ComboCb) when the new public API surface
// lands in Phase 2 — at which point the new boost gets its own
// _builtin_PlotLines_cb / _builtin_PlotHistogram_cb forwarders.

def private plot_finalize(widget_ident : string; kind : string; var state : PlotState) {
    var entry : WidgetEntry
    unsafe {
        let ser <- @ capture(& state) () : JsonValue ? {
            return JV(state)
        }
        let disp <- @ capture(& state) (action : string; payload : JsonValue ?) {
            // Plot is read-only; no L2/L3 actions. Dispatcher is a no-op.
        }
        widget_finalize(widget_ident, kind, entry, ser, disp)
    }
}

[widget]
def plot_lines(var state : PlotState; text : string;
               values : array<float>;
               scale_min : float = 3.40282347e38f;       // FLT_MAX → auto-fit
               scale_max : float = 3.40282347e38f;
               graph_size : float2 = float2(0.0f, 0.0f)) {
    state.title = text
    state.bounds = (scale_min, scale_max)
    state.samples = length(values)
    if (!empty(values)) {
        unsafe {
            imgui::PlotLines(text, addr(values[0]), length(values), 0,
                             "", scale_min, scale_max, graph_size,
                             typeinfo sizeof(type<float>))
        }
    } else {
        Text("(empty: {text})")
    }
    plot_finalize(widget_ident, "plot_lines", state)
}

[widget]
def plot_histogram(var state : PlotState; text : string;
                   values : array<float>;
                   scale_min : float = 3.40282347e38f;
                   scale_max : float = 3.40282347e38f;
                   graph_size : float2 = float2(0.0f, 0.0f)) {
    state.title = text
    state.bounds = (scale_min, scale_max)
    state.samples = length(values)
    if (!empty(values)) {
        unsafe {
            imgui::PlotHistogram(text, addr(values[0]), length(values), 0,
                                 "", scale_min, scale_max, graph_size,
                                 typeinfo sizeof(type<float>))
        }
    } else {
        Text("(empty: {text})")
    }
    plot_finalize(widget_ident, "plot_histogram", state)
}

// ===== Plot getters (callback) =====
//
// Procedural variants of plot_lines / plot_histogram — `getter(idx) :
// float` produces sample values per frame instead of a backing
// `array<float>`. Useful for synthesized signals, virtualized buffers, or
// computed series. The lambda fires synchronously inside the C call;
// thunk lifetime is bounded by the call.

[widget]
def plot_lines_getter(var state : PlotState; text : string;
                      values_count : int;
                      getter : lambda<(idx : int) : float>;
                      overlay_text : string = "";
                      scale_min : float = 3.40282347e38f;
                      scale_max : float = 3.40282347e38f;
                      graph_size : float2 = float2(0.0f, 0.0f)) {
    state.title = text
    state.bounds = (scale_min, scale_max)
    state.samples = values_count
    _builtin_PlotLines_cb(text, values_count, 0, overlay_text,
                          scale_min, scale_max, graph_size, getter)
    plot_finalize(widget_ident, "plot_lines_getter", state)
}

[widget]
def plot_histogram_getter(var state : PlotState; text : string;
                          values_count : int;
                          getter : lambda<(idx : int) : float>;
                          overlay_text : string = "";
                          scale_min : float = 3.40282347e38f;
                          scale_max : float = 3.40282347e38f;
                          graph_size : float2 = float2(0.0f, 0.0f)) {
    state.title = text
    state.bounds = (scale_min, scale_max)
    state.samples = values_count
    _builtin_PlotHistogram_cb(text, values_count, 0, overlay_text,
                              scale_min, scale_max, graph_size, getter)
    plot_finalize(widget_ident, "plot_histogram_getter", state)
}
