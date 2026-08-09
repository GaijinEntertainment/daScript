options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options _allow_imgui_legacy = true

module imgui_widgets_builtin shared

require imgui public
require imgui/imgui_boost_runtime public
require imgui/imgui_boost_v2 public
require imgui/imgui_icons
require daslib/json public
require daslib/json_boost
require strings
require math

//! Built-in widgets defined via `[widget]` — the same mechanism users get.
//! Each def hands ``(state_addr, ti)`` (the state's address + its runtime
//! ``TypeInfo``) to ``widget_finalize``; common per-frame fields (hex_id/
//! bbox/hover/active/focus) are captured inside widget_finalize; per-kind
//! state lives in the state struct alone.
//!
//! HTTP dispatch goes through ``g_dispatchers`` keyed by ``(*ti).hash`` (the
//! state struct's runtime type), populated by the ``[widget_dispatch]``
//! defs paired with each state struct further down. One ``[widget_dispatch]``
//! body covers every widget kind that shares that state — e.g. one
//! ToggleState dispatcher handles ``checkbox`` / ``radio_button`` /
//! ``selectable`` / ``menu_item``. ClickState (the button family) carries no
//! dispatcher — an ``imgui_click`` is a real synthetic mouse click. No
//! per-widget serializer or dispatcher lambda is emitted; snapshot reads
//! route through ``sprint_json_at(state_addr, *ti)`` and set-actions
//! through ``sscan_json_at(state_addr, *ti)``.

// ===== Common finalize helpers =====
//
// Generic state-addr+ti plumbing for [widget]/[container] state structs.
// Pull (state_addr, ti) out of a state-by-ref param exactly once, then route
// to widget_finalize / container_finalize. The kind-specific action handlers
// live in [widget_dispatch] defs near each state struct's call sites; HTTP
// dispatch matches them by ti.hash via g_dispatchers in imgui_boost_runtime.

def private finalize_state(widget_ident : string; kind : string;
                           var state : auto(ST); var entry : WidgetEntry) {
    unsafe {
        widget_finalize(widget_ident, kind, entry,
                        addr(state),
                        reinterpret<TypeInfo const?>(typeinfo rtti_typeinfo(type<ST>)))
    }
}

def private finalize_container_state(widget_ident : string; kind : string;
                                     var state : auto(ST); var entry : WidgetEntry) {
    unsafe {
        container_finalize(widget_ident, kind, entry,
                           addr(state),
                           reinterpret<TypeInfo const?>(typeinfo rtti_typeinfo(type<ST>)))
    }
}

// Keep the snapshot-visible label current without per-frame allocation: the
// visible part is everything before "##"; reassign only when it changed.
def private update_widget_text(var target : string&; text : string) {
    var cut = find(text, "##")
    if (cut < 0) {
        cut = length(text)
    }
    if (length(target) == cut && (cut == 0 || starts_with(text, target))) return
    target = slice(text, 0, cut)
}

def private click_finalize(widget_ident : string; kind : string; var state : ClickState) {
    //! Click-style finalize (Button family). State path; dispatch handled per-kind via [widget_dispatch].
    var entry : WidgetEntry
    finalize_state(widget_ident, kind, state, entry)
    register_focusable(widget_ident)
}

def private toggle_finalize(widget_ident : string; kind : string; var state : ToggleState) {
    //! Bool-toggle finalize (Checkbox, RadioButton-bool, Selectable, MenuItem). State path; dispatch handled per-kind via [widget_dispatch].
    var entry : WidgetEntry
    finalize_state(widget_ident, kind, state, entry)
    register_focusable(widget_ident)
}

def private radio_int_finalize(widget_ident : string; kind : string; var state : RadioIntState; v_button : int) {
    //! Int-form RadioButton finalize; also registers a per-site geometry alias `<path>#<v_button>` so each option is independently click-/test-targetable.
    var entry : WidgetEntry
    finalize_state(widget_ident, kind, state, entry)
    register_focusable(widget_ident)
    unsafe {
        register_radio_site_alias(widget_ident, v_button, addr(state),
                                  reinterpret<TypeInfo const?>(typeinfo rtti_typeinfo(type<RadioIntState>)))
    }
}

def private text_value_finalize(widget_ident : string; kind : string; var state : InputTextState) {
    //! String-pending finalize. State path; dispatch handled via [widget_dispatch] (clones `value` string into the context heap).
    var entry : WidgetEntry
    finalize_state(widget_ident, kind, state, entry)
    register_focusable(widget_ident)
}

def edit_finalize(widget_ident : string; kind : string; var ptr : auto(TT)? implicit) {
    //! External-pointer finalize: writes (ptr, ti=TypeInfo of TT) into g_widgets so
    //! snapshot + dispatch route through `*ptr` via sprint_json_at / sscan_json_at.
    //! Caller owns the pointee lifetime; pass `safe_addr(local)` or `unsafe(addr(struct.field))`.
    var entry : WidgetEntry
    unsafe {
        widget_finalize(widget_ident, kind, entry,
                        reinterpret<void?>(ptr),
                        reinterpret<TypeInfo const?>(typeinfo rtti_typeinfo(type<TT>)))
    }
    register_focusable(widget_ident)
}

def edit_finalize_subs(widget_ident : string; kind : string; var ptr : auto(TT)? implicit;
                       var subs : array<float4>) {
    //! edit_finalize plus a caller-built per-item sub_bboxes array (external-pointer combo): open-popup
    //! item rects captured one-per-Selectable so a driver can target item N. Empty subs (popup closed)
    //! falls back to the combo box itself. `subs` is moved out; pointer/ti routing as edit_finalize.
    var entry : WidgetEntry
    if (!empty(subs)) {
        entry.sub_bboxes <- subs
    }
    unsafe {
        widget_finalize(widget_ident, kind, entry,
                        reinterpret<void?>(ptr),
                        reinterpret<TypeInfo const?>(typeinfo rtti_typeinfo(type<TT>)))
    }
    register_focusable(widget_ident)
}

def edit_container_open_close_finalize(widget_ident : string; kind : string;
                                       var ptr : bool? implicit;
                                       bbox : float4 = float4(0.0f, 0.0f, 0.0f, 0.0f)) {
    //! [edit_container] finalize for bool-typed open/close pointers. Pointer + ti go into g_widgets so the per-kind [widget_dispatch] handles "set"/"open"/"close" by writing through ``*ptr``.
    var entry : WidgetEntry
    capture_item_bbox(entry, bbox)
    unsafe {
        container_finalize(widget_ident, kind, entry,
                           reinterpret<void?>(ptr),
                           reinterpret<TypeInfo const?>(typeinfo rtti_typeinfo(type<bool>)))
    }
}

// ===== Per-state-struct dispatchers =====

// One ``[widget_dispatch]`` body per state struct — keyed by the first param's struct type, so each
// fires for every kind sharing that struct. Click-only structs (ClickState) get none (a real synthetic
// click is handled before dispatch); display-only structs get none (dispatch_or_err succeeds silently).

[widget_dispatch]
def _toggle_dispatch(var state : ToggleState; action : string; payload : string) {
    if (action == "set") {
        unsafe {
            sscan_json_at(payload, addr(state.pending_value),
                          *reinterpret<TypeInfo const?>(typeinfo rtti_typeinfo(type<bool>)))
        }
        state.pending_toggle = true
    }
}

[widget_dispatch]
def _radio_int_dispatch(var state : RadioIntState; action : string; payload : string) {
    if (action == "set") {
        unsafe {
            sscan_json_at(payload, addr(state.pending_value),
                          *reinterpret<TypeInfo const?>(typeinfo rtti_typeinfo(type<int>)))
        }
        state.pending_set = true
    }
}

[widget_dispatch]
def _input_text_dispatch(var state : InputTextState; action : string; payload : string) {
    if (action == "set") {
        var v : string
        unsafe {
            sscan_json_at(payload, addr(v),
                          *reinterpret<TypeInfo const?>(typeinfo rtti_typeinfo(type<string>)))
        }
        state.pending_value := v
        state.has_pending = true
    }
}

[widget_dispatch]
def _text_show_dispatch(var state : TextShowState; action : string; payload : string) {
    if (action == "set") {
        var v : string
        unsafe {
            sscan_json_at(payload, addr(v),
                          *reinterpret<TypeInfo const?>(typeinfo rtti_typeinfo(type<string>)))
        }
        state.value := v
    }
}

[widget_dispatch]
def _tree_node_dispatch(var state : TreeNodeState; action : string; payload : string) {
    if (action == "open") {
        state.pending_open = true
    } elif (action == "close") {
        state.pending_close = true
    }
}

// pending_value family — shared body, one thin dispatch per state struct.

def private pending_value_apply(var s : auto(S); payload : string) {
    unsafe {
        sscan_json_at(payload, addr(s.pending_value),
                      *reinterpret<TypeInfo const?>(typeinfo rtti_typeinfo(type<typedecl(s.pending_value)>)))
    }
    s.has_pending = true
}

[widget_dispatch]
def _pv_drag_float(var state : DragStateFloat; action : string; payload : string) {
    if (action == "set") {
        pending_value_apply(state, payload)
    }
}
[widget_dispatch]
def _pv_drag_float2(var state : DragStateFloat2; action : string; payload : string) {
    if (action == "set") {
        pending_value_apply(state, payload)
    }
}
[widget_dispatch]
def _pv_drag_float3(var state : DragStateFloat3; action : string; payload : string) {
    if (action == "set") {
        pending_value_apply(state, payload)
    }
}
[widget_dispatch]
def _pv_drag_float4(var state : DragStateFloat4; action : string; payload : string) {
    if (action == "set") {
        pending_value_apply(state, payload)
    }
}
[widget_dispatch]
def _pv_drag_int(var state : DragStateInt; action : string; payload : string) {
    if (action == "set") {
        pending_value_apply(state, payload)
    }
}
[widget_dispatch]
def _pv_drag_int2(var state : DragStateInt2; action : string; payload : string) {
    if (action == "set") {
        pending_value_apply(state, payload)
    }
}
[widget_dispatch]
def _pv_drag_int3(var state : DragStateInt3; action : string; payload : string) {
    if (action == "set") {
        pending_value_apply(state, payload)
    }
}
[widget_dispatch]
def _pv_drag_int4(var state : DragStateInt4; action : string; payload : string) {
    if (action == "set") {
        pending_value_apply(state, payload)
    }
}
[widget_dispatch]
def _pv_drag_range_float(var state : DragStateRangeFloat; action : string; payload : string) {
    if (action == "set") {
        pending_value_apply(state, payload)
    }
}
[widget_dispatch]
def _pv_drag_range_int(var state : DragStateRangeInt; action : string; payload : string) {
    if (action == "set") {
        pending_value_apply(state, payload)
    }
}
[widget_dispatch]
def _pv_slider_float(var state : SliderStateFloat; action : string; payload : string) {
    if (action == "set") {
        pending_value_apply(state, payload)
    }
}
[widget_dispatch]
def _pv_slider_float2(var state : SliderStateFloat2; action : string; payload : string) {
    if (action == "set") {
        pending_value_apply(state, payload)
    }
}
[widget_dispatch]
def _pv_slider_float3(var state : SliderStateFloat3; action : string; payload : string) {
    if (action == "set") {
        pending_value_apply(state, payload)
    }
}
[widget_dispatch]
def _pv_slider_float4(var state : SliderStateFloat4; action : string; payload : string) {
    if (action == "set") {
        pending_value_apply(state, payload)
    }
}
[widget_dispatch]
def _pv_slider_int(var state : SliderStateInt; action : string; payload : string) {
    if (action == "set") {
        pending_value_apply(state, payload)
    }
}
[widget_dispatch]
def _pv_slider_int2(var state : SliderStateInt2; action : string; payload : string) {
    if (action == "set") {
        pending_value_apply(state, payload)
    }
}
[widget_dispatch]
def _pv_slider_int3(var state : SliderStateInt3; action : string; payload : string) {
    if (action == "set") {
        pending_value_apply(state, payload)
    }
}
[widget_dispatch]
def _pv_slider_int4(var state : SliderStateInt4; action : string; payload : string) {
    if (action == "set") {
        pending_value_apply(state, payload)
    }
}
[widget_dispatch]
def _pv_input_float(var state : InputStateFloat; action : string; payload : string) {
    if (action == "set") {
        pending_value_apply(state, payload)
    }
}
[widget_dispatch]
def _pv_input_float2(var state : InputStateFloat2; action : string; payload : string) {
    if (action == "set") {
        pending_value_apply(state, payload)
    }
}
[widget_dispatch]
def _pv_input_float3(var state : InputStateFloat3; action : string; payload : string) {
    if (action == "set") {
        pending_value_apply(state, payload)
    }
}
[widget_dispatch]
def _pv_input_float4(var state : InputStateFloat4; action : string; payload : string) {
    if (action == "set") {
        pending_value_apply(state, payload)
    }
}
[widget_dispatch]
def _pv_input_int(var state : InputStateInt; action : string; payload : string) {
    if (action == "set") {
        pending_value_apply(state, payload)
    }
}
[widget_dispatch]
def _pv_input_int2(var state : InputStateInt2; action : string; payload : string) {
    if (action == "set") {
        pending_value_apply(state, payload)
    }
}
[widget_dispatch]
def _pv_input_int3(var state : InputStateInt3; action : string; payload : string) {
    if (action == "set") {
        pending_value_apply(state, payload)
    }
}
[widget_dispatch]
def _pv_input_int4(var state : InputStateInt4; action : string; payload : string) {
    if (action == "set") {
        pending_value_apply(state, payload)
    }
}
[widget_dispatch]
def _pv_input_double(var state : InputStateDouble; action : string; payload : string) {
    if (action == "set") {
        pending_value_apply(state, payload)
    }
}
[widget_dispatch]
def _pv_color3(var state : ColorState3; action : string; payload : string) {
    if (action == "set") {
        pending_value_apply(state, payload)
    }
}
[widget_dispatch]
def _pv_color4(var state : ColorState4; action : string; payload : string) {
    if (action == "set") {
        pending_value_apply(state, payload)
    }
}
[widget_dispatch]
def _pv_combo(var state : ComboState; action : string; payload : string) {
    if (action == "set") {
        pending_value_apply(state, payload)
    }
}
[widget_dispatch]
def _pv_listbox(var state : ListBoxState; action : string; payload : string) {
    if (action == "set") {
        pending_value_apply(state, payload)
    }
}


// ===== Triggers (click family) =====

[widget]
def button(var state : ClickState; text : string;
           size : float2 = float2(0.0f, 0.0f)) : bool {
    let clicked = Button(text, ImVec2(size.x, size.y))
    state.clicked = clicked
    if (clicked) {
        state.click_count++
    }
    update_widget_text(state.text, text)
    click_finalize(widget_ident, "button", state)
    return clicked
}

[widget]
def icon_button(var state : ClickState; glyph : string;
                tip : string = ""; size : float = 20.0f; weight : float = 1.0f) : bool {
    //! Themed square icon button from imgui_icons (`glyph` = icon name); click semantics mirror `button`.
    //! A non-empty `tip` shows a hover tooltip. `size` is the glyph box side in px (base*content_scale
    //! for HDPI), `weight` scales stroke thickness. Args are positional in declared order.
    let clicked = icon_button_draw(widget_ident, glyph, size, weight)
    if (!empty(tip) && BeginItemTooltip()) {
        TextUnformatted(tip)
        EndTooltip()
    }
    state.clicked = clicked
    if (clicked) {
        state.click_count++
    }
    update_widget_text(state.text, glyph)
    click_finalize(widget_ident, "icon_button", state)
    return clicked
}

[widget]
def tab_item_button(var state : ClickState; text : string;
                    flags : ImGuiTabItemFlags = ImGuiTabItemFlags.None) : bool {
    //! `TabItemButton(label, flags)` — a button styled as a tab, no content pane (unlike `tab_item`);
    //! use for Leading-/Trailing-anchored controls (e.g. "+" to add tabs). Click semantics mirror
    //! `button`. Must be called INSIDE a `tab_bar` body.
    let clicked = TabItemButton(text, flags)
    state.clicked = clicked
    if (clicked) {
        state.click_count++
    }
    click_finalize(widget_ident, "tab_item_button", state)
    return clicked
}

[widget]
def small_button(var state : ClickState; text : string) : bool {
    let clicked = SmallButton(text)
    state.clicked = clicked
    if (clicked) {
        state.click_count++
    }
    update_widget_text(state.text, text)
    click_finalize(widget_ident, "small_button", state)
    return clicked
}

[widget]
def arrow_button(var state : ClickState; text : string; dir : ImGuiDir) : bool {
    let clicked = ArrowButton(text, dir)
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
    let clicked = InvisibleButton(text, size, flags)
    state.clicked = clicked
    if (clicked) {
        state.click_count++
    }
    click_finalize(widget_ident, "invisible_button", state)
    return clicked
}

[widget]
def image_button(var state : ClickState; text : string; user_texture_id : ImTextureRef; size : float2;
                 uv0 : float2 = float2(0.0f, 0.0f);
                 uv1 : float2 = float2(1.0f, 1.0f);
                 bg_col : float4 = float4(0.0f, 0.0f, 0.0f, 0.0f);
                 tint_col : float4 = float4(1.0f, 1.0f, 1.0f, 1.0f)) : bool {
    let clicked = ImageButton(text, user_texture_id, size, uv0, uv1, bg_col, tint_col)
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
    update_widget_text(state.text, text)
    toggle_finalize(widget_ident, "checkbox", state)
    return changed
}

[widget]
def radio_button(var state : ToggleState; text : string) : bool {
    //! Bool-form RadioButton — single toggle. ImGui's RadioButton(label,bool) doesn't mutate the bool,
    //! so the daslang side flips state.value on click ("single radio = checkbox shaped like a radio").
    //! Grouped radios use radio_button_int.
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
    update_widget_text(state.text, text)
    toggle_finalize(widget_ident, "selectable", state)
    return changed
}

[widget]
def selectable_hover(var state : EmptyMarkerState; text : string;
                     flags : ImGuiSelectableFlags = ImGuiSelectableFlags.None;
                     size : float2 = float2(0.0f, 0.0f)) : bool {
    //! Hover-target selectable. Like `selectable` but `selected` is pinned false and never toggles —
    //! use when the row's hover surface drives a side-effect and `selectable`'s click semantics would
    //! be wrong. Returns true when the just-drawn item is hovered this frame.
    Selectable(text, false, flags, size)
    let hovered = IsItemHovered()
    marker_finalize(widget_ident, "selectable_hover", state)
    return hovered
}

[widget]
def menu_item(var state : ToggleState; text : string;
              shortcut : string = ""; enabled : bool = true) : bool {
    //! Toggleable menu item — `state.value` is the check-mark state (persists across reload); click
    //! toggles, `enabled=false` greys out and disables clicks. For static (non-toggleable) items use
    //! the sibling `menu_label`.
    if (state.pending_toggle) {
        state.value = state.pending_value
        state.pending_toggle = false
    }
    let changed = MenuItem(text, shortcut, unsafe(addr(state.value)), enabled)
    state.changed = changed
    update_widget_text(state.text, text)
    toggle_finalize(widget_ident, "menu_item", state)
    return changed
}

[widget]
def menu_label(var state : ClickState; text : string;
               shortcut : string = ""; selected : bool = false;
               enabled : bool = true) : bool {
    //! Static (non-toggleable) menu item. `selected` is a caller-owned bool the widget never mutates —
    //! use for section headers (`enabled=false`) and permanent-state labels (`selected=true`). For
    //! toggleable items use `menu_item`.
    let clicked = MenuItem(text, shortcut, selected, enabled)
    state.clicked = clicked
    if (clicked) {
        state.click_count++
    }
    update_widget_text(state.text, text)
    click_finalize(widget_ident, "menu_label", state)
    return clicked
}

[widget]
def selectable_label(var state : ClickState; text : string; selected : bool;
                     flags : ImGuiSelectableFlags = ImGuiSelectableFlags.None;
                     size : float2 = float2(0.0f, 0.0f)) : bool {
    //! Selectable row whose selected-appearance is a caller-owned bool — reports clicks but never
    //! toggles its own state. Use for exclusive selection (`selectable_label(ROW[i], name, group.value
    //! == i)`); for the self-toggling form use `selectable`. Parallels `menu_label` vs `menu_item`.
    let clicked = Selectable(text, selected, flags, size)
    state.clicked = clicked
    if (clicked) {
        state.click_count++
    }
    update_widget_text(state.text, text)
    click_finalize(widget_ident, "selectable_label", state)
    return clicked
}

// ===== Tree leaf form =====

// `tree_node_ex` wraps a bare `TreeNodeEx` and returns the open bool WITHOUT claiming a `TreePop()`:
// the caller must pair `TreePop()` when the node opened AND `flags` lack `NoTreePushOnOpen`. Choose it
// over the `tree_node` container when you need post-header most-recent-item state (IsItemClicked, etc.).

[widget]
def tree_node_ex(var state : TreeNodeState; text : string;
                 flags : ImGuiTreeNodeFlags) : bool {
    //! Leaf-form `TreeNodeEx` — returns the open bool, caller pairs `TreePop()`
    //! manually when needed. See section comment above for when to choose this
    //! over the `tree_node` container.
    if (state.pending_open) {
        SetNextItemOpen(true, ImGuiCond.Always)
        state.pending_open = false
    }
    if (state.pending_close) {
        SetNextItemOpen(false, ImGuiCond.Always)
        state.pending_close = false
    }
    state.flags = flags
    let im_open = TreeNodeEx(text, flags)
    state.opened = im_open
    var entry : WidgetEntry
    finalize_state(widget_ident, "tree_node_ex", state, entry)
    return im_open
}

// ===== Radio (int family) =====

[widget]
def radio_button_int(var state : RadioIntState; text : string; v_button : int) : bool {
    //! Grouped-radio form. Each call site picks `v_button`; click writes state.value = v_button, and
    //! calls sharing one RadioIntState ident form a logical group. Since the sites share one snapshot
    //! path, each registers a per-site geometry alias `<path>#<v_button>` for independent targeting.
    if (state.pending_set) {
        state.value = state.pending_value
        state.pending_set = false
    }
    let changed = RadioButton(text, unsafe(addr(state.value)), v_button)
    state.changed = changed
    radio_int_finalize(widget_ident, "radio_button_int", state, v_button)
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
    let item_width = CalcItemWidth()
    let changed = DragFloat2(text, unsafe(addr(state.value.x)), speed, mn, mx, format, flags)
    state.changed = changed
    pending_value_finalize_vec(widget_ident, "drag_float2", state, 2, item_width)
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
    let item_width = CalcItemWidth()
    let changed = DragFloat3(text, unsafe(addr(state.value.x)), speed, mn, mx, format, flags)
    state.changed = changed
    pending_value_finalize_vec(widget_ident, "drag_float3", state, 3, item_width)
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
    let item_width = CalcItemWidth()
    let changed = DragFloat4(text, unsafe(addr(state.value.x)), speed, mn, mx, format, flags)
    state.changed = changed
    pending_value_finalize_vec(widget_ident, "drag_float4", state, 4, item_width)
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
    let item_width = CalcItemWidth()
    let changed = DragInt2(text, unsafe(addr(state.value.x)), speed, mn, mx, format, flags)
    state.changed = changed
    pending_value_finalize_vec(widget_ident, "drag_int2", state, 2, item_width)
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
    let item_width = CalcItemWidth()
    let changed = DragInt3(text, unsafe(addr(state.value.x)), speed, mn, mx, format, flags)
    state.changed = changed
    pending_value_finalize_vec(widget_ident, "drag_int3", state, 3, item_width)
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
    let item_width = CalcItemWidth()
    let changed = DragInt4(text, unsafe(addr(state.value.x)), speed, mn, mx, format, flags)
    state.changed = changed
    pending_value_finalize_vec(widget_ident, "drag_int4", state, 4, item_width)
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
    let item_width = CalcItemWidth()
    let changed = DragFloatRange2(text, unsafe(addr(state.value.x)), unsafe(addr(state.value.y)),
                                  speed, mn, mx, format, format, flags)
    state.changed = changed
    // two side-by-side handles via PushMultiItemsWidths(2): sub_bboxes[0]=lo, [1]=hi
    pending_value_finalize_vec(widget_ident, "drag_float_range2", state, 2, item_width)
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
    let item_width = CalcItemWidth()
    let changed = DragIntRange2(text, unsafe(addr(state.value.x)), unsafe(addr(state.value.y)),
                                speed, mn, mx, format, format, flags)
    state.changed = changed
    // two side-by-side handles via PushMultiItemsWidths(2): sub_bboxes[0]=lo, [1]=hi
    pending_value_finalize_vec(widget_ident, "drag_int_range2", state, 2, item_width)
    return changed
}

// ===== Stateful inputs (slider family) =====

// A zero-init bounds (a forgotten bounds=) is a degenerate min==max==0 slider that can't move;
// substitute ImGui's stock range so a slider with no bounds set is still usable instead of silently
// dead. Sliders only — drag's (0,0)=unbounded is deliberate and keeps its zero-init default.
def private slider_range(b : tuple<float; float>) : tuple<float; float> {
    return b._0 == 0.0f && b._1 == 0.0f ? (0.0f, 1.0f) : b
}
def private slider_range(b : tuple<int; int>) : tuple<int; int> {
    return b._0 == 0 && b._1 == 0 ? (0, 100) : b
}

[widget]
def slider_float(var state : SliderStateFloat; text : string;
                 format : string = "%.3f";
                 flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let (mn, mx) = slider_range(state.bounds)
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
    let (mn, mx) = slider_range(state.bounds)
    let item_width = CalcItemWidth()
    let changed = SliderFloat2(text, unsafe(addr(state.value.x)), mn, mx, format, flags)
    state.changed = changed
    pending_value_finalize_vec(widget_ident, "slider_float2", state, 2, item_width)
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
    let (mn, mx) = slider_range(state.bounds)
    let item_width = CalcItemWidth()
    let changed = SliderFloat3(text, unsafe(addr(state.value.x)), mn, mx, format, flags)
    state.changed = changed
    pending_value_finalize_vec(widget_ident, "slider_float3", state, 3, item_width)
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
    let (mn, mx) = slider_range(state.bounds)
    let item_width = CalcItemWidth()
    let changed = SliderFloat4(text, unsafe(addr(state.value.x)), mn, mx, format, flags)
    state.changed = changed
    pending_value_finalize_vec(widget_ident, "slider_float4", state, 4, item_width)
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
    let (mn, mx) = slider_range(state.bounds)
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
    let (mn, mx) = slider_range(state.bounds)
    let item_width = CalcItemWidth()
    let changed = SliderInt2(text, unsafe(addr(state.value.x)), mn, mx, format, flags)
    state.changed = changed
    pending_value_finalize_vec(widget_ident, "slider_int2", state, 2, item_width)
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
    let (mn, mx) = slider_range(state.bounds)
    let item_width = CalcItemWidth()
    let changed = SliderInt3(text, unsafe(addr(state.value.x)), mn, mx, format, flags)
    state.changed = changed
    pending_value_finalize_vec(widget_ident, "slider_int3", state, 3, item_width)
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
    let (mn, mx) = slider_range(state.bounds)
    let item_width = CalcItemWidth()
    let changed = SliderInt4(text, unsafe(addr(state.value.x)), mn, mx, format, flags)
    state.changed = changed
    pending_value_finalize_vec(widget_ident, "slider_int4", state, 4, item_width)
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
    let (mn, mx) = slider_range(state.bounds)
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
    let (mn, mx) = slider_range(state.bounds)
    let changed = VSliderInt(text, size, unsafe(addr(state.value)), mn, mx, format, flags)
    state.changed = changed
    pending_value_finalize(widget_ident, "vslider_int", state)
    return changed
}

// ===== Stateful inputs (input numeric family) =====

// InputFloat/Int/Double don't clamp (no bounds field). Scalar forms expose step/step_fast; vector
// forms don't, and int vectors hardcode "%d". Flags are ImGuiInputTextFlags (NOT SliderFlags).

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

// ColorEdit3/ColorPicker3 share ColorState3 (RGB); ColorEdit4/ColorPicker4 share ColorState4 (RGBA).
// ColorButton is a click trigger (caller passes the swatch per-frame; reuses ClickState).

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
    //! Optional reference color: pass `has_ref_col=true` + `ref_col` to surface ImGui's "[ref]" swatch
    //! alongside the picker (default null). The local copy gives `addr` a frame-stable address; ImGui
    //! reads it synchronously inside ColorPicker4.
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

// Both take `items : array<string>` and marshal C-side with no shim. `combo` packs a zero-separated
// buffer (daslang `string` can't carry embedded nulls); `list_box` passes the array storage directly
// (each element IS a `char *`, so `addr(items[0])` matches `const char* const items[]`).

def private constrain_combo_popup(max_items : int) {
    // Cap popup height to N item rows (default -1 lets ImGui size it): N text lines + window padding.
    if (max_items <= 0) return
    let h = GetTextLineHeightWithSpacing() * float(max_items) + GetStyle().WindowPadding.y * 2.0f
    SetNextWindowSizeConstraints(float2(0.0f, 0.0f), float2(FLT_MAX, h))
}

[widget]
def combo(var state : ComboState; text : string; items : array<string>;
          popup_max_height_in_items : int = -1) : bool {
    //! `imgui_force_set {"value": N}` selects the N-th item next frame; `popup_max_height_in_items = -1`
    //! lets ImGui pick a default. Built as BeginCombo + a Selectable loop (not all-in-one Combo) so each
    //! item rect is captured into sub_bboxes — a driver clicks item N via widget_component_point.
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let n = length(items)
    let preview = (state.value >= 0 && state.value < n) ? items[state.value] : ""
    var changed = false
    var subs : array<float4>
    constrain_combo_popup(popup_max_height_in_items)
    if (BeginCombo(text, preview, ImGuiComboFlags.None)) {
        for (i in range(n)) {
            let is_sel = i == state.value
            if (Selectable(items[i], is_sel)) {
                state.value = i
                changed = true
            }
            if (is_sel) {
                SetItemDefaultFocus()
            }
            subs |> push(capture_current_item_rect())
        }
        EndCombo()
    }
    state.changed = changed
    pending_value_finalize_subs(widget_ident, "combo", state, subs)
    return changed
}

[widget]
def color_button(var state : ClickState; desc_id : string;
                 col : float4;
                 size : float2 = float2(0.0f, 0.0f);
                 flags : ImGuiColorEditFlags = ImGuiColorEditFlags.None) : bool {
    //! Click-trigger swatch. The displayed color is per-call (state holds only click bookkeeping);
    //! `size = (0, 0)` lets ImGui pick a default square. Param order puts size before flags so callers
    //! can pass `(desc_id, col, size = …)` without a `flags = None` middle arg.
    let clicked = ColorButton(desc_id, col, flags, size)
    state.clicked = clicked
    if (clicked) {
        state.click_count++
    }
    click_finalize(widget_ident, "color_button", state)
    return clicked
}

[widget]
def color_button_hover(var state : EmptyMarkerState; desc_id : string;
                       col : float4;
                       size : float2 = float2(0.0f, 0.0f);
                       flags : ImGuiColorEditFlags = ImGuiColorEditFlags.None) : bool {
    //! Hover-target swatch. Like `color_button` but never registers a click trigger — use when the
    //! swatch is the surface for a hover-driven side-effect at the call site. Returns true when the
    //! just-drawn item is hovered this frame.
    ColorButton(desc_id, col, flags, size)
    let hovered = IsItemHovered()
    marker_finalize(widget_ident, "color_button_hover", state)
    return hovered
}

// ===== Text-input family (Phase 0b.4) =====

// Buffer-as-pointer: state owns `array<uint8> buffer`, the widget passes `addr(buffer[0])` to the C
// extern, ImGui mutates in place, `value` mirrors it (re-cloned on `changed`). `buffer` is `@optional`
// (dropped on hot-reload), so the init below covers both first-frame and post-reload restore.

def private input_text_buffer_init(var state : InputTextState) {
    // Indexed-table first touch (TABLE[k]) zero-inits the state past the field default.
    if (state.capacity <= 0) {
        state.capacity = 256
    }
    if (length(state.buffer) != state.capacity) {
        state.buffer |> resize(state.capacity)
        sync_buffer_from_value(state)
    }
}

def private sync_buffer_from_value(var state : InputTextState) {
    // Zero the buffer, then copy value's bytes clamped to capacity-1 (preserves the trailing null).
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
        state.value = clone_string(reinterpret<string>(pbuf))
    }
}

def private apply_pending_text(var state : InputTextState) {
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
        sync_buffer_from_value(state)
    }
}

[widget]
def input_text(var state : InputTextState; text : string;
               flags : ImGuiInputTextFlags = ImGuiInputTextFlags.None) : bool {
    //! Single-line text input. `imgui_force_set {"value":"…"}` overwrites the
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
    //! Single-line input that grows `state.buffer` when the user types past `state.capacity`, via
    //! ImGui's CallbackResize plumbed through a daslang lambda (invoked synchronously inside InputText,
    //! so no ABI pin). `imgui_force_set` past capacity also grows.
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

[widget]
def input_text_callback(var state : InputTextState; text : string;
                        flags : ImGuiInputTextFlags;
                        cb : lambda<(var data : ImGuiInputTextCallbackData) : int>) : bool {
    //! Single-line input with a user-supplied event callback — use for CallbackCompletion/History/
    //! Always/CharFilter (canonical consumer: the REPL in `examples/imgui_demo/app_console.das`). The
    //! `data` pointer is valid only during the synchronous call. For resize use `input_text_growable`.
    input_text_buffer_init(state)
    apply_pending_text(state)
    var changed : bool
    unsafe {
        changed = _builtin_InputText_cb(addr(state.buffer[0]), state.capacity, text, flags, cb)
    }
    state.changed = changed
    if (changed) {
        sync_value_from_buffer(state)
    }
    text_value_finalize(widget_ident, "input_text_callback", state)
    return changed
}

// ===== Filter family =====

def private text_filter_finalize(widget_ident : string; kind : string; var state : TextFilterState) {
    //! Filter editor finalize. Read-only via the registry's sprint_json_at rail; dispatch is a registered no-op (a future "set" path would mirror the value back into filter.InputBuf and call filter.Build()).
    var entry : WidgetEntry
    finalize_state(widget_ident, kind, state, entry)
    register_focusable(widget_ident)
}

[widget]
def text_filter(var state : TextFilterState; text : string = "Filter (\"incl,-excl\") (\"error\")";
                width : float = 0.0f) : bool {
    //! Boost wrapper over `ImGuiTextFilter::Draw` — renders the inline editor and parses comma-separated
    //! include/exclude tokens (`-prefix` excludes). Returns `true` the frame the expression changed; pair
    //! with `passes_filter(state, line)` to gate items. Empty filter = match-all.
    var buf_view : string
    unsafe {
        buf_view = reinterpret<string>(addr(state.filter.InputBuf))
    }
    if (state.value != buf_view) {
        state.filter = ImGuiTextFilter(state.value)
    }
    let changed = state.filter |> Draw(text, width)
    if (changed) {
        unsafe {
            state.value := clone_string(reinterpret<string>(addr(state.filter.InputBuf)))
        }
    }
    text_filter_finalize(widget_ident, "text_filter", state)
    return changed
}

def public passes_filter(var state : TextFilterState; line : string) : bool {
    //! Predicate companion to `text_filter` (delegates to the bound `PassFilter`). Empty filter matches
    //! all lines. NB: an exclude-only filter (e.g. `-error`) also has no include terms, so
    //! `passes_filter(state, "")` returns true even when active — use `is_active(state)` to detect that.
    return PassFilter(state.filter, line)
}

def public is_active(state : TextFilterState) : bool {
    //! `ImGuiTextFilter::IsActive()` — true iff parsing produced at least one include OR exclude term.
    //! Use to gate filter-active vs -inactive paths (e.g. clipper-cull vs sequential scan in log views).
    return state.filter |> IsActive()
}

// ===== Combo (callback-getter) =====

[widget]
def combo_getter(var state : ComboState; text : string;
                 items_count : int;
                 getter : lambda<(idx : int; var result : string&) : bool>;
                 popup_max_height_in_items : int = -1) : bool {
    //! Procedural combo — items come from a per-frame lambda instead of an `array<string>` (for large,
    //! virtualized, or computed lists). `getter(idx, result)` sets `result` to the idx-th label and
    //! returns true (false stops); fires synchronously. Popup item rects land in sub_bboxes for drivers.
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    var preview = ""
    if (state.value >= 0 && state.value < items_count) {
        var lbl : string
        if (invoke(getter, state.value, lbl)) {
            preview = lbl
        }
    }
    var changed = false
    var subs : array<float4>
    constrain_combo_popup(popup_max_height_in_items)
    if (BeginCombo(text, preview, ImGuiComboFlags.None)) {
        for (i in range(items_count)) {
            var lbl : string
            if (invoke(getter, i, lbl)) {
                let is_sel = i == state.value
                if (Selectable(lbl, is_sel)) {
                    state.value = i
                    changed = true
                }
                if (is_sel) {
                    SetItemDefaultFocus()
                }
                subs |> push(capture_current_item_rect())
            }
        }
        EndCombo()
    }
    state.changed = changed
    pending_value_finalize_subs(widget_ident, "combo_getter", state, subs)
    return changed
}

// ===== Display: text_show =====

// Read-only display widget. The serializer surfaces `state.value` (so tests `expect_value(STATUS,
// "saved")` with no plumbing); the dispatcher accepts "set" so tests can drive the value externally.

def private text_show_finalize(widget_ident : string; kind : string; var state : TextShowState) {
    //! Display-only widget finalize with a "set" action handled by the per-kind [widget_dispatch] (writes state.value).
    var entry : WidgetEntry
    finalize_state(widget_ident, kind, state, entry)
}

[widget]
def text_show(var state : TextShowState; value : string = "") {
    //! Display-only widget surfaced through telemetry. `text_show(STATUS, value="saved")` sets
    //! state.value every frame (call-site is source of truth); `text_show(STATUS)` displays whatever's
    //! in state.value (set via `STATUS.value = …` or `imgui_force_set`). Serializes for `expect_value`.
    if (!empty(value)) {
        state.value := value
    }
    Text(state.value)
    text_show_finalize(widget_ident, "text_show", state)
}

// ===== Display: narrative =====

// Read-only display widgets (text/decoration). The serializer emits telemetry payload (NarrativeState
// → {value}, LabelTextState → {key, value}, EmptyMarkerState → {}); dispatchers are no-ops and
// `register_focusable` is NOT called (display widgets aren't focusable).

def private narrative_finalize(widget_ident : string; kind : string; var state : NarrativeState) {
    //! Read-only narrative finalize. State payload via sprint_json_at; dispatch is a registered no-op.
    var entry : WidgetEntry
    finalize_state(widget_ident, kind, state, entry)
}

def private label_text_finalize(widget_ident : string; kind : string; var state : LabelTextState) {
    //! Read-only label-text finalize. Same shape as narrative.
    var entry : WidgetEntry
    finalize_state(widget_ident, kind, state, entry)
}

def private marker_finalize(widget_ident : string; kind : string; var state : EmptyMarkerState) {
    //! Stateless decoration finalize. Same shape as narrative.
    var entry : WidgetEntry
    finalize_state(widget_ident, kind, state, entry)
}

[widget]
def text(var state : NarrativeState; text : string) {
    //! Plain text display. `state.value` echoes the call-site string for
    //! `expect_value(IDENT, "literal")` assertions.
    state.value := text
    TextUnformatted(text)
    narrative_finalize(widget_ident, "text", state)
}

[widget]
def text_unformatted(var state : NarrativeState; text : string) {
    //! Alias for `text` — kept for explicit clarity at call sites where the
    //! "no printf" intent matters. Identical behavior to `text`.
    state.value := text
    TextUnformatted(text)
    narrative_finalize(widget_ident, "text_unformatted", state)
}

[widget]
def text_wrapped(var state : NarrativeState; text : string) {
    //! Text wrapped to the current window or content-region width.
    state.value := text
    PushTextWrapPos(0.0f)
    TextUnformatted(text)
    PopTextWrapPos()
    narrative_finalize(widget_ident, "text_wrapped", state)
}

[widget]
def text_colored(var state : NarrativeState; color : float4; text : string) {
    //! Tinted text — `color` overrides ``ImGuiCol_Text`` for this draw.
    state.value := text
    PushStyleColor(ImGuiCol.Text, color)
    TextUnformatted(text)
    PopStyleColor(1)
    narrative_finalize(widget_ident, "text_colored", state)
}

[widget]
def text_disabled(var state : NarrativeState; text : string) {
    //! Text rendered in the disabled style (``ImGuiCol_TextDisabled``).
    state.value := text
    PushStyleColor(ImGuiCol.Text, GetStyleColorVec4(ImGuiCol.TextDisabled))
    TextUnformatted(text)
    PopStyleColor(1)
    narrative_finalize(widget_ident, "text_disabled", state)
}

[widget]
def bullet_text(var state : NarrativeState; text : string) {
    //! Bullet point followed by text — like `bullet` + `text` in one call.
    state.value := text
    Bullet()
    TextUnformatted(text)
    narrative_finalize(widget_ident, "bullet_text", state)
}

[widget]
def bullet(var state : EmptyMarkerState) {
    //! Renders a bullet glyph and stays on the current line. Follow with
    //! `same_line` + content for prefix bullets, or use `bullet_text` for
    //! "bullet + text" in one call.
    Bullet()
    marker_finalize(widget_ident, "bullet", state)
}

[widget]
def label_text(var state : LabelTextState; key : string; value : string) {
    //! Two-part display: `key` on the left in the label gutter, `value`
    //! on the right. Both echoed for telemetry. The struct field is `key`
    //! (not `label`) because `label` is reserved in gen2.
    state.key := key
    state.value := value
    LabelText(key, value)
    label_text_finalize(widget_ident, "label_text", state)
}

[widget]
def separator(var state : EmptyMarkerState) {
    //! Full-width horizontal divider line.
    Separator()
    marker_finalize(widget_ident, "separator", state)
}

[widget]
def separator_text(var state : NarrativeState; text : string) {
    //! Horizontal divider with an inline label.
    state.value := text
    SeparatorText(text)
    narrative_finalize(widget_ident, "separator_text", state)
}

// ===== Display: layout =====

// Cursor-repositioning primitives — they draw no glyphs but ARE visible (every widget's bbox depends
// on the cursor). Wrapping as `[widget]`s lets playwright assert relative placement by telemetry ident.

[widget]
def same_line(var state : EmptyMarkerState;
              offset_from_start_x : float = 0.0f;
              spacing : float = -1.0f) {
    //! Stay on the same line as the previous widget. `offset_from_start_x`
    //! pins the next widget to that x within the current window; `spacing`
    //! is the horizontal gap (negative = use ``ItemSpacing.x`` style default).
    SameLine(offset_from_start_x, spacing)
    marker_finalize(widget_ident, "same_line", state)
}

[widget]
def new_line(var state : EmptyMarkerState) {
    //! Force a line break — undoes a preceding `same_line`.
    NewLine()
    marker_finalize(widget_ident, "new_line", state)
}

[widget]
def spacing(var state : EmptyMarkerState) {
    //! Vertical gap of one ``ItemSpacing.y`` unit.
    Spacing()
    marker_finalize(widget_ident, "spacing", state)
}

[widget]
def dummy(var state : EmptyMarkerState; size : float2) {
    //! Reserve a `size`-pixel rectangle of empty space (no draw, but the
    //! cursor advances). Useful for spacing or to consume layout columns.
    Dummy(size)
    marker_finalize(widget_ident, "dummy", state)
}

[widget]
def align_text_to_frame_padding(var state : EmptyMarkerState) {
    //! Vertically align the next text widget to fit a framed item's baseline —
    //! the standard prefix when pairing `text(...)` with an inline input or
    //! button so the label sits centred on the frame.
    AlignTextToFramePadding()
    marker_finalize(widget_ident, "align_text_to_frame_padding", state)
}

[widget]
def indent(var state : EmptyMarkerState; indent_w : float = 0.0f) {
    //! Push the cursor right by `indent_w` (or `IndentSpacing` style default
    //! when `0.0f`) for the rest of the current scope. Pair with `unindent`
    //! when done. For block-scoped indent prefer the `with_indent` wrapper.
    Indent(indent_w)
    marker_finalize(widget_ident, "indent", state)
}

[widget]
def unindent(var state : EmptyMarkerState; indent_w : float = 0.0f) {
    //! Pull the cursor left by `indent_w` (or `IndentSpacing` when `0.0f`).
    //! Pair with `indent` to bracket a section manually; for block-scoped
    //! indent prefer `with_indent`.
    Unindent(indent_w)
    marker_finalize(widget_ident, "unindent", state)
}

// ===== Display: progress / image =====

// `progress_bar` shows a fraction in [0,1] (indeterminate when < 0); `image` blits a texture handle.
// Per-call args echo into state for telemetry; no dispatcher (read-only).

def private progress_bar_finalize(widget_ident : string; kind : string; var state : ProgressBarState) {
    //! Read-only progress finalize. Same shape as narrative.
    var entry : WidgetEntry
    finalize_state(widget_ident, kind, state, entry)
}

def private image_finalize(widget_ident : string; kind : string; var state : ImageState) {
    //! Read-only image finalize. Same shape as narrative.
    var entry : WidgetEntry
    finalize_state(widget_ident, kind, state, entry)
}

[widget]
def progress_bar(var state : ProgressBarState; fraction : float;
                 size : float2 = float2(-1.0f, 0.0f);
                 overlay : string = "") {
    //! Progress bar in [0, 1]; pass a negative fraction for indeterminate
    //! animation. `size` defaults to (-1, 0) — full content-region width,
    //! one-frame-tall. `overlay` is optional center-aligned text (e.g. "42%").
    state.fraction = fraction
    state.size = size
    state.overlay := overlay
    ProgressBar(fraction, size, overlay)
    progress_bar_finalize(widget_ident, "progress_bar", state)
}

[widget]
def image(var state : ImageState; user_texture_id : ImTextureRef; size : float2;
          uv0 : float2 = float2(0.0f, 0.0f);
          uv1 : float2 = float2(1.0f, 1.0f);
          tint_col : float4 = float4(1.0f, 1.0f, 1.0f, 1.0f);
          border_col : float4 = float4(0.0f, 0.0f, 0.0f, 0.0f)) {
    //! Blit `user_texture_id` (an ``ImTextureRef``) into a `size`-pixel rect with `uv0`/`uv1` sampling;
    //! `tint_col` modulates texel color. `border_col` is telemetry-only — ``Image`` takes no per-call
    //! border (it comes from the ``ImGuiCol_Border`` style). Args echo into state for snapshot assertions.
    state.size = size
    state.uv0 = uv0
    state.uv1 = uv1
    state.tint_col = tint_col
    state.border_col = border_col
    ImageWithBg(user_texture_id, size, uv0, uv1, float4(0.0f, 0.0f, 0.0f, 0.0f), tint_col)
    image_finalize(widget_ident, "image", state)
}

def public image_texture_id(texture : ImTextureRef const) : uint64 {
    //! Extract the backend handle for APIs that retain an image reference
    //! across frames. `ImTextureRef` itself is a non-local handled type and
    //! therefore must not be stored in ordinary daScript state structs.
    return texture.GetTexID()
}

// ===== Display: tooltip (flat form) =====

// Stateless one-liner tooltips: `set_tooltip` always shows; `set_item_tooltip` shows only when the
// preceding widget is hovered. Block-form tooltips (arbitrary widgets inside) live in
// `imgui_containers_builtin.das` as the `tooltip` / `item_tooltip` [container]s.

[widget]
def set_tooltip(var state : NarrativeState; text : string) {
    //! Show `text` as the active tooltip; overrides any previous SetTooltip
    //! in the same frame. For hover-gated tooltips use `set_item_tooltip`.
    state.value := text
    SetTooltip(text)
    narrative_finalize(widget_ident, "set_tooltip", state)
}

[widget]
def set_item_tooltip(var state : NarrativeState; text : string;
                     flags : ImGuiHoveredFlags = ImGuiHoveredFlags.ForTooltip) {
    //! Show `text` as a tooltip when the preceding widget is hovered; default `flags = ForTooltip` pulls
    //! per-style delay/stationary. Finalized (reports `rendered: true`) only on frames the tooltip
    //! actually shows, so telemetry tracks visibility — a recording self-verifies via record_check_rendered.
    state.value := text
    if (IsItemHovered(flags)) {
        SetTooltip(text)
        narrative_finalize(widget_ident, "set_item_tooltip", state)
    } else {
        // Not hovered: the tooltip didn't render, so we skip narrative_finalize (it stays unregistered
        // in the snapshot). But widget_prelude's PushID(widget_ident) ran unconditionally, so we must
        // still pop it here or the ID stack leaks — ImGui 1.92 flags this as "Missing PopID()".
        PopID()
    }
}

// ===== Plot lines / histogram (array form) =====

// Display widgets backed by `PlotState`. Values come per-frame as an `array<float>` param (not stored
// on state); state tracks last-frame call args (title, bounds, sample count) for telemetry. No dispatcher.

def private plot_finalize(widget_ident : string; kind : string; var state : PlotState) {
    //! Read-only plot finalize. Same shape as narrative.
    var entry : WidgetEntry
    finalize_state(widget_ident, kind, state, entry)
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

// Procedural variants of plot_lines / plot_histogram — `getter(idx) : float` produces samples per
// frame instead of a backing `array<float>`. The lambda fires synchronously inside the C call.

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

// ===== External-pointer rail (edit_* family) =====

// Caller owns the value via `safe_addr(local)` / `unsafe(addr(struct.field))`; the renderer reads/writes
// through the pointer directly. No state struct, no `@live` preservation (external owner controls
// persistence). Required `id="..."` at the call site keys the telemetry path.

[edit_widget]
def edit_checkbox(var ptr : bool? implicit; text : string = "") : bool {
    //! ``Checkbox(label, ptr)`` against a caller-owned ``bool?``. Telemetry payload is ``{value}``; ``imgui_force_set`` writes through ``*ptr``.
    let lbl = empty(text) ? widget_ident : text
    let changed = Checkbox(lbl, ptr)
    edit_finalize(widget_ident, "edit_checkbox", ptr)
    return changed
}

[edit_widget]
def edit_menu_item(var ptr : bool? implicit; text : string = "";
                   shortcut : string = ""; enabled : bool = true) : bool {
    //! ``MenuItem(label, shortcut, ptr, enabled)`` against a caller-owned ``bool?``. Mirrors ``edit_checkbox`` shape for menu contexts — the menu item shows a check mark while ``*ptr`` is true and toggles it on click. Caller owns persistence; no ``@live`` preservation. Required ``id="..."`` at the call site keys the telemetry path.
    let lbl = empty(text) ? widget_ident : text
    let changed = MenuItem(lbl, shortcut, ptr, enabled)
    edit_finalize(widget_ident, "edit_menu_item", ptr)
    return changed
}

[edit_widget]
def edit_radio_button(var ptr : bool? implicit; text : string = "") : bool {
    //! Bool-form ``RadioButton`` against a caller-owned ``bool?``; the widget flips ``*ptr`` on click (ImGui's RadioButton-bool doesn't mutate). For grouped int-form radios, declare a ``RadioIntState`` and use ``radio_button_int``.
    let lbl = empty(text) ? widget_ident : text
    let im_clicked = unsafe(RadioButton(lbl, *ptr))
    if (im_clicked) {
        unsafe {
            *ptr = !*ptr
        }
    }
    edit_finalize(widget_ident, "edit_radio_button", ptr)
    return im_clicked
}

[edit_widget]
def edit_slider_float(var ptr : float? implicit; text : string = "";
                      v_min : float = 0.0f; v_max : float = 1.0f;
                      format : string = "%.3f";
                      flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    //! ``SliderFloat`` against a caller-owned ``float?``. Range is per-call (no state-side ``bounds``); other args mirror :ref:`slider_float <function-imgui_widgets_builtin_slider_float_SliderStateFloat_string_string_ImGuiSliderFlags>`.
    let lbl = empty(text) ? widget_ident : text
    let changed = SliderFloat(lbl, ptr, v_min, v_max, format, flags)
    edit_finalize(widget_ident, "edit_slider_float", ptr)
    return changed
}

[edit_widget]
def edit_slider_int(var ptr : int? implicit; text : string = "";
                    v_min : int = 0; v_max : int = 100;
                    format : string = "%d";
                    flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    //! ``SliderInt`` against a caller-owned ``int?``. Range is per-call.
    let lbl = empty(text) ? widget_ident : text
    let changed = SliderInt(lbl, ptr, v_min, v_max, format, flags)
    edit_finalize(widget_ident, "edit_slider_int", ptr)
    return changed
}

[edit_widget]
def edit_vslider_float(var ptr : float? implicit; text : string = "";
                       size : float2 = float2(40.0f, 200.0f);
                       v_min : float = 0.0f; v_max : float = 1.0f;
                       format : string = "%.3f";
                       flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    //! ``VSliderFloat`` against a caller-owned ``float?``. Vertical slider sibling of ``edit_slider_float`` — ``size`` is per-call (typical ~40×200).
    let lbl = empty(text) ? widget_ident : text
    let changed = VSliderFloat(lbl, size, ptr, v_min, v_max, format, flags)
    edit_finalize(widget_ident, "edit_vslider_float", ptr)
    return changed
}

[edit_widget]
def edit_vslider_int(var ptr : int? implicit; text : string = "";
                     size : float2 = float2(40.0f, 200.0f);
                     v_min : int = 0; v_max : int = 100;
                     format : string = "%d";
                     flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    //! ``VSliderInt`` against a caller-owned ``int?``. Vertical slider sibling of ``edit_slider_int``.
    let lbl = empty(text) ? widget_ident : text
    let changed = VSliderInt(lbl, size, ptr, v_min, v_max, format, flags)
    edit_finalize(widget_ident, "edit_vslider_int", ptr)
    return changed
}

[edit_widget]
def edit_drag_float(var ptr : float? implicit; text : string = "";
                    speed : float = 1.0f;
                    v_min : float = 0.0f; v_max : float = 0.0f;
                    format : string = "%.3f";
                    flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    //! ``DragFloat`` against a caller-owned ``float?``. ``v_min == v_max == 0`` ⇒ unbounded (ImGui convention).
    let lbl = empty(text) ? widget_ident : text
    let changed = DragFloat(lbl, ptr, speed, v_min, v_max, format, flags)
    edit_finalize(widget_ident, "edit_drag_float", ptr)
    return changed
}

[edit_widget]
def edit_drag_int(var ptr : int? implicit; text : string = "";
                  speed : float = 1.0f;
                  v_min : int = 0; v_max : int = 0;
                  format : string = "%d";
                  flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    //! ``DragInt`` against a caller-owned ``int?``. ``v_min == v_max == 0`` ⇒ unbounded.
    let lbl = empty(text) ? widget_ident : text
    let changed = DragInt(lbl, ptr, speed, v_min, v_max, format, flags)
    edit_finalize(widget_ident, "edit_drag_int", ptr)
    return changed
}

[edit_widget]
def edit_input_float(var ptr : float? implicit; text : string = "";
                     step : float = 0.0f;
                     step_fast : float = 0.0f;
                     format : string = "%.3f";
                     flags : ImGuiInputTextFlags = ImGuiInputTextFlags.None) : bool {
    //! ``InputFloat`` against a caller-owned ``float?``. No clamping (matches :ref:`input_float <function-imgui_widgets_builtin_input_float_InputStateFloat_string_float_float_string_ImGuiInputTextFlags>`); ``step``/``step_fast`` drive the ±buttons.
    let lbl = empty(text) ? widget_ident : text
    let changed = InputFloat(lbl, ptr, step, step_fast, format, flags)
    edit_finalize(widget_ident, "edit_input_float", ptr)
    return changed
}

[edit_widget]
def edit_input_int(var ptr : int? implicit; text : string = "";
                   step : int = 1;
                   step_fast : int = 100;
                   flags : ImGuiInputTextFlags = ImGuiInputTextFlags.None) : bool {
    //! ``InputInt`` against a caller-owned ``int?``. No clamping.
    let lbl = empty(text) ? widget_ident : text
    let changed = InputInt(lbl, ptr, step, step_fast, flags)
    edit_finalize(widget_ident, "edit_input_int", ptr)
    return changed
}

// ===== External-pointer rail (10b: vectors) =====

// Vector overloads (float2/3/4 + int2/3/4) for slider/drag. Each `TN?` is reinterpret-cast to `T?` at
// the call site (ImGui's Slider*/Drag* take a single `T*` as contiguous components — no vector binding);
// `edit_finalize` keeps the full vector pointer so the serializer round-trips the whole tuple.

[edit_widget]
def edit_slider_float2(var ptr : float2? implicit; text : string = "";
                       v_min : float = 0.0f; v_max : float = 1.0f;
                       format : string = "%.3f";
                       flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    //! ``SliderFloat2`` against a caller-owned ``float2?``.
    let lbl = empty(text) ? widget_ident : text
    let changed = SliderFloat2(lbl, unsafe(reinterpret<float?>(ptr)),
                               v_min, v_max, format, flags)
    edit_finalize(widget_ident, "edit_slider_float2", ptr)
    return changed
}

[edit_widget]
def edit_slider_float3(var ptr : float3? implicit; text : string = "";
                       v_min : float = 0.0f; v_max : float = 1.0f;
                       format : string = "%.3f";
                       flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    //! ``SliderFloat3`` against a caller-owned ``float3?``.
    let lbl = empty(text) ? widget_ident : text
    let changed = SliderFloat3(lbl, unsafe(reinterpret<float?>(ptr)),
                               v_min, v_max, format, flags)
    edit_finalize(widget_ident, "edit_slider_float3", ptr)
    return changed
}

[edit_widget]
def edit_slider_float4(var ptr : float4? implicit; text : string = "";
                       v_min : float = 0.0f; v_max : float = 1.0f;
                       format : string = "%.3f";
                       flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    //! ``SliderFloat4`` against a caller-owned ``float4?``.
    let lbl = empty(text) ? widget_ident : text
    let changed = SliderFloat4(lbl, unsafe(reinterpret<float?>(ptr)),
                               v_min, v_max, format, flags)
    edit_finalize(widget_ident, "edit_slider_float4", ptr)
    return changed
}

[edit_widget]
def edit_slider_int2(var ptr : int2? implicit; text : string = "";
                     v_min : int = 0; v_max : int = 100;
                     format : string = "%d";
                     flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    //! ``SliderInt2`` against a caller-owned ``int2?``.
    let lbl = empty(text) ? widget_ident : text
    let changed = SliderInt2(lbl, unsafe(reinterpret<int?>(ptr)),
                             v_min, v_max, format, flags)
    edit_finalize(widget_ident, "edit_slider_int2", ptr)
    return changed
}

[edit_widget]
def edit_slider_int3(var ptr : int3? implicit; text : string = "";
                     v_min : int = 0; v_max : int = 100;
                     format : string = "%d";
                     flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    //! ``SliderInt3`` against a caller-owned ``int3?``.
    let lbl = empty(text) ? widget_ident : text
    let changed = SliderInt3(lbl, unsafe(reinterpret<int?>(ptr)),
                             v_min, v_max, format, flags)
    edit_finalize(widget_ident, "edit_slider_int3", ptr)
    return changed
}

[edit_widget]
def edit_slider_int4(var ptr : int4? implicit; text : string = "";
                     v_min : int = 0; v_max : int = 100;
                     format : string = "%d";
                     flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    //! ``SliderInt4`` against a caller-owned ``int4?``.
    let lbl = empty(text) ? widget_ident : text
    let changed = SliderInt4(lbl, unsafe(reinterpret<int?>(ptr)),
                             v_min, v_max, format, flags)
    edit_finalize(widget_ident, "edit_slider_int4", ptr)
    return changed
}

[edit_widget]
def edit_drag_float2(var ptr : float2? implicit; text : string = "";
                     speed : float = 1.0f;
                     v_min : float = 0.0f; v_max : float = 0.0f;
                     format : string = "%.3f";
                     flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    //! ``DragFloat2`` against a caller-owned ``float2?``.
    let lbl = empty(text) ? widget_ident : text
    let changed = DragFloat2(lbl, unsafe(reinterpret<float?>(ptr)),
                             speed, v_min, v_max, format, flags)
    edit_finalize(widget_ident, "edit_drag_float2", ptr)
    return changed
}

[edit_widget]
def edit_drag_float3(var ptr : float3? implicit; text : string = "";
                     speed : float = 1.0f;
                     v_min : float = 0.0f; v_max : float = 0.0f;
                     format : string = "%.3f";
                     flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    //! ``DragFloat3`` against a caller-owned ``float3?``.
    let lbl = empty(text) ? widget_ident : text
    let changed = DragFloat3(lbl, unsafe(reinterpret<float?>(ptr)),
                             speed, v_min, v_max, format, flags)
    edit_finalize(widget_ident, "edit_drag_float3", ptr)
    return changed
}

[edit_widget]
def edit_drag_float4(var ptr : float4? implicit; text : string = "";
                     speed : float = 1.0f;
                     v_min : float = 0.0f; v_max : float = 0.0f;
                     format : string = "%.3f";
                     flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    //! ``DragFloat4`` against a caller-owned ``float4?``.
    let lbl = empty(text) ? widget_ident : text
    let changed = DragFloat4(lbl, unsafe(reinterpret<float?>(ptr)),
                             speed, v_min, v_max, format, flags)
    edit_finalize(widget_ident, "edit_drag_float4", ptr)
    return changed
}

[edit_widget]
def edit_drag_int2(var ptr : int2? implicit; text : string = "";
                   speed : float = 1.0f;
                   v_min : int = 0; v_max : int = 0;
                   format : string = "%d";
                   flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    //! ``DragInt2`` against a caller-owned ``int2?``.
    let lbl = empty(text) ? widget_ident : text
    let changed = DragInt2(lbl, unsafe(reinterpret<int?>(ptr)),
                           speed, v_min, v_max, format, flags)
    edit_finalize(widget_ident, "edit_drag_int2", ptr)
    return changed
}

[edit_widget]
def edit_drag_int3(var ptr : int3? implicit; text : string = "";
                   speed : float = 1.0f;
                   v_min : int = 0; v_max : int = 0;
                   format : string = "%d";
                   flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    //! ``DragInt3`` against a caller-owned ``int3?``.
    let lbl = empty(text) ? widget_ident : text
    let changed = DragInt3(lbl, unsafe(reinterpret<int?>(ptr)),
                           speed, v_min, v_max, format, flags)
    edit_finalize(widget_ident, "edit_drag_int3", ptr)
    return changed
}

[edit_widget]
def edit_drag_int4(var ptr : int4? implicit; text : string = "";
                   speed : float = 1.0f;
                   v_min : int = 0; v_max : int = 0;
                   format : string = "%d";
                   flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    //! ``DragInt4`` against a caller-owned ``int4?``.
    let lbl = empty(text) ? widget_ident : text
    let changed = DragInt4(lbl, unsafe(reinterpret<int?>(ptr)),
                           speed, v_min, v_max, format, flags)
    edit_finalize(widget_ident, "edit_drag_int4", ptr)
    return changed
}

// ===== External-pointer rail (10c: input vectors, color, combo, special) =====

// Remaining external-pointer overloads: input vector forms, input_double, the color family, combo
// (array-of-string-pointers form — C reads `items[i]` lazily, no per-frame buffer packing),
// slider_angle (radians in, degrees shown), and checkbox_flags (bit-mask toggle on an `int?`).

[edit_widget]
def edit_input_float2(var ptr : float2? implicit; text : string = "";
                      format : string = "%.3f";
                      flags : ImGuiInputTextFlags = ImGuiInputTextFlags.None) : bool {
    //! ``InputFloat2`` against a caller-owned ``float2?``. No clamping.
    let lbl = empty(text) ? widget_ident : text
    let changed = InputFloat2(lbl, unsafe(reinterpret<float?>(ptr)), format, flags)
    edit_finalize(widget_ident, "edit_input_float2", ptr)
    return changed
}

[edit_widget]
def edit_input_float3(var ptr : float3? implicit; text : string = "";
                      format : string = "%.3f";
                      flags : ImGuiInputTextFlags = ImGuiInputTextFlags.None) : bool {
    //! ``InputFloat3`` against a caller-owned ``float3?``.
    let lbl = empty(text) ? widget_ident : text
    let changed = InputFloat3(lbl, unsafe(reinterpret<float?>(ptr)), format, flags)
    edit_finalize(widget_ident, "edit_input_float3", ptr)
    return changed
}

[edit_widget]
def edit_input_float4(var ptr : float4? implicit; text : string = "";
                      format : string = "%.3f";
                      flags : ImGuiInputTextFlags = ImGuiInputTextFlags.None) : bool {
    //! ``InputFloat4`` against a caller-owned ``float4?``.
    let lbl = empty(text) ? widget_ident : text
    let changed = InputFloat4(lbl, unsafe(reinterpret<float?>(ptr)), format, flags)
    edit_finalize(widget_ident, "edit_input_float4", ptr)
    return changed
}

[edit_widget]
def edit_input_int2(var ptr : int2? implicit; text : string = "";
                    flags : ImGuiInputTextFlags = ImGuiInputTextFlags.None) : bool {
    //! ``InputInt2`` against a caller-owned ``int2?``.
    let lbl = empty(text) ? widget_ident : text
    let changed = InputInt2(lbl, unsafe(reinterpret<int?>(ptr)), flags)
    edit_finalize(widget_ident, "edit_input_int2", ptr)
    return changed
}

[edit_widget]
def edit_input_int3(var ptr : int3? implicit; text : string = "";
                    flags : ImGuiInputTextFlags = ImGuiInputTextFlags.None) : bool {
    //! ``InputInt3`` against a caller-owned ``int3?``.
    let lbl = empty(text) ? widget_ident : text
    let changed = InputInt3(lbl, unsafe(reinterpret<int?>(ptr)), flags)
    edit_finalize(widget_ident, "edit_input_int3", ptr)
    return changed
}

[edit_widget]
def edit_input_int4(var ptr : int4? implicit; text : string = "";
                    flags : ImGuiInputTextFlags = ImGuiInputTextFlags.None) : bool {
    //! ``InputInt4`` against a caller-owned ``int4?``.
    let lbl = empty(text) ? widget_ident : text
    let changed = InputInt4(lbl, unsafe(reinterpret<int?>(ptr)), flags)
    edit_finalize(widget_ident, "edit_input_int4", ptr)
    return changed
}

[edit_widget]
def edit_input_double(var ptr : double? implicit; text : string = "";
                      step : double = 0.0lf;
                      step_fast : double = 0.0lf;
                      format : string = "%.6f";
                      flags : ImGuiInputTextFlags = ImGuiInputTextFlags.None) : bool {
    //! ``InputDouble`` against a caller-owned ``double?``.
    let lbl = empty(text) ? widget_ident : text
    let changed = InputDouble(lbl, ptr, step, step_fast, format, flags)
    edit_finalize(widget_ident, "edit_input_double", ptr)
    return changed
}

// ===== Generic scalar rail (edit_drag_scalar / edit_slider_scalar / edit_input_scalar) =====

// One wrapper per family over ImGui's void*-routed DragScalar/SliderScalar/InputScalar. Compile-time
// dispatch via `typeinfo stripped_typename(T)` picks the ImGuiDataType + default format. Supported T:
// int8/uint8/int16/uint16/int/uint/int64/uint64/float/double; anything else fails via `concept_assert`.

def private scalar_data_type(t : type<auto(T)>) : ImGuiDataType {
    static_if (typeinfo stripped_typename(t) == "int8") {
        return ImGuiDataType.S8
    } static_elif (typeinfo stripped_typename(t) == "uint8") {
        return ImGuiDataType.U8
    } static_elif (typeinfo stripped_typename(t) == "int16") {
        return ImGuiDataType.S16
    } static_elif (typeinfo stripped_typename(t) == "uint16") {
        return ImGuiDataType.U16
    } static_elif (typeinfo stripped_typename(t) == "int") {
        return ImGuiDataType.S32
    } static_elif (typeinfo stripped_typename(t) == "uint") {
        return ImGuiDataType.U32
    } static_elif (typeinfo stripped_typename(t) == "int64") {
        return ImGuiDataType.S64
    } static_elif (typeinfo stripped_typename(t) == "uint64") {
        return ImGuiDataType.U64
    } static_elif (typeinfo stripped_typename(t) == "float") {
        return ImGuiDataType.Float
    } static_elif (typeinfo stripped_typename(t) == "double") {
        return ImGuiDataType.Double
    } else {
        concept_assert(false, "scalar widget: unsupported type - expected int8/uint8/int16/uint16/int/uint/int64/uint64/float/double")
    }
}

def private scalar_default_format(t : type<auto(T)>) : string {
    static_if (typeinfo stripped_typename(t) == "float") {
        return "%.3f"
    } static_elif (typeinfo stripped_typename(t) == "double") {
        return "%.6f"
    } static_elif (typeinfo stripped_typename(t) == "uint8"
                || typeinfo stripped_typename(t) == "uint16"
                || typeinfo stripped_typename(t) == "uint"
                || typeinfo stripped_typename(t) == "uint64") {
        return "%u"
    } else {
        return "%d"
    }
}

[edit_widget]
def edit_drag_scalar(var ptr : auto(T)? implicit;
                     text : string = "";
                     speed : float = 1.0f;
                     v_min : T = default<T>;
                     v_max : T = default<T>;
                     format : string = "";
                     flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    //! ``DragScalar`` against a caller-owned ``T?`` (any supported scalar). ``v_min == v_max`` (default)
    //! selects ImGui's no-clamp behavior; otherwise both bounds pass through. Empty ``format`` falls back
    //! to the type-specific default.
    let lbl = empty(text) ? widget_ident : text
    let fmt = empty(format) ? scalar_default_format(type<T>) : format
    var lo = v_min
    var hi = v_max
    let has_clamp = (v_min != v_max) || (v_min != default<T>)
    var p : T? = ptr
    unsafe {
        let lo_ptr = has_clamp ? addr<void? const>(lo) : null
        let hi_ptr = has_clamp ? addr<void? const>(hi) : null
        let data_ptr = reinterpret<void?>(p)
        let changed = DragScalar(lbl, scalar_data_type(type<T>),
                                 data_ptr, speed, lo_ptr, hi_ptr, fmt, flags)
        edit_finalize(widget_ident, "edit_drag_scalar", p)
        return changed
    }
}

[edit_widget]
def edit_slider_scalar(var ptr : auto(T)? implicit;
                       text : string = "";
                       v_min : T;
                       v_max : T;
                       format : string = "";
                       flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    //! ``SliderScalar`` against a caller-owned ``T?``. Both bounds are
    //! required (matches ImGui — no null shortcut). Empty ``format`` falls
    //! back to the type-specific default.
    let lbl = empty(text) ? widget_ident : text
    let fmt = empty(format) ? scalar_default_format(type<T>) : format
    var lo = v_min
    var hi = v_max
    var p : T? = ptr
    unsafe {
        let lo_ptr = addr<void? const>(lo)
        let hi_ptr = addr<void? const>(hi)
        let data_ptr = reinterpret<void?>(p)
        let changed = SliderScalar(lbl, scalar_data_type(type<T>),
                                   data_ptr, lo_ptr, hi_ptr, fmt, flags)
        edit_finalize(widget_ident, "edit_slider_scalar", p)
        return changed
    }
}

[edit_widget]
def edit_input_scalar(var ptr : auto(T)? implicit;
                      text : string = "";
                      step : T = default<T>;
                      step_fast : T = default<T>;
                      format : string = "";
                      flags : ImGuiInputTextFlags = ImGuiInputTextFlags.None) : bool {
    //! ``InputScalar`` against a caller-owned ``T?``. ``step == 0`` (default)
    //! selects ImGui's no-step-button behavior; non-zero ``step`` shows the
    //! +/- buttons. Empty ``format`` falls back to the type-specific default.
    let lbl = empty(text) ? widget_ident : text
    let fmt = empty(format) ? scalar_default_format(type<T>) : format
    var stp = step
    var stp_fast = step_fast
    var p : T? = ptr
    unsafe {
        let step_ptr = (step != default<T>) ? addr<void? const>(stp) : null
        let step_fast_ptr = (step_fast != default<T>) ? addr<void? const>(stp_fast) : null
        let data_ptr = reinterpret<void?>(p)
        let changed = InputScalar(lbl, scalar_data_type(type<T>),
                                  data_ptr, step_ptr, step_fast_ptr, fmt, flags)
        edit_finalize(widget_ident, "edit_input_scalar", p)
        return changed
    }
}

[edit_widget]
def edit_color_edit3(var ptr : float3? implicit; text : string = "";
                     flags : ImGuiColorEditFlags = ImGuiColorEditFlags.None) : bool {
    //! ``ColorEdit3`` against a caller-owned ``float3?`` (RGB).
    let lbl = empty(text) ? widget_ident : text
    let changed = ColorEdit3(lbl, unsafe(reinterpret<float?>(ptr)), flags)
    edit_finalize(widget_ident, "edit_color_edit3", ptr)
    return changed
}

[edit_widget]
def edit_color_edit4(var ptr : float4? implicit; text : string = "";
                     flags : ImGuiColorEditFlags = ImGuiColorEditFlags.None) : bool {
    //! ``ColorEdit4`` against a caller-owned ``float4?`` (RGBA).
    let lbl = empty(text) ? widget_ident : text
    let changed = ColorEdit4(lbl, unsafe(reinterpret<float?>(ptr)), flags)
    edit_finalize(widget_ident, "edit_color_edit4", ptr)
    return changed
}

[edit_widget]
def edit_color_picker3(var ptr : float3? implicit; text : string = "";
                       flags : ImGuiColorEditFlags = ImGuiColorEditFlags.None) : bool {
    //! ``ColorPicker3`` against a caller-owned ``float3?`` (RGB).
    let lbl = empty(text) ? widget_ident : text
    let changed = ColorPicker3(lbl, unsafe(reinterpret<float?>(ptr)), flags)
    edit_finalize(widget_ident, "edit_color_picker3", ptr)
    return changed
}

[edit_widget]
def edit_color_picker4(var ptr : float4? implicit; text : string = "";
                       flags : ImGuiColorEditFlags = ImGuiColorEditFlags.None;
                       ref_col : float4 = float4(0.0f, 0.0f, 0.0f, 0.0f);
                       has_ref_col : bool = false) : bool {
    //! ``ColorPicker4`` against a caller-owned ``float4?`` (RGBA). Optional reference swatch via ``has_ref_col`` + ``ref_col``.
    let lbl = empty(text) ? widget_ident : text
    var ref_local = ref_col
    let ref_ptr = has_ref_col ? unsafe(addr(ref_local.x)) : null
    let changed = ColorPicker4(lbl, unsafe(reinterpret<float?>(ptr)), flags, ref_ptr)
    edit_finalize(widget_ident, "edit_color_picker4", ptr)
    return changed
}

[edit_widget]
def edit_combo(var ptr : int? implicit; text : string = "";
               items : array<string>;
               popup_max_height_in_items : int = -1) : bool {
    //! ``BeginCombo`` + a ``Selectable`` loop against a caller-owned ``int?`` index. Built this way (not
    //! all-in-one ``Combo()``) so each open-popup item rect lands in sub_bboxes for driver targeting,
    //! like the state-managed ``combo``. The picked index writes straight through ``*ptr``.
    let lbl = empty(text) ? widget_ident : text
    let n = length(items)
    let cur = unsafe(*ptr)
    let preview = (cur >= 0 && cur < n) ? items[cur] : ""
    var changed = false
    var subs : array<float4>
    constrain_combo_popup(popup_max_height_in_items)
    if (BeginCombo(lbl, preview, ImGuiComboFlags.None)) {
        for (i in range(n)) {
            let is_sel = i == cur
            if (Selectable(items[i], is_sel)) {
                unsafe(*ptr) = i
                changed = true
            }
            if (is_sel) {
                SetItemDefaultFocus()
            }
            subs |> push(capture_current_item_rect())
        }
        EndCombo()
    }
    edit_finalize_subs(widget_ident, "edit_combo", ptr, subs)
    return changed
}

[edit_widget]
def edit_slider_angle(var ptr : float? implicit; text : string = "";
                      v_degrees_min : float = -360.0f;
                      v_degrees_max : float = 360.0f;
                      format : string = "%.0f deg";
                      flags : ImGuiSliderFlags = ImGuiSliderFlags.None) : bool {
    //! ``SliderAngle`` against a caller-owned ``float?`` (radians on the pointer side; degrees on the UI side).
    let lbl = empty(text) ? widget_ident : text
    let changed = SliderAngle(lbl, ptr, v_degrees_min, v_degrees_max, format, flags)
    edit_finalize(widget_ident, "edit_slider_angle", ptr)
    return changed
}

[edit_widget]
def edit_checkbox_flags(var ptr : auto(FLAGT)? implicit; text : string = "";
                        flags_value : FLAGT = default<FLAGT>) : bool {
    //! ``CheckboxFlags`` against a caller-owned ``FLAGT?`` enum bitfield; toggles the ``flags_value`` bit. Payload serializes the underlying ``int``; visual state on the bit is implicit.
    let lbl = empty(text) ? widget_ident : text
    var p : FLAGT? = ptr
    unsafe {
        let changed = imgui::CheckboxFlags(lbl, reinterpret<int?>(p), int(flags_value))
        edit_finalize(widget_ident, "edit_checkbox_flags", p)
        return changed
    }
}

[edit_widget]
def edit_collapsing_header(var ptr : bool? implicit; text : string = "";
                           flags : ImGuiTreeNodeFlags = ImGuiTreeNodeFlags.None) : bool {
    //! ``CollapsingHeader(label, p_visible, flags)`` against a caller-owned ``bool?`` p_open. `*ptr ==
    //! false` hides the header entirely; the X close-button writes `*ptr = false`. The `if (...)` body
    //! runs only when visible AND expanded. State-owned sibling: `collapsing_header(closable=true)`.
    let lbl = empty(text) ? widget_ident : text
    var im_open : bool = false
    if (unsafe(*ptr)) {
        im_open = CollapsingHeader(lbl, ptr, flags)
    }
    edit_finalize(widget_ident, "edit_collapsing_header", ptr)
    return im_open
}

// ===== Imperative commands (scroll / focus / clipboard) =====

// Non-`[widget]` `def public` aliases over raw ImGui calls — they take no block and produce no state,
// so they don't fit the [widget] / [container] mold.

def public set_scroll_here_y(center_y_ratio : float = 1.0f) {
    //! `SetScrollHereY(center_y_ratio)` — scrolls so the recently-submitted item is anchored at the
    //! given vertical ratio (0=top, 0.5=middle, 1=bottom). Tail-follow regions call
    //! `set_scroll_here_y(1.0f)` after the last entry.
    SetScrollHereY(center_y_ratio)
}

def public set_keyboard_focus_here(offset : int = 0) {
    //! `SetKeyboardFocusHere(offset)` — moves keyboard focus to the item `offset` ahead (`0`) or behind
    //! (`-1` = the item just submitted). REPL-style inputs use `-1` after Enter to refocus the editbox
    //! for the next command.
    SetKeyboardFocusHere(offset)
}

def public set_item_default_focus() {
    //! `SetItemDefaultFocus()` — marks the previous item as the default
    //! focus target on first frame of a window. Used on the primary input
    //! widget so the user can start typing immediately.
    SetItemDefaultFocus()
}

def public log_to_clipboard(auto_open_depth : int = -1) {
    //! `LogToClipboard(auto_open_depth)` — starts capturing subsequent
    //! text-emitting widgets to the OS clipboard until `log_finish()` is
    //! called. `auto_open_depth = -1` means capture all depths.
    LogToClipboard(auto_open_depth)
}

def public log_finish() {
    //! `LogFinish()` — stops the active log capture started by
    //! `log_to_clipboard` (or any other `LogTo*` call) and finalizes the
    //! clipboard contents.
    LogFinish()
}

def public log_text(s : string) {
    //! `LogText(s)` — append literal text into the active log capture
    //! without rendering. Only meaningful between `log_to_clipboard()`
    //! and `log_finish()` (or other `LogTo*` brackets).
    LogText(s)
}

