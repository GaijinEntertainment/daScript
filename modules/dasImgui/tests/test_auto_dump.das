options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Regression for the wait-family auto-dump (``set_auto_dump_on_timeout``, default on): any
//! ``wait_*`` helper that times out logs the live UI state once via ``log_snapshot`` before
//! returning null/false, so a red headless / slow-CI run is self-explaining with zero per-test
//! dump code.
//!
//! This test asserts the toggle + the null-on-timeout contract WITHOUT triggering a real dump:
//! it times out with auto-dump turned OFF, so the suite stays quiet on green runs (a dump on
//! every pass would defeat the "dump only on failure" point). The live dump is verified
//! by hand and shows up for real the next time a wait genuinely fails in anger.
//! Runs headless on ubuntu + macOS like the rest of the integration suite.

let TRIGGERS_FEATURE = "modules/dasImgui/examples/features/triggers.das"
let ABSENT = "MAIN_WIN/NO_SUCH_WIDGET"

[test]
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
