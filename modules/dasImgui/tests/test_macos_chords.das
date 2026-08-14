options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui public
require imgui/imgui_commands public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! The command registry under the macOS input model, on every platform.
//!
//! The feature app forces io.ConfigMacOSXBehaviors = true, so ImGui swaps
//! Ctrl<->Super inside AddKeyEvent exactly like a real Mac. The contract
//! proven here: bindings and synth chords both speak LOGICAL Ctrl-speak —
//! ImGui's input swap alone maps a physical Cmd onto them. Regression
//! coverage for the nightly macOS double swap (registry declared Super
//! chords while ImGui had already turned Cmd into logical Ctrl, so no
//! primary chord could ever fire on macOS).

let FEATURE = "modules/dasImgui/examples/features/macos_chords.das"
let GUARD_SEC = 15.0f

def private probe(app : ImguiApp) : JsonValue? {
    return post_command(app, "macos_chords_state", null)
}

[test]
def test_macos_chords(t : T?) {
    with_imgui_app(FEATURE) $(d) {
        t |> run("bindings declare logical Ctrl chords") <| @(t : T?) {
            var snap0 = wait_for_render(d, "MAIN_WIN", GUARD_SEC)
            t |> success(snap0 != null, "MAIN_WIN renders")
            if (snap0 == null) return
            var state = probe(d)
            t |> success(state?["macos_behaviors"] ?? false,
                         "feature app runs with ConfigMacOSXBehaviors forced on")
            t |> equal(state?["primary_chord"] ?? 0,
                       int(ImGuiKey.K) | COMMAND_MOD_CTRL,
                       "primary binding matches on the logical Ctrl bit - ImGui's input swap owns the Cmd mapping")
            t |> equal(state?["secondary_chord"] ?? 0,
                       int(ImGuiKey.J) | COMMAND_MOD_SUPER,
                       "secondary binding matches on the logical Super bit")
            t |> equal(state?["primary_text"] ?? "", "Cmd+K",
                       "display text still speaks the platform key name")
        }

        t |> run("synth Ctrl surfaces as logical Ctrl") <| @(t : T?) {
            let ctrl_key = int(ImGuiKey.LeftCtrl)
            post_command(d, "imgui_key_press", JV((key = ctrl_key)))
            let saw_ctrl = wait_until(d, 240) $(var snap) {
                return ((snap?["io"]?["key_ctrl"] ?? false)
                    && !(snap?["io"]?["key_super"] ?? true))
            }
            t |> success(saw_ctrl != null,
                         "synth Ctrl held reads back as io.KeyCtrl under macOS behaviors")
            post_command(d, "imgui_key_release", JV((key = ctrl_key)))
            let cleared = wait_until(d, 240) $(var snap) {
                return !(snap?["io"]?["key_ctrl"] ?? true)
            }
            t |> success(cleared != null, "io modifier clears after release")
        }

        t |> run("primary chord fires end-to-end") <| @(t : T?) {
            let before = probe(d)?["primary_fired"] ?? 0
            post_command(d, "imgui_key_chord", JV((mods = ["Ctrl"], key = "K")))
            t |> success(wait_for_key_idle(d, 4.0f), "Ctrl+K chord completes")
            let fired = wait_cond(d, GUARD_SEC) $() : bool {
                return (probe(d)?["primary_fired"] ?? 0) > before
            }
            t |> success(fired, "logical Ctrl+K fires the primary-bound command")
        }

        t |> run("secondary chord fires end-to-end") <| @(t : T?) {
            let before = probe(d)?["secondary_fired"] ?? 0
            post_command(d, "imgui_key_chord", JV((mods = ["Super"], key = "J")))
            t |> success(wait_for_key_idle(d, 4.0f), "Super+J chord completes")
            let fired = wait_cond(d, GUARD_SEC) $() : bool {
                return (probe(d)?["secondary_fired"] ?? 0) > before
            }
            t |> success(fired, "logical Super+J fires the secondary-bound command")
        }

        t |> run("capture stores the logical primary modifier") <| @(t : T?) {
            let armed = post_command(d, "macos_chords_arm_capture", null)
            t |> success(armed?["ok"] ?? false, "capture arms on probe.primary")
            post_command(d, "imgui_key_chord", JV((mods = ["Ctrl"], key = "M")))
            t |> success(wait_for_key_idle(d, 4.0f), "Ctrl+M chord completes")
            let captured = wait_cond(d, GUARD_SEC) $() : bool {
                return !(probe(d)?["capture_armed"] ?? true)
            }
            t |> success(captured, "capture consumes the chord")
            var state = probe(d)
            t |> equal(state?["primary_key"] ?? 0, int(ImGuiKey.M),
                       "captured binding lands on the tapped key")
            t |> success(state?["primary_is_primary"] ?? false,
                         "held logical Ctrl captures as the PRIMARY modifier")
            t |> success(!(state?["primary_is_secondary"] ?? true),
                         "held logical Ctrl does not misfile as secondary")
        }
    }
}
