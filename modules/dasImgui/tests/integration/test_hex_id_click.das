options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Integration smoke for Phase 2.3 — hex_id-targeted dispatch.
//!
//! `imgui_click` accepts either a path string ("DEMO_BTN") or a hex_id
//! string ("0x{hex}"). Hex_ids resolve via g_id_to_path; when found, the
//! click goes through the L2 dispatcher just like a name-targeted call.
//!
//! Gates:
//!   1. DEMO_BTN renders and exposes a hex_id in the snapshot.
//!   2. imgui_click via the hex_id form returns Result.ok with the
//!      RESOLVED path ("DEMO_BTN") in `value` (proves classify_target
//!      reverse-mapped before dispatch).
//!   3. After the click, DEMO_BTN.payload.click_count incremented (proves
//!      the L2 dispatcher actually fired, not just the resolution).
//!   4. Garbage hex (well-formed prefix, value not in g_id_to_path AND
//!      not in g_id_to_bbox) returns Result.err.

let HEX_ID_CLICK_FEATURE = "modules/dasImgui/examples/features/hex_id_click.das"

[test]
def test_hex_id_click_dispatches_via_path(t : T?) {
    with_imgui_app(HEX_ID_CLICK_FEATURE) $(d) {
        // Gate 1: DEMO_BTN reaches the snapshot with a non-zero hex_id.
        var snap0 = wait_for_widget(d, "DEMO_BTN", 15.0f)
        t |> success(snap0 != null, "DEMO_BTN registers (snapshot reachable)")
        if (snap0 == null) return
        let hex_id = snap0?["globals"]?["DEMO_BTN"]?["hex_id"] ?? 0u
        t |> success(hex_id != 0u, "DEMO_BTN.hex_id is non-zero")
        let hex_str = "0x{hex_id:x}"

        // Gate 2: hex_id click returns ok-Result with the RESOLVED path.
        let resp = click(d, hex_str)
        t |> success(resp != null, "hex_id click returned a response")
        t |> equal(resp?["ok"] ?? false, true,
                   "hex_id click response.ok = true")
        t |> equal(resp?["value"] ?? "", "DEMO_BTN",
                   "hex_id click response.value = resolved path (DEMO_BTN)")

        // Gate 3: click_count actually incremented — proves the L2
        // dispatcher fired, not just the resolution.
        let counted = wait_for_int_value(d, "DEMO_BTN", "click_count", 1, 300)
        t |> success(counted, "DEMO_BTN.click_count reached 1 after hex_id click")

        // Gate 4: garbage hex (well-formed prefix, but no widget at that id)
        // → unresolvable, both g_id_to_path and g_id_to_bbox miss.
        let bad_resp = click(d, "0xdeadbeef")
        t |> equal(bad_resp?["ok"] ?? true, false,
                   "garbage hex_id → response.ok = false")
        t |> success(!empty(bad_resp?["error"] ?? ""),
                     "garbage hex_id → response.error populated")
    }
}
