options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_boost_runtime
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! The live snapshot: what the registry reports and under which path,
//! who owns the payloads, bbox transforms, and the wait family's
//! timeout dump.

let TRIGGERS_FEATURE = "modules/dasImgui/examples/features/triggers.das"

[test(description = "a single imgui_snapshot roundtrip over POST /command returns the triggers app's registry with MAIN_WIN/SAVE_BTN in it", timeout = 1200)]
def test_one_snapshot(t : T?) {
    with_imgui_app(TRIGGERS_FEATURE) $(d) {
        let snap = wait_for_widget(d, "MAIN_WIN/SAVE_BTN", 15.0f)
        t |> success(snap != null, "snapshot returned non-null payload with MAIN_WIN/SAVE_BTN registered")
    }
}

def fixture_payload(_ctx : void?; _id : int) : JsonValue? => JV((text = "snapshot payload"))

[test(description = "imgui_snapshot moves each widget's serialized payload into the returned JSON, leaving the registry entry no pointer to it")]
def test_snapshot_transfers_payload_ownership(t : T?) {
    let context = CreateContext(null)
    g_registry["capture_fixture"] <- WidgetEntry(kind = "fixture", serialize = @@fixture_payload)
    var snapshot = imgui_snapshot(null)
    t |> success(g_registry["capture_fixture"].payload == null, "registry must not retain a pointer owned by returned JSON")
    t |> equal(snapshot?.globals?.capture_fixture?.payload?.text ?? "", "snapshot payload")
    delete_json(snapshot)
    clear(g_registry)
    DestroyContext(context)
}

def private near(a : float; b : float; tol : float) : bool {
    let d = a - b
    return d <= tol && d >= -tol
}

[test(description = "under a pushed snapshot item transform a button and a tab header report bbox = canvas_bbox * scale + offset, and widgets before the push and after the pop carry no canvas_bbox", timeout = 1200)]
def test_snapshot_transform(t : T?) {
    with_imgui_app("modules/dasImgui/examples/features/snapshot_transform.das") $(d) {
        var snap = wait_for_visible(d, "MAIN_WIN/XFORM_BTN", 15.0f)
        t |> success(snap != null, "snapshot returned with the transformed button registered")
        if (snap == null) return

        // ---- no active transform before: screen-native, no canvas_bbox ----
        t |> success(widget_exists(snap, "MAIN_WIN/PLAIN_BTN"), "plain button present")
        let plain_cbb = find_widget(snap, "MAIN_WIN/PLAIN_BTN")?["canvas_bbox"]?["z"] ?? -1.0f
        t |> success(plain_cbb < 0.0f, "plain button carries no canvas_bbox (no active transform)")

        // ---- under the transform: bbox = local*scale + offset, canvas_bbox = local ----
        let xf = find_widget(snap, "MAIN_WIN/XFORM_BTN")
        let lx = xf?["canvas_bbox"]?["x"] ?? -1.0f
        let ly = xf?["canvas_bbox"]?["y"] ?? -1.0f
        let lz = xf?["canvas_bbox"]?["z"] ?? -1.0f
        let lw = xf?["canvas_bbox"]?["w"] ?? -1.0f
        t |> success(lx >= 0.0f && lz > lx, "xform button carries a canvas_bbox (local rect)")
        let bx = xf?["bbox"]?["x"] ?? 0.0f
        let by = xf?["bbox"]?["y"] ?? 0.0f
        let bz = xf?["bbox"]?["z"] ?? 0.0f
        let bw = xf?["bbox"]?["w"] ?? 0.0f
        t |> success(near(bx, lx * 2.0f + 100.0f, 0.5f), "bbox.x == local.x*2 + 100")
        t |> success(near(by, ly * 3.0f + 50.0f, 0.5f), "bbox.y == local.y*3 + 50")
        t |> success(near(bz, lz * 2.0f + 100.0f, 0.5f), "bbox.z == local.z*2 + 100")
        t |> success(near(bw, lw * 3.0f + 50.0f, 0.5f), "bbox.w == local.w*3 + 50")

        // ---- a CONTAINER header under the transform is also screen-space + canvas_bbox ----
        // tab_item captures its header via current_item_bbox() through the same capture_item_bbox
        // chokepoint as leaf widgets, so it gets the screen-mapped bbox AND a canvas_bbox.
        t |> success(widget_exists(snap, "MAIN_WIN/XFORM_TABS/XFORM_TAB"),
                     "tab_item header present under transform")
        let tab = find_widget(snap, "MAIN_WIN/XFORM_TABS/XFORM_TAB")
        let tcx = tab?["canvas_bbox"]?["x"] ?? -1.0f
        t |> success(tcx >= 0.0f, "tab_item header carries canvas_bbox under transform")
        let tbx = tab?["bbox"]?["x"] ?? 0.0f
        t |> success(near(tbx, tcx * 2.0f + 100.0f, 0.5f),
                     "tab_item header bbox.x == canvas_bbox.x*2 + 100 (screen-mapped)")

        // ---- after pop: screen-native again, no canvas_bbox ----
        t |> success(widget_exists(snap, "MAIN_WIN/AFTER_BTN"), "after-pop button present")
        let after_cbb = find_widget(snap, "MAIN_WIN/AFTER_BTN")?["canvas_bbox"]?["z"] ?? -1.0f
        t |> success(after_cbb < 0.0f, "after-pop button carries no canvas_bbox (pop restored identity)")
    }
}

[test(description = "a widget whose render path never runs still appears in the snapshot through its init registration, with rendered false and a readable payload, beside a rendered one with a bbox", timeout = 1200)]
def test_unrendered_widget_visible(t : T?) {
    with_imgui_app("modules/dasImgui/examples/features/snapshot_unrendered.das") $(d) {
        var snap = wait_for_visible(d, "MAIN_WIN/VISIBLE_VAL", 15.0f)
        t |> success(snap != null, "VISIBLE_VAL appears in registry")
        if (snap == null) return

        // Gate 1: VISIBLE_VAL is rendered (has bbox).
        t |> success(widget_rendered(snap, "MAIN_WIN/VISIBLE_VAL"),
                     "VISIBLE_VAL has bbox - rendered this frame")
        var visible = find_widget(snap, "MAIN_WIN/VISIBLE_VAL")
        t |> equal(visible?["kind"] ?? "", "slider_float",
                   "VISIBLE_VAL.kind")
        t |> success(visible?["payload"]?["value"] != null,
                     "VISIBLE_VAL.payload.value present")

        // Gate 2: NEVER_RENDERED appears at all — proves [init] registration
        // fires for widgets whose render path is unreachable at runtime.
        t |> success(widget_exists(snap, "NEVER_RENDERED"),
                     "NEVER_RENDERED appears in g_widgets via [init], not via render")

        // Gate 3: NEVER_RENDERED carries rendered:false and a state payload.
        var hidden = find_widget(snap, "NEVER_RENDERED")
        t |> success(!widget_rendered(snap, "NEVER_RENDERED"),
                     "NEVER_RENDERED has rendered:false - not rendered this frame")
        t |> equal(hidden?["rendered"] ?? true, false,
                   "NEVER_RENDERED.rendered == false")
        t |> equal(hidden?["kind"] ?? "", "slider_float",
                   "NEVER_RENDERED.kind preserved through serializer")
        t |> success(hidden?["payload"]?["value"] != null,
                     "NEVER_RENDERED.payload.value still readable via captured serializer")
    }
}

[test(description = "with_id scopes and per-call id= and path= overrides shape each widget's snapshot path and diverge its ImGui hex_id, and an id= literal never becomes a path segment", timeout = 1200)]
def test_id_override(t : T?) { // nolint:STYLE038
    with_imgui_app("modules/dasImgui/examples/features/id_override.das") $(d) {
        t |> run("with_id_distinct_scopes") <| @(t : T?) {
            // Wait for the deepest leaf to land — proves the entire with_id chain
            // walked during the first frame.
            var snap0 = wait_for_widget(d, "MAIN_WIN/outer/inner/NESTED_BTN", 15.0f)
            t |> success(snap0 != null, "nested with_id leaf registered under 4-level path")
            if (snap0 == null) return

            // Each with_id scope shows up as its own path segment.
            t |> success(widget_exists(snap0, "MAIN_WIN/section_a/SAVE_A_BTN"),
                         "SAVE_A_BTN registers under section_a scope")
            t |> success(widget_exists(snap0, "MAIN_WIN/section_b/SAVE_B_BTN"),
                         "SAVE_B_BTN registers under section_b scope")

            // Same single-global widget rendered twice under distinct with_id
            // scopes — check_unique_render keys off the full path, so no panic.
            t |> success(widget_exists(snap0, "MAIN_WIN/xa/SHARED_BTN"),
                         "SHARED_BTN registers under xa scope")
            t |> success(widget_exists(snap0, "MAIN_WIN/xb/SHARED_BTN"),
                         "SHARED_BTN also registers under xb scope (same state global, distinct path)")

            // Distinct ImGui hashes — PushID(s) is part of with_id, so the two
            // SHARED_BTN render sites must have different hex_id values. hex_id
            // is a uint in WidgetEntry; coerce default to 0u accordingly.
            let hex_a = find_widget(snap0, "MAIN_WIN/xa/SHARED_BTN")?["hex_id"] ?? 0u
            let hex_b = find_widget(snap0, "MAIN_WIN/xb/SHARED_BTN")?["hex_id"] ?? 0u
            t |> success(hex_a != 0u && hex_b != 0u,
                         "both SHARED_BTN renders expose hex_id")
            t |> success(hex_a != hex_b,
                         "with_id makes ImGui hashes diverge between scopes")
        }
        reset(d)

        t |> run("with_id_nested") <| @(t : T?) {
            var snap0 = wait_for_widget(d, "MAIN_WIN/outer/inner/NESTED_BTN", 15.0f)
            t |> success(snap0 != null, "nested with_id chain produces composed path")
            if (snap0 == null) return

            // The nested path leaf exists; sibling lookup at intermediate levels
            // would not (with_id doesn't register a container entry of its own,
            // it just contributes a path segment).
            t |> equal(find_widget(snap0, "MAIN_WIN/outer/inner/NESTED_BTN")?["kind"] ?? "",
                       "button", "nested leaf registers as button kind")
        }
        reset(d)

        t |> run("per_call_id_override") <| @(t : T?) {
            // id="hashed_v1" — telemetry path is the BARE identifier (no path
            // override), but the button's hex_id is mangled with "hashed_v1".
            var snap0 = wait_for_widget(d, "MAIN_WIN/HASHED_BTN", 15.0f)
            t |> success(snap0 != null,
                         "id= override: telemetry path stays the bare identifier")
            if (snap0 == null) return

            // Baseline hex_id: a widget rendered without id= override. SAVE_A_BTN sits inside
            // with_id("section_a") so its hex_id is also mangled — not a baseline. Use NESTED_BTN
            // just to confirm hex_ids are present and uint-valued.
            let hashed = find_widget(snap0, "MAIN_WIN/HASHED_BTN")?["hex_id"] ?? 0u
            t |> success(hashed != 0u,
                         "id= override: hex_id exposed and non-zero")
        }
        reset(d)

        t |> run("per_call_path_override") <| @(t : T?) {
            // path="rps_v1" — telemetry path adopts the override; bare identifier
            // RPS_PATH is NOT in the registry.
            var snap0 = wait_for_widget(d, "MAIN_WIN/rps_v1", 15.0f)
            t |> success(snap0 != null,
                         "path= override: telemetry path becomes the override literal")
            if (snap0 == null) return
            t |> equal(find_widget(snap0, "MAIN_WIN/rps_v1")?["kind"] ?? "",
                       "slider_float", "path= override widget registers as slider_float kind")
            t |> success(!widget_exists(snap0, "MAIN_WIN/RPS_PATH"),
                         "path= override: bare identifier path is NOT registered")
        }
        reset(d)

        t |> run("per_call_id_and_path_combined") <| @(t : T?) {
            // id="rps_a", path="rps_b" — path leaf is "rps_b"; hex_id mangled
            // by both PushID("rps_a") (from id=) AND widget_prelude's PushID
            // ("rps_b" from widget_ident).
            var snap0 = wait_for_widget(d, "MAIN_WIN/rps_b", 15.0f)
            t |> success(snap0 != null,
                         "id=+path= combined: path takes the path= value")
            if (snap0 == null) return
            t |> success(!widget_exists(snap0, "MAIN_WIN/RPS_BOTH"),
                         "id=+path= combined: bare identifier path is NOT registered")
            t |> success(!widget_exists(snap0, "MAIN_WIN/rps_a"),
                         "id=+path= combined: id= literal does NOT become a path segment")

            // hex_id for rps_b must differ from rps_v1's (which had no id=
            // wrapping). Same widget_ident contribution but different outer
            // PushID, so hashes diverge.
            let combined = find_widget(snap0, "MAIN_WIN/rps_b")?["hex_id"] ?? 0u
            let path_only = find_widget(snap0, "MAIN_WIN/rps_v1")?["hex_id"] ?? 0u
            t |> success(combined != 0u && path_only != 0u,
                         "both hex_ids present")
            t |> success(combined != path_only,
                         "id= layered on path= yields a distinct hex_id from path= alone")
        }
    }
}

let ABSENT = "MAIN_WIN/NO_SUCH_WIDGET"

//! Times out with auto-dump OFF, so a green run stays quiet; the live
//! dump itself shows up the next time a wait genuinely fails.
[test(description = "auto-dump on wait timeout defaults to on and toggles, and a wait for an absent widget returns null on timeout", timeout = 1200)]
def test_auto_dump_on_timeout(t : T?) {
    with_imgui_app(TRIGGERS_FEATURE) $(d) {
        // boot: a real widget renders, so a dump (if one fired) would have a registry to show
        t |> success(wait_for_widget(d, "MAIN_WIN/SAVE_BTN", 15.0f) != null, "demo booted (MAIN_WIN/SAVE_BTN)")

        // auto-dump is on by default
        t |> success(auto_dump_on_timeout_enabled(), "auto-dump defaults to on")

        // turn it OFF, then deliberately time out on an absent widget — verifies the
        // null-on-timeout contract while keeping the suite quiet (no dump on a green run)
        set_auto_dump_on_timeout(false)
        t |> success(!auto_dump_on_timeout_enabled(), "set_auto_dump_on_timeout(false) flips the flag")
        t |> success(wait_for_widget(d, ABSENT, 1.0f) == null, "wait_for_widget on an absent widget times out (silently, while off)")
        set_auto_dump_on_timeout(true)
        t |> success(auto_dump_on_timeout_enabled(), "restored to on")
    }
}

[test(description = "a button firing log_to_clipboard, log_text and log_finish back to back counts its click and leaves the surrounding widgets registered", timeout = 1200)]
def test_log_to_text_smoke(t : T?) {
    with_imgui_app("modules/dasImgui/examples/features/log_to_text.das") $(d) {
        var snap = wait_for_widget(d, "MAIN_WIN/COPY_BTN", 15.0f)
        t |> success(snap != null, "COPY_BTN registers")
        if (snap == null) return

        let btn = find_widget(snap, "MAIN_WIN/COPY_BTN")
        t |> equal(btn?["kind"] ?? "", "button",
                   "COPY_BTN.kind == 'button'")

        for (ident in ["LEAD", "SEP", "STATIC_1", "STATIC_2"]) {
            t |> success(widget_exists(snap, "MAIN_WIN/{ident}"),
                         "{ident} registers")
        }

        // Click COPY_BTN — fires the log_to_clipboard / 3× log_text /
        // log_finish sequence. Clipboard contents not snapshot-visible.
        // Value here = no panic on the imperative chain.
        click(d, "MAIN_WIN/COPY_BTN")
        t |> success(wait_for_int_value(d, "MAIN_WIN/COPY_BTN", "click_count", 1, 300),
                     "COPY_BTN.click_count == 1 after click (log chain non-fatal)")

        // STATIC_1/2 still register the frame after the log session closes.
        var snap_after = snapshot(d)
        t |> success(widget_exists(snap_after, "MAIN_WIN/STATIC_1"),
                     "STATIC_1 still registers after log session")
        t |> success(widget_exists(snap_after, "MAIN_WIN/STATIC_2"),
                     "STATIC_2 still registers after log session")
    }
}

[test(description = "text_show keeps a source-initialized value pinned, picks up a daslang-side write to a display-only value, and round-trips imgui_force_set", timeout = 1200)]
def test_output_text_smoke(t : T?) {
    with_imgui_app("modules/dasImgui/examples/features/output_text.das") $(d) {
        var snap = wait_for_render(d, "MAIN_WIN/BANNER", 15.0f)
        t |> success(snap != null, "MAIN_WIN/BANNER renders")
        if (snap == null) return

        // Gate 1: source-init form keeps banner pinned to "ready".
        let banner = widget_payload_field(snap, "MAIN_WIN/BANNER", "value") ?? ""
        t |> equal(banner, "ready", "BANNER value pinned to source-init literal")

        // Gate 2: display-only form starts empty, then bump button writes it.
        let status0 = widget_payload_field(snap, "MAIN_WIN/STATUS_LINE", "value") ?? "<missing>"
        t |> equal(status0, "", "STATUS_LINE starts empty")

        click(d, "MAIN_WIN/BUMP_BTN")
        let snap_after_bump = wait_until(d, 300) $(var snap1) {
            let v = widget_payload_field(snap1, "MAIN_WIN/STATUS_LINE", "value") ?? ""
            return !empty(v)
        }
        t |> success(snap_after_bump != null,
                     "STATUS_LINE picks up daslang-side write after bump")

        // Gate 3: imgui_force_set on a text_show round-trips.
        force_set_value(d, "MAIN_WIN/STATUS_LINE", JV("remote"))
        t |> success(wait_for_string_value(d, "MAIN_WIN/STATUS_LINE", "value", "remote", 300),
                     "imgui_force_set writes 'remote' to STATUS_LINE.value")
    }
}
