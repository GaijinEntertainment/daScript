options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public

//! Smallest possible exercise of the harness — spawn daslang-live against a
//! known-good feature file, wait for /status, send /shutdown, drain. No
//! widget interaction. Pinned the harness lifecycle in isolation; once this
//! is green, test_triggers can lean on the same primitives without doubt.

let TRIGGERS_FEATURE = "modules/dasImgui/examples/features/triggers.das"

[test]
def test_spawn_and_shutdown(t : T?) {
    with_imgui_app_opt(TRIGGERS_FEATURE, 9090, 30.0f, 30.0f) $(d) {
        // Body intentionally empty. with_imgui_app_opt's prelude already gated
        // on /status; the postlude POSTs /shutdown and drains. If we get
        // here, daslang-live is alive and answering HV.
    }
    t |> success(true, "harness spawned daslang-live, posted /shutdown, exited cleanly")
}
