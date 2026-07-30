options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module imgui_docking_builtin shared

require imgui public
require imgui/imgui_boost_runtime public
require imgui/imgui_boost_v2 public
require daslib/json public
require daslib/json_boost
require math

//! Phase 7 docking — boost wrappers around ImGui's native DockSpace + Begin/End
//! dockable windows. `dockspace(NAME, ...)` creates a full-viewport dockspace
//! and captures its dock_id into `state.dock_id` so user code can call
//! DockBuilder*(state.dock_id) to seed an initial layout. `dock_window(NAME, ...)`
//! is a registry-aware wrapper around Begin/End with `closable` (X-button
//! routes to `state.open`) plus pending-flag plumbing for live commands
//! (imgui_dock / imgui_undock / imgui_focus). Path-prefix on child widgets
//! works the same way as containers_builtin's `window` — `dock_window(EXPLORER)
//! { button(REFRESH_BTN, ...) }` registers `EXPLORER/REFRESH_BTN`.

struct DockSpaceState {
    //! Backs the `dockspace` container. `dock_id` (captured from DockSpaceOverViewport on first render) drives
    //! the user's DockBuilder*(state.dock_id) layout calls; `pending_reset` (from `imgui_dock_reset`) makes the
    //! renderer clear DockBuilder state + `has_initial_layout` so the user's setup function runs again.
    @live dock_id : uint = 0u                 //! viewport dockspace id; captured on first render
    @optional has_initial_layout : bool = false //! flips true once user has seeded a layout; user reads this to gate their setup
    @optional flags : ImGuiDockNodeFlags      //! per-call flags (echoed for telemetry)
    @optional pending_reset : bool            //! imgui_dock_reset queued; applied next frame
}

struct DockWindowState {
    //! Backs the `dock_window` container (mirrors WindowState). `open` is @live so closable windows survive
    //! reload; the `pending_*` flags flow from live commands and apply via SetNextWindow* at the top of the next
    //! frame — per-field semantics on the fields below (the `*_set` gates make ``(0,0)`` a valid request, not a sentinel).
    @live open : bool = true                  //! window open state; preserved across reload
    @optional flags : ImGuiWindowFlags        //! per-call window flags (echoed for telemetry)
    @optional pos : float2                    //! observed window position
    @optional size : float2                   //! observed window size
    @optional docked : bool                   //! observed IsWindowDocked()
    @optional dock_id : uint                  //! observed GetWindowDockID()
    @optional pending_open : bool             //! imgui_open queued; applied next frame
    @optional pending_close : bool            //! imgui_close queued; applied next frame
    @optional pending_dock_to : uint = 0u     //! imgui_dock queued; SetNextWindowDockID(value) next frame
    @optional pending_undock : bool           //! imgui_undock queued; SetNextWindowDockID(0) next frame
    @optional pending_raise : bool            //! imgui_raise queued; SetNextWindowFocus() next frame (selects the tab when docked)
    @optional pending_pos : float2            //! imgui_set_window_pos queued — pos
    @optional pending_pos_set : bool          //! gates pending_pos (so (0,0) is a valid request)
    @optional pending_size : float2           //! imgui_set_window_pos queued — size
    @optional pending_size_set : bool         //! gates pending_size
}

// ===== Finalize helpers =====

// `dockspace_finalize` installs `imgui_dock_reset`; `dock_window_finalize` installs the union of
// open/close/dock/undock actions on one dispatcher lambda (matches `open_close_finalize` in containers_builtin).

def private dockspace_finalize(widget_ident : string; kind : string;
                               var state : DockSpaceState) {
    //! Dockspace finalize. State path; the per-kind [widget_dispatch] handles "reset".
    var entry : WidgetEntry
    unsafe {
        container_finalize(widget_ident, kind, entry,
                           addr(state),
                           reinterpret<TypeInfo const?>(typeinfo rtti_typeinfo(type<DockSpaceState>)))
    }
}

def private dock_window_finalize(widget_ident : string; var state : DockWindowState) {
    //! dock_window finalize. State path; the per-kind [widget_dispatch] handles open/close/dock/undock/raise/set_window_pos.
    var entry : WidgetEntry
    unsafe {
        container_finalize(widget_ident, "dock_window", entry,
                           addr(state),
                           reinterpret<TypeInfo const?>(typeinfo rtti_typeinfo(type<DockWindowState>)))
    }
}

// Per-state-struct dispatchers for the docking family.

[widget_dispatch]
def _dockspace_dispatch(var state : DockSpaceState; action : string; payload : string) {
    if (action == "reset") {
        state.pending_reset = true
    }
}

[widget_dispatch]
def _dock_window_dispatch(var state : DockWindowState; action : string; payload : string) {
    if (action == "open") {
        state.pending_open = true
    } elif (action == "close") {
        state.pending_close = true
    } elif (action == "dock") {
        unsafe {
            sscan_json_at(payload, addr(state.pending_dock_to),
                          *reinterpret<TypeInfo const?>(typeinfo rtti_typeinfo(type<uint>)))
        }
    } elif (action == "undock") {
        state.pending_undock = true
    } elif (action == "raise") {
        state.pending_raise = true
    } elif (action == "set_window_pos") {
        // Payload: ``{x, y[, w, h]}`` — pos always set, size optional. The one
        // place we still touch JsonValue at dispatch time; alternative would
        // be a dedicated payload struct just for this action.
        var err : string
        let v = read_json(payload, err)
        if (v != null) {
            state.pending_pos = float2(v?["x"] ?? 0.0f, v?["y"] ?? 0.0f)
            state.pending_pos_set = true
            let has_w = v?["w"] != null
            let has_h = v?["h"] != null
            if (has_w || has_h) {
                state.pending_size = float2(v?["w"] ?? state.size.x,
                                            v?["h"] ?? state.size.y)
                state.pending_size_set = true
            }
        }
    }
}

// ===== dockspace =====

[container]
def dockspace(var state : DockSpaceState; flags : ImGuiDockNodeFlags; blk : block) {
    //! Full-viewport dockspace — `DockSpaceOverViewport(GetMainViewport(), flags, null)` paired with the user's block. The body runs unconditionally (the dockspace is always present); dock_window calls inside the body are the dockable panels. `state.dock_id` is captured from the return value so user code can drive `DockBuilder*` layout calls against it. `imgui_dock_reset` flips `has_initial_layout` back to false and removes the DockBuilder node so the user's setup function re-runs.
    if (state.pending_reset) {
        if (state.dock_id != 0u) {
            DockBuilderRemoveNode(state.dock_id)
        }
        state.has_initial_layout = false
        state.pending_reset = false
    }
    state.flags = flags
    let viewport = GetMainViewport()
    // leading dockspace_id arg: 0 = auto-generate.
    let id = DockSpaceOverViewport(0u, viewport, int(flags), null)
    state.dock_id = id
    invoke(blk)
    dockspace_finalize(widget_ident, "dockspace", state)
}

// ===== dockspace_in_window =====

[container]
def dockspace_in_window(var state : DockSpaceState; size : float2;
                        flags : ImGuiDockNodeFlags; blk : block) {
    //! Explicit-host dockspace — call inside a `window(HOST, (flags = MenuBar | NoDocking | ...))` body. Emits `DockSpace(GetID(widget_ident), size, flags, null)` so the dock node lives inside the host window rather than claiming the full viewport (compare `dockspace` which uses `DockSpaceOverViewport`). The host window owns its own MenuBar / NoDocking flags; this container only manages the dock node. `size=(0,0)` (typical) fills the host's available content region. `state.dock_id` is captured for `DockBuilder*` layout calls and `imgui_dock_reset` re-runs the user's setup.
    if (state.pending_reset) {
        if (state.dock_id != 0u) {
            DockBuilderRemoveNode(state.dock_id)
        }
        state.has_initial_layout = false
        state.pending_reset = false
    }
    state.flags = flags
    let id = GetID(widget_ident)
    DockSpace(id, size, int(flags), null)
    state.dock_id = id
    invoke(blk)
    dockspace_finalize(widget_ident, "dockspace_in_window", state)
}

// ===== dock_window =====

[container]
def dock_window(var state : DockWindowState; text : string; closable : bool;
                flags : ImGuiWindowFlags; blk : block) {
    //! Dockable Begin/End window — mirrors `window` from containers_builtin but participates in a dockspace. `closable=true` wires the X-button to `state.open`; `imgui_open`/`imgui_close` route through pending flags. `imgui_dock {"value":<dock_id>}` and `imgui_undock` route through SetNextWindowDockID at the top of the next frame.
    if (state.pending_open) {
        state.open = true
        state.pending_open = false
    }
    if (state.pending_close) {
        state.open = false
        state.pending_close = false
    }
    if (state.pending_dock_to != 0u) {
        SetNextWindowDockID(state.pending_dock_to, ImGuiCond.Always)
        state.pending_dock_to = 0u
    }
    if (state.pending_undock) {
        SetNextWindowDockID(0u, ImGuiCond.Always)
        state.pending_undock = false
    }
    if (state.pending_raise) {
        SetNextWindowFocus()       // raises the tab to the front when this window is docked in a stack
        state.pending_raise = false
    }
    if (state.pending_pos_set) {
        SetNextWindowPos(ImVec2(state.pending_pos.x, state.pending_pos.y), ImGuiCond.Always)
        state.pending_pos_set = false
    }
    if (state.pending_size_set) {
        SetNextWindowSize(ImVec2(state.pending_size.x, state.pending_size.y), ImGuiCond.Always)
        state.pending_size_set = false
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
            state.docked = IsWindowDocked()
            state.dock_id = GetWindowDockID()
            invoke(blk)
        }
        End()
    }
    dock_window_finalize(widget_ident, state)
}
