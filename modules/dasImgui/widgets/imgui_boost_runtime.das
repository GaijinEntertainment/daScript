options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module imgui_boost_runtime shared

require imgui public
require live/live_vars public
require live/live_commands
require live_host
require daslib/json public
require daslib/json_boost public
require daslib/coroutines public
require daslib/async_boost
require daslib/result public
require math
require strings

//! Runtime data layer for the dasImgui boost.
//!
//! Three things live here:
//!   1. State structs the user-facing `[widget] def` blocks consume.
//!   2. The per-frame registry (`g_registry : table<string; WidgetEntry>`)
//!      and its on-demand snapshot serializer (`JV(WidgetEntry)`).
//!   3. The unified L2/L3 dispatcher table (`g_dispatchers`) plus the
//!      `imgui_click` / `imgui_set` live_command handlers that drain it.
//!
//! Per-kind detail (click_count for buttons, value/bounds for sliders) lives
//! ONLY in the per-kind state struct; the registry stores a per-widget
//! serializer lambda that reflects the state struct on demand at snapshot
//! time. Per-frame cost is one struct copy + one lambda capture; JSON only
//! materializes when `imgui_snapshot` actually fires.

// ===== Per-kind state structs =====
//
// Mixed @live (preserved across hot-reload) and @optional (transient,
// dropped from JV when zero-valued). Common per-frame imgui state — hex_id,
// bbox, hover/active/focus — lives in WidgetEntry, not here, so the structs
// stay small per kind.

struct ClickState {
    @live click_count : int                      //! cumulative across reloads
    @optional pending_click : bool               // queued by imgui_click L2; consumed next frame
    @optional clicked : bool                     // true on the frame the button fired
}

struct SliderStateFloat {
    @live value : float                          //! preserved across reload
    @live bounds : tuple<float; float>           //! (min, max)
    @optional has_pending : bool                 // imgui_set L3 sets; render fn consumes
    @optional pending_value : float
    @optional changed : bool
}

struct SliderStateFloat2 {
    @live value : float2
    @live bounds : tuple<float; float>
    @optional has_pending : bool
    @optional pending_value : float2
    @optional changed : bool
}

struct SliderStateFloat3 {
    @live value : float3
    @live bounds : tuple<float; float>
    @optional has_pending : bool
    @optional pending_value : float3
    @optional changed : bool
}

struct SliderStateFloat4 {
    @live value : float4
    @live bounds : tuple<float; float>
    @optional has_pending : bool
    @optional pending_value : float4
    @optional changed : bool
}

struct SliderStateInt {
    @live value : int
    @live bounds : tuple<int; int>
    @optional has_pending : bool
    @optional pending_value : int
    @optional changed : bool
}

struct SliderStateInt2 {
    @live value : int2
    @live bounds : tuple<int; int>
    @optional has_pending : bool
    @optional pending_value : int2
    @optional changed : bool
}

struct SliderStateInt3 {
    @live value : int3
    @live bounds : tuple<int; int>
    @optional has_pending : bool
    @optional pending_value : int3
    @optional changed : bool
}

struct SliderStateInt4 {
    @live value : int4
    @live bounds : tuple<int; int>
    @optional has_pending : bool
    @optional pending_value : int4
    @optional changed : bool
}

struct ToggleState {
    //! Shared across Checkbox / RadioButton (bool form) / Selectable / MenuItem.
    //! `pending_toggle` + `pending_value` let imgui_set steer the next frame's
    //! value deterministically; the renderer consumes-then-clears.
    @live value : bool
    @optional pending_toggle : bool                  // imgui_set L3 sets; consumed next frame
    @optional pending_value : bool
    @optional changed : bool                         // true on the frame the widget fired
}

struct RadioIntState {
    //! Backs RadioButton(int form). `value` is the currently-selected button
    //! id; the widget def takes the per-call `v_button` and writes value=v_button
    //! when the user clicks. `imgui_set {"value":N}` steers the selection.
    @live value : int
    @optional pending_set : bool
    @optional pending_value : int
    @optional changed : bool
}

// Drag* family. One state struct per scalar-or-vector value type the imgui
// API exposes; `bounds` is the only persistent config (zero-init = unclamped
// per ImGui's v_min==v_max semantics). `speed`, `format`, `flags` are passed
// at the call site — globals can't apply struct field defaults (they're
// zero-init), and string defaults trip the uninit-safety check. `pending_value`
// matches `value`'s type so the generic `drag_set_finalize` helper in
// imgui_widgets_builtin can capture state and steer it from imgui_set.

struct DragStateFloat {
    @live value : float
    @live bounds : tuple<float; float>           //! (min, max); zero-init = unclamped
    @optional has_pending : bool
    @optional pending_value : float
    @optional changed : bool
}

struct DragStateFloat2 {
    @live value : float2
    @live bounds : tuple<float; float>
    @optional has_pending : bool
    @optional pending_value : float2
    @optional changed : bool
}

struct DragStateFloat3 {
    @live value : float3
    @live bounds : tuple<float; float>
    @optional has_pending : bool
    @optional pending_value : float3
    @optional changed : bool
}

struct DragStateFloat4 {
    @live value : float4
    @live bounds : tuple<float; float>
    @optional has_pending : bool
    @optional pending_value : float4
    @optional changed : bool
}

struct DragStateInt {
    @live value : int
    @live bounds : tuple<int; int>
    @optional has_pending : bool
    @optional pending_value : int
    @optional changed : bool
}

struct DragStateInt2 {
    @live value : int2
    @live bounds : tuple<int; int>
    @optional has_pending : bool
    @optional pending_value : int2
    @optional changed : bool
}

struct DragStateInt3 {
    @live value : int3
    @live bounds : tuple<int; int>
    @optional has_pending : bool
    @optional pending_value : int3
    @optional changed : bool
}

struct DragStateInt4 {
    @live value : int4
    @live bounds : tuple<int; int>
    @optional has_pending : bool
    @optional pending_value : int4
    @optional changed : bool
}

struct DragStateRangeFloat {
    //! `value.x` = current_min, `value.y` = current_max. Bounds clamp both.
    //! `imgui_set {"value":[lo,hi]}` writes the whole range atomically.
    @live value : float2
    @live bounds : tuple<float; float>
    @optional has_pending : bool
    @optional pending_value : float2
    @optional changed : bool
}

struct DragStateRangeInt {
    //! `value.x` = current_min, `value.y` = current_max.
    @live value : int2
    @live bounds : tuple<int; int>
    @optional has_pending : bool
    @optional pending_value : int2
    @optional changed : bool
}

// Input* numeric family. No bounds — InputFloat/Int/Double don't clamp;
// step / step_fast / format are per-call args (uniform with Drag/Slider).
// Only `value` persists. Same has_pending/pending_value/changed shape so
// `pending_value_finalize` covers the dispatch.

struct InputStateFloat {
    @live value : float
    @optional has_pending : bool
    @optional pending_value : float
    @optional changed : bool
}

struct InputStateFloat2 {
    @live value : float2
    @optional has_pending : bool
    @optional pending_value : float2
    @optional changed : bool
}

struct InputStateFloat3 {
    @live value : float3
    @optional has_pending : bool
    @optional pending_value : float3
    @optional changed : bool
}

struct InputStateFloat4 {
    @live value : float4
    @optional has_pending : bool
    @optional pending_value : float4
    @optional changed : bool
}

struct InputStateInt {
    @live value : int
    @optional has_pending : bool
    @optional pending_value : int
    @optional changed : bool
}

struct InputStateInt2 {
    @live value : int2
    @optional has_pending : bool
    @optional pending_value : int2
    @optional changed : bool
}

struct InputStateInt3 {
    @live value : int3
    @optional has_pending : bool
    @optional pending_value : int3
    @optional changed : bool
}

struct InputStateInt4 {
    @live value : int4
    @optional has_pending : bool
    @optional pending_value : int4
    @optional changed : bool
}

struct InputStateDouble {
    @live value : double
    @optional has_pending : bool
    @optional pending_value : double
    @optional changed : bool
}

// Color* family. ColorEdit3/ColorPicker3 share ColorState3 (RGB);
// ColorEdit4/ColorPicker4 share ColorState4 (RGBA). ColorButton is
// click-only (no value stored — caller passes the color per-frame),
// so it reuses ClickState (mirrors `button` / `small_button`). `flags`
// is per-call; ImGui's color flags (DisplayHSV, Float, NoInputs, …)
// are call-site config, not persistent state.

struct ColorState3 {
    @live value : float3                         //! RGB in [0..1]
    @optional has_pending : bool
    @optional pending_value : float3
    @optional changed : bool
}

struct ColorState4 {
    @live value : float4                         //! RGBA in [0..1]
    @optional has_pending : bool
    @optional pending_value : float4
    @optional changed : bool
}

// Choice family. `value` is the selected item index; `imgui_set {"value":N}`
// steers the next-frame selection. The user passes `items : array<string>`
// per-frame; combo/list_box marshal it C-side without a shim:
//   - combo packs a zero-separated `array<uint8>` (kept on state for capacity
//     reuse) and reinterprets `addr(buf[0])` as `string#` for ImGui's
//     null-separated overload.
//   - list_box passes `addr(items[0])` directly — daslang `array<string>`
//     storage is `char **`, which matches ImGui's `const char* const items[]`.
// BeginCombo/EndCombo + BeginListBox/EndListBox (block-arg manual loop) roll
// into 0b.3 with the rest of the [container] family.

struct ComboState {
    @live value : int = -1                       //! selected index, -1 = none
    @optional has_pending : bool
    @optional pending_value : int
    @optional changed : bool
    @optional items_buf : array<uint8>           // transient zero-separated buffer; rebuilt each frame
}

struct ListBoxState {
    @live value : int = -1                       //! selected index, -1 = none
    @optional has_pending : bool
    @optional pending_value : int
    @optional changed : bool
}

// Text-input family. The buffer is the canonical storage; `value` is the
// extracted (clone-on-change) string presented to user code.
// `input_text_growable` reuses this struct; its CallbackResize lambda
// updates `capacity` + `buffer` in place.

struct InputTextState {
    @live value : string = ""                             //! current text content (clone of buffer)
    @live capacity : int = 256                            //! working buffer size in bytes (incl. trailing null)
    @optional has_pending : bool
    @optional pending_value : string = ""
    @optional changed : bool
    @optional buffer : array<uint8>                       // working buffer; mirrors `value`, allocated to capacity on first frame / post-reload
}

struct TextShowState {
    //! Backs the `text_show` display widget (master plan §4.3a). Read-only
    //! from the user's perspective — `value` is set either by an
    //! initializer at the call site (`text_show(STATUS, value="saved")`)
    //! or programmatically (`STATUS.value = compute_status()`). The
    //! standard serializer surfaces `value` for telemetry / playwright
    //! `expect_value(app, STATUS, "saved")`. Tests can also drive the
    //! value externally via `imgui_set` — the dispatcher writes
    //! `state.value` directly.
    @live value : string = ""
}

struct PlotState {
    //! Backs the `plot_lines` / `plot_histogram` display widgets. The
    //! plotted values are passed per-frame as a parameter (not stored in
    //! state) — same shape as the imgui_test_engine and daslib v1
    //! conventions. State carries last-frame's call args for telemetry:
    //! the visible label, the (scale_min, scale_max) range, and the
    //! sample count. Read-only — no dispatcher.
    @optional title : string                           // overlay text
    @optional bounds : tuple<float; float>             // (scale_min, scale_max)
    @optional samples : int                            // length(values) last frame
}

// ===== Container family (Phase 0b.3) =====
//
// `[container]` widgets wrap a daslang block argument and push themselves onto
// `g_container_path` so leaf children get hierarchical registry keys. Per-kind
// state captures only what's worth remembering across frames + reloads — open
// state for togglable containers, layout dims for windows / children, flags
// for everything that takes them. Stateless containers (group, menu_bar,
// tooltip, item_tooltip) get an empty struct so the [widget] auto-emit gate
// still has something to instantiate.

struct WindowState {
    //! Backs the `window` container. `open` defaults to true so a closable
    //! window renders from frame 1; the X-button or `imgui_close` flips it
    //! false. `@live` carries that across reload. Everything else is per-
    //! frame observed (pos/size/collapsed/scroll) or echoed from per-call
    //! args (flags). ImGui persists window pos itself via imgui.ini, so
    //! duplicating it as `@live` would only fight that. `imgui_open` /
    //! `imgui_close` flow through pending_open/close; the renderer applies
    //! them at the top of the next frame. Per-call config (closable, flags)
    //! is supplied via the named-tuple at the call site — same convention
    //! as `[widget]`.
    @live open : bool = true
    @optional flags : ImGuiWindowFlags
    @optional pos : float2
    @optional size : float2
    @optional collapsed : bool
    @optional scroll : float2                 // (GetScrollX, GetScrollY) of the window
    @optional scroll_max : float2             // (GetScrollMaxX, GetScrollMaxY)
    @optional pending_open : bool
    @optional pending_close : bool
}

struct ChildState {
    //! Backs the `child` container. All fields are observed (scroll); per-
    //! call config (size, child_flags, window_flags) is supplied via the
    //! named-tuple at the call site.
    @optional size : float2
    @optional child_flags : ImGuiChildFlags
    @optional window_flags : ImGuiWindowFlags
    @optional scroll : float2                 // (GetScrollX, GetScrollY)
    @optional scroll_max : float2             // (GetScrollMaxX, GetScrollMaxY)
}

struct GroupState {
    //! Backs the `group` container. Pure layout grouping — no persistent or
    //! observable state, but the [container] macro still needs a struct to
    //! hang the auto-emitted global on. The `_placeholder` field stays
    //! @optional so JV(state) emits `{}` when the group is rendered.
    @optional _placeholder : bool
}

struct MenuBarState {
    //! Backs the `menu_bar` container. Stateless wrapper.
    @optional _placeholder : bool
}

struct MenuState {
    //! Backs the `menu` container — a single drop-down inside a menu bar.
    //! `enabled` is per-call; `opened` is per-frame observed.
    @optional enabled : bool
    @optional opened : bool
}

struct PopupState {
    //! Shared by `popup` and `popup_modal`. `open` is @live so the user-driven
    //! state survives reload. `imgui_open` / `imgui_close` route through
    //! pending_open/close; the renderer issues OpenPopup/CloseCurrentPopup
    //! on the next frame in response.
    @live open : bool
    @optional flags : ImGuiWindowFlags
    @optional pending_open : bool
    @optional pending_close : bool
}

struct TabBarState {
    //! Backs the `tab_bar` container. ImGui drives active-tab selection
    //! itself; we just observe.
    @optional flags : ImGuiTabBarFlags
}

struct TabItemState {
    //! Backs the `tab_item` container. `open` defaults to true so a closable
    //! tab renders from frame 1; the X-button or `imgui_close` flips it false.
    //! `@live` carries that across reload.
    @live open : bool = true
    @optional flags : ImGuiTabItemFlags
    @optional pending_open : bool
    @optional pending_close : bool
}

struct TooltipState {
    //! Backs both `tooltip` and `item_tooltip`. Tooltips don't persist —
    //! they're a per-frame chrome pop.
    @optional _placeholder : bool
}

struct TreeNodeState {
    //! Backs the `tree_node` container. ImGui owns the open/close state
    //! internally; we mirror what's observable so snapshots reflect truth,
    //! and the dispatcher routes open/close through SetNextItemOpen.
    @optional flags : ImGuiTreeNodeFlags
    @optional opened : bool                       // observed per-frame: TreeNode returned true
    @optional pending_open : bool
    @optional pending_close : bool
}

struct CollapsingHeaderState {
    //! Backs the `collapsing_header` container. Same shape as TreeNodeState
    //! since the imgui call structurally mirrors it (returns bool, no End()).
    @optional flags : ImGuiTreeNodeFlags
    @optional opened : bool
    @optional pending_open : bool
    @optional pending_close : bool
}

// ===== Per-frame registry =====
//
// One entry per render of any boost-wrapped widget, keyed by the widget's
// daslang identifier ("SAVE_BTN" / "SPEED" / "VOLUME" / ...). Cleared at
// the start of each frame.

struct WidgetEntry {
    //! Per-frame snapshot of one rendered widget. Common fields populated
    //! every frame; per-kind detail is materialized on demand by the
    //! serializer lambda stored in `g_serializers[ident]` (parallel table)
    //! and pasted into `payload` at imgui_snapshot time. The lambda lives
    //! outside the entry because `g_registry` is cleared every frame and
    //! we need the serializer (which captures `& state` of the per-widget
    //! global) to survive that clear.
    kind : string
    hex_id : uint
    bbox : float4                                // packed [x0, y0, x1, y1]
    @optional hover : bool
    @optional active : bool
    @optional focus : bool
    @optional payload : JsonValue?               // populated by widget_entry_jv from g_serializers
}

def private widget_entry_jv(ident : string; var entry : WidgetEntry) : JsonValue? {
    //! One JsonValue blob per widget. The per-kind `payload` comes from
    //! `g_serializers[ident]` (the lambda the widget def registered) and
    //! lands on `entry.payload` before serialization; everything else is
    //! auto-walked off WidgetEntry by JV(struct), with @optional fields
    //! skipped when zero-valued.
    if (key_exists(g_serializers, ident)) {
        entry.payload = invoke(g_serializers[ident])
    }
    return JV(entry)
}

var g_registry : table<string; WidgetEntry>
var g_frame : int = 0

// ===== Long-lived widget registry (Phase 1) =====
//
// `g_widgets` is the long-lived twin of `g_registry`. The [widget] macro
// emits a `register_widget(...)` call into a per-module shared `[init]`
// function (see daslib/decs_boost.das:583-589 for the setup_call_list
// pattern), so every widget appears here at module load — even before its
// containing window/tab is opened.
//
// `imgui_snapshot` walks `g_widgets` for the full widget enumeration and
// joins each entry with `g_registry[k]` for this frame's bbox / hover /
// active / focus when present. Closed-window widgets surface with
// `rendered: false` and the state struct's serializer payload.
//
// Keying: registration uses the widget's bare ident (the macro can't
// know the container chain at expansion time when the user's container
// is hidden behind helpers). On first render `widget_finalize` migrates
// the entry from `bareName` to `path_key` (container-aware) so siblings
// across containers stay distinct.

struct WidgetMeta {
    module_name : string
    kind : string
    state_addr : void?
    serializer : lambda<() : JsonValue?>
    last_seen_frame : int
}

var g_widgets : table<string; WidgetMeta>

// ===== Phase 2 reverse maps + coroutine runner =====
//
// `g_id_to_path` is a long-lived hex_id → path_key map populated by every
// widget_finalize / container_finalize. Lets `io_jv()` resolve `GetActiveID()`
// (an opaque ImGuiID) back to the user's identifier path. Surviving across
// frames mirrors `g_widgets`; cleared on hot-reload via clear_module_widgets.
//
// `g_id_to_bbox` is a per-frame hex_id → bbox map for L1 click/drag fallback
// when the target string is "0x..." but not in g_id_to_path. Cleared at the
// top of begin_frame() alongside g_registry.
//
// `g_coroutines` holds in-flight L1 input coroutines (imgui_drag, imgui_type_text).
// Each [async] command spawns one Coroutine (iterator<bool>); the runtime
// advances the array one step per frame in begin_frame() via advance_coroutines.

var private g_id_to_path : table<uint; string>
var private g_id_to_bbox : table<uint; float4>
var private g_coroutines : array<Coroutine>
var private g_focusable : table<string>     //! set: paths whose widgets accept keyboard focus
var private g_pending_focus : table<string> //! set: paths to focus on their next widget_prelude

def public id_to_path(hid : uint) : string {
    //! Safe accessor — returns the path_key for `hid` or "" when unmapped.
    //! Don't expose g_id_to_path directly: keeping mutation server-internal
    //! lets the table grow private accessors (eraseed_paths, hex_id by path)
    //! without breaking external callers.
    return key_exists(g_id_to_path, hid) ? g_id_to_path[hid] : ""
}

def public id_to_bbox(hid : uint) : float4 {
    //! Safe accessor — returns the per-frame bbox for `hid` or `float4(0)`.
    //! Per-frame: only valid for ids that rendered in the previous frame
    //! (cleared at frame top before re-population).
    return key_exists(g_id_to_bbox, hid) ? g_id_to_bbox[hid] : float4(0.0f, 0.0f, 0.0f, 0.0f)
}

// Deferred (Phase 4 / on-demand): a separate `extras` channel for
// transient per-frame data that doesn't belong in the persistent state
// struct (text-input cursor / selection, tooltip content). Phase 1.2
// initially planned a `lambda<() : JsonValue?> extras` field on
// WidgetMeta — dropped after a clean-room read of imgui_test_engine
// confirmed cursor/selection isn't worth public-API gymnastics (test
// engine reads InputText via clipboard, never exposes cursor/selection).
// All current "transient observed state" fits inline in the kind's state
// struct (ChildState.scroll / WindowState.collapsed / TreeNodeState.opened).
// Reintroduce the extras channel only when a real consumer (e.g. Phase 4
// visual aids) needs data that doesn't fit a state-struct field.

// ===== Container path stack =====
//
// `g_container_path` is the chain of currently-open `[container]` idents,
// outermost-first. Pushed on container body entry, popped on body exit. Leaf
// widgets read the chain at finalize time to build hierarchical registry +
// dispatcher keys ("MAIN_WIN/SETTINGS_TAB/RPS"). Containers themselves push
// before invoking their block, so by the time their body calls
// container_finalize, self is already at the chain's tail.
//
// `g_container_path_str` is the cached "/"-joined form, kept in sync by
// push/pop so leaf finalize is one string interpolation rather than an
// O(depth) join per leaf per frame.

var g_container_path : array<string>
var private g_container_path_str : string

def public container_path_push(widget_ident : string) {
    //! Append `widget_ident` to the open-container chain. Called by the
    //! `[container]` macro at the top of every wrapped body. The matching
    //! `container_path_pop` lives in body.finalList so it runs even on early
    //! return (panic still skips it — same risk surface as widget_finalize).
    g_container_path |> push(widget_ident)
    if (length(g_container_path) == 1) {
        g_container_path_str = widget_ident
    } else {
        g_container_path_str = "{g_container_path_str}/{widget_ident}"
    }
}

def public container_path_pop() {
    //! Pop the tail container ident. Idempotent on empty (defensive — a panic
    //! mid-frame followed by a recovery would otherwise leak).
    return if (empty(g_container_path))
    g_container_path |> pop()
    if (empty(g_container_path)) {
        g_container_path_str = ""
    } else {
        // Rebuild via build_string — pop is O(depth) anyway; avoid the O(depth^2)
        // allocation churn of a chain of `s = "{s}/.."` interpolations.
        g_container_path_str = build_string() $(var w) {
            for (i in range(length(g_container_path))) {
                if (i > 0) {
                    w |> write("/")
                }
                w |> write(g_container_path[i])
            }
        }
    }
}

def public widget_path_key(widget_ident : string) : string {
    //! Build the registry/dispatcher key for a leaf widget: the open container
    //! chain joined by "/" then the leaf's own ident. Empty container path
    //! degrades to bare ident — preserves pre-0b.3 behavior for unnested widgets.
    return widget_ident if (empty(g_container_path_str))
    return "{g_container_path_str}/{widget_ident}"
}

def public container_path_key() : string {
    //! Build the registry/dispatcher key for a container's own entry. The
    //! container has already pushed itself onto `g_container_path`, so the
    //! cached path string is the answer directly.
    return g_container_path_str
}

def begin_frame() {
    //! Call once per frame BEFORE rendering any widgets. Bumps the frame
    //! counter, clears the per-frame registry + per-frame hex_id→bbox map,
    //! and defensively clears the container path — a panic mid-frame the
    //! previous tick would otherwise leave a stale chain.
    //!
    //! L1 input coroutines are NOT advanced here. ImGui_ImplGlfw_NewFrame()
    //! pushes the real cursor into the IO event queue every frame, so a
    //! coroutine that runs before GLFW gets immediately overridden. Call
    //! `advance_coroutines()` between `ImGui_ImplGlfw_NewFrame()` and
    //! `NewFrame()` instead — see io_synth_drag.das for the canonical
    //! ordering. Examples with no L1 input can skip the call entirely.
    g_frame++
    g_registry |> clear()
    g_id_to_bbox |> clear()
    g_rendered_this_frame |> clear()
    if (!empty(g_container_path)) {
        g_container_path |> clear()
        g_container_path_str = ""
    }
}

var private g_end_of_frame_hooks : table<string; function<() : void>>

def public register_end_of_frame_hook(name : string; hook : function<() : void>) {
    //! Register a per-frame painter invoked by `end_of_frame()` after widget
    //! rendering. `name` is the slot key — re-registering the same name
    //! REPLACES the prior entry, so [init] re-running on daslang-live reload
    //! doesn't multiply painters. Visual-aids / future overlay modules call
    //! this from `[init]` so their painters fire once per frame without each
    //! example wiring them up explicitly. Function-pointer (not lambda) —
    //! hooks read module globals and don't need capture; keeps registration
    //! copy-by-value.
    g_end_of_frame_hooks[name] = hook
}

def end_of_frame() {
    //! Call once per frame AFTER rendering all widgets (and after
    //! `EndFrame()` is safe — we draw onto `GetForegroundDrawList()` which
    //! is finalized at `Render()`). Drives the per-frame painters registered
    //! by `register_end_of_frame_hook` — highlight overlays, mouse trail,
    //! narrate callouts, etc. No-op when no hooks registered.
    for (h in values(g_end_of_frame_hooks)) {
        invoke(h)
    }
}

var private g_post_command_hooks : table<string; function<(command : string; target : string) : void>>

def public register_post_command_hook(name : string; hook : function<(command : string; target : string) : void>) {
    //! Symmetric to register_end_of_frame_hook. Each hook fires after a
    //! successful imgui_* command (click / set / focus / open / close /
    //! drag / type_text) with `(command_name, resolved_target)`. Used by
    //! imgui_visual_aids to auto-flash the touched widget. Same name-keyed
    //! idempotent semantics — re-registering replaces, so daslang-live
    //! reload doesn't accumulate hooks.
    g_post_command_hooks[name] = hook
}

def private dispatch_post_command_hooks(command : string; target : string) {
    for (h in values(g_post_command_hooks)) {
        invoke(h, command, target)
    }
}

def public advance_coroutines() {
    //! Step every active L1 coroutine one frame, using the `next/empty`
    //! pattern from daslib's `cr_run_all`. `next(c, t)` advances the
    //! generator to its next yield (or the trailing `return false`); after
    //! the generator falls off the end, `empty(c)` flips true and we drop
    //! the slot. Iterates from the tail so `erase` doesn't shift unseen
    //! indices.
    //!
    //! Must be called between `ImGui_ImplGlfw_NewFrame()` and `NewFrame()`
    //! — see `begin_frame()` doc-comment for the ordering rationale.
    var i = length(g_coroutines)
    while (i > 0) {
        i--
        var t : bool
        next(g_coroutines[i], t)
        if (empty(g_coroutines[i])) {
            delete g_coroutines[i]
            g_coroutines |> erase(i)
        }
    }
}

def public spawn_coroutine(var c : Coroutine) {
    //! Public entry for L1 commands to schedule a coroutine that yields one
    //! event per frame. Move-in via `<-`; iterator is non-copyable. The
    //! runtime advances on the next begin_frame().
    g_coroutines |> emplace(c)
}

// ===== Render helpers =====
//
// `widget_prelude` runs at the top of every macro-wrapped widget body
// (PushID for unique imgui scoping). `widget_finalize` runs at the bottom
// — captures the common per-frame fields, registers the lazy serializer
// + dispatcher on the first render of each ident, and balances the PushID.

def public widget_prelude(widget_ident : string) {
    PushID(widget_ident)
    if (!empty(g_pending_focus)) {
        let path_key = widget_path_key(widget_ident)
        if (key_exists(g_pending_focus, path_key)) {
            SetKeyboardFocusHere(0)
            g_pending_focus |> erase(path_key)
        }
    }
}

def public register_focusable(widget_ident : string) {
    //! Tag the widget's path as accepting keyboard focus. Called from each
    //! interactive finalize helper (click/toggle/radio_int/pending_value/
    //! text_value) after widget_finalize. Display-only finalize helpers
    //! (text_show, plot) skip this — `imgui_focus` returns err for those.
    g_focusable |> insert(widget_path_key(widget_ident))
}

def public pending_value_container_finalize(widget_ident : string; kind : string;
                                             var state : auto) {
    //! Container counterpart of `pending_value_finalize` for layout widgets
    //! (split_h / split_v / dock_left). Same generic state shape (has_pending
    //! / pending_value), same `imgui_set` action, but routed through
    //! `container_finalize` so the container's own path doesn't get the
    //! widget_ident appended twice (the [container] macro already pushed it
    //! onto g_container_path before the body ran). No `register_focusable` —
    //! layout containers aren't keyboard-focus targets.
    var entry : WidgetEntry
    unsafe {
        let ser <- @ capture(& state) () : JsonValue ? {
            return JV(state)
        }
        let disp <- @ capture(& state) (action : string; payload : JsonValue ?) {
            if (action == "set") {
                state.pending_value = from_JV(payload?["value"],
                                              type<typedecl(state.pending_value)>,
                                              default<typedecl(state.pending_value)>)
                state.has_pending = true
            }
        }
        container_finalize(widget_ident, kind, entry, ser, disp)
    }
}

def public pending_value_finalize(widget_ident : string; kind : string; var state : auto) {
    //! Lambda + dispatcher install for the pending-value family — every
    //! state shape with `has_pending` / `pending_value` / `changed`
    //! fields, dispatched on action "set". Generic on the state struct;
    //! `typedecl(state.pending_value)` drives the from_JV decode so one
    //! body covers all of Drag* (10 kinds) / Slider* (incl. VSlider, 10
    //! kinds) / Input numeric / Color / Combo / ListBox / layout splitters
    //! (split_h / split_v / dock_left) without per-type duplication.
    //! InputText goes through `text_value_finalize` (in imgui_widgets_builtin)
    //! instead — it needs `:=` to clone the JV-owned string bytes into the
    //! widget context heap before the JV drops on dispatcher return.
    var entry : WidgetEntry
    unsafe {
        let ser <- @ capture(& state) () : JsonValue ? {
            return JV(state)
        }
        let disp <- @ capture(& state) (action : string; payload : JsonValue ?) {
            if (action == "set") {
                state.pending_value = from_JV(payload?["value"],
                                              type<typedecl(state.pending_value)>,
                                              default<typedecl(state.pending_value)>)
                state.has_pending = true
            }
        }
        widget_finalize(widget_ident, kind, entry, ser, disp)
    }
    register_focusable(widget_ident)
}

def public clear_module_widgets(module_name : string) {
    //! Erase every g_widgets entry tagged with this module — and the
    //! matching g_id_to_path entries that point to those paths. Called as
    //! the first line of each user module's `register`imgui`widgets()`
    //! [init] function (prepended by the [widget] macro on first widget
    //! per module). On hot-reload, the [init] re-fires → clear-then-refill
    //! drops widgets removed from source AND replaces lambdas with fresh
    //! closures bound to post-reload state addresses. Entries from other
    //! modules are untouched.
    //!
    //! Phase 2.1: also sweep g_id_to_path entries whose value matches a
    //! cleared path. The reverse map is rebuilt by widget_finalize on the
    //! next render — but the sweep prevents stale hex_id → dead-path
    //! resolutions during the gap.
    var paths_to_remove : array<string>
    for (k, m in keys(g_widgets), values(g_widgets)) {
        if (m.module_name == module_name) {
            paths_to_remove |> push(k)
        }
    }
    for (k in paths_to_remove) {
        g_widgets |> erase(k)
    }
    var ids_to_remove : array<uint>
    for (id, pk in keys(g_id_to_path), values(g_id_to_path)) {
        for (dead in paths_to_remove) {
            if (pk == dead) {
                ids_to_remove |> push(id)
                break
            }
        }
    }
    for (id in ids_to_remove) {
        g_id_to_path |> erase(id)
    }
    for (dead in paths_to_remove) {
        g_focusable |> erase(dead)
        g_pending_focus |> erase(dead)
    }
}

def public widget_registered(path : string) : bool {
    //! Idempotency probe for indexed-form wrappers — the per-(kind, IDENT)
    //! helper generated by the [widget]/[container] macro calls this each
    //! frame and only re-fires `register_widget` for keys missing from
    //! `g_widgets`. Returns false post-`clear_module_widgets` so reloads
    //! re-bind the serializer's `&state` capture to the post-reload
    //! address even when the @live table preserved the value.
    return key_exists(g_widgets, path)
}

def public register_widget(module_name : string;
                           bare_ident : string;
                           kind : string;
                           state_addr : void?;
                           var serializer : lambda<() : JsonValue?>) {
    //! Module-init registration into the long-lived `g_widgets` table.
    //! Called from the per-module `[init]` function the [widget] macro
    //! builds via setup_call_list. Keyed on bare ident; on first render
    //! `widget_finalize` migrates the entry to its container-aware
    //! path_key. The entry is tagged with `module_name` so reloads can
    //! clear-then-refill via `clear_module_widgets` (see above).
    unsafe {
        g_widgets[bare_ident] <- WidgetMeta(
            module_name = module_name,
            kind = kind,
            state_addr = state_addr,
            serializer <- serializer,
            last_seen_frame = -1
        )
    }
}

var private g_serializers : table<string; lambda<() : JsonValue?>>
var private g_dispatcher_seen : table<string; bool>

// Per-frame "this path rendered already" set. Each path_key may be
// finalized exactly once between begin_frame()/end_of_frame() — collision
// means a single-global widget got rendered twice in the same frame
// (lexical duplicate, or invisibly aliased through two distinct call
// sites). Cleared at begin_frame(); checked at widget_finalize /
// container_finalize entry. Per API_REWORK §4.4: "loud, not a warning —
// telemetry needs an unambiguous path → widget mapping or test commands
// can't locate the right widget."
var private g_rendered_this_frame : table<string>

def private check_unique_render(path_key : string; kind : string) {
    //! Single-global widget renders exactly once per frame. Second visit of
    //! the same path key in the same frame trips an immediate panic — the
    //! caller's stack trace points at the duplicate render site. For
    //! lexical loops, the fix is the IDENT[i] / IDENT[key] form (auto-
    //! distinguishes by subscript-in-path). For two-window aliasing of the
    //! same widget, declare a second IDENT or use the table form keyed by
    //! container.
    //!
    //! Exempt kinds: widget families where the SAME state is intentionally
    //! shared across N calls in one frame — `radio_button_int` is the
    //! canonical case (three calls with different `v_button` arguments
    //! drive one shared `value`). These are detected by kind name; the
    //! list grows as new shared-state widget kinds land.
    return if (kind == "radio_button_int")
    if (key_exists(g_rendered_this_frame, path_key)) {
        let msg = "dasImgui: widget '{path_key}' ({kind}) rendered twice in frame {g_frame}. Single-global widgets must render exactly once per frame. For multiple instances, declare a table  variable private <NAME> : table<int; <STATE_TYPE>>  and call with subscript:  {kind}(<NAME>[i], (...))"
        panic(msg)
    }
    g_rendered_this_frame |> insert(path_key)
}

def public widget_finalize(widget_ident : string;
                           kind : string;
                           var entry : WidgetEntry;
                           var serializer : lambda<() : JsonValue?>;
                           var dispatcher : lambda<(action : string; payload : JsonValue?) : void>) {
    //! Capture common per-frame fields, register the L2/L3 dispatcher closure
    //! on first sight of the path key, and overwrite the serializer every
    //! frame. The serializer captures `& state` of the per-widget global —
    //! addresses are stable across frames so the lambda is rebuilt cheaply
    //! and the latest one is always correct. Hot-reload clears
    //! `g_dispatcher_seen` (NOT @live), so the first post-reload frame
    //! re-registers a fresh dispatcher bound to the post-reload state.
    //! Path key includes any open `[container]` ancestors (0b.3) — sibling
    //! idents under different containers map to distinct addresses.
    let path_key = widget_path_key(widget_ident)
    check_unique_render(path_key, kind)
    entry.kind = kind
    entry.hex_id = GetID(widget_ident)
    let rmin = GetItemRectMin()
    let rmax = GetItemRectMax()
    entry.bbox = float4(rmin.x, rmin.y, rmax.x, rmax.y)
    entry.hover = IsItemHovered(ImGuiHoveredFlags.None)
    entry.active = IsItemActive()
    entry.focus = IsItemFocused()
    g_id_to_path[entry.hex_id] = path_key
    g_id_to_bbox[entry.hex_id] = entry.bbox
    unsafe {
        g_serializers[path_key] <- serializer
    }
    if (!key_exists(g_dispatcher_seen, path_key)) {
        unsafe {
            g_dispatchers[path_key] <- dispatcher
        }
        g_dispatcher_seen[path_key] = true
    }
    g_registry[path_key] <- entry
    widgets_mark_seen(widget_ident, path_key)
    PopID()
}

def private widgets_mark_seen(bare_ident : string; path_key : string) {
    //! Phase 1: the [widget] macro registered this widget under its bare
    //! ident at module init. On first render we know the container-aware
    //! path_key and migrate the entry. Subsequent renders just bump
    //! last_seen_frame. If neither key is present (user wrote a [widget]
    //! body without going through the macro — e.g. tests calling
    //! widget_finalize directly), do nothing.
    if (bare_ident != path_key && key_exists(g_widgets, bare_ident)) {
        var moved <- g_widgets[bare_ident]
        g_widgets |> erase(bare_ident)
        unsafe {
            g_widgets[path_key] <- moved
        }
    }
    if (key_exists(g_widgets, path_key)) {
        g_widgets[path_key].last_seen_frame = g_frame
    }
}

def public container_finalize(widget_ident : string;
                              kind : string;
                              var entry : WidgetEntry;
                              var serializer : lambda<() : JsonValue?>;
                              var dispatcher : lambda<(action : string; payload : JsonValue?) : void>) {
    //! Container counterpart of widget_finalize. Self is already at the tail
    //! of g_container_path (the [container] macro's prelude pushed it before
    //! the user body ran), so the path key is the cached chain string itself.
    //! Containers don't capture bbox/hover/active/focus — those imgui
    //! introspection calls aren't meaningful at the End-pair scope (the
    //! "current item" is whatever ran last inside the body, not the
    //! container). Per-kind state (window pos/size, popup open, …) lives in
    //! the state struct and is reflected through `serializer`.
    let path_key = container_path_key()
    check_unique_render(path_key, kind)
    entry.kind = kind
    entry.hex_id = GetID(widget_ident)
    g_id_to_path[entry.hex_id] = path_key
    unsafe {
        g_serializers[path_key] <- serializer
    }
    if (!key_exists(g_dispatcher_seen, path_key)) {
        unsafe {
            g_dispatchers[path_key] <- dispatcher
        }
        g_dispatcher_seen[path_key] = true
    }
    g_registry[path_key] <- entry
    widgets_mark_seen(widget_ident, path_key)
    PopID()
}

// ===== L2/L3 dispatcher table =====
//
// One closure per registered widget. The closure switches on `action`
// ("click", "set", "open", "close") and applies the payload to the
// captured state global. Lazy first-frame registration via widget_finalize
// above keeps the boost's macro layer free of per-kind init emission.

var private g_dispatchers : table<string; lambda<(action : string; payload : JsonValue?) : void>>

// ===== Phase 2 — Result wire format + per-command await =====
//
// Every [live_command] handler returns a Result<T, string> (rendered to JSON
// via the JV overload below). `with_await` is the generic wrapper that auto-
// converts any payload to JSON and threads per-command await modifiers
// (await_frames / await_until / timeout_sec / fire_and_forget) into the
// response so the transport-side await_imgui (widgets/imgui_transport) can
// poll for quiescence after dispatch. Server never blocks — running on the
// GLFW main thread inside tick() means a server-side wait deadlocks the
// frame loop that drives quiescence.

def public JV(r : $Result < auto(TT); auto(EE) >) : JsonValue? {
    //! Wire format for Result<T, E>: `{"ok": bool, "value"|"error": ...}`.
    //! Phase 2 transport contract — every L1/L2/L3 handler returns a
    //! Result; clients read `r?["ok"]` then `r?["value"]` or `r?["error"]`.
    var tab : table<string; JsonValue?>
    tab |> insert("ok", JV(r |> is_ok))
    if (r |> is_ok) {
        tab |> insert("value", JV(r._value))
    } else {
        tab |> insert("error", JV(r._error))
    }
    return JV(tab)
}

struct AwaitModifiers {
    //! Per-command await knobs. Pulled from `input["await"]` by `with_await`;
    //! all fields optional with sensible defaults (5s, no-op).
    @optional await_frames : int = -1
    @optional await_until : string = ""
    @optional timeout_sec : float = 5.0f
    @optional fire_and_forget : bool = false
}

def private with_await(input : JsonValue?; payload : auto(P)) : JsonValue? {
    //! Generic wire-shape helper. JV-converts `payload` (Result, struct,
    //! whatever has a JV overload) and merges per-command await modifiers
    //! from `input["await"]` into the response. Fire-and-forget skips the
    //! merge entirely.
    let mods = from_JV(input?["await"], default<AwaitModifiers>)
    var jv = JV(payload)
    if (mods.fire_and_forget) return jv
    var tab : table<string; JsonValue?>
    if (jv != null && (jv.value is _object)) {
        for (k, v in keys(jv.value as _object), values(jv.value as _object)) {
            tab[k] = v
        }
    }
    if (mods.await_frames >= 0) {
        tab["await_frames"] = JV(mods.await_frames)
    }
    if (!empty(mods.await_until)) {
        tab["await_until"] = JV(mods.await_until)
    }
    tab["timeout_sec"] = JV(mods.timeout_sec)
    return JV(tab)
}

// ===== [live_command] handlers =====

def private extract_string_field(obj : JsonValue?; name : string) : string {
    let v = obj?[name]
    if (v == null || !(v.value is _string)) return ""
    return string(v.value as _string)
}

def private widget_meta_jv(path_key : string; meta : WidgetMeta) : JsonValue? {
    //! Snapshot blob for a widget that's registered in `g_widgets` but did
    //! NOT render this frame — no per-frame entry to join. State payload
    //! comes from the meta's serializer; bbox/hover/active/focus are
    //! absent. `rendered: false` lets clients distinguish "in the source
    //! but currently hidden" from "doesn't exist".
    return JV((kind = meta.kind, rendered = false, payload = invoke(meta.serializer)))
}

def private io_jv() : JsonValue? {
    //! Snapshot of public ImGuiIO state at the moment imgui_snapshot
    //! fired. Mouse position via GetMousePos(); button state via
    //! IsMouseDown for left/middle/right (3-element bool array, mirrors
    //! ImGui's standard ordering). Modifier-key state via IsKeyDown on
    //! the ImGuiMod_* keys (key_ctrl/shift/alt/super) — synth tests
    //! assert on these to detect "Ctrl+A combo actually visible to
    //! widgets" regressions. Phase 2.1: active_id (raw ImGuiID) and
    //! active_widget (resolved path_key, when g_id_to_path knows the id)
    //! drop in only when GetActiveID() is non-zero.
    let pos = GetMousePos()
    var tab : table<string; JsonValue?>
    tab |> insert("mouse_pos", JV(float2(pos.x, pos.y)))
    tab |> insert("mouse_buttons", JV(IsMouseDown(ImGuiMouseButton.Left),
                                      IsMouseDown(ImGuiMouseButton.Middle),
                                      IsMouseDown(ImGuiMouseButton.Right)))
    tab |> insert("key_ctrl",  JV(IsKeyDown(ImGuiKey.ImGuiMod_Ctrl)))
    tab |> insert("key_shift", JV(IsKeyDown(ImGuiKey.ImGuiMod_Shift)))
    tab |> insert("key_alt",   JV(IsKeyDown(ImGuiKey.ImGuiMod_Alt)))
    tab |> insert("key_super", JV(IsKeyDown(ImGuiKey.ImGuiMod_Super)))
    let active_id = GetActiveID()
    if (active_id != 0u) {
        tab |> insert("active_id", JV(active_id))
        if (key_exists(g_id_to_path, active_id)) {
            tab |> insert("active_widget", JV(g_id_to_path[active_id]))
        }
    }
    return JV(tab)
}

[live_command(description = "Snapshot of all registered widgets, with per-frame state joined when rendered")]
def imgui_snapshot(input : JsonValue?) : JsonValue? {
    var globals : table<string; JsonValue?>
    // Walk long-lived registry first — covers unrendered widgets too.
    // EXCEPT: indexed-form entries (path contains '[') expire after one
    // frame absent. A user-table key that got removed must vanish from
    // telemetry; otherwise stale state serializers fire on slots that
    // may have been reused, returning misleading values.
    for (k, meta in keys(g_widgets), values(g_widgets)) {
        if (key_exists(g_registry, k)) {
            globals |> insert(k, widget_entry_jv(k, g_registry[k]))
        } else {
            continue if (find(k, "[") >= 0 && meta.last_seen_frame < g_frame - 1)
            globals |> insert(k, widget_meta_jv(k, meta))
        }
    }
    // Fold in any per-frame entry not in g_widgets — e.g. containers
    // (which still write g_registry but don't go through register_widget
    // yet). Keeps existing 0b behavior intact while the macro is migrated.
    for (k, v in keys(g_registry), values(g_registry)) {
        if (!key_exists(g_widgets, k)) {
            globals |> insert(k, widget_entry_jv(k, v))
        }
    }
    return JV({"frame" => JV(g_frame), "globals" => JV(globals), "io" => io_jv()})
}

def private classify_target(target : string) : tuple<int; string> {
    //! Map target string → (level, resolved). Levels: 2 = L2 path (call the
    //! dispatcher closure), 1 = L1 hex_id (no L2 path; only "click" supported
    //! via bbox synthesis), 0 = unresolvable.
    //!
    //! Resolution order:
    //!   1. "0x..." prefix → parse hex; if g_id_to_path knows it, level 2 with
    //!      the resolved path; else level 1 with the original hex string
    //!      (caller probes g_id_to_bbox).
    //!   2. otherwise treat as a path; level 2 if g_dispatchers has it,
    //!      else level 0.
    if (empty(target)) return (0, "")
    let n = length(target)
    let is_hex = n > 2 && slice(target, 0, 2) == "0x" || n > 2 && slice(target, 0, 2) == "0X"
    if (is_hex) {
        let hid = parse_hex_id(target)
        if (hid == 0u) return (0, target)
        if (key_exists(g_id_to_path, hid)) return (2, g_id_to_path[hid])
        return (1, target)
    }
    if (key_exists(g_dispatchers, target)) return (2, target)
    return (0, target)
}

[async]
def private click_at_coro(center : float2; button : int) : void {
    //! L1 single click: AddMousePosEvent(center) + AddMouseButtonEvent(down)
    //! one frame, AddMouseButtonEvent(up) the next. Used by the hex_id-
    //! targeted L1 fallback when the widget has no L2 dispatcher.
    var io & = unsafe(GetIO())
    io |> AddMousePosEvent(center.x, center.y)
    io |> AddMouseButtonEvent(button, true)
    await_next_frame()
    io |> AddMouseButtonEvent(button, false)
}

def private dispatch_or_err(action : string; input : JsonValue?) : Result<string, string> {
    //! Resolve `target` and invoke its dispatcher closure with `(action, input)`.
    //! Targets: L2 path key (g_dispatchers) or "0x..." hex_id. Hex_ids that
    //! map back to a known path go through L2; hex_ids without a path fall
    //! to L1 click synthesis (only `click` action; the widget still has to
    //! have rendered at least once so g_id_to_bbox knows its bbox).
    let target = extract_string_field(input, "target")
    let (level, resolved) = classify_target(target)
    if (level == 0) return err("unresolvable target: {target}", type<string>)
    if (level == 2) {
        if (action == "focus") {
            if (!key_exists(g_focusable, resolved)) return err("target {resolved} is not focusable (display-only widget)", type<string>)
            g_pending_focus |> insert(resolved)
            return ok(resolved, type<string>)
        }
        invoke(g_dispatchers[resolved], action, input)
        return ok(resolved, type<string>)
    }
    if (action != "click") return err("L1 hex_id fallback only supports 'click', got '{action}'", type<string>)
    let hid = parse_hex_id(resolved)
    if (!key_exists(g_id_to_bbox, hid)) return err("hex_id {resolved} not seen this frame (no bbox to click)", type<string>)
    let bb = g_id_to_bbox[hid]
    let center = float2((bb.x + bb.z) * 0.5f, (bb.y + bb.w) * 0.5f)
    spawn_coroutine(click_at_coro(center, 0))
    return ok("L1:{resolved}", type<string>)
}

[live_command(description = "Queue an L2 click on a widget identified by 'target'")]
def imgui_click(input : JsonValue?) : JsonValue? {
    let r = dispatch_or_err("click", input)
    if (r._is_ok) dispatch_post_command_hooks("click", r._value)
    return with_await(input, r)
}

[live_command(description = "Queue an L3 value set on a widget ('target' name + 'value')")]
def imgui_set(input : JsonValue?) : JsonValue? {
    let r = dispatch_or_err("set", input)
    if (r._is_ok) dispatch_post_command_hooks("set", r._value)
    return with_await(input, r)
}

[live_command(description = "Queue an open command on a container widget ('target' name)")]
def imgui_open(input : JsonValue?) : JsonValue? {
    let r = dispatch_or_err("open", input)
    if (r._is_ok) dispatch_post_command_hooks("open", r._value)
    return with_await(input, r)
}

[live_command(description = "Queue a close command on a container widget ('target' name)")]
def imgui_close(input : JsonValue?) : JsonValue? {
    let r = dispatch_or_err("close", input)
    if (r._is_ok) dispatch_post_command_hooks("close", r._value)
    return with_await(input, r)
}

[live_command(description = "Force keyboard focus to a widget identified by 'target' (path or hex_id)")]
def imgui_focus(input : JsonValue?) : JsonValue? {
    let r = dispatch_or_err("focus", input)
    if (r._is_ok) dispatch_post_command_hooks("focus", r._value)
    return with_await(input, r)
}

// ===== Phase 2.5 — Quiescence probe =====
//
// imgui_await is a polling probe: server reports current liveness state
// (`{quiescent, frame, pending_coroutines, active_id}`) and the client
// (widgets/imgui_transport — Phase 2.6) drives the wait loop. Server-side
// blocking is incompatible with the GLFW-main-thread invariant — the
// dispatch handler runs on the same thread that advances frames.

struct AwaitArgs {
    @optional until : string = "quiescent"
    @optional frame : int = -1
}

struct AwaitResult {
    quiescent : bool = false
    frame : int = 0
    pending_coroutines : int = 0
    active_id : uint = 0u
}

// AwaitResult is a plain-fields struct — JV(struct) auto-walker handles
// the {quiescent, frame, pending_coroutines, active_id} shape for free.

def private is_quiescent() : bool {
    //! True when no widget is being interacted with: no active item, no
    //! mouse button held, no widget owns ImGui's active_id, and no L1
    //! input coroutines are still in-flight.
    return (!IsAnyItemActive() &&
            !IsAnyMouseDown() &&
            GetActiveID() == 0u &&
            empty(g_coroutines))
}

[live_command(description = "Probe quiescence; returns AwaitResult with frame, pending_coroutines, active_id")]
def imgui_await(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, default<AwaitArgs>)
    var result = AwaitResult(frame = g_frame,
                             pending_coroutines = length(g_coroutines),
                             active_id = GetActiveID())
    if (args.frame >= 0) {
        result.quiescent = g_frame >= args.frame
    } else {
        result.quiescent = is_quiescent()
    }
    return with_await(input, ok(result, type<string>))
}

// ===== Phase 2.2 — L1 IO synthesis (coroutine-driven) =====
//
// drag/type_text push raw ImGuiIO events through `[async]` coroutines that
// yield one event per frame. The widget consumes them on subsequent frames
// like any other input. Targets resolve via the same path/hex_id rails the
// L2 dispatchers use. `target` for type_text is informational until Phase
// 2.4 wires the focus arm; chars go to whichever widget already holds focus.

struct DragArgs {
    @optional target : string = ""
    @optional dx : float = 0.0f
    @optional dy : float = 0.0f
    @optional steps : int = 4
    @optional button : int = 0
}

struct TypeTextArgs {
    @optional target           : string = ""
    @optional text             : string = ""
    @optional per_char_frames  : int    = 1
    @optional total_frames     : int    = 0
}

def private parse_hex_id(s : string) : uint {
    //! Parse a "0x<hex>" / "0X<hex>" string into uint. Returns 0u on empty,
    //! missing prefix, or parse failure — 0 is imgui's "no widget" sentinel,
    //! so the failure value is distinguishable from any real id.
    let n = length(s)
    if (n <= 2) return 0u
    let prefix = slice(s, 0, 2)
    if (prefix != "0x" && prefix != "0X") return 0u
    return uint(to_int64(slice(s, 2), true))
}

def public widget_rect(target : string) : tuple<bool; float4> {
    //! Map a target string to its full last-known bbox rect (x0,y0,x1,y1).
    //! Tries L2 path lookup in g_registry first; falls through to hex_id
    //! parse + g_id_to_bbox. Returns (false, float4(0)) when neither hits.
    //! Public so overlay modules (visual_aids, future) can draw on the
    //! foreground drawlist without re-walking g_registry themselves.
    if (key_exists(g_registry, target)) return (true, g_registry[target].bbox)
    let hid = parse_hex_id(target)
    if (hid != 0u && key_exists(g_id_to_bbox, hid)) return (true, g_id_to_bbox[hid])
    return (false, float4(0.0f, 0.0f, 0.0f, 0.0f))
}

def public active_widget_path() : string {
    //! Returns the L2 registry path of the currently focused/active widget,
    //! or "" when nothing is active or the active widget isn't boost-wrapped
    //! (only wrapped widgets are mapped in g_id_to_path). Used by overlays
    //! that want to react to "what is the user typing into right now" without
    //! polling io_jv()'s snapshot every frame.
    let aid = GetActiveID()
    return "" if (aid == 0u)
    return id_to_path(aid)
}

def private resolve_target_bbox(target : string) : tuple<bool; float2> {
    //! Map a target string to the screen-space center of its last-known
    //! bbox. Tries L2 path lookup in g_registry first; falls through to
    //! hex_id parse + g_id_to_bbox. Returns (false, 0,0) when neither hits
    //! — the caller surfaces that as a Result.err.
    let r = widget_rect(target)
    if (!r._0) return (false, float2(0.0f, 0.0f))
    let bb = r._1
    return (true, float2((bb.x + bb.z) * 0.5f, (bb.y + bb.w) * 0.5f))
}

[async]
def private drag_coro(start : float2; endp : float2; steps : int; button : int) : void {
    //! L1 drag: press at `start`, walk through `steps` interpolated points,
    //! release at `endp`. One IO event per await_next_frame() so the widget
    //! sees discrete down → moves → up across consecutive frames.
    var io & = unsafe(GetIO())
    io |> AddMousePosEvent(start.x, start.y)
    io |> AddMouseButtonEvent(button, true)
    await_next_frame()
    for (i in range(1, steps + 1)) {
        let t = float(i) / float(steps)
        let p = start + (endp - start) * t
        io |> AddMousePosEvent(p.x, p.y)
        await_next_frame()
    }
    io |> AddMouseButtonEvent(button, false)
}

[async]
def private type_text_coro(text : string; per_char_frames : int) : void {
    //! L1 keyboard input: push one character every `per_char_frames` frames
    //! to the focused widget via AddInputCharactersUTF8. ASCII-correct;
    //! multi-byte UTF-8 codepoints would split mid-sequence — keep payload
    //! ASCII for now. `per_char_frames=1` is the legacy 1-char/frame rate;
    //! larger values pace typing for tutorial recordings.
    var io & = unsafe(GetIO())
    let n = length(text)
    let gap = max(1, per_char_frames)
    for (i in range(n)) {
        io |> AddInputCharactersUTF8(slice(text, i, i + 1))
        for (_ in range(gap)) {
            await_next_frame()
        }
    }
}

[live_command(description = "L1 drag: synthesize mouse-press → N moves → release at target widget bbox center")]
def imgui_drag(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, default<DragArgs>)
    let (found, start) = resolve_target_bbox(args.target)
    if (!found) return with_await(input, err("drag target unresolved: {args.target}", type<string>))
    let endp = start + float2(args.dx, args.dy)
    spawn_coroutine(drag_coro(start, endp, max(args.steps, 1), args.button))
    dispatch_post_command_hooks("drag", args.target)
    return with_await(input, ok(args.target, type<string>))
}

[live_command(description = "L1 keyboard: push UTF-8 chars to the focused widget. Args: text, per_char_frames (default 1) | total_frames (preferred for fixed-duration typing)")]
def imgui_type_text(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, default<TypeTextArgs>)
    if (empty(args.text)) return with_await(input, err("missing 'text'", type<string>))
    var gap = max(1, args.per_char_frames)
    if (args.total_frames > 0) {
        gap = max(1, args.total_frames / length(args.text))
    }
    spawn_coroutine(type_text_coro(args.text, gap))
    if (!empty(args.target)) dispatch_post_command_hooks("type_text", args.target)
    return with_await(input, ok(args.target, type<string>))
}
