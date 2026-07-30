options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options persistent_heap   // collectable heap so per-frame JsonValue churn (snapshots, payloads) is reclaimed by delete + GC, not leaked; unifies program-wide

module imgui_boost_runtime shared

require imgui public
require live/live_vars public
require live/live_commands
require live_host public
require daslib/json public
require daslib/json_boost public
require daslib/coroutines public
require daslib/async_boost
require daslib/archive
require daslib/result public
require daslib/rtti
require math
require strings

// See handmade/module-imgui_boost_runtime.rst for the runtime overview.

// ===== Per-kind state structs =====
//
// Mixed @live (preserved across hot-reload) and @optional (transient,
// dropped from JV when zero-valued). Common per-frame imgui state — hex_id,
// bbox, hover/active/focus — lives in WidgetEntry, not here, so the structs
// stay small per kind.

struct ClickState {
    @live click_count : int                      //! cumulative across reloads
    @optional clicked : bool                     //! true on the frame the button fired
    @optional text : string                      //! visible label (## id suffix stripped) for snapshot/tests
}

struct SliderStateFloat {
    @live value : float                          //! preserved across reload
    @live bounds : tuple<float; float>           //! (min, max)
    @optional has_pending : bool                 //! imgui_force_set L3 sets; render fn consumes
    @optional pending_value : float              //! next-frame value queued by imgui_force_set
    @optional changed : bool                     //! true on the frame the widget fired
}

struct SliderStateFloat2 {
    @live value : float2                         //! preserved across reload
    @live bounds : tuple<float; float>           //! (min, max) applied to every component
    @optional has_pending : bool                 //! imgui_force_set queued; consumed next frame
    @optional pending_value : float2             //! next-frame value queued by imgui_force_set
    @optional changed : bool                     //! true on the frame the widget fired
}

struct SliderStateFloat3 {
    @live value : float3                         //! preserved across reload
    @live bounds : tuple<float; float>           //! (min, max) applied to every component
    @optional has_pending : bool                 //! imgui_force_set queued; consumed next frame
    @optional pending_value : float3             //! next-frame value queued by imgui_force_set
    @optional changed : bool                     //! true on the frame the widget fired
}

struct SliderStateFloat4 {
    @live value : float4                         //! preserved across reload
    @live bounds : tuple<float; float>           //! (min, max) applied to every component
    @optional has_pending : bool                 //! imgui_force_set queued; consumed next frame
    @optional pending_value : float4             //! next-frame value queued by imgui_force_set
    @optional changed : bool                     //! true on the frame the widget fired
}

struct SliderStateInt {
    @live value : int                            //! preserved across reload
    @live bounds : tuple<int; int>               //! (min, max)
    @optional has_pending : bool                 //! imgui_force_set queued; consumed next frame
    @optional pending_value : int                //! next-frame value queued by imgui_force_set
    @optional changed : bool                     //! true on the frame the widget fired
}

struct SliderStateInt2 {
    @live value : int2                           //! preserved across reload
    @live bounds : tuple<int; int>               //! (min, max) applied to every component
    @optional has_pending : bool                 //! imgui_force_set queued; consumed next frame
    @optional pending_value : int2               //! next-frame value queued by imgui_force_set
    @optional changed : bool                     //! true on the frame the widget fired
}

struct SliderStateInt3 {
    @live value : int3                           //! preserved across reload
    @live bounds : tuple<int; int>               //! (min, max) applied to every component
    @optional has_pending : bool                 //! imgui_force_set queued; consumed next frame
    @optional pending_value : int3               //! next-frame value queued by imgui_force_set
    @optional changed : bool                     //! true on the frame the widget fired
}

struct SliderStateInt4 {
    @live value : int4                           //! preserved across reload
    @live bounds : tuple<int; int>               //! (min, max) applied to every component
    @optional has_pending : bool                 //! imgui_force_set queued; consumed next frame
    @optional pending_value : int4               //! next-frame value queued by imgui_force_set
    @optional changed : bool                     //! true on the frame the widget fired
}

struct ToggleState {
    //! Shared across Checkbox / RadioButton (bool form) / Selectable / MenuItem.
    //! `pending_toggle` + `pending_value` let imgui_force_set steer the next frame's
    //! value deterministically; the renderer consumes-then-clears.
    @live value : bool                               //! current toggle state; preserved across reload
    @optional pending_toggle : bool                  //! imgui_force_set L3 sets; consumed next frame
    @optional pending_value : bool                   //! next-frame value queued by imgui_force_set
    @optional changed : bool                         //! true on the frame the widget fired
    @optional text : string                          //! visible label (## id suffix stripped) for snapshot/tests
}

struct RadioIntState {
    //! Backs RadioButton(int form). `value` is the currently-selected button
    //! id; the widget def takes the per-call `v_button` and writes value=v_button
    //! when the user clicks. `imgui_force_set {"value":N}` steers the selection.
    @live value : int                                //! currently-selected button id
    @optional pending_set : bool                     //! imgui_force_set queued; consumed next frame
    @optional pending_value : int                    //! next-frame selection queued by imgui_force_set
    @optional changed : bool                         //! true on the frame the selection changed
}

// Drag* family — one state struct per scalar/vector value type. `bounds` is the only persistent
// config (zero-init = unclamped, per ImGui's v_min==v_max). speed/format/flags are per-call
// (globals can't apply field defaults; string defaults trip the uninit check).

struct DragStateFloat {
    @live value : float                          //! preserved across reload
    @live bounds : tuple<float; float>           //! (min, max); zero-init = unclamped
    @optional has_pending : bool                 //! imgui_force_set queued; consumed next frame
    @optional pending_value : float              //! next-frame value queued by imgui_force_set
    @optional changed : bool                     //! true on the frame the widget fired
}

struct DragStateFloat2 {
    @live value : float2                         //! preserved across reload
    @live bounds : tuple<float; float>           //! (min, max); zero-init = unclamped
    @optional has_pending : bool                 //! imgui_force_set queued; consumed next frame
    @optional pending_value : float2             //! next-frame value queued by imgui_force_set
    @optional changed : bool                     //! true on the frame the widget fired
}

struct DragStateFloat3 {
    @live value : float3                         //! preserved across reload
    @live bounds : tuple<float; float>           //! (min, max); zero-init = unclamped
    @optional has_pending : bool                 //! imgui_force_set queued; consumed next frame
    @optional pending_value : float3             //! next-frame value queued by imgui_force_set
    @optional changed : bool                     //! true on the frame the widget fired
}

struct DragStateFloat4 {
    @live value : float4                         //! preserved across reload
    @live bounds : tuple<float; float>           //! (min, max); zero-init = unclamped
    @optional has_pending : bool                 //! imgui_force_set queued; consumed next frame
    @optional pending_value : float4             //! next-frame value queued by imgui_force_set
    @optional changed : bool                     //! true on the frame the widget fired
}

struct DragStateInt {
    @live value : int                            //! preserved across reload
    @live bounds : tuple<int; int>               //! (min, max); zero-init = unclamped
    @optional has_pending : bool                 //! imgui_force_set queued; consumed next frame
    @optional pending_value : int                //! next-frame value queued by imgui_force_set
    @optional changed : bool                     //! true on the frame the widget fired
}

struct DragStateInt2 {
    @live value : int2                           //! preserved across reload
    @live bounds : tuple<int; int>               //! (min, max); zero-init = unclamped
    @optional has_pending : bool                 //! imgui_force_set queued; consumed next frame
    @optional pending_value : int2               //! next-frame value queued by imgui_force_set
    @optional changed : bool                     //! true on the frame the widget fired
}

struct DragStateInt3 {
    @live value : int3                           //! preserved across reload
    @live bounds : tuple<int; int>               //! (min, max); zero-init = unclamped
    @optional has_pending : bool                 //! imgui_force_set queued; consumed next frame
    @optional pending_value : int3               //! next-frame value queued by imgui_force_set
    @optional changed : bool                     //! true on the frame the widget fired
}

struct DragStateInt4 {
    @live value : int4                           //! preserved across reload
    @live bounds : tuple<int; int>               //! (min, max); zero-init = unclamped
    @optional has_pending : bool                 //! imgui_force_set queued; consumed next frame
    @optional pending_value : int4               //! next-frame value queued by imgui_force_set
    @optional changed : bool                     //! true on the frame the widget fired
}

struct DragStateRangeFloat {
    //! `value.x` = current_min, `value.y` = current_max. Bounds clamp both.
    //! `imgui_force_set {"value":[lo,hi]}` writes the whole range atomically.
    @live value : float2                         //! (current_min, current_max); preserved across reload
    @live bounds : tuple<float; float>           //! (min, max) clamp applied to both endpoints
    @optional has_pending : bool                 //! imgui_force_set queued; consumed next frame
    @optional pending_value : float2             //! next-frame range queued by imgui_force_set
    @optional changed : bool                     //! true on the frame the range changed
}

struct DragStateRangeInt {
    //! `value.x` = current_min, `value.y` = current_max.
    @live value : int2                           //! (current_min, current_max); preserved across reload
    @live bounds : tuple<int; int>               //! (min, max) clamp applied to both endpoints
    @optional has_pending : bool                 //! imgui_force_set queued; consumed next frame
    @optional pending_value : int2               //! next-frame range queued by imgui_force_set
    @optional changed : bool                     //! true on the frame the range changed
}

// Input* numeric family. No bounds — InputFloat/Int/Double don't clamp; step/step_fast/format
// are per-call. Only `value` persists; same has_pending/pending_value/changed shape as Drag/Slider
// so `pending_value_finalize` covers the dispatch.

struct InputStateFloat {
    @live value : float                          //! preserved across reload
    @optional has_pending : bool                 //! imgui_force_set queued; consumed next frame
    @optional pending_value : float              //! next-frame value queued by imgui_force_set
    @optional changed : bool                     //! true on the frame the widget fired
}

struct InputStateFloat2 {
    @live value : float2                         //! preserved across reload
    @optional has_pending : bool                 //! imgui_force_set queued; consumed next frame
    @optional pending_value : float2             //! next-frame value queued by imgui_force_set
    @optional changed : bool                     //! true on the frame the widget fired
}

struct InputStateFloat3 {
    @live value : float3                         //! preserved across reload
    @optional has_pending : bool                 //! imgui_force_set queued; consumed next frame
    @optional pending_value : float3             //! next-frame value queued by imgui_force_set
    @optional changed : bool                     //! true on the frame the widget fired
}

struct InputStateFloat4 {
    @live value : float4                         //! preserved across reload
    @optional has_pending : bool                 //! imgui_force_set queued; consumed next frame
    @optional pending_value : float4             //! next-frame value queued by imgui_force_set
    @optional changed : bool                     //! true on the frame the widget fired
}

struct InputStateInt {
    @live value : int                            //! preserved across reload
    @optional has_pending : bool                 //! imgui_force_set queued; consumed next frame
    @optional pending_value : int                //! next-frame value queued by imgui_force_set
    @optional changed : bool                     //! true on the frame the widget fired
}

struct InputStateInt2 {
    @live value : int2                           //! preserved across reload
    @optional has_pending : bool                 //! imgui_force_set queued; consumed next frame
    @optional pending_value : int2               //! next-frame value queued by imgui_force_set
    @optional changed : bool                     //! true on the frame the widget fired
}

struct InputStateInt3 {
    @live value : int3                           //! preserved across reload
    @optional has_pending : bool                 //! imgui_force_set queued; consumed next frame
    @optional pending_value : int3               //! next-frame value queued by imgui_force_set
    @optional changed : bool                     //! true on the frame the widget fired
}

struct InputStateInt4 {
    @live value : int4                           //! preserved across reload
    @optional has_pending : bool                 //! imgui_force_set queued; consumed next frame
    @optional pending_value : int4               //! next-frame value queued by imgui_force_set
    @optional changed : bool                     //! true on the frame the widget fired
}

struct InputStateDouble {
    @live value : double                         //! preserved across reload
    @optional has_pending : bool                 //! imgui_force_set queued; consumed next frame
    @optional pending_value : double             //! next-frame value queued by imgui_force_set
    @optional changed : bool                     //! true on the frame the widget fired
}

// Color* family. ColorEdit3/ColorPicker3 share ColorState3 (RGB); ColorEdit4/ColorPicker4 share
// ColorState4 (RGBA). ColorButton is click-only (caller passes color per-frame) so it reuses
// ClickState. flags + ImGui color flags (DisplayHSV/Float/NoInputs/…) are per-call, not persistent.

struct ColorState3 {
    @live value : float3                         //! RGB in [0..1]
    @optional has_pending : bool                 //! imgui_force_set queued; consumed next frame
    @optional pending_value : float3             //! next-frame RGB queued by imgui_force_set
    @optional changed : bool                     //! true on the frame the color changed
}

struct ColorState4 {
    @live value : float4                         //! RGBA in [0..1]
    @optional has_pending : bool                 //! imgui_force_set queued; consumed next frame
    @optional pending_value : float4             //! next-frame RGBA queued by imgui_force_set
    @optional changed : bool                     //! true on the frame the color changed
}

// Choice family. `value` = selected index; `imgui_force_set {"value":N}` steers next frame.
// items marshal C-side without a shim: combo packs a zero-separated array<uint8> reinterpreted as
// `string#` for ImGui's null-separated overload; list_box passes addr(items[0]) (array<string> is char**).

struct ComboState {
    @live value : int = -1                       //! selected index, -1 = none
    @optional has_pending : bool                 //! imgui_force_set queued; consumed next frame
    @optional pending_value : int                //! next-frame selection queued by imgui_force_set
    @optional changed : bool                     //! true on the frame the selection changed
    @optional items_buf : array<uint8>           //! transient zero-separated buffer; rebuilt each frame
}

struct ListBoxState {
    @live value : int = -1                       //! selected index, -1 = none
    @optional has_pending : bool                 //! imgui_force_set queued; consumed next frame
    @optional pending_value : int                //! next-frame selection queued by imgui_force_set
    @optional changed : bool                     //! true on the frame the selection changed
}

// Text-input family. `buffer` is canonical storage; `value` is the extracted (clone-on-change)
// string given to user code. `input_text_growable` reuses this struct — its CallbackResize lambda
// updates `capacity`+`buffer` in place.

struct InputTextState {
    @live value : string = ""                             //! current text content (clone of buffer)
    @live capacity : int = 256                            //! working buffer size in bytes (incl. trailing null)
    @optional has_pending : bool                          //! imgui_force_set queued; consumed next frame
    @optional pending_value : string = ""                 //! next-frame text queued by imgui_force_set
    @optional changed : bool                              //! true on the frame the text changed
    @optional buffer : array<uint8>                       //! working buffer; mirrors `value`, allocated to capacity on first frame / post-reload
}

struct TextShowState {
    //! Backs the `text_show` display widget. Read-only from the user's perspective — `value` is set
    //! by a call-site initializer (`text_show(STATUS, value="saved")`), programmatically, or via
    //! `imgui_force_set`; the standard serializer surfaces it for telemetry.
    @live value : string = ""                          //! displayed text; settable via call-site initializer or imgui_force_set
}

struct PlotState {
    //! Backs `plot_lines` / `plot_histogram`. Plotted values are passed per-frame (not stored);
    //! state carries last-frame's call args for telemetry — label, (scale_min, scale_max) range,
    //! sample count. Read-only — no dispatcher.
    @optional title : string                           //! overlay text
    @optional bounds : tuple<float; float>             //! (scale_min, scale_max)
    @optional samples : int                            //! length(values) last frame
}

struct NarrativeState {
    //! Backs every text-bearing narrative widget (text / text_unformatted / text_wrapped /
    //! text_colored / text_disabled / bullet_text / separator_text / set_tooltip / set_item_tooltip).
    //! Displayed string is per-call, echoed into `value` each frame for telemetry. Read-only.
    @optional value : string                           //! per-frame echo of call-site `text` arg
}

struct LabelTextState {
    //! Backs the `label_text` widget. ImGui's `LabelText(label, fmt)` shows label-key on the left,
    //! value-text on the right; both per-call, echoed for telemetry. Read-only. (Field is `key` not
    //! `label` — `label` is reserved in gen2.)
    @optional key : string                             //! per-frame echo of the left-side label string
    @optional value : string                           //! per-frame echo of the right-side value string
}

struct EmptyMarkerState {
    //! Backs the stateless decoration widgets (`separator`, `spacing`, `new_line`, `dummy`,
    //! `same_line`, `bullet`). The state global exists only because [widget] auto-emit needs
    //! something to instantiate.
    @optional _placeholder : bool                      //! unused; keeps the struct non-empty for [widget] auto-emit
}

struct ImageState {
    //! Backs the `image` display widget. Texture handle is opaque (void?),
    //! so telemetry echoes only the actionable per-call args — size, uv
    //! rectangle, tint/border colors. Read-only — no dispatcher.
    @optional size : float2                            //! per-call display size
    @optional uv0 : float2                             //! upper-left uv
    @optional uv1 : float2                             //! lower-right uv
    @optional tint_col : float4                        //! tint applied to texels
    @optional border_col : float4                      //! border color (zero alpha = no border)
}

struct ProgressBarState {
    //! Backs the `progress_bar` widget. `fraction` is per-frame per-call (caller drives progress);
    //! `overlay` is the optional center-aligned label; `size` is the per-call display size.
    //! Read-only — no dispatcher.
    @optional fraction : float                         //! per-frame progress in [0,1]; -1 means indeterminate
    @optional overlay : string                         //! optional overlay text (e.g. "42/100")
    @optional size : float2                            //! per-call display size; (-1, 0) defaults to full-width
}

// ===== Container family (Phase 0b.3) =====

// `[container]` widgets wrap a block arg and push onto `g_container_path` so leaf children get
// hierarchical registry keys. Per-kind state keeps only what's worth remembering across frames +
// reloads; stateless containers get an empty struct so [widget] auto-emit has something to instantiate.

struct WindowState {
    //! Backs the `window` container. `open` defaults true so a closable window renders from frame 1;
    //! X-button / `imgui_close` flips it, `@live` carries it across reload. Pos is NOT @live (ImGui persists
    //! it via imgui.ini). imgui_open/close route through pending_open/close, applied at the next frame's top.
    @live open : bool = true                  //! window open state; preserved across reload
    @optional flags : ImGuiWindowFlags        //! per-call ImGuiWindowFlags (echoed for telemetry)
    @optional pos : float2                    //! observed window position (top-left, screen coords)
    @optional size : float2                   //! observed window size
    @optional collapsed : bool                //! observed collapsed state
    @optional scroll : float2                 //! (GetScrollX, GetScrollY) of the window
    @optional scroll_max : float2             //! (GetScrollMaxX, GetScrollMaxY)
    @optional pending_open : bool             //! imgui_open queued; applied next frame
    @optional pending_close : bool            //! imgui_close queued; applied next frame
    @optional pending_size : float2           //! imgui_set_window_size queued — applied via SetNextWindowSize next frame
    @optional pending_size_set : bool         //! gates pending_size (so (0,0) is a valid request, not a sentinel)
    @optional pending_scroll : float2         //! imgui_click scroll-into-view queued — applied via SetNextWindowScroll next frame
    @optional pending_scroll_set : bool       //! gates pending_scroll (so (0,0) is a valid request, not a sentinel)
}

struct ChildState {
    //! Backs the `child` container. All fields are observed (scroll); per-
    //! call config (size, child_flags, window_flags) is supplied via the
    //! named-tuple at the call site.
    @optional size : float2                   //! per-call child region size (echoed for telemetry)
    @optional child_flags : ImGuiChildFlags   //! per-call ImGuiChildFlags
    @optional window_flags : ImGuiWindowFlags //! per-call ImGuiWindowFlags
    @optional scroll : float2                 //! (GetScrollX, GetScrollY)
    @optional scroll_max : float2             //! (GetScrollMaxX, GetScrollMaxY)
}

struct GroupState {
    //! Backs the `group` container. Pure layout grouping — no persistent/observable state, but
    //! [container] still needs a struct to hang the auto-emitted global on. `_placeholder` stays
    //! @optional so JV(state) emits `{}`.
    @optional _placeholder : bool             //! unused; keeps the struct non-empty for [container] auto-emit
}

struct MenuBarState {
    //! Backs the `menu_bar` container. Stateless wrapper.
    @optional _placeholder : bool             //! unused; keeps the struct non-empty for [container] auto-emit
}

struct MenuState {
    //! Backs the `menu` container — a single drop-down inside a menu bar.
    //! `enabled` is per-call; `opened` is per-frame observed.
    @optional enabled : bool                  //! per-call enable flag (echoed for telemetry)
    @optional opened : bool                   //! observed per-frame: menu is currently expanded
}

struct PopupState {
    //! Shared by `popup` and `popup_modal`. `open` is @live (survives reload). imgui_open/close route
    //! through pending_open/close; the renderer issues OpenPopup/CloseCurrentPopup on the next frame.
    @live open : bool                         //! popup open state; preserved across reload
    @optional flags : ImGuiWindowFlags        //! per-call ImGuiWindowFlags
    @optional pending_open : bool             //! imgui_open queued; OpenPopup fires next frame
    @optional pending_close : bool            //! imgui_close queued; CloseCurrentPopup fires next frame
}

struct TabBarState {
    //! Backs the `tab_bar` container. ImGui drives active-tab selection
    //! itself; we just observe.
    @optional flags : ImGuiTabBarFlags        //! per-call ImGuiTabBarFlags
}

struct TableState {
    //! Backs the `table` container. ImGui owns column-resize/sort state internally (TableSettings);
    //! per-call config is echoed for snapshot consumers. sort_specs / multi-select capture is
    //! deferred to the `tables.das` port.
    @optional columns : int                   //! per-call column count
    @optional flags : ImGuiTableFlags         //! per-call ImGuiTableFlags
    @optional outer_size : float2             //! per-call outer_size (echoed)
    @optional inner_width : float             //! per-call inner_width (echoed)
}

struct TabItemState {
    //! Backs the `tab_item` container. `open` defaults to true so a closable
    //! tab renders from frame 1; the X-button or `imgui_close` flips it false.
    //! `@live` carries that across reload.
    @live open : bool = true                  //! tab open state; preserved across reload
    @optional flags : ImGuiTabItemFlags       //! per-call ImGuiTabItemFlags
    @optional pending_open : bool             //! imgui_open queued; applied next frame
    @optional pending_close : bool            //! imgui_close queued; applied next frame
    @optional pending_select : bool           //! imgui_select queued; applied next frame (SetSelected)
    @optional selected : bool                 //! observed: is this the active tab this frame (snapshot-visible)
}

struct WrapTabBarState {
    //! Backs `wrap_tab_bar` (the multi-row/wrapping tab bar ImGui lacks). Active tab is an index like
    //! `combo`: `imgui_force_set {"value":N}` selects tab N next frame. `labels` mirrors last frame's
    //! tabs (header strip is laid out a frame behind so it can precede the bodies).
    @live value : int                         //! active tab index; preserved across reload
    @optional has_pending : bool              //! imgui_force_set queued; consumed next frame
    @optional pending_value : int             //! next-frame index queued by imgui_force_set
    @optional labels : array<string>          //! tab labels from the previous frame (snapshot-visible)
}

struct TooltipState {
    //! Backs both `tooltip` and `item_tooltip`. Tooltips don't persist —
    //! they're a per-frame chrome pop.
    @optional _placeholder : bool             //! unused; keeps the struct non-empty for [container] auto-emit
}

struct PopupContextItemState {
    //! Backs `popup_context_item`. Right-click attaches to the previous item
    //! and ImGui drives open/close internally; no pending flags needed.
    @optional _placeholder : bool             //! unused; keeps the struct non-empty for [container] auto-emit
}

struct PopupContextWindowState {
    //! Backs `popup_context_window`. Right-click anywhere in the enclosing
    //! window opens the popup; ImGui drives open/close internally.
    @optional _placeholder : bool             //! unused; keeps the struct non-empty for [container] auto-emit
}

struct PopupWindowState {
    //! Backs `popup_window` — stateless BeginPopup/EndPopup wrapper for the manual-trigger pattern:
    //! caller drives `open_popup(str_id, flags)` (typically on hover+release), then `popup_window`
    //! brackets the body when ImGui reports it open. ImGui owns the lifecycle by `str_id`, no pending flags.
    @optional _placeholder : bool             //! unused; keeps the struct non-empty for [container] auto-emit
}

struct TextFilterState {
    //! Backs `text_filter`. Holds the bound C++ `ImGuiTextFilter` inline — zero-init is safe (empty
    //! InputBuf/Filters ImVector, CountGrep=0), no ctor call needed. Renders the inline InputText via
    //! the bound `Draw()`; `passes_filter` delegates to the hand-bound `PassFilter(filter, text)`.
    @safe_when_uninitialized @optional value : string  //! mirrored InputBuf text for snapshot/telemetry
    @safe_when_uninitialized filter : ImGuiTextFilter
}

// JV + archive::serialize overrides for TextFilterState: the bound `ImGuiTextFilter` has no
// daslang JV/serialize binding (and no safe `delete`), so the generic field-walking generics would
// fail to instantiate. Only the editor-text mirror is serialized; the filter regenerates from `value` post-reload.
def public JV(value : TextFilterState) : JsonValue? {
    return JV((value = value.value))
}

def public serialize(var arch : Archive; var value : TextFilterState) {
    arch |> _::serialize(value.value)
}

struct DragDropSourceState {
    //! Backs `drag_drop_source`. Drag activation lives entirely in ImGui's
    //! internal drag-drop state machine; body runs only on active drag.
    @optional _placeholder : bool             //! unused; keeps the struct non-empty for [container] auto-emit
}

struct DragDropTargetState {
    //! Backs `drag_drop_target`. Body runs only when a payload-bearing drag
    //! is hovering this target — caller still calls AcceptDragDropPayload inside.
    accepted : int                            //! running count of drops ImGui delivered here; telemetry rail to self-verify a real drop landed
}

struct TreeNodeState {
    //! Backs the `tree_node` container. ImGui owns the open/close state
    //! internally; we mirror what's observable so snapshots reflect truth,
    //! and the dispatcher routes open/close through SetNextItemOpen.
    @optional flags : ImGuiTreeNodeFlags      //! per-call ImGuiTreeNodeFlags
    @optional opened : bool                   //! observed per-frame: TreeNode returned true
    @optional pending_open : bool             //! imgui_open queued; SetNextItemOpen(true) fires next frame
    @optional pending_close : bool            //! imgui_close queued; SetNextItemOpen(false) fires next frame
}

struct CollapsingHeaderState {
    //! Backs `collapsing_header`. Same shape as TreeNodeState plus a `@live open` slot for the
    //! X-button form (CollapsingHeader(label, bool* p_visible, flags)); `open` defaults true so a
    //! closable section renders from frame 1, X-button / `imgui_close` flips it.
    @live open : bool = true                  //! closable form's p_visible state; preserved across reload
    @optional flags : ImGuiTreeNodeFlags      //! per-call ImGuiTreeNodeFlags
    @optional opened : bool                   //! observed per-frame: CollapsingHeader returned true
    @optional pending_open : bool             //! imgui_open queued; SetNextItemOpen(true) fires next frame
    @optional pending_close : bool            //! imgui_close queued; SetNextItemOpen(false) fires next frame
}

struct MainMenuBarState {
    //! Backs the `main_menu_bar` container — `BeginMainMenuBar()` /
    //! `EndMainMenuBar()`. Screen-top, persistent; ImGui owns visibility,
    //! we just register the container path. Stateless wrapper.
    @optional _placeholder : bool             //! unused; keeps the struct non-empty for [container] auto-emit
}

// ===== Per-frame registry =====

// One entry per render of any boost-wrapped widget, keyed by the widget's daslang identifier
// ("SAVE_BTN" / "SPEED" / ...). Cleared at the start of each frame.

struct WidgetEntry {
    //! Per-frame snapshot of one rendered widget. Common fields populated each frame; per-kind detail
    //! is materialized on demand at imgui_snapshot from the ``(addr, ti)`` pair in ``g_widgets[ident]``
    //! (via ``sprint_json_at`` + ``read_json``). Stateless widgets instead set the ``serialize`` hook (+ ctx/id), built lazily at snapshot — zero per-frame JSON.
    kind : string                                //! widget kind ("button", "slider_float", ...)
    hex_id : uint                                //! ImGui's GetID() for this widget on this frame
    bbox : float4                                //! packed [x0, y0, x1, y1] — ALWAYS screen-space
    @optional canvas_bbox : float4               //! packed [x0,y0,x1,y1] in a transformed canvas's LOCAL space. Emitted ONLY when the widget was captured under a non-identity snapshot item-transform (e.g. inside a node-editor canvas) — `bbox` is then its screen-space mapping. Absent for the common case where the widget is already screen-space.
    @optional sub_bboxes : array<float4>         //! per-component sub-rects for multi-component widgets (slider_float2/3/4, slider_int2/3/4, drag_float2/3/4, drag_int2/3/4, and the two-handle drag_*_range2 forms where [0]=lo, [1]=hi): each entry is one handle's screen-space [x0,y0,x1,y1]. Lets a driver target component N deterministically (widget_component_point) instead of guessing x within the combined bbox. Absent for single-component widgets.
    @optional hover : bool                       //! IsItemHovered() this frame
    @optional active : bool                      //! IsItemActive() this frame
    @optional focus : bool                       //! IsItemFocused() this frame
    @optional payload : JsonValue?               //! populated by widget_entry_jv from (addr, ti) via sprint_json_at + read_json, or from the serialize hook below
    @optional serialize_id : int                 //! id/key passed to `serialize` (stateless lazy-payload hook); JV-skipped at 0
    serialize_ctx : void?                        //! opaque context passed to `serialize` (e.g. editor handle); JV skips void?
    serialize : function<(ctx : void?; id : int) : JsonValue?>  //! lazy per-snapshot payload builder for stateless widgets — never called per frame; JV skips functions
}

def private resolve_meta_addr(meta : WidgetMeta) : void? {
    return meta.addr_getter_str(meta.k_str) if (meta.has_str_key)
    return meta.addr_getter_int(meta.k_int) if (meta.addr_getter_int != null)
    return meta.state_addr
}

def private state_payload_jv(state_addr : void?; ti : TypeInfo const?) : JsonValue? {
    //! Build the per-widget state JSON payload. Always routes through ``sprint_json_at`` (the C++ debug walker). For non-Structure types (bool / scalar / vector pointee — e.g. ``edit_*`` rails), wraps the raw output in a ``{"value": ...}`` envelope to match the historic shape; for structures the walker output is used as-is. ``from_JV<vecN>`` accepts both the array form ``[x,y,z]`` (which the walker emits) and the object form ``{x,y,z}``, so callers don't need a per-kind serializer to massage the wire shape. Returns ``null`` when ``state_addr`` or ``ti`` is missing.
    return null if (state_addr == null || ti == null)
    var err : string
    let raw = sprint_json_at(state_addr, *ti, false)
    return read_json("\{\"value\":{raw}\}", err) if ((*ti).basicType != Type.tStructure)
    return read_json(raw, err)
}

def private widget_entry_jv(ident : string; var entry : WidgetEntry) : JsonValue? {
    // Payload: reflected (state_addr, ti) from g_widgets[ident] (stateful), else the lazy `serialize` hook (stateless).
    let meta = unsafe(g_widgets?[ident])
    if (meta != null) {
        entry.payload = state_payload_jv(resolve_meta_addr(*meta), meta.ti)
    } elif (entry.serialize != null) {
        entry.payload = entry.serialize(entry.serialize_ctx, entry.serialize_id)
    }
    return JV(entry)
}

var g_registry : table<string; WidgetEntry>
var g_frame : int = 0

// ===== Long-lived widget registry (Phase 1) =====

// `g_widgets` is the long-lived twin of `g_registry`: [widget] emits `register_widget(...)` into a
// per-module `[init]`, so every widget appears at module load (even before its container opens).
// imgui_snapshot walks it and joins `g_registry[k]` for this frame's bbox/hover/active/focus; keying starts at the bare ident, then widget_finalize migrates it to the container-aware path_key on first render so cross-container siblings stay distinct.

struct WidgetMeta {
    module_name : string                                  //! owning module; lets clear_module_widgets drop on reload
    kind : string                                         //! widget kind ("button", "slider_float", ...)
    state_addr : void?                                    //! non-indexed: stable address of the module-scope state global (set by ``register_widget``). Indexed widgets pass ``null`` here and re-resolve per-k through ``addr_getter_int`` / ``addr_getter_str``. Named ``state_addr`` (not ``addr``) because daslang reserves ``addr`` as the builtin address-of operator.
    ti : TypeInfo const?                            //! ``TypeInfo`` of the state value at ``addr`` (or the indexed slot's element). Drives ``sprint_json_at`` at snapshot time and ``sscan_json_at`` on dispatcher set-actions without per-call-site daslang thunks.
    addr_getter_int : function<(k : int) : void?>         //! int-indexed path only: resolves live state-struct address per-call against the user's ``table<int; …>``. Null for non-indexed widgets — use ``addr`` instead.
    k_int : int                                           //! int key stored at registration for indexed-int widgets; 0 when not indexed.
    addr_getter_str : function<(k : string) : void?>      //! string-indexed path only: resolves live state-struct address for ``table<string; …>``. Null when ``has_str_key=false``.
    k_str : string                                        //! string key stored at registration; empty when ``has_str_key=false``.
    has_str_key : bool                                    //! true => use ``addr_getter_str`` + ``k_str``; false => use ``state_addr`` (non-indexed) or ``addr_getter_int`` + ``k_int``.
    last_seen_frame : int                                 //! last g_frame this path appeared in g_registry; -1 = never rendered
}

var g_widgets : table<string; WidgetMeta>

// Sentinel module_name for entries created lazily by `widgets_mark_seen` (the [edit_widget]
// external-pointer path). Pre-registered entries carry `compiling_module().name` ("" for main files);
// lazy entries get this sentinel so the freshness gate can drop them after a frame absent without hitting main-file entries.
let private LAZY_EDIT_WIDGET_MODULE = "<edit_widget>"

// ===== Phase 2 reverse maps + coroutine runner =====

// `g_id_to_path`: long-lived hex_id → path_key (populated by every finalize) so `io_jv()` resolves
// GetActiveID's opaque ImGuiID back to a user path. `g_id_to_bbox`: per-frame hex_id → bbox, L1
// click/drag fallback for "0x..." targets absent from g_id_to_path. `g_coroutines`: in-flight synth coroutines, advanced one step/frame by advance_coroutines.

var private g_id_to_path : table<uint; string>
var private g_id_to_bbox : table<uint; float4>
var private g_coroutines : array<Coroutine>
var private g_focusable : table<string>     //! set: paths whose widgets accept keyboard focus
var private g_pending_focus : table<string> //! set: paths to focus on their next widget_prelude

def public id_to_path(hid : uint) : string {
    //! Resolve a raw ImGui ``hex_id`` to its boost path_key, or ``""`` when unmapped.
    return key_exists(g_id_to_path, hid) ? g_id_to_path[hid] : ""
}

def public id_to_bbox(hid : uint) : float4 {
    //! Per-frame ``hex_id`` → bbox lookup; returns ``float4(0)`` when the id didn't render this frame.
    return key_exists(g_id_to_bbox, hid) ? g_id_to_bbox[hid] : float4(0.0f, 0.0f, 0.0f, 0.0f)
}

// No separate `extras` channel for transient per-frame data (text-input cursor/selection, tooltip
// content): it all fits inline in each kind's state struct (ChildState.scroll, WindowState.collapsed,
// TreeNodeState.opened). Reintroduce only when a consumer needs data that doesn't fit a state field.

// ===== Container path stack =====

// `g_container_path`: chain of open `[container]` idents, outermost-first — pushed on body entry,
// popped on exit; leaf widgets read it at finalize for hierarchical registry/dispatcher keys.
// Containers push BEFORE invoking their block, so self is at the tail by container_finalize. `g_container_path_str` caches the "/"-joined form (push/pop-synced) so leaf finalize is one interpolation, not an O(depth) join.

var g_container_path : array<string>
var private g_container_path_str : string

def public container_path_push(widget_ident : string) {
    //! Append ``widget_ident`` to the open-container chain; emitted by the ``[container]`` macro prelude.
    g_container_path |> push(widget_ident)
    if (length(g_container_path) == 1) {
        g_container_path_str = widget_ident
    } else {
        g_container_path_str = "{g_container_path_str}/{widget_ident}"
    }
}

def public container_path_pop() {
    //! Pop the tail container ident off the open-container chain; idempotent on empty.
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
    //! Build a leaf widget's registry key: open container chain (``"/"``-joined) + ``widget_ident``; bare ident when unnested.
    return widget_ident if (empty(g_container_path_str))
    return "{g_container_path_str}/{widget_ident}"
}

def public container_path_key() : string {
    //! Registry/dispatcher key for the currently-open container's own entry.
    return g_container_path_str
}

def begin_frame() {
    //! Per-frame reset: bumps the frame counter and clears the per-frame registry, ``hex_id``\ →bbox map, and container path.
    //! Call before rendering widgets; L1 coroutines advance separately via :ref:`advance_coroutines <function-imgui_boost_runtime_advance_coroutines>`.
    g_frame++
    sample_memory()
    g_registry |> clear()
    g_id_to_bbox |> clear()
    g_rendered_this_frame |> clear()
    if (!empty(g_container_path)) {
        g_container_path |> clear()
        g_container_path_str = ""
    }
    // Defensive: a transformed canvas balances push/pop within a frame, but a missed pop
    // (e.g. a panic in the block) would otherwise leave the transform active for every
    // later frame and corrupt unrelated widget bboxes. Reset like the other per-frame state.
    if (!empty(g_item_xform_stack)) {
        g_item_xform_stack |> clear()
    }
}

// ===== Memory / GC-pressure tracking (opt-in) =====

// Sample the heap once per frame (from begin_frame), hold a baseline + running max, log periodically
// while enabled, emit a final delta line at teardown. Off by default — one bool test per frame when off.

var private g_debug_memory : bool = false
var private g_mem_baseline_set : bool = false
var private g_mem_heap_baseline : uint64 = 0ul
var private g_mem_str_baseline : uint64 = 0ul
var private g_mem_heap_max : uint64 = 0ul
var private g_mem_str_max : uint64 = 0ul
var private g_mem_frame_count : int = 0

//! Frames between periodic memory logs while ``imgui_debug_memory`` is on (0 disables the periodic line; the teardown report still fires). ~2s at 60fps.
var public imgui_mem_log_interval : int = 120

def public imgui_reset_memory_baseline() {
    //! Snapshot current heap + string-heap usage as the leak-tracking baseline. Call after
    //! one-time setup (window/fonts/first frame) so only steady-state growth is measured.
    g_mem_heap_baseline = heap_bytes_allocated()
    g_mem_str_baseline = string_heap_bytes_allocated()
    g_mem_heap_max = g_mem_heap_baseline
    g_mem_str_max = g_mem_str_baseline
    g_mem_frame_count = 0
    g_mem_baseline_set = true
}

def public imgui_debug_memory(enabled : bool) {
    //! Toggle per-frame heap / string-heap GC-pressure logging. Enabling (re)sets the baseline,
    //! so a steadily-growing heap surfaces as the per-frame ``+delta`` climbing instead of flat.
    g_debug_memory = enabled
    if (enabled) {
        imgui_reset_memory_baseline()
    }
}

def public imgui_memory_report() {
    //! Emit a final heap / string-heap delta-vs-baseline line, plus a WARNING if either grew past
    //! 64kb (a steady per-frame leak / GC-pressure smell). Called at teardown by with_imgui_app /
    //! with_recording_app; safe to call by hand. No-op until a baseline has been taken.
    return if (!g_mem_baseline_set)
    let heap_now = heap_bytes_allocated()
    let str_now = string_heap_bytes_allocated()
    let dheap = int64(heap_now) - int64(g_mem_heap_baseline)
    let dstr = int64(str_now) - int64(g_mem_str_baseline)
    to_log(LOG_INFO, "imgui mem: heap {heap_now/1024ul}kb (peak {g_mem_heap_max/1024ul}kb, delta {dheap}), str {str_now/1024ul}kb (peak {g_mem_str_max/1024ul}kb, delta {dstr})\n")
    if (dheap > 65536l || dstr > 65536l) {
        to_log(LOG_WARNING, "imgui mem: WARNING growth since baseline — heap +{dheap} str +{dstr} bytes (per-frame leak / GC pressure)\n")
    }
}

def private sample_memory() {
    return if (!g_debug_memory)
    if (!g_mem_baseline_set) {
        imgui_reset_memory_baseline()
    }
    let heap_cur = heap_bytes_allocated()
    let str_cur = string_heap_bytes_allocated()
    g_mem_heap_max = max(g_mem_heap_max, heap_cur)
    g_mem_str_max = max(g_mem_str_max, str_cur)
    g_mem_frame_count++
    if (imgui_mem_log_interval > 0 && g_mem_frame_count % imgui_mem_log_interval == 0) {
        let dheap = int64(heap_cur) - int64(g_mem_heap_baseline)
        let dstr = int64(str_cur) - int64(g_mem_str_baseline)
        to_log(LOG_INFO, "imgui mem [frame {g_frame}]: heap {heap_cur/1024ul}kb (max {g_mem_heap_max/1024ul}kb, +{dheap}), str {str_cur/1024ul}kb (max {g_mem_str_max/1024ul}kb, +{dstr})\n")
    }
}

var private g_end_of_frame_hooks : table<string; function<() : void>>

def public register_end_of_frame_hook(name : string; hook : function<() : void>) {
    //! Register a per-frame painter fired by :ref:`end_of_frame <function-imgui_boost_runtime_end_of_frame>`; re-registering the same ``name`` replaces, so live-reload doesn't multiply hooks.
    g_end_of_frame_hooks[name] = hook
}

def end_of_frame() {
    //! Per-frame painter drain — invokes every hook registered via :ref:`register_end_of_frame_hook <function-imgui_boost_runtime_register_end_of_frame_hook_string_function_ls__c_void_gr_>`. Call after rendering all widgets.
    for (h in values(g_end_of_frame_hooks)) {
        invoke(h)
    }
}

var private g_post_command_hooks : table<string; function<(command : string; target : string) : void>>

def public register_post_command_hook(name : string; hook : function<(command : string; target : string) : void>) {
    //! Register a hook fired after every successful ``imgui_*`` command with ``(command_name, resolved_target)``; same name-keyed idempotent semantics as :ref:`register_end_of_frame_hook <function-imgui_boost_runtime_register_end_of_frame_hook_string_function_ls__c_void_gr_>`.
    g_post_command_hooks[name] = hook
}

def private dispatch_post_command_hooks(command : string; target : string) {
    for (h in values(g_post_command_hooks)) {
        invoke(h, command, target)
    }
}

def public advance_coroutines() {
    //! Step every active L1 input coroutine one frame; finished coroutines drop out. ``harness_begin_frame`` calls this automatically — demo code should not call it directly. Exposed for the rare host that doesn't use the harness; in that case must run between ``ImGui_ImplGlfw_NewFrame()`` and ``NewFrame()`` so synthetic events aren't clobbered by the GLFW backend.
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
    //! Schedule an L1 input coroutine (one event per frame); :ref:`advance_coroutines <function-imgui_boost_runtime_advance_coroutines>` drains it.
    g_coroutines |> emplace(c)
}

// ===== Render helpers =====

// `widget_prelude` runs at the top of every macro-wrapped widget body (PushID for unique imgui
// scoping); `widget_finalize` runs at the bottom — captures per-frame fields, registers the lazy
// serializer + dispatcher on first render, and balances the PushID with PopID.

def public widget_prelude(widget_ident : string) {
    //! Top-of-body helper for ``[widget]`` macros: ``PushID(widget_ident)`` and apply any pending :ref:`imgui_focus <function-imgui_boost_runtime_imgui_focus_JsonValue_q_>` request.
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
    //! Tag this widget's path as accepting keyboard focus; called from interactive finalize helpers so :ref:`imgui_focus <function-imgui_boost_runtime_imgui_focus_JsonValue_q_>` accepts the target.
    g_focusable |> insert(widget_path_key(widget_ident))
}

def public pending_value_container_finalize(widget_ident : string; kind : string;
                                            var state : auto(ST)) {
    //! Container-side finalize for state structs with ``pending_value``/``has_pending``. Writes (addr(state), TypeInfo of ST) into ``g_widgets``; per-kind dispatcher routes ``set`` actions via ``[widget_dispatch]`` or a manual ``register_widget_kind`` entry.
    var entry : WidgetEntry
    unsafe {
        container_finalize(widget_ident, kind, entry,
                           addr(state),
                           reinterpret<TypeInfo const?>(typeinfo rtti_typeinfo(type<ST>)))
    }
}

def public pending_value_finalize(widget_ident : string; kind : string; var state : auto(ST)) {
    //! Generic finalize for any state struct with ``pending_value``/``has_pending`` (Drag/Slider/Input-numeric/Color/Combo/ListBox). Writes (addr(state), TypeInfo of ST) into ``g_widgets``; per-kind dispatcher is registered via ``[widget_dispatch]`` and routes the ``set`` action through ``sscan_json`` on the typed ``state.pending_value``.
    var entry : WidgetEntry
    unsafe {
        widget_finalize(widget_ident, kind, entry,
                        addr(state),
                        reinterpret<TypeInfo const?>(typeinfo rtti_typeinfo(type<ST>)))
    }
    register_focusable(widget_ident)
}

def private capture_sub_bboxes_into(var entry : WidgetEntry; components : int; item_width : float) {
    // Per-component sub-rects for a SliderScalarN widget (screen-mapped), splitting item_width like PushMultiItemsWidths — uses item_width not the combined GetItemRect width (the label sits past item_width).
    if (components <= 1) return
    let rmin = GetItemRectMin()
    let rmax = GetItemRectMax()
    let inner = GetStyle().ItemInnerSpacing.x
    let w_one = float(int((item_width - inner * float(components - 1)) / float(components)))   // IM_TRUNC, matches PushMultiItemsWidths
    var subs : array<float4>
    subs |> reserve(components)
    for (i in range(components)) {
        let x0 = rmin.x + float(i) * (w_one + inner)
        let wi = i < components - 1 ? w_one : item_width - (w_one + inner) * float(components - 1)
        let local = float4(x0, rmin.y, x0 + wi, rmax.y)
        subs |> push(empty(g_item_xform_stack) ? local : item_xform_screen(local))
    }
    entry.sub_bboxes <- subs
}

def public pending_value_finalize_vec(widget_ident : string; kind : string;
                                      var state : auto(ST); components : int; item_width : float) {
    //! Multi-component variant of ``pending_value_finalize`` for the vector/range numeric widgets
    //! (``slider_float2/3/4``, ``drag_float2/3/4``, the two-handle ``drag_*_range2``): same dispatch
    //! wiring plus per-component ``sub_bboxes`` so a driver targets one handle. ``item_width`` = ``CalcItemWidth()`` captured at the call site.
    var entry : WidgetEntry
    capture_sub_bboxes_into(entry, components, item_width)
    unsafe {
        widget_finalize(widget_ident, kind, entry,
                        addr(state),
                        reinterpret<TypeInfo const?>(typeinfo rtti_typeinfo(type<ST>)))
    }
    register_focusable(widget_ident)
}

def public pending_value_finalize_subs(widget_ident : string; kind : string;
                                       var state : auto(ST); var subs : array<float4>) {
    //! ``pending_value_finalize`` plus a caller-BUILT per-item ``sub_bboxes`` — for widgets whose items
    //! aren't separately registered and can't be derived analytically (combo popup items, captured
    //! one-per-Selectable via ``capture_current_item_rect``). Empty ``subs`` leaves ``sub_bboxes`` absent; ``subs`` is moved out.
    var entry : WidgetEntry
    if (!empty(subs)) {
        entry.sub_bboxes <- subs
    }
    unsafe {
        widget_finalize(widget_ident, kind, entry,
                        addr(state),
                        reinterpret<TypeInfo const?>(typeinfo rtti_typeinfo(type<ST>)))
    }
    register_focusable(widget_ident)
}

def public clear_module_widgets(module_name : string) {
    //! Drop every widget tagged with this module from the long-lived registries (g_widgets / id_to_path / focusable / pending_focus); called from each module's ``[init]`` so hot-reload re-binds cleanly.
    var paths_to_remove <- [for (k, m in keys(g_widgets), values(g_widgets)); k; where m.module_name == module_name]
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
    //! Idempotency probe used by indexed-form wrappers — true when ``path`` is already in ``g_widgets``.
    return key_exists(g_widgets, path)
}

def public register_widget(module_name : string;
                           bare_ident : string;
                           kind : string;
                           state_addr : void?;
                           ti : TypeInfo const?) {
    //! Module-init registration of a non-indexed widget into the long-lived ``g_widgets`` table. ``state_addr`` is the stable address of the module-scope state global (passed by the ``[widget]`` macro as ``unsafe(addr(IDENT))``); ``ti`` is its ``TypeInfo`` (``typeinfo rtti_typeinfo(IDENT)``). Both feed ``sprint_json_at`` at snapshot time — no per-widget getter/serializer functions emitted. For indexed widgets use ``register_widget_indexed`` / ``register_widget_str``.
    unsafe {
        g_widgets[bare_ident] <- WidgetMeta(
            module_name = module_name,
            kind = kind,
            state_addr = state_addr,
            ti = ti,
            last_seen_frame = -1
        )
    }
}

def public register_widget_indexed(module_name : string;
                                   bare_ident : string;
                                   kind : string;
                                   addr_getter : function<(k : int) : void?>;
                                   ti : TypeInfo const?;
                                   k : int) {
    //! Int-indexed sibling of ``register_widget``. ``addr_getter(k)`` re-resolves per-call against the user's ``table<int; …>`` (slots may be erased between frames); ``ti`` is the element's ``TypeInfo``. ``k`` is the table key, stored once at registration.
    unsafe {
        g_widgets[bare_ident] <- WidgetMeta(
            module_name = module_name,
            kind = kind,
            ti = ti,
            addr_getter_int = addr_getter,
            k_int = k,
            last_seen_frame = -1
        )
    }
}

def public register_widget_str(module_name : string;
                               bare_ident : string;
                               kind : string;
                               addr_getter : function<(k : string) : void?>;
                               ti : TypeInfo const?;
                               k : string) {
    //! String-keyed sibling of ``register_widget_indexed`` for ``table<string; …>``. Sets ``has_str_key`` so the lookup routes through ``addr_getter_str(k_str)``.
    unsafe {
        g_widgets[bare_ident] <- WidgetMeta(
            module_name = module_name,
            kind = kind,
            ti = ti,
            addr_getter_str = addr_getter,
            k_str := k,
            has_str_key = true,
            last_seen_frame = -1
        )
    }
}

def public lookup_state_addr(path : string) : void? {
    //! Resolve a widget's current state-struct address by path. For non-indexed widgets returns ``meta.state_addr`` directly; for indexed widgets invokes the registered ``addr_getter`` so the live table slot is re-resolved on every call (slots may be erased between frames). Returns null when ``path`` is absent from ``g_widgets`` or the getter reports the slot is gone.
    let meta = unsafe(g_widgets?[path])
    return null if (meta == null)
    return resolve_meta_addr(*meta)
}

def public state_jv(path : string; t : type<auto(T)>) : JsonValue? {
    //! Safe serializer-side helper for ``[widget]`` finalize lambdas — resolves the live state-struct address at ``path`` and returns ``JV(state)``. Returns ``null`` when the slot is gone (erased indexed key) so the snapshot reports the widget without a payload instead of dereferencing a stale capture. Pair with ``with_state`` on the dispatcher side; together they let custom-widget finalize helpers stay free of ``unsafe`` / ``reinterpret``.
    let raw = lookup_state_addr(path)
    return null if (raw == null)
    unsafe {
        return JV(*reinterpret<T?>(raw))
    }
}

def public with_state(path : string; t : type<auto(T)>;
                      blk : block<(var s : T -const&) : void>) {
    //! Safe dispatcher-side helper for ``[widget]`` finalize lambdas — resolves the live state-struct address at ``path`` and invokes ``blk`` with a mutable reference to the state. No-op when the slot is gone (erased indexed key). The block body looks like a plain ``state.field = ...`` mutation; the ``unsafe`` / ``reinterpret`` cast is hidden in this helper.
    let raw = lookup_state_addr(path)
    return if (raw == null)
    unsafe {
        var s & = *reinterpret<T -const?>(raw)
        invoke(blk, s)
    }
}

// Per-frame "this path rendered already" set — each path_key must finalize exactly once between
// begin_frame()/end_of_frame(). A collision means a single-global widget rendered twice (lexical dup
// or aliased call sites); it's loud, not a warning — telemetry needs an unambiguous path → widget map.
var private g_rendered_this_frame : table<string>

def private check_unique_render(path_key : string; kind : string) {
    // radio_button_int intentionally shares state across N per-frame calls; other kinds panic on duplicate render.
    return if (kind == "radio_button_int")
    if (key_exists(g_rendered_this_frame, path_key)) {
        let msg = "dasImgui: widget '{path_key}' ({kind}) rendered twice in frame {g_frame}. Single-global widgets must render exactly once per frame. For multiple instances, declare a table  variable private <NAME> : table<int; <STATE_TYPE>>  and call with subscript:  {kind}(<NAME>[i], (...))"
        panic(msg)
    }
    g_rendered_this_frame |> insert(path_key)
}

// ===== Snapshot item-transform stack (for transformed canvases, e.g. node-editor) =====

// A custom canvas (node-editor's ImGuiEx::Canvas) remaps ImGui's coords to canvas-local, so
// GetItemRectMin/Max returns canvas-local for interior widgets. To keep snapshots uniformly SCREEN-space,
// the canvas pushes an affine item-transform for its block; widget_finalize maps each rect to screen (bbox) and keeps the local rect (canvas_bbox). A STACK (canvases nest); each entry is the absolute local→screen affine, so push sets / pop restores with no composition. Empty stack = identity.
struct private ItemXform {
    scale  : float2
    offset : float2
}
var private g_item_xform_stack : array<ItemXform>

def public push_snapshot_item_transform(scale : float2; offset : float2) {
    //! Push a `screen = local * scale + offset` mapping applied to every widget's captured item rect
    //! until the matching `pop_snapshot_item_transform()`. A canvas (e.g. `node_editor`) pushes this so
    //! interior widgets report screen-space `bbox` plus a `canvas_bbox` with the local rect. Nestable.
    g_item_xform_stack |> push(ItemXform(scale = scale, offset = offset))
}

def public pop_snapshot_item_transform() {
    //! Pop the innermost item-transform pushed by `push_snapshot_item_transform`,
    //! restoring the enclosing canvas's mapping (or identity at the outermost level).
    if (!empty(g_item_xform_stack)) {
        g_item_xform_stack |> pop()
    }
}

def private item_xform_screen(local : float4) : float4 {
    // Map a captured local item rect to screen via the innermost (top-of-stack) affine.
    let top = g_item_xform_stack[length(g_item_xform_stack) - 1]
    return float4(
        local.x * top.scale.x + top.offset.x,
        local.y * top.scale.y + top.offset.y,
        local.z * top.scale.x + top.offset.x,
        local.w * top.scale.y + top.offset.y)
}

def public capture_item_bbox(var entry : WidgetEntry; local : float4) {
    //! Record a captured imgui item rect into `entry`: `bbox` screen-mapped via the active item-transform,
    //! plus `canvas_bbox` (local rect) when the transform moved it. Single capture chokepoint for leaf
    //! widgets and container headers. The (0,0,0,0) sentinel (headerless container) is left untouched — no phantom rect at the transform offset.
    if (local.x == 0.0f && local.y == 0.0f && local.z == 0.0f && local.w == 0.0f) {
        entry.bbox = local
        return
    }
    if (empty(g_item_xform_stack)) {
        entry.bbox = local
        return
    }
    let screen = item_xform_screen(local)
    entry.bbox = screen
    if (screen.x != local.x || screen.y != local.y ||
        screen.z != local.z || screen.w != local.w) {
        entry.canvas_bbox = local
    }
}

def public capture_current_item_rect() : float4 {
    //! The just-submitted ImGui item's screen-space rect [x0,y0,x1,y1], mapped through the active
    //! item-transform (identity in the common case). For popup/list items NOT separately registered
    //! (combo popup items): capture one rect per item after its Selectable to build sub_bboxes.
    let rmin = GetItemRectMin()
    let rmax = GetItemRectMax()
    let local = float4(rmin.x, rmin.y, rmax.x, rmax.y)
    return empty(g_item_xform_stack) ? local : item_xform_screen(local)
}

def public widget_finalize(widget_ident : string;
                           kind : string;
                           var entry : WidgetEntry;
                           state_addr : void?;
                           ti : TypeInfo const?) {
    //! Bottom-of-body helper for ``[widget]`` macros: capture bbox/hover/active/focus, write (state_addr, ti) into ``g_widgets[path_key]``, and ``PopID``.
    //! Snapshot reads via ``sprint_json_at(state_addr, *ti, …)``; HTTP dispatch resolves the handler from ``g_dispatchers`` keyed by ``(*ti).hash`` (the state struct's runtime type). No per-widget lambdas required.
    let path_key = widget_path_key(widget_ident)
    check_unique_render(path_key, kind)
    entry.kind = kind
    entry.hex_id = GetID(widget_ident)
    let rmin = GetItemRectMin()
    let rmax = GetItemRectMax()
    capture_item_bbox(entry, float4(rmin.x, rmin.y, rmax.x, rmax.y))
    entry.hover = IsItemHovered(ImGuiHoveredFlags.None)
    entry.active = IsItemActive()
    entry.focus = IsItemFocused()
    g_id_to_path[entry.hex_id] = path_key
    g_id_to_bbox[entry.hex_id] = entry.bbox
    // hex_id (GetID(widget_ident)) differs from ImGui's real active-item id for input widgets, so
    // focus_rect / snapshot active_widget couldn't resolve typing focus. When THIS widget is active,
    // also map GetActiveID() (authoritative) → path — gated on entry.active, additive (hex_id untouched).
    if (entry.active) {
        g_id_to_path[GetActiveID()] = path_key
    }
    g_registry[path_key] <- entry
    widgets_mark_seen(widget_ident, path_key, kind, state_addr, ti)
    PopID()
}

def public register_radio_site_alias(widget_ident : string; v_button : int;
                                     state_addr : void?; ti : TypeInfo const?) {
    //! Register a per-site geometry alias ``<path>#<v_button>`` for a grouped ``radio_button_int`` so
    //! each option is independently click/test-targetable though all sites share one ident (hence one
    //! ``<path>``) for state. Call right after the site's finalize; ``widgets_mark_seen(alias, alias, …)`` skips the bare-ident migration (equal args) and refreshes the meta.
    let alias = "{widget_path_key(widget_ident)}#{v_button}"
    var entry : WidgetEntry
    entry.kind = "radio_button_int"
    let rmin = GetItemRectMin()
    let rmax = GetItemRectMax()
    capture_item_bbox(entry, float4(rmin.x, rmin.y, rmax.x, rmax.y))
    g_registry[alias] <- entry
    widgets_mark_seen(alias, alias, "radio_button_int", state_addr, ti)
}

def public register_clickable_bbox(alias_ident : string; id_tag : string; kind : string) {
    //! Surface a draw-time clickable region (a raw Button/InvisibleButton a ``[container]`` draws itself)
    //! as child alias ``<container>/alias_ident``, so drivers can target a REAL synthetic click by path.
    //! Geometry-only, written into ``g_registry`` (rides the per-frame clear). Call inside the container body right after ``Button(id_tag, …)``: ``id_tag`` is the ImGui id (for GetID), ``alias_ident`` the snapshot path leaf.
    let alias = widget_path_key(alias_ident)
    check_unique_render(alias, kind)   // loud on a duplicated alias, like every finalize
    var entry = WidgetEntry(kind = kind, hex_id = GetID(id_tag))
    let rmin = GetItemRectMin()
    let rmax = GetItemRectMax()
    capture_item_bbox(entry, float4(rmin.x, rmin.y, rmax.x, rmax.y))
    entry.hover = IsItemHovered(ImGuiHoveredFlags.None)
    entry.active = IsItemActive()
    g_id_to_path[entry.hex_id] = alias
    g_id_to_bbox[entry.hex_id] = entry.bbox
    g_registry[alias] <- entry
}

def public register_clickable_state_bbox(alias_ident : string; id_tag : string; kind : string;
                                         state_addr : void?; ti : TypeInfo const?) {
    //! Stateful sibling of ``register_clickable_bbox`` for hand-drawn components.
    //! The geometry remains a real ImGui hit target while ``state_addr``/``ti``
    //! make the component's semantic state available in ``imgui_snapshot``.
    let alias = widget_path_key(alias_ident)
    check_unique_render(alias, kind)
    var entry = WidgetEntry(kind = kind, hex_id = GetID(id_tag))
    let rmin = GetItemRectMin()
    let rmax = GetItemRectMax()
    capture_item_bbox(entry, float4(rmin.x, rmin.y, rmax.x, rmax.y))
    entry.hover = IsItemHovered(ImGuiHoveredFlags.None)
    entry.active = IsItemActive()
    entry.focus = IsItemFocused()
    g_id_to_path[entry.hex_id] = alias
    g_id_to_bbox[entry.hex_id] = entry.bbox
    g_registry[alias] <- entry
    widgets_mark_seen(alias_ident, alias, kind, state_addr, ti)
}

def public register_handle_bbox(tag : string) {
    //! Surface a split/dock drag-handle's geometry as child alias ``<container>/HANDLE``, so drivers can
    //! target a REAL synthetic drag at the handle (not only ``imgui_force_set``-ing the fraction). Call
    //! inside the ``[container]`` body after the handle ``InvisibleButton(tag, …)``. Fixed ``HANDLE`` alias (one per split).
    register_clickable_bbox("HANDLE", tag, "splitter_handle")
}

def private widgets_mark_seen(bare_ident : string; path_key : string;
                              kind : string; state_addr : void?; ti : TypeInfo const?) {
    // First render: migrate the bare-ident entry to its container-aware path_key. Subsequent renders just bump last_seen_frame.
    if (bare_ident != path_key && key_exists(g_widgets, bare_ident)) {
        var moved <- g_widgets[bare_ident]
        g_widgets |> erase(bare_ident)
        unsafe {
            g_widgets[path_key] <- moved
        }
    }
    // Refresh (state_addr, ti) each frame; [edit_widget] widgets that skipped register_widget get their meta created lazily here, with the LAZY sentinel module_name so the freshness gate can expire them.
    if (state_addr != null && ti != null) {
        if (key_exists(g_widgets, path_key)) {
            unsafe {
                var meta & = g_widgets[path_key]
                meta.state_addr = state_addr
                meta.ti = ti
            }
        } else {
            unsafe {
                g_widgets[path_key] <- WidgetMeta(
                    module_name = LAZY_EDIT_WIDGET_MODULE,
                    kind = kind,
                    state_addr = state_addr,
                    ti = ti,
                    last_seen_frame = -1
                )
            }
        }
    }
    if (key_exists(g_widgets, path_key)) {
        g_widgets[path_key].last_seen_frame = g_frame
    }
}

def public container_finalize(widget_ident : string;
                              kind : string;
                              var entry : WidgetEntry;
                              state_addr : void?;
                              ti : TypeInfo const?) {
    //! Container counterpart of ``widget_finalize`` (sibling above); writes ``(state_addr, ti)`` into ``g_widgets`` at the cached container path key.
    let path_key = container_path_key()
    check_unique_render(path_key, kind)
    entry.kind = kind
    entry.hex_id = GetID(widget_ident)
    g_id_to_path[entry.hex_id] = path_key
    g_registry[path_key] <- entry
    widgets_mark_seen(widget_ident, path_key, kind, state_addr, ti)
    PopID()
}

// ===== L2/L3 dispatcher table — per-state-struct-type, addr-routed =====

// One top-level function per state struct, registered into ``g_dispatchers`` at module init via
// ``[widget_dispatch]``, keyed by the state's runtime ``TypeInfo.hash``. At dispatch, ``dispatch_or_err``
// resolves the target's ``(state_addr, ti)``, looks up ``g_dispatchers[(*ti).hash]``, and invokes it with ``(state_addr, action, payload)`` — the dispatcher casts state_addr to its struct type and runs ``sscan_json`` on the typed pending field. No per-widget capture frame or ``from_JV<T>`` instantiation.

typedef KindDispatcher = function<(state_addr : void?; action : string; payload : string) : void>

// Keyed by the state's runtime ``TypeInfo.hash`` — one dispatcher per state-struct type covers every
// widget kind using that struct (one ToggleState dispatcher serves checkbox/radio_button/selectable/menu_item).
// ClickState has none (``imgui_click`` is a real synth click, handled pre-lookup); non-Structure ti fall back to the generic ``sscan_json_at`` write-through in ``dispatch_or_err``.
var private g_dispatchers : table<uint64; KindDispatcher>

def public register_widget_dispatch_for_type(ti : TypeInfo const?; dispatcher : KindDispatcher) {
    //! Register a dispatcher for every widget whose state has this ``TypeInfo``. Emitted by the ``[widget_dispatch]`` annotation. The user-facing function name is irrelevant — what matters is the first parameter's struct type.
    return if (ti == null)
    g_dispatchers[(*ti).hash] = dispatcher
}

// ===== Phase 2 — Result wire format + per-command await =====

// Every [live_command] handler returns a Result<T, string> (JSON via the JV overload below). `with_await`
// auto-converts the payload and threads per-command await modifiers (await_frames/await_until/timeout_sec/
// fire_and_forget) into the response so transport-side await_imgui polls for quiescence. The server NEVER blocks — it runs on the GLFW main thread inside tick(), so a server-side wait would deadlock the frame loop that drives quiescence.

def public JV(r : $Result < auto(TT); auto(EE) >) : JsonValue? {
    //! Wire format for ``Result<T, E>``: ``{"ok": bool, "value"|"error": ...}``; the transport contract every ``imgui_*`` handler returns.
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
    @optional await_frames : int = -1            //! min frames to advance before considering the command settled; -1 = no frame floor
    @optional await_until : string = ""          //! quiescence predicate name (empty = default is_quiescent())
    @optional timeout_sec : float = 5.0f         //! client-side wait ceiling (seconds)
    @optional fire_and_forget : bool = false     //! when true, client skips the await loop entirely
}

def private with_await(input : JsonValue?; payload : auto(P)) : JsonValue? {
    let mods = from_JV(input?["await"], default<AwaitModifiers>)
    var jv = JV(payload)
    if (mods.fire_and_forget) return jv
    // Add the await modifiers directly onto the result object (was: copy every
    // field into a fresh table and return that, which orphaned `jv` every call).
    if (jv != null && jv.value is _object) {
        if (mods.await_frames >= 0) {
            jv |> update("await_frames", JV(mods.await_frames))
        }
        if (!empty(mods.await_until)) {
            jv |> update("await_until", JV(mods.await_until))
        }
        jv |> update("timeout_sec", JV(mods.timeout_sec))
    }
    return jv
}

// ===== [live_command] handlers =====

def private extract_string_field(obj : JsonValue?; name : string) : string {
    let v = obj?[name]
    if (v == null || !(v.value is _string)) return ""
    return v.value as _string
}

def private widget_meta_jv(_path_key : string; meta : WidgetMeta) : JsonValue? {
    // Snapshot blob for a widget registered but not rendered this frame: rendered=false, state payload only. State payload is built from (addr, ti) via sprint_json_at + read_json — no per-widget serializer function.
    return JV((kind = meta.kind, rendered = false, payload = state_payload_jv(resolve_meta_addr(meta), meta.ti)))
}

def private io_jv() : JsonValue? {
    let pos = GetMousePos()
    var tab : table<string; JsonValue?>   // nolint:STYLE021 - active_id/active_widget below are conditional keys; JV((...)) can't express conditional fields
    tab |> insert("mouse_pos", JV(float2(pos.x, pos.y)))
    tab |> insert("mouse_buttons", JV(IsMouseDown(ImGuiMouseButton.Left),
                                      IsMouseDown(ImGuiMouseButton.Middle),
                                      IsMouseDown(ImGuiMouseButton.Right)))
    tab |> insert("key_ctrl",  JV(IsKeyDown(ImGuiKey.ImGuiMod_Ctrl)))
    tab |> insert("key_shift", JV(IsKeyDown(ImGuiKey.ImGuiMod_Shift)))
    tab |> insert("key_alt",   JV(IsKeyDown(ImGuiKey.ImGuiMod_Alt)))
    tab |> insert("key_super", JV(IsKeyDown(ImGuiKey.ImGuiMod_Super)))
    tab |> insert("config_macos_behaviors", JV(GetIO().ConfigMacOSXBehaviors))
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
def imgui_snapshot(_input : JsonValue?) : JsonValue? {
    //! Live-command: return ``{frame, globals, io}`` — every registered widget joined with its per-frame entry when rendered.
    var globals : table<string; JsonValue?>
    // Walk the long-lived registry first (covers unrendered widgets too). EXCEPT indexed-form entries
    // (path contains '['), which expire after one frame absent: a removed user-table key must vanish
    // from telemetry, else stale serializers fire on reused slots and return misleading values.
    for (k, meta in keys(g_widgets), values(g_widgets)) {
        if (key_exists(g_registry, k)) {
            globals |> insert(k, widget_entry_jv(k, g_registry[k]))
        } else {
            // Freshness gate for entries clear_module_widgets can't reach on @live reload: indexed slots
            // (path has '[', key may be erased) and lazy [edit_widget] entries (LAZY sentinel module_name).
            // Both self-expire after one frame absent so snapshot won't deref a state_addr dangling after a heap reset; pre-registered "" (main-file) entries are safe (clear_module_widgets("") covers them).
            let needs_freshness = find(k, "[") >= 0 || meta.module_name == LAZY_EDIT_WIDGET_MODULE
            continue if (needs_freshness && meta.last_seen_frame < g_frame - 1)
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
    // Map target → (level, resolved). 2 = L2 path (dispatcher rendered at least once), 1 = L1 hex_id (click-only via bbox synthesis), 0 = unresolvable.
    if (empty(target)) return (0, "")
    let n = length(target)
    let is_hex = n > 2 && slice(target, 0, 2) == "0x" || n > 2 && slice(target, 0, 2) == "0X"
    if (is_hex) {
        let hid = parse_hex_id(target)
        if (hid == 0u) return (0, target)
        if (key_exists(g_id_to_path, hid)) return (2, g_id_to_path[hid])
        return (1, target)
    }
    if (key_exists(g_widgets, target)) return (2, target)
    // Geometry-only clickable aliases (no dispatcher) → level 1, click/drag by bbox but resolved by name, so a wrap_tab_item header / split handle is targetable by path not just hex_id.
    if (key_exists(g_registry, target)) return (1, target)
    return (0, target)
}

// ===== Synthetic input core (boost-owned) =====

// Low-level synth mouse/keyboard + per-frame tick + real-input control. Like imgui_test_engine, synth
// input is persistent state (cursor pos + button mask + held keys) re-asserted as io.Add*Event every
// frame while owned. Timeline playback lives in imgui_live_core, plugging in via these hooks (boost never requires live). Real input is suppressed by detaching the GLFW backend callbacks at source.

var public synth_cursor_owned : bool = false   //! true once any synth IO fired; overlays render against the synth cursor
var private mouse_cursor_x : float = 0.0
var private mouse_cursor_y : float = 0.0
var public synth_buttons_mask : int = 0        //! held mouse-button bitmask (bit b => button b down); mirrors imgui_test_engine MouseButtonsValue
var public synth_click_button : int = -1       //! last synth-clicked button (visual_aids click flash)
var public synth_click_action : int = 0        //! 1 press / 0 release
var public synth_click_t : float = -1.0
// FIFO serialization for click coroutines: each click claims a ticket at dispatch (issue order) and
// waits until it's served, so back-to-back clicks drive the button strictly in issue order regardless
// of how the per-frame command drain batches them or the reverse-order coroutine advance resumes them.
var private g_click_ticket_next    : int = 0   // next ticket to hand out (bumped at dispatch, just before spawn)
var private g_click_ticket_serving : int = 0   // ticket currently allowed to drive the button

// Toggleable per-section debug logging: a section name maps to enabled, emit via ambient to_log gated
// on it (one set-lookup when off). Deliberately a tiny local gate, NOT a daslib/logger require — boost
// is foundational, so we don't pull logger+debugger into every dasImgui app's compile.
var private g_log_sections : table<string>
var private g_log_buf : array<string>   // drainable capped buffer of emitted lines (imgui_log_drain returns + clears)
def log_section(name : string; on : bool) {
    //! Enable/disable a debug log section. Sections are checked by log_section_on; gated emit sites
    //! cost one set-lookup when off, so heavy per-frame logging is free until a section is turned on.
    if (on) { g_log_sections |> insert(name) } else { g_log_sections |> erase(name) }
}
def log_section_on(name : string) : bool {
    //! True if debug log section ``name`` is enabled. Guard heavy per-frame emit: ``if (log_section_on("x")) clk_log(...)``.
    return key_exists(g_log_sections, name)
}
def private clk_log(section, line : string) {
    // Emit one line to the ambient log AND a capped drainable buffer (imgui_log_drain → JSON over transport/MCP, since a host's stdout only reaches a test on panic).
    return if (!log_section_on(section))
    let entry = "[{section}] {line}"
    to_log(LOG_INFO, "{entry}\n")
    if (length(g_log_buf) < 8000) { g_log_buf |> push(entry) }
}

var public synth_held_keys : table<int>        //! currently-held ImGuiKey values (visual_aids keycap HUD)
var public synth_last_keycode : int = 0        //! last ImGuiKey synth-pressed; 0 = none
var public synth_last_keycode_t : float = -1.0
var public synth_last_codepoint : uint = 0u
var public synth_last_codepoint_t : float = -1.0

var private g_user_control : bool = true
var private g_input_callbacks_installed : bool = true

// imgui_live_core registers its timeline advance / reset here.
var private g_synth_frame_hooks : table<string; function<() : void>>
var private g_synth_stop_hooks  : table<string; function<() : void>>

def public register_synth_frame_hook(name : string; hook : function<() : void>) {
    //! Register a per-frame synth callback invoked by :ref:`imgui_synth_tick <function-imgui_boost_runtime_imgui_synth_tick>` (imgui_live_core's timeline playback uses this).
    g_synth_frame_hooks[name] = hook
}

def public register_synth_stop_hook(name : string; hook : function<() : void>) {
    //! Register a callback invoked when control is handed back to the user (``set_user_control(true)``) — stop in-flight playback.
    g_synth_stop_hooks[name] = hook
}

// The windowed GLFW backend (imgui_live.das, the daslang side of native imgui_app)
// fills this so set_user_control can detach real input AT THE SOURCE. Null on a
// non-GLFW host — user control then governs only synth cursor ownership.
var private g_real_input_toggle : function<(enabled : bool) : void>

def public register_real_input_toggle(hook : function<(enabled : bool) : void>) {
    //! Register the backend's real-input enable/disable callback. :ref:`imgui_synth_tick <function-imgui_boost_runtime_imgui_synth_tick>` invokes it on a ``set_user_control`` transition (the GLFW backend wires ``imgui_set_real_input_callbacks``).
    g_real_input_toggle = hook
}

def public get_synth_cursor() : tuple<bool; float; float> {
    //! Current synth cursor as ``(owned, x, y)``; ``owned`` means overlays should render against this instead of ``GetMousePos()``.
    return (synth_cursor_owned, mouse_cursor_x, mouse_cursor_y)
}

def public get_synth_keys() : tuple<bool; uint; float> {
    //! Synth-key state as ``(any_held, last_codepoint, last_codepoint_t)``.
    return (!empty(synth_held_keys), synth_last_codepoint, synth_last_codepoint_t)
}

// --- mouse primitives ---

def public synth_mouse_move(x, y : float) {
    //! Teleport the synth cursor to ``(x, y)`` and take ownership (the real cursor can't move it until ``set_user_control(true)`` hands back).
    mouse_cursor_x = x
    mouse_cursor_y = y
    synth_cursor_owned = true
    var io & = unsafe(GetIO())
    io |> AddMousePosEvent(x, y)
}

def public synth_mouse_button(button : int; action : int) {
    //! Press (``action==1``) / release synth mouse ``button``. Emits the cursor pos immediately before the button so ImGui records ``MouseClickedPos`` at the synth coords and ``IsItemHovered()`` passes.
    var io & = unsafe(GetIO())
    if (synth_cursor_owned) {
        io |> AddMousePosEvent(mouse_cursor_x, mouse_cursor_y)
    }
    io |> AddMouseButtonEvent(button, action == 1)
    if (action == 1) {
        synth_buttons_mask |= (1 << button)
        synth_click_t = get_uptime()   // latch the flash anchor on press; release must not reset it
    } else {
        synth_buttons_mask &= ~(1 << button)
    }
    synth_click_button = button
    synth_click_action = action
    synth_cursor_owned = true
}

def public synth_mouse_wheel(dx, dy : float) {
    //! Synthetic mouse wheel; takes cursor ownership.
    var io & = unsafe(GetIO())
    io |> AddMouseWheelEvent(dx, dy)
    synth_cursor_owned = true
}

def public release_held_buttons() {
    //! Release every currently-held synth mouse button.
    for (b in range(0, 5)) {
        if ((synth_buttons_mask & (1 << b)) != 0) {
            synth_mouse_button(b, 0)
        }
    }
}

// --- keyboard primitives ---

def private as_imgui_key(key : int) : ImGuiKey {
    // No int→enum ctor in daslang, so reinterpret int→ImGuiKey (same width; synth stores keys as int).
    return unsafe(reinterpret<ImGuiKey>(key))
}

def private mod_alias_for_side_key(key : int) : int {
    // ImGui's IsKeyDown(ImGuiMod_*) / Shortcut() consult the alias key, so emitting only the side key leaves it unset (mirrors imgui_test_engine).
    if (key == int(ImGuiKey.LeftCtrl)  || key == int(ImGuiKey.RightCtrl)) return int(ImGuiKey.ImGuiMod_Ctrl)
    if (key == int(ImGuiKey.LeftShift) || key == int(ImGuiKey.RightShift)) return int(ImGuiKey.ImGuiMod_Shift)
    if (key == int(ImGuiKey.LeftAlt)   || key == int(ImGuiKey.RightAlt)) return int(ImGuiKey.ImGuiMod_Alt)
    if (key == int(ImGuiKey.LeftSuper) || key == int(ImGuiKey.RightSuper)) return int(ImGuiKey.ImGuiMod_Super)
    return 0
}

def public synth_key_press(key : int) {
    //! Press synth ``key`` (ImGuiKey int); auto-pairs the ``ImGuiMod_*`` alias for side-mod keys so ``Shortcut()`` routing sees the chord.
    var io & = unsafe(GetIO())
    io |> AddKeyEvent(as_imgui_key(key), true)
    synth_held_keys |> insert(key)
    let mod_alias = mod_alias_for_side_key(key)
    if (mod_alias != 0) {
        io |> AddKeyEvent(as_imgui_key(mod_alias), true)
        synth_held_keys |> insert(mod_alias)
    }
    synth_last_keycode   = key
    synth_last_keycode_t = get_uptime()
}

def public synth_key_release(key : int) {
    //! Release synth ``key`` (and its mod alias).
    var io & = unsafe(GetIO())
    io |> AddKeyEvent(as_imgui_key(key), false)
    synth_held_keys |> erase(key)
    let mod_alias = mod_alias_for_side_key(key)
    if (mod_alias != 0) {
        io |> AddKeyEvent(as_imgui_key(mod_alias), false)
        synth_held_keys |> erase(mod_alias)
    }
}

def public synth_char(codepoint : uint) {
    //! Push a synthetic Unicode character to the focused widget.
    var io & = unsafe(GetIO())
    io |> AddInputCharacter(codepoint)
    synth_last_codepoint   = codepoint
    synth_last_codepoint_t = get_uptime()
}

def public release_held_keys() {
    //! Release every currently-held synth key.
    if (empty(synth_held_keys)) return
    var held : array<int>
    held |> reserve(length(synth_held_keys))
    for (k in keys(synth_held_keys)) {
        held |> push(k)
    }
    for (k in held) {
        synth_key_release(k)
    }
}

// --- user control (real-input detach) ---

def public user_control_enabled() : bool {
    //! False when ``set_user_control`` has disabled real input — :ref:`imgui_synth_tick <function-imgui_boost_runtime_imgui_synth_tick>` drops the backend input queue each frame so synth fully owns IO.
    return g_user_control
}

def public set_user_control_enabled(enabled : bool) {
    //! Toggle whether real mouse/keyboard reach the UI. The GLFW-callback detach/reattach happens on the next ``imgui_synth_tick`` (must run on the render thread), so this is safe to call off-thread.
    g_user_control = enabled
    if (enabled) {
        // Hand control back: stop in-flight synth playback (live's timelines, via
        // the stop-hooks), release our held inputs, drop ownership last (release_*
        // re-arm it, so clear after).
        for (h in values(g_synth_stop_hooks)) {
            invoke(h)
        }
        release_held_buttons()
        release_held_keys()
        synth_cursor_owned = false
    } else {
        // Take over: clear keys/buttons latched right now so their release events aren't dropped by the
        // per-frame queue clear (which would leave ImGui with stuck input). ClearInputKeys() +
        // ClearInputMouse() cover keys and mouse.
        var io & = unsafe(GetIO())
        io |> ClearInputKeys()
        io |> ClearInputMouse()
    }
}

[before_reload]
def reattach_real_input_before_reload() {
    // Live reload reuses the ImGui context but resets module globals to their `true` defaults. If real
    // input was detached at reload, the callbacks stay removed while the new program thinks they're
    // installed → real input permanently dead. Re-attach so the handoff matches post-reload defaults.
    if (!g_input_callbacks_installed) {
        if (g_real_input_toggle != null) {
            invoke(g_real_input_toggle, true)
        }
        g_input_callbacks_installed = true
    }
}

def public imgui_synth_tick() {
    //! Per-frame synth entry point — call between the backend's ``*_NewFrame()`` and ``ImGui::NewFrame()``. Detaches/reattaches real input on a user-control transition, advances registered timelines, and re-asserts held synth state (idempotent — ImGui dedupes same-state events). No-op when nothing is armed. ``harness_new_frame`` calls this for you.

    // Real-input suppression at the GLFW source: glfwSet*Callback must run on the render thread (which
    // this does). On a g_user_control transition, detach/reattach the backend's input callbacks so real
    // mouse/keyboard never reach ImGui's queue; synth uses io.Add*Event directly and is unaffected.
    if (g_user_control != g_input_callbacks_installed) {
        if (g_real_input_toggle != null) {
            invoke(g_real_input_toggle, g_user_control)
        }
        g_input_callbacks_installed = g_user_control
        if (!g_user_control) {
            // One-shot on take-over: drop real input queued before the detach took effect. Deliberately
            // NOT every frame — that would also wipe synth events emitted earlier this frame (e.g. the
            // click coroutine's release), leaving ImGui with a stuck button.
            var io & = unsafe(GetIO())
            io |> ClearEventsQueue()
            // Seize the cursor at its current position: ImGui_ImplGlfw_NewFrame keeps polling
            // glfwGetCursorPos every frame regardless of the detached callbacks, so unless we own the
            // cursor the gated re-assert is skipped until the first synth move and the real pos bleeds in.
            if (!synth_cursor_owned) {
                if (IsMousePosValid(null)) {
                    let cur = GetMousePos()
                    mouse_cursor_x = cur.x
                    mouse_cursor_y = cur.y
                }
                synth_cursor_owned = true
            }
        }
    }
    // Advance the live-side timeline playback (mouse/key), which calls the synth_* primitives above.
    for (h in values(g_synth_frame_hooks)) {
        invoke(h)
    }
    let synth_armed = synth_cursor_owned || !empty(synth_held_keys) || synth_buttons_mask != 0
    return if (!synth_armed)
    // Re-assert held state every frame (imgui_test_engine model). Idempotent.
    var io & = unsafe(GetIO())
    if (synth_cursor_owned) {
        io |> AddMousePosEvent(mouse_cursor_x, mouse_cursor_y)
    }
    for (b in range(0, 5)) {
        if ((synth_buttons_mask & (1 << b)) != 0) {
            io |> AddMouseButtonEvent(b, true)
        }
    }
    for (k in keys(synth_held_keys)) {
        io |> AddKeyEvent(as_imgui_key(k), true)
    }
}

// Scroll `target` into its top-level window's viewport before clicking — a real user scrolls a
// below-the-fold widget on-screen first (imgui_test_engine ScrollToItem). Queues the window's
// pending_scroll; returns the applied vertical delta (0 = visible / not a windowed path / no room).
def private scroll_target_into_view(target : string) : float {
    let slash = find(target, "/")
    if (slash < 0) return 0.0f                        // bare window / hex_id: first segment IS the window
    let win_name = slice(target, 0, slash)
    let meta = unsafe(g_widgets?[win_name])
    if (meta == null || meta.kind != "window" || meta.state_addr == null) return 0.0f
    let r = widget_rect(target)
    if (!r._0) return 0.0f
    let bb = r._1                                     // (x0=left, y0=top, x1=right, y1=bottom)
    var ws = unsafe(reinterpret<WindowState?>(meta.state_addr))
    if (ws == null) return 0.0f
    let visible_top = ws.pos.y + GetFrameHeight()     // content top, below the title bar
    let visible_bottom = ws.pos.y + ws.size.y
    let margin = GetTextLineHeight()
    var delta = 0.0f
    if (bb.w > visible_bottom) {                      // bottom edge below the fold → scroll down
        delta = (bb.w - visible_bottom) + margin
    } elif (bb.y < visible_top) {                     // top edge above the fold → scroll up
        delta = -((visible_top - bb.y) + margin)
    } else {
        return 0.0f                                   // already fully visible
    }
    let new_y = clamp(ws.scroll.y + delta, 0.0f, ws.scroll_max.y)
    let applied = new_y - ws.scroll.y
    if (abs(applied) < 1.0f) return 0.0f              // already at the needed scroll (e.g. clamped)
    ws.pending_scroll = float2(ws.scroll.x, new_y)
    ws.pending_scroll_set = true
    return applied
}

// click_at_coro: a real synthetic click via the owned synth primitives (imgui_test_engine model) —
// move → settle hover one frame → reset double-click timer → press → hold → release. The hover-settle
// frame is load-bearing: tab bars (TabItemButton) register the press only once HoveredId is set from a prior frame. Clicks FIFO-serialize by ticket (g_click_ticket_serving): without ordering the reverse-order coroutine advance runs batched clicks LIFO — firing a menu item before the right-click that opens it, or merging two presses into one.
[async]
def private click_at_coro(center : float2; scrolled : bool; button : int; ticket : int) : void {
    while (g_click_ticket_serving != ticket) {
        await_next_frame()
    }
    if (log_section_on("synth.click")) clk_log("synth.click", "SERVE ticket={ticket} frame={GetFrameCount()} from=({mouse_cursor_x},{mouse_cursor_y}) to=({center.x},{center.y}) scrolled={scrolled} active={GetActiveID()} hover={IsAnyItemHovered()}")
    if (scrolled) await_next_frame()
    // Animate the approach (no warp); the final step ends on `center` and its await doubles as the hover-settle frame (load-bearing for tab-bar press registration).
    let sx = mouse_cursor_x
    let sy = mouse_cursor_y
    let dx = center.x - sx
    let dy = center.y - sy
    // Constant cursor SPEED: ~11 px/step so a long hop takes proportionally more frames (not a warp); the cap is a sanity ceiling.
    let steps = max(int(max(abs(dx), abs(dy)) / 11.0f) + 1, 2) |> min(96)
    for (i in range(1, steps + 1)) {
        let f = float(i) / float(steps)
        synth_mouse_move(sx + dx * f, sy + dy * f)
        await_next_frame()
        if (log_section_on("synth.click.move")) clk_log("synth.click.move", "ticket={ticket} step={i}/{steps} frame={GetFrameCount()} pos=({sx + dx * f},{sy + dy * f}) active={GetActiveID()} hover={IsAnyItemHovered()}")
    }
    unsafe {
        var io & = GetIO()
        io.MouseClickedTime[button] = -3.40282347e+38lf  // -FLT_MAX: never auto-double-click
    }
    synth_mouse_button(button, 1)
    await_next_frame()
    if (log_section_on("synth.click")) clk_log("synth.click", "PRESS-HOLD ticket={ticket} button={button} frame={GetFrameCount()} pos=({center.x},{center.y}) active={GetActiveID()} hover={IsAnyItemHovered()}")
    synth_mouse_button(button, 0)
    g_click_ticket_serving++
    if (log_section_on("synth.click")) clk_log("synth.click", "RELEASE ticket={ticket} frame={GetFrameCount()} active={GetActiveID()} hover={IsAnyItemHovered()}")
}

// --- immediate synth live_commands (transport calls these; live adds the animated forms) ---

struct MousePosArgs {
    x : float = 0.0
    y : float = 0.0
}

[live_command(description="Teleport synthetic cursor (ImGui bypass). Args: x, y")]
def imgui_mouse_pos(input : JsonValue?) : JsonValue? {
    //! Live-command: teleport the synth cursor to ``(x, y)`` and take ownership.
    let args = from_JV(input, type<MousePosArgs>)
    synth_mouse_move(args.x, args.y)
    return JV((x = args.x, y = args.y))
}

struct MouseClickArgs {
    @optional button : int    = 0
    action           : string = "press"
}

[live_command(description="Synthetic mouse button (ImGui bypass). Args: button (0/1/2), action (press|release)")]
def imgui_mouse_click(input : JsonValue?) : JsonValue? {
    //! Live-command: press/release a synth mouse button at the current synth cursor.
    let args = from_JV(input, type<MouseClickArgs>)
    var act : int
    if (args.action == "press") {
        act = 1
    } elif (args.action == "release") {
        act = 0
    } else {
        return JV((error = "invalid action; expected \"press\" or \"release\"", action = args.action))
    }
    synth_mouse_button(args.button, act)
    return JV((button = args.button, action = args.action))
}

struct MouseScrollArgs {
    x : float = 0.0
    y : float = 0.0
}

[live_command(description="Synthetic mouse scroll (ImGui bypass). Args: x, y (offsets)")]
def imgui_mouse_scroll(input : JsonValue?) : JsonValue? {
    //! Live-command: synthetic mouse wheel.
    let args = from_JV(input, type<MouseScrollArgs>)
    synth_mouse_wheel(args.x, args.y)
    return JV((x = args.x, y = args.y))
}

struct KeyPressArgs {
    key : int = 0   // ImGuiKey value (cast to int over the wire)
}

[live_command(description="Synthetic key press (ImGui bypass). Args: key (ImGuiKey int)")]
def imgui_key_press(input : JsonValue?) : JsonValue? {
    //! Live-command: press a synth key (ImGuiKey int); auto-pairs the mod alias.
    let args = from_JV(input, type<KeyPressArgs>)
    return JV((error = "missing key")) if (args.key == 0)
    synth_key_press(args.key)
    return JV((key = args.key, action = "press"))
}

[live_command(description="Synthetic key release (ImGui bypass). Args: key (ImGuiKey int)")]
def imgui_key_release(input : JsonValue?) : JsonValue? {
    //! Live-command: release a synth key (ImGuiKey int).
    let args = from_JV(input, type<KeyPressArgs>)
    return JV((error = "missing key")) if (args.key == 0)
    synth_key_release(args.key)
    return JV((key = args.key, action = "release"))
}

struct KeyCharArgs {
    codepoint : uint = 0u
}

[live_command(description="Synthetic Unicode char (ImGui bypass). Args: codepoint")]
def imgui_key_char(input : JsonValue?) : JsonValue? {
    //! Live-command: push a synthetic Unicode character to the focused widget.
    let args = from_JV(input, type<KeyCharArgs>)
    return JV((error = "missing codepoint")) if (args.codepoint == 0u)
    synth_char(args.codepoint)
    return JV((codepoint = args.codepoint))
}

struct UserControlArgs {
    @optional enabled : bool = true
}

[live_command(description="Toggle real user input. When disabled, the synth tick detaches the GLFW backend's input callbacks (and drops queued input) so ONLY synth IO drives the UI — no real-input bleed racing the synth timeline. Re-enable to hand control back. Args: enabled (bool)")]
def set_user_control(input : JsonValue?) : JsonValue? {
    //! Live-command: enable/disable real mouse+keyboard reaching the UI.
    let args = from_JV(input, type<UserControlArgs>)
    set_user_control_enabled(args.enabled)
    return JV((user_control = g_user_control))
}

def private dispatch_or_err(action : string; input : JsonValue?) : Result<string, string> {
    let target = extract_string_field(input, "target")
    let (level, resolved) = classify_target(target)
    if (log_section_on("synth.click")) clk_log("synth.click", "ATTEMPT action='{action}' target='{target}' level={level} resolved='{resolved}' frame={GetFrameCount()}")
    if (level == 0) return err("unresolvable target: {target}", type<string>)
    // A click drives ImGui's own input path via click_at_coro (real behavior); resolve_target_bbox handles both L2 (path) and L1 (hex_id) targets.
    if (action == "click") {
        let button = input?["button"] ?? 0
        let (got, center) = resolve_target_bbox(resolved)
        if (log_section_on("synth.click")) clk_log("synth.click", "BBOX resolved='{resolved}' got={got} center=({center.x},{center.y}) frame={GetFrameCount()}")
        if (!got) return err("target {resolved} has no bbox this frame (not rendered)", type<string>)
        // Post-scroll center = current center shifted by the applied delta, computed HERE (registry is live mid-dispatch) because the coroutine can't read the freshly-cleared registry in begin_frame.
        let scroll_dy = scroll_target_into_view(resolved)
        let target_center = float2(center.x, center.y - scroll_dy)
        // Claim the FIFO ticket AFTER every early-return above — a ticket with no coroutine to serve it stalls g_click_ticket_serving and hangs all later clicks.
        let my_ticket = g_click_ticket_next
        g_click_ticket_next++
        if (log_section_on("synth.click")) clk_log("synth.click", "DISPATCH target='{target}' resolved='{resolved}' level={level} center=({center.x},{center.y}) scroll_dy={scroll_dy} target_center=({target_center.x},{target_center.y}) ticket={my_ticket} serving={g_click_ticket_serving} frame={GetFrameCount()} active={GetActiveID()} hover={IsAnyItemHovered()}")
        spawn_coroutine(click_at_coro(target_center, scroll_dy != 0.0f, button, my_ticket))
        // Return the clean resolved target (path or hex_id) — the auto-highlight hook resolves it via widget_rect and tests assert it.
        return ok(resolved, type<string>)
    }
    if (level == 2) {
        if (action == "focus") {
            if (!key_exists(g_focusable, resolved)) return err("target {resolved} is not focusable (display-only widget)", type<string>)
            g_pending_focus |> insert(resolved)
            return ok(resolved, type<string>)
        }
        let meta = unsafe(g_widgets?[resolved])
        if (meta == null) return err("no widget meta for {resolved}", type<string>)
        let state_addr = resolve_meta_addr(*meta)
        if (state_addr == null) return err("state slot gone for {resolved}", type<string>)
        return err("widget meta missing TypeInfo for {resolved} (kind {meta.kind})", type<string>) if (meta.ti == null)
        // payload = JSON of the action's "value" (typed pending field, or whole object for multi-field like set_window_pos); "" when there's no value field (open/close don't read it).
        let value = input?["value"]
        var payload : string
        if (value != null) {
            payload = write_json(value)
        }
        let ti_hash = (*meta.ti).hash
        let dispatcher = unsafe(g_dispatchers?[ti_hash])
        if (dispatcher != null) {
            invoke(*dispatcher, state_addr, action, payload)
            return ok(resolved, type<string>)
        }
        // No dispatcher: non-Structure ti ([edit_widget] scalar pointee) fall back to a generic write-through; structures with none are display-only (silently succeed).
        if ((*meta.ti).basicType != Type.tStructure && action == "set") {
            unsafe {
                sscan_json_at(payload, state_addr, *meta.ti)
            }
        }
        return ok(resolved, type<string>)
    }
    // L1 hex_id targets support clicks only (handled above); no rail for other actions.
    return err("L1 hex_id target {resolved} supports 'click' only, got '{action}'", type<string>)
}

[live_command(description = "Real synthetic mouse click on the widget identified by 'target' (optional 'button')")]
def imgui_click(input : JsonValue?) : JsonValue? {
    //! Live-command: a real synthetic mouse click on ``input["target"]`` (path or ``0x...`` hex_id;
    //! optional ``input["button"]`` — 0 left / 1 right / 2 middle). Presses/releases through ImGui's own
    //! input path, so the widget behaves exactly as under a real click. For an exact value, use ``imgui_force_set``.
    let r = dispatch_or_err("click", input)
    if (r._is_ok) dispatch_post_command_hooks("click", r._value)
    return with_await(input, r)
}

[live_command(description = "Force a widget's value directly, bypassing input ('target' name + 'value')")]
def imgui_force_set(input : JsonValue?) : JsonValue? {
    //! Live-command: force the next-frame value of ``input["target"]`` to ``input["value"]``. A BYPASS —
    //! writes state directly, doing what a user couldn't (exact value, off-screen or inactive widget).
    //! For a faithful interaction prefer ``imgui_click`` / ``imgui_key_type``; use this only when the value is the goal.
    let r = dispatch_or_err("set", input)
    if (r._is_ok) dispatch_post_command_hooks("set", r._value)
    return with_await(input, r)
}

[live_command(description = "Toggle a debug log section in the host ('name' + bool 'on') for toggleable per-frame instrumentation")]
def imgui_log_section(input : JsonValue?) : JsonValue? {
    //! Live-command: enable/disable host-side debug log section ``input["name"]`` per ``input["on"]``.
    //! Gated emit sites cost one set-lookup when off, so heavy per-frame logging is free until enabled.
    //! Runs in the host context, so it reaches dispatch/coroutine instrumentation a test's context can't.
    let name = extract_string_field(input, "name")
    let on = input?["on"] ?? false
    log_section(name, on)
    return JV((ok = true, name = name, on = on))
}

[live_command(description = "Return + clear the host's buffered debug-log lines (from enabled log sections)")]
def imgui_log_drain(_input : JsonValue?) : JsonValue? {
    //! Live-command: return the host-side instrumentation lines accumulated since the last drain
    //! (a JSON array of strings) and clear the buffer. The transport-readable companion to the
    //! stdout emit — a test or MCP client reads the dispatch/coroutine trace from the response.
    var result = JV(g_log_buf)
    g_log_buf |> clear()
    return result
}

[live_command(description = "Queue an open command on a container widget ('target' name)")]
def imgui_open(input : JsonValue?) : JsonValue? {
    //! Live-command: open the container named by ``input["target"]`` (window/popup/tree_node/collapsing_header/tab_item).
    let r = dispatch_or_err("open", input)
    if (r._is_ok) dispatch_post_command_hooks("open", r._value)
    return with_await(input, r)
}

[live_command(description = "Queue a close command on a container widget ('target' name)")]
def imgui_close(input : JsonValue?) : JsonValue? {
    //! Live-command: close the container named by ``input["target"]`` (window/popup/tree_node/collapsing_header/tab_item).
    let r = dispatch_or_err("close", input)
    if (r._is_ok) dispatch_post_command_hooks("close", r._value)
    return with_await(input, r)
}

[live_command(description = "Select (activate) a tab_item in a tab_bar ('target' name)")]
def imgui_select(input : JsonValue?) : JsonValue? {
    //! Live-command: activate the ``tab_item`` named by ``input["target"]`` — the automation counterpart
    //! of clicking a tab (ImGui owns active-tab selection, so an inactive tab can't be reached by
    //! ``imgui_click``). Routes through ImGuiTabItemFlags.SetSelected next frame; sets ``state.selected = true``.
    let r = dispatch_or_err("select", input)
    if (r._is_ok) dispatch_post_command_hooks("select", r._value)
    return with_await(input, r)
}

[live_command(description = "Force keyboard focus to a widget identified by 'target' (path or hex_id)")]
def imgui_focus(input : JsonValue?) : JsonValue? {
    //! Live-command: force keyboard focus onto ``input["target"]``; the widget's path must have been tagged via :ref:`register_focusable <function-imgui_boost_runtime_register_focusable_string>`.
    let r = dispatch_or_err("focus", input)
    if (r._is_ok) dispatch_post_command_hooks("focus", r._value)
    return with_await(input, r)
}

[live_command(description = "Re-dock a dock_window to a specific dock node ('target' name, 'value' = dock_id uint)")]
def imgui_dock(input : JsonValue?) : JsonValue? {
    //! Live-command: re-dock the dock_window named by ``input["target"]`` into the dock node id supplied in ``input["value"]`` (a ``uint`` from a ``DockBuilder*`` call). Routes through SetNextWindowDockID on the next frame.
    let r = dispatch_or_err("dock", input)
    if (r._is_ok) dispatch_post_command_hooks("dock", r._value)
    return with_await(input, r)
}

[live_command(description = "Pop a dock_window out of its current dock node ('target' name)")]
def imgui_undock(input : JsonValue?) : JsonValue? {
    //! Live-command: undock the dock_window named by ``input["target"]``; the window becomes free-floating. Routes through SetNextWindowDockID(0) on the next frame.
    let r = dispatch_or_err("undock", input)
    if (r._is_ok) dispatch_post_command_hooks("undock", r._value)
    return with_await(input, r)
}

[live_command(description = "Raise a dock_window to the front of its dock node — selects its tab when it's stacked with others ('target' name)")]
def imgui_raise(input : JsonValue?) : JsonValue? {
    //! Live-command: bring the dock_window named by ``input["target"]`` to the front of its dock node — when several windows are docked into one node (so they render as a tab strip), this selects that window's tab. Routes through SetNextWindowFocus() on the next frame. The automation counterpart of clicking a tab; ``imgui_click`` can't reach a dock node's internal tab bar.
    let r = dispatch_or_err("raise", input)
    if (r._is_ok) dispatch_post_command_hooks("raise", r._value)
    return with_await(input, r)
}

[live_command(description = "Clear a dockspace's DockBuilder layout so the user's setup runs again ('target' = dockspace name)")]
def imgui_dock_reset(input : JsonValue?) : JsonValue? {
    //! Live-command: reset the dockspace named by ``input["target"]`` — clears the DockBuilder node tree and flips ``state.has_initial_layout`` back to false so the user's setup function re-runs on the next frame.
    let r = dispatch_or_err("reset", input)
    if (r._is_ok) dispatch_post_command_hooks("reset", r._value)
    return with_await(input, r)
}

[live_command(description = "Reposition a dock_window ('target' name, 'value' = JSON with x/y and optional w/h)")]
def imgui_set_window_pos(input : JsonValue?) : JsonValue? {
    //! Live-command: reposition the dock_window named by ``input["target"]``. ``value`` is a JSON object with required ``x``/``y`` and optional ``w``/``h``. Routes through SetNextWindowPos / SetNextWindowSize on the next frame; useful for placing a window after ``imgui_undock`` (the floating-window default location is implementation-defined) or for scripted layouts.
    let r = dispatch_or_err("set_window_pos", input)
    if (r._is_ok) dispatch_post_command_hooks("set_window_pos", r._value)
    return with_await(input, r)
}

[live_command(description = "Resize a window ('target' name, 'value' = JSON with w/h)")]
def imgui_set_window_size(input : JsonValue?) : JsonValue? {
    //! Live-command: resize the window named by ``input["target"]`` to (``w``, ``h``). Routes through SetNextWindowSize on the next frame via the regular-window dispatcher's ``pending_size`` rail. Used by the ``window_size_constraints`` recording driver to demonstrate the SetNextWindowSizeConstraints callbacks (snap/aspect/quantize) without needing a real border-drag synth.
    let r = dispatch_or_err("set_window_size", input)
    if (r._is_ok) dispatch_post_command_hooks("set_window_size", r._value)
    return with_await(input, r)
}

// ===== Phase 2.5 — Quiescence probe =====

// imgui_await is a polling probe: the server reports liveness ({quiescent, frame, pending_coroutines,
// active_id}) and the client drives the wait loop. Server-side blocking is incompatible with the
// GLFW-main-thread invariant — the dispatch handler runs on the same thread that advances frames.

struct AwaitArgs {
    @optional until : string = "quiescent"       //! quiescence predicate name to poll
    @optional frame : int = -1                   //! when >=0, return quiescent once g_frame >= this value
}

struct AwaitResult {
    quiescent : bool = false                     //! true when the requested condition currently holds
    frame : int = 0                              //! server's current g_frame
    pending_coroutines : int = 0                 //! length(g_coroutines) — in-flight L1 input drivers
    active_id : uint = 0u                        //! GetActiveID() — non-zero when a widget is being interacted with
}

// AwaitResult is a plain-fields struct — JV(struct) auto-walker handles
// the {quiescent, frame, pending_coroutines, active_id} shape for free.

def private is_quiescent() : bool {
    return (!IsAnyItemActive() &&
            !IsAnyMouseDown() &&
            GetActiveID() == 0u &&
            empty(g_coroutines))
}

[live_command(description = "Probe quiescence; returns AwaitResult with frame, pending_coroutines, active_id")]
def imgui_await(input : JsonValue?) : JsonValue? {
    //! Live-command: poll quiescence. Returns ``{quiescent, frame, pending_coroutines, active_id}`` — clients drive the wait loop client-side (the server runs on the GLFW thread).
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

def private parse_hex_id(s : string) : uint {
    // Parse "0x..." / "0X..." → uint; 0u on failure (matches ImGui's "no widget" sentinel).
    let n = length(s)
    if (n <= 2) return 0u
    let prefix = slice(s, 0, 2)
    if (prefix != "0x" && prefix != "0X") return 0u
    return uint(to_int64(slice(s, 2), true))
}

def public widget_rect(target : string) : tuple<bool; float4> {
    //! Resolve a target string (path or ``0x...`` hex_id) to its last-known bbox ``(x0,y0,x1,y1)``; returns ``(false, float4(0))`` on miss.
    if (key_exists(g_registry, target)) return (true, g_registry[target].bbox)
    let hid = parse_hex_id(target)
    if (hid != 0u && key_exists(g_id_to_bbox, hid)) return (true, g_id_to_bbox[hid])
    return (false, float4(0.0f, 0.0f, 0.0f, 0.0f))
}

def public active_widget_path() : string {
    //! Path of the currently active/focused boost-wrapped widget, or ``""`` when none.
    let aid = GetActiveID()
    return "" if (aid == 0u)
    return id_to_path(aid)
}

def private is_left_glyph_kind(kind : string) : bool {
    // Toggle-family widgets render a left glyph (check/radio), so their click should land on it, not the label-inflated row center; selectable/menu_item have no glyph and keep center-targeting.
    return kind == "checkbox" || kind == "radio_button" || kind == "radio_button_int"
}

def private resolve_target_bbox(target : string) : tuple<bool; float2> {
    let r = widget_rect(target)
    if (!r._0) return (false, float2(0.0f, 0.0f))
    let bb = r._1
    let cy = (bb.y + bb.w) * 0.5f
    // Aim at the glyph for the toggle family — ImGui's left frame-height square, so its center is half a row-height in from the left; the bbox stays the full row.
    if (key_exists(g_registry, target) && is_left_glyph_kind(g_registry[target].kind)) {
        let h = bb.w - bb.y
        return (true, float2(bb.x + h * 0.5f, cy))
    }
    return (true, float2((bb.x + bb.z) * 0.5f, cy))
}
