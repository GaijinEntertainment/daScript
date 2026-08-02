options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options _allow_imgui_legacy = true

module imgui_containers_builtin shared

require imgui public
require imgui/imgui_boost_runtime public
require imgui/imgui_boost_v2 public
require imgui/imgui_widgets_builtin       // edit_finalize for [edit_container] rails
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
// (group, menu_bar, menu, child, tab_bar, tooltip, item_tooltip). No
// ``[widget_dispatch]`` is registered for those state structs; on a stray
// action targeting one, ``dispatch_or_err`` silently returns ok (the
// Structure-with-no-dispatcher path is treated as display-only). The
// serializer still reflects state for snapshot.
//
// `combo_select` / `list_box` reuse ComboState / ListBoxState (a
// `pending_value : int` shape) and share the per-state-struct dispatcher
// from imgui_widgets_builtin (one ``[widget_dispatch]`` per state type
// covers every kind that uses that state).

def private open_close_finalize(widget_ident : string; kind : string; var state : auto(ST);
                                bbox : float4 = float4(0.0f, 0.0f, 0.0f, 0.0f)) {
    //! Container finalize for state with ``pending_open`` / ``pending_close``. Dispatch routes through ``g_dispatchers`` keyed by ``ST``'s ``TypeInfo.hash`` to the ``[widget_dispatch] def _(var state : ST; …)`` body below.
    var entry : WidgetEntry
    capture_item_bbox(entry, bbox)
    unsafe {
        container_finalize(widget_ident, kind, entry,
                           addr(state),
                           reinterpret<TypeInfo const?>(typeinfo rtti_typeinfo(type<ST>)))
    }
}

def public stateless_finalize(widget_ident : string; kind : string; var state : auto(ST);
                              bbox : float4 = float4(0.0f, 0.0f, 0.0f, 0.0f)) {
    //! Container finalize for stateless containers (no dispatcher actions). Snapshot rail still reflects state for telemetry; no ``[widget_dispatch]`` is registered for ``ST``, so ``g_dispatchers[(*ti).hash]`` is absent and ``dispatch_or_err`` silently returns ok if a stray action ever targets one (Structure-with-no-dispatcher = display-only).
    var entry : WidgetEntry
    capture_item_bbox(entry, bbox)
    unsafe {
        container_finalize(widget_ident, kind, entry,
                           addr(state),
                           reinterpret<TypeInfo const?>(typeinfo rtti_typeinfo(type<ST>)))
    }
}

def private current_item_bbox() : float4 {
    // GetItemRectMin/Max for the "last item" — the clickable header a container's Begin* emits (menu/tab_item); finalizers map it to screen via capture_item_bbox.
    let rmin = GetItemRectMin()
    let rmax = GetItemRectMax()
    return float4(rmin.x, rmin.y, rmax.x, rmax.y)
}

// ===== Notes on ergonomics =====

// `[container]` defs take per-call config via a named-tuple (like `[widget]`), destructured
// positionally: window(MAIN_WIN, (text="Main", closable=false, flags=ImGuiWindowFlags.None)) <| ${body}.
// All config args are required — mid-position defaults can't align the trailing block-arg slot.

// ===== Window family =====

[container]
def window(var state : WindowState; text : string; closable : bool;
           flags : ImGuiWindowFlags; blk : block) {
    //! `Begin(name, p_open, flags)` paired with `End()` whenever Begin was called. With
    //! `closable=true` and `state.open` false we SKIP Begin/End entirely (ImGui idiom: a closed window
    //! stops being drawn). `imgui_open`/`imgui_close` route through pending_open/close.
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
        if (state.pending_size_set) {
            SetNextWindowSize(ImVec2(state.pending_size.x, state.pending_size.y), ImGuiCond.Always)
            state.pending_size_set = false
        }
        if (state.pending_scroll_set) {
            SetNextWindowScroll(ImVec2(state.pending_scroll.x, state.pending_scroll.y))
            state.pending_scroll_set = false
        }
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

// ===== Window mutation / viewport helpers =====

// `SetWindowSize` (current/last window), the `GetCenter()` viewport method, and `IsMousePosValid`
// are thin caller-driven pass-throughs. Wrapped (not allow-listed) so consumer code stays free of
// raw `imgui::*` calls.

def public set_window_size(size : float2; cond : ImGuiCond = ImGuiCond.None) {
    //! `SetWindowSize(size, cond)` — resize the current/last window. Pair with
    //! `SetNextWindowSizeConstraints` to clamp into a fixed range.
    SetWindowSize(size, cond)
}

def public viewport_center(self : ImGuiViewport implicit) : float2 {
    //! `viewport.GetCenter()` — viewport's center-screen position. Pair with
    //! `SetNextWindowPos(center, ImGuiCond.Always, float2(0.5,0.5))` to center
    //! the next window on the viewport.
    return self |> GetCenter()
}

def public is_mouse_pos_valid() : bool {
    //! `IsMousePosValid(NULL)` — true when ImGui has a sensible mouse position
    //! (false on platforms / first frame where the mouse hasn't been observed).
    return IsMousePosValid(null)
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

def public child_scroll_reenter(widget_ident : string; text : string; blk : block) {
    //! Re-enter an existing child (from `child(IDENT, …)`) to nudge scroll via `SetScrollX`/`SetScrollY`
    //! in `blk`. MUST replay the original `PushID(IDENT)` so `BeginChild` hashes to the SAME window —
    //! without it ImGui makes a phantom sibling and the nudge silently no-ops. No registry entry.
    PushID(widget_ident)
    BeginChild(text, float2(0.0f, 0.0f), ImGuiChildFlags.None, ImGuiWindowFlags.None)
    invoke(blk)
    EndChild()
    PopID()
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
def main_menu_bar(var state : MainMenuBarState; blk : block) {
    //! `BeginMainMenuBar()` / `EndMainMenuBar()` — screen-top persistent menu bar; only-on-true
    //! pairing. Unlike `menu_bar` it needs NO enclosing `window(...)` with the MenuBar flag — call
    //! it directly at the top of the per-frame body (after NewFrame, before any window).
    let im_open = BeginMainMenuBar()
    if (im_open) {
        invoke(blk)
        EndMainMenuBar()
    }
    stateless_finalize(widget_ident, "main_menu_bar", state)
}

[container]
def menu(var state : MenuState; text : string; enabled : bool; blk : block) {
    //! `BeginMenu(label, enabled)` / `EndMenu()` — only-on-true pairing.
    //! `enabled` is a per-call knob; `state.opened` mirrors the per-frame
    //! BeginMenu return so snapshots tell whether the dropdown is showing.
    state.enabled = enabled
    let im_open = BeginMenu(text, enabled)
    state.opened = im_open
    // Capture the menu HEADER rect right after BeginMenu — it's the parent-strip clickable item
    // ``IsItem*`` resolves against. Deferring to ``stateless_finalize`` after EndMenu would capture
    // something inside the cascade body, useless for recording drivers / playwright clicks.
    let header_bbox = current_item_bbox()
    if (im_open) {
        invoke(blk)
        EndMenu()
    }
    stateless_finalize(widget_ident, "menu", state, header_bbox)
}

// ===== Popup family =====

[container]
def popup(var state : PopupState; text : string;
          flags : ImGuiWindowFlags; blk : block) {
    //! `BeginPopup(name, flags)` / `EndPopup()` — only-on-true pairing. `imgui_open` enqueues
    //! `OpenPopup(text)` next frame (opens via ImGui's internal stack); `imgui_close` enqueues a
    //! `CloseCurrentPopup` inside the active popup body.
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
def popup_context_item(var state : PopupContextItemState; str_id : string;
                       flags : ImGuiPopupFlags; blk : block) {
    //! `BeginPopupContextItem(str_id, flags)` / `EndPopup()` — right-click
    //! context popup attached to the previously submitted item. ImGui drives
    //! open/close internally on right-click; body runs only while open.
    let im_open = BeginPopupContextItem(str_id, flags)
    if (im_open) {
        invoke(blk)
        EndPopup()
    }
    stateless_finalize(widget_ident, "popup_context_item", state)
}

[container]
def popup_context_window(var state : PopupContextWindowState; str_id : string;
                         flags : ImGuiPopupFlags; blk : block) {
    //! `BeginPopupContextWindow(str_id, flags)` / `EndPopup()` — right-click inside the enclosing
    //! window opens a context popup; ImGui drives open/close, body runs only while open. Pass a
    //! unique non-empty `str_id` (the C `"##ContextWindow"` default needs a NULL daslang can't pass).
    let im_open = BeginPopupContextWindow(str_id, flags)
    if (im_open) {
        invoke(blk)
        EndPopup()
    }
    stateless_finalize(widget_ident, "popup_context_window", state)
}

[container]
def popup_window(var state : PopupWindowState; str_id : string;
                 flags : ImGuiWindowFlags; blk : block) {
    //! `BeginPopup(str_id, flags)` / `EndPopup()` — stateless manual-trigger; caller drives opening via
    //! `open_popup(str_id, flags)`, body runs only while ImGui reports it open. Peels `widget_ident` off
    //! the ID stack around Begin/EndPopup so BeginPopup's hash matches the caller's OpenPopup (called outside the IDENT push).
    PopID()
    let im_open = BeginPopup(str_id, flags)
    if (im_open) {
        // Body renders under widget_ident inside the popup window; pop it before
        // EndPopup so the popup's ID stack balances (1.92 errors on a leftover push).
        PushID(widget_ident)
        invoke(blk)
        PopID()
        EndPopup()
    }
    // Restore widget_ident on the parent stack for the [container] finalize's pop.
    PushID(widget_ident)
    stateless_finalize(widget_ident, "popup_window", state)
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

// ===== Popup control (trigger paired with popup / popup_modal containers) =====

def open_popup(var state : PopupState) : void {
    //! Queue `OpenPopup(state.text)` for the paired `popup`/`popup_modal` container's next visit —
    //! it reads `pending_open` before `BeginPopup`, so the button-click → popup linkage is observable
    //! as a state mutation (visible to live/mouse-cards).
    state.pending_open = true
}

def open_popup(str_id : string;
               flags : ImGuiPopupFlags = ImGuiPopupFlags.None) : void {
    //! String-id form for stateless popups (paired with `popup_context_item`).
    //! No telemetry — open-state lives in ImGui internals.
    OpenPopup(str_id, flags)
}

def open_popup_on_item_click(str_id : string;
                             flags : ImGuiPopupFlags = ImGuiPopupFlags.MouseButtonRight) : void {
    //! Right-click on the previously-submitted item opens `str_id` popup.
    //! Stateless trigger paired with `popup_context_item`.
    OpenPopupOnItemClick(str_id, flags)
}

def open_popup_on_item_click(var state : PopupState;
                             flags : ImGuiPopupFlags = ImGuiPopupFlags.MouseButtonRight) : void {
    //! Stateful right-click trigger for `popup`/`popup_modal` containers: checks ``IsMouseReleased`` +
    //! ``IsItemHovered``, then queues ``state.pending_open`` so the container opens the popup inside its own
    //! ``PushID(IDENT)`` scope. Use this — the str_id form's ``GetID(str_id)`` won't match the container's push.
    let mb = int(flags) & int(ImGuiPopupFlags.MouseButtonMask_)
    if (IsMouseReleased(unsafe(reinterpret<ImGuiMouseButton>(mb)))
            && IsItemHovered(ImGuiHoveredFlags.AllowWhenBlockedByPopup)) {
        state.pending_open = true
    }
}

def close_current_popup(var state : PopupState) : void {
    //! Close the active popup. Sets `pending_close` for state-transition
    //! telemetry AND calls `CloseCurrentPopup()` directly so the close
    //! takes effect on the current frame (matches raw-call semantics).
    state.pending_close = true
    CloseCurrentPopup()
}

def close_current_popup() : void {
    //! No-state form for body-internal use where a `PopupState` ident isn't
    //! in scope (e.g., inside `popup_context_item`, which is stateless).
    CloseCurrentPopup()
}

def is_popup_open(state : PopupState) : bool {
    //! True while the paired popup container is open this frame.
    return state.open
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
    //! `BeginTabItem(label, p_open, flags)` / `EndTabItem()` — only-on-true. With `closable=true` the
    //! X-button toggles `state.open`, and we skip BeginTabItem entirely once it's false. `state.open`
    //! defaults to true (see TabItemState) so closable tabs render from frame 1.
    if (state.pending_open) {
        state.open = true
        state.pending_open = false
    }
    if (state.pending_close) {
        state.open = false
        state.pending_close = false
    }
    // imgui_select queues activation: pass SetSelected for one frame, then clear.
    var eff_flags = flags
    if (state.pending_select) {
        eff_flags |= ImGuiTabItemFlags.SetSelected
        state.pending_select = false
    }
    state.flags = flags
    if (!closable || state.open) {
        var p_open : bool? = null
        if (closable) {
            unsafe {
                p_open = addr(state.open)
            }
        }
        let im_open = BeginTabItem(text, p_open, eff_flags)
        state.selected = im_open   // observable active-tab state (snapshot-visible)
        //! Same rationale as ``menu()``: the tab strip header is the
        //! ``IsItem*`` target right after BeginTabItem. Inside the body
        //! ``last item`` shifts to per-widget items and EndTabItem pops it.
        let header_bbox = current_item_bbox()
        if (im_open) {
            invoke(blk)
            EndTabItem()
        }
        open_close_finalize(widget_ident, "tab_item", state, header_bbox)
    } else {
        open_close_finalize(widget_ident, "tab_item", state)
    }
}

[edit_container]
def edit_tab_item(var p_open : bool? implicit; text : string;
                  flags : ImGuiTabItemFlags; blk : block) {
    //! ``BeginTabItem(label, p_open, flags)`` / ``EndTabItem()`` against a caller-owned ``bool?``.
    //! External-pointer sibling of ``tab_item``: ``*p_open == false`` skips the tab (chrome + body); the
    //! X button writes ``*p_open = false``. For shared open state a second surface drives. Same ``id="..."`` rule.
    var header_bbox = float4(0.0f, 0.0f, 0.0f, 0.0f)
    if (unsafe(*p_open)) {
        let im_open = BeginTabItem(text, p_open, flags)
        // Same rationale as `tab_item`: the tab strip header is the IsItem*
        // target right after BeginTabItem; capture its bbox before the body
        // pushes inner items.
        header_bbox = current_item_bbox()
        if (im_open) {
            invoke(blk)
            EndTabItem()
        }
    }
    edit_container_open_close_finalize(widget_ident, "edit_tab_item", p_open, header_bbox)
}

// ===== Wrapping tab bar (multi-row) =====

// ImGui's native TabBar shrinks/ellipsizes or scrolls tabs; it never wraps to rows.
// `wrap_tab_bar`/`wrap_tab_item` draw the headers as a wrapped button strip and render only the
// active tab's body. The strip lays out from the PREVIOUS frame's labels, so a change is one frame late.

struct private WrapCtx {
    value : int               // active tab index for this bar
    count : int               // running declaration index of wrap_tab_item calls
    labels : array<string>    // labels collected this frame -> state.labels
}
var private g_wrap : array<WrapCtx>   // bar stack (supports nesting)

[widget_dispatch]
def _wrap_tab_bar_dispatch(var state : WrapTabBarState; action : string; payload : string) {
    if (action == "set") {
        unsafe {
            sscan_json_at(payload, addr(state.pending_value),
                          *reinterpret<TypeInfo const?>(typeinfo rtti_typeinfo(type<int>)))
        }
        state.has_pending = true
    }
}

def private draw_wrap_strip(var state : WrapTabBarState) {
    let n = length(state.labels)
    return if (n == 0)
    state.value = clamp(state.value, 0, n - 1)
    let pad = GetStyle().FramePadding.x
    let spacing = GetStyle().ItemSpacing.x
    let vis_x2 = GetCursorScreenPos().x + GetContentRegionAvail().x
    var last_x2 = 0.0f
    for (i in range(n)) {
        let lbl = state.labels[i]
        if (i > 0) {
            let bw = CalcTextSize(lbl).x + pad * 2.0f
            if (last_x2 + spacing + bw < vis_x2) {
                SameLine()
            }
        }
        let is_active = i == state.value
        if (is_active) {
            PushStyleColor(ImGuiCol.Button, GetStyleColorVec4(ImGuiCol.ButtonActive))
        }
        if (Button(lbl, ImVec2(0.0f, 0.0f))) {
            state.value = i
        }
        // Surface each header as <bar>/<label> so a real synth click can select a tab by name (lands in state.value, which the container serializes for verification).
        register_clickable_bbox(lbl, lbl, "wrap_tab_item")
        if (is_active) {
            PopStyleColor(1)
        }
        last_x2 = GetItemRectMax().x
    }
    Separator()
}

[container]
def wrap_tab_bar(var state : WrapTabBarState; blk : block) {
    //! A multi-row, wrapping tab bar (native `tab_bar` shrinks/ellipsizes/scrolls instead). Active
    //! tab is an index; `imgui_force_set {"value":N}` selects tab N next frame. Pair with `wrap_tab_item`.
    //! The header strip lays out from the previous frame's labels, so a changed label set is one frame late.
    if (state.has_pending) {
        state.value = state.pending_value
        state.has_pending = false
    }
    draw_wrap_strip(state)
    var ctx : WrapCtx
    ctx.value = state.value
    g_wrap |> emplace(ctx)
    invoke(blk)
    let top = length(g_wrap) - 1
    state.labels <- g_wrap[top].labels
    g_wrap |> resize(top)
    pending_value_container_finalize(widget_ident, "wrap_tab_bar", state)
}

def public wrap_tab_item(text : string; blk : block) {
    //! One tab of a `wrap_tab_bar`. Its body block renders only while this tab
    //! is the active one. Tabs are indexed in declaration order — keep the
    //! order stable across frames (the header strip is laid out a frame behind).
    let top = length(g_wrap) - 1
    return if (top < 0)
    let idx = g_wrap[top].count
    g_wrap[top].count = idx + 1
    g_wrap[top].labels |> push(text)
    if (idx == g_wrap[top].value) {
        PushID(text)
        invoke(blk)
        PopID()
    }
}

// ===== Tooltip family =====

[container]
def tooltip(var state : TooltipState; blk : block) {
    //! `BeginTooltip()` / `EndTooltip()` — only-on-true pairing. Caller owns the IsItemHovered (or
    //! other) gate around invoking it, else the tooltip renders every frame it's reached. For the
    //! "show only when item hovered" idiom use `item_tooltip` instead.
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
    //! `TreeNodeEx(label, flags) : bool` paired with `TreePop()` only when the node returned true
    //! (user expanded it). `imgui_open`/`imgui_close` route through `SetNextItemOpen` so the next
    //! frame's TreeNodeEx already reflects the requested state.
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
    // The clickable header (chevron + label) is the IsItem* target right
    // after TreeNodeEx; capture its rect before the body pushes inner items
    // so a driver can click the header/arrow like a human (mirrors tab_item).
    let header_bbox = current_item_bbox()
    if (im_open) {
        invoke(blk)
        TreePop()
    }
    open_close_finalize(widget_ident, "tree_node", state, header_bbox)
}

[container]
def collapsing_header(var state : CollapsingHeaderState; text : string;
                      closable : bool; flags : ImGuiTreeNodeFlags; blk : block) {
    //! `CollapsingHeader(label, p_visible, flags) : bool` — like TreeNode but WITHOUT a TreePop pair
    //! (ImGui closes it itself). With `closable=true` the X-button flips `state.open` false and the
    //! section hides entirely. Body runs only when expanded AND (if closable) still open.
    if (state.pending_open) {
        SetNextItemOpen(true, ImGuiCond.Always)
        state.open = true
        state.pending_open = false
    }
    if (state.pending_close) {
        SetNextItemOpen(false, ImGuiCond.Always)
        state.pending_close = false
    }
    state.flags = flags
    var im_open : bool
    // The header strip (chevron + label, + X when closable) is the IsItem* target right after
    // CollapsingHeader; capture its rect before the body pushes inner items so a driver can click
    // the header/arrow (mirrors tree_node). Stays zero while a closable section is hidden.
    var header_bbox = float4(0.0f, 0.0f, 0.0f, 0.0f)
    if (closable) {
        if (state.open) {
            unsafe {
                im_open = CollapsingHeader(text, addr(state.open), flags)
            }
            header_bbox = current_item_bbox()
        }
        // If state.open is false, the section is hidden — don't issue the
        // ImGui call. CollapsingHeader-with-p_visible only renders while
        // *p_visible is true; we mirror that gate exactly.
    } else {
        im_open = CollapsingHeader(text, flags)
        header_bbox = current_item_bbox()
    }
    state.opened = im_open
    if (im_open) {
        invoke(blk)
    }
    open_close_finalize(widget_ident, "collapsing_header", state, header_bbox)
}

// ===== Choice block-arg family =====

// `combo_select` is the BeginCombo counterpart to the leaf `combo`; `list_box` is unified on
// BeginListBox/EndListBox (legacy items-array form dropped). Both reuse selection state (ComboState/
// ListBoxState) — `value : int` is the selection index; the dispatcher's "set" writes it for next frame.

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
        container_finalize(widget_ident, "combo_select", entry,
                           addr(state),
                           reinterpret<TypeInfo const?>(typeinfo rtti_typeinfo(type<ComboState>)))
    }
}

[container]
def list_box(var state : ListBoxState; text : string; size : float2; blk : block) {
    //! `BeginListBox(label, size)` / `EndListBox()` — only-on-true. Body
    //! emits the per-item rows. `imgui_force_set {"value":N}` steers next-frame
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
        container_finalize(widget_ident, "list_box", entry,
                           addr(state),
                           reinterpret<TypeInfo const?>(typeinfo rtti_typeinfo(type<ListBoxState>)))
    }
}

// ===== Drag-drop family =====

// Both bodies gate on ImGui's internal drag-drop state: `drag_drop_source` runs only while the
// previous item is picked up; `drag_drop_target` runs only while a payload-bearing drag hovers.
// Payload set/accept are raw `SetDragDropPayload`/`AcceptDragDropPayload` (caller-owned, cross-widget data).

[container]
def drag_drop_source(var state : DragDropSourceState; flags : ImGuiDragDropFlags;
                     blk : block) {
    //! `BeginDragDropSource(flags)` / `EndDragDropSource()` — only-on-true.
    //! Body calls `SetDragDropPayload(type, data, sz, cond)` to publish the
    //! payload and renders the drag preview (Text, Image, etc.).
    let im_open = BeginDragDropSource(flags)
    if (im_open) {
        invoke(blk)
        EndDragDropSource()
    }
    stateless_finalize(widget_ident, "drag_drop_source", state)
}

[container]
def drag_drop_target(var state : DragDropTargetState; blk : block) {
    //! `BeginDragDropTarget()` / `EndDragDropTarget()` — only-on-true. Body calls `AcceptDragDropPayload`
    //! to receive the payload on drop. `state.accepted` counts the drops ImGui delivers here — the body's
    //! accept is raw user code invisible to telemetry, so this is the rail a driver/test asserts against.
    let im_open = BeginDragDropTarget()
    if (im_open) {
        invoke(blk)
        // ImGui flips the active payload's `Delivery` true on the one frame a
        // drop releases over an accepting target. We only read it inside
        // `im_open`, so the count is attributed to the hovered target.
        let pl = GetDragDropPayload()
        if (pl != null && pl.Delivery) {
            state.accepted++
        }
        EndDragDropTarget()
    }
    stateless_finalize(widget_ident, "drag_drop_target", state)
}

// ===== Per-state-struct dispatchers (containers) =====

// One [widget_dispatch] body per state struct, keyed by ti.hash, fires for every container kind
// sharing that struct (popup + popup_modal share PopupState, etc.). TreeNode/Combo/ListBox dispatchers
// live in imgui_widgets_builtin. Stateless containers get no dispatcher and silently no-op.

[widget_dispatch]
def _window_dispatch(var state : WindowState; action : string; payload : string) {
    if (action == "open") {
        state.pending_open = true
    } elif (action == "close") {
        state.pending_close = true
    } elif (action == "set_window_size") {
        // Payload `{w, h}` (both optional, else current size). Used by record_window_size_constraints
        // to demo SetNextWindowSizeConstraints callbacks (snap/aspect/quantize): request odd sizes and
        // watch the constraint reshape them.
        var perr : string
        let v = read_json(payload, perr)
        if (v != null) {
            state.pending_size = float2(v?["w"] ?? state.size.x,
                                        v?["h"] ?? state.size.y)
            state.pending_size_set = true
        }
    }
}

[widget_dispatch]
def _popup_dispatch(var state : PopupState; action : string; payload : string) {
    if (action == "open") {
        state.pending_open = true
    } elif (action == "close") {
        state.pending_close = true
    }
}

[widget_dispatch]
def _tab_item_dispatch(var state : TabItemState; action : string; payload : string) {
    if (action == "open") {
        state.pending_open = true
    } elif (action == "close") {
        state.pending_close = true
    } elif (action == "select") {
        state.pending_select = true
    }
}

[widget_dispatch]
def _collapsing_header_dispatch(var state : CollapsingHeaderState; action : string; payload : string) {
    if (action == "open") {
        state.pending_open = true
    } elif (action == "close") {
        state.pending_close = true
    }
}
