options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module imgui_containers_builtin shared

require imgui public
require imgui/imgui_boost_runtime public
require imgui/imgui_boost_v2 public
require daslib/json public
require daslib/json_boost
require math

//! Phase 0b.3 container widgets — block-arg wrappers around ImGui's Begin/End
//! family. Each is annotated with `[container]`, which wraps the body with
//! `container_path_push(widget_ident)` / `container_path_pop()` so leaf
//! widgets rendered inside `invoke(blk)` get hierarchical registry keys
//! ("MAIN_WIN/SETTINGS_TAB/RPS"). The Begin → invoke → End pairing rules
//! differ per kind (Begin/End and BeginChild/EndChild always-pair; everything
//! else End-only-on-true), so each def authors that pairing inline rather
//! than the macro guessing.

// ===== Common finalize helpers =====
//
// Two shapes cover the 15 containers' dispatch needs:
//
// `open_close_finalize` — for state with `pending_open` / `pending_close`
// fields (window, popup, popup_modal, tab_item, tree_node, collapsing_header).
// Routes `imgui_open` / `imgui_close` actions to the pending flags; the
// renderer consumes them at the top of the next frame.
//
// `stateless_finalize` — for containers whose dispatch surface is empty
// (group, menu_bar, menu, child, tab_bar, tooltip, item_tooltip). The
// dispatcher lambda is a no-op; the serializer still reflects state for
// snapshot.
//
// `combo_select` / `list_box_select` reuse ComboState / ListBoxState (a
// `pending_value : int` shape) and inline their own dispatcher because the
// shared `pending_value_finalize` helper is private to imgui_widgets_builtin.

def private open_close_finalize(widget_ident : string; kind : string; var state : auto) {
    var entry : WidgetEntry
    unsafe {
        let ser <- @ capture(& state) () : JsonValue ? {
            return JV(state)
        }
        let disp <- @ capture(& state) (action : string; payload : JsonValue ?) {
            if (action == "open") {
                state.pending_open = true
            } elif (action == "close") {
                state.pending_close = true
            }
        }
        container_finalize(widget_ident, kind, entry, ser, disp)
    }
}

def private stateless_finalize(widget_ident : string; kind : string; var state : auto) {
    var entry : WidgetEntry
    unsafe {
        let ser <- @ capture(& state) () : JsonValue ? {
            return JV(state)
        }
        let disp <- @ capture(& state) (action : string; payload : JsonValue ?) {
            // No actions; serializer is the only telemetry surface for stateless containers.
        }
        container_finalize(widget_ident, kind, entry, ser, disp)
    }
}

// ===== Notes on ergonomics =====
//
// `[container]` defs follow the same named-tuple convention as `[widget]`:
// per-call config goes through the tuple, e.g.
//
//   window(MAIN_WIN, (text="Main", closable=false, flags=ImGuiWindowFlags.None)) <| ${body}
//
// The CallMacro destructures the named-tuple positionally (matching the
// function's parameter declaration order — see mouse card
// `dasimgui-widget-named-tuple-args-positional-not-named`). The block-as-
// last-arg sugar appends the block at the trailing slot, so the function
// signature is `(state, widget_ident, …named-tuple positional args…, blk)`.
//
// Defaults at the end DON'T work for [container] (they'd land the block in
// the wrong slot — daslang positional resolution doesn't auto-fill mid-
// position defaults to align a trailing block-arg). All per-call config args
// are required; the user passes them all in the named-tuple. The default
// values from imgui (e.g. ImGuiWindowFlags.None) are spelled out at the
// call site, mirroring how other dasImgui widgets surface flag args.

// ===== Window family =====

[container]
def window(var state : WindowState; text : string; closable : bool;
           flags : ImGuiWindowFlags; blk : block) {
    //! `Begin(name, p_open, flags)` paired with `End()` whenever Begin was
    //! called. When `closable=true` and `state.open` is false, we SKIP
    //! Begin/End entirely (the ImGui idiom — a closable window stops being
    //! drawn once the X-button or imgui_close fires; the application is
    //! expected to stop calling Begin, not continue with `*p_open=false`).
    //! `imgui_open` / `imgui_close` route through pending_open/close.
    //! `state.open` defaults to true (see WindowState) so closable windows
    //! render from frame 1.
    if (state.pending_open) {
        state.open = true
        state.pending_open = false
    }
    if (state.pending_close) {
        state.open = false
        state.pending_close = false
    }
    state.flags = flags
    if (!closable || state.open) {
        var p_open : bool? = null
        if (closable) {
            unsafe {
                p_open = addr(state.open)
            }
        }
        let im_open = Begin(text, p_open, flags)
        if (im_open) {
            let pos = GetWindowPos()
            let size = GetWindowSize()
            state.pos = float2(pos.x, pos.y)
            state.size = float2(size.x, size.y)
            state.collapsed = IsWindowCollapsed()
            state.scroll = float2(GetScrollX(), GetScrollY())
            invoke(blk)
            // scroll_max derives from the content extent — must be read
            // AFTER the body emits items, before End() pops the window.
            state.scroll_max = float2(GetScrollMaxX(), GetScrollMaxY())
        }
        End()
    }
    open_close_finalize(widget_ident, "window", state)
}

[container]
def child(var state : ChildState; text : string; size : float2;
          child_flags : ImGuiChildFlags; window_flags : ImGuiWindowFlags;
          blk : block) {
    //! `BeginChild(str, size, child_flags, window_flags)` paired unconditionally
    //! with `EndChild()` per modern ImGui's "must always end" rule. Scroll
    //! position is observed when the child rendered.
    state.size = size
    state.child_flags = child_flags
    state.window_flags = window_flags
    let im_open = BeginChild(text, size, child_flags, window_flags)
    if (im_open) {
        state.scroll = float2(GetScrollX(), GetScrollY())
        invoke(blk)
        // scroll_max derives from the content extent — read AFTER invoke.
        state.scroll_max = float2(GetScrollMaxX(), GetScrollMaxY())
    }
    EndChild()
    stateless_finalize(widget_ident, "child", state)
}

[container]
def group(var state : GroupState; blk : block) {
    //! `BeginGroup()` / `EndGroup()` — pure layout grouping, no return value
    //! to gate on, no per-frame state to capture.
    BeginGroup()
    invoke(blk)
    EndGroup()
    stateless_finalize(widget_ident, "group", state)
}

// ===== Menu family =====

[container]
def menu_bar(var state : MenuBarState; blk : block) {
    //! `BeginMenuBar()` / `EndMenuBar()` — only-on-true pairing. Renders the
    //! menu bar of the enclosing window (Begin must have been called with
    //! ImGuiWindowFlags.MenuBar) or a child window.
    let im_open = BeginMenuBar()
    if (im_open) {
        invoke(blk)
        EndMenuBar()
    }
    stateless_finalize(widget_ident, "menu_bar", state)
}

[container]
def menu(var state : MenuState; text : string; enabled : bool; blk : block) {
    //! `BeginMenu(label, enabled)` / `EndMenu()` — only-on-true pairing.
    //! `enabled` is a per-call knob; `state.opened` mirrors the per-frame
    //! BeginMenu return so snapshots tell whether the dropdown is showing.
    state.enabled = enabled
    let im_open = BeginMenu(text, enabled)
    state.opened = im_open
    if (im_open) {
        invoke(blk)
        EndMenu()
    }
    stateless_finalize(widget_ident, "menu", state)
}

// ===== Popup family =====

[container]
def popup(var state : PopupState; text : string;
          flags : ImGuiWindowFlags; blk : block) {
    //! `BeginPopup(name, flags)` / `EndPopup()` — only-on-true pairing.
    //! `imgui_open` enqueues OpenPopup(text) on the next frame; the popup
    //! then opens via ImGui's internal stack. `imgui_close` enqueues a
    //! CloseCurrentPopup inside the active popup body.
    if (state.pending_open) {
        OpenPopup(text, ImGuiPopupFlags.None)
        state.open = true
        state.pending_open = false
    }
    state.flags = flags
    let im_open = BeginPopup(text, flags)
    if (im_open) {
        if (state.pending_close) {
            CloseCurrentPopup()
            state.open = false
            state.pending_close = false
        }
        invoke(blk)
        EndPopup()
    } else {
        // ImGui closed the popup (click outside, ESC, …); reflect it.
        state.open = false
        state.pending_close = false
    }
    open_close_finalize(widget_ident, "popup", state)
}

[container]
def popup_modal(var state : PopupState; text : string; closable : bool;
                flags : ImGuiWindowFlags; blk : block) {
    //! `BeginPopupModal(name, p_open, flags)` / `EndPopup()`. Same lifecycle
    //! as `popup`; with `closable=true`, ImGui's X-button is wired to
    //! `state.open` (the click flips it false and ImGui will route a close).
    if (state.pending_open) {
        OpenPopup(text, ImGuiPopupFlags.None)
        state.open = true
        state.pending_open = false
    }
    state.flags = flags
    var p_open : bool? = null
    if (closable) {
        unsafe {
            p_open = addr(state.open)
        }
    }
    let im_open = BeginPopupModal(text, p_open, flags)
    if (im_open) {
        if (state.pending_close) {
            CloseCurrentPopup()
            state.open = false
            state.pending_close = false
        }
        invoke(blk)
        EndPopup()
    } else {
        state.open = false
        state.pending_close = false
    }
    open_close_finalize(widget_ident, "popup_modal", state)
}

// ===== Tab family =====

[container]
def tab_bar(var state : TabBarState; text : string;
            flags : ImGuiTabBarFlags; blk : block) {
    //! `BeginTabBar(id, flags)` / `EndTabBar()` — only-on-true pairing.
    //! Active-tab selection lives in ImGui's internal storage; we just
    //! observe via the body of each tab_item.
    state.flags = flags
    let im_open = BeginTabBar(text, flags)
    if (im_open) {
        invoke(blk)
        EndTabBar()
    }
    stateless_finalize(widget_ident, "tab_bar", state)
}

[container]
def tab_item(var state : TabItemState; text : string; closable : bool;
             flags : ImGuiTabItemFlags; blk : block) {
    //! `BeginTabItem(label, p_open, flags)` / `EndTabItem()` — only-on-true.
    //! With `closable=true` the X-button toggles `state.open`; we skip
    //! BeginTabItem entirely once `state.open` is false. `state.open` defaults
    //! to true (see TabItemState) so closable tabs render from frame 1.
    if (state.pending_open) {
        state.open = true
        state.pending_open = false
    }
    if (state.pending_close) {
        state.open = false
        state.pending_close = false
    }
    state.flags = flags
    if (!closable || state.open) {
        var p_open : bool? = null
        if (closable) {
            unsafe {
                p_open = addr(state.open)
            }
        }
        let im_open = BeginTabItem(text, p_open, flags)
        if (im_open) {
            invoke(blk)
            EndTabItem()
        }
    }
    open_close_finalize(widget_ident, "tab_item", state)
}

// ===== Tooltip family =====

[container]
def tooltip(var state : TooltipState; blk : block) {
    //! `BeginTooltip()` / `EndTooltip()` — only-on-true pairing in modern ImGui.
    //! Caller is responsible for the IsItemHovered gate (or any other gate)
    //! around invoking this widget — tooltips render every frame they're
    //! reached otherwise. For the "show only when item hovered" idiom, use
    //! `item_tooltip` instead.
    let im_open = BeginTooltip()
    if (im_open) {
        invoke(blk)
        EndTooltip()
    }
    stateless_finalize(widget_ident, "tooltip", state)
}

[container]
def item_tooltip(var state : TooltipState; blk : block) {
    //! `BeginItemTooltip()` / `EndTooltip()` — only-on-true. Equivalent to
    //! `if (IsItemHovered()) { BeginTooltip(); ...; EndTooltip(); }` but
    //! with ImGui's lazy-evaluation of the hover gate.
    let im_open = BeginItemTooltip()
    if (im_open) {
        invoke(blk)
        EndTooltip()
    }
    stateless_finalize(widget_ident, "item_tooltip", state)
}

// ===== Tree family =====

[container]
def tree_node(var state : TreeNodeState; text : string;
              flags : ImGuiTreeNodeFlags; blk : block) {
    //! `TreeNodeEx(label, flags) : bool` paired with `TreePop()` only when
    //! the node returned true (i.e. the user expanded it). `imgui_open` /
    //! `imgui_close` route through `SetNextItemOpen` so the next frame's
    //! TreeNodeEx call already reflects the requested state.
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
    if (im_open) {
        invoke(blk)
        TreePop()
    }
    open_close_finalize(widget_ident, "tree_node", state)
}

[container]
def collapsing_header(var state : CollapsingHeaderState; text : string;
                      flags : ImGuiTreeNodeFlags; blk : block) {
    //! `CollapsingHeader(label, flags) : bool` — same shape as TreeNode but
    //! WITHOUT a TreePop pair (ImGui handles the close on its own). The
    //! body runs only when the header is expanded.
    if (state.pending_open) {
        SetNextItemOpen(true, ImGuiCond.Always)
        state.pending_open = false
    }
    if (state.pending_close) {
        SetNextItemOpen(false, ImGuiCond.Always)
        state.pending_close = false
    }
    state.flags = flags
    let im_open = CollapsingHeader(text, flags)
    state.opened = im_open
    if (im_open) {
        invoke(blk)
    }
    open_close_finalize(widget_ident, "collapsing_header", state)
}

// ===== Choice block-arg family =====
//
// `combo_select` and `list_box_select` are the BeginCombo / BeginListBox
// counterparts to the leaf `combo` / `list_box` widgets shipped in 0b.2-choice.
// They reuse the same state structs (ComboState / ListBoxState) — `value : int`
// is the user-driven selection index. Body code is responsible for emitting
// Selectable items inside; the dispatcher's "set" action just writes
// `state.value` for the next frame. Per-call args (preview_value, flags, size)
// follow the named-tuple convention.

[container]
def combo_select(var state : ComboState; text : string; preview_value : string;
                 flags : ImGuiComboFlags; blk : block) {
    //! `BeginCombo(label, preview, flags)` / `EndCombo()` — only-on-true.
    //! `preview_value` is the closed-state label; the body emits the
    //! per-item Selectable rows. Pending value flow matches the leaf combo.
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let im_open = BeginCombo(text, preview_value, flags)
    if (im_open) {
        invoke(blk)
        EndCombo()
    }
    var entry : WidgetEntry
    unsafe {
        let ser <- @ capture(& state) () : JsonValue ? {
            return JV(state)
        }
        let disp <- @ capture(& state) (action : string; payload : JsonValue ?) {
            if (action == "set") {
                state.pending_value = payload?["value"] ?? 0
                state.has_pending = true
            }
        }
        container_finalize(widget_ident, "combo_select", entry, ser, disp)
    }
}

[container]
def list_box_select(var state : ListBoxState; text : string; size : float2; blk : block) {
    //! `BeginListBox(label, size)` / `EndListBox()` — only-on-true. Body
    //! emits the per-item rows. `imgui_set {"value":N}` steers next-frame
    //! selection; bodies typically read `state.value` to tag the active row.
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    let im_open = BeginListBox(text, size)
    if (im_open) {
        invoke(blk)
        EndListBox()
    }
    var entry : WidgetEntry
    unsafe {
        let ser <- @ capture(& state) () : JsonValue ? {
            return JV(state)
        }
        let disp <- @ capture(& state) (action : string; payload : JsonValue ?) {
            if (action == "set") {
                state.pending_value = payload?["value"] ?? 0
                state.has_pending = true
            }
        }
        container_finalize(widget_ident, "list_box_select", entry, ser, disp)
    }
}
