options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public
require math

//! Containers and layout helpers: path nesting and geometry.

let CONTAINERS_LAYOUT_FEATURE = "modules/dasImgui/examples/features/containers_layout.das"

[test(description = "tab_bar, tab_item, tree_node, collapsing_header, menu_bar and menu register under nested path keys, force_set reaches a five-deep leaf, and imgui_close / imgui_open hide and re-show a tree node's leaves.", timeout = 1200)]
def test_containers_layout_smoke(t : T?) {
    with_imgui_app(CONTAINERS_LAYOUT_FEATURE) $(d) {
        // Wait for the deepest leaf to register — proves the entire nested
        // path was walked during the first frame.
        var snap0 = wait_for_widget(d, "LAYOUT_WIN/MAIN_TABS/SETTINGS_TAB/PHYSICS_TREE/GRAVITY", 15.0f)
        t |> success(snap0 != null,
                     "deeply-nested GRAVITY leaf registered under 5-level path")
        if (snap0 == null) return

        // ----- Each container registers under its own path key -----
        t |> equal(find_widget(snap0, "LAYOUT_WIN")?["kind"] ?? "",
                   "window", "LAYOUT_WIN.kind == window")
        t |> equal(find_widget(snap0, "LAYOUT_WIN/MAIN_TABS")?["kind"] ?? "",
                   "tab_bar", "tab_bar registers under window's path")
        t |> equal(find_widget(snap0, "LAYOUT_WIN/MAIN_TABS/SETTINGS_TAB")?["kind"] ?? "",
                   "tab_item", "tab_item registers under tab_bar's path")
        t |> equal(find_widget(snap0, "LAYOUT_WIN/MAIN_TABS/SETTINGS_TAB/PHYSICS_TREE")?["kind"] ?? "",
                   "tree_node", "tree_node registers under tab_item's path")

        // ----- Sibling tab is also registered (tab_bar paints all tab_item
        //       headers regardless of which one is currently active) -----
        t |> success(widget_exists(snap0, "LAYOUT_WIN/MAIN_TABS/AUDIO_TAB"),
                     "sibling AUDIO_TAB also registered alongside SETTINGS_TAB")

        // ----- Menu bar + menus are part of the path too -----
        t |> equal(find_widget(snap0, "LAYOUT_WIN/MAIN_BAR")?["kind"] ?? "",
                   "menu_bar", "menu_bar registers")
        // FILE_MENU's body only renders when the user clicks open the menu,
        // so its content nesting (FILE_MENU/FILE_NEW etc.) is tested
        // implicitly only — the menu container itself registers every frame.
        t |> equal(find_widget(snap0, "LAYOUT_WIN/MAIN_BAR/FILE_MENU")?["kind"] ?? "",
                   "menu", "menu registers under menu_bar's path")

        // ----- imgui_force_set steers a leaf nested inside tree_node -----
        force_set_value(d, "LAYOUT_WIN/MAIN_TABS/SETTINGS_TAB/PHYSICS_TREE/GRAVITY", JV(12.5f))
        t |> success(wait_for_payload_value(d,
                        "LAYOUT_WIN/MAIN_TABS/SETTINGS_TAB/PHYSICS_TREE/GRAVITY",
                        "value", 12.5f, 300),
                     "imgui_force_set on 5-deep nested leaf works")

        // ----- collapsing_header path key -----
        t |> equal(find_widget(snap0, "LAYOUT_WIN/LOG_HEADER")?["kind"] ?? "",
                   "collapsing_header", "collapsing_header registers")
        // It's flagged DefaultOpen so opened=true is expected on first frame.
        t |> success(wait_for_bool_value(d, "LAYOUT_WIN/LOG_HEADER", "opened", true, 300),
                     "DefaultOpen header reports opened=true")

        // ----- imgui_close on a tree_node closes it via SetNextItemOpen -----
        // Assert by leaf-visibility (closed tree_node hides its leaves), not `opened==false`
        // (`@optional opened` omits the field when false); leaf disappearance is the real signal.
        close_widget(d, "LAYOUT_WIN/MAIN_TABS/SETTINGS_TAB/PHYSICS_TREE")
        let snap_closed = wait_until(d, 300) $(var snap) {
            return !widget_rendered(snap, "LAYOUT_WIN/MAIN_TABS/SETTINGS_TAB/PHYSICS_TREE/GRAVITY")
        }
        t |> success(snap_closed != null,
                     "imgui_close on tree_node hides its nested leaves next frame")

        // ----- imgui_open re-opens it -----
        open_widget(d, "LAYOUT_WIN/MAIN_TABS/SETTINGS_TAB/PHYSICS_TREE")
        t |> success(wait_for_render(d,
                        "LAYOUT_WIN/MAIN_TABS/SETTINGS_TAB/PHYSICS_TREE/GRAVITY",
                        5.0f) != null,
                     "imgui_open on tree_node re-renders its leaves")
    }
}

let OVERLAY_FEATURE = "modules/dasImgui/examples/features/containers_overlay.das"

[test(description = "combo_select and list_box register and take force_set indices, and imgui_open / imgui_close show and hide a popup's and a modal's bodies.", timeout = 1200)]
def test_containers_overlay_smoke(t : T?) {
    with_imgui_app(OVERLAY_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "OVERLAY_WIN/OPEN_POPUP_BTN", 15.0f)
        t |> success(snap0 != null, "OVERLAY_WIN/OPEN_POPUP_BTN registered")
        if (snap0 == null) return

        // ----- combo_select is registered under the window's path -----
        var combo_entry = find_widget(snap0, "OVERLAY_WIN/PALETTE_COMBO")
        t |> equal(combo_entry?["kind"] ?? "", "combo_select",
                   "combo_select registers under nested path")
        var listbox_entry = find_widget(snap0, "OVERLAY_WIN/TASK_LIST")
        t |> equal(listbox_entry?["kind"] ?? "", "list_box",
                   "list_box registers under nested path")

        // ----- imgui_force_set on combo_select index -----
        force_set_value(d, "OVERLAY_WIN/PALETTE_COMBO", JV(2))
        t |> success(wait_for_int_value(d, "OVERLAY_WIN/PALETTE_COMBO", "value", 2, 300),
                     "imgui_force_set steers PALETTE_COMBO to 2 (Forest)")

        // ----- imgui_force_set on list_box index -----
        force_set_value(d, "OVERLAY_WIN/TASK_LIST", JV(3))
        t |> success(wait_for_int_value(d, "OVERLAY_WIN/TASK_LIST", "value", 3, 300),
                     "imgui_force_set steers TASK_LIST to 3 (Package)")

        // ----- imgui_open on a popup flips state.open via the dispatcher -----
        // OPTIONS_POPUP starts closed; imgui_open routes through pending_open, consumed next frame
        // by OpenPopup() — so the popup body runs and its leaf OPT_VSYNC appears in the snapshot.
        open_widget(d, "OVERLAY_WIN/OPTIONS_POPUP")
        let snap_popup_open = wait_until(d, 300) $(var snap) {
            return widget_rendered(snap, "OVERLAY_WIN/OPTIONS_POPUP/OPT_VSYNC")
        }
        t |> success(snap_popup_open != null,
                     "imgui_open OPTIONS_POPUP renders popup body (OPT_VSYNC visible)")

        // ----- imgui_close on the open popup closes it -----
        close_widget(d, "OVERLAY_WIN/OPTIONS_POPUP")
        let snap_popup_closed = wait_until(d, 300) $(var snap) {
            return !widget_rendered(snap, "OVERLAY_WIN/OPTIONS_POPUP/OPT_VSYNC")
        }
        t |> success(snap_popup_closed != null,
                     "imgui_close OPTIONS_POPUP hides popup body next frame")

        // ----- imgui_open on popup_modal -----
        open_widget(d, "OVERLAY_WIN/CONFIRM_MODAL")
        let snap_modal = wait_until(d, 300) $(var snap) {
            return widget_rendered(snap, "OVERLAY_WIN/CONFIRM_MODAL/CONFIRM_YES")
        }
        t |> success(snap_modal != null,
                     "imgui_open CONFIRM_MODAL renders modal body")

        // Modal stays up until pending_close fires; close it via imgui_close.
        close_widget(d, "OVERLAY_WIN/CONFIRM_MODAL")
        let snap_modal_closed = wait_until(d, 300) $(var snap) {
            return !widget_rendered(snap, "OVERLAY_WIN/CONFIRM_MODAL/CONFIRM_YES")
        }
        t |> success(snap_modal_closed != null,
                     "imgui_close CONFIRM_MODAL closes the modal")
    }
}

let WINDOW_FEATURE = "modules/dasImgui/examples/features/containers_window.das"

[test(description = "Leaves inside window, child and group containers register only under hierarchical path keys, click and force_set dispatch through those keys, and imgui_open / imgui_close paint and hide a closable window's leaves.", timeout = 1200)]
def test_containers_window_smoke(t : T?) {
    with_imgui_app(WINDOW_FEATURE) $(d) {
        // The first leaf inside MAIN_WIN registers under the hierarchical
        // path "MAIN_WIN/SAVE" — that's the gate the harness should wait on.
        var snap0 = wait_for_widget(d, "MAIN_WIN/SAVE", 15.0f)
        t |> success(snap0 != null, "MAIN_WIN/SAVE registered after first frame")
        if (snap0 == null) return

        // ----- Container itself appears in registry under path key -----
        var main_entry = find_widget(snap0, "MAIN_WIN")
        t |> equal(main_entry?["kind"] ?? "", "window",
                   "MAIN_WIN registers as kind=window")

        // ----- Leaves nested inside the container use hierarchical keys -----
        var save_entry = find_widget(snap0, "MAIN_WIN/SAVE")
        t |> equal(save_entry?["kind"] ?? "", "button",
                   "MAIN_WIN/SAVE registers as kind=button (leaf under container)")

        // Bare "SAVE" must NOT exist as a top-level key — the only entry is
        // the path-prefixed one.
        t |> success(!widget_exists(snap0, "SAVE"),
                     "bare SAVE not registered (path-keyed only)")

        // ----- Click via the path-keyed dispatcher -----
        click(d, "MAIN_WIN/SAVE")
        t |> success(wait_for_int_value(d, "MAIN_WIN/SAVE", "click_count", 1, 300),
                     "imgui_click MAIN_WIN/SAVE bumps click_count")

        // ----- imgui_force_set on a nested slider -----
        force_set_value(d, "MAIN_WIN/SPEED", JV(7.5f))
        t |> success(wait_for_payload_value(d, "MAIN_WIN/SPEED", "value", 7.5f, 300),
                     "imgui_force_set steers MAIN_WIN/SPEED to 7.5")

        // ----- Three-deep nesting: window > child > leaf -----
        var snap1 = snapshot(d)
        var wire_entry = find_widget(snap1, "MAIN_WIN/LEFT_PANEL/WIRE")
        t |> equal(wire_entry?["kind"] ?? "", "checkbox",
                   "MAIN_WIN/LEFT_PANEL/WIRE registers under 3-deep path")
        force_set_value(d, "MAIN_WIN/LEFT_PANEL/WIRE", JV(true))
        t |> success(wait_for_bool_value(d, "MAIN_WIN/LEFT_PANEL/WIRE", "value", true, 300),
                     "imgui_force_set MAIN_WIN/LEFT_PANEL/WIRE flips it true")

        // ----- group(RIGHT_GROUP) inside MAIN_WIN, then leaf inside it -----
        var power_entry = find_widget(snap1, "MAIN_WIN/RIGHT_GROUP/POWER")
        t |> equal(power_entry?["kind"] ?? "", "slider_int",
                   "MAIN_WIN/RIGHT_GROUP/POWER registers under nested group path")
        force_set_value(d, "MAIN_WIN/RIGHT_GROUP/POWER", JV(42))
        t |> success(wait_for_int_value(d, "MAIN_WIN/RIGHT_GROUP/POWER", "value", 42, 300),
                     "imgui_force_set on a leaf nested in window>group works")

        // ----- imgui_open on a closable window flips state.open -----
        // DETAIL_WIN starts closed (state.open zero-init); open it, expect its first leaf (PING)
        // to paint. Existence is registered at [init], so we test paint via widget_rendered.
        open_widget(d, "DETAIL_WIN")
        let snap_open = wait_until(d, 300) $(var snap) {
            return widget_rendered(snap, "DETAIL_WIN/PING")
        }
        t |> success(snap_open != null,
                     "imgui_open DETAIL_WIN flips state.open and renders nested leaves")

        // ----- imgui_close hides the window again -----
        // PING stays in g_widgets (rendered:false) but stops painting.
        close_widget(d, "DETAIL_WIN")
        let snap_closed = wait_until(d, 300) $(var snap) {
            return !widget_rendered(snap, "DETAIL_WIN/PING")
        }
        t |> success(snap_closed != null,
                     "imgui_close DETAIL_WIN stops nested leaves from painting next frame")
    }
}

let LAYOUT_HELPERS_FEATURE = "modules/dasImgui/examples/features/layout_helpers.das"

[test(description = "split_h, split_v and dock_left register their own kinds with leaves under their panes, place the panes in order inside the content region, and move the pane boundary on both force_set and a synth drag of the handle.", timeout = 1200)]
def test_layout_helpers(t : T?) { // nolint:STYLE038
    with_imgui_app(LAYOUT_HELPERS_FEATURE) $(d) {
        t |> run("smoke") <| @(t : T?) {
            // All four leaves register through their containers / splits / dock.
            var snap0 = wait_for_widget(d, "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN/LEFT_BTN", 15.0f)
            t |> success(snap0 != null,
                         "deeply-nested LEFT_BTN registers under window/main/split_v/split_h")
            if (snap0 == null) return

            t |> success(widget_exists(snap0, "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN/RIGHT_BTN"),
                         "RIGHT_BTN registers in split_h right pane")
            t |> success(widget_exists(snap0, "LAYOUT_WIN/SPLIT_VERT/BOTTOM_BTN"),
                         "BOTTOM_BTN registers in split_v bottom pane")
            t |> success(widget_exists(snap0, "LAYOUT_WIN/SIDEBAR/SIDEBAR_BTN"),
                         "SIDEBAR_BTN registers inside dock_left sidebar")

            // Layout widgets register as their own kinds.
            t |> equal(find_widget(snap0, "LAYOUT_WIN/SIDEBAR")?["kind"] ?? "",
                       "dock_left", "dock_left registers as 'dock_left' kind")
            t |> equal(find_widget(snap0, "LAYOUT_WIN/SPLIT_VERT")?["kind"] ?? "",
                       "split_v", "split_v registers as 'split_v' kind")
            t |> equal(find_widget(snap0, "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN")?["kind"] ?? "",
                       "split_h", "split_h registers as 'split_h' kind")
        }
        reset(d)

        t |> run("geometry") <| @(t : T?) {
            var snap0 = wait_for_widget(d, "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN/RIGHT_BTN", 15.0f)
            t |> success(snap0 != null, "snapshot reachable")
            if (snap0 == null) return

            // bbox is float4 serialized as {x,y,z,w} = (x0, y0, x1, y1).
            let lb = find_widget(snap0, "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN/LEFT_BTN")?["bbox"]
            let rb = find_widget(snap0, "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN/RIGHT_BTN")?["bbox"]
            let lb_x1 = lb?["z"] ?? 0.0f
            let rb_x0 = rb?["x"] ?? 0.0f
            t |> success(lb_x1 < rb_x0,
                         "split_h: LEFT_BTN.right ({lb_x1}) < RIGHT_BTN.left ({rb_x0})")

            // SIDEBAR_BTN must be to the left of the main pane buttons (dock_left).
            let sb = find_widget(snap0, "LAYOUT_WIN/SIDEBAR/SIDEBAR_BTN")?["bbox"]
            let sb_x1 = sb?["z"] ?? 0.0f
            let lb_x0 = lb?["x"] ?? 0.0f
            t |> success(sb_x1 < lb_x0,
                         "dock_left: SIDEBAR_BTN.right ({sb_x1}) < LEFT_BTN.left ({lb_x0})")

            // split_v: BOTTOM_BTN must be below split_h's row.
            let bb = find_widget(snap0, "LAYOUT_WIN/SPLIT_VERT/BOTTOM_BTN")?["bbox"]
            let bb_y0 = bb?["y"] ?? 0.0f
            let lb_y1 = lb?["w"] ?? 0.0f
            t |> success(bb_y0 > lb_y1,
                         "split_v: BOTTOM_BTN.top ({bb_y0}) > LEFT_BTN.bottom ({lb_y1})")

            // Fit (regression guard for the dock_left -> split_v line-height overflow): the bottom
            // pane must render INSIDE the content region. dock_left's handle spans full content
            // height, so its bbox bottom (bbox.w) is the region floor the bottom pane must stay above.
            let region_floor = find_widget(snap0, "LAYOUT_WIN/SIDEBAR/HANDLE")?["bbox"]?["w"] ?? 0.0f
            let bb_y1 = bb?["w"] ?? 0.0f
            t |> success(region_floor > 0.0f && bb_y1 <= region_floor + 1.0f,
                         "split_v fits: BOTTOM_BTN.bottom ({bb_y1}) within content floor ({region_floor})")
        }
        reset(d)

        t |> run("set_value") <| @(t : T?) {
            var snap0 = wait_for_widget(d, "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN/LEFT_BTN", 15.0f)
            if (snap0 == null) {
                t |> success(false, "snapshot reachable")
                return
            }

            // Baseline: state.value reflects init=0.3.
            let v0 = find_widget(snap0, "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN")?["payload"]?["value"] ?? 0.0f
            t |> success(abs(v0 - 0.3f) < 1.0e-4f,
                         "SPLIT_MAIN.value baseline ({v0}) matches init=0.3")

            // L3 drive: push split to 0.7, verify state converges next frame.
            force_set_value(d, "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN", JV(0.7f))
            let landed = wait_for_payload_value(d,
                            "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN",
                            "value", 0.7f, 300)
            t |> success(landed, "SPLIT_MAIN.value -> 0.7 after imgui_force_set")

            // Geometry follows: RIGHT_BTN sits at the right pane's start, so growing the left
            // fraction 0.3 → 0.7 shifts the pane boundary (and RIGHT_BTN.left) rightward. LEFT_BTN
            // tracks the left-aligned button, not the pane edge, so it barely moves.
            var snap_lt = snapshot(d)
            let rb_x0_orig = find_widget(snap0, "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN/RIGHT_BTN")?["bbox"]?["x"] ?? 0.0f
            let rb_x0_now = find_widget(snap_lt, "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN/RIGHT_BTN")?["bbox"]?["x"] ?? 0.0f
            t |> success(rb_x0_now > rb_x0_orig + 10.0f,
                         "L3 force_set_value: RIGHT_BTN.left shifted right ({rb_x0_orig} -> {rb_x0_now})")
        }
        reset(d)

        t |> run("drag") <| @(t : T?) {
            // The split handle now surfaces its geometry as a child alias
            // <split>/HANDLE, so a synth-mouse drag targets it directly — the
            // L1 coverage gap that used to force imgui_force_set.
            let HANDLE = "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN/HANDLE"
            var snap0 = wait_for_widget(d, HANDLE, 15.0f)
            t |> success(snap0 != null, "split_h HANDLE surfaces its bbox")
            if (snap0 == null) return

            t |> equal(find_widget(snap0, HANDLE)?["kind"] ?? "",
                       "splitter_handle", "HANDLE registers as 'splitter_handle' kind")

            // Non-degenerate vertical strip between the two panes.
            let hb = find_widget(snap0, HANDLE)?["bbox"]
            let hx0 = hb?["x"] ?? 0.0f
            let hx1 = hb?["z"] ?? 0.0f
            let hy0 = hb?["y"] ?? 0.0f
            let hy1 = hb?["w"] ?? 0.0f
            t |> success(hx1 > hx0 && hy1 > hy0 + 50.0f,
                         "HANDLE bbox is a tall vertical strip ([{hx0},{hx1}] x [{hy0},{hy1}])")

            let v0 = find_widget(snap0, "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN")?["payload"]?["value"] ?? 0.0f
            let rb0 = find_widget(snap0, "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN/RIGHT_BTN")?["bbox"]?["x"] ?? 0.0f

            // Real synth drag of the handle to the right. split_h width is
            // pixel-driven, so the left fraction must rise — no force_set.
            let resp = drag(d, HANDLE, 80.0f, 0.0f, 8)
            t |> equal(resp?["ok"] ?? false, true, "drag(HANDLE, +80) hit the handle")

            // Gate the drag play to completion before polling value/geometry,
            // so the snapshot below reads a fully-landed gesture.
            t |> success(wait_for_mouse_idle(d, 10.0f), "drag drains before value/geometry read")

            let moved = wait_until(d, 300) $(snap) => (find_widget(snap, "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN")?["payload"]?["value"] ?? 0.0f) > v0 + 0.02f
            t |> success(moved != null, "synth drag raised SPLIT_MAIN.value above baseline {v0}")

            var snap1 = snapshot(d)
            let rb1 = find_widget(snap1, "LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN/RIGHT_BTN")?["bbox"]?["x"] ?? 0.0f
            t |> success(rb1 > rb0 + 5.0f,
                         "synth drag shifted RIGHT_BTN.left right ({rb0} -> {rb1})")
        }
    }
}

let LAYOUT_PRIMITIVES_FEATURE = "modules/dasImgui/examples/features/layout_primitives.das"

[test(description = "same_line, spacing, dummy and new_line markers each register an entry under the window.", timeout = 1200)]
def test_layout_primitives_smoke(t : T?) {
    with_imgui_app(LAYOUT_PRIMITIVES_FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/LAY_LEAD", 15.0f)
        t |> success(snap != null, "LAY_LEAD renders")
        if (snap == null) return

        let markers = ["LAY_SAME_LINE", "LAY_SAME_LINE_2", "LAY_SPACING_ROW",
                       "LAY_DUMMY", "LAY_NEW_LINE"]
        for (m in markers) {
            let path = "MAIN_WIN/{m}"
            t |> success(widget_exists(snap, path), "marker {path} registered")
        }
    }
}

let ALIGN_TEXT_FEATURE = "modules/dasImgui/examples/features/align_text_to_frame_padding.das"

[test(description = "align_text_to_frame_padding registers its kind and shifts the next text down toward its framed sibling's centre, further than bare text on the row above sits.", timeout = 1200)]
def test_align_text_to_frame_padding_smoke(t : T?) {
    with_imgui_app(ALIGN_TEXT_FEATURE) $(d) {
        var snap = wait_for_widget(d, "MAIN_WIN/LABEL_A", 15.0f)
        t |> success(snap != null, "LABEL_A registers")
        if (snap == null) return

        let align = find_widget(snap, "MAIN_WIN/ALIGN_B")
        t |> equal(align?["kind"] ?? "", "align_text_to_frame_padding",
                   "ALIGN_B.kind == 'align_text_to_frame_padding'")

        for (ident in ["LEAD", "BTN_A", "LABEL_B", "BTN_B"]) {
            t |> success(widget_exists(snap, "MAIN_WIN/{ident}"),
                         "{ident} registers")
        }

        let lab_a_y = find_widget(snap, "MAIN_WIN/LABEL_A")?["bbox"]?["y"] ?? 0.0f
        let lab_b_y = find_widget(snap, "MAIN_WIN/LABEL_B")?["bbox"]?["y"] ?? 0.0f
        let btn_a_y = find_widget(snap, "MAIN_WIN/BTN_A")?["bbox"]?["y"] ?? 0.0f
        let btn_b_y = find_widget(snap, "MAIN_WIN/BTN_B")?["bbox"]?["y"] ?? 0.0f

        // Row B is on a later row than Row A.
        t |> success(lab_b_y > lab_a_y,
                     "LABEL_B.y ({lab_b_y}) > LABEL_A.y ({lab_a_y})")

        // Row A: bare text sits at the line top — label.y == button.y. Row B:
        // align_text_to_frame_padding shifts LABEL_B's baseline down by FramePadding.y to align
        // with BTN_B's centre (label.y > button.y), so B's shift delta exceeds A's.
        let row_a_shift = lab_a_y - btn_a_y
        let row_b_shift = lab_b_y - btn_b_y
        t |> success(row_b_shift > row_a_shift,
                     "Row B label shifted down ({row_b_shift}) > Row A ({row_a_shift})")
        t |> success(row_b_shift > 0.0f,
                     "Row B label is below button top (shift {row_b_shift} > 0)")
    }
}

let UNINDENT_FEATURE = "modules/dasImgui/examples/features/unindent.das"

[test(description = "Every indent and unindent marker registers an entry, and an asymmetric three-indent plus one-large-unindent frame renders without underflowing the indent stack.", timeout = 1200)]
def test_unindent_smoke(t : T?) {
    with_imgui_app(UNINDENT_FEATURE) $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/IND_1", 15.0f)
        t |> success(snap != null, "IND_1 renders")
        if (snap == null) return

        for (ident in ["IND_1", "IND_2", "UND_2", "UND_1",
                       "IND_A", "IND_B", "IND_C", "UND_BIG"]) {
            t |> success(widget_exists(snap, "MAIN_WIN/{ident}"),
                         "{ident} marker registered")
        }
    }
}

let COLS_FEATURE = "modules/dasImgui/examples/features/columns_demo.das"

[test(description = "columns() registers each column's children under the window path with no path leaf of its own, orders the columns left to right, and flows the next statement below them.", timeout = 1200)]
def test_columns(t : T?) {
    with_imgui_app(COLS_FEATURE) $(d) {
        t |> run("smoke") <| @(t : T?) {
            var snap0 = wait_for_widget(d, "COLS_WIN/C0_BTN", 15.0f)
            t |> success(snap0 != null, "C0_BTN registers under COLS_WIN")
            if (snap0 == null) return

            t |> success(widget_exists(snap0, "COLS_WIN/C1_BTN"),
                         "C1_BTN registers in column 1")
            t |> success(widget_exists(snap0, "COLS_WIN/C2_BTN"),
                         "C2_BTN registers in column 2")
            t |> success(widget_exists(snap0, "COLS_WIN/AFTER_BTN"),
                         "AFTER_BTN registers after the columns_close()")
        }
        reset(d)

        t |> run("geometry") <| @(t : T?) {
            var snap0 = wait_for_widget(d, "COLS_WIN/C2_BTN", 15.0f)
            t |> success(snap0 != null, "snapshot reachable")
            if (snap0 == null) return

            // bbox is float4 = {x, y, z, w} = (x0, y0, x1, y1).
            let c0 = find_widget(snap0, "COLS_WIN/C0_BTN")?["bbox"]
            let c1 = find_widget(snap0, "COLS_WIN/C1_BTN")?["bbox"]
            let c2 = find_widget(snap0, "COLS_WIN/C2_BTN")?["bbox"]
            let c0_x = c0?["x"] ?? 0.0f
            let c1_x = c1?["x"] ?? 0.0f
            let c2_x = c2?["x"] ?? 0.0f
            t |> success(c0_x < c1_x,
                         "columns ordered L->R: C0.left ({c0_x}) < C1.left ({c1_x})")
            t |> success(c1_x < c2_x,
                         "columns ordered L->R: C1.left ({c1_x}) < C2.left ({c2_x})")

            // AFTER_BTN flows below the columns block (not inside any column).
            let after = find_widget(snap0, "COLS_WIN/AFTER_BTN")?["bbox"]
            let after_y = after?["y"] ?? 0.0f
            let c0_y1 = c0?["w"] ?? 0.0f
            t |> success(after_y > c0_y1,
                         "AFTER_BTN.top ({after_y}) > C0_BTN.bottom ({c0_y1})")
        }
    }
}

let WITH_INDENT_FEATURE = "modules/dasImgui/examples/features/with_indent.das"

[test(description = "The with_indent demo, whose wrapper registers no widget of its own, renders its window.", timeout = 1200)]
def test_with_indent_smoke(t : T?) {
    with_imgui_app(WITH_INDENT_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "MAIN_WIN", 15.0f)
        t |> success(snap0 != null, "MAIN_WIN registers (demo renders)")
        if (snap0 == null) return
        t |> equal(find_widget(snap0, "MAIN_WIN")?["kind"] ?? "", "window",
                   "MAIN_WIN.kind == 'window'")
    }
}

let WITH_ITEM_WIDTH_FEATURE = "modules/dasImgui/examples/features/with_item_width.das"

[test(description = "A slider inside with_item_width(120) renders with a bbox width near 120 pixels.", timeout = 1200)]
def test_with_item_width_smoke(t : T?) {
    with_imgui_app(WITH_ITEM_WIDTH_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "MAIN_WIN/NARROW_W", 15.0f)
        t |> success(snap0 != null, "NARROW_W slider registers")
        if (snap0 == null) return

        // Slider bbox is float4 (x0, y0, x1, y1) serialized as JV with x/y/z/w.
        let bb = find_widget(snap0, "MAIN_WIN/NARROW_W")?["bbox"]
        let x0 = bb?["x"] ?? 0.0f
        let x1 = bb?["z"] ?? 0.0f
        let width = x1 - x0
        // ImGui adds ~label + framepadding; the slider's drawn rect lands close
        // to 120 but not exact. Allow [60, 200].
        t |> success(width > 60.0f && width < 200.0f,
                     "NARROW_W width ({width}) within with_item_width(120) range")
    }
}

let WITH_TEXT_WRAP_POS_FEATURE = "modules/dasImgui/examples/features/with_text_wrap_pos.das"

[test(description = "The with_text_wrap_pos demo, whose wrapper registers no widget of its own, renders its window.", timeout = 1200)]
def test_with_text_wrap_pos_smoke(t : T?) {
    with_imgui_app(WITH_TEXT_WRAP_POS_FEATURE) $(d) {
        var snap0 = wait_for_widget(d, "MAIN_WIN", 15.0f)
        t |> success(snap0 != null, "MAIN_WIN registers (demo renders)")
        if (snap0 == null) return
        t |> equal(find_widget(snap0, "MAIN_WIN")?["kind"] ?? "", "window",
                   "MAIN_WIN.kind == 'window'")
    }
}
